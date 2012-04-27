/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "QueuedActions.h"
#include "FileDownload.h"
#include "AgentCharacterWnd.h"
#include "AgentBalloonWnd.h"
#include "AgentStreamInfo.h"
#include "AgentTextObject.h"
#include "DirectSoundLipSync.h"
#include "EventNotify.h"
#include "DaGlobalConfig.h"
#include "SapiVoiceCache.h"
#include "Sapi5Voice.h"
#include "Sapi5Err.h"
#ifndef	_WIN64
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#endif
#include "Registry.h"
#include "DebugStr.h"
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_REQUESTS		(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_SPEECH		(GetProfileDebugInt(_T("DebugSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_SPEECH			_DEBUG_SPEECH
#define	_LOG_QUEUE_OPS		(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
//#define	_TRACE_BUSY_TIME	LogIfActive|LogTimeMs|LogHighVolume
#endif

#ifndef	_LOG_SPEECH
#define	_LOG_SPEECH		LogNormal
#endif

#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS	LogDetails
#endif

#ifdef	_TRACE_BUSY_TIME
#include "Elapsed.h"
CAtlMap <const CQueuedSpeak*, DWORD>	sBusyStart;
#endif

//////////////////////////////////////////////////////////////////////

CQueuedSpeak::CQueuedSpeak (CAgentBalloonOptions* pBalloonOptions, long pCharID, long pReqID)
:	CQueuedAction (QueueActionSpeak, pCharID, pReqID),
	mVoice (NULL),
	mAnimated (false)
{
	mVoiceNotifyId[0] = 0;
	mVoiceNotifyId[1] = 0;
	if	(pBalloonOptions)
	{
		mBalloonOptions = new CAgentBalloonOptions (*pBalloonOptions);
	}
}

CQueuedSpeak::~CQueuedSpeak ()
{
	CDirectSoundLipSync *	lLipSync;

#ifdef	_TRACE_BUSY_TIME
	sBusyStart.RemoveKey (this);
#endif
	SetVoice (NULL);

	if	(lLipSync = static_cast <CDirectSoundLipSync *> (mSoundFilter.GetInterfacePtr()))
	{
		try
		{
			lLipSync->Stop ();
			lLipSync->Disconnect ();
		}
		catch AnyExceptionSilent
	}
	SafeFreeSafePtr (mSoundFilter);

	if	(mTextObject)
	{
		try
		{
			mTextObject->Detach (true);
		}
		catch AnyExceptionSilent
	}
	SafeFreeSafePtr (mTextObjectRef);
}

//////////////////////////////////////////////////////////////////////

void CQueuedSpeak::Initialize (CAgentText& pText, CSapiVoice* pVoice)
{
	if	(mText = new CAgentText (pText.GetSapiVersion ()))
	{
		mText->operator= (pText);
	}
	mTextObject = NULL;
	mTextObjectRef = mTextObject;

	if	(GetFullText ().IsEmpty ())
	{
		mBalloonOptions = NULL;
	}
	SetVoice (pVoice);
}

void CQueuedSpeak::Initialize (CAgentTextObject* pTextObject, CSapiVoice* pVoice)
{
	mTextObject = pTextObject;
	mTextObjectRef = pTextObject;
	mText = NULL;

	if	(GetFullText ().IsEmpty ())
	{
		mBalloonOptions = NULL;
	}
	SetVoice (pVoice);
}

bool CQueuedSpeak::SetVoice (CSapiVoice* pVoice)
{
	bool	lRet = false;

	try
	{
		CSapiVoiceCache *	lVoiceCache;

		if	(
				(
					(!mStarted)
				||	(!pVoice)
				)
			&&	(mVoice != pVoice)
			&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			)
		{
			_ISapiVoiceEventSink *				lVoiceNotifySink [2] = {NULL, NULL};
			CNotifySourcesOwner <CSapiVoice>*	lVoiceNotifySources [2] = {NULL, NULL};

			if	(mVoice)
			{
				if	(mVoiceNotifyId[0])
				{
					if	(lVoiceNotifySink[0] = mVoice->FindNotifySink (mVoiceNotifyId[0]))
					{
						lVoiceNotifySources[0] = mVoice->GetNotifySources (lVoiceNotifySink[0]);
					}
					mVoiceNotifyId[0] = 0;
				}
				if	(mVoiceNotifyId[1])
				{
					if	(lVoiceNotifySink[1] = mVoice->FindNotifySink (mVoiceNotifyId[1]))
					{
						lVoiceNotifySources[1] = mVoice->GetNotifySources (lVoiceNotifySink[1]);
					}
					mVoiceNotifyId[1] = 0;
				}
				if	(
						(lVoiceNotifySink[0])
					||	(lVoiceNotifySink[1])
					)
				{
					mVoice->Stop ();
				}
				if	(lVoiceNotifySink[0])
				{
					mVoice->RemoveNotifySink (lVoiceNotifySink[0]);
				}
				if	(lVoiceNotifySink[1])
				{
					mVoice->RemoveNotifySink (lVoiceNotifySink[1]);
				}
				lVoiceCache->RemoveVoiceClient (mVoice, this);
			}
			if	(mVoice = pVoice)
			{
				if	(!lVoiceCache->CacheVoice (mVoice, this))
				{
					lVoiceCache->AddVoiceClient (mVoice, this);
				}
				if	(lVoiceNotifySink[0])
				{
					mVoiceNotifyId[0] = pVoice->AddNotifySink (lVoiceNotifySink[0], lVoiceNotifySources[0]);
				}
				if	(lVoiceNotifySink[1])
				{
					mVoiceNotifyId[1] = pVoice->AddNotifySink (lVoiceNotifySink[1], lVoiceNotifySources[1]);
				}
			}
			if	(mText)
			{
				mText->SetSapiVersion (mVoice->SafeIsValid ());
			}
			if	(mTextObject)
			{
				mTextObject->mText.SetSapiVersion (mVoice->SafeIsValid ());
			}
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CAtlString CQueuedSpeak::GetFullText ()
{
	if	(mText)
	{
		return mText->GetFullText ();
	}
	else
	if	(mTextObject)
	{
		return mTextObject->mText.GetFullText ();
	}
	return CAtlString();
}

CAtlString CQueuedSpeak::GetSpeechText ()
{
	if	(mText)
	{
		return mText->GetSpeechText ();
	}
	else
	if	(mTextObject)
	{
		return mTextObject->mText.GetSpeechText ();
	}
	return CAtlString();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CQueuedSpeak::Advance (CQueuedActions& pQueue, CAgentWnd* pAgentWnd)
{
	bool				lRet = false;
	CAgentCharacterWnd*	lCharacterWnd;
	HRESULT				lResult = S_OK;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
#ifdef	DebugTimeStart
		DebugTimeStart
#endif
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueuedSpeak [%p(%d)] [%s] [%s]"), pAgentWnd, mCharID, this, mReqID, DebugStr(GetFullText()), DebugStr(mSoundUrl));
		}
#endif
		if	(!mStarted)
		{
			lResult = SpeechIsReady (pAgentWnd);

			if	(lResult == S_FALSE)
			{
				lRet = true;
			}
			else
			if	(pQueue.GetNextAction (QueueActionSpeak) != this)
			{
				lRet = true; // Already deleted during recursion
			}
		}

		if	(
				(!lRet)
			&&	(lResult == S_OK)
			&&	(mStarted)
			)
		{
			if	(SpeechIsBusy (pAgentWnd))
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p{%d}] QueuedSpeak [%p{%d)] is busy"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				lRet = true;
			}
			else
			if	(pQueue.GetNextAction (QueueActionSpeak) != this)
			{
				lRet = true; // Already deleted during recursion
			}
		}
		else
		if	(
				(!lRet)
			&&	(lResult == S_OK)
			&&	(!mStarted)
			)
		{
			if	(SpeechIsBusy (pAgentWnd))
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueuedSpeak [%p{%d)] still busy"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				lRet = true;
			}
			else
			if	(pQueue.GetNextAction (QueueActionSpeak) != this)
			{
				lRet = true; // Already deleted during recursion
			}
			else
			if	(
					(!mAnimated)
				&&	(ShowSpeechAnimation (pQueue, pAgentWnd))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueuedSpeak [%p(%d)] animated"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				lRet = true;
			}
			else
			if	(
					(mAnimated)
				&&	(FAILED (lResult = PrepareSpeech (pAgentWnd)))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueuedSpeak [%p(%d)] failed"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
			}
			else
			if	(pQueue.GetNextAction (QueueActionSpeak) != this)
			{
				lRet = true; // Already deleted during recursion
			}
			else
			{
				NotifyStarted (pAgentWnd->mNotify);

				if	(pQueue.GetNextAction (QueueActionSpeak) == this)
				{
					if	(
							(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
						&&	(!lCharacterWnd->IsCharShown ())
						)
					{
						lResult = AGENTERR_CHARACTERNOTVISIBLE;
					}
					else
					{
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Show QueuedSpeak [%p(%d)] [%s] [%s]"), pAgentWnd, mCharID, this, mReqID, DebugStr(GetFullText()), DebugStr(mSoundUrl));
						}
#endif
						if	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
						{
							if	(mText)
							{
								lCharacterWnd->SetLastSpeech (*mText);
							}
							else
							if	(mTextObject)
							{
								lCharacterWnd->SetLastSpeech (mTextObject->mText);
							}
						}
						if	(SUCCEEDED (lResult = StartSpeech (pQueue, pAgentWnd)))
						{
							lRet = true;
						}
						else
						if	(pQueue.GetNextAction (QueueActionSpeak) != this)
						{
							lRet = true; // Already deleted during recursion
						}
					}
				}
				else
				{
					lRet = true; // Was deleted during NotifyStarted
				}
			}
		}

		if	(
				(!lRet)
			&&	(pQueue.GetNextAction (QueueActionSpeak) == this)
			)
		{
			CDirectSoundLipSync *	lLipSync;
			CAgentBalloonWnd*		lBalloonWnd;

#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] EndQueuedSpeak [%p] [%d] Started [%u] Animated [%u] Balloon [%u]"), pAgentWnd, mCharID, this, mReqID, mStarted, mAnimated, ShowBalloon());
			}
#endif
			pQueue.RemoveHead ();

			if	(mTextObject)
			{
				mTextObject->Detach ();
			}
			if	(mVoice)
			{
				mVoice->RemoveNotifySink (mVoice->FindNotifySink (mVoiceNotifyId[0]));
				mVoice->RemoveNotifySink (mVoice->FindNotifySink (mVoiceNotifyId[1]));
			}
			if	(
					(!mSoundUrl.IsEmpty ())
				&&	(lLipSync = static_cast <CDirectSoundLipSync *> (mSoundFilter.GetInterfacePtr()))
				)
			{
				lLipSync->Disconnect ();
			}
			if	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			{
				lCharacterWnd->StopMouthAnimation ();

				if	(
						(ShowBalloon())
					&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					lBalloonWnd->AbortSpeechText ();
				}
			}

#ifdef	_LOG_QUEUE_OPS
			if	(LogIsActive (_LOG_QUEUE_OPS))
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Done QueuedSpeak [%p(%d)] [%s] [%s] [%8.8X]"), pAgentWnd, mCharID, this, mReqID, DebugStr(GetFullText()), DebugStr(mSoundUrl), lResult);
			}
#endif
			NotifyComplete (pAgentWnd->mNotify, lResult);
		}

#ifdef	DebugTimeStart
		DebugTimeStop
		LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f CQueuedSpeak::Advance"), DebugTimeElapsed);
#endif
	}
	return lRet;
}

bool CQueuedSpeak::Pause (CQueuedActions& pQueue, CAgentWnd* pAgentWnd, bool pPause)
{
	bool					lRet = false;
	CAgentCharacterWnd*	lCharacterWnd;
	CAgentBalloonWnd*		lBalloonWnd;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedSpeech [%p(%d)] Started [%u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted);
	}
#endif
	if	(pPause)
	{
		if	(!mPaused)
		{
			lRet = true;
		}
		if	(
				(mStarted)
			&&	(mVoice->SafeIsValid ())
			)
		{
			mVoice->Pause ();
		}
		if	(
				(mStarted)
			&&	(ShowBalloon())
			&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
			)
		{
			lBalloonWnd->Pause (true);
		}
	}
	else
	{
		if	(mPaused)
		{
			lRet = true;
		}
		if	(
				(mStarted)
			&&	(ShowBalloon())
			&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
			)
		{
			lBalloonWnd->Pause (false);
		}
		if	(
				(mStarted)
			&&	(mVoice->SafeIsValid ())
			)
		{
			mVoice->Resume ();
		}
	}
	mPaused = pPause;

	return lRet;
}

bool CQueuedSpeak::Abort (CQueuedActions& pQueue, CAgentWnd* pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool	lRet = false;

#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] AbortQueuedSpeak [%d] [%d] Started [%u] Animated [%u] Balloon [%u]"), pAgentWnd, mCharID, mReqID, mStarted, mAnimated, ShowBalloon());
	}
#endif
	if	(mStarted)
	{
		CAgentCharacterWnd*	lCharacterWnd;
		CAgentBalloonWnd*		lBalloonWnd;
		CDirectSoundLipSync *	lLipSync;

		if	(mVoice->SafeIsValid())
		{
			mVoice->RemoveNotifySink (mVoice->FindNotifySink (mVoiceNotifyId[0]));
			mVoice->RemoveNotifySink (mVoice->FindNotifySink (mVoiceNotifyId[1]));
			mVoice->Stop ();
		}
		if	(
				(!mSoundUrl.IsEmpty ())
			&&	(lLipSync = static_cast <CDirectSoundLipSync *> (mSoundFilter.GetInterfacePtr()))
			)
		{
			lLipSync->Stop ();
			lLipSync->Disconnect ();
			lRet = true;
		}

		if	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
		{
			lCharacterWnd->StopMouthAnimation ();

			if	(
					(ShowBalloon())
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->IsWindow ())
				)
			{
#ifdef	_STRICT_COMPATIBILITY
				lBalloonWnd->HideBalloon (true);
#else
				lBalloonWnd->AbortSpeechText ();
#endif
				lRet = true;
			}
		}
		if	(mTextObject)
		{
			mTextObject->Detach (true);
		}
	}
	if	(mAnimated)
	{
		if	(!pAgentWnd->ShowGesture (NULL))
		{
			pAgentWnd->Stop ();
		}
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CQueuedSpeak::SpeechIsBusy (CAgentWnd* pAgentWnd)
{
	bool					lRet = false;
	CAgentCharacterWnd*	lCharacterWnd;
	CAgentBalloonWnd*		lBalloonWnd = NULL;

#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(mStarted)
	{
		if	(
				(
					(mSoundUrl.IsEmpty ())
				&&	(mVoice->SafeIsValid ())
				&&	(mVoice->SafeIsSpeaking ())
				)
			||	(
					(ShowBalloon())
				&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->IsWindow ())
				&&	(lBalloonWnd->IsBusy (false))
				)
			)
		{
			lRet = true;
		}
	}
	else
	{
		if	(
				(
					(mVoice->SafeIsValid ())
				&&	(mVoice->SafeIsSpeaking ())
				)
			||	(
					(!mSoundUrl.IsEmpty ())
				&&	(PathIsURL (mSoundUrl))
				)
			||	(
					(ShowBalloon())
				&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->IsWindow ())
#ifdef	_STRICT_COMPATIBILITY
				&&	(lBalloonWnd->IsBusy (false))
#else
				&&	(lBalloonWnd->IsBusy (lBalloonWnd->IsAutoSize()))
#endif
				)
			)
		{
			lRet = true;

#ifdef	_TRACE_BUSY_TIME
			try
			{
				DWORD	lStartTime;

				if	(sBusyStart.Lookup (this, lStartTime))
				{
					if	(ElapsedTicks (lStartTime) > 1000)
					{
						LogMessage (_TRACE_BUSY_TIME, _T("[%p] Waiting [%d] for voice [%p] [%u] [%ls]"), this, ElapsedTicks (lStartTime), mVoice, mVoice->SafeIsSpeaking (), (BSTR)mVoice->GetDisplayName());
						sBusyStart [this] = GetTickCount();
					}
				}
				else
				{
					sBusyStart [this] = GetTickCount();
				}
			}
			catch AnyExceptionSilent
#endif
		}
	}

#ifdef	_TRACE_BUSY_TIME
	if	(!lRet)
	{
		DWORD	lStartTime;

		if	(
				(mVoice->SafeIsValid ())
			&&	(sBusyStart.Lookup (this, lStartTime))
			)
		{
			LogMessage (_TRACE_BUSY_TIME, _T("[%p] Got [%d] voice [%p] [%u] [%ls]"), this, ElapsedTicks (lStartTime), mVoice, mVoice->SafeIsSpeaking (), (BSTR)mVoice->GetDisplayName());
		}
		sBusyStart.RemoveKey (this);
	}
#endif

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CQueuedSpeak::SpeechIsBusy [%u]"), DebugTimeElapsed, lRet);
#endif
	return lRet;
}

HRESULT CQueuedSpeak::SpeechIsReady (CAgentWnd* pAgentWnd)
{
	HRESULT	lResult = S_OK;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(!mSoundUrl.IsEmpty ())
	{
		if	(PathIsURL (mSoundUrl))
		{
			INT_PTR				lNotifyNdx;
			CEventNotify*		lNotify;
			CFileDownload*		lSoundDownload = NULL;

			for	(lNotifyNdx = 0; lNotifyNdx < (INT_PTR)pAgentWnd->mNotify.GetCount(); lNotifyNdx++)
			{
				if	(
						(lNotify = pAgentWnd->mNotify [lNotifyNdx])
					&&	(lSoundDownload = lNotify->_FindSoundDownload (mSoundUrl))
					)
				{
					break;
				}
			}

			if	(
					(lSoundDownload)
				&&	(lSoundDownload->IsDownloadComplete () == S_OK)
				)
			{
				mSoundUrl = lSoundDownload->GetCacheName ();
			}
			else
			if	(
					(lSoundDownload)
				&&	(
						(lSoundDownload->IsDownloadStarted ())
					||	(lSoundDownload->IsDownloadStarting ())
					)
				&&	(!lSoundDownload->IsDownloadCancelling ())
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] waiting for [%s]"), pAgentWnd, mCharID, this, mReqID, DebugStr(mSoundUrl));
				}
#endif
				lResult = S_FALSE;
			}
			else
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] failed for [%s]"), pAgentWnd, mCharID, this, mReqID, DebugStr(mSoundUrl));
				}
#endif
				lResult = AGENTPROVERROR_WAVEINVALID;
			}
		}
	}
	else
	if	(!mVoice->SafeIsValid())
	{
		lResult = AGENTWARNING_TTSENGINENOTFOUND;
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CQueuedSpeak::SpeechIsReady [%8.8X]"), DebugTimeElapsed, lResult);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CQueuedSpeak::PrepareSpeech (CAgentWnd* pAgentWnd)
{
	HRESULT	lResult = S_FALSE;

#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(!mSoundUrl.IsEmpty ())
	{
		if	(!mSoundFilter)
		{
			tPtr <CComObject <CDirectSoundLipSync> >	lLipSync;

			if	(SUCCEEDED (CComObject <CDirectSoundLipSync>::CreateInstance (lLipSync.Free())))
			{
				lResult = lLipSync->Connect (pAgentWnd->GetGraphBuilder(), mSoundUrl, pAgentWnd->GetAgentStreamInfo());
				if	(SUCCEEDED (lResult))
				{
					mSoundFilter = lLipSync.Detach ();
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}
	else
	if	(mVoice->SafeIsValid())
	{
#ifndef	_WIN64
		CSapi4Voice*	lSapi4Voice;
		CAgentFile*	lAgentFile;

		if	(lSapi4Voice = dynamic_cast <CSapi4Voice*> (mVoice))
		{
			if	(
					(lAgentFile = pAgentWnd->GetAgentFile())
				&&	(lAgentFile->Tts.Pitch > 0)
				)
			{
				LogSapi4Err (LogNormal|LogTime, lSapi4Voice->SetPitch (lAgentFile->Tts.Pitch));
			}
			if	(
					(lAgentFile)
				&&	(lAgentFile->Tts.Speed > 0)
				)
			{
				LogSapi4Err (LogNormal|LogTime, lSapi4Voice->SetRate (CDaSettingsConfig().ApplyVoiceRate (lAgentFile->Tts.Speed, 4)));
			}
			else
			{
				LogSapi4Err (LogNormal|LogTime, lSapi4Voice->SetRate (CDaSettingsConfig().ApplyVoiceRate (lSapi4Voice->GetDefaultRate(), 4)));
			}
		}
		else
#endif
		{
			LogSapi5Err (LogNormal|LogTime, mVoice->SetRate (CDaSettingsConfig().CalcVoiceRate()));
		}
		lResult = S_OK;
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CQueuedSpeak::PrepareSpeech"), DebugTimeElapsed);
#endif
	return lResult;
}

HRESULT CQueuedSpeak::StartSpeech (CQueuedActions& pQueue, CAgentWnd* pAgentWnd)
{
	HRESULT					lResult = S_FALSE;
	bool					lBalloonShown = false;
	CAgentCharacterWnd*		lCharacterWnd;
	CAgentBalloonWnd*		lBalloonWnd = NULL;
	CDirectSoundLipSync *	lLipSync;

#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(!mSoundUrl.IsEmpty ())
	{
		if	(
				(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			&&	(lLipSync = static_cast <CDirectSoundLipSync *> (mSoundFilter.GetInterfacePtr()))
			)
		{
			lCharacterWnd->StartMouthAnimation ((long)(lLipSync->GetDuration() / MsPer100Ns));
		}

		if	(
				(pQueue.GetNextAction (QueueActionSpeak) == this)
			&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			)
		{
			if	(ShowBalloon())
			{
				if	(
						(lBalloonWnd = lCharacterWnd->GetBalloonWnd (true))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					lBalloonWnd->ApplyOptions (mBalloonOptions);
					if	(mText)
					{
						lBalloonShown = lBalloonWnd->ShowBalloonSpeech (*mText, true);
					}
					else
					if	(mTextObject)
					{
						lBalloonShown = lBalloonWnd->ShowBalloonSpeech (mTextObject->mText, true);
					}
				}
			}
#ifdef	_STRICT_COMPATIBILITY
			else
			{
				if	(
						(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					lBalloonWnd->HideBalloon (true);
				}
			}
#endif
		}

		if	(pQueue.GetNextAction (QueueActionSpeak) == this)
		{
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] CAgentCharacterWnd Speak [%s] [%s]"), pAgentWnd, mCharID, DebugStr(GetFullText()), DebugStr(mSoundUrl));
			}
#endif
#ifdef	DebugTimeStart
			DebugTimeStart
#endif
			lResult = pAgentWnd->PlayFromTo (0, pAgentWnd->GetDurationMs(), false);
#ifdef	DebugTimeStart
			DebugTimeStop
			LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CQueuedSpeak::PlayFromTo"), DebugTimeElapsed);
#endif
		}
	}
	else
	if	(mVoice->SafeIsValid())
	{
		mVoice->ClearNotifySinks ();

		if	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
		{
			mVoiceNotifyId[0] = mVoice->AddNotifySink (lCharacterWnd, lCharacterWnd);
			mVoice->SetEventCharID (mCharID);
		}

		if	(mTextObject)
		{
			mTextObject->Attach (mCharID, pAgentWnd->GetNotifyClientNotify (mCharID), mVoice);
		}

		if	(
				(pQueue.GetNextAction (QueueActionSpeak) == this)
			&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			)
		{
			if	(ShowBalloon())
			{
				if	(
						(lBalloonWnd = lCharacterWnd->GetBalloonWnd (true))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					mVoiceNotifyId[1] = mVoice->AddNotifySink (lBalloonWnd, lBalloonWnd);
					lBalloonWnd->ApplyOptions (mBalloonOptions);
					if	(mText)
					{
						lBalloonShown = lBalloonWnd->ShowBalloonSpeech (*mText);
					}
					else
					if	(mTextObject)
					{
						lBalloonShown = lBalloonWnd->ShowBalloonSpeech (mTextObject->mText);
					}
				}
			}
#ifdef	_STRICT_COMPATIBILITY
			else
			{
				if	(
						(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					lBalloonWnd->HideBalloon (true);
				}
			}
#endif
		}

		if	(pQueue.GetNextAction (QueueActionSpeak) == this)
		{
#ifdef	_LOG_SPEECH
			if	(LogIsActive (_LOG_SPEECH))
			{
				LogMessage (_LOG_SPEECH, _T("[%p] [%d] CAgentCharacterWnd Speak   [%s]"), pAgentWnd, mCharID, DebugStr(GetSpeechText()));
				LogMessage (_LOG_SPEECH, _T("[%p] [%d]                    Text    [%s]"), pAgentWnd, mCharID, DebugStr(GetFullText()));
				LogMessage (_LOG_SPEECH, _T("[%p] [%d]                    Voice   [%u] Rate [%u] Volume [%u]"), pAgentWnd, mCharID, mVoice->SafeIsValid (), mVoice->GetRate(), mVoice->GetVolume());

				if	(
						(ShowBalloon())
					&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
					&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					LogMessage (_LOG_SPEECH, _T("[%p] [%d]                    Balloon [%u] AutoSize [%u] AutoPace [%u] AutoHide [%u]"), pAgentWnd, mCharID, lBalloonWnd->IsWindowVisible(), lBalloonWnd->IsAutoSize(), lBalloonWnd->IsAutoPace(), lBalloonWnd->IsAutoHide());
				}
			}
#endif
			lResult = mVoice->Speak (GetSpeechText());
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	if	(FAILED (lResult))
	{
		if	(
				(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
			&&	(lBalloonWnd->IsWindow ())
			)
		{
			lBalloonWnd->AbortSpeechText ();
			lBalloonWnd->HideBalloon (true);
		}
	}
	else
	if	(
			(lBalloonShown)
		&&	(lBalloonWnd)
		)
	{
		lBalloonWnd->ShowBalloonAuto ();
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CQueuedSpeak::StartSpeech"), DebugTimeElapsed);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CQueuedSpeak::ShowSpeechAnimation (CQueuedActions& pQueue, CAgentWnd* pAgentWnd)
{
	bool	lRet = false;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(!mAnimated)
	{
		CAgentStreamInfo*		lStreamInfo;
		long					lAnimationNdx = -1;
		long					lSpeakingFrameNdx = -1;
		const CAtlStringArray*	lGestures;
		LPCTSTR					lGesture;

		mAnimated = true;

		if	(
				(lStreamInfo = pAgentWnd->GetAgentStreamInfo())
			&&	(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			&&	(lStreamInfo->FindAnimationSpeakingFrame (&lSpeakingFrameNdx, lAnimationNdx) == S_OK)
			&&	(lSpeakingFrameNdx >= 0)
			)
		{
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Speech Animation [%d] [%ls] Frame [%d]"), pAgentWnd, mCharID, lAnimationNdx, (BSTR)(pAgentWnd->GetAgentFile()->GetAnimation (lAnimationNdx)->Name), lSpeakingFrameNdx);
			}
#endif
		}
		else
		if	(
				(lGestures = pAgentWnd->GetFileStates() (_T("SPEAKING")))
			&&	(lGestures->GetCount() > 0)
			)
		{
			CAtlOwnPtrList <CQueuedAction>	lQueue;

			pQueue.RemoveHead ();
			pQueue.PushQueue (lQueue);
			pAgentWnd->ShowState (_T("SPEAKING"));
			pQueue.AddTail (this);
			pQueue.PopQueue (lQueue);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Showed SPEAKING state"), pAgentWnd, mCharID);
			}
#endif
			lRet = true;
		}
		else
		if	(
				(pAgentWnd->ShowGesture (lGesture = _T("RestPose")))
			||	(pAgentWnd->ShowGesture (lGesture = _T("Explain")))
			)
		{
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Showed %s for speech"), pAgentWnd, mCharID, lGesture);
			}
#endif
			lRet = true;
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CQueuedSpeak::ShowSpeechAnimation"), DebugTimeElapsed);
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedThink::CQueuedThink (CAgentBalloonOptions* pBalloonOptions, long pCharID, long pReqID)
:	CQueuedAction (QueueActionThink, pCharID, pReqID)
{
	if	(pBalloonOptions)
	{
		mBalloonOptions = new CAgentBalloonOptions (*pBalloonOptions);
	}
}

CQueuedThink::~CQueuedThink ()
{
	SafeFreeSafePtr (mTextObjectRef);
}

//////////////////////////////////////////////////////////////////////

void CQueuedThink::Initialize (CAgentText& pText)
{
	if	(mText = new CAgentText (pText.GetSapiVersion()))
	{
		mText->operator= (pText);
	}
	mTextObject = NULL;
	mTextObjectRef = NULL;
}

void CQueuedThink::Initialize (CAgentTextObject* pTextObject)
{
	mTextObject = pTextObject;
	mTextObjectRef = pTextObject;
	mText = NULL;
}

CAtlString CQueuedThink::GetFullText ()
{
	if	(mText)
	{
		return mText->GetFullText ();
	}
	else
	if	(mTextObject)
	{
		return mTextObject->mText.GetFullText ();
	}
	return CAtlString();
}

//////////////////////////////////////////////////////////////////////

bool CQueuedThink::Advance (CQueuedActions& pQueue, CAgentWnd* pAgentWnd)
{
	bool					lRet = false;
	CAgentCharacterWnd*	lCharacterWnd;
	CAgentBalloonWnd*		lBalloonWnd;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedThink [%p] [%d] [%s]"), pAgentWnd, mCharID, this, mReqID, DebugStr(GetFullText()));
		}
#endif
		if	(!mStarted)
		{
			if	(
					(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->IsWindow ())
				&&	(lBalloonWnd->IsBusy (true))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedThink [%p] [%d] is still busy"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				lRet = true;
			}
			else
			{
				NotifyStarted (pAgentWnd->mNotify);

				if	(pQueue.GetNextAction (QueueActionThink) == this)
				{
					if	(
							(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
						&&	(!lCharacterWnd->IsCharShown ())
						)
					{
						pQueue.RemoveHead ();
						NotifyComplete (pAgentWnd->mNotify, AGENTERR_CHARACTERNOTVISIBLE);
					}
					else
					if	(
							(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
						&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (true))
						&&	(lBalloonWnd->IsWindow ())
						)
					{
//
//	MS Agent doesn't do this
//
//						pAgentWnd->ShowGesture (_T("Think"));
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Show QueuedThink [%p] [%d] [%s]"), pAgentWnd, mCharID, this, mReqID, DebugStr(GetFullText()));
						}
#endif
						lBalloonWnd->ApplyOptions (mBalloonOptions);
						if	(
								(mText)
							?	(lBalloonWnd->ShowBalloonThought (*mText))
							:	(mTextObject)
							?	(lBalloonWnd->ShowBalloonThought (mTextObject->mText))
							:	(false)
							)
						{
							lBalloonWnd->ShowBalloonAuto ();
						}
						lRet = true;
					}
					else
					{
						pQueue.RemoveHead ();
						NotifyComplete (pAgentWnd->mNotify, AGENTERR_NOBALLOON);
					}
				}
				else
				{
					lRet = true; // Was deleted during NotifyStarted
				}
			}
		}
		else
		{
			if	(
					(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->IsWindow ())
				&&	(lBalloonWnd->IsBusy (false))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedThink [%p] [%d] is busy"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				lRet = true;
			}
			else
			if	(pQueue.GetNextAction (QueueActionThink) == this)
			{
				if	(
						(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
					&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					lBalloonWnd->AbortSpeechText ();
				}
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Done QueuedThink [%p] [%d]"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				pQueue.RemoveHead ();
				NotifyComplete (pAgentWnd->mNotify);
			}
			else
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

bool CQueuedThink::Pause (CQueuedActions& pQueue, CAgentWnd* pAgentWnd, bool pPause)
{
	bool					lRet = false;
	CAgentCharacterWnd*	lCharacterWnd;
	CAgentBalloonWnd*		lBalloonWnd;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedThink [%p(%d)] Started [%u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted);
	}
#endif
	if	(pPause)
	{
		if	(!mPaused)
		{
			lRet = true;
		}
		if	(
				(mStarted)
			&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
			)
		{
			lBalloonWnd->Pause (true);
		}
	}
	else
	{
		if	(mPaused)
		{
			lRet = true;
		}
		if	(
				(mStarted)
			&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
			&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
			)
		{
			lBalloonWnd->Pause (false);
		}
	}
	mPaused = pPause;

	return lRet;
}

bool CQueuedThink::Abort (CQueuedActions& pQueue, CAgentWnd* pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool					lRet = false;
	CAgentCharacterWnd*	lCharacterWnd;
	CAgentBalloonWnd*		lBalloonWnd;

	if	(
			(mStarted)
		&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd*> (pAgentWnd))
		&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
		&&	(lBalloonWnd->IsWindow ())
		)
	{
#ifdef	_STRICT_COMPATIBILITY
		lBalloonWnd->HideBalloon (true);
#else
		lBalloonWnd->AbortSpeechText ();
#endif
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CQueuedSpeak::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Speech"), _T("Balloon [%u] Animated [%u] Speaking [%u] Voice [%ls] Text [%s]"), ShowBalloon(), mAnimated, mVoice->SafeIsSpeaking(), mVoice->SafeIsValid()?(BSTR)mVoice->GetDisplayName():NULL, mText?(LPCTSTR)DebugStr(mText->GetFullText()):NULL);
		}
		catch AnyExceptionSilent
	}
#endif
}

void CQueuedThink::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Think"), _T("Text [%s]"), mText?(LPCTSTR)DebugStr(mText->GetFullText()):NULL);
		}
		catch AnyExceptionSilent
	}
#endif
}
