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
#include "StdAfx.h"
#include "TextSize.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CTextSize::CTextSize (CFont * pFont, bool pMeasureTwips)
:	mFontHandle (0),
	mOldFont (NULL)
{
	mDC.CreateCompatibleDC (0);
	if	(pMeasureTwips)
	{
		mDC.SetMapMode (MM_TWIPS);
	}

	if	(pFont->GetSafeHandle ())
	{
		mOldFont = mDC.SelectObject (pFont->GetSafeHandle ());
	}
}

CTextSize::CTextSize (CFontHolder & pFont, bool pMeasureTwips)
:	mFontHandle (0),
	mOldFont (NULL)
{
	mDC.CreateCompatibleDC (0);
	if	(pMeasureTwips)
	{
		mDC.SetMapMode (MM_TWIPS);
	}

	if	(
			((mOleFont = pFont.m_pFont) != NULL)
		&&	(SUCCEEDED (mOleFont->get_hFont (&mFontHandle)))
		)
	{
		mOleFont->AddRefHfont (mFontHandle);
		mOldFont = mDC.SelectObject ((HGDIOBJ) mFontHandle);
	}
}

CTextSize::~CTextSize ()
{
	if	(mOldFont)
	{
		mDC.SelectObject (mOldFont);
	}

	mDC.DeleteDC ();

	if	(
			(mOleFont != NULL)
		&&	(mFontHandle)
		)
	{
		mOleFont->ReleaseHfont (mFontHandle);
	}
}

//////////////////////////////////////////////////////////////////////

CFont * CTextSize::GetFont ()
{
	return mDC.GetCurrentFont ();
}

void CTextSize::GetTextMetrics (TEXTMETRIC & pMetrics)
{
	mDC.GetTextMetrics (&pMetrics);
}

//////////////////////////////////////////////////////////////////////

CSize CTextSize::MeasureText (LPCTSTR pText, INT_PTR pTextLen, HDC pDC, HFONT pFont, bool pMultiline, long pWidth)
{
	CSize	lRet (0, 0);
	CSize	lVptExt;
	CSize	lWinExt;
	HGDIOBJ	lOldFont;

#ifdef	_DEBUG
	if	(!pDC)
	{
		LogMessage (LogIfActive, _T("CTextSize::MeasureText with NULL DC"));
	}
#endif

	if	(::GetMapMode (pDC) != MM_TWIPS)
	{
		::SetWindowExtEx (pDC, 1, 1, &lWinExt);
		::SetViewportExtEx (pDC, 1, 1, &lVptExt);
	}

	if	(pFont)
	{
		lOldFont = ::SelectObject (pDC, pFont);
	}

	try
	{
		if	(
				(pText)
			&&	(pTextLen < 0)
			)
		{
			pTextLen = _tcslen (pText);
		}
		if	(
				(!pText)
			||	(pTextLen == 0)
			)
		{
			pText = _T("M");
			pTextLen = 1;
		}

		if	(pMultiline)
		{
			CRect lRect (0, 0, 0, 0);
			if	(pWidth > 0)
			{
				lRect.right = pWidth;
				::DrawText (pDC, pText, (int)pTextLen, &lRect, DT_LEFT | DT_NOCLIP | DT_NOPREFIX | DT_EXTERNALLEADING | DT_WORDBREAK | DT_CALCRECT);
			}
			else
			{
				::DrawText (pDC, pText, (int)pTextLen, &lRect, DT_LEFT | DT_NOCLIP | DT_NOPREFIX | DT_EXTERNALLEADING | DT_CALCRECT);
			}

			lRet = lRect.Size ();
		}
		else
		{
			::GetTextExtentPoint32 (pDC, pText, (int)pTextLen, &lRet);
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

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CPoint CTextSize::TwipsToPels (HDC pDC, const CPoint & pPoint, bool pLogUnits)
{
	CPoint	lPoint (pPoint);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	lPoint.x = MulDiv (lPoint.x, GetDeviceCaps (pDC, LOGPIXELSX), 1440);
	lPoint.y = MulDiv (lPoint.y, GetDeviceCaps (pDC, LOGPIXELSY), 1440);

	if	(pLogUnits)
	{
		DPtoLP (pDC, &lPoint, 1);
	}
	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint;
}

CPoint CTextSize::PelsToTwips (HDC pDC, const CPoint & pPoint, bool pLogUnits)
{
	CPoint	lPoint (pPoint);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	if	(pLogUnits)
	{
		LPtoDP (pDC, &lPoint, 1);
	}

	lPoint.x = MulDiv (lPoint.x, 1440, GetDeviceCaps (pDC, LOGPIXELSX));
	lPoint.y = MulDiv (lPoint.y, 1440, GetDeviceCaps (pDC, LOGPIXELSY));

	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint;
}

CSize CTextSize::TwipsToPels (HDC pDC, const CSize & pSize, bool pLogUnits)
{
	CPoint	lOrigin (0, 0);
	CPoint	lPoint (pSize);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	lPoint.x = MulDiv (lPoint.x, GetDeviceCaps (pDC, LOGPIXELSX), 1440);
	lPoint.y = MulDiv (lPoint.y, GetDeviceCaps (pDC, LOGPIXELSY), 1440);

	if	(pLogUnits)
	{
		DPtoLP (pDC, &lOrigin, 1);
		DPtoLP (pDC, &lPoint, 1);
	}
	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint - lOrigin;
}

CSize CTextSize::PelsToTwips (HDC pDC, const CSize & pSize, bool pLogUnits)
{
	CPoint	lOrigin (0, 0);
	CPoint	lPoint (pSize);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	if	(pLogUnits)
	{
		LPtoDP (pDC, &lOrigin, 1);
		LPtoDP (pDC, &lPoint, 1);
	}

	lPoint.x = MulDiv (lPoint.x - lOrigin.x, 1440, GetDeviceCaps (pDC, LOGPIXELSX));
	lPoint.y = MulDiv (lPoint.y - lOrigin.y, 1440, GetDeviceCaps (pDC, LOGPIXELSY));

	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint;
}

CRect CTextSize::TwipsToPels (HDC pDC, const CRect & pRect, bool pLogUnits)
{
	return CRect (TwipsToPels (pDC, pRect.TopLeft (), pLogUnits), TwipsToPels (pDC, pRect.BottomRight (), pLogUnits));
}

CRect CTextSize::PelsToTwips (HDC pDC, const CRect & pRect, bool pLogUnits)
{
	return CRect (PelsToTwips (pDC, pRect.TopLeft (), pLogUnits), PelsToTwips (pDC, pRect.BottomRight (), pLogUnits));
}

//////////////////////////////////////////////////////////////////////

CPoint CTextSize::DeciPointsToPels (HDC pDC, const CPoint & pPoint, bool pLogUnits)
{
	CPoint	lPoint (pPoint);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	lPoint.x = MulDiv (lPoint.x, GetDeviceCaps (pDC, LOGPIXELSX), 720);
	lPoint.y = MulDiv (lPoint.y, GetDeviceCaps (pDC, LOGPIXELSY), 720);

	if	(pLogUnits)
	{
		DPtoLP (pDC, &lPoint, 1);
	}
	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint;
}

CPoint CTextSize::PelsToDeciPoints (HDC pDC, const CPoint & pPoint, bool pLogUnits)
{
	CPoint	lPoint (pPoint);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	if	(pLogUnits)
	{
		LPtoDP (pDC, &lPoint, 1);
	}

	lPoint.x = MulDiv (lPoint.x, 720, GetDeviceCaps (pDC, LOGPIXELSX));
	lPoint.y = MulDiv (lPoint.y, 720, GetDeviceCaps (pDC, LOGPIXELSY));

	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint;
}

CSize CTextSize::DeciPointsToPels (HDC pDC, const CSize & pSize, bool pLogUnits)
{
	CPoint	lOrigin (0, 0);
	CPoint	lPoint (pSize);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	lPoint.x = MulDiv (lPoint.x, GetDeviceCaps (pDC, LOGPIXELSX), 720);
	lPoint.y = MulDiv (lPoint.y, GetDeviceCaps (pDC, LOGPIXELSY), 720);

	if	(pLogUnits)
	{
		DPtoLP (pDC, &lOrigin, 1);
		DPtoLP (pDC, &lPoint, 1);
	}
	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint - lOrigin;
}

CSize CTextSize::PelsToDeciPoints (HDC pDC, const CSize & pSize, bool pLogUnits)
{
	CPoint	lOrigin (0, 0);
	CPoint	lPoint (pSize);
	HDC		lTempDC = NULL;

	if	(
			(!pDC)
		&&	(lTempDC = CreateCompatibleDC (0))
		)
	{
		pDC = lTempDC;
	}

	if	(pLogUnits)
	{
		LPtoDP (pDC, &lOrigin, 1);
		LPtoDP (pDC, &lPoint, 1);
	}

	lPoint.x = MulDiv (lPoint.x - lOrigin.x, 720, GetDeviceCaps (pDC, LOGPIXELSX));
	lPoint.y = MulDiv (lPoint.y - lOrigin.y, 720, GetDeviceCaps (pDC, LOGPIXELSY));

	if	(lTempDC)
	{
		DeleteDC (lTempDC);
	}
	return lPoint;
}

CRect CTextSize::DeciPointsToPels (HDC pDC, const CRect & pRect, bool pLogUnits)
{
	return CRect (DeciPointsToPels (pDC, pRect.TopLeft (), pLogUnits), DeciPointsToPels (pDC, pRect.BottomRight (), pLogUnits));
}

CRect CTextSize::PelsToDeciPoints (HDC pDC, const CRect & pRect, bool pLogUnits)
{
	return CRect (PelsToDeciPoints (pDC, pRect.TopLeft (), pLogUnits), PelsToDeciPoints (pDC, pRect.BottomRight (), pLogUnits));
}

//////////////////////////////////////////////////////////////////////

CPoint CTextSize::TwipsToPels (const CPoint & pPoint, bool pLogUnits)
{
	return TwipsToPels (mDC.m_hAttribDC, pPoint, pLogUnits);
}
CPoint CTextSize::PelsToTwips (const CPoint & pPoint, bool pLogUnits)
{
	return PelsToTwips (mDC.m_hAttribDC, pPoint, pLogUnits);
}
CSize CTextSize::TwipsToPels (const CSize & pSize, bool pLogUnits)
{
	return TwipsToPels (mDC.m_hAttribDC, pSize, pLogUnits);
}
CSize CTextSize::PelsToTwips (const CSize & pSize, bool pLogUnits)
{
	return PelsToTwips (mDC.m_hAttribDC, pSize, pLogUnits);
}
CRect CTextSize::TwipsToPels (const CRect & pRect, bool pLogUnits)
{
	return TwipsToPels (mDC.m_hAttribDC, pRect, pLogUnits);
}
CRect CTextSize::PelsToTwips (const CRect & pRect, bool pLogUnits)
{
	return PelsToTwips (mDC.m_hAttribDC, pRect, pLogUnits);
}

//////////////////////////////////////////////////////////////////////

CPoint CTextSize::DeciPointsToPels (const CPoint & pPoint, bool pLogUnits)
{
	return DeciPointsToPels (mDC.m_hAttribDC, pPoint, pLogUnits);
}
CPoint CTextSize::PelsToDeciPoints (const CPoint & pPoint, bool pLogUnits)
{
	return PelsToDeciPoints (mDC.m_hAttribDC, pPoint, pLogUnits);
}
CSize CTextSize::DeciPointsToPels (const CSize & pSize, bool pLogUnits)
{
	return DeciPointsToPels (mDC.m_hAttribDC, pSize, pLogUnits);
}
CSize CTextSize::PelsToDeciPoints (const CSize & pSize, bool pLogUnits)
{
	return PelsToDeciPoints (mDC.m_hAttribDC, pSize, pLogUnits);
}
CRect CTextSize::DeciPointsToPels (const CRect & pRect, bool pLogUnits)
{
	return DeciPointsToPels (mDC.m_hAttribDC, pRect, pLogUnits);
}
CRect CTextSize::PelsToDeciPoints (const CRect & pRect, bool pLogUnits)
{
	return PelsToDeciPoints (mDC.m_hAttribDC, pRect, pLogUnits);
}
