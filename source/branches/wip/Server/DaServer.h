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
#ifndef DASERVER_H_INCLUDED_
#define DASERVER_H_INCLUDED_
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "DaCoreRes.h"
#include "DaServerOdl.h"
#include "DaGuid.h"
#include "DaServerLifetime.h"
#include "DaInternalNotify.h"
#include "DaError.h"
#include "AgentFileCache.h"
#include "TimerNotify.h"
#include "InterfaceMap.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef	_STRICT_COMPATIBILITY
#define	_STRICT_COMPATIBILITY
#endif
#ifndef	_TRACE_CHARACTER_ACTIONS
#define	_TRACE_CHARACTER_ACTIONS
#endif
/////////////////////////////////////////////////////////////////////////////

class CDaAgentPropertySheet;
class CDaAgentAudioOutputProperties;
class CDaAgentSpeechInputProperties;
class CDaAgentCommandWindow;
class CPropSheetCharSel;
class CVoiceCommandsWnd;
class CDaAgentCharacter;
class CDaSvrCharacterFiles;

class CDaServerApp : public CWinApp, public CAgentFileCache, public CDaServerLifetime, public IDaInternalNotify, protected ITimerNotifySink
{
public:
	CDaServerApp ();
	~CDaServerApp ();
	DECLARE_DYNAMIC (CDaServerApp)

// Attributes
	long								mNextCharID;
	CPtrTypeArray <interface IDaNotify>	mNotify;
	static const UINT					mOptionsChangedMsgId;
	static const UINT					mDefaultCharacterChangedMsgId;

// Operations
	bool ShowSettings (LPCTSTR pStartPage = NULL);

	CDaAgentPropertySheet * GetAgentPropertySheet (bool pCreate, LPCTSTR pClientMutexName = NULL);
	CDaAgentAudioOutputProperties * GetAgentAudioOutputProperties (bool pCreate, LPCTSTR pClientMutexName = NULL);
	CDaAgentSpeechInputProperties * GetAgentSpeechInputProperties (bool pCreate, LPCTSTR pClientMutexName = NULL);
	CDaAgentCommandWindow * GetAgentCommandWindow (bool pCreate, LPCTSTR pClientMutexName = NULL);
	CPropSheetCharSel * GetPropSheetCharSel (bool pCreate, LPCTSTR pClientMutexName = NULL);
	CDaSvrCharacterFiles * GetSvrCharacterFiles (bool pCreate, LPCTSTR pClientMutexName = NULL);

	CDaAgentCharacter * GetAppCharacter (long pCharID);
	CDaAgentCharacter * GetListenCharacter ();
	CVoiceCommandsWnd * GetVoiceCommandsWnd (bool pCreate, long pCharID = 0);
	void OnCharacterListening (long pCharID, bool pListening, long pCause);
	bool IsHotKeyStillPressed () const;

	bool AddTimerNotify (UINT_PTR pTimerId, DWORD pInterval, ITimerNotifySink * pNotifySink);
	bool DelTimerNotify (UINT_PTR pTimerId);
	bool HasTimerNotify (UINT_PTR pTimerId);
	CTimerNotify * GetTimerNotify (UINT_PTR pTimerId);

	bool TraceCharacterAction (long pCharID, LPCTSTR pAction, LPCTSTR pFormat = NULL, ...);

// Overrides
	//{{AFX_VIRTUAL(CDaServerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run ();
	virtual void _OnCharacterLoaded (long pCharID);
	virtual void _OnCharacterUnloaded (long pCharID);
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _OnOptionsChanged ();
	virtual void _OnDefaultCharacterChanged ();
	protected:
//	virtual BOOL PumpMessage();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual void OnTimerNotify (class CTimerNotify * pTimerNotify, UINT_PTR pTimerId);
	//}}AFX_VIRTUAL

// Implementation
public:
	//{{AFX_MSG(CDaServerApp)
	afx_msg void OnThreadHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBroadcastOptionsChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBroadcastDefaultCharacterChanged(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OnDeleteAgentPropertySheet (CDaAgentPropertySheet * pAgentPropertySheet);
	void OnDeleteAgentAudioOutputProperties (CDaAgentAudioOutputProperties * pAgentAudioOutputProperties);
	void OnDeleteAgentSpeechInputProperties (CDaAgentSpeechInputProperties * pAgentSpeechInputProperties);
	void OnDeleteAgentCommandWindow (CDaAgentCommandWindow * pAgentCommandWindow);
	void OnDeletePropSheetCharSel (CPropSheetCharSel * pPropSheetCharSel);
	void OnDeleteSvrCharacterFiles (CDaSvrCharacterFiles * pSvrCharacterFiles);

protected:
	void _InitInstance ();
	void _ExitInstance ();
	void RegisterServer ();
	void UnregisterServer ();
	void SetVoiceCommandClients (long pCharID);
	void SetVoiceCommandNames (long pCharID);
	bool StartActionTrace (long pCharID);
	bool StopActionTrace (long pCharID);

protected:
	tPtr <CDaAgentPropertySheet>			mAgentPropertySheet;
	tPtr <CDaAgentAudioOutputProperties>	mAgentAudioOutputProperties;
	tPtr <CDaAgentSpeechInputProperties>	mAgentSpeechInputProperties;
	tPtr <CDaAgentCommandWindow>			mAgentCommandWindow;
	tPtr <CPropSheetCharSel>				mPropSheetCharSel;
	tPtr <CDaSvrCharacterFiles>				mSvrCharacterFiles;
	tPtr <CVoiceCommandsWnd>				mVoiceCommandsWnd;
	DWORD									mLastHotKey;
	UINT									mClientLifetimeTimer;
	CTimerNotifies							mTimerNotifies;
	CStringMap <long>						mActionTraceLog;
};

/////////////////////////////////////////////////////////////////////////////

#define TheServerApp ((CDaServerApp *) AfxGetApp ())

extern const GUID gDaTypeLibId;
extern const WORD gDaTypeLibVerMajor;
extern const WORD gDaTypeLibVerMinor;

extern const GUID gDaMsTypeLibId;
extern const WORD gDaMsTypeLibVerMajor;
extern const WORD gDaMsTypeLibVerMinor;

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF(IDaSvrUserInput, __uuidof(IDaSvrUserInput));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommand, __uuidof(IDaSvrCommand));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommands, __uuidof(IDaSvrCommands));
_COM_SMARTPTR_TYPEDEF(IDaSvrSpeechInputProperties, __uuidof(IDaSvrSpeechInputProperties));
_COM_SMARTPTR_TYPEDEF(IDaSvrAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties));
_COM_SMARTPTR_TYPEDEF(IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));
_COM_SMARTPTR_TYPEDEF(IDaSvrBalloon, __uuidof(IDaSvrBalloon));
_COM_SMARTPTR_TYPEDEF(IDaSvrCommandWindow, __uuidof(IDaSvrCommandWindow));
_COM_SMARTPTR_TYPEDEF(IDaSvrNotifySink15, __uuidof(IDaSvrNotifySink15));
_COM_SMARTPTR_TYPEDEF(IDaSvrNotifySink, __uuidof(IDaSvrNotifySink));

_COM_SMARTPTR_TYPEDEF(IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF(IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF(IDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine));
_COM_SMARTPTR_TYPEDEF(IDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines));
_COM_SMARTPTR_TYPEDEF(IDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine));
_COM_SMARTPTR_TYPEDEF(IDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines));

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASERVER_H_INCLUDED_
