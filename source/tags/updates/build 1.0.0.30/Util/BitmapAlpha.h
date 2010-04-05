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
#if !defined(AFX_BITMAPALPHA_H__57A92F2E_8FB7_4B09_A07F_6BC3C964633D__INCLUDED_)
#define AFX_BITMAPALPHA_H__57A92F2E_8FB7_4B09_A07F_6BC3C964633D__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CBitmapAlpha
{
public:
	static bool AlphaBlend (HDC pDst, const CRect & pDstRect, HDC pSrc, const CRect & pSrcRect, BYTE pAlpha = 255, bool pAllowInternal = true);
	static bool AlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha = 255, bool pAllowInternal = true);

	static CBrush * GetAlphaBrush (HDC pDC, COLORREF pColor, BYTE pAlpha, bool pAlphaPrecalc = true);
	static bool AlphaScaleBitmap (CBitmap & pBitmap, BYTE pAlpha, LPBYTE * pBitmapBits = NULL);

	static bool AlphaClearBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pClearColor, BYTE pAlpha = 255);
	static bool AlphaFillBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pAlpha = 255, bool pClearBlack = false, bool pClearWhite = false);
	static bool AlphaSaturateBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pMinAlpha = 0, BYTE pMaxAlpha = 255);
	static bool AlphaInvertBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, bool pClearBlack = true, bool pSkipBlack = false, bool pSkipWhite = false);
	static bool AlphaFixBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pMinAlpha = 0);
	static bool AlphaClampBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, BYTE pMinAlpha, BYTE pMaxAlpha, BYTE pNewAlpha);
	static bool AlphaFeatherBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, const CRect & pCenter, BYTE pAlpha = 255, bool pRectangular = true, bool pPrecalc = true);
	static bool AlphaCalcBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pOpaqueColor = 0xFFFFFFFF, COLORREF pSaturateColor = 0);
	static bool AlphaHitTestBitmap (CBitmap & pBitmap, LPBYTE pBitmapBits, const CPoint & pPoint, const CRect & pBounds, const CRect * pAperture = NULL, BYTE pMinAlpha = 1, BYTE pMaxAlpha = 255);

	static bool AlphaPreDrawBuffer (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pFillColor = RGB (0xFF,0xFF,0xFF));
	static bool AlphaPostDrawBuffer (CBitmap & pBitmap, LPBYTE pBitmapBits, COLORREF pMaskColor = RGB (0x00,0x00,0x00), bool pAlphaPreCalc = false);
private:
	static bool InternalAlphaBlend (HDC pDst, int pDstX, int pDstY, int pDstCx, int pDstCy, HDC pSrc, int pSrcX, int pSrcY, int pSrcCx, int pSrcCy, BYTE pAlpha, bool & pLogSizeMismatch, bool & pDevSizeMismatch);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BITMAPALPHA_H__57A92F2E_8FB7_4B09_A07F_6BC3C964633D__INCLUDED_)
