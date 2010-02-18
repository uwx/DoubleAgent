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
#ifndef DACHARACTERPROPS_H_INCLUDED_
#define DACHARACTERPROPS_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E565-A208-11DE-ABF2-002421116FB2}")) CDaCharacterProps : public CCmdTarget
{
	DECLARE_DYNCREATE(CDaCharacterProps)
	DECLARE_OLECREATE_EX(CDaCharacterProps)
protected:
	CDaCharacterProps ();
public:
	virtual ~CDaCharacterProps ();

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaCharacterProps)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART (ObjectWithSite, IObjectWithSite)
        STDMETHOD (SetSite) (IUnknown *pUnkSite);
        STDMETHOD (GetSite) (REFIID riid, void **ppvSite);
	END_INTERFACE_PART (ObjectWithSite)

	BEGIN_INTERFACE_PART (ShellExtInit, IShellExtInit)
        STDMETHOD (Initialize) (LPCITEMIDLIST pFolderId, IDataObject * pDataObject, HKEY pProgIdKey);
	END_INTERFACE_PART (ShellExtInit)

	BEGIN_INTERFACE_PART (ShellPropSheetExt, IShellPropSheetExt)
        STDMETHOD (AddPages) (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam);
        STDMETHOD (ReplacePage) (EXPPS pPageID, LPFNSVADDPROPSHEETPAGE pReplaceProc, LPARAM pLparam);
	END_INTERFACE_PART (ShellPropSheetExt)

	DECLARE_INTERFACE_MAP()

protected:
	IUnknownPtr				mExplorerInstance;
	IUnknownPtr				mSite;
	tPtr <class CAgentFile>	mAgentFile;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACHARACTERPROPS_H_INCLUDED_
