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
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E515-A208-11DE-ABF2-002421116FB2}")) DaSvrSpeechInput :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrSpeechInput, &__uuidof(DaSvrSpeechInput)>,
	public IDispatchImpl<IDaSvrSpeechInput, &__uuidof(IDaSvrSpeechInput), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrSpeechInput), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrSpeechInput ();
	virtual ~DaSvrSpeechInput ();

// Attributes
public:

// Operations
public:
	static DaSvrSpeechInput * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRSPEECHINPUT)
	DECLARE_NOT_AGGREGATABLE(DaSvrSpeechInput)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrSpeechInput)
		COM_INTERFACE_ENTRY(IDaSvrSpeechInput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrSpeechInput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentSpeechInputProperties), IDaSvrSpeechInput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrSpeechInput)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrSpeechInput
	HRESULT STDMETHODCALLTYPE GetInstalled (long *Installed);
	HRESULT STDMETHODCALLTYPE GetEnabled (long *Enabled);
	HRESULT STDMETHODCALLTYPE GetHotKey (BSTR *HotKey);
	HRESULT STDMETHODCALLTYPE GetLCID (unsigned long *LCIDCurrent);
	HRESULT STDMETHODCALLTYPE GetEngine (BSTR *Engine);
	HRESULT STDMETHODCALLTYPE SetEngine (BSTR Engine);
	HRESULT STDMETHODCALLTYPE GetListeningTip (long *ListeningTip);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrSpeechInput), DaSvrSpeechInput)

/////////////////////////////////////////////////////////////////////////////
