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
#include "StdAfx.h"
#include <aclapi.h>
#include <sddl.h>
#include <lmcons.h>
#include <shlwapi.h>
#include "SecurityDesc.h"
#include "UserSecurity.h"
#include "ThreadSecurity.h"
#include "MallocPtr.h"
#include "Log.h"

#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "shlwapi.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

CSecurityDesc::CSecurityDesc (LPCTSTR pDescriptorString)
:	mDescriptor (NULL),
	mOwner (NULL),
	mGroup (NULL),
	mDAcl (NULL),
	mSAcl (NULL),
	mIsAbsolute (false)
{
	if	(pDescriptorString)
	{
		operator= (pDescriptorString);
	}
}

CSecurityDesc::CSecurityDesc (HANDLE pToken)
:	mDescriptor (NULL),
	mOwner (NULL),
	mGroup (NULL),
	mDAcl (NULL),
	mSAcl (NULL),
	mIsAbsolute (false)
{
	operator= (pToken);
}

CSecurityDesc::~CSecurityDesc ()
{
	Clear ();
}

//////////////////////////////////////////////////////////////////////

bool CSecurityDesc::IsValid () const
{
	if	(
			(mDescriptor)
		&&	(IsValidSecurityDescriptor (mDescriptor))
		)
	{
		return true;
	}
	return false;
}

void CSecurityDesc::Clear ()
{
	if	(mDescriptor)
	{
		try
		{
			LocalFree (mDescriptor);
		}
		catch AnyExceptionSilent
	}

	if	(mIsAbsolute)
	{
		try
		{
			if	(mOwner)
			{
				LocalFree (mOwner);
			}
			if	(mGroup)
			{
				LocalFree (mGroup);
			}
			if	(mDAcl)
			{
				LocalFree (mDAcl);
			}
			if	(mSAcl)
			{
				LocalFree (mSAcl);
			}
		}
		catch AnyExceptionSilent
	}

	mIsAbsolute = false;
	mDescriptor = NULL;
	mOwner = NULL;
	mGroup = NULL;
	mDAcl = NULL;
	mSAcl = NULL;
}

//////////////////////////////////////////////////////////////////////

bool CSecurityDesc::IsAbsolute () const
{
	return mIsAbsolute;
}

bool CSecurityDesc::MakeAbsolute ()
{
	bool	lRet = false;

	if	(
			(IsValid ())
		&&	(!IsAbsolute ())
		)
	{
		try
		{
			PSECURITY_DESCRIPTOR	lDescriptor = NULL;
			PSID					lOwner = NULL;
			PSID					lGroup = NULL;
			PACL					lDAcl = NULL;
			PACL					lSAcl = NULL;
			DWORD					lDescriptorSize = 0;
			DWORD					lOwnerSize = 0;
			DWORD					lGroupSize = 0;
			DWORD					lDAclSize = 0;
			DWORD					lSAclSize = 0;

			MakeAbsoluteSD (mDescriptor, lDescriptor, &lDescriptorSize, lDAcl, &lDAclSize, lSAcl, &lSAclSize, lOwner, &lOwnerSize, lGroup, &lGroupSize);
			if	(lDescriptorSize)
			{
				lDescriptor = (PSECURITY_DESCRIPTOR) LocalAlloc (LPTR, lDescriptorSize);
			}
			if	(lOwnerSize)
			{
				lOwner = (PSID) LocalAlloc (LPTR, lOwnerSize);
			}
			if	(lGroupSize)
			{
				lGroup = (PSID) LocalAlloc (LPTR, lGroupSize);
			}
			if	(lDAclSize)
			{
				lDAcl = (PACL) LocalAlloc (LPTR, lDAclSize);
			}
			if	(lSAclSize)
			{
				lSAcl = (PACL) LocalAlloc (LPTR, lSAclSize);
			}

			if	(MakeAbsoluteSD (mDescriptor, lDescriptor, &lDescriptorSize, lDAcl, &lDAclSize, lSAcl, &lSAclSize, lOwner, &lOwnerSize, lGroup, &lGroupSize))
			{
				Clear ();

				mIsAbsolute = true;
				mDescriptor = lDescriptor;
				mOwner = lOwner;
				mGroup = lGroup;
				mDAcl = lDAcl;
				mSAcl = lSAcl;

				lRet = true;
			}
			else
			{
				LogWinErr (LogIfActive, GetLastError ());

				if	(lDescriptor)
				{
					LocalFree (lDescriptor);
				}
				if	(lOwner)
				{
					LocalFree (lOwner);
				}
				if	(lGroup)
				{
					LocalFree (lGroup);
				}
				if	(lDAcl)
				{
					LocalFree (lDAcl);
				}
				if	(lSAcl)
				{
					LocalFree (lSAcl);
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

DWORD CSecurityDesc::MakeSelfRelative ()
{
	DWORD	lRet = 0;

	if	(
			(IsValid ())
		&&	(IsAbsolute ())
		)
	{
		try
		{
			PSECURITY_DESCRIPTOR	lDescriptor = NULL;
			DWORD					lDescriptorSize = 0;

			MakeSelfRelativeSD (mDescriptor, lDescriptor, &lDescriptorSize);

			if	(lDescriptorSize)
			{
				lDescriptor = (PSECURITY_DESCRIPTOR) LocalAlloc (LPTR, lDescriptorSize);
			}
			if	(MakeSelfRelativeSD (mDescriptor, lDescriptor, &lDescriptorSize))
			{
				BOOL	lOwnerDefaulted = FALSE;
				BOOL	lGroupDefaulted = FALSE;
				BOOL	lDAclPresent = FALSE;
				BOOL	lDAclDefaulted = FALSE;
				BOOL	lSAclPresent = FALSE;
				BOOL	lSAclDefaulted = FALSE;

				Clear ();

				mIsAbsolute = false;
				mDescriptor = lDescriptor;
				GetSecurityDescriptorOwner (mDescriptor, &mOwner, &lOwnerDefaulted);
				GetSecurityDescriptorGroup (mDescriptor, &mGroup, &lGroupDefaulted);
				GetSecurityDescriptorDacl (mDescriptor, &lDAclPresent, &mDAcl, &lDAclDefaulted);
				GetSecurityDescriptorSacl (mDescriptor, &lSAclPresent, &mSAcl, &lSAclDefaulted);

				lRet = lDescriptorSize;
			}
			else
			{
				LogWinErr (LogIfActive, GetLastError ());

				if	(lDescriptor)
				{
					LocalFree (lDescriptor);
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CSecurityDesc::HasInformation (SECURITY_INFORMATION pInformationType) const
{
	if	(
			(
				(pInformationType & OWNER_SECURITY_INFORMATION)
			&&	(mOwner != NULL)
			)
		||	(
				(pInformationType & GROUP_SECURITY_INFORMATION)
			&&	(mGroup != NULL)
			)
		||	(
				(pInformationType & DACL_SECURITY_INFORMATION)
			&&	(mDAcl != NULL)
			)
		||	(
				(pInformationType & SACL_SECURITY_INFORMATION)
			&&	(mSAcl != NULL)
			)
		)
	{
		return true;
	}
	return false;
}

bool CSecurityDesc::LimitInformation (SECURITY_INFORMATION pInformationType)
{
	bool	lRet = false;

	if	(
			(IsValid ())
		&&	(HasInformation (pInformationType ^ (OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION|SACL_SECURITY_INFORMATION)))
		&&	(
				(IsAbsolute ())
			||	(MakeAbsolute ())
			)
		)
	{
		if	(
				((pInformationType & OWNER_SECURITY_INFORMATION) == 0)
			&&	(mOwner != NULL)
			&&	(SetSecurityDescriptorOwner (mDescriptor, NULL, TRUE))
			)
		{
			try
			{
				LocalFree (mOwner);
			}
			catch AnyExceptionSilent
			mOwner = NULL;
			lRet = true;
		}

		if	(
				((pInformationType & GROUP_SECURITY_INFORMATION) == 0)
			&&	(mGroup != NULL)
			&&	(SetSecurityDescriptorGroup (mDescriptor, NULL, TRUE))
			)
		{
			try
			{
				LocalFree (mGroup);
			}
			catch AnyExceptionSilent
			mGroup = NULL;
			lRet = true;
		}

		if	(
				((pInformationType & DACL_SECURITY_INFORMATION) == 0)
			&&	(mDAcl != NULL)
			&&	(SetSecurityDescriptorDacl (mDescriptor, FALSE, mDAcl, TRUE))
			)
		{
			try
			{
				LocalFree (mDAcl);
			}
			catch AnyExceptionSilent
			mDAcl = NULL;
			lRet = true;
		}

		if	(
				((pInformationType & SACL_SECURITY_INFORMATION) == 0)
			&&	(mSAcl != NULL)
			&&	(SetSecurityDescriptorSacl (mDescriptor, FALSE, mSAcl, TRUE))
			)
		{
			try
			{
				LocalFree (mSAcl);
			}
			catch AnyExceptionSilent
			mSAcl = NULL;
			lRet = true;
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSecurityDesc & CSecurityDesc::operator= (LPCTSTR pDescriptorString)
{
	Clear ();
#ifdef	_UNICODE
	if	(pDescriptorString)
	{
		if	(ConvertStringSecurityDescriptorToSecurityDescriptor (pDescriptorString, SDDL_REVISION_1, &mDescriptor, NULL))
		{
			BOOL	lOwnerDefaulted = FALSE;
			BOOL	lGroupDefaulted = FALSE;
			BOOL	lDAclPresent = FALSE;
			BOOL	lDAclDefaulted = FALSE;
			BOOL	lSAclPresent = FALSE;
			BOOL	lSAclDefaulted = FALSE;

			GetSecurityDescriptorOwner (mDescriptor, &mOwner, &lOwnerDefaulted);
			GetSecurityDescriptorGroup (mDescriptor, &mGroup, &lGroupDefaulted);
			GetSecurityDescriptorDacl (mDescriptor, &lDAclPresent, &mDAcl, &lDAclDefaulted);
			GetSecurityDescriptorSacl (mDescriptor, &lSAclPresent, &mSAcl, &lSAclDefaulted);
		}
		else
		{
			LogWinErr (LogDetails, GetLastError ());
		}
	}
#endif
	return *this;
}

CSecurityDesc::operator CString () const
{
	CString	lRet;
#ifdef	_UNICODE
	if	(mDescriptor)
	{
		SECURITY_INFORMATION	lFlags = 0;
		LPTSTR					lString = NULL;
		ULONG					lStrLen = 0;

		if	(mOwner)
		{
			lFlags |= OWNER_SECURITY_INFORMATION;
		}
		if	(mGroup)
		{
			lFlags |= GROUP_SECURITY_INFORMATION;
		}
		if	(mDAcl)
		{
			lFlags |= DACL_SECURITY_INFORMATION;
		}
		if	(mSAcl)
		{
			lFlags |= SACL_SECURITY_INFORMATION;
		}

		if	(ConvertSecurityDescriptorToStringSecurityDescriptor (mDescriptor, SDDL_REVISION_1, lFlags, &lString, &lStrLen))
		{
			lRet = lString;
		}
		if	(lString)
		{
			LocalFree (lString);
		}
	}
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSecurityDesc & CSecurityDesc::operator= (HANDLE pToken)
{
	Clear ();
#ifdef	_UNICODE
	if	(pToken)
	{
		try
		{
			tMallocPtr <TOKEN_DEFAULT_DACL>		lTokenDAcl;
			tMallocPtr <TOKEN_OWNER>			lTokenOwner;
			tMallocPtr <TOKEN_PRIMARY_GROUP>	lTokenGroup;
			DWORD								lDataSize;

			if	(
					(mDescriptor = (PSECURITY_DESCRIPTOR) LocalAlloc (LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH))
				&&	(InitializeSecurityDescriptor (mDescriptor, SECURITY_DESCRIPTOR_REVISION))
				)
			{
				try
				{
					GetTokenInformation (pToken, TokenOwner, NULL, 0, &(lDataSize=0));
					if	(
							(lDataSize > 0)
						&&	(lTokenOwner.AllocBulk (lDataSize))
						&&	(GetTokenInformation (pToken, TokenOwner, lTokenOwner, lDataSize, &lDataSize))
						)
					{
						SetSecurityDescriptorOwner (mDescriptor, lTokenOwner->Owner, FALSE);
					}

					GetTokenInformation (pToken, TokenPrimaryGroup, NULL, 0, &(lDataSize=0));
					if	(
							(lDataSize > 0)
						&&	(lTokenGroup.AllocBulk (lDataSize))
						&&	(GetTokenInformation (pToken, TokenPrimaryGroup, lTokenGroup, lDataSize, &lDataSize))
						)
					{
						SetSecurityDescriptorGroup (mDescriptor, lTokenGroup->PrimaryGroup, FALSE);
					}

					GetTokenInformation (pToken, TokenDefaultDacl, NULL, 0, &(lDataSize=0));
					if	(
							(lDataSize > 0)
						&&	(lTokenDAcl.AllocBulk (lDataSize))
						&&	(GetTokenInformation (pToken, TokenDefaultDacl, lTokenDAcl, lDataSize, &lDataSize))
						)
					{
						SetSecurityDescriptorDacl (mDescriptor, (lTokenDAcl->DefaultDacl!=NULL), lTokenDAcl->DefaultDacl, FALSE);
					}
				}
				catch AnyExceptionDebug

				mIsAbsolute = true;
				MakeSelfRelative ();
			}
			else
			if	(mDescriptor)
			{
				LogWinErr (LogDetails, GetLastError ());
				LocalFree (mDescriptor);
				mDescriptor = NULL;
			}
		}
		catch AnyExceptionDebug
	}
#endif
	return *this;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::GetEffectiveAccessGranted (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		pAccess = 0;

		if	(
				(IsValid ())
			&&	(mDAcl)
			)
		{
			tS <TRUSTEE>	lTrustee;
			CString			lUser = CUserSecurity::BuildUserTrustee (pUser, &lTrustee);

			lError = GetEffectiveRightsFromAcl (mDAcl, &lTrustee, &pAccess);
			if	(lError == ERROR_SUCCESS)
			{
				pAccess &= pMask;
			}
			else
			if	(LogIsActive ())
			{
				LogWinErr (LogDetails, lError);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

DWORD CSecurityDesc::GetEffectiveAccessDenied (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		pAccess = 0;

		if	(
				(IsValid ())
			&&	(mDAcl)
			)
		{
			tS <TRUSTEE>	lTrustee;
			CString			lUser = CUserSecurity::BuildUserTrustee (pUser, &lTrustee);

			lError = GetEffectiveRightsFromAcl (mDAcl, &lTrustee, &pAccess);
			if	(lError == ERROR_SUCCESS)
			{
				pAccess ^= pMask;
				pAccess &= STANDARD_RIGHTS_ALL|FILE_ALL_ACCESS;
			}
			else
			if	(LogIsActive ())
			{
				LogWinErr (LogDetails, lError);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::GetExplicitAccessGranted (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		pAccess = 0;

		if	(
				(IsValid ())
			&&	(mDAcl)
			)
		{
			CString				lComputerName;
			CString				lUser = CUserSecurity::ParseUserId (pUser, &lComputerName);
			ULONG				lEntryCount = 0;
			PEXPLICIT_ACCESS 	lEntries = NULL;
			ULONG				lNdx;

			lError = GetExplicitEntriesFromAcl (mDAcl, &lEntryCount, &lEntries);
			if	(lError == ERROR_SUCCESS)
			{
				for	(lNdx = 0; lNdx < lEntryCount; lNdx++)
				{
					CString			lAccountName;
					DWORD			lAccountNameSize;
					CString			lDomainName;
					DWORD			lDomainNameSize;
					SID_NAME_USE	lSidUse;

					if	(lEntries [lNdx].grfAccessMode == GRANT_ACCESS)
					{
						if	(
								(
									(GetTrusteeForm (&lEntries [lNdx].Trustee) == TRUSTEE_IS_NAME)
								&&	(
										(_tcsicmp (GetTrusteeName (&lEntries [lNdx].Trustee), lUser) == 0)
									||	(_tcsicmp (GetTrusteeName (&lEntries [lNdx].Trustee), pUser) == 0)
									)
								)
							||	(
									(GetTrusteeForm (&lEntries [lNdx].Trustee) == TRUSTEE_IS_SID)
								&&	(LookupAccountSid (NULL, lEntries [lNdx].Trustee.ptstrName, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
								&&	(_tcsicmp (lDomainName, lComputerName) == 0)
								&&	(_tcsicmp (lAccountName, lUser) == 0)
								)
							)
						{
							pAccess |= lEntries [lNdx].grfAccessPermissions;
							pAccess &= pMask;
						}
					}
				}
			}
			else
			if	(LogIsActive ())
			{
				LogWinErr (LogDetails, lError);
			}

			if	(lEntries)
			{
				LocalFree (lEntries);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::GetExplicitAccessDenied (LPCTSTR pUser, ACCESS_MASK & pAccess, ACCESS_MASK pMask)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		pAccess = 0;

		if	(
				(IsValid ())
			&&	(mDAcl)
			)
		{
			CString				lComputerName;
			CString				lUser = CUserSecurity::ParseUserId (pUser, &lComputerName);
			ULONG				lEntryCount = 0;
			PEXPLICIT_ACCESS 	lEntries = NULL;
			ULONG				lNdx;

			lError = GetExplicitEntriesFromAcl (mDAcl, &lEntryCount, &lEntries);
			if	(lError == ERROR_SUCCESS)
			{
				for	(lNdx = 0; lNdx < lEntryCount; lNdx++)
				{
					CString			lAccountName;
					DWORD			lAccountNameSize;
					CString			lDomainName;
					DWORD			lDomainNameSize;
					SID_NAME_USE	lSidUse;

					if	(lEntries [lNdx].grfAccessMode == DENY_ACCESS)
					{
						if	(
								(
									(GetTrusteeForm (&lEntries [lNdx].Trustee) == TRUSTEE_IS_NAME)
								&&	(
										(_tcsicmp (GetTrusteeName (&lEntries [lNdx].Trustee), lUser) == 0)
									||	(_tcsicmp (GetTrusteeName (&lEntries [lNdx].Trustee), pUser) == 0)
									)
								)
							||	(
									(GetTrusteeForm (&lEntries [lNdx].Trustee) == TRUSTEE_IS_SID)
								&&	(LookupAccountSid (NULL, lEntries [lNdx].Trustee.ptstrName, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
								&&	(_tcsicmp (lDomainName, lComputerName) == 0)
								&&	(_tcsicmp (lAccountName, lUser) == 0)
								)
							)
						{
							pAccess |= lEntries [lNdx].grfAccessPermissions;
							pAccess &= pMask;
						}
					}
				}
			}
			else
			if	(LogIsActive ())
			{
				LogWinErr (LogDetails, lError);
			}

			if	(lEntries)
			{
				LocalFree (lEntries);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::GrantExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess, DWORD pInheritance)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(IsValid ())
		{
			CString					lUser = CUserSecurity::FormatUserId (pUser);
			tS <EXPLICIT_ACCESS>	lAccess;
			PACL					lNewDAcl = NULL;

			if	(
					(!IsAbsolute ())
				&&	(!MakeAbsolute ())
				)
			{
				lError = GetLastError ();
			}
			else
			{
				BuildExplicitAccessWithName (&lAccess, const_cast <LPTSTR> ((LPCTSTR) lUser), pAccess, GRANT_ACCESS, pInheritance&VALID_INHERIT_FLAGS);

				lError = SetEntriesInAcl (1, &lAccess, mDAcl, &lNewDAcl);
				if	(lError == ERROR_SUCCESS)
				{
					if	(SetSecurityDescriptorDacl (mDescriptor, TRUE, lNewDAcl, FALSE))
					{
						lError = -1;
						if	(mDAcl)
						{
							LocalFree (mDAcl);
						}
						mDAcl = lNewDAcl;
					}
					else
					{
						lError = GetLastError ();
						LocalFree (lNewDAcl);
					}
				}

				if	(
						(lError != ERROR_SUCCESS)
					&&	(lError != -1)
					&&	(LogIsActive ())
					)
				{
					LogWinErr (LogDetails, lError);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

DWORD CSecurityDesc::GrantExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess, SID_NAME_USE pSidType, DWORD pInheritance)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(IsValidSid (pUserSid))
			)
		{
			tS <EXPLICIT_ACCESS>	lAccess;
			PACL					lNewDAcl = NULL;

			if	(
					(!IsAbsolute ())
				&&	(!MakeAbsolute ())
				)
			{
				lError = GetLastError ();
			}
			else
			{
				lAccess.grfAccessMode = GRANT_ACCESS;
				lAccess.grfAccessPermissions = pAccess;
				lAccess.grfInheritance = pInheritance&VALID_INHERIT_FLAGS;
				BuildTrusteeWithSid (&lAccess.Trustee, pUserSid);
				if	(pSidType == SidTypeUnknown)
				{
					CString	lAccountName;
					DWORD	lAccountNameSize;
					CString	lDomainName;
					DWORD	lDomainNameSize;

					LookupAccountSid (NULL, pUserSid, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &pSidType);
				}
				switch (pSidType)
				{
					case SidTypeUser : lAccess.Trustee.TrusteeType = TRUSTEE_IS_USER; break;
					case SidTypeGroup : lAccess.Trustee.TrusteeType = TRUSTEE_IS_GROUP; break;
					case SidTypeDomain : lAccess.Trustee.TrusteeType = TRUSTEE_IS_DOMAIN; break;
					case SidTypeAlias : lAccess.Trustee.TrusteeType = TRUSTEE_IS_ALIAS; break;
					case SidTypeWellKnownGroup : lAccess.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP; break;
					case SidTypeComputer : lAccess.Trustee.TrusteeType = TRUSTEE_IS_COMPUTER; break;
				}

				lError = SetEntriesInAcl (1, &lAccess, mDAcl, &lNewDAcl);
				if	(lError == ERROR_SUCCESS)
				{
					if	(SetSecurityDescriptorDacl (mDescriptor, TRUE, lNewDAcl, FALSE))
					{
						lError = -1;
						if	(mDAcl)
						{
							LocalFree (mDAcl);
						}
						mDAcl = lNewDAcl;
					}
					else
					{
						lError = GetLastError ();
						LocalFree (lNewDAcl);
					}
				}

				if	(
						(lError != ERROR_SUCCESS)
					&&	(lError != -1)
					&&	(LogIsActive ())
					)
				{
					LogWinErr (LogDetails, lError);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::UngrantExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(mDAcl)
			)
		{
			CString				lUser = CUserSecurity::FormatUserId (pUser);
			tArrayPtr <BYTE>	lSidBuffer;
			PSID				lSid = NULL;
			DWORD				lSidSize = 0;
			CString				lDomain;
			DWORD				lDomainSize = 0;
			SID_NAME_USE		lSidUse;

			LookupAccountName (NULL, lUser, NULL, &lSidSize, NULL, &lDomainSize, &lSidUse);

			if	(
					(lSidSize)
				&&	(lSidBuffer = new BYTE [lSidSize])
				&&	(lSid = (PSID) (LPBYTE) lSidBuffer)
				&&	(LookupAccountName (NULL, lUser, lSid, &lSidSize, lDomain.GetBuffer (lDomainSize), &lDomainSize, &lSidUse))
				)
			{
				lError = ERROR_SUCCESS;
				lDomain.ReleaseBuffer ();

				if	(LogIsActive (LogVerbose))
				{
					LogMessage (LogVerbose, _T("  User [%s] Domain [%s] Sid [%d] [%s]"), lUser, lDomain, lSidSize, CUserSecurity::GetSidStr (lSid));
				}
			}
			else
			{
				lError = GetLastError ();
				if	(lError == ERROR_SUCCESS)
				{
					lError = ERROR_INVALID_PARAMETER;
				}
				else
				{
					LogWinErr (LogNormal, lError);
				}
				LogMessage (LogNormal, _T("Cannot find account [%s]"), lUser);
				lSid = NULL;
			}

			if	(lError == ERROR_SUCCESS)
			{
				lError = UngrantExplicitAccess (lSid, pAccess);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}


DWORD CSecurityDesc::UngrantExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(mDAcl)
			&&	(IsValidSid (pUserSid))
			)
		{
			tS <ACL_SIZE_INFORMATION>	lAclSize;

			if	(GetAclInformation (mDAcl, &lAclSize, sizeof (lAclSize), AclSizeInformation))
			{
				try
				{
					long					lAceNdx;
					ACCESS_ALLOWED_ACE *	lAce;
					PSID					lAceSid;

					for	(lAceNdx = lAclSize.AceCount-1; lAceNdx >= 0; lAceNdx--)
					{
						try
						{
							if	(
									(GetAce (mDAcl, lAceNdx, (void **) &lAce))
								&&	(lAce->Header.AceType == ACCESS_ALLOWED_ACE_TYPE)
								&&	(lAceSid = (PSID) (void **) &lAce->SidStart)
								&&	(IsValidSid (lAceSid))
								)
							{
								if	(LogIsActive (LogVerbose))
								{
									LogMessage (LogVerbose|LogHighVolume, _T("  Ace [%8.8X] Account [%s] [%s]"), lAce->Mask, CUserSecurity::GetSidName (lAceSid), CUserSecurity::GetSidStr (lAceSid));
								}

								if	(
										(lAce->Mask & pAccess)
									&&	(EqualSid (pUserSid, lAceSid))
									)
								{
									if	((lAce->Mask & ~pAccess) == 0)
									{
										if	(DeleteAce (mDAcl, lAceNdx))
										{
											lError = -1;
										}
									}
									else
									{
										lAce->Mask &= ~pAccess;
										lError = -1;
									}
								}
							}
						}
						catch AnyExceptionDebug
					}
				}
				catch AnyExceptionDebug
			}
			else
			{
				lError = GetLastError ();
				LogWinErr (LogDetails, lError);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::DenyExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess, DWORD pInheritance)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(IsValid ())
		{
			CString					lUser = CUserSecurity::FormatUserId (pUser);
			tS <EXPLICIT_ACCESS>	lAccess;
			PACL					lNewDAcl = NULL;

			if	(
					(!IsAbsolute ())
				&&	(!MakeAbsolute ())
				)
			{
				lError = GetLastError ();
			}
			else
			{
				BuildExplicitAccessWithName (&lAccess, const_cast <LPTSTR> ((LPCTSTR) lUser), pAccess, DENY_ACCESS, pInheritance&VALID_INHERIT_FLAGS);

				lError = SetEntriesInAcl (1, &lAccess, mDAcl, &lNewDAcl);
				if	(lError == ERROR_SUCCESS)
				{
					if	(SetSecurityDescriptorDacl (mDescriptor, TRUE, lNewDAcl, FALSE))
					{
						lError = -1;
						if	(mDAcl)
						{
							LocalFree (mDAcl);
						}
						mDAcl = lNewDAcl;
					}
					else
					{
						lError = GetLastError ();
						LocalFree (lNewDAcl);
					}
				}

				if	(
						(lError != ERROR_SUCCESS)
					&&	(lError != -1)
					&&	(LogIsActive ())
					)
				{
					LogWinErr (LogDetails, lError);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

DWORD CSecurityDesc::DenyExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess, SID_NAME_USE pSidType, DWORD pInheritance)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(IsValidSid (pUserSid))
			)
		{
			tS <EXPLICIT_ACCESS>	lAccess;
			PACL					lNewDAcl = NULL;

			if	(
					(!IsAbsolute ())
				&&	(!MakeAbsolute ())
				)
			{
				lError = GetLastError ();
			}
			else
			{
				lAccess.grfAccessMode = DENY_ACCESS;
				lAccess.grfAccessPermissions = pAccess;
				lAccess.grfInheritance = pInheritance&VALID_INHERIT_FLAGS;
				BuildTrusteeWithSid (&lAccess.Trustee, pUserSid);
				if	(pSidType == SidTypeUnknown)
				{
					CString	lAccountName;
					DWORD	lAccountNameSize;
					CString	lDomainName;
					DWORD	lDomainNameSize;

					LookupAccountSid (NULL, pUserSid, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &pSidType);
				}
				switch (pSidType)
				{
					case SidTypeUser : lAccess.Trustee.TrusteeType = TRUSTEE_IS_USER; break;
					case SidTypeGroup : lAccess.Trustee.TrusteeType = TRUSTEE_IS_GROUP; break;
					case SidTypeDomain : lAccess.Trustee.TrusteeType = TRUSTEE_IS_DOMAIN; break;
					case SidTypeAlias : lAccess.Trustee.TrusteeType = TRUSTEE_IS_ALIAS; break;
					case SidTypeWellKnownGroup : lAccess.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP; break;
					case SidTypeComputer : lAccess.Trustee.TrusteeType = TRUSTEE_IS_COMPUTER; break;
				}

				lError = SetEntriesInAcl (1, &lAccess, mDAcl, &lNewDAcl);
				if	(lError == ERROR_SUCCESS)
				{
					if	(SetSecurityDescriptorDacl (mDescriptor, TRUE, lNewDAcl, FALSE))
					{
						lError = -1;
						if	(mDAcl)
						{
							LocalFree (mDAcl);
						}
						mDAcl = lNewDAcl;
					}
					else
					{
						lError = GetLastError ();
						LocalFree (lNewDAcl);
					}
				}

				if	(
						(lError != ERROR_SUCCESS)
					&&	(lError != -1)
					&&	(LogIsActive ())
					)
				{
					LogWinErr (LogDetails, lError);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::UndenyExplicitAccess (LPCTSTR pUser, ACCESS_MASK pAccess)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(mDAcl)
			)
		{
			CString				lUser = CUserSecurity::FormatUserId (pUser);
			tArrayPtr <BYTE>	lSidBuffer;
			PSID				lSid = NULL;
			DWORD				lSidSize = 0;
			CString				lDomain;
			DWORD				lDomainSize = 0;
			SID_NAME_USE		lSidUse;

			LookupAccountName (NULL, lUser, NULL, &lSidSize, NULL, &lDomainSize, &lSidUse);

			if	(
					(lSidSize)
				&&	(lSidBuffer = new BYTE [lSidSize])
				&&	(lSid = (PSID) (LPBYTE) lSidBuffer)
				&&	(LookupAccountName (NULL, lUser, lSid, &lSidSize, lDomain.GetBuffer (lDomainSize), &lDomainSize, &lSidUse))
				)
			{
				lError = ERROR_SUCCESS;
				lDomain.ReleaseBuffer ();

				if	(LogIsActive (LogVerbose))
				{
					LogMessage (LogVerbose, _T("  User [%s] Domain [%s] Sid [%d] [%s]"), lUser, lDomain, lSidSize, CUserSecurity::GetSidStr (lSid));
				}
			}
			else
			{
				lError = GetLastError ();
				if	(lError == ERROR_SUCCESS)
				{
					lError = ERROR_INVALID_PARAMETER;
				}
				else
				{
					LogWinErr (LogNormal, lError);
				}
				LogMessage (LogNormal, _T("Cannot find account [%s]"), lUser);
				lSid = NULL;
			}

			if	(lError == ERROR_SUCCESS)
			{
				lError = UndenyExplicitAccess (lSid, pAccess);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

DWORD CSecurityDesc::UndenyExplicitAccess (PSID pUserSid, ACCESS_MASK pAccess)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(mDAcl)
			&&	(IsValidSid (pUserSid))
			)
		{
			tS <ACL_SIZE_INFORMATION>	lAclSize;

			if	(GetAclInformation (mDAcl, &lAclSize, sizeof (lAclSize), AclSizeInformation))
			{
				try
				{
					long					lAceNdx;
					ACCESS_ALLOWED_ACE *	lAce;
					PSID					lAceSid;

					for	(lAceNdx = lAclSize.AceCount-1; lAceNdx >= 0; lAceNdx--)
					{
						try
						{
							if	(
									(GetAce (mDAcl, lAceNdx, (void **) &lAce))
								&&	(lAce->Header.AceType == ACCESS_DENIED_ACE_TYPE)
								&&	(lAceSid = (PSID) (void **) &lAce->SidStart)
								&&	(IsValidSid (lAceSid))
								)
							{
								if	(LogIsActive (LogVerbose))
								{
									LogMessage (LogVerbose|LogHighVolume, _T("  Ace [%8.8X] Account [%s] [%s]"), lAce->Mask, CUserSecurity::GetSidName (lAceSid), CUserSecurity::GetSidStr (lAceSid));
								}

								if	(
										(lAce->Mask & pAccess)
									&&	(EqualSid (pUserSid, lAceSid))
									)
								{
									if	((lAce->Mask & ~pAccess) == 0)
									{
										if	(DeleteAce (mDAcl, lAceNdx))
										{
											lError = -1;
										}
									}
									else
									{
										lAce->Mask &= ~pAccess;
										lError = -1;
									}
								}
							}
						}
						catch AnyExceptionDebug
					}
				}
				catch AnyExceptionDebug
			}
			else
			{
				lError = GetLastError ();
				LogWinErr (LogDetails, lError);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CSecurityDesc::ConsolidateExplicitAccess (bool pAccessGrants, bool pAccessDenials)
{
	bool	lRet = false;

	try
	{
		tS <ACL_SIZE_INFORMATION>	lAclSize;

		if	(
				(IsValid ())
			&&	(mDAcl)
			&&	(GetAclInformation (mDAcl, &lAclSize, sizeof (lAclSize), AclSizeInformation))
			)
		{
			long				lAceNdx1, lAceNdx2;
			PACCESS_ALLOWED_ACE	lAce1, lAce2;
			PSID				lAceSid1, lAceSid2;

			for	(lAceNdx1 = lAclSize.AceCount-1; lAceNdx1 >= 0; lAceNdx1--)
			{
				try
				{
					if	(
							(GetAce (mDAcl, lAceNdx1, (void **) &lAce1))
						&&	(
								(
									(pAccessGrants)
								&&	(lAce1->Header.AceType == ACCESS_ALLOWED_ACE_TYPE)
								)
							||	(
									(pAccessDenials)
								&&	(lAce1->Header.AceType == ACCESS_DENIED_ACE_TYPE)
								)
							)
						&&	(lAceSid1 = (PSID) (void **) &lAce1->SidStart)
						&&	(IsValidSid (lAceSid1))
						)
					{
						for	(lAceNdx2 = lAceNdx1-1; lAceNdx2 >= 0; lAceNdx2--)
						{
							if	(
									(GetAce (mDAcl, lAceNdx2, (void **) &lAce2))
								&&	(lAce2->Header.AceType == lAce1->Header.AceType)
								&&	(lAce2->Header.AceFlags == lAce1->Header.AceFlags)
								&&	(lAceSid2 = (PSID) (void **) &lAce2->SidStart)
								&&	(EqualSid (lAceSid2, lAceSid1))
								)
							{
								lAce2->Mask |= lAce1->Mask;
								if	(DeleteAce (mDAcl, lAceNdx1))
								{
									lRet = true;
									break;
								}
							}
						}
					}
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::SetOwner (LPCTSTR pOwner)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(IsValid ())
		{
			CString				lUser = CUserSecurity::FormatUserId (pOwner);
			tArrayPtr <BYTE>	lSidBuffer;
			PSID				lSid = NULL;
			DWORD				lSidSize = 0;
			CString				lDomain;
			DWORD				lDomainSize = 0;
			SID_NAME_USE		lSidUse;

			LookupAccountName (NULL, lUser, NULL, &lSidSize, NULL, &lDomainSize, &lSidUse);

			if	(
					(lSidSize)
				&&	(lSidBuffer = new BYTE [lSidSize])
				&&	(lSid = (PSID) (LPBYTE) lSidBuffer)
				&&	(LookupAccountName (NULL, lUser, lSid, &lSidSize, lDomain.GetBuffer (lDomainSize), &lDomainSize, &lSidUse))
				)
			{
				if	(
						(lSidUse == SidTypeUser)
					||	(lSidUse == SidTypeGroup)
					||	(lSidUse == SidTypeWellKnownGroup)
					||	(lSidUse == SidTypeAlias)
					)
				{
					lError = SetOwner (lSid);
				}
				else
				{
					lError = ERROR_INVALID_PARAMETER;
				}
			}
			else
			{
				lError = GetLastError ();
				if	(lError == ERROR_SUCCESS)
				{
					lError = ERROR_INVALID_PARAMETER;
				}
				else
				{
					LogWinErr (LogNormal, lError);
				}
				LogMessage (LogNormal, _T("Cannot find account [%s]"), lUser);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

DWORD CSecurityDesc::SetOwner (PSID pOwnerSid)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(IsValidSid (pOwnerSid))
			)
		{
			if	(
					(!IsAbsolute ())
				&&	(!MakeAbsolute ())
				)
			{
				lError = GetLastError ();
			}
			else
			if	(
					(mOwner != NULL)
				&&	(EqualSid (pOwnerSid, mOwner))
				)
			{
				lError = -1;
			}
			else
			{
				DWORD	lSidSize = GetLengthSid (pOwnerSid);
				PSID	lOwner = (PSID) LocalAlloc (LPTR, lSidSize);

				if	(
						(CopySid (lSidSize, lOwner, pOwnerSid))
					&&	(SetSecurityDescriptorOwner (mDescriptor, lOwner, FALSE))
					)
				{
					try
					{
						if	(mOwner != NULL)
						{
							LocalFree (mOwner);
						}
					}
					catch AnyExceptionSilent

					mOwner = lOwner;
					lError = ERROR_SUCCESS;
				}
				else
				{
					lError = GetLastError ();
					if	(lError == ERROR_SUCCESS)
					{
						lError = ERROR_INVALID_PARAMETER;
					}
					else
					{
						LogWinErr (LogNormal, lError);
					}
					LocalFree (lOwner);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::SetGroup (LPCTSTR pGroup)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(IsValid ())
		{
			CString				lUser = CUserSecurity::FormatUserId (pGroup);
			tArrayPtr <BYTE>	lSidBuffer;
			PSID				lSid = NULL;
			DWORD				lSidSize = 0;
			CString				lDomain;
			DWORD				lDomainSize = 0;
			SID_NAME_USE		lSidUse;

			LookupAccountName (NULL, lUser, NULL, &lSidSize, NULL, &lDomainSize, &lSidUse);

			if	(
					(lSidSize)
				&&	(lSidBuffer = new BYTE [lSidSize])
				&&	(lSid = (PSID) (LPBYTE) lSidBuffer)
				&&	(LookupAccountName (NULL, lUser, lSid, &lSidSize, lDomain.GetBuffer (lDomainSize), &lDomainSize, &lSidUse))
				)
			{
				if	(
						(lSidUse == SidTypeGroup)
					||	(lSidUse == SidTypeWellKnownGroup)
					||	(lSidUse == SidTypeAlias)
					)
				{
					lError = SetGroup (lSid);
				}
				else
				{
					lError = ERROR_INVALID_PARAMETER;
				}
			}
			else
			{
				lError = GetLastError ();
				if	(lError == ERROR_SUCCESS)
				{
					lError = ERROR_INVALID_PARAMETER;
				}
				else
				{
					LogWinErr (LogNormal, lError);
				}
				LogMessage (LogNormal, _T("Cannot find account [%s]"), lUser);
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

DWORD CSecurityDesc::SetGroup (PSID pGroupSid)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(
				(IsValid ())
			&&	(IsValidSid (pGroupSid))
			)
		{
			if	(
					(!IsAbsolute ())
				&&	(!MakeAbsolute ())
				)
			{
				lError = GetLastError ();
			}
			else
			if	(
					(mGroup != NULL)
				&&	(EqualSid (pGroupSid, mGroup))
				)
			{
				lError = -1;
			}
			else
			{
				DWORD	lSidSize = GetLengthSid (pGroupSid);
				PSID	lGroup = (PSID) LocalAlloc (LPTR, lSidSize);

				if	(
						(CopySid (lSidSize, lGroup, pGroupSid))
					&&	(SetSecurityDescriptorGroup (mDescriptor, lGroup, FALSE))
					)
				{
					try
					{
						if	(mGroup != NULL)
						{
							LocalFree (mGroup);
						}
					}
					catch AnyExceptionSilent

					mGroup = lGroup;
					lError = ERROR_SUCCESS;
				}
				else
				{
					lError = GetLastError ();
					if	(lError == ERROR_SUCCESS)
					{
						lError = ERROR_INVALID_PARAMETER;
					}
					else
					{
						LogWinErr (LogNormal, lError);
					}
					LocalFree (lGroup);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

DWORD CSecurityDesc::AccessCheck (ACCESS_MASK & pAccess, bool pThreadAccess, ACCESS_MASK pMask)
{
	DWORD	lError = ERROR_INVALID_DATA;

	try
	{
		if	(IsValid ())
		{
			CGenericHandle		lToken;
			tS<GENERIC_MAPPING>	lGenericMapping;
			tS<PRIVILEGE_SET>	lPrivilegeSet;
			DWORD				lPrivilegeSetLength = sizeof (lPrivilegeSet);
			DWORD				lAccessMask = 0;
			BOOL				lAccessStatus = FALSE;

			lGenericMapping.GenericRead = FILE_GENERIC_READ;
			lGenericMapping.GenericWrite = FILE_GENERIC_WRITE;
			lGenericMapping.GenericExecute = FILE_GENERIC_EXECUTE;
			lGenericMapping.GenericAll = FILE_ALL_ACCESS;

			if	(ImpersonateSelf (SecurityImpersonation))
			{
				try
				{
					if	(
							(pThreadAccess)
						?	(OpenThreadToken (GetCurrentThread (), TOKEN_QUERY, TRUE, &lToken))
						:	(OpenProcessToken (GetCurrentProcess (), TOKEN_QUERY, &lToken))
						)
					{
						try
						{
							if	(
									(::AccessCheck (mDescriptor, lToken, MAXIMUM_ALLOWED, &lGenericMapping, &lPrivilegeSet, &lPrivilegeSetLength, &lAccessMask, &lAccessStatus))
								&&	(lAccessStatus)
								)
							{
								pAccess = lAccessMask;
								lError = ERROR_SUCCESS;
							}
							else
							{
								lError = GetLastError ();
							}
						}
						catch AnyExceptionSilent

						lToken.Close ();
					}
					else
					{
						lError = GetLastError ();
					}
				}
				catch AnyExceptionSilent

				RevertToSelf ();
			}
			else
			{
				lError = GetLastError ();
			}
		}
	}
	catch AnyExceptionDebug

	return lError;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CSecurityDesc::DumpAccess (UINT pLogLevel, bool pLogNulls, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lFormat (pFormat);
			CString	lIndent;

			if	(!lFormat.IsEmpty ())
			{
				lFormat.TrimLeft ();

				if	(lFormat.IsEmpty ())
				{
					lIndent = pFormat;
				}
				else
				if	(lFormat.GetLength () < (int)_tcslen (pFormat))
				{
					lIndent = CString (_T(' '), (int)_tcslen (pFormat) - lFormat.GetLength ());
				}

				if	(!lFormat.IsEmpty ())
				{
					va_list 	lArgPtr;
					TCHAR		lMessage [2048];

					va_start (lArgPtr, pFormat);
					_vsntprintf (lMessage, sizeof (lMessage) / sizeof (TCHAR), pFormat, lArgPtr);
					LogMessage (pLogLevel, _T("%s"), lMessage);
				}
			}
			lIndent += _T("  ");

			if	(IsValid ())
			{
				DumpAccessDetails (pLogLevel, mDescriptor, mOwner, mGroup, mDAcl, mSAcl, pLogNulls, lIndent);
			}
			else
			{
				LogMessage (pLogLevel, _T("%sInvalid security descriptor [%p]"), lIndent, mDescriptor);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

void CSecurityDesc::DumpHandleAccess (UINT pLogLevel, HANDLE pHandle, SE_OBJECT_TYPE pHandleType, LPCTSTR pTitle, bool pLogNulls)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CThreadSecurity		lThreadSecurity;
			CSecurityDesc	lDescriptor;
			DWORD				lError;

			lThreadSecurity.GetSecurityPrivileges ();

			try
			{
				CString	lTitle (pTitle);
				CString	lIndent;

				if	(!lTitle.IsEmpty ())
				{
					lTitle.TrimLeft ();
					if	(lTitle.IsEmpty ())
					{
						lIndent = pTitle;
					}
					else
					if	(lTitle.GetLength () < (int)_tcslen (pTitle))
					{
						lIndent = CString (_T(' '), (int)_tcslen (pTitle) - lTitle.GetLength ());
					}
				}

				if	(!lTitle.IsEmpty ())
				{
					LogMessage (pLogLevel, _T("%sAccess for %s [%p]"), lIndent, lTitle, pHandle);
				}
				else
				{
					LogMessage (pLogLevel, _T("%sAccess for [%p]"), lIndent, pHandle);
				}
				lIndent += _T("  ");

				if	(
						((lError = GetSecurityInfo (pHandle, pHandleType, OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION|SACL_SECURITY_INFORMATION, &lDescriptor.mOwner, &lDescriptor.mGroup, &lDescriptor.mDAcl, &lDescriptor.mSAcl, &lDescriptor.mDescriptor)) == ERROR_SUCCESS)
					||	((lError = GetSecurityInfo (pHandle, pHandleType, OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION, &lDescriptor.mOwner, &lDescriptor.mGroup, &lDescriptor.mDAcl, NULL, &lDescriptor.mDescriptor)) == ERROR_SUCCESS)
					||	((lError = GetSecurityInfo (pHandle, pHandleType, OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION, &lDescriptor.mOwner, &lDescriptor.mGroup, NULL, NULL, &lDescriptor.mDescriptor)) == ERROR_SUCCESS)
					)
				{
					lDescriptor.DumpAccess (pLogLevel, pLogNulls, lIndent);
				}
				else
				{
					LogWinErr (pLogLevel, lError);
				}
			}
			catch AnyException
		}
		catch AnyException
	}
}

void CSecurityDesc::DumpObjectAccess (UINT pLogLevel, HANDLE pHandle, LPCTSTR pTitle, bool pLogNulls)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CThreadSecurity		lThreadSecurity;
			DWORD				lDescriptorSize = sizeof (SECURITY_DESCRIPTOR);
			tArrayPtr <BYTE>	lDescriptor = new BYTE [lDescriptorSize];

			lThreadSecurity.GetSecurityPrivileges ();

			try
			{
				CString					lTitle (pTitle);
				CString					lIndent;
				SECURITY_INFORMATION	lInfoType;
				PSID					lOwner = NULL;
				PSID					lGroup = NULL;
				PACL					lDAcl = NULL;
				PACL					lSAcl = NULL;
				BOOL					lPresent = FALSE;
				BOOL					lDefaulted = FALSE;

				if	(!lTitle.IsEmpty ())
				{
					lTitle.TrimLeft ();
					if	(lTitle.IsEmpty ())
					{
						lIndent = pTitle;
					}
					else
					if	(lTitle.GetLength () < (int)_tcslen (pTitle))
					{
						lIndent = CString (_T(' '), (int)_tcslen (pTitle) - lTitle.GetLength ());
					}
				}

				if	(pTitle)
				{
					LogMessage (pLogLevel, _T("%sAccess for %s [%p]"), lIndent, pTitle, pHandle);
				}
				else
				{
					LogMessage (pLogLevel, _T("%sAccess for [%p]"), lIndent, pHandle);
				}
				lIndent += _T("  ");

				if	(
						(
							(lInfoType = OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION|SACL_SECURITY_INFORMATION)
						&&	(!GetUserObjectSecurity (pHandle, &lInfoType, NULL, 0, &lDescriptorSize))
						&&	(lDescriptorSize > 0)
						&&	(lDescriptor = new BYTE [lDescriptorSize])
						&&	(GetUserObjectSecurity (pHandle, &lInfoType, (PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, lDescriptorSize, &lDescriptorSize))
						)
					||	(
							(lInfoType = OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION)
						&&	(!GetUserObjectSecurity (pHandle, &lInfoType, NULL, 0, &lDescriptorSize))
						&&	(lDescriptorSize > 0)
						&&	(lDescriptor = new BYTE [lDescriptorSize])
						&&	(GetUserObjectSecurity (pHandle, &lInfoType, (PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, lDescriptorSize, &lDescriptorSize))
						)
					||	(
							(lInfoType = OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION)
						&&	(!GetUserObjectSecurity (pHandle, &lInfoType, NULL, 0, &lDescriptorSize))
						&&	(lDescriptorSize > 0)
						&&	(lDescriptor = new BYTE [lDescriptorSize])
						&&	(GetUserObjectSecurity (pHandle, &lInfoType, (PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, lDescriptorSize, &lDescriptorSize))
						)
					)
				{
					if	(lInfoType & OWNER_SECURITY_INFORMATION)
					{
						GetSecurityDescriptorOwner ((PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, &lOwner, &lDefaulted);
					}
					if	(lInfoType & GROUP_SECURITY_INFORMATION)
					{
						GetSecurityDescriptorGroup ((PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, &lGroup, &lDefaulted);
					}
					if	(lInfoType & DACL_SECURITY_INFORMATION)
					{
						GetSecurityDescriptorDacl ((PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, &lPresent, &lDAcl, &lDefaulted);
					}
					if	(lInfoType & SACL_SECURITY_INFORMATION)
					{
						GetSecurityDescriptorSacl ((PISECURITY_DESCRIPTOR)(LPBYTE)lDescriptor, &lPresent, &lSAcl, &lDefaulted);
					}
					DumpAccessDetails (pLogLevel, lDescriptor, lOwner, lGroup, lDAcl, lSAcl, pLogNulls, lIndent);
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError ());
				}
			}
			catch AnyException
		}
		catch AnyException
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

static CString GetSidUseStr (SID_NAME_USE pSidUse)
{
	CString	lRet;

	switch (pSidUse)
	{
		case SidTypeUser:
		{
			lRet = _T("SidTypeUser");
		}	break;
		case SidTypeGroup:
		{
			lRet = _T("SidTypeGroup");
		}	break;
		case SidTypeDomain:
		{
			lRet = _T("SidTypeDomain");
		}	break;
		case SidTypeAlias:
		{
			lRet = _T("SidTypeAlias");
		}	break;
		case SidTypeWellKnownGroup:
		{
			lRet = _T("SidTypeWellKnownGroup");
		}	break;
		case SidTypeDeletedAccount:
		{
			lRet = _T("SidTypeDeletedAccount");
		}	break;
		case SidTypeInvalid:
		{
			lRet = _T("SidTypeInvalid");
		}	break;
		case SidTypeUnknown:
		{
			lRet = _T("SidTypeUnknown");
		}	break;
		case SidTypeComputer:
		{
			lRet = _T("SidTypeComputer");
		}	break;
		default:
		{
			lRet.Format (_T("(%u)"), pSidUse);
		}
	}

	return CString (lRet);
}

//////////////////////////////////////////////////////////////////////

void CSecurityDesc::DumpAclExplicit (UINT pLogLevel, PACL pAcl, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(!LogIsActive (pLogLevel))
	{
		return;
	}

	try
	{
		CString	lTitle (pTitle);
		CString	lIndent (pIndent);

		if	(lTitle.IsEmpty ())
		{
			lTitle = _T("Access Control");
		}

		try
		{
			tS <ACL_SIZE_INFORMATION>	lSizeInfo;
			ULONG						lNdx;

			if	(GetAclInformation (pAcl, &lSizeInfo, sizeof(lSizeInfo), AclSizeInformation))
			{
				LogMessage (pLogLevel, _T("%s%s Aces [%d]"), lIndent, lTitle, lSizeInfo.AceCount);

				for (lNdx = 0; lNdx < lSizeInfo.AceCount; lNdx++)
				{
					ACE_HEADER *	lAce = NULL;
					ACCESS_MASK		lAccessMask = 0;
					CString			lAccessStr;
					PSID			lSid = NULL;
					LPTSTR			lSidStr = NULL;
					CString			lSidName;

					if	(GetAce (pAcl, lNdx, (void**)&lAce))
					{
						switch (lAce->AceType)
						{
							case ACCESS_ALLOWED_ACE_TYPE:
							case ACCESS_ALLOWED_COMPOUND_ACE_TYPE:
							case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
							case ACCESS_ALLOWED_CALLBACK_ACE_TYPE:
							case ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE:
							{
								lAccessMask = ((PACCESS_ALLOWED_ACE) lAce)->Mask;
								lSid = (PSID)&(((PACCESS_ALLOWED_ACE) lAce)->SidStart);
							}	break;
							case ACCESS_DENIED_ACE_TYPE:
							case ACCESS_DENIED_OBJECT_ACE_TYPE:
							case ACCESS_DENIED_CALLBACK_ACE_TYPE:
							case ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE:
							{
								lAccessMask = ((PACCESS_DENIED_ACE) lAce)->Mask;
								lSid = (PSID)&(((PACCESS_DENIED_ACE) lAce)->SidStart);
							}	break;
							case SYSTEM_AUDIT_ACE_TYPE:
							case SYSTEM_AUDIT_OBJECT_ACE_TYPE:
							case SYSTEM_AUDIT_CALLBACK_ACE_TYPE:
							case SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE:
							{
								lAccessMask = ((PSYSTEM_AUDIT_ACE) lAce)->Mask;
								lSid = (PSID)&(((PSYSTEM_AUDIT_ACE) lAce)->SidStart);
							}	break;
							case SYSTEM_ALARM_ACE_TYPE:
							case SYSTEM_ALARM_OBJECT_ACE_TYPE:
							case SYSTEM_ALARM_CALLBACK_ACE_TYPE:
							case SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE:
							{
								lAccessMask = ((PSYSTEM_ALARM_ACE) lAce)->Mask;
								lSid = (PSID)&(((PSYSTEM_ALARM_ACE) lAce)->SidStart);
							}	break;
#ifdef	SYSTEM_MANDATORY_LABEL_ACE_TYPE
							case SYSTEM_MANDATORY_LABEL_ACE_TYPE:
							{
								lAccessMask = ((PSYSTEM_MANDATORY_LABEL_ACE) lAce)->Mask;
								lSid = (PSID)&(((PSYSTEM_MANDATORY_LABEL_ACE) lAce)->SidStart);
							}	break;
#endif
						}

#ifdef	SYSTEM_MANDATORY_LABEL_ACE_TYPE
						if	(lAce->AceType == SYSTEM_MANDATORY_LABEL_ACE_TYPE)
						{
							lAccessStr.Empty();

							if	(lAccessMask & SYSTEM_MANDATORY_LABEL_NO_WRITE_UP)
							{
								lAccessStr += _T(" NO_WRITE_UP");
							}
							if	(lAccessMask & SYSTEM_MANDATORY_LABEL_NO_READ_UP)
							{
								lAccessStr += _T(" NO_READ_UP");
							}
							if	(lAccessMask & SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP)
							{
								lAccessStr += _T(" NO_EXECUTE_UP");
							}
							lAccessStr.TrimLeft();
						}
						else
#endif
						{
							lAccessStr = AccessMaskStr (lAccessMask);
						}

						if	(
								(lSid)
							&&	(ConvertSidToStringSid (lSid, &lSidStr))
							)
						{
							lSidName = lSidStr;
							LocalFree (lSidStr);
						}

						LogMessage (pLogLevel, _T("%s  AceType [%2.2X] [%s] AceFlags [%2.2X] [%s] Access [%8.8X] [%s] Sid [%s]"), lIndent, lAce->AceType, AceTypeStr (lAce->AceType), lAce->AceFlags, AceFlagsStr (lAce->AceFlags), lAccessMask, lAccessStr, lSidName);
					}
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			ULONG				lEntryCount = 0;
			PEXPLICIT_ACCESS 	lEntries = NULL;
			ULONG				lNdx;

			if	(GetExplicitEntriesFromAcl (pAcl, &lEntryCount, &lEntries) == ERROR_SUCCESS)
			{
				LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);

				for	(lNdx = 0; lNdx < lEntryCount; lNdx++)
				{
					CString			lAccountName;
					DWORD			lAccountNameSize;
					CString			lDomainName;
					DWORD			lDomainNameSize;
					SID_NAME_USE	lSidUse;
					CString			lAccessMask;
					CString			lAccessMode;
					CString			lInherit;
					CString			lSidName;
					CString			lUseName;

					lAccessMode = AccessModeStr (lEntries [lNdx].grfAccessMode);
					lAccessMask = AccessMaskStr (lEntries [lNdx].grfAccessPermissions);
					lInherit = AccessInheritStr (lEntries [lNdx].grfInheritance);

#ifdef	_UNICODE
					if	(GetTrusteeForm (&lEntries [lNdx].Trustee) == TRUSTEE_IS_NAME)
					{
						lAccountName = GetTrusteeName (&lEntries [lNdx].Trustee);
					}
					else
					if	(GetTrusteeForm (&lEntries [lNdx].Trustee) == TRUSTEE_IS_SID)
#endif
					{
						LPTSTR	lSidStr = NULL;

						if	(LookupAccountSid (NULL, lEntries [lNdx].Trustee.ptstrName, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
						{
							lDomainName.ReleaseBuffer ();
							lAccountName.ReleaseBuffer ();
							lUseName = GetSidUseStr (lSidUse);
						}
#ifdef	_UNICODE
						else
						if	(ConvertSidToStringSid (lEntries [lNdx].Trustee.ptstrName, &lSidStr))
						{
							lAccountName = lSidStr;
							LocalFree (lSidStr);
							lSidStr = NULL;
						}

						if	(ConvertSidToStringSid (lEntries [lNdx].Trustee.ptstrName, &lSidStr))
						{
							lSidName = lSidStr;
							LocalFree (lSidStr);
							lSidStr = NULL;
						}
#endif
					}

					LogMessage (pLogLevel, _T("%s  Mode [%s] Account [%s\\%s] Access [%8.8X] [%s] Inherit [%s] AccountType [%s] Sid [%s]"), lIndent, lAccessMode, lDomainName, lAccountName, lEntries [lNdx].grfAccessPermissions, lAccessMask, lInherit, lUseName, lSidName);
				}
			}

			if	(lEntries)
			{
				LocalFree (lEntries);
			}
		}
		catch AnyExceptionSilent
	}
	catch AnyExceptionSilent
}

void CSecurityDesc::DumpAclEffective (UINT pLogLevel, PACL pAcl, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(!LogIsActive (pLogLevel))
	{
		return;
	}

	try
	{
		CString			lTitle (pTitle);
		CString			lIndent (pIndent);
		CString			lUserName;
		DWORD			lUserNameSize;
		tS <TRUSTEE>	lTrustee;
		DWORD			lAccessMask;
#ifdef	_UNICODE
		tSidPtr			lSid;
#endif
		if	(lTitle.IsEmpty ())
		{
			lTitle = _T("Effective Access");
		}
		LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);

		if	(GetUserName (lUserName.GetBuffer (UNLEN), &(lUserNameSize=UNLEN+1)))
		{
			lUserName.ReleaseBuffer ();
			BuildTrusteeWithName (&lTrustee, const_cast <LPTSTR> ((LPCTSTR) lUserName));

			if	(GetEffectiveRightsFromAcl (pAcl, &lTrustee, &(lAccessMask=0)) == ERROR_SUCCESS)
			{
				LogMessage (pLogLevel, _T("%s  %-24s [%s]"), lIndent, lUserName, AccessMaskStr (lAccessMask));
			}
		}

		lTrustee.Clear ();
#ifdef	_UNICODE
		BuildTrusteeWithSid (&lTrustee, CUserSecurity::SidEveryone (lSid));
#else
		BuildTrusteeWithName (&lTrustee, const_cast <LPTSTR> ((LPCTSTR) CUserSecurity::UserNameEveryone ()));
#endif
		if	(GetEffectiveRightsFromAcl (pAcl, &lTrustee, &(lAccessMask=0)) == ERROR_SUCCESS)
		{
			LogMessage (pLogLevel, _T("%s  %-24s [%s]"), lIndent, CUserSecurity::UserNameEveryone (), AccessMaskStr (lAccessMask));
		}

		lTrustee.Clear ();
#ifdef	_UNICODE
		BuildTrusteeWithSid (&lTrustee, CUserSecurity::SidAdministrators (lSid));
#else
		BuildTrusteeWithName (&lTrustee, const_cast <LPTSTR> ((LPCTSTR) CUserSecurity::UserNameAdministrators ()));
#endif
		if	(GetEffectiveRightsFromAcl (pAcl, &lTrustee, &(lAccessMask=0)) == ERROR_SUCCESS)
		{
			LogMessage (pLogLevel, _T("%s  %-24s [%s]"), lIndent, CUserSecurity::UserNameAdministrators (), AccessMaskStr (lAccessMask));
		}

#ifdef	_UNICODE
		lTrustee.Clear ();
		BuildTrusteeWithSid (&lTrustee, CUserSecurity::SidUsers (lSid));
		if	(GetEffectiveRightsFromAcl (pAcl, &lTrustee, &(lAccessMask=0)) == ERROR_SUCCESS)
		{
			LogMessage (pLogLevel, _T("%s  %-24s [%s]"), lIndent, CUserSecurity::UserNameUsers (), AccessMaskStr (lAccessMask));
		}
#endif
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CSecurityDesc::DumpAccessDetails (UINT pLogLevel, PSECURITY_DESCRIPTOR pDescriptor, PSID pOwner, PSID pGroup, PACL pDAcl, PACL pSAcl, bool pLogNulls, LPCTSTR pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (pIndent);

		try
		{
			CString			lAccountName;
			DWORD			lAccountNameSize;
			CString			lDomainName;
			DWORD			lDomainNameSize;
			SID_NAME_USE	lSidUse;

			if	(pOwner)
			{
				if	(LookupAccountSid (NULL, pOwner, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
				{
					lDomainName.ReleaseBuffer ();
					lAccountName.ReleaseBuffer ();
					LogMessage (pLogLevel, _T("%sOwner [%s\\%s] Type [%s] Sid [%s]"), lIndent, lDomainName, lAccountName, GetSidUseStr (lSidUse), CUserSecurity::GetSidStr (pOwner));
				}
				else
				{
					LogMessage (pLogLevel, _T("%sUnknown Owner [%s]"), lIndent, CUserSecurity::GetSidStr (pOwner));
				}
			}
			else
			if	(pLogNulls)
			{
				LogMessage (pLogLevel, _T("%sOwner unavailable"), lIndent);
			}

			if	(pGroup)
			{
				if	(LookupAccountSid (NULL, pGroup, lAccountName.GetBuffer (MAX_PATH), &(lAccountNameSize = MAX_PATH), lDomainName.GetBuffer (MAX_PATH), &(lDomainNameSize = MAX_PATH), &lSidUse))
				{
					lDomainName.ReleaseBuffer ();
					lAccountName.ReleaseBuffer ();
					LogMessage (pLogLevel, _T("%sGroup [%s\\%s] Type [%s] Sid [%s]"), lIndent, lDomainName, lAccountName, GetSidUseStr (lSidUse), CUserSecurity::GetSidStr (pGroup));
				}
				else
				{
					LogMessage (pLogLevel, _T("%sUnknown Group [%s]"), lIndent, CUserSecurity::GetSidStr (pGroup));
				}
			}
			else
			if	(pLogNulls)
			{
				LogMessage (pLogLevel, _T("%sGroup unavailable"), lIndent);
			}

			if	(pDAcl)
			{
				DumpAclExplicit (pLogLevel, pDAcl, _T("Discretionary Access"), lIndent);
				DumpAclEffective (pLogLevel, pDAcl, _T("Effective Access"), lIndent);
			}
			else
			if	(pLogNulls)
			{
				LogMessage (pLogLevel, _T("%sNo Discretionary Access"), lIndent);
			}

			if	(pSAcl)
			{
				DumpAclExplicit (pLogLevel, pSAcl, _T("System Access"), lIndent);
			}
			else
			if	(pLogNulls)
			{
				LogMessage (pLogLevel, _T("%sNo System Access"), lIndent);
			}
		}
		catch AnyExceptionSilent

		if	(pDescriptor)
		{
			try
			{
				CGenericHandle		lThreadToken;
				tS<GENERIC_MAPPING>	lGenericMapping;
				tS<PRIVILEGE_SET>	lPrivilegeSet;
				DWORD				lPrivilegeSetLength = sizeof (lPrivilegeSet);
				DWORD				lAccessMask = 0;
				BOOL				lAccessStatus = FALSE;

				lGenericMapping.GenericRead = FILE_GENERIC_READ;
				lGenericMapping.GenericWrite = FILE_GENERIC_WRITE;
				lGenericMapping.GenericExecute = FILE_GENERIC_EXECUTE;
				lGenericMapping.GenericAll = FILE_ALL_ACCESS;

				if	(ImpersonateSelf (SecurityImpersonation))
				{
					try
					{
						if	(OpenThreadToken (GetCurrentThread (), TOKEN_QUERY, FALSE, &lThreadToken))
						{
							try
							{
								if	(
										(::AccessCheck (pDescriptor, lThreadToken, MAXIMUM_ALLOWED, &lGenericMapping, &lPrivilegeSet, &lPrivilegeSetLength, &lAccessMask, &lAccessStatus))
									&&	(lAccessStatus)
									)
								{
									LogMessage (pLogLevel, _T("%sAccessCheck [%s] [%8.8X] [%s]"), lIndent, CUserSecurity::GetUserId (), lAccessMask, AccessMaskStr(lAccessMask));
								}
								else
								{
									//LogWinErr (pLogLevel, GetLastError ());
								}
							}
							catch AnyExceptionSilent

							lThreadToken.Close ();
						}
						else
						{
							LogWinErr (pLogLevel, GetLastError ());
						}
					}
					catch AnyExceptionSilent

					RevertToSelf ();
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError ());
				}
			}
			catch AnyExceptionSilent
		}
	}
}

//////////////////////////////////////////////////////////////////////

void CSecurityDesc::DumpAccessDetails (UINT pLogLevel, LPCTSTR pDescriptor, bool pLogNulls, LPCTSTR pIndent)
{
	if	(
			(pDescriptor != NULL)
		&&	(LogIsActive (pLogLevel))
		)
	{
		try
		{
			CSecurityDesc	lDescriptor (pDescriptor);

			if	(lDescriptor.mDescriptor)
			{
				lDescriptor.DumpAccess (pLogLevel, pLogNulls, pIndent);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CSecurityDesc::AccessModeStr (int pAccessMode)
{
	CString	lAccessMode;

	switch (pAccessMode)
	{
		case GRANT_ACCESS:
		{
			lAccessMode = _T("GRANT_ACCESS");
		}	break;
		case SET_ACCESS:
		{
			lAccessMode = _T("SET_ACCESS");
		}	break;
		case DENY_ACCESS:
		{
			lAccessMode = _T("DENY_ACCESS");
		}	break;
		case REVOKE_ACCESS:
		{
			lAccessMode = _T("REVOKE_ACCESS");
		}	break;
		case SET_AUDIT_SUCCESS:
		{
			lAccessMode = _T("SET_AUDIT_SUCCESS");
		}	break;
		case SET_AUDIT_FAILURE:
		{
			lAccessMode = _T("SET_AUDIT_FAILURE");
		}	break;
		default:
		{
			lAccessMode.Format (_T("(%u)"), pAccessMode);
		}
	}
	return lAccessMode;
}

CString CSecurityDesc::AccessMaskStr (DWORD pAccessMask)
{
	CString	lAccessMask;

	if	((pAccessMask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)
	{
		lAccessMask += _T(" STANDARD_RIGHTS_ALL");
	}
	else
	if	((pAccessMask & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)
	{
		lAccessMask += _T(" STANDARD_RIGHTS_REQUIRED");
	}
	else
	if	((pAccessMask & STANDARD_RIGHTS_READ) == STANDARD_RIGHTS_READ)
	{
		lAccessMask += _T(" STANDARD_RIGHTS_READ");
	}
	else
	{
		if	(pAccessMask & DELETE)
		{
			lAccessMask += _T(" DELETE");
		}
		if	(pAccessMask & READ_CONTROL)
		{
			lAccessMask += _T(" READ_CONTROL");
		}
		if	(pAccessMask & WRITE_DAC)
		{
			lAccessMask += _T(" WRITE_DAC");
		}
		if	(pAccessMask & WRITE_OWNER)
		{
			lAccessMask += _T(" WRITE_OWNER");
		}
		if	(pAccessMask & SYNCHRONIZE)
		{
			lAccessMask += _T(" SYNCHRONIZE");
		}
	}

	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == SPECIFIC_RIGHTS_ALL)
	{
		lAccessMask += _T(" SPECIFIC_RIGHTS_ALL");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == (FILE_ALL_ACCESS & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_ALL_ACCESS");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == (FILE_GENERIC_READ & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_READ");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == (FILE_GENERIC_WRITE & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_WRITE");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == (FILE_GENERIC_EXECUTE & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_EXECUTE");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == ((FILE_GENERIC_READ|FILE_GENERIC_WRITE) & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_READ");
		lAccessMask += _T(" FILE_GENERIC_WRITE");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == ((FILE_GENERIC_READ|FILE_GENERIC_EXECUTE) & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_READ");
		lAccessMask += _T(" FILE_GENERIC_EXECUTE");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == ((FILE_GENERIC_WRITE|FILE_GENERIC_EXECUTE) & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_WRITE");
		lAccessMask += _T(" FILE_GENERIC_EXECUTE");
	}
	else
	if	((pAccessMask & SPECIFIC_RIGHTS_ALL) == ((FILE_GENERIC_READ|FILE_GENERIC_WRITE|FILE_GENERIC_EXECUTE) & SPECIFIC_RIGHTS_ALL))
	{
		lAccessMask += _T(" FILE_GENERIC_READ");
		lAccessMask += _T(" FILE_GENERIC_WRITE");
		lAccessMask += _T(" FILE_GENERIC_EXECUTE");
	}
	else
	{
		if	(pAccessMask & FILE_READ_DATA)
		{
			lAccessMask += _T(" FILE_READ_DATA");
		}
		if	(pAccessMask & FILE_WRITE_DATA)
		{
			lAccessMask += _T(" FILE_WRITE_DATA");
		}
		if	(pAccessMask & FILE_APPEND_DATA)
		{
			lAccessMask += _T(" FILE_APPEND_DATA");
		}
		if	(pAccessMask & FILE_READ_EA)
		{
			lAccessMask += _T(" FILE_READ_EA");
		}
		if	(pAccessMask & FILE_WRITE_EA)
		{
			lAccessMask += _T(" FILE_WRITE_EA");
		}
		if	(pAccessMask & FILE_EXECUTE)
		{
			lAccessMask += _T(" FILE_EXECUTE");
		}
		if	(pAccessMask & FILE_READ_ATTRIBUTES)
		{
			lAccessMask += _T(" FILE_READ_ATTRIBUTES");
		}
		if	(pAccessMask & FILE_WRITE_ATTRIBUTES)
		{
			lAccessMask += _T(" FILE_WRITE_ATTRIBUTES");
		}

		if	((pAccessMask & FILE_GENERIC_READ) == FILE_GENERIC_READ)
		{
			lAccessMask += _T(" FILE_GENERIC_READ");
		}
		if	((pAccessMask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE)
		{
			lAccessMask += _T(" FILE_GENERIC_WRITE");
		}
		if	((pAccessMask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE)
		{
			lAccessMask += _T(" FILE_GENERIC_EXECUTE");
		}
	}

	if	(pAccessMask & GENERIC_READ)
	{
		lAccessMask += _T(" GENERIC_READ");
	}
	if	(pAccessMask & GENERIC_WRITE)
	{
		lAccessMask += _T(" GENERIC_WRITE");
	}
	if	(pAccessMask & GENERIC_EXECUTE)
	{
		lAccessMask += _T(" GENERIC_EXECUTE");
	}
	if	(pAccessMask & GENERIC_ALL)
	{
		lAccessMask += _T(" GENERIC_ALL");
	}
	if	(pAccessMask & ACCESS_SYSTEM_SECURITY)
	{
		lAccessMask += _T(" ACCESS_SYSTEM_SECURITY");
	}

	lAccessMask.TrimLeft ();
	lAccessMask.TrimRight ();
	return lAccessMask;
}

CString CSecurityDesc::AccessInheritStr (DWORD pAccessInherit)
{
	CString	lAccessInherit;

	if	(pAccessInherit & OBJECT_INHERIT_ACE)
	{
		lAccessInherit += _T(" OBJECT_INHERIT_ACE");
	}
	if	(pAccessInherit & CONTAINER_INHERIT_ACE)
	{
		lAccessInherit += _T(" CONTAINER_INHERIT_ACE");
	}
	if	(pAccessInherit & NO_PROPAGATE_INHERIT_ACE)
	{
		lAccessInherit += _T(" NO_PROPAGATE_INHERIT_ACE");
	}
	if	(pAccessInherit & INHERIT_ONLY_ACE)
	{
		lAccessInherit += _T(" INHERIT_ONLY_ACE");
	}
	if	(pAccessInherit & INHERITED_ACE)
	{
		lAccessInherit += _T(" INHERITED_ACE");
	}

	lAccessInherit.TrimLeft ();
	lAccessInherit.TrimRight ();
	return lAccessInherit;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CSecurityDesc::AceTypeStr (BYTE pAceType)
{
	CString	lAceType;

	switch (pAceType)
	{
		case ACCESS_ALLOWED_ACE_TYPE:					lAceType = _T("ACCESS_ALLOWED"); break;
		case ACCESS_DENIED_ACE_TYPE:					lAceType = _T("ACCESS_DENIED"); break;
		case SYSTEM_AUDIT_ACE_TYPE:						lAceType = _T("SYSTEM_AUDIT"); break;
		case SYSTEM_ALARM_ACE_TYPE:						lAceType = _T("SYSTEM_ALARM"); break;
		case ACCESS_ALLOWED_COMPOUND_ACE_TYPE:			lAceType = _T("ACCESS_ALLOWED_COMPOUND"); break;
		case ACCESS_ALLOWED_OBJECT_ACE_TYPE:			lAceType = _T("ACCESS_ALLOWED_OBJECT"); break;
		case ACCESS_DENIED_OBJECT_ACE_TYPE:				lAceType = _T("ACCESS_DENIED_OBJECT"); break;
		case SYSTEM_AUDIT_OBJECT_ACE_TYPE:				lAceType = _T("SYSTEM_AUDIT_OBJECT"); break;
		case SYSTEM_ALARM_OBJECT_ACE_TYPE:				lAceType = _T("SYSTEM_ALARM_OBJECT"); break;
		case ACCESS_ALLOWED_CALLBACK_ACE_TYPE:			lAceType = _T("ACCESS_ALLOWED_CALLBACK"); break;
		case ACCESS_DENIED_CALLBACK_ACE_TYPE:			lAceType = _T("ACCESS_DENIED_CALLBACK"); break;
		case ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE:	lAceType = _T("ACCESS_ALLOWED_CALLBACK_OBJECT"); break;
		case ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE:	lAceType = _T("ACCESS_DENIED_CALLBACK_OBJECT"); break;
		case SYSTEM_AUDIT_CALLBACK_ACE_TYPE:			lAceType = _T("SYSTEM_AUDIT_CALLBACK"); break;
		case SYSTEM_ALARM_CALLBACK_ACE_TYPE:			lAceType = _T("SYSTEM_ALARM_CALLBACK"); break;
		case SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE:		lAceType = _T("SYSTEM_AUDIT_CALLBACK_OBJECT"); break;
		case SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE:		lAceType = _T("SYSTEM_ALARM_CALLBACK_OBJECT"); break;
#ifdef	SYSTEM_MANDATORY_LABEL_ACE_TYPE
		case SYSTEM_MANDATORY_LABEL_ACE_TYPE:			lAceType = _T("SYSTEM_MANDATORY_LABEL"); break;
#endif
		default:
		{
			lAceType.Format (_T("(%2.2X"), pAceType);
		}
	}

	return lAceType;
}

CString CSecurityDesc::AceFlagsStr (BYTE pAceFlags)
{
	CString	lAceFlags;

	if	(pAceFlags & OBJECT_INHERIT_ACE)
	{
		lAceFlags += _T(" OBJECT_INHERIT_ACE");
	}
	if	(pAceFlags & CONTAINER_INHERIT_ACE)
	{
		lAceFlags += _T(" CONTAINER_INHERIT_ACE");
	}
	if	(pAceFlags & NO_PROPAGATE_INHERIT_ACE)
	{
		lAceFlags += _T(" NO_PROPAGATE_INHERIT_ACE");
	}
	if	(pAceFlags & INHERIT_ONLY_ACE)
	{
		lAceFlags += _T(" INHERIT_ONLY_ACE");
	}
	if	(pAceFlags & INHERITED_ACE)
	{
		lAceFlags += _T(" INHERITED_ACE");
	}
	if	(pAceFlags & SUCCESSFUL_ACCESS_ACE_FLAG)
	{
		lAceFlags += _T(" SUCCESSFUL_ACCESS_ACE");
	}
	if	(pAceFlags & FAILED_ACCESS_ACE_FLAG)
	{
		lAceFlags += _T(" FAILED_ACCESS_ACE");
	}

	lAceFlags.TrimLeft ();
	lAceFlags.TrimRight ();
	return lAceFlags;
}
