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
#ifndef	IMAGETOOLS_H_INCLUDED_
#define IMAGETOOLS_H_INCLUDED_
#pragma once

#include <atlimage.h>
#include "Color.h"

//////////////////////////////////////////////////////////////////////

class CImageTools
{
public:
	static bool CreateColorImage (const CSize& pSize, ATL::CImage& pImage, bool pForBlending = true, bool pTopDown = false);
	static bool RecreateColorImage (const CSize& pSize, ATL::CImage& pImage, bool pForBlending = true, bool pTopDown = false);

	static bool CreateWindowImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending = true, bool pTopDown = false);
	static bool RecreateWindowImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending = true, bool pTopDown = false);

	static bool CreateClientImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending = true, bool pTopDown = false);
	static bool RecreateClientImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending = true, bool pTopDown = false, bool pExactFit = true);

	static bool ShrinkImage (ATL::CImage& pImage, UINT pFactor, UINT pBlend = 1);
	static bool ShrinkImage (ATL::CImage& pSrcImage, ATL::CImage& pTrgImage, UINT pBlend = 1);
	static bool SmearImage (ATL::CImage& pImage, UINT pFactor = 1);

	static bool GetImageInfo (HBITMAP pImage, BITMAPINFO& pBitmapInfo, BYTE** pBitmapBits = NULL);
	static bool GetImageInfo (ATL::CImage& pImage, BITMAPINFO& pBitmapInfo, BYTE** pBitmapBits = NULL) {return GetImageInfo ((HBITMAP)pImage, pBitmapInfo, pBitmapBits);}

	static bool MapImageColors (HBITMAP pImage, const CColor& pColorFill, const CColor * pColorLight = NULL, const CColor * pColorDark = NULL, const CColor * pColorWhite = NULL, const CColor * pColorBlack = NULL, BYTE pVarianceMask = 0x07);
	static bool MapImageColors (ATL::CImage& pImage, const CColor& pColorFill, const CColor * pColorLight = NULL, const CColor * pColorDark = NULL, const CColor * pColorWhite = NULL, const CColor * pColorBlack = NULL, BYTE pVarianceMask = 0x07) {return MapImageColors ((HBITMAP)pImage, pColorFill, pColorLight, pColorDark, pColorWhite, pColorBlack, pVarianceMask);}
};

//////////////////////////////////////////////////////////////////////

static inline CSize GetImageSize (const ATL::CImage& pImage)
{
	return CSize (pImage.GetWidth(), pImage.GetHeight());
}

static inline LPBYTE GetImageBits (const ATL::CImage& pImage)
{
	if	(pImage.GetPitch() < 0)
	{
		return ((LPBYTE) pImage.GetBits()) + ((pImage.GetHeight()-1)*pImage.GetPitch());
	}
	else
	{
		return (LPBYTE) pImage.GetBits ();
	}
}

static inline int GetImageBytes (const ATL::CImage& pImage)
{
	return pImage.GetHeight() * abs (pImage.GetPitch());
}

static inline bool IsImageTopDown (const ATL::CImage& pImage)
{
	return (pImage.GetPitch() < 0);
}

//////////////////////////////////////////////////////////////////////

#endif // IMAGETOOLS_H_INCLUDED_
