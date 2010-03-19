/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaControl.h"
#include "DaControlObj.h"

class ATL_NO_VTABLE __declspec(uuid("{1147E538-A208-11DE-ABF2-002421116FB2}")) CDaCtlSpeechInput :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlSpeechInput, &__uuidof(DaCtlSpeechInput)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSpeechInput), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlSpeechInput, &__uuidof(IDaCtlSpeechInput), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlSpeechInput();
	~CDaCtlSpeechInput();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSPEECHINPUT)
	DECLARE_NOT_AGGREGATABLE(CDaCtlSpeechInput)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlSpeechInput)
		COM_INTERFACE_ENTRY(IDaCtlSpeechInput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSpeechInput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlSpeechInput), IDaCtlSpeechInput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlSpeechInput)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlSpeechInput
	STDMETHOD(get_Enabled)(VARIANT_BOOL * VoiceEnabled);;
	STDMETHOD(get_Language)(BSTR * Language);;
	STDMETHOD(get_HotKey)(BSTR * HotKey);;
	STDMETHOD(get_Installed)(VARIANT_BOOL * VoiceInstalled);;
	STDMETHOD(get_Engine)(BSTR * Engine);;
	STDMETHOD(put_Engine)(BSTR Engine);;
	STDMETHOD(get_ListeningTip)(VARIANT_BOOL * ListeningTip);;

// Attributes
public:

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (CDaControlObj * pOwner);
	CDaControlObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Implementation
public:
	IDaSvrSpeechInputPropertiesPtr	mServerObject;
private:
	CDaControlObj *					mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSpeechInput), CDaCtlSpeechInput)

/////////////////////////////////////////////////////////////////////////////
