/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaCmnCommand.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E535-A208-11DE-ABF2-002421116FB2}")) DaCtlCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCommand, &__uuidof(DaCtlCommand)>,
	public IDispatchImpl<IDaCtlCommand2, &__uuidof(IDaCtlCommand2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommand), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlCommand();
	~DaCtlCommand();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrCommand2Ptr		mServerObject;
#endif
	long					mServerId;
	tPtr <CDaCmnCommand>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaCtlCharacter * pOwner);
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
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlCommand2
	HRESULT STDMETHODCALLTYPE get_Voice (BSTR * Voice);
	HRESULT STDMETHODCALLTYPE put_Voice (BSTR Voice);
	HRESULT STDMETHODCALLTYPE get_Caption (BSTR * Caption);
	HRESULT STDMETHODCALLTYPE put_Caption (BSTR Caption);
	HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL * Enabled);
	HRESULT STDMETHODCALLTYPE put_Enabled (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL * Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_Confidence (long* Confidence);
	HRESULT STDMETHODCALLTYPE put_Confidence (long Confidence);
	HRESULT STDMETHODCALLTYPE get_ConfidenceText (BSTR * Text);
	HRESULT STDMETHODCALLTYPE put_ConfidenceText (BSTR Text);
	HRESULT STDMETHODCALLTYPE get_HelpContextID (long* ID);
	HRESULT STDMETHODCALLTYPE put_HelpContextID (long ID);
	HRESULT STDMETHODCALLTYPE get_VoiceCaption (BSTR * Caption);
	HRESULT STDMETHODCALLTYPE put_VoiceCaption (BSTR Caption);

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
