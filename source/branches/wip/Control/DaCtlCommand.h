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
#include "DaCtlCommands.h"

class ATL_NO_VTABLE __declspec(uuid("{1147E535-A208-11DE-ABF2-002421116FB2}")) CDaCtlCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlCommand, &__uuidof(DaCtlCommand)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommand), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCommand, &__uuidof(IDaCtlCommand), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlCommand();
	~CDaCtlCommand();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCOMMAND)
	DECLARE_NOT_AGGREGATABLE(CDaCtlCommand)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlCommand)
		COM_INTERFACE_ENTRY(IDaCtlCommand)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCommand)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommand), IDaCtlCommand)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommandEx), IDaCtlCommand)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlCommand)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCommand
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

// Attributes
public:
	IDaSvrCommandPtr	mServerObject;
	long				mServerId;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (CDaCtlCharacter * pOwner);
	CDaCtlCharacter * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Implementation
private:
	CDaCtlCharacter *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCommand), CDaCtlCommand)

/////////////////////////////////////////////////////////////////////////////
