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
#include <sphelper.h>
#include "DaCore.h"
#include "AgentListeningWnd.h"
#include "Localize.h"
#include "TextSize.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_POSITION	LogNormal|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CAgentListeningWnd, CToolTipCtrl)

CAgentListeningWnd::CAgentListeningWnd ()
:	mCharID (0),
	mLangID (LANG_USER_DEFAULT)
{
}

CAgentListeningWnd::~CAgentListeningWnd ()
{
	if	(m_hWnd)
	{
		DestroyWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAgentListeningWnd, CToolTipCtrl)
	//{{AFX_MSG_MAP(CAgentListeningWnd)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentListeningWnd::Create (CWnd * pParentWnd)
{
	bool			lRet = false;
	tS <LOGFONT>	lLogFont;

	if	(
			(IsWindow (pParentWnd->GetSafeHwnd ()))
		&&	(CToolTipCtrl::Create (pParentWnd, TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE))
		)
	{
		if	(
				(GetObject (GetStockObject (DEFAULT_GUI_FONT), sizeof(lLogFont), &lLogFont) > 0)
			&&	(mFont.CreateFontIndirect (&lLogFont))
			)
		{
			SetFont (&mFont);
		}

		mToolInfo.uFlags = TTF_CENTERTIP|TTF_TRACK|TTF_ABSOLUTE|TTF_TRANSPARENT;
		mToolInfo.hwnd = m_hWnd;
		mToolInfo.uId = 0;
		mToolInfo.lpszText = _T("");
		SendMessage (TTM_ADDTOOL, 0, (LPARAM)&mToolInfo);

		SetOwner (pParentWnd);
		ModifyStyleEx (0, WS_EX_TOPMOST);
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
				(IsWindow (m_hWnd))
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

	if	(IsWindow (m_hWnd))
	{
		Activate (TRUE);
		PositionTipWnd ();
		SetWindowPos (&wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		lRet = true;
	}
	return lRet;
}

bool CAgentListeningWnd::HideTipWnd ()
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		if	(IsWindowVisible ())
		{
			lRet = true;
		}
		Activate (FALSE);
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
	if	(IsWindow (m_hWnd))
	{
		if	(GetOwner()->IsWindowVisible ())
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

bool CAgentListeningWnd::CalcWinRect (CRect & pWinRect)
{
	CRect						lOldRect (pWinRect);
	CRect						lOwnerRect;
	CRect						lBoundsRect (0,0,0,0);
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(GetMonitorInfo (MonitorFromWindow (NULL, MONITOR_DEFAULTTOPRIMARY), &lMonitorInfo))
	{
		lBoundsRect.CopyRect (&lMonitorInfo.rcWork);
	}
	else
	if	(!SystemParametersInfo (SPI_GETWORKAREA, 0, (void *) &lBoundsRect, 0))
	{
		lBoundsRect.SetRect (0, 0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));
	}
#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Position [%s]"), FormatRect(pWinRect));
#endif

	if	(GetOwner()->IsWindowVisible ())
	{
		GetOwner()->GetWindowRect (&mToolInfo.rect);
		lOwnerRect.CopyRect (&mToolInfo.rect);
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
			(IsWindow (m_hWnd))
		&&	(mCharID > 0)
		)
	{
		if	(GetOwner()->IsWindowVisible ())
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
			(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
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

CString CAgentListeningWnd::GetCharacterCaption (UINT pCaptionStrId) const
{
	CString	lCaption;

	lCaption = CLocalize::LoadString (pCaptionStrId, mLangID);
	lCaption.Replace (_T("%1s"), _T("%1"));
	lCaption = FormatString (lCaption, mCharacterName);
	return lCaption;
}

CString CAgentListeningWnd::GetCommandsCaption () const
{
	CString	lCaption;

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

CString CAgentListeningWnd::GetCommandCaption (LPCTSTR pCommand) const
{
	CString	lCaption;

	lCaption = CLocalize::LoadString (IDS_COMMANDS_VOICE_HEARD, mLangID);
	lCaption.Replace (_T("%1s"), _T("%1"));
	lCaption = FormatString (lCaption, pCommand);
	return lCaption;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentListeningWnd::ShowToolText (LPCTSTR pCaption, LPCTSTR pDetail)
{
	CString	lCaption (pCaption);
	CString	lDetail (pDetail);
	CRect	lMargin;

	if	(mCaption != lCaption)
	{
		mCaption = lCaption;
		GetMargin (&lMargin);
		SetMaxTipWidth (CTextSize (GetFont(), false).MeasureText (mCaption).cx + lMargin.left + lMargin.right);
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

void CAgentListeningWnd::OnWindowPosChanging (WINDOWPOS *lpwndpos)
{
	CToolTipCtrl::OnWindowPosChanging (lpwndpos);

#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Listening OnWindowPosChanging [%s]"), WindowPosStr(*lpwndpos));
#endif
	if	((lpwndpos->flags & (SWP_NOMOVE|SWP_NOSIZE)) == 0)
	{
		CRect	lWinRect (lpwndpos->x, lpwndpos->y, lpwndpos->x+lpwndpos->cx, lpwndpos->y+lpwndpos->cy);

		if	(CalcWinRect (lWinRect))
		{
			lpwndpos->x = lWinRect.left;
			lpwndpos->y = lWinRect.top;
			lpwndpos->cx = lWinRect.Width();
			lpwndpos->cy = lWinRect.Height();
#ifdef	_DEBUG_POSITION
			LogMessage (_DEBUG_POSITION, _T("          OnWindowPosChanging [%s]"), WindowPosStr(*lpwndpos));
#endif
		}
	}
}

void CAgentListeningWnd::OnWindowPosChanged (WINDOWPOS *lpwndpos)
{
	CToolTipCtrl::OnWindowPosChanged (lpwndpos);
#ifdef	_DEBUG_POSITION
	LogMessage (_DEBUG_POSITION, _T("Listening OnWindowPosChanged  [%s]"), WindowPosStr(*lpwndpos));
#endif
}

/////////////////////////////////////////////////////////////////////////////

_MFC_NCHITTEST_RESULT CAgentListeningWnd::OnNcHitTest (CPoint point)
{
	return HTTRANSPARENT;
}
