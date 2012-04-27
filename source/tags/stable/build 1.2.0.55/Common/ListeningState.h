/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "EventNotify.h"
#include "SapiInputCache.h"
#include "Sapi5Input.h"
#include "TimerNotify.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnCharacter;
class CAgentWnd;
class CAgentListeningWnd;
class CVoiceCommandsWnd;

/////////////////////////////////////////////////////////////////////////////

interface _IListeningAnchor
{
	virtual CVoiceCommandsWnd* GetVoiceCommandsWnd (bool pCreate, long pCharID = 0) = 0;
	virtual bool IsHotKeyStillPressed () const = 0;

	virtual bool AddListeningTimer (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink) = 0;
	virtual bool DelListeningTimer (UINT_PTR pTimerId) = 0;
	virtual bool HasListeningTimer (UINT_PTR pTimerId) = 0;
	virtual CTimerNotify* GetListeningTimer (UINT_PTR pTimerId) = 0;
};

/////////////////////////////////////////////////////////////////////////////

class CListeningState :
	public CSapiInputClient,
	protected _ITimerNotifySink,
	protected _IEventReflect,
	public CNotifySourcesOwner2 <CSapi5InputContext, _ISapi5InputEventSink, CListeningState>,
	public _ISapi5InputEventSink
{
public:
	CListeningState (CDaCmnCharacter& pCharacter);
	virtual ~CListeningState ();

// Attributes
public:
	bool IsManual () const;
	bool IsAutomatic () const;
	bool IsSuspended () const;
	bool IsActive () const;
	bool IsListening () const;
	bool IsHearing () const;

	long GetCharID () const;
	LANGID GetLangID () const;

// Operations
public:
	HRESULT StartListening (bool pManual);
	HRESULT StopListening (bool pManual, long pCause);
	HRESULT KeepListening (bool pManual);
	HRESULT SuspendListening (bool pSuspend);
	HRESULT TransferState (CListeningState * pToState);

	bool PlaySapiInputPrompt (bool pManualListen, LPCTSTR pSoundName);
	void SetSapiInputClients (long pCharID);
	void SetSapiInputNames (long pCharID);

// Overrides
public:
	virtual void OnSapi5InputEvent (const class CSpEvent& pEvent);
	virtual void OnTimerNotify (CTimerNotify* pTimerNotify, UINT_PTR pTimerId);
	virtual void _OnCharacterLoaded (long pCharID);
	virtual void _OnCharacterUnloaded (long pCharID);
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);

// Implementation
protected:
	HRESULT GetInputContext ();
	HRESULT StartInputContext (CSapi5InputContext* pPrevInputContext = NULL);
	HRESULT ShowListeningTip (bool pShow, bool pListening, LPCTSTR pReason = NULL);

	void StartListenTimers (bool pManual);
	void StopListenTimers ();
	void GrabListenTimers (CListeningState& pFromState);

protected:
	CDaCmnCharacter&			mCharacter;
	CSapi5Input*				mSapi5Input;
	tPtr <CSapi5InputContext>	mSapi5InputContext;
	bool						mHearingStateShown;
	bool						mListenSuspended;
	const DWORD					mListenDelayManual;
	const DWORD					mListenDelayHeard;
	UINT_PTR					mListenTimerManual;
	UINT_PTR					mListenTimerAuto;
	UINT_PTR					mListenTimerHotkey;
	UINT_PTR					mListenTimerHeard;
	static const UINT_PTR		mListenTimerIdManual;
	static const UINT_PTR		mListenTimerIdAuto;
	static const UINT_PTR		mListenTimerIdHotkey;
	static const UINT_PTR		mListenTimerIdHeard;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CListeningAnchor :
	public _IListeningAnchor
{
public:
	CListeningAnchor (class CListeningGlobal& pGlobal);
	virtual ~CListeningAnchor ();

// Attributes
public:
	class CListeningGlobal&	mGlobal;
	CWindow*					mOwnerWnd;

	bool IsStarted () const {return mStarted;}

// Overrides
public:
	void Startup (HWND pHotKeyWnd);
	void Shutdown ();

	virtual CVoiceCommandsWnd* GetVoiceCommandsWnd (bool pCreate, long pCharID = 0);
	virtual bool IsHotKeyStillPressed () const;

	virtual bool AddListeningTimer (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink);
	virtual bool DelListeningTimer (UINT_PTR pTimerId);
	virtual bool HasListeningTimer (UINT_PTR pTimerId);
	virtual CTimerNotify* GetListeningTimer (UINT_PTR pTimerId);

// Implementation
protected:
	bool AddTimerNotify (HWND pTimerWnd, UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink);
	bool DelTimerNotify (HWND pTimerWnd, UINT_PTR pTimerId);

protected:
	CTimerNotifies	mTimerNotifies;
	HWND			mHotKeyWnd;
	bool			mStarted;
};

/////////////////////////////////////////////////////////////////////////////

class CListeningGlobal : public virtual _IEventNotify
{
public:
	CListeningGlobal (class CGlobalAnchor& pAnchor);
	virtual ~CListeningGlobal ();

// Attributes
public:
	class CGlobalAnchor&	mAnchor;

	bool IsStarted () const {return mStarted;}
	bool IsSuspended () const {return mSuspended;}

// Operations
public:
	void Startup ();
	void Shutdown ();
	void Suspend ();
	void Resume ();

	CVoiceCommandsWnd* GetVoiceCommandsWnd (bool pCreate = false, CWindow* pOwnerWnd = NULL);

	void SetVoiceCommandCharacter (long pCharID);
	void SetVoiceCommandClients (long pCharID);
	void SetVoiceCommandNames (long pCharID);

	bool IsHotKeyStillPressed () const;

// Overrides
public:
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _AppActivated (bool pActive);
	virtual void _OptionsChanged ();

// Implementation
protected:
	bool RegisterHotKey (HWND pHotKeyWnd);
	bool UnregisterHotKey (HWND pHotKeyWnd);
	void RegisterHotKeys ();
	void UnregisterHotKeys ();

public:
	void AddHotKeyWnd (HWND pHotKeyWnd);
	void RemoveHotKeyWnd (HWND pHotKeyWnd);
	bool OnHotKey (WPARAM wParam, LPARAM lParam);

protected:
	tPtr <CVoiceCommandsWnd>	mVoiceCommandsWnd;
	CAtlArrayEx <HWND>			mHotKeyWnds;
	DWORD						mLastHotKey;
	bool						mStarted;
	bool						mSuspended;
	static const int			mHotKeyRegisterId;
};

/////////////////////////////////////////////////////////////////////////////
