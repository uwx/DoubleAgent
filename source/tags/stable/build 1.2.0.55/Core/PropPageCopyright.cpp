/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <shellapi.h>
#include "DaCore.h"
#include "PropPageCopyright.h"
#include "Localize.h"
#include "FileVersionEx.h"
#include "ImageBuffer.h"

#pragma comment (lib,"comctl32.lib")

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CPropPageCopyright)

CPropPageCopyright::CPropPageCopyright()
:	CAtlPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCopyright::CPropPageCopyright"), this);
#endif
	tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
	lInitControls.dwICC = ICC_WIN95_CLASSES|ICC_LINK_CLASS|ICC_INTERNET_CLASSES;
	InitCommonControlsEx (&lInitControls);
}

CPropPageCopyright::~CPropPageCopyright()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCopyright::~CPropPageCopyright"), this);
#endif
}

CPropPageCopyright * CPropPageCopyright::CreateInstance ()
{
	return new CPropPageCopyright;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageCopyright::OnInitDialog ()
{
	mVersionTitle.Attach		(GetDlgItem (IDC_PROPPAGE_CPR_VERSION_TITLE));
	mProductVersion.Attach		(GetDlgItem (IDC_PROPPAGE_CPR_VERSION));
	mProductName.Attach			(GetDlgItem (IDC_PROPPAGE_CPR_NAME));
	mIconControl.Attach			(GetDlgItem (IDC_PROPPAGE_CPR_ICON));
	mCopyright.Attach			(GetDlgItem (IDC_PROPPAGE_CPR_COPYRIGHT));
	mMaVersionTitle.Attach		(GetDlgItem (IDC_PROPPAGE_CPR_VERSION_TITLE_MA));
	mMaProductVersion.Attach	(GetDlgItem (IDC_PROPPAGE_CPR_VERSION_MA));
	mMaProductName.Attach		(GetDlgItem (IDC_PROPPAGE_CPR_NAME_MA));
	mMaIcon.Attach				(GetDlgItem (IDC_PROPPAGE_CPR_ICON_MA));
	mMaCopyright.Attach			(GetDlgItem (IDC_PROPPAGE_CPR_COPYRIGHT_MA));

	InitFonts ();
	ShowDaVersion ();
	ShowMaVersion ();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageCopyright::InitFonts ()
{
	HFONT			lFont;
	tS <LOGFONT>	lLogFont;

	if	(
			(!mLargeFont.GetSafeHandle ())
		&&	(lFont = GetFont())
		&&	(::GetObject (lFont, sizeof(lLogFont), &lLogFont))
		)
	{
		lLogFont.lfWidth = 0;
		lLogFont.lfHeight = MulDiv (lLogFont.lfHeight, 3, 2);
		mLargeFont = CreateFontIndirect (&lLogFont);
	}
	if	(mLargeFont.GetSafeHandle ())
	{
		mProductName.SetFont (mLargeFont);
		mMaProductName.SetFont (mLargeFont);
	}
}

void CPropPageCopyright::ShowDaVersion ()
{
	CSize		lIconSize;
	CAtlString	lVersionStr (_DOUBLEAGENT_VERSION_STR);

	AlignTop (mIconControl, mProductName);
	lIconSize.cy = ChildWndRect (mProductVersion).bottom - ChildWndRect (mProductName).top;
	lIconSize.cx = lIconSize.cy;
	if	(mIcon = (HICON)LoadImage (_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDD_PROPPAGE_COPYRIGHT), IMAGE_ICON, lIconSize.cx, lIconSize.cy, LR_DEFAULTCOLOR))
	{
		mIconControl.ModifyStyle (SS_TYPEMASK, SS_OWNERDRAW);
		UpdateSize (mIconControl, lIconSize);
	}

	mProductVersion.SetWindowText (lVersionStr);
	mCopyright.SetWindowText (_T(_DOUBLEAGENT_COPYRIGHT));
}

void CPropPageCopyright::ShowMaVersion ()
{
	CAtlString		lMaServerPath;
	CModuleHandle	lMaServerModule;
	UINT			lErrMode;

	mMaProductName.SetWindowText (CLocalize::LoadString (IDS_MS_AGENT));

	GetWindowsDirectory (lMaServerPath.GetBuffer (MAX_PATH), MAX_PATH);
	PathAppend (lMaServerPath.GetBuffer (MAX_PATH), _T("MsAgent"));
	PathAppend (lMaServerPath.GetBuffer (MAX_PATH), _T("AgentSvr.exe"));
	lMaServerPath.ReleaseBuffer ();

	lErrMode = SetErrorMode (SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX);
	try
	{
		lMaServerModule = ::LoadLibraryEx (lMaServerPath, NULL, LOAD_LIBRARY_AS_DATAFILE);
	}
	catch AnyExceptionSilent
	SetErrorMode (lErrMode);

	if	(lMaServerModule.SafeIsValid())
	{
		HICON								lIcon;
		CFileVersionEx						lVersion (lMaServerPath);
		CFileVersionEx::CVersionString*	lCopyright;

		if	(lIcon = (HICON)LoadImage (lMaServerModule, MAKEINTRESOURCE(113), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR|LR_DEFAULTSIZE))
		{
			mMaIcon.SetIcon (lIcon);
		}

		mMaProductVersion.SetWindowText (lVersion.ProductVersionString ());

		if	(lCopyright = lVersion.mStrings.Find (lVersion.mCopyrightName))
		{
			mMaCopyright.SetWindowText (lCopyright->mValue);
		}
	}
	else
	{
		mMaVersionTitle.ShowWindow (SW_HIDE);
		mMaProductVersion.ShowWindow (SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageCopyright::OnDrawIcon (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPDRAWITEMSTRUCT	lDIS = (LPDRAWITEMSTRUCT) lParam;

	if	(
			(lDIS->hwndItem == mIconControl.m_hWnd)
		&&	(mIcon.GetSafeHandle ())
		)
	{
		CRect			lDrawRect (lDIS->rcItem);
		CImageBuffer	lImageBuffer;

		if	(lImageBuffer.CreateBuffer (lDrawRect.Size ()))
		{
			FillSolidRect (lImageBuffer.GetDC (), lDrawRect, GetSysColor(COLOR_WINDOW));
			DrawIconEx (lImageBuffer.GetDC (), 0, 0, mIcon, lDrawRect.Width(), lDrawRect.Height(), 0, NULL, DI_NORMAL);
			BitBlt (lDIS->hDC, lDrawRect.left, lDrawRect.top, lDrawRect.Width(), lDrawRect.Height(), lImageBuffer.GetDC(), 0, 0, SRCCOPY);
			lImageBuffer.EndBuffer ();
		}
		else
		{
			DrawIconEx (lDIS->hDC, lDrawRect.left, lDrawRect.top, mIcon, lDrawRect.Width(), lDrawRect.Height(), 0, GetStockBrush(COLOR_WINDOW), DI_NORMAL);
		}
		return TRUE;
	}
	return 0;
}

LRESULT CPropPageCopyright::OnLinkClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	PNMLINK	lNotify = (PNMLINK)pnmh;

	if	(lNotify->item.szID[0])
	{
		try
		{
			CLicenseDlg	lLicenseDlg;
			lLicenseDlg.mLicenseURL = lNotify->item.szID;
			lLicenseDlg.DoModal (m_hWnd);
		}
		catch AnyExceptionSilent
	}
	else
	if	(lNotify->item.szUrl[0])
	{
		try
		{
			if	((int)ShellExecute (NULL, _T("open"), lNotify->item.szUrl, NULL, NULL, SW_SHOWNORMAL) <= 32)
			{
				MessageBeep (MB_ICONERROR);
			}
		}
		catch AnyExceptionSilent
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageCopyright::CLicenseDlg::OnInitDialog (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND		lTextWnd;
	CRect		lTextRect;
	CAtlString	lLicenseURL;

	CenterWindow ();

	lTextWnd = GetDlgItem (IDC_LICENSE_TEXT);
	::GetWindowRect (lTextWnd, &lTextRect);
	ScreenToClient (&lTextRect);
	::DestroyWindow (lTextWnd);

	if	(mLicenseText.Create (m_hWnd, lTextRect, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL, WS_EX_STATICEDGE, IDC_LICENSE_TEXT))
	{
		GetModuleFileName (_AtlBaseModule.GetModuleInstance(), lLicenseURL.GetBuffer(MAX_PATH), MAX_PATH);
		lLicenseURL.ReleaseBuffer ();
		lLicenseURL.Format (_T("res://%s/%s"), CAtlString((LPCTSTR)lLicenseURL), mLicenseURL);
		mLicenseText.CreateControl (lLicenseURL);
	}
	return TRUE;
}

LRESULT CPropPageCopyright::CLicenseDlg::OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog (IDCANCEL);
	return 0;
}

LRESULT CPropPageCopyright::CLicenseDlg::OnOk (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog (IDOK);
	return 0;
}
