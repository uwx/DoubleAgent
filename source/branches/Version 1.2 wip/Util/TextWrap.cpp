/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <shlwapi.h>
#include "TextWrap.h"
#include "DebugStr.h"
#include "Log.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////

CTextWrap::CTextWrap (LPCTSTR pBreakBefore, LPCTSTR pBreakAfter, int pWrapIndent)
:	mBounds (0, 0, 0, 0),
	mSize (0, 0),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter),
	mWrapIndent (pWrapIndent),
	mUseExternalLeading (false),
	mUseInternalLeading (true)
{
}

CTextWrap::CTextWrap (const CRect& pBounds, LPCTSTR pBreakBefore, LPCTSTR pBreakAfter, int pWrapIndent)
:	mBounds (pBounds),
	mSize (0, 0),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter),
	mWrapIndent (pWrapIndent),
	mUseExternalLeading (false),
	mUseInternalLeading (true)
{
}

CTextWrap::CTextWrap (const CTextWrap& pSource)
{
	operator= (pSource);
}

CTextWrap::~CTextWrap ()
{
}

//////////////////////////////////////////////////////////////////////

CTextWrap& CTextWrap::operator= (const CTextWrap& pSource)
{
	mBounds = pSource.mBounds;
	mSize = pSource.mSize;
	mWrapIndent = pSource.mWrapIndent;
	mBreakBefore = pSource.mBreakBefore;
	mBreakAfter = pSource.mBreakAfter;
	mUseExternalLeading = pSource.mUseExternalLeading;
	mUseInternalLeading = pSource.mUseInternalLeading;
	mTextLines.DeleteAll ();
	return *this;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSize CTextWrap::MeasureText (LPCTSTR pText, HDC pDC, HFONT pFont, bool* pWordBroken)
{
	HDC					lTempDC = NULL;
	CSize				lVptExt;
	CSize				lWinExt;
	HGDIOBJ				lOldFont;
	tS <TEXTMETRIC>		lFontMetrics;
	CRect				lBounds (mBounds);
	LPCTSTR				lText = pText;
	int					lTextLen = (int)_tcslen (pText);
	tPtr <POLYTEXT>		lTextLine;
	CSize				lLineSize;
	CSize				lFitSize;
	int					lFitCount;
	LPCTSTR				lEolChar;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	mSize.cx = 0;
	mSize.cy = 0;
	mTextLines.DeleteAll ();

	if	(::GetMapMode (pDC) != MM_TWIPS)
	{
		::SetWindowExtEx (pDC, 1, 1, &lWinExt);
		::SetViewportExtEx (pDC, 1, 1, &lVptExt);
	}

	if	(pFont)
	{
		lOldFont = ::SelectObject (pDC, pFont);
	}
	if	(pWordBroken)
	{
		(*pWordBroken) = false;
	}

	GetTextMetrics (pDC, &lFontMetrics);

	try
	{
		while	(
					(lTextLen > 0)
				&&	(lTextLine = new tS <POLYTEXT>)
				)
		{
			lTextLine->lpstr = lText;
			lTextLine->x = GetLineIndent (lText, lTextLen, pDC, (int)mTextLines.GetCount ());
			lBounds.left = mBounds.left + lTextLine->x;

			if	(::GetTextExtentExPoint (pDC, lText, lTextLen, lBounds.Width (), &(lFitCount = 0), NULL, &lFitSize))
			{
				lFitCount = max (lFitCount, min (lTextLen, 2));
				if	(
						(lEolChar = _tcschr (lText, _T('\n')))
					&&	((lEolChar - lText + 1) < lFitCount)
					)
				{
					lFitCount = (int)(lEolChar - lText + 1);
				}
				else
				{
					lEolChar = NULL;
				}

				if	(lEolChar)
				{
					::GetTextExtentPoint32 (pDC, lText, lFitCount-1, &lFitSize);
				}
				else
				if	(
						(lFitCount >= 2)
					&&	(lFitCount < lTextLen)
					)
				{
					int		lCount = lFitCount;
					int		lLastBreak = -1;
					bool	lBreakAfter;

					if	(
							(IsBreakChar (pText, (int)(lText-pText)+lCount, 1, lBreakAfter))
						&&	(!lBreakAfter)
						)
					{
						lLastBreak = 1;
					}
					else
					{
						while	(lCount > 1)
						{
							lCount--;
							if	(IsBreakChar (pText, (int)(lText-pText)+lCount, 1, lBreakAfter))
							{
								if	(lBreakAfter)
								{
									lCount++;
								}
								lLastBreak = 1;
								break;
							}
						}
					}

					if	(
							(lCount <= 1)
						&&	(lFitCount >= 2)
						)
					{
						lCount = lFitCount;
						lLastBreak = -1;

						if	(
								(IsBreakChar (pText, (int)(lText-pText)+lCount, 2, lBreakAfter))
							&&	(!lBreakAfter)
							)
						{
							lLastBreak = 2;
						}
						else
						{
							while	(lCount > 1)
							{
								lCount--;
								if	(IsBreakChar (pText, (int)(lText-pText)+lCount, 2, lBreakAfter))
								{
									if	(lBreakAfter)
									{
										lCount++;
									}
									lLastBreak = 2;
									break;
								}
							}
						}
					}

					if	(lCount > 1)
					{
						lFitCount = lCount;
					}

					while	(
								(lFitCount < lTextLen)
							&&	(
									(_istspace (lText [lFitCount]))
								||	(
										(IsBreakChar (pText, (int)(lText-pText)+lFitCount, 0, lBreakAfter))
									&&	(lBreakAfter)
									)
								)
							)
					{
						lFitCount++;
						lLastBreak = max (lLastBreak, 0);
					}

					if	(
							(pWordBroken)
						&&	(lLastBreak < 0)
						)
					{
						(*pWordBroken) = true;
					}

					::GetTextExtentPoint32 (pDC, lText, lFitCount, &lFitSize);
				}

				lTextLine->n = lFitCount;
				lTextLine->rcl.right = lFitSize.cx;
				lTextLine->rcl.top = mSize.cy;
				lTextLine->rcl.bottom = lTextLine->rcl.top + lFitSize.cy;

				while	(
							(lTextLine->n > 0)
						&&	(lText [lTextLine->n - 1] == _T('\n'))
						)
				{
					lTextLine->n--;
				}

				lTextLen -= lFitCount;
				lText += lFitCount;
				mSize.cx = max (mSize.cx, lTextLine->x + lFitSize.cx);
				mSize.cy += lFitSize.cy;

				if	(
						(mTextLines.GetCount () > 0)
					&&	(mUseExternalLeading)
					)
				{
					lTextLine->y += lFontMetrics.tmExternalLeading;
					lTextLine->rcl.bottom += lTextLine->y;
					mSize.cy += lTextLine->y;
				}
				if	(!mUseInternalLeading)
				{
					mSize.cy -= lFontMetrics.tmInternalLeading;
				}

				mTextLines.Add (lTextLine.Detach ());
			}
			else
			{
				LogWinErr (LogIfActive|LogTime, GetLastError (), _T("GetTextExtentExPoint"));
				mTextLines.DeleteAll ();
				break;
			}
		}

		if	(
				(mTextLines.GetCount () > 1)
			&&	(!mUseInternalLeading)
			)
		{
			mSize.cy += lFontMetrics.tmInternalLeading;
		}
	}
	catch AnyExceptionDebug

	if	(::GetMapMode (pDC) != MM_TWIPS)
	{
		::SetWindowExtEx (pDC, lWinExt.cx, lWinExt.cy, NULL);
		::SetViewportExtEx (pDC, lVptExt.cx, lVptExt.cy, NULL);
	}

	if	(pFont)
	{
		::SelectObject (pDC, lOldFont);
	}
	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}

	mBounds.bottom = mBounds.top + mSize.cy;
	return mSize;
}

void CTextWrap::DrawText (HDC pDC, HFONT pFont, const CRect* pClipRect, HDC pAttribDC)
{
	DrawText (pDC, mBounds, NULL, pFont, pClipRect, pAttribDC);
}

void CTextWrap::DrawText (HDC pDC, const CRect& pBounds, LPCTSTR pText, HFONT pFont, const CRect* pClipRect, HDC pAttribDC)
{
	HGDIOBJ	lOldFont = NULL;

	if	(pClipRect)
	{
		CRect	lClipRect (*pClipRect);

		SaveDC (pDC);
		if	(
				(pAttribDC)
			&&	(pAttribDC != pDC)
			)
		{
			DPtoLP (pAttribDC, (LPPOINT) &lClipRect, 2);
		}
		IntersectClipRect (pDC, lClipRect.left, lClipRect.top, lClipRect.right, lClipRect.bottom);
	}

	if	(pFont)
	{
		lOldFont = SelectObject (pDC, pFont);
	}

	try
	{
		int			lNdx;
		POLYTEXT *	lLine;
		CPoint		lLinePos;
		CRect		lLineRect;
		LPCTSTR		lLineText;
		int			lLineStart = 0;
		int			lLineLength;
		LPCTSTR		lTextEnd;

		if	(pText)
		{
			lTextEnd = pText + _tcslen (pText);
		}

		for	(lNdx = 0; lLine = mTextLines (lNdx); lNdx++)
		{
			lLineLength = lLine->n;

			if	(pText)
			{
				lLineText = pText + lLineStart;
				while	(
							(lLineText < lTextEnd)
						&&	(*lLineText == _T('\n'))
						)
				{
					lLineStart++;
					lLineText++;
				}
				if	(lLineText >= lTextEnd)
				{
					break;
				}
				lLineLength = min (lLineLength, lTextEnd - lLineText);
			}
			else
			{
				lLineText = lLine->lpstr;
			}

			lLinePos.x = pBounds.left + lLine->x;
			lLinePos.y = pBounds.top + lLine->rcl.top + lLine->y;
			lLineRect.SetRect (pBounds.left, pBounds.top + lLine->rcl.top, pBounds.right, pBounds.top + lLine->rcl.bottom);

			if	(
					(pAttribDC)
				&&	(pAttribDC != pDC)
				)
			{
				DPtoLP (pDC, &lLinePos, 1);
				DPtoLP (pDC, (LPPOINT) &lLineRect, 2);
			}
			DrawLine (pDC, lNdx, lLineText, lLineLength, lLinePos, lLineRect);

			lLineStart += lLine->n;
		}
	}
	catch AnyExceptionDebug

	if	(pFont)
	{
		SelectObject (pDC, lOldFont);
	}
	if	(pClipRect)
	{
		RestoreDC (pDC, -1);
	}
}

void CTextWrap::DrawLine (HDC pDC, int pLineNum, LPCTSTR pLineText, int pLineLength, CPoint& pLinePos, CRect& pLineRect)
{
	ExtTextOut (pDC, pLinePos.x, pLinePos.y, 0, &pLineRect, pLineText, pLineLength, NULL);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CTextWrap::Offset (int pXOffset, int pYOffset)
{
	mBounds.OffsetRect (pXOffset, pYOffset);
}

UINT CTextWrap::CenterLines ()
{
	UINT		lRet = 0;
	int			lNdx;
	POLYTEXT *	lLine;

	for	(lNdx = 0; lLine = mTextLines (lNdx); lNdx++)
	{
		if	(lLine->x != (mBounds.Width () - lLine->rcl.right + lLine->rcl.left) / 2)
		{
			lLine->x = (mBounds.Width () - lLine->rcl.right + lLine->rcl.left) / 2;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString CTextWrap::GetWrappedText () const
{
	CString				lWrappedText;
	int					lNdx;
	const POLYTEXT *	lLine;

	for	(lNdx = 0; lLine = mTextLines (lNdx); lNdx++)
	{
		if	(lNdx > 0)
		{
			lWrappedText += _T("\n");
		}
		lWrappedText += CString (lLine->lpstr, lLine->n);
	}
	return lWrappedText;
}

CRect CTextWrap::GetUsedRect (bool pClipPartialLines, LPCTSTR pText) const
{
	CRect		lUsedRect (mBounds.right, mBounds.top, mBounds.left, mBounds.top);
	int			lNdx;
	POLYTEXT *	lLine;
	CRect		lLineRect;
	LPCTSTR		lLineText;
	int			lLineStart = 0;
	int			lLineLength;
	LPCTSTR		lTextEnd;

	if	(pText)
	{
		lTextEnd = pText + _tcslen (pText);
	}

	for	(lNdx = 0; lLine = mTextLines (lNdx); lNdx++)
	{
		lLineLength = lLine->n;

		if	(pText)
		{
			lLineText = pText + lLineStart;
			while	(
						(lLineText < lTextEnd)
					&&	(*lLineText == _T('\n'))
					)
			{
				lLineStart++;
				lLineText++;
			}
			if	(lLineText >= lTextEnd)
			{
				break;
			}
			lLineLength = min (lLineLength, lTextEnd - lLineText);
		}
		else
		{
			lLineText = lLine->lpstr;
		}

		lLineRect.SetRect (mBounds.left + lLine->rcl.left, mBounds.top + lLine->rcl.top, mBounds.left + lLine->rcl.right, mBounds.top + lLine->rcl.bottom);
		lLineStart += lLine->n;

		if	(lLineRect.top >= mBounds.bottom)
		{
			break;
		}
		else
		{
			lUsedRect.left = min (lUsedRect.left, lLineRect.left);
			lUsedRect.right = max (lUsedRect.right, lLineRect.right);

			if	(lLineRect.bottom <= mBounds.bottom)
			{
				lUsedRect.bottom = lLineRect.bottom;
			}
			else
			if	(pClipPartialLines)
			{
				break;
			}
			else
			{
				lUsedRect.bottom = max (lUsedRect.bottom, mBounds.bottom);
			}
		}
	}
	return lUsedRect;
}

//////////////////////////////////////////////////////////////////////

CString CTextWrap::GetLineText (int pLineNdx) const
{
	CString				lLineText;
	const POLYTEXT *	lLine;

	if	(lLine = mTextLines (pLineNdx))
	{
		lLineText += CString (lLine->lpstr, lLine->n);
	}
	return lLineText;
}

CRect CTextWrap::GetLineRect (int pLineNdx) const
{
	CRect				lLineRect (0,0,0,0);
	const POLYTEXT *	lLine;

	if	(lLine = mTextLines (pLineNdx))
	{
		lLineRect.SetRect (mBounds.left, mBounds.top + lLine->rcl.top, mBounds.right, mBounds.top + lLine->rcl.bottom);
	}
	return lLineRect;
}

CPoint CTextWrap::GetLinePos (int pLineNdx) const
{
	CPoint				lLinePos (0,0);
	const POLYTEXT *	lLine;

	if	(lLine = mTextLines (pLineNdx))
	{
		lLinePos.x = mBounds.left + lLine->rcl.left + lLine->x;
		lLinePos.y = mBounds.top + lLine->rcl.top + lLine->y;
	}
	return lLinePos;
}

int CTextWrap::GetLineWidth (int pLineNdx) const
{
	int					lLineWidth = 0;
	const POLYTEXT *	lLine;

	if	(lLine = mTextLines (pLineNdx))
	{
		lLineWidth = lLine->rcl.right - lLine->rcl.left;
	}
	return lLineWidth;
}

int CTextWrap::GetLineHeight (int pLineNdx) const
{
	int					lLineHeight = 0;
	const POLYTEXT *	lLine;

	if	(lLine = mTextLines (pLineNdx))
	{
		lLineHeight = lLine->rcl.bottom - lLine->rcl.top;
	}
	return lLineHeight;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CTextWrap::IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter)
{
	TCHAR	lChar = pText [pNdx];

	if	(pPriority > 0)
	{
		if	(
				(!mBreakAfter.IsEmpty ())
			&&	(mBreakAfter.Find (lChar) >= 0)
			)
		{
			pBreakAfter = true;
			return true;
		}
		else
		if	(
				(pNdx > 0)
			&&	(!mBreakBefore.IsEmpty ())
			&&	(mBreakBefore.Find (lChar) >= 0)
			)
		{
			pBreakAfter = false;
			return true;
		}
	}

	if	(_istspace (lChar))
	{
		pBreakAfter = false;
		return true;
	}
	return false;
}

int CTextWrap::GetLineIndent (LPCTSTR pText, int pTextLen, HDC pDC, int pLineNdx)
{
	if	(mWrapIndent < 0)
	{
		mWrapIndent = CTextSize::MeasureText (CString (pText, min (pTextLen, abs (mWrapIndent))), pDC).cx;
	}
	if	(pLineNdx > 0)
	{
		return max (mWrapIndent, 0);
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CTextWrapPath::CTextWrapPath ()
:	CTextWrap (CRect (0,0,0,0), _T("\\/]"), _T("["), -2)
{
}

CTextWrapPath::CTextWrapPath (const CRect& pBounds)
:	CTextWrap (pBounds, _T("\\/]"), _T("["), -2)
{
}

CTextWrapPath::~CTextWrapPath ()
{
}

//////////////////////////////////////////////////////////////////////

bool CTextWrapPath::IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter)
{
	TCHAR	lChar = pText [pNdx];

	if	(pPriority >= 2)
	{
		if	(
				(_istspace (lChar))
			||	(
					(pNdx > 0)
				&&	(lChar == _T('.'))
				)
			)
		{
			pBreakAfter = false;
			return true;
		}
		else
		if	(lChar == _T(':'))
		{
			pBreakAfter = true;
			return true;
		}
	}

	if	(pPriority >= 1)
	{
		if	(
				(PathGetCharType (lChar) == GCT_SEPARATOR)
			||	(mBreakAfter.Find (lChar) >= 0)
			)
		{
			pBreakAfter = true;
			return true;
		}
		else
		if	(
				(
					(pNdx > 0)
				&&	(pText [pNdx-1] != _T('.'))
				&&	(pText [pNdx-1] != _T(':'))
				)
			&&	(mBreakBefore.Find (lChar) >= 0)
			)
		{
			pBreakAfter = false;
			return true;
		}
	}

	return false;
}
