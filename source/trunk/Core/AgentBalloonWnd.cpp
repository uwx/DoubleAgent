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
#include "..\Server\DaAgentNotify.h"
#include "AgentBalloonWnd.h"
#include "AgentBalloonShape.h"
#include "AgentPopupWnd.h"
#include "BitmapAlpha.h"
#include "Sapi5Voice.h"
#include "StringArrayEx.h"
#include "NotifyLock.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#include "BitmapDebugger.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_LAYOUT		LogNormal|LogHighVolume
//#define	_DEBUG_RTL			LogNormal|LogHighVolume
//#define	_DEBUG_DRAW			LogNormal|LogHighVolume
#define	_DEBUG_SHOW_HIDE		(GetProfileDebugInt(_T("DebugBalloonShow"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_AUTO_HIDE		(GetProfileDebugInt(_T("DebugBalloonHide"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_AUTO_SIZE		(GetProfileDebugInt(_T("DebugBalloonSize"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_AUTO_PACE		(GetProfileDebugInt(_T("DebugBalloonPace"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_AUTO_SCROLL		(GetProfileDebugInt(_T("DebugBalloonScroll"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugBalloonSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_OPTIONS			(GetProfileDebugInt(_T("DebugBalloonOptions"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_FONT				(GetProfileDebugInt(_T("DebugBalloonFont"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Balloon"),LogDetails,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

const USHORT	CAgentBalloonWnd::mMinLines = 1;
const USHORT	CAgentBalloonWnd::mMaxLines = 128;
const USHORT	CAgentBalloonWnd::mDefLines = 2;

const USHORT	CAgentBalloonWnd::mMinPerLine = 8;
const USHORT	CAgentBalloonWnd::mMaxPerLine = 255;
const USHORT	CAgentBalloonWnd::mDefPerLine = 32;

static const int	sSpeechPacingLookAhead = 2;

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CAgentBalloonWnd, CToolTipCtrl)

CAgentBalloonWnd::CAgentBalloonWnd ()
:	mStyle (BALLOON_STYLE_AUTOPACE|BALLOON_STYLE_AUTOHIDE),
	mLines (mDefLines),
	mPerLine (mDefPerLine),
	mClipPartialLines (true),
	mBkColor (GetSysColor (COLOR_INFOBK)),
	mFgColor (GetSysColor (COLOR_INFOTEXT)),
	mBrColor (GetSysColor (COLOR_INFOTEXT)),
	mCharID (0),
	mLangID (GetUserDefaultUILanguage ()),
	mAutoPaceDisabled (false),
	mAutoPaceTime (300),
	mAutoPaceTimer (0),
	mAutoHideTimer (0),
	mAutoHideTime (0),
	mAutoScrollTimer (0),
	mPacingSpeech (false),
	mPacingWord (false),
	mApplyingLayout (false),
	mApplyingRegion (false),
	mInNotify (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::CAgentBalloonWnd (%d)"), this, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

CAgentBalloonWnd::~CAgentBalloonWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::~CAgentBalloonWnd (%d)"), this, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Detach (-1, NULL);
}

void CAgentBalloonWnd::OnFinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::OnFinalRelease [%u]"), this, IsInNotify());
	}
#endif
	if	(IsInNotify() == 0)
	{
		CCmdTarget::OnFinalRelease ();
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAgentBalloonWnd, CToolTipCtrl)
	//{{AFX_MSG_MAP(CAgentBalloonWnd)
	ON_NOTIFY_REFLECT_EX(TTN_SHOW, OnShow)
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_PRINT, OnPrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::SetOptions (const CAgentFileBalloon & pFileBalloon, IDaSvrBalloon * pCharBalloon, LANGID pLangID)
{
	bool			lRet = false;
	tS <LOGFONT>	lLogFont;
	tS <LOGFONT>	lOldFont;
	USHORT			lOldLines = mLines;
	USHORT			lOldPerLine = mPerLine;
	COLORREF		lOldBkColor = mBkColor;
	COLORREF		lOldFgColor = mFgColor;
	COLORREF		lOldBrColor= mBrColor;

	mLines = (USHORT)pFileBalloon.mLines;
	mPerLine = (USHORT)pFileBalloon.mPerLine;
	mBkColor = pFileBalloon.mBkColor;
	mFgColor = pFileBalloon.mFgColor;
	mBrColor = pFileBalloon.mBrColor;

	if	(pCharBalloon)
	{
		long	lLongVal;

		if	(pCharBalloon->GetStyle (&lLongVal) == S_OK)
		{
			mStyle = lLongVal;
		}
		if	(pCharBalloon->GetNumLines (&lLongVal) == S_OK)
		{
			mLines = (USHORT)lLongVal;
		}
		if	(pCharBalloon->GetNumCharsPerLine (&lLongVal) == S_OK)
		{
			mPerLine = (USHORT)lLongVal;
		}
		if	(pCharBalloon->GetBackColor (&lLongVal) == S_OK)
		{
			mBkColor = lLongVal;
		}
		if	(pCharBalloon->GetForeColor (&lLongVal) == S_OK)
		{
			mFgColor = lLongVal;
		}
		if	(pCharBalloon->GetBorderColor (&lLongVal) == S_OK)
		{
			mBrColor = lLongVal;
		}
	}

	if	(
			(mLines != lOldLines)
		||	(mPerLine != lOldPerLine)
		||	(mBkColor != lOldBkColor)
		||	(mFgColor != lOldFgColor)
		||	(mBrColor != lOldBrColor)
		)
	{
		lRet = true;
	}
#ifdef	_DEBUG_OPTIONS
	if	(LogIsActive (_DEBUG_OPTIONS))
	{
		LogMessage (_DEBUG_OPTIONS, _T("[%p] CAgentBalloonWnd Style [%8.8X] AutoSize [%u] AutoPace [%u] AutoHide [%u]"), this, mStyle, IsAutoSize(), IsAutoPace(), IsAutoHide());
		LogMessage (_DEBUG_OPTIONS, _T("[%p]                  Lines [%hu] PerLine [%hu] BkColor [%8.8X] FgColor [%8.8X] BrColor [%8.8X]"), this, mLines, mPerLine, mBkColor, mFgColor, mBrColor);
	}
#endif

	CopyBalloonFont (pFileBalloon, lLogFont);
	CopyBalloonFont (pCharBalloon, lLogFont);
	if	(pLangID)
	{
		SetFontLangID (lLogFont, pLangID);
	}

	if	(
			(mFont.GetSafeHandle())
		&&	(mFont.GetLogFont (&lOldFont))
		&&	(memcmp (&lOldFont, &lLogFont, sizeof(LOGFONT)) == 0)
		)
	{
#ifdef	_DEBUG_FONT
		if	(LogIsActive (_DEBUG_FONT))
		{
			LogMessage (_DEBUG_FONT, _T("[%p] CAgentBalloonWnd SameFont [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), this, lLogFont.lfFaceName, lLogFont.lfHeight, lLogFont.lfWidth, lLogFont.lfWeight, lLogFont.lfItalic, lLogFont.lfUnderline, lLogFont.lfStrikeOut, lLogFont.lfCharSet, lLogFont.lfPitchAndFamily, lLogFont.lfQuality);
		}
#endif
	}
	else
	{
		mFont.DeleteObject ();
#ifdef	_DEBUG_FONT
		if	(LogIsActive (_DEBUG_FONT))
		{
			LogMessage (_DEBUG_FONT, _T("[%p] CAgentBalloonWnd Font [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), this, lLogFont.lfFaceName, lLogFont.lfHeight, lLogFont.lfWidth, lLogFont.lfWeight, lLogFont.lfItalic, lLogFont.lfUnderline, lLogFont.lfStrikeOut, lLogFont.lfCharSet, lLogFont.lfPitchAndFamily, lLogFont.lfQuality);
			if	(GetActualFont (lLogFont, lLogFont))
			{
				LogMessage (_DEBUG_FONT, _T("[%p]           Actual Font [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), this, lLogFont.lfFaceName, lLogFont.lfHeight, lLogFont.lfWidth, lLogFont.lfWeight, lLogFont.lfItalic, lLogFont.lfUnderline, lLogFont.lfStrikeOut, lLogFont.lfCharSet, lLogFont.lfPitchAndFamily, lLogFont.lfQuality);
			}
		}
		else
#endif
		{
			GetActualFont (lLogFont, lLogFont);
		}
		if	(lLogFont.lfHeight)
		{
			mFont.CreateFontIndirect (&lLogFont);
		}
		lRet = true;
	}

	if	(
			(lRet)
		&&	(IsWindow (m_hWnd))
		)
	{
		if	(mStyle & BALLOON_STYLE_BALLOON_ON)
		{
			Invalidate ();
		}
		else
		{
			HideBalloon (true);
		}
		SetTipBkColor (mBkColor);
		SetTipTextColor (mFgColor);
		if	(mFont.GetSafeHandle())
		{
			SetFont (&mFont);
		}
		else
		{
			SetFont (NULL);
		}
		if	(IsWindowVisible ())
		{
			MoveBalloon ();
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::CopyBalloonFont (const CAgentFileBalloon & pFileBalloon, LOGFONT & pFont)
{
	if	(
			(pFileBalloon.mFontName.Ptr())
		&&	(pFileBalloon.mFontHeight != 0)
		)
	{
		_tcscpy (pFont.lfFaceName, CString ((BSTR)pFileBalloon.mFontName));
		pFont.lfHeight = pFileBalloon.mFontHeight;
		pFont.lfWeight = (pFileBalloon.mFontWeight > 0) ? pFileBalloon.mFontWeight : FW_NORMAL;
		pFont.lfItalic = (pFileBalloon.mFontItalic != 0);
		pFont.lfUnderline = (pFileBalloon.mFontUnderline != 0);
		pFont.lfStrikeOut = (pFileBalloon.mFontStrikethru != 0);
		pFont.lfCharSet = pFileBalloon.mFontCharset;
		pFont.lfQuality = CLEARTYPE_QUALITY;

		return true;
	}
	return false;
}

bool CAgentBalloonWnd::CopyBalloonFont (const LOGFONT & pFont, CAgentFileBalloon & pFileBalloon)
{
	if	(
			(pFont.lfFaceName [0])
		&&	(pFont.lfHeight != 0)
		)
	{
		pFileBalloon.mFontName = CString (pFont.lfFaceName).AllocSysString ();
		pFileBalloon.mFontHeight = pFont.lfHeight;
		pFileBalloon.mFontWeight = (USHORT)pFont.lfWeight;
		pFileBalloon.mFontItalic = (pFont.lfItalic != 0);
		pFileBalloon.mFontUnderline = (pFont.lfUnderline != 0);
		pFileBalloon.mFontStrikethru = (pFont.lfStrikeOut != 0);
		pFileBalloon.mFontCharset = pFont.lfCharSet;

		return true;
	}
	return false;
}

bool CAgentBalloonWnd::CopyBalloonFont (IDaSvrBalloon * pCharBalloon, LOGFONT & pFont)
{
	if	(pCharBalloon)
	{
		long		lLongVal;
		short		lShortVal;
		tBstrPtr	lBstrVal;

		if	(
				(pCharBalloon->GetFontName (lBstrVal.Free()) == S_OK)
			&&	(lBstrVal.Ptr())
			)
		{
			_tcscpy (pFont.lfFaceName, CString ((BSTR)lBstrVal));
		}
		if	(
				(pCharBalloon->GetFontSize (&lLongVal) == S_OK)
			&&	(lLongVal != 0)
			)
		{
			pFont.lfHeight = lLongVal;
		}

		if	(pCharBalloon->GetFontBold (&lLongVal) == S_OK)
		{
			pFont.lfWeight = lLongVal ? FW_BOLD : FW_NORMAL;
		}
		if	(pCharBalloon->GetFontItalic (&lLongVal) == S_OK)
		{
			pFont.lfItalic = (lLongVal != 0);
		}
		if	(pCharBalloon->GetFontUnderline (&lLongVal) == S_OK)
		{
			pFont.lfUnderline = (lLongVal != 0);
		}
		if	(pCharBalloon->GetFontStrikethru (&lLongVal) == S_OK)
		{
			pFont.lfStrikeOut = (lLongVal != 0);
		}
		if	(pCharBalloon->GetFontCharSet (&lShortVal) == S_OK)
		{
			pFont.lfCharSet = LOBYTE(lShortVal);
		}
		pFont.lfQuality = CLEARTYPE_QUALITY;

		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::SetFontLangID (LOGFONT & pFont, LANGID pLangID)
{
	tS <CHARSETINFO>	lCharSet;

	if	(TranslateCharsetInfo ((LPDWORD)MAKELCID (pLangID, SORT_DEFAULT), &lCharSet, TCI_SRCLOCALE))
	{
#ifdef	_DEBUG_FONT
		if	(LogIsActive (_DEBUG_FONT))
		{
			LogMessage (_DEBUG_FONT, _T("  LangID [%4.4hX] CharSet [%u] CodePage [%u] Usb [%8.8X%8.8X%8.8X%8.8X] Csb [%8.8X%8.8X]"), pLangID, lCharSet.ciCharset, lCharSet.ciACP, lCharSet.fs.fsUsb[0],lCharSet.fs.fsUsb[1],lCharSet.fs.fsUsb[2],lCharSet.fs.fsUsb[3], lCharSet.fs.fsCsb[0],lCharSet.fs.fsCsb[1]);
		}
#endif
		pFont.lfCharSet = lCharSet.ciCharset;
		return true;
	}
	return false;
}

bool CAgentBalloonWnd::GetActualFont (const LOGFONT & pFont, LOGFONT & pActualFont, bool pUpdateSize, bool pUpdateStyle)
{
	CDC		lDC;
	CFont	lFont;

	memcpy (&pActualFont, &pFont, sizeof(LOGFONT));

	if	(
			(_tcsicmp (pActualFont.lfFaceName, _T("MS Sans Serif")) == 0)
		||	(_tcsicmp (pActualFont.lfFaceName, _T("MS Shell Dlg")) == 0)
		)
	{
		_tcscpy (pActualFont.lfFaceName, _T("MS Shell Dlg 2"));
	}

	if	(
			(lFont.CreateFontIndirect (&pActualFont))
		&&	(lDC.CreateCompatibleDC (NULL))
		)
	{
		tS <TEXTMETRIC> lMetric;
		TCHAR			lFontFace [MAX_PATH];

		lDC.SelectObject (&lFont);
		if	(::GetTextFace (lDC, sizeof(lFontFace)/sizeof(TCHAR), lFontFace))
		{
			_tcsncpy (pActualFont.lfFaceName, lFontFace, sizeof(pActualFont.lfFaceName)/sizeof(pActualFont.lfFaceName[0]));
		}
		if	(::GetTextMetrics (lDC, &lMetric))
		{
			pActualFont.lfCharSet = lMetric.tmCharSet;
			if	(pUpdateSize)
			{
				if	(pFont.lfHeight < 0)
				{
					pActualFont.lfHeight = -lMetric.tmAscent;
				}
				else
				{
					pActualFont.lfHeight = lMetric.tmHeight;
				}
			}
			if	(pUpdateStyle)
			{
				pActualFont.lfWeight = lMetric.tmWeight;
				pActualFont.lfItalic = lMetric.tmItalic;
				pActualFont.lfUnderline = lMetric.tmUnderlined;
				pActualFont.lfStrikeOut = lMetric.tmStruckOut;
			}
		}

		lDC.DeleteDC();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::Create (CWnd * pParentWnd)
{
	bool	lRet = false;

	if	(
			(IsWindow (pParentWnd->GetSafeHwnd ()))
		&&	(CToolTipCtrl::Create (pParentWnd, TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE))
		)
	{
		ModifyStyle (WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx (0, WS_EX_TOPMOST);

		mToolInfo.uFlags = TTF_ABSOLUTE|TTF_TRACK|TTF_TRANSPARENT;
		mToolInfo.hwnd = m_hWnd;
		if	(mText.GetFullText().IsEmpty ())
		{
			mToolInfo.lpszText = _T(" ");
		}
		else
		{
			mToolInfo.lpszText = (LPTSTR)(LPCTSTR)mText.GetFullText();
		}
		SendMessage (TTM_ADDTOOL, 0, (LPARAM)&mToolInfo);

		SetOwner (pParentWnd);
		SetTipBkColor (mBkColor);
		SetTipTextColor (mFgColor);
		if	(mFont.GetSafeHandle())
		{
			SetFont (&mFont);
		}
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd [%p] Parent [%p] [%p] Owner [%p] [%p]"), this, m_hWnd, pParentWnd->GetSafeHwnd(), ::GetParent(m_hWnd), GetOwner()->GetSafeHwnd(), ::GetWindow(m_hWnd, GW_OWNER));
		}
#endif
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::Attach (long pCharID, IDaNotify * pNotify, bool pSetActiveCharID)
{
	bool	lRet = false;

	if	(
			(pNotify)
		&&	(mNotify.AddUnique (pNotify) >= 0)
		)
	{
		lRet = true;
	}
	if	(
			(pSetActiveCharID)
		&&	(mCharID != pCharID)
		)
	{
		mCharID = pCharID;
		mText.SetSapiVersion (0); // Reset to default
		lRet = true;
	}
	return lRet;
}

bool CAgentBalloonWnd::Detach (long pCharID, IDaNotify * pNotify)
{
	bool	lRet = false;

	if	(
			(pNotify)
		&&	(mNotify.Remove (pNotify) >= 0)
		)
	{
		lRet = true;
	}

	if	(
			(pCharID < 0)
		&&	(!pNotify)
		)
	{
		for	(INT_PTR lNdx = mNotify.GetUpperBound(); lNdx >= 0; lNdx--)
		{
			if	(Detach (-1, mNotify [lNdx]))
			{
				lRet = true;
			}
		}
		mNotify.RemoveAll ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::PreNotify ()
{
	if	(
			(this)
		&&	(m_dwRef > 0)
		&&	(mNotify.GetSize() > 0)
		)
	{
		mInNotify++;
		return true;
	}
	return false;
}

bool CAgentBalloonWnd::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		if	(
				(mInNotify == 0)
			&&	(m_dwRef == 0)
			)
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive (_LOG_INSTANCE))
			{
				LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd PostNotify -> OnFinalRelease"), this);
			}
#endif
			OnFinalRelease ();
			return false;
		}
		return true;
	}
	return false;
}

UINT CAgentBalloonWnd::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentBalloonWnd::GetCharID () const
{
	return mCharID;
}

LANGID CAgentBalloonWnd::GetLangID () const
{
	return mLangID;
}

bool CAgentBalloonWnd::IsSpeechShape () const
{
	if	(mShape)
	{
		return mShape->IsKindOf (RUNTIME_CLASS (CAgentBalloonSpeak)) ? true : false;
	}
	return false;
}

bool CAgentBalloonWnd::IsThoughtShape () const
{
	if	(mShape)
	{
		return mShape->IsKindOf (RUNTIME_CLASS (CAgentBalloonThink)) ? true : false;
	}
	return false;
}

bool CAgentBalloonWnd::IsBusy (bool pForIdle) const
{
	if	(
			(IsWindow (m_hWnd))
		&&	(GetStyle () & WS_VISIBLE)
		&&	(
				(mPacingSpeech)
			||	(mAutoPaceTimer != 0)
			||	(mAutoScrollTimer != 0)
			||	(
					(pForIdle)
				&&	(mAutoHideTimer != 0)
				)
			)
		)
	{
		return true;
	}
	return false;
}

bool CAgentBalloonWnd::IsDrawingLayered () const
{
//	if	(
//			(m_hWnd)
//		&&	(GetExStyle () & WS_EX_LAYERED)
//		)
//	{
//		return true;
//	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::IsAutoSize () const
{
	return ((mStyle & BALLOON_STYLE_SIZETOTEXT) != 0);
}

bool CAgentBalloonWnd::IsAutoPace () const
{
	return ((mStyle & BALLOON_STYLE_AUTOPACE) != 0) && (!mAutoPaceDisabled);
}

bool CAgentBalloonWnd::IsAutoHide () const
{
	return ((mStyle & BALLOON_STYLE_AUTOHIDE) != 0);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::ShowBalloonSpeech (LPCTSTR pText, UINT pSapiVersion, bool pNoAutoPace)
{
	CAgentText	lText (CString(pText).TrimLeft().TrimRight(), pSapiVersion);

	return ShowBalloonSpeech (lText, pNoAutoPace);
}

bool CAgentBalloonWnd::ShowBalloonSpeech (const CAgentText & pText, bool pNoAutoPace)
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		bool	lWasVisible;
		bool	lTextChanged;

		lWasVisible = ShowingBalloon ();
		lTextChanged = ShowBalloonText (pText, pText.GetSapiVersion(), pNoAutoPace);
		if	(lRet = ShowBalloon (true, lTextChanged))
		{
			ShowedBalloon (lWasVisible);
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::ShowBalloonThought (LPCTSTR pText, UINT pSapiVersion, bool pNoAutoPace)
{
	CAgentText	lText (CString(pText).TrimLeft().TrimRight(), pSapiVersion);

	return ShowBalloonThought (lText, pNoAutoPace);
}

bool CAgentBalloonWnd::ShowBalloonThought (const CAgentText & pText, bool pNoAutoPace)
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		bool	lWasVisible;
		bool	lTextChanged;

		lWasVisible = ShowingBalloon ();
		lTextChanged = ShowBalloonText (pText, 0, pNoAutoPace);
		if	(lRet = ShowBalloon (false, lTextChanged))
		{
			ShowedBalloon (lWasVisible);
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::ShowBalloonNow ()
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		bool	lWasVisible;

		lWasVisible = ShowingBalloon ();
		if	(lRet = ShowBalloon (IsSpeechShape(), false))
		{
			ShowedBalloon (lWasVisible);
		}
	}
	return lRet;
}

bool CAgentBalloonWnd::ShowBalloonAuto ()
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		if	(IsWindowVisible ())
		{
			if	(!StartAutoPace ())
			{
				StartAutoHide ();
			}
			lRet = true;
		}
		else
		{
			StopAutoPace ();
			StopAutoScroll ();
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::ShowingBalloon ()
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		if	(IsWindowVisible ())
		{
			lRet = true;
		}
		StopAutoHide ();
	}
	return lRet;
}

bool CAgentBalloonWnd::ShowBalloonText (const CAgentText & pText, UINT pForSpeech, bool pNoAutoPace)
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		CString	lFullText (mText.GetFullText ());
		CString	lSpeechText (mText.GetSpeechText ());
		UINT	lSapiVersion = mText.GetSapiVersion ();

		mAutoPaceDisabled = pNoAutoPace;
		if	(pForSpeech)
		{
			mText.SetSapiVersion (pForSpeech);
		}

		if	(
				(IsAutoSize ())
			||	(!IsWindowVisible())
			)
		{
#ifdef	_DEBUG_AUTO_PACE
			if	(
					(LogIsActive (_DEBUG_AUTO_PACE))
				&&	(mText.GetWordCount() > 0)
				)
			{
				LogMessage (_DEBUG_AUTO_PACE, _T("[%p] ResetText"), this);
			}
#endif
#ifdef	_DEBUG_AUTO_SCROLL
			if	(
					(LogIsActive (_DEBUG_AUTO_SCROLL))
				&&	(mText.GetScrollMax() > 0)
				)
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] ResetScroll [%+d] in [%+d - %+d] by [%d]"), this, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
			}
#endif
			mText = pText;
			mText.DisplayFirstWord (pForSpeech ? true : false);
		}
		else
		{
			mText += pText;
		}

		if	(
				(mText.GetSapiVersion () != lSapiVersion)
			||	(mText.GetFullText () != lFullText)
			||	(mText.GetSpeechText () != lSpeechText)
			)
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CAgentBalloonWnd::ShowBalloon (bool pForSpeech, bool pTextChanged)
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
#if	TRUE
		if	(
				(pTextChanged)
			&&	(mShape.Ptr())
			&&	(IsWindowsVista_AtLeast ())
			&&	(
					(IsAutoSize ())
				||	(!IsWindowVisible())
				||	(
						(pForSpeech)
					?	(!IsSpeechShape ())
					:	(!IsThoughtShape ())
					)
				)
			)
		{
			CNotifyLock	lLock (0);
//
//	On Vista or later, we recreate the window.  This stops the previous window image from being
//	flashed on the screen before the new window image is displayed.
//
			CWnd *	lParent = GetOwner ();

#ifdef	_DEBUG_SHOW_HIDE
			if	(LogIsActive (_DEBUG_SHOW_HIDE))
			{
				LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] Recreate Balloon"), this);
			}
#endif
			DestroyWindow ();
			Create (lParent);
		}
#endif

		if	(IsWindow (m_hWnd))
		{
			CNotifyLock	lLock (0);
			CRect		lWinRect;

			if	(mText.GetFullText().IsEmpty ())
			{
				mToolInfo.lpszText = _T(" ");
			}
			else
			{
				mToolInfo.lpszText = (LPTSTR)(LPCTSTR)mText.GetFullText();
			}

			if	(
					(
						(pForSpeech)
					?	(mShape = new CAgentBalloonSpeak)
					:	(mShape = new CAgentBalloonThink)
					)
				&&	(CalcWinRect (lWinRect))
				)
			{
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("Balloon TTM_UPDATETIPTEXT [%s]"), FormatRect(lWinRect));
#endif
				SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (lWinRect.left, lWinRect.top));
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("Balloon TTM_UPDATETIPTEXT [%s]"), DebugStr(mToolInfo.lpszText));
#endif
				SendMessage (TTM_UPDATETIPTEXT, 0, (LPARAM)&mToolInfo);
				SendMessage (TTM_UPDATE);
				ApplyLayout (lWinRect);

#ifdef	_DEBUG_SHOW_HIDE
				if	(LogIsActive (_DEBUG_SHOW_HIDE))
				{
					LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] Show Balloon [%u %u] [%s]"), this, pForSpeech, pTextChanged, DebugStr(mText.GetFullText()));
				}
#endif
				Activate (TRUE);

#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("Balloon TTM_TRACKACTIVATE"));
#endif
				SendMessage (TTM_TRACKACTIVATE, TRUE, (LPARAM)&mToolInfo);
			}

			if	(IsWindowVisible())
			{
				SendMessage (TTM_UPDATE);
				SetWindowPos (&wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
				lRet = true;
			}
		}
	}
	return lRet;
}

void CAgentBalloonWnd::ShowedBalloon (bool pWasVisible)
{
	if	(
			(!pWasVisible)
		&&	(IsWindow (m_hWnd))
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->BalloonVisibleState (lNotify->_GetNotifyClient (mCharID), TRUE);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::HideBalloon (bool pFast)
{
	bool	lRet = false;
	bool	lWasVisible = false;

	if	(IsWindow (m_hWnd))
	{
#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] Hide Balloon [%u %u]"), this, pFast, IsWindowVisible());
		}
#endif
		if	(IsWindowVisible())
		{
			lWasVisible = true;
		}
		if	(pFast)
		{
			ShowWindow (SW_HIDE);
		}
		else
		{
			SendMessage (TTM_POP);
		}
		Activate (FALSE);
		StopAutoHide ();
		mPacingSpeech = false;
		lRet = true;
	}

	if	(
			(lRet)
		&&	(lWasVisible)
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->BalloonVisibleState (lNotify->_GetNotifyClient (mCharID), FALSE);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return lRet;
}

bool CAgentBalloonWnd::MoveBalloon ()
{
	bool	lRet = false;
	CRect	lWinRect;

	if	(
			(IsWindow (m_hWnd))
		&&	(CalcWinRect (lWinRect))
		)
	{
#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p]   MoveBalloon [%s]"), this, FormatRect(lWinRect));
		}
#endif
		SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (lWinRect.left, lWinRect.top));
		ApplyLayout (lWinRect);
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CString CAgentBalloonWnd::GetDisplayText ()
{
	return mText.GetFullText ();
}

CString CAgentBalloonWnd::GetSpeechText ()
{
	return mText.GetSpeechText ();
}

bool CAgentBalloonWnd::AbortSpeechText ()
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		if	(StopAutoPace ())
		{
			lRet = true;
		}
		if	(StopAutoScroll ())
		{
			lRet = true;
		}
		if	(StartAutoHide ())
		{
			lRet = true;
		}
		mAutoPaceDisabled = false;
		mPacingSpeech = false;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::CalcLayoutRects (CRect & pTextRect, CRect & pOwnerRect, CRect & pBounds)
{
	bool						lRet = false;
	CWnd *						lOwner;
	HMONITOR					lMonitor;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(
			(IsWindow (m_hWnd))
		&&	(lOwner = GetOwner())
		)
	{
		lOwner->GetWindowRect (&pOwnerRect);

		if	(
				(IsAutoSize ())
			||	(mLines < mMinLines)
			)
		{
			pTextRect = CRect (CPoint (0,0), mText.CalcTextSize (GetFont(), min(max(mPerLine,mMinPerLine),mMaxPerLine)));
		}
		else
		{
			pTextRect = CRect (CPoint (0,0), mText.CalcTextSize (GetFont(), min(max(mPerLine,mMinPerLine),mMaxPerLine), min(max(mLines,mMinLines),mMaxLines)));
		}
		pTextRect.OffsetRect (pOwnerRect.CenterPoint().x - pTextRect.CenterPoint().x, pOwnerRect.top - pTextRect.bottom);

#ifdef	_DEBUG_AUTO_SIZE
		if	(LogIsActive (_DEBUG_AUTO_SIZE))
		{
			LogMessage (_DEBUG_AUTO_SIZE, _T("[%p]   CalcLayout [%d %d] for [%s]"), this, pTextRect.Width(), pTextRect.Height(), DebugStr(mText.GetFullText()));
		}
#endif

		if	(
				(
					(lMonitor = MonitorFromWindow (lOwner->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST))
				||	(lMonitor = MonitorFromWindow (lOwner->GetSafeHwnd(), MONITOR_DEFAULTTOPRIMARY))
				)
			&&	(GetMonitorInfo (lMonitor, &lMonitorInfo))
			)
		{
			pBounds.CopyRect (&lMonitorInfo.rcWork);
		}
		else
		if	(!SystemParametersInfo (SPI_GETWORKAREA, 0, (LPVOID)&pBounds, 0))
		{
			pBounds.SetRect (0, 0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));
		}
		lRet = true;
	}
	return lRet;
}

bool CAgentBalloonWnd::CalcWinRect (CRect & pWinRect, bool pOnShow)
{
	CRect	lTextRect;
	CRect	lOwnerRect;
	CRect	lBounds;

	if	(
			(mShape)
		&&	(CalcLayoutRects (lTextRect, lOwnerRect, lBounds))
		)
	{
		if	(!pOnShow)
		{
			mShapeSize = NULL;
		}
		pWinRect = mShape->RecalcLayout (lTextRect, lOwnerRect, lBounds);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::ApplyLayout (const CRect & pWinRect, bool pOnShow)
{
	if	(!pOnShow)
	{
		mShapeSize = new CSize (pWinRect.Size());
	}
	if	(!mApplyingLayout)
	{
		mApplyingLayout = true;
		try
		{
			mShapeBuffer.EndBuffer (true, true);
			MoveWindow (pWinRect);
			if	(ApplyRegion ())
			{
				RedrawWindow ();
			}
		}
		catch AnyExceptionSilent
		mApplyingLayout = false;
	}
#ifdef	_DEBUG_LAYOUT
	else
	{
		LogMessage (_DEBUG_LAYOUT, _T("Balloon ApplyLayout recursion skipped"));
	}
#endif
}

bool CAgentBalloonWnd::ApplyRegion (bool pRedraw)
{
	bool	lRet = false;

	if	(!mApplyingRegion)
	{
		mApplyingRegion = true;
		try
		{
			CRgn	lRgn;

			if	(
					(mShape)
				&&	(lRgn.Attach (mShape->GetBalloonRgn ()))
				)
			{
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("Balloon SetWindowRgn"));
#endif
				SetLastError (0);
				if	(SetWindowRgn ((HRGN)lRgn.Detach(), (pRedraw!=false)))
				{
					lRet = true;
				}
				else
				{
					LogWinErr (LogNormal, GetLastError(), _T("SetWindowRgn"));
#ifdef	_DEBUG_LAYOUT
					LogMessage (_DEBUG_LAYOUT, _T("Balloon SetWindowRgn failed"));
#endif
				}
			}
#ifdef	_DEBUG_LAYOUT
			else
			{
				LogMessage (_DEBUG_LAYOUT, _T("Balloon SetWindowRgn no region"));
			}
#endif
		}
		catch AnyExceptionSilent
		mApplyingRegion = false;
	}
#ifdef	_DEBUG_LAYOUT
	else
	{
		LogMessage (_DEBUG_LAYOUT, _T("Balloon ApplyRegion recursion skipped"));
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::StartAutoPace ()
{
	bool	lRet = false;

	if	(
			(IsWindow (m_hWnd))
		&&	(IsAutoPace ())
		&&	(mText.GetWordCount() > 0)
		)
	{
		mAutoPaceTimer = SetTimer ((UINT_PTR)&mAutoPaceTimer, mAutoPaceTime, NULL);
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPace [%u] started for [%u] at [%d] of [%d]"), this, mAutoPaceTimer, mAutoPaceTime, mText.GetWordDisplayed(), mText.GetWordCount());
		}
#endif
		lRet = true;
	}
	else
	{
		StopAutoPace ();
	}
	return lRet;
}

bool CAgentBalloonWnd::StopAutoPace ()
{
	bool	lRet = false;

	if	(
			(IsWindow (m_hWnd))
		&&	(mAutoPaceTimer)
		)
	{
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPace [%u] stopped"), this, mAutoPaceTimer);
		}
#endif
		KillTimer (mAutoPaceTimer);
		lRet = true;
	}
	mAutoPaceTimer = 0;
	mText.DisplayAllWords ();
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::StartAutoHide ()
{
	bool				lRet = false;
	CAgentPopupWnd *	lOwner;

	if	(IsWindow (m_hWnd))
	{
		if	(
				(lOwner = DYNAMIC_DOWNCAST (CAgentPopupWnd, GetOwner ()))
			&&	(lOwner->KeepBalloonVisible (this))
			)
		{
			if	(
					(!IsAutoSize ())
				&&	(IsAutoPace ())
				)
			{
				mAutoHideTime = GetDelayTime (TTDT_INITIAL);
			}
			else
			if	(
					(IsAutoSize ())
				&&	(!IsAutoPace ())
				)
			{
				mAutoHideTime = GetDelayTime (TTDT_AUTOPOP);
			}
			else
			{
				mAutoHideTime = GetDelayTime (TTDT_AUTOPOP)/2;
			}
		}
		else
		if	(!IsAutoHide ())
		{
			mAutoHideTime = GetDelayTime (TTDT_INITIAL);
		}
		else
		if	(IsAutoPace ())
		{
			mAutoHideTime = GetDelayTime (TTDT_AUTOPOP)/2;
		}
		else
		{
			mAutoHideTime = GetDelayTime (TTDT_AUTOPOP);
		}
#ifdef	_DEBUG_AUTO_HIDE_NOT
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			mAutoHideTime *= 10;
		}
#endif

		mAutoHideTimer = SetTimer ((UINT_PTR)&mAutoHideTimer, mAutoHideTime, NULL);

#ifdef	_DEBUG_AUTO_HIDE
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p] AutoHide [%u] started with [%u]"), this, mAutoHideTimer, mAutoHideTime);
		}
#endif
		lRet = true;
	}
	else
	{
		StopAutoHide ();
	}
	return lRet;
}

bool CAgentBalloonWnd::StopAutoHide ()
{
	bool	lRet = false;

	if	(
			(IsWindow (m_hWnd))
		&&	(mAutoHideTimer)
		)
	{
		KillTimer (mAutoHideTimer);
#ifdef	_DEBUG_AUTO_HIDE
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p] AutoHide [%u] stopped"), this, mAutoHideTimer);
		}
#endif
		lRet = true;
	}
	mAutoHideTimer = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::StartAutoScroll ()
{
	DWORD	lScrollTime;

	if	(
			(IsWindow (m_hWnd))
		&&	(mShape)
		&&	(mText.CanScroll (mShape->mTextRect))
		)
	{
		if	(lScrollTime = mText.InitScroll (mShape->mTextRect, (mAutoScrollTimer==0), ((!IsAutoPace()) && mClipPartialLines), mAutoPaceTime))
		{
			mAutoScrollTimer = SetTimer ((UINT_PTR)&mAutoScrollTimer, lScrollTime, NULL);
#ifdef	_DEBUG_AUTO_SCROLL
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] AutoScroll [%u] started [%u] at [%+d] in [%+d - %+d] by [%d]"), this, mAutoScrollTimer, lScrollTime, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
			}
#endif
			return true;
		}
	}
	else
	{
#ifdef	_DEBUG_AUTO_SCROLL
		if	(
				(LogIsActive (_DEBUG_AUTO_SCROLL))
			&&	(mText.GetScrollMax() > 0)
			)
		{
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] StopScroll [%+d] in [%+d - %+d] by [%d]"), this, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
		}
#endif
		StopAutoScroll ();
	}
	return false;
}

bool CAgentBalloonWnd::StopAutoScroll ()
{
	bool	lRet = false;

	if	(
			(IsWindow (m_hWnd))
		&&	(mAutoScrollTimer)
		)
	{
#ifdef	_DEBUG_AUTO_SCROLL
		if	(LogIsActive (_DEBUG_AUTO_SCROLL))
		{
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] AutoScroll [%u] stopped"), this, mAutoScrollTimer);
		}
#endif
		KillTimer (mAutoScrollTimer);
		lRet = true;
	}
	mAutoScrollTimer = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::OnVoiceStart (long pCharID)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd OnVoiceStart [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, mText.GetSpeechText(), IsAutoPace (), IsAutoSize ());
	}
#endif
	mPacingSpeech = true;
	StopAutoPace ();
	StopAutoHide ();

	if	(
			(IsAutoPace ())
		||	(!IsAutoSize ())
		)
	{
		if	(mText.DisplayFirstWord (true))
		{
			ShowedVoiceWord ();
		}
	}
	else
	{
		mPacingSpeech = false;
	}
}

void CAgentBalloonWnd::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd OnVoiceEnd [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, mText.GetSpeechText(), IsAutoPace (), IsAutoSize ());
	}
#endif
	mPacingSpeech = false;
	if	(
			(
				(IsAutoPace ())
			||	(!IsAutoSize ())
			)
		&&	(mText.DisplayAllWords (true))
		)
	{
		ShowedVoiceWord ();
	}
	StopAutoPace ();
	StartAutoHide ();
}

void CAgentBalloonWnd::OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd   OnVoiceWord [%u (%d)] [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, pWordPos, pWordLength, DebugStr(mText.GetSpeechText().Mid (pWordPos, pWordLength)), IsAutoPace (), IsAutoSize ());
	}
#endif
	if	(
			(
				(IsAutoPace ())
			||	(!IsAutoSize ())
			)
		&&	(mText.DisplayThisWord (pWordPos, pWordLength, true))
		)
	{
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPaceSpeech word [%d] of [%d] [%s]"), this, mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
		}
#endif
		ShowedVoiceWord ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::ShowedVoiceWord ()
{
	if	(IsWindowVisible ())
	{
		mPacingWord = true;
		SendMessage (TTM_UPDATE);
		mPacingWord = false;
		RedrawWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::OnTimer(UINT_PTR nIDEvent)
{
	if	(
			(mAutoPaceTimer)
		&&	(nIDEvent == mAutoPaceTimer)
		)
	{
		if	(mText.DisplayNextWord ())
		{
#ifdef	_DEBUG_AUTO_PACE
			if	(LogIsActive (_DEBUG_AUTO_PACE))
			{
				LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPace [%u] word [%d] of [%d] [%s]"), this, mAutoPaceTimer, mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
			}
#endif
			if	(IsWindowVisible ())
			{
				RedrawWindow ();
			}
		}
		else
		{
#ifdef	_DEBUG_AUTO_PACE
			if	(LogIsActive (_DEBUG_AUTO_PACE))
			{
				LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPace [%u] complete [%s]"), this, mAutoPaceTimer, DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
			}
#endif
			StopAutoPace ();
			StartAutoHide ();
		}
	}
	else
	if	(
			(mAutoHideTimer)
		&&	(nIDEvent == mAutoHideTimer)
		)
	{
		CAgentPopupWnd *	lOwner;

#ifdef	_DEBUG_AUTO_HIDE
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p] AutoHide [%u] hide [%u]"), this, mAutoHideTimer, IsAutoHide());
		}
#endif
		StopAutoHide ();
		if	(
				(IsAutoHide ())
			&&	(lOwner = DYNAMIC_DOWNCAST (CAgentPopupWnd, GetOwner ()))
			&&	(!lOwner->KeepBalloonVisible (this))
			)
		{
			HideBalloon ();
		}
	}
	else
	if	(
			(mAutoScrollTimer)
		&&	(nIDEvent == mAutoScrollTimer)
		)
	{
		if	(
				(!mShape)
			||	(!mText.CanScroll (mShape->mTextRect))
			)
		{
#ifdef	_DEBUG_AUTO_SCROLL
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] StopScroll [%d] at [%+d] of [%+d - %+d]"), this, mAutoScrollTimer, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax());
			}
#endif
			StopAutoScroll ();
		}
		else
		if	(mText.Scroll ())
		{
#ifdef	_DEBUG_AUTO_SCROLL
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] AutoScroll [%u] at [%+d] in [%+d - %+d] by [%d]"), this, mAutoScrollTimer, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
			}
#endif
			if	(IsWindowVisible ())
			{
				RedrawWindow ();
			}
			if	(mAutoPaceTimer)
			{
				SetTimer (mAutoPaceTimer, mAutoPaceTime, NULL); // Delay autopace while scrolling
			}
		}
		else
		if	(!IsWindowVisible ())
		{
			StopAutoScroll ();
		}
	}
	else
	{
		CToolTipCtrl::OnTimer(nIDEvent);
	}
}

void CAgentBalloonWnd::OnDestroy()
{
#ifdef	_DEBUG_SHOW_HIDE
	if	(LogIsActive (_DEBUG_SHOW_HIDE))
	{
		LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] OnDestroy"), this);
	}
#endif
	mToolInfo.Clear ();
	mAutoPaceTimer = 0;
	mAutoHideTimer = 0;
	mAutoScrollTimer = 0;
	mPacingSpeech = false;
	mApplyingLayout = false;
	mApplyingRegion = false;
	mShapeSize = NULL;

	CToolTipCtrl::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::OnPaint ()
{
	tPtr <CPaintDC>	lPaintDC = new CPaintDC (this);
	CRect			lClientRect;

	GetClientRect (&lClientRect);

	if	(
			(
				(mDrawBuffer.GetBitmapSize () == lClientRect.Size ())
			||	(mDrawBuffer.CreateBuffer (lClientRect.Size(), true, true))
			)
		&&	(mDrawBuffer.StartBuffer ())
		)
	{
		DrawBalloon (mDrawBuffer.mDC, lClientRect);

		if	(IsDrawingLayered ())
		{
			BLENDFUNCTION	lBlendFunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
			CRect			lWinRect;

			lPaintDC = NULL;
			GetWindowRect (&lWinRect);
			if	(!UpdateLayeredWindow (NULL, &lWinRect.TopLeft(), &lWinRect.Size(), &mDrawBuffer.mDC, &lClientRect.TopLeft(), 0, &lBlendFunc, ULW_ALPHA))
			{
				ModifyStyleEx (WS_EX_LAYERED, 0);
				ModifyStyleEx (0, WS_EX_LAYERED);
				UpdateLayeredWindow (NULL, &lWinRect.TopLeft(), &lWinRect.Size(), &mDrawBuffer.mDC, &lClientRect.TopLeft(), 0, &lBlendFunc, ULW_ALPHA);
			}
		}
		else
		{
			lPaintDC->BitBlt (lClientRect.left, lClientRect.top, lClientRect.Width(), lClientRect.Height(), &mDrawBuffer.mDC, lClientRect.left, lClientRect.top, SRCCOPY);
		}
		mDrawBuffer.EndBuffer ();
	}
	else
	{
		DrawBalloon (lPaintDC->GetSafeHdc (), lClientRect);
	}
	mShapeBuffer.EndBuffer ();
}

BOOL CAgentBalloonWnd::OnEraseBkgnd (CDC * pDC)
{
	if	(IsDrawingLayered ())
	{
		return TRUE;
	}
	return CToolTipCtrl::OnEraseBkgnd (pDC);
}

LRESULT CAgentBalloonWnd::OnPrint(WPARAM wParam, LPARAM lParam)
{
	return Default ();
}

LRESULT CAgentBalloonWnd::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	if	(lParam & PRF_CLIENT)
	{
		CRect	lClientRect;

		GetClientRect (&lClientRect);
		DrawBalloon ((HDC)wParam, &lClientRect);
	}
	return 0;
}

BOOL CAgentBalloonWnd::OnCustomDraw (NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW	lNotify = (LPNMCUSTOMDRAW) pNMHDR;

	(*pResult) = CDRF_DODEFAULT;

#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("Balloon %s [%d %d %d %d (%d %d)] [%p] [%p]"), CustomDrawStage(lNotify), lNotify->rc.left, lNotify->rc.top, lNotify->rc.right, lNotify->rc.bottom, lNotify->rc.right-lNotify->rc.left, lNotify->rc.bottom-lNotify->rc.top, lNotify->hdc, ::WindowFromDC(lNotify->hdc));
#endif

	if	(lNotify->dwDrawStage == CDDS_PREPAINT)
	{
		(*pResult) |= CDRF_SKIPDEFAULT;
		return TRUE;
	}
	return FALSE;
}

void CAgentBalloonWnd::DrawBalloon (HDC pDC, const CRect & pDrawRect)
{
	CRect	lTextRect (pDrawRect);
	CRect	lMargin;

	if	(mShapeBuffer.GetBitmapSize () == pDrawRect.Size ())
	{
		mShapeBuffer.StartBuffer ();
	}
	else
	if	(
			(mShapeBuffer.CreateBuffer (pDrawRect.Size (), true, true))
		&&	(mShapeBuffer.StartBuffer ())
		)
	{
		if	(IsDrawingLayered ())
		{
			mShapeBuffer.PauseBuffer ();
			CBitmapAlpha::AlphaPreDrawBuffer (mShapeBuffer.mBitmap, mShapeBuffer.mBitmapBits, 0x00010101);
			mShapeBuffer.ResumeBuffer ();
		}

		mShape->Draw (mShapeBuffer.mDC, mBkColor, mBrColor);

		if	(IsDrawingLayered ())
		{
			mShapeBuffer.PauseBuffer ();
			CBitmapAlpha::AlphaPostDrawBuffer (mShapeBuffer.mBitmap, mShapeBuffer.mBitmapBits, 0x00010101);
			mShapeBuffer.ResumeBuffer ();
		}

#ifdef	_DEBUG_DRAW
		mShapeBuffer.PauseBuffer ();
		CBitmapDebugger().SaveBitmap (mShapeBuffer.mBitmap, _T("Balloon"));
		mShapeBuffer.ResumeBuffer ();
#endif
#ifdef	_DEBUG_DRAW
		mShapeBuffer.PauseBuffer ();
		static CBitmapDebugger lDebugger;
		lDebugger.ShowBitmap (mShapeBuffer.mBitmap);
		mShapeBuffer.ResumeBuffer ();
#endif
	}

	if	(mShapeBuffer.mDC.GetSafeHdc ())
	{
		::BitBlt (pDC, pDrawRect.left, pDrawRect.top, pDrawRect.Width(), pDrawRect.Height(), mShapeBuffer.mDC, pDrawRect.left, pDrawRect.top, SRCCOPY);
	}
	else
	{
		mShape->Draw (pDC, mBkColor, mBrColor);
	}

	GetMargin (&lMargin);
	lTextRect.DeflateRect (lMargin);
	DrawBalloonText (pDC, lTextRect);
}

void CAgentBalloonWnd::DrawBalloonText (HDC pDC, const CRect & pDrawRect)
{
	HGDIOBJ	lOldFont = NULL;
	DWORD	lLayout;
	CString	lText;
	CRect	lClipRect;
	INT_PTR	lPaceLookAhead = min (mText.GetWordCount() - mText.GetWordDisplayed() - 1, sSpeechPacingLookAhead);

	if	(mFont.GetSafeHandle())
	{
		lOldFont = ::SelectObject (pDC, mFont.GetSafeHandle());
	}
	::SetBkMode (pDC, TRANSPARENT);
	::SetTextColor (pDC, mFgColor);

	if	(IsAutoPace ())
	{
		lText = mText.GetDisplayText ();
	}
	else
	{
		lText = mText.GetFullText();
	}

	if	(
			(!IsAutoSize ())
		&&	(mPacingSpeech)
		&&	(lPaceLookAhead > 0)
		&&	(mText.GetWordDisplayed()+lPaceLookAhead < mText.GetWordCount())
		)
	{
//
//		When using BALLOON_STYLE_AUTOPACE with speech
//		  Measure for the next word so scrolling gets started early
//		When NOT using BALLOON_STYLE_AUTOPACE with speech
//		  Measure for the current word so scrolling matches the speech
//
 		CRect	lBounds;

		mText.mBounds = mShape->mTextRect;
		mText.MeasureText (mText.GetDisplayText (lPaceLookAhead), pDC);
		lBounds = mText.mBounds;
		mText.mBounds = mShape->mTextRect;
		mText.MeasureText (lText, pDC);
#ifdef	_DEBUG_AUTO_SCROLL
		if	(LogIsActive (_DEBUG_AUTO_SCROLL))
		{
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] MeasureText for word [%d (%+d)] [%s] (%d -> %d)"), this, mText.GetWordDisplayed()+lPaceLookAhead, lPaceLookAhead, mText.GetDisplayWord(mText.GetWordDisplayed()+lPaceLookAhead), mText.mBounds.Height(), lBounds.Height());
		}
#endif
		mText.mBounds = lBounds;
	}
	else
	{
		mText.mBounds = mShape->mTextRect;
		mText.MeasureText (lText, pDC);
	}

	lLayout = ApplyFontLayout (pDC);
	lClipRect.SetRect (mText.mBounds.left, mShape->mTextRect.top, mText.mBounds.right, mShape->mTextRect.bottom);

#ifdef	_DEBUG_NOT
	CBrush lBrush;
	lBrush.CreateSolidBrush (RGB(0xFF,0xC0,0xC0));
	::FillRect (pDC, &lClipRect, (HBRUSH)lBrush);
#endif

	if	(
			(!IsAutoSize ())
		&&	(mText.CanScroll (mShape->mTextRect))
		)
	{
		if	(StartAutoScroll ())
		{
#ifdef	_DEBUG_AUTO_SCROLL_NOT
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] Clip [%d] Text [%d (%d)] Lines [%d]"), this, mShape->mTextRect.Height(), mText.GetSize().cy, mText.mBounds.Height(), mText.GetLineCount());
			}
#endif
		}
		if	(mClipPartialLines)
		{
			mText.ApplyScroll (mShape->mTextRect, &lClipRect);
		}
		else
		{
			mText.ApplyScroll (mShape->mTextRect);
		}
#ifdef	_DEBUG_AUTO_SCROLL_NOT
		if	(LogIsActive (_DEBUG_AUTO_SCROLL))
		{
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] Clip [%d] Text [%d (%d)] Scroll [%d (%d)]"), this, mShape->mTextRect.Height(), mText.GetSize().cy, mText.mBounds.Height(), mText.GetScrollPos(), mShape->mTextRect.top - mText.mBounds.top);
		}
#endif
	}

	mText.DrawText (pDC, NULL, &lClipRect);

	::SetLayout (pDC, lLayout);
	if	(lOldFont)
	{
		::SelectObject (pDC, lOldFont);
	}
}

DWORD CAgentBalloonWnd::ApplyFontLayout (HDC pDC)
{
	DWORD			lLayout;
	tS <LOGFONT>	lLogFont;

	lLayout = ::GetLayout (pDC);

	if	(
			(mFont.GetSafeHandle())
		&&	(mFont.GetLogFont (&lLogFont))
		)
	{
		if	(
				(lLogFont.lfCharSet == ARABIC_CHARSET)
			||	(lLogFont.lfCharSet == HEBREW_CHARSET)
			)
		{
			if	(lLayout != LAYOUT_RTL)
			{
				CRect	lClientRect;
				CRect	lClipRect;

				GetClientRect (&lClientRect);
				GetClipBox (pDC, &lClipRect);
				lClientRect.right = max (lClientRect.right, lClipRect.right);

				::SetLayout (pDC, LAYOUT_RTL);
				mText.mBounds.left = lClientRect.right - mShape->mTextRect.right;
				mText.mBounds.right = lClientRect.right - mShape->mTextRect.left;
#ifdef	_DEBUG_RTL
				LogMessage (_DEBUG_RTL, _T("LAYOUT_RTL Client [%s] Shape [%s] Text [%s] Line [%s]"), FormatRect(lClientRect), FormatRect(mShape->mTextRect), FormatRect(mText.mBounds), FormatPoint(mText.GetLinePos(0)));
#endif
			}
		}
		else
		{
			if	(lLayout == LAYOUT_RTL)
			{
				CRect	lClientRect;
				CRect	lClipRect;

				GetClientRect (&lClientRect);
				GetClipBox (pDC, &lClipRect);
				lClientRect.right = max (lClientRect.right, lClipRect.right);

				::SetLayout (pDC, 0);
				mText.mBounds.left = lClientRect.right - mShape->mTextRect.right;
				mText.mBounds.right = lClientRect.right - mShape->mTextRect.left;
#ifdef	_DEBUG_RTL
				LogMessage (_DEBUG_RTL, _T("~LAYOUT_RTL Client [%s] Shape [%s] Text [%s] Line [%s]"), FormatRect(lClientRect), FormatRect(mShape->mTextRect), FormatRect(mText.mBounds), FormatPoint(mText.GetLinePos(0)));
#endif
			}
		}
	}

	return lLayout;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CAgentBalloonWnd::OnShow (NMHDR* pNMHDR, LRESULT* pResult)
{
	if	(mPacingWord)
	{
		return TRUE;
	}
	else
	{
		CRect	lWinRect;

#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p]   OnShow Balloon"), this);
		}
#endif
#ifdef	_DEBUG_LAYOUT
		GetWindowRect (&lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("Balloon OnShow [%s]"), FormatRect(lWinRect));
#endif

		if	(CalcWinRect (lWinRect, true))
		{
#ifdef	_DEBUG_SHOW_HIDE
			if	(LogIsActive (_DEBUG_SHOW_HIDE))
			{
				LogMessage (_DEBUG_SHOW_HIDE, _T("[%p]   OnShow [%s]"), this, FormatRect(lWinRect));
			}
#endif
			ApplyLayout (lWinRect, true);
			(*pResult) = TRUE;
			return TRUE;
		}
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::OnWindowPosChanging (WINDOWPOS *lpwndpos)
{
	CToolTipCtrl::OnWindowPosChanging (lpwndpos);
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("Balloon OnWindowPosChanging [%s]"), WindowPosStr(*lpwndpos));
#endif

	if	((lpwndpos->flags & SWP_NOSIZE) == 0)
	{
		if	(mShapeSize)
		{
			lpwndpos->cx = mShapeSize->cx;
			lpwndpos->cy = mShapeSize->cy;
		}
	}

	if	(
			(lpwndpos->flags & (SWP_SHOWWINDOW|SWP_FRAMECHANGED))
		&&	(!mApplyingRegion)
		)
	{
		ApplyRegion (true);
	}
}

void CAgentBalloonWnd::OnWindowPosChanged (WINDOWPOS *lpwndpos)
{
	CToolTipCtrl::OnWindowPosChanged (lpwndpos);
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("Balloon OnWindowPosChanged  [%s]"), WindowPosStr(*lpwndpos));
#endif
}

/////////////////////////////////////////////////////////////////////////////

_MFC_NCHITTEST_RESULT CAgentBalloonWnd::OnNcHitTest (CPoint point)
{
	return HTTRANSPARENT;
}
