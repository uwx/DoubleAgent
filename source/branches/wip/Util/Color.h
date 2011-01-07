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
#if !defined(AFX_COLOR_H__D80C3B0A_93F5_440E_A586_A24BBE726355__INCLUDED_)
#define AFX_COLOR_H__D80C3B0A_93F5_440E_A586_A24BBE726355__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

typedef struct
{
	float Hue;
	float Saturation;
	float Luminosity;
}	HSLTRIPLE;

//////////////////////////////////////////////////////////////////////

class CColor
#ifdef	__AFX_H__
	: public CObject
#endif
{
public:
	CColor (COLORREF pColor = 0);
	virtual ~CColor () {}

	CColor (const CColor & pSource);
	CColor & operator= (const CColor & pSource);
	CColor & operator= (COLORREF pColor);
	bool operator== (const CColor & pColor) const;
	bool operator!= (const CColor & pColor) const;

	CColor & operator|= (const CColor & pSource);
	CColor & operator|= (COLORREF pColor);
	CColor & operator&= (const CColor & pSource);
	CColor & operator&= (COLORREF pColor);

// Attributes
	operator COLORREF () const {return mColor;}

	void GetRGB (BYTE & pRed, BYTE & pGreen, BYTE & pBlue) const;
	void GetRGBA (BYTE & pRed, BYTE & pGreen, BYTE & pBlue, BYTE & pAlpha) const;
	void GetHSL (float & pHue, float & pSaturation, float & pLuminosity) const;

	CColor & SetRGB (BYTE pRed, BYTE pGreen, BYTE pBlue);
	CColor & SetRGBA (BYTE pRed, BYTE pGreen, BYTE pBlue, BYTE pAlpha);
	CColor & SetHSL (float pHue, float pSaturation, float pLuminosity);

	inline void GetRGB (RGBTRIPLE & pRGB) const {GetRGB (pRGB.rgbtRed, pRGB.rgbtGreen, pRGB.rgbtBlue);}
	inline void GetRGB (RGBQUAD & pRGB) const {GetRGB (pRGB.rgbRed, pRGB.rgbGreen, pRGB.rgbBlue);}
	inline void GetRGBA (RGBQUAD & pRGB) const {GetRGBA (pRGB.rgbRed, pRGB.rgbGreen, pRGB.rgbBlue, pRGB.rgbReserved);}
	inline void GetHSL (HSLTRIPLE & pHSL) const {GetHSL (pHSL.Hue, pHSL.Saturation, pHSL.Luminosity);}

	inline CColor & SetRGB (const RGBTRIPLE & pRGB) {SetRGB (pRGB.rgbtRed, pRGB.rgbtGreen, pRGB.rgbtBlue); return *this;}
	inline CColor & SetRGB (const RGBQUAD & pRGB) {SetRGB (pRGB.rgbRed, pRGB.rgbGreen, pRGB.rgbBlue); return *this;}
	inline CColor & SetRGBA (const RGBQUAD & pRGB) {SetRGBA (pRGB.rgbRed, pRGB.rgbGreen, pRGB.rgbBlue, pRGB.rgbReserved); return *this;}
	inline CColor & SetHSL (const HSLTRIPLE & pHSL) {SetHSL (pHSL.Hue, pHSL.Saturation, pHSL.Luminosity); return *this;}

	inline float GetHue () const {HSLTRIPLE lHsl; GetHSL (lHsl); return lHsl.Hue;}
	inline float GetSaturation () const {HSLTRIPLE lHsl; GetHSL (lHsl); return lHsl.Saturation;}
	inline float GetLuminosity () const {HSLTRIPLE lHsl; GetHSL (lHsl); return lHsl.Luminosity;}

	inline BYTE GetRed () const {return GetRValue (mColor);}
	inline BYTE GetGreen () const {return GetGValue (mColor);}
	inline BYTE GetBlue () const {return GetBValue (mColor);}
	inline BYTE GetAlpha () const {return (LOBYTE(mColor>>24));}

	inline CColor & SetRed (BYTE pRed) {mColor&=0xFFFFFF00; mColor|=(COLORREF)pRed; return *this;}
	inline CColor & SetGreen (BYTE pGreen) {mColor&=0xFFFF00FF; mColor|=((COLORREF)pGreen)<<8; return *this;}
	inline CColor & SetBlue (BYTE pBlue) {mColor&=0xFF00FFFF; mColor|=((COLORREF)pBlue)<<16; return *this;}
	inline CColor & SetAlpha (BYTE pAlpha) {mColor&=0x00FFFFFF; mColor|=((COLORREF)pAlpha)<<24; return *this;}

	inline COLORREF GetRedMask () const {return mColor&0x000000FF;}
	inline COLORREF GetGreenMask () const {return mColor&0x0000FF00;}
	inline COLORREF GetBlueMask () const {return mColor&0x00FF0000;}
	inline COLORREF GetAlphaMask () const {return mColor&0xFF000000;}
	inline COLORREF GetRGBMask () const {return mColor&0x00FFFFFF;}

	inline COLORREF SetRedMask (COLORREF pRedMask) {mColor&=0xFFFFFF00; mColor|=(pRedMask&0x000000FF); return mColor;}
	inline COLORREF SetGreenMask (COLORREF pGreenMask) {mColor&=0xFFFF00FF; mColor|=(pGreenMask&0x0000FF00); return mColor;}
	inline COLORREF SetBlueMask (COLORREF pBlueMask) {mColor&=0xFF00FFFF; mColor|=(pBlueMask&0x00FF0000); return mColor;}
	inline COLORREF SetAlphaMask (COLORREF pAlphaMask) {mColor&=0x00FFFFFF; mColor|=(pAlphaMask&0xFF000000); return mColor;}
	inline COLORREF SetRGBMask (COLORREF pRGBMask) {mColor&=0xFF000000; mColor|=(pRGBMask&0x00FFFFFF); return mColor;}
	inline COLORREF SetRGBAMask (COLORREF pRGBMask, COLORREF pAlphaMask = 0xFF000000) {mColor=(pRGBMask&0x00FFFFFF)|(pAlphaMask&0xFF000000); return mColor;}

	void GetRGB (TRIVERTEX & pVertex) const;
	void GetRGBA (TRIVERTEX & pVertex) const;

// Operations
	bool IsGrayColor (float pMaxSaturation = 0.1f) const;

    CColor GetShade (float pPercentSaturation, float pPercentLuminosity) const;
    CColor GetMonoColor (int pBrightnessBias = 0) const;
    CColor GetGrayColor (int pBrightnessBias = 0) const;
	static CColor GetAverage (const CColor & pColor1, const CColor & pColor2);

    CColor MakeContrastColor (int pBrightnessBias = 0, float pContrast = 0.25f) const;
    CColor MakeFillTextColor (float pContrast = 0.6f, float pMaxSaturation = 0.8f) const;
    CColor MakeTextColor (float pContrast = 0.5f, float pMaxSaturation = 0.9f, COLORREF pBkColor = GetSysColor (COLOR_WINDOW)) const;
	CColor MakeAlphaColor (BYTE pAlpha) const {return CColor ((mColor&0x00FFFFFF)|(((COLORREF)pAlpha)<<24));}

	CColor ClampHSL (float pMinHue = 0.0f, float pMaxHue = 1.0f, float pMinSaturation = 0.0f, float pMaxSaturation = 1.0f, float pMinLuminosity = 0.0f, float pMaxLuminosity = 1.0f) const;
    CColor ClampShadePercent (float pPercentSaturation, float pPercentLuminosity, float pMinHue = 0.0f, float pMaxHue = 1.0f, float pMinSaturation = 0.0f, float pMaxSaturation = 1.0f, float pMinLuminosity = 0.0f, float pMaxLuminosity = 1.0f) const;
    CColor ClampShadeDelta (float pDeltaSaturation, float pDeltaLuminosity, float pMinHue = 0.0f, float pMaxHue = 1.0f, float pMinSaturation = 0.0f, float pMaxSaturation = 1.0f, float pMinLuminosity = 0.0f, float pMaxLuminosity = 1.0f) const;

    static void RgbToHsl (BYTE pRed, BYTE pGreen, BYTE pBlue, float & pHue, float & pSaturation, float & pLuminosity);
    static void HslToRgb (float pHue, float pSaturation, float pLuminosity, BYTE & pRed, BYTE & pGreen, BYTE & pBlue);

    static void RgbToHsl (BYTE pRed, BYTE pGreen, BYTE pBlue, BYTE & pHue, BYTE & pSaturation, BYTE & pLuminosity);
    static void HslToRgb (BYTE pHue, BYTE pSaturation, BYTE pLuminosity, BYTE & pRed, BYTE & pGreen, BYTE & pBlue);

    static inline void RgbToHsl (const RGBTRIPLE & pRGB, HSLTRIPLE & pHSL) {RgbToHsl (pRGB.rgbtRed, pRGB.rgbtBlue, pRGB.rgbtGreen, pHSL.Hue, pHSL.Saturation, pHSL.Luminosity);}
    static inline void HslToRgb (const HSLTRIPLE & pHSL, RGBTRIPLE & pRGB) {HslToRgb (pHSL.Hue, pHSL.Saturation, pHSL.Luminosity, pRGB.rgbtRed, pRGB.rgbtBlue, pRGB.rgbtGreen);}

    static inline void RgbToHsl (const RGBQUAD & pRGB, HSLTRIPLE & pHSL) {RgbToHsl (pRGB.rgbRed, pRGB.rgbBlue, pRGB.rgbGreen, pHSL.Hue, pHSL.Saturation, pHSL.Luminosity);}
    static inline void HslToRgb (const HSLTRIPLE & pHSL, RGBQUAD & pRGB) {HslToRgb (pHSL.Hue, pHSL.Saturation, pHSL.Luminosity, pRGB.rgbRed, pRGB.rgbBlue, pRGB.rgbGreen);}

	static void RgbToXyz (float pRed, float pGreen, float pBlue, float & pXVal, float & pYVal, float & pZVal);
	static void RgbToXyz (BYTE pRed, BYTE pGreen, BYTE pBlue, BYTE & pXVal, BYTE & pYVal, BYTE & pZVal, bool pScaled = true);

#ifdef	_GDIPLUSCOLOR_H
	operator Gdiplus::Color () const {return Gdiplus::Color (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor));}
	Gdiplus::Color GdiplusAlpha () const {return Gdiplus::Color (LOBYTE (mColor>>24), GetRValue (mColor), GetGValue (mColor), GetBValue (mColor));}
#endif

#ifdef	D3DCOLOR_RGBA
	D3DCOLOR D3DColor () const {return D3DCOLOR_RGBA (GetRValue (mColor), GetGValue (mColor), GetBValue (mColor), LOBYTE (mColor>>24));}
#else
	DWORD D3DColor () const {return (mColor&0xFF000000) | (GetRValue (mColor)<<16) | (GetGValue (mColor)<<8) | GetBValue (mColor);}
#endif

// Implementation
protected:
#ifdef	__AFX_H__
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	COLORREF	mColor;
};

#endif // !defined(AFX_COLOR_H__D80C3B0A_93F5_440E_A586_A24BBE726355__INCLUDED_)
