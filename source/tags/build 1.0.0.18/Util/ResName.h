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
#if !defined(AFX_RESNAME_H__3A94287C_64F7_4D18_A059_CFCB66C09B18__INCLUDED_)
#define AFX_RESNAME_H__3A94287C_64F7_4D18_A059_CFCB66C09B18__INCLUDED_
#pragma once

#include "MallocPtr.h"
#include "AfxTemplEx.h"

//////////////////////////////////////////////////////////////////////

class CResName
{
public:
	CResName (WORD pResId, LPCTSTR pResType);
	CResName (LPCTSTR pResName, LPCTSTR pResType);
	CResName (const CResName & pSource);
	CResName & operator= (const CResName & pSource);
	virtual ~CResName () {}

	LPCTSTR	Type() const {return mType;}
	LPCTSTR Name() const {return mId ? MAKEINTRESOURCE (mId) : (LPCTSTR) mName;}
	WORD Id() const {return mId;}
	LPCTSTR DispName() const {return mName;}

private:
	LPCTSTR				mType;
	WORD				mId;
	tMallocPtr <TCHAR>	mName;
};

typedef COwnPtrArray <CResName> CResNameArray;

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RESNAME_H__3A94287C_64F7_4D18_A059_CFCB66C09B18__INCLUDED_)
