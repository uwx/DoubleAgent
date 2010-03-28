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

class ATL_NO_VTABLE __declspec(uuid("{1147E554-A208-11DE-ABF2-002421116FB2}")) CDaCtlSpeechEngines :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlSpeechEngines, &__uuidof(DaCtlSpeechEngines)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSpeechEngines), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlSpeechEngines, &__uuidof(IDaCtlSpeechEngines), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlSpeechEngines();
	~CDaCtlSpeechEngines();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSPEECHENGINES)
	DECLARE_NOT_AGGREGATABLE(CDaCtlSpeechEngines)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlSpeechEngines)
		COM_INTERFACE_ENTRY(IDaCtlSpeechEngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSpeechEngines)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlSpeechEngines)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlSpeechEngines
	STDMETHOD(get_Item)(VARIANT Index,  IDaCtlSpeechEngine * * SpeechEngine);
	STDMETHOD(get_Count)(long * Value);
	STDMETHOD(get__NewEnum)(IUnknown * * Enum);

// Attributes
public:
	IDaSvrSpeechEnginesPtr					mServerObject;
	CInterfaceArray <IDaCtlSpeechEngine>	mSpeechEngines;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (CDaControlObj * pOwner);
	CDaControlObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Implementation
private:
	CDaControlObj *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSpeechEngines), CDaCtlSpeechEngines)

/////////////////////////////////////////////////////////////////////////////
