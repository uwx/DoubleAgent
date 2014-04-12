/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#ifdef	__cplusplus_cli
#include "TextWrap.h"
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Text;
using namespace System::Windows;
using namespace System::Windows::Media;
#else
#include <shlwapi.h>
#include "TextWrap.h"
#endif
#include "DebugStr.h"
#include "Log.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
namespace DoubleAgent {
#else
#pragma comment(lib, "shlwapi.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif
#endif

//////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
//////////////////////////////////////////////////////////////////////

CTextWrap::CTextWrap ()
:	mBounds (0,0,0,0),
	mSize (0,0),
	mWrapIndent (0),
	mClipPartialLines (false)
{
}

CTextWrap::CTextWrap (System::String^ pBreakBefore, System::String^ pBreakAfter)
:	mBounds (0,0,0,0),
	mSize (0,0),
	mWrapIndent (0),
	mClipPartialLines (false),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter)
{
}

CTextWrap::CTextWrap (System::String^ pBreakBefore, System::String^ pBreakAfter, System::Single pWrapIndent)
:	mBounds (0,0,0,0),
	mSize (0,0),
	mWrapIndent (pWrapIndent),
	mClipPartialLines (false),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter)
{
}

CTextWrap::CTextWrap (System::Drawing::RectangleF pBounds)
:	mBounds (pBounds),
	mSize (0,0),
	mWrapIndent (0),
	mClipPartialLines (false)
{
}

CTextWrap::CTextWrap (System::Drawing::RectangleF pBounds, System::String^ pBreakBefore, System::String^ pBreakAfter)
:	mBounds (pBounds),
	mSize (0,0),
	mWrapIndent (0),
	mClipPartialLines (false),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter)
{
}

CTextWrap::CTextWrap (System::Drawing::RectangleF pBounds, System::String^ pBreakBefore, System::String^ pBreakAfter, System::Single pWrapIndent)
:	mBounds (pBounds),
	mSize (0,0),
	mWrapIndent (pWrapIndent),
	mClipPartialLines (false),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter)
{
}

CTextWrap::CTextWrap (CTextWrap^ pSource)
{
	operator= (pSource);
}

//////////////////////////////////////////////////////////////////////

CTextWrap^ CTextWrap::operator= (CTextWrap^ pSource)
{
	mBounds = pSource->mBounds;
	mSize = pSource->mSize;
	mWrapIndent = pSource->mWrapIndent;
	mClipPartialLines = pSource->mClipPartialLines;
	mBreakBefore = pSource->mBreakBefore;
	mBreakAfter = pSource->mBreakAfter;
	mTextLines = nullptr;
	return this;
}

CTextWrap::TextLine::TextLine ()
{
}

//////////////////////////////////////////////////////////////////////

System::Drawing::SizeF CTextWrap::Size::get()
{
	return mSize;
}

System::Int32 CTextWrap::LineCount::get()
{
	return (mTextLines) ? mTextLines->Length : 0;
}

array <CTextWrap::TextLine^>^ CTextWrap::Lines::get()
{
	return mTextLines;
}

//////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
//////////////////////////////////////////////////////////////////////

CTextWrap::CTextWrap (LPCTSTR pBreakBefore, LPCTSTR pBreakAfter, int pWrapIndent)
:	mBounds (0, 0, 0, 0),
	mSize (0, 0),
	mBreakBefore (pBreakBefore),
	mBreakAfter (pBreakAfter),
	mWrapIndent (pWrapIndent),
	mClipPartialLines (false),
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
	mClipPartialLines (false),
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
	mClipPartialLines = pSource.mClipPartialLines;
	mBreakBefore = pSource.mBreakBefore;
	mBreakAfter = pSource.mBreakAfter;
	mUseExternalLeading = pSource.mUseExternalLeading;
	mUseInternalLeading = pSource.mUseInternalLeading;
	mTextLines.DeleteAll ();
	return *this;
}

//////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::Drawing::SizeF CTextWrap::MeasureText (System::String^ pText)
{
	return MeasureText (pText, nullptr, nullptr);
}

System::Drawing::SizeF CTextWrap::MeasureText (System::String^ pText, System::Drawing::Graphics^ pGraphics, System::Drawing::Font^ pFont)
{
	Boolean lWordBroken;
	return MeasureText (pText, pGraphics, pFont, lWordBroken);
}

System::Drawing::SizeF CTextWrap::MeasureText (System::String^ pText, System::Drawing::Graphics^ pGraphics, System::Drawing::Font^ pFont, System::Boolean% pWordBroken)
{
	HDC						lTempDC = NULL;
	Graphics^				lGraphics = pGraphics;
	System::Drawing::Font^	lFont = pFont;
	RectangleF				lBounds (mBounds);
	StringFormat^			lStringFormat;
	String^					lText = pText;
	TextLine^				lTextLine;
	SizeF					lLineSize;
	SizeF					lFitSize;
	int						lFitCount;
	int						lFitLines;
	int						lEolChar;

	pWordBroken = false;
	
	if	(!pGraphics)
	{
		lTempDC = CreateCompatibleDC (0);
		lGraphics = Graphics::FromHdc ((IntPtr)lTempDC);
		lGraphics->TextRenderingHint = TextRenderingHint::AntiAlias;
	}
	if	(!pFont)
	{
		lFont = System::Drawing::SystemFonts::DefaultFont;
	}

	mSize.Width = 0;
	mSize.Height = 0;
	mTextLines = gcnew array <TextLine^> (0);

	lStringFormat = safe_cast <StringFormat^> (System::Drawing::StringFormat::GenericTypographic->Clone());
	lStringFormat->Trimming = StringTrimming::Word;
	lStringFormat->FormatFlags = StringFormatFlags::NoWrap;

	try
	{
		while	(
					(lText->Length > 0)
				&&	((lTextLine = gcnew TextLine) != nullptr)
				)
		{
			lTextLine->mString = lText;
			lTextLine->mBounds.X = GetLineIndent (lText, lText->Length, lGraphics, lFont, mTextLines->Length);
			lBounds.X = mBounds.X + lTextLine->mBounds.X;
			lBounds.Width = mBounds.Width - lTextLine->mBounds.X;

			lEolChar = lText->IndexOf (_T('\n'));
			lFitSize = lGraphics->MeasureString ((lEolChar >= 0)?lText->Substring(0,lEolChar):lText, lFont, SizeF((Single)Math::Floor(lBounds.Width),0), lStringFormat, (lFitCount = 0), (lFitLines=0));

			if	(lFitCount > 0)
			{
				lFitCount = Math::Max (lFitCount, Math::Min (pText->Length, 2));

				if	(lEolChar >= 0)
				{
					if	(lEolChar == lFitCount)
					{
						lFitCount++;
					}
				}
				else
				if	(
						(lFitCount >= 2)
					&&	(lFitCount < lText->Length)
					)
				{
					int		lCount = lFitCount;
					int		lLastBreak = -1;
					bool	lBreakAfter;

					if	(
							(IsBreakChar (pText, pText->Length-lText->Length+lCount, 1, lBreakAfter))
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
							if	(IsBreakChar (pText, pText->Length-lText->Length+lCount, 1, lBreakAfter))
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
								(IsBreakChar (pText, pText->Length-lText->Length+lCount, 2, lBreakAfter))
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
								if	(IsBreakChar (pText, pText->Length-lText->Length+lCount, 2, lBreakAfter))
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
								(lFitCount < lText->Length)
							&&	(
									(Char::IsWhiteSpace (lText [lFitCount]))
								||	(
										(IsBreakChar (pText, pText->Length-lText->Length+lFitCount, 0, lBreakAfter))
									&&	(lBreakAfter)
									)
								)
							)
					{
						lFitCount++;
						lLastBreak = Math::Max (lLastBreak, 0);
					}

					if	(lLastBreak < 0)
					{
						pWordBroken = true;
					}
				}

				lFitSize.Width = lGraphics->MeasureString (lText->Substring(0,lFitCount), lFont, (int)lBounds.Width, lStringFormat).Width;
				lFitSize.Height = (Single)Math::Ceiling (lGraphics->MeasureString ("x", lFont, Int16::MaxValue, System::Drawing::StringFormat::GenericDefault).Height);

				lTextLine->mString = lText->Substring (0, lFitCount);
				lTextLine->mBounds.X = 0;
				lTextLine->mBounds.Width = lFitSize.Width;
				lTextLine->mBounds.Y = mSize.Height;
				lTextLine->mBounds.Height = lFitSize.Height;

				while	(
							(!String::IsNullOrEmpty (lTextLine->mString))
						&&	(lTextLine->mString[lTextLine->mString->Length-1] == _T('\n'))
						)
				{
					lTextLine->mString = lTextLine->mString->Substring (0, lTextLine->mString->Length-1);
				}

				lText = lText->Substring (lFitCount);
				mSize.Width = Math::Max (mSize.Width, lTextLine->mBounds.Right);
				mSize.Height += lFitSize.Height;

				Array::Resize (mTextLines, mTextLines->Length+1);
				mTextLines[mTextLines->Length-1] = lTextLine;
			}
		}
	}
	catch AnyExceptionDebug

	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}

	mBounds.Height = mSize.Height;
	return mSize;
}

//////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
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

	if	(pWordBroken)
	{
		(*pWordBroken) = false;
	}
	
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
//////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
//////////////////////////////////////////////////////////////////////
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pColor)
{
	DrawText (pGraphics, gcnew SolidBrush (pColor));
}
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Brush^ pBrush)
{
	DrawText (pGraphics, pBrush, nullptr);
}

void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pColor, System::Drawing::Font^ pFont)
{
	DrawText (pGraphics, gcnew SolidBrush (pColor), pFont, nullptr);
}
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont)
{
	DrawText (pGraphics, pBrush, pFont, nullptr);
}

void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pColor, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect)
{
	DrawText (pGraphics, mBounds, nullptr, gcnew SolidBrush (pColor), pFont, pClipRect);
}
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect)
{
	DrawText (pGraphics, mBounds, nullptr, pBrush, pFont, pClipRect);
}
 
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Color pColor)
{
	DrawText (pGraphics, mBounds, pText, gcnew SolidBrush (pColor), nullptr, nullptr);
}
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Brush^ pBrush)
{
	DrawText (pGraphics, mBounds, pText, pBrush, nullptr, nullptr);
}

void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Color pColor, System::Drawing::Font^ pFont)
{
	DrawText (pGraphics, mBounds, pText, gcnew SolidBrush (pColor), pFont, nullptr);
}
void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont)
{
	DrawText (pGraphics, mBounds, pText, pBrush, pFont, nullptr);
}

void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Color pColor, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect)
{
	DrawText (pGraphics, mBounds, pText, gcnew SolidBrush (pColor), pFont, pClipRect);
}

void CTextWrap::DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect)
{
	System::Drawing::Drawing2D::GraphicsState^	lState = nullptr;
	
	if	(pClipRect)
	{
		lState = pGraphics->Save ();
		pGraphics->IntersectClip (*pClipRect);
	}

	try
	{
		String^		lLineText;
		int			lLineStart = 0;

		for each (TextLine^ lLine in mTextLines)
		{
			if	(pClipRect)
			{
				if	(lLine->mBounds.Top + mBounds.Top > pClipRect->Bottom)
				{
					break;
				}
				if	(
						(mClipPartialLines)
					&&	(
							(lLine->mBounds.Bottom + mBounds.Top > pClipRect->Bottom)
						&&	(lLine->mBounds.Top + mBounds.Top <= pClipRect->Bottom)
						)
					)
				{
					break;
				}
			}

			if	(pText)
			{
				if	(lLineStart >= pText->Length)
				{
					break;
				}
				lLineText = pText->Substring (lLineStart, Math::Min (lLine->mString->Length, pText->Length-lLineStart));
				while	(
							(!String::IsNullOrEmpty (lLineText))
						&&	(lLineText[0] == _T('\n'))
						)
				{
					lLineStart++;
					lLineText = lLineText->Substring (1);
				}
				if	(String::IsNullOrEmpty (lLineText))
				{
					break;
				}
			}
			else
			{
				lLineText = lLine->mString;
			}

			pGraphics->DrawString (lLineText, pFont, pBrush, pBounds.X+lLine->mBounds.X, pBounds.Y+lLine->mBounds.Y, System::Drawing::StringFormat::GenericTypographic); 

			lLineStart += lLine->mString->Length;
		}
	}
	catch AnyExceptionDebug

	if	(lState)
	{
		pGraphics->Restore (lState);
	}
}

//////////////////////////////////////////////////////////////////////
#else	__cplusplus_cli
//////////////////////////////////////////////////////////////////////

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
			lLinePos.x = pBounds.left + lLine->x;
			lLinePos.y = pBounds.top + lLine->rcl.top + lLine->y;
			lLineRect.SetRect (pBounds.left, pBounds.top + lLine->rcl.top, pBounds.right, pBounds.top + lLine->rcl.bottom);

			if	(pClipRect)			
			{
				if	(lLineRect.top >= pClipRect->bottom)
				{
					break;
				}
				if	(
						(mClipPartialLines)
					&&	(
							(lLineRect.bottom > pClipRect->bottom)
						&&	(lLineRect.top <= pClipRect->bottom)
						)
					)
				{
					break;
				}
			}

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
#endif	__cplusplus_cli
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CTextWrap::Offset (System::Drawing::PointF pOffset)
{
	Offset (pOffset.X, pOffset.Y);
}
void CTextWrap::Offset (System::Single pXOffset, System::Single pYOffset)
{
	mBounds.Offset (pXOffset, pYOffset);
}
#else
void CTextWrap::Offset (int pXOffset, int pYOffset)
{
	mBounds.OffsetRect (pXOffset, pYOffset);
}
#endif

#ifdef	__cplusplus_cli
System::UInt32 CTextWrap::CenterLines ()
{
	System::UInt32	lRet = 0;
	TextLine^		lLine;
	
	if (mTextLines)
	{
		for each (lLine in mTextLines)
		{
			if	(lLine->mBounds.X != (mBounds.Width - lLine->mBounds.Width) / 2)
			{
				lLine->mBounds.X = (mBounds.Width - lLine->mBounds.Width) / 2;
				lRet++;
			}
		}
	}
	return lRet;
}
#else
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
			lRet++;
		}
	}
	return lRet;
}
#endif

//////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CTextWrap::GetWrappedText ()
{
	System::String^	lWrappedText = String::Empty;
	Boolean			lFirstLine = true;
	TextLine^		lLine;

	if (mTextLines)
	{
		for each (lLine in mTextLines)
		{
			if	(lFirstLine)
			{
				lFirstLine = false;
			}
			else
			{
				lWrappedText += "\n";
			}
			lWrappedText += lLine->mString;
		}
	}
	return lWrappedText;
}
#else
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
#endif

#ifdef	__cplusplus_cli
System::Drawing::RectangleF CTextWrap::GetUsedRect ()
{
	return GetUsedRect (false);
}

System::Drawing::RectangleF CTextWrap::GetUsedRect (System::Boolean pClipPartialLines)
{
	return GetUsedRect (pClipPartialLines, nullptr);
}

System::Drawing::RectangleF CTextWrap::GetUsedRect (System::Boolean pClipPartialLines, System::String^ pText)
#else
CRect CTextWrap::GetUsedRect (bool pClipPartialLines, LPCTSTR pText) const
#endif
{
#ifdef	__cplusplus_cli
	RectangleF	lUsedRect (mBounds.Right, mBounds.Top, 0, 0);
	RectangleF	lLineRect;
	String^		lLineText;
	int			lLineStart = 0;
#else
	CRect		lUsedRect (mBounds.right, mBounds.top, mBounds.left, mBounds.top);
	int			lNdx;
	POLYTEXT *	lLine;
	CRect		lLineRect;
	LPCTSTR		lLineText;
	int			lLineStart = 0;
	int			lLineLength;
	LPCTSTR		lTextEnd;
#endif	

#ifdef	__cplusplus_cli
	for each (TextLine^ lLine in mTextLines)
#else
	if	(pText)
	{
		lTextEnd = pText + _tcslen (pText);
	}
	for	(lNdx = 0; lLine = mTextLines (lNdx); lNdx++)
#endif	
	{
#ifdef	__cplusplus_cli
		if	(pText)
		{
			if	(lLineStart >= pText->Length)
			{
				break;
			}
			lLineText = pText->Substring (lLineStart, Math::Min (lLine->mString->Length, pText->Length-lLineStart));
			while	(
						(!String::IsNullOrEmpty (lLineText))
					&&	(lLineText[0] == _T('\n'))
					)
			{
				lLineStart++;
				lLineText = lLineText->Substring (1);
			}
			if	(String::IsNullOrEmpty (lLineText))
			{
				break;
			}
		}
		else
		{
			lLineText = lLine->mString;
		}
#else
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
#endif			

#ifdef	__cplusplus_cli
		lLineRect.X = mBounds.X + lLine->mBounds.X;
		lLineRect.Y = mBounds.Y + lLine->mBounds.Y;
		lLineRect.Width = lLine->mBounds.Width;
		lLineRect.Height = lLine->mBounds.Height;
		lLineStart += lLine->mString->Length;
#else
		lLineRect.SetRect (mBounds.left + lLine->rcl.left, mBounds.top + lLine->rcl.top, mBounds.left + lLine->rcl.right, mBounds.top + lLine->rcl.bottom);
		lLineStart += lLine->n;
#endif		

#ifdef	__cplusplus_cli
		if	(lLineRect.Top >= mBounds.Bottom)
#else
		if	(lLineRect.top >= mBounds.bottom)
#endif		
		{
			break;
		}
		else
		{
#ifdef	__cplusplus_cli
			lUsedRect.X = Math::Min (lUsedRect.X, lLineRect.X);
			lUsedRect.Width = Math::Max (lUsedRect.Right, lLineRect.Right) - lUsedRect.Left;
#else
			lUsedRect.left = min (lUsedRect.left, lLineRect.left);
			lUsedRect.right = max (lUsedRect.right, lLineRect.right);
#endif

#ifdef	__cplusplus_cli
			if	(lLineRect.Bottom <= mBounds.Bottom)
			{
				lUsedRect.Height = lLineRect.Bottom - lUsedRect.Top;
			}
			else
			if	(pClipPartialLines)
			{
				break;
			}
			else
			{
				lUsedRect.Height = Math::Max (lUsedRect.Bottom, mBounds.Bottom) - lUsedRect.Top;
			}
#else
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
#endif
		}
	}
	return lUsedRect;
}

//////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CTextWrap::GetLineText (System::Int32 pLineNdx)
{
	TextLine^	lLine;

	if	(
			(mTextLines)
		&&	(pLineNdx >= 0)
		&&	(pLineNdx < mTextLines->Length)
		&&	(lLine = mTextLines [pLineNdx])
		)
	{
		return lLine->mString;
	}
	return String::Empty;
}
#else
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
#endif

#ifdef	__cplusplus_cli
System::Drawing::RectangleF CTextWrap::GetLineRect (System::Int32 pLineNdx)
{
	System::Drawing::RectangleF	lLineRect (0,0,0,0);
	TextLine^					lLine;
	
	if	(
			(mTextLines)
		&&	(pLineNdx >= 0)
		&&	(pLineNdx < mTextLines->Length)
		&&	(lLine = mTextLines [pLineNdx])
		)
	{
		lLineRect.X = mBounds.X;
		lLineRect.Y = mBounds.Y + lLine->mBounds.Y;
		lLineRect.Width = mBounds.Width;
		lLineRect.Height = lLine->mBounds.Height;
	}
	return lLineRect;
}
#else
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
#endif

#ifdef	__cplusplus_cli
System::Drawing::PointF CTextWrap::GetLinePos (System::Int32 pLineNdx)
{
	System::Drawing::PointF	lLinePos (0,0);
	TextLine^				lLine;
	
	if	(
			(mTextLines)
		&&	(pLineNdx >= 0)
		&&	(pLineNdx < mTextLines->Length)
		&&	(lLine = mTextLines [pLineNdx])
		)
	{
		lLinePos.X = mBounds.X + lLine->mBounds.X;
		lLinePos.Y = mBounds.Y + lLine->mBounds.Y;
	}
	return lLinePos;
}
#else
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
#endif

#ifdef	__cplusplus_cli
System::Int32 CTextWrap::GetLineWidth (System::Int32 pLineNdx)
{
	TextLine^	lLine;
	
	if	(
			(mTextLines)
		&&	(pLineNdx >= 0)
		&&	(pLineNdx < mTextLines->Length)
		&&	(lLine = mTextLines [pLineNdx])
		)
	{
		lLine->mBounds.Width;
	}
	return 0;
}
#else
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
#endif

#ifdef	__cplusplus_cli
System::Int32 CTextWrap::GetLineHeight (System::Int32 pLineNdx)
{
	TextLine^	lLine;
	
	if	(
			(mTextLines)
		&&	(pLineNdx >= 0)
		&&	(pLineNdx < mTextLines->Length)
		&&	(lLine = mTextLines [pLineNdx])
		)
	{
		lLine->mBounds.Height;
	}
	return 0;
}
#else
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
#endif

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::Boolean CTextWrap::IsBreakChar (System::String^ pText, int pNdx, UINT pPriority, System::Boolean% pBreakAfter)
{
	Char									lChar = pText [pNdx];
	System::Globalization::UnicodeCategory	lCharCategory;

	if	(pPriority > 0)
	{
		if	(
				(!String::IsNullOrEmpty (mBreakAfter))
			&&	(mBreakAfter->IndexOf (lChar) >= 0)
			)
		{
			pBreakAfter = true;
			return true;
		}
		else
		if	(
				(pNdx > 0)
			&&	(!String::IsNullOrEmpty (mBreakBefore))
			&&	(mBreakBefore->IndexOf (lChar) >= 0)
			)
		{
			pBreakAfter = false;
			return true;
		}
	}

	lCharCategory = Char::GetUnicodeCategory (lChar);
	if	(
			(lCharCategory == System::Globalization::UnicodeCategory::SpaceSeparator)
		||	(lCharCategory == System::Globalization::UnicodeCategory::LineSeparator)
		||	(lCharCategory == System::Globalization::UnicodeCategory::ParagraphSeparator)
		)
	{
		pBreakAfter = false;
		return true;
	}
	return false;
}
#else
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
#endif

#ifdef	__cplusplus_cli
System::Single CTextWrap::GetLineIndent (System::String^ pText, int pTextLen, System::Drawing::Graphics^ pGraphics, System::Drawing::Font^ pFont, int pLineNdx)
{
	if	(mWrapIndent < 0)
	{
		mWrapIndent = pGraphics->MeasureString (pText->Substring (0, Math::Min (pTextLen, Math::Abs((int)mWrapIndent))), pFont).Width;
	}
	if	(pLineNdx > 0)
	{
		return Math::Max (mWrapIndent, 0.0f);
	}
	else
	{
		return 0;
	}
}
#else
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
#endif

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
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
//////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
//////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
