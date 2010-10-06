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

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

#ifdef	__AFXWIN_H__
#define	_MY_DC_HANDLE	mDC.m_hAttribDC
#else
#define	_MY_DC_HANDLE	mDC
#endif

//////////////////////////////////////////////////////////////////////

CTextSize::CTextSize (HFONT pFont, bool pMeasureTwips)
:	mOleFontHandle (0),
	mOldFont (NULL)
{
#ifdef	__AFXWIN_H__
	mDC.CreateCompatibleDC (0);
#else
	mDC.Attach (CreateCompatibleDC (0));
#endif
	if	(pMeasureTwips)
	{
		::SetMapMode (mDC, MM_TWIPS);
	}
	if	(pFont)
	{
		mOldFont = ::SelectObject (mDC, pFont);
	}
}

#ifdef	__AFXWIN_H__
CTextSize::CTextSize (CFont * pFont, bool pMeasureTwips)
:	mOleFontHandle (0),
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
#endif

#ifdef	__AFXCTL_H__
CTextSize::CTextSize (CFontHolder & pFont, bool pMeasureTwips)
:	mOleFontHandle (0),
	mOldFont (NULL)
{
	mDC.CreateCompatibleDC (0);
	if	(pMeasureTwips)
	{
		mDC.SetMapMode (MM_TWIPS);
	}

	if	(
			((mOleFont = pFont.m_pFont) != NULL)
		&&	(SUCCEEDED (mOleFont->get_hFont (&mOleFontHandle)))
		)
	{
		mOleFont->AddRefHfont (mOleFontHandle);
		mOldFont = mDC.SelectObject ((HGDIOBJ) mOleFontHandle);
	}
}
#endif

CTextSize::~CTextSize ()
{
#ifdef	__AFXWIN_H__
	if	(mOldFont)
	{
		mDC.SelectObject (mOldFont);
	}
	mDC.DeleteDC ();
#else
	if	(mOldFont)
	{
		::SelectObject (mDC, mOldFont);
	}
	mDC.Close ();
#endif
#ifdef	__AFXCTL_H__
	if	(
			(mOleFont != NULL)
		&&	(mOleFontHandle)
		)
	{
		mOleFont->ReleaseHfont (mOleFontHandle);
	}
#endif
}

//////////////////////////////////////////////////////////////////////

#ifdef	__AFXWIN_H__
CFont * CTextSize::GetFont ()
{
	return mDC.GetCurrentFont ();
}
#else
HFONT CTextSize::GetFont () const
{
	return (HFONT)::GetCurrentObject(mDC, OBJ_FONT);
}
#endif

void CTextSize::GetTextMetrics (TEXTMETRIC & pMetrics)
{
	::GetTextMetrics (mDC, &pMetrics);
}

//////////////////////////////////////////////////////////////////////

CSize CTextSize::MeasureText (LPCTSTR pText, INT_PTR pTextLen, HDC pDC, HFONT pFont, bool pMultiline, long pWidth)
{
	CSize		lRet (0, 0);
	CSize		lVptExt;
	CSize		lWinExt;
	tPtr <CDC>	lDC;
	HGDIOBJ		lOldFont;

	if	(
			(!pDC)
		&&	(lDC = new CDC)
		&&	(lDC->CreateCompatibleDC (NULL))
		)
	{
		pDC = lDC->GetSafeHdc ();
	}

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
	return TwipsToPels (_MY_DC_HANDLE, pPoint, pLogUnits);
}
CPoint CTextSize::PelsToTwips (const CPoint & pPoint, bool pLogUnits)
{
	return PelsToTwips (_MY_DC_HANDLE, pPoint, pLogUnits);
}
CSize CTextSize::TwipsToPels (const CSize & pSize, bool pLogUnits)
{
	return TwipsToPels (_MY_DC_HANDLE, pSize, pLogUnits);
}
CSize CTextSize::PelsToTwips (const CSize & pSize, bool pLogUnits)
{
	return PelsToTwips (_MY_DC_HANDLE, pSize, pLogUnits);
}
CRect CTextSize::TwipsToPels (const CRect & pRect, bool pLogUnits)
{
	return TwipsToPels (_MY_DC_HANDLE, pRect, pLogUnits);
}
CRect CTextSize::PelsToTwips (const CRect & pRect, bool pLogUnits)
{
	return PelsToTwips (_MY_DC_HANDLE, pRect, pLogUnits);
}

//////////////////////////////////////////////////////////////////////

CPoint CTextSize::DeciPointsToPels (const CPoint & pPoint, bool pLogUnits)
{
	return DeciPointsToPels (_MY_DC_HANDLE, pPoint, pLogUnits);
}
CPoint CTextSize::PelsToDeciPoints (const CPoint & pPoint, bool pLogUnits)
{
	return PelsToDeciPoints (_MY_DC_HANDLE, pPoint, pLogUnits);
}
CSize CTextSize::DeciPointsToPels (const CSize & pSize, bool pLogUnits)
{
	return DeciPointsToPels (_MY_DC_HANDLE, pSize, pLogUnits);
}
CSize CTextSize::PelsToDeciPoints (const CSize & pSize, bool pLogUnits)
{
	return PelsToDeciPoints (_MY_DC_HANDLE, pSize, pLogUnits);
}
CRect CTextSize::DeciPointsToPels (const CRect & pRect, bool pLogUnits)
{
	return DeciPointsToPels (_MY_DC_HANDLE, pRect, pLogUnits);
}
CRect CTextSize::PelsToDeciPoints (const CRect & pRect, bool pLogUnits)
{
	return PelsToDeciPoints (_MY_DC_HANDLE, pRect, pLogUnits);
}
