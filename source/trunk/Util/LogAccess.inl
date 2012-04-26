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
#ifndef	_LOG_DISABLED
#ifdef	_UNICODE
#include <aclapi.h>
/////////////////////////////////////////////////////////////////////////////
class CLogAccess
{
public:
	PSECURITY_ATTRIBUTES GetSecurity ()
	{
		if	(mSecurity.nLength)
		{
			return &mSecurity;
		}
		return NULL;
	}

	CLogAccess ()
	:	mDescriptor (NULL),
		mAcl (NULL)
	{
		memset (&mSecurity, 0, sizeof (mSecurity));

		try
		{
			EXPLICIT_ACCESS	lAccess;

			if	(
					(mDescriptor = (PSECURITY_DESCRIPTOR *) LocalAlloc (LPTR, sizeof (SECURITY_DESCRIPTOR)))
				&&	(InitializeSecurityDescriptor (mDescriptor, SECURITY_DESCRIPTOR_REVISION))
				)
			{
				memset (&lAccess, 0, sizeof (lAccess));
				BuildExplicitAccessWithName (&lAccess, _T("Everyone"), STANDARD_RIGHTS_ALL | FILE_ALL_ACCESS, GRANT_ACCESS, NO_PROPAGATE_INHERIT_ACE);

				if	(SetEntriesInAcl (1, &lAccess, NULL, &mAcl) == ERROR_SUCCESS)
				{
					if	(
							(SetSecurityDescriptorGroup (mDescriptor, NULL, TRUE))
						&&	(SetSecurityDescriptorOwner (mDescriptor, NULL, TRUE))
						&&	(SetSecurityDescriptorDacl (mDescriptor, TRUE, mAcl, FALSE))
						&&	(SetSecurityDescriptorSacl (mDescriptor, FALSE, NULL, TRUE))
						)
					{
						mSecurity.nLength = sizeof (mSecurity);
						mSecurity.lpSecurityDescriptor = mDescriptor;
						mSecurity.bInheritHandle = FALSE;
					}
				}
			}

			if	(!mSecurity.nLength)
			{
				FreeMemory ();
			}
		}
		catch (...)
		{}
	}

	CLogAccess::~CLogAccess ()
	{
		FreeMemory ();
	}

private:
	void FreeMemory ()
	{
		try
		{
			if	(mDescriptor)
			{
				LocalFree (mDescriptor);
				mDescriptor = NULL;
			}
		}
		catch (...)
		{}
		try
		{
			if	(mAcl)
			{
				LocalFree (mAcl);
				mAcl = NULL;
			}
		}
		catch (...)
		{}
	}

private:
	SECURITY_ATTRIBUTES		mSecurity;
	PSECURITY_DESCRIPTOR	mDescriptor;
	PACL					mAcl;
};
/////////////////////////////////////////////////////////////////////////////
static CLogAccess gLogAccess;
#define	_LOG_FILE_SECURITY (gLogAccess.GetSecurity ())
/////////////////////////////////////////////////////////////////////////////
#endif	// _UNICODE
#endif	// _LOG_DISABLED
