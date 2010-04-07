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
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "AgentNotifyIcon.h"
#include "ServerNotifySink.h"
#include "SapiVoiceCache.h"
#include "SapiInputCache.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E50D-A208-11DE-ABF2-002421116FB2}")) DaSvrCharacter :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCharacter, &__uuidof(DaSvrCharacter)>,
	public IDispatchImpl<IDaSvrCharacter2, &__uuidof(IDaSvrCharacter2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCharacter), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public IStdMarshalInfo,
	public CAgentFileClient,
	public CSapiVoiceClient,
	public CSapiInputClient,
	public CSvrObjLifetime,
	protected _IServerNotifySink
{
public:
	DaSvrCharacter ();
	virtual ~DaSvrCharacter ();

// Attributes
public:
	long GetCharID () const {return mCharID;}
	LANGID GetLangID () const {return mLangID;}
	CAgentFile * GetFile () const {return mFile;}
	BSTR GetName () const;

	bool IsVisible (bool pOrIsShowing = true) const;
	bool IsShowing () const;
	bool IsHiding () const;
	bool IsInputActive () const;
	bool IsClientActive () const;
	bool IsSpeaking () const;
	bool IsListening () const;
	bool IsHearing () const;
	bool IsIdleEnabled () const;
	bool IsSoundEnabled (bool pIgnoreGlobalConfig = false) const;
	bool IsAutoPopupMenu () const;
	bool IsIconShown () const;
	bool IsIconVisible () const;
	long GetActiveClient () const;

// Operations
public:
	static DaSvrCharacter * CreateInstance (long pCharID, CAgentFileCache * pUsedFileCache, _IServerNotify * pNotify, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease();

	long Show (bool pFast, bool pImmediate = false);
	long Hide (bool pFast, bool pImmediate = false);
	bool SetClientActive (bool pActive, bool pInputActive);
	int GetClientCount (int pSkipCharID = 0) const;

	HRESULT OpenFile (CAgentFile * pFile, DWORD pInitialStyle);
	HRESULT SetLangID (LANGID pLangID);
	HRESULT SetStyle (DWORD pRemoveStyle, DWORD pAddStyle);
	HRESULT StartListening (bool pManual);
	HRESULT StopListening (bool pManual, long pCause);

	static HRESULT GetDefaultSpeechEngine (CAgentFile * pFile, IDaSvrSpeechEngine ** pSpeechEngine, LPCTSTR pClientMutexName = NULL);
	static HRESULT FindSpeechEngines (CAgentFile * pFile, LANGID pLangId, short pGender, IDaSvrSpeechEngines ** pSpeechEngines, LPCTSTR pClientMutexName = NULL);
	static HRESULT GetDefaultRecognitionEngine (CAgentFile * pFile, IDaSvrRecognitionEngine ** pRecognitionEngine, LPCTSTR pClientMutexName = NULL);
	static HRESULT FindRecognitionEngines (CAgentFile * pFile, LANGID pLangId, IDaSvrRecognitionEngines ** pRecognitionEngines, LPCTSTR pClientMutexName = NULL);

// Overrides
public:
	virtual void OnClientEnded ();
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual bool _OnDownloadComplete (CFileDownload * pDownload);
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl);
	virtual bool _OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual bool _OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual void _OnOptionsChanged ();
	virtual void _OnDefaultCharacterChanged ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRCHARACTER)
	DECLARE_NOT_AGGREGATABLE(DaSvrCharacter)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_PROTECT_FINAL_RELEASE(CComObjectRootEx<CComMultiThreadModel>)

	BEGIN_COM_MAP(DaSvrCharacter)
		COM_INTERFACE_ENTRY(IDaSvrCharacter2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCharacter2)
		COM_INTERFACE_ENTRY2(IDaSvrCharacter, IDaSvrCharacter2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCharacter), IDaSvrCharacter2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCharacterEx), IDaSvrCharacter2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IStdMarshalInfo)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrBalloon), 0, &DelegateIDaSvrBalloon)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentBalloon), 0, &DelegateIDaSvrBalloon)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentBalloonEx), 0, &DelegateIDaSvrBalloon)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrCommands), 0, &DelegateIDaSvrCommands)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentCommands), 0, &DelegateIDaSvrCommands)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentCommandsEx), 0, &DelegateIDaSvrCommands)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrCharacter)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

	static HRESULT WINAPI DelegateIDaSvrBalloon (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);
	static HRESULT WINAPI DelegateIDaSvrCommands (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrCharacter2
	HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
	HRESULT STDMETHODCALLTYPE SetPosition (long lLeft, long lTop);
	HRESULT STDMETHODCALLTYPE GetPosition (long *plLeft, long *plTop);
	HRESULT STDMETHODCALLTYPE SetSize (long lWidth, long lHeight);
	HRESULT STDMETHODCALLTYPE GetSize (long *plWidth, long *plHeight);
	HRESULT STDMETHODCALLTYPE GetName (BSTR *pbszName);
	HRESULT STDMETHODCALLTYPE GetDescription (BSTR *pbszDescription);
	HRESULT STDMETHODCALLTYPE GetTTSSpeed (long *pdwSpeed);
	HRESULT STDMETHODCALLTYPE GetTTSPitch (short *pwPitch);
	HRESULT STDMETHODCALLTYPE Activate (short sState);
	HRESULT STDMETHODCALLTYPE SetIdleOn (long bOn);
	HRESULT STDMETHODCALLTYPE GetIdleOn (long *pbOn);
	HRESULT STDMETHODCALLTYPE Prepare (long dwType, BSTR bszName, long bQueue, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE Play (BSTR bszAnimation, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE Stop (long dwReqID);
	HRESULT STDMETHODCALLTYPE StopAll (long lTypes);
	HRESULT STDMETHODCALLTYPE Wait (long dwReqID, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE Interrupt (long dwReqID, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE Show (long bFast, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE Hide (long bFast, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE Speak (BSTR bszText, BSTR bszUrl, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE MoveTo (short x, short y, long lSpeed, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE GestureAt (short x, short y, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE GetMoveCause (long *pdwCause);
	HRESULT STDMETHODCALLTYPE GetVisibilityCause (long *pdwCause);
	HRESULT STDMETHODCALLTYPE HasOtherClients (long *plNumOtherClients);
	HRESULT STDMETHODCALLTYPE SetSoundEffectsOn (long bOn);
	HRESULT STDMETHODCALLTYPE GetSoundEffectsOn (long *pbOn);
	HRESULT STDMETHODCALLTYPE SetName (BSTR bszName);
	HRESULT STDMETHODCALLTYPE SetDescription (BSTR bszDescription);
	HRESULT STDMETHODCALLTYPE GetExtraData (BSTR *pbszExtraData);

	HRESULT STDMETHODCALLTYPE ShowPopupMenu (short x, short y);
	HRESULT STDMETHODCALLTYPE SetAutoPopupMenu (long bAutoPopupMenu);
	HRESULT STDMETHODCALLTYPE GetAutoPopupMenu (long *pbAutoPopupMenu);
	HRESULT STDMETHODCALLTYPE GetHelpFileName (BSTR *pbszName);
	HRESULT STDMETHODCALLTYPE SetHelpFileName (BSTR bszName);
	HRESULT STDMETHODCALLTYPE SetHelpModeOn (long bHelpModeOn);
	HRESULT STDMETHODCALLTYPE GetHelpModeOn (long *pbHelpModeOn);
	HRESULT STDMETHODCALLTYPE SetHelpContextID (long ulID);
	HRESULT STDMETHODCALLTYPE GetHelpContextID (long *pulID);
	HRESULT STDMETHODCALLTYPE GetActive (short *psState);
	HRESULT STDMETHODCALLTYPE Listen (long bListen);
	HRESULT STDMETHODCALLTYPE SetLanguageID (long langid);
	HRESULT STDMETHODCALLTYPE GetLanguageID (long *plangid);
	HRESULT STDMETHODCALLTYPE GetTTSModeID (BSTR *pbszModeID);
	HRESULT STDMETHODCALLTYPE SetTTSModeID (BSTR bszModeID);
	HRESULT STDMETHODCALLTYPE GetSRModeID (BSTR *pbszModeID);
	HRESULT STDMETHODCALLTYPE SetSRModeID (BSTR bszModeID);
	HRESULT STDMETHODCALLTYPE GetGUID (BSTR *pbszID);
	HRESULT STDMETHODCALLTYPE GetOriginalSize (long *plWidth, long *plHeight);
	HRESULT STDMETHODCALLTYPE Think (BSTR bszText, long *pdwReqID);
	HRESULT STDMETHODCALLTYPE GetVersion(short *psMajor, short *psMinor);
	HRESULT STDMETHODCALLTYPE GetAnimationNames (IUnknown **punkEnum);
	HRESULT STDMETHODCALLTYPE GetSRStatus (long *plStatus);

	HRESULT STDMETHODCALLTYPE get_Style (long *Style);
	HRESULT STDMETHODCALLTYPE put_Style (long Style);
	HRESULT STDMETHODCALLTYPE get_HasIcon (boolean *HasIcon);
	HRESULT STDMETHODCALLTYPE GenerateIcon (long ClipLeft = 0, long ClipTop = 0, long ClipWidth = -1, long ClipHeight = -1);
	HRESULT STDMETHODCALLTYPE get_IconVisible (boolean *IconVisible);
	HRESULT STDMETHODCALLTYPE get_IconIdentity (BSTR *IconIdentity);
	HRESULT STDMETHODCALLTYPE put_IconIdentity (BSTR IconIdentity);
	HRESULT STDMETHODCALLTYPE GetIconIdentity (GUID *IconIdentity);
	HRESULT STDMETHODCALLTYPE SetIconIdentity (const GUID *IconIdentity);
	HRESULT STDMETHODCALLTYPE get_IconTip (BSTR *IconTip);
	HRESULT STDMETHODCALLTYPE put_IconTip (BSTR IconTip);

	HRESULT STDMETHODCALLTYPE GetSpeechEngine (boolean GetDefault, IDaSvrSpeechEngine **SpeechEngine);
	HRESULT STDMETHODCALLTYPE FindSpeechEngines (long LanguageID, IDaSvrSpeechEngines **SpeechEngines);
	HRESULT STDMETHODCALLTYPE GetRecognitionEngine (boolean GetDefault, IDaSvrRecognitionEngine **RecognitionEngine);
	HRESULT STDMETHODCALLTYPE FindRecognitionEngines (long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines);

	// IStdMarshalInfo
    HRESULT STDMETHODCALLTYPE GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid);

// Implementation
public:
	class DaSvrCommands * GetCommandsObj (bool pCreateObject);
	class DaSvrBalloon * GetBalloonObj (bool pCreateObject);
	class CAgentPopupWnd * GetAgentWnd ();
	class CAgentBalloonWnd * GetBalloonWnd (bool pCreateObject);
	class CAgentListeningWnd * GetListeningWnd (bool pCreateObject);

	bool DoMenuCommand (USHORT pCommandId);
	HRESULT StopAll (long pStopTypes, HRESULT pReqStatus);

	class CSapiVoice * GetSapiVoice (bool pCreateObject, LPCTSTR pVoiceName = NULL);
	void ReleaseSapiVoice ();
	class CSapi5Input * GetSapiInput (bool pCreateObject, LPCTSTR pEngineName = NULL);
	void ReleaseSapiInput ();

	bool ShowListeningState (bool pShow);
	bool ShowHearingState (bool pShow);
	bool ShowIcon (bool pShow);

public:
	bool PreNotify ();
	bool PostNotify ();
	UINT IsInNotify () const;

protected:
	IDaSvrBalloon * GetBalloonInterface (bool pCreateObject);
	void PropagateLangID ();
	HRESULT DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID);
	bool DoContextMenu (HWND pOwner, const CPoint & pPosition);
	bool DoDefaultCommand (HWND pOwner, const CPoint & pPosition);

	LPVOID FindOtherRequest (long pReqID, DaSvrCharacter *& pOtherCharacter);
	void TransferListeningState (DaSvrCharacter * pOtherCharacter);
	long GetListeningStatus ();

public:
	_IServerNotify *							mNotify;
protected:
	long										mCharID;
	LANGID										mLangID;
	CAgentFile *								mFile;
	CAgentFileCache *							mUsedFileCache;
	class CAgentPopupWnd *						mWnd;
	IUnknownPtr									mWndRefHolder;
	LPUNKNOWN									mSvrBalloon;
	LPUNKNOWN									mSvrCommands;
	class CSapiVoice *							mSapiVoice;
	class CSapi5Input *							mSapiInput;
	tPtr <class CListeningState>				mListeningState;
	CAtlOwnPtrMap <long, class CQueuedPrepare>	mPrepares;
	bool										mIdleEnabled;
	bool										mSoundEnabled;
	bool										mAutoPopupMenu;
	CAgentIconData								mIconData;
private:
	UINT										mInNotify;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCharacter), DaSvrCharacter)

/////////////////////////////////////////////////////////////////////////////
