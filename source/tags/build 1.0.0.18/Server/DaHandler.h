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
#ifndef DAHANDLER_H_INCLUDED_
#define DAHANDLER_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CDaHandlerApp : public CWinApp
{
public:
	CDaHandlerApp();
	~CDaHandlerApp();
	DECLARE_DYNAMIC(CDaHandlerApp)

// Attributes
public:

// Operations
	void StartThreadLifetime ();
	void EndAllLifetimes ();

// Overrides
	//{{AFX_VIRTUAL(CDaHandlerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDaHandlerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	COwnPtrMap <CString, CMutex, LPCTSTR>	mLifetimeMutex;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAHANDLER_H_INCLUDED_
