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

#include "DaCoreRes.h"
#include "DaServerRes.h"
#include "DaServerOdl.h"
#include "DaGuid.h"
#include "ServerLifetime.h"
#include "ServerNotifySink.h"
#include "DaError.h"
#include "AgentFileCache.h"
#include "TimerNotify.h"
#include "PropertySheet.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef	_STRICT_COMPATIBILITY
#define	_STRICT_COMPATIBILITY
#endif
#ifndef	_TRACE_CHARACTER_ACTIONS
#define	_TRACE_CHARACTER_ACTIONS
#endif
/////////////////////////////////////////////////////////////////////////////

class DaSvrPropertySheet;
class DaSvrSettings;
class DaSvrAudioOutput;
class DaSvrSpeechInput;
class DaSvrCommandsWindow;
class DaSvrCharacter;
class DaSvrCharacterFiles;
class CVoiceCommandsWnd;

/////////////////////////////////////////////////////////////////////////////

class CDaServerModule : public CAtlExeModuleT<CDaServerModule>,
	public CAgentFileCache,
	public CServerLifetime,
	public _IServerNotifySink,
	public _AtlPropSheetOwner,
	protected _ITimerNotifySink
{
public:
	CDaServerModule();
	~CDaServerModule();

	DECLARE_LIBID(__uuidof(DaServerTypeLib))
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DASERVERAPP, "{1147E500-A208-11DE-ABF2-002421116FB2}")

// Attributes
	long										mNextCharID;
	CAtlPtrTypeArray <interface _IServerNotify>	mNotify;
	static const UINT							mOptionsChangedMsgId;
	static const UINT							mDefaultCharacterChangedMsgId;

// Operations
	bool ShowSettings (LPCTSTR pStartPage = NULL);

	DaSvrPropertySheet * GetSvrPropertySheet (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrSettings * GetSvrSettings (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrAudioOutput * GetSvrAudioOutput (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrSpeechInput * GetSvrSpeechInput (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrCommandsWindow * GetSvrCommandsWindow (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrCharacterFiles * GetSvrCharacterFiles (bool pCreate, LPCTSTR pClientMutexName = NULL);

	DaSvrCharacter * GetAppCharacter (long pCharID);
	DaSvrCharacter * GetListenCharacter ();
	CVoiceCommandsWnd * GetVoiceCommandsWnd (bool pCreate, long pCharID = 0);
	void OnCharacterListening (long pCharID, bool pListening, long pCause);
	bool IsHotKeyStillPressed () const;

	bool AddTimerNotify (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink);
	bool DelTimerNotify (UINT_PTR pTimerId);
	bool HasTimerNotify (UINT_PTR pTimerId);
	CTimerNotify * GetTimerNotify (UINT_PTR pTimerId);

	bool TraceCharacterAction (long pCharID, LPCTSTR pAction, LPCTSTR pFormat = NULL, ...);

// Overrides
public:
	HRESULT RegisterAppId ();
	HRESULT UnregisterAppId ();
	bool ParseCommandLine (LPCTSTR lpCmdLine, HRESULT* pnRetCode);
	HRESULT PreMessageLoop (int nShowCmd);
	HRESULT PostMessageLoop ();
	void RunMessageLoop();
public:
	virtual void _OnCharacterLoaded (long pCharID);
	virtual void _OnCharacterUnloaded (long pCharID);
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _OnOptionsChanged ();
	virtual void _OnDefaultCharacterChanged ();
protected:
	virtual void OnTimerNotify (class CTimerNotify * pTimerNotify, UINT_PTR pTimerId);
	virtual void OnShowModelessPropertySheet (class CAtlPropertySheet * pPropertySheet);
	virtual void OnHideModelessPropertySheet (class CAtlPropertySheet * pPropertySheet);

// Implementation
public:
	BEGIN_MSG_MAP(CDaServerModule)
		MESSAGE_HANDLER(WM_HOTKEY, OnThreadHotKey)
		MESSAGE_HANDLER(mOptionsChangedMsgId, OnBroadcastOptionsChanged)
		MESSAGE_HANDLER(mDefaultCharacterChangedMsgId, OnBroadcastDefaultCharacterChanged)
	END_MSG_MAP()

	LRESULT OnThreadHotKey (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnBroadcastOptionsChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnBroadcastDefaultCharacterChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

public:
	void OnDeleteSvrPropertySheet (DaSvrPropertySheet * pSvrPropertySheet);
	void OnDeleteSvrSettings (DaSvrSettings * pSvrSettings);
	void OnDeleteSvrAudioOutput (DaSvrAudioOutput * pSvrAudioOutput);
	void OnDeleteSvrSpeechInput (DaSvrSpeechInput * pSvrSpeechInput);
	void OnDeleteSvrCommandsWindow (DaSvrCommandsWindow * pSvrCommandsWindow);
	void OnDeleteSvrCharacterFiles (DaSvrCharacterFiles * pSvrCharacterFiles);

protected:
	void _PreMessageLoop (bool pForModal);
	void _PostMessageLoop (bool pForModal);
	void SetVoiceCommandClients (long pCharID);
	void SetVoiceCommandNames (long pCharID);
	bool StartActionTrace (long pCharID);
	bool StopActionTrace (long pCharID);

protected:
	typedef CAtlMap <long, CString, CPrimitiveElementTraits<long>, CStringElementTraitsI<CString> > CActionTraceMap;

	tPtr <DaSvrPropertySheet>				mSvrPropertySheet;
	tPtr <DaSvrSettings>					mSvrSettings;
	tPtr <DaSvrAudioOutput>					mSvrAudioOutput;
	tPtr <DaSvrSpeechInput>					mSvrSpeechInput;
	tPtr <DaSvrCommandsWindow>				mSvrCommandsWindow;
	tPtr <DaSvrCharacterFiles>				mSvrCharacterFiles;
	tPtr <CVoiceCommandsWnd>				mVoiceCommandsWnd;
	tPtr <class CComMessageFilter>			mMessageFilter;
	DWORD									mLastHotKey;
	UINT									mClientLifetimeTimer;
	CTimerNotifies							mTimerNotifies;
	CAtlPtrTypeArray <CAtlPropertySheet>	mModelessPropSheets;
	CActionTraceMap							mActionTraceMap;
};

extern CDaServerModule _AtlModule;

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF(IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF(IDaSvrBalloon, __uuidof(IDaSvrBalloon));
_COM_SMARTPTR_TYPEDEF(IDaSvrBalloon2, __uuidof(IDaSvrBalloon2));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommand, __uuidof(IDaSvrCommand));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommand2, __uuidof(IDaSvrCommand2));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommands, __uuidof(IDaSvrCommands));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommands2, __uuidof(IDaSvrCommands2));
_COM_SMARTPTR_TYPEDEF(IDaSvrUserInput, __uuidof(IDaSvrUserInput));
_COM_SMARTPTR_TYPEDEF(IDaSvrUserInput2, __uuidof(IDaSvrUserInput2));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommandsWindow, __uuidof(IDaSvrCommandsWindow));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommandsWindow2, __uuidof(IDaSvrCommandsWindow2));
_COM_SMARTPTR_TYPEDEF(IDaSvrSpeechInput, __uuidof(IDaSvrSpeechInput));
_COM_SMARTPTR_TYPEDEF(IDaSvrAudioOutput, __uuidof(IDaSvrAudioOutput));
_COM_SMARTPTR_TYPEDEF(IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));
_COM_SMARTPTR_TYPEDEF(IDaSvrPropertySheet2, __uuidof(IDaSvrPropertySheet2));
_COM_SMARTPTR_TYPEDEF(IDaSvrNotifySink, __uuidof(IDaSvrNotifySink));

_COM_SMARTPTR_TYPEDEF(IDaSvrSettings, __uuidof(IDaSvrSettings));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF(IDaSvrTTSEngine, __uuidof(IDaSvrTTSEngine));
_COM_SMARTPTR_TYPEDEF(IDaSvrTTSEngines, __uuidof(IDaSvrTTSEngines));
_COM_SMARTPTR_TYPEDEF(IDaSvrSREngine, __uuidof(IDaSvrSREngine));
_COM_SMARTPTR_TYPEDEF(IDaSvrSREngines, __uuidof(IDaSvrSREngines));
_COM_SMARTPTR_TYPEDEF(IDaSvrAnimationNames, __uuidof(IDaSvrAnimationNames));

/////////////////////////////////////////////////////////////////////////////
