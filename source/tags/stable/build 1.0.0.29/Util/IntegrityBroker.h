/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009 Cinnamon Software Inc.
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
#ifndef INTEGRITYBROKER_H_INCLUDED_
#define INTEGRITYBROKER_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CIntegrityBroker
{
public:
	CIntegrityBroker ();
	~CIntegrityBroker ();

// Attributes

// Operations
	static bool IsProcessElevated (HANDLE pProcessHandle);
	static HANDLE MakeRestrictedToken (HANDLE pProcessHandle);
	static HANDLE StartRestrictedProcess (HANDLE pRestrictedToken, LPCTSTR pCommandLine);

// Overrides
	//{{AFX_VIRTUAL(CDaServerApp)
	//}}AFX_VIRTUAL

// Implementation
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // INTEGRITYBROKER_H_INCLUDED_
