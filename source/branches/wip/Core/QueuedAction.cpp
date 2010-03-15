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
#include "StdAfx.h"
#include "DaCore.h"
#include "QueuedAction.h"
#include "AgentBalloonWnd.h"
#include "Sapi5Voice.h"
#include "SapiVoiceCache.h"
#include "..\Server\DaAgentNotify.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_DEBUG_REQUESTS	(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_OPS	(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#endif

#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS	LogVerbose
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CQueuedAction, CObject)

CQueuedAction::CQueuedAction (QueueAction pAction, long pCharID, long pReqID)
:	mAction (pAction), mCharID (pCharID), mReqID (pReqID), mStarted (false)
{
}

CQueuedAction::~CQueuedAction ()
{
}

//////////////////////////////////////////////////////////////////////

void CQueuedAction::NotifyStarted (IDaNotify * pNotify)
{
	if	(
			(mReqID > 0)
		&&	(pNotify != NULL)
		&&	(pNotify->_GetNotifyClient (mCharID) == mCharID)
		)
	{
#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestStart    [%d]"), mCharID, mReqID);
#endif
		pNotify->RequestStart (mReqID);
	}
}

void CQueuedAction::NotifyStarted (CPtrTypeArray <IDaNotify> & pNotify)
{
	mStarted = true;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive())
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%d] Action [%d] started"), mCharID, mReqID);
	}
#endif

	if	(mReqID > 0)
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d] NotifyStarted   [%d] [%d]"), mCharID, mReqID, pNotify.GetSize());
#endif
		for	(lNotifyNdx = 0; lNotify = pNotify (lNotifyNdx); lNotifyNdx++)
		{
			NotifyStarted (lNotify);
		}
	}
}

void CQueuedAction::NotifyComplete (IDaNotify * pNotify, HRESULT pReqStatus)
{
	if	(
			(mReqID > 0)
		&&	(pNotify != NULL)
		&&	(pNotify->_GetNotifyClient (mCharID) == mCharID)
		)
	{
#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestComplete [%d] [%8.8X]"), mCharID, mReqID, pReqStatus);
#endif
		pNotify->RequestComplete (mReqID, pReqStatus);
	}
}

void CQueuedAction::NotifyComplete (CPtrTypeArray <IDaNotify> & pNotify, HRESULT pReqStatus)
{
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive())
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%d] Action [%d] complete [%8.8X]"), mCharID, mReqID, pReqStatus);
	}
#endif

	if	(mReqID > 0)
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d] NotifyComplete  [%d] [%d]"), mCharID, mReqID, pNotify.GetSize());
#endif
		for	(lNotifyNdx = 0; lNotify = pNotify (lNotifyNdx); lNotifyNdx++)
		{
			NotifyComplete (lNotify, pReqStatus);
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedThink::CQueuedThink (long pCharID, long pReqID)
:	CQueuedAction (QueueActionThink, pCharID, pReqID)
{
}

CQueuedThink::~CQueuedThink ()
{
}

//////////////////////////////////////////////////////////////////////

CQueuedSpeak::CQueuedSpeak (class CSapiVoice * pVoice, bool pShowBalloon, long pCharID, long pReqID)
:	CQueuedAction (QueueActionSpeak, pCharID, pReqID),
	mVoice (NULL),
	mShowBalloon (pShowBalloon),
	mAnimated (false)
{
	SetVoice (pVoice);
}

CQueuedSpeak::~CQueuedSpeak ()
{
	SetVoice (NULL);
}

bool CQueuedSpeak::SetVoice (class CSapiVoice * pVoice)
{
	bool	lRet = false;

	try
	{
		CSapiVoiceCache *	lVoiceCache;
		CSapi5Voice *		lSapi5Voice;

		if	(
				(
					(!mStarted)
				||	(!pVoice)
				)
			&&	(mVoice != pVoice)
			&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			)
		{
			if	(mVoice)
			{
				try
				{
					if	(lSapi5Voice = DYNAMIC_DOWNCAST (CSapi5Voice, mVoice))
					{
						lSapi5Voice->ClearEventSinks ();
					}
				}
				catch AnyExceptionSilent

				lVoiceCache->RemoveVoiceClient (mVoice, this);
			}
			if	(mVoice = pVoice)
			{
				if	(!lVoiceCache->CacheVoice (mVoice, this))
				{
					lVoiceCache->AddVoiceClient (mVoice, this);
				}
			}
			mText.SetSapiVersion (mVoice->SafeIsValid ());
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
