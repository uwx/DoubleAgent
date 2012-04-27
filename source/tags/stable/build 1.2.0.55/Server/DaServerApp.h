/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "DaVersion.h"
#include "DaError.h"
#include "ServerLifetime.h"
#include "AgentAnchor.h"
#include "EventNotify.h"
#include "TimerNotify.h"
#include "PropertySheet.h"
#include "ListeningState.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef	_TRACE_CHARACTER_ACTIONS
#define	_TRACE_CHARACTER_ACTIONS
#endif
/////////////////////////////////////////////////////////////////////////////

class DaServer;
class DaSvrPropertySheet;
class DaSvrSettings;
class DaSvrAudioOutput;
class DaSvrSpeechInput;
class DaSvrCommandsWindow;
class DaSvrCharacter;
class DaSvrCharacterFiles;

#pragma warning (push)
#pragma warning (disable: 4250 4584)
/////////////////////////////////////////////////////////////////////////////

class CDaServerModule : public CAtlExeModuleT<CDaServerModule>,
	public CServerLifetime,
	public CGlobalAnchor,
	public CEventGlobal,
	public CListeningGlobal,
	public CListeningAnchor,
	public _IEventNotify,
	public _AtlPropSheetOwner,
	protected _ITimerNotifySink
{
public:
	CDaServerModule();
	~CDaServerModule();

	DECLARE_LIBID(__uuidof(DoubleAgentSvr_TypeLib))
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DASERVERAPP, "{1147E500-A208-11DE-ABF2-002421116FB2}")

// Attributes
	static const UINT	mOptionsChangedMsgId;
	static const UINT	mDefaultCharacterChangedMsgId;

	class CComMessageFilter* GetMessageFilter ();

// Operations
	bool ShowSettings (LPCTSTR pStartPage = NULL);

	DaSvrPropertySheet* GetSvrPropertySheet (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrSettings* GetSvrSettings (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrAudioOutput* GetSvrAudioOutput (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrSpeechInput* GetSvrSpeechInput (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrCommandsWindow* GetSvrCommandsWindow (bool pCreate, LPCTSTR pClientMutexName = NULL);
	DaSvrCharacterFiles* GetSvrCharacterFiles (bool pCreate, LPCTSTR pClientMutexName = NULL);

	bool TraceCharacterAction (long pCharID, LPCTSTR pAction, LPCTSTR pFormat = NULL, ...);

// Overrides
public:
	HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL);
	HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);
	HRESULT RegisterAppId ();
	HRESULT UnregisterAppId ();
	bool ParseCommandLine (LPCTSTR lpCmdLine, HRESULT* pnRetCode);
	HRESULT PreMessageLoop (int nShowCmd);
	HRESULT PostMessageLoop ();
	void RunMessageLoop ();
	static void UninitializeCom ();
public:
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _AppActivated (bool pActive);
	virtual void _OptionsChanged ();
protected:
	virtual void OnTimerNotify (class CTimerNotify* pTimerNotify, UINT_PTR pTimerId);
	virtual void OnShowModelessPropertySheet (class CAtlPropertySheet* pPropertySheet);
	virtual void OnHideModelessPropertySheet (class CAtlPropertySheet* pPropertySheet);

// Implementation
public:
	BEGIN_MSG_MAP(CDaServerModule)
		MESSAGE_HANDLER(WM_HOTKEY, OnThreadHotKey)
		MESSAGE_HANDLER(mOptionsChangedMsgId, OnBroadcastOptionsChanged)
		MESSAGE_HANDLER(mDefaultCharacterChangedMsgId, OnBroadcastDefaultCharacterChanged)
	END_MSG_MAP()

	LRESULT OnThreadHotKey (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBroadcastOptionsChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBroadcastDefaultCharacterChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	void OnDeleteSvrPropertySheet (DaSvrPropertySheet* pSvrPropertySheet);
	void OnDeleteSvrSettings (DaSvrSettings* pSvrSettings);
	void OnDeleteSvrAudioOutput (DaSvrAudioOutput* pSvrAudioOutput);
	void OnDeleteSvrSpeechInput (DaSvrSpeechInput* pSvrSpeechInput);
	void OnDeleteSvrCommandsWindow (DaSvrCommandsWindow* pSvrCommandsWindow);
	void OnDeleteSvrCharacterFiles (DaSvrCharacterFiles* pSvrCharacterFiles);

protected:
	void _PreMessageLoop (bool pForModal);
	void _PostMessageLoop (bool pForModal);
	bool StartActionTrace (long pCharID);
	bool StopActionTrace (long pCharID);

protected:
	typedef CAtlMap <long, CAtlString, CPrimitiveElementTraits<long>, CStringElementTraitsI<CAtlString> > CActionTraceMap;

	tPtr <DaSvrPropertySheet>				mSvrPropertySheet;
	tPtr <DaSvrSettings>					mSvrSettings;
	tPtr <DaSvrAudioOutput>					mSvrAudioOutput;
	tPtr <DaSvrSpeechInput>					mSvrSpeechInput;
	tPtr <DaSvrCommandsWindow>				mSvrCommandsWindow;
	tPtr <DaSvrCharacterFiles>				mSvrCharacterFiles;
	tPtr <class CComMessageFilter>			mMessageFilter;
	UINT									mClientLifetimeTimer;
	CAtlPtrTypeArray <CAtlPropertySheet>	mModelessPropSheets;
	CActionTraceMap							mActionTraceMap;
};

extern CDaServerModule _AtlModule;
extern int LogCrashCode (unsigned int pCode, LPCSTR pFile, UINT pLine, int pAction);

#import "mscorlib.tlb" rename("ReportEvent","CorReportEvent") no_smart_pointers raw_interfaces_only raw_method_prefix("cor_") raw_property_prefixes("corget_","corput_","corputref_")

/////////////////////////////////////////////////////////////////////////////
#pragma warning (pop)

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
_COM_SMARTPTR_TYPEDEF(IDaSvrNotifySink2, __uuidof(IDaSvrNotifySink2));

_COM_SMARTPTR_TYPEDEF(IDaSvrSettings, __uuidof(IDaSvrSettings));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF(IDaSvrFormattedText, __uuidof(IDaSvrFormattedText));
_COM_SMARTPTR_TYPEDEF(IDaSvrTTSEngine, __uuidof(IDaSvrTTSEngine));
_COM_SMARTPTR_TYPEDEF(IDaSvrTTSEngines, __uuidof(IDaSvrTTSEngines));
_COM_SMARTPTR_TYPEDEF(IDaSvrSREngine, __uuidof(IDaSvrSREngine));
_COM_SMARTPTR_TYPEDEF(IDaSvrSREngines, __uuidof(IDaSvrSREngines));

/////////////////////////////////////////////////////////////////////////////

#ifndef	_DEBUG
#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif
#ifndef	_LOG_ABANDONED
#define	_LOG_ABANDONED	LogDetails
#endif
#ifndef	_LOG_RESULTS
#define	_LOG_RESULTS	LogNormal
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
