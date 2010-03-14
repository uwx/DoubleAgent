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
#ifndef DAELEVATEDSETTINGS_H_INCLUDED_
#define DAELEVATEDSETTINGS_H_INCLUDED_
#pragma once

#include "RegistrySearch.h"
#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E566-A208-11DE-ABF2-002421116FB2}")) CDaElevatedSettings : public CCmdTarget, protected CRegistrySearch
{
	DECLARE_DYNCREATE(CDaElevatedSettings)
	DECLARE_OLECREATE_EX(CDaElevatedSettings)
protected:
	CDaElevatedSettings ();
public:
	virtual ~CDaElevatedSettings ();

// Attributes
public:

// Operations
public:
	HRESULT SetTreatAs (REFGUID pClassId, REFGUID pTreatAs);
	HRESULT SetTreatAs (HKEY pClassesRoot, REFGUID pClassId, REFGUID pTreatAs);

// Overrides
	//{{AFX_VIRTUAL(CDaElevatedSettings)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDaElevatedSettings)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(OleCommandTarget, IOleCommandTarget)
        HRESULT STDMETHODCALLTYPE QueryStatus (const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT *pCmdText);
        HRESULT STDMETHODCALLTYPE Exec (const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut);
	END_INTERFACE_PART(OleCommandTarget)

	DECLARE_INTERFACE_MAP()

public:
	static COleObjectFactoryEx & GetClassFactory() {return factory;}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAELEVATEDSETTINGS_H_INCLUDED_
