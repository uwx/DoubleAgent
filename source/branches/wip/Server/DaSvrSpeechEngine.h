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

class ATL_NO_VTABLE __declspec(uuid("{1147E51F-A208-11DE-ABF2-002421116FB2}")) DaSvrSpeechEngine :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrSpeechEngine, &__uuidof(DaSvrSpeechEngine)>,
	public IDispatchImpl<IDaSvrSpeechEngine, &__uuidof(IDaSvrSpeechEngine), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrSpeechEngine), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrSpeechEngine ();
	virtual ~DaSvrSpeechEngine ();

// Attributes
public:

// Operations
public:
	static DaSvrSpeechEngine * CreateInstance (class CSapi5VoiceInfo * pVoiceInfo, LPCTSTR pClientMutexName = NULL);
#ifndef	_WIN64
	static DaSvrSpeechEngine * CreateInstance (class CSapi4VoiceInfo * pVoiceInfo, LPCTSTR pClientMutexName = NULL);
#endif
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRSPEECHENGINE)
	DECLARE_NOT_AGGREGATABLE(DaSvrSpeechEngine)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrSpeechEngine)
		COM_INTERFACE_ENTRY(IDaSvrSpeechEngine)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrSpeechEngine)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrSpeechEngine)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrSpeechEngine
	HRESULT STDMETHODCALLTYPE GetTTSModeID (BSTR *TTSModeID);
	HRESULT STDMETHODCALLTYPE GetDisplayName (BSTR *DisplayName);
	HRESULT STDMETHODCALLTYPE GetManufacturer (BSTR *Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT STDMETHODCALLTYPE GetGender (short *Gender);
	HRESULT STDMETHODCALLTYPE GetLanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE GetLanguageName (BSTR *LanguageName, boolean EnglishName = TRUE);

// Implementation
protected:
	class CSapi5VoiceInfo * mSapi5Voice;
#ifndef	_WIN64
	class CSapi4VoiceInfo *	mSapi4Voice;
#endif
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrSpeechEngine), DaSvrSpeechEngine)

/////////////////////////////////////////////////////////////////////////////
