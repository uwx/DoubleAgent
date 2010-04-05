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
#pragma once
#include "DaGuid.h"
#include "DaShellRes.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E565-A208-11DE-ABF2-002421116FB2}")) CDaCharacterProps :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IObjectWithSiteImpl<CDaCharacterProps>,
	public IShellExtInit,
	public IShellPropSheetExt,
	public CComCoClass<CDaCharacterProps, &__uuidof(CDaCharacterProps)>
{
protected:
	CDaCharacterProps ();
public:
	virtual ~CDaCharacterProps ();
	static CDaCharacterProps * CreateInstance ();

// Attributes
public:

// Operations
public:
	static HRESULT WINAPI UpdateRegistryOverride (BOOL bRegister);
	void FinalRelease();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACHARACTERPROPS)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCharacterProps)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IShellPropSheetExt)
		COM_INTERFACE_ENTRY(IObjectWithSite)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCharacterProps)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	END_CATEGORY_MAP()

// Interfaces
public:
	// IObjectWithSite
    STDMETHOD (SetSite) (IUnknown *pUnkSite);
    STDMETHOD (GetSite) (REFIID riid, void **ppvSite);

	// IShellExtInit
    STDMETHOD (Initialize) (LPCITEMIDLIST pFolderId, IDataObject * pDataObject, HKEY pProgIdKey);

	// IShellPropSheetExt
    STDMETHOD (AddPages) (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam);
    STDMETHOD (ReplacePage) (EXPPS pPageID, LPFNSVADDPROPSHEETPAGE pReplaceProc, LPARAM pLparam);

// Implementation
protected:
	IUnknownPtr				mExplorerInstance;
	IUnknownPtr				mSite;
	tPtr <class CAgentFile>	mAgentFile;
};

/////////////////////////////////////////////////////////////////////////////

#define	UpdateRegistry	UpdateRegistryOverride
OBJECT_ENTRY_AUTO(__uuidof(CDaCharacterProps), CDaCharacterProps)
#undef	UpdateRegistry

/////////////////////////////////////////////////////////////////////////////
