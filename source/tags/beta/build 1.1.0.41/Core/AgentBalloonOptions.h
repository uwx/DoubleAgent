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
#pragma once
#include "AgentFile.h"
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275 4150)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CAgentBalloonOptions
{
	DECLARE_DLL_OBJECT(CAgentBalloonOptions)
public:
	CAgentBalloonOptions ();
	CAgentBalloonOptions (DWORD pStyle, const CAgentFileBalloon & pFileBalloon, LANGID pLangID = 0);
	CAgentBalloonOptions (const CAgentBalloonOptions & pSource);
	CAgentBalloonOptions & operator= (const CAgentBalloonOptions & pSource);
	bool operator== (const CAgentBalloonOptions & pSource) const;
	bool operator!= (const CAgentBalloonOptions & pSource) const;

// Attributes
public:
	DWORD				mStyle;
	USHORT				mLines;
	USHORT				mPerLine;
	COLORREF			mBkColor;
	COLORREF			mFgColor;
	COLORREF			mBrColor;
	LOGFONT				mFont;

	static const USHORT	mMinLines;
	static const USHORT	mMaxLines;
	static const USHORT	mDefLines;
	static const USHORT	mMinPerLine;
	static const USHORT	mMaxPerLine;
	static const USHORT	mDefPerLine;

	bool IsEnabled () const;
	bool IsAutoSize () const;
	bool IsAutoPace () const;
	bool IsAutoHide () const;
	bool ClipPartialLines () const;

// Operations
public:
	static bool CopyBalloonFont (const CAgentFileBalloon & pFileBalloon, LOGFONT & pFont);
	static bool CopyBalloonFont (const LOGFONT & pFont, CAgentFileBalloon & pFileBalloon);
	static bool SetFontLangID (LOGFONT & pFont, LANGID pLangID);
	static bool GetActualFont (const LOGFONT & pFont, LOGFONT & pActualFont, bool pUpdateSize = true, bool pUpdateStyle = true);
	static bool FontEqual (HFONT pFont1, HFONT pFont2);
	static bool FontEqual (const LOGFONT & pFont1, const LOGFONT & pFont2);

	void LogOptions (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pPrefix = NULL);
	static void LogFont (UINT pLogLevel, const LOGFONT & pFont, LPCTSTR pTitle = NULL, LPCTSTR pPrefix = NULL);
};

/////////////////////////////////////////////////////////////////////////////

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
