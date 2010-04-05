/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include <AfxHtml.h>
#include "DaCore.h"
#include "PropPageCopyright.h"
#include "Localize.h"
#include "FileVersionEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropPageCopyright, CPropertyPage)

BEGIN_MESSAGE_MAP(CPropPageCopyright, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageCopyright)
	ON_NOTIFY(NM_CLICK,IDC_PROPPAGE_CPR_LICENSELINK,OnLinkClick)
	ON_NOTIFY(NM_CLICK,IDC_PROPPAGE_CPR_REFLINK,OnLinkClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropPageCopyright::CPropPageCopyright()
:	CPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCopyright::CPropPageCopyright"), this);
#endif
	//{{AFX_DATA_INIT(CPropPageCopyright)
	//}}AFX_DATA_INIT

	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD))
	{
		m_psp.pszTitle = (LPCTSTR) (m_strCaption = mPropPageFix.GetTemplateCaption (m_psp.pResource));
	}
	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_CAN)))
	{
		m_psp.dwFlags |= PSP_DLGINDIRECT;
//
//	Doesn't work for Arabic and Hebrew, and the page content is in English anyway
//
//		if	(m_psp.pszTitle)
//		{
//			m_psp.pResource = mPropPageFix.SetTemplateCaption (m_psp.pResource, m_psp.pszTitle);
//		}
	}
}

CPropPageCopyright::~CPropPageCopyright()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCopyright::~CPropPageCopyright"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageCopyright::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageCopyright)
	DDX_Control(pDX, IDC_PROPPAGE_CPR_VERSION_TITLE, mVersionTitle);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_VERSION, mProductVersion);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_NAME, mProductName);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_ICON, mIcon);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_COPYRIGHT, mCopyright);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_VERSION_TITLE_MA, mMaVersionTitle);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_VERSION_MA, mMaProductVersion);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_NAME_MA, mMaProductName);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_ICON_MA, mMaIcon);
	DDX_Control(pDX, IDC_PROPPAGE_CPR_COPYRIGHT_MA, mMaCopyright);
	//}}AFX_DATA_MAP

	if	(!pDX->m_bSaveAndValidate)
	{
		InitFonts ();
		ShowDaVersion ();
		ShowMaVersion ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageCopyright::InitFonts ()
{
	CFont *			lFont;
	tS <LOGFONT>	lLogFont;

	if	(
			(!mLargeFont.GetSafeHandle ())
		&&	(lFont = GetFont())
		&&	(lFont->GetLogFont (&lLogFont))
		)
	{
		lLogFont.lfWidth = 0;
		lLogFont.lfHeight = MulDiv (lLogFont.lfHeight, 3, 2);
		mLargeFont.CreateFontIndirect (&lLogFont);
	}
	if	(mLargeFont.GetSafeHandle ())
	{
		mProductName.SetFont (&mLargeFont);
		mMaProductName.SetFont (&mLargeFont);
	}
}

void CPropPageCopyright::ShowDaVersion ()
{
	CSize	lIconSize;
	HICON	lIcon;

	AlignTop (&mIcon, &mProductName);
	lIconSize.cy = ChildWndRect (mProductVersion).bottom - ChildWndRect (mProductName).top;
	lIconSize.cx = lIconSize.cy;
	if	(lIcon = (HICON)LoadImage (AfxGetResourceHandle(), MAKEINTRESOURCE(IDD_PROPPAGE_COPYRIGHT), IMAGE_ICON, lIconSize.cx, lIconSize.cy, LR_DEFAULTCOLOR))
	{
		mIcon.SetIcon (lIcon);
	}
	mIcon.ModifyStyle (0, SS_REALSIZECONTROL);
	UpdateSize (&mIcon, lIconSize);

	mProductVersion.SetWindowText (_T(_DOUBLEAGENT_VERSION_STR));
	mCopyright.SetWindowText (_T(_DOUBLEAGENT_COPYRIGHT));
}

void CPropPageCopyright::ShowMaVersion ()
{
	CString			lMaServerPath;
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
		CFileVersionEx::CVersionString *	lCopyright;

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

void CPropPageCopyright::OnLinkClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	PNMLINK	lNotify = (PNMLINK)pNMHDR;

	if	(lNotify->item.szID[0])
	{
		try
		{
			CLicenseDlg	lLicenseDlg (this);

			lLicenseDlg.mLicenseURL = lNotify->item.szID;
			lLicenseDlg.DoModal ();
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
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CPropPageCopyright::CLicenseDlg::CLicenseDlg (CWnd * pParentWnd)
:	CDialog (IDD_LICENSE, pParentWnd)
{
}

BOOL CPropPageCopyright::CLicenseDlg::OnInitDialog ()
{
	BOOL						lRet;
	CWnd *						lTextWnd;
	CRect						lTextRect;
	CRect						lWinRect;
	HMONITOR					lMonitor;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	lRet = CDialog::OnInitDialog ();

	CenterWindow ();
	GetWindowRect (&lWinRect);
	if	(
			(
				(lMonitor = MonitorFromPoint (lWinRect.TopLeft(), MONITOR_DEFAULTTONEAREST))
			||	(lMonitor = MonitorFromPoint (lWinRect.TopLeft(), MONITOR_DEFAULTTOPRIMARY))
			)
		&&	(GetMonitorInfo (lMonitor, &lMonitorInfo))
		)
	{
		if	(lWinRect.right > lMonitorInfo.rcWork.right)
		{
			lWinRect.OffsetRect (lMonitorInfo.rcWork.right - lWinRect.right, 0);
		}
		if	(lWinRect.left < lMonitorInfo.rcWork.left)
		{
			lWinRect.OffsetRect (lMonitorInfo.rcWork.left - lWinRect.left, 0);
		}
		if	(lWinRect.bottom > lMonitorInfo.rcWork.bottom)
		{
			lWinRect.OffsetRect (0, lMonitorInfo.rcWork.bottom - lWinRect.bottom);
		}
		if	(lWinRect.top < lMonitorInfo.rcWork.top)
		{
			lWinRect.OffsetRect (0, lMonitorInfo.rcWork.top - lWinRect.top);
		}
	}
	MoveWindow (&lWinRect);

	lTextWnd = GetDlgItem (IDC_LICENSE_TEXT);
	lTextWnd->GetWindowRect (&lTextRect);
	ScreenToClient (&lTextRect);
	lTextWnd->DestroyWindow ();

	mLicenseText = (CHtmlView *) CHtmlView::CreateObject ();
	if	(mLicenseText->Create (NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_TABSTOP, lTextRect, this, IDC_LICENSE_TEXT))
	{
		mLicenseText->ModifyStyleEx (0, WS_EX_STATICEDGE, SWP_FRAMECHANGED);
		mLicenseText->LoadFromResource (mLicenseURL);
	}

	return lRet;
}
