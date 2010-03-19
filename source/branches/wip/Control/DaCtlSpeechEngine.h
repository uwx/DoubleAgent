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

class ATL_NO_VTABLE CDaCtlSpeechEngine :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlSpeechEngine, &__uuidof(DaCtlSpeechEngine)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSpeechEngine), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlSpeechEngine, &__uuidof(IDaCtlSpeechEngine), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlSpeechEngine();
	~CDaCtlSpeechEngine();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSPEECHENGINE)
	DECLARE_NOT_AGGREGATABLE(CDaCtlSpeechEngine)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlSpeechEngine)
		COM_INTERFACE_ENTRY(IDaCtlSpeechEngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSpeechEngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlSpeechEngine)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlSpeechEngine
	STDMETHOD(get_TTSModeID)(BSTR * TTSModeID);
	STDMETHOD(get_DisplayName)(BSTR * DisplayName);
	STDMETHOD(get_Manufacturer)(BSTR * Manufacturer);
	STDMETHOD(GetVersion)(short * MajorVersion,  short * MinorVersion);
	STDMETHOD(get_Gender)(short * Gender);
	STDMETHOD(get_LanguageID)(long * LanguageID);
	STDMETHOD(get_LanguageName)(VARIANT_BOOL EnglishName,  BSTR * LanguageName);

// Attributes
public:
	IDaSvrSpeechEnginePtr	mServerObject;

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

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSpeechEngine), CDaCtlSpeechEngine)

/////////////////////////////////////////////////////////////////////////////
