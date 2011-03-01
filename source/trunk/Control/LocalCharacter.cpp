/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "LocalCharacter.h"
#include "AgentPopupWnd.h"
#include "AgentBalloonWnd.h"
#include "VoiceCommandsWnd.h"
#include "DaCtlBalloon.h"
#include "DaCtlCommands.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_ACTIVE		(GetProfileDebugInt(_T("DebugActive"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_SUSPEND		MinLogLevel(GetProfileDebugInt(_T("DebugSuspend"),LogVerbose,true)&0xFFFF|LogTimeMs,_DEBUG_ACTIVE)
#endif

/////////////////////////////////////////////////////////////////////////////

static const DWORD	ActiveStateEx_Visible = 0x00010000;
static const DWORD	ActiveStateEx_BalloonVisible = 0x00020000;
static const DWORD	ActiveStateEx_CommandsVisible = 0x00040000;

/////////////////////////////////////////////////////////////////////////////

CLocalCharacter::CLocalCharacter (DaCtlCharacter & pOwner)
:	mOwner (pOwner)
{
	mActiveState = _AtlModule.IsAppActive() ? ActiveState_Inactive : ActiveState_Suspended;
	mLocalStyle = CharacterStyle_SuspendPause|CharacterStyle_SuspendHide;
}

CLocalCharacter::~CLocalCharacter ()
{
}

/////////////////////////////////////////////////////////////////////////////

DWORD CLocalCharacter::GetStyle () const
{
	return CDaCmnCharacter::GetStyle () | (mLocalStyle & LocalCharacterStyle);
}

HRESULT CLocalCharacter::SetStyle (DWORD pRemoveStyle, DWORD pAddStyle)
{
	if	(
			(pAddStyle & CharacterStyle_SuspendPause)
		&&	(!(pAddStyle & CharacterStyle_SuspendStop))
		)
	{
		pRemoveStyle |= CharacterStyle_SuspendStop;
	}

	mLocalStyle &= ~(pRemoveStyle & LocalCharacterStyle);
	mLocalStyle |= (pAddStyle & LocalCharacterStyle);

	if	(mLocalStyle & CharacterStyle_SuspendStop)
	{
		mLocalStyle &= ~CharacterStyle_SuspendPause;
	}
	if	(!(mLocalStyle & (CharacterStyle_SuspendPause|CharacterStyle_SuspendStop)))
	{
		mLocalStyle &= ~CharacterStyle_SuspendHide;
	}

	if	(mLocalStyle & (CharacterStyle_SuspendPause|CharacterStyle_SuspendStop))
	{
		if	(!_AtlModule.IsAppActive ())
		{
			mOwner.SetSuspended (true);
		}
	}
	else
	if	(pRemoveStyle & (CharacterStyle_SuspendPause|CharacterStyle_SuspendStop))
	{
		if	(_AtlModule.IsAppActive ())
		{
			mOwner.SetSuspended (false);
		}
	}

	return CDaCmnCharacter::SetStyle (pRemoveStyle, pAddStyle);
}

/////////////////////////////////////////////////////////////////////////////

short CLocalCharacter::GetActiveState () const
{
	if	(mActiveState & ActiveState_Suspended)
	{
		return LOWORD (mActiveState);
	}
	else
	{
		return CDaCmnCharacter::GetActiveState ();
	}
}

HRESULT CLocalCharacter::SetActiveState (short pActiveState)
{
	HRESULT					lResult = S_FALSE;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CAgentBalloonWnd *		lBalloonWnd;
	CVoiceCommandsWnd *		lCommandsWnd;

#ifdef	_DEBUG_SUSPEND
	if	(
			(pActiveState != GetActiveState())
		&&	(LogIsActive (_DEBUG_SUSPEND))
		)
	{
		LogMessage (_DEBUG_SUSPEND, _T("[%p(%d)] [%p(%d)] [%u] DaCtlCharacter::SetActiveState [%hu] to [%hu] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ActiveClient [%d] InputActive [%d] Listen [%d]"), mOwner.SafeGetOwner(), mOwner.SafeGetOwnerUsed(), &mOwner, max(mOwner.m_dwRef,-1), mOwner.IsSuspended(), GetActiveState(), pActiveState, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
	}
#endif

	if	(pActiveState & ActiveState_Suspended)
	{
		pActiveState &= ~ActiveState_Suspended;

		if	(
				(pActiveState == ActiveState_Inactive)
			||	(pActiveState == ActiveState_Active)
			||	(pActiveState == ActiveState_InputActive)
			)
		{
			pActiveState |= ActiveState_Suspended;

			if	(LOWORD (mActiveState) != pActiveState)
			{
				mActiveState = (DWORD) pActiveState;
				lResult = S_OK;
			}

			if	(mLocalStyle & CharacterStyle_SuspendStop)
			{
				StopListening (true, ListenComplete_CharacterClientDeactivated);
				StopListening (false, ListenComplete_CharacterClientDeactivated);
			}
			else
			if	(
					(mLocalStyle & CharacterStyle_SuspendPause)
				&&	(mListeningState)
				&&	(mListeningState->GetCharID() == mCharID)
				&&	(mListeningState->IsActive ())
				)
			{
				mListeningState->SuspendListening (true);
			}

			if	(mLocalStyle & CharacterStyle_SuspendStop)
			{
				StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_Visibility, AGENTERR_CHARACTERNOTACTIVE);
			}
			else
			if	(
					(mLocalStyle & CharacterStyle_SuspendPause)
				&&	(lCharacterWnd = GetCharacterWnd (false))
				)
			{
				lCharacterWnd->PauseQueue (true);
			}

			if	(
					(mLocalStyle & CharacterStyle_SuspendHide)
				&&	(mListeningAnchor)
				&&	(lCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (false))
				&&	(lCommandsWnd->IsWindow ())
				&&	(lCommandsWnd->IsWindowVisible ())
				)
			{
				mActiveState |= ActiveStateEx_CommandsVisible;
				lCommandsWnd->Hide ();
			}

			if	(
					(mLocalStyle & CharacterStyle_SuspendHide)
				&&	(lCharacterWnd = GetCharacterWnd (false))
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->GetCharID () == mCharID)
				&&	(lBalloonWnd->IsWindow ())
				&&	(lBalloonWnd->IsWindowVisible ())
				)
			{
				mActiveState |= ActiveStateEx_BalloonVisible;
				lBalloonWnd->HideBalloon (true);
			}

			if	(
					(mLocalStyle & CharacterStyle_SuspendHide)
				&&	(IsVisible ())
				)
			{
				mActiveState |= ActiveStateEx_Visible;

				if	(lPopupWnd = GetPopupWnd (false))
				{
					lPopupWnd->ShowWindow (SW_HIDE);
				}
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	else
	{
		if	(mActiveState & ActiveState_Suspended)
		{
			if	(
					(mActiveState & ActiveStateEx_Visible)
				&&	(lPopupWnd = GetPopupWnd (false))
				)
			{
				lPopupWnd->ShowWindow (SW_SHOW);
			}

			if	(
					(mActiveState & ActiveStateEx_BalloonVisible)
				&&	(lCharacterWnd = GetCharacterWnd (false))
				&&	(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
				&&	(lBalloonWnd->IsWindow ())
				)
			{
				lBalloonWnd->ShowBalloonNow ();
			}

			if	(
					(mActiveState & ActiveStateEx_CommandsVisible)
				&&	(mListeningAnchor)
				&&	(lCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (false))
				&&	(lCommandsWnd->IsWindow ())
				)
			{
				lCommandsWnd->Show (false);
			}

			if	(lCharacterWnd = GetCharacterWnd (false))
			{
				lCharacterWnd->PauseQueue (false);
			}

			if	(
					(mListeningState)
				&&	(mListeningState->GetCharID() == mCharID)
				&&	(mListeningState->IsActive ())
				)
			{
				mListeningState->SuspendListening (false);
			}
		}

		lResult = CDaCmnCharacter::SetActiveState (pActiveState);
		mActiveState = (DWORD) CDaCmnCharacter::GetActiveState ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CLocalCharacter::_OnAppActivated (bool pActive)
{
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] OnAppActivated [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ActiveClient [%d] InputActive [%d] Listen [%d]"), mCharID, pActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
	}
#endif
	if	(mLocalStyle & (CharacterStyle_SuspendPause|CharacterStyle_SuspendStop))
	{
		mOwner.SetSuspended (!pActive);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCmnCommands * CLocalCharacter::GetCommands (bool pCreateObject)
{
	CDaCmnCommands *	lRet = NULL;
	DaCtlCommands *		lCommands;

	if	(lCommands = mOwner.GetCommands ())
	{
		lRet = lCommands->mLocalObject;
	}
	return lRet;
}

CDaCmnBalloon * CLocalCharacter::GetBalloon (bool pCreateObject)
{
	CDaCmnBalloon *	lRet = NULL;
	DaCtlBalloon *	lBalloon;

	if	(lBalloon = mOwner.GetBalloon ())
	{
		lRet = lBalloon->mLocalObject;
	}
	return lRet;
}
