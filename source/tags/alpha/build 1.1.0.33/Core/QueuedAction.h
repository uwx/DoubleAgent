/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "AgentText.h"
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
	CQueuedAction (QueueAction pAction, long pCharID, long pReqID = -1);
	virtual ~CQueuedAction ();

// Attributes
public:
	QueueAction	mAction;
	long		mCharID;
	long		mReqID;
	bool		mStarted;

// Operations
public:
	void NotifyStarted (class CEventNotify * pNotify);
	void NotifyStarted (CAtlPtrTypeArray <class CEventNotify> & pNotify);
	void NotifyComplete (class CEventNotify * pNotify, HRESULT pReqStatus = S_OK);
	void NotifyComplete (CAtlPtrTypeArray <class CEventNotify> & pNotify, HRESULT pReqStatus = S_OK);

// Overrides

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedState : public CQueuedAction
{
public:
	CQueuedState (LPCTSTR pStateName, long pCharID, long pReqID = -1) : mStateName (pStateName), CQueuedAction (QueueActionState, pCharID, pReqID) {}

// Attributes
public:
	CAtlString		mStateName;
	CAtlStringArray	mGestures;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedGesture : public CQueuedAction
{
public:
	CQueuedGesture (LPCTSTR pGestureName, long pCharID, long pReqID = -1) : mGestureName (pGestureName), CQueuedAction (QueueActionGesture, pCharID, pReqID) {}

// Attributes
public:
	CAtlString	mStateName;
	CAtlString	mGestureName;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedShow : public CQueuedAction
{
public:
	CQueuedShow (long pCharID, long pReqID = -1) : CQueuedAction (QueueActionShow, pCharID, pReqID), mFast (false), mVisibilityCause (-1), mAnimationShown (false) {}

// Attributes
public:
	bool	mFast;
	int		mVisibilityCause;
	bool	mAnimationShown;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedHide : public CQueuedAction
{
public:
	CQueuedHide (long pCharID, long pReqID = -1) : CQueuedAction (QueueActionHide, pCharID, pReqID), mFast (false), mVisibilityCause (-1), mAnimationShown (false) {}

// Attributes
public:
	bool	mFast;
	int		mVisibilityCause;
	bool	mAnimationShown;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedMove : public CQueuedAction
{
public:
	CQueuedMove (long pCharID, long pReqID = -1) : CQueuedAction (QueueActionMove, pCharID, pReqID), mAnimationShown (false), mEndAnimationShown (false), mMoveStarted (false) {}

// Attributes
public:
	bool			mAnimationShown;
	CAtlString		mAnimationState;
	bool			mEndAnimationShown;
	CPoint			mPosition;
	tPtr <CPoint>	mMoveStarted;
	DWORD			mTimeStarted;
	DWORD			mTimeAllowed;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedThink : public CQueuedAction
{
public:
	CQueuedThink (long pCharID, long pReqID = -1);
	virtual ~CQueuedThink ();

// Attributes
public:
	CAtlString							mText;
	tPtr <struct CAgentBalloonOptions>	mBalloonOptions;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedSpeak : public CQueuedAction, public CSapiVoiceClient
{
public:
	CQueuedSpeak (class CSapiVoice * pVoice, bool pShowBalloon, long pCharID, long pReqID = -1);
	virtual ~CQueuedSpeak ();

// Attributes
public:
	class CSapiVoice *					mVoice;
	CAgentTextParse						mText;
	CAtlString							mSoundUrl;
	bool								mShowBalloon;
	tPtr <struct CAgentBalloonOptions>	mBalloonOptions;
	bool								mAnimated;
	IUnknownPtr							mSoundFilter;

// Operations
	bool SetVoice (class CSapiVoice * pVoice);
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedWait : public CQueuedAction
{
public:
	CQueuedWait (long pOtherCharID, long pOtherReqID, long pCharID, long pReqID = -1) : CQueuedAction (QueueActionWait, pCharID, pReqID), mOtherCharID (pOtherCharID), mOtherReqID (pOtherReqID) {}

// Attributes
public:
	long	mOtherCharID;
	long	mOtherReqID;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedInterrupt : public CQueuedAction
{
public:
	CQueuedInterrupt (long pOtherCharID, long pOtherReqID, long pCharID, long pReqID = -1) : CQueuedAction (QueueActionInterrupt, pCharID, pReqID), mOtherCharID (pOtherCharID), mOtherReqID (pOtherReqID) {}

// Attributes
public:
	long	mOtherCharID;
	long	mOtherReqID;
};

/////////////////////////////////////////////////////////////////////////////
