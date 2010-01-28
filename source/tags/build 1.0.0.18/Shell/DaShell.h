/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef DASHELL_H_INCLUDED_
#define DASHELL_H_INCLUDED_
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "DaShellRes.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E564-A208-11DE-ABF2-002421116FB2}")) CDaShellApp : public CWinApp
{
public:
	CDaShellApp();
	~CDaShellApp();
	DECLARE_DYNAMIC(CDaShellApp)

// Attributes
public:
	tPtr <class CPropSheetCpl>	mCplPropSheet;
	CString						mCplStartPage;

// Operations
	static void RegisterCpl ();
	static void UnregisterCpl ();

// Overrides
	//{{AFX_VIRTUAL(CDaShellApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDaShellApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#define TheShellApp ((CDaShellApp *) AfxGetApp ())

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASHELL_H_INCLUDED_
