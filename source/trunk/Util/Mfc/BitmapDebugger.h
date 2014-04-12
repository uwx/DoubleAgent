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
#ifndef _BITMAP_DEBUGGER_H_
#define _BITMAP_DEBUGGER_H_
//////////////////////////////////////////////////////////////////////
class CBitmapDebugger : public CWnd
{
public:
    CBitmapDebugger (CWnd * pParentWin = NULL);
    virtual ~CBitmapDebugger ();

//---------------------------------------------------------------------------

    void ShowBitmap
    (
        HBITMAP pBitmap,
        UINT pScale = 1,
		LPCTSTR pTitle = NULL,
		bool pShowAlpha = true,
        bool pMakeCopy = true
    );

    void ShowIcon
    (
        HICON pIcon,
        UINT pScale = 1,
		LPCTSTR pTitle = NULL
    );

	int DoModal ();

//---------------------------------------------------------------------------

	static bool DumpBitmapInfo (UINT pLogLevel, HBITMAP pBitmap, LPCTSTR pTitle = NULL);
	static bool DumpBitmapInfo (UINT pLogLevel, BITMAPINFO& pBitmap, LPCTSTR pTitle = NULL);

	static bool DumpBitmap (UINT pLogLevel, HBITMAP pBitmap, LPCTSTR pTitle = NULL, bool pAlphaOnly = false, UINT pMaxWidth = 100, UINT pMaxHeight = 100);
	static bool DumpBitmap (UINT pLogLevel, BITMAP& pBitmap, LPCTSTR pTitle = NULL, bool pAlphaOnly = false, UINT pMaxWidth = 100, UINT pMaxHeight = 100);
	static bool DumpBitmap (UINT pLogLevel, BITMAPINFO& pBitmap, LPBYTE pBits, LPCTSTR pTitle = NULL, bool pAlphaOnly = false, UINT pMaxWidth = 100, UINT pMaxHeight = 100);

	static bool DumpPalette (UINT pLogLevel, HPALETTE pPalette, LPCTSTR pTitle = NULL);

//---------------------------------------------------------------------------

	static bool SaveBitmap (HBITMAP pBitmap, LPCTSTR pFileName, LPCTSTR pPath = NULL, bool pReplace = true);
	static bool SaveIcon (HICON pIcon, LPCTSTR pFileName, LPCTSTR pPath = NULL, bool pReplace = true);

	static void EnableSeries (bool pEnable);
	static bool SeriesEnabled ();
	static bool ClearAllSeries ();
	static UINT GetNextSeriesNum ();
	static void SetNextSeriesNum (UINT pNextSeriesNum);

	UINT StartSeries (LPCTSTR pFileName = NULL);
	UINT EndSeries (bool pSave = true);
	UINT UpdateSeries (HBITMAP pBitmap, bool pTakeBitmap = false);

//---------------------------------------------------------------------------

	static bool CopyBitmap (CBitmap& pTrg, HBITMAP pSrc);
	static bool CreateBitmap (CDC& pDC, const CSize& pSize, UINT pBitCount, CBitmap& pBitmap);

private:

	//{{AFX_MSG(CBitmapDebugger)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint ();
	afx_msg void OnClose ();
	afx_msg void OnDestroy ();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool CreateMyWindow (LPCTSTR pTitle);
	void SetWindowSize (CSize& pSize);
	void DrawLabel (CDC& pDC, const CRect& pRect, LPCTSTR pLabel);

	bool AlphaBlend (CDC& pDst, const CRect& pDstRect, CDC& pSrc, const CRect& pSrcRect, BYTE pAlpha = 255);

	UINT SaveSeries (const CSize& pSize, UINT pBitCount, int pMinNdx, int pMaxNdx);
	static CString GetDefaultPath ();
	static bool MakePathExist (LPCTSTR pPath);
	static CString GetNewFileName (const CString pFileName);

	CWnd *								mParentWin;
    CBitmap								mBitmap;
    CBitmap								mBitmapMask;
	CBitmap								mBitmapAlpha;
	HICON								mIcon;
    bool								mOwnBitmap;
    UINT								mScale;
	UINT								mSeriesNum;
	CString								mSeriesName;
	CArray <tPtr <CBitmap>, CBitmap *>	mSeries;
	static UINT							mLastSeriesNum;
	static bool							mSeriesEnabled;

	typedef BOOL (WINAPI *				tAlphaBlend) (HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
	tAlphaBlend							mAlphaBlend;
	HMODULE								mEffectsModule;
};
//////////////////////////////////////////////////////////////////////
#endif
