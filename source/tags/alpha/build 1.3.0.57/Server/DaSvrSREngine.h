/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include "DaServerApp.h"
#include "DaCmnSREngine.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E521-A208-11DE-ABF2-002421116FB2}")) DaSvrSREngine :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrSREngine, &__uuidof(DaSvrSREngine)>,
	public IDispatchImpl<IDaSvrSREngine, &__uuidof(IDaSvrSREngine), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrSREngine), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CDaCmnSREngine,
	public CSvrObjLifetime
{
public:
	DaSvrSREngine ();
	virtual ~DaSvrSREngine ();

// Attributes
public:

// Operations
public:
	static DaSvrSREngine * CreateInstance (class CSapi5InputInfo* pInputInfo, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRSRENGINE)
	DECLARE_NOT_AGGREGATABLE(DaSvrSREngine)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrSREngine)
		COM_INTERFACE_ENTRY(IDaSvrSREngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrSREngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrSREngine)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrSREngine
	HRESULT STDMETHODCALLTYPE get_SRModeID (BSTR *SRModeID);
	HRESULT STDMETHODCALLTYPE get_DisplayName (BSTR *DisplayName);
	HRESULT STDMETHODCALLTYPE get_Manufacturer (BSTR *Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName);
	HRESULT STDMETHODCALLTYPE get_LanguageIDs (SAFEARRAY**LanguageIds);
	HRESULT STDMETHODCALLTYPE get_LanguageNames (VARIANT_BOOL EnglishNames, SAFEARRAY**LanguageNames);

// Implementation
protected:
	void Disconnect (bool pAbandonned);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrSREngine), DaSvrSREngine)

/////////////////////////////////////////////////////////////////////////////
