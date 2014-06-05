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
#include "DaCmnTTSPrivate.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E573-A208-11DE-ABF2-002421116FB2}")) DaSvrTTSPrivate :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrTTSPrivate, &__uuidof(DaSvrTTSPrivate)>,
	public IDispatchImpl<IDaSvrTTSPrivate, &__uuidof(IDaSvrTTSPrivate), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrTTSPrivate), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CDaCmnTTSPrivate,
	public CSvrObjLifetime
{
public:
	DaSvrTTSPrivate ();
	virtual ~DaSvrTTSPrivate ();

// Attributes
public:

// Operations
public:
	static DaSvrTTSPrivate * CreateInstance (LPCTSTR pClientMutexName = NULL);

	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRTTSPRIVATE)
	DECLARE_NOT_AGGREGATABLE(DaSvrTTSPrivate)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrTTSPrivate)
		COM_INTERFACE_ENTRY(IDaSvrTTSPrivate)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrTTSPrivate)
		COM_INTERFACE_ENTRY2(IDaSvrTTSEngine, IDaSvrTTSPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrTTSPrivate)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrTTSEngine
	HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR *TTSModeID);
	HRESULT STDMETHODCALLTYPE get_DisplayName (BSTR *DisplayName);
	HRESULT STDMETHODCALLTYPE get_Manufacturer (BSTR *Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT STDMETHODCALLTYPE get_Gender (SpeechGenderType *Gender);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName);

	// IDaSvrTTSPrivate
	HRESULT STDMETHODCALLTYPE get_InitDisplayName (BSTR * DisplayName);
    HRESULT STDMETHODCALLTYPE put_InitDisplayName (BSTR DisplayName);
	HRESULT STDMETHODCALLTYPE get_InitManufacturer (BSTR * Manufacturer);
    HRESULT STDMETHODCALLTYPE put_InitManufacturer (BSTR Manufacturer);
	HRESULT STDMETHODCALLTYPE get_InitVersion (BSTR * Version);
    HRESULT STDMETHODCALLTYPE put_InitVersion (BSTR Version);
	HRESULT STDMETHODCALLTYPE get_InitGender (SpeechGenderType * Gender);
    HRESULT STDMETHODCALLTYPE put_InitGender (SpeechGenderType Gender);
	HRESULT STDMETHODCALLTYPE get_InitLanguageID (long* LanguageID);
    HRESULT STDMETHODCALLTYPE put_InitLanguageID (long LanguageID);
    HRESULT STDMETHODCALLTYPE get_InitString (BSTR ValuePath, BSTR *StringValue);
    HRESULT STDMETHODCALLTYPE put_InitString (BSTR ValuePath, BSTR StringValue);
    HRESULT STDMETHODCALLTYPE get_InitAttribute (BSTR AttributeName, BSTR *AttributeValue);
    HRESULT STDMETHODCALLTYPE put_InitAttribute (BSTR AttributeName, BSTR AttributeValue);
	HRESULT STDMETHODCALLTYPE get_InitFilePath (BSTR FileId, BSTR *FilePath);
    HRESULT STDMETHODCALLTYPE put_InitFilePath (BSTR FileId, BSTR FilePath);

    HRESULT STDMETHODCALLTYPE InitFromRegistry (BSTR RegistryPath, BSTR VoiceId, VARIANT_BOOL *Success);

// Implementation
protected:
	void Disconnect (bool pAbandonned);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrTTSPrivate), DaSvrTTSPrivate)

/////////////////////////////////////////////////////////////////////////////
