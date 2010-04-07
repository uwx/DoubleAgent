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
#include "..\Server\ServerNotify.h"
#include "AgentBalloonWnd.h"
#include "AgentBalloonShape.h"
#include "AgentPopupWnd.h"
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
//#define	_DEBUG_DRAW			LogNormal|LogHighVolume|LogTimeMs
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

/////////////////////////////////////////////////////////////////////////////

const USHORT	CAgentBalloonWnd::mMinLines = 1;
const USHORT	CAgentBalloonWnd::mMaxLines = 128;
const USHORT	CAgentBalloonWnd::mDefLines = 2;

const USHORT	CAgentBalloonWnd::mMinPerLine = 8;
const USHORT	CAgentBalloonWnd::mMaxPerLine = 255;
const USHORT	CAgentBalloonWnd::mDefPerLine = 32;

UINT			CAgentBalloonWnd::mVoiceStartMsg = RegisterWindowMessage (_T("7781F0DC-B3CD-4ce9-8828-95C4FB56BBE2"));
UINT			CAgentBalloonWnd::mVoiceEndMsg = RegisterWindowMessage (_T("0BEA4D03-95C6-4c2f-A5BF-EFFE63D24D8F"));
UINT			CAgentBalloonWnd::mVoiceWordMsg = RegisterWindowMessage (_T("82C73827-F1B8-4223-824B-BC0953892D56"));

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
	mPacingWord (false),
	mApplyingLayout (false),
	mApplyingRegion (false),
	mDebugRecursionLevel (0),
	mOwnerWnd (NULL),
	mInNotify (0)
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

CAgentBalloonWnd * CAgentBalloonWnd::CreateInstance (long pCharID, CAtlPtrTypeArray <interface _IServerNotify> & pNotify)
{
	CComObject<CAgentBalloonWnd> *	lInstance = NULL;
	
	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<CAgentBalloonWnd>::CreateInstance (&lInstance))))
	{
		lInstance->mCharID = pCharID;
		lInstance->mNotify.Copy (pNotify);
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
	return (IsInNotify() == 0);
}

void CAgentBalloonWnd::OnFinalMessage (HWND)
{
	if	(HasFinalReleased ())
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
CString CAgentBalloonWnd::RecursionIndent () const {return CAtlString(_T(' '), (int)mDebugRecursionLevel*2);}
#else
UINT CAgentBalloonWnd::EnterRecursion () const {return 0;}
UINT CAgentBalloonWnd::ExitRecursion () const {return 0;}
CAtlString CAgentBalloonWnd::RecursionIndent () const {return CAtlString();}
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::SetOptions (const CAgentFileBalloon & pFileBalloon, IDaSvrBalloon * pCharBalloon, LANGID pLangID)
{
	bool						lRet = false;
	CAgentBalloonOptions *		lOldOptions = mPendingOptions.Ptr() ? mPendingOptions.Ptr() : &mOptions;
	tPtr <CAgentBalloonOptions>	lNewOptions = new CAgentBalloonOptions (*lOldOptions);

	lNewOptions->mLines = (USHORT)pFileBalloon.mLines;
	lNewOptions->mPerLine = (USHORT)pFileBalloon.mPerLine;
	lNewOptions->mBkColor = pFileBalloon.mBkColor;
	lNewOptions->mFgColor = pFileBalloon.mFgColor;
	lNewOptions->mBrColor = pFileBalloon.mBrColor;

	if	(pCharBalloon)
	{
		long	lLongVal;

		if	(pCharBalloon->GetStyle (&lLongVal) == S_OK)
		{
			lNewOptions->mStyle = lLongVal;
		}
		if	(pCharBalloon->GetNumLines (&lLongVal) == S_OK)
		{
			lNewOptions->mLines = (USHORT)lLongVal;
		}
		if	(pCharBalloon->GetNumCharsPerLine (&lLongVal) == S_OK)
		{
			lNewOptions->mPerLine = (USHORT)lLongVal;
		}
		if	(pCharBalloon->GetBackColor (&lLongVal) == S_OK)
		{
			lNewOptions->mBkColor = lLongVal;
		}
		if	(pCharBalloon->GetForeColor (&lLongVal) == S_OK)
		{
			lNewOptions->mFgColor = lLongVal;
		}
		if	(pCharBalloon->GetBorderColor (&lLongVal) == S_OK)
		{
			lNewOptions->mBrColor = lLongVal;
		}
	}

	CopyBalloonFont (pFileBalloon, lNewOptions->mFont);
	CopyBalloonFont (pCharBalloon, lNewOptions->mFont);
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
#ifdef	_DEBUG_OPTIONS
		if	(LogIsActive (_DEBUG_OPTIONS))
		{
			LogMessage (_DEBUG_OPTIONS, _T("[%p] ApplyOptions  Style [%8.8X] AutoSize [%u] AutoPace [%u] AutoHide [%u] PartialLines [%u]"), this, mOptions.mStyle, IsAutoSize(), IsAutoPace(), IsAutoHide(), !ClipPartialLines());
			LogMessage (_DEBUG_OPTIONS, _T("[%p]               Lines [%hu] PerLine [%hu] BkColor [%8.8X] FgColor [%8.8X] BrColor [%8.8X]"), this, mOptions.mLines, mOptions.mPerLine, mOptions.mBkColor, mOptions.mFgColor, mOptions.mBrColor);
		}
#endif
		SendMessage (TTM_SETTIPBKCOLOR, (WPARAM)mOptions.mBkColor);
		SendMessage (TTM_SETTIPTEXTCOLOR, (WPARAM)mOptions.mFgColor);

		mFont.Close ();
		if	(
				(mOptions.mFont.lfHeight)
			&&	(mFont = CreateFontIndirect (&mOptions.mFont))
			)
		{
			SetFont (mFont);
		}
		else
		{
			SetFont (NULL);
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
			(pFileBalloon.mFontName.Ptr())
		&&	(pFileBalloon.mFontHeight != 0)
		)
	{
		_tcscpy (pFont.lfFaceName, CAtlString ((BSTR)pFileBalloon.mFontName));
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
		pFileBalloon.mFontName = CAtlString (pFont.lfFaceName).AllocSysString ();
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
			_tcscpy (pFont.lfFaceName, CAtlString ((BSTR)lBstrVal));
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::Create (CWindow * pOwnerWnd)
{
	bool	lRet = false;

	if	(
			(pOwnerWnd)
		&&	(pOwnerWnd->IsWindow ())
		&&	(SubclassWindow (::CreateWindowEx (0, TOOLTIPS_CLASS, _T(""), WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE, 0,0,0,0, pOwnerWnd->m_hWnd, 0, _AtlBaseModule.GetModuleInstance(), NULL)))
		)
	{
		mOwnerWnd = pOwnerWnd;
		ModifyStyle (WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx (0, WS_EX_TOPMOST);

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

bool CAgentBalloonWnd::Attach (long pCharID, _IServerNotify * pNotify, bool pSetActiveCharID)
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

bool CAgentBalloonWnd::Detach (long pCharID, _IServerNotify * pNotify)
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

bool CAgentBalloonWnd::IsDrawingLayered () const
{
	//if	(
	//		(IsWindowsVista_AtLeast ())
	//	&&	(m_hWnd)
	//	&&	(GetExStyle () & WS_EX_LAYERED)
	//	)
	//{
	//	return true;
	//}
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

	if	(IsWindow ())
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

	if	(IsWindow ())
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
		StopAutoHide ();
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

	if	(IsWindow ())
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
//
//	On Vista or later, we recreate the window.  This stops the previous window image from being
//	flashed on the screen before the new window image is displayed.
//
#ifdef	_DEBUG_SHOW_HIDE
			if	(LogIsActive (_DEBUG_SHOW_HIDE))
			{
				LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] Recreate Balloon"), this);
			}
#endif
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon Recreate"), RecursionIndent());
#endif

			EnterRecursion ();
			DestroyWindow ();
			Create (mOwnerWnd);
			ExitRecursion ();

#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon Recreate Done"), RecursionIndent());
#endif
		}
		else
#endif
		{
			ApplyOptions ();
		}

		if	(IsWindow ())
		{
			CRect	lWinRect;

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
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_TRACKPOSITION [%s]"), RecursionIndent(), FormatRect(lWinRect));
#endif
				EnterRecursion ();
				SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (lWinRect.left, lWinRect.top));
				ExitRecursion ();

#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_UPDATETIPTEXT [%s]"), RecursionIndent(), DebugStr(mToolInfo.lpszText));
#endif
				EnterRecursion ();
				SendMessage (TTM_UPDATETIPTEXT, 0, (LPARAM)&mToolInfo);
				ExitRecursion ();

#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_UPDATE"), RecursionIndent());
#endif
				EnterRecursion ();
				SendMessage (TTM_UPDATE);
				ExitRecursion ();

				ApplyLayout (lWinRect);

#ifdef	_DEBUG_SHOW_HIDE
				if	(LogIsActive (_DEBUG_SHOW_HIDE))
				{
					LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] Show Balloon [%u %u] [%s]"), this, pForSpeech, pTextChanged, DebugStr(mText.GetFullText()));
				}
#endif
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_ACTIVATE"), RecursionIndent());
#endif
				EnterRecursion ();
				SendMessage (TTM_ACTIVATE, TRUE);
				ExitRecursion ();

#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_TRACKACTIVATE"), RecursionIndent());
#endif
				EnterRecursion ();
				SendMessage (TTM_TRACKACTIVATE, TRUE, (LPARAM)&mToolInfo);
				ExitRecursion ();
			}
			else
			if	(IsWindowVisible())
			{
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_UPDATE"), RecursionIndent());
#endif
				EnterRecursion ();
				SendMessage (TTM_UPDATE);
				ExitRecursion ();
			}
			if	(IsWindowVisible())
			{
#ifdef	_DEBUG_LAYOUT
				LogMessage (_DEBUG_LAYOUT, _T("%sBalloon HWND_TOPMOST"), RecursionIndent());
#endif
				EnterRecursion ();
				SetWindowPos (HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
				ExitRecursion ();
				lRet = true;
			}

			ExitRecursion ();
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon ShowBalloon Done"), RecursionIndent());
#endif
		}
	}
	return lRet;
}

void CAgentBalloonWnd::ShowedBalloon (bool pWasVisible)
{
	if	(
			(!pWasVisible)
		&&	(IsWindow ())
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR				lNotifyNdx;
			_IServerNotify *	lNotify;

			for	(lNotifyNdx = (INT_PTR)mNotify.GetCount()-1; lNotify = mNotify (lNotifyNdx); lNotifyNdx--)
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

	if	(IsWindow ())
	{
#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p] Hide Balloon [%u %u]"), this, pFast, IsWindowVisible());
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
			INT_PTR				lNotifyNdx;
			_IServerNotify *	lNotify;

			for	(lNotifyNdx = (INT_PTR)mNotify.GetCount()-1; lNotify = mNotify (lNotifyNdx); lNotifyNdx--)
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
			(IsWindow ())
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonWnd::CalcLayoutRects (CRect & pTextRect, CRect & pOwnerRect, CRect & pBounds)
{
	bool						lRet = false;
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
				(IsAutoSize ())
			||	(mOptions.mLines < mMinLines)
			)
		{
			pTextRect = CRect (CPoint (0,0), mText.CalcTextSize (GetFont(), min(max(mOptions.mPerLine,mMinPerLine),mMaxPerLine)));
		}
		else
		{
			pTextRect = CRect (CPoint (0,0), mText.CalcTextSize (GetFont(), min(max(mOptions.mPerLine,mMinPerLine),mMaxPerLine), min(max(mOptions.mLines,mMinLines),mMaxLines)));
		}
		pTextRect.OffsetRect (pOwnerRect.CenterPoint().x - pTextRect.CenterPoint().x, pOwnerRect.top - pTextRect.bottom);

#ifdef	_DEBUG_AUTO_SIZE
		if	(LogIsActive (_DEBUG_AUTO_SIZE))
		{
			LogMessage (_DEBUG_AUTO_SIZE, _T("[%p]   CalcLayout [%s] for [%s]"), this, FormatSize(pTextRect.Size()), DebugStr(mText.GetFullText()));
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

	if	(!mApplyingRegion)
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
			(IsWindow ())
		&&	(mAutoPaceTimer)
		)
	{
#ifdef	_DEBUG_AUTO_PACE
		if	(LogIsActive (_DEBUG_AUTO_PACE))
		{
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPace [%u] stopped"), this, mAutoPaceTimer);
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
	bool				lRet = false;
	CAgentPopupWnd *	lOwner;

	if	(IsWindow ())
	{
		if	(
				(lOwner = dynamic_cast <CAgentPopupWnd *> ((CAgentWnd*)mOwnerWnd))
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
			(IsWindow ())
		&&	(mAutoScrollTimer)
		)
	{
#ifdef	_DEBUG_AUTO_SCROLL
		if	(LogIsActive (_DEBUG_AUTO_SCROLL))
		{
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] AutoScroll [%u] stopped"), this, mAutoScrollTimer);
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
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd OnVoiceStart [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize ());
	}
#endif
	PostMessage (mVoiceStartMsg, pCharID);
}

void CAgentBalloonWnd::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd OnVoiceEnd [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize ());
	}
#endif
	PostMessage (mVoiceEndMsg, pCharID);
}

void CAgentBalloonWnd::OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd   OnVoiceWord [%u (%d)] [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, pWordPos, pWordLength, DebugStr(mText.GetSpeechText().Mid (pWordPos, pWordLength)), IsAutoPace (), IsAutoSize ());
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
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd OnVoiceStartMsg [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize ());
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
			ShowedVoiceWord (false);
		}
	}
	else
	{
		mPacingSpeech = false;
	}
	return 0;
}

LRESULT CAgentBalloonWnd::OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd OnVoiceEndMsg [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, DebugStr(mText.GetSpeechText()), IsAutoPace (), IsAutoSize ());
	}
#endif
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
				ShowedVoiceWord (false);
			}
			StopAutoPace ();
		}
		else
		{
			if	(mText.DisplayNextWord (true))
			{
				ShowedVoiceWord (false);
			}
			StartAutoPace ();
		}
	}
	else
	{
		StopAutoPace ();
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
		LogMessage (_DEBUG_SPEECH, _T("[%p] CAgentBalloonWnd   OnVoiceWordMsg [%u (%d)] [%s] IsAutoPace [%u] IsAutoSize [%u]"), this, lWordPos, lWordLength, DebugStr(mText.GetSpeechText().Mid (lWordPos, lWordLength)), IsAutoPace (), IsAutoSize ());
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
			LogMessage (_DEBUG_AUTO_PACE, _T("[%p] AutoPaceSpeech word [%d] of [%d] [%s]"), this, mText.GetWordDisplayed(), mText.GetWordCount(), DebugStr(mText.GetDisplayWord(mText.GetWordDisplayed())));
		}
#endif
		ShowedVoiceWord (true);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonWnd::ShowedVoiceWord (bool pFastRefresh)
{
	if	(IsWindowVisible ())
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
				&&	(lDC = GetDC ())
				&&	(
						(mDrawBuffer.GetImageSize () == lClientRect.Size ())
					||	(mDrawBuffer.CreateBuffer (lClientRect.Size(), true, true))
					)
				&&	(mDrawBuffer.StartBuffer ())
				)
			{
				if	(
						(mShapeBuffer.GetImageSize () == lClientRect.Size ())
					&&	(mShapeBuffer.StartBuffer ())
					)
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

				::BitBlt (lDC, mShape->mTextRect.left, mShape->mTextRect.top, mShape->mTextRect.Width(), mShape->mTextRect.Height(), mDrawBuffer.GetDC(), mShape->mTextRect.left, mShape->mTextRect.top, SRCCOPY);
				lTextDrawn = true;
			}

			mDrawBuffer.EndBuffer ();
			if	(lDC)
			{
				ReleaseDC (lDC);
			}
		}

		if	(!lTextDrawn)
		{
#ifdef	_DEBUG_LAYOUT
			LogMessage (_DEBUG_LAYOUT, _T("%sBalloon TTM_UPDATE"), RecursionIndent());
#endif
			EnterRecursion ();
			mPacingWord = true;
			SendMessage (TTM_UPDATE);
			mPacingWord = false;
			ExitRecursion ();
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
	LRESULT	lResult = DefWindowProc ();

	if	(
			(mAutoPaceTimer)
		&&	(wParam == mAutoPaceTimer)
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
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] StopScroll [%d] no scrolling"), this, mAutoScrollTimer);
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
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] StopScroll [%d] at [%+d] of [%+d - %+d]"), this, mAutoScrollTimer, mText.GetScrollPos(), mText.GetScrollMin(), mText.GetScrollMax());
			}
#endif
			StopAutoScroll ();
		}
	}

	if	(
			(mAutoHideTimer)
		&&	(wParam == mAutoHideTimer)
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
			&&	(lOwner = dynamic_cast <CAgentPopupWnd *> ((CAgentWnd*)mOwnerWnd))
			&&	(!lOwner->KeepBalloonVisible (this))
			)
		{
			HideBalloon ();
		}
#ifdef	_DEBUG_AUTO_HIDE
		else
		if	(LogIsActive (_DEBUG_AUTO_HIDE))
		{
			LogMessage (_DEBUG_AUTO_HIDE, _T("[%p] AutoHide [%u] skipped"), this, mAutoHideTimer);
		}
#endif
	}
	return lResult;
}

LRESULT CAgentBalloonWnd::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

	bHandled = FALSE;
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

	if	(lPaintDC = BeginPaint (&lPaintStruct))
	{
#ifdef	_DEBUG_DRAW
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnPaint [%p] [%p] Visible [%u]"), RecursionIndent(), (HDC)lPaintDC, m_hWnd, IsWindowVisible());
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
				if	(!::UpdateLayeredWindow (m_hWnd, NULL, &lWinRect.TopLeft(), &lWinRect.Size(), mDrawBuffer.GetDC(), &lClientRect.TopLeft(), 0, &lBlendFunc, ULW_ALPHA))
				{
					ModifyStyleEx (WS_EX_LAYERED, 0);
					ModifyStyleEx (0, WS_EX_LAYERED);
					::UpdateLayeredWindow (m_hWnd, NULL, &lWinRect.TopLeft(), &lWinRect.Size(), mDrawBuffer.GetDC(), &lClientRect.TopLeft(), 0, &lBlendFunc, ULW_ALPHA);
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
	if	(IsDrawingLayered ())
	{
		return TRUE;
	}
	if	(IsWindowsVista_AtLeast ())
	{
		return TRUE;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CAgentBalloonWnd::OnPrint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CAgentBalloonWnd::OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if	(lParam & PRF_CLIENT)
	{
		CRect	lClientRect;

		GetClientRect (&lClientRect);
#ifdef	_DEBUG_DRAW
		LogMessage (_DEBUG_DRAW, _T("%sBalloon OnPrintClient [%p] [%p] Visible [%u]"), RecursionIndent(), (HDC)wParam, ::WindowFromDC((HDC)wParam), IsWindowVisible());
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
	LogMessage (_DEBUG_DRAW, _T("%sBalloon %s [%d %d %d %d (%d %d)] DC [%p] DCWin [%p] Win [%p] Visible [%u]"), RecursionIndent(), CustomDrawStage(lNotify), lNotify->rc.left, lNotify->rc.top, lNotify->rc.right, lNotify->rc.bottom, lNotify->rc.right-lNotify->rc.left, lNotify->rc.bottom-lNotify->rc.top, lNotify->hdc, ::WindowFromDC(lNotify->hdc), m_hWnd, IsWindowVisible());
#endif

	if	(lNotify->dwDrawStage == CDDS_PREPAINT)
	{
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
		LogMessage (_DEBUG_DRAW, _T("%sBalloon Draw [%s] In [%p] [%s] Margin [%s] Visible [%u]"), RecursionIndent(), FormatRect(pDrawRect), ::WindowFromDC(pDC), FormatSize(lWinRect.Size()), FormatMargin(lMargin), IsWindowVisible());
	}
#endif

	if	(mShapeBuffer.GetImageSize () == pDrawRect.Size ())
	{
		mShapeBuffer.StartBuffer ();
	}
	else
	{
		bool	lScaleBuffer = IsDrawingLayered ();
		int		lBufferScale = lScaleBuffer ? 3 : 1;
		CSize	lBufferSize (pDrawRect.Width()*lBufferScale, pDrawRect.Height()*lBufferScale);

		if	(mShapeBuffer.CreateBuffer (lBufferSize, true, true))
		{
			mShape->Draw (*mShapeBuffer.mDC, mOptions.mBkColor, mOptions.mBrColor, lBufferScale);
			if	(lScaleBuffer)
			{
				mShapeBuffer.UnscaleBuffer (lBufferScale);
			}

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
				LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] Clip [%d] Text [%d (%d)] Lines [%d]"), this, mShape->mTextRect.Height(), mText.GetSize().cy, mText.mBounds.Height(), mText.GetLineCount());
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
			LogMessage (_DEBUG_AUTO_SCROLL, _T("[%p] Clip [%d] Text [%d (%d)] Scroll [%d (%d)]"), this, mShape->mTextRect.Height(), mText.GetSize().cy, mText.mBounds.Height(), mText.GetScrollPos(), mShape->mTextRect.top - mText.mBounds.top);
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
	if	(!mPacingWord)
	{
		CRect	lWinRect;

#ifdef	_DEBUG_SHOW_HIDE
		if	(LogIsActive (_DEBUG_SHOW_HIDE))
		{
			LogMessage (_DEBUG_SHOW_HIDE, _T("[%p]   OnShow Balloon"), this);
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
				LogMessage (_DEBUG_SHOW_HIDE, _T("[%p]   OnShow [%s]"), this, FormatRect(lWinRect));
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
	LRESULT		lResult = DefWindowProc ();
	LPWINDOWPOS	lWindowPos = (LPWINDOWPOS) lParam;

#ifdef	_DEBUG_LAYOUT
	if	(LogIsActive (_DEBUG_LAYOUT))
	{
		CRect	lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnWindowPosChanging [%s] Actual [%s]"), RecursionIndent(), WindowPosStr(*lWindowPos), FormatRect(lWinRect));
	}
#endif

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
#ifdef	_DEBUG_LAYOUT
	if	(LogIsActive (_DEBUG_LAYOUT))
	{
		LPWINDOWPOS	lWindowPos = (LPWINDOWPOS) lParam;
		CRect		lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnWindowPosChanged  [%s] Actual [%s]"), RecursionIndent(), WindowPosStr(*lWindowPos), FormatRect(lWinRect));
	}
#endif
	bHandled = FALSE;
	return 0;
}

LRESULT CAgentBalloonWnd::OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_LAYOUT
	if	(LogIsActive (_DEBUG_LAYOUT))
	{
		CRect lWinRect;
		GetWindowRect (lWinRect);
		LogMessage (_DEBUG_LAYOUT, _T("%sBalloon OnSize [%d %d] [%s]"), RecursionIndent(), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), FormatRect(lWinRect));
	}
#endif
	bHandled = FALSE;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentBalloonWnd::OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return HTTRANSPARENT;
}
