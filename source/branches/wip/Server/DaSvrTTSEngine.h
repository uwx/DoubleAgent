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

class ATL_NO_VTABLE __declspec(uuid("{1147E51F-A208-11DE-ABF2-002421116FB2}")) DaSvrTTSEngine :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrTTSEngine, &__uuidof(DaSvrTTSEngine)>,
	public IDispatchImpl<IDaSvrTTSEngine, &__uuidof(IDaSvrTTSEngine), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrTTSEngine), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrTTSEngine ();
	virtual ~DaSvrTTSEngine ();

// Attributes
public:

// Operations
public:
	static DaSvrTTSEngine * CreateInstance (class CSapi5VoiceInfo * pVoiceInfo, LPCTSTR pClientMutexName = NULL);
#ifndef	_WIN64
	static DaSvrTTSEngine * CreateInstance (class CSapi4VoiceInfo * pVoiceInfo, LPCTSTR pClientMutexName = NULL);
#endif
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRTTSENGINE)
	DECLARE_NOT_AGGREGATABLE(DaSvrTTSEngine)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrTTSEngine)
		COM_INTERFACE_ENTRY(IDaSvrTTSEngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrTTSEngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrTTSEngine)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrTTSEngine
	HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR *TTSModeID);
	HRESULT STDMETHODCALLTYPE get_DisplayName (BSTR *DisplayName);
	HRESULT STDMETHODCALLTYPE get_Manufacturer (BSTR *Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT STDMETHODCALLTYPE get_Gender (SpeechGenderType *Gender);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName);

// Implementation
protected:
	class CSapi5VoiceInfo * mSapi5Voice;
#ifndef	_WIN64
	class CSapi4VoiceInfo *	mSapi4Voice;
#endif
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrTTSEngine), DaSvrTTSEngine)

/////////////////////////////////////////////////////////////////////////////
