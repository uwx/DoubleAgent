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
#ifndef	IMAGEALPHA_H_INCLUDED_
#define IMAGEALPHA_H_INCLUDED_
#pragma once

#include <atlimage.h>

//////////////////////////////////////////////////////////////////////

class CImageAlpha
{
public:
	static bool AlphaBlend (HDC pDst, const CRect& pDstRect, HDC pSrc, const CRect& pSrcRect, BYTE pAlpha = 255, bool pAllowInternal = true);
	static bool AlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha = 255, bool pAllowInternal = true);

	static HBRUSH GetAlphaBrush (COLORREF pColor, BYTE pAlpha, bool pAlphaPrecalc = true);
	static bool AlphaScaleImage (ATL::CImage& pImage, BYTE pAlpha);

	static bool AlphaClearImage (ATL::CImage& pImage, COLORREF pClearColor, BYTE pAlpha = 255);
	static bool AlphaFillImage (ATL::CImage& pImage, BYTE pAlpha = 255, bool pClearBlack = false, bool pClearWhite = false);
	static bool AlphaSaturateImage (ATL::CImage& pImage, BYTE pMinAlpha = 0, BYTE pMaxAlpha = 255);
	static bool AlphaInvertImage (ATL::CImage& pImage, bool pClearBlack = true, bool pSkipBlack = false, bool pSkipWhite = false);
	static bool AlphaFixImage (ATL::CImage& pImage, BYTE pMinAlpha = 0);
	static bool AlphaClampImage (ATL::CImage& pImage, BYTE pMinAlpha, BYTE pMaxAlpha, BYTE pNewAlpha);
	static bool AlphaFeatherImage (ATL::CImage& pImage, const CRect& pCenter, BYTE pAlpha = 255, bool pRectangular = true, bool pPrecalc = true);
	static bool AlphaCalcImage (ATL::CImage& pImage, COLORREF pOpaqueColor = 0xFFFFFFFF, COLORREF pSaturateColor = 0);
	static bool AlphaHitTestImage (ATL::CImage& pImage, const CPoint& pPoint, const CRect& pBounds, const CRect* pAperture = NULL, BYTE pMinAlpha = 1, BYTE pMaxAlpha = 255);

	static bool AlphaPreDrawBuffer (ATL::CImage& pImage, COLORREF pFillColor = RGB (0xFF,0xFF,0xFF));
	static bool AlphaPostDrawBuffer (ATL::CImage& pImage, COLORREF pMaskColor = RGB (0x00,0x00,0x00), bool pAlphaPreCalc = false);
private:
	static bool InternalAlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha, bool& pLogSizeMismatch, bool& pDevSizeMismatch);
};

//////////////////////////////////////////////////////////////////////

#endif // IMAGEALPHA_H_INCLUDED_
