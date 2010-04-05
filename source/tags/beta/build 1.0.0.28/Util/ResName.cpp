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
#include "ResName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CResName::CResName (WORD pResId, LPCTSTR pResType)
:	mType (pResType),
	mId (pResId)
{
	CString	lName;

	lName.Format (_T("#%hu"), pResId);
	mName.Attach (AfxAllocTaskString (lName));
}

CResName::CResName (LPCTSTR pResName, LPCTSTR pResType)
:	mType (pResType),
	mId (0)
{
	if	(pResName)
	{
		mName.Attach (AfxAllocTaskString (pResName));
	}
}

CResName::CResName (const CResName & pSource)
:	mType (pSource.mType),
	mId (pSource.mId)
{
	if	(pSource.mName)
	{
		mName.Attach (AfxAllocTaskString (pSource.mName));
	}
}

CResName & CResName::operator= (const CResName & pSource)
{
	mType = pSource.mType;
	mId = pSource.mId;
	mName.Free ();
	if	(pSource.mName)
	{
		mName.Attach (AfxAllocTaskString (pSource.mName));
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////
