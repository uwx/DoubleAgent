/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "TextWrap.h"

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
/////////////////////////////////////////////////////////////////////////////

class CAgentText
{
public:
	CAgentText (UINT pSapiVersion = 5);
	CAgentText (const CAtlStringArray& pWords, UINT pSapiVersion = 5);
	CAgentText (const CAgentText& pText, UINT pSapiVersion = 5);
	virtual ~CAgentText ();

// Attributes
public:
	UINT GetSapiVersion () const;
	bool SetSapiVersion (UINT pSapiVersion);

	const CAtlString& GetFullText () const {return mText;}
	CAtlString GetSpeechText () const;

// Operations
public:
	CAgentText& operator= (const CAtlStringArray& pWords);
	CAgentText& operator+= (const CAtlStringArray& pWords);
	CAgentText& operator= (const CAgentText& pText);
	CAgentText& operator+= (const CAgentText& pText);

// Implementation
public:
	void Append (const CAgentText& pText, bool pAppendSpeech = false);
	void Append (const CAtlStringArray& pTextWords, const CAtlStringArray& pSpeechWords, bool pAppendSpeech = false);

protected:
	UINT			mSapiVersion;
	CAtlString		mText;
	CAtlStringArray	mTextWords;
	CAtlStringArray	mSpeechWords;
};

/////////////////////////////////////////////////////////////////////////////

class CAgentTextDraw : public CTextWrap, public CAgentText
{
public:
	CAgentTextDraw (UINT pSapiVersion = 5);
	CAgentTextDraw (const CAgentText& pText, UINT pSapiVersion = 5);
	CAgentTextDraw (const CAgentTextDraw& pText, UINT pSapiVersion = 5);
	virtual ~CAgentTextDraw ();

// Attributes
public:
	CAtlString GetDisplayText (INT_PTR pLookAhead = 0) const;
	CAtlString GetDisplayWord (INT_PTR pWordNdx) const;

	INT_PTR GetWordCount () const {return mTextWords.GetCount();}
	INT_PTR GetWordDisplayed () const {return mWordDisplayed;}
	INT_PTR GetLookAhead () const {return min (GetWordCount() - GetWordDisplayed() - 1, mDefaultLookAhead);}

	bool CanPace () const;
	bool CanScroll (const CRect& pTextBounds) const;
	long GetScrollPos () const {return mScrollPos;}
	long GetScrollInc () const {return mScrollInc;}
	long GetScrollMin () const {return mScrollMin;}
	long GetScrollMax () const {return mScrollMax;}

	static const DWORD		mDefaultAutoPaceTime;
	static const INT_PTR	mDefaultLookAhead;

// Operations
public:
	CAgentTextDraw& operator= (const CAgentText& pText);
	CAgentTextDraw& operator+= (const CAgentText& pText);
	CAgentTextDraw& operator= (const CAgentTextDraw& pText);
	CAgentTextDraw& operator+= (const CAgentTextDraw& pText);

	bool DisplayFirstWord (bool pForSpeech = false);
	bool DisplayNextWord (bool pForSpeech = false);
	bool DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech = true);
	bool DisplayThisWord (INT_PTR pWordNdx, bool pForSpeech = true);
	bool DisplayAllWords (bool pForSpeech = false);

	CSize CalcTextSize (HFONT pFont, USHORT pPerLine, USHORT pLines);
	CSize CalcTextSize (HFONT pFont, USHORT pPerLine);

	DWORD CalcScroll (const CRect& pTextBounds, long& pScrollInc, long& pScrollMin, long& pScrollMax, bool pClipLines = false, DWORD pMaxLineTime = 0) const;
	DWORD InitScroll (const CRect& pTextBounds, bool pForceReinit = false, bool pClipLines = false, DWORD pMaxLineTime = 0);
	bool ApplyScroll (const CRect& pTextBounds, CRect* pClipRect = NULL);
	bool Scroll ();

// Overrides

// Implementation
public:
	void ResetState (bool pFullReset);
protected:
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter);

protected:
	INT_PTR									mWordDisplayed;
	long									mScrollPos;
	long									mScrollInc;
	long									mScrollMin;
	long									mScrollMax;
private:
	mutable INT_PTR							mTextCacheLimit;
	mutable INT_PTR							mTextCacheStart;
	mutable CAtlOwnPtrArray <CAtlString>	mTextCache;
};

/////////////////////////////////////////////////////////////////////////////
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////
