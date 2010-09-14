/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include <AgtErr.h>
#include "DaCoreRes.h"
#include "DaCmnCharacter.h"
#include "DaCmnCommands.h"
#include "DaGlobalConfig.h"
#include "ListeningState.h"
#include "AgentAnchor.h"
#include "AgentListeningWnd.h"
#include "VoiceCommandsWnd.h"
#include "Sapi5Input.h"
#include "SapiInputCache.h"
#include "Registry.h"
#include "Localize.h"
#include "MallocPtr.h"
#include <sphelper.h>

#pragma comment(lib, "winmm.lib")

#ifdef	_DEBUG
#define	_DEBUG_LISTEN			(GetProfileDebugInt(_T("DebugListen"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_NOTIFY_PATH		(GetProfileDebugInt(_T("DebugNotifyPath"),LogVerbose,true)&0xFFFF)
//#define	_DEBUG_START_STOP	LogNormal
//#define	_DEBUG_HOT_KEY		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT_PTR	CListeningState::mListenTimerIdManual = RegisterWindowMessage (_T("9C726EFD-8DC7-466A-9204-CCA3FAC4A2F9"));
const UINT_PTR	CListeningState::mListenTimerIdAuto = RegisterWindowMessage (_T("4D5E66E2-FA40-4312-874E-12867C2B6559"));
const UINT_PTR	CListeningState::mListenTimerIdHotkey = RegisterWindowMessage (_T("D6C3E7DA-3A76-4245-8465-0670EE66BACB"));
const UINT_PTR	CListeningState::mListenTimerIdHeard = RegisterWindowMessage (_T("A9FE3C4D-D99F-411E-BAD8-F0A0773F1CA4"));

/////////////////////////////////////////////////////////////////////////////

CListeningState::CListeningState (CDaCmnCharacter & pCharacter)
:	mCharacter (pCharacter),
	mSapi5Input (NULL),
	mHearingStateShown (false),
	mListenSuspended (false),
	mListenDelayManual (10000),
	mListenDelayHeard (3000),
	mListenTimerManual (0),
	mListenTimerAuto (0),
	mListenTimerHotkey (0),
	mListenTimerHeard (0)
{
	mCharacter.mNotify->RegisterEventReflect (this, true);
}

CListeningState::~CListeningState()
{
	mCharacter.mNotify->RegisterEventReflect (this, false);

	StopListenTimers ();
	ShowListeningTip (false, false);

	if	(mSapi5InputContext)
	{
		try
		{
			mSapi5InputContext->ClearEventSinks ();
			mSapi5InputContext->StopListening ();
		}
		catch AnyExceptionDebug

		SafeFreeSafePtr (mSapi5InputContext);
	}

	if	(mSapi5Input)
	{
		try
		{
			CSapiInputCache *	lInputCache;

			if	(lInputCache = CSapiInputCache::GetStaticInstance ())
			{
				lInputCache->RemoveInputClient (mSapi5Input, this);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CListeningState::IsManual () const
{
	return (mListenTimerManual != 0);
}

bool CListeningState::IsAutomatic () const
{
	return (mListenTimerAuto != 0);
}

bool CListeningState::IsSuspended () const
{
	return mListenSuspended;
}

bool CListeningState::IsActive () const
{
	if	(
			(mSapi5InputContext->SafeIsValid ())
		&&	(
				(mListenTimerManual)
			||	(mListenTimerAuto)
			||	(mListenTimerHotkey)
			||	(mListenTimerHeard)
			)
		)
	{
		return true;
	}
	return false;
}

bool CListeningState::IsListening () const
{
	if	(
			(mSapi5InputContext->SafeIsValid ())
		&&	(mSapi5InputContext->IsListening ())
		)
	{
		return true;
	}
	return false;
}

bool CListeningState::IsHearing () const
{
	if	(
			(mSapi5InputContext->SafeIsValid ())
		&&	(mSapi5InputContext->IsHearing ())
		)
	{
		return true;
	}
	return false;
}

long CListeningState::GetCharID () const
{
	return mCharacter.GetCharID ();
}

LANGID CListeningState::GetLangID () const
{
	return mCharacter.GetLangID ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::StartListening (bool pManual)
{
	HRESULT				lResult = S_OK;
	CDaSettingsConfig	lSettingsConfig;

#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] StartListening Manual [%u] Enabled [%u] IsAuto [%u] Active [%u %u] Listening [%u %u] CharActive [%u]"), this, GetCharID(), pManual, CDaSettingsConfig().LoadConfig().mSrEnabled, IsAutomatic(), IsActive(), IsSuspended(), IsListening(), (mSapi5InputContext.Ptr()?mSapi5InputContext->FindEventSink(this):false), (mCharacter.mNotify->mAnchor->mAnchor.GetActiveCharacter()==GetCharID()));
#endif

	lSettingsConfig.LoadConfig ();

	if	(!lSettingsConfig.mSrEnabled)
	{
		lResult = AGENTVOICEERROR_SPEECHDISABLED;
	}
	else
	if	(IsSuspended ())
	{
		lResult = AGENTERR_CHARACTERNOTACTIVE;
	}
	else
	{
		if	(SUCCEEDED (lResult = GetInputContext ()))
		{
			lResult = StartInputContext ();

			if	(
					(SUCCEEDED (lResult))
				&&	(!pManual)
				&&	(!IsAutomatic ())
				)
			{
				if	(mCharacter.ShowListeningState (true))
				{
					mHearingStateShown = false;
				}
			}

			if	(
					(lResult == S_OK)
				&&	(!IsActive ())
				)
			{
				try
				{
					mCharacter.mNotify->mGlobal->_CharacterListening (GetCharID(), true, 0);
				}
				catch AnyExceptionSilent

				if	(lSettingsConfig.mSrListeningPrompt)
				{
					if	(!PlaySapiInputPrompt (pManual, _T("HubOnSound")))
					{
						MessageBeep (0);
					}
				}
			}
		}

		if	(SUCCEEDED (lResult))
		{
			ShowListeningTip (true, true);
			StartListenTimers (pManual);
		}
		else
		if	(
				(!pManual)
			&&	(lResult == AGENTVOICEERROR_NOTINSTALLED)
			)
		{
			ShowListeningTip (true, false, CLocalize::LoadString (IDS_COMMANDS_NO_SPEECH_INPUT, mCharacter.GetLangID()));
			StartListenTimers (pManual);
		}
		else
		{
			ShowListeningTip (true, false);
		}
	}
#ifdef	_DEBUG_LISTEN
	LogComErr (_DEBUG_LISTEN, lResult, _T("[%p(%d)] StartListening"), this, GetCharID());
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::StopListening (bool pManual, long pCause)
{
	HRESULT	lResult = S_OK;

	if	(
			(pCause == ListenComplete_CharacterClientDeactivated)
		&&	(!IsActive ())
		&&	(!IsListening ())
		)
	{
		lResult = S_FALSE;
	}
	else
	{
#ifdef	_DEBUG_LISTEN
		CAtlString	lCauseStr;
		switch (pCause)
		{
			case ListenComplete_ProgramDisabled:			lCauseStr = _T("ProgramDisabled"); break;
			case ListenComplete_ProgramTimedOut:			lCauseStr = _T("ProgramTimedOut"); break;
			case ListenComplete_UserTimedOut:				lCauseStr = _T("UserTimedOut"); break;
			case ListenComplete_UserReleasedKey:			lCauseStr = _T("UserReleasedKey"); break;
			case ListenComplete_UserSpeechEnded:			lCauseStr = _T("UserSpeechEnded"); break;
			case ListenComplete_CharacterClientDeactivated:	lCauseStr = _T("CharacterClientDeactivated"); break;
			case ListenComplete_DefaultCharacterChanged:	lCauseStr = _T("DefaultCharacterChanged"); break;
			case ListenComplete_UserDisabled:				lCauseStr = _T("UserDisabled"); break;
			default:										lCauseStr.Format (_T("%d"), pCause); break;
		}
		LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] StopListening Manual [%u] IsAuto [%u] Active [%u %u] Listening [%u %u] CharActive [%u] Cause[%s]"), this, GetCharID(), pManual, IsAutomatic(), IsActive(), IsSuspended(), IsListening(), (mSapi5InputContext.Ptr()?mSapi5InputContext->FindEventSink(this):false), (mCharacter.mNotify->mAnchor->mAnchor.GetActiveCharacter()==GetCharID()), lCauseStr);
#endif

		if	(
				(pManual)
			&&	(IsAutomatic ())
			)
		{
			lResult = S_FALSE;
		}
		else
		{
			bool	lWasListening = false;

			if	(mSapi5InputContext)
			{
				lResult = mSapi5InputContext->StopListening ();

				lWasListening = mSapi5InputContext->FindEventSink (this);
				mSapi5InputContext->RemoveEventSink (this);
			}
			if	(
					(lWasListening)
				&&	(CDaSettingsConfig().LoadConfig().mSrListeningPrompt)
				)
			{
				PlaySapiInputPrompt (pManual, _T("HubOffSound"));
			}
			if	(
					(pManual)
				||	(!lWasListening)
				||	(!mListenTimerHeard)
				)
			{
				StopListenTimers ();
				ShowListeningTip (false, false);
			}
			if	(lWasListening)
			{
				mCharacter.ShowListeningState (false);
				mCharacter.ShowHearingState (false);

				try
				{
					mCharacter.mNotify->mGlobal->_CharacterListening (GetCharID(), false, pCause);
				}
				catch AnyExceptionSilent
			}
		}
#ifdef	_DEBUG_LISTEN
		LogComErr (_DEBUG_LISTEN, lResult, _T("[%p(%d)] StopListening"), this, GetCharID());
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::KeepListening (bool pManual)
{
	HRESULT	lResult = S_FALSE;

#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] KeepListening Manual [%u] Enabled [%u] IsAuto [%u] Active [%u %u] Listening [%u %u] CharActive [%u]"), this, GetCharID(), pManual, CDaSettingsConfig().LoadConfig().mSrEnabled, IsAutomatic(), IsActive(), IsSuspended(), IsListening(), (mSapi5InputContext.Ptr()?mSapi5InputContext->FindEventSink(this):false), (mCharacter.mNotify->mAnchor->mAnchor.GetActiveCharacter()==GetCharID()));
#endif
	if	(
			(!pManual)
		&&	(IsAutomatic ())
		&&	(IsListening ())
		)
	{
		DWORD	lHotKeyDelay;

		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHotkey);
		if	(lHotKeyDelay = CDaSettingsConfig().LoadConfig().mSrHotKeyDelay)
		{
			mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerHotkey = mListenTimerIdHotkey, lHotKeyDelay, this);
		}
		else
		{
			mListenTimerHotkey = 0;
		}
		lResult = S_OK;
	}
	return lResult;
}

HRESULT CListeningState::SuspendListening (bool pSuspend)
{
	HRESULT	lResult = S_FALSE;
#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] SuspendListening [%u] Enabled [%u] IsAuto [%u] Active [%u %u] Listening [%u %u] CharActive [%u]"), this, GetCharID(), pSuspend, CDaSettingsConfig().LoadConfig().mSrEnabled, IsAutomatic(), IsActive(), IsSuspended(), IsListening(), (mSapi5InputContext.Ptr()?mSapi5InputContext->FindEventSink(this):false), (mCharacter.mNotify->mAnchor->mAnchor.GetActiveCharacter()==GetCharID()));
#endif

	if	(pSuspend)
	{
		if	(mSapi5InputContext->SafeIsValid ())
		{
			mSapi5InputContext->PauseListening (true);
		}
		if	(!mListenSuspended)
		{
			ShowListeningTip (true, false);
		}
	}
	else
	{
		if	(mListenSuspended)
		{
			ShowListeningTip (true, true);
			StartListenTimers (IsManual());
		}
		if	(mSapi5InputContext->SafeIsValid ())
		{
			mSapi5InputContext->PauseListening (false);
		}
	}
	if	(SUCCEEDED (lResult))
	{
		mListenSuspended = pSuspend;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::TransferState (CListeningState * pToState)
{
	HRESULT	lResult = S_FALSE;

	if	(!pToState)
	{
		lResult = E_POINTER;
	}
	else
	{
#ifdef _DEBUG_LISTEN
		LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] TransferState [%p] [%d] Manual [%u] Automatic [%u]"), this, GetCharID(), pToState, pToState->GetCharID(), IsManual(), IsAutomatic());
#endif
		if	(
				(!IsListening ())
			||	(pToState->IsListening ())
			)
		{
			lResult = E_UNEXPECTED;
		}
		else
		{
			if	(SUCCEEDED (lResult = pToState->GetInputContext ()))
			{
				mSapi5InputContext->RemoveEventSink (this);
				mSapi5InputContext->StopListening ();
			}
			if	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = pToState->StartInputContext (mSapi5InputContext)))
				)
			{
				pToState->GrabListenTimers (*this);
				if	(IsAutomatic ())
				{
					pToState->mCharacter.ShowListeningState (true);
				}
				ShowListeningTip (false, false);
				pToState->ShowListeningTip (true, true);

				try
				{
					mCharacter.mNotify->mGlobal->_CharacterListening (GetCharID(), false, ListenComplete_CharacterClientDeactivated);
					mCharacter.mNotify->mGlobal->_CharacterListening (pToState->GetCharID(), true, 0);
				}
				catch AnyExceptionSilent
			}
			else
			if	(lResult == AGENTVOICEERROR_NOTINSTALLED)
			{
				pToState->GrabListenTimers (*this);
				ShowListeningTip (false, false);
				pToState->ShowListeningTip (true, false, CLocalize::LoadString (IDS_COMMANDS_NO_SPEECH_INPUT, pToState->mCharacter.GetLangID()));
			}
			else
			{
				mSapi5InputContext->AddEventSink (this);
				ShowListeningTip (true, false);
			}
		}
#ifdef	_DEBUG_LISTEN
		LogComErr (_DEBUG_LISTEN, lResult, _T("[%p(%d)] TransferState [%d]"), this, GetCharID(), pToState->GetCharID());
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::GetInputContext ()
{
	HRESULT	lResult = S_OK;

	if	(!mSapi5InputContext)
	{
		mSapi5InputContext = CSapi5InputContext::CreateInstance();
	}
	if	(!mSapi5InputContext)
	{
		lResult = E_OUTOFMEMORY;
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(!mSapi5InputContext->IsInitialized())
		)
	{
		if	(
				(!mSapi5Input)
			&&	(mSapi5Input = mCharacter.GetSapiInput (true))
			)
		{
			try
			{
				CSapiInputCache *	lInputCache;

				if	(lInputCache = CSapiInputCache::GetStaticInstance ())
				{
					lInputCache->AddInputClient (mSapi5Input, this);
				}
			}
			catch AnyExceptionDebug
		}

		if	(mSapi5Input)
		{
			lResult = mSapi5InputContext->Initialize (mSapi5Input, mCharacter.GetLangID());
			if	(SUCCEEDED (lResult))
			{
				SetSapiInputClients (-1);
				SetSapiInputNames (-1);
			}
		}
		else
		{
			lResult = AGENTVOICEERROR_NOTINSTALLED;
		}
	}
	return lResult;
}

HRESULT CListeningState::StartInputContext (CSapi5InputContext * pPrevInputContext)
{
	HRESULT				lResult = S_FALSE;
	CDaCmnCommands *	lCommands;

	if	(
			(mSapi5InputContext)
		&&	(!mSapi5InputContext->IsListening ())
		)
	{
		mSapi5InputContext->RemoveEventSink (this);

		if	(
				(lCommands = mCharacter.GetCommands (true))
			&&	(lCommands->SetupVoiceContext (mSapi5InputContext))
			)
		{
			lResult = mSapi5InputContext->StartListening ();

			if	(SUCCEEDED (lResult))
			{
				mSapi5InputContext->AddEventSink (this, pPrevInputContext);
				lResult = S_OK;
			}
		}
		else
		{
			lResult = AGENTVOICEERROR_INVALIDMENU;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::ShowListeningTip (bool pShow, bool pListening, LPCTSTR pReason)
{
	HRESULT					lResult = S_FALSE;
	CAgentListeningWnd *	lListeningWnd = NULL;

	if	(
			(pShow)
		&&	(
				(pListening)
			||	(pReason)
			)
		)
	{
		if	(CDaSettingsConfig().LoadConfig().mSrListeningTip)
		{
			lListeningWnd = mCharacter.GetListeningWnd (true);
		}
	}
	else
	{
		lListeningWnd = mCharacter.GetListeningWnd (false);
	}

	if	(lListeningWnd)
	{
		if	(pShow)
		{
			CDaCmnCommands *	lCommands;
			CAtlString			lCommandsCaption;

			if	(lCommands = mCharacter.GetCommands (true))
			{
				lCommandsCaption = lCommands->GetVoiceCommandsCaption ();
			}
			if	(pListening)
			{
				lListeningWnd->ShowCharacterListening (lCommandsCaption);
			}
			else
			{
				lListeningWnd->ShowCharacterNotListening (lCommandsCaption, pReason);
			}
			lListeningWnd->ShowTipWnd ();
		}
		else
		{
			lListeningWnd->Detach (GetCharID());
		}
		lResult = S_OK;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CListeningState::StartListenTimers (bool pManual)
{
	DWORD	lHotKeyDelay;

	if	(!IsAutomatic ())
	{
		mHearingStateShown = false;
	}
	mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHeard);
	mListenTimerHeard = 0;

	if	(pManual)
	{
		if	(!IsAutomatic ())
		{
			mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerManual);
			mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerManual = mListenTimerIdManual, mListenDelayManual, this);
		}
	}
	else
	{
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerManual);
		mListenTimerManual = 0;
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHotkey);
		if	(lHotKeyDelay = CDaSettingsConfig().LoadConfig().mSrHotKeyDelay)
		{
			mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerHotkey = mListenTimerIdHotkey, lHotKeyDelay, this);
		}
		else
		{
			mListenTimerHotkey = 0;
		}
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerAuto);
		mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerAuto = mListenTimerIdAuto, 500, this);
	}
}

void CListeningState::StopListenTimers ()
{
	if	(mListenTimerManual)
	{
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerManual);
		mListenTimerManual = 0;
	}
	if	(mListenTimerAuto)
	{
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerAuto);
		mListenTimerAuto = 0;
	}
	if	(mListenTimerHotkey)
	{
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHotkey);
		mListenTimerHotkey = 0;
	}
	if	(mListenTimerHeard)
	{
		mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHeard);
		mListenTimerHeard = 0;
	}
	mHearingStateShown = false;
}

void CListeningState::GrabListenTimers (CListeningState & pFromState)
{
	CTimerNotify *	lTimer;

	if	(
			(pFromState.mListenTimerManual)
		&&	(lTimer = mCharacter.mListeningAnchor->GetListeningTimer (pFromState.mListenTimerManual))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerManual = pFromState.mListenTimerManual;
		pFromState.mListenTimerManual = 0;
	}
	if	(
			(pFromState.mListenTimerAuto)
		&&	(lTimer = mCharacter.mListeningAnchor->GetListeningTimer (pFromState.mListenTimerAuto))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerAuto = pFromState.mListenTimerAuto;
		pFromState.mListenTimerAuto = 0;
	}
	if	(
			(pFromState.mListenTimerHotkey)
		&&	(lTimer = mCharacter.mListeningAnchor->GetListeningTimer (pFromState.mListenTimerHotkey))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerHotkey = pFromState.mListenTimerHotkey;
		pFromState.mListenTimerHotkey = 0;
	}
	if	(
			(pFromState.mListenTimerHeard)
		&&	(lTimer = mCharacter.mListeningAnchor->GetListeningTimer (pFromState.mListenTimerHeard))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerHeard = pFromState.mListenTimerHeard;
		pFromState.mListenTimerHeard = 0;
	}
	mHearingStateShown = pFromState.mHearingStateShown;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CListeningState::OnTimerNotify (CTimerNotify * pTimerNotify, UINT_PTR pTimerId)
{
	if	(pTimerId)
	{
		if	(pTimerId == mListenTimerManual)
		{
#ifdef	_DEBUG_LISTEN
			LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Listening Manual timeout"), this, GetCharID());
#endif
			if	(!IsSuspended ())
			{
				if	(mListenTimerAuto)
				{
					mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerManual);
					mListenTimerManual = 0;
				}
				else
				{
					StopListening (false, ListenComplete_ProgramTimedOut);
				}
			}
		}
		else
		if	(pTimerId == mListenTimerAuto)
		{
#ifdef	_DEBUG_LISTEN_NOT
			LogMessage (_DEBUG_LISTEN|LogHighVolume, _T("[%p(%d)] Hotkey Held [%u]"), this, GetCharID(), mCharacter.mListeningAnchor->IsHotKeyStillPressed ());
#endif
			if	(!IsSuspended ())
			{
				if	(
						(!mListenTimerHotkey)
					&&	(!mCharacter.mListeningAnchor->IsHotKeyStillPressed ())
					)
				{
#ifdef	_DEBUG_LISTEN
					LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Listening AutoTimer timeout"), this, GetCharID());
#endif
					mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerAuto);
					mListenTimerAuto = 0;
					StopListening (false, ListenComplete_UserReleasedKey);
				}
			}
		}
		else
		if	(pTimerId == mListenTimerHotkey)
		{
#ifdef	_DEBUG_LISTEN
			LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Listening Hotkey timeout [%u]"), this, GetCharID(), mCharacter.mListeningAnchor->IsHotKeyStillPressed ());
#endif
			if	(!IsSuspended ())
			{
				mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHotkey);
				mListenTimerHotkey = 0;
				if	(!mCharacter.mListeningAnchor->IsHotKeyStillPressed ())
				{
					mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerAuto);
					mListenTimerAuto = 0;
					StopListening (false, ListenComplete_UserTimedOut);
				}
			}
		}
		else
		if	(pTimerId == mListenTimerHeard)
		{
			CAgentListeningWnd *	lListeningWnd;
#ifdef	_DEBUG_LISTEN
			LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Listening Heard timeout"), this, GetCharID());
#endif
			if	(!IsSuspended ())
			{
				mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHeard);
				mListenTimerHeard = 0;

				if	(IsActive ())
				{
					if	(
							(!IsAutomatic ())
						||	(!mCharacter.mListeningAnchor->IsHotKeyStillPressed ())
						)
					{
						StopListening (false, ListenComplete_UserSpeechEnded);
					}
				}
				else
				if	(
						(lListeningWnd = mCharacter.GetListeningWnd (false))
					&&	(lListeningWnd->GetCharID() == GetCharID())
					)
				{
					lListeningWnd->Detach (GetCharID());
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CListeningState::OnSapi5InputEvent (const CSpEvent & pEvent)
{
#ifdef	_DEBUG_LISTEN_NOT
	if	(pEvent.eEventId == SPEI_INTERFERENCE)
	{
		LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Interference [%s]"), this, GetCharID(), InterferenceStr (pEvent.Interference()));
	}
#endif
#ifdef	_DEBUG_LISTEN_NOT
	if	(pEvent.eEventId == SPEI_HYPOTHESIS)
	{
		LogRecoResult (_DEBUG_LISTEN, pEvent.RecoResult(), _T("[%p(%d)] Hypothesis"), this, GetCharID());
	}
#endif
#ifdef	_DEBUG_LISTEN
	if	(pEvent.eEventId == SPEI_RECOGNITION)
	{
		LogRecoResult (_DEBUG_LISTEN, pEvent.RecoResult(), _T("[%p(%d)] Recognition"), this, GetCharID());
	}
#endif
#ifdef	_DEBUG_LISTEN
	if	(
			(pEvent.eEventId == SPEI_FALSE_RECOGNITION)
		&&	(pEvent.RecoResult())
		)
	{
		LogRecoResult (_DEBUG_LISTEN, pEvent.RecoResult(), _T("[%p(%d)] False Recognition"), this, GetCharID());
	}
#endif

	if	(IsSuspended ())
	{
#ifdef	_DEBUG_LISTEN
		LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] OnSapi5InputEvent - Suspended"), this, GetCharID());
#endif	
	}
	else
	if	(pEvent.eEventId == SPEI_RECOGNITION)
	{
		CAgentListeningWnd *	lListeningWnd;
		ISpRecoResultPtr		lRecoResult = pEvent.RecoResult();
		tMallocPtr <SPPHRASE>	lPhrase;
		CDaCmnCharacter *		lCharacter;
		CDaCmnCommands *		lCommands;
		CDaCmnCommand *			lCommand = NULL;

		if	(
				(lRecoResult != NULL)
			&&	(SUCCEEDED (lRecoResult->GetPhrase (lPhrase.Free())))
			)
		{
			if	(
					(lPhrase->ullGrammarID == mSapi5InputContext->mGrammarIdCommands)
				&&	(lCommands = mCharacter.GetCommands (true))
				)
			{
				lCommand = lCommands->GetCommand ((USHORT)lPhrase->Rule.ulId);
			}

			if	(
					(lListeningWnd = mCharacter.GetListeningWnd (false))
				&&	(lListeningWnd->GetCharID() == GetCharID())
				)
			{
				CAtlString			lHeardText;
				tMallocPtr <WCHAR>	lPhraseText;
				BYTE				lPhraseDisplay;

				if	(
						(lCommand)
#ifdef	_STRICT_COMPATIBILITY
//
//	This is NOT the way it's documented in the Windows SDK, but it IS the way MS Agent works on Windows Vista
//
					&&	((short)lPhrase->Rule.Confidence*100 < lCommand->mConfidenceThreshold)
#else
					&&	((short)lPhrase->Rule.Confidence*100 <= lCommand->mConfidenceThreshold)
#endif
					)
				{
					lHeardText = lCommand->mConfidenceText;
				}
				if	(
						(lHeardText.IsEmpty ())
					&&	(SUCCEEDED (lRecoResult->GetText (SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, lPhraseText.Free(), &lPhraseDisplay)))
					)
				{
					lHeardText = (LPCWSTR)lPhraseText;
				}
				if	(!lHeardText.IsEmpty ())
				{
					lListeningWnd->ShowCharacterHeard (lHeardText);
					mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHeard);
					mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerHeard = mListenTimerIdHeard, mListenDelayHeard, this);
				}
			}

			if	(lPhrase->ullGrammarID == mSapi5InputContext->mGrammarIdGlobal)
			{
#ifdef	_DEBUG_LISTEN
				LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Heard Global command [%d]"), this, GetCharID(), lPhrase->Rule.ulId);
#endif
				lCharacter = &mCharacter;

				lCharacter->ShowListeningState (false);
				lCharacter->ShowHearingState (false);
				mHearingStateShown = false;

				if	(lCharacter->PreNotify ())
				{
					try
					{
						lCharacter->NotifyVoiceCommand (LOWORD(lPhrase->Rule.ulId), lRecoResult, true);
					}
					catch AnyExceptionSilent

					if	(!lCharacter->PostNotify ())
					{
						lCharacter = NULL; // My character has been unloaded
					}
				}

				if	(
						(lCharacter)
					&&	(lCharacter->DoMenuCommand (LOWORD(lPhrase->Rule.ulId)))
					)
				{
					if	(mListenTimerManual)
					{
						mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerManual);
						mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerManual, mListenDelayManual, this);
					}
				}
				else
				if	(
						(lCharacter) // If my character was unloaded, I've been deleted
					&&	(HIWORD (lPhrase->Rule.ulId))
					&&	(lCharacter = mCharacter.mNotify->mAnchor->mAnchor.GetGlobalCharacter (LOWORD(lPhrase->Rule.ulId)))
					)
				{
#ifdef	_DEBUG_LISTEN
					LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Heard Character name [%d] IsVisible [%d] IsClientActive [%d] IsInputActive [%d]"), this, GetCharID(), lCharacter->GetCharID(), lCharacter->IsVisible (), lCharacter->IsClientActive (), lCharacter->IsInputActive ());
#endif
					lCharacter->DoMenuActivate ();
				}
			}
			else
			if	(
					(lPhrase->ullGrammarID == mSapi5InputContext->mGrammarIdCommands)
				&&	(lCommand)
#ifdef	_STRICT_COMPATIBILITY
//
//	This is NOT the way it's documented in the Windows SDK, but it IS the way MS Agent works on Windows Vista
//
				&&	((short)lPhrase->Rule.Confidence*100 >= lCommand->mConfidenceThreshold)
#endif
				)
			{
#ifdef	_DEBUG_LISTEN
				LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] Heard Character [%d] command [%d]"), this, GetCharID(), GetCharID(), lPhrase->Rule.ulId);
#endif
				lCharacter = &mCharacter;

				lCharacter->ShowListeningState (false);
				lCharacter->ShowHearingState (false);
				mHearingStateShown = false;

				if	(lCharacter->PreNotify ())
				{
					try
					{
						lCharacter->NotifyVoiceCommand (LOWORD(lPhrase->Rule.ulId), lRecoResult, false);
					}
					catch AnyExceptionSilent

					if	(!lCharacter->PostNotify ())
					{
						lCharacter = NULL; // My character has been unloaded
					}
				}
			}
		}
	}
	else
	if	(pEvent.eEventId == SPEI_FALSE_RECOGNITION)
	{
		CAgentListeningWnd *	lListeningWnd;

		PlaySapiInputPrompt (false, _T("MisrecoSound"));

		if	(
				(lListeningWnd = mCharacter.GetListeningWnd (false))
			&&	(lListeningWnd->GetCharID() == GetCharID())
			)
		{
			lListeningWnd->ShowCharacterHeard (NULL);
		}
	}
	else
	if	(pEvent.eEventId == SPEI_START_SR_STREAM)
	{
		CAgentListeningWnd *	lListeningWnd;

#ifdef	_DEBUG_LISTEN
		LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] SPEI_START_SR_STREAM"), this, GetCharID());
#endif
		if	(
				(lListeningWnd = mCharacter.GetListeningWnd (false))
			&&	(lListeningWnd->GetCharID() == GetCharID())
			)
		{
			lListeningWnd->ShowCharacterListening (NULL);
		}
	}
	else
	if	(pEvent.eEventId == SPEI_END_SR_STREAM)
	{
		CAgentListeningWnd *	lListeningWnd;

#ifdef	_DEBUG_LISTEN
		LogMessage (_DEBUG_LISTEN, _T("[%p(%d)] SPEI_END_SR_STREAM"), this, GetCharID());
#endif
		if	(
				(lListeningWnd = mCharacter.GetListeningWnd (false))
			&&	(lListeningWnd->GetCharID() == GetCharID())
			)
		{
			lListeningWnd->ShowCharacterNotListening (NULL);
		}
	}
	else
	if	(pEvent.eEventId == SPEI_SOUND_START)
	{
		if	(!mHearingStateShown)
		{
			mCharacter.ShowHearingState (true);
			mHearingStateShown = true; // Don't care if ShowHearingState succeeded or failed.

			if	(mListenTimerHotkey)
			{
				mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerHotkey);
				mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerHotkey = mListenTimerIdHotkey, CDaSettingsConfig().LoadConfig().mSrHotKeyDelay, this);
			}
			else
			if	(mListenTimerManual)
			{
				mCharacter.mListeningAnchor->DelListeningTimer (mListenTimerManual);
				mCharacter.mListeningAnchor->AddListeningTimer (mListenTimerManual = mListenTimerIdManual, mListenDelayManual, this);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////

bool CListeningState::PlaySapiInputPrompt (bool pManual, LPCTSTR pSoundName)
{
	CRegKeyEx	lRootKey (HKEY_CURRENT_USER, _T("AppEvents\\Schemes\\Apps\\sapisvr"), true);
	CRegKeyEx	lSoundKey (lRootKey, pSoundName, true);
	CRegKeyEx	lFileKey (lSoundKey, _T(".current"), true);

	if	(
			(!lFileKey.Value().Value().IsEmpty ())
		&&	(PlaySound (lFileKey.Value().Expand().Value(), NULL, SND_FILENAME|SND_NODEFAULT|SND_ASYNC))
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CListeningState::SetSapiInputClients (long pCharID)
{
	if	(mSapi5InputContext)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = mCharacter.mNotify->mAnchor->mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				CDaCmnCharacter *					lCharacter;

				if	(mCharacter.mNotify->mAnchor->mAnchor.GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							)
						{
							mSapi5InputContext->SetCharacterClient (lCharacter->GetCharID(), lCharacter->GetActiveClient());
							if	(pCharID > 0)
							{
								break;
							}
						}
					}
					if	(lClientNdx >= 0)
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void CListeningState::SetSapiInputNames (long pCharID)
{
	if	(mSapi5InputContext)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = mCharacter.mNotify->mAnchor->mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				CDaCmnCharacter *						lCharacter;
				CDaCmnCommands *						lCommands;
				BSTR								lName;

				if	(mCharacter.mNotify->mAnchor->mAnchor.GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							&&	(lName = lCharacter->GetName ())
							)
						{
							lCommands = lCharacter->GetCommands (true);
							mSapi5InputContext->SetCharacterName (lCharacter->GetCharID(), CAtlString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
							if	(pCharID > 0)
							{
								break;
							}
						}
					}
					if	(lClientNdx >= 0)
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CListeningState::_OnCharacterLoaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningState::_OnCharacterLoaded [%d]"), pCharID);
#endif
	SetSapiInputClients (pCharID);
	SetSapiInputNames (pCharID);
}

void CListeningState::_OnCharacterUnloaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningState::_OnCharacterUnloaded [%d]"), pCharID);
#endif
	if	(mSapi5InputContext)
	{
		mSapi5InputContext->RemoveCharacter (pCharID);
	}
}

void CListeningState::_OnCharacterNameChanged (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningState::_OnCharacterNameChanged [%d]"), pCharID);
#endif
	SetSapiInputNames (pCharID);
}

void CListeningState::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningState::_OnCharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
	try
	{
		if	(pActiveCharID > 0)
		{
			SetSapiInputClients (pActiveCharID);
			SetSapiInputNames (pActiveCharID);
		}
		if	(pInactiveCharID > 0)
		{
			SetSapiInputClients (-1);
			SetSapiInputNames (pInactiveCharID);
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CListeningAnchor::CListeningAnchor (CListeningGlobal & pGlobal)
:	mGlobal (pGlobal),
	mOwnerWnd (NULL),
	mHotKeyWnd (NULL),
	mStarted (false)
{
}

CListeningAnchor::~CListeningAnchor ()
{
	Shutdown ();
}

/////////////////////////////////////////////////////////////////////////////

void CListeningAnchor::Startup (HWND pHotKeyWnd)
{
#ifdef	_DEBUG_START_STOP
	LogMessage (_DEBUG_START_STOP, _T("CListeningAnchor::Startup [%p]"), pHotKeyWnd);
#endif
	if	(mStarted)
	{
		Shutdown ();
	}
	mHotKeyWnd = pHotKeyWnd;
	mGlobal.AddHotKeyWnd (mHotKeyWnd);
	mStarted = true;
}

void CListeningAnchor::Shutdown ()
{
#ifdef	_DEBUG_START_STOP
	LogMessage (_DEBUG_START_STOP, _T("CListeningAnchor::Shutdown [%p] [%d]"), mHotKeyWnd, mTimerNotifies.GetCount());
#endif
	try
	{
		mTimerNotifies.DeleteAll ();
	}
	catch AnyExceptionSilent

	if	(mHotKeyWnd)
	{
		mGlobal.RemoveHotKeyWnd (mHotKeyWnd);
		mHotKeyWnd = NULL;
	}
	mStarted = false;
}

/////////////////////////////////////////////////////////////////////////////

CVoiceCommandsWnd * CListeningAnchor::GetVoiceCommandsWnd (bool pCreate, long pCharID)
{
	CVoiceCommandsWnd *	lRet = mGlobal.GetVoiceCommandsWnd (pCreate, mOwnerWnd);

	if	(pCharID > 0)
	{
		mGlobal.SetVoiceCommandCharacter (pCharID);
	}
	return lRet;
}

bool CListeningAnchor::IsHotKeyStillPressed () const
{
	return mGlobal.IsHotKeyStillPressed ();
}

/////////////////////////////////////////////////////////////////////////////

bool CListeningAnchor::AddListeningTimer (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink)
{
	return AddTimerNotify (NULL, pTimerId, pInterval, pNotifySink);
}

bool CListeningAnchor::DelListeningTimer (UINT_PTR pTimerId)
{
	return DelTimerNotify (NULL, pTimerId);
}

bool CListeningAnchor::HasListeningTimer (UINT_PTR pTimerId)
{
	return (mTimerNotifies.FindTimer (pTimerId) >= 0);
}

CTimerNotify * CListeningAnchor::GetListeningTimer (UINT_PTR pTimerId)
{
	return mTimerNotifies.GetTimer (pTimerId);
}

/////////////////////////////////////////////////////////////////////////////

bool CListeningAnchor::AddTimerNotify (HWND pTimerWnd, UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink)
{
	bool			lRet = false;
	CTimerNotify *	lTimer;

	if	(
			(pTimerId != 0)
		&&	(pNotifySink != NULL)
		&&	(pInterval >= USER_TIMER_MINIMUM)
		&&	(mTimerNotifies.FindTimer (pTimerId) < 0)
		&&	(lTimer = new CTimerNotify (pTimerId, pNotifySink))
		)
	{
		mTimerNotifies.Add (lTimer);

		if	(lTimer->StartTimer (pTimerWnd, pInterval))
		{
			lRet = true;
		}
		else
		{
			mTimerNotifies.Remove (lTimer);
		}
	}
	return lRet;
}

bool CListeningAnchor::DelTimerNotify (HWND pTimerWnd, UINT_PTR pTimerId)
{
	bool				lRet = false;
	tPtr <CTimerNotify>	lTimer;

	if	(lTimer = mTimerNotifies.GetTimer (pTimerId))
	{
		mTimerNotifies.Remove (lTimer);
		lTimer->StopTimer (pTimerWnd);
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CListeningGlobal::CListeningGlobal (class CGlobalAnchor & pAnchor)
:	mAnchor (pAnchor),
	mLastHotKey (0),
	mStarted (false),
	mSuspended (false)
{
}

CListeningGlobal::~CListeningGlobal ()
{
	Shutdown ();
}

/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::Startup ()
{
#ifdef	_DEBUG_START_STOP
	LogMessage (_DEBUG_START_STOP, _T("CListeningGlobal::Startup"));
#endif
	if	(mStarted)
	{
		Shutdown ();
	}
	mStarted = true;
	mSuspended = true;
}

void CListeningGlobal::Shutdown ()
{
#ifdef	_DEBUG_START_STOP
	LogMessage (_DEBUG_START_STOP, _T("CListeningGlobal::Shutdown [%p]"), mVoiceCommandsWnd.Ptr());
#endif
	UnregisterHotKeys ();
	mHotKeyWnds.RemoveAll ();
	SafeFreeSafePtr (mVoiceCommandsWnd);
	mStarted = false;
}

void CListeningGlobal::Suspend ()
{
#ifdef	_DEBUG_START_STOP
	LogMessage (_DEBUG_START_STOP, _T("CListeningGlobal::Suspend"));
#endif
	if	(mStarted)
	{
		UnregisterHotKeys ();
		mSuspended = true;
	}
}

void CListeningGlobal::Resume ()
{
#ifdef	_DEBUG_START_STOP
	LogMessage (_DEBUG_START_STOP, _T("CListeningGlobal::Resume"));
#endif
	if	(
			(mStarted)
		&&	(mSuspended)
		)
	{
		RegisterHotKeys ();
		mSuspended = false;
	}
}

/////////////////////////////////////////////////////////////////////////////

CVoiceCommandsWnd * CListeningGlobal::GetVoiceCommandsWnd (bool pCreate, CWindow * pOwnerWnd)
{
	if	(
			(!mVoiceCommandsWnd)
		&&	(pCreate)
		&&	(mVoiceCommandsWnd = CVoiceCommandsWnd::CreateInstance ())
		)
	{
		if	(mVoiceCommandsWnd->Create (pOwnerWnd))
		{
			SetVoiceCommandClients (-1);
			SetVoiceCommandNames (-1);
		}
		else
		{
			SafeFreeSafePtr (mVoiceCommandsWnd);
		}
	}
	return mVoiceCommandsWnd;
}

/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::SetVoiceCommandCharacter (long pCharID)
{
	if	(
			(mVoiceCommandsWnd)
		&&	(pCharID > 0)
		&&	(mVoiceCommandsWnd->GetCharID() != pCharID)
		)
	{
		try
		{
			CDaCmnCharacter *	lCharacter;
			CDaCmnCommands *	lCommands = NULL;
			BSTR				lName = NULL;

			if	(lCharacter = mAnchor.GetGlobalCharacter (pCharID))
			{
				lName = lCharacter->GetName ();
				lCommands = lCharacter->GetCommands (true);
			}
			mVoiceCommandsWnd->SetCharacter (pCharID, CAtlString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::SetVoiceCommandClients (long pCharID)
{
	if	(mVoiceCommandsWnd)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				CDaCmnCharacter *					lCharacter;

				if	(mAnchor.GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							)
						{
							mVoiceCommandsWnd->SetCharacterClient (lCharacter->GetCharID(), lCharacter->GetActiveClient());
							if	(pCharID > 0)
							{
								break;
							}
						}
					}
					if	(lClientNdx >= 0)
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::SetVoiceCommandNames (long pCharID)
{
	if	(mVoiceCommandsWnd)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				CDaCmnCharacter *					lCharacter;
				CDaCmnCommands *					lCommands;
				BSTR								lName;

				if	(mAnchor.GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							&&	(lName = lCharacter->GetName())
							)
						{
							lCommands = lCharacter->GetCommands (true);
							mVoiceCommandsWnd->SetCharacterName (lCharacter->GetCharID(), CAtlString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
							if	(pCharID > 0)
							{
								break;
							}
						}
					}
					if	(lClientNdx >= 0)
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

const int	CListeningGlobal::mHotKeyRegisterId = 1;

#ifndef	MOD_NOREPEAT
#define MOD_NOREPEAT	0x4000
#endif

/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::AddHotKeyWnd (HWND pHotKeyWnd)
{
	if	(
			(mHotKeyWnds.AddUnique (pHotKeyWnd) >= 0)
		&&	(mStarted)
		&&	(!mSuspended)
		)
	{
		RegisterHotKey (pHotKeyWnd);
	}
}

void CListeningGlobal::RemoveHotKeyWnd (HWND pHotKeyWnd)
{
	UnregisterHotKey (pHotKeyWnd);
	mHotKeyWnds.Remove (pHotKeyWnd);
}

/////////////////////////////////////////////////////////////////////////////

bool CListeningGlobal::RegisterHotKey (HWND pHotKeyWnd)
{
	bool				lRet = false;
	CDaSettingsConfig	lConfig;
	UINT				lHotKeyCode;
	UINT				lHotKeyMod = 0;

#ifdef	_DEBUG_HOT_KEY
	LogMessage (_DEBUG_HOT_KEY, _T("CListeningGlobal::RegisterHotKey [%p] [%d]"), pHotKeyWnd, mHotKeyRegisterId);
#endif
	::UnregisterHotKey (pHotKeyWnd, mHotKeyRegisterId);

	if	(LOBYTE (lConfig.LoadConfig().mSrHotKey) != 0)
	{
		lHotKeyCode = LOBYTE (lConfig.mSrHotKey);
		if	(IsWindows7_AtLeast ())
		{
			lHotKeyMod |= MOD_NOREPEAT;
		}
		if	(HIBYTE (lConfig.mSrHotKey) & HOTKEYF_ALT)
		{
			lHotKeyMod |= MOD_ALT;
		}
		if	(HIBYTE (lConfig.mSrHotKey) & HOTKEYF_CONTROL)
		{
			lHotKeyMod |= MOD_CONTROL;
		}
		if	(HIBYTE (lConfig.mSrHotKey) & HOTKEYF_SHIFT)
		{
			lHotKeyMod |= MOD_SHIFT;
		}
		if	(::RegisterHotKey (pHotKeyWnd, mHotKeyRegisterId, lHotKeyMod, lHotKeyCode))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CListeningGlobal::UnregisterHotKey (HWND pHotKeyWnd)
{
	bool	lRet = false;

#ifdef	_DEBUG_HOT_KEY
	LogMessage (_DEBUG_HOT_KEY, _T("CListeningGlobal::UnregisterHotKey [%p] [%d]"), pHotKeyWnd, mHotKeyRegisterId);
#endif
	if	(::UnregisterHotKey (pHotKeyWnd, mHotKeyRegisterId))
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::RegisterHotKeys ()
{
	if	(mAnchor.IsAppActive ())
	{
		bool	lGlobalRegistered = false;
		INT_PTR	lNdx;

		for	(lNdx = 0; lNdx < (INT_PTR)mHotKeyWnds.GetCount(); lNdx++)
		{
			if	(mHotKeyWnds [lNdx])
			{
				RegisterHotKey (mHotKeyWnds [lNdx]);
			}
			else
			if	(!lGlobalRegistered)
			{
				RegisterHotKey (NULL);
				lGlobalRegistered = true;
			}
		}
	}
	mLastHotKey = 0;
}

void CListeningGlobal::UnregisterHotKeys ()
{
	INT_PTR	lNdx;

	for	(lNdx = 0; lNdx < (INT_PTR)mHotKeyWnds.GetCount(); lNdx++)
	{
		UnregisterHotKey (mHotKeyWnds [lNdx]);
	}
	mLastHotKey = 0;
}

/////////////////////////////////////////////////////////////////////////////

bool CListeningGlobal::OnHotKey (WPARAM wParam, LPARAM lParam)
{
	if	(
			(mStarted)
		&&	(!mSuspended)
		&&	(wParam == (WPARAM) mHotKeyRegisterId)
		)
	{
		mLastHotKey = HIWORD (lParam);
		if	(LOWORD (lParam) & MOD_ALT)
		{
			mLastHotKey |= VK_MENU << 8;
		}
		if	(LOWORD (lParam) & MOD_CONTROL)
		{
			mLastHotKey |= VK_CONTROL << 16;
		}
		if	(LOWORD (lParam) & MOD_SHIFT)
		{
			mLastHotKey |= VK_SHIFT << 24;
		}

#ifdef	_DEBUG_HOT_KEY
		LogMessage (_DEBUG_HOT_KEY, _T("CListeningGlobal::OnHotKey [%8.8X]"), mLastHotKey);
#endif
		try
		{
			CDaCmnCharacter *	lCharacter = mAnchor.GetGlobalCharacter (mAnchor.GetListenCharacter ());

			if	(lCharacter)
			{
				LogComErr (LogDetails, lCharacter->StartListening (false));
			}
		}
		catch AnyExceptionDebug

		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CListeningGlobal::IsHotKeyStillPressed () const
{
	if	(
			(mStarted)
		&&	(!mSuspended)
		&&	(LOBYTE (LOWORD (mLastHotKey)))
		&&	(GetAsyncKeyState (LOBYTE (LOWORD (mLastHotKey))) < 0)
		&&	(
				(!HIBYTE (LOWORD (mLastHotKey)))
			||	(GetAsyncKeyState (HIBYTE (LOWORD (mLastHotKey))) < 0)
			)
		&&	(
				(!LOBYTE (HIWORD (mLastHotKey)))
			||	(GetAsyncKeyState (LOBYTE (HIWORD (mLastHotKey))) < 0)
			)
		&&	(
				(!HIBYTE (HIWORD (mLastHotKey)))
			||	(GetAsyncKeyState (HIBYTE (HIWORD (mLastHotKey))) < 0)
			)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CListeningGlobal::_CharacterLoaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningGlobal::_CharacterLoaded [%d]"), pCharID);
#endif
	SetVoiceCommandClients (pCharID);
	SetVoiceCommandNames (pCharID);
}

void CListeningGlobal::_CharacterUnloaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningGlobal::_CharacterUnloaded [%d]"), pCharID);
#endif
	if	(mVoiceCommandsWnd)
	{
		mVoiceCommandsWnd->RemoveCharacter (pCharID);
	}
}

void CListeningGlobal::_CharacterNameChanged (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningGlobal::_CharacterNameChanged [%d]"), pCharID);
#endif
	SetVoiceCommandNames (pCharID);
}

void CListeningGlobal::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CListeningGlobal::_CharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
	if	(pActiveCharID > 0)
	{
		SetVoiceCommandClients (pActiveCharID);
	}
	if	(pInactiveCharID > 0)
	{
		SetVoiceCommandClients (pInactiveCharID);
	}
}

void CListeningGlobal::_AppActivated (bool pActive)
{
	if	(pActive)
	{
		Resume ();
	}
	else
	{
		Suspend ();
	}
}

void CListeningGlobal::_OptionsChanged ()
{
	if	(
			(mStarted)
		&&	(!mSuspended)
		)
	{
		RegisterHotKeys ();
	}
}
