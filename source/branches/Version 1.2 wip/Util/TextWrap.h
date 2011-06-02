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
#pragma once

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#else
#include "TextSize.h"
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CTextWrap
#else
class CTextWrap
#endif
{
public:
#ifdef	__cplusplus_cli
	CTextWrap ();
	CTextWrap (System::String^ pBreakBefore, System::String^ pBreakAfter);
	CTextWrap (System::String^ pBreakBefore, System::String^ pBreakAfter, System::Single pWrapIndent);
	CTextWrap (System::Drawing::RectangleF pBounds);
	CTextWrap (System::Drawing::RectangleF pBounds, System::String^ pBreakBefore, System::String^ pBreakAfter);
	CTextWrap (System::Drawing::RectangleF pBounds, System::String^ pBreakBefore, System::String^ pBreakAfter, System::Single pWrapIndent);
	CTextWrap (CTextWrap^ pSource);
#else
	CTextWrap (LPCTSTR pBreakBefore = NULL, LPCTSTR pBreakAfter = NULL, int pWrapIndent = 0);
	CTextWrap (const CRect& pBounds, LPCTSTR pBreakBefore = NULL, LPCTSTR pBreakAfter = NULL, int pWrapIndent = 0);
	CTextWrap (const CTextWrap& pSource);
	virtual ~CTextWrap ();
#endif

// Attributes
#ifdef	__cplusplus_cli
	System::Drawing::RectangleF	mBounds;
	System::Single				mWrapIndent;
#else
	CRect						mBounds;
	int							mWrapIndent;
	bool						mUseExternalLeading;
	bool						mUseInternalLeading;
#endif	

#ifdef	__cplusplus_cli
	ref struct TextLine
	{
		System::String^				mString;
		System::Drawing::RectangleF	mBounds;
		
		TextLine ();
	};
#endif

#ifdef	__cplusplus_cli
	property System::Drawing::SizeF Size {System::Drawing::SizeF get();}
	property System::Int32 LineCount {System::Int32 get();}
	property array <TextLine^>^ Lines {array <TextLine^>^ get();}
#else
	const CSize& GetSize () const							{return mSize;}
	int GetLineCount () const								{return (int)mTextLines.GetCount();}
	const COwnPtrArray <POLYTEXT>& GetLines () const		{return mTextLines;}
#endif	

// Operations
#ifdef	__cplusplus_cli
	System::Drawing::SizeF MeasureText (System::String^ pText);
	System::Drawing::SizeF MeasureText (System::String^ pText, System::Drawing::Graphics^ pGraphics, System::Drawing::Font^ pFont);
	virtual System::Drawing::SizeF MeasureText (System::String^ pText, System::Drawing::Graphics^ pGraphics, System::Drawing::Font^ pFont, System::Boolean% pWordBroken);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pColor);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pColor, System::Drawing::Font^ pFont);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pColor, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Brush^ pBrush);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont);
	virtual void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Color pColor);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Color pColor, System::Drawing::Font^ pFont);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Color pColor, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Brush^ pBrush);
	void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont);
	virtual void DrawText (System::Drawing::Graphics^ pGraphics, System::Drawing::RectangleF pBounds, System::String^ pText, System::Drawing::Brush^ pBrush, System::Drawing::Font^ pFont, System::Drawing::RectangleF^ pClipRect);
#else
	virtual CSize MeasureText (LPCTSTR pText, HDC pDC = NULL, HFONT pFont = NULL, bool* pWordBroken = NULL);
	virtual void DrawText (HDC pDC, HFONT pFont = NULL, const CRect* pClipRect = NULL, HDC pAttribDC = NULL);
	virtual void DrawText (HDC pDC, const CRect& pBounds, LPCTSTR pText, HFONT pFont = NULL, const CRect* pClipRect = NULL, HDC pAttribDC = NULL);
#endif	

#ifdef	__AFXWIN_H__
	CSize MeasureText (LPCTSTR pText, CDC& pDC, HFONT pFont = NULL, bool* pWordBroken = NULL) {return MeasureText (pText, pDC.m_hAttribDC, pFont, pWordBroken);}
	void DrawText (CDC& pDC, HFONT pFont = NULL, const CRect* pClipRect = NULL) {DrawText (pDC.m_hDC, pFont, pClipRect, pDC.IsPrinting() ? NULL : pDC.m_hAttribDC);}
	void DrawText (CDC& pDC, const CRect& pBounds, LPCTSTR pText, HFONT pFont = NULL, const CRect* pClipRect = NULL) {DrawText (pDC.m_hDC, pBounds, pText, pFont, pClipRect, pDC.IsPrinting() ? NULL : pDC.m_hAttribDC);}
#endif

#ifdef	__cplusplus_cli
	void Offset (System::Single pXOffset, System::Single pYOffset);
	void Offset (System::Drawing::PointF pOffset);
	System::UInt32 CenterLines ();
#else
	void Offset (int pXOffset, int pYOffset);
	void Offset (const CPoint& pOffset) {Offset (pOffset.x, pOffset.y);}
	UINT CenterLines ();
#endif	

#ifdef	__cplusplus_cli
	System::String^ GetWrappedText ();
	System::Drawing::RectangleF GetUsedRect ();
	System::Drawing::RectangleF GetUsedRect (System::Boolean pClipPartialLines);
	System::Drawing::RectangleF GetUsedRect (System::Boolean pClipPartialLines, System::String^ pText);
	System::String^ GetLineText (System::Int32 pLineNdx);
	System::Drawing::RectangleF GetLineRect (System::Int32 pLineNdx);
	System::Drawing::PointF GetLinePos (System::Int32 pLineNdx);
	System::Int32 GetLineWidth (System::Int32 pLineNdx);
	System::Int32 GetLineHeight (System::Int32 pLineNdx);
#else
	CString GetWrappedText () const;
	CRect GetUsedRect (bool pClipPartialLines = false, LPCTSTR pText = NULL) const;
	CString GetLineText (int pLineNdx) const;
	CRect GetLineRect (int pLineNdx) const;
	CPoint GetLinePos (int pLineNdx) const;
	int GetLineWidth (int pLineNdx) const;
	int GetLineHeight (int pLineNdx) const;
#endif	

// Implementation
protected:
#ifdef	__cplusplus_cli
	virtual System::Boolean IsBreakChar (System::String^ pText, int pNdx, UINT pPriority, System::Boolean% pBreakAfter);
	virtual System::Single GetLineIndent (System::String^ pText, int pTextLen, System::Drawing::Graphics^ pGraphics, System::Drawing::Font^ pFont, int pLineNdx);
internal:
	CTextWrap^ operator= (CTextWrap^ pSource);
#else
	virtual void DrawLine (HDC pDC, int pLineNum, LPCTSTR pLineText, int pLineLength, CPoint& pLinePos, CRect& pLineRect);
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter);
	virtual int GetLineIndent (LPCTSTR pText, int pTextLen, HDC pDC, int pLineNdx);

	CTextWrap& operator= (const CTextWrap& pSource);
#endif	

protected:
#ifdef	__cplusplus_cli
	System::Drawing::SizeF	mSize;
	System::String^			mBreakBefore;
	System::String^			mBreakAfter;
	array <TextLine^>^		mTextLines;
#else
	CSize					mSize;
	CString					mBreakBefore;
	CString					mBreakAfter;
	COwnPtrArray <POLYTEXT>	mTextLines;
#endif	
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli

class CTextWrapPath : public CTextWrap
{
public:
	CTextWrapPath ();
	CTextWrapPath (const CRect& pBounds);
	virtual ~CTextWrapPath ();

// Implementation
protected:
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter);
};

#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
