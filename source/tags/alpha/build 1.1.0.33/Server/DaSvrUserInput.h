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
#include "DaCmnUserInput.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E511-A208-11DE-ABF2-002421116FB2}")) DaSvrUserInput :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrUserInput, &__uuidof(DaSvrUserInput)>,
	public IDispatchImpl<IDaSvrUserInput2, &__uuidof(IDaSvrUserInput2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrUserInput), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CDaCmnUserInput,
	public CSvrObjLifetime
{
public:
	DaSvrUserInput ();
	virtual ~DaSvrUserInput ();

// Attributes
public:

// Operations
public:
	static DaSvrUserInput * CreateInstance (interface ISpRecoResult * pRecoResult, bool pGlobalCommand, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRUSERINPUT)
	DECLARE_NOT_AGGREGATABLE(DaSvrUserInput)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrUserInput)
		COM_INTERFACE_ENTRY(IDaSvrUserInput2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrUserInput2)
		COM_INTERFACE_ENTRY2(IDaSvrUserInput, IDaSvrUserInput2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentUserInput), IDaSvrUserInput2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(IDaSvrUserInput2)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrUserInput2
	HRESULT STDMETHODCALLTYPE GetCount (long *Count);
	HRESULT STDMETHODCALLTYPE GetItemID (long ItemIndex, long *CommandID);
	HRESULT STDMETHODCALLTYPE GetItemConfidence (long ItemIndex, long *Confidence);
	HRESULT STDMETHODCALLTYPE GetItemText (long ItemIndex, BSTR *Text);
	HRESULT STDMETHODCALLTYPE GetAllItemData (VARIANT *ItemIndices, VARIANT *ItemConfidences, VARIANT *ItemText);

	HRESULT STDMETHODCALLTYPE get_Count (long *Count);
	HRESULT STDMETHODCALLTYPE get_ItemCommandID (long ItemIndex, long *ItemCommandID);
	HRESULT STDMETHODCALLTYPE get_ItemConfidence (long ItemIndex, long *ItemConfidence);
	HRESULT STDMETHODCALLTYPE get_ItemText (long ItemIndex, BSTR *ItemText);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrUserInput), DaSvrUserInput)

/////////////////////////////////////////////////////////////////////////////
