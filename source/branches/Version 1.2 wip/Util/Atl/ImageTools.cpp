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
#include "ImageTools.h"
#include "ImageBuffer.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////

bool CImageTools::CreateColorImage (const CSize& pSize, ATL::CImage& pImage, bool pForBlending, bool pTopDown)
{
	pImage.Destroy ();

	if	(pImage.Create (pSize.cx, pTopDown?-pSize.cy:pSize.cy, pForBlending?32:24, pForBlending?ATL::CImage::createAlphaChannel:0))
	{
		GdiFlush ();
		memset (GetImageBits(pImage), 0, GetImageBytes(pImage));
		return true;
	}
	return false;
}

bool CImageTools::RecreateColorImage (const CSize& pSize, ATL::CImage& pImage, bool pForBlending, bool pTopDown)
{
	if	(
			((HBITMAP)pImage == NULL)
		||	(pImage.GetWidth () != pSize.cx)
		||	(pImage.GetHeight () != pSize.cy)
		||	(pImage.GetBPP () != (pForBlending?32:24))
		)
	{
		return CreateColorImage (pSize, pImage, pForBlending, pTopDown);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CImageTools::CreateWindowImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending, bool pTopDown)
{
	if	(IsWindow (pWindow))
	{
		CRect	lWindowRect;

		GetWindowRect (pWindow, &lWindowRect);
		return CreateColorImage (lWindowRect.Size (), pImage, pForBlending, pTopDown);
	}
	return false;
}

bool CImageTools::RecreateWindowImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending, bool pTopDown)
{
	if	(IsWindow (pWindow))
	{
		CRect		lWindowRect;

		GetWindowRect (pWindow, &lWindowRect);

		if	(
				((HBITMAP)pImage == NULL)
			||	(pImage.GetWidth () != lWindowRect.Width ())
			||	(pImage.GetHeight () != lWindowRect.Height ())
			)
		{
			return CreateWindowImage (pWindow, pImage, pForBlending, pTopDown);
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CImageTools::CreateClientImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending, bool pTopDown)
{
	if	(IsWindow (pWindow))
	{
		CRect	lClientRect;

		GetClientRect (pWindow, &lClientRect);
		return CreateColorImage (lClientRect.Size (), pImage, pForBlending, pTopDown);
	}
	return false;
}

bool CImageTools::RecreateClientImage (HWND pWindow, ATL::CImage& pImage, bool pForBlending, bool pTopDown, bool pExactFit)
{
	if	(IsWindow (pWindow))
	{
		CRect		lClientRect;

		GetClientRect (pWindow, &lClientRect);

		if	(
				((HBITMAP)pImage == NULL)
			||	(
					(pExactFit)
				?	(
						(pImage.GetWidth () != lClientRect.Width ())
					||	(pImage.GetHeight () != lClientRect.Height ())
					)
				:	(
						(pImage.GetWidth () < lClientRect.Width ())
					||	(pImage.GetWidth () > (lClientRect.Width () * 3 / 2))
					||	(pImage.GetHeight () < lClientRect.Height ())
					||	(pImage.GetHeight () > (lClientRect.Height () * 3 / 2))
					)
				)
			)
		{
			return CreateClientImage (pWindow, pImage, pForBlending, pTopDown);
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CImageTools::ShrinkImage (ATL::CImage& pImage, UINT pFactor, UINT pBlend)
{
	bool			lRet = false;
	ATL::CImage		lTrgImage;
	CSize			lTrgImageSize;
	LPBYTE			lSrcBits = NULL;
	LPBYTE			lTrgBits = NULL;
	CPoint			lTrgPixel;
	CPoint			lSrcPixel;
	long			lTrgNdx;
	long			lSrcNdx;
	long			lSrcPitch = abs(pImage.GetPitch ());
	long			lTrgPitch;
	long			lBitsPerPixel = pImage.GetBPP ();
	long			lBytesPerPixel;
	long			lFactorLong = pFactor;
	long			lFactorCenter = (pFactor-1)/2;
	UINT			lFactorMul = 1;
	UINT			lFactorCenterMul = 1;
	UINT			lFactorDiv = pFactor*pFactor;
	UINT			lPixel [4];
	UINT			lPixelCenter [4];

	if	(pBlend > 1)
	{
		UINT lBlend = min (pBlend, lFactorDiv);
		lFactorMul = lFactorDiv - lBlend;
		lFactorCenterMul = lBlend * (lFactorDiv - 1L);
		lFactorDiv *= (lFactorDiv - 1L);
	}

	//LogMessage (LogNormal|LogTime, _T("Shrink Factor [%u] Blend [%u] MulDiv [%u + %u / %u] [%f]"), pFactor, pBlend, lFactorCenterMul, lFactorMul, lFactorDiv, (float) lFactorCenterMul / (float) lFactorDiv * 100.0f);

	if	(
			((HBITMAP)pImage)
		&&	(pImage.IsDIBSection ())
		&&	(
				(lBitsPerPixel == 32)
			||	(lBitsPerPixel == 24)
			)
		&&	(lSrcBits = GetImageBits (pImage))
		)
	{
		lTrgImageSize.cx = pImage.GetWidth() / pFactor;
		lTrgImageSize.cy = pImage.GetHeight() / pFactor;
		lBytesPerPixel = (pImage.GetBPP() == 24) ? 3 : 4;

		if	(
				(CreateColorImage (lTrgImageSize, lTrgImage, (lBitsPerPixel == 32)))
			&&	(lTrgBits = GetImageBits (lTrgImage))
			)
		{
			lTrgPitch = abs(lTrgImage.GetPitch());

			for	(lTrgPixel.y = lTrgImageSize.cy - 1L; lTrgPixel.y >= 0; lTrgPixel.y--)
			{
				for	(lTrgPixel.x = lTrgImageSize.cx - 1L; lTrgPixel.x >= 0; lTrgPixel.x--)
				{
					memset (lPixel, 0, sizeof (lPixel));
					memset (lPixelCenter, 0, sizeof (lPixelCenter));

					lTrgNdx = (lTrgPixel.y * lTrgPitch) + (lTrgPixel.x * lBytesPerPixel);

					for	(lSrcPixel.y = lFactorLong-1L; lSrcPixel.y >= 0; lSrcPixel.y--)
					{
						for	(lSrcPixel.x = lFactorLong-1L; lSrcPixel.x >= 0; lSrcPixel.x--)
						{
							lSrcNdx = (((lTrgPixel.y * lFactorLong) + lSrcPixel.y) * lSrcPitch) + ((lTrgPixel.x * lFactorLong) + lSrcPixel.x) * lBytesPerPixel;

							if	(
									(lSrcPixel.x == lFactorCenter)
								&&	(lSrcPixel.y == lFactorCenter)
								)
							{
								lPixelCenter [0] = (UINT) lSrcBits [lSrcNdx];
								lPixelCenter [1] = (UINT) lSrcBits [lSrcNdx+1];
								lPixelCenter [2] = (UINT) lSrcBits [lSrcNdx+2];
								if	(lBitsPerPixel == 32)
								{
									lPixelCenter [3] = (UINT) lSrcBits [lSrcNdx+3];
								}
							}
							else
							{
								lPixel [0] += (UINT) lSrcBits [lSrcNdx];
								lPixel [1] += (UINT) lSrcBits [lSrcNdx+1];
								lPixel [2] += (UINT) lSrcBits [lSrcNdx+2];
								if	(lBitsPerPixel == 32)
								{
									lPixel [3] += (UINT) lSrcBits [lSrcNdx+3];
								}
							}
						}
					}

					lPixel [0] = ((lPixelCenter [0] * lFactorCenterMul) + (lPixel [0] * lFactorMul)) / lFactorDiv;
					lPixel [1] = ((lPixelCenter [1] * lFactorCenterMul) + (lPixel [1] * lFactorMul)) / lFactorDiv;
					lPixel [2] = ((lPixelCenter [2] * lFactorCenterMul) + (lPixel [2] * lFactorMul)) / lFactorDiv;

					lTrgBits [lTrgNdx] = (BYTE) min (lPixel [0], (UINT) 255);
					lTrgBits [lTrgNdx+1] = (BYTE) min (lPixel [1], (UINT) 255);
					lTrgBits [lTrgNdx+2] = (BYTE) min (lPixel [2], (UINT) 255);

					if	(lBitsPerPixel == 32)
					{
						lPixel [3] = ((lPixelCenter [3] * lFactorCenterMul) + (lPixel [3] * lFactorMul)) / lFactorDiv;
						lTrgBits [lTrgNdx+3] = (BYTE) min (lPixel [3], (UINT) 255);
					}
				}
			}

			pImage.Destroy ();
			pImage.Attach (lTrgImage.Detach ());
			lRet = true;
		}
	}
	return lRet;
}

bool CImageTools::ShrinkImage (ATL::CImage& pSrcImage, ATL::CImage& pTrgImage, UINT pBlend)
{
	bool			lRet = false;
	LPBYTE			lSrcBits;
	LPBYTE			lTrgBits;
	CPoint			lTrgPixel;
	CPoint			lSrcPixel;
	long			lTrgNdx;
	long			lSrcNdx;
	long			lSrcPitch = abs(pSrcImage.GetPitch ());
	long			lTrgPitch = abs(pTrgImage.GetPitch ());
	long			lBitsPerPixel = pSrcImage.GetBPP ();
	long			lBytesPerPixel;
	CPoint			lFactor;
	CPoint			lFactorCenter;
	UINT			lFactorMul = 1;
	UINT			lFactorCenterMul = 1;
	UINT			lFactorDiv;
	UINT			lPixel [4];
	UINT			lPixelCenter [4];

	if	(
			((HBITMAP)pSrcImage)
		&&	(pSrcImage.IsDIBSection ())
		&&	(
				(lBitsPerPixel == 32)
			||	(lBitsPerPixel == 24)
			)
		&&	(lSrcBits = GetImageBits (pSrcImage))
		&&	((HBITMAP)pTrgImage)
		&&	(pTrgImage.IsDIBSection ())
		&&	(pTrgImage.GetBPP () == pSrcImage.GetBPP ())
		&&	(lTrgBits = GetImageBits (pTrgImage))
		&&	((lFactor.x = pSrcImage.GetWidth() / pTrgImage.GetWidth()) > 0)
		&&	((lFactor.y = pSrcImage.GetHeight() / pTrgImage.GetHeight()) > 0)
		)
	{
		lFactorCenter.x = (lFactor.x-1L)/2L;
		lFactorCenter.y = (lFactor.y-1L)/2L;
		lFactorDiv = lFactor.x * lFactor.y;
		if	(pBlend > 1)
		{
			UINT lBlend = min (pBlend, lFactorDiv);
			lFactorMul = lFactorDiv - lBlend;
			lFactorCenterMul = lBlend * (lFactorDiv - 1L);
			lFactorDiv *= (lFactorDiv - 1L);
		}

		lBytesPerPixel = (lBitsPerPixel == 24) ? 3 : 4;

		for	(lTrgPixel.y = pTrgImage.GetHeight() - 1L; lTrgPixel.y >= 0; lTrgPixel.y--)
		{
			for	(lTrgPixel.x = pTrgImage.GetWidth() - 1L; lTrgPixel.x >= 0; lTrgPixel.x--)
			{
				memset (lPixel, 0, sizeof (lPixel));
				memset (lPixelCenter, 0, sizeof (lPixelCenter));

				lTrgNdx = (lTrgPixel.y * lTrgPitch) + (lTrgPixel.x * lBytesPerPixel);

				for	(lSrcPixel.y = lFactor.y-1L; lSrcPixel.y >= 0; lSrcPixel.y--)
				{
					for	(lSrcPixel.x = lFactor.x-1L; lSrcPixel.x >= 0; lSrcPixel.x--)
					{
						lSrcNdx = (((lTrgPixel.y * lFactor.y) + lSrcPixel.y) * lSrcPitch) + ((lTrgPixel.x * lFactor.x) + lSrcPixel.x) * lBytesPerPixel;

						if	(
								(lSrcPixel.x == lFactorCenter.x)
							&&	(lSrcPixel.y == lFactorCenter.y)
							)
						{
							lPixelCenter [0] = (UINT) lSrcBits [lSrcNdx];
							lPixelCenter [1] = (UINT) lSrcBits [lSrcNdx+1];
							lPixelCenter [2] = (UINT) lSrcBits [lSrcNdx+2];
							if	(lBitsPerPixel == 32)
							{
								lPixelCenter [3] = (UINT) lSrcBits [lSrcNdx+3];
							}
						}
						else
						{
							lPixel [0] += (UINT) lSrcBits [lSrcNdx];
							lPixel [1] += (UINT) lSrcBits [lSrcNdx+1];
							lPixel [2] += (UINT) lSrcBits [lSrcNdx+2];
							if	(lBitsPerPixel == 32)
							{
								lPixel [3] += (UINT) lSrcBits [lSrcNdx+3];
							}
						}
					}
				}

				lPixel [0] = ((lPixelCenter [0] * lFactorCenterMul) + (lPixel [0] * lFactorMul)) / lFactorDiv;
				lPixel [1] = ((lPixelCenter [1] * lFactorCenterMul) + (lPixel [1] * lFactorMul)) / lFactorDiv;
				lPixel [2] = ((lPixelCenter [2] * lFactorCenterMul) + (lPixel [2] * lFactorMul)) / lFactorDiv;

				lTrgBits [lTrgNdx] = (BYTE) min (lPixel [0], (UINT) 255);
				lTrgBits [lTrgNdx+1] = (BYTE) min (lPixel [1], (UINT) 255);
				lTrgBits [lTrgNdx+2] = (BYTE) min (lPixel [2], (UINT) 255);

				if	(lBitsPerPixel == 32)
				{
					lPixel [3] = ((lPixelCenter [3] * lFactorCenterMul) + (lPixel [3] * lFactorMul)) / lFactorDiv;
					lTrgBits [lTrgNdx+3] = (BYTE) min (lPixel [3], (UINT) 255);
				}
			}
		}

		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CImageTools::SmearImage (ATL::CImage& pImage, UINT pFactor)
{
	bool		lRet = false;
	ATL::CImage	lTrgImage;
	LPBYTE		lSrcBits;
	LPBYTE		lTrgBits;
	CPoint		lMaxPixel;
	CPoint		lTrgPixel;
	CPoint		lSrcPixel;
	long		lNdx;
	long		lTrgNdx;
	long		lSrcNdx;
	long		lPitch = abs (pImage.GetPitch ());
	long		lBitsPerPixel = pImage.GetBPP ();
	long		lBytesPerPixel;
	long		lAperture = pFactor;
	long		lPixel [4];
	long		lCount;

	if	(
			((HBITMAP)pImage)
		&&	(pImage.IsDIBSection ())
		&&	(
				(lBitsPerPixel == 24)
			||	(lBitsPerPixel == 32)
			)
		&&	(lSrcBits = GetImageBits (pImage))
		&&	(CreateColorImage (CSize (pImage.GetWidth(), pImage.GetHeight()), lTrgImage, (lBitsPerPixel == 32)))
		&&	(lTrgBits = GetImageBits (lTrgImage))
		)
	{
		lBytesPerPixel = (lBitsPerPixel == 32) ? 4 : 3;
		lMaxPixel.x = pImage.GetWidth() - 1;
		lMaxPixel.y = pImage.GetHeight() - 1;

		for	(lTrgPixel.y = 0; lTrgPixel.y <= lMaxPixel.y; lTrgPixel.y++)
		{
			for	(lTrgPixel.x = 0; lTrgPixel.x <= lMaxPixel.x; lTrgPixel.x++)
			{
				memset (lPixel, 0, sizeof (lPixel));
				lCount = 0;

				for	(lSrcPixel.y = max (lTrgPixel.y - lAperture, 0); lSrcPixel.y <= min (lTrgPixel.y + lAperture, lMaxPixel.y); lSrcPixel.y++)
				{
					for	(lSrcPixel.x = max (lTrgPixel.x - lAperture, 0); lSrcPixel.x <= min (lTrgPixel.x + lAperture, lMaxPixel.x); lSrcPixel.x++)
					{
						lSrcNdx = (lSrcPixel.y * lPitch) + (lSrcPixel.x * lBytesPerPixel);

						for	(lNdx = 0; lNdx < lBytesPerPixel; lNdx++)
						{
							lPixel [lNdx] += (ULONG) lSrcBits [lSrcNdx+lNdx];
						}
						lCount++;
					}
				}

				for	(lNdx = 0; lNdx < lBytesPerPixel; lNdx++)
				{
					lPixel [lNdx] /= lCount;
				}

				lTrgNdx = (lTrgPixel.y * lPitch) + (lTrgPixel.x * lBytesPerPixel);

				for	(lNdx = 0; lNdx < lBytesPerPixel; lNdx++)
				{
					lTrgBits [lTrgNdx+lNdx] = (BYTE) min (lPixel [lNdx], 255);
				}
			}
		}

		pImage.Destroy ();
		pImage.Attach (lTrgImage.Detach ());
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CImageTools::GetImageInfo (HBITMAP pImage, BITMAPINFO& pBitmapInfo, BYTE** pBitmapBits)
{
	bool			lRet = false;
	tS <BITMAP>		lBitmap;

	memset (&pBitmapInfo, 0, sizeof (pBitmapInfo));
	pBitmapInfo.bmiHeader.biSize = sizeof (pBitmapInfo.bmiHeader);

	if	(pBitmapBits)
	{
		(*pBitmapBits) = NULL;
	}

	if	(
			(pImage)
		&&	(GetObject (pImage, sizeof (lBitmap), &lBitmap))
		&&	(lBitmap.bmBitsPixel >= 24)
		)
	{
		pBitmapInfo.bmiHeader.biBitCount = lBitmap.bmBitsPixel;
		pBitmapInfo.bmiHeader.biPlanes = lBitmap.bmPlanes;
		pBitmapInfo.bmiHeader.biWidth = lBitmap.bmWidth;
		pBitmapInfo.bmiHeader.biHeight = lBitmap.bmHeight;
		pBitmapInfo.bmiHeader.biSizeImage = lBitmap.bmHeight * lBitmap.bmWidthBytes;
		pBitmapInfo.bmiHeader.biCompression = BI_RGB;

		if	(pBitmapBits)
		{
			CMemDCHandle		lDC;
			tArrayPtr <BYTE>	lBitmapBits;

			if	(
					(lBitmapBits = new BYTE [pBitmapInfo.bmiHeader.biSizeImage])
				&&	(lDC.Attach (CreateCompatibleDC (NULL)))
				&&	(GetDIBits (lDC, pImage, 0, pBitmapInfo.bmiHeader.biHeight, (LPVOID) (LPBYTE) lBitmapBits, &pBitmapInfo, DIB_RGB_COLORS))
				)
			{
				(*pBitmapBits) = lBitmapBits.Detach ();
			}
		}

		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CImageTools::MapImageColors (HBITMAP pImage, const CColor& pColorFill, const CColor * pColorLight, const CColor * pColorDark, const CColor * pColorWhite, const CColor * pColorBlack, BYTE pVarianceMask)
{
	bool			lRet = false;
//TODO
ATLASSERT(FALSE);
#if	FALSE
	CImageBuffer	lBuffer;

	if	(lBuffer.mBitmap.Attach (pBitmap))
	{
		try
		{
			tS <BITMAP>		lBitmap;
			CPoint			lPixel;
			COLORREF		lVarianceMask = ~RGB (pVarianceMask, pVarianceMask, pVarianceMask);
			COLORREF		lColor;
			COLORREF		l3dFace = GetSysColor (COLOR_3DFACE) & lVarianceMask;
			COLORREF		l3dLight = GetSysColor (COLOR_3DLIGHT) & lVarianceMask;
			COLORREF		l3dDark = GetSysColor (COLOR_3DSHADOW) & lVarianceMask;
			COLORREF		l3dWhite = GetSysColor (COLOR_3DHIGHLIGHT) & lVarianceMask;
			COLORREF		l3dBlack = GetSysColor (COLOR_3DDKSHADOW) & lVarianceMask;
			CColor			lColorLight;
			CColor			lColorDark;
			CColor			lColorWhite;
			CColor			lColorBlack;

			if	(
					(lBuffer.mBitmap.GetBitmap (&lBitmap))
				&&	(lBuffer.StartBuffer ())
				)
			{
				lColorLight = pColorFill.ClampShadePercent (100.0f, 110.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.4f, 0.95f);
				lColorDark = pColorFill.ClampShadePercent (90.0f, 90.0f, 0.0f, 1.0f, 0.0f, 0.8f, 0.1f, 0.8f);
				lColorWhite = pColorFill.ClampShadePercent (100.0f, 140.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.75f, 1.0f);
				lColorBlack = pColorFill.ClampShadePercent (75.0f, 80.0f, 0.0f, 1.0f, 0.0f, 0.4f, 0.0f, 0.7f);

				if	(pColorLight)
				{
					lColorLight = pColorLight->GetRGBMask ();
				}
				if	(pColorDark)
				{
					lColorDark = pColorDark->GetRGBMask ();
				}
				if	(pColorWhite)
				{
					lColorWhite = pColorWhite->GetRGBMask ();
				}
				if	(pColorBlack)
				{
					lColorBlack = pColorBlack->GetRGBMask ();
				}

				for	(lPixel.y = 0; lPixel.y < lBitmap.bmHeight; lPixel.y++)
				{
					for	(lPixel.x = 0; lPixel.x < lBitmap.bmHeight; lPixel.x++)
					{
						lColor = lBuffer.mDC.GetPixel (lPixel) & lVarianceMask;
						if	(
								(lColor == l3dFace)
							||	(lColor == RGB (0xC0, 0xC0, 0xC0))
							)
						{
							lBuffer.mDC.SetPixel (lPixel, pColorFill);
						}
						else
						if	(lColor == l3dLight)
						{
							lBuffer.mDC.SetPixel (lPixel, lColorLight);
						}
						else
						if	(
								(lColor == l3dDark)
							||	(lColor == RGB (0x80, 0x80, 0x80))
							)
						{
							lBuffer.mDC.SetPixel (lPixel, lColorDark);
						}
						else
						if	(
								(lColor == l3dWhite)
							||	(lColor == RGB (0xFF, 0xFF, 0xFF))
							)
						{
							lBuffer.mDC.SetPixel (lPixel, lColorWhite);
						}
						else
						if	(
								(lColor == l3dBlack)
							||	(lColor == RGB (0x00, 0x00, 0x00))
							)
						{
							lBuffer.mDC.SetPixel (lPixel, lColorBlack);
						}
					}
				}
				lBuffer.EndBuffer ();
				lRet = true;
			}
		}
		catch AnyException

		lBuffer.mBitmap.Detach ();
	}
#endif
	return lRet;
}
