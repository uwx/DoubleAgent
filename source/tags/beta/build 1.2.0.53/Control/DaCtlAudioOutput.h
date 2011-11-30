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
#include "DaControl.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E537-A208-11DE-ABF2-002421116FB2}")) DaCtlAudioOutput :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlAudioOutput, &__uuidof(DaCtlAudioOutput)>,
	public IDispatchImpl<IDaCtlAudioOutput, &__uuidof(IDaCtlAudioOutput), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlAudioOutput), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlAudioOutput ();
	~DaCtlAudioOutput ();

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
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLAUDIOOUTPUT)
	DECLARE_NOT_AGGREGATABLE(DaCtlAudioOutput)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlAudioOutput)
		COM_INTERFACE_ENTRY(IDaCtlAudioOutput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlAudioOutput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlAudioObject), IDaCtlAudioOutput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlAudioObjectEx), IDaCtlAudioOutput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlAudioOutput)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlAudioOutput
	HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL * AudioEnabled);
	HRESULT STDMETHODCALLTYPE get_SoundEffects (VARIANT_BOOL * SoundEffects);
	HRESULT STDMETHODCALLTYPE get_Status (short * Available);

// Implementation
public:
#ifndef	_DACORE_LOCAL
	IDaSvrAudioOutputPtr	mServerObject;
#endif
private:
	DaControl *				mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlAudioOutput), DaCtlAudioOutput)

/////////////////////////////////////////////////////////////////////////////
