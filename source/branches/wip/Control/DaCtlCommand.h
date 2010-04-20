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
#include "DaControlMod.h"
#include "DaCtlCommands.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E535-A208-11DE-ABF2-002421116FB2}")) DaCtlCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCommand, &__uuidof(DaCtlCommand)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommand), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCommand2, &__uuidof(IDaCtlCommand2), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	DaCtlCommand();
	~DaCtlCommand();

// Attributes
public:
	IDaSvrCommand2Ptr	mServerObject;
	long				mServerId;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (DaCtlCharacter * pOwner);
	DaCtlCharacter * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCOMMAND)
	DECLARE_NOT_AGGREGATABLE(DaCtlCommand)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlCommand)
		COM_INTERFACE_ENTRY(IDaCtlCommand2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCommand2)
		COM_INTERFACE_ENTRY2(IDaCtlCommand, IDaCtlCommand2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommand), IDaCtlCommand2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommandEx), IDaCtlCommand2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCommand)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCommand2
	STDMETHOD(get_Voice)(BSTR * Voice);
	STDMETHOD(put_Voice)(BSTR Voice);
	STDMETHOD(get_Caption)(BSTR * Caption);
	STDMETHOD(put_Caption)(BSTR Caption);
	STDMETHOD(get_Enabled)(VARIANT_BOOL * Enabled);
	STDMETHOD(put_Enabled)(VARIANT_BOOL Enabled);
	STDMETHOD(get_Visible)(VARIANT_BOOL * Visible);
	STDMETHOD(put_Visible)(VARIANT_BOOL Visible);
	STDMETHOD(get_Confidence)(long * Confidence);
	STDMETHOD(put_Confidence)(long Confidence);
	STDMETHOD(get_ConfidenceText)(BSTR * Text);
	STDMETHOD(put_ConfidenceText)(BSTR Text);
	STDMETHOD(get_HelpContextID)(long * ID);
	STDMETHOD(put_HelpContextID)(long ID);
	STDMETHOD(get_VoiceCaption)(BSTR * Caption);
	STDMETHOD(put_VoiceCaption)(BSTR Caption);

	HRESULT STDMETHODCALLTYPE get_VoiceGrammar (BSTR *VoiceGrammar);
	HRESULT STDMETHODCALLTYPE put_VoiceGrammar (BSTR VoiceGrammar);
	HRESULT STDMETHODCALLTYPE get_ConfidenceThreshold (long *ConfidenceThreshold);
	HRESULT STDMETHODCALLTYPE put_ConfidenceThreshold (long ConfidenceThreshold);
	HRESULT STDMETHODCALLTYPE get_Name (BSTR *Name);

// Implementation
private:
	DaCtlCharacter *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCommand), DaCtlCommand)

/////////////////////////////////////////////////////////////////////////////
