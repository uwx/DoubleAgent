/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "AgentFile.h"
#include "SapiVoiceCache.h"

/////////////////////////////////////////////////////////////////////////////

enum QueueAction
{
	QueueActionState,
	QueueActionGesture,
	QueueActionShow,
	QueueActionHide,
	QueueActionMove,
	QueueActionThink,
	QueueActionSpeak,
	QueueActionWait,
	QueueActionInterrupt,
	QueueActionPrepare
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedAction
{
public:
#ifdef	_TRACE_ACTION_INSTANCE
	static CAtlPtrTypeArray<CQueuedAction>	mInstances;
#endif
	CQueuedAction (QueueAction pAction, long pCharID, long pReqID = -1);
	virtual ~CQueuedAction ();

// Attributes
public:
	QueueAction	mAction;
	long		mCharID;
	long		mReqID;
	bool		mStarted:2;
	bool		mPaused:2;

// Operations
public:
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd) = 0;
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause) = 0;
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL) = 0;

	void NotifyStarted (class CEventNotify * pNotify);
	void NotifyStarted (CAtlPtrTypeArray <class CEventNotify> & pNotify);
	void NotifyComplete (class CEventNotify * pNotify, HRESULT pReqStatus = S_OK);
	void NotifyComplete (CAtlPtrTypeArray <class CEventNotify> & pNotify, HRESULT pReqStatus = S_OK);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
protected:
	void _LogAction (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pActionType, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedState : public CQueuedAction
{
public:
	CQueuedState (LPCTSTR pStateName, long pCharID, long pReqID = -1);
	virtual ~CQueuedState ();

// Attributes
public:
	CAtlString		mStateName;
	CAtlStringArray	mGestures;

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedGesture : public CQueuedAction
{
public:
	CQueuedGesture (LPCTSTR pGestureName, long pCharID, long pReqID = -1);
	virtual ~CQueuedGesture ();

// Attributes
public:
	CAtlString	mStateName;
	CAtlString	mGestureName;

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedShow : public CQueuedAction
{
public:
	CQueuedShow (long pCharID, long pReqID = -1);
	virtual ~CQueuedShow ();

// Attributes
public:
	bool	mFast;
	int		mVisibilityCause;
	bool	mAnimationShown;

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedHide : public CQueuedAction
{
public:
	CQueuedHide (long pCharID, long pReqID = -1);
	virtual ~CQueuedHide ();

// Attributes
public:
	bool	mFast;
	int		mVisibilityCause;
	bool	mAnimationShown;

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedMove : public CQueuedAction
{
public:
	CQueuedMove (long pCharID, long pReqID = -1);
	virtual ~CQueuedMove ();

// Attributes
public:
	bool			mAnimationShown;
	CAtlString		mAnimationState;
	bool			mEndAnimationShown;
	CPoint			mPosition;
	tPtr <CPoint>	mMoveStarted;
	DWORD			mTimeStarted;
	DWORD			mTimeAllowed;
	DWORD			mTimePaused;

// Operations
	bool IsCycling () const;
	bool Cycle (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedThink : public CQueuedAction
{
public:
	CQueuedThink (class CAgentBalloonOptions * pBalloonOptions, long pCharID, long pReqID = -1);
	virtual ~CQueuedThink ();

// Attributes
public:
	tPtr <class CAgentText>				mText;
	class CAgentTextObject *			mTextObject;
	IUnknownPtr							mTextObjectRef;
	tPtr <class CAgentBalloonOptions>	mBalloonOptions;

// Operations
	void Initialize (class CAgentText & pText);
	void Initialize (class CAgentTextObject * pTextObject);

	CAtlString GetFullText ();

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedSpeak : public CQueuedAction, public CSapiVoiceClient
{
public:
	CQueuedSpeak (class CAgentBalloonOptions * pBalloonOptions, long pCharID, long pReqID = -1);
	virtual ~CQueuedSpeak ();

// Attributes
public:
	class CSapiVoice *					mVoice;
	tPtr <class CAgentText>				mText;
	class CAgentTextObject *			mTextObject;
	IUnknownPtr							mTextObjectRef;
	CAtlString							mSoundUrl;
	tPtr <class CAgentBalloonOptions>	mBalloonOptions;
	bool								mAnimated;
	IUnknownPtr							mSoundFilter;

	bool ShowBalloon () const {return !!mBalloonOptions;}

// Operations
	void Initialize (class CAgentText & pText, class CSapiVoice * pVoice);
	void Initialize (class CAgentTextObject * pTextObject, class CSapiVoice * pVoice);
	bool SetVoice (class CSapiVoice * pVoice);

	CAtlString GetFullText ();
	CAtlString GetSpeechText ();

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

// Implementation
protected:
	bool SpeechIsBusy (class CAgentWnd * pAgentWnd);
	HRESULT SpeechIsReady (class CAgentWnd * pAgentWnd);
	HRESULT PrepareSpeech (class CAgentWnd * pAgentWnd);
	HRESULT StartSpeech (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	bool ShowSpeechAnimation (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;

protected:
	ULONG	mVoiceNotifyId [2];
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedWait : public CQueuedAction
{
public:
	CQueuedWait (long pOtherCharID, long pOtherReqID, long pCharID, long pReqID = -1);
	virtual ~CQueuedWait ();

// Attributes
public:
	long	mOtherCharID;
	long	mOtherReqID;

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedInterrupt : public CQueuedAction
{
public:
	CQueuedInterrupt (long pOtherCharID, long pOtherReqID, long pCharID, long pReqID = -1);
	virtual ~CQueuedInterrupt ();

// Attributes
public:
	long	mOtherCharID;
	long	mOtherReqID;

// Overrides
	virtual bool Advance (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd);
	virtual bool Pause (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, bool pPause);
	virtual bool Abort (class CQueuedActions & pQueue, class CAgentWnd * pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	virtual void LogAction (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
};

/////////////////////////////////////////////////////////////////////////////
