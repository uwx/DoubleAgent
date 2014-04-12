/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#include <sddl.h>
#include "ThreadSecurity.h"
#include "StringArrayEx.h"
#include "MallocPtr.h"
#include "OsVer.h"
#include "Log.h"

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

CThreadSecurity::CThreadSecurity (HANDLE pProcessHandle)
:	mProcessHandle (pProcessHandle),
	mThreadHandle (NULL)
{
#ifdef	_UNICODE
	if	(!mProcessHandle)
	{
		mProcessHandle = GetCurrentProcess ();
		mThreadHandle = GetCurrentThread ();
	}
#endif
}

CThreadSecurity::~CThreadSecurity ()
{
#ifdef	_UNICODE
	RestorePrivileges ();
	mTokenHandle.Close ();
#endif
}

//////////////////////////////////////////////////////////////////////

bool CThreadSecurity::GetBackupPrivileges (bool pAllThreads)
{
	return GetPrivileges (pAllThreads, SE_BACKUP_NAME, SE_RESTORE_NAME, NULL);
}

bool CThreadSecurity::GetSecurityPrivileges (bool pAllThreads)
{
	return GetPrivileges (pAllThreads, SE_SECURITY_NAME, NULL);
}

bool CThreadSecurity::GetDebugPrivileges (bool pAllThreads)
{
	return GetPrivileges (pAllThreads, SE_DEBUG_NAME, NULL);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CThreadSecurity::GetPrivileges (bool pAllThreads, LPCTSTR pSecurityName, ...)
{
#ifdef	_UNICODE
	bool	lRet = false;

	try
	{
		if	(
				(!mTokenHandle)
			&&	(
					(
						(!pAllThreads)
					&&	(OpenThreadToken (mThreadHandle, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, FALSE, &mTokenHandle))
					)
				||	(OpenProcessToken (mProcessHandle, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &mTokenHandle))
				)
			)
		{
			try
			{
				va_list					lArgPtr;
				LPCTSTR					lSecurityName;
				DWORD					lOldBufferSize = sizeof (TOKEN_PRIVILEGES);
				DWORD					lNewBufferSize = sizeof (TOKEN_PRIVILEGES);
				PTOKEN_PRIVILEGES		lOldPrivileges;
				PTOKEN_PRIVILEGES		lNewPrivileges;

				va_start (lArgPtr, pSecurityName);
				for	(lSecurityName = pSecurityName; lSecurityName; lSecurityName = va_arg (lArgPtr, LPCTSTR))
				{
					lNewBufferSize += sizeof (LUID_AND_ATTRIBUTES);
				}
				va_end (lArgPtr);

#ifdef	__AFXCOLL_H__
				mNewPrivileges.SetSize (lNewBufferSize);
#else
				mNewPrivileges.SetCount (lNewBufferSize);
#endif
				lNewPrivileges = (PTOKEN_PRIVILEGES) mNewPrivileges.GetData();
				lNewPrivileges->PrivilegeCount = 0;

				va_start (lArgPtr, pSecurityName);
				for	(lSecurityName = pSecurityName; lSecurityName; lSecurityName = va_arg (lArgPtr, LPCTSTR))
				{
					if	(LookupPrivilegeValue (NULL, lSecurityName, &lNewPrivileges->Privileges [lNewPrivileges->PrivilegeCount].Luid))
					{
						lNewPrivileges->Privileges [lNewPrivileges->PrivilegeCount].Attributes = SE_PRIVILEGE_ENABLED;
						lNewPrivileges->PrivilegeCount++;
					}
					else
					{
						LogWinErr (LogDetails|LogTime, GetLastError (), _T("LookupPrivilegeValue [%s]"), lSecurityName);
					}
				}
				va_end (lArgPtr);

#ifdef	__AFXCOLL_H__
				mOldPrivileges.SetSize (lOldBufferSize);
#else
				mOldPrivileges.SetCount (lOldBufferSize);
#endif
				lOldPrivileges = (PTOKEN_PRIVILEGES) mOldPrivileges.GetData();
				lOldPrivileges->PrivilegeCount = 0;

				try
				{
					if	(AdjustTokenPrivileges (mTokenHandle, FALSE, lNewPrivileges, lOldBufferSize, lOldPrivileges, &lOldBufferSize))
					{
						if	(GetLastError () == NOERROR)
						{
							lRet = true;
						}
					}
					else
					if	(lOldBufferSize > (DWORD) mOldPrivileges.GetCount())
					{
#ifdef	__AFXCOLL_H__
						mOldPrivileges.SetSize (lOldBufferSize);
#else
						mOldPrivileges.SetCount (lOldBufferSize);
#endif
						lOldPrivileges = (PTOKEN_PRIVILEGES) mOldPrivileges.GetData();
						lOldPrivileges->PrivilegeCount = 0;

						if	(AdjustTokenPrivileges (mTokenHandle, FALSE, lNewPrivileges, lOldBufferSize, lOldPrivileges, &lOldBufferSize))
						{
							if	(GetLastError () == NOERROR)
							{
								lRet = true;
							}
						}
					}
#ifdef	_DEBUG_NOT
					if	(lRet)
					{
						if	(mOldPrivileges.GetData())
						{
							LogTokenPrivileges ((PTOKEN_PRIVILEGES) mOldPrivileges.GetData(), LogDebug, _T("Old"));
						}
						if	(mNewPrivileges.GetData())
						{
							LogTokenPrivileges ((PTOKEN_PRIVILEGES) mNewPrivileges.GetData(), LogDebug, _T("New"));
						}
						LogTokenPrivileges (mTokenHandle, LogDebug, _T("Adjusted"));
					}
#endif
				}
				catch AnyExceptionDebug
			}
			catch AnyExceptionDebug

			if	(!lRet)
			{
				if	(LogIsActive ())
				{
					LogWinErr (LogDetails|LogTime, GetLastError (), _T("AdjustTokenPrivileges"));
				}

				RestorePrivileges ();
				mOldPrivileges.RemoveAll ();
				mNewPrivileges.RemoveAll ();
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
#else
	return false;
#endif
}

void CThreadSecurity::RestorePrivileges ()
{
#ifdef	_UNICODE
	if	(
			((HANDLE) mTokenHandle)
		&&	(mOldPrivileges.GetCount() > 0)
		)
	{
		PTOKEN_PRIVILEGES	lOldPrivileges = (PTOKEN_PRIVILEGES) mOldPrivileges.GetData();

		if	(
				(AdjustTokenPrivileges (mTokenHandle, FALSE, lOldPrivileges, 0, NULL, NULL))
			&&	(LogIsActive ())
			)
		{
			//LogMessage (LogDetails|LogTime, _T("  Privileges restored"));
		}

		if	(LogIsActive ())
		{
			LogWinErr (LogDetails|LogTime, GetLastError (), _T("AdjustTokenPrivileges"));
		}
	}

	mTokenHandle.Close ();
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	_UNICODE
#ifndef	MSGFLT_ADD
#define	MSGFLT_ADD		1
#define MSGFLT_REMOVE	2
#endif
typedef BOOL (WINAPI * tChangeWindowMessageFilter) (UINT message,DWORD dwFlag);
#endif

bool CThreadSecurity::AllowUiPiMessage (WORD pMessageId, bool pAllow)
{
	bool	lRet = false;
#ifdef	_UNICODE
	if	(IsWindowsVista_AtLeast ())
	{
		try
		{
			HMODULE						lModuleHandle;
			tChangeWindowMessageFilter	lFilterProc;

			if	(
					(lModuleHandle = GetModuleHandle (_T("USER32")))
				&&	(lFilterProc = (tChangeWindowMessageFilter) GetProcAddress (lModuleHandle, "ChangeWindowMessageFilter"))
				&&	((*lFilterProc) (pMessageId, pAllow?MSGFLT_ADD:MSGFLT_REMOVE))
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
#endif
	return	lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void LogProcessSecurity (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenSecurity (lToken, pLogLevel, lTitle);
/**/			LogTokenType (lToken, pLogLevel, lTitle);
/**/			LogTokenIntegrity (lToken, pLogLevel, lTitle);
/**/			LogTokenElevation (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError(), _T("OpenProcessToken [%p]"), pProcess);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogProcessUser (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenUser (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogProcessOwner (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenOwner (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogProcessGroup (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenGroup (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogProcessGroups (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenGroups (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogProcessPrivileges (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenPrivileges (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogProcessIntegrity (HANDLE pProcess, UINT pLogLevel, LPCTSTR pFormat, ...)
{
#if	(_MSC_VER >= 1400)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			HANDLE	lToken;

			if	(OpenProcessToken (pProcess, TOKEN_QUERY, &lToken))
			{
				if	(pFormat)
				{
					va_list 	lArgPtr;

					va_start (lArgPtr, pFormat);
					_vsntprintf (lTitle.GetBuffer(4096), 4096, pFormat, lArgPtr);
					lTitle.ReleaseBuffer ();
				}
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Process");
				}
				LogTokenIntegrity (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void LogThreadSecurity (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenThreadToken (pThread, TOKEN_QUERY, FALSE, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Thread");
				}
				LogTokenType (lToken, pLogLevel, lTitle);
				LogTokenSecurity (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError(), _T("OpenThreadToken [%p]"), pThread);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogThreadUser (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenThreadToken (pThread, TOKEN_QUERY, FALSE, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Thread");
				}
				LogTokenUser (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogThreadOwner (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenThreadToken (pThread, TOKEN_QUERY, FALSE, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Thread");
				}
				LogTokenOwner (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogThreadGroup (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenThreadToken (pThread, TOKEN_QUERY, FALSE, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Thread");
				}
				LogTokenGroup (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogThreadGroups (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenThreadToken (pThread, TOKEN_QUERY, FALSE, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Thread");
				}
				LogTokenGroups (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogThreadPrivileges (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HANDLE	lToken;

			if	(OpenThreadToken (pThread, TOKEN_QUERY, FALSE, &lToken))
			{
				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Thread");
				}
				LogTokenPrivileges (lToken, pLogLevel, lTitle);

				CloseHandle (lToken);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void LogTokenSecurity (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		LogTokenUser (pToken, pLogLevel, pTitle);
		LogTokenOwner (pToken, pLogLevel, pTitle);
		LogTokenGroup (pToken, pLogLevel, pTitle);
		LogTokenGroups (pToken, pLogLevel, pTitle);
		LogTokenPrivileges (pToken, pLogLevel, pTitle);
		LogTokenIntegrity (pToken, pLogLevel, pTitle);
	}
#endif
}

void LogTokenUser (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString					lTitle (pTitle);
			tMallocPtr <TOKEN_USER>	lTokenUser;
			DWORD					lDataSize = 0;
			CString					lAccountName;
			DWORD					lAccountNameSize;
			CString					lDomainName;
			DWORD					lDomainNameSize;
			SID_NAME_USE			lSidUse = SidTypeInvalid;
			LPTSTR					lSidStr = NULL;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			GetTokenInformation (pToken, TokenUser, NULL, 0, &lDataSize);
			if	(
					(lDataSize > 0)
				&&	(lTokenUser.AllocBulk (lDataSize))
				&&	(GetTokenInformation (pToken, TokenUser, lTokenUser, lDataSize, &lDataSize))
				)
			{
				if	(LookupAccountSid (NULL, lTokenUser->User.Sid, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
				{
					lAccountName.ReleaseBuffer ();
					lDomainName.ReleaseBuffer ();
					LogMessage (pLogLevel, _T("%s User   [%s\\%s]"), lTitle, lDomainName, lAccountName);
				}
				else
				{
					ConvertSidToStringSid (lTokenUser->User.Sid, &lSidStr);
					LogMessage (pLogLevel, _T("%s User   [%s]"), lTitle, lSidStr);
					LocalFree (lSidStr);
				}
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenUser [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogTokenOwner (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString						lTitle (pTitle);
			tMallocPtr <TOKEN_OWNER>	lTokenOwner;
			DWORD						lDataSize = 0;
			CString						lAccountName;
			DWORD						lAccountNameSize;
			CString						lDomainName;
			DWORD						lDomainNameSize;
			SID_NAME_USE				lSidUse = SidTypeInvalid;
			LPTSTR						lSidStr = NULL;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			GetTokenInformation (pToken, TokenOwner, NULL, 0, &lDataSize);
			if	(
					(lDataSize > 0)
				&&	(lTokenOwner.AllocBulk (lDataSize))
				&&	(GetTokenInformation (pToken, TokenOwner, lTokenOwner, lDataSize, &lDataSize))
				)
			{
				if	(LookupAccountSid (NULL, lTokenOwner->Owner, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
				{
					lAccountName.ReleaseBuffer ();
					lDomainName.ReleaseBuffer ();
					LogMessage (pLogLevel, _T("%s Owner  [%s\\%s]"), lTitle, lDomainName, lAccountName);
				}
				else
				{
					ConvertSidToStringSid (lTokenOwner->Owner, &lSidStr);
					LogMessage (pLogLevel, _T("%s Owner  [%s]"), lTitle, lSidStr);
					LocalFree (lSidStr);
				}
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenOwner [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogTokenGroup (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString								lTitle (pTitle);
			tMallocPtr <TOKEN_PRIMARY_GROUP>	lTokenGroup;
			DWORD								lDataSize = 0;
			CString								lAccountName;
			DWORD								lAccountNameSize;
			CString								lDomainName;
			DWORD								lDomainNameSize;
			SID_NAME_USE						lSidUse = SidTypeInvalid;
			LPTSTR								lSidStr = NULL;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			GetTokenInformation (pToken, TokenPrimaryGroup, NULL, 0, &lDataSize);
			if	(
					(lDataSize > 0)
				&&	(lTokenGroup.AllocBulk (lDataSize))
				&&	(GetTokenInformation (pToken, TokenPrimaryGroup, lTokenGroup, lDataSize, &lDataSize))
				)
			{
				if	(LookupAccountSid (NULL, lTokenGroup->PrimaryGroup, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
				{
					lAccountName.ReleaseBuffer ();
					lDomainName.ReleaseBuffer ();
					LogMessage (pLogLevel, _T("%s Group  [%s\\%s]"), lTitle, lDomainName, lAccountName);
				}
				else
				{
					ConvertSidToStringSid (lTokenGroup->PrimaryGroup, &lSidStr);
					LogMessage (pLogLevel, _T("%s Group  [%s]"), lTitle, lSidStr);
					LocalFree (lSidStr);
				}
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenPrimaryGroup [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogTokenGroups (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString						lTitle (pTitle);
			tMallocPtr <TOKEN_GROUPS>	lTokenGroups;
			DWORD						lDataSize = 0;
			DWORD						lGroupNdx;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			GetTokenInformation (pToken, TokenGroups, NULL, 0, &lDataSize);
			if	(
					(lDataSize > 0)
				&&	(lTokenGroups.AllocBulk (lDataSize))
				&&	(GetTokenInformation (pToken, TokenGroups, lTokenGroups, lDataSize, &lDataSize))
				)
			{
				LogMessage (pLogLevel, _T("%s Groups [%u]"), lTitle, lTokenGroups->GroupCount);

				for	(lGroupNdx = 0; lGroupNdx < lTokenGroups->GroupCount; lGroupNdx++)
				{
					try
					{
						CString			lAccountName;
						DWORD			lAccountNameSize;
						CString			lDomainName;
						DWORD			lDomainNameSize;
						SID_NAME_USE	lSidUse = SidTypeInvalid;
						LPTSTR			lSidStr = NULL;
						CStringArray	lAttributeNames;
						CString			lAttributes;

						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_MANDATORY)
						{
							lAttributeNames.Add (_T("Mandatory"));
						}
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_OWNER)
						{
							lAttributeNames.Add (_T("Owner"));
						}
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_ENABLED)
						{
							lAttributeNames.Add (_T("Enabled"));
						}
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_ENABLED_BY_DEFAULT)
						{
							lAttributeNames.Add (_T("EnabledByDefault"));
						}
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_USE_FOR_DENY_ONLY)
						{
							lAttributeNames.Add (_T("DenyOnly"));
						}
#ifdef	SE_GROUP_INTEGRITY_ENABLED
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_INTEGRITY)
						{
							lAttributeNames.Add (_T("Integrity"));
						}
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_INTEGRITY_ENABLED)
						{
							lAttributeNames.Add (_T("IntegrityEnabled"));
						}
#endif
						if	((lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_LOGON_ID) == SE_GROUP_LOGON_ID)
						{
							lAttributeNames.Add (_T("LogonId"));
						}
						else
						if	(lTokenGroups->Groups[lGroupNdx].Attributes & SE_GROUP_RESOURCE)
						{
							lAttributeNames.Add (_T("Resource"));
						}
						if	(lAttributeNames.GetCount() > 0)
						{
							lAttributes.Format (_T(" [%s]"), JoinStringArray (lAttributeNames, _T(" ")));
						}

						if	(LookupAccountSid (NULL, lTokenGroups->Groups[lGroupNdx].Sid, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
						{
							lAccountName.ReleaseBuffer ();
							lDomainName.ReleaseBuffer ();
							LogMessage (pLogLevel, _T("  Group %c [%s\\%s]%s"), ((lTokenGroups->Groups[lGroupNdx].Attributes & SE_PRIVILEGE_ENABLED) ? _T('*') : _T(' ')), lDomainName, lAccountName, lAttributes);
						}
						else
						{
							ConvertSidToStringSid (lTokenGroups->Groups[lGroupNdx].Sid, &lSidStr);
							LogMessage (pLogLevel, _T("  Group %c [%s]"), ((lTokenGroups->Groups[lGroupNdx].Attributes & SE_PRIVILEGE_ENABLED) ? _T('*') : _T(' ')), lSidStr, lAttributes);
							LocalFree (lSidStr);
						}
					}
					catch AnyExceptionSilent
				}
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenGroups [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogTokenPrivileges (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			tMallocPtr <TOKEN_PRIVILEGES>	lTokenPrivileges;
			DWORD							lDataSize = 0;

			GetTokenInformation (pToken, TokenPrivileges, NULL, 0, &lDataSize);
			if	(
					(lDataSize > 0)
				&&	(lTokenPrivileges.AllocBulk (lDataSize))
				&&	(GetTokenInformation (pToken, TokenPrivileges, lTokenPrivileges, lDataSize, &lDataSize))
				)
			{
				LogTokenPrivileges (lTokenPrivileges, pLogLevel, pTitle);
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenPrivileges [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void LogTokenPrivileges (PTOKEN_PRIVILEGES pTokenPrivileges, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			DWORD	lPrivilegeNdx;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			LogMessage (pLogLevel, _T("%s Privileges [%u]"), lTitle, pTokenPrivileges->PrivilegeCount);

			for	(lPrivilegeNdx = 0; lPrivilegeNdx < pTokenPrivileges->PrivilegeCount; lPrivilegeNdx++)
			{
				try
				{
					CString	lPrivilegeName;
					DWORD	lPrivilegeNameSize;
					CString	lDisplayName;
					DWORD	lDisplayNameSize;
					DWORD	lDisplayNameLanguage;
					CString	lAttributes;

					LookupPrivilegeName (NULL, &pTokenPrivileges->Privileges[lPrivilegeNdx].Luid, lPrivilegeName.GetBuffer (MAX_PATH), &(lPrivilegeNameSize = MAX_PATH));
					lPrivilegeName.ReleaseBuffer ();
					LookupPrivilegeDisplayName (NULL, lPrivilegeName, lDisplayName.GetBuffer (MAX_PATH), &(lDisplayNameSize = MAX_PATH), &(lDisplayNameLanguage = 0));
					lDisplayName.ReleaseBuffer ();

					if	(pTokenPrivileges->Privileges[lPrivilegeNdx].Attributes & SE_PRIVILEGE_ENABLED_BY_DEFAULT)
					{
						lAttributes = _T(" [EnabledByDefault]");
					}
					LogMessage (pLogLevel, _T("  Privilege %c [%s] [%s]%s"), ((pTokenPrivileges->Privileges[lPrivilegeNdx].Attributes & SE_PRIVILEGE_ENABLED) ? _T('*') : _T(' ')), lPrivilegeName, lDisplayName, lAttributes);
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void LogTokenType (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_UNICODE
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString					lTitle (pTitle);
			tS <TOKEN_STATISTICS>	lStatistics;
			DWORD					lDataSize = sizeof (lStatistics);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}
			if	(GetTokenInformation (pToken, TokenStatistics, &lStatistics, lDataSize, &lDataSize))
			{
				CString	lTokenType;
				CString	lImpersonationLevel;

				switch (lStatistics.TokenType)
				{
					case TokenPrimary: lTokenType = _T("Primary"); break;
					case TokenImpersonation: lTokenType = _T("Impersonation"); break;
				}

				if	(IsTokenRestricted (pToken))
				{
					lTokenType.Format (_T("%s (Restricted"), CString((LPCTSTR)lTokenType));
				}

				LogMessage (pLogLevel, _T("%s Type   [%u] [%s]"), lTitle, lStatistics.TokenType, lTokenType);

				if	(lStatistics.TokenType == TokenImpersonation)
				{
					switch (lStatistics.ImpersonationLevel)
					{
						case SecurityAnonymous: lImpersonationLevel = _T("Anonymous"); break;
						case SecurityIdentification: lImpersonationLevel = _T("Identification"); break;
						case SecurityImpersonation: lImpersonationLevel = _T("Impersonation"); break;
						case SecurityDelegation: lImpersonationLevel = _T("Delegation"); break;
					}
					LogMessage (pLogLevel, _T("%s Level  [%u] [%s]"), lTitle, lStatistics.ImpersonationLevel, lImpersonationLevel);
				}
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenInformation [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void LogTokenIntegrity (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#if	(_MSC_VER >= 1400)
	if	(
			(LogIsActive (pLogLevel))
		&&	(IsWindowsVista_AtLeast ())
		)
	{
		try
		{
			CString								lTitle (pTitle);
			tMallocPtr <TOKEN_MANDATORY_LABEL>	lTokenIntegrity;
			DWORD								lDataSize = 0;
			DWORD								lIntegrityLevel;
			LPTSTR								lSidStr = NULL;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			GetTokenInformation (pToken, TokenIntegrityLevel, NULL, 0, &lDataSize);
			if	(
					(lDataSize > 0)
				&&	(lTokenIntegrity.AllocBulk (lDataSize))
				&&	(GetTokenInformation (pToken, TokenIntegrityLevel, lTokenIntegrity, lDataSize, &lDataSize))
				)
			{
				lIntegrityLevel = *GetSidSubAuthority (lTokenIntegrity->Label.Sid, (DWORD)(UCHAR)(*GetSidSubAuthorityCount(lTokenIntegrity->Label.Sid)-1));

				if	(lIntegrityLevel == SECURITY_MANDATORY_LOW_RID)
				{
					LogMessage (pLogLevel, _T("%s Integrity Low"), lTitle);
				}
				else
				if	(lIntegrityLevel == SECURITY_MANDATORY_MEDIUM_RID)
				{
					LogMessage (pLogLevel, _T("%s Integrity Medium"), lTitle);
				}
				else
				if	(lIntegrityLevel == SECURITY_MANDATORY_HIGH_RID)
				{
					LogMessage (pLogLevel, _T("%s Integrity High"), lTitle);
				}
				else
				if	(lIntegrityLevel == SECURITY_MANDATORY_SYSTEM_RID)
				{
					LogMessage (pLogLevel, _T("%s Integrity System"), lTitle);
				}
				else
				if	(
						(lTokenIntegrity->Label.Sid)
					&&	(IsValidSid (lTokenIntegrity->Label.Sid))
					&&	(ConvertSidToStringSid (lTokenIntegrity->Label.Sid, &lSidStr))
					)
				{
					LogMessage (pLogLevel, _T("%s Integrity [%s]"), lTitle, lSidStr);
					LocalFree (lSidStr);
				}
				else
				{
					LogMessage (pLogLevel, _T("%s Integrity unavailable"), lTitle);
				}
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetTokenInformation [%u]"), lDataSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void LogTokenElevation (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle)
{
#if	(_MSC_VER >= 1400)
	if	(
			(LogIsActive (pLogLevel))
		&&	(IsWindowsVista_AtLeast ())
		)
	{
		try
		{
			CString	lTitle (pTitle);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Token");
			}

			{
				TOKEN_ELEVATION_TYPE	lElevationType;
				DWORD					lDataSize = sizeof (lElevationType);

				if	(GetTokenInformation (pToken, TokenElevationType, &lElevationType, lDataSize, &lDataSize))
				{
					LogMessage (pLogLevel, _T("%s ElevationType [%u]"), lTitle, lElevationType);
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError (), _T("GetTokenInformation [%u]"), lDataSize);
				}
			}
			{
				tS <TOKEN_ELEVATION>	lElevation;
				DWORD					lDataSize = sizeof (lElevation);

				if	(GetTokenInformation (pToken, TokenElevation, &lElevation, lDataSize, &lDataSize))
				{
					LogMessage (pLogLevel, _T("%s Elevation [%u]"), lTitle, lElevation.TokenIsElevated);
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError (), _T("GetTokenInformation [%u]"), lDataSize);
				}
			}
			{
				tS <TOKEN_MANDATORY_POLICY>	lMandatoryPolicy;
				DWORD						lDataSize = sizeof (lMandatoryPolicy);
				CString						lPolicyStr;

				if	(GetTokenInformation (pToken, TokenMandatoryPolicy, &lMandatoryPolicy, lDataSize, &lDataSize))
				{
					if	(lMandatoryPolicy.Policy & TOKEN_MANDATORY_POLICY_NO_WRITE_UP)
					{
						lPolicyStr += _T("NO_WRITE_UP ");
					}
					if	(lMandatoryPolicy.Policy & TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN)
					{
						lPolicyStr += _T("NEW_PROCESS_MIN ");
					}
					lPolicyStr.TrimRight ();
					LogMessage (pLogLevel, _T("%s MandatoryPolicy [%2.2X] [%s]"), lTitle, lMandatoryPolicy.Policy, lPolicyStr);
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError (), _T("GetTokenInformation [%u]"), lDataSize);
				}
			}
			{
				tS <TOKEN_LINKED_TOKEN>	lLinkedToken;
				DWORD					lDataSize = sizeof (lLinkedToken);

				if	(GetTokenInformation (pToken, TokenLinkedToken, &lLinkedToken, lDataSize, &lDataSize))
				{
					LogMessage (pLogLevel, _T("%s Token [%p] Linked [%p]"), lTitle, pToken, lLinkedToken.LinkedToken);
/**/				LogTokenType (lLinkedToken.LinkedToken, pLogLevel, _T("  Linked"));
/**/				LogTokenIntegrity (lLinkedToken.LinkedToken, pLogLevel, _T("  Linked"));
					CloseHandle (lLinkedToken.LinkedToken);
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError (), _T("GetTokenInformation [%u]"), lDataSize);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}
