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
#if !defined(AFX_TEXTSIZE_H__73626BD3_B71D_4BFA_9B98_29853DCB6B16__INCLUDED_)
#define AFX_TEXTSIZE_H__73626BD3_B71D_4BFA_9B98_29853DCB6B16__INCLUDED_
#pragma once

#include <comdef.h>
#include <AfxCtl.h>
#include "AfxTemplEx.h"

//////////////////////////////////////////////////////////////////////

class CTextSize
{
public:
	CTextSize (CFont * pFont, bool pMeasureTwips);
	CTextSize (CFontHolder & pFont, bool pMeasureTwips);
	virtual ~CTextSize ();

// Attributes
	CDC & GetDC () {return mDC;}
	CFont * GetFont ();
	void GetTextMetrics (TEXTMETRIC & pMetrics);

// Operations
	friend CSize GetTextSize (LPCTSTR pText, HFONT pFont = NULL);

	CSize MeasureText (const CString & pText, bool pMultiline = false, long pWidth = 0)													{return MeasureText ((LPCTSTR) pText, pText.GetLength (), mDC.m_hAttribDC, NULL, pMultiline, pWidth);}
	CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen = -1, bool pMultiline = false, long pWidth = 0)									{return MeasureText (pText, pTextLen, mDC.m_hAttribDC, NULL, pMultiline, pWidth);}

	static CSize MeasureText (const CString & pText, HDC pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0)				{return MeasureText ((LPCTSTR) pText, pText.GetLength (), pDC, pFont, pMultiline, pWidth);}
	static CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen, HDC pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0);

	static CSize MeasureText (const CString & pText, CDC & pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0)			{return MeasureText ((LPCTSTR) pText, pText.GetLength (), pDC.m_hAttribDC, pFont, pMultiline, pWidth);}
	static CSize MeasureText (LPCTSTR pText, INT_PTR pTextLen, CDC & pDC, HFONT pFont = NULL, bool pMultiline = false, long pWidth = 0)	{return MeasureText (pText, pTextLen, pDC.m_hAttribDC, pFont, pMultiline, pWidth);}

	CPoint TwipsToPels (const CPoint & pPoint, bool pLogUnits = false);
	CPoint PelsToTwips (const CPoint & pPoint, bool pLogUnits = false);
	CSize TwipsToPels (const CSize & pSize, bool pLogUnits = false);
	CSize PelsToTwips (const CSize & pSize, bool pLogUnits = false);
	CRect TwipsToPels (const CRect & pRect, bool pLogUnits = false);
	CRect PelsToTwips (const CRect & pRect, bool pLogUnits = false);

	static CPoint TwipsToPels (HDC pDC, const CPoint & pPoint, bool pLogUnits = false);
	static CPoint PelsToTwips (HDC pDC, const CPoint & pPoint, bool pLogUnits = false);
	static CSize TwipsToPels (HDC pDC, const CSize & pSize, bool pLogUnits = false);
	static CSize PelsToTwips (HDC pDC, const CSize & pSize, bool pLogUnits = false);
	static CRect TwipsToPels (HDC pDC, const CRect & pRect, bool pLogUnits = false);
	static CRect PelsToTwips (HDC pDC, const CRect & pRect, bool pLogUnits = false);

	static CPoint TwipsToPels (CDC & pDC, const CPoint & pPoint, bool pLogUnits = false)		{return TwipsToPels (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CPoint PelsToTwips (CDC & pDC, const CPoint & pPoint, bool pLogUnits = false)		{return PelsToTwips (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CSize TwipsToPels (CDC & pDC, const CSize & pSize, bool pLogUnits = false)			{return TwipsToPels (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CSize PelsToTwips (CDC & pDC, const CSize & pSize, bool pLogUnits = false)			{return PelsToTwips (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CRect TwipsToPels (CDC & pDC, const CRect & pRect, bool pLogUnits = false)			{return TwipsToPels (pDC.m_hAttribDC, pRect, pLogUnits);}
	static CRect PelsToTwips (CDC & pDC, const CRect & pRect, bool pLogUnits = false)			{return PelsToTwips (pDC.m_hAttribDC, pRect, pLogUnits);}

	CPoint DeciPointsToPels (const CPoint & pPoint, bool pLogUnits = false);
	CPoint PelsToDeciPoints (const CPoint & pPoint, bool pLogUnits = false);
	CSize DeciPointsToPels (const CSize & pSize, bool pLogUnits = false);
	CSize PelsToDeciPoints (const CSize & pSize, bool pLogUnits = false);
	CRect DeciPointsToPels (const CRect & pRect, bool pLogUnits = false);
	CRect PelsToDeciPoints (const CRect & pRect, bool pLogUnits = false);

	static CPoint DeciPointsToPels (HDC pDC, const CPoint & pPoint, bool pLogUnits = false);
	static CPoint PelsToDeciPoints (HDC pDC, const CPoint & pPoint, bool pLogUnits = false);
	static CSize DeciPointsToPels (HDC pDC, const CSize & pSize, bool pLogUnits = false);
	static CSize PelsToDeciPoints (HDC pDC, const CSize & pSize, bool pLogUnits = false);
	static CRect DeciPointsToPels (HDC pDC, const CRect & pRect, bool pLogUnits = false);
	static CRect PelsToDeciPoints (HDC pDC, const CRect & pRect, bool pLogUnits = false);

	static CPoint DeciPointsToPels (CDC & pDC, const CPoint & pPoint, bool pLogUnits = false)	{return DeciPointsToPels (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CPoint PelsToDeciPoints (CDC & pDC, const CPoint & pPoint, bool pLogUnits = false)	{return PelsToDeciPoints (pDC.m_hAttribDC, pPoint, pLogUnits);}
	static CSize DeciPointsToPels (CDC & pDC, const CSize & pSize, bool pLogUnits = false)		{return DeciPointsToPels (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CSize PelsToDeciPoints (CDC & pDC, const CSize & pSize, bool pLogUnits = false)		{return PelsToDeciPoints (pDC.m_hAttribDC, pSize, pLogUnits);}
	static CRect DeciPointsToPels (CDC & pDC, const CRect & pRect, bool pLogUnits = false)		{return DeciPointsToPels (pDC.m_hAttribDC, pRect, pLogUnits);}
	static CRect PelsToDeciPoints (CDC & pDC, const CRect & pRect, bool pLogUnits = false)		{return PelsToDeciPoints (pDC.m_hAttribDC, pRect, pLogUnits);}

// Implementation
private:
	IFontPtr	mOleFont;
	HFONT		mFontHandle;
	HGDIOBJ		mOldFont;
	CDC			mDC;
};

//////////////////////////////////////////////////////////////////////

inline CSize GetTextSize (LPCTSTR pText, HFONT pFont)
{
	CSize	lRet (0,0);
	CDC		lDC;
	HGDIOBJ	lOldFont;

	if	(lDC.CreateCompatibleDC (0))
	{
		if	(pFont)
		{
			lOldFont = lDC.SelectObject (pFont);
		}
		lRet = lDC.GetTextExtent (pText, (int)_tcslen (pText));
		if	(pFont)
		{
			lDC.SelectObject (lOldFont);
		}
		lDC.DeleteDC ();
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TEXTSIZE_H__73626BD3_B71D_4BFA_9B98_29853DCB6B16__INCLUDED_)
