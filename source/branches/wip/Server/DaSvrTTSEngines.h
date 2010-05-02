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
#include "DaCmnTTSEngines.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E520-A208-11DE-ABF2-002421116FB2}")) DaSvrTTSEngines :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrTTSEngines, &__uuidof(DaSvrTTSEngines)>,
	public IDispatchImpl<IDaSvrTTSEngines, &__uuidof(IDaSvrTTSEngines), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrTTSEngines), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CDaCmnTTSEngines,
	public CSvrObjLifetime
{
public:
	DaSvrTTSEngines ();
	virtual ~DaSvrTTSEngines ();

// Attributes
public:

// Operations
public:
	static DaSvrTTSEngines * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRTTSENGINES)
	DECLARE_NOT_AGGREGATABLE(DaSvrTTSEngines)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrTTSEngines)
		COM_INTERFACE_ENTRY(IDaSvrTTSEngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrTTSEngines)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloon), IDaSvrTTSEngines)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloonEx), IDaSvrTTSEngines)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrTTSEngines)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrTTSEngines
	HRESULT STDMETHODCALLTYPE get_Item (long Index, IDaSvrTTSEngine **TTSEngine);
	HRESULT STDMETHODCALLTYPE get_Count (long *Count);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown **ppunkEnum);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrTTSEngines), DaSvrTTSEngines)

/////////////////////////////////////////////////////////////////////////////
