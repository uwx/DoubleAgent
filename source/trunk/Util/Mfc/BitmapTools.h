/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#if !defined(AFX_BITMAPTOOLS_H__07446EA1_9B44_4881_AD3D_76A9F82679C8__INCLUDED_)
#define AFX_BITMAPTOOLS_H__07446EA1_9B44_4881_AD3D_76A9F82679C8__INCLUDED_
#pragma once

#include "Color.h"

//////////////////////////////////////////////////////////////////////

class CBitmapTools
{
public:
	static bool CreateColorBitmap (const CSize& pSize, CBitmap& pBitmap, LPBYTE* pBmpBits = NULL, bool pForBlending = true, bool pTopDown = false);
	static bool RecreateColorBitmap (const CSize& pSize, CBitmap& pBitmap, LPBYTE* pBmpBits = NULL, bool pForBlending = true, bool pTopDown = false);

	static bool CreateWindowBitmap (CWnd& pWindow, CBitmap& pBitmap, LPBYTE* pBmpBits = NULL, bool pForBlending = true, bool pTopDown = false);
	static bool RecreateWindowBitmap (CWnd& pWindow, CBitmap& pBitmap, LPBYTE* pBmpBits = NULL, bool pForBlending = true, bool pTopDown = false);

	static bool CreateClientBitmap (CWnd& pWindow, CBitmap& pBitmap, LPBYTE* pBmpBits = NULL, bool pForBlending = true, bool pTopDown = false);
	static bool RecreateClientBitmap (CWnd& pWindow, CBitmap& pBitmap, LPBYTE* pBmpBits = NULL, bool pForBlending = true, bool pTopDown = false, bool pExactFit = true);

	static bool ShrinkBitmap (CBitmap& pBitmap, LPBYTE & pBitmapBits, UINT pFactor, UINT pBlend = 1);
	static bool ShrinkBitmap (CBitmap& pSrcBitmap, LPBYTE & pSrcBitmapBits, CBitmap& pTrgBitmap, LPBYTE & pTrgBitmapBits, UINT pBlend = 1);
	static bool SmearBitmap (CBitmap& pBitmap, LPBYTE & pBitmapBits, UINT pFactor = 1);

	static bool GetBitmapInfo (HBITMAP pBitmap, BITMAPINFO& pBitmapInfo, BYTE** pBitmapBits = NULL);
	static bool GetBitmapInfo (CBitmap& pBitmap, BITMAPINFO& pBitmapInfo, BYTE** pBitmapBits = NULL) {return GetBitmapInfo ((HBITMAP) pBitmap.GetSafeHandle (), pBitmapInfo, pBitmapBits);}

	static bool MapBitmapColors (HBITMAP pBitmap, const CColor& pColorFill, const CColor * pColorLight = NULL, const CColor * pColorDark = NULL, const CColor * pColorWhite = NULL, const CColor * pColorBlack = NULL, BYTE pVarianceMask = 0x07);
	static bool MapBitmapColors (CBitmap& pBitmap, const CColor& pColorFill, const CColor * pColorLight = NULL, const CColor * pColorDark = NULL, const CColor * pColorWhite = NULL, const CColor * pColorBlack = NULL, BYTE pVarianceMask = 0x07) {return MapBitmapColors ((HBITMAP) pBitmap.GetSafeHandle (), pColorFill, pColorLight, pColorDark, pColorWhite, pColorBlack, pVarianceMask);}
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BITMAPTOOLS_H__07446EA1_9B44_4881_AD3D_76A9F82679C8__INCLUDED_)
