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
#include "ImageDebugger.h"
#include "ImageAlpha.h"
#include "Log.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef	_DEBUG
//#define	_DEBUG_SERIES	LogNormal
#endif

////////////////////////////////////////////////////////////////////////

LPCTSTR __declspec(selectany)	_AtlProfilePath = _T("Software\\Cinnamon\\");
LPCTSTR __declspec(selectany)	_AtlProfileName = _T("Debug");
LPCTSTR							sProfileKey = _T("ImageDebugger");
LPCTSTR							sProfileX = _T("Left");
LPCTSTR							sProfileY = _T("Top");
static const int				sLabelWidth = 50;

////////////////////////////////////////////////////////////////////////

CImageDebugger::CImageDebugger (HWND pParentWin)
:	mOwnBitmap (false),
	mParentWin (pParentWin),
	mScale (1),
	mSeriesNum (0)
{
}

////////////////////////////////////////////////////////////////////////

CImageDebugger::~CImageDebugger ()
{
	if	(IsWindow ())
	{
		DestroyWindow ();
	}

	if	(
			(mBitmap.GetSafeHandle ())
		&&	(mOwnBitmap)
		)
	{
		mBitmap.Close ();
	}
	else
	{
		mBitmap.Detach ();
	}

	if	(
			(mBitmapMask.GetSafeHandle ())
		&&	(mOwnBitmap)
		)
	{
		mBitmapMask.Close ();
	}
	else
	{
		mBitmapMask.Detach ();
	}
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

void CImageDebugger::ShowBitmap
(
	HBITMAP pBitmap,
	UINT pScale,
	LPCTSTR pTitle,
	bool pShowAlpha,
	bool pMakeCopy
)
{
	CSize			lBitmapSize;
	tS <BITMAP>		lBitmapInfo;
	CBitmapHandle	lBitmap;

	if	(
			(
				(pMakeCopy)
			?	(CopyBitmap (lBitmap, pBitmap))
			:	(lBitmap.Attach (pBitmap))
			)
		&&	(GetObject (lBitmap, sizeof (lBitmapInfo), &lBitmapInfo))
		)
	{
		if	(
				(mBitmap.GetSafeHandle ())
			&&	(mOwnBitmap)
			)
		{
			mBitmap.Close ();
		}
		else
		{
			mBitmap.Detach ();
		}

		if	(
				(mBitmapMask.GetSafeHandle ())
			&&	(mOwnBitmap)
			)
		{
			mBitmapMask.Close ();
		}
		else
		{
			mBitmapMask.Detach ();
		}

		mBitmapAlpha.Close ();
		mIcon.Close ();

		if	(pMakeCopy)
		{
			mOwnBitmap = true;
		}
		else
		{
			mOwnBitmap = false;
		}

		mBitmap.Attach (lBitmap.Detach ());
		mScale = max (min (pScale, (UINT) 16), (UINT) 1);

		lBitmapSize.cx = lBitmapInfo.bmWidth;
		lBitmapSize.cy = lBitmapInfo.bmHeight;
		lBitmapSize.cx *= mScale;
		lBitmapSize.cy *= mScale;

		if	(
				(pShowAlpha)
			&&	(lBitmapInfo.bmBitsPixel > 24)
			)
		{
			lBitmapInfo.Clear ();

			if	(
					(CopyBitmap (mBitmapAlpha, mBitmap))
				&&	(GetObject (mBitmapAlpha, sizeof (lBitmapInfo), &lBitmapInfo))
				&&	(lBitmapInfo.bmBits)
				)
			{
				try
				{
					LPBYTE	lBits = (LPBYTE) lBitmapInfo.bmBits;
					long	lNdx;

					for	(lNdx = 0; lNdx < (lBitmapInfo.bmWidth * lBitmapInfo.bmHeight * 4); lNdx += 4)
					{
						lBits [lNdx] = lBits [lNdx+3];
						lBits [lNdx+1] = lBits [lNdx+3];
						lBits [lNdx+2] = lBits [lNdx+3];
						lBits [lNdx+3] = 255;
					}
				}
				catch AnyExceptionSilent

#ifndef	_UNICODE
				if	(mScale == 1)
				{
					lBitmapSize.cy *= 8;
				}
				else
#endif
				{
					lBitmapSize.cy *= 5;
				}
			}
			else
			{
				mBitmapAlpha.Close ();
#ifndef	_UNICODE
				if	(mScale == 1)
				{
					lBitmapSize.cy *= 7;
				}
				else
#endif
				{
					lBitmapSize.cy *= 4;
				}
			}
		}

		if	(
				(IsWindow ())
			||	(CreateMyWindow (pTitle))
			)
		{
			SetWindowSize (lBitmapSize);

			if	(pTitle)
			{
				SetWindowText (pTitle);
			}
			RedrawWindow ();
		}
	}
}

////////////////////////////////////////////////////////////////////////

void CImageDebugger::ShowIcon
(
	HICON pIcon,
	UINT pScale,
	LPCTSTR pTitle
)
{
	if	(pIcon)
	{
		if	(
				(mBitmap.GetSafeHandle ())
			&&	(mOwnBitmap)
			)
		{
			mBitmap.Close ();
		}
		else
		{
			mBitmap.Detach ();
		}

		if	(
				(mBitmapMask.GetSafeHandle ())
			&&	(mOwnBitmap)
			)
		{
			mBitmapMask.Close ();
		}
		else
		{
			mBitmapMask.Detach ();
		}

		mBitmapAlpha.Close ();
		mIcon.Close ();

		ICONINFO	lIconInfo;
		CSize		lIconSize;

		memset (&lIconInfo, 0, sizeof(lIconInfo));

		if	(GetIconInfo (pIcon, &lIconInfo))
		{
			mOwnBitmap = true;

			mIcon = (HICON) CopyImage (pIcon, IMAGE_ICON, 0, 0, LR_COPYRETURNORG);

			if	(mBitmap.Attach (lIconInfo.hbmColor))
			{
				tS <BITMAP>	lBitmapInfo;

				GetObject (mBitmap, sizeof (lBitmapInfo), &lBitmapInfo);
				if	(lBitmapInfo.bmHeight == lBitmapInfo.bmWidth * 2)
				{
					lBitmapInfo.bmHeight /= 2;
				}
				lIconSize.cx = lBitmapInfo.bmWidth;
				lIconSize.cy = lBitmapInfo.bmHeight;

				if	(mBitmapMask.Attach (lIconInfo.hbmMask))
				{
					tS <BITMAP>	lMaskInfo;

					GetObject (mBitmapMask, sizeof (lMaskInfo), &lMaskInfo);
					lIconSize.cy += lBitmapInfo.bmHeight;
				}

				lIconSize.cy += (lBitmapInfo.bmHeight * 3);

				if	(lBitmapInfo.bmBitsPixel > 24)
				{
					lBitmapInfo.Clear ();

					if	(
							(CopyBitmap (mBitmapAlpha, mBitmap))
						&&	(GetObject (mBitmapAlpha, sizeof (lBitmapInfo), &lBitmapInfo))
						&&	(lBitmapInfo.bmBits)
						)
					{
						try
						{
							LPBYTE	lBits = (LPBYTE) lBitmapInfo.bmBits;
							long	lNdx;

							for	(lNdx = 0; lNdx < (lBitmapInfo.bmWidth * lBitmapInfo.bmHeight * 4); lNdx += 4)
							{
								lBits [lNdx] = lBits [lNdx+3];
								lBits [lNdx+1] = lBits [lNdx+3];
								lBits [lNdx+2] = lBits [lNdx+3];
								lBits [lNdx+3] = 255;
							}
						}
						catch AnyExceptionSilent

						lIconSize.cy += lBitmapInfo.bmHeight;
					}
					else
					{
						mBitmapAlpha.Close ();
					}
				}
			}
			else
			if	(mBitmap.Attach (lIconInfo.hbmMask))
			{
				tS <BITMAP>	lBitmapInfo;

				GetObject (mBitmap, sizeof (lBitmapInfo), &lBitmapInfo);
				lIconSize.cx = lBitmapInfo.bmWidth;
				lIconSize.cy = lBitmapInfo.bmHeight;
			}

			if	(
					(IsWindow ())
				||	(CreateMyWindow (pTitle))
				)
			{
				mScale = max (min (pScale, (UINT) 16), (UINT) 1);
				lIconSize.cx *= mScale;
				lIconSize.cy *= mScale;

				SetWindowSize (lIconSize);

				if	(pTitle)
				{
					SetWindowText (pTitle);
				}
				Invalidate (TRUE);
				UpdateWindow ();
			}
		}
		else
		{
			LogWinErr (LogIfActive|LogTime, GetLastError (), _T("GetIconInfo"));
		}
	}
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

int CImageDebugger::DoModal ()
{
//TODO
ATLASSERT(FALSE);
#if	FALSE
	return RunModalLoop (MLF_NOKICKIDLE);
#endif
	return 0;
}

bool CImageDebugger::CreateMyWindow (LPCTSTR pTitle)
{
	CRect			lInitialRect;
	CPoint			lInitialPos;
	CAtlString		lTitle (pTitle);
	ATL::CRegKey	lRootKey;
	ATL::CRegKey	lAppKey;
	ATL::CRegKey	lProfileKey;
	DWORD			lProfileVal;

	lInitialRect.right = GetSystemMetrics (SM_CXFULLSCREEN);
	lInitialRect.bottom = GetSystemMetrics (SM_CYFULLSCREEN);
	lInitialRect.left = lInitialRect.right - GetSystemMetrics (SM_CXICON);
	lInitialRect.top = lInitialRect.bottom - GetSystemMetrics (SM_CYICON);

	if	(
			(lRootKey.Open (HKEY_CURRENT_USER, _AtlProfilePath, KEY_READ) == ERROR_SUCCESS)
		&&	(lAppKey.Open (lRootKey, _AtlProfileName, KEY_READ) == ERROR_SUCCESS)
		&&	(lProfileKey.Open (lAppKey, sProfileKey+lTitle, KEY_READ) == ERROR_SUCCESS)
		)
	{
		lInitialPos = lInitialRect.TopLeft ();
		if	(lProfileKey.QueryDWORDValue (sProfileX, lProfileVal) == ERROR_SUCCESS)
		{
			lInitialPos.x = (int) lProfileVal;
		}
		if	(lProfileKey.QueryDWORDValue (sProfileY, lProfileVal) == ERROR_SUCCESS)
		{
			lInitialPos.y = (int) lProfileVal;
		}
		lInitialRect.OffsetRect (lInitialPos.x - lInitialRect.left, lInitialPos.y - lInitialRect.top);
	}

	if	(Create (mParentWin, lInitialRect, lTitle, WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_POPUP|WS_CLIPSIBLINGS, WS_EX_TOOLWINDOW|WS_EX_TOPMOST))
	{
		ShowWindow (SW_SHOWNOACTIVATE);
		UpdateWindow ();
		return true;
	}
	return false;
}

void CImageDebugger::SetWindowSize (CSize& pSize)
{
	CSize	lScreenSize;
	CRect	lOldRect;
	CRect	lNewRect;

	lScreenSize.cx = GetSystemMetrics (SM_CXFULLSCREEN);
	lScreenSize.cy = GetSystemMetrics (SM_CYFULLSCREEN);

	GetWindowRect (&lOldRect);
	lNewRect = CRect (CPoint (0, 0), pSize);
	lNewRect.right += sLabelWidth;
	AdjustWindowRectEx (&lNewRect, GetStyle (), FALSE, GetExStyle ());
	lNewRect = CRect (lOldRect.TopLeft (), lNewRect.Size ());

	if	(lNewRect.right > lScreenSize.cx)
	{
		lNewRect.OffsetRect (lScreenSize.cx-lNewRect.right, 0);
	}
	if	(lNewRect.left < 0)
	{
		lNewRect.OffsetRect (-lNewRect.left, 0);
	}
	if	(lNewRect.bottom > lScreenSize.cy)
	{
		lNewRect.OffsetRect (0, lScreenSize.cy-lNewRect.bottom);
	}
	if	(lNewRect.top < 0)
	{
		lNewRect.OffsetRect (0, -lNewRect.top);
	}

	if	(!lNewRect.EqualRect (&lOldRect))
	{
		MoveWindow (&lNewRect, TRUE);
	}
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

LRESULT CImageDebugger::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAtlString		lTitle;
	CRect			lRect;
	ATL::CRegKey	lRootKey;
	ATL::CRegKey	lAppKey;
	ATL::CRegKey	lProfileKey;

	GetWindowText (lTitle);
	GetWindowRect (&lRect);

	lRootKey.Create (HKEY_CURRENT_USER, _AtlProfilePath);
	lAppKey.Create (lRootKey, _AtlProfileName);
	lProfileKey.Create (lAppKey, sProfileKey+lTitle);
	lProfileKey.SetDWORDValue (sProfileX, lRect.left);
	lProfileKey.SetDWORDValue (sProfileY, lRect.top);

	bHandled = FALSE;
	return 0;
}

LRESULT CImageDebugger::OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if	(m_nFlags & WF_MODALLOOP)
	//{
	//	EndModalLoop (IDCLOSE);
	//}
	bHandled = FALSE;
	return 0;
}

LRESULT CImageDebugger::OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if	(m_nFlags & WF_MODALLOOP)
	//{
	//	EndModalLoop (IDOK);
	//}
	bHandled = FALSE;
	return 0;
}

LRESULT CImageDebugger::OnRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if	(m_nFlags & WF_MODALLOOP)
	//{
	//	EndModalLoop (IDCANCEL);
	//}
	bHandled = FALSE;
	return 0;
}

LRESULT CImageDebugger::OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if	(mBitmap.GetSafeHandle ())
	{
		return TRUE;
	}
	bHandled = FALSE;
	return 0;
}

////////////////////////////////////////////////////////////////////////

LRESULT CImageDebugger::OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	tS <PAINTSTRUCT>	lPaintStruct;
	CMemDCHandle		lPaintDC;
	CRect				lClientRect;
	CPoint				lImagePos;
	CSize				lImageSize;
	CRect				lLabelRect;
	CMemDCHandle 		lWorkDC;
	CBrushHandle		lHalftoneBrush;
	BITMAP				lBitmapInfo;
	HGDIOBJ  			lOldBitmap;

	if	(lPaintDC = BeginPaint (&lPaintStruct))
	{
		GetClientRect (&lClientRect);
		lImagePos = lClientRect.TopLeft ();
		lImageSize = lClientRect.Size ();

		SetBkMode (lPaintDC, OPAQUE);
		lHalftoneBrush = CreateHalftoneBrush ();

		if	(
				(mBitmap.GetSafeHandle ())
			&&	(lWorkDC.Attach (CreateCompatibleDC (lPaintDC)))
			&&	(lClientRect.Width () > sLabelWidth)
			)
		{
			GetObject (mBitmap, sizeof (lBitmapInfo), &lBitmapInfo);

			if	(
					(mIcon.GetSafeHandle ())
				&&	(mBitmapMask.GetSafeHandle ())
				)
			{
				if	(
						(lBitmapInfo.bmBitsPixel > 24)
					&&	(mBitmapAlpha.GetSafeHandle ())
					)
				{
					lImageSize.cy /= 6;

					SetBkColor (lPaintDC, RGB (0xFF, 0x80, 0X80));
					SetTextColor (lPaintDC, RGB (0xFF, 0xC0, 0xC0));
					FillRect (lPaintDC, CRect (CPoint (sLabelWidth, lImageSize.cy*3), lImageSize), lHalftoneBrush);
				}
				else
				{
					lImageSize.cy /= 5;

					SetBkColor (lPaintDC, RGB (0xFF, 0x80, 0X80));
					SetTextColor (lPaintDC, RGB (0xFF, 0xC0, 0xC0));
					FillRect (lPaintDC, CRect (CPoint (sLabelWidth, lImageSize.cy*2), lImageSize), lHalftoneBrush);
				}
			}
			else
			if	(
					(lBitmapInfo.bmBitsPixel > 24)
				&&	(mBitmapAlpha.GetSafeHandle ())
				)
			{
#ifndef	_UNICODE
				if	(mScale == 1)
				{
					lImageSize.cy /= 8;
				}
				else
#endif
				{
					lImageSize.cy /= 5;
				}
			}
			else
			if	(lBitmapInfo.bmBitsPixel > 24)
			{
#ifndef	_UNICODE
				if	(mScale == 1)
				{
					lImageSize.cy /= 7;
				}
				else
#endif
				{
					lImageSize.cy /= 4;
				}
			}
			else
			if	(mBitmapMask.GetSafeHandle ())
			{
				lImageSize.cy /= 2;
			}

			lImagePos.x += sLabelWidth;
			lImageSize.cx -= sLabelWidth;
			lLabelRect = lClientRect;
			lLabelRect.right = lLabelRect.left + sLabelWidth;
			lLabelRect.bottom = lLabelRect.top + lImageSize.cy;

			if	(lBitmapInfo.bmBitsPixel > 24)
			{
				SetBkColor (lPaintDC, RGB (0xFF, 0x80, 0X80));
				SetTextColor (lPaintDC, RGB (0xFF, 0xC0, 0xC0));
#ifdef	_UNICODE
				FillRect (lPaintDC, &CRect (lImagePos, lImageSize), lHalftoneBrush);
#else
				FillRect (lPaintDC, &CRect (lImagePos, CSize (lImageSize.cx, lImageSize.cy*2)), lHalftoneBrush);
#endif
			}

			SetTextColor (lPaintDC, RGB (0x00, 0x00, 0x00));
			SetBkColor (lPaintDC, RGB (0xFF, 0xFF, 0xFF));
			SetTextColor (lWorkDC, RGB (0x00, 0x00, 0x00));
			SetBkColor (lWorkDC, RGB (0xFF, 0xFF, 0xFF));

			lOldBitmap = SelectObject (lWorkDC, mBitmap);
			if	(lBitmapInfo.bmBitsPixel <= 24)
			{
				DrawLabel (lPaintDC, lLabelRect, _T("Blt"));
				StretchBlt (lPaintDC, lImagePos.x, lImagePos.y, lImageSize.cx, lImageSize.cy, lWorkDC, 0, 0, lBitmapInfo.bmWidth, lBitmapInfo.bmHeight, SRCCOPY);
			}
			else
			{
				DrawLabel (lPaintDC, lLabelRect, _T("Blend"));
				CImageAlpha::AlphaBlend (lPaintDC, CRect (lImagePos, lImageSize), lWorkDC, CRect (CPoint (0, 0), CSize (lBitmapInfo.bmWidth, lBitmapInfo.bmHeight)));
			}

			SelectObject (lWorkDC, lOldBitmap);
			lImagePos.y += lImageSize.cy;
			lLabelRect.OffsetRect (0, lImageSize.cy);

			if	(
					(lBitmapInfo.bmBitsPixel > 24)
				&&	(mBitmapAlpha.GetSafeHandle ())
				)
			{
				DrawLabel (lPaintDC, lLabelRect, _T("Alpha"));

				lOldBitmap = SelectObject (lWorkDC, mBitmapAlpha);
				StretchBlt (lPaintDC, lImagePos.x, lImagePos.y, lImageSize.cx, lImageSize.cy, lWorkDC, 0, 0, lBitmapInfo.bmWidth, lBitmapInfo.bmHeight, SRCCOPY);
				SelectObject (lWorkDC, lOldBitmap);
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);
			}

			if	(
					(lBitmapInfo.bmBitsPixel > 24)
				&&	(!mBitmapMask.GetSafeHandle ())
				)
			{
				lOldBitmap = SelectObject (lWorkDC, mBitmap);

				DrawLabel (lPaintDC, lLabelRect, _T("Bg FF"));

				FillSolidRect (lPaintDC, &CRect (lImagePos, lImageSize), RGB (0xFF, 0xFF, 0xFF));
				CImageAlpha::AlphaBlend (lPaintDC, CRect (lImagePos, lImageSize), lWorkDC, CRect (CPoint (0, 0), CSize (lBitmapInfo.bmWidth, lBitmapInfo.bmHeight)));
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);

				DrawLabel (lPaintDC, lLabelRect, _T("Bg 00"));

				FillSolidRect (lPaintDC, &CRect (lImagePos, lImageSize), RGB (0x00, 0x00, 0x00));
				CImageAlpha::AlphaBlend (lPaintDC, CRect (lImagePos, lImageSize), lWorkDC, CRect (CPoint (0, 0), CSize (lBitmapInfo.bmWidth, lBitmapInfo.bmHeight)));
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);

				DrawLabel (lPaintDC, lLabelRect, _T("Blt"));
				StretchBlt (lPaintDC, lImagePos.x, lImagePos.y, lImageSize.cx, lImageSize.cy, lWorkDC, 0, 0, lBitmapInfo.bmWidth, lBitmapInfo.bmHeight, SRCCOPY);

				SelectObject (lWorkDC, lOldBitmap);
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);
			}

			if	(mBitmapMask.GetSafeHandle ())
			{
				DrawLabel (lPaintDC, lLabelRect, _T("Mask"));

				SetTextColor (lPaintDC, RGB (0x00, 0x00, 0x00));
				SetBkColor (lPaintDC, RGB (0xFF, 0xFF, 0xFF));
				SetTextColor (lWorkDC, RGB (0x00, 0x00, 0x00));
				SetBkColor (lWorkDC, RGB (0xFF, 0xFF, 0xFF));

				GetObject (mBitmapMask, sizeof(lBitmapInfo), &lBitmapInfo);
				lOldBitmap = SelectObject (lWorkDC, mBitmapMask);
				StretchBlt (lPaintDC, lImagePos.x, lImagePos.y, lImageSize.cx, lImageSize.cy, lWorkDC, 0, 0, lBitmapInfo.bmWidth, lBitmapInfo.bmHeight, SRCCOPY);
				SelectObject (lWorkDC, lOldBitmap);
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);
			}
			lWorkDC.Close ();

			if	(mIcon)
			{
				DrawLabel (lPaintDC, lLabelRect, _T("Icon"));
				DrawIconEx (lPaintDC, lImagePos.x, lImagePos.y, mIcon, lImageSize.cx, lImageSize.cy, 0, NULL, DI_NORMAL);
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);

				DrawLabel (lPaintDC, lLabelRect, _T("Icon"));
				FillSolidRect (lPaintDC, &CRect (lImagePos, lImageSize), RGB (0xFF, 0xFF, 0xFF));
				DrawIconEx (lPaintDC, lImagePos.x, lImagePos.y, mIcon, lImageSize.cx, lImageSize.cy, 0, NULL, DI_NORMAL);
				lImagePos.y += lImageSize.cy;
				lLabelRect.OffsetRect (0, lImageSize.cy);

				DrawLabel (lPaintDC, lLabelRect, _T("Icon"));
				FillSolidRect (lPaintDC, &CRect (lImagePos, lImageSize), RGB (0x00, 0x00, 0x00));
				DrawIconEx (lPaintDC, lImagePos.x, lImagePos.y, mIcon, lImageSize.cx, lImageSize.cy, 0, NULL, DI_NORMAL);
			}
		}

		EndPaint (&lPaintStruct);
	}
	lPaintDC.Detach ();
	return 0;
}

////////////////////////////////////////////////////////////////////////

void CImageDebugger::DrawLabel (HDC pDC, const CRect& pRect, LPCTSTR pLabel)
{
	SaveDC (pDC);
	SetTextColor (pDC, RGB (0x00, 0x00, 0xDD));
	SetBkColor (pDC, RGB (0xC0, 0xC0, 0xC0));
	SetBkMode (pDC, OPAQUE);
	ExtTextOut (pDC, pRect.left, pRect.top, ETO_OPAQUE | ETO_CLIPPED, &pRect, pLabel, (UINT)_tcslen(pLabel), NULL);
	RestoreDC (pDC, -1);
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

bool CImageDebugger::CopyBitmap (CBitmapHandle& pTrg, HBITMAP pSrc)
{
	bool			lRet = false;
	tS <BITMAP>		lBitmap;

	pTrg.Close ();

	if	(
			(pSrc)
		&&	(GetObject (pSrc, sizeof (lBitmap), &lBitmap))
		)
	{
		if	(lBitmap.bmBitsPixel >= 24)
		{
			CMemDCHandle		lDC;
			tS <BITMAPINFO>		lDibInfo;
			tArrayPtr <BYTE>	lSrcBits;
			LPBYTE				lTrgBits;

			lDibInfo.bmiHeader.biSize = sizeof (lDibInfo.bmiHeader);
			lDibInfo.bmiHeader.biBitCount = lBitmap.bmBitsPixel;
			lDibInfo.bmiHeader.biPlanes = lBitmap.bmPlanes;
			lDibInfo.bmiHeader.biWidth = lBitmap.bmWidth;
			lDibInfo.bmiHeader.biHeight = lBitmap.bmHeight;
			lDibInfo.bmiHeader.biSizeImage = lBitmap.bmHeight * lBitmap.bmWidthBytes;
			lDibInfo.bmiHeader.biCompression = BI_RGB;

			if	(
					(lSrcBits = new BYTE [lDibInfo.bmiHeader.biSizeImage])
				&&	(lDC.Attach (CreateCompatibleDC (NULL)))
				&&	(GetDIBits (lDC, pSrc, 0, lDibInfo.bmiHeader.biHeight, (LPVOID) (LPBYTE) lSrcBits, &lDibInfo, DIB_RGB_COLORS))
				&&	(pTrg.Attach (CreateDIBSection (lDC, &lDibInfo, DIB_RGB_COLORS, (LPVOID *) &lTrgBits, NULL, NULL)))
				)
			{
				memcpy (lTrgBits, (LPBYTE) lSrcBits, lDibInfo.bmiHeader.biSizeImage);
				GdiFlush ();
				lRet = true;
			}
		}
		else
		if	(pTrg.Attach ((HBITMAP) CopyImage (pSrc, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CImageDebugger::CreateBitmap (HDC pDC, const CSize& pSize, UINT pBitCount, CBitmapHandle& pBitmap)
{
	bool			lRet = false;
	CMemDCHandle	lDC;
	tS <BITMAPINFO>	lBmpInfo;
	LPBYTE 			lBmpBits;

	lBmpInfo.bmiHeader.biSize = sizeof (lBmpInfo.bmiHeader);
	lBmpInfo.bmiHeader.biPlanes = 1;
	lBmpInfo.bmiHeader.biWidth = pSize.cx;
	lBmpInfo.bmiHeader.biHeight = pSize.cy;
	lBmpInfo.bmiHeader.biCompression = BI_RGB;

	if	(pBitCount > 24)
	{
		lBmpInfo.bmiHeader.biBitCount = 32;
		lBmpInfo.bmiHeader.biSizeImage = lBmpInfo.bmiHeader.biWidth * lBmpInfo.bmiHeader.biHeight * 4;
	}
	else
	{
		lBmpInfo.bmiHeader.biBitCount = 24;
		lBmpInfo.bmiHeader.biSizeImage = lBmpInfo.bmiHeader.biWidth * lBmpInfo.bmiHeader.biHeight * 3;
	}

	pBitmap.Close ();

	if	(pBitmap.Attach (CreateDIBSection (pDC, &lBmpInfo, DIB_RGB_COLORS, (void**) &lBmpBits, NULL, NULL)))
	{
		memset (lBmpBits, 0, lBmpInfo.bmiHeader.biSizeImage);
		GdiFlush ();
		lRet = true;
	}

	return lRet;
}

HBRUSH CImageDebugger::CreateHalftoneBrush ()
{
	WORD			lGrayPattern [8];
	CBitmapHandle	lGrayBitmap;
	int				lNdx;

	for (lNdx = 0; lNdx < 8; lNdx++)
	{
		lGrayPattern[lNdx] = (WORD)(0x5555 << (lNdx & 1));
	}
	if	(lGrayBitmap = ::CreateBitmap (8, 8, 1, 1, lGrayPattern))
	{
		return ::CreatePatternBrush (lGrayBitmap);
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

bool CImageDebugger::DumpBitmapInfo (UINT pLogLevel, HBITMAP pBitmap, LPCTSTR pTitle)
{
	tSS <BITMAPV5HEADER, DWORD>		lHeader;
	tS <BITMAP>						lBitmap;
	CMemDCHandle					lDC;

	if	(GetObject (pBitmap, sizeof (lBitmap), &lBitmap))
	{
		LogMessage (pLogLevel, _T("%s"), (pTitle ? pTitle : _T("Bitmap")));
		LogMessage (pLogLevel, _T("  [%d x %d] [%hu] (%hu %d %d)"), lBitmap.bmWidth, lBitmap.bmHeight, lBitmap.bmBitsPixel, lBitmap.bmPlanes, lBitmap.bmWidthBytes, lBitmap.bmType);

		if	(lDC.Attach (CreateCompatibleDC (0)))
		{
			if	(GetDIBits (lDC, pBitmap, 0, 0, NULL, (BITMAPINFO *) &lHeader, DIB_RGB_COLORS))
			{
				LPCTSTR	lVersion;
				if	(lHeader.bV5Size >= sizeof (BITMAPV5HEADER))
				{
					lVersion = _T("V5");
				}
				else
				if	(lHeader.bV5Size >= sizeof (BITMAPV4HEADER))
				{
					lVersion = _T("V4");
				}
				else
				{
					lVersion = _T("V3");
				}

				LogMessage (pLogLevel, _T("  [%d x %d] [%hu] [%s] [%u %u]"), lHeader.bV5Width, lHeader.bV5Height, lHeader.bV5BitCount, lVersion, lHeader.bV5ClrUsed, lHeader.bV5ClrImportant);

				switch (lHeader.bV5Compression)
				{
					case BI_RGB:
					{
						LogMessage (pLogLevel, _T("  BI_RGB"));
					}	break;
					case BI_RLE4:
					{
						LogMessage (pLogLevel, _T("  BI_RLE4"));
					}	break;
					case BI_RLE8:
					{
						LogMessage (pLogLevel, _T("  BI_RLE8"));
					}	break;
					case BI_BITFIELDS:
					{
						if	(lHeader.bV5Size >= sizeof (BITMAPV4HEADER))
						{
							LogMessage (pLogLevel, _T("  BI_BITFIELDS [%8.8X] [%8.8X] [%8.8X] [%8.8X]"), lHeader.bV5AlphaMask, lHeader.bV5RedMask, lHeader.bV5GreenMask, lHeader.bV5BlueMask);
						}
						else
						{
							LogMessage (pLogLevel, _T("  BI_BITFIELDS [%8.8X] [%8.8X] [%8.8X]"), lHeader.bV5RedMask, lHeader.bV5GreenMask, lHeader.bV5BlueMask);
						}
					}	break;
				}
			}
		}
		return true;
	}

	return false;
}

bool CImageDebugger::DumpBitmapInfo (UINT pLogLevel, BITMAPINFO& pBitmap, LPCTSTR pTitle)
{
	LogMessage (pLogLevel, _T("%s"), (pTitle ? pTitle : _T("Bitmap")));

	BITMAPV5HEADER &	lHeader = (BITMAPV5HEADER &) pBitmap.bmiHeader;
	LPCTSTR				lVersion;

	if	(lHeader.bV5Size >= sizeof (BITMAPV5HEADER))
	{
		lVersion = _T("V5");
	}
	else
	if	(lHeader.bV5Size >= sizeof (BITMAPV4HEADER))
	{
		lVersion = _T("V4");
	}
	else
	{
		lVersion = _T("V3");
	}

	LogMessage (pLogLevel, _T("  [%d x %d] [%hu] [%s] [%u %u]"), lHeader.bV5Width, lHeader.bV5Height, lHeader.bV5BitCount, lVersion, lHeader.bV5ClrUsed, lHeader.bV5ClrImportant);

	switch (lHeader.bV5Compression)
	{
		case BI_RGB:
		{
			LogMessage (pLogLevel, _T("  BI_RGB"));
		}	break;
		case BI_RLE4:
		{
			LogMessage (pLogLevel, _T("  BI_RLE4"));
		}	break;
		case BI_RLE8:
		{
			LogMessage (pLogLevel, _T("  BI_RLE8"));
		}	break;
		case BI_BITFIELDS:
		{
			if	(lHeader.bV5Size >= sizeof (BITMAPV4HEADER))
			{
				LogMessage (pLogLevel, _T("  BI_BITFIELDS [%8.8X] [%8.8X] [%8.8X] [%8.8X]"), lHeader.bV5AlphaMask, lHeader.bV5RedMask, lHeader.bV5GreenMask, lHeader.bV5BlueMask);
			}
			else
			{
				LogMessage (pLogLevel, _T("  BI_BITFIELDS [%8.8X] [%8.8X] [%8.8X]"), lHeader.bV5RedMask, lHeader.bV5GreenMask, lHeader.bV5BlueMask);
			}
		}	break;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////

bool CImageDebugger::DumpBitmap (UINT pLogLevel, HBITMAP pBitmap, LPCTSTR pTitle, bool pAlphaOnly, UINT pMaxWidth, UINT pMaxHeight)
{
	tS <BITMAP>	lBitmap;

	try
	{
		if	(
				(LogIsActive (pLogLevel))
			&&	(pBitmap)
			&&	(GetObject (pBitmap, sizeof (lBitmap), &lBitmap))
			)
		{
			if	(lBitmap.bmBits)
			{
				return DumpBitmap (pLogLevel, lBitmap, pTitle, pAlphaOnly, pMaxWidth, pMaxHeight);
			}
			else
			if	(lBitmap.bmWidthBytes)
			{
				tArrayPtr <BYTE>	lBits;

				if	(
						(lBits = new BYTE [lBitmap.bmWidthBytes * lBitmap.bmHeight])
					&&	(GetBitmapBits (pBitmap, lBitmap.bmWidthBytes * lBitmap.bmHeight, (LPVOID) (LPBYTE) lBits))
					)
				{
					lBitmap.bmBits = lBits;
					return DumpBitmap (pLogLevel, lBitmap, pTitle, pAlphaOnly, pMaxWidth, pMaxHeight);
				}
			}
		}
	}
	catch AnyExceptionSilent

	return false;
}

bool CImageDebugger::DumpBitmap (UINT pLogLevel, BITMAP& pBitmap, LPCTSTR pTitle, bool pAlphaOnly, UINT pMaxWidth, UINT pMaxHeight)
{
	bool	lRet = false;

	try
	{
		if	(
				(LogIsActive (pLogLevel))
			&&	(pBitmap.bmBits)
			)
		{
			CPoint	lPixel;
			long	lScanBytes = pBitmap.bmWidthBytes;
			long	lNdx;

			LogMessage (pLogLevel, _T("%s [%d x %d] [%d]"), (pTitle ? pTitle : _T("Bitmap")), pBitmap.bmWidth, pBitmap.bmHeight, pBitmap.bmBitsPixel);

			if	(lScanBytes <= 0)
			{
				lScanBytes = ((pBitmap.bmWidth * (long) pBitmap.bmBitsPixel) + 7L) / 8L;
				lScanBytes = ((lScanBytes + 3L) / 4L) * 4L;
			}

			try
			{
				for	(lPixel.y = pBitmap.bmHeight-1; lPixel.y >= max (pBitmap.bmHeight - (long) pMaxHeight, 0); lPixel.y--)
				{
					CAtlString	lRow;

					for	(lPixel.x = 0; lPixel.x < min (pBitmap.bmWidth, (long) pMaxWidth); lPixel.x++)
					{
						CAtlString	lStr;

/*
						if	(pBitmap.bmBitsPixel > 1)
						{
							lNdx = ((pBitmap.bmHeight - lPixel.y - 1L) * lScanBytes) + ((lPixel.x * (long) pBitmap.bmBitsPixel) / 8L);
						}
						else
*/
						{
							lNdx = (lPixel.y * lScanBytes) + ((lPixel.x * (long) pBitmap.bmBitsPixel) / 8L);
						}

						switch (pBitmap.bmBitsPixel)
						{
							case 1:
							{
								int             lShift = (int) (7L - (lPixel.x % 8L));
								unsigned char   lMask = (unsigned char) 0x01;
								if  (lShift)
								{
									lMask <<= lShift;
								}

								if  (((LPBYTE) pBitmap.bmBits) [lNdx] & lMask)
								{
									lRow += _T("1");
								}
								else
								{
									lRow += _T("0");
								}
							}   break;
							case 4:
							{
								unsigned char   lBit = ((LPBYTE) pBitmap.bmBits) [lNdx];
								if  (lPixel.x % 2L)
								{
									lBit &= 0x0F;
								}
								else
								{
									lBit >>= 4;
								}
								lStr.Format (_T("%1.1X"), lBit);
								lRow += lStr;
							}   break;
							case 8:
							{
								lStr.Format (_T("%2.2X "), ((LPBYTE) pBitmap.bmBits) [lNdx]);
								lRow += lStr;
							}   break;
							case 16:
							{
								WORD *	lPel = (WORD *) (((BYTE*) pBitmap.bmBits) + lNdx);
								RGBQUAD	lColor;

								lColor.rgbBlue = ((*lPel) & 0x001F) << 3;
								lColor.rgbGreen = ((*lPel) & 0x03E0) >> 2;
								lColor.rgbRed = ((*lPel) & 0x7C00) >> 7;

								lStr.Format (_T("%2.2X%2.2X%2.2X "), lColor.rgbRed, lColor.rgbGreen, lColor.rgbBlue);
								lRow += lStr;
							}   break;
							case 24:
							{
								lStr.Format (_T("%2.2X%2.2X%2.2X "), ((LPBYTE) pBitmap.bmBits) [lNdx+2], ((LPBYTE) pBitmap.bmBits) [lNdx+1], ((LPBYTE) pBitmap.bmBits) [lNdx]);
								lRow += lStr;
							}   break;
							case 32:
							{
								if	(pAlphaOnly)
								{
									lStr.Format (_T("%2.2X "), ((LPBYTE) pBitmap.bmBits) [lNdx+3]);
								}
								else
								{
									lStr.Format (_T("%2.2X%2.2X%2.2X%2.2X "), ((LPBYTE) pBitmap.bmBits) [lNdx+3], ((LPBYTE) pBitmap.bmBits) [lNdx+2], ((LPBYTE) pBitmap.bmBits) [lNdx+1], ((LPBYTE) pBitmap.bmBits) [lNdx]);
								}
								lRow += lStr;
							}
							default:
							{}
						}
					}

					lRow.TrimRight ();
					LogMessage (pLogLevel, lRow);
				}

				lRet = true;
			}
			catch AnyExceptionSilent
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

bool CImageDebugger::DumpBitmap (UINT pLogLevel, BITMAPINFO& pBitmap, LPBYTE pBits, LPCTSTR pTitle, bool pAlphaOnly, UINT pMaxWidth, UINT pMaxHeight)
{
	try
	{
		if	(
				(LogIsActive (pLogLevel))
			&&	(pBits)
			)
		{
			tS <BITMAP>	lBitmap;

			lBitmap.bmWidth = pBitmap.bmiHeader.biWidth;
			lBitmap.bmHeight = abs (pBitmap.bmiHeader.biHeight);
			lBitmap.bmBitsPixel = pBitmap.bmiHeader.biBitCount;
			lBitmap.bmPlanes = pBitmap.bmiHeader.biPlanes;
			lBitmap.bmBits = pBits;

			return DumpBitmap (pLogLevel, lBitmap, pTitle, pAlphaOnly, pMaxWidth, pMaxHeight);
		}
	}
	catch AnyExceptionSilent

	return false;
}

////////////////////////////////////////////////////////////////////////

bool CImageDebugger::DumpPalette (UINT pLogLevel, HPALETTE pPalette, LPCTSTR pTitle)
{
	bool	lRet = false;

	try
	{
		if	(
				(LogIsActive (pLogLevel))
			&&	(pPalette)
			)
		{
			CAtlString					lTitle (pTitle);
			UINT						lEntryCount;
			tArrayPtr <PALETTEENTRY>	lEntries;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Palette");
			}
			lEntryCount = GetPaletteEntries (pPalette, 0, 256, NULL);

			LogMessage (pLogLevel, _T("%s [%u]"), lTitle, lEntryCount);

			if	(
					(lEntryCount > 0)
				&&	(lEntries = new PALETTEENTRY [lEntryCount])
				&&	(GetPaletteEntries (pPalette, 0, lEntryCount, lEntries) == lEntryCount)
				)
			{
				UINT		lCount = 0;
				UINT		lNdx;
				CAtlString	lValues;
				CAtlString	lFlags;
				CAtlString	lString;

				for	(lNdx = 0; lNdx <= lEntryCount; lNdx++)
				{
					if	(
							(
								(lNdx == lEntryCount)
							||	((lNdx % 16) == 0)
							)
						&&	(!lValues.IsEmpty ())
						)
					{
						lValues.TrimRight ();
						lFlags.TrimRight ();
						LogMessage (pLogLevel, _T("[%3.3u - %3.3u] [%s] [%s]"), lCount, lNdx-1, lValues, lFlags);

						lCount = lNdx;
						lValues.Empty ();
						lFlags.Empty ();
					}
					if	(lNdx >= lEntryCount)
					{
						break;
					}

					lString.Format (_T("%2.2X%2.2X%2.2X "), lEntries [(int)lNdx].peRed, lEntries [(int)lNdx].peGreen, lEntries [(int)lNdx].peBlue);
					lValues += lString;
					lString.Format (_T("%2.2X "), lEntries [(int)lNdx].peFlags);
					lFlags += lString;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

bool CImageDebugger::SaveBitmap (HBITMAP pBitmap, LPCTSTR pFileName, LPCTSTR pPath, bool pReplace)
{
	bool	lRet = false;
    BITMAP	lBitmap;

	try
	{
		if	(
				(pBitmap)
			&&	(GetObject (pBitmap, sizeof (lBitmap), &lBitmap))
//			&&	(lBitmap.bmBits)
			)
		{
			CAtlString				lFileName (pFileName);
			CAtlString 				lPath (pPath);
			CFileHandle				lFile;
			tS <BITMAPFILEHEADER>	lBmpFileHdr;
			BITMAPINFO *			lBmpInfo;
			tArrayPtr <BYTE>		lBmpInfoBuffer;
			tArrayPtr <BYTE>		lBits;
			long					lInfoSize = sizeof (BITMAPINFOHEADER);
			ULONG					lBitsSize = lBitmap.bmHeight * lBitmap.bmWidthBytes;
			ULONG					lColorCount = (ULONG) 1L << lBitmap.bmBitsPixel;
			CMemDCHandle			lDC;

			if	(lBitmap.bmBitsPixel < 24)
			{
				lInfoSize += lColorCount * sizeof (RGBQUAD);
			}

			if	(
					(lBmpInfoBuffer = new BYTE [lInfoSize])
				&&	(lBmpInfo = (BITMAPINFO *) (LPBYTE) lBmpInfoBuffer)
				&&	(lDC.Attach (CreateIC (_T("DISPLAY"), NULL, NULL, NULL)))
				)
			{
				try
				{
					memset (lBmpInfo, 0, lInfoSize);
					lBmpInfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
					lBmpInfo->bmiHeader.biWidth = (long) lBitmap.bmWidth;
					lBmpInfo->bmiHeader.biHeight = (long) lBitmap.bmHeight;
					lBmpInfo->bmiHeader.biPlanes = 1;
					lBmpInfo->bmiHeader.biBitCount = lBitmap.bmBitsPixel;
					lBmpInfo->bmiHeader.biCompression = BI_RGB;
					lBmpInfo->bmiHeader.biSizeImage = lBitsSize;

					if	(
							(GetDIBits (lDC, pBitmap, 0, lBitmap.bmHeight, NULL, lBmpInfo, DIB_RGB_COLORS))
						&&	(lBitsSize = lBmpInfo->bmiHeader.biSizeImage)
						&&	(lBits = new BYTE [lBitsSize])
						&&	(GetDIBits (lDC, pBitmap, 0, lBitmap.bmHeight, (LPBYTE) lBits, lBmpInfo, DIB_RGB_COLORS))
						)
					{
						lPath.TrimLeft ();
						lPath.TrimRight ();
						if	(lPath.IsEmpty ())
						{
							lPath = GetDefaultPath ();
						}
						else
						if	(PathIsFileSpec (lPath))
						{
							PathCombine (lPath.GetBuffer(MAX_PATH), GetDefaultPath(), pPath);
							lPath.ReleaseBuffer ();
						}

						if	(MakePathExist (lPath))
						{
							lFileName.TrimLeft ();
							lFileName.TrimRight ();

							if	(lFileName.IsEmpty ())
							{
								lFileName = _T("Dump");
								pReplace = false;
							}
							else
							{
								PathStripPath (lFileName.GetBuffer (MAX_PATH));
								PathRemoveExtension (lFileName.GetBuffer (MAX_PATH));
							}
							PathAddExtension (lFileName.GetBuffer (MAX_PATH), _T(".bmp"));
							PathCombine (lFileName.GetBuffer (MAX_PATH), lPath, CAtlString (lFileName));
							lFileName.ReleaseBuffer ();

							lFile = CreateFile (lFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, (pReplace ? CREATE_ALWAYS : CREATE_NEW), FILE_ATTRIBUTE_NORMAL, NULL);

							while	(
										(!pReplace)
									&&	(!lFile.SafeIsValid ())
									&&	(GetLastError () == ERROR_FILE_EXISTS)
									)
							{
								PathStripPath (lFileName.GetBuffer (MAX_PATH));
								PathRemoveExtension (lFileName.GetBuffer (MAX_PATH));
								lFileName.ReleaseBuffer ();
								lFileName = GetNewFileName (lFileName);
								PathAddExtension (lFileName.GetBuffer (MAX_PATH), _T(".bmp"));
								PathCombine (lFileName.GetBuffer (MAX_PATH), lPath, CAtlString (lFileName));
								lFileName.ReleaseBuffer ();

								lFile = CreateFile (lFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, (pReplace ? CREATE_ALWAYS : CREATE_NEW), FILE_ATTRIBUTE_NORMAL, NULL);
							}

							if	(!lFile.SafeIsValid ())
							{
								LogWinErr (LogNormal|LogTime, GetLastError (), _T("CreateFile"));
							}
							else
							{
								try
								{
									lBmpFileHdr.bfType = 0x4D42;
									lBmpFileHdr.bfSize = sizeof (lBmpFileHdr) + lInfoSize + lBitsSize;
									lBmpFileHdr.bfOffBits = sizeof (lBmpFileHdr) + lInfoSize;

									DWORD	lWrite;
									DWORD	lWritten;

									if	(
											(WriteFile (lFile, &lBmpFileHdr, lWrite = sizeof (lBmpFileHdr), &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										&&	(WriteFile (lFile, lBmpInfo, lWrite = lInfoSize, &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										&&	(WriteFile (lFile, (LPBYTE) lBits, lWrite = lBitsSize, &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										)
									{
										lRet = true;
									}
									else
									{
										LogWinErr (LogNormal|LogTime, GetLastError (), _T("WriteFile"));
									}
								}
								catch AnyExceptionSilent

								lFile.Close ();
							}
						}
					}
					else
					{
						LogWinErr (LogNormal|LogTime, GetLastError (), _T("GetDIBits"));
					}
				}
				catch AnyExceptionSilent
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(2)
//////////////////////////////////////////////////////////////////////

typedef struct
{
	BYTE			bWidth;
	BYTE			bHeight;
	BYTE			bColorCount;
	BYTE			bReserved;
	WORD			wPlanes;
	WORD			wBitCount;
	DWORD			dwBytesInRes;
	DWORD			dwImageOffset;
}	ICONDIRENTRY;

typedef struct
{
	WORD			idReserved;
	WORD			idType;
	WORD			idCount;
}	ICONDIR;

typedef struct
{
	BYTE			bWidth;
	BYTE			bHeight;
	BYTE			bColorCount;
	BYTE			bReserved;
	WORD			wPlanes;
	WORD			wBitCount;
	DWORD			dwBytesInRes;
	WORD			nID;
}	GRPICONDIRENTRY;

typedef struct
{
	WORD			idReserved;
	WORD			idType;
	WORD			idCount;
	GRPICONDIRENTRY idEntries[1];
}	GRPICONDIR;

//////////////////////////////////////////////////////////////////////
#pragma pack(pop)
//////////////////////////////////////////////////////////////////////

bool CImageDebugger::SaveIcon (HICON pIcon, LPCTSTR pFileName, LPCTSTR pPath, bool pReplace)
{
	bool	lRet = false;

	try
	{
		ICONINFO		lIconInfo;
		BITMAP			lBitmap;
		CBitmapHandle	lColorBmp;
		CBitmapHandle	lMaskBmp;

		memset (&lIconInfo, 0, sizeof(lIconInfo));

		if	(
				(pIcon)
			&&	(GetIconInfo (pIcon, &lIconInfo))
			&&	(lColorBmp.Attach (lIconInfo.hbmColor))
			&&	(lMaskBmp.Attach (lIconInfo.hbmMask))
			&&	(GetObject ((HBITMAP)lColorBmp, sizeof(lBitmap), &lBitmap))
			)
		{
			CAtlString 				lFileName (pFileName);
			CAtlString 				lPath (pPath);
			CFileHandle				lFile;
			tS <ICONDIR>			lIconDir;
			tS <ICONDIRENTRY>		lIconDirEntry;
			BITMAPINFO *			lColorBmpInfo;
			tArrayPtr <BYTE>		lColorBmpInfoBuffer;
			long					lColorInfoSize = sizeof (BITMAPINFOHEADER);
			tArrayPtr <BYTE>		lColorBits;
			ULONG					lColorBitsSize = lBitmap.bmHeight * lBitmap.bmWidthBytes;
			ULONG					lColorCount = (ULONG) 1L << lBitmap.bmBitsPixel;
			BITMAPINFO *			lMaskBmpInfo;
			tArrayPtr <BYTE>		lMaskBmpInfoBuffer;
			tArrayPtr <BYTE>		lMaskBits;
			ULONG					lMaskBitsSize;
			CMemDCHandle			lDC;

			if	(lBitmap.bmBitsPixel < 24)
			{
				lColorInfoSize += lColorCount * sizeof (RGBQUAD);
			}

			if	(
					(lColorBmpInfoBuffer = new BYTE [lColorInfoSize])
				&&	(lColorBmpInfo = (BITMAPINFO *) (LPBYTE) lColorBmpInfoBuffer)
				&&	(lMaskBmpInfoBuffer = new BYTE [sizeof (BITMAPINFO) + sizeof (RGBQUAD)])
				&&	(lMaskBmpInfo = (BITMAPINFO *) (LPBYTE) lMaskBmpInfoBuffer)
				&&	(lDC.Attach (CreateIC (_T("DISPLAY"), NULL, NULL, NULL)))
				)
			{
				try
				{
					memset (lColorBmpInfo, 0, lColorInfoSize);
					lColorBmpInfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
					lColorBmpInfo->bmiHeader.biWidth = (long) lBitmap.bmWidth;
					lColorBmpInfo->bmiHeader.biHeight = (long) lBitmap.bmHeight;
					lColorBmpInfo->bmiHeader.biPlanes = 1;
					lColorBmpInfo->bmiHeader.biBitCount = lBitmap.bmBitsPixel;
					lColorBmpInfo->bmiHeader.biCompression = BI_RGB;
					lColorBmpInfo->bmiHeader.biSizeImage = lColorBitsSize;

					memset (lMaskBmpInfo, 0, sizeof (BITMAPINFOHEADER));
					lMaskBmpInfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
					lMaskBmpInfo->bmiHeader.biWidth = (long) lBitmap.bmWidth;
					lMaskBmpInfo->bmiHeader.biHeight = (long) lBitmap.bmHeight;
					lMaskBmpInfo->bmiHeader.biPlanes = 1;
					lMaskBmpInfo->bmiHeader.biBitCount = 1;
					lMaskBmpInfo->bmiHeader.biCompression = BI_RGB;
					lMaskBmpInfo->bmiColors [0].rgbRed = 0;
					lMaskBmpInfo->bmiColors [0].rgbGreen = 0;
					lMaskBmpInfo->bmiColors [0].rgbBlue = 0;
					lMaskBmpInfo->bmiColors [1].rgbRed = 255;
					lMaskBmpInfo->bmiColors [1].rgbGreen = 255;
					lMaskBmpInfo->bmiColors [1].rgbBlue = 255;

					if	(
							(GetDIBits (lDC, lColorBmp, 0, lBitmap.bmHeight, NULL, lColorBmpInfo, DIB_RGB_COLORS))
						&&	(lColorBitsSize = lColorBmpInfo->bmiHeader.biSizeImage)
						&&	(lColorBits = new BYTE [lColorBitsSize])
						&&	(GetDIBits (lDC, lColorBmp, 0, lBitmap.bmHeight, (LPBYTE) lColorBits, lColorBmpInfo, DIB_RGB_COLORS))

						&&	(GetDIBits (lDC, lMaskBmp, 0, lBitmap.bmHeight, NULL, lMaskBmpInfo, DIB_RGB_COLORS))
						&&	(lMaskBitsSize = lMaskBmpInfo->bmiHeader.biSizeImage)
						&&	(lMaskBits = new BYTE [lMaskBitsSize])
						&&	(GetDIBits (lDC, lMaskBmp, 0, lBitmap.bmHeight, (LPBYTE) lMaskBits, lMaskBmpInfo, DIB_RGB_COLORS))
						)
					{
						lPath.TrimLeft ();
						lPath.TrimRight ();
						if	(lPath.IsEmpty ())
						{
							lPath = GetDefaultPath ();
						}
						else
						if	(PathIsFileSpec (lPath))
						{
							PathCombine (lPath.GetBuffer(MAX_PATH), GetDefaultPath(), pPath);
							lPath.ReleaseBuffer ();
						}

						if	(MakePathExist (lPath))
						{
							lFileName.TrimLeft ();
							lFileName.TrimRight ();

							if	(lFileName.IsEmpty ())
							{
								lFileName = _T("Dump");
								pReplace = false;
							}
							else
							{
								PathStripPath (lFileName.GetBuffer (MAX_PATH));
								PathRemoveExtension (lFileName.GetBuffer (MAX_PATH));
							}
							PathAddExtension (lFileName.GetBuffer (MAX_PATH), _T(".ico"));
							PathCombine (lFileName.GetBuffer (MAX_PATH), lPath, CAtlString (lFileName));
							lFileName.ReleaseBuffer ();

							lFile = CreateFile (lFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, (pReplace ? CREATE_ALWAYS : CREATE_NEW), FILE_ATTRIBUTE_NORMAL, NULL);

							while	(
										(!pReplace)
									&&	(!lFile.SafeIsValid ())
									&&	(GetLastError () == ERROR_FILE_EXISTS)
									)
							{
								PathStripPath (lFileName.GetBuffer (MAX_PATH));
								PathRemoveExtension (lFileName.GetBuffer (MAX_PATH));
								lFileName.ReleaseBuffer ();
								lFileName = GetNewFileName (lFileName);
								PathAddExtension (lFileName.GetBuffer (MAX_PATH), _T(".ico"));
								PathCombine (lFileName.GetBuffer (MAX_PATH), lPath, CAtlString (lFileName));
								lFileName.ReleaseBuffer ();

								lFile = CreateFile (lFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, (pReplace ? CREATE_ALWAYS : CREATE_NEW), FILE_ATTRIBUTE_NORMAL, NULL);
							}

							if	(!lFile.SafeIsValid ())
							{
								LogWinErr (LogNormal|LogTime, GetLastError (), _T("CreateFile"));
							}
							else
							{
								try
								{
									lIconDir.idCount = 1;
									lIconDir.idType = RES_ICON;

									lIconDirEntry.bWidth = (BYTE) min (lBitmap.bmWidth, 255);
									lIconDirEntry.bHeight = (BYTE) min (lBitmap.bmHeight, 255);
									lIconDirEntry.bColorCount = (BYTE) lColorCount;
									lIconDirEntry.wPlanes = lBitmap.bmPlanes;
									lIconDirEntry.wBitCount = lBitmap.bmBitsPixel;
									lIconDirEntry.dwBytesInRes = lColorInfoSize + lColorBitsSize + lMaskBitsSize;
									lIconDirEntry.dwImageOffset = sizeof (lIconDir) + sizeof (lIconDirEntry);

									lColorBmpInfo->bmiHeader.biHeight *= 2;
									lColorBmpInfo->bmiHeader.biSizeImage += lMaskBmpInfo->bmiHeader.biSizeImage;

									DWORD	lWrite;
									DWORD	lWritten;

									if	(
											(WriteFile (lFile, &lIconDir, lWrite = sizeof (lIconDir), &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										&&	(WriteFile (lFile, &lIconDirEntry, lWrite = sizeof (lIconDirEntry), &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										&&	(WriteFile (lFile, lColorBmpInfo, lWrite = lColorInfoSize, &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										&&	(WriteFile (lFile, (LPBYTE) lColorBits, lWrite = lColorBitsSize, &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										&&	(WriteFile (lFile, (LPBYTE) lMaskBits, lWrite = lMaskBitsSize, &(lWritten = 0), NULL))
										&&	(lWritten == lWrite)
										)
									{
										lRet = true;
									}
									else
									{
										LogWinErr (LogNormal|LogTime, GetLastError (), _T("WriteFile"));
									}
								}
								catch AnyExceptionSilent

								lFile.Close ();
							}
						}
					}
					else
					{
						LogWinErr (LogNormal|LogTime, GetLastError (), _T("GetDIBits"));
					}
				}
				catch AnyExceptionSilent
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

UINT	CImageDebugger::mLastSeriesNum = 0;
bool	CImageDebugger::mSeriesEnabled = false;

static const CSize	sMaxSeriesSize (1024, 768);

////////////////////////////////////////////////////////////////////////

void CImageDebugger::EnableSeries (bool pEnable)
{
	mSeriesEnabled = pEnable;
}

bool CImageDebugger::SeriesEnabled ()
{
	return mSeriesEnabled;
}

bool CImageDebugger::ClearAllSeries ()
{
	bool				lRet = false;
	CAtlString			lPath;
	tS <SHFILEOPSTRUCT>	lFileOp;

	mLastSeriesNum = 0;

	try
	{
		lPath = GetDefaultPath ();
		PathAppend (lPath.GetBuffer (MAX_PATH), _T("*.*"));
		lPath.ReleaseBuffer ();
		lPath.GetBufferSetLength (lPath.GetLength ()+1);

		lFileOp.wFunc = FO_DELETE;
		lFileOp.pFrom = lPath;
		lFileOp.fFlags = FOF_ALLOWUNDO | FOF_NORECURSION | FOF_FILESONLY | FOF_SILENT | FOF_NOCONFIRMATION;

		if	(SHFileOperation (&lFileOp) == 0)
		{
#ifdef	_DEBUG_SERIES
			LogMessage (_DEBUG_SERIES, _T("Cleared [%s]"), (LPCTSTR) lPath);
#endif
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

UINT CImageDebugger::GetNextSeriesNum ()
{
	return mLastSeriesNum+1;
}

void CImageDebugger::SetNextSeriesNum (UINT pNextSeriesNum)
{
	mLastSeriesNum = max (pNextSeriesNum, 1) - 1;
}

UINT CImageDebugger::StartSeries (LPCTSTR pFileName)
{
	EndSeries (false);

	if	(mSeriesEnabled)
	{
		try
		{
			mSeriesName = pFileName;
			PathUnquoteSpaces (mSeriesName.GetBuffer (MAX_PATH));
			PathStripPath (mSeriesName.GetBuffer (MAX_PATH));
			PathRemoveExtension (mSeriesName.GetBuffer (MAX_PATH));
			mSeriesName.ReleaseBuffer ();
			mSeriesName.TrimLeft ();
			mSeriesName.TrimRight ();

			mSeries.RemoveAll ();
			mSeriesNum = ++mLastSeriesNum;

			if	(mSeriesName.IsEmpty ())
			{
				mSeriesName.Format (_T("%3.3u Series"), mSeriesNum);
			}
			else
			{
				mSeriesName.Format (_T("%3.3u %s"), mSeriesNum, CAtlString (mSeriesName));
			}

#ifdef	_DEBUG_SERIES
			LogMessage (_DEBUG_SERIES, _T("Start series [%s]"), (LPCTSTR) mSeriesName);
#endif

			return (mSeriesNum);
		}
		catch AnyExceptionSilent
	}
	return 0;
}

UINT CImageDebugger::EndSeries (bool pSave)
{
	UINT	lRet = 0;

	try
	{
		if	(
				(pSave)
			&&	(mSeries.GetCount() > 0)
			)
		{
			INT_PTR	lStartNdx = 0;
			INT_PTR	lEndNdx;
			CSize	lSeriesSize (0, 0);
			CSize	lRowSize (0, 0);
			UINT	lMaxBitCount = 0;

#ifdef	_DEBUG_SERIES
			LogMessage (_DEBUG_SERIES, _T("Save series [%s] [%u]"), (LPCTSTR) mSeriesName, mSeries.GetCount());
#endif

			for	(lEndNdx = 0; lEndNdx < (INT_PTR)mSeries.GetCount(); lEndNdx++)
			{
				tS <BITMAP>	lBitmap;

				if	(
						(mSeries.GetAt (lEndNdx).GetSafeHandle ())
					&&	(GetObject (mSeries.GetAt (lEndNdx), sizeof (lBitmap), &lBitmap))
					)
				{
					if	(
							(lRowSize.cx > 0)
						&&	(lRowSize.cx + lBitmap.bmWidth >= sMaxSeriesSize.cx)
						)
					{
						lSeriesSize.cx = max (lSeriesSize.cx, lRowSize.cx);
						if	(lSeriesSize.cy)
						{
							lSeriesSize.cy += 4;
						}
						lSeriesSize.cy += lRowSize.cy;
						lRowSize.cx = 0;
						lRowSize.cy = 0;

						if	((lSeriesSize.cy + lBitmap.bmHeight) >= sMaxSeriesSize.cy)
						{
							if	(lStartNdx == 0)
							{
								mSeriesName += _T(" [1]");
							}
							lRet += SaveSeries (lSeriesSize, lMaxBitCount, lStartNdx, lEndNdx-1);
							lSeriesSize.cx = 0;
							lSeriesSize.cy = 0;
							lMaxBitCount = 0;
							lStartNdx = lEndNdx;
						}

						lEndNdx--;
						continue;
					}
					else
					{
						lSeriesSize.cx = max (lSeriesSize.cx, lRowSize.cx);
					}

					if	(lRowSize.cx)
					{
						lRowSize.cx += 4;
					}
					lRowSize.cx += lBitmap.bmWidth;
					lRowSize.cy = max (lRowSize.cy, lBitmap.bmHeight);
					lMaxBitCount = max (lMaxBitCount, (UINT) lBitmap.bmBitsPixel);

#ifdef	_DEBUG_SERIES
					LogMessage (_DEBUG_SERIES, _T("  Series [%s %d] [%d x %d] [%u] at [%d %d] in [%d %d]"), (LPCTSTR) mSeriesName, lEndNdx, lBitmap.bmWidth, lBitmap.bmHeight, lBitmap.bmBitsPixel, lRowSize.cx-lBitmap.bmWidth, lSeriesSize.cy, lSeriesSize, lSeriesSize.cy+lRowSize.cy);
#endif
				}
			}

			if	(
					(lRowSize.cx > 0)
				&&	(lRowSize.cy > 0)
				)
			{
				lSeriesSize.cx = max (lSeriesSize.cx, lRowSize.cx);
				if	(lSeriesSize.cy)
				{
					lSeriesSize.cy += 4;
				}
				lSeriesSize.cy += lRowSize.cy;
			}
			lEndNdx--;

			if	(
					(lSeriesSize.cx > 0)
				&&	(lSeriesSize.cy > 0)
				)
			{
				lRet += SaveSeries (lSeriesSize, lMaxBitCount, lStartNdx, lEndNdx);
			}
		}
#ifdef	_DEBUG_SERIES
		else
		if	(
				(pSave)
			&&	(mSeriesNum)
			)
		{
			LogMessage (_DEBUG_SERIES, _T("End series [%s]"), (LPCTSTR) mSeriesName);
		}
		else
		if	(mSeries.GetSize () > 0)
		{
			LogMessage (_DEBUG_SERIES, _T("Discard series [%s] [%u]"), (LPCTSTR) mSeriesName, mSeries.GetSize ());
		}
#endif
	}
	catch AnyExceptionSilent

	try
	{
		mSeriesNum = 0;
		mSeriesName.Empty ();
		mSeries.RemoveAll ();
	}
	catch AnyExceptionSilent

	return lRet;
}

UINT CImageDebugger::UpdateSeries (HBITMAP pBitmap, bool pTakeBitmap)
{
	UINT			lRet = 0;
	tS <BITMAP>		lBitmap;
	CBitmapHandle	lCopy;

	try
	{
		if	(
				(mSeriesNum)
			&&	(pBitmap)
			&&	(GetObject (pBitmap, sizeof (lBitmap), &lBitmap))
			&&	(
					(pTakeBitmap)
				?	(lCopy.Attach (pBitmap))
				:	(CopyBitmap (lCopy, pBitmap))
				)
			)
		{
			lRet = (UINT)mSeries.Add (lCopy.Detach ());
#ifdef	_DEBUG_SERIES
			LogMessage (_DEBUG_SERIES, _T("  Series [%s %d] [%d x %d] [%u]"), (LPCTSTR) mSeriesName, lRet, lBitmap.bmWidth, lBitmap.bmHeight, lBitmap.bmBitsPixel);
#endif
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

UINT CImageDebugger::SaveSeries (const CSize& pSize, UINT pBitCount, INT_PTR pMinNdx, INT_PTR pMaxNdx)
{
	UINT			lRet = 0;
	CBitmapHandle	lSeries;
	CMemDCHandle	lSrcDC;
	CMemDCHandle	lTrgDC;
	HGDIOBJ			lSrcOldBmp;
	HGDIOBJ			lTrgOldBmp;

#ifdef	_DEBUG_SERIES
	LogMessage (_DEBUG_SERIES, _T("Series [%s] Save [%d - %d] [%d x %d] [%u]"), (LPCTSTR) mSeriesName, pMinNdx, pMaxNdx, pSize.cx, pSize.cy, pBitCount);
#endif

	if	(
			(lSrcDC.Attach (CreateCompatibleDC (0)))
		&&	(lTrgDC.Attach (CreateCompatibleDC (0)))
		&&	(CreateBitmap (lTrgDC, pSize, pBitCount, lSeries))
		)
	{
		INT_PTR			lNdx;
		CPoint			lTrgPos (0, 0);
		CSize			lRowSize (0, 0);
		CBrushHandle	lBrush = CreateHalftoneBrush ();

		lTrgOldBmp = SelectObject (lTrgDC, lSeries);

		SetBkColor (lTrgDC, RGB (0xFF, 0x80, 0X80));
		SetTextColor (lTrgDC, RGB (0xFF, 0xC0, 0xC0));
		FillRect (lTrgDC, &CRect (CPoint (0, 0), pSize), lBrush);
		SetBkColor (lTrgDC, RGB (0x00, 0x00, 0x00));
		SetTextColor (lTrgDC, RGB (0xFF, 0xFF, 0xFF));

		try
		{
			for	(lNdx = pMinNdx; lNdx <= pMaxNdx; lNdx++)
			{
				tS <BITMAP>		lBitmap;
				CSize			lBitmapSize;
				BLENDFUNCTION	lBlend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

				if	(
						(mSeries.GetAt (lNdx).GetSafeHandle ())
					&&	(GetObject (mSeries.GetAt (lNdx), sizeof (lBitmap), &lBitmap))
					)
				{
					if	(
							(lRowSize.cx > 0)
						&&	(lRowSize.cx + lBitmap.bmWidth > pSize.cx)
						)
					{
						lTrgPos.y += lRowSize.cy + 4;
						lRowSize.cx = 0;
						lRowSize.cy = 0;
					}
					lTrgPos.x = lRowSize.cx;

#ifdef	_DEBUG_SERIES
					LogMessage (_DEBUG_SERIES, _T("  Series [%s %d] [%d x %d] [%u] at [%d %d]"), (LPCTSTR) mSeriesName, lNdx, lBitmap.bmWidth, lBitmap.bmHeight, lBitmap.bmBitsPixel, lTrgPos.x, lTrgPos.y);
#endif

					lSrcOldBmp = SelectObject (lSrcDC, mSeries.GetAt (lNdx).GetSafeHandle ());
					lBitmapSize = CSize (lBitmap.bmWidth, lBitmap.bmHeight);
					if	(
							(pBitCount <= 24)
						||	(!AlphaBlend (lTrgDC, lTrgPos.x, lTrgPos.y, lBitmapSize.cx, lBitmapSize.cy, lSrcDC, 0, 0, lBitmapSize.cx, lBitmapSize.cy, lBlend))
						)
					{
						BitBlt (lTrgDC, lTrgPos.x, lTrgPos.y, lBitmapSize.cx, lBitmapSize.cy, lSrcDC, 0, 0, SRCCOPY);
					}
					SelectObject (lSrcDC, lSrcOldBmp);

					lRowSize.cx += lBitmap.bmWidth + 4;
					lRowSize.cy = max (lRowSize.cy, lBitmap.bmHeight);
				}
			}
		}
		catch AnyExceptionSilent

		SelectObject (lTrgDC, lTrgOldBmp);

		SaveBitmap (lSeries, mSeriesName, NULL, false);
	}

	lSrcDC.Close ();
	lTrgDC.Close ();

	return lRet;
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

CAtlString CImageDebugger::GetDefaultPath ()
{
	CAtlString	lRet (gLogFileName);
	PathRemoveExtension (lRet.GetBuffer (MAX_PATH));
	lRet.ReleaseBuffer ();
	lRet.TrimRight ();
	return CAtlString (lRet);
}

bool CImageDebugger::MakePathExist (LPCTSTR pPath)
{
	SetLastError (0);

	if	(
			(PathIsRoot (pPath))
		||	(CreateDirectory (pPath, NULL))
		||	(GetLastError () == ERROR_ALREADY_EXISTS)
		)
	{
		return true;
	}
	else
	if	(GetLastError () == ERROR_PATH_NOT_FOUND)
	{
		CAtlString	lParent (pPath);

		PathRemoveFileSpec (lParent.GetBuffer (lParent.GetLength ()));
		lParent.ReleaseBuffer ();

		if	(
				(MakePathExist (lParent))
			&&	(CreateDirectory (pPath, NULL))
			)
		{
			return true;
		}
	}

	LogWinErr (LogNormal|LogTime, GetLastError (), _T("CreateDirectory"));
	return false;
}

CAtlString CImageDebugger::GetNewFileName (const CAtlString pFileName)
{
	CAtlString	lRet (pFileName);
	CAtlString	lName (pFileName);
	int			lNdx;
	int			lNumStart = INT_MAX;
	int			lNumEnd = -1;
	int			lSequence = 1;

	lName.TrimRight ();

	for (lNdx = lName.GetLength ()-1; lNdx >= 0; lNdx--)
	{
		if	(_istdigit (lName [lNdx]))
		{
			lNumStart = min (lNumStart, lNdx);
			if	(lNumEnd < 0)
			{
				lNumEnd = lNdx;
			}
			continue;
		}
		else
		if	(
				(lName [lNdx] == _T(']'))
			&&	(lNdx == lName.GetLength ()-1)
			)
		{
			continue;
		}
		else
		if	(
				(lName [lNdx] == _T('['))
			&&	(lNdx < lName.GetLength ()-2)
			&&	(lName [lName.GetLength ()-1] == _T(']'))
			)
		{
			lName.Delete (lName.GetLength ()-1);
			lName.Delete (lNdx);
			lNumStart--;
			lNumEnd--;
			break;
		}
		else
		{
			break;
		}
	}

	if	(lNumStart <= lNumEnd)
	{
		lSequence = _ttoi (lName.Mid (lNumStart, lNumEnd-lNumStart+1)) + 1;
		lName = lName.Left (lNumStart);
		lName.TrimRight ();
	}

	lRet.Format (_T("%s [%d]"), (LPCTSTR) lName, lSequence);

	return lRet;
}
