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
#include "AgentBalloonOptions.h"
#include "Registry.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_FONT	(GetProfileDebugInt(_T("DebugBalloonFont"),LogVerbose,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

const USHORT	CAgentBalloonOptions::mMinLines = 1;
const USHORT	CAgentBalloonOptions::mMaxLines = 128;
const USHORT	CAgentBalloonOptions::mDefLines = 2;

const USHORT	CAgentBalloonOptions::mMinPerLine = 8;
const USHORT	CAgentBalloonOptions::mMaxPerLine = 255;
const USHORT	CAgentBalloonOptions::mDefPerLine = 32;

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentBalloonOptions)

CAgentBalloonOptions::CAgentBalloonOptions ()
{
	mStyle = BalloonStyle_AutoPace|BalloonStyle_AutoHide;
	mLines = mDefLines;
	mPerLine = mDefPerLine;
	mBkColor = GetSysColor (COLOR_INFOBK);
	mFgColor = GetSysColor (COLOR_INFOTEXT);
	mBrColor = GetSysColor (COLOR_INFOTEXT);
	memset (&mFont, 0, sizeof(LOGFONT));
}

CAgentBalloonOptions::CAgentBalloonOptions (DWORD pStyle, const CAgentFileBalloon & pFileBalloon, LANGID pLangID)
{
	mStyle = pStyle;
	mLines = (USHORT)pFileBalloon.mLines;
	mPerLine = (USHORT)pFileBalloon.mPerLine;
	mBkColor = pFileBalloon.mBkColor;
	mFgColor = pFileBalloon.mFgColor;
	mBrColor = pFileBalloon.mBrColor;

	memset (&mFont, 0, sizeof(LOGFONT));
	CopyBalloonFont (pFileBalloon, mFont);
	if	(pLangID)
	{
		SetFontLangID (mFont, pLangID);
	}
	GetActualFont (mFont, mFont);
}

CAgentBalloonOptions::CAgentBalloonOptions (const CAgentBalloonOptions & pSource)
{
	operator= (pSource);
}

/////////////////////////////////////////////////////////////////////////////

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

bool CAgentBalloonOptions::IsEnabled () const
{
	return ((mStyle & BalloonStyle_Enabled) != 0);
}

bool CAgentBalloonOptions::IsAutoSize () const
{
	return ((mStyle & BalloonStyle_SizeToText) != 0);
}

bool CAgentBalloonOptions::IsAutoPace () const
{
	return ((mStyle & BalloonStyle_AutoPace) != 0);
}

bool CAgentBalloonOptions::IsAutoHide () const
{
	return ((mStyle & BalloonStyle_AutoHide) != 0);
}

bool CAgentBalloonOptions::ClipPartialLines () const
{
	return (((mStyle & BalloonStyle_SizeToText) == 0) && ((mStyle & BalloonStyle_ShowPartialLines) == 0));
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonOptions::CopyBalloonFont (const CAgentFileBalloon & pFileBalloon, LOGFONT & pFont)
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

bool CAgentBalloonOptions::CopyBalloonFont (const LOGFONT & pFont, CAgentFileBalloon & pFileBalloon)
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

bool CAgentBalloonOptions::SetFontLangID (LOGFONT & pFont, LANGID pLangID)
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

bool CAgentBalloonOptions::GetActualFont (const LOGFONT & pFont, LOGFONT & pActualFont, bool pUpdateSize, bool pUpdateStyle)
{
	CMemDCHandle	lDC;
	CFontHandle		lFont;
	LOGFONT			lFontSpec;

	memcpy (&lFontSpec, &pFont, sizeof(LOGFONT));
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
				if	(lFontSpec.lfHeight < 0)
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
#ifdef	_DEBUG_FONT
		if	(LogIsActive (_DEBUG_FONT))
		{
			LogFont	(_DEBUG_FONT, lFontSpec, _T("  Font  "));
			LogFont	(_DEBUG_FONT, pActualFont, _T("  Actual"));
		}
#endif
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonOptions::FontEqual (HFONT pFont1, HFONT pFont2)
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

bool CAgentBalloonOptions::FontEqual (const LOGFONT & pFont1, const LOGFONT & pFont2)
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

void CAgentBalloonOptions::LogOptions (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pPrefix)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle (pTitle);
			CAtlString	lPrefix (pPrefix);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Balloon Options");
			}
			if	(!lPrefix.IsEmpty ())
			{
				lPrefix += _T(' ');
			}
			LogMessage (pLogLevel, _T("%s%s [%8.8X] AutoSize [%u] AutoPace [%u] AutoHide [%u] PartialLines [%u]"), lPrefix, lTitle, mStyle, IsAutoSize(), IsAutoPace(), IsAutoHide(), !ClipPartialLines());
			LogMessage (pLogLevel, _T("%s%*c Lines [%hu] PerLine [%hu] BkColor [%8.8X] FgColor [%8.8X] BrColor [%8.8X]"), lPrefix, lTitle.GetLength(), _T(' '), mLines, mPerLine, mBkColor, mFgColor, mBrColor);
		}
		catch AnyExceptionSilent
	}
}

void CAgentBalloonOptions::LogFont (UINT pLogLevel, const LOGFONT & pFont, LPCTSTR pTitle, LPCTSTR pPrefix)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle (pTitle);
			CAtlString	lPrefix (pPrefix);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Balloon Font");
			}
			if	(!lPrefix.IsEmpty ())
			{
				lPrefix += _T(' ');
			}

			LogMessage (pLogLevel, _T("%s%s [%s] [%d %d] B-I-U-S [%u-%u-%u-%u] CS-PF-Q [%u-%u-%u]"), lPrefix, lTitle, pFont.lfFaceName, pFont.lfHeight, pFont.lfWidth, pFont.lfWeight, pFont.lfItalic, pFont.lfUnderline, pFont.lfStrikeOut, pFont.lfCharSet, pFont.lfPitchAndFamily, pFont.lfQuality);
		}
		catch AnyExceptionSilent
	}
}
