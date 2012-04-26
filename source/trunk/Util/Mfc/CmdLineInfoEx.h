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
#if !defined(AFX_CMDLINEINFOEX_H__862801F9_7E13_4285_ABD7_6C5CC079D652__INCLUDED_)
#define AFX_CMDLINEINFOEX_H__862801F9_7E13_4285_ABD7_6C5CC079D652__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CCmdLineInfoEx : public CCommandLineInfo
{
public:
	CCmdLineInfoEx ();
	virtual ~CCmdLineInfoEx ();
	DECLARE_DYNAMIC (CCmdLineInfoEx)

// Attributes
	UINT	mLogLevelDebug;

// Operations
	bool ParseOpenRecent (CRecentFileList * pRecent, bool pDefaultFileNew = false, bool pDefaultFileOpen = false, bool pDefaultOpenRecent = false);
	bool RequiresOpenDlg () const;

	static int SplitCmdLine (CStringArray& pCmdLineParts); // for quoted paths __targv is unreliable, as is CommandLineToArgvW
	static int SplitCmdLine (LPCTSTR pCmdLine, CStringArray& pCmdLineParts);

// Overrides
	//{{AFX_VIRTUAL(CCmdLineInfoEx)
	public:
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
	//}}AFX_VIRTUAL
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CMDLINEINFOEX_H__862801F9_7E13_4285_ABD7_6C5CC079D652__INCLUDED_)
