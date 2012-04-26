/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#pragma once
#include "AgentFile.h"
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////

class CAgentBalloonOptions
{
	DECLARE_DLL_OBJECT_EX(CAgentBalloonOptions, _DACORE_IMPEXP)
public:
	_DACORE_IMPEXP CAgentBalloonOptions ();
	_DACORE_IMPEXP CAgentBalloonOptions (DWORD pStyle, const CAgentFileBalloon& pFileBalloon, LANGID pLangID = 0);
	_DACORE_IMPEXP CAgentBalloonOptions (const CAgentBalloonOptions& pSource);
	_DACORE_IMPEXP CAgentBalloonOptions& operator= (const CAgentBalloonOptions& pSource);
	_DACORE_IMPEXP bool operator== (const CAgentBalloonOptions& pSource) const;
	_DACORE_IMPEXP bool operator!= (const CAgentBalloonOptions& pSource) const;

// Attributes
public:
	DWORD		mStyle;
	USHORT		mLines;
	USHORT		mPerLine;
	COLORREF	mBkColor;
	COLORREF	mFgColor;
	COLORREF	mBrColor;
	LOGFONT		mFont;

	_DACORE_IMPEXP bool IsEnabled () const;
	_DACORE_IMPEXP bool IsAutoSize () const;
	_DACORE_IMPEXP bool IsAutoPace () const;
	_DACORE_IMPEXP bool IsAutoHide () const;
	_DACORE_IMPEXP bool AppendText () const;
	_DACORE_IMPEXP bool ClipPartialLines () const;

// Operations
public:
	_DACORE_IMPEXP static bool CopyBalloonFont (const CAgentFileBalloon& pFileBalloon, LOGFONT& pFont);
	_DACORE_IMPEXP static bool CopyBalloonFont (const LOGFONT& pFont, CAgentFileBalloon& pFileBalloon);
	_DACORE_IMPEXP static bool SetFontLangID (LOGFONT& pFont, LANGID pLangID);
	_DACORE_IMPEXP static bool GetActualFont (const LOGFONT& pFont, LOGFONT& pActualFont, bool pUpdateSize = true, bool pUpdateStyle = true);
	_DACORE_IMPEXP static bool FontEqual (HFONT pFont1, HFONT pFont2);
	_DACORE_IMPEXP static bool FontEqual (const LOGFONT& pFont1, const LOGFONT& pFont2);

	_DACORE_IMPEXP void LogOptions (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pPrefix = NULL);
	_DACORE_IMPEXP static void LogFont (UINT pLogLevel, const LOGFONT& pFont, LPCTSTR pTitle = NULL, LPCTSTR pPrefix = NULL);
};

/////////////////////////////////////////////////////////////////////////////
