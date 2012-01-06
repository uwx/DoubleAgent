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
#include "StdAfx.h"
#pragma unmanaged
#include <math.h>
#include "HelperTemplates.h"
#include "ImageAlpha.h"
#include "ImageTools.h"
#include "Color.h"
#include "Log.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#endif

#pragma comment(lib, "msimg32.lib")

#ifdef	_DEBUG
//#define	_DEBUG_INTERNAL_ALPHA	LogNormal|LogHighVolume
#endif

//////////////////////////////////////////////////////////////////////

bool CImageAlpha::AlphaBlend (HDC pDst, const CRect& pDstRect, HDC pSrc, const CRect& pSrcRect, BYTE pAlpha, bool pAllowInternal)
{
	return AlphaBlend (pDst, pDstRect.left, pDstRect.top, pDstRect.Width (), pDstRect.Height (), pSrc, pSrcRect.left, pSrcRect.top, pSrcRect.Width (), pSrcRect.Height (), pAlpha, pAllowInternal);
}

bool CImageAlpha::AlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha, bool pAllowInternal)
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
		{
#ifdef	_DEBUG_INTERNAL_ALPHA
			if	(!pAllowInternal)
#else
#ifndef	_UNICODE
			if	(!pAllowInternal)
#endif
#endif
			{
				if	(::AlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, pSrc, pSrcX, pSrcY, pSrcCx, pSrcCy, lBlend))
				{
					lRet = true;
				}
				else
				{
					LogWinErr (LogVerbose|LogTime, GetLastError (), _T("AlphaBlend [%d %d %d %d] [%d] [%d %d %d %d] [%d] [%2.2X]"), pDstX, pDstY, pDstCx, pDstCy, GetDeviceCaps (pDst,BITSPIXEL), pSrcX, pSrcY, pSrcCx, pSrcCy, GetDeviceCaps (pSrc,BITSPIXEL), pAlpha);
				}
			}

			if	(
					(!lRet)
#ifndef	_DEBUG_INTERNAL_ALPHA
				&&	(pAllowInternal)
#endif
				)
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
					ATL::CImage	lTrgImage;
					CRect		lSrcRect (CPoint (pSrcX, pSrcY), CSize (pSrcCx, pSrcCy));
					CPoint		lWinOrg;
					CPoint		lVptOrg;
					CSize		lVptExt;
					CSize		lWinExt;
					CPoint		lAbsOrg (pDstX, pDstY);

					::LPtoDP (pDst, (LPPOINT) &lDstRect, 2);
					::LPtoDP (pSrc, (LPPOINT) &lSrcRect, 2);

					if	(CImageTools::CreateColorImage (lDstRect.Size (), lTrgImage, true))
					{
						ATL::CImageDC	lTrgDC (lTrgImage);

						::GetWindowOrgEx (pDst, &lWinOrg);
						::GetViewportOrgEx (pDst, &lVptOrg);
						::GetWindowExtEx (pDst, &lWinExt);
						::GetViewportExtEx (pDst, &lVptExt);

						::SetMapMode (lTrgDC, GetMapMode (pDst));
						::SetWindowExtEx (lTrgDC, lWinExt.cx, lWinExt.cy, NULL);
						::SetViewportExtEx (lTrgDC, lVptExt.cx, lVptExt.cy, NULL);

						::BitBlt (lTrgDC, 0, 0, pSrcCx, pSrcCy, pSrc, pSrcX, pSrcY, SRCCOPY);

						::SetWindowOrgEx (lTrgDC, lWinOrg.x, lWinOrg.y, NULL);
						::LPtoDP (lTrgDC, &lAbsOrg, 1);
						::SetViewportOrgEx (lTrgDC, -lAbsOrg.x, -lAbsOrg.y, NULL);

#ifdef	_DEBUG_INTERNAL_ALPHA
						if	(LogIsActive (_DEBUG_INTERNAL_ALPHA))
						{
							lSrcRect = CRect (CPoint (pDstX, pDstY), CSize (pSrcCx, pSrcCy));
							lAlphaBuffer.mDC.LPtoDP (&lSrcRect);
							LogMessage (LogNormal|LogTime, _T("InternalAlpha failed [%u] [%u] WinOrg [%d %d] VptOrg [%d %d] WinExt [%d %d] VptExt [%d %d] Src [%d,%d %d,%d] Dst [%d,%d %d,%d]"), lLogSizeMismatch, lDevSizeMismatch, lWinOrg.x, lWinOrg.y, lVptOrg.x, lVptOrg.y, lWinExt.cx, lWinExt.cy, lVptExt.cx, lVptExt.cy, lSrcRect.left, lSrcRect.top, lSrcRect.Width (), lSrcRect.Height (), lDstRect.left, lDstRect.top, lDstRect.Width (), lDstRect.Height ());
						}
#endif
						if	(InternalAlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, lTrgDC, pDstX, pDstY, pDstCx, pDstCy, pAlpha, lLogSizeMismatch, lDevSizeMismatch))
						{
							lRet = true;
						}
						else
						if	(
								(!lLogSizeMismatch)
							&&	(!lDevSizeMismatch)
							)
						{
							::SetWindowOrgEx (lTrgDC, 0, 0, NULL);
							::SetViewportOrgEx (lTrgDC, 0, 0, NULL);
							SetLastError (0);

							if	(::AlphaBlend (pDst, pDstX, pDstY, pDstCx, pDstCy, lTrgDC, 0, 0, pDstCx, pDstCy, lBlend))
							{
								lRet = true;
							}
							else
#ifdef	_DEBUG_INTERNAL_ALPHA
							if	(LogIsActive (_DEBUG_INTERNAL_ALPHA))
							{
								LogWinErr (MinLogLevel(_DEBUG_INTERNAL_ALPHA,LogAlways), GetLastError (), _T("AlphaBlend"));
							}
							else
#endif
							if	(LogIsActive ())
							{
								LogWinErr (LogVerbose|LogTime, GetLastError (), _T("AlphaBlend"));
							}
						}
					}
				}
			}
		}
	}
	catch AnyExceptionSilent

#ifdef	_DEBUG_INTERNAL_ALPHA
	if	(
			(!lRet)
		&&	(LogIsActive (_DEBUG_INTERNAL_ALPHA))
		)
	{
		CRect lClipBox;
		GetClipBox (pDst, &lClipBox);
		LogMessage (_DEBUG_INTERNAL_ALPHA, _T("AlphaBlend [%d,%d %d,%d] <- [%d,%d %d,%d] (%p [%d] [%d %d %d %d] <- %p [%d] [%2.2X]) failed"), pDstX, pDstY, pDstCx, pDstCy, pSrcX, pSrcY, pSrcCx, pSrcCy, pDst, GetDeviceCaps (pDst,BITSPIXEL), lClipBox.left, lClipBox.top, lClipBox.right, lClipBox.bottom, pSrc, GetDeviceCaps (pSrc,BITSPIXEL), pAlpha);
	}
#endif

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CImageAlpha::InternalAlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha, bool& pLogSizeMismatch, bool& pDevSizeMismatch)
{
	bool		lRet = false;
	CRect		lDstRect (CPoint (pDstX, pDstY), CSize (pDstCx, pDstCy));
	CRect		lSrcRect (CPoint (pSrcX, pSrcY), CSize (pSrcCx, pSrcCy));
	HBITMAP		lDstBitmap = NULL;
	HBITMAP		lSrcBitmap = NULL;
	tS <BITMAP>	lDstBmpInfo;
	tS <BITMAP>	lSrcBmpInfo;
	LPBYTE		lDstBmpBits = NULL;
	LPBYTE		lSrcBmpBits = NULL;

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
#ifdef	_DEBUG_INTERNAL_ALPHA
			else
			if	(LogIsActive (_DEBUG_INTERNAL_ALPHA))
			{
				LogMessage (_DEBUG_INTERNAL_ALPHA, _T("InternalAlphaBlend [%d %p %d %p] <- [%d %p %d %p] - Can't do"), GetObjectType ((HGDIOBJ) pDst), lDstBitmap, lDstBmpInfo.bmBitsPixel, lDstBmpInfo.bmBits, GetObjectType ((HGDIOBJ) pSrc), lSrcBitmap, lSrcBmpInfo.bmBitsPixel, lSrcBmpInfo.bmBits);
			}
#endif
			else
			if	(LogIsActive ())
			{
				LogMessage (LogVerbose|LogTime, _T("InternalAlphaBlend - Can't do"));
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
			LogMessage (LogNormal|LogTime, _T("Alpha [%u] [%d,%d %d,%d] [%d,%d] in [%d,%d %d,%d] [%d,%d] to [%d,%d %d,%d] [%d,%d] in [%d,%d %d,%d] [%d,%d]"), pAlpha, lSrcRect.left, lSrcRect.top, lSrcRect.right, lSrcRect.bottom, lSrcRect.Width (), lSrcRect.Height (), lSrcClipBox.left, lSrcClipBox.top, lSrcClipBox.right, lSrcClipBox.bottom, lSrcClipBox.Width (), lSrcClipBox.Height (), lDstRect.left, lDstRect.top, lDstRect.right, lDstRect.bottom, lDstRect.Width (), lDstRect.Height (), lDstClipBox.left, lDstClipBox.top, lDstClipBox.right, lDstClipBox.bottom, lDstClipBox.Width (), lDstClipBox.Height ());
			if	(!lDstRect.EqualRect (&CRect (CPoint (pDstX, pDstY), CSize (pDstCx, pDstCy))))
			{
				LogMessage (LogNormal|LogTime, _T("  LogDst [%d,%d %d,%d] DevDst [%d,%d %d,%d]"), pDstX, pDstY, pDstX+pDstCx, pDstY+pDstCy, lDstRect.left, lDstRect.top, lDstRect.right, lDstRect.bottom);
			}
			if	(!lSrcRect.EqualRect (&CRect (CPoint (pSrcX, pSrcY), CSize (pSrcCx, pSrcCy))))
			{
				LogMessage (LogNormal|LogTime, _T("  LogSrc [%d,%d %d,%d] DevSrc [%d,%d %d,%d]"), pSrcX, pSrcY, pSrcX+pSrcCx, pSrcY+pSrcCy, lSrcRect.left, lSrcRect.top, lSrcRect.right, lSrcRect.bottom);
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

HBRUSH CImageAlpha::GetAlphaBrush (COLORREF pColor, BYTE pAlpha, bool pAlphaPrecalc)
{
	HBRUSH			lBrush;
	ATL::CImage		lBrushImage;
	tS <LOGBRUSH>	lBrushInfo;
	LPBYTE			lBrushBits;
	tS <DIBSECTION> lBrushDib;
	CGlobalHandle	lBrushDibHandle;
	LPBYTE			lBrushDibPtr;
	int				lMaxNdx;
	int				lBitNdx;

	if	(
			(lBrushImage.Create (8, 8, 32))
		&&	(lBrushBits = GetImageBits (lBrushImage))
		)
	{
		lMaxNdx = GetImageBytes(lBrushImage);
		for	(lBitNdx = 0; lBitNdx < lMaxNdx; lBitNdx+=4)
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
		CImageDebugger::DumpBitmap (LogNormal|LogTime, lBmpInfo, lBrushBits, _T("Brush Bits"));
#endif
		if	(
				(::GetObject ((HBITMAP)lBrushImage, sizeof (DIBSECTION), &lBrushDib) == sizeof (DIBSECTION))
			&&	(lBrushDibHandle = GlobalAlloc (GHND, sizeof (lBrushDib.dsBmih) + lBrushDib.dsBmih.biSizeImage))
			)
		{
			lBrushDibPtr = (LPBYTE) GlobalLock (lBrushDibHandle);
			memcpy (lBrushDibPtr, &lBrushDib.dsBmih, sizeof (lBrushDib.dsBmih));
			memcpy (lBrushDibPtr+sizeof (lBrushDib.dsBmih), lBrushBits, lBrushDib.dsBmih.biSizeImage);
			GlobalUnlock (lBrushDibHandle);

			lBrushInfo.lbStyle = BS_DIBPATTERN;
			lBrushInfo.lbHatch = (ULONG) lBrushDibPtr;
			lBrush = CreateBrushIndirect (&lBrushInfo);
			if	(!lBrush)
			{
				LogWinErr (LogNormal|LogTime, GetLastError ());
			}
		}
		else
		{
			LogWinErr (LogNormal|LogTime, GetLastError ());
		}
	}
	else
	{
		LogWinErr (LogNormal|LogTime, GetLastError ());
	}
	return lBrush;
}

//////////////////////////////////////////////////////////////////////

bool CImageAlpha::AlphaScaleImage (ATL::CImage& pImage, BYTE pAlpha)
{
	bool	lRet = false;

	if	((HBITMAP)pImage)
	{
		try
		{
			CSize		lImageSize (pImage.GetWidth(), pImage.GetHeight());
			ATL::CImage	lTrgImage;

			if	(CImageTools::CreateColorImage (lImageSize, lTrgImage, true))
			{
				ATL::CImageDC	lSrcDC (pImage);
				ATL::CImageDC	lTrgDC (lTrgImage);

				pImage.AlphaBlend (lTrgDC, 0, 0, lImageSize.cx, lImageSize.cy, 0, 0, lImageSize.cx, lImageSize.cy, pAlpha);
				lRet = true;
			}

			if	(lRet)
			{
				HBITMAP	lBitmap = pImage.Detach ();
				pImage.Attach (lTrgImage.Detach ());
				lTrgImage.Attach (lBitmap);
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CImageAlpha::AlphaClearImage (ATL::CImage& pImage, COLORREF pClearColor, BYTE pAlpha)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		COLORREF*	lBitmapBits = (COLORREF*) GetImageBits (pImage);
		COLORREF	lClearColor = (pClearColor & 0x00FFFFFF) | (((COLORREF) pAlpha) << 24);
		long		lMaxNdx = GetImageBytes (pImage) / sizeof (COLORREF);
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

bool CImageAlpha::AlphaFillImage (ATL::CImage& pImage, BYTE pAlpha, bool pClearBlack, bool pClearWhite)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
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
						&&	(lBitmapBits [lNdx] == 0)
						&&	(lBitmapBits [lNdx+1] == 0)
						&&	(lBitmapBits [lNdx+2] == 0)
						)
					{
						lBitmapBits [lNdx+3] = 0;
						continue;
					}

					if	(
							(pClearWhite)
						&&	(lBitmapBits [lNdx] == 255)
						&&	(lBitmapBits [lNdx+1] == 255)
						&&	(lBitmapBits [lNdx+2] == 255)
						)
					{
						lBitmapBits [lNdx] = 0;
						lBitmapBits [lNdx+1] = 0;
						lBitmapBits [lNdx+2] = 0;
						lBitmapBits [lNdx+3] = 0;
						continue;
					}

					lBitmapBits [lNdx+3] = pAlpha;
				}
			}
			else
			{
				for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
				{
					lBitmapBits [lNdx] = pAlpha;
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CImageAlpha::AlphaSaturateImage (ATL::CImage& pImage, BYTE pMinAlpha, BYTE pMaxAlpha)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
		long	lNdx;

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				lBitmapBits [lNdx] = lBitmapBits [lNdx+3];
				lBitmapBits [lNdx+1] = lBitmapBits [lNdx+3];
				lBitmapBits [lNdx+2] = lBitmapBits [lNdx+3];
				lBitmapBits [lNdx+3] = min (max (lBitmapBits [lNdx+3], pMinAlpha), pMaxAlpha);
			}

			lRet = true;
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

bool CImageAlpha::AlphaInvertImage (ATL::CImage& pImage, bool pClearBlack, bool pSkipBlack, bool pSkipWhite)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
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
						&&	(lBitmapBits [lNdx] == 0)
						&&	(lBitmapBits [lNdx+1] == 0)
						&&	(lBitmapBits [lNdx+2] == 0)
						)
					{
						continue;
					}

					if	(
							(pSkipWhite)
						&&	(lBitmapBits [lNdx] == 255)
						&&	(lBitmapBits [lNdx+1] == 255)
						&&	(lBitmapBits [lNdx+2] == 255)
						)
					{
						continue;
					}

					lBitmapBits [lNdx+3] = 255-lBitmapBits [lNdx+3];

					if	(
							(pClearBlack)
						&&	(lBitmapBits [lNdx+3] == 0)
						)
					{
						lBitmapBits [lNdx] = 0;
						lBitmapBits [lNdx+1] = 0;
						lBitmapBits [lNdx+2] = 0;
					}
				}
			}
			else
			{
				for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
				{
					lBitmapBits [lNdx] = 255-lBitmapBits [lNdx];
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CImageAlpha::AlphaFixImage (ATL::CImage& pImage, BYTE pMinAlpha)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
		long	lNdx;

		try
		{
			for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
			{
				if	(lBitmapBits [lNdx] <= pMinAlpha)
				{
					lBitmapBits [lNdx-1] = 0;
					lBitmapBits [lNdx-2] = 0;
					lBitmapBits [lNdx-3] = 0;
					lRet = true;
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

bool CImageAlpha::AlphaClampImage (ATL::CImage& pImage, BYTE pMinAlpha, BYTE pMaxAlpha, BYTE pNewAlpha)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
		long	lNdx;

		try
		{
			for	(lNdx = 3; lNdx < lMaxNdx; lNdx+=4)
			{
				if	(
						(lBitmapBits [lNdx] >= pMinAlpha)
					&&	(lBitmapBits [lNdx] <= pMaxAlpha)
					)
				{
					lBitmapBits [lNdx] = pNewAlpha;
					lRet = true;
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CImageAlpha::AlphaFeatherImage (ATL::CImage& pImage, const CRect& pCenter, BYTE pAlpha, bool pRectangular, bool pPrecalc)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		try
		{
			LPBYTE	lBitmapBits = GetImageBits (pImage);
			CSize	lBitmapSize (pImage.GetWidth(), pImage.GetHeight());
			long	lBitmapPitch = abs(pImage.GetPitch());
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
			lFeather.right = (lBitmapSize.cx-pCenter.right+1)*(lBitmapSize.cx-pCenter.right+1);
			lFeather.bottom = (lBitmapSize.cy-pCenter.bottom+1)*(lBitmapSize.cy-pCenter.bottom+1);

			for	(lPixel.x = 0; lPixel.x < lBitmapSize.cx; lPixel.x++)
			{
				for	(lPixel.y = 0; lPixel.y < lBitmapSize.cy; lPixel.y++)
				{
					lNdx = ((lBitmapSize.cy-lPixel.y-1) * lBitmapPitch) + (lPixel.x * 4) + 3;
					lAlphaOld = (long) lBitmapBits [lNdx];

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
							lBitmapBits [lNdx] = 0;
							lBitmapBits [lNdx-1] = 0;
							lBitmapBits [lNdx-2] = 0;
							lBitmapBits [lNdx-3] = 0;
						}
						else
						if	(lAlphaNew != lAlphaOld)
						{
							lBitmapBits [lNdx] = (BYTE) lAlphaNew;
							if	(pPrecalc)
							{
								lBitmapBits [lNdx-1] = (BYTE) min (MulDiv ((long) lBitmapBits [lNdx-1], lAlphaNew, lAlphaOld), 255);
								lBitmapBits [lNdx-2] = (BYTE) min (MulDiv ((long) lBitmapBits [lNdx-2], lAlphaNew, lAlphaOld), 255);
								lBitmapBits [lNdx-3] = (BYTE) min (MulDiv ((long) lBitmapBits [lNdx-3], lAlphaNew, lAlphaOld), 255);
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

bool CImageAlpha::AlphaCalcImage (ATL::CImage& pImage, COLORREF pOpaqueColor, COLORREF pSaturateColor)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE		lBitmapBits = GetImageBits (pImage);
		long		lMaxNdx = GetImageBytes (pImage);
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
				lBitTotal = (long) lBitmapBits [lNdx] + (long) lBitmapBits [lNdx+1] + (long) lBitmapBits [lNdx+2];
//				lBitAlpha = MulDiv ((255 * 3) - labs (lBitTotal - lOpaqueTotal), 255, lOpaqueAlpha * 3);
				lBitAlpha = MulDiv (lBitTotal, lOpaqueAlpha, lOpaqueTotal);
				lBitAlpha = max (min (lBitAlpha, 255), 0);

				if	(lBitAlpha > 0)
				{
					lBitmapBits [lNdx+3] = (BYTE) lBitAlpha;

					if	(pSaturateColor)
					{
						lBitColor.Saturation = lSaturateColor.Saturation * (float) (UINT) lBitAlpha / 255.0f;
						CColor::HslToRgb (lSaturateColor.Hue, lBitColor.Saturation, lSaturateColor.Luminosity, lBitmapBits [lNdx+2], lBitmapBits [lNdx+1], lBitmapBits [lNdx]);

						lBitmapBits [lNdx] = (BYTE) MulDiv (lBitmapBits [lNdx], lBitAlpha, 255);
						lBitmapBits [lNdx+1] = (BYTE) MulDiv (lBitmapBits [lNdx+1], lBitAlpha, 255);
						lBitmapBits [lNdx+2] = (BYTE) MulDiv (lBitmapBits [lNdx+2], lBitAlpha, 255);
					}
				}
				else
				{
					lBitmapBits [lNdx] = 0;
					lBitmapBits [lNdx+1] = 0;
					lBitmapBits [lNdx+2] = 0;
					lBitmapBits [lNdx+3] = 0;
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CImageAlpha::AlphaHitTestImage (ATL::CImage& pImage, const CPoint& pPoint, const CRect& pBounds, const CRect* pAperture, BYTE pMinAlpha, BYTE pMaxAlpha)
{
	bool	lRet = false;

	if	(
			(pBounds.PtInRect (pPoint))
		&&	((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		try
		{
			CSize	lBitmapSize (pImage.GetWidth(), pImage.GetHeight());
			CPoint	lPoint (pPoint);
			LPBYTE	lPointBits;

			lPoint -= pBounds.TopLeft ();
			lPointBits = (LPBYTE) pImage.GetPixelAddress (lPoint.x, lPoint.y);

			if	(
					(lPointBits [3] >= pMinAlpha)
				&&	(lPointBits [3] <= pMaxAlpha)
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

				for	(lPoint.x = lAperture.left; lPoint.x < lAperture.right; lPoint.x++)
				{
					for	(lPoint.y = lAperture.top; lPoint.y < lAperture.bottom; lPoint.y++)
					{
						lPointBits = (LPBYTE) pImage.GetPixelAddress (lPoint.x, lPoint.y);

						if	(
								(lPointBits [3] >= pMinAlpha)
							&&	(lPointBits [3] <= pMaxAlpha)
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

bool CImageAlpha::AlphaPreDrawBuffer (ATL::CImage& pImage, COLORREF pFillColor)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
		long	lNdx;
		BYTE	lFillRed = GetRValue (pFillColor);
		BYTE	lFillGreen = GetGValue (pFillColor);
		BYTE	lFillBlue = GetBValue (pFillColor);

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				lBitmapBits [lNdx] = lFillBlue;
				lBitmapBits [lNdx+1] = lFillGreen;
				lBitmapBits [lNdx+2] = lFillRed;
				lBitmapBits [lNdx+3] = 1;
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}

bool CImageAlpha::AlphaPostDrawBuffer (ATL::CImage& pImage, COLORREF pMaskColor, bool pAlphaPreCalc)
{
	bool	lRet = false;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.GetBPP () == 32)
		&&	(pImage.IsDIBSection ())
		)
	{
		LPBYTE	lBitmapBits = GetImageBits (pImage);
		long	lMaxNdx = GetImageBytes (pImage);
		long	lNdx;
		BYTE	lMaskRed = GetRValue (pMaskColor);
		BYTE	lMaskGreen = GetGValue (pMaskColor);
		BYTE	lMaskBlue = GetBValue (pMaskColor);
		BYTE	lMaskAlpha = ((RGBQUAD *) &pMaskColor)->rgbReserved;

		try
		{
			for	(lNdx = 0; lNdx < lMaxNdx; lNdx+=4)
			{
				if	(lBitmapBits [lNdx+3] == lMaskAlpha)
				{
					lBitmapBits [lNdx+3] = 255;
				}
				else
				if	(lBitmapBits [lNdx+3] == 1)
				{
					lBitmapBits [lNdx] = lMaskBlue;
					lBitmapBits [lNdx+1] = lMaskGreen;
					lBitmapBits [lNdx+2] = lMaskRed;
					lBitmapBits [lNdx+3] = 0;
				}
				else
				if	(pAlphaPreCalc)
				{
					lBitmapBits [lNdx] = (BYTE) MulDiv ((long) lBitmapBits [lNdx], (long) lBitmapBits [lNdx+3], 255);
					lBitmapBits [lNdx+1] = (BYTE) MulDiv ((long) lBitmapBits [lNdx+1], (long) lBitmapBits [lNdx+3], 255);
					lBitmapBits [lNdx+2] = (BYTE) MulDiv ((long) lBitmapBits [lNdx+2], (long) lBitmapBits [lNdx+3], 255);
				}
			}
		}
		catch AnyExceptionSilent

		lRet = true;
	}

	return lRet;
}
