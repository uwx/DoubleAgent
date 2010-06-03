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
#include "ServerNotify.h"
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E500-A208-11DE-ABF2-002421116FB2}")) DaServer :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaServer, &__uuidof(DaServer)>,
	public IDispatchImpl<IDaServer2, &__uuidof(IDaServer2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfo2Impl<&__uuidof(DaServer), &__uuidof(_DaSvrEvents), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public IStdMarshalInfo,
	public CSvrObjLifetime,
	public CInstanceAnchor,
	public CEventNotifyHolder<DaServer>,
	protected _IEventReflect
{
public:
	DaServer();
	virtual ~DaServer();

// Attributes
public:
	CComObjectStackEx <CServerNotify>	mNotify;

// Operations
public:
	static HRESULT WINAPI UpdateRegistryOverride (BOOL bRegister);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease();

// Overrides
public:
	virtual void OnClientEnded ();
	virtual bool _OnDownloadComplete (CFileDownload * pDownload);
	virtual bool _PreNotify ();
	virtual bool _PostNotify ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASERVER)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_PROTECT_FINAL_RELEASE(CComObjectRootEx<CComMultiThreadModel>)

	BEGIN_COM_MAP(DaServer)
		COM_INTERFACE_ENTRY(IStdMarshalInfo)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDispatch), 0, &CatchFirstQueryInterface)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaServer2), 0, &CatchFirstQueryInterface)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaServer), 0, &CatchFirstQueryInterface)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgent), 0, &CatchFirstQueryInterface)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentEx), 0, &CatchFirstQueryInterface)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IConnectionPointContainer), 0, &CatchFirstQueryInterface)
		COM_INTERFACE_ENTRY(IDaServer2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaServer2)
		COM_INTERFACE_ENTRY2(IDaServer, IDaServer2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgent), IDaServer2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentEx), IDaServer2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrPropertySheet2), 0, &DelegateIDaSvrPropertySheet)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrPropertySheet), 0, &DelegateIDaSvrPropertySheet)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentPropertySheet), 0, &DelegateIDaSvrPropertySheet)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrSettings), 0, &DelegateIDaSvrSettings)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrAudioOutput), 0, &DelegateIDaSvrAudioOutput)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentAudioOutputProperties), 0, &DelegateIDaSvrAudioOutput)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentAudioOutputPropertiesEx), 0, &DelegateIDaSvrAudioOutput)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrSpeechInput), 0, &DelegateIDaSvrSpeechInput)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentSpeechInputProperties), 0, &DelegateIDaSvrSpeechInput)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrCommandsWindow2), 0, &DelegateIDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrCommandsWindow), 0, &DelegateIDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentCommandWindow), 0, &DelegateIDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrCharacterFiles), 0, &DelegateIDaSvrCharacterFiles)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaServer)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

	static HRESULT WINAPI CatchFirstQueryInterface (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrPropertySheet (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrSettings (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrAudioOutput (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrSpeechInput (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrCommandsWindow (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrCharacterFiles (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaServer2
	HRESULT STDMETHODCALLTYPE Load (VARIANT vLoadKey, long * pdwCharID, long * RequestID);
	HRESULT STDMETHODCALLTYPE Unload (long CharacterID);
	HRESULT STDMETHODCALLTYPE Register (IUnknown * punkNotifySink, long * pdwSinkID);
	HRESULT STDMETHODCALLTYPE Unregister (long dwSinkID);
	HRESULT STDMETHODCALLTYPE GetCharacter (long CharacterID, IDispatch ** Character);
	HRESULT STDMETHODCALLTYPE GetSuspended (long * Suspended);

    HRESULT STDMETHODCALLTYPE GetCharacterEx (long CharacterID, IDaSvrCharacter **Character);
    HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
    HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties (short x, short y, long UseDefaultPosition);

	HRESULT STDMETHODCALLTYPE get_Character (long CharacterID, IDaSvrCharacter2 **Character2);
	HRESULT STDMETHODCALLTYPE get_CharacterFiles (IDaSvrCharacterFiles **CharacterFiles);
	HRESULT STDMETHODCALLTYPE get_CharacterStyle (long *CharacterStyle);
	HRESULT STDMETHODCALLTYPE put_CharacterStyle (long CharacterStyle);
	HRESULT STDMETHODCALLTYPE get_TTSEngines (IDaSvrTTSEngines **TTSEngines);
	HRESULT STDMETHODCALLTYPE FindTTSEngines (long LanguageID, short Gender, IDaSvrTTSEngines **TTSEngines);
	HRESULT STDMETHODCALLTYPE GetCharacterTTSEngine (VARIANT LoadKey, IDaSvrTTSEngine **TTSEngine);
	HRESULT STDMETHODCALLTYPE FindCharacterTTSEngines (VARIANT LoadKey, long LanguageID, IDaSvrTTSEngines **TTSEngines);
	HRESULT STDMETHODCALLTYPE get_SREngines (IDaSvrSREngines **SREngines);
	HRESULT STDMETHODCALLTYPE FindSREngines (long LanguageID, IDaSvrSREngines **SREngines);
	HRESULT STDMETHODCALLTYPE GetCharacterSREngine (VARIANT LoadKey, IDaSvrSREngine **SREngine);
	HRESULT STDMETHODCALLTYPE FindCharacterSREngines (VARIANT LoadKey, long LanguageID, IDaSvrSREngines **SREngines);
	HRESULT STDMETHODCALLTYPE get_PropertySheet (IDaSvrPropertySheet2 **PropertySheet);
	HRESULT STDMETHODCALLTYPE get_CommandsWindow (IDaSvrCommandsWindow2 **CommandsWindow);
	HRESULT STDMETHODCALLTYPE get_Settings (IDaSvrSettings **Settings);

	// IStdMarshalInfo
    HRESULT STDMETHODCALLTYPE GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid);

// Implementation
protected:
	void UnloadAllCharacters (bool pAbandonned = false);
	HRESULT LoadCharacter (LPCTSTR pFilePath, long & pCharID, long & pReqID);
	HRESULT UnloadCharacter (long pCharID);

protected:
	CAtlOwnPtrMap <long, CFileDownload>	mCharactersLoading;
private:
	DWORD								mCharacterStyle;
	UINT								mUsingHandler;
	CAtlTypeArray <long>				mInNotifyUnregister;
};

/////////////////////////////////////////////////////////////////////////////

#define	UpdateRegistry	UpdateRegistryOverride
OBJECT_ENTRY_AUTO(__uuidof(DaServer), DaServer)
#undef	UpdateRegistry

/////////////////////////////////////////////////////////////////////////////
