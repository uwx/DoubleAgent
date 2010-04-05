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
#include "DaControl.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E555-A208-11DE-ABF2-002421116FB2}")) DaCtlRecognitionEngine :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlRecognitionEngine, &__uuidof(DaCtlRecognitionEngine)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlRecognitionEngine), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlRecognitionEngine, &__uuidof(IDaCtlRecognitionEngine), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	DaCtlRecognitionEngine();
	~DaCtlRecognitionEngine();

// Attributes
public:
	IDaSvrRecognitionEnginePtr	mServerObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLRECOGNITIONENGINE)
	DECLARE_NOT_AGGREGATABLE(DaCtlRecognitionEngine)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlRecognitionEngine)
		COM_INTERFACE_ENTRY(IDaCtlRecognitionEngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlRecognitionEngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlRecognitionEngine)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlRecognitionEngine
	STDMETHOD(get_SRModeID)(BSTR * SRModeID);
	STDMETHOD(get_DisplayName)(BSTR * DisplayName);
	STDMETHOD(get_Manufacturer)(BSTR * Manufacturer);
	STDMETHOD(GetVersion)(short * MajorVersion,  short * MinorVersion);
	STDMETHOD(get_LanguageID)(long * LanguageID);
	STDMETHOD(get_LanguageName)(VARIANT_BOOL EnglishName,  BSTR * LanguageName);
	STDMETHOD(get_LanguageIDs)(SAFEARRAY * * LanguageIDs);
	STDMETHOD(get_LanguageNames)(VARIANT_BOOL EnglishNames, SAFEARRAY * * LanguageNames);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlRecognitionEngine), DaCtlRecognitionEngine)

/////////////////////////////////////////////////////////////////////////////
