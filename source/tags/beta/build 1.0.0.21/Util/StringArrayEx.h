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
#if !defined(AFX_STRINGARRAYEX_H__E3106ECA_0749_4B13_B520_84BFFC69DC1A__INCLUDED_)
#define AFX_STRINGARRAYEX_H__E3106ECA_0749_4B13_B520_84BFFC69DC1A__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CStringArrayEx : public CStringArray
{
public:
	CStringArrayEx () {}
	virtual ~CStringArrayEx () {}
	DECLARE_DYNAMIC(CStringArrayEx)

	friend INT_PTR AddUniqueString (CStringArray & pStrings, const CString & pString, bool pIgnoreCase = true);
	friend INT_PTR FindString (const CStringArray & pStrings, const CString & pFind, bool pIgnoreCase = true);
	friend INT_PTR RemoveString (CStringArray & pStrings, const CString & pFind, bool pAll = false, bool pIgnoreCase = true);

	friend void SortStrings (CStringArray & pStrings, bool pIgnoreCase = true);
	friend INT_PTR AddSortedString (CStringArray & pStrings, const CString & pString, bool pAllowDuplicates = false, bool pIgnoreCase = true);
	friend INT_PTR AddSortedStringReverse (CStringArray & pStrings, const CString & pString, bool pAllowDuplicates = false, bool pIgnoreCase = true);
	friend INT_PTR FindSortedString (const CStringArray & pStrings, const CString & pFind, bool pIgnoreCase = true);
	friend INT_PTR FindNextSortedString (const CStringArray & pStrings, const CString & pFind, INT_PTR pStartAfter = -1, bool pIgnoreCase = true);

	friend INT_PTR MakeStringArray (LPCTSTR pString, CStringArray & pStrings, LPCTSTR pDelimiters, bool pIncludeEmpty = false, bool pTrim = true, int pStringLength = -1);
	friend CString JoinStringArray (const CStringArray & pStrings, LPCTSTR pDelimiter, bool pIncludeEmpty = false, INT_PTR pFirstNdx = 0, INT_PTR pLastNdx = INT_MAX);

	friend INT_PTR ReplaceAll (CStringArray & pStrings, TCHAR pReplace, TCHAR pReplaceWith);
	friend INT_PTR ReplaceAll (CStringArray & pStrings, LPCTSTR pReplace, LPCTSTR pReplaceWith);

	friend bool CompareStringArrays (const CStringArray & pStrings1, const CStringArray & pStrings2, bool pIgnoreCase = true);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_STRINGARRAYEX_H__E3106ECA_0749_4B13_B520_84BFFC69DC1A__INCLUDED_)
