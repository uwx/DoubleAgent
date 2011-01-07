/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
#if !defined(AFX_GUIDSTR_H__BB8DC7EF_0FB8_4729_83DD_69369CBCE88E__INCLUDED_)
#define AFX_GUIDSTR_H__BB8DC7EF_0FB8_4729_83DD_69369CBCE88E__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CGuidStr
{
public:
	CGuidStr (REFGUID pGuid);
	~CGuidStr () {}

	operator CString &() {return mValue;}
	operator const CString &() const {return mValue;}
	operator LPCTSTR () const {return mValue;}

	static GUID Parse (LPCTSTR pGuidStr);
	static CString GuidName (REFGUID pGuid);

private:
	CString	mValue;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GUIDSTR_H__BB8DC7EF_0FB8_4729_83DD_69369CBCE88E__INCLUDED_)
