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
#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "Color.h"

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////

CColor::CColor (COLORREF pColor)
:	mColor (pColor)
{
}

CColor::CColor (const CColor& pSource)
:	mColor (pSource.mColor)
{
}

CColor& CColor::operator= (const CColor& pSource)
{
	mColor = pSource.mColor;
	return *this;
}

CColor& CColor::operator= (COLORREF pColor)
{
	mColor = pColor;
	return *this;
}

bool CColor::operator== (const CColor& pColor) const
{
	return (mColor == pColor.mColor);
}

bool CColor::operator!= (const CColor& pColor) const
{
	return (mColor != pColor.mColor);
}

CColor& CColor::operator|= (const CColor& pSource)
{
	mColor |= pSource.mColor;
	return *this;
}

CColor& CColor::operator|= (COLORREF pColor)
{
	mColor |= pColor;
	return *this;
}

CColor& CColor::operator&= (const CColor& pSource)
{
	mColor &= pSource.mColor;
	return *this;
}

CColor& CColor::operator&= (COLORREF pColor)
{
	mColor &= pColor;
	return *this;
}

//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
#ifdef _DEBUG
void CColor::Dump (CDumpContext& dc) const
{
	CString	lDump;
	float	lHue, lSaturation, lLuminosity;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);
	lDump.Format (_T("%2.2X %2.2X %2.2X (%1.2f %1.2f %1.2f)"), GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);

#ifdef	_DEBUG_NOT
	BYTE	lRed, lGreen, lBlue;
	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);
	lDump.Format (_T("%s (%2.2X %2.2X %2.2X)"), CString (lDump), lRed, lGreen, lBlue);
#endif

	dc << lDump;
}
#endif
#endif

//////////////////////////////////////////////////////////////////////

void CColor::GetRGB (BYTE& pRed, BYTE& pGreen, BYTE& pBlue) const
{
	pRed = GetRValue (mColor);
	pGreen = GetGValue (mColor);
	pBlue = GetBValue (mColor);
}

void CColor::GetRGBA (BYTE& pRed, BYTE& pGreen, BYTE& pBlue, BYTE& pAlpha) const
{
	pRed = GetRValue (mColor);
	pGreen = GetGValue (mColor);
	pBlue = GetBValue (mColor);
	pAlpha = LOBYTE (mColor>>24);
}

void CColor::GetHSL (float& pHue, float& pSaturation, float& pLuminosity) const
{
	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), pHue, pSaturation, pLuminosity);
}

CColor& CColor::SetRGB (BYTE pRed, BYTE pGreen, BYTE pBlue)
{
	mColor = RGB (pRed, pGreen, pBlue);
	return *this;
}

CColor& CColor::SetRGBA (BYTE pRed, BYTE pGreen, BYTE pBlue, BYTE pAlpha)
{
	mColor = RGB (pRed, pGreen, pBlue) | (((COLORREF) pAlpha) << 24);
	return *this;
}

CColor& CColor::SetHSL (float pHue, float pSaturation, float pLuminosity)
{
	BYTE	lRed, lGreen, lBlue;

	HslToRgb (pHue, pSaturation, pLuminosity, lRed, lGreen, lBlue);
	mColor = RGB (lRed, lGreen, lBlue);
	return *this;
}

void CColor::GetRGB (TRIVERTEX& pVertex) const
{
	pVertex.Red = MAKEWORD (0, GetRValue (mColor));
	pVertex.Green = MAKEWORD (0, GetGValue (mColor));
	pVertex.Blue = MAKEWORD (0, GetBValue (mColor));
	pVertex.Alpha = 0;
}

void CColor::GetRGBA (TRIVERTEX& pVertex) const
{
	pVertex.Red = MAKEWORD (0, GetRValue (mColor));
	pVertex.Green = MAKEWORD (0, GetGValue (mColor));
	pVertex.Blue = MAKEWORD (0, GetBValue (mColor));
	pVertex.Alpha = MAKEWORD (0, LOBYTE (mColor>>24));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CColor CColor::GetShade (float pPercentSaturation, float pPercentLuminosity) const
{
	float	lHue, lSaturation, lLuminosity;
	BYTE	lRed, lGreen, lBlue;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);
	lSaturation *= pPercentSaturation / 100.0f;
	lLuminosity *= pPercentLuminosity / 100.0f;
	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);

	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

CColor CColor::ClampShadePercent (float pPercentSaturation, float pPercentLuminosity, float pMinHue, float pMaxHue, float pMinSaturation, float pMaxSaturation, float pMinLuminosity, float pMaxLuminosity) const
{
	float	lHue, lSaturation, lLuminosity;
	BYTE	lRed, lGreen, lBlue;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);
	lSaturation *= pPercentSaturation / 100.0f;
	lLuminosity *= pPercentLuminosity / 100.0f;

	lHue = max (lHue, (max (pMinHue, 0.0f)));
	lHue = min (lHue, (min (pMaxHue, 1.0f)));
	lSaturation = max (lSaturation, (max (pMinSaturation, 0.0f)));
	lSaturation = min (lSaturation, (min (pMaxSaturation, 1.0f)));
	lLuminosity = max (lLuminosity, (max (pMinLuminosity, 0.0f)));
	lLuminosity = min (lLuminosity, (min (pMaxLuminosity, 1.0f)));

	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);

	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

CColor CColor::ClampShadeDelta (float pDeltaSaturation, float pDeltaLuminosity, float pMinHue, float pMaxHue, float pMinSaturation, float pMaxSaturation, float pMinLuminosity, float pMaxLuminosity) const
{
	float	lHue, lSaturation, lLuminosity;
	BYTE	lRed, lGreen, lBlue;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);
	lSaturation += pDeltaSaturation;
	lLuminosity += pDeltaLuminosity;

	lHue = max (lHue, (max (pMinHue, 0.0f)));
	lHue = min (lHue, (min (pMaxHue, 1.0f)));
	lSaturation = max (lSaturation, (max (pMinSaturation, 0.0f)));
	lSaturation = min (lSaturation, (min (pMaxSaturation, 1.0f)));
	lLuminosity = max (lLuminosity, (max (pMinLuminosity, 0.0f)));
	lLuminosity = min (lLuminosity, (min (pMaxLuminosity, 1.0f)));

	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);

	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

//////////////////////////////////////////////////////////////////////

CColor CColor::GetMonoColor (int pBrightnessBias) const
{
	float	lHue, lSaturation, lLuminosity;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);

	if	(lSaturation > 0.0f)
	{
		lLuminosity += (float) pBrightnessBias / 3.0f;
	}

	lLuminosity = max (lLuminosity, 0.0f);
	lLuminosity = min (lLuminosity, 1.0f);

	if	(lLuminosity > 0.5f)
	{
		return CColor (RGB (0xFF, 0xFF, 0xFF) | GetAlphaMask ());
	}
	else
	{
		return CColor (RGB (0x00, 0x00, 0x00) | GetAlphaMask ());
	}
}

bool CColor::IsGrayColor (float pMaxSaturation) const
{
	float	lHue, lSaturation, lLuminosity;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);

	return (lSaturation <= pMaxSaturation);
}

CColor CColor::GetGrayColor (int pBrightnessBias) const
{
	float	lHue, lSaturation, lLuminosity;
	BYTE	lRed, lGreen, lBlue;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);

	if	(lSaturation > 0.25f)
	{
		lLuminosity += (float) pBrightnessBias / 4.0f;
	}

	lLuminosity = max (lLuminosity, 0.0f);
	lLuminosity = min (lLuminosity, 1.0f);

	HslToRgb (lHue, 0.0f, lLuminosity, lRed, lGreen, lBlue);
	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

//////////////////////////////////////////////////////////////////////

CColor CColor::MakeContrastColor (int pBrightnessBias, float pContrast) const
{
	float	lHue, lSaturation, lLuminosity;
	BYTE	lRed, lGreen, lBlue;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);

	if	(lLuminosity <= 0.4f + ((float) pBrightnessBias / 20.0f))
	{
		lLuminosity = 0.6f + (pContrast * 0.3f);
	}
	else
	if	(lLuminosity >= 0.8f + ((float) pBrightnessBias / 20.0f))
	{
		lLuminosity = 0.2f + ((1.0f - pContrast) * 0.2f);
	}
	else
	if	(
			(lLuminosity > 0.65f + ((float) pBrightnessBias / 20.0f))
		||	(lLuminosity > (1.0f - pContrast))
		)
	{
		lLuminosity -= pContrast;
	}
	else
	{
		lLuminosity += pContrast;
	}

	lLuminosity = max (lLuminosity, 0.0f);
	lLuminosity = min (lLuminosity, 1.0f);

	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);
	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

CColor CColor::MakeFillTextColor (float pContrast, float pMaxSaturation) const
{
	float	lHue, lSaturation, lLuminosity;
	float	lXVal, lYVal, lZVal;
	BYTE	lRed, lGreen, lBlue;

	pContrast = min (max (pContrast, 0.0f), 1.0f);
	pMaxSaturation = min (max (pMaxSaturation, 0.0f), 1.0f);

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);
	RgbToXyz ((float) GetRValue (mColor) / 255.0f, (float) GetGValue (mColor) / 255.0f, (float) GetBValue (mColor) / 255.0f, lXVal, lYVal, lZVal);

	if	(lSaturation <= 0.01f)
	{
		pContrast = min (pContrast * 2.0f, 1.0f);
	}

	if	(
			(lXVal <= 0.23f)
		&&	(lYVal <= 0.23f)
		)
	{
		lLuminosity += pContrast;
	}
	else
	{
		lLuminosity -= pContrast;
	}

	lLuminosity = max (lLuminosity, 0.0f);
	lLuminosity = min (lLuminosity, 1.0f);
	lSaturation = max (lSaturation, 0.0f);
	lSaturation = min (lSaturation, pMaxSaturation);

	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);
	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

CColor CColor::MakeTextColor (float pContrast, float pMaxSaturation, COLORREF pBkColor) const
{
	float	lHue, lSaturation, lLuminosity;
	float	lXVal, lYVal, lZVal;
	BYTE	lRed, lGreen, lBlue;
	float	lContrastBoost = 0.0f;

	pContrast = min (max (pContrast, 0.0f), 1.0f);
	pMaxSaturation = min (max (pMaxSaturation, 0.0f), 1.0f);

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);
	if	(lSaturation <= 0.01f)
	{
		pContrast = min (pContrast * 2.0f, 1.0f);
	}
	else
	{
		HslToRgb (lHue, lSaturation, 0.5f, lRed, lGreen, lBlue);
		RgbToXyz ((float) lRed / 255.0f, (float) lGreen / 255.0f, (float) lBlue / 255.0f, lXVal, lYVal, lZVal);
		lContrastBoost = lYVal / 2.0f;
		//LogMessage (LogDebug, _T("[%2.2X %2.2X %2.2X] [%1.2f %1.2f] [%1.2f %1.2f %1.2f] (%1.2f)"), lRed, lGreen, lBlue, lHue, lSaturation, lXVal, lYVal, lZVal, lXVal+lYVal);
	}

	RgbToHsl (GetRValue (pBkColor), GetGValue (pBkColor), GetBValue (pBkColor), lXVal, lYVal, lLuminosity);
	RgbToXyz ((float) GetRValue (pBkColor) / 255.0f, (float) GetGValue (pBkColor) / 255.0f, (float) GetBValue (pBkColor) / 255.0f, lXVal, lYVal, lZVal);

	if	(
			(lXVal <= 0.23f)
		&&	(lYVal <= 0.23f)
		)
	{
		lLuminosity += pContrast + lContrastBoost;
	}
	else
	{
		lLuminosity -= pContrast + lContrastBoost;
	}

	lLuminosity = max (lLuminosity, 0.1f);
	lLuminosity = min (lLuminosity, 0.9f);
	lSaturation = max (lSaturation, 0.0f);
	lSaturation = min (lSaturation, pMaxSaturation);

	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);
	//LogMessage (LogDebug, _T("[%2.2X %2.2X %2.2X] [%1.2f %1.2f %1.2f]"), lRed, lGreen, lBlue, lHue, lSaturation, lLuminosity);
	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

//////////////////////////////////////////////////////////////////////

CColor CColor::ClampHSL (float pMinHue, float pMaxHue, float pMinSaturation, float pMaxSaturation, float pMinLuminosity, float pMaxLuminosity) const
{
	float	lHue, lSaturation, lLuminosity;
	BYTE	lRed, lGreen, lBlue;

	RgbToHsl (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), lHue, lSaturation, lLuminosity);

	lHue = max (lHue, (max (pMinHue, 0.0f)));
	lHue = min (lHue, (min (pMaxHue, 1.0f)));
	lSaturation = max (lSaturation, (max (pMinSaturation, 0.0f)));
	lSaturation = min (lSaturation, (min (pMaxSaturation, 1.0f)));
	lLuminosity = max (lLuminosity, (max (pMinLuminosity, 0.0f)));
	lLuminosity = min (lLuminosity, (min (pMaxLuminosity, 1.0f)));

	HslToRgb (lHue, lSaturation, lLuminosity, lRed, lGreen, lBlue);
	return CColor (RGB (lRed, lGreen, lBlue) | GetAlphaMask ());
}

CColor CColor::GetAverage (const CColor& pColor1, const CColor& pColor2)
{
	CColor	lRet;
	RGBQUAD	lRgb1;
	RGBQUAD	lRgb2;
	RGBQUAD	lRgb3;

	pColor1.GetRGBA (lRgb1);
	pColor2.GetRGBA (lRgb2);

	lRgb3.rgbRed = (BYTE) (((UINT) lRgb1.rgbRed + (UINT) lRgb2.rgbRed) / 2);
	lRgb3.rgbGreen = (BYTE) (((UINT) lRgb1.rgbGreen + (UINT) lRgb2.rgbGreen) / 2);
	lRgb3.rgbBlue = (BYTE) (((UINT) lRgb1.rgbBlue + (UINT) lRgb2.rgbBlue) / 2);
	lRgb3.rgbReserved = (BYTE) (((UINT) lRgb1.rgbReserved + (UINT) lRgb2.rgbReserved) / 2);

	lRet.SetRGBA (lRgb3);
	return CColor (lRet);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CColor::RgbToHsl (BYTE pRed, BYTE pGreen, BYTE pBlue, float& pHue, float& pSaturation, float& pLuminosity)
{
	float	lRed = ((float) (short) (unsigned short) pRed) / 255.0f;
	float	lGreen = ((float) (short) (unsigned short) pGreen) / 255.0f;
	float	lBlue = ((float) (short) (unsigned short) pBlue) / 255.0f;

	float	lRGBMin = min (min (lRed, lGreen), lBlue);
	float	lRGBMax = max (max (lRed, lGreen), lBlue);

	pLuminosity = ((lRGBMin + lRGBMax) / 2.0f);

	if	(lRGBMin == lRGBMax)
	{
		pHue = 1.0f;
		pSaturation = 0.0f;
	}
	else
	{
		if	(pLuminosity <= 0.5f)
		{
			pSaturation = ((lRGBMax - lRGBMin) / (lRGBMax + lRGBMin));
		}
		else
		{
			pSaturation = ((lRGBMax - lRGBMin) / (2.0f - lRGBMax - lRGBMin));
		}

		float	 lRDelta = ((lRGBMax - lRed) / (lRGBMax - lRGBMin) / 6.0f);
		float	 lGDelta = ((lRGBMax - lGreen) / (lRGBMax - lRGBMin) / 6.0f);
		float	 lBDelta = ((lRGBMax - lBlue) / (lRGBMax - lRGBMin) / 6.0f);

		if	(lRed == lRGBMax)
		{
			pHue = lBDelta - lGDelta;
		}
		else
		if	(lGreen == lRGBMax)
		{
			pHue = lRDelta - lBDelta + (1.0f / 3.0f);
		}
		else
		{
			pHue = lGDelta - lRDelta + (2.0f / 3.0f);
		}

		if	(pHue < 0.0f)
		{
			pHue += 1.0f;
		}

		if	(pHue >= 1.0f)
		{
			pHue -= 1.0f;
		}
	}
}

void CColor::HslToRgb (float pHue, float pSaturation, float pLuminosity, BYTE& pRed, BYTE& pGreen, BYTE& pBlue)
{
	while (pHue < 0.0f)
	{
		pHue += 1.0f;
	}
	while (pHue > 1.0f)
	{
		pHue -= 1.0f;
	}
	pLuminosity = min (max (pLuminosity, 0.0f), 1.0f);
	pSaturation = min (max (pSaturation, 0.0f), 1.0f);

	float	lRGBMin;
	float	lRGBMax;
	float	lRed, lGreen, lBlue;

	if	(pLuminosity <= 0.5)
	{
		lRGBMin = (pLuminosity - (pLuminosity * pSaturation));
		lRGBMax = (pLuminosity + (pLuminosity * pSaturation));
	}
	else
	{
		lRGBMin = ((pLuminosity * pSaturation) + pLuminosity - pSaturation);
		lRGBMax = (pLuminosity - (pLuminosity * pSaturation) + pSaturation);
	}

	pHue *= 6.0f;

	if	(pHue < 1.0f)
	{
		lRed = lRGBMax;
		lBlue = lRGBMin;
		lGreen = (lRGBMax + ((pHue - 1.0f) * (lRGBMax - lRGBMin)));
	}
	else
	if	(pHue < 2.0f)
	{
		lGreen = lRGBMax;
		lBlue = lRGBMin;
		lRed = (lRGBMax - ((pHue - 1.0f) * (lRGBMax - lRGBMin)));
	}
	else
	if	(pHue < 3.0f)
	{
		lGreen = lRGBMax;
		lRed = lRGBMin;
		lBlue = (lRGBMax + ((pHue - 3.0f) * (lRGBMax - lRGBMin)));
	}
	else
	if	(pHue < 4.0f)
	{
		lBlue = lRGBMax;
		lRed = lRGBMin;
		lGreen = (lRGBMax - ((pHue - 3.0f) * (lRGBMax - lRGBMin)));
	}
	else
	if	(pHue < 5.0f)
	{
		lBlue = lRGBMax;
		lGreen = lRGBMin;
		lRed = (lRGBMax + ((pHue - 5.0f) * (lRGBMax - lRGBMin)));
	}
	else
	{
		lRed = lRGBMax;
		lGreen = lRGBMin;
		lBlue = (lRGBMax - ((pHue - 5.0f) * (lRGBMax - lRGBMin)));
	}

	if	(lRed < 0.0f)
	{
		lRed += 1.0f;
	}
	if	(lRed > 1.0f)
	{
		lRed -= 1.0f;
	}

	if	(lGreen < 0.0f)
	{
		lGreen += 1.0f;
	}
	if	(lGreen > 1.0f)
	{
		lGreen -= 1.0f;
	}

	if	(lBlue < 0.0f)
	{
		lBlue += 1.0f;
	}
	if	(lBlue > 1.0f)
	{
		lBlue -= 1.0f;
	}

	lRed *= 255.0f;
	lGreen *= 255.0f;
	lBlue *= 255.0f;

	pRed = (BYTE) (unsigned short) (short) (long) lRed;
	pGreen = (BYTE) (unsigned short) (short) (long) lGreen;
	pBlue = (BYTE) (unsigned short) (short) (long) lBlue;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CColor::RgbToHsl (BYTE pRed, BYTE pGreen, BYTE pBlue, BYTE& pHue, BYTE& pSaturation, BYTE& pLuminosity)
{
	float	lHue;
	float	lSaturation;
	float	lLuminosity;

	RgbToHsl (pRed, pGreen, pBlue, lHue, lSaturation, lLuminosity);

	pHue = (BYTE) (UINT) (long) (lHue * 255.0f);
	pSaturation = (BYTE) (UINT) (long) (lSaturation * 255.0f);
	pLuminosity = (BYTE) (UINT) (long) (lLuminosity * 255.0f);
}

void CColor::HslToRgb (BYTE pHue, BYTE pSaturation, BYTE pLuminosity, BYTE& pRed, BYTE& pGreen, BYTE& pBlue)
{
	float	lHue = ((float) (int) pHue) / 255.0f;
	float	lSaturation = ((float) (int) pSaturation) / 255.0f;
	float	lLuminosity = ((float) (int) pLuminosity) / 255.0f;

	HslToRgb (lHue, lSaturation, lLuminosity, pRed, pGreen, pBlue);
}

//////////////////////////////////////////////////////////////////////

void CColor::RgbToXyz (float pRed, float pGreen, float pBlue, float& pXVal, float& pYVal, float& pZVal)
{
	pXVal =	((pRed * (0.412453f / 255.0f)) + (pGreen * (0.357580f / 255.0f)) + (pBlue * (0.180423f / 255.0f))) / 0.0095047f;
	pYVal =	((pRed * (0.212671f / 255.0f)) + (pGreen * (0.715160f / 255.0f)) + (pBlue * (0.072169f / 255.0f))) / 0.0100000f;
	pZVal =	((pRed * (0.019334f / 255.0f)) + (pGreen * (0.119193f / 255.0f)) + (pBlue * (0.950227f / 255.0f))) / 0.0108883f;
}

void CColor::RgbToXyz (BYTE pRed, BYTE pGreen, BYTE pBlue, BYTE& pXVal, BYTE& pYVal, BYTE& pZVal, bool pScaled)
{
	float	lXVal;
	float	lYVal;
	float	lZVal;

	RgbToXyz ((float) (int) pRed, (float) (int) pGreen, (float) (int) pBlue, lXVal, lYVal, lZVal);

	if	(pScaled)
	{
		pXVal = (BYTE) (UINT) min ((long) (lXVal * 2.55f), 255L);
		pYVal = (BYTE) (UINT) min ((long) (lYVal * 2.55f), 255L);
		pZVal = (BYTE) (UINT) min ((long) (lZVal * 2.55f), 255L);
	}
	else
	{
		pXVal = (BYTE) (UINT) (long) lXVal;
		pYVal = (BYTE) (UINT) (long) lYVal;
		pZVal = (BYTE) (UINT) (long) lZVal;
	}
}
