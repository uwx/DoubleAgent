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
#include "ServerNotifySink.h"
#include "SapiInputCache.h"
#include "Sapi5InputEventSink.h"
#include "TimerNotify.h"

/////////////////////////////////////////////////////////////////////////////

class DaSvrCharacter;
class CAgentWnd;
class CAgentListeningWnd;
class CSapi5Input;
class CSapi5InputContext;

class CListeningState :
	public CSapiInputClient,
	protected ISapi5InputEventSink,
	protected _ITimerNotifySink,
	protected _IServerNotifySink
{
public:
	CListeningState (DaSvrCharacter & pCharacter);
	virtual ~CListeningState ();

// Attributes
public:
	bool IsManual () const;
	bool IsAutomatic () const;
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
	HRESULT TransferState (CListeningState * pToState);

	bool PlaySapiInputPrompt (bool pManualListen, LPCTSTR pSoundName);
	void SetSapiInputClients (long pCharID);
	void SetSapiInputNames (long pCharID);

// Overrides
public:
	virtual void OnSapi5InputEvent (const class CSpEvent & pEvent);
	virtual void OnTimerNotify (CTimerNotify * pTimerNotify, UINT_PTR pTimerId);
	virtual void _OnCharacterLoaded (long pCharID);
	virtual void _OnCharacterUnloaded (long pCharID);
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);

// Implementation
protected:
	HRESULT GetInputContext ();
	HRESULT StartInputContext (CSapi5InputContext * pPrevInputContext = NULL);
	HRESULT ShowListeningTip (bool pShow, bool pListening, LPCTSTR pReason = NULL);

	void StartListenTimers (bool pManual);
	void StopListenTimers ();
	void GrabListenTimers (CListeningState & pFromState);

protected:
	DaSvrCharacter &			mCharacter;
	CSapi5Input *				mSapi5Input;
	tPtr <CSapi5InputContext>	mSapi5InputContext;
	bool						mHearingStateShown;
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
