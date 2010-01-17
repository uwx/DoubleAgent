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
#include <math.h>
#include "HelperTemplates.h"
#include "BitmapAlpha.h"
#include "BitmapBuffer.h"
#include "Color.h"
#include "Log.h"

#pragma comment(lib, "msimg32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

bool CBitmapAlpha::AlphaBlend (HDC pDst, const CRect & pDstRect, HDC pSrc, const CRect & pSrcRect, BYTE pAlpha, bool pAllowInternal)
{
	return AlphaBlend (pDst, pDstRect.left, pDstRect.top, pDstRect.Width (), pDstRect.Height (), pSrc, pSrcRect.left, pSrcRect.top, pSrcRect.Width (), pSrcRect.Height (), pAlpha, pAllowInternal);
}

bool CBitmapAlpha::AlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha, bool pAllowInternal)
{
	bool	lRet = false;

	try
	{
		BLENDFUNCTION	lBlend = {AC_SRC_OVER, 0, pAlpha, AC_SRC_ALPHA};
		CRect			lDstRect (CPoint (pDstX, pDstY), CSize (pDstCx, pDstCy));

		if	(!RectVisible (pDst, &lDstRect))
		{
			lRet = true;
		}
		else
#ifndef	_UNICODE
		if	(pAllowInternal)
		{
			bool	lLogSizeMismatch = false;
			bool	lDevSizeMismatch = false;

			if	(InternalAlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, pSrc, pSrcX, pSrcY, pSrcCx, pSrcCy, pAlpha, lLogSizeMismatch, lDevSizeMismatch))
			{
				lRet = true;
			}
			else
			if	(!lLogSizeMismatch)
			{
				static CBitmapBuffer	lAlphaBuffer;
				CRect					lSrcRect (CPoint (pSrcX, pSrcY), CSize (pSrcCx, pSrcCy));
				CPoint					lWinOrg;
				CPoint					lVptOrg;
				CSize					lVptExt;
				CSize					lWinExt;
				CPoint					lAbsOrg (pDstX, pDstY);

				LPtoDP (pDst, (LPPOINT) &lDstRect, 2);
				LPtoDP (pSrc, (LPPOINT) &lSrcRect, 2);

				if	(lAlphaBuffer.CreateBuffer (lDstRect.Size (), true))
				{
					GetWindowOrgEx (pDst, &lWinOrg);
					GetViewportOrgEx (pDst, &lVptOrg);
					GetWindowExtEx (pDst, &lWinExt);
					GetViewportExtEx (pDst, &lVptExt);

					lAlphaBuffer.mDC.SetMapMode (GetMapMode (pDst));
					lAlphaBuffer.mDC.SetWindowExt (lWinExt);
					lAlphaBuffer.mDC.SetViewportExt (lVptExt);

					BitBlt (lAlphaBuffer.mDC, 0, 0, pSrcCx, pSrcCy, pSrc, pSrcX, pSrcY, SRCCOPY);

					lAlphaBuffer.mDC.SetWindowOrg (lWinOrg.x, lWinOrg.y);
					lAlphaBuffer.mDC.LPtoDP (&lAbsOrg);
					lAlphaBuffer.mDC.SetViewportOrg (-lAbsOrg.x, -lAbsOrg.y);

#ifdef	_DEBUG
					if	(LogIsActive (LogVerbose))
					{
						lSrcRect = CRect (CPoint (pDstX, pDstY), CSize (pSrcCx, pSrcCy));
						lAlphaBuffer.mDC.LPtoDP (&lSrcRect);
						LogMessage (LogNormal, _T("InternalAlpha failed [%u] [%u] WinOrg [%d %d] VptOrg [%d %d] WinExt [%d %d] VptExt [%d %d] Src [%d,%d %d,%d] Dst [%d,%d %d,%d]"), lLogSizeMismatch, lDevSizeMismatch, lWinOrg.x, lWinOrg.y, lVptOrg.x, lVptOrg.y, lWinExt.cx, lWinExt.cy, lVptExt.cx, lVptExt.cy, lSrcRect.left, lSrcRect.top, lSrcRect.Width (), lSrcRect.Height (), lDstRect.left, lDstRect.top, lDstRect.Width (), lDstRect.Height ());
					}
#endif
					if	(InternalAlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, lAlphaBuffer.mDC, pDstX, pDstY, pDstCx, pDstCy, pAlpha, lLogSizeMismatch, lDevSizeMismatch))
					{
						lRet = true;
					}
					else
					if	(
							(!lLogSizeMismatch)
						&&	(!lDevSizeMismatch)
						)
					{
						lAlphaBuffer.mDC.SetWindowOrg (0, 0);
						lAlphaBuffer.mDC.SetViewportOrg (0, 0);
						SetLastError (0);

						if	(::AlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, lAlphaBuffer.mDC, 0, 0, pDstCx, pDstCy, lBlend))
						{
							lRet = true;
						}
						else
						{
#ifdef	_DEBUG
							LogWinErr (LogAlways, GetLastError (), _T("AlphaBlend"));
#else
							LogWinErr (LogVerbose, GetLastError (), _T("AlphaBlend"));
#endif
						}
					}
					lAlphaBuffer.EndBuffer ();
				}
			}
		}
		else
#endif
		if	(::AlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, pSrc, pSrcX, pSrcY, pSrcCx, pSrcCy, lBlend))
		{
			lRet = true;
		}
		else
		{
			LogWinErr (LogNormal, GetLastError (), _T("AlphaBlend"));
		}
	}
	catch AnyExceptionSilent

#ifdef	_DEBUG
	if	(
			(!lRet)
		&&	(LogIsActive ())
		)
	{
		CRect lClipBox;
		GetClipBox (pDst, &lClipBox);
		LogMessage (LogNormal, _T("AlphaBlend [%d,%d %d,%d] <- [%d,%d %d,%d] (%p [%d %d %d %d] <- %p [%u]) failed"), pDstX, pDstY, pDstCx, pDstCy, pSrcX, pSrcY, pSrcCx, pSrcCy, pDst, lClipBox.left, lClipBox.top, lClipBox.right, lClipBox.bottom, pSrc, pAlpha);
	}
#endif

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CBitmapAlpha::InternalAlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha, bool & pLogSizeMismatch, bool & pDevSizeMismatch)
{
	bool	lRet = false;
	CRect	lDstRect (CPoint (pDstX, pDstY), CSize (pDstCx, pDstCy));
	CRect	lSrcRect (CPoint (pSrcX, pSrcY), CSize (pSrcCx, pSrcCy));
	HBITMAP	lDstBitmap;
	HBITMAP	lSrcBitmap;
	BITMAP	lDstBmpInfo;
	BITMAP	lSrcBmpInfo;
	LPBYTE	lDstBmpBits;
	LPBYTE	lSrcBmpBits;

	pLogSizeMismatch = false;
	pDevSizeMismatch = false;

	try
	{
		if	(
				(lDstRect.Width () != lSrcRect.Width ())
			||	(lDstRect.Height () != lSrcRect.Height ())
			)
		{
			pLogSizeMismatch = true;
		}

		LPtoDP (pDst, (LPPOINT) &lDstRect, 2);
		LPtoDP (pSrc, (LPPOINT) &lSrcRect, 2);

		if	(
				(lDstRect.Width () != lSrcRect.Width ())
			||	(lDstRect.Height () != lSrcRect.Height ())
			)
		{
			pDevSizeMismatch = true;
		}
		else
		{
			if	(
					(GetObjectType ((HGDIOBJ) pDst) == OBJ_MEMDC)
				&&	(GetObjectType ((HGDIOBJ) pSrc) == OBJ_MEMDC)
				&&	(lDstBitmap = (HBITMAP) GetCurrentObject (pDst, OBJ_BITMAP))
				&&	(lSrcBitmap = (HBITMAP) GetCurrentObject (pSrc, OBJ_BITMAP))
				&&	(GetObject (lDstBitmap, sizeof (BITMAP), &lDstBmpInfo))
				&&	(GetObject (lSrcBitmap, sizeof (BITMAP), &lSrcBmpInfo))
				&&	(
						(lDstBmpInfo.bmBitsPixel == 32)
					||	(lDstBmpInfo.bmBitsPixel == 24)
					)
				&&	(lSrcBmpInfo.bmBitsPixel == 32)
				&&	(lDstBmpBits = (LPBYTE) lDstBmpInfo.bmBits)
				&&	(lSrcBmpBits = (LPBYTE) lSrcBmpInfo.bmBits)
				)
			{
				lRet = true;
			}
			else
			{
				LogMessage (LogVerbose, _T("InternalAlphaBlend - Can't do"));
			}
		}
	}
	catch AnyExceptionSilent

	if	(lRet)
	{
		CPoint	lDstPoint;
		CPoint	lSrcPoint;
		long	lDstNdx;
		long	lSrcNdx;
		long	lDstBytesPixel = (lDstBmpInfo.bmBitsPixel == 32) ? 4 : 3;
		UINT	lConstantAlpha = pAlpha;
		UINT	lSrcAlpha;
		UINT	lDstAlpha;

		try
		{
#ifdef	_DEBUG_NOT
			LogMessage (LogNormal, _T("Alpha [%u] [%d,%d %d,%d] [%d,%d] in [%d,%d %d,%d] [%d,%d] to [%d,%d %d,%d] [%d,%d] in [%d,%d %d,%d] [%d,%d]"), pAlpha, lSrcRect.left, lSrcRect.top, lSrcRect.right, lSrcRect.bottom, lSrcRect.Width (), lSrcRect.Height (), lSrcClipBox.left, lSrcClipBox.top, lSrcClipBox.right, lSrcClipBox.bottom, lSrcClipBox.Width (), lSrcClipBox.Height (), lDstRect.left, lDstRect.top, lDstRect.right, lDstRect.bottom, lDstRect.Width (), lDstRect.Height (), lDstClipBox.left, lDstClipBox.top, lDstClipBox.right, lDstClipBox.bottom, lDstClipBox.Width (), lDstClipBox.Height ());
			if	(!lDstRect.EqualRect (&CRect (CPoint (pDstX, pDstY), CSize (pDstCx, pDstCy))))
			{
				LogMessage (LogNormal, _T("  LogDst [%d,%d %d,%d] DevDst [%d,%d %d,%d]"), pDstX, pDstY, pDstX+pDstCx, pDstY+pDstCy, lDstRect.left, lDstRect.top, lDstRect.right, lDstRect.bottom);
			}
			if	(!lSrcRect.EqualRect (&CRect (CPoint (pSrcX, pSrcY), CSize (pSrcCx, pSrcCy))))
			{
				LogMessage (LogNormal, _T("  LogSrc [%d,%d %d,%d] DevSrc [%d,%d %d,%d]"), pSrcX, pSrcY, pSrcX+pSrcCx, pSrcY+pSrcCy, lSrcRect.left, lSrcRect.top, lSrcRect.right, lSrcRect.bottom);
			}
#endif

			for	(lDstPoint.y = lDstRect.top, lSrcPoint.y = lSrcRect.top; (lDstPoint.y < lDstRect.bottom) && (lSrcPoint.y < lSrcRect.bottom); lDstPoint.y++, lSrcPoint.y++)
			{
				if	(
						(lDstPoint.y < 0)
					||	(lDstPoint.y >= lDstBmpInfo.bmHeight)
					||	(lSrcPoint.y < 0)
					||	(lSrcPoint.y >= lSrcBmpInfo.bmHeight)
					)
				{
					continue;
				}

				for	(lDstPoint.x = lDstRect.left, lSrcPoint.x = lSrcRect.left; (lDstPoint.x < lDstRect.right) && (lSrcPoint.x < lSrcRect.right); lDstPoint.x++, lSrcPoint.x++)
				{
					if	(
							(lDstPoint.x < 0)
						||	(lDstPoint.x >= lDstBmpInfo.bmWidth)
						||	(lSrcPoint.x < 0)
						||	(lSrcPoint.x >= lSrcBmpInfo.bmWidth)
						)
					{
						continue;
					}

					lSrcNdx = ((lSrcBmpInfo.bmHeight - lSrcPoint.y - 1L) * lSrcBmpInfo.bmWidthBytes) + (lSrcPoint.x * 4);
					lSrcAlpha = (UINT) lSrcBmpBits [lSrcNdx+3] * lConstantAlpha / (UINT) 255;

					if	(lSrcAlpha == 0)
					{
						continue;
					}

					lDstNdx = ((lDstBmpInfo.bmHeight - lDstPoint.y - 1L) * lDstBmpInfo.bmWidthBytes) + (lDstPoint.x * lDstBytesPixel);
					lDstAlpha = (UINT) 255 - lSrcAlpha;

					lDstBmpBits [lDstNdx] = (BYTE) min (((UINT) lDstBmpBits [lDstNdx] * lDstAlpha / (UINT) 255) + (UINT) lSrcBmpBits [lSrcNdx] * lConstantAlpha / (UINT) 255, (UINT) 255);
					lDstBmpBits [lDstNdx+1] = (BYTE) min (((UINT) lDstBmpBits [lDstNdx+1] * lDstAlpha / (UINT) 255) + (UINT) lSrcBmpBits [lSrcNdx+1] * lConstantAlpha / (UINT) 255, (UINT) 255);
					lDstBmpBits [lDstNdx+2] = (BYTE) min (((UINT) lDstBmpBits [lDstNdx+2] * lDstAlpha / (UINT) 255) + (UINT) lSrcBmpBits [lSrcNdx+2] * lConstantAlpha / (UINT) 255, (UINT) 255);
					if	(lDstBmpInfo.bmBitsPixel == 32)
					{
						lDstBmpBits [lDstNdx+3] = (BYTE) min (((UINT) lDstBmpBits [lDstNdx+3] * lDstAlpha / (UINT) 255) + lSrcAlpha, (UINT) 255);
					}
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CBrush * CBitmapAlpha::GetAlphaBrush (HDC pDC, COLORREF pColor, BYTE pAlpha, bool pAlphaPrecalc)
{
	tPtr <CBrush>	lBrush;
	CBitmap			lBrushBmp;
	LPBYTE			lBrushBits = NULL;
	HGLOBAL			lBrushDibHandle = 0;
	LPBYTE			lBrushDib;
	tS <LOGBRUSH>	lBrushInfo;
	UINT			lBitNdx;
	tS <BITMAPINFO>	lBmpInfo;
	CDC				lMemDC;

	lBmpInfo.bmiHeader.biSize = sizeof (lBmpInfo.bmiHeader);
	lBmpInfo.bmiHeader.biPlanes = 1;
	lBmpInfo.bmiHeader.biBitCount = 32;
	lBmpInfo.bmiHeader.biWidth = 8;
	lBmpInfo.bmiHeader.biHeight = 8;
	lBmpInfo.bmiHeader.biCompression = BI_RGB;
	lBmpInfo.bmiHeader.biSizeImage = 256;

	if	(
			(!pDC)
		&&	(lMemDC.CreateCompatibleDC (0))
		)
	{
		pDC = lMemDC;
	}

	if	(
			(lBrush = new CBrush)
		&&	(lBrushBmp.Attach (CreateDIBSection (pDC, &lBmpInfo, DIB_RGB_COLORS, (void **) &lBrushBits, NULL, NULL)))
		)
	{
		GdiFlush ();

		for	(lBitNdx = 0; lBitNdx < lBmpInfo.bmiHeader.biSizeImage; lBitNdx+=4)
		{
			if	(pAlphaPrecalc)
			{
				lBrushBits [lBitNdx] = (BYTE) ((UINT) GetBValue (pColor) * (UINT) pAlpha / 255);
				lBrushBits [lBitNdx+1] = (BYTE) ((UINT) GetGValue (pColor) * (UINT) pAlpha / 255);
				lBrushBits [lBitNdx+2] = (BYTE) ((UINT) GetRValue (pColor) * (UINT) pAlpha / 255);
			}
			else
			{
				lBrushBits [lBitNdx] = GetBValue (pColor);
				lBrushBits [lBitNdx+1] = GetGValue (pColor);
				lBrushBits [lBitNdx+2] = GetRValue (pColor);
			}
			lBrushBits [lBitNdx+3] = pAlpha;
		}

#ifdef	_DEBUG_NOT
		CBitmapDebugger::DumpBitmap (LogNormal, lBmpInfo, lBrushBits, _T("Brush Bits"));
#endif

		if	(lBrushDibHandle = GlobalAlloc (GHND, sizeof (lBmpInfo) + lBmpInfo.bmiHeader.biSizeImage))
		{
			lBrushDib = (LPBYTE) GlobalLock (lBrushDibHandle);
			memcpy (lBrushDib, &lBmpInfo.bmiHeader, sizeof (lBmpInfo.bmiHeader));
			memcpy (lBrushDib+sizeof (lBmpInfo.bmiHeader), lBrushBits, lBmpInfo.bmiHeader.biSizeImage);
			GlobalUnlock (lBrushDibHandle);

			lBrushInfo.Clear ();
			lBrushInfo.lbStyle = BS_DIBPATTERN;
			lBrushInfo.lbHatch = (ULONG) lBrushDib;
			if	(!lBrush->CreateBrushIndirect (&lBrushInfo))
			{
				LogWinErr (LogNormal, GetLastError ());
				lBrush = NULL;
			}

			GlobalFree (lBrushDibHandle);
		}
		else
		{
			LogWinErr (LogNormal, GetLastError ());
			lBrush = NULL;
		}
	}
	else
	{
		LogWinErr (LogNormal, GetLastError ());
		lBrush = NULL;
	}

	if	(lMemDC.GetSafeHdc ())
	{
		lMemDC.DeleteDC ();
	}

	return lBrush.Detach ();
}

//////////////////////////////////////////////////////////////////////

bool CBitmapAlpha::AlphaScaleBitmap (CBitmap & pBitmap, BYTE pAlpha, LPBYTE * pBitmapBits)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		)
	{
		try
		{
			CBitmapBuffer	lSrcBuffer;
			CBitmapBuffer	lTrgBuffer;

			if	(
					(lSrcBuffer.mBitmap.Attach (pBitmap.GetSafeHandle ()))
				&&	(lSrcBuffer.StartBuffer ())
				&&	(lTrgBuffer.CreateBuffer (CSize (lBitmap.bmWidth, lBitmap.bmHeight), true))
				)
			{
				AlphaBlend (lTrgBuffer.mDC, 0, 0, lBitmap.bmWidth, lBitmap.bmHeight, lSrcBuffer.mDC, 0, 0, lBitmap.bmWidth, lBitmap.bmHeight, pAlpha, false);
				lRet = true;
			}

			lTrgBuffer.EndBuffer ();
			lSrcBuffer.EndBuffer ();
			lSrcBuffer.mBitmap.Detach ();

			if	(lRet)
			{
				lSrcBuffer.mBitmap.Attach (pBitmap.Detach ());
				pBitmap.Attach (lTrgBuffer.mBitmap.Detach ());
				if	(pBitmapBits)
				{
					(*pBitmapBits) = lTrgBuffer.mBitmapBits;
				}
			}

			lTrgBuffer.EndBuffer (true, true);
			lSrcBuffer.EndBuffer (true, true);
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CBitmapAlpha::AlphaClearBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pClearColor, BYTE pAlpha)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		COLORREF *	lBitmapBits = (COLORREF *) pBitmapBits;
		COLORREF	lClearColor = (pClearColor & 0x00FFFFFF) | (((COLORREF) pAlpha) << 24);
		long		lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes / sizeof (COLORREF);
		long		lNdx;

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx++)
			{
				lBitmapBits [lNdx] = lClearColor;
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CBitmapAlpha::AlphaFillBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pAlpha, bool pClearBlack, bool pClearWhite)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;

		try
		{
			if	(
					(pClearBlack)
				||	(pClearWhite)
				)
			{
				for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
				{
					if	(
							(pClearBlack)
						&&	(pBitmapBits [lNdx] == 0)
						&&	(pBitmapBits [lNdx+1] == 0)
						&&	(pBitmapBits [lNdx+2] == 0)
						)
					{
						pBitmapBits [lNdx+3] = 0;
						continue;
					}

					if	(
							(pClearWhite)
						&&	(pBitmapBits [lNdx] == 255)
						&&	(pBitmapBits [lNdx+1] == 255)
						&&	(pBitmapBits [lNdx+2] == 255)
						)
					{
						pBitmapBits [lNdx] = 0;
						pBitmapBits [lNdx+1] = 0;
						pBitmapBits [lNdx+2] = 0;
						pBitmapBits [lNdx+3] = 0;
						continue;
					}

					pBitmapBits [lNdx+3] = pAlpha;
				}
			}
			else
			{
				for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
				{
					pBitmapBits [lNdx] = pAlpha;
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CBitmapAlpha::AlphaSaturateBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pMinAlpha, BYTE pMaxAlpha)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				pBitmapBits [lNdx] = pBitmapBits [lNdx+3];
				pBitmapBits [lNdx+1] = pBitmapBits [lNdx+3];
				pBitmapBits [lNdx+2] = pBitmapBits [lNdx+3];
				pBitmapBits [lNdx+3] = min (max (pBitmapBits [lNdx+3], pMinAlpha), pMaxAlpha);
			}

			lRet = true;
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

bool CBitmapAlpha::AlphaInvertBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, bool pClearBlack, bool pSkipBlack, bool pSkipWhite)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;

		try
		{
			if	(
					(pClearBlack)
				||	(pSkipBlack)
				||	(pSkipWhite)
				)
			{
				for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
				{
					if	(
							(pSkipBlack)
						&&	(pBitmapBits [lNdx] == 0)
						&&	(pBitmapBits [lNdx+1] == 0)
						&&	(pBitmapBits [lNdx+2] == 0)
						)
					{
						continue;
					}

					if	(
							(pSkipWhite)
						&&	(pBitmapBits [lNdx] == 255)
						&&	(pBitmapBits [lNdx+1] == 255)
						&&	(pBitmapBits [lNdx+2] == 255)
						)
					{
						continue;
					}

					pBitmapBits [lNdx+3] = 255-pBitmapBits [lNdx+3];

					if	(
							(pClearBlack)
						&&	(pBitmapBits [lNdx+3] == 0)
						)
					{
						pBitmapBits [lNdx] = 0;
						pBitmapBits [lNdx+1] = 0;
						pBitmapBits [lNdx+2] = 0;
					}
				}
			}
			else
			{
				for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
				{
					pBitmapBits [lNdx] = 255-pBitmapBits [lNdx];
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CBitmapAlpha::AlphaFixBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pMinAlpha)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;

		try
		{
			for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
			{
				if	(pBitmapBits [lNdx] <= pMinAlpha)
				{
					pBitmapBits [lNdx-1] = 0;
					pBitmapBits [lNdx-2] = 0;
					pBitmapBits [lNdx-3] = 0;
					lRet = true;
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

bool CBitmapAlpha::AlphaClampBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pMinAlpha, BYTE pMaxAlpha, BYTE pNewAlpha)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;

		try
		{
			for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
			{
				if	(
						(pBitmapBits [lNdx] >= pMinAlpha)
					&&	(pBitmapBits [lNdx] <= pMaxAlpha)
					)
				{
					pBitmapBits [lNdx] = pNewAlpha;
					lRet = true;
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CBitmapAlpha::AlphaFeatherBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, const CRect & pCenter, BYTE pAlpha, bool pRectangular, bool pPrecalc)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		try
		{
			CPoint	lPixel;
			CRect	lFeather;
			bool	lXEdge;
			bool	lYEdge;
			long	lNdx;
			long	lAlphaOld;
			long	lAlphaNew;
			long	lAlphaX;
			long	lAlphaY;
			long	lAlphaMax = min ((long) pAlpha, 255);

			lFeather.left = (pCenter.left+1)*(pCenter.left+1);
			lFeather.top = (pCenter.top+1)*(pCenter.top+1);
			lFeather.right = (lBitmap.bmWidth-pCenter.right+1)*(lBitmap.bmWidth-pCenter.right+1);
			lFeather.bottom = (lBitmap.bmHeight-pCenter.bottom+1)*(lBitmap.bmHeight-pCenter.bottom+1);

			for	(lPixel.x = 0; lPixel.x < lBitmap.bmWidth; lPixel.x++)
			{
				for	(lPixel.y = 0; lPixel.y < lBitmap.bmHeight; lPixel.y++)
				{
					lNdx = ((lBitmap.bmHeight-lPixel.y-1) * lBitmap.bmWidthBytes) + (lPixel.x * 4) + 3;
					lAlphaOld = (long) pBitmapBits [lNdx];

					if	(lAlphaOld)
					{
						lXEdge = true;
						lYEdge = true;

						if	(lPixel.x < pCenter.left)
						{
							lAlphaX = lAlphaOld - MulDiv (lAlphaOld, (pCenter.left-lPixel.x)*(pCenter.left-lPixel.x), lFeather.left);
						}
						else
						if	(lPixel.x >= pCenter.right)
						{
							lAlphaX = lAlphaOld - MulDiv (lAlphaOld, (lPixel.x-pCenter.right+1)*(lPixel.x-pCenter.right+1), lFeather.right);
						}
						else
						{
							lAlphaX = lAlphaOld;
							lXEdge = false;
						}

						if	(lPixel.y < pCenter.top)
						{
							lAlphaY = lAlphaOld - MulDiv (lAlphaOld, (pCenter.top-lPixel.y)*(pCenter.top-lPixel.y), lFeather.top);
						}
						else
						if	(lPixel.y >= pCenter.bottom)
						{
							lAlphaY = lAlphaOld - MulDiv (lAlphaOld, (lPixel.y-pCenter.bottom+1)*(lPixel.y-pCenter.bottom+1), lFeather.bottom);
						}
						else
						if	(lXEdge)
						{
							lAlphaY = lAlphaOld;
							lYEdge = false;
						}
						else
						{
							lPixel.y = max (lPixel.y, pCenter.bottom-1);
							continue;
						}

						if	(pRectangular)
						{
							lAlphaNew = min (lAlphaX, lAlphaY);
						}
						else
						if	(!lXEdge)
						{
							lAlphaNew = lAlphaY;
						}
						else
						if	(!lYEdge)
						{
							lAlphaNew = lAlphaX;
						}
						else
						{
							lAlphaNew = min (max (lAlphaOld - dtol (pow (_hypot (sqrt((double)(lAlphaOld-lAlphaX)), sqrt((double)(lAlphaOld-lAlphaY))), 2.0)), 0), lAlphaOld);
						}

						lAlphaNew = min (max (MulDiv (lAlphaNew, lAlphaMax, 255), 0), 255);

						//LogMessage (LogDebugFast, _T("Pixel [%d %d] Alpha [%u] to [%u]"), lPixel.x, lPixel.y, lAlphaOld, lAlphaNew);

						if	(lAlphaNew == 0)
						{
							pBitmapBits [lNdx] = 0;
							pBitmapBits [lNdx-1] = 0;
							pBitmapBits [lNdx-2] = 0;
							pBitmapBits [lNdx-3] = 0;
						}
						else
						if	(lAlphaNew != lAlphaOld)
						{
							pBitmapBits [lNdx] = (BYTE) lAlphaNew;
							if	(pPrecalc)
							{
								pBitmapBits [lNdx-1] = (BYTE) min (MulDiv ((long) pBitmapBits [lNdx-1], lAlphaNew, lAlphaOld), 255);
								pBitmapBits [lNdx-2] = (BYTE) min (MulDiv ((long) pBitmapBits [lNdx-2], lAlphaNew, lAlphaOld), 255);
								pBitmapBits [lNdx-3] = (BYTE) min (MulDiv ((long) pBitmapBits [lNdx-3], lAlphaNew, lAlphaOld), 255);
							}
						}
					}
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

bool CBitmapAlpha::AlphaCalcBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pOpaqueColor, COLORREF pSaturateColor)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmap.bmBits)
			)
		)
	{
		long		lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long		lNdx;
		long		lOpaqueTotal = (long) GetRValue (pOpaqueColor) + (long) GetGValue (pOpaqueColor) + (long) GetBValue (pOpaqueColor);
		long		lOpaqueAlpha = (long) (pOpaqueColor >> 24);
		long		lBitTotal;
		long		lBitAlpha;
		HSLTRIPLE	lBitColor;
		HSLTRIPLE	lSaturateColor;

		if	(lOpaqueTotal == 0)
		{
			lOpaqueTotal = 255;
		}
		if	(lOpaqueAlpha == 0)
		{
			lOpaqueAlpha = 255;
		}
		if	(pSaturateColor)
		{
			CColor (pSaturateColor).GetHSL (lSaturateColor);
		}

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				lBitTotal = (long) pBitmapBits [lNdx] + (long) pBitmapBits [lNdx+1] + (long) pBitmapBits [lNdx+2];
//				lBitAlpha = MulDiv ((255 * 3) - labs (lBitTotal - lOpaqueTotal), 255, lOpaqueAlpha * 3);
				lBitAlpha = MulDiv (lBitTotal, lOpaqueAlpha, lOpaqueTotal);
				lBitAlpha = max (min (lBitAlpha, 255), 0);

				if	(lBitAlpha > 0)
				{
					pBitmapBits [lNdx+3] = (BYTE) lBitAlpha;

					if	(pSaturateColor)
					{
						lBitColor.Saturation = lSaturateColor.Saturation * (float) (UINT) lBitAlpha / 255.0f;
						CColor::HslToRgb (lSaturateColor.Hue, lBitColor.Saturation, lSaturateColor.Luminosity, pBitmapBits [lNdx+2], pBitmapBits [lNdx+1], pBitmapBits [lNdx]);

						pBitmapBits [lNdx] = (BYTE) MulDiv (pBitmapBits [lNdx], lBitAlpha, 255);
						pBitmapBits [lNdx+1] = (BYTE) MulDiv (pBitmapBits [lNdx+1], lBitAlpha, 255);
						pBitmapBits [lNdx+2] = (BYTE) MulDiv (pBitmapBits [lNdx+2], lBitAlpha, 255);
					}
				}
				else
				{
					pBitmapBits [lNdx] = 0;
					pBitmapBits [lNdx+1] = 0;
					pBitmapBits [lNdx+2] = 0;
					pBitmapBits [lNdx+3] = 0;
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CBitmapAlpha::AlphaHitTestBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, const CPoint & pPoint, const CRect & pBounds, const CRect * pAperture, BYTE pMinAlpha, BYTE pMaxAlpha)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmapDef;

	if	(
			(pBounds.PtInRect (pPoint))
		&&	(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmapDef))
		&&	(lBitmapDef.bmBitsPixel == 32)
		&&	(
				(pBitmapBits)
			||	(pBitmapBits = (LPBYTE) lBitmapDef.bmBits)
			)
		)
	{
		try
		{
			CPoint	lPoint (pPoint);
			long	lNdx;

			lPoint -= pBounds.TopLeft ();
			lNdx = ((lBitmapDef.bmHeight-lPoint.y-1) * lBitmapDef.bmWidthBytes) + (lPoint.x * 4) + 3;

			if	(
					(pBitmapBits [lNdx] >= pMinAlpha)
				&&	(pBitmapBits [lNdx] <= pMaxAlpha)
				)
			{
				lRet = true;
			}
			else
			if	(pAperture)
			{
				CRect	lAperture (pPoint, pPoint);

				lAperture.InflateRect (*pAperture);
				lAperture.IntersectRect (&lAperture, &pBounds);
				lAperture.OffsetRect (-pBounds.left, -pBounds.top);

				for	(lPoint.x = lAperture.left; lPoint.x < lAperture.right; lPoint.x += 3)
				{
					for	(lPoint.y = lAperture.top; lPoint.y < lAperture.bottom; lPoint.y += 3)
					{
						lNdx = ((lBitmapDef.bmHeight-lPoint.y-1) * lBitmapDef.bmWidth * 4) + (lPoint.x * 4) + 3;

						if	(
								(pBitmapBits [lNdx] >= pMinAlpha)
							&&	(pBitmapBits [lNdx] <= pMaxAlpha)
							)
						{
							lRet = true;
							break;
						}
					}
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CBitmapAlpha::AlphaPreDrawBuffer (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pFillColor)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(pBitmapBits)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;
		BYTE	lFillRed = GetRValue (pFillColor);
		BYTE	lFillGreen = GetGValue (pFillColor);
		BYTE	lFillBlue = GetBValue (pFillColor);

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				pBitmapBits [lNdx] = lFillBlue;
				pBitmapBits [lNdx+1] = lFillGreen;
				pBitmapBits [lNdx+2] = lFillRed;
				pBitmapBits [lNdx+3] = 1;
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CBitmapAlpha::AlphaPostDrawBuffer (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pMaskColor, bool pAlphaPreCalc)
{
	bool		lRet = false;
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmBitsPixel == 32)
		&&	(pBitmapBits)
		)
	{
		long	lMaxNdx = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		long	lNdx;
		BYTE	lMaskRed = GetRValue (pMaskColor);
		BYTE	lMaskGreen = GetGValue (pMaskColor);
		BYTE	lMaskBlue = GetBValue (pMaskColor);
		BYTE	lMaskAlpha = ((RGBQUAD *) &pMaskColor)->rgbReserved;

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				if	(pBitmapBits [lNdx+3] == lMaskAlpha)
				{
					pBitmapBits [lNdx+3] = 255;
				}
				else
				if	(pBitmapBits [lNdx+3] == 1)
				{
					pBitmapBits [lNdx] = lMaskBlue;
					pBitmapBits [lNdx+1] = lMaskGreen;
					pBitmapBits [lNdx+2] = lMaskRed;
					pBitmapBits [lNdx+3] = 0;
				}
				else
				if	(pAlphaPreCalc)
				{
					pBitmapBits [lNdx] = (BYTE) MulDiv ((long) pBitmapBits [lNdx], (long) pBitmapBits [lNdx+3], 255);
					pBitmapBits [lNdx+1] = (BYTE) MulDiv ((long) pBitmapBits [lNdx+1], (long) pBitmapBits [lNdx+3], 255);
					pBitmapBits [lNdx+2] = (BYTE) MulDiv ((long) pBitmapBits [lNdx+2], (long) pBitmapBits [lNdx+3], 255);
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}
