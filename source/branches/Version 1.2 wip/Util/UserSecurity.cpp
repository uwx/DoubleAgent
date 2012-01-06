/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
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
#include <shlwapi.h>
#include <shlobj.h>
#include <lmcons.h>
#include <aclapi.h>
#include <sddl.h>
#ifdef	_UNICODE
#define	SECURITY_WIN32
#include <security.h>
#endif
#include "UserSecurity.h"
#include "ThreadSecurity.h"
#include "Log.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "secur32.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

static SID_IDENTIFIER_AUTHORITY	sWorldAuthority = SECURITY_WORLD_SID_AUTHORITY;
static SID_IDENTIFIER_AUTHORITY	sLocalAuthority = SECURITY_LOCAL_SID_AUTHORITY;
static SID_IDENTIFIER_AUTHORITY	sNtAuthority = SECURITY_NT_AUTHORITY;
static SID_IDENTIFIER_AUTHORITY	sNullAuthority = SECURITY_NULL_SID_AUTHORITY;

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CUserSecurity::GetUserId ()
{
	CString	lUserName;
	DWORD	lUserNameSize;

#ifdef	_UNICODE
	if	(GetUserNameEx (NameSamCompatible, lUserName.GetBuffer (UNLEN), &(lUserNameSize=UNLEN+1)))
	{
		lUserName.ReleaseBuffer ();

		if	(lUserName.Find (_T('$')) >= 0)
		{
			CString	lLegacyUserName;
			DWORD	lLegacyUserNameSize;

			::GetUserName (lLegacyUserName.GetBuffer (UNLEN), &(lLegacyUserNameSize=UNLEN+1));
			lLegacyUserName.ReleaseBuffer ();
			if	(lLegacyUserName.CompareNoCase (_T("SYSTEM")) == 0)
			{
				lUserName = UserNameSystem ();
			}
		}
	}
	else
	{
		LogWinErr (LogDetails|LogTime, GetLastError (), _T("GetUserNameEx"));
	}
#else
	::GetUserName (lUserName.GetBuffer (UNLEN), &(lUserNameSize=UNLEN+1));
	PathCombine (lUserName.GetBuffer (MAX_PATH), GetComputerName (), CString((LPCTSTR)lUserName));
	lUserName.ReleaseBuffer ();
#endif
	return lUserName;
}

CString CUserSecurity::GetUserName ()
{
	CString	lUserName;

#ifdef	_UNICODE
	lUserName = GetUserId ();
	PathStripPath (lUserName.GetBuffer (lUserName.GetLength ()));
#else
	DWORD	lUserNameSize;
	::GetUserName (lUserName.GetBufferSetLength (UNLEN), &(lUserNameSize=UNLEN+1));
	lUserName.ReleaseBuffer ();
#endif
	return lUserName;
}

CString CUserSecurity::GetComputerName ()
{
	CString	lComputerName;
	DWORD	lComputerNameSize;

	if	(!::GetComputerName (lComputerName.GetBuffer (MAX_COMPUTERNAME_LENGTH), &(lComputerNameSize=MAX_COMPUTERNAME_LENGTH+1)))
	{
		LogWinErr (LogIfActive|LogTime, GetLastError (), _T("GetComputerName"));
	}
	lComputerName.ReleaseBuffer ();

	return lComputerName;
}

//////////////////////////////////////////////////////////////////////

bool CUserSecurity::GetTokenUser (HANDLE pUserToken, CString& pUserName, CString& pDomainName)
{
	bool	lRet = false;

	pUserName.Empty ();
	pDomainName.Empty ();

#ifdef	_UNICODE
	try
	{
		if	(pUserToken)
		{
			tArrayPtr <BYTE>	lTokenUser;
			DWORD				lInfoSize = 0;
			DWORD				lUserNameSize = MAX_PATH;
			DWORD				lDomainNameSize = MAX_PATH;
			SID_NAME_USE		lNameUse = SidTypeUnknown;

			GetTokenInformation (pUserToken, TokenUser, NULL, NULL, &lInfoSize);

			if	(
					(lInfoSize)
				&&	(lTokenUser = new BYTE [lInfoSize])
				&&	(GetTokenInformation (pUserToken, TokenUser, (BYTE*) lTokenUser, lInfoSize, &lInfoSize))
				)
			{
				if	(LookupAccountSid (NULL, ((TOKEN_USER *) (BYTE*) lTokenUser)->User.Sid, pUserName.GetBuffer (lUserNameSize), &lUserNameSize, pDomainName.GetBuffer (lDomainNameSize), &lDomainNameSize, &lNameUse))
				{
					lRet = true;
				}
				else
				{
					LogWinErr (LogDetails|LogTime, GetLastError (), _T("LookupAccountSid"));
				}
				pUserName.ReleaseBuffer ();
				pDomainName.ReleaseBuffer ();
			}
			else
			{
				LogWinErr (LogDetails|LogTime, GetLastError (), _T("GetTokenInformation"));
			}

			lTokenUser = NULL;
		}
		else
		{
			LogWinErr (LogDetails|LogTime, ERROR_INVALID_HANDLE);
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

bool CUserSecurity::GetTokenUser (HANDLE pUserToken, CString& pUserId)
{
	bool	lRet = false;

	pUserId.Empty ();
#ifdef	_UNICODE
	CString	lUserName;
	CString	lDomainName;

	if	(GetTokenUser (pUserToken, lUserName, lDomainName))
	{
		if	(
				(!lUserName.IsEmpty())
			||	(!lDomainName.IsEmpty())
			)
		{
			PathCombine (pUserId.GetBuffer (MAX_PATH), lDomainName, lUserName);
			pUserId.ReleaseBuffer ();
		}
		lRet = true;
	}
#endif
	return lRet;
}

CString CUserSecurity::GetTokenUser (HANDLE pUserToken)
{
	CString	lTokenUser;
	GetTokenUser (pUserToken, lTokenUser);
	return lTokenUser;
}

//////////////////////////////////////////////////////////////////////

CString CUserSecurity::FormatUserId (LPCTSTR pUserId, bool pDefaultEveryone)
{
	CString	lUserId (pUserId);

	if	(lUserId.IsEmpty ())
	{
		if	(pDefaultEveryone)
		{
			lUserId = UserNameEveryone ();
		}
		else
		{
			lUserId = GetUserId ();
		}
	}
	else
	if	(PathIsFileSpec (lUserId))
	{
		PathCombine (lUserId.GetBuffer(MAX_PATH), GetComputerName (), pUserId);
		lUserId.ReleaseBuffer ();
	}

	return lUserId;
}

CString CUserSecurity::ParseUserId (LPCTSTR pUserId, CString * pComputerName, bool pDefaultEveryone)
{
	CString	lUserName (pUserId);
	CString	lComputerName;

	if	(lUserName.IsEmpty ())
	{
		if	(pDefaultEveryone)
		{
			lUserName = UserNameEveryone ();
		}
		else
		{
			lUserName = GetUserName ();
			lComputerName = GetComputerName ();
		}
	}
	else
	if	(PathIsFileSpec (lUserName))
	{
		lComputerName = GetComputerName ();
	}
	else
	{
		lComputerName = lUserName;
		PathRemoveFileSpec (lComputerName.GetBuffer (lComputerName.GetLength ()));
		lComputerName.ReleaseBuffer ();
		PathStripPath (lUserName.GetBuffer (lUserName.GetLength ()));
		lUserName.ReleaseBuffer ();
	}

	if	(pComputerName)
	{
		(*pComputerName) = lComputerName;
	}
	return lUserName;
}

//////////////////////////////////////////////////////////////////////

CString CUserSecurity::BuildUserTrustee (LPCTSTR pUser, LPVOID pTrustee, bool pDefaultEveryone)
{
	CString		lUser (pUser);
	PTRUSTEE	lTrustee = (PTRUSTEE) pTrustee;

	if	(lUser.IsEmpty ())
	{
		if	(pDefaultEveryone)
		{
			lUser = UserNameEveryone ();
		}
		else
		{
			lUser = GetUserId ();
		}
		BuildTrusteeWithName (lTrustee, const_cast <LPTSTR> ((LPCTSTR) lUser));
	}
	else
	{
		if	(PathIsFileSpec (lUser))
		{
			PathCombine (lUser.GetBuffer (MAX_PATH), GetComputerName (), pUser);
		}
		BuildTrusteeWithName (lTrustee, const_cast <LPTSTR> ((LPCTSTR) lUser));
	}

	return lUser;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CUserSecurity::GetSidName (PSID pSid)
{
	CString	lRet;

	try
	{
		CString			lAccountName;
		DWORD			lAccountNameSize;
		CString			lDomainName;
		DWORD			lDomainNameSize;
		SID_NAME_USE	lSidUse = SidTypeInvalid;

		if	(
				(pSid)
			&&	(IsValidSid (pSid))
			&&	(LookupAccountSid (NULL, pSid, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
			)
		{
			lAccountName.ReleaseBuffer ();
			lDomainName.ReleaseBuffer ();
			PathCombine (lRet.GetBuffer (MAX_PATH), lDomainName, lAccountName);
			lRet.ReleaseBuffer ();
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

CString CUserSecurity::GetSidStr (PSID pSid)
{
	CString	lRet;

#ifdef	_UNICODE
	try
	{
		LPTSTR	lSidStr = NULL;

		if	(
				(pSid)
			&&	(IsValidSid (pSid))
			&&	(ConvertSidToStringSid (pSid, &lSidStr))
			)
		{
			lRet = lSidStr;
			LocalFree (lSidStr);
		}
	}
	catch AnyExceptionSilent
#endif

	return lRet;
}

bool CUserSecurity::GetNameSid (LPCTSTR pAccountName, CByteArray& pAccountSid, CString * pDomainName, SID_NAME_USE * pAccountType)
{
	bool	lRet = false;

	try
	{
		DWORD			lSidSize = 0;
		CString			lDomain;
		DWORD			lDomainSize = 0;
		SID_NAME_USE	lSidUse;

		LookupAccountName (NULL, pAccountName, NULL, &lSidSize, NULL, &lDomainSize, &lSidUse);

		if	(lSidSize)
		{
#ifdef	__AFXCOLL_H__
			pAccountSid.SetSize (lSidSize);
#else
			pAccountSid.SetCount (lSidSize);
#endif

			if	(LookupAccountName (NULL, pAccountName, (PSID) pAccountSid.GetData(), &lSidSize, lDomain.GetBuffer (lDomainSize), &lDomainSize, &lSidUse))
			{
				lDomain.ReleaseBuffer ();

				if	(pDomainName)
				{
					(*pDomainName) = lDomain;
				}
				if	(pAccountType)
				{
					(*pAccountType) = lSidUse;
				}
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CUserSecurity::IsSidNull (PSID pSid)
{
	bool	lRet = false;

	try
	{
		PSID_IDENTIFIER_AUTHORITY	lSidAuthority;
		PUCHAR						lSidSubCount;
		PDWORD						lSidSubAthority;

		if	(
				(IsValidSid (pSid))
			&&	(lSidAuthority = GetSidIdentifierAuthority (pSid))
			&&	(lSidSubCount = GetSidSubAuthorityCount (pSid))
			&&	(*lSidSubCount == 1)
			&&	(lSidSubAthority = GetSidSubAuthority (pSid, 0))
			&&	(memcmp (lSidAuthority, &sNullAuthority, sizeof (sNullAuthority)) == 0)
			&&	(*lSidSubAthority == SECURITY_NULL_RID)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CUserSecurity::IsSidEveryone (PSID pSid)
{
	bool	lRet = false;

	try
	{
		PSID_IDENTIFIER_AUTHORITY	lSidAuthority;
		PUCHAR						lSidSubCount;
		PDWORD						lSidSubAthority;

		if	(
				(IsValidSid (pSid))
			&&	(lSidAuthority = GetSidIdentifierAuthority (pSid))
			&&	(lSidSubCount = GetSidSubAuthorityCount (pSid))
			&&	(*lSidSubCount == 1)
			&&	(lSidSubAthority = GetSidSubAuthority (pSid, 0))
			&&	(
					(
						(memcmp (lSidAuthority, &sWorldAuthority, sizeof (sWorldAuthority)) == 0)
					&&	(*lSidSubAthority == SECURITY_WORLD_RID)
					)
				||	(
						(memcmp (lSidAuthority, &sLocalAuthority, sizeof (sLocalAuthority)) == 0)
					&&	(*lSidSubAthority == SECURITY_LOCAL_RID)
					)
				)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CUserSecurity::IsSidSystem (PSID pSid)
{
	bool	lRet = false;

	try
	{
		PSID_IDENTIFIER_AUTHORITY	lSidAuthority;
		PUCHAR						lSidSubCount;
		PDWORD						lSidSubAthority;

		if	(
				(IsValidSid (pSid))
			&&	(lSidAuthority = GetSidIdentifierAuthority (pSid))
			&&	(memcmp (lSidAuthority, &sNtAuthority, sizeof (sNtAuthority)) == 0)
			&&	(lSidSubCount = GetSidSubAuthorityCount (pSid))
			&&	(*lSidSubCount == 1)
			&&	(lSidSubAthority = GetSidSubAuthority (pSid, 0))
			&&	(*lSidSubAthority == SECURITY_LOCAL_SYSTEM_RID)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CUserSecurity::IsSidService (PSID pSid)
{
	bool	lRet = false;

	try
	{
		PSID_IDENTIFIER_AUTHORITY	lSidAuthority;
		PUCHAR						lSidSubCount;
		PDWORD						lSidSubAthority;

		if	(
				(IsValidSid (pSid))
			&&	(lSidAuthority = GetSidIdentifierAuthority (pSid))
			&&	(memcmp (lSidAuthority, &sNtAuthority, sizeof (sNtAuthority)) == 0)
			&&	(lSidSubCount = GetSidSubAuthorityCount (pSid))
			&&	(*lSidSubCount == 1)
			&&	(lSidSubAthority = GetSidSubAuthority (pSid, 0))
			&&	(*lSidSubAthority == SECURITY_LOCAL_SERVICE_RID)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CUserSecurity::IsSidNetService (PSID pSid)
{
	bool	lRet = false;

	try
	{
		PSID_IDENTIFIER_AUTHORITY	lSidAuthority;
		PUCHAR						lSidSubCount;
		PDWORD						lSidSubAthority;

		if	(
				(IsValidSid (pSid))
			&&	(lSidAuthority = GetSidIdentifierAuthority (pSid))
			&&	(memcmp (lSidAuthority, &sNtAuthority, sizeof (sNtAuthority)) == 0)
			&&	(lSidSubCount = GetSidSubAuthorityCount (pSid))
			&&	(*lSidSubCount == 1)
			&&	(lSidSubAthority = GetSidSubAuthority (pSid, 0))
			&&	(*lSidSubAthority == SECURITY_NETWORK_SERVICE_RID)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CUserSecurity::IsSidBuiltinGroup (PSID pSid)
{
	bool	lRet = false;

	try
	{
		PSID_IDENTIFIER_AUTHORITY	lSidAuthority;
		PUCHAR						lSidSubCount;
		PDWORD						lSidSubAthority;

		if	(
				(IsValidSid (pSid))
			&&	(lSidAuthority = GetSidIdentifierAuthority (pSid))
			&&	(memcmp (lSidAuthority, &sNtAuthority, sizeof (sNtAuthority)) == 0)
			&&	(lSidSubCount = GetSidSubAuthorityCount (pSid))
			&&	(*lSidSubCount > 0)
			&&	(lSidSubAthority = GetSidSubAuthority (pSid, 0))
			&&	(*lSidSubAthority == SECURITY_BUILTIN_DOMAIN_RID)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CUserSecurity::IsSidAllUsers (PSID pSid)
{
	bool	lRet = false;

#ifdef	_UNICODE
	try
	{
		static CByteArray	lAllUsersSid;

		if	(lAllUsersSid.GetCount() <= 0)
		{
			try
			{
				CString				lComputerName;
				DWORD				lComputerNameSize;
				CByteArray			lComputerSid;
				PSID				lRootSid;
				BYTE				lSubCount;
				CTypeArray <DWORD>	lSubIds;
				tSidPtr				lSid;

				if	(::GetComputerName (lComputerName.GetBuffer (MAX_COMPUTERNAME_LENGTH), &(lComputerNameSize=MAX_COMPUTERNAME_LENGTH+1)))
				{
					lComputerName.ReleaseBuffer ();

					if	(
							(GetNameSid (lComputerName, lComputerSid))
						&&	(IsValidSid (lRootSid = (PSID) lComputerSid.GetData()))
						&&	(lSubCount = *GetSidSubAuthorityCount (lRootSid))
						)
					{
#ifdef	__AFXCOLL_H__
						lSubIds.SetSize (8);
#else
						lSubIds.SetCount (8);
#endif
						for	(BYTE lNdx = 0; lNdx < lSubCount; lNdx++)
						{
							lSubIds [lNdx] = *GetSidSubAuthority (lRootSid, (DWORD) lNdx);
						}
						lSubIds [lSubCount] = DOMAIN_GROUP_RID_USERS;

						if	(AllocateAndInitializeSid (GetSidIdentifierAuthority (lRootSid), lSubCount+1, lSubIds [0], lSubIds [1], lSubIds [2], lSubIds [3], lSubIds [4], lSubIds [5], lSubIds [6], lSubIds [7], (void**) lSid.Free ()))
						{
#ifdef	__AFXCOLL_H__
							lAllUsersSid.SetSize (GetLengthSid (lSid));
#else
							lAllUsersSid.SetCount (GetLengthSid (lSid));
#endif
							CopySid ((DWORD)lAllUsersSid.GetCount(), (PSID) lAllUsersSid.GetData(), lSid);
						}
					}
				}
			}
			catch AnyExceptionDebug

			if	(lAllUsersSid.GetCount() <= 0)
			{
#ifdef	__AFXCOLL_H__
				lAllUsersSid.SetSize (1);
#else
				lAllUsersSid.SetCount (1);
#endif
			}
		}

		if	(
				(IsValidSid (pSid))
			&&	(EqualSid (pSid, (PSID) lAllUsersSid.GetData()))
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifdef	_UNICODE
//////////////////////////////////////////////////////////////////////

PSID CUserSecurity::SidMe (tSidPtr& pSid)
{
	CString			lUserName;
	DWORD			lUserNameSize;
	CString			lDomainName;
	DWORD			lDomainSize = 0;
	DWORD			lSidSize = 0;
	SID_NAME_USE	lSidUse;

	if	(GetUserNameEx (NameSamCompatible, lUserName.GetBuffer (UNLEN), &(lUserNameSize=UNLEN+1)))
	{
		lUserName.ReleaseBuffer ();
		LookupAccountName (NULL, lUserName, NULL, &lSidSize, NULL, &lDomainSize, &lSidUse);

		if	(
				(AllocateAndInitializeSid (&sWorldAuthority, 8, 0, 0, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
			&&	(lSidSize <= GetLengthSid(pSid))
			&&	(LookupAccountName (NULL, lUserName, pSid, &lSidSize, lDomainName.GetBuffer(lDomainSize), &lDomainSize, &lSidUse))
			)
		{
			return pSid;
		}
		else
		{
			pSid.Free();
		}
	}
	return NULL;
}

PSID CUserSecurity::SidEveryone (tSidPtr& pSid)
{
	if	(AllocateAndInitializeSid (&sWorldAuthority, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
	{
		return pSid;
	}
	return NULL;
}

PSID CUserSecurity::SidSystem (tSidPtr& pSid)
{
	if	(AllocateAndInitializeSid (&sNtAuthority, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
	{
		return pSid;
	}
	return NULL;
}

PSID CUserSecurity::SidService (tSidPtr& pSid)
{
	if	(AllocateAndInitializeSid (&sNtAuthority, 1, SECURITY_LOCAL_SERVICE_RID, 0, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
	{
		return pSid;
	}
	return NULL;
}

PSID CUserSecurity::SidNetService (tSidPtr& pSid)
{
	if	(AllocateAndInitializeSid (&sNtAuthority, 1, SECURITY_NETWORK_SERVICE_RID, 0, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
	{
		return pSid;
	}
	return NULL;
}

PSID CUserSecurity::SidAdministrators (tSidPtr& pSid)
{
	if	(AllocateAndInitializeSid (&sNtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
	{
		return pSid;
	}
	return NULL;
}

PSID CUserSecurity::SidUsers (tSidPtr& pSid)
{
	if	(AllocateAndInitializeSid (&sNtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_USERS, 0, 0, 0, 0, 0, 0, (void**) pSid.Free ()))
	{
		return pSid;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////////////////////////

CString CUserSecurity::UserNameEveryone (bool pIncludeDomain)
{
	CString	lRet;

#ifdef	_UNICODE
	static CString	lUserName;

	if	(lUserName.IsEmpty ())
	{
		tSidPtr	lSid;

		if	(SidEveryone (lSid))
		{
			lUserName = GetSidName (lSid);
		}
	}
	lRet = (LPCTSTR)lUserName;
#endif

	if	(lRet.IsEmpty ())
	{
		lRet = _T("Everyone");
	}
	else
	if	(!pIncludeDomain)
	{
		PathStripPath (lRet.GetBuffer (lRet.GetLength ()));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

CString CUserSecurity::UserNameSystem (bool pIncludeDomain)
{
	CString	lRet;

#ifdef	_UNICODE
	static CString	lUserName;

	if	(lUserName.IsEmpty ())
	{
		tSidPtr	lSid;

		if	(SidSystem (lSid))
		{
			lUserName = GetSidName (lSid);
		}
	}
	lRet = (LPCTSTR)lUserName;
#endif

	if	(
			(!lRet.IsEmpty ())
		&&	(!pIncludeDomain)
		)
	{
		PathStripPath (lRet.GetBuffer (lRet.GetLength ()));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

CString CUserSecurity::UserNameService (bool pIncludeDomain)
{
	CString	lRet;

#ifdef	_UNICODE
	static CString	lUserName;

	if	(lUserName.IsEmpty ())
	{
		tSidPtr	lSid;

		if	(SidService (lSid))
		{
			lUserName = GetSidName (lSid);
		}
	}
	lRet = (LPCTSTR)lUserName;
#endif

	if	(
			(!lRet.IsEmpty ())
		&&	(!pIncludeDomain)
		)
	{
		PathStripPath (lRet.GetBuffer (lRet.GetLength ()));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

CString CUserSecurity::UserNameNetService (bool pIncludeDomain)
{
	CString	lRet;

#ifdef	_UNICODE
	static CString	lUserName;

	if	(lUserName.IsEmpty ())
	{
		tSidPtr	lSid;

		if	(SidNetService (lSid))
		{
			lUserName = GetSidName (lSid);
		}
	}
	lRet = (LPCTSTR)lUserName;
#endif

	if	(
			(!lRet.IsEmpty ())
		&&	(!pIncludeDomain)
		)
	{
		PathStripPath (lRet.GetBuffer (lRet.GetLength ()));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

CString CUserSecurity::UserNameAdministrators (bool pIncludeDomain)
{
	CString	lRet;

#ifdef	_UNICODE
	static CString	lUserName;

	if	(lUserName.IsEmpty ())
	{
		tSidPtr	lSid;

		if	(SidAdministrators (lSid))
		{
			lUserName = GetSidName (lSid);
		}
	}
	lRet = (LPCTSTR)lUserName;
#endif

	if	(
			(!lRet.IsEmpty ())
		&&	(!pIncludeDomain)
		)
	{
		PathStripPath (lRet.GetBuffer (lRet.GetLength ()));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

CString CUserSecurity::UserNameUsers (bool pIncludeDomain)
{
	CString	lRet;

#ifdef	_UNICODE
	static CString	lUserName;

	if	(lUserName.IsEmpty ())
	{
		tSidPtr	lSid;

		if	(SidUsers (lSid))
		{
			lUserName = GetSidName (lSid);
		}
	}
	lRet = (LPCTSTR)lUserName;
#endif

	if	(
			(!lRet.IsEmpty ())
		&&	(!pIncludeDomain)
		)
	{
		PathStripPath (lRet.GetBuffer (lRet.GetLength ()));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CUserSecurity::IsUserAdministrator ()
{
#ifdef	_UNICODE
#if	(_MSC_VER >= 1400)
	return IsUserAnAdmin() ? true : false;
#else
	bool	lRet = false;

	try
	{
		tSidPtr	lAdministratorsGroup;
		BOOL	lIsMember = FALSE;

		if	(AllocateAndInitializeSid (&sNtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, (void**) lAdministratorsGroup.Free ()))
		{
			if	(
					(CheckTokenMembership (NULL, lAdministratorsGroup, &lIsMember))
				&&	(lIsMember)
				)
			{
				lRet = true;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
#endif
#else
	return true;
#endif
}

bool CUserSecurity::CanUserRegisterClass ()
{
#if	(_MSC_VER >= 1400)
	return IsUserAnAdmin() ? true : false;
#endif
#ifdef	_UNICODE_LATER
	bool	lRet = false;

	try
	{
		DWORD				lDescriptorSize = sizeof (SECURITY_DESCRIPTOR);
		tArrayPtr <BYTE>	lDescriptor = new BYTE [lDescriptorSize];

		if	(RegGetKeySecurity (HKEY_CLASSES_ROOT, DACL_SECURITY_INFORMATION, (PISECURITY_DESCRIPTOR) lDescriptor.Ptr (), &lDescriptorSize) == ERROR_INSUFFICIENT_BUFFER)
		{
			lDescriptor = new BYTE [lDescriptorSize];
		}

		if	(LogWinErr (LogNormal|LogTime, RegGetKeySecurity (HKEY_CLASSES_ROOT, DACL_SECURITY_INFORMATION, (PISECURITY_DESCRIPTOR) lDescriptor.Ptr (), &lDescriptorSize)) == ERROR_SUCCESS)
		{
			BOOL	lDAclPresent = FALSE;
			BOOL	lDAclDefaulted = FALSE;
			PACL	lDAcl = NULL;

			GetSecurityDescriptorDacl ((PISECURITY_DESCRIPTOR) lDescriptor.Ptr (), &lDAclPresent, &lDAcl, &lDAclDefaulted);

			if	(lDAcl)
			{
				tS <TRUSTEE>	lTrustee;
				DWORD			lAccessMask = 0;

				BuildTrusteeWithName (&lTrustee, _T("CURRENT_USER"));
				GetEffectiveRightsFromAcl (lDAcl, &lTrustee, &lAccessMask);

				if	(
						(lAccessMask & FILE_READ_DATA)
					&&	(lAccessMask & FILE_WRITE_DATA)
					&&	(lAccessMask & FILE_APPEND_DATA)
					)
				{
					lRet = true;
				}
			}
			else
			{
				lRet = true;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
#else
	return true;
#endif
}
