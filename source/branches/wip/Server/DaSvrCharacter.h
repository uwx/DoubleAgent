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

	static HRESULT GetDefaultTTSEngine (CAgentFile * pFile, IDaSvrTTSEngine ** pTTSEngine, LPCTSTR pClientMutexName = NULL);
	static HRESULT FindTTSEngines (CAgentFile * pFile, LANGID pLangId, short pGender, IDaSvrTTSEngines ** pTTSEngines, LPCTSTR pClientMutexName = NULL);
	static HRESULT GetDefaultSREngine (CAgentFile * pFile, IDaSvrSREngine ** pSREngine, LPCTSTR pClientMutexName = NULL);
	static HRESULT FindSREngines (CAgentFile * pFile, LANGID pLangId, IDaSvrSREngines ** pSREngines, LPCTSTR pClientMutexName = NULL);

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
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrBalloon2), 0, &DelegateIDaSvrBalloon)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentBalloon), 0, &DelegateIDaSvrBalloon)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IAgentBalloonEx), 0, &DelegateIDaSvrBalloon)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IDaSvrCommands2), 0, &DelegateIDaSvrCommands)
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
	HRESULT STDMETHODCALLTYPE GetVisible (long *Visible);
	HRESULT STDMETHODCALLTYPE SetPosition (long Left, long Top);
	HRESULT STDMETHODCALLTYPE GetPosition (long *Left, long *Top);
	HRESULT STDMETHODCALLTYPE SetSize (long Width, long Height);
	HRESULT STDMETHODCALLTYPE GetSize (long *Width, long *Height);
	HRESULT STDMETHODCALLTYPE GetName (BSTR *Name);
	HRESULT STDMETHODCALLTYPE GetDescription (BSTR *Description);
	HRESULT STDMETHODCALLTYPE GetTTSSpeed (long *Speed);
	HRESULT STDMETHODCALLTYPE GetTTSPitch (short *Pitch);
	HRESULT STDMETHODCALLTYPE Activate (short State);
	HRESULT STDMETHODCALLTYPE SetIdleOn (long On);
	HRESULT STDMETHODCALLTYPE GetIdleOn (long *On);
	HRESULT STDMETHODCALLTYPE Prepare (long Type, BSTR Name, long Queue, long *RequestID);
	HRESULT STDMETHODCALLTYPE Play (BSTR Animation, long *RequestID);
	HRESULT STDMETHODCALLTYPE Stop (long RequestID);
	HRESULT STDMETHODCALLTYPE StopAll (long Types);
	HRESULT STDMETHODCALLTYPE Wait (long WaitForRequestID, long *RequestID);
	HRESULT STDMETHODCALLTYPE Interrupt (long InterruptRequestID, long *RequestID);
	HRESULT STDMETHODCALLTYPE Show (long Fast, long *RequestID);
	HRESULT STDMETHODCALLTYPE Hide (long Fast, long *RequestID);
	HRESULT STDMETHODCALLTYPE Speak (BSTR Text, BSTR Url, long *RequestID);
	HRESULT STDMETHODCALLTYPE MoveTo (short x, short y, long Speed, long *RequestID);
	HRESULT STDMETHODCALLTYPE GestureAt (short x, short y, long *RequestID);
	HRESULT STDMETHODCALLTYPE GetMoveCause (long *Cause);
	HRESULT STDMETHODCALLTYPE GetVisibilityCause (long *Cause);
	HRESULT STDMETHODCALLTYPE HasOtherClients (long *OtherClientCount);
	HRESULT STDMETHODCALLTYPE SetSoundEffectsOn (long On);
	HRESULT STDMETHODCALLTYPE GetSoundEffectsOn (long *On);
	HRESULT STDMETHODCALLTYPE SetName (BSTR Name);
	HRESULT STDMETHODCALLTYPE SetDescription (BSTR Description);
	HRESULT STDMETHODCALLTYPE GetExtraData (BSTR *ExtraData);

	HRESULT STDMETHODCALLTYPE ShowPopupMenu (short x, short y);
	HRESULT STDMETHODCALLTYPE SetAutoPopupMenu (long AutoPopupMenu);
	HRESULT STDMETHODCALLTYPE GetAutoPopupMenu (long *AutoPopupMenu);
	HRESULT STDMETHODCALLTYPE GetHelpFileName (BSTR *Name);
	HRESULT STDMETHODCALLTYPE SetHelpFileName (BSTR Name);
	HRESULT STDMETHODCALLTYPE SetHelpModeOn (long HelpModeOn);
	HRESULT STDMETHODCALLTYPE GetHelpModeOn (long *HelpModeOn);
	HRESULT STDMETHODCALLTYPE SetHelpContextID (long ContextID);
	HRESULT STDMETHODCALLTYPE GetHelpContextID (long *ContextID);
	HRESULT STDMETHODCALLTYPE GetActive (short *State);
	HRESULT STDMETHODCALLTYPE Listen (long Listen);
	HRESULT STDMETHODCALLTYPE SetLanguageID (long LanguageID);
	HRESULT STDMETHODCALLTYPE GetLanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE GetTTSModeID (BSTR *ModeID);
	HRESULT STDMETHODCALLTYPE SetTTSModeID (BSTR ModeID);
	HRESULT STDMETHODCALLTYPE GetSRModeID (BSTR *ModeID);
	HRESULT STDMETHODCALLTYPE SetSRModeID (BSTR ModeID);
	HRESULT STDMETHODCALLTYPE GetGUID (BSTR *GUID);
	HRESULT STDMETHODCALLTYPE GetOriginalSize (long *Width, long *Height);
	HRESULT STDMETHODCALLTYPE Think (BSTR Text, long *RequestID);
	HRESULT STDMETHODCALLTYPE GetVersion(short *MajorVersion, short *MinorVersion);
	HRESULT STDMETHODCALLTYPE GetAnimationNames (IUnknown **punkEnum);
	HRESULT STDMETHODCALLTYPE GetSRStatus (long *Status);

	HRESULT STDMETHODCALLTYPE get_Balloon (IDaSvrBalloon2 **Balloon);
	HRESULT STDMETHODCALLTYPE get_Commands (IDaSvrCommands2 **Commands);
	HRESULT STDMETHODCALLTYPE get_Style (long *Style);
	HRESULT STDMETHODCALLTYPE put_Style (long Style);
	HRESULT STDMETHODCALLTYPE get_HasIcon (VARIANT_BOOL *HasIcon);
	HRESULT STDMETHODCALLTYPE GenerateIcon (long ClipLeft = 0, long ClipTop = 0, long ClipWidth = -1, long ClipHeight = -1);
	HRESULT STDMETHODCALLTYPE get_IconVisible (VARIANT_BOOL *IconVisible);
	HRESULT STDMETHODCALLTYPE get_IconIdentity (BSTR *IconIdentity);
	HRESULT STDMETHODCALLTYPE put_IconIdentity (BSTR IconIdentity);
	HRESULT STDMETHODCALLTYPE GetIconIdentity (GUID *IconIdentity);
	HRESULT STDMETHODCALLTYPE SetIconIdentity (const GUID *IconIdentity);
	HRESULT STDMETHODCALLTYPE get_IconTip (BSTR *IconTip);
	HRESULT STDMETHODCALLTYPE put_IconTip (BSTR IconTip);

	HRESULT STDMETHODCALLTYPE get_TTSEngine (VARIANT_BOOL GetDefault, IDaSvrTTSEngine **TTSEngine);
	HRESULT STDMETHODCALLTYPE FindTTSEngines (long LanguageID, IDaSvrTTSEngines **TTSEngines);
	HRESULT STDMETHODCALLTYPE get_SREngine (VARIANT_BOOL GetDefault, IDaSvrSREngine **SREngine);
	HRESULT STDMETHODCALLTYPE FindSREngines (long LanguageID, IDaSvrSREngines **SREngines);

	HRESULT STDMETHODCALLTYPE get_CharacterID (long *CharacterID);
	HRESULT STDMETHODCALLTYPE get_GUID (BSTR *GUID);
	HRESULT STDMETHODCALLTYPE get_Name (BSTR *Name);
	HRESULT STDMETHODCALLTYPE put_Name (BSTR Name);
	HRESULT STDMETHODCALLTYPE get_Description (BSTR *Description);
	HRESULT STDMETHODCALLTYPE put_Description (BSTR Description);
	HRESULT STDMETHODCALLTYPE get_ExtraData (BSTR *ExtraData);
	HRESULT STDMETHODCALLTYPE get_FileName (BSTR *FileName);
	HRESULT STDMETHODCALLTYPE get_FilePath (BSTR *FilePath);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long *LanguageID);
	HRESULT STDMETHODCALLTYPE put_LanguageID (long LanguageID);
	HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR *TTSModeID);
	HRESULT STDMETHODCALLTYPE put_TTSModeID (BSTR TTSModeID);
	HRESULT STDMETHODCALLTYPE get_SRModeID (BSTR *SRModeID);
	HRESULT STDMETHODCALLTYPE put_SRModeID (BSTR SRModeID);
	HRESULT STDMETHODCALLTYPE get_Left (short *Left);
	HRESULT STDMETHODCALLTYPE put_Left (short Left);
	HRESULT STDMETHODCALLTYPE get_Top (short *Top);
	HRESULT STDMETHODCALLTYPE put_Top (short Top);
	HRESULT STDMETHODCALLTYPE get_Width (short *Width);
	HRESULT STDMETHODCALLTYPE put_Width (short Width);
	HRESULT STDMETHODCALLTYPE get_Height (short *Height);
	HRESULT STDMETHODCALLTYPE put_Height (short Height);
	HRESULT STDMETHODCALLTYPE get_OriginalWidth (short *OriginalWidth);
	HRESULT STDMETHODCALLTYPE get_OriginalHeight (short *OriginalHeight);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
	HRESULT STDMETHODCALLTYPE get_ActiveState (ActiveStateType *ActiveState);
	HRESULT STDMETHODCALLTYPE put_ActiveState (ActiveStateType ActiveState);
	HRESULT STDMETHODCALLTYPE get_IdleState (VARIANT_BOOL *IdleState);
	HRESULT STDMETHODCALLTYPE put_IdleState (VARIANT_BOOL IdleState);
	HRESULT STDMETHODCALLTYPE get_OtherClientCount (long *OtherClientCount);
	HRESULT STDMETHODCALLTYPE get_MoveCause (MoveCauseType *MoveCause);
	HRESULT STDMETHODCALLTYPE get_VisibilityCause (VisibilityCauseType *VisibilityCause);
	HRESULT STDMETHODCALLTYPE get_ListeningStatus (ListeningStatusType *ListeningStatus);

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
	IDaSvrCommands2 * GetCommandsInterface (bool pCreateObject);
	IDaSvrBalloon2 * GetBalloonInterface (bool pCreateObject);
	void PropagateLangID ();
	HRESULT DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID);
	bool DoContextMenu (HWND pOwner, const CPoint & pPosition);
	bool DoDefaultCommand (HWND pOwner, const CPoint & pPosition);

	LPVOID FindOtherRequest (long pReqID, DaSvrCharacter *& pOtherCharacter);
	void TransferListeningState (DaSvrCharacter * pOtherCharacter);

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
