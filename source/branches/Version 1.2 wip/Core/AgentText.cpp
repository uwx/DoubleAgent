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
#include "StdAfx.h"
#pragma unmanaged
/////////////////////////////////////////////////////////////////////////////
#include "AgentText.h"
#include "StringArrayEx.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_TEXT		(GetProfileDebugInt(_T("DebugSpeechText"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
//#define	_DEBUG_CACHE	LogDebugFast
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentText::CAgentText (UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
}

CAgentText::CAgentText (const CAtlStringArray& pWords, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
	operator= (pWords);
}

CAgentText::CAgentText (const CAgentText& pText, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
	operator= (pText);
}

CAgentText::~CAgentText ()
{
}

/////////////////////////////////////////////////////////////////////////////

CAgentText& CAgentText::operator= (const CAtlStringArray& pWords)
{
	mTextWords.RemoveAll ();
	operator+= (pWords);
	return *this;
}

CAgentText& CAgentText::operator+= (const CAtlStringArray& pWords)
{
	Append (pWords, pWords);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentText& CAgentText::operator= (const CAgentText& pText)
{
	mSapiVersion = pText.mSapiVersion;
	mText = pText.mText;
	mTextWords.Copy (pText.mTextWords);
	mSpeechWords.Copy (pText.mSpeechWords);
	return *this;
}

CAgentText& CAgentText::operator+= (const CAgentText& pText)
{
	Append (pText.mTextWords, pText.mSpeechWords);
	return *this;
}

void CAgentText::Append (const CAgentText& pText, bool pAppendSpeech)
{
	Append (pText.mTextWords, pText.mSpeechWords, pAppendSpeech);
}

void CAgentText::Append (const CAtlStringArray& pTextWords, const CAtlStringArray& pSpeechWords, bool pAppendSpeech)
{
	if	(
			(mTextWords.GetCount() > 0)
		&&	(pTextWords.GetCount() > 0)
		)
	{
		mTextWords [(INT_PTR)mTextWords.GetCount()-1].TrimRight();
		mTextWords.Add (_T("\n"));
		mTextWords.Append (pTextWords);
	}
	else
	{
		mTextWords.Copy (pTextWords);
	}

	mText = JoinStringArray (mTextWords, _T(""));

	if	(pSpeechWords.GetCount() > 0)
	{
		if	(
				(pAppendSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mSpeechWords.Add (_T("\n"));
			mSpeechWords.Append (pSpeechWords);
		}
		else
		{
			mSpeechWords.Copy (pSpeechWords);
		}
	}
	else
	if	(pTextWords.GetCount() > 0)
	{
		if	(
				(pAppendSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mSpeechWords.Add (_T("\n"));
			mSpeechWords.Append (pTextWords);
		}
		else
		{
			mSpeechWords.Copy (pTextWords);
		}
	}

#ifdef	_DEBUG_TEXT
	if	(!mText.IsEmpty())
	{
		LogMessage (_DEBUG_TEXT, _T("Text   [%s]"), DebugStr(mText));
	}
	if	(mTextWords.GetCount() > 0)
	{
		LogMessage (_DEBUG_TEXT, _T("Words  [%s]"), DebugStr(JoinStringArray(mTextWords, _T(""), true)));
	}
	if	(mSpeechWords.GetCount() > 0)
	{
		LogMessage (_DEBUG_TEXT, _T("Speech [%s]"), DebugStr(JoinStringArray(mSpeechWords, _T(""), true)));
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

UINT CAgentText::GetSapiVersion () const
{
	return mSapiVersion;
}

bool CAgentText::SetSapiVersion (UINT pSapiVersion)
{
	if	(pSapiVersion == 0)
	{
		pSapiVersion = 5;
	}
	if	(
			(pSapiVersion == 4)
		||	(pSapiVersion == 5)
		)
	{
		mSapiVersion = pSapiVersion;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentText::GetSpeechText () const
{
	if	(mSpeechWords.GetCount() > 0)
	{
		return JoinStringArray (mSpeechWords, _T(""));
	}
	else
	{
		return mText;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

const DWORD		CAgentTextDraw::mDefaultAutoPaceTime = 300;
const INT_PTR	CAgentTextDraw::mDefaultLookAhead = 2;

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw::CAgentTextDraw (UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::CAgentTextDraw (const CAgentText& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::CAgentTextDraw (const CAgentTextDraw& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::~CAgentTextDraw ()
{
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw& CAgentTextDraw::operator= (const CAgentText& pText)
{
	CAgentText::operator= (pText);
	ResetState (true);
	DisplayFirstWord ();
	return *this;
}

CAgentTextDraw& CAgentTextDraw::operator+= (const CAgentText& pText)
{
	CAgentText::operator+= (pText);
	ResetState (false);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw& CAgentTextDraw::operator= (const CAgentTextDraw& pText)
{
	CTextWrap::operator= (pText);
	CAgentText::operator= (pText);
	ResetState (true);

	mWordDisplayed = pText.mWordDisplayed;
	mScrollPos = pText.mScrollPos;
	mScrollInc = pText.mScrollInc;
	mScrollMin = pText.mScrollMin;
	mScrollMax = pText.mScrollMax;
	return *this;
}

CAgentTextDraw& CAgentTextDraw::operator+= (const CAgentTextDraw& pText)
{
	CAgentText::operator+= (pText);
	ResetState (false);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextDraw::ResetState (bool pFullReset)
{
	mBounds.SetRectEmpty ();

	mTextCacheLimit = 5;
	mTextCacheStart = 0;
	mTextCache.DeleteAll ();

	if	(pFullReset)
	{
		mWordDisplayed = -1;
		mScrollPos = LONG_MIN;
		mScrollInc = 0;
		mScrollMin = 0;
		mScrollMax = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentTextDraw::GetDisplayText (INT_PTR pLookAhead) const
{
	CAtlString	lDisplayText;
//
//	The caching is primitive but does the job.  It assumes that display text will be accessed
//	with constantly growing length.
//
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed	< (INT_PTR)mTextWords.GetCount())
		)
	{
		if	(mWordDisplayed	>= 0)
		{
			INT_PTR		lWordDisplayed = min (mWordDisplayed + pLookAhead, (INT_PTR)mTextWords.GetCount()-1);
			INT_PTR		lTextCacheNdx;
			CAtlString*	lTextCache;

			while	(
						(mTextCacheStart < mWordDisplayed)
					&&	(mTextCache.GetCount() > 0)
					)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("  DeleteCached [%p] CacheStart [%d] CacheSize [%d]"), mTextCache (0), mTextCacheStart+1, mTextCache.GetCount()-1);
#endif
				mTextCache.DeleteAt (0);
				mTextCacheStart++;
			}

			mTextCacheStart = max (mTextCacheStart, mWordDisplayed);
			lTextCacheNdx = lWordDisplayed - mTextCacheStart;
#ifdef	_DEBUG_CACHE
			LogMessage (_DEBUG_CACHE, _T("GetDisplayText [%d] CacheStart [%d] CacheNdx [%d] CacheSize [%d]"), lWordDisplayed, mTextCacheStart, lTextCacheNdx, mTextCache.GetCount());
#endif

			if	(
					(lTextCacheNdx >= 0)
				&&	(lTextCacheNdx < mTextCacheLimit)
				)
			{
				if	(lTextCache = mTextCache (lTextCacheNdx))
				{
#ifdef	_DEBUG_CACHE
					LogMessage (_DEBUG_CACHE, _T("  UseCached [%p] [%d] at [%d]"), lTextCache, lWordDisplayed, lTextCacheNdx);
#endif
					lDisplayText = (LPCTSTR)(*lTextCache);
				}
				else
				{
					lDisplayText = JoinStringArray (mTextWords, _T(""), false, 0, lWordDisplayed);
					mTextCache.SetAtGrow (lTextCacheNdx, new CAtlString ((LPCTSTR)lDisplayText));
#ifdef	_DEBUG_CACHE
					LogMessage (_DEBUG_CACHE, _T("  AddCached [%p] [%d] at [%d] CacheSize [%d] Text [%s]"), mTextCache[lTextCacheNdx], lWordDisplayed, lTextCacheNdx, mTextCache.GetCount(), DebugStr(*(mTextCache[lTextCacheNdx])));
#endif
				}
			}
			else
			{
				lDisplayText = JoinStringArray (mTextWords, _T(""), false, 0, lWordDisplayed);
			}
		}
	}
	else
	{
		lDisplayText = (LPCTSTR)mText;
	}
	return lDisplayText;
}

CAtlString CAgentTextDraw::GetDisplayWord (INT_PTR pWordNdx) const
{
	if	(
			(pWordNdx >= 0)
		&&	(pWordNdx < (INT_PTR)mTextWords.GetCount())
		)
	{
		return mTextWords [pWordNdx];
	}
	return CAtlString();
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentTextDraw::DisplayFirstWord (bool pForSpeech)
{
	mWordDisplayed = min (0, (INT_PTR)mTextWords.GetCount()-1);
	if	(pForSpeech)
	{
		mWordDisplayed = max (mWordDisplayed-1, -1);
		if	(mSpeechWords.GetCount() > 0)
		{
			mWordDisplayed = max (mWordDisplayed, mTextWords.GetCount() - mSpeechWords.GetCount());
		}
	}
	while	(
				(mWordDisplayed >= 0)
			&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
			&&	(mTextWords [mWordDisplayed].IsEmpty())
			)
	{
		mWordDisplayed++;
	}
	mBounds.SetRectEmpty ();

	return (mWordDisplayed >= 0);
}

bool CAgentTextDraw::DisplayNextWord (bool pForSpeech)
{
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
		)
	{
		if	(
				(pForSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mWordDisplayed = max (mWordDisplayed, mTextWords.GetCount() - mSpeechWords.GetCount());
		}

		mWordDisplayed++;

		while	(
					(mWordDisplayed >= 0)
				&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
				&&	(mTextWords [mWordDisplayed].IsEmpty())
				)
		{
			mWordDisplayed++;
		}
		return true;
	}
	return false;
}

bool CAgentTextDraw::DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech)
{
	INT_PTR	lWordNdx;
	long	lWordEnd = pWordPos + pWordLength;

	if	(pForSpeech)
	{
		for	(lWordNdx = 0; lWordNdx < (INT_PTR)mSpeechWords.GetCount(); lWordNdx++)
		{
			lWordEnd -= mSpeechWords [lWordNdx].GetLength ();
			if	(lWordEnd <= 0)
			{
				break;
			}
		}
		lWordNdx += (int)(mTextWords.GetCount() - mSpeechWords.GetCount());
		lWordNdx = min (lWordNdx, (INT_PTR)mTextWords.GetCount()-1);
	}
	else
	{
		for	(lWordNdx = 0; lWordNdx < (INT_PTR)mTextWords.GetCount(); lWordNdx++)
		{
			lWordEnd -= mTextWords [lWordNdx].GetLength ();
			if	(lWordEnd <= 0)
			{
				break;
			}
		}
		lWordNdx = min (lWordNdx, (INT_PTR)mTextWords.GetCount()-1);
	}

	if	(mWordDisplayed != lWordNdx)
	{
		return DisplayThisWord (lWordNdx, pForSpeech);
	}
	return false;
}

bool CAgentTextDraw::DisplayThisWord (INT_PTR pWordNdx, bool pForSpeech)
{
	if	(
			(pWordNdx >= 0)
		&&	(pWordNdx < (INT_PTR)mTextWords.GetCount())
		)
	{
		mWordDisplayed = pWordNdx;
		return true;
	}
	return false;
}

bool CAgentTextDraw::DisplayAllWords (bool pForSpeech)
{
	if	(mWordDisplayed != (INT_PTR)mTextWords.GetCount()-1)
	{
		mWordDisplayed = (INT_PTR)mTextWords.GetCount()-1;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentTextDraw::CanPace () const
{
	return (GetWordCount() > 0);
}

bool CAgentTextDraw::CanScroll (const CRect& pTextBounds) const
{
	return (mBounds.bottom > pTextBounds.bottom);
}

DWORD CAgentTextDraw::CalcScroll (const CRect& pTextBounds, long& pScrollInc, long& pScrollMin, long& pScrollMax, bool pClipLines, DWORD pMaxLineTime) const
{
	DWORD	lScrollTime = 0;

	if	(mBounds.bottom > pTextBounds.bottom)
	{
		long	lScrollScale;
		long	lScrollInc;

		pScrollMin = pClipLines ? GetLineRect(0).bottom - pTextBounds.bottom : 0;
		pScrollMax = mBounds.Height() - pTextBounds.Height();
		pScrollInc = max (MulDiv (GetSize().cy, 1, GetLineCount()*4), 1);

		lScrollTime = GetProfileInt (_T("windows"), _T("DragScrollInterval"), DD_DEFSCROLLINTERVAL) * 2;
		lScrollScale = ((long)lScrollTime + 9) / 10;
		lScrollInc = max (MulDiv (pScrollInc, 1, lScrollScale), 1);
		if	(lScrollInc < pScrollInc)
		{
			lScrollTime = max (MulDiv (lScrollTime, lScrollInc, pScrollInc), 10);
			pScrollInc = lScrollInc;
		}
		if	(
				(pMaxLineTime > 0)
			&&	(MulDiv (GetSize().cy, lScrollTime, GetLineCount() * pScrollInc) > (long)pMaxLineTime)
			)
		{
			lScrollTime = MulDiv (GetLineCount(), pMaxLineTime * pScrollInc, GetSize().cy);
		}
	}
	else
	{
		pScrollInc = 0;
		pScrollMax = 0;
	}
	return lScrollTime;
}

DWORD CAgentTextDraw::InitScroll (const CRect& pTextBounds, bool pForceReinit, bool pClipLines, DWORD pMaxLineTime)
{
	DWORD	lScrollTime;
	long	lScrollInc = mScrollInc;
	long	lScrollMin = mScrollMin;
	long	lScrollMax = mScrollMax;

	lScrollTime = CalcScroll (pTextBounds, lScrollInc, lScrollMin, lScrollMax, pClipLines, pMaxLineTime);
	if	(!lScrollTime)
	{
		mScrollPos = LONG_MIN;
	}

	if	(
			(mScrollInc != lScrollInc)
		||	(mScrollMin != lScrollMin)
		||	(mScrollMax != lScrollMax)
		||	(mScrollPos < lScrollMin)
		||	(mScrollPos > lScrollMax)
		||	(
				(pForceReinit)
			&&	(mScrollPos < lScrollMax)
			)
		)
	{
		mScrollInc = lScrollInc;
		mScrollMin = lScrollMin;
		mScrollMax = lScrollMax;
		mScrollPos = min (max (mScrollPos, mScrollMin), mScrollMax);
		return lScrollTime;
	}
	return 0;
}

bool CAgentTextDraw::ApplyScroll (const CRect& pTextBounds, CRect* pClipRect)
{
	if	(mScrollPos > 0)
	{
		mBounds.OffsetRect (0, -min (mScrollPos, max (mBounds.bottom - pTextBounds.bottom, 0)));

		if	(pClipRect)
		{
			int		lLineNdx;
			CRect	lLineRect;

			for	(lLineNdx = (int)mTextLines.GetCount()-1; lLineNdx >= 1; lLineNdx--)
			{
				lLineRect = GetLineRect (lLineNdx);
				if	(
						(lLineRect.bottom > pClipRect->bottom)
					&&	(lLineRect.top <= pClipRect->bottom)
					)
				{
					pClipRect->bottom = lLineRect.top;
					break;
				}
			}
		}
		return true;
	}
	return false;
}

bool CAgentTextDraw::Scroll ()
{
	if	(
			(mScrollPos < mScrollMax)
		&&	(mScrollInc > 0)
		)
	{
		mScrollPos = min (mScrollPos + mScrollInc, mScrollMax);
		return (mScrollPos > 0);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CSize CAgentTextDraw::CalcTextSize (HFONT pFont, USHORT pPerLine, USHORT pLines)
{
	CSize			lRet;
	CTextSize		lTextSize (pFont, false);
	tS <TEXTMETRIC>	lTextMetrics;

	lTextSize.GetTextMetrics (lTextMetrics);
	lRet = lTextSize.MeasureText (_T("x"), 1);
	lRet.cx *= (long)(short)pPerLine;
	lRet.cy = (long)(short)pLines * (lTextMetrics.tmAscent+lTextMetrics.tmDescent);

	return lRet;
}

CSize CAgentTextDraw::CalcTextSize (HFONT pFont, USHORT pPerLine)
{
	CSize		lRet;
	CTextSize	lTextSize (pFont, false);

	lRet = lTextSize.MeasureText (_T("x"), 1);
	lRet.cx *= (long)(short)pPerLine;
	mBounds.SetRect (0, 0, lRet.cx, INT_MAX);
	lRet.cy = MeasureText (GetFullText(), lTextSize.GetDC(), pFont).cy;
	mBounds.right = mBounds.left;

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentTextDraw::IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter)
{
	if	(mTextWords.GetCount() > 0)
	{
		int	lCharNdx = pNdx;
		int	lWordNdx;

		for	(lWordNdx = 0; (lCharNdx > 0) && (lWordNdx < (int)mTextWords.GetCount()); lWordNdx++)
		{
			lCharNdx -= mTextWords [lWordNdx].GetLength ();
		}
		pBreakAfter = false;
		return (lCharNdx == 0);
	}
	else
	{
		return CTextWrap::IsBreakChar (pText, pNdx, pPriority, pBreakAfter);
	}
}
