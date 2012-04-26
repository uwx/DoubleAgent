/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
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
#pragma managed(push,off)
/////////////////////////////////////////////////////////////////////////////
#include <comdef.h>
/////////////////////////////////////////////////////////////////////////////

class CTextSize
{
public:
	CTextSize (HFONT, bool pMeasureTwips);
#ifdef	__AFXWIN_H__
	CTextSize (CFont * pFont, bool pMeasureTwips);
#endif
#ifdef	__AFXCTL_H__
	CTextSize (CFontHolder& pFont, bool pMeasureTwips);
#endif
	virtual ~CTextSize ();

// Attributes
#ifdef	__AFXWIN_H__
	CDC& GetDC () {return mDC;}
	CFont * GetFont ();
#else
	HDC GetDC () const {return mDC;}
	HFONT GetFont () const;
#endif
	void GetTextMetrics (TEXTMETRIC& pMetrics);

// Operations
	friend CSize GetTextSize (LPCTSTR pText, HFONT pFont = NULL);

#ifdef	__AFXWIN_H__
	CSize MeasureText (const CString& pText, bool pMultiline = false, long pWidth = 0)													{return MeasureText ((LPCTSTR) pText, pText.GetLength (), mDC.m_hAttribDC, NULL, pMultiline, pWidth);}
	CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen = -1, bool pMultiline = false, long pWidth = 0)									{return MeasureText (pText, pTextLen, mDC.m_hAttribDC, NULL, pMultiline, pWidth);}
#else
	CSize MeasureText (const CString& pText, bool pMultiline = false, long pWidth = 0)													{return MeasureText ((LPCTSTR) pText, pText.GetLength (), mDC, NULL, pMultiline, pWidth);}
	CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen = -1, bool pMultiline = false, long pWidth = 0)									{return MeasureText (pText, pTextLen, mDC, NULL, pMultiline, pWidth);}
#endif

	static CSize MeasureText (const CString& pText, HDC pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0)				{return MeasureText ((LPCTSTR) pText, pText.GetLength (), pDC, pFont, pMultiline, pWidth);}
	static CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen, HDC pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0);

#ifdef	__AFXWIN_H__
	static CSize MeasureText (const CString& pText, CDC& pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0)			{return MeasureText ((LPCTSTR) pText, pText.GetLength (), pDC.m_hAttribDC, pFont, pMultiline, pWidth);}
	static CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen, CDC& pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0)	{return MeasureText (pText, pTextLen, pDC.m_hAttribDC, pFont, pMultiline, pWidth);}
#endif

	CPoint TwipsToPels (const CPoint& pPoint, bool pLogUnits = false);
	CPoint PelsToTwips (const CPoint& pPoint, bool pLogUnits = false);
	CSize TwipsToPels (const CSize& pSize, bool pLogUnits = false);
	CSize PelsToTwips (const CSize& pSize, bool pLogUnits = false);
	CRect TwipsToPels (const CRect& pRect, bool pLogUnits = false);
	CRect PelsToTwips (const CRect& pRect, bool pLogUnits = false);

	static CPoint TwipsToPels (HDC pDC, const CPoint& pPoint, bool pLogUnits = false);
	static CPoint PelsToTwips (HDC pDC, const CPoint& pPoint, bool pLogUnits = false);
	static CSize TwipsToPels (HDC pDC, const CSize& pSize, bool pLogUnits = false);
	static CSize PelsToTwips (HDC pDC, const CSize& pSize, bool pLogUnits = false);
	static CRect TwipsToPels (HDC pDC, const CRect& pRect, bool pLogUnits = false);
	static CRect PelsToTwips (HDC pDC, const CRect& pRect, bool pLogUnits = false);

#ifdef	__AFXWIN_H__
	static CPoint TwipsToPels (CDC& pDC, const CPoint& pPoint, bool pLogUnits = false)		{return TwipsToPels (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CPoint PelsToTwips (CDC& pDC, const CPoint& pPoint, bool pLogUnits = false)		{return PelsToTwips (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CSize TwipsToPels (CDC& pDC, const CSize& pSize, bool pLogUnits = false)			{return TwipsToPels (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CSize PelsToTwips (CDC& pDC, const CSize& pSize, bool pLogUnits = false)			{return PelsToTwips (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CRect TwipsToPels (CDC& pDC, const CRect& pRect, bool pLogUnits = false)			{return TwipsToPels (pDC.m_hAttribDC, pRect, pLogUnits);}
	static CRect PelsToTwips (CDC& pDC, const CRect& pRect, bool pLogUnits = false)			{return PelsToTwips (pDC.m_hAttribDC, pRect, pLogUnits);}
#endif

	CPoint DeciPointsToPels (const CPoint& pPoint, bool pLogUnits = false);
	CPoint PelsToDeciPoints (const CPoint& pPoint, bool pLogUnits = false);
	CSize DeciPointsToPels (const CSize& pSize, bool pLogUnits = false);
	CSize PelsToDeciPoints (const CSize& pSize, bool pLogUnits = false);
	CRect DeciPointsToPels (const CRect& pRect, bool pLogUnits = false);
	CRect PelsToDeciPoints (const CRect& pRect, bool pLogUnits = false);

	static CPoint DeciPointsToPels (HDC pDC, const CPoint& pPoint, bool pLogUnits = false);
	static CPoint PelsToDeciPoints (HDC pDC, const CPoint& pPoint, bool pLogUnits = false);
	static CSize DeciPointsToPels (HDC pDC, const CSize& pSize, bool pLogUnits = false);
	static CSize PelsToDeciPoints (HDC pDC, const CSize& pSize, bool pLogUnits = false);
	static CRect DeciPointsToPels (HDC pDC, const CRect& pRect, bool pLogUnits = false);
	static CRect PelsToDeciPoints (HDC pDC, const CRect& pRect, bool pLogUnits = false);

#ifdef	__AFXWIN_H__
	static CPoint DeciPointsToPels (CDC& pDC, const CPoint& pPoint, bool pLogUnits = false)	{return DeciPointsToPels (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CPoint PelsToDeciPoints (CDC& pDC, const CPoint& pPoint, bool pLogUnits = false)	{return PelsToDeciPoints (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CSize DeciPointsToPels (CDC& pDC, const CSize& pSize, bool pLogUnits = false)		{return DeciPointsToPels (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CSize PelsToDeciPoints (CDC& pDC, const CSize& pSize, bool pLogUnits = false)		{return PelsToDeciPoints (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CRect DeciPointsToPels (CDC& pDC, const CRect& pRect, bool pLogUnits = false)		{return DeciPointsToPels (pDC.m_hAttribDC, pRect, pLogUnits);}
	static CRect PelsToDeciPoints (CDC& pDC, const CRect& pRect, bool pLogUnits = false)		{return PelsToDeciPoints (pDC.m_hAttribDC, pRect, pLogUnits);}
#endif

// Implementation
private:
#ifdef	__AFXWIN_H__
	CDC				mDC;
#else
	CMemDCHandle	mDC;
#endif
#ifdef	__AFXCTL_H__
	IFontPtr		mOleFont;
#endif
	HFONT			mOleFontHandle;
	HGDIOBJ			mOldFont;
};

/////////////////////////////////////////////////////////////////////////////

inline CSize GetTextSize (LPCTSTR pText, HFONT pFont)
{
	CSize			lRet (0,0);
	CMemDCHandle	lDC;
	HGDIOBJ			lOldFont;

	if	(lDC.Attach (::CreateCompatibleDC (0)))
	{
		if	(pFont)
		{
			lOldFont = ::SelectObject (lDC, pFont);
		}
		::GetTextExtentPoint32 (lDC, pText, (int)_tcslen (pText), &lRet);
		if	(pFont)
		{
			::SelectObject (lDC, lOldFont);
		}
		lDC.Close ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////
