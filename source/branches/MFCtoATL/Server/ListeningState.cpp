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
#include "StdAfx.h"
#include "DaServerApp.h"
#include "DaSvrCharacter.h"
#include "DaSvrCommands.h"
#include "DaSvrUserInput.h"
#include "DaGlobalConfig.h"
#include "ListeningState.h"
#include "AgentPopupWnd.h"
#include "AgentListeningWnd.h"
#include "Sapi5Input.h"
#include "SapiInputCache.h"
#include "Registry.h"
#include "Localize.h"
#include "MallocPtr.h"
#include <sphelper.h>

#pragma comment(lib, "winmm.lib")

#ifdef	_DEBUG
//#define	_DEBUG_SPEECH	LogNormal|LogTime|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT_PTR	CListeningState::mListenTimerIdManual = RegisterWindowMessage (_T("9C726EFD-8DC7-466A-9204-CCA3FAC4A2F9"));
const UINT_PTR	CListeningState::mListenTimerIdAuto = RegisterWindowMessage (_T("4D5E66E2-FA40-4312-874E-12867C2B6559"));
const UINT_PTR	CListeningState::mListenTimerIdHotkey = RegisterWindowMessage (_T("D6C3E7DA-3A76-4245-8465-0670EE66BACB"));
const UINT_PTR	CListeningState::mListenTimerIdHeard = RegisterWindowMessage (_T("A9FE3C4D-D99F-411E-BAD8-F0A0773F1CA4"));

/////////////////////////////////////////////////////////////////////////////

CListeningState::CListeningState (DaSvrCharacter & pCharacter)
:	mCharacter (pCharacter),
	mSapi5Input (NULL),
	mHearingStateShown (false),
	mListenDelayManual (10000),
	mListenDelayHeard (3000),
	mListenTimerManual (0),
	mListenTimerAuto (0),
	mListenTimerHotkey (0),
	mListenTimerHeard (0)
{
	mCharacter.mNotify->_RegisterInternalNotify (this, true);
}

CListeningState::~CListeningState()
{
	mCharacter.mNotify->_RegisterInternalNotify (this, false);

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
	HRESULT					lResult = S_OK;
	CDaSpeechInputConfig	lInputConfig;

#ifdef	_DEBUG_SPEECH
	LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] StartListening Manual [%u] Enabled [%u] Active [%u] Listening [%u %u] CharActive [%u]"), this, GetCharID(), pManual, CDaSpeechInputConfig().LoadConfig().mEnabled, IsActive(), IsListening(), (mSapi5InputContext.Ptr()?mSapi5InputContext->FindEventSink(this):false), (mCharacter.mNotify->_GetActiveCharacter()==GetCharID()));
#endif

	lInputConfig.LoadConfig ();

	if	(!lInputConfig.mEnabled)
	{
		lResult = AGENTVOICEERROR_SPEECHDISABLED;
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
					_AtlModule.OnCharacterListening (GetCharID(), true, 0);
				}
				catch AnyExceptionSilent

				if	(lInputConfig.mListeningPrompt)
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
#ifdef	_DEBUG_SPEECH
	LogComErr (_DEBUG_SPEECH, lResult, _T("[%p] [%d] StartListening"), this, GetCharID());
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
#ifdef	_DEBUG_SPEECH
		CString	lCauseStr;
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
		LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] StopListening Manual [%u] Active [%u] Listening [%u %u] CharActive [%u] Cause [%s]"), this, GetCharID(), pManual, IsActive(), IsListening(), (mSapi5InputContext.Ptr()?mSapi5InputContext->FindEventSink(this):false), (mCharacter.mNotify->_GetActiveCharacter()==GetCharID()), lCauseStr);
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
				&&	(CDaSpeechInputConfig().LoadConfig().mListeningPrompt)
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
					_AtlModule.OnCharacterListening (GetCharID(), false, pCause);
				}
				catch AnyExceptionSilent
			}
		}
#ifdef	_DEBUG_SPEECH
		LogComErr (_DEBUG_SPEECH, lResult, _T("[%p] [%d] StopListening"), this, GetCharID());
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CListeningState::KeepListening (bool pManual)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(!pManual)
		&&	(IsAutomatic ())
		&&	(IsListening ())
		)
	{
		DWORD	lHotKeyDelay;

		_AtlModule.DelTimerNotify (mListenTimerHotkey);
		if	(lHotKeyDelay = CDaSpeechInputConfig().LoadConfig().mHotKeyDelay)
		{
			_AtlModule.AddTimerNotify (mListenTimerHotkey = mListenTimerIdHotkey, lHotKeyDelay, this);
		}
		else
		{
			mListenTimerHotkey = 0;
		}
		lResult = S_OK;
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
#ifdef _DEBUG_SPEECH
		LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] TransferState [%p] [%d] Manual [%u] Automatic [%u]"), this, GetCharID(), pToState, pToState->GetCharID(), IsManual(), IsAutomatic());
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
					_AtlModule.OnCharacterListening (GetCharID(), false, ListenComplete_CharacterClientDeactivated);
					_AtlModule.OnCharacterListening (pToState->GetCharID(), true, 0);
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
#ifdef	_DEBUG_SPEECH
		LogComErr (_DEBUG_SPEECH, lResult, _T("[%p] [%d] TransferState [%d]"), this, GetCharID(), pToState->GetCharID());
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
	DaSvrCommands *	lCommands;

	if	(
			(mSapi5InputContext)
		&&	(!mSapi5InputContext->IsListening ())
		)
	{
		mSapi5InputContext->RemoveEventSink (this);

		if	(
				(lCommands = mCharacter.GetCommandsObj (true))
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
		if	(CDaSpeechInputConfig().LoadConfig().mListeningTip)
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
			DaSvrCommands *	lCommands;
			CString				lCommandsCaption;

			if	(lCommands = mCharacter.GetCommandsObj (true))
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
	_AtlModule.DelTimerNotify (mListenTimerHeard);
	mListenTimerHeard = 0;

	if	(pManual)
	{
		if	(!IsAutomatic ())
		{
			_AtlModule.DelTimerNotify (mListenTimerManual);
			_AtlModule.AddTimerNotify (mListenTimerManual = mListenTimerIdManual, mListenDelayManual, this);
		}
	}
	else
	{
		_AtlModule.DelTimerNotify (mListenTimerManual);
		mListenTimerManual = 0;
		_AtlModule.DelTimerNotify (mListenTimerHotkey);
		if	(lHotKeyDelay = CDaSpeechInputConfig().LoadConfig().mHotKeyDelay)
		{
			_AtlModule.AddTimerNotify (mListenTimerHotkey = mListenTimerIdHotkey, lHotKeyDelay, this);
		}
		else
		{
			mListenTimerHotkey = 0;
		}
		_AtlModule.DelTimerNotify (mListenTimerAuto);
		_AtlModule.AddTimerNotify (mListenTimerAuto = mListenTimerIdAuto, 500, this);
	}
}

void CListeningState::StopListenTimers ()
{
	if	(mListenTimerManual)
	{
		_AtlModule.DelTimerNotify (mListenTimerManual);
		mListenTimerManual = 0;
	}
	if	(mListenTimerAuto)
	{
		_AtlModule.DelTimerNotify (mListenTimerAuto);
		mListenTimerAuto = 0;
	}
	if	(mListenTimerHotkey)
	{
		_AtlModule.DelTimerNotify (mListenTimerHotkey);
		mListenTimerHotkey = 0;
	}
	if	(mListenTimerHeard)
	{
		_AtlModule.DelTimerNotify (mListenTimerHeard);
		mListenTimerHeard = 0;
	}
	mHearingStateShown = false;
}

void CListeningState::GrabListenTimers (CListeningState & pFromState)
{
	CTimerNotify *	lTimer;

	if	(
			(pFromState.mListenTimerManual)
		&&	(lTimer = _AtlModule.GetTimerNotify (pFromState.mListenTimerManual))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerManual = pFromState.mListenTimerManual;
		pFromState.mListenTimerManual = 0;
	}
	if	(
			(pFromState.mListenTimerAuto)
		&&	(lTimer = _AtlModule.GetTimerNotify (pFromState.mListenTimerAuto))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerAuto = pFromState.mListenTimerAuto;
		pFromState.mListenTimerAuto = 0;
	}
	if	(
			(pFromState.mListenTimerHotkey)
		&&	(lTimer = _AtlModule.GetTimerNotify (pFromState.mListenTimerHotkey))
		)
	{
		lTimer->SetNotifySink (this);
		mListenTimerHotkey = pFromState.mListenTimerHotkey;
		pFromState.mListenTimerHotkey = 0;
	}
	if	(
			(pFromState.mListenTimerHeard)
		&&	(lTimer = _AtlModule.GetTimerNotify (pFromState.mListenTimerHeard))
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
#ifdef	_DEBUG_SPEECH
			LogMessage (_DEBUG_SPEECH, _T("[%p] Listening Manual timeout"), this);
#endif
			if	(mListenTimerAuto)
			{
				_AtlModule.DelTimerNotify (mListenTimerManual);
				mListenTimerManual = 0;
			}
			else
			{
				StopListening (false, ListenComplete_ProgramTimedOut);
			}
		}
		else
		if	(pTimerId == mListenTimerAuto)
		{
#ifdef	_DEBUG_SPEECH_NOT
			LogMessage (_DEBUG_SPEECH|LogHighVolume, _T("[%p] Hotkey Held [%u]"), this, _AtlModule.IsHotKeyStillPressed ());
#endif
			if	(
					(!mListenTimerHotkey)
				&&	(!_AtlModule.IsHotKeyStillPressed ())
				)
			{
#ifdef	_DEBUG_SPEECH
				LogMessage (_DEBUG_SPEECH, _T("[%p] Listening AutoTimer timeout"), this);
#endif
				_AtlModule.DelTimerNotify (mListenTimerAuto);
				mListenTimerAuto = 0;
				StopListening (false, ListenComplete_UserReleasedKey);
			}
		}
		else
		if	(pTimerId == mListenTimerHotkey)
		{
#ifdef	_DEBUG_SPEECH
			LogMessage (_DEBUG_SPEECH, _T("[%p] Listening Hotkey timeout [%u]"), this, _AtlModule.IsHotKeyStillPressed ());
#endif
			_AtlModule.DelTimerNotify (mListenTimerHotkey);
			mListenTimerHotkey = 0;
			if	(!_AtlModule.IsHotKeyStillPressed ())
			{
				_AtlModule.DelTimerNotify (mListenTimerAuto);
				mListenTimerAuto = 0;
				StopListening (false, ListenComplete_UserTimedOut);
			}
		}
		else
		if	(pTimerId == mListenTimerHeard)
		{
			CAgentListeningWnd *	lListeningWnd;
#ifdef	_DEBUG_SPEECH
			LogMessage (_DEBUG_SPEECH, _T("[%p] Listening Heard timeout"), this);
#endif
			_AtlModule.DelTimerNotify (mListenTimerHeard);
			mListenTimerHeard = 0;

			if	(IsActive ())
			{
				if	(
						(!IsAutomatic ())
					||	(!_AtlModule.IsHotKeyStillPressed ())
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

/////////////////////////////////////////////////////////////////////////////

void CListeningState::OnSapi5InputEvent (const CSpEvent & pEvent)
{
#ifdef	_DEBUG_SPEECH_NOT
	if	(pEvent.eEventId == SPEI_INTERFERENCE)
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] Interference [%s]"), this, InterferenceStr (pEvent.Interference()));
	}
#endif
#ifdef	_DEBUG_SPEECH_NOT
	if	(pEvent.eEventId == SPEI_HYPOTHESIS)
	{
		LogRecoResult (_DEBUG_SPEECH, pEvent.RecoResult(), _T("[%p] Hypothesis"), this);
	}
#endif
#ifdef	_DEBUG_SPEECH
	if	(pEvent.eEventId == SPEI_RECOGNITION)
	{
		LogRecoResult (_DEBUG_SPEECH, pEvent.RecoResult(), _T("[%p] Recognition"), this);
	}
#endif
#ifdef	_DEBUG_SPEECH
	if	(
			(pEvent.eEventId == SPEI_FALSE_RECOGNITION)
		&&	(pEvent.RecoResult())
		)
	{
		LogRecoResult (_DEBUG_SPEECH, pEvent.RecoResult(), _T("[%p] False Recognition"), this);
	}
#endif

	if	(pEvent.eEventId == SPEI_RECOGNITION)
	{
		CAgentListeningWnd *	lListeningWnd;
		ISpRecoResultPtr		lRecoResult = pEvent.RecoResult();
		tMallocPtr <SPPHRASE>	lPhrase;
		DaSvrCharacter *		lCharacter;
		DaSvrCommands *		lCommands;
		DaSvrCommand *		lCommand = NULL;
		DaSvrUserInput *		lUserInput;
		IDaSvrUserInputPtr		lNotifyUserInput;

		if	(
				(lRecoResult != NULL)
			&&	(SUCCEEDED (lRecoResult->GetPhrase (lPhrase.Free())))
			)
		{
			if	(
					(lPhrase->ullGrammarID == mSapi5InputContext->mGrammarIdCommands)
				&&	(lCommands = mCharacter.GetCommandsObj (true))
				)
			{
				lCommand = lCommands->GetCommand ((USHORT)lPhrase->Rule.ulId);
			}

			if	(
					(lListeningWnd = mCharacter.GetListeningWnd (false))
				&&	(lListeningWnd->GetCharID() == GetCharID())
				)
			{
				CString				lHeardText;
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
					_AtlModule.DelTimerNotify (mListenTimerHeard);
					_AtlModule.AddTimerNotify (mListenTimerHeard = mListenTimerIdHeard, mListenDelayHeard, this);
				}
			}

			if	(lPhrase->ullGrammarID == mSapi5InputContext->mGrammarIdGlobal)
			{
#ifdef	_DEBUG_SPEECH
				LogMessage (_DEBUG_SPEECH, _T("[%p] Heard Global command [%d]"), this, lPhrase->Rule.ulId);
#endif
				lCharacter = &mCharacter;

				lCharacter->ShowListeningState (false);
				lCharacter->ShowHearingState (false);
				mHearingStateShown = false;

				if	(lUserInput = DaSvrUserInput::CreateInstance (lRecoResult, true))
				{
					lNotifyUserInput = lUserInput->GetControllingUnknown();
				}

				if	(lCharacter->PreNotify ())
				{
					try
					{
#ifdef	_STRICT_COMPATIBILITY
						lCharacter->mNotify->Command (lPhrase->Rule.ulId, lNotifyUserInput);
#else
						lCharacter->mNotify->Command (0, lNotifyUserInput);
#endif
					}
					catch AnyExceptionSilent

					if	(!lCharacter->PostNotify ())
					{
						lCharacter = NULL; // My character has been unloaded
					}
				}

				if	(
						(lCharacter)
					&&	(lCharacter->DoMenuCommand ((USHORT)lPhrase->Rule.ulId))
					)
				{
					if	(mListenTimerManual)
					{
						_AtlModule.DelTimerNotify (mListenTimerManual);
						_AtlModule.AddTimerNotify (mListenTimerManual, mListenDelayManual, this);
					}
				}
				else
				if	(
						(lCharacter) // If my character was unloaded, I've been deleted
					&&	(HIWORD (lPhrase->Rule.ulId))
					&&	(lCharacter = _AtlModule.GetAppCharacter (LOWORD(lPhrase->Rule.ulId)))
					)
				{
#ifdef	_DEBUG_SPEECH
					LogMessage (_DEBUG_SPEECH, _T("[%p] Heard Character name [%d] IsVisible [%d] IsClientActive [%d] IsInputActive [%d]"), this, lCharacter->GetCharID(), lCharacter->IsVisible (), lCharacter->IsClientActive (), lCharacter->IsInputActive ());
#endif
					if	(lCharacter->IsVisible ())
					{
						if	(
								(!lCharacter->IsInputActive ())
							||	(!lCharacter->IsClientActive ())
							)
						{
							lCharacter->SetClientActive (true, true);
						}
					}
					else
					{
						if	(!lCharacter->IsClientActive ())
						{
							lCharacter->SetClientActive (true, false);
						}
						lCharacter->Show (false);
					}
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
#ifdef	_DEBUG_SPEECH
				LogMessage (_DEBUG_SPEECH, _T("[%p] Heard Character [%d] command [%d]"), this, GetCharID(), lPhrase->Rule.ulId);
#endif
				lCharacter = &mCharacter;

				lCharacter->ShowListeningState (false);
				lCharacter->ShowHearingState (false);
				mHearingStateShown = false;

				if	(lUserInput = DaSvrUserInput::CreateInstance (lRecoResult, false))
				{
					lNotifyUserInput = lUserInput->GetControllingUnknown();
				}

				if	(lCharacter->PreNotify ())
				{
					try
					{
						lCharacter->mNotify->Command (lPhrase->Rule.ulId, lNotifyUserInput);
					}
					catch AnyExceptionSilent

					if	(!lCharacter->PostNotify ())
					{
						lCharacter = NULL; // My character has been unloaded
					}
				}
			}

			SafeFreeSafePtr (lNotifyUserInput);
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

#ifdef	_DEBUG_SPEECH
		LogMessage (_DEBUG_SPEECH, _T("[%p] SPEI_START_SR_STREAM"), this);
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

#ifdef	_DEBUG_SPEECH
		LogMessage (_DEBUG_SPEECH, _T("[%p] SPEI_END_SR_STREAM"), this);
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
				_AtlModule.DelTimerNotify (mListenTimerHotkey);
				_AtlModule.AddTimerNotify (mListenTimerHotkey = mListenTimerIdHotkey, CDaSpeechInputConfig().LoadConfig().mHotKeyDelay, this);
			}
			else
			if	(mListenTimerManual)
			{
				_AtlModule.DelTimerNotify (mListenTimerManual);
				_AtlModule.AddTimerNotify (mListenTimerManual = mListenTimerIdManual, mListenDelayManual, this);
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

			for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				DaSvrCharacter *					lCharacter;

				if	(_AtlModule.GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
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

			for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				DaSvrCharacter *					lCharacter;
				DaSvrCommands *					lCommands;
				BSTR								lName;

				if	(_AtlModule.GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							&&	(lName = lCharacter->GetName ())
							)
						{
							lCommands = lCharacter->GetCommandsObj (true);
							mSapi5InputContext->SetCharacterName (lCharacter->GetCharID(), CString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
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
	SetSapiInputClients (pCharID);
	SetSapiInputNames (pCharID);
}

void CListeningState::_OnCharacterUnloaded (long pCharID)
{
	if	(mSapi5InputContext)
	{
		mSapi5InputContext->RemoveCharacter (pCharID);
	}
}

void CListeningState::_OnCharacterNameChanged (long pCharID)
{
	SetSapiInputNames (pCharID);
}

void CListeningState::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
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
