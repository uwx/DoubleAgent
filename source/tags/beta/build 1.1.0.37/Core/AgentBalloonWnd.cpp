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
#include "EventNotify.h"
#include "AgentBalloonWnd.h"
#include "AgentBalloonShape.h"
#include "AgentCharacterWnd.h"
#include "ImageAlpha.h"
#include "Sapi5Voice.h"
#include "StringArrayEx.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "DebugWin.h"
#include "DebugProcess.h"
#include "ImageDebugger.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_LAYOUT		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_RTL			LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_DRAW			LogNormal|LogTimeMs
#define	_DEBUG_SHOW_HIDE		(GetProfileDebugInt(_T("DebugBalloonShow"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_AUTO_HIDE		(GetProfileDebugInt(_T("DebugBalloonHide"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_AUTO_SIZE		(GetProfileDebugInt(_T("DebugBalloonSize"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_AUTO_PACE		(GetProfileDebugInt(_T("DebugBalloonPace"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_AUTO_SCROLL		(GetProfileDebugInt(_T("DebugBalloonScroll"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugBalloonSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_OPTIONS			(GetProfileDebugInt(_T("DebugBalloonOptions"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_FONT				(GetProfileDebugInt(_T("DebugBalloonFont"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Balloon"),LogDetails,true)&0xFFFF)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
//#define	_TRACE_RESOURCES_EX		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#define	_DRAW_LAYERED	IsWindowsVista_AtLeast()

/////////////////////////////////////////////////////////////////////////////

const USHORT	CAgentBalloonWnd::mMinLines = 1;
const USHORT	CAgentBalloonWnd::mMaxLines = 128;
const USHORT	CAgentBalloonWnd::mDefLines = 2;

const USHORT	CAgentBalloonWnd::mMinPerLine = 8;
const USHORT	CAgentBalloonWnd::mMaxPerLine = 255;
const USHORT	CAgentBalloonWnd::mDefPerLine = 32;

const UINT		CAgentBalloonWnd::mVoiceStartMsg = RegisterWindowMessage (_T("7781F0DC-B3CD-4ce9-8828-95C4FB56BBE2"));
const UINT		CAgentBalloonWnd::mVoiceEndMsg = RegisterWindowMessage (_T("0BEA4D03-95C6-4c2f-A5BF-EFFE63D24D8F"));
const UINT		CAgentBalloonWnd::mVoiceWordMsg = RegisterWindowMessage (_T("82C73827-F1B8-4223-824B-BC0953892D56"));

static const int	sSpeechPacingLookAhead = 2;

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentBalloonOptions)
IMPLEMENT_DLL_OBJECT(CAgentBalloonWnd)

CAgentBalloonWnd::CAgentBalloonWnd ()
:	mCharID (0),
	mLangID (GetUserDefaultUILanguage ()),
	mAutoPaceDisabled (false),
	mAutoPaceTime (300),
	mAutoPaceTimer (0),
	mAutoHideTimer (0),
	mAutoHideTime (0),
	mAutoScrollTimer (0),
	mPacingSpeech (false),
	mRedrawDisabled (false),
	mApplyingLayout (false),
	mApplyingRegion (false),
	mPaused (false),
	mPausedWord (-1),
	mDebugRecursionLevel (0),
	mOwnerWnd (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::CAgentBalloonWnd (%d)"), this, _AtlModule.GetLockCount());
	}
#endif
}

CAgentBalloonWnd::~CAgentBalloonWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::~CAgentBalloonWnd (%d)"), this, _AtlModule.GetLockCount());
	}
#endif
	if	(IsWindow ())
	{
		DestroyWindow ();
	}
	Detach (-1, NULL);
}

CAgentBalloonWnd * CAgentBalloonWnd::CreateInstance (long pCharID, CAtlPtrTypeArray <CEventNotify> & pNotify)
{
	CComObject<CAgentBalloonWnd> *	lInstance = NULL;
	INT_PTR							lNdx;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<CAgentBalloonWnd>::CreateInstance (&lInstance))))
	{
		lInstance->mCharID = pCharID;
		lInstance->mNotify.Copy (pNotify);

		for	(lNdx = 0; lNdx < (INT_PTR)lInstance->mNotify.GetCount(); lNdx++)
		{
			if	(lInstance->mNotify [lNdx])
			{
				lInstance->mNotify [lNdx]->RegisterEventLock (lInstance, true);
			}
		}
	}
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::FinalRelease [%u]"), this, IsInNotify());
	}
#endif
}

bool CAgentBalloonWnd::CanFinalRelease ()
{
	if	(IsInNotify() == 0)
	{
		return true;
	}
	else
	if	(IsWindow ())
	{
		PostMessage (NULL, 0, 0); // To ensure OnFinalMessage gets called
	}
	return false;
}

void CAgentBalloonWnd::OnFinalMessage (HWND)
{
	if	(
			(HasFinalReleased ())
		&&	(CanFinalRelease ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd::OnFinalMessage [%u]"), this, IsInNotify());
		}
#endif
		m_dwRef = 1;
		Release ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
UINT CAgentBalloonWnd::EnterRecursion () const {return ++mDebugRecursionLevel;}
UINT CAgentBalloonWnd::ExitRecursion () const {mDebugRecursionLevel=max(mDebugRecursionLevel,1); return --mDebugRecursionLevel;}
CAtlString CAgentBalloonWnd::RecursionIndent () const {return CAtlString(_T(' '), (int)mDebugRecursionLevel*2);}
#else
UINT CAgentBalloonWnd::EnterRecursion () const {return 0;}
UINT CAgentBalloonWnd::ExitRecursion () const {return 0;}
CAtlString CAgentBalloonWnd::RecursionIndent () const {return CAtlString();}
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::SetOptions (DWORD pStyle, const CAgentFileBalloon & pFileBalloon, LANGID pLangID)
{
	bool						lRet = false;
	CAgentBalloonOptions *		lOldOptions = mPendingOptions.Ptr() ? mPendingOptions.Ptr() : &mOptions;
	tPtr <CAgentBalloonOptions>	lNewOptions = new CAgentBalloonOptions (*lOldOptions);

	lNewOptions->mStyle = pStyle;
	lNewOptions->mLines = (USHORT)pFileBalloon.mLines;
	lNewOptions->mPerLine = (USHORT)pFileBalloon.mPerLine;
	lNewOptions->mBkColor = pFileBalloon.mBkColor;
	lNewOptions->mFgColor = pFileBalloon.mFgColor;
	lNewOptions->mBrColor = pFileBalloon.mBrColor;

	CopyBalloonFont (pFileBalloon, lNewOptions->mFont);
	if	(pLangID)
	{
		SetFontLangID (lNewOptions->mFont, pLangID);
	}
	if	(mFont.GetSafeHandle())
	{
		GetObject (mFont, sizeof(lOldOptions->mFont), &lOldOptions->mFont);
	}

#ifdef	_DEBUG_FONT
	if	(LogIsActive (_DEBUG_FONT))
	{
		LOGFONT	lActualFont;

		if	(memcmp (&lOldOptions->mFont, &lNewOptions->mFont, sizeof(LOGFONT)) == 0)
		{
			LogMessage (_DEBUG_FONT, _T("[%p] CAgentBalloonWnd SameFont [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), this, lOldOptions->mFont.lfFaceName, lOldOptions->mFont.lfHeight, lOldOptions->mFont.lfWidth, lOldOptions->mFont.lfWeight, lOldOptions->mFont.lfItalic, lOldOptions->mFont.lfUnderline, lOldOptions->mFont.lfStrikeOut, lOldOptions->mFont.lfCharSet, lOldOptions->mFont.lfPitchAndFamily, lOldOptions->mFont.lfQuality);
		}
		else
		{
			LogMessage (_DEBUG_FONT, _T("[%p] CAgentBalloonWnd Font [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), this, lNewOptions->mFont.lfFaceName, lNewOptions->mFont.lfHeight, lNewOptions->mFont.lfWidth, lNewOptions->mFont.lfWeight, lNewOptions->mFont.lfItalic, lNewOptions->mFont.lfUnderline, lNewOptions->mFont.lfStrikeOut, lNewOptions->mFont.lfCharSet, lNewOptions->mFont.lfPitchAndFamily, lNewOptions->mFont.lfQuality);
			if	(GetActualFont (lNewOptions->mFont, lActualFont))
			{
				LogMessage (_DEBUG_FONT, _T("[%p]           Actual Font [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), this, lActualFont.lfFaceName, lActualFont.lfHeight, lActualFont.lfWidth, lActualFont.lfWeight, lActualFont.lfItalic, lActualFont.lfUnderline, lActualFont.lfStrikeOut, lActualFont.lfCharSet, lActualFont.lfPitchAndFamily, lActualFont.lfQuality);
			}
		}
	}
#endif

	GetActualFont (lNewOptions->mFont, lNewOptions->mFont);

	if	(lNewOptions->operator != (*lOldOptions))
	{
#ifdef	_DEBUG_OPTIONS
		if	(LogIsActive (_DEBUG_OPTIONS))
		{
			CAgentBalloonOptions	lSaveOptions (mOptions);
			mOptions = *lNewOptions;
			LogMessage (_DEBUG_OPTIONS, _T("[%p] SetOptions    Style [%8.8X] AutoSize [%u] AutoPace [%u] AutoHide [%u] PartialLines [%u]"), this, mOptions.mStyle, IsAutoSize(), IsAutoPace(), IsAutoHide(), !ClipPartialLines());
			LogMessage (_DEBUG_OPTIONS, _T("[%p]               Lines [%hu] PerLine [%hu] BkColor [%8.8X] FgColor [%8.8X] BrColor [%8.8X]"), this, mOptions.mLines, mOptions.mPerLine, mOptions.mBkColor, mOptions.mFgColor, mOptions.mBrColor);
			mOptions = lSaveOptions;
		}
#endif
		mPendingOptions = lNewOptions.Detach ();

		if	(
				(IsWindow ())
			&&	(!(mPendingOptions->mStyle & BalloonStyle_Enabled))
			)
		{
			HideBalloon (true);
		}
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CAgentBalloonOptions * CAgentBalloonWnd::GetNextOptions () const
{
	if	(mPendingOptions)
	{
		return new CAgentBalloonOptions (*mPendingOptions);
	}
	else
	{
		return new CAgentBalloonOptions (mOptions);
	}
}

bool CAgentBalloonWnd::ApplyOptions (CAgentBalloonOptions * pOptions)
{
	bool	lRet = false;

	if	(pOptions)
	{
		mOptions = *pOptions;
	}

	if	(IsWindow ())
	{
		CFontHandle	lFont;

#ifdef	_DEBUG_OPTIONS
		if	(LogIsActive (_DEBUG_OPTIONS))
		{
			LogMessage (_DEBUG_OPTIONS, _T("[%p] ApplyOptions  Style [%8.8X] AutoSize [%u] AutoPace [%u] AutoHide [%u] PartialLines [%u]"), this, mOptions.mStyle, IsAutoSize(), IsAutoPace(), IsAutoHide(), !ClipPartialLines());
			LogMessage (_DEBUG_OPTIONS, _T("[%p]               Lines [%hu] PerLine [%hu] BkColor [%8.8X] FgColor [%8.8X] BrColor [%8.8X]"), this, mOptions.mLines, mOptions.mPerLine, mOptions.mBkColor, mOptions.mFgColor, mOptions.mBrColor);
		}
#endif
		if	(mOptions.mFont.lfHeight)
		{
			lFont = CreateFontIndirect (&mOptions.mFont);
		}
		
		if	(
				((COLORREF) SendMessage (TTM_GETTIPBKCOLOR) != mOptions.mBkColor)
			||	((COLORREF) SendMessage (TTM_GETTIPTEXTCOLOR) != mOptions.mFgColor)
			||	(!FontEqual (mFont, lFont))
			)
		{
			EnterRecursion ();

			try
			{
				if	(IsWindowVisible ())
				{
#ifdef	_DEBUG_SHOW_HIDE
					if	(LogIsActive (_DEBUG_SHOW_HIDE))
					{
						LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)] Hide Balloon for ApplyOptions"), this, IsPaused());
					}
#endif
					ShowWindow (SW_HIDE);
					SendMessage (TTM_ACTIVATE, FALSE);
				}
				SendMessage (TTM_SETTIPBKCOLOR, (WPARAM)mOptions.mBkColor);
				SendMessage (TTM_SETTIPTEXTCOLOR, (WPARAM)mOptions.mFgColor);
				mFont = lFont.Detach ();
				SetFont (mFont.GetSafeHandle());
				mShapeBuffer.EndBuffer (true);
			}
			catch AnyExceptionDebug

			ExitRecursion ();
		}
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonOptions::CAgentBalloonOptions ()
{
	mStyle = BalloonStyle_AutoPace|BalloonStyle_AutoHide;
	mLines = CAgentBalloonWnd::mDefLines;
	mPerLine = CAgentBalloonWnd::mDefPerLine;
	mBkColor = GetSysColor (COLOR_INFOBK);
	mFgColor = GetSysColor (COLOR_INFOTEXT);
	mBrColor = GetSysColor (COLOR_INFOTEXT);
	memset (&mFont, 0, sizeof(LOGFONT));
}

CAgentBalloonOptions::CAgentBalloonOptions (const CAgentBalloonOptions & pSource)
{
	operator= (pSource);
}

CAgentBalloonOptions & CAgentBalloonOptions::operator= (const CAgentBalloonOptions & pSource)
{
	mStyle = pSource.mStyle;
	mLines = pSource.mLines;
	mPerLine = pSource.mPerLine;
	mBkColor = pSource.mBkColor;
	mFgColor = pSource.mFgColor;
	mBrColor = pSource.mBrColor;
	memcpy (&mFont, &pSource.mFont, sizeof(LOGFONT));

	return *this;
}

bool CAgentBalloonOptions::operator== (const CAgentBalloonOptions & pSource) const
{
	return	(
				(mStyle == pSource.mStyle)
			&&	(mLines == pSource.mLines)
			&&	(mPerLine == pSource.mPerLine)
			&&	(mBkColor == pSource.mBkColor)
			&&	(mFgColor == pSource.mFgColor)
			&&	(mBrColor == pSource.mBrColor)
			&&	(memcmp (&mFont, &pSource.mFont, sizeof(LOGFONT)) == 0)
			);
}

bool CAgentBalloonOptions::operator!= (const CAgentBalloonOptions & pSource) const
{
	return !operator== (pSource);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::CopyBalloonFont (const CAgentFileBalloon & pFileBalloon, LOGFONT & pFont)
{
	if	(
			(pFileBalloon.mFont.lfFaceName[0])
		&&	(pFileBalloon.mFont.lfHeight != 0)
		)
	{
		_tcscpy (pFont.lfFaceName, pFileBalloon.mFont.lfFaceName);
		pFont.lfHeight = pFileBalloon.mFont.lfHeight;
		pFont.lfWeight = pFileBalloon.mFont.lfWeight;
		pFont.lfItalic = pFileBalloon.mFont.lfItalic;
		pFont.lfUnderline = pFileBalloon.mFont.lfUnderline;
		pFont.lfStrikeOut = pFileBalloon.mFont.lfStrikeOut;
		pFont.lfCharSet = pFileBalloon.mFont.lfCharSet;
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
		pFileBalloon.mFont = pFont;
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
	CMemDCHandle	lDC;
	CFontHandle		lFont;

	memcpy (&pActualFont, &pFont, sizeof(LOGFONT));

	if	(
			(_tcsicmp (pActualFont.lfFaceName, _T("MS Sans Serif")) == 0)
		||	(_tcsicmp (pActualFont.lfFaceName, _T("MS Shell Dlg")) == 0)
		)
	{
		_tcscpy (pActualFont.lfFaceName, _T("MS Shell Dlg 2"));
	}

	if	(
			(lFont = CreateFontIndirect (&pActualFont))
		&&	(lDC.Attach (CreateCompatibleDC (NULL)))
		)
	{
		tS <TEXTMETRIC> lMetric;
		TCHAR			lFontFace [MAX_PATH];

		SelectObject (lDC, lFont);
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
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::FontEqual (HFONT pFont1, HFONT pFont2)
{
	tS <LOGFONT>	lFont1;
	tS <LOGFONT>	lFont2;
	
	if	(
			(pFont1)
		&&	(GetObject (pFont1, sizeof(LOGFONT), &lFont1))
		&&	(pFont2)
		&&	(GetObject (pFont2, sizeof(LOGFONT), &lFont2))
		)
	{
		return FontEqual (lFont1, lFont2);
	}
	return false;
}

bool CAgentBalloonWnd::FontEqual (const LOGFONT & pFont1, const LOGFONT & pFont2)
{
	if	(
			(pFont1.lfWidth == pFont2.lfWidth)
		&&	(pFont1.lfHeight == pFont2.lfHeight)
		&&	(pFont1.lfWeight == pFont2.lfWeight)
		&&	(pFont1.lfItalic == pFont2.lfItalic)
		&&	(pFont1.lfUnderline == pFont2.lfUnderline)
		&&	(pFont1.lfStrikeOut == pFont2.lfStrikeOut)
		&&	(pFont1.lfCharSet == pFont2.lfCharSet)
		&&	(_tcscmp (pFont1.lfFaceName, pFont2.lfFaceName) == 0)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::Create (CWindow * pOwnerWnd, DWORD pExStyle)
{
	bool	lRet = false;

	if	(
			(pOwnerWnd)
		&&	(pOwnerWnd->IsWindow ())
		&&	(CAgentBalloonWndObj::Create (pOwnerWnd->m_hWnd, CRect(0,0,0,0), _T(""), WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE, pExStyle, 0U, NULL))
		)
	{
		mOwnerWnd = pOwnerWnd;
#ifdef	_DRAW_LAYERED
		if	(IsDrawingLayered ())
		{
			SetClassLong (m_hWnd, GCL_STYLE, GetClassLong (m_hWnd, GCL_STYLE & ~CS_DROPSHADOW));
		}
		else
		{
			SetClassLong (m_hWnd, GCL_STYLE, GetClassLong (m_hWnd, GCL_STYLE | CS_DROPSHADOW));
		}
#endif
		ModifyStyle (WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx (WS_EX_TOPMOST, pExStyle);

		mToolInfo.uFlags = TTF_ABSOLUTE|TTF_TRACK|TTF_TRANSPARENT;
		mToolInfo.hwnd = pOwnerWnd->m_hWnd;
		if	(mText.GetFullText().IsEmpty ())
		{
			mToolInfo.lpszText = _T(" ");
		}
		else
		{
			mToolInfo.lpszText = (LPTSTR)(LPCTSTR)mText.GetFullText();
		}
		SendMessage (TTM_ADDTOOL, 0, (LPARAM)&mToolInfo);

		ApplyOptions ();
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd [%p] Parent [%p] [%p] Owner [%p]"), this, m_hWnd, pOwnerWnd->m_hWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER));
		}
#endif
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::Attach (long pCharID, CEventNotify * pNotify, bool pSetActiveCharID)
{
	bool	lRet = false;

	if	(
			(pNotify)
		&&	(mNotify.AddUnique (pNotify) >= 0)
		)
	{
		pNotify->RegisterEventLock (this, true);
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

bool CAgentBalloonWnd::Detach (long pCharID, CEventNotify * pNotify)
{
	bool	lRet = false;

	if	(
			(pNotify)
		&&	(mNotify.Remove (pNotify) >= 0)
		)
	{
		pNotify->RegisterEventLock (this, false);
		lRet = true;
	}

	if	(
			(pCharID < 0)
		&&	(!pNotify)
		)
	{
		for	(INT_PTR lNdx = (INT_PTR)mNotify.GetCount()-1; lNdx >= 0; lNdx--)
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

bool CAgentBalloonWnd::_PreNotify ()
{
	if	(m_dwRef > 0)
	{
		return CEventNotifiesClient<CAgentBalloonWnd>::_PreNotify ();
	}
	return false;
}

bool CAgentBalloonWnd::_PostNotify ()
{
	CEventNotifiesClient<CAgentBalloonWnd>::_PostNotify ();

	if	(
			(HasFinalReleased ())
		&&	(CanFinalRelease ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p] CAgentBalloonWnd PostNotify -> DestroyWindow"), this);
		}
#endif
		if	(IsWindow ())
		{
			DestroyWindow ();
		}
		return false;
	}
	return true;
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
		return dynamic_cast <const CAgentBalloonSpeak *> (mShape.Ptr()) ? true : false;
	}
	return false;
}

bool CAgentBalloonWnd::IsThoughtShape () const
{
	if	(mShape)
	{
		return dynamic_cast <const CAgentBalloonThink *> (mShape.Ptr()) ? true : false;
	}
	return false;
}

bool CAgentBalloonWnd::IsBusy (bool pForIdle) const
{
	if	(
			(IsWindow ())
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
		//LogMessage (LogDebugFast, _T("--- Balloon PacingSpeech [%u] AutoPaceTimer [%u] AutoScrollTimer [%u] AutoHideTimer [%u]"), mPacingSpeech, mAutoPaceTimer, mAutoScrollTimer, mAutoScrollTimer, mAutoHideTimer);
		return true;
	}
	return false;
}

bool CAgentBalloonWnd::IsPaused () const
{
	return mPaused;
}

bool CAgentBalloonWnd::IsDrawingLayered () const
{
#ifdef	_DRAW_LAYERED
	if	(
			(_DRAW_LAYERED)
		&&	(m_hWnd)
		&&	(GetExStyle () & WS_EX_LAYERED)
		)
	{
		return true;
	}
#endif
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::IsAutoSize () const
{
	return ((mOptions.mStyle & BalloonStyle_SizeToText) != 0);
}

bool CAgentBalloonWnd::IsAutoPace () const
{
	return ((mOptions.mStyle & BalloonStyle_AutoPace) != 0) && (!mAutoPaceDisabled);
}

bool CAgentBalloonWnd::IsAutoHide () const
{
	return ((mOptions.mStyle & BalloonStyle_AutoHide) != 0);
}

bool CAgentBalloonWnd::ClipPartialLines () const
{
	return (((mOptions.mStyle & BalloonStyle_SizeToText) == 0) && ((mOptions.mStyle & BalloonStyle_ShowPartialLines) == 0));
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::ShowBalloonSpeech (LPCTSTR pText, UINT pSapiVersion, bool pNoAutoPace)
{
	CAgentTextParse	lText (CAtlString(pText).TrimLeft().TrimRight(), pSapiVersion);

	return ShowBalloonSpeech (lText, pNoAutoPace);
}

bool CAgentBalloonWnd::ShowBalloonSpeech (const CAgentText & pText, bool pNoAutoPace)
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(!IsPaused ())
		)
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
	CAgentTextParse	lText (CAtlString(pText).TrimLeft().TrimRight(), pSapiVersion);

	return ShowBalloonThought (lText, pNoAutoPace);
}

bool CAgentBalloonWnd::ShowBalloonThought (const CAgentText & pText, bool pNoAutoPace)
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(!IsPaused ())
		)
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

	if	(IsWindow ())
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

	if	(IsWindow ())
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

	if	(IsWindow ())
	{
		if	(IsWindowVisible ())
		{
			lRet = true;
		}
		if	(!IsPaused ())
		{
			StopAutoHide ();
		}
	}
	return lRet;
}

bool CAgentBalloonWnd::ShowBalloonText (const CAgentText & pText, UINT pForSpeech, bool pNoAutoPace)
{
	bool	lRet = false;

	if	(IsWindow ())
	{
		CAtlString	lFullText (mText.GetFullText ());
		CAtlString	lSpeechText (mText.GetSpeechText ());
		UINT		lSapiVersion = mText.GetSapiVersion ();

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
				LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] ResetText"), this, IsPaused());
			}
#endif
#ifdef	_DEBUG_AUTO_SCROLL
			if	(
					(LogIsActive (_DEBUG_AUTO_SCROLL))
				&&	(mText.GetScrollMax() > 0)
				)
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] ResetScroll [%+d] in [%+d - %+d] by [%d]"), this, IsPaused(), mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
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

	if	(IsWindow ())
	{
		CRect	lWinRect;

		if	(
				(IsWindowVisible ())
			&&	(
					(pForSpeech)
				?	(IsThoughtShape())
				:	(IsSpeechShape())
				)
			)
		{
#ifdef	_DEBUG_SHOW_HIDE
			if	(LogIsActive (_DEBUG_SHOW_HIDE))
			{
				LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)] Hide Balloon for shape change"), this, IsPaused());
			}
#endif
			ShowWindow (SW_HIDE);
		}

		if	(
				(IsWindowsVista_AtLeast ())
			&&	(IsDrawingLayered ())
			&&	(
					(IsAutoSize ())
				||	(!IsWindowVisible ())
				)
			)
		{
			//
			//	On Vista or later, recreate the window to stop DWM from showing a blank window or the previous window image.
			//
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon Recreate"), RecursionIndent());
#endif
			EnterRecursion ();
			DestroyWindow ();
			Create (mOwnerWnd, mOwnerWnd->GetExStyle() & WS_EX_TOPMOST);
			ExitRecursion ();
		}
		else
		{
			ApplyOptions ();
		}

		if	(
				(pForSpeech)
			?	(mShape = new CAgentBalloonSpeak)
			:	(mShape = new CAgentBalloonThink)
			)
		{
#ifdef	_DRAW_LAYERED
			if	(IsDrawingLayered ())
			{
				mShape->mUseGdiplus = new CUseGdiplus;
			}
#endif
		}

		if	(mText.GetFullText().IsEmpty ())
		{
			mToolInfo.lpszText = _T(" ");
		}
		else
		{
			mToolInfo.lpszText = (LPTSTR)(LPCTSTR)mText.GetFullText();
		}

#ifdef	_DEBUG_LAYOUT
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ShowBalloon [%u]"), RecursionIndent(), IsWindowVisible());
#endif
		EnterRecursion ();
		
		try
		{
			if	(
					(mShape)
				&&	(CalcWinRect (lWinRect))
				)
			{
				SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (lWinRect.left, lWinRect.top));
				SendMessage (TTM_UPDATETIPTEXT, 0, (LPARAM)&mToolInfo);
				SendMessage (TTM_UPDATE);

				ApplyLayout (lWinRect);
#ifdef	_DEBUG_SHOW_HIDE
				if	(LogIsActive (_DEBUG_SHOW_HIDE))
				{
					LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)] Show Balloon [%u %u] [%s]"), this, IsPaused(), pForSpeech, pTextChanged, DebugStr(mText.GetFullText()));
				}
#endif
				SendMessage (TTM_ACTIVATE, TRUE);
				SendMessage (TTM_TRACKACTIVATE, TRUE, (LPARAM)&mToolInfo);
			}
			else
			if	(IsWindowVisible())
			{
				SendMessage (TTM_UPDATE);
			}
			if	(IsWindowVisible())
			{
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon HWND_TOP"), RecursionIndent());
#endif
				EnterRecursion ();
				SetWindowPos ((GetExStyle() & WS_EX_TOPMOST) ? HWND_TOPMOST : HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
				ExitRecursion ();
				lRet = true;
			}
		}
		catch AnyExceptionDebug

		ExitRecursion ();
#ifdef	_DEBUG_LAYOUT
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ShowBalloon Done"), RecursionIndent());
#endif
	}
	return lRet;
}

void CAgentBalloonWnd::ShowedBalloon (bool pWasVisible)
{
	if	(
			(!pWasVisible)
		&&	(IsWindow ())
		&&	(!IsPaused ())
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = (INT_PTR)mNotify.GetCount()-1; lNotify = mNotify (lNotifyNdx); lNotifyNdx--)
			{
				lNotify->BalloonVisibleState (lNotify->GetNotifyClient (mCharID), TRUE);
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

	if	(IsWindow ())
	{
#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)] Hide Balloon [%u %u]"), this, IsPaused(), pFast, IsWindowVisible());
		}
#endif
#ifdef	_DEBUG_LAYOUT
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon HideBalloon"), RecursionIndent());
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
		SendMessage (TTM_ACTIVATE, FALSE);

		if	(!IsPaused ())
		{
			StopAutoHide ();
			mPacingSpeech = false;
		}
		lRet = true;
	}

	if	(
			(lRet)
		&&	(lWasVisible)
		&&	(!IsPaused ())
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = (INT_PTR)mNotify.GetCount()-1; lNotify = mNotify (lNotifyNdx); lNotifyNdx--)
			{
				lNotify->BalloonVisibleState (lNotify->GetNotifyClient (mCharID), FALSE);
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
			(IsWindow ())
		&&	(CalcWinRect (lWinRect))
		)
	{
#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)]   MoveBalloon [%s]"), this, IsPaused(), FormatRect(lWinRect));
		}
#endif
		SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (lWinRect.left, lWinRect.top));
		ApplyLayout (lWinRect);
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentBalloonWnd::GetDisplayText ()
{
	return mText.GetFullText ();
}

CAtlString CAgentBalloonWnd::GetSpeechText ()
{
	return mText.GetSpeechText ();
}

bool CAgentBalloonWnd::AbortSpeechText ()
{
	bool	lRet = false;

	if	(IsWindow ())
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

bool CAgentBalloonWnd::Pause (bool pPause)
{
	bool	lRet = false;

	if	(mPaused != pPause)
	{
		if	(mPaused = pPause)
		{
			mPausedWord = mText.GetWordDisplayed ();
#ifdef	_DEBUG_AUTO_PACE
			if	(LogIsActive (_DEBUG_AUTO_PACE))
			{
				LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPacePause  word [%d] of [%d] [%s]"), this, IsPaused(), mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
			}
#endif
		}
		else
		{
			if	(
					(mPausedWord > 0)
				&&	(mPausedWord != mText.GetWordDisplayed ())
				)
			{
				ShowedVoiceWord (false);
			}
			if	(mAutoHideTimer)
			{
				StartAutoHide (); // Restart for full AutoHide time
			}
		}
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::CalcLayoutRects (CRect & pTextRect, CRect & pOwnerRect, CRect & pBounds)
{
	bool						lRet = false;
	CAgentCharacterWnd *		lOwner;
	HMONITOR					lMonitor;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(
			(IsWindow ())
		&&	(mOwnerWnd)
		&&	(mOwnerWnd->IsWindow ())
		)
	{
		mOwnerWnd->GetWindowRect (&pOwnerRect);

		if	(
				(mOwnerWnd->GetStyle () & WS_CHILD)
			&&	(lOwner = dynamic_cast <CAgentCharacterWnd *> ((CAgentWnd*)mOwnerWnd))
			)
		{
			CRect	lVideoRect = lOwner->GetVideoRect ();

			lVideoRect.OffsetRect (pOwnerRect.left, pOwnerRect.top);
			pOwnerRect = lVideoRect;
		}

		if	(
				(IsAutoSize ())
			||	(mOptions.mLines < mMinLines)
			)
		{
			pTextRect = CRect (CPoint (0,0), mText.CalcTextSize (mFont.GetSafeHandle()?mFont.GetSafeHandle():GetFont(), min(max(mOptions.mPerLine,mMinPerLine),mMaxPerLine)));
		}
		else
		{
			pTextRect = CRect (CPoint (0,0), mText.CalcTextSize (mFont.GetSafeHandle()?mFont.GetSafeHandle():GetFont(), min(max(mOptions.mPerLine,mMinPerLine),mMaxPerLine), min(max(mOptions.mLines,mMinLines),mMaxLines)));
		}
		pTextRect.OffsetRect (pOwnerRect.CenterPoint().x - pTextRect.CenterPoint().x, pOwnerRect.top - pTextRect.bottom);

#ifdef	_DEBUG_AUTO_SIZE
		if	(LogIsActive (_DEBUG_AUTO_SIZE))
		{
			LogMessage (_DEBUG_AUTO_SIZE, _T("[%p(%u)]   CalcLayout [%s] for [%s]"), this, IsPaused(), FormatSize(pTextRect.Size()), DebugStr(mText.GetFullText()));
		}
#endif

		if	(
				(
					(lMonitor = MonitorFromWindow (mOwnerWnd->m_hWnd, MONITOR_DEFAULTTONEAREST))
				||	(lMonitor = MonitorFromWindow (mOwnerWnd->m_hWnd, MONITOR_DEFAULTTOPRIMARY))
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
#ifdef	_DEBUG_LAYOUT
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon RecalcLayout [%s] [%s] [%s] -> [%s]"), RecursionIndent(), FormatRect(lTextRect), FormatRect(lOwnerRect), FormatRect(lBounds), FormatRect(pWinRect));
#endif
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
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ApplyLayout [%d] [%s]"), RecursionIndent(), pOnShow, FormatRect(pWinRect));
#endif
			EnterRecursion ();
			mShapeBuffer.EndBuffer (true);
			MoveWindow (pWinRect);
			if	(ApplyRegion ())
			{
				RedrawWindow ();
			}
			else
			{
				Invalidate ();
			}
			ExitRecursion ();
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ApplyLayout [%d] [%s] Done"), RecursionIndent(), pOnShow, FormatRect(pWinRect));
#endif
		}
		catch AnyExceptionSilent
		mApplyingLayout = false;
	}
#ifdef	_DEBUG_LAYOUT
	else
	{
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ApplyLayout recursion skipped"), RecursionIndent());
	}
#endif
}

bool CAgentBalloonWnd::ApplyRegion (bool pRedraw)
{
	bool	lRet = false;

	if	(
			(!mApplyingRegion)
		&&	(!IsDrawingLayered ())
		)
	{
		mApplyingRegion = true;
		try
		{
			CRgnHandle	lRgn;

			if	(
					(mShape)
				&&	(lRgn.Attach (mShape->GetBalloonRgn ()))
				)
			{
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon SetWindowRgn [%s] [%s]"), RecursionIndent(), FormatRect(mShape->mBalloonRect), (mShapeSize ? (LPCTSTR)FormatSize(*mShapeSize) : NULL));
#endif
				EnterRecursion ();
				SetLastError (0);
				if	(SetWindowRgn (lRgn.Detach(), (pRedraw!=false)))
				{
					ExitRecursion ();
#ifdef	_DEBUG_LAYOUT
					LogMessage (_DEBUG_LAYOUT, _T("%sBalloon SetWindowRgn [%s] [%s] Done"), RecursionIndent(), FormatRect(mShape->mBalloonRect), (mShapeSize ? (LPCTSTR)FormatSize(*mShapeSize) : NULL));
#endif
					lRet = true;
				}
				else
				{
					ExitRecursion ();
					LogWinErr (LogNormal, GetLastError(), _T("SetWindowRgn"));
#ifdef	_DEBUG_LAYOUT
					LogMessage (_DEBUG_LAYOUT, _T("%sBalloon SetWindowRgn failed"), RecursionIndent());
#endif
				}
			}
#ifdef	_DEBUG_LAYOUT
			else
			{
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon SetWindowRgn no region"), RecursionIndent());
			}
#endif
		}
		catch AnyExceptionSilent
		mApplyingRegion = false;
	}
#ifdef	_DEBUG_LAYOUT
	else
	{
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ApplyRegion recursion skipped"), RecursionIndent());
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
			(IsWindow ())
		&&	(IsAutoPace ())
		&&	(mText.GetWordCount() > 0)
		)
	{
#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::SetAutoPaceTimer [%u]"), this, mAutoPaceTimer);
		}
#endif

		mAutoPaceTimer = SetTimer ((UINT_PTR)&mAutoPaceTimer, mAutoPaceTime, NULL);

#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::SetAutoPaceTimer [%u] Done"), this, mAutoPaceTimer);
		}
#endif
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPace [%u] started for [%u] at [%d] of [%d]"), this, IsPaused(), mAutoPaceTimer, mAutoPaceTime, mText.GetWordDisplayed(), mText.GetWordCount());
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
			(IsWindow ())
		&&	(mAutoPaceTimer)
		)
	{
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPace [%u] stopped"), this, IsPaused(), mAutoPaceTimer);
		}
#endif
#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::KillAutoPaceTimer [%u]"), this, mAutoPaceTimer);
		}
#endif

		KillTimer (mAutoPaceTimer);

#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::KillAutoPaceTimer Done"), this);
		}
#endif
		lRet = true;
	}
	mAutoPaceTimer = 0;
	mText.DisplayAllWords ();
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::StartAutoHide ()
{
	bool					lRet = false;
	CAgentCharacterWnd *	lOwner;

	if	(IsWindow ())
	{
		if	(
				(lOwner = dynamic_cast <CAgentCharacterWnd *> ((CAgentWnd*)mOwnerWnd))
			&&	(lOwner->KeepBalloonVisible (this))
			)
		{
			if	(
					(!IsAutoSize ())
				&&	(IsAutoPace ())
				)
			{
				mAutoHideTime = (DWORD)SendMessage (TTM_GETDELAYTIME, TTDT_INITIAL);
			}
			else
			if	(
					(IsAutoSize ())
				&&	(!IsAutoPace ())
				)
			{
				mAutoHideTime = (DWORD)SendMessage (TTM_GETDELAYTIME, TTDT_AUTOPOP);
			}
			else
			{
				mAutoHideTime = (DWORD)SendMessage (TTM_GETDELAYTIME, TTDT_AUTOPOP)/2;
			}
		}
		else
		if	(!IsAutoHide ())
		{
			mAutoHideTime = (DWORD)SendMessage (TTM_GETDELAYTIME, TTDT_INITIAL);
		}
		else
		if	(IsAutoPace ())
		{
			mAutoHideTime = (DWORD)SendMessage (TTM_GETDELAYTIME, TTDT_AUTOPOP)/2;
		}
		else
		{
			mAutoHideTime = (DWORD)SendMessage (TTM_GETDELAYTIME, TTDT_AUTOPOP);
		}
#ifdef	_DEBUG_AUTO_HIDE_NOT
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			mAutoHideTime *= 10;
		}
#endif
#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::SetAutoHideTimer [%u]"), this, mAutoHideTimer);
		}
#endif

		mAutoHideTimer = SetTimer ((UINT_PTR)&mAutoHideTimer, mAutoHideTime, NULL);

#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::SetAutoHideTimer [%u] Done"), this, mAutoHideTimer);
		}
#endif
#ifdef	_DEBUG_AUTO_HIDE
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p(%u)] AutoHide [%u] started with [%u]"), this, IsPaused(), mAutoHideTimer, mAutoHideTime);
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
			(IsWindow ())
		&&	(mAutoHideTimer)
		)
	{
#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::KillAutoHideTimer [%u]"), this, mAutoHideTimer);
		}
#endif

		KillTimer (mAutoHideTimer);

#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::KillAutoHideTimer Done"), this);
		}
#endif
#ifdef	_DEBUG_AUTO_HIDE
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p(%u)] AutoHide [%u] stopped"), this, IsPaused(), mAutoHideTimer);
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
			(IsWindow ())
		&&	(mShape)
		&&	(mText.CanScroll (mShape->mTextRect))
		)
	{
		if	(lScrollTime = mText.InitScroll (mShape->mTextRect, (mAutoScrollTimer==0), ((!IsAutoPace()) && ClipPartialLines()), mAutoPaceTime))
		{
#ifdef	_TRACE_RESOURCES_EX
			if	(LogIsActive (_TRACE_RESOURCES_EX))
			{
				CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::SetAutoScrollTimer [%u]"), this, mAutoScrollTimer);
			}
#endif

			mAutoScrollTimer = SetTimer ((UINT_PTR)&mAutoScrollTimer, lScrollTime, NULL);

#ifdef	_TRACE_RESOURCES_EX
			if	(LogIsActive (_TRACE_RESOURCES_EX))
			{
				CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::SetAutoScrollTimer [%u] Done"), this, mAutoScrollTimer);
			}
#endif
#ifdef	_DEBUG_AUTO_SCROLL
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] AutoScroll [%u] started [%u] at [%+d] in [%+d - %+d] by [%d]"), this, IsPaused(), mAutoScrollTimer, lScrollTime, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
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
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] StopScroll [%+d] in [%+d - %+d] by [%d]"), this, IsPaused(), mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
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
			(IsWindow ())
		&&	(mAutoScrollTimer)
		)
	{
#ifdef	_DEBUG_AUTO_SCROLL
		if	(LogIsActive (_DEBUG_AUTO_SCROLL))
		{
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] AutoScroll [%u] stopped"), this, IsPaused(), mAutoScrollTimer);
		}
#endif
#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::KillAutoScrollTimer [%u]"), this, mAutoScrollTimer);
		}
#endif

		KillTimer (mAutoScrollTimer);

#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::KillAutoScrollTimer Done"), this);
		}
#endif
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
		LogMessage (_DEBUG_SPEECH, _T("[%p(%u)] CAgentBalloonWnd OnVoiceStart [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, IsPaused(), DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize ());
	}
#endif
	PostMessage (mVoiceStartMsg, pCharID);
}

void CAgentBalloonWnd::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p(%u)] CAgentBalloonWnd OnVoiceEnd [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, IsPaused(), DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize ());
	}
#endif
	PostMessage (mVoiceEndMsg, pCharID);
}

void CAgentBalloonWnd::OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p(%u)] CAgentBalloonWnd   OnVoiceWord [%u (%d)] [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, IsPaused(), pWordPos, pWordLength, DebugStr(mText.GetSpeechText().Mid (pWordPos, pWordLength)), IsAutoPace (), IsAutoSize ());
	}
#endif
	PostMessage (mVoiceWordMsg, pCharID, MAKELPARAM (pWordPos, pWordLength));
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p(%u)] CAgentBalloonWnd OnVoiceStartMsg [%s] IsAutoPace [%u] IsAutoSize [%u] Word [%d] of [%d]"), this, IsPaused(), DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize (), mText.GetWordDisplayed(), mText.GetWordCount());
	}
#endif
	StopAutoHide ();

	if	(!IsPaused ())
	{
		mPacingSpeech = true;
		StopAutoPace ();
	
		if	(
				(IsAutoPace ())
			||	(!IsAutoSize ())
			)
		{
			if	(
					(mPausedWord > 0)
				&&	(mText.DisplayThisWord (mPausedWord, true))
				)
			{
#ifdef	_DEBUG_AUTO_PACE
				if	(LogIsActive (_DEBUG_AUTO_PACE))
				{
					LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPaceResume word [%d] of [%d] [%s]"), this, IsPaused(), mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
				}
#endif
				ShowedVoiceWord (false);
			}
			else
			if	(mText.DisplayFirstWord (true))
			{
#ifdef	_DEBUG_AUTO_PACE
				if	(LogIsActive (_DEBUG_AUTO_PACE))
				{
					LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPaceSpeech word [%d] of [%d] [%s]"), this, IsPaused(), mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
				}
#endif
				ShowedVoiceWord (false);
			}
			mPausedWord = -1;
		}
		else
		{
			mPacingSpeech = false;
		}
	}
	return 0;
}

LRESULT CAgentBalloonWnd::OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p(%u)] CAgentBalloonWnd OnVoiceEndMsg [%s] IsAutoPace [%u] IsAutoSize [%u] Word [%d] of [%d]"), this, IsPaused(), DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize (), mText.GetWordDisplayed(), mText.GetWordCount());
	}
#endif
	if	(!IsPaused ())
	{
		mPacingSpeech = false;

		if	(
				(IsAutoPace ())
			||	(!IsAutoSize ())
			)
		{
			if	(mText.GetWordDisplayed() >= mText.GetWordCount()-2)
			{
				if	(mText.DisplayAllWords (true))
				{
#ifdef	_DEBUG_AUTO_PACE
					if	(LogIsActive (_DEBUG_AUTO_PACE))
					{
						LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPaceSpeech word [%d] of [%d] [%s]"), this, IsPaused(), mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
					}
#endif
					ShowedVoiceWord (false);
				}
				StopAutoPace ();
			}
			else
			{
				if	(mText.DisplayNextWord (true))
				{
#ifdef	_DEBUG_AUTO_PACE
					if	(LogIsActive (_DEBUG_AUTO_PACE))
					{
						LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPaceSpeech word [%d] of [%d] [%s]"), this, IsPaused(), mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
					}
#endif
					ShowedVoiceWord (false);
				}
				StartAutoPace ();
			}
		}
		else
		{
			StopAutoPace ();
		}
  	}
	if	(!mAutoPaceTimer)
	{
		StartAutoHide ();
	}

	return 0;
}

LRESULT CAgentBalloonWnd::OnVoiceWordMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	UINT	lWordPos = (UINT)LOWORD(lParam);
	int		lWordLength = (int)HIWORD(lParam);
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p(%u)] CAgentBalloonWnd   OnVoiceWordMsg [%u (%d)] [%s] IsAutoPace [%u] IsAutoSize [%u] Word [%d] of [%d]"), this, IsPaused(), lWordPos, lWordLength, DebugStr(mText.GetSpeechText().Mid (lWordPos, lWordLength)), IsAutoPace (), IsAutoSize (), mText.GetWordDisplayed(), mText.GetWordCount());
	}
#endif
	if	(
			(
				(IsAutoPace ())
			||	(!IsAutoSize ())
			)
		&&	(mText.DisplayThisWord (lWordPos, lWordLength, true))
		)
	{
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPaceSpeech word [%d] of [%d] [%s]"), this, IsPaused(), mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
		}
#endif
		if	(IsPaused ())
		{
			mPausedWord = mText.GetWordDisplayed ();
		}
		ShowedVoiceWord (true);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::ShowedVoiceWord (bool pFastRefresh)
{
	if	(
			(IsWindowVisible ())
		&&	(!mRedrawDisabled)
		)
	{
		bool	lTextDrawn = false;

		if	(pFastRefresh)
		{
			CRect	lClientRect;
			CRect	lMargin;
			HDC		lDC = NULL;

			GetClientRect (&lClientRect);

			if	(
					(mShape)
				&&	(mShapeBuffer.GetImageSize () == lClientRect.Size ())
				&&	(lDC = GetDC ())
				&&	(
						(mDrawBuffer.GetImageSize () == lClientRect.Size ())
					||	(mDrawBuffer.CreateBuffer (lClientRect.Size(), true, true))
					)
				&&	(mDrawBuffer.StartBuffer ())
				)
			{
				if	(mShapeBuffer.StartBuffer ())
				{
					::BitBlt (mDrawBuffer.GetDC(), lClientRect.left, lClientRect.top, lClientRect.Width(), lClientRect.Height(), *mShapeBuffer.mDC, lClientRect.left, lClientRect.top, SRCCOPY);
				}
				else
				{
					FillSolidRect (mDrawBuffer.GetDC(), &lClientRect, mOptions.mBkColor);
				}
				mShapeBuffer.EndBuffer ();

				SendMessage (TTM_GETMARGIN, 0, (LPARAM)&lMargin);
				lClientRect.DeflateRect (lMargin);
				DrawBalloonText (mDrawBuffer.GetDC(), lClientRect);

				if	(IsDrawingLayered ())
				{
					BLENDFUNCTION	lBlendFunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
					CRect			lWinRect;

					GetWindowRect (&lWinRect);

					if	(::UpdateLayeredWindow (m_hWnd, NULL, &lWinRect.TopLeft(), &lWinRect.Size(), mDrawBuffer.GetDC(), &CPoint (0,0), 0, &lBlendFunc, ULW_ALPHA))
					{
						lTextDrawn = true;
					}
#ifdef	_DEBUG_DRAW
					else
					{
						LogMessage (_DEBUG_DRAW, _T("%sBalloon UpdateLayeredWindow (AutoPaceWord) failed"), RecursionIndent());
					}
#endif
				}
				else
				{
					::BitBlt (lDC, mShape->mTextRect.left, mShape->mTextRect.top, mShape->mTextRect.Width(), mShape->mTextRect.Height(), mDrawBuffer.GetDC(), mShape->mTextRect.left, mShape->mTextRect.top, SRCCOPY);
					lTextDrawn = true;
				}
			}

			mDrawBuffer.EndBuffer ();
			if	(lDC)
			{
				ReleaseDC (lDC);
			}
		}

		if	(!lTextDrawn)
		{
			mRedrawDisabled = true;
			SendMessage (TTM_UPDATE);
			mRedrawDisabled = false;
#ifdef	_DEBUG_DRAW
			LogMessage (_DEBUG_DRAW, _T("%sBalloon RedrawWindow (AutoPaceWord)"), RecursionIndent());
#endif
			RedrawWindow ();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = DefWindowProc (uMsg, wParam, lParam);

	if	(
			(mAutoPaceTimer)
		&&	(wParam == mAutoPaceTimer)
		&&	(!IsPaused ())
		)
	{
		if	(mText.DisplayNextWord ())
		{
#ifdef	_DEBUG_AUTO_PACE
			if	(LogIsActive (_DEBUG_AUTO_PACE))
			{
				LogMessage (_DEBUG_AUTO_PACE, _T("[%p(%u)] AutoPace [%u] word [%d] of [%d] [%s]"), this, IsPaused(), mAutoPaceTimer, mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
			}
#endif
			if	(IsWindowVisible ())
			{
#ifdef	_DEBUG_DRAW
				LogMessage (_DEBUG_DRAW, _T("%sBalloon RedrawWindow (AutoPace)"), RecursionIndent());
#endif
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

	if	(
			(mAutoScrollTimer)
		&&	(wParam == mAutoScrollTimer)
		&&	(!IsPaused ())
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
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] StopScroll [%d] no scrolling"), this, IsPaused(), mAutoScrollTimer);
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
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] AutoScroll [%u] at [%+d] in [%+d - %+d] by [%d]"), this, IsPaused(), mAutoScrollTimer, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax(), mText.GetScrollInc());
			}
#endif
			if	(IsWindowVisible ())
			{
#ifdef	_DEBUG_DRAW
				LogMessage (_DEBUG_DRAW, _T("%sBalloon RedrawWindow (AutoScroll)"), RecursionIndent());
#endif
				RedrawWindow ();
			}
			if	(mAutoPaceTimer)
			{
#ifdef	_TRACE_RESOURCES_EX
				if	(LogIsActive (_TRACE_RESOURCES_EX))
				{
					CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::ResetAutoPaceTimer [%u]"), this, mAutoPaceTimer);
				}
#endif

				SetTimer (mAutoPaceTimer, mAutoPaceTime, NULL); // Delay autopace while scrolling

#ifdef	_TRACE_RESOURCES_EX
				if	(LogIsActive (_TRACE_RESOURCES_EX))
				{
					CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentBalloonWnd::ResetAutoPaceTimer [%u] Done"), this, mAutoPaceTimer);
				}
#endif
			}
		}
		else
		if	(
				(mText.GetScrollPos() >= mText.GetScrollMax())
			||	(!IsWindowVisible ())
			)
		{
#ifdef	_DEBUG_AUTO_SCROLL
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] StopScroll [%d] at [%+d] of [%+d - %+d]"), this, IsPaused(), mAutoScrollTimer, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax());
			}
#endif
			StopAutoScroll ();
		}
	}

	if	(
			(mAutoHideTimer)
		&&	(wParam == mAutoHideTimer)
		&&	(!IsPaused ())
		)
	{
		CAgentCharacterWnd *	lOwner;

#ifdef	_DEBUG_AUTO_HIDE
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p(%u)] AutoHide [%u] hide [%u]"), this, IsPaused(), mAutoHideTimer, IsAutoHide());
		}
#endif
		StopAutoHide ();
		if	(
				(IsAutoHide ())
			&&	(lOwner = dynamic_cast <CAgentCharacterWnd *> ((CAgentWnd*)mOwnerWnd))
			&&	(!lOwner->KeepBalloonVisible (this))
			)
		{
			HideBalloon ();
		}
#ifdef	_DEBUG_AUTO_HIDE
		else
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p(%u)] AutoHide [%u] skipped"), this, IsPaused(), mAutoHideTimer);
		}
#endif
	}
	return lResult;
}

LRESULT CAgentBalloonWnd::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
#ifdef	_DEBUG_SHOW_HIDE
	if	(LogIsActive (_DEBUG_SHOW_HIDE))
	{
		LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)] OnDestroy"), this, IsPaused());
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

#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CAgentBalloonWnd [%p] Parent [%p] OnDestroy"), this, IsPaused(), m_hWnd, ::GetParent(m_hWnd));
	}
#endif
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	tS <PAINTSTRUCT>	lPaintStruct;
	CMemDCHandle		lPaintDC;
	CRect				lClientRect;

	if	(
			(!mRedrawDisabled)
		&&	(lPaintDC = BeginPaint (&lPaintStruct))
		)
	{
#ifdef	_DEBUG_DRAW
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnPaint [%p] [%p] Visible [%u %u]"), RecursionIndent(), (HDC)lPaintDC, m_hWnd, IsWindowVisible(), mRedrawDisabled);
#endif
		GetClientRect (&lClientRect);

		if	(
				(
					(mDrawBuffer.GetImageSize () == lClientRect.Size ())
				||	(mDrawBuffer.CreateBuffer (lClientRect.Size(), true, true))
				)
			&&	(mDrawBuffer.StartBuffer ())
			)
		{
			DrawBalloon (mDrawBuffer.GetDC(), lClientRect);

			if	(IsDrawingLayered ())
			{
				BLENDFUNCTION	lBlendFunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
				CRect			lWinRect;

				EndPaint (&lPaintStruct);
				lPaintDC.Detach ();

				GetWindowRect (&lWinRect);
				if	(!::UpdateLayeredWindow (m_hWnd, NULL, &lWinRect.TopLeft(), &lWinRect.Size(), mDrawBuffer.GetDC(), &CPoint (0,0), 0, &lBlendFunc, ULW_ALPHA))
				{
#ifdef	_DEBUG_DRAW
					LogMessage (_DEBUG_DRAW, _T("%sBalloon Reset WS_EX_LAYERED (OnPaint)"), RecursionIndent());
#endif
					ModifyStyleEx (WS_EX_LAYERED, 0);
					ModifyStyleEx (0, WS_EX_LAYERED);
					::UpdateLayeredWindow (m_hWnd, NULL, &lWinRect.TopLeft(), &lWinRect.Size(), mDrawBuffer.GetDC(), &CPoint (0,0), 0, &lBlendFunc, ULW_ALPHA);
				}
			}
			else
			{
				::BitBlt (lPaintDC, lClientRect.left, lClientRect.top, lClientRect.Width(), lClientRect.Height(), mDrawBuffer.GetDC(), lClientRect.left, lClientRect.top, SRCCOPY);
			}
			mDrawBuffer.EndBuffer ();
		}
		else
		{
			DrawBalloon (lPaintDC, lClientRect);
		}

		if	(lPaintDC.GetSafeHandle ())
		{
			EndPaint (&lPaintStruct);
		}
		mShapeBuffer.EndBuffer ();
	}
	lPaintDC.Detach ();
	return 0;
}

LRESULT CAgentBalloonWnd::OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	if	(IsDrawingLayered ())
	{
#ifdef	_DEBUG_DRAW
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnEraseBkgnd Visible [%u %u]"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
		bHandled = TRUE;
		return TRUE;
	}
	if	(
			(mRedrawDisabled)
		||	(IsWindowsVista_AtLeast ())
		)
	{
		bHandled = TRUE;
		return TRUE;
	}
	return 0;
}

LRESULT CAgentBalloonWnd::OnPrint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnPrint Visible [%u %u]"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
	return 0;
}

LRESULT CAgentBalloonWnd::OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if	(lParam & PRF_CLIENT)
	{
		CRect	lClientRect;

		GetClientRect (&lClientRect);
#ifdef	_DEBUG_DRAW
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnPrintClient [%p] [%p] Visible [%u %u]"), RecursionIndent(), (HDC)wParam, ::WindowFromDC((HDC)wParam), IsWindowVisible(), mRedrawDisabled);
#endif
		DrawBalloon ((HDC)wParam, &lClientRect);
	}
	return 0;
}

LRESULT CAgentBalloonWnd::OnCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LRESULT			lResult = CDRF_DODEFAULT;
	LPNMCUSTOMDRAW	lNotify = (LPNMCUSTOMDRAW) pnmh;

#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon %s [%s] DC [%p] DCWin [%p] Win [%p] Visible [%u %u]"), RecursionIndent(), CustomDrawStage(lNotify), FormatRect(lNotify->rc), lNotify->hdc, ::WindowFromDC(lNotify->hdc), m_hWnd, IsWindowVisible(), mRedrawDisabled);
#endif

	if	(lNotify->dwDrawStage == CDDS_PREPAINT)
	{
		CRect	lClientRect;

		GetClientRect (&lClientRect);

		if	(
				(
					(mDrawBuffer.GetImageSize () == lClientRect.Size ())
				||	(mDrawBuffer.CreateBuffer (lClientRect.Size(), true, true))
				)
			&&	(mDrawBuffer.StartBuffer ())
			)
		{
			DrawBalloon (mDrawBuffer.GetDC(), lClientRect);
			BitBlt (lNotify->hdc, lNotify->rc.left, lNotify->rc.top, lClientRect.Width(), lClientRect.Height(), mDrawBuffer.GetDC(), 0, 0, SRCCOPY);
			mDrawBuffer.EndBuffer ();
		}
		else
		{
			lClientRect.OffsetRect (lNotify->rc.left-lClientRect.left, lNotify->rc.top-lClientRect.top);
			DrawBalloon (lNotify->hdc, &lClientRect);
		}
		lResult |= CDRF_SKIPDEFAULT;
	}
	return lResult;
}

void CAgentBalloonWnd::DrawBalloon (HDC pDC, const CRect & pDrawRect)
{
	CRect	lTextRect (pDrawRect);
	CRect	lMargin;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif
#ifdef	_DEBUG_DRAW
	if	(LogIsActive (_DEBUG_DRAW))
	{
		CRect	lWinRect;
		GetWindowRect (&lWinRect);
		SendMessage (TTM_GETMARGIN, 0, (LPARAM)&lMargin);
		LogMessage (_DEBUG_DRAW, _T("%sBalloon Draw [%s] In [%p] [%s] Margin [%s] Visible [%u %u]"), RecursionIndent(), FormatRect(pDrawRect), ::WindowFromDC(pDC), FormatSize(lWinRect.Size()), FormatMargin(lMargin), IsWindowVisible(), mRedrawDisabled);
	}
#endif

	if	(mShapeBuffer.GetImageSize () == pDrawRect.Size ())
	{
		mShapeBuffer.StartBuffer ();
	}
	else
	{
		if	(mShapeBuffer.CreateBuffer (pDrawRect.Size(), true, true))
		{
			mShape->Draw (*mShapeBuffer.mDC, mOptions.mBkColor, mOptions.mBrColor);

#ifdef	_DEBUG_DRAW_EX
			mShapeBuffer.PauseBuffer ();
			CImageDebugger::SaveBitmap (mShapeBuffer.GetImage(), _T("Balloon"));
			mShapeBuffer.ResumeBuffer ();
#endif
#ifdef	_DEBUG_DRAW_EX
			mShapeBuffer.PauseBuffer ();
			static CImageDebugger lDebugger;
			lDebugger.ShowBitmap (mShapeBuffer.GetImage());
			mShapeBuffer.ResumeBuffer ();
#endif
		}
	}

	SendMessage (TTM_GETMARGIN, 0, (LPARAM)&lMargin);
	lTextRect.DeflateRect (lMargin);

	if	(mShapeBuffer.GetDC ())
	{
		::BitBlt (pDC, pDrawRect.left, pDrawRect.top, pDrawRect.Width(), pDrawRect.Height(), *mShapeBuffer.mDC, pDrawRect.left, pDrawRect.top, SRCCOPY);
	}
	else
	{
		mShape->Draw (pDC, mOptions.mBkColor, mOptions.mBrColor);
	}
	DrawBalloonText (pDC, lTextRect);

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentBalloonWnd::DrawBalloon"), DebugTimeElapsed);
#endif
}

void CAgentBalloonWnd::DrawBalloonText (HDC pDC, const CRect & pDrawRect)
{
	HGDIOBJ	lOldFont = NULL;
	DWORD	lLayout;
	CRect	lClipRect;
	CRect	lTextBounds;
	int		lTextLength = mText.GetFullText().GetLength();
	INT_PTR	lPaceLookAhead = min (mText.GetWordCount() - mText.GetWordDisplayed() - 1, sSpeechPacingLookAhead);
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(mFont.GetSafeHandle())
	{
		lOldFont = ::SelectObject (pDC, mFont.GetSafeHandle());
	}
	::SetBkMode (pDC, TRANSPARENT);
	::SetTextColor (pDC, mOptions.mFgColor);

	if	(mText.mBounds.IsRectEmpty ())
	{
		mText.mBounds = mShape->mTextRect;
		mText.MeasureText (mText.GetFullText(), pDC);
	}
	lTextBounds = mText.mBounds;
	lClipRect = mShape->mTextRect;

	if	(
			(!IsAutoSize ())
		&&	(mText.CanScroll (mShape->mTextRect))
		)
	{
//
//		When using BalloonStyle_AutoPace with speech
//		  Measure for the next word so scrolling gets started early
//		When NOT using BalloonStyle_AutoPace with speech
//		  Measure for the current word so scrolling matches the speech

		if	(
				(IsAutoPace ())
			||	(mPacingSpeech)
			)
		{
			if	(
					(mPacingSpeech)
				&&	(lPaceLookAhead > 0)
				)
			{
				mText.mBounds = mText.GetUsedRect (false, mText.GetDisplayText(lPaceLookAhead));
			}
			else
			{
				mText.mBounds = mText.GetUsedRect (false, mText.GetDisplayText());
			}
			mText.mBounds.left = lTextBounds.left;
			mText.mBounds.right = lTextBounds.right;
		}

		if	(StartAutoScroll ())
		{
#ifdef	_DEBUG_AUTO_SCROLL_NOT
			if	(LogIsActive (_DEBUG_AUTO_SCROLL))
			{
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] Clip [%d] Text [%d (%d)] Lines [%d]"), this, IsPaused(), mShape->mTextRect.Height(), mText.GetSize().cy, mText.mBounds.Height(), mText.GetLineCount());
			}
#endif
		}
		if	(ClipPartialLines())
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
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p(%u)] Clip [%d] Text [%d (%d)] Scroll [%d (%d)]"), this, IsPaused(), mShape->mTextRect.Height(), mText.GetSize().cy, mText.mBounds.Height(), mText.GetScrollPos(), mShape->mTextRect.top - mText.mBounds.top);
		}
#endif
	}

	lLayout = ApplyFontLayout (pDC);
	lClipRect.left = mText.mBounds.left;
	lClipRect.right = mText.mBounds.right;

	if	(
			(IsAutoPace ())
		&&	(mText.GetWordDisplayed()+lPaceLookAhead < mText.GetWordCount())
		)
	{
		//
		//	Note - AutoPace AND AutoScroll (i.e. !AutoSize) AND !ClipPartial actually does show partial lines,
		//	it's just that the partial line at the end is empty.  It looks like a bug, but it's not.
		//
		mText.DrawText (pDC, mText.mBounds, mText.GetDisplayText(), NULL, &lClipRect);
	}
	else
	{
		mText.DrawText (pDC, NULL, &lClipRect);
	}
	mText.mBounds = lTextBounds;

	::SetLayout (pDC, lLayout);
	if	(lOldFont)
	{
		::SelectObject (pDC, lOldFont);
	}

	if	(
			(pDC == mDrawBuffer.GetDC ())
		&&	(IsDrawingLayered ())
		)
	{
		CBrushHandle	lAlphaBrush;
		CRgnHandle		lAlphaRgn;
		int				lROP2;

		if	(
				(lAlphaRgn = ::CreateRectRgnIndirect (mShape->mTextRect))
			&&	(lAlphaBrush = CImageAlpha::GetAlphaBrush (0, 255))
			)
		{
			lROP2 = ::GetROP2 (pDC);
			::SetROP2 (pDC, R2_MERGEPEN);
			::FillRgn (pDC, lAlphaRgn, lAlphaBrush);
			::SetROP2 (pDC, lROP2);
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentBalloonWnd::DrawBalloonText"), DebugTimeElapsed);
#endif
}

DWORD CAgentBalloonWnd::ApplyFontLayout (HDC pDC)
{
	DWORD			lLayout;
	tS <LOGFONT>	lLogFont;

	lLayout = ::GetLayout (pDC);

	if	(
			(mFont.GetSafeHandle())
		&&	(GetObject (mFont, sizeof(lLogFont), &lLogFont))
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

LRESULT CAgentBalloonWnd::OnShow(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LRESULT	lResult = FALSE;

	if	(!mRedrawDisabled)
	{
		CRect	lWinRect;

#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)]   OnShow Balloon [%u %u]"), this, IsPaused(), IsWindowVisible(), mRedrawDisabled);
		}
#endif
#ifdef	_DEBUG_LAYOUT
		if	(LogIsActive (_DEBUG_LAYOUT))
		{
			GetWindowRect (&lWinRect);
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnShow [%s] [%u]"), RecursionIndent(), FormatRect(lWinRect), IsWindowVisible());
		}
#endif

		EnterRecursion ();
		if	(CalcWinRect (lWinRect, true))
		{
#ifdef	_DEBUG_SHOW_HIDE
			if	(LogIsActive (_DEBUG_SHOW_HIDE))
			{
				LogMessage (_DEBUG_SHOW_HIDE, _T("[%p(%u)]   OnShow [%s]"), this, IsPaused(), FormatRect(lWinRect));
			}
#endif
			ApplyLayout (lWinRect, true);
			lResult = TRUE;
		}
		ExitRecursion ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnWindowPosChanging (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT		lResult = DefWindowProc (uMsg, wParam, lParam);
	LPWINDOWPOS	lWindowPos = (LPWINDOWPOS) lParam;

#ifdef	_DEBUG_LAYOUT
	if	(LogIsActive (_DEBUG_LAYOUT))
	{
		CRect	lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnWindowPosChanging [%s] Actual [%s]"), RecursionIndent(), WindowPosStr(*lWindowPos), FormatRect(lWinRect));
	}
#endif
#ifdef	_DEBUG_DRAW
	if	(
			(mRedrawDisabled)
		||	(lWindowPos->flags & SWP_SHOWWINDOW)
		)
	{
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnWindowPosChanging [%s] Visible [%u %u]"), RecursionIndent(),  WindowPosStr(*lWindowPos), IsWindowVisible(), mRedrawDisabled);
	}
#endif
	
	if	(mRedrawDisabled)
	{
		lWindowPos->flags &= ~SWP_SHOWWINDOW;
		lWindowPos->flags &= ~SWP_NOREDRAW;
	}

	if	((lWindowPos->flags & SWP_NOSIZE) == 0)
	{
		if	(mShapeSize)
		{
			lWindowPos->cx = mShapeSize->cx;
			lWindowPos->cy = mShapeSize->cy;
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnWindowPosChanging [%s] ShapeSize [%s]"), RecursionIndent(), WindowPosStr(*lWindowPos), FormatSize(*mShapeSize));
#endif
		}
	}

	if	(
			(lWindowPos->flags & (SWP_SHOWWINDOW|SWP_FRAMECHANGED))
		&&	(!mApplyingRegion)
		&&	(!mApplyingLayout)
		)
	{
		ApplyRegion (true);
	}
	return lResult;
}

LRESULT CAgentBalloonWnd::OnWindowPosChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
#ifdef	_DEBUG_LAYOUT
	if	(LogIsActive (_DEBUG_LAYOUT))
	{
		LPWINDOWPOS	lWindowPos = (LPWINDOWPOS) lParam;
		CRect		lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnWindowPosChanged  [%s] Actual [%s]"), RecursionIndent(), WindowPosStr(*lWindowPos), FormatRect(lWinRect));
	}
#endif
#ifdef	_DEBUG_DRAW
	if	(
			(mRedrawDisabled)
		||	(((LPWINDOWPOS)lParam)->flags & SWP_SHOWWINDOW)
		)
	{
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnWindowPosChanged [%s] Visible [%u %u]"), RecursionIndent(),  WindowPosStr(*((LPWINDOWPOS)lParam)), IsWindowVisible(), mRedrawDisabled);
	}
	EnterRecursion ();
	LRESULT lResult = DefWindowProc (uMsg, wParam, lParam);
	ExitRecursion ();
	bHandled = TRUE;
#endif
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
#ifdef	_DEBUG_LAYOUT
	if	(LogIsActive (_DEBUG_LAYOUT))
	{
		CRect lWinRect;
		GetWindowRect (lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnSize [%d %d] [%s]"), RecursionIndent(), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), FormatRect(lWinRect));
	}
#endif
	mShapeBuffer.EndBuffer (true);
	return 0;
}

LRESULT CAgentBalloonWnd::OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnShowWindow [%d] Visible [%u %u]"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	mShapeBuffer.EndBuffer (true);
	return 0;
}

LRESULT CAgentBalloonWnd::OnSetRedraw (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnSetRedraw [%d] Visible [%u %u]"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	mShapeBuffer.EndBuffer (true);
	mRedrawDisabled = (wParam == 0);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnNcPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = (mRedrawDisabled || IsDrawingLayered ()) ? TRUE : FALSE;
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnNcPaint Visible [%u %u]"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
	return 0;
}

LRESULT CAgentBalloonWnd::OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = TRUE;
	return HTTRANSPARENT;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnTtmActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult;
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_ACTIVATE [%u]"), RecursionIndent(), wParam);
#endif
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmActivate [%d] Visible [%u %u]"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	mShapeBuffer.EndBuffer (true);
	if	(
			(wParam)
		&&	(mRedrawDisabled)
		)
	{
		SetRedraw (TRUE);
	}
	EnterRecursion ();
	lResult = DefWindowProc (uMsg, wParam, lParam);
	ExitRecursion ();
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmActivate [%d] Visible [%u %u] Done"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	bHandled = TRUE;
	return lResult;
}

LRESULT CAgentBalloonWnd::OnTtmTrackPosition (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult;
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_TRACKPOSITION [%d %d]"), RecursionIndent(), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmTrackPosition [%d] Visible [%u %u]"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	mShapeBuffer.EndBuffer (true);
	EnterRecursion ();
	lResult = DefWindowProc (uMsg, wParam, lParam);
	ExitRecursion ();
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmTrackPosition [%d] Visible [%u %u] Done"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	bHandled = TRUE;
	return lResult;
}

LRESULT CAgentBalloonWnd::OnTtmTrackActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult;
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_TRACKACTIVATE [%d]"), RecursionIndent(), wParam);
#endif
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmTrackActivate [%d] Visible [%u %u]"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	mShapeBuffer.EndBuffer (true);
	if	(
			(wParam)
		&&	(mRedrawDisabled)
		)
	{
		SetRedraw (TRUE);
	}
	if	(
			(wParam)
		&&	(!IsWindowVisible())
		&&	(IsDrawingLayered ())
		)
	{
		//
		//	Initially set to transparent until first WM_PAINT is received.
		//
		SetLayeredWindowAttributes (m_hWnd, 0, 0, LWA_ALPHA);
	}
	EnterRecursion ();
	lResult = DefWindowProc (uMsg, wParam, lParam);
	ExitRecursion ();
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmTrackActivate [%d] Visible [%u %u] Done"), RecursionIndent(), wParam, IsWindowVisible(), mRedrawDisabled);
#endif
	bHandled = TRUE;
	return lResult;
}

LRESULT CAgentBalloonWnd::OnTtmUpdateTipText (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_UPDATETIPTEXT [%s]"), RecursionIndent(), DebugStr(((TOOLINFO*)lParam)->lpszText));
#endif
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmUpdateTipText Visible [%u %u]"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
	if	(
			(!mRedrawDisabled)
		&&	(!IsDrawingLayered ())
		)
	{
		EnterRecursion ();
		lResult = DefWindowProc (uMsg, wParam, lParam);
		ExitRecursion ();
	}
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmUpdateTipText Visible [%u %u] Done"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
	bHandled = TRUE;
	return lResult;
}

LRESULT CAgentBalloonWnd::OnTtmUpdate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_UPDATE"), RecursionIndent());
#endif
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmUpdate Visible [%u %u]"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
	EnterRecursion ();
	if	(IsDrawingLayered ())
	{
		RedrawWindow ();
	}
	else
	{
		lResult = DefWindowProc (uMsg, wParam, lParam);
	}
	ExitRecursion ();
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("%sBalloon OnTtmUpdate Visible [%u %u] Done"), RecursionIndent(), IsWindowVisible(), mRedrawDisabled);
#endif
	bHandled = TRUE;
	return lResult;
}
