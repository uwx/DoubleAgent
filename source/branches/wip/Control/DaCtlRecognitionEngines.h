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

class ATL_NO_VTABLE CDaCtlRecognitionEngines :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlRecognitionEngines, &__uuidof(DaCtlRecognitionEngines)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlRecognitionEngines), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlRecognitionEngines, &__uuidof(IDaCtlRecognitionEngines), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlRecognitionEngines();
	~CDaCtlRecognitionEngines();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLRECOGNITIONENGINES)
	DECLARE_NOT_AGGREGATABLE(CDaCtlRecognitionEngines)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlRecognitionEngines)
		COM_INTERFACE_ENTRY(IDaCtlRecognitionEngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlRecognitionEngines)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlRecognitionEngines)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlRecognitionEngines
	STDMETHOD(get_Item)(VARIANT Index,  IDaCtlRecognitionEngine * * RecognitionEngine);
	STDMETHOD(get_Count)(long * Value);
	STDMETHOD(get__NewEnum)(IUnknown * * Enum);

// Attributes
public:
	IDaSvrRecognitionEnginesPtr											mServerObject;
	CArrayEx <IDaCtlRecognitionEnginePtr, IDaCtlRecognitionEngine *>	mRecognitionEngines;

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

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlRecognitionEngines), CDaCtlRecognitionEngines)

/////////////////////////////////////////////////////////////////////////////
