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
#if !defined(AFX_SECURITYDESCRIPTOR_H__589CDE21_C70E_4a33_9BC3_70BACEF162FC__INCLUDED_)
#define AFX_SECURITYDESCRIPTOR_H__589CDE21_C70E_4a33_9BC3_70BACEF162FC__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CSecurityDescriptor
{
public:
	CSecurityDescriptor (LPCTSTR pDescriptorString = NULL);
	CSecurityDescriptor (HANDLE pToken);
	virtual ~CSecurityDescriptor ();

// Attributes
	PSECURITY_DESCRIPTOR	mDescriptor;
	PSID					mOwner;
	PSID					mGroup;
	PACL					mDAcl;
	PACL					mSAcl;

	bool IsValid () const;
	bool HasInformation (SECURITY_INFORMATION pInformationType) const;

// Conversions
	CSecurityDescriptor & operator= (LPCTSTR pDescriptorString);
	CSecurityDescriptor & operator= (HANDLE pToken);
	operator CString () const;

// Operations
	bool LimitInformation (SECURITY_INFORMATION pInformationType);
	bool ConsolidateExplicitAccess (bool pAccessGrants = true, bool pAccessDenials = true);

	DWORD GetEffectiveAccessGranted (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask = STANDARD_RIGHTS_ALL|FILE_ALL_ACCESS);
	DWORD GetEffectiveAccessDenied (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask = STANDARD_RIGHTS_ALL|FILE_ALL_ACCESS);
	DWORD GetExplicitAccessGranted (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask = STANDARD_RIGHTS_ALL|FILE_ALL_ACCESS);
	DWORD GetExplicitAccessDenied (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask = STANDARD_RIGHTS_ALL|FILE_ALL_ACCESS);

	DWORD GrantExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess, DWORD pInheritance = NO_PROPAGATE_INHERIT_ACE);
	DWORD GrantExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess, SID_NAME_USE pSidType = SidTypeUnknown, DWORD pInheritance = NO_PROPAGATE_INHERIT_ACE);
	DWORD UngrantExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess);
	DWORD UngrantExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess);
	DWORD DenyExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess, DWORD pInheritance = NO_PROPAGATE_INHERIT_ACE);
	DWORD DenyExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess, SID_NAME_USE pSidType = SidTypeUnknown, DWORD pInheritance = NO_PROPAGATE_INHERIT_ACE);
	DWORD UndenyExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess);
	DWORD UndenyExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess);

	DWORD SetOwner (LPCTSTR pOwner);
	DWORD SetOwner (PSID pOwnerSid);
	DWORD SetGroup (LPCTSTR pGroup);
	DWORD SetGroup (PSID pGroupSid);

	DWORD AccessCheck (ACCESS_MASK & pAccess, bool pThreadAccess = true, ACCESS_MASK pMask = STANDARD_RIGHTS_ALL|FILE_ALL_ACCESS);

// Logging
public:
	void DumpAccess (UINT pLogLevel, bool pLogNulls = true, LPCTSTR pFormat = NULL, ...);
	static void DumpHandleAccess (UINT pLogLevel, HANDLE pHandle, enum _SE_OBJECT_TYPE pHandleType, LPCTSTR pTitle = NULL, bool pLogNulls = true);
	static void DumpObjectAccess (UINT pLogLevel, HANDLE pHandle, LPCTSTR pTitle = NULL, bool pLogNulls = true);

	static void DumpAccessDetails (UINT pLogLevel, PVOID pDescriptor, PVOID pOwner, PVOID pGroup, struct _ACL * pDAcl, struct _ACL * pSAcl, bool pLogNulls = true, LPCTSTR pIndent = _T("  "));
	static void DumpAccessDetails (UINT pLogLevel, LPCTSTR pDescriptor, bool pLogNulls = true, LPCTSTR pIndent = _T("  "));
	static void DumpAclExplicit (UINT pLogLevel, struct _ACL * pAcl, LPCTSTR pTitle = NULL, LPCTSTR pIndent = _T("  "));
	static void DumpAclEffective (UINT pLogLevel, struct _ACL * pAcl, LPCTSTR pTitle = NULL, LPCTSTR pIndent = _T("  "));

	static CString AccessModeStr (int pAccessMode);
	static CString AccessMaskStr (DWORD pAccessMask);
	static CString AccessInheritStr (DWORD pAccessInherit);
	static CString AceTypeStr (BYTE pAceType);
	static CString AceFlagsStr (BYTE pAceFlags);

// Implementation
public:
	void Clear ();
	bool IsAbsolute () const;
	bool MakeAbsolute ();
	DWORD MakeSelfRelative ();

private:
	bool mIsAbsolute;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class tSidPtrFree
{
protected:
	static inline void _Init (SID * & pPtr) {}
	static inline void _Free (SID * pPtr)
	{
		if	(pPtr)
		{
			FreeSid (pPtr);
		}
	}
};

class tSidPtr : public tPtr <SID, tSidPtrFree>
{
public:
	tSidPtr () {}
	tSidPtr (const SID * pPtr) {operator= (pPtr);}
	tSidPtr (const tSidPtr & pPtr) {operator= (pPtr);}

	tSidPtr & operator= (const SID * pPtr)
	{
		PBYTE	lSubAuthorityCount;
		BYTE	lSubAuthorityNum;
		DWORD	lSubAuthority [8] = {0,0,0,0,0,0,0,0};
		PDWORD	lSubAuthorityPtr;

		_Free (mPtr);
		mPtr = NULL;

		if	(
				(pPtr)
			&&	(IsValidSid ((PSID) pPtr))
			&&	(lSubAuthorityCount = GetSidSubAuthorityCount ((PSID) pPtr))
			)
		{
			for	(lSubAuthorityNum = 0; lSubAuthorityNum < *lSubAuthorityCount; lSubAuthorityNum++)
			{
				if	(lSubAuthorityPtr = GetSidSubAuthority ((PSID) pPtr, lSubAuthorityNum))
				{
					lSubAuthority [lSubAuthorityNum] = *lSubAuthorityPtr;
				}
			}
			AllocateAndInitializeSid (GetSidIdentifierAuthority ((PSID) pPtr), *lSubAuthorityCount, lSubAuthority[0], lSubAuthority[1], lSubAuthority[2], lSubAuthority[3], lSubAuthority[4], lSubAuthority[5], lSubAuthority[6], lSubAuthority[7], (PSID*) &mPtr);
		}
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SECURITYDESCRIPTOR_H__589CDE21_C70E_4a33_9BC3_70BACEF162FC__INCLUDED_)
