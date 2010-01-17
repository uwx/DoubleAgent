/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#ifndef MSCONTROL_H_INCLUDED_
#define MSCONTROL_H_INCLUDED_
#pragma once

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

/////////////////////////////////////////////////////////////////////////////

class CMsControlApp : public CWinApp
{
public:
	CMsControlApp ();
	virtual ~CMsControlApp ();
	DECLARE_DYNAMIC(CMsControlApp);

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CMsControlApp)
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // MSCONTROL_H_INCLUDED_
