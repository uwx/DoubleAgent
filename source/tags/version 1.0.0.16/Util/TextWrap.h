/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#if !defined(AFX_TEXTWRAP_H__73626BD3_B71D_4BFA_9B98_29853DCB6B16__INCLUDED_)
#define AFX_TEXTWRAP_H__73626BD3_B71D_4BFA_9B98_29853DCB6B16__INCLUDED_
#pragma once

#include "TextSize.h"

//////////////////////////////////////////////////////////////////////

class CTextWrap
{
public:
	CTextWrap (LPCTSTR pBreakBefore = NULL, LPCTSTR pBreakAfter = NULL, int pWrapIndent = 0);
	CTextWrap (const CRect & pBounds, LPCTSTR pBreakBefore = NULL, LPCTSTR pBreakAfter = NULL, int pWrapIndent = 0);
	CTextWrap (const CTextWrap & pSource);
	virtual ~CTextWrap ();

// Attributes
	CRect	mBounds;
	int		mWrapIndent;
	bool	mUseExternalLeading;
	bool	mUseInternalLeading;

	const CSize & GetSize () const							{return mSize;}
	int GetLineCount () const								{return (int)mTextLines.GetSize ();}
	const COwnPtrArray <POLYTEXT> & GetLines () const		{return mTextLines;}

// Operations
	virtual CSize MeasureText (LPCTSTR pText, HDC pDC = NULL, HFONT pFont = NULL, bool * pWordBroken = NULL);
	virtual void DrawText (HDC pDC, HFONT pFont = NULL, const CRect * pClipRect = NULL, HDC pAttribDC = NULL);
	virtual void DrawText (HDC pDC, const CRect & pBounds, LPCTSTR pText, HFONT pFont = NULL, const CRect * pClipRect = NULL, HDC pAttribDC = NULL);

	CSize MeasureText (LPCTSTR pText, CDC & pDC, HFONT pFont = NULL, bool * pWordBroken = NULL) {return MeasureText (pText, pDC.m_hAttribDC, pFont, pWordBroken);}
	void DrawText (CDC & pDC, HFONT pFont = NULL, const CRect * pClipRect = NULL) {DrawText (pDC.m_hDC, pFont, pClipRect, pDC.IsPrinting() ? NULL : pDC.m_hAttribDC);}
	void DrawText (CDC & pDC, const CRect & pBounds, LPCTSTR pText, HFONT pFont = NULL, const CRect * pClipRect = NULL) {DrawText (pDC.m_hDC, pBounds, pText, pFont, pClipRect, pDC.IsPrinting() ? NULL : pDC.m_hAttribDC);}

	void Offset (int pXOffset, int pYOffset);
	void Offset (const CPoint & pOffset) {Offset (pOffset.x, pOffset.y);}
	UINT CenterLines ();

	CString GetLineText (int pLineNdx) const;
	CRect GetLineRect (int pLineNdx) const;
	CPoint GetLinePos (int pLineNdx) const;
	CString GetWrappedText () const;

// Implementation
protected:
	virtual void DrawLine (HDC pDC, int pLineNum, LPCTSTR pLineText, int pLineLength, CPoint & pLinePos, CRect & pLineRect);
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool & pBreakAfter);
	virtual int GetLineIndent (LPCTSTR pText, int pTextLen, HDC pDC, int pLineNdx);

	CTextWrap & operator= (const CTextWrap & pSource);

protected:
	CSize					mSize;
	CString					mBreakBefore;
	CString					mBreakAfter;
	COwnPtrArray <POLYTEXT>	mTextLines;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class CTextWrapPath : public CTextWrap
{
public:
	CTextWrapPath ();
	CTextWrapPath (const CRect & pBounds);
	virtual ~CTextWrapPath ();

// Implementation
protected:
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool & pBreakAfter);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TEXTWRAP_H__73626BD3_B71D_4BFA_9B98_29853DCB6B16__INCLUDED_)
