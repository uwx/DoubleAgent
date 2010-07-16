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
#include "DaCmnSREngine.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E555-A208-11DE-ABF2-002421116FB2}")) DaCtlSREngine :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlSREngine, &__uuidof(DaCtlSREngine)>,
	public IDispatchImpl<IDaCtlSREngine, &__uuidof(IDaCtlSREngine), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSREngine), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISupportErrorInfo
{
public:
	DaCtlSREngine();
	~DaCtlSREngine();

// Attributes
public:
	IDaSvrSREnginePtr		mServerObject;
	tPtr <CDaCmnSREngine>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSRENGINE)
	DECLARE_NOT_AGGREGATABLE(DaCtlSREngine)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlSREngine)
		COM_INTERFACE_ENTRY(IDaCtlSREngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSREngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlSREngine)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlSREngine
	HRESULT STDMETHODCALLTYPE get_SRModeID (BSTR * SRModeID);
	HRESULT STDMETHODCALLTYPE get_DisplayName (BSTR * DisplayName);
	HRESULT STDMETHODCALLTYPE get_Manufacturer (BSTR * Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short * MajorVersion,  short * MinorVersion);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long * LanguageID);
	HRESULT STDMETHODCALLTYPE get_LanguageName (VARIANT_BOOL EnglishName,  BSTR * LanguageName);
	HRESULT STDMETHODCALLTYPE get_LanguageIDs (SAFEARRAY ** LanguageIDs);
	HRESULT STDMETHODCALLTYPE get_LanguageNames (VARIANT_BOOL EnglishNames, SAFEARRAY ** LanguageNames);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSREngine), DaCtlSREngine)

/////////////////////////////////////////////////////////////////////////////
