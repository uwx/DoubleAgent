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
#ifndef _IMAGEDEBUGGER_H_
#define _IMAGEDEBUGGER_H_

#include <atlwin.h>
#include <atlimage.h>
#include "HandleTemplates.h"
#include "AtlCollEx.h"

//////////////////////////////////////////////////////////////////////

class CImageDebugger : public CWindowImpl<CImageDebugger>
{
public:
    CImageDebugger (HWND pParentWin = NULL);
    virtual ~CImageDebugger ();

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
	static bool DumpBitmapInfo (UINT pLogLevel, BITMAPINFO & pBitmap, LPCTSTR pTitle = NULL);

	static bool DumpBitmap (UINT pLogLevel, HBITMAP pBitmap, LPCTSTR pTitle = NULL, bool pAlphaOnly = false, UINT pMaxWidth = 100, UINT pMaxHeight = 100);
	static bool DumpBitmap (UINT pLogLevel, BITMAP & pBitmap, LPCTSTR pTitle = NULL, bool pAlphaOnly = false, UINT pMaxWidth = 100, UINT pMaxHeight = 100);
	static bool DumpBitmap (UINT pLogLevel, BITMAPINFO & pBitmap, LPBYTE pBits, LPCTSTR pTitle = NULL, bool pAlphaOnly = false, UINT pMaxWidth = 100, UINT pMaxHeight = 100);

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

	static bool CopyBitmap (CBitmapHandle & pTrg, HBITMAP pSrc);
	static bool CreateBitmap (HDC pDC, const CSize & pSize, UINT pBitCount, CBitmapHandle & pBitmap);
	static HBRUSH CreateHalftoneBrush ();

protected:
	BEGIN_MSG_MAP(CImageDebugger)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnRButtonDown)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	//}}AFX_MSG

private:
	bool CreateMyWindow (LPCTSTR pTitle);
	void SetWindowSize (CSize & pSize);
	void DrawLabel (HDC pDC, const CRect & pRect, LPCTSTR pLabel);

	UINT SaveSeries (const CSize & pSize, UINT pBitCount, INT_PTR pMinNdx, INT_PTR pMaxNdx);
	static CAtlString GetDefaultPath ();
	static bool MakePathExist (LPCTSTR pPath);
	static CAtlString GetNewFileName (const CAtlString pFileName);

private:
	HWND							mParentWin;
    CBitmapHandle					mBitmap;
    CBitmapHandle					mBitmapMask;
	CBitmapHandle					mBitmapAlpha;
	CIconHandle						mIcon;
    bool							mOwnBitmap;
    UINT							mScale;
	UINT							mSeriesNum;
	CAtlString						mSeriesName;
	CAtlTypeArray <CBitmapHandle>	mSeries;
	static UINT						mLastSeriesNum;
	static bool						mSeriesEnabled;
};

//////////////////////////////////////////////////////////////////////
#endif	// _IMAGEDEBUGGER_H_
