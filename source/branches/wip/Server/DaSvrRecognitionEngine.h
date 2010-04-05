/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E521-A208-11DE-ABF2-002421116FB2}")) DaSvrRecognitionEngine :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrRecognitionEngine, &__uuidof(DaSvrRecognitionEngine)>,
	public IDispatchImpl<IDaSvrRecognitionEngine, &__uuidof(IDaSvrRecognitionEngine), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrRecognitionEngine), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrRecognitionEngine ();
	virtual ~DaSvrRecognitionEngine ();

// Attributes
public:

// Operations
public:
	static DaSvrRecognitionEngine * CreateInstance (class CSapi5InputInfo * pInputInfo, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRRECOGNITIONENGINE)
	DECLARE_NOT_AGGREGATABLE(DaSvrRecognitionEngine)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrRecognitionEngine)
		COM_INTERFACE_ENTRY(IDaSvrRecognitionEngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrRecognitionEngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrRecognitionEngine)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrRecognitionEngine
	HRESULT STDMETHODCALLTYPE GetSRModeID (BSTR *SRModeID);
	HRESULT STDMETHODCALLTYPE GetDisplayName (BSTR *DisplayName);
	HRESULT STDMETHODCALLTYPE GetManufacturer (BSTR *Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT STDMETHODCALLTYPE GetLanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE GetLanguageName (BSTR *LanguageName, boolean EnglishName = TRUE);
	HRESULT STDMETHODCALLTYPE GetLanguageIDs (SAFEARRAY **LanguageIds);
	HRESULT STDMETHODCALLTYPE GetLanguageNames (SAFEARRAY **LanguageNames, boolean EnglishNames = TRUE);

// Implementation
protected:
	class CSapi5InputInfo * mSapi5Input;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrRecognitionEngine), DaSvrRecognitionEngine)

/////////////////////////////////////////////////////////////////////////////
