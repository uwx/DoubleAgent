/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaServerApp.h"
#include "RegistrySearch.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E566-A208-11DE-ABF2-002421116FB2}")) CDaElevatedSettings :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaServer, &__uuidof(CDaElevatedSettings)>,
	public IOleCommandTarget,
	protected CRegistrySearch
{
public:
	CDaElevatedSettings ();
	virtual ~CDaElevatedSettings ();

// Attributes
public:

// Operations
public:
	static HRESULT WINAPI UpdateRegistryOverride (BOOL bRegister);

	HRESULT SetTreatAs (REFGUID pClassId, REFGUID pTreatAs);
	HRESULT SetTreatAs (HKEY pClassesRoot, REFGUID pClassId, REFGUID pTreatAs);

// Overrides

// Declarations
protected:
	DECLARE_REGISTRY_RESOURCEID(IDR_DAEMULATION)
	DECLARE_NOT_AGGREGATABLE(CDaElevatedSettings)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaElevatedSettings)
		COM_INTERFACE_ENTRY(IOleCommandTarget)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaElevatedSettings)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	END_CATEGORY_MAP()

// Interfaces
public:
	// IOleCommandTarget
    HRESULT STDMETHODCALLTYPE QueryStatus (const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT *pCmdText);
    HRESULT STDMETHODCALLTYPE Exec (const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut);
};

/////////////////////////////////////////////////////////////////////////////

#define	UpdateRegistry	UpdateRegistryOverride
OBJECT_ENTRY_AUTO(__uuidof(CDaElevatedSettings), CDaElevatedSettings)
#undef	UpdateRegistry

/////////////////////////////////////////////////////////////////////////////
