/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#if !defined(AFX_USERSECURITY_H__B8C8A734_EC22_4CB7_B620_72592E3155FE__INCLUDED_)
#define AFX_USERSECURITY_H__B8C8A734_EC22_4CB7_B620_72592E3155FE__INCLUDED_
#pragma once

#include "SecurityDesc.h"

//////////////////////////////////////////////////////////////////////

class CUserSecurity
{
public:
	CUserSecurity () {}
	~CUserSecurity () {}

// Operations
	static CString GetUserId ();
	static CString GetUserName ();
	static CString GetComputerName ();

	static bool GetTokenUser (HANDLE pUserToken, CString & pUserName, CString & pDomainName);
	static bool GetTokenUser (HANDLE pUserToken, CString & pUserId);
	static CString GetTokenUser (HANDLE pUserToken);

	static CString FormatUserId (LPCTSTR pUserId, bool pDefaultEveryone = true);
	static CString ParseUserId (LPCTSTR pUserId, CString * pComputerName = NULL, bool pDefaultEveryone = true);

	static CString GetSidName (PSID pSid);
	static CString GetSidName (const CByteArray & pSid) {return GetSidName ((PSID) pSid.GetData());}
	static CString GetSidStr (PSID pSid);
	static CString GetSidStr (const CByteArray & pSid) {return GetSidStr ((PSID) pSid.GetData());}
	static bool GetNameSid (LPCTSTR pAccountName, CByteArray & pAccountSid, CString * pDomainName = NULL, SID_NAME_USE * pAccountType = NULL);

	static bool IsSidNull (PSID pSid);
	static bool IsSidEveryone (PSID pSid);
	static bool IsSidSystem (PSID pSid);
	static bool IsSidService (PSID pSid);
	static bool IsSidNetService (PSID pSid);
	static bool IsSidBuiltinGroup (PSID pSid);
	static bool IsSidAllUsers (PSID pSid);

	static bool IsSidNull (const CByteArray & pSid) {return IsSidNull ((PSID) pSid.GetData());}
	static bool IsSidEveryone (const CByteArray & pSid) {return IsSidEveryone ((PSID) pSid.GetData());}
	static bool IsSidSystem (const CByteArray & pSid) {return IsSidSystem ((PSID) pSid.GetData());}
	static bool IsSidService (const CByteArray & pSid) {return IsSidService ((PSID) pSid.GetData());}
	static bool IsSidNetService (const CByteArray & pSid) {return IsSidNetService ((PSID) pSid.GetData());}
	static bool IsSidBuiltinGroup (const CByteArray & pSid) {return IsSidBuiltinGroup ((PSID) pSid.GetData());}
	static bool IsSidAllUsers (const CByteArray & pSid) {return IsSidAllUsers ((PSID) pSid.GetData());}

#ifdef	_UNICODE
	static bool IsSidNull (const tSidPtr & pSid) {return IsSidNull ((PSID)(const SID*) pSid);}
	static bool IsSidEveryone (const tSidPtr & pSid) {return IsSidEveryone ((PSID)(const SID*) pSid);}
	static bool IsSidSystem (const tSidPtr & pSid) {return IsSidSystem ((PSID)(const SID*) pSid);}
	static bool IsSidService (const tSidPtr & pSid) {return IsSidService ((PSID)(const SID*) pSid);}
	static bool IsSidNetService (const tSidPtr & pSid) {return IsSidNetService ((PSID)(const SID*) pSid);}
	static bool IsSidBuiltinGroup (const tSidPtr & pSid) {return IsSidBuiltinGroup ((PSID)(const SID*) pSid);}
	static bool IsSidAllUsers (const tSidPtr & pSid) {return IsSidAllUsers ((PSID)(const SID*) pSid);}
#endif

#ifdef	_UNICODE
	static PSID SidMe (tSidPtr & pSid);
	static PSID SidEveryone (tSidPtr & pSid);
	static PSID SidSystem (tSidPtr & pSid);
	static PSID SidService (tSidPtr & pSid);
	static PSID SidNetService (tSidPtr & pSid);
	static PSID SidAdministrators (tSidPtr & pSid);
	static PSID SidUsers (tSidPtr & pSid);
#endif

	static CString UserNameEveryone (bool pIncludeDomain = true);
	static CString UserNameSystem (bool pIncludeDomain = true);
	static CString UserNameService (bool pIncludeDomain = true);
	static CString UserNameNetService (bool pIncludeDomain = true);
	static CString UserNameAdministrators (bool pIncludeDomain = true);
	static CString UserNameUsers (bool pIncludeDomain = true);

	static bool IsUserAdministrator ();
	static bool CanUserRegisterClass ();

	static CString BuildUserTrustee (LPCTSTR pUser, LPVOID pTrustee, bool pDefaultEveryone = true);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_USERSECURITY_H__B8C8A734_EC22_4CB7_B620_72592E3155FE__INCLUDED_)
