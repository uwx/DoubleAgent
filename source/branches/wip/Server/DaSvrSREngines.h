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
#include "DaServerApp.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E522-A208-11DE-ABF2-002421116FB2}")) DaSvrSREngines :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrSREngines, &__uuidof(DaSvrSREngines)>,
	public IDispatchImpl<IDaSvrSREngines, &__uuidof(IDaSvrSREngines), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrSREngines), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrSREngines ();
	virtual ~DaSvrSREngines ();

// Attributes
public:
	CAtlPtrTypeArray <class CSapi5InputInfo>	mSapi5Inputs;

// Operations
public:
	static DaSvrSREngines * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

	void UseAllInputs ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRSRENGINES)
	DECLARE_NOT_AGGREGATABLE(DaSvrSREngines)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrSREngines)
		COM_INTERFACE_ENTRY(IDaSvrSREngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrSREngines)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrSREngines)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrSREngines
	HRESULT STDMETHODCALLTYPE get_Item (long Index, IDaSvrSREngine **SREngine);
	HRESULT STDMETHODCALLTYPE get_Count (long *Count);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown **ppunkEnum);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrSREngines), DaSvrSREngines)

/////////////////////////////////////////////////////////////////////////////
