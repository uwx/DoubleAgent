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
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E514-A208-11DE-ABF2-002421116FB2}")) DaSvrAudioOutput :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrAudioOutput, &__uuidof(DaSvrAudioOutput)>,
	public IDispatchImpl<IDaSvrAudioOutput, &__uuidof(IDaSvrAudioOutput), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrAudioOutput), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrAudioOutput ();
	virtual ~DaSvrAudioOutput ();

// Attributes
public:

// Operations
public:
	static DaSvrAudioOutput * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

	long GetAudioStatus ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRAUDIOOUTPUT)
	DECLARE_NOT_AGGREGATABLE(DaSvrAudioOutput)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrAudioOutput)
		COM_INTERFACE_ENTRY(IDaSvrAudioOutput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrAudioOutput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentAudioOutputProperties), IDaSvrAudioOutput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentAudioOutputPropertiesEx), IDaSvrAudioOutput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrAudioOutput)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrAudioOutput
	HRESULT STDMETHODCALLTYPE GetEnabled (long *Enabled);
	HRESULT STDMETHODCALLTYPE GetUsingSoundEffects (long *UsingSoundEffects);
	HRESULT STDMETHODCALLTYPE GetStatus (long *Status);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrAudioOutput), DaSvrAudioOutput)

/////////////////////////////////////////////////////////////////////////////
