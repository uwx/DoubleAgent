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
#ifndef QUEUEDACTION_H_INCLUDED_
#define QUEUEDACTION_H_INCLUDED_
#pragma once

#include "AgentFile.h"
#include "AgentText.h"

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

class CQueuedAction : public CObject
{
public:
	CQueuedAction (QueueAction pAction, long pCharID, long pReqID = -1);
	virtual ~CQueuedAction ();
	DECLARE_DYNAMIC (CQueuedAction)

// Attributes
public:
	QueueAction	mAction;
	long		mCharID;
	long		mReqID;
	bool		mStarted;

// Operations
public:
	void NotifyStarted (interface IDaNotify * pNotify);
	void NotifyStarted (CPtrTypeArray <interface IDaNotify> & pNotify);
	void NotifyComplete (interface IDaNotify * pNotify, HRESULT pReqStatus = S_OK);
	void NotifyComplete (CPtrTypeArray <interface IDaNotify> & pNotify, HRESULT pReqStatus = S_OK);

// Overrides
	//{{AFX_VIRTUAL(CQueuedAction)
	//}}AFX_VIRTUAL

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
	CString			mStateName;
	CStringArray	mGestures;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedGesture : public CQueuedAction
{
public:
	CQueuedGesture (LPCTSTR pGestureName, long pCharID, long pReqID = -1) : mGestureName (pGestureName), CQueuedAction (QueueActionGesture, pCharID, pReqID) {}

// Attributes
public:
	CString	mStateName;
	CString	mGestureName;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedShow : public CQueuedAction
{
public:
	CQueuedShow (long pCharID, long pReqID = -1) : CQueuedAction (QueueActionShow, pCharID, pReqID), mFast (false), mAnimationShown (false) {}

// Attributes
public:
	bool	mFast;
	bool	mAnimationShown;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedHide : public CQueuedAction
{
public:
	CQueuedHide (long pCharID, long pReqID = -1) : CQueuedAction (QueueActionHide, pCharID, pReqID), mFast (false), mAnimationShown (false) {}

// Attributes
public:
	bool	mFast;
	bool	mAnimationShown;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedMove : public CQueuedAction
{
public:
	CQueuedMove (long pCharID, long pReqID = -1) : CQueuedAction (QueueActionMove, pCharID, pReqID), mAnimationShown (false), mEndAnimationShown (false), mMoveStarted (false) {}

// Attributes
public:
	bool	mAnimationShown;
	CString	mAnimationState;
	bool	mEndAnimationShown;
	CPoint	mPosition;
	tPtr <CPoint>	mMoveStarted;
	DWORD	mTimeStarted;
	DWORD	mTimeAllowed;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedThink : public CQueuedAction
{
public:
	CQueuedThink (long pCharID, long pReqID = -1);
	virtual ~CQueuedThink ();

// Attributes
public:
	CString	mText;
	tPtr <struct CAgentBalloonOptions>	mBalloonOptions;
};

/////////////////////////////////////////////////////////////////////////////

class CQueuedSpeak : public CQueuedAction
{
public:
	CQueuedSpeak (class CSapiVoice * pVoice, bool pShowBalloon, long pCharID, long pReqID = -1);
	virtual ~CQueuedSpeak ();

// Attributes
public:
	class CSapiVoice *	mVoice;
	CAgentText			mText;
	CString				mSoundUrl;
	bool								mShowBalloon;
	tPtr <struct CAgentBalloonOptions>	mBalloonOptions;
	bool				mAnimated;
	tPtr <CObject>		mSoundFilter;

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

#endif // QUEUEDACTION_H_INCLUDED_
