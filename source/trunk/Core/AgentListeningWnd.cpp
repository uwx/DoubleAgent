/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include <sphelper.h>
#include "DaCore.h"
#include "AgentListeningWnd.h"
#include "AgentCharacterWnd.h"
#include "Localize.h"
#include "TextSize.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_POSITION	LogNormal|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentListeningWnd)

CAgentListeningWnd::CAgentListeningWnd ()
:	mCharID (0),
	mLangID (LANG_USER_DEFAULT),
	mOwnerWnd (NULL)
{
}

CAgentListeningWnd::~CAgentListeningWnd ()
{
	if	(IsWindow ())
	{
		DestroyWindow ();
	}
}

CAgentListeningWnd* CAgentListeningWnd::CreateInstance ()
{
	return new CAgentListeningWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentListeningWnd::Create (CWindow* pOwnerWnd, DWORD pExStyle)
{
	bool			lRet = false;
	tS <LOGFONT>	lLogFont;

	if	(
			(pOwnerWnd)
		&&	(pOwnerWnd->IsWindow())
		&&	(SubclassWindow (::CreateWindowEx (pExStyle, TOOLTIPS_CLASS, _T(""), WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX, 0,0,0,0, pOwnerWnd->m_hWnd, 0, _AtlBaseModule.GetModuleInstance(), NULL)))
		)
	{
		mOwnerWnd = pOwnerWnd;

		if	(
				(GetObject (GetStockObject (DEFAULT_GUI_FONT), sizeof(lLogFont), &lLogFont) > 0)
			&&	(mFont = CreateFontIndirect (&lLogFont))
			)
		{
			SetFont (mFont);
		}

		mToolInfo.uFlags = TTF_CENTERTIP|TTF_TRACK|TTF_ABSOLUTE|TTF_TRANSPARENT;
		mToolInfo.hwnd = pOwnerWnd->m_hWnd;
		mToolInfo.uId = 0;
		mToolInfo.lpszText = _T("");
		SendMessage (TTM_ADDTOOL, 0, (LPARAM)&mToolInfo);

		ModifyStyleEx (WS_EX_TOPMOST, pExStyle);
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentListeningWnd::Attach (long pCharID, LPCTSTR pCharacterName)
{
	bool	lRet = false;

	if	(pCharacterName)
	{
		if	(mCharID != pCharID)
		{
			mCharID = pCharID;
			lRet = true;
		}
		mCharacterName = pCharacterName;
	}
	else
	{
		if	(mCharID == pCharID)
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CAgentListeningWnd::Detach (long pCharID)
{
	bool	lRet = false;

	if	(mCharID == pCharID)
	{
		mCharID = 0;

		if	(
				(IsWindow ())
			&&	(IsWindowVisible ())
			)
		{
			HideTipWnd ();
		}
		lRet = true;
	}
	return lRet;
}

bool CAgentListeningWnd::SetLangID (LANGID pLangID)
{
	if	(mLangID != pLangID)
	{
		mLangID = pLangID;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentListeningWnd::ShowTipWnd ()
{
	bool	lRet = false;

	if	(IsWindow ())
	{
		SendMessage (TTM_ACTIVATE, TRUE);
		PositionTipWnd ();
		if	(GetExStyle() & WS_EX_TOPMOST)
		{
			SetWindowPos (HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);
		}
		lRet = true;
	}
	return lRet;
}

bool CAgentListeningWnd::HideTipWnd ()
{
	bool	lRet = false;

	if	(IsWindow ())
	{
		if	(IsWindowVisible ())
		{
			lRet = true;
		}
		SendMessage (TTM_ACTIVATE, FALSE);
		mCaption.Empty ();
		mDetail.Empty ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentListeningWnd::PositionTipWnd ()
{
	CRect	lWinRect;

	GetWindowRect (&lWinRect);
	CalcWinRect (lWinRect);

	SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (lWinRect.CenterPoint().x, lWinRect.CenterPoint().y));
	SendMessage (TTM_TRACKACTIVATE, TRUE, (LPARAM)&mToolInfo);
}

void CAgentListeningWnd::RefreshTipWnd ()
{
	if	(IsWindow ())
	{
		if	(::IsWindowVisible (GetWindow (GW_OWNER)))
		{
			ShowToolText (mCaption, mDetail);
		}
		else
		{
			ShowToolText (mCaption, CLocalize::LoadString (IDS_COMMANDS_SHOW_CHAR_PROMPT, mLangID));
		}
		PositionTipWnd ();
	}
}

bool CAgentListeningWnd::CalcWinRect (CRect& pWinRect)
{
	CRect						lOldRect (pWinRect);
	CAgentCharacterWnd*		lOwner;
	CRect						lOwnerRect;
	CRect						lBoundsRect (0,0,0,0);
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(GetMonitorInfo (MonitorFromWindow (NULL, MONITOR_DEFAULTTOPRIMARY), &lMonitorInfo))
	{
		lBoundsRect.CopyRect (&lMonitorInfo.rcWork);
	}
	else
	if	(!SystemParametersInfo (SPI_GETWORKAREA, 0, (void*) &lBoundsRect, 0))
	{
		lBoundsRect.SetRect (0, 0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));
	}
#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Position [%s]"), FormatRect(pWinRect));
#endif

	if	(::IsWindowVisible (GetWindow (GW_OWNER)))
	{
		mOwnerWnd->GetWindowRect (&lOwnerRect);
		if	(
				(mOwnerWnd->GetStyle () & WS_CHILD)
			&&	(lOwner = dynamic_cast <CAgentCharacterWnd*> ((CAgentWnd*)mOwnerWnd))
			)
		{
			CRect	lVideoRect = lOwner->GetVideoRect ();

			lVideoRect.OffsetRect (lOwnerRect.left, lOwnerRect.top);
			lOwnerRect = lVideoRect;
		}
		::CopyRect (&mToolInfo.rect, &lOwnerRect);

#ifdef	_DEBUG_POSITION
		LogMessage (_DEBUG_POSITION, _T("  Below  [%s]"), FormatRect(lOwnerRect));
		LogMessage (_DEBUG_POSITION, _T("  Within [%s]"), FormatRect(lBoundsRect));
#endif
		pWinRect.OffsetRect (lOwnerRect.CenterPoint().x - pWinRect.CenterPoint().x, lOwnerRect.bottom - pWinRect.top);
		if	(pWinRect.bottom > lBoundsRect.bottom)
		{
			if	(pWinRect.CenterPoint().x > lBoundsRect.CenterPoint().x)
			{
				pWinRect.OffsetRect (lOwnerRect.left - pWinRect.right, lOwnerRect.CenterPoint().y - pWinRect.CenterPoint().y);
			}
			else
			{
				pWinRect.OffsetRect (lOwnerRect.right - pWinRect.left, lOwnerRect.CenterPoint().y - pWinRect.CenterPoint().y);
			}
		}
		else
		{
			if	(pWinRect.left < lBoundsRect.left)
			{
				pWinRect.OffsetRect (lBoundsRect.left - pWinRect.left, 0);
			}
			else
			if	(pWinRect.right > lBoundsRect.right)
			{
				pWinRect.OffsetRect (lBoundsRect.right - pWinRect.right, 0);
			}
		}
	}
	else
	{
#ifdef	_DEBUG_POSITION
		LogMessage (_DEBUG_POSITION, _T("  Within [%s]"), FormatRect(lBoundsRect));
#endif
		pWinRect.OffsetRect (lBoundsRect.right - pWinRect.right, lBoundsRect.bottom - pWinRect.bottom);
	}

#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("  At     [%s]"), FormatRect(pWinRect));
#endif
	return !lOldRect.EqualRect (pWinRect);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentListeningWnd::ShowCharacterListening (LPCTSTR pCommandsCaption)
{
	if	(pCommandsCaption)
	{
		mCommandsCaption = pCommandsCaption;
	}
	if	(
			(IsWindow ())
		&&	(mCharID > 0)
		)
	{
		if	(::IsWindowVisible (GetWindow (GW_OWNER)))
		{
			ShowToolText (GetCharacterCaption (IDS_COMMANDS_LISTENING_PROMPT), GetCommandsCaption ());
		}
		else
		{
			ShowToolText (GetCharacterCaption (IDS_COMMANDS_LISTENING_PROMPT), CLocalize::LoadString (IDS_COMMANDS_SHOW_CHAR_PROMPT, mLangID));
		}
		PositionTipWnd ();
	}
}

void CAgentListeningWnd::ShowCharacterNotListening (LPCTSTR pCommandsCaption, LPCTSTR pReason)
{
	if	(pCommandsCaption)
	{
		mCommandsCaption = pCommandsCaption;
	}
	if	(
			(IsWindow ())
		&&	(mCharID > 0)
		)
	{
		if	(pReason)
		{
			ShowToolText (GetCharacterCaption (IDS_COMMANDS_LISTENING_NOT_PROMPT), pReason);
		}
		else
		{
			ShowToolText (GetCharacterCaption (IDS_COMMANDS_LISTENING_NOT_PROMPT), GetCommandsCaption ());
		}
		PositionTipWnd ();
	}
}

void CAgentListeningWnd::ShowCharacterHeard (LPCTSTR pCommand)
{
	if	(
			(IsWindow ())
		&&	(mCharID > 0)
		)
	{
		if	(pCommand)
		{
			ShowToolText (GetCharacterCaption (IDS_COMMANDS_LISTENING_PROMPT), GetCommandCaption (pCommand));
		}
		else
		{
			ShowToolText (GetCharacterCaption (IDS_COMMANDS_LISTENING_PROMPT), CLocalize::LoadString (IDS_COMMANDS_HEARD_GARBAGE, mLangID));
		}
		PositionTipWnd ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentListeningWnd::GetCharacterCaption (UINT pCaptionStrId) const
{
	CAtlString	lCaption;

	lCaption = CLocalize::LoadString (pCaptionStrId, mLangID);
	lCaption.Replace (_T("%1s"), _T("%1"));
	lCaption = FormatString (lCaption, mCharacterName);
	return lCaption;
}

CAtlString CAgentListeningWnd::GetCommandsCaption () const
{
	CAtlString	lCaption;

	if	(mCommandsCaption.IsEmpty ())
	{
		lCaption = CLocalize::LoadString (IDS_FOR_COMMANDS, mLangID);
	}
	else
	{
		lCaption = CLocalize::LoadString (IDS_FOR_COMMANDS_EX, mLangID);
		lCaption.Replace (_T("%1s"), _T("%1"));
		lCaption = FormatString (lCaption, mCommandsCaption);
	}
	return lCaption;
}

CAtlString CAgentListeningWnd::GetCommandCaption (LPCTSTR pCommand) const
{
	CAtlString	lCaption;

	lCaption = CLocalize::LoadString (IDS_COMMANDS_VOICE_HEARD, mLangID);
	lCaption.Replace (_T("%1s"), _T("%1"));
	lCaption = FormatString (lCaption, pCommand);
	return lCaption;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentListeningWnd::ShowToolText (LPCTSTR pCaption, LPCTSTR pDetail)
{
	CAtlString	lCaption (pCaption);
	CAtlString	lDetail (pDetail);
	CRect		lMargin;

	if	(mCaption != lCaption)
	{
		mCaption = lCaption;
		SendMessage (TTM_GETMARGIN, 0, (LPARAM)&lMargin);
		SendMessage (TTM_SETMAXTIPWIDTH, 0, CTextSize (GetFont(), false).MeasureText (mCaption).cx + lMargin.left + lMargin.right);
#ifdef	_DEBUG_POSITION
		LogMessage (_DEBUG_POSITION, _T("Listening TTM_SETTITLE"));
#endif
		SendMessage (TTM_SETTITLE, (WPARAM)TTI_NONE, (LPARAM)(LPCTSTR)mCaption);
	}
	if	(mDetail != lDetail)
	{
		mDetail = lDetail;
		mToolInfo.lpszText = (LPTSTR)(LPCTSTR)mDetail;
#ifdef	_DEBUG_POSITION
		LogMessage (_DEBUG_POSITION, _T("Listening TTM_UPDATETIPTEXT"));
#endif
		SendMessage (TTM_UPDATETIPTEXT, 0, (LPARAM)&mToolInfo);
	}
#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Listening TTM_UPDATE"));
#endif
	SendMessage (TTM_UPDATE);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentListeningWnd::OnWindowPosChanging (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT		lResult = DefWindowProc ();
	LPWINDOWPOS	lWindowPos = (LPWINDOWPOS) lParam;

#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Listening OnWindowPosChanging [%s]"), WindowPosStr(*lWindowPos));
#endif
	if	((lWindowPos->flags & (SWP_NOMOVE|SWP_NOSIZE)) == 0)
	{
		CRect	lWinRect (lWindowPos->x, lWindowPos->y, lWindowPos->x+lWindowPos->cx, lWindowPos->y+lWindowPos->cy);

		if	(CalcWinRect (lWinRect))
		{
			lWindowPos->x = lWinRect.left;
			lWindowPos->y = lWinRect.top;
			lWindowPos->cx = lWinRect.Width();
			lWindowPos->cy = lWinRect.Height();
#ifdef	_DEBUG_POSITION
			LogMessage (_DEBUG_POSITION, _T("          OnWindowPosChanging [%s]"), WindowPosStr(*lWindowPos));
#endif
		}
	}
	return lResult;
}

LRESULT CAgentListeningWnd::OnWindowPosChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT		lResult = DefWindowProc ();
	LPWINDOWPOS	lWindowPos = (LPWINDOWPOS) lParam;
#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Listening OnWindowPosChanged  [%s]"), WindowPosStr(*lWindowPos));
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentListeningWnd::OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return HTTRANSPARENT;
}
