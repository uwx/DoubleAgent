/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaServer.h"
#include "IntegrityBroker.h"
#include "UserSecurity.h"
#include "ThreadSecurity.h"
#include "MallocPtr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////

CIntegrityBroker::CIntegrityBroker()
{
}

CIntegrityBroker::~CIntegrityBroker()
{
}

/////////////////////////////////////////////////////////////////////////////

bool CIntegrityBroker::IsProcessElevated (HANDLE pProcessHandle)
{
	bool	lRet = false;
	
	if	(pProcessHandle)
	{
		CGenericHandle			lProcessToken;
		tS <TOKEN_ELEVATION>	lElevation;
		DWORD					lDataSize = sizeof (lElevation);

		if	(
				(OpenProcessToken (pProcessHandle, TOKEN_QUERY, &lProcessToken))
			&&	(GetTokenInformation (lProcessToken, TokenElevation, &lElevation, lDataSize, &lDataSize))
			)
		{
			if	(lElevation.TokenIsElevated)
			{
				lRet = true;
			}
		}
		else
		{
			LogWinErr (LogNormal, GetLastError());
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HANDLE CIntegrityBroker::MakeRestrictedToken (HANDLE pProcessHandle)
{
	HANDLE			lRet = NULL;
	CGenericHandle	lProcessToken;
	CGenericHandle	lRestrictedToken;

	if	(OpenProcessToken (pProcessHandle, MAXIMUM_ALLOWED, &lProcessToken))
	{
		tSidPtr								lSid;
		tS <SID_AND_ATTRIBUTES>				lDisabledSids;
		tS <TOKEN_LINKED_TOKEN>				lLinkedToken;
		DWORD								lDataSize = sizeof (lLinkedToken);
		DWORD								lDeletePrivilegeCount = 0;
		tMallocPtr <LUID_AND_ATTRIBUTES>	lDeletePrivileges;

		lDisabledSids.Sid = CUserSecurity::SidAdministrators (lSid);

		if	(
				(GetTokenInformation (lProcessToken, TokenLinkedToken, &lLinkedToken, lDataSize, &lDataSize))
			&&	(lLinkedToken.LinkedToken)
			)
		{
			try
			{
				DWORD							lTokenDataSize;
				DWORD							lLinkedDataSize;
				tMallocPtr <TOKEN_PRIVILEGES>	lTokenPrivileges;
				tMallocPtr <TOKEN_PRIVILEGES>	lLinkedPrivileges;
				DWORD							lPrivilegeNdx;
				DWORD							lLinkedNdx;

				GetTokenInformation (lProcessToken, TokenPrivileges, NULL, 0, &lTokenDataSize);
				GetTokenInformation (lLinkedToken.LinkedToken, TokenPrivileges, NULL, 0, &lLinkedDataSize);

				if	(
						(lTokenDataSize > 0)
					&&	(lTokenPrivileges.AllocBulk (lTokenDataSize))
					&&	(GetTokenInformation (lProcessToken, TokenPrivileges, lTokenPrivileges, lTokenDataSize, &lTokenDataSize))
					&&	(lLinkedDataSize > 0)
					&&	(lLinkedPrivileges.AllocBulk (lLinkedDataSize))
					&&	(GetTokenInformation (lLinkedToken.LinkedToken, TokenPrivileges, lLinkedPrivileges, lLinkedDataSize, &lLinkedDataSize))
					&&	(lDeletePrivileges.AllocBulk (sizeof(LUID_AND_ATTRIBUTES) * lTokenPrivileges->PrivilegeCount))
					)
				{
					for	(lPrivilegeNdx = 0; lPrivilegeNdx < lTokenPrivileges->PrivilegeCount; lPrivilegeNdx++)
					{
						for	(lLinkedNdx = 0; lLinkedNdx < lLinkedPrivileges->PrivilegeCount; lLinkedNdx++)
						{
							if	(memcmp (&lTokenPrivileges->Privileges [lPrivilegeNdx].Luid, &lLinkedPrivileges->Privileges [lLinkedNdx].Luid, sizeof(LUID)) == 0)
							{
								break;
							}
						}
						if	(lLinkedNdx >= lLinkedPrivileges->PrivilegeCount)
						{
#ifdef	_DEBUG_NOT						
							{
								CString	lPrivilegeName;
								DWORD	lPrivilegeNameSize;
								LookupPrivilegeName (NULL, &lTokenPrivileges->Privileges[lPrivilegeNdx].Luid, lPrivilegeName.GetBuffer (MAX_PATH), &(lPrivilegeNameSize = MAX_PATH));
								lPrivilegeName.ReleaseBuffer ();
								LogMessage (LogNormal, _T("  Delete privilege [%s]"), lPrivilegeName);
							}
#endif							
							lDeletePrivileges [(long)lDeletePrivilegeCount].Luid = lTokenPrivileges->Privileges [lPrivilegeNdx].Luid;
							lDeletePrivileges [(long)lDeletePrivilegeCount].Attributes = 0;
							lDeletePrivilegeCount++;
						}
					}
#ifdef	_DEBUG_NOT					
					LogMessage (LogNormal, _T("Process [%u] privileges, Linked [%u] privileges, Delete [%u] privileges"), lTokenPrivileges->PrivilegeCount, lLinkedPrivileges->PrivilegeCount, lDeletePrivilegeCount);
#endif					
				}
			}
			catch AnyExceptionSilent

			CloseHandle (lLinkedToken.LinkedToken);
		}

		lDisabledSids.Sid = CUserSecurity::SidAdministrators (lSid);
		
		if	(CreateRestrictedToken (lProcessToken, 0, 1, &lDisabledSids, lDeletePrivilegeCount, lDeletePrivileges, 0, NULL, &lRestrictedToken))
		{
			static SID_IDENTIFIER_AUTHORITY	lAuthority = SECURITY_MANDATORY_LABEL_AUTHORITY;
			tS <TOKEN_MANDATORY_LABEL>		lMandatoryLabel;
			tS <PROCESS_INFORMATION>		lProcessInfo;
			tSS <STARTUPINFO, DWORD>		lStartupInfo;
			ULONG							lExitCode = 0;
			
			if	(AllocateAndInitializeSid (&lAuthority, 1, SECURITY_MANDATORY_MEDIUM_RID, 0, 0, 0, 0, 0, 0, 0, (void **) lSid.Free ()))
			{
				lMandatoryLabel.Label.Attributes = SE_GROUP_INTEGRITY;
				lMandatoryLabel.Label.Sid = lSid;
				
				if	(SetTokenInformation (lRestrictedToken, TokenIntegrityLevel, &lMandatoryLabel, sizeof(lMandatoryLabel)+GetLengthSid(lMandatoryLabel.Label.Sid)))
				{
#ifdef	_DEBUG				
					LogTokenSecurity (lProcessToken, LogDetails, _T("Process"));
					LogTokenSecurity (lRestrictedToken, LogDetails, _T("Restricted"));
#endif					
					lRet = lRestrictedToken.Detach ();
				}
				else
				{
					LogWinErr (LogNormal, GetLastError(), _T("SetTokenInformation"));
				}
			}
		}
		else
		{
			LogWinErr (LogNormal, GetLastError(), _T("DuplicateTokenEx"));
		}
	}
	else
	{
		LogWinErr (LogNormal, GetLastError(), _T("OpenProcessToken"));
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HANDLE CIntegrityBroker::StartRestrictedProcess (HANDLE pRestrictedToken, LPCTSTR pCommandLine)
{
	HANDLE						lRet = NULL;
	tS <PROCESS_INFORMATION>	lProcessInfo;
	tSS <STARTUPINFO, DWORD>	lStartupInfo;
	ULONG						lExitCode = 0;
			
	if	(LogIsActive ())
	{
		LogTokenIntegrity (pRestrictedToken, LogNormal, CString(_T("Launch "))+CString(pCommandLine));
	}

	lStartupInfo.lpDesktop = _T("winsta0\\default");
	lStartupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK;

	if	(CreateProcessAsUser (pRestrictedToken, NULL, (LPTSTR)pCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &lStartupInfo, &lProcessInfo))
	{
		if	(LogIsActive ())
		{
			LogMessage (LogNormal, _T("Launched [%u (0x%4.4X)"), lProcessInfo.dwProcessId, lProcessInfo.dwProcessId);
		}
		
//		LogMessage (LogNormal|LogTimeMs, _T("WaitForInputIdle..."));
//		WaitForInputIdle (lProcessInfo.hProcess, 1000);
//		LogMessage (LogNormal|LogTimeMs, _T("WaitForInputIdle Done"));
		
		CloseHandle (lProcessInfo.hThread);
		lRet = lProcessInfo.hProcess;
	}
	else
	{
		LogWinErr (LogNormal, GetLastError(), _T("CreateProcessAsUser [%s]"), GetCommandLine());
	}
	return lRet;
}
