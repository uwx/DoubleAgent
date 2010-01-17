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
#if !defined(AFX_ERRORINFO_H__2B33B284_306C_4f28_BC6D_999978D7599A__INCLUDED_)
#define AFX_ERRORINFO_H__2B33B284_306C_4f28_BC6D_999978D7599A__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CErrorInfo
{
public:
	CErrorInfo ();
	CErrorInfo (LPCTSTR pDescription, LPCTSTR pSource = NULL, const GUID & pGuid = GUID_NULL, LPCTSTR pHelpFile = NULL, DWORD pHelpContext = 0);
	CErrorInfo (HRESULT pError, LPCTSTR pSource = NULL, const GUID & pGuid = GUID_NULL, LPCTSTR pHelpFile = NULL, DWORD pHelpContext = 0);
	~CErrorInfo () {}

// Attributes
	GUID	mGuid;
	CString	mDescription;
	CString	mSource;
	CString	mHelpFile;
	DWORD	mHelpContext;

// Operations
	HRESULT GetErrorInfo ();
	HRESULT	PutErrorInfo ();

	static bool HasErrorInfo ();
	static void ClearErrorInfo ();

// Implementation
public:
	static CString GetSysDescription (HRESULT pError);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ERRORINFO_H__2B33B284_306C_4f28_BC6D_999978D7599A__INCLUDED_)
