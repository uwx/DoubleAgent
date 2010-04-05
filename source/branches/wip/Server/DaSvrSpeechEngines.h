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

class ATL_NO_VTABLE __declspec(uuid("{1147E520-A208-11DE-ABF2-002421116FB2}")) DaSvrSpeechEngines :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrSpeechEngines, &__uuidof(DaSvrSpeechEngines)>,
	public IDispatchImpl<IDaSvrSpeechEngines, &__uuidof(IDaSvrSpeechEngines), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrSpeechEngines), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CSvrObjLifetime
{
public:
	DaSvrSpeechEngines ();
	virtual ~DaSvrSpeechEngines ();

// Attributes
public:
	CAtlPtrTypeArray <class CSapi5VoiceInfo>	mSapi5Voices;
#ifndef	_WIN64
	CAtlPtrTypeArray <class CSapi4VoiceInfo>	mSapi4Voices;
#endif

// Operations
public:
	static DaSvrSpeechEngines * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

	void UseAllVoices ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRSPEECHENGINES)
	DECLARE_NOT_AGGREGATABLE(DaSvrSpeechEngines)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrSpeechEngines)
		COM_INTERFACE_ENTRY(IDaSvrSpeechEngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrSpeechEngines)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloon), IDaSvrSpeechEngines)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloonEx), IDaSvrSpeechEngines)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrSpeechEngines)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrSpeechEngines
	HRESULT STDMETHODCALLTYPE get_Item (long Index, IDaSvrSpeechEngine **SpeechEngine);
	HRESULT STDMETHODCALLTYPE get_Count (long *Count);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrSpeechEngines), DaSvrSpeechEngines)

/////////////////////////////////////////////////////////////////////////////
