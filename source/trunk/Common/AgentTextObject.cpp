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
#include "StdAfx.h"
#include "AgentTextObject.h"
#include "SapiVoice.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "DebugStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_SPEECH_EVENTS	(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true,_T("Core"))&0xFFFF|LogTimeMs|LogHighVolume)
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT	CAgentTextObject::mVoiceStartMsg = RegisterWindowMessage (_T("7781F0DC-B3CD-4ce9-8828-95C4FB56BBE2"));
const UINT	CAgentTextObject::mVoiceEndMsg = RegisterWindowMessage (_T("0BEA4D03-95C6-4c2f-A5BF-EFFE63D24D8F"));
const UINT	CAgentTextObject::mVoiceWordMsg = RegisterWindowMessage (_T("82C73827-F1B8-4223-824B-BC0953892D56"));

/////////////////////////////////////////////////////////////////////////////

CAgentTextObject::CAgentTextObject ()
:	mCharID (0),
	mVoice (NULL),
	mVoiceStartNotified (false),
	mVoiceEndNotified (false)
{
}

CAgentTextObject::~CAgentTextObject ()
{
	ClearNotifySources ();
	SafeFreeSafePtr (mMsgPostingWnd);
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextObject::Attach (long pCharID, CEventNotify * pNotify, CSapiVoice * pVoice)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] CAgentTextObject Attach [%d] [%p] [%p] [%p]"), m_pOuterUnknown, pCharID, pNotify, pVoice, mMsgPostingWnd.Ptr());
	}
#endif
	mText.ResetState (true);
	mCharID = pCharID;
	mNotify = pNotify;
	mVoiceStartNotified = false;
	mVoiceEndNotified = false;

	if	(mVoice != pVoice)
	{
		if	(mVoice)
		{
			mVoice->RemoveNotifySink (this);
		}
		if	(mVoice = pVoice)
		{
			mVoice->AddNotifySink (this, this);
		}
	}

	if	(!mMsgPostingWnd)
	{
		mMsgPostingWnd = new CMsgPostingWnd <CAgentTextObject> (*this);
	}
}

void CAgentTextObject::Detach (bool pStopped)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] CAgentTextObject Detach [%d] [%p] [%p] [%p]"), m_pOuterUnknown, mCharID, mNotify, mVoice, mMsgPostingWnd.Ptr());
	}
#endif
	if	(
			(mNotify)
		&&	(mVoiceStartNotified)
		&&	(!mVoiceEndNotified)
		&&	(mMsgPostingWnd)
		&&	(mMsgPostingWnd->IsWindow())
		)
	{
		mMsgPostingWnd->SendMessage (mVoiceEndMsg, mCharID, (LPARAM)pStopped);
	}
	SafeFreeSafePtr (mMsgPostingWnd);

	if	(mVoice)
	{
		try
		{
			mVoice->RemoveNotifySink (this);
		}
		catch AnyExceptionSilent
	}
	mVoice = NULL;

	mNotify = NULL;
	mVoiceStartNotified = false;
	mVoiceEndNotified = true;
	mCharID = 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentTextObject::OnVoiceStart (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d %d] CAgentTextObject OnVoiceStart    [%p]"), m_pOuterUnknown, mCharID, pCharID, mMsgPostingWnd.Ptr());
	}
#endif
	mVoiceStartNotified = true;
	mVoiceEndNotified = false;

	if	(
			(mNotify)
		&&	(mMsgPostingWnd)
		&&	(mMsgPostingWnd->IsWindow())
		)
	{
		mMsgPostingWnd->PostMessage (mVoiceStartMsg, pCharID);
	}
}

void CAgentTextObject::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d %d] CAgentTextObject OnVoiceEnd    [%p]"), m_pOuterUnknown, mCharID, pCharID, mMsgPostingWnd.Ptr());
	}
#endif
	mVoiceEndNotified = true;

	if	(
			(mNotify)
		&&	(mMsgPostingWnd)
		&&	(mMsgPostingWnd->IsWindow())
		)
	{
		mMsgPostingWnd->PostMessage (mVoiceEndMsg, pCharID);
	}
}

void CAgentTextObject::OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d %d] CAgentTextObject OnVoiceWord    [%u (%d)] [%p]"), m_pOuterUnknown, mCharID, pCharID, pWordPos, pWordLength, mMsgPostingWnd.Ptr());
	}
#endif
	mText.DisplayThisWord (pWordPos, pWordLength, true);

	if	(
			(mNotify)
		&&	(mMsgPostingWnd)
		&&	(mMsgPostingWnd->IsWindow())
		)
	{
		mMsgPostingWnd->PostMessage (mVoiceWordMsg, pCharID, (LPARAM)mText.GetWordDisplayed());
	}
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentTextObject::OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d %d] CAgentTextObject OnVoiceStartMsg [%p] [%p]"), m_pOuterUnknown, mCharID, wParam, m_pOuterUnknown, mNotify);
	}
#endif
	if	(
			(m_pOuterUnknown)
		&&	(mNotify)
		)
	{
		if	(PreNotify ())
		{
			try
			{
				mNotify->OnSpeechStart (mCharID, m_pOuterUnknown);
			}
			catch AnyExceptionSilent
		}
		PostNotify ();
	}
	return 0;
}

LRESULT CAgentTextObject::OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d %d] CAgentTextObject OnVoiceEndMsg [%u] [%p] [%p]"), m_pOuterUnknown, mCharID, wParam, lParam, m_pOuterUnknown, mNotify);
	}
#endif
	if	(
			(m_pOuterUnknown)
		&&	(mNotify)
		)
	{
		if	(PreNotify ())
		{
			try
			{
				mNotify->OnSpeechEnd (mCharID, m_pOuterUnknown, (lParam)?VARIANT_TRUE:VARIANT_FALSE);
			}
			catch AnyExceptionSilent
		}
		PostNotify ();
	}
	return 0;
}

LRESULT CAgentTextObject::OnVoiceWordMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d %d] CAgentTextObject OnVoiceWordMsg [%d] [%p] [%p]"), m_pOuterUnknown, mCharID, wParam, lParam, m_pOuterUnknown, mNotify);
	}
#endif
	if	(
			(m_pOuterUnknown)
		&&	(mNotify)
		)
	{
		if	(PreNotify ())
		{
			try
			{
				mNotify->OnSpeechWord (mCharID, m_pOuterUnknown, (long)lParam);
			}
			catch AnyExceptionSilent
		}
		PostNotify ();
	}
	return 0;
}
