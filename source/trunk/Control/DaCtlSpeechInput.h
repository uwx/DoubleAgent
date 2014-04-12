/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControlMod.h"
#include "DaControl.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E538-A208-11DE-ABF2-002421116FB2}")) DaCtlSpeechInput :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlSpeechInput, &__uuidof(DaCtlSpeechInput)>,
	public IDispatchImpl<IDaCtlSpeechInput, &__uuidof(IDaCtlSpeechInput), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSpeechInput), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlSpeechInput();
	~DaCtlSpeechInput();

// Attributes
public:

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);
	void Disconnect (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSPEECHINPUT)
	DECLARE_NOT_AGGREGATABLE(DaCtlSpeechInput)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlSpeechInput)
		COM_INTERFACE_ENTRY(IDaCtlSpeechInput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSpeechInput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlSpeechInput), IDaCtlSpeechInput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlSpeechInput)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlSpeechInput
	HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL * VoiceEnabled);;
	HRESULT STDMETHODCALLTYPE get_Language (BSTR * Language);;
	HRESULT STDMETHODCALLTYPE get_HotKey (BSTR * HotKey);;
	HRESULT STDMETHODCALLTYPE get_Installed (VARIANT_BOOL * VoiceInstalled);;
	HRESULT STDMETHODCALLTYPE get_Engine (BSTR * Engine);;
	HRESULT STDMETHODCALLTYPE put_Engine (BSTR Engine);;
	HRESULT STDMETHODCALLTYPE get_ListeningTip (VARIANT_BOOL * ListeningTip);;

// Implementation
public:
#ifndef	_DACORE_LOCAL
	IDaSvrSpeechInputPtr	mServerObject;
#endif
private:
	DaControl *				mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSpeechInput), DaCtlSpeechInput)

/////////////////////////////////////////////////////////////////////////////
