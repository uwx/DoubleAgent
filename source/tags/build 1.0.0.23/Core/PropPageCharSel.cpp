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
#include "DaCore.h"
#include "PropPageCharSel.h"
#include "DaGlobalConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropPageCharSel, CPropertyPage)

CPropPageCharSel::CPropPageCharSel()
:	CPropertyPage(IDD, IDS_PROPPAGE_CHARSEL),
	mFileNdx (0)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCharSel::CPropPageCharSel"), this);
#endif
	//{{AFX_DATA_INIT(CPropPageCharSel)
	//}}AFX_DATA_INIT

	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD))
	{
		m_psp.dwFlags |= PSP_DLGINDIRECT;
	}
}

CPropPageCharSel::~CPropPageCharSel()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCharSel::~CPropPageCharSel"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageCharSel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageCharSel)
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_NEXT, mNextButton);
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_BACK, mBackButton);
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_PREVIEW, mCharPreview);
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_TITLE, mCharTitle);
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_NAME_TITLE, mCharNameTitle);
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_NAME, mCharName);
	DDX_Control(pDX, IDC_PROPPAGE_CHARSEL_DESC, mCharDesc);
	//}}AFX_DATA_MAP

	if	(pDX->m_bSaveAndValidate)
	{
		CAgentFile *	lFile;

		if	(
				(lFile = mFiles.Files() (mFileNdx))
			&&	(CString ((BSTR)mFiles.GetDefCharPath ()).CompareNoCase (CString ((BSTR)lFile->GetPath())) != 0)
			&&	(SUCCEEDED (LogComErr (LogIfActive, mFiles.SetDefCharPath (CString ((BSTR)lFile->GetPath())))))
			)
		{
			SetModified (FALSE);
			try
			{
				DWORD	lTargets = BSM_APPLICATIONS;
				BroadcastSystemMessage (BSF_FORCEIFHUNG|BSF_POSTMESSAGE, &lTargets, DA_BROADCAST_DEFCHAR_CHANGED, 0, 0);
			}
			catch AnyExceptionSilent
		}
	}
	else
	{
		if	(mFiles.Files().GetSize() <= 0)
		{
			mFiles.Load ();
		}
		mFileNdx = mFiles.FindDefChar ();
		mFileNdx = max (mFileNdx, 0);
		ShowCharacter ();
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPropPageCharSel, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageCharSel)
	ON_BN_CLICKED(IDC_PROPPAGE_CHARSEL_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PROPPAGE_CHARSEL_BACK, OnBack)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CPropPageCharSel::ShowCharacter ()
{
	CAgentFile *		lFile;
	CAgentFileName *	lFileName;
	CRect				lClientRect;
	CRect				lPreviewRect;

	if	(
			(lFile = mFiles.Files() (mFileNdx))
		&&	(lFileName = lFile->FindName ())
		)
	{
		mCharName.SetWindowText (CString ((BSTR)lFileName->mName));
		mCharDesc.SetWindowText (CString ((BSTR)lFileName->mDesc1));
	}
	else
	{
		mCharName.SetWindowText (_T(""));
		mCharDesc.SetWindowText (_T(""));
	}

	SafeFreeSafePtr (mPreviewWnd);

	if	(
			(lFile)
		&&	(mPreviewWnd = (CAgentPreviewWnd *) CAgentPreviewWnd::CreateObject())
		&&	(mPreviewWnd->Create (mCharPreview.m_hWnd))
		&&	(mPreviewWnd->Open (CString ((BSTR)lFile->GetPath())))
		)
	{
		mCharPreview.ModifyStyleEx (WS_EX_CLIENTEDGE, WS_EX_STATICEDGE, SWP_FRAMECHANGED);
		mCharPreview.ShowWindow (SW_SHOWNA);

		mPreviewWnd->GetWindowRect (&lPreviewRect);
		mCharPreview.GetClientRect (&lClientRect);
		mCharPreview.ScreenToClient (&lPreviewRect);
		lPreviewRect.OffsetRect (lClientRect.CenterPoint() - lPreviewRect.CenterPoint());
		mPreviewWnd->MoveWindow (&lPreviewRect);
		mPreviewWnd->SetBkColor (GetSysColor (COLOR_WINDOW));
		mPreviewWnd->EnableSound (true);
		mPreviewWnd->EnableIdle (true);
		mPreviewWnd->ShowWindow (SW_SHOW);

		if	(GetStyle () & WS_VISIBLE)
		{
			StartCharacter ();
		}
	}
	else
	{
		SafeFreeSafePtr (mPreviewWnd);
	}

	mNextButton.EnableWindow (mFileNdx < mFiles.Files().GetUpperBound());
	mBackButton.EnableWindow (mFileNdx > 0);
}

void CPropPageCharSel::StartCharacter ()
{
	if	(IsWindow (mPreviewWnd->GetSafeHwnd ()))
	{
		if	(
				(!mPreviewWnd->ShowAnimation (_T("Greet")))
			&&	(!mPreviewWnd->ShowAnimation (_T("Greeting")))
			&&	(!mPreviewWnd->ShowAnimation (_T("RestPose")))
			&&	(!mPreviewWnd->ShowAnimation (_T("Show")))
			)
		{
			mPreviewWnd->ShowAnimation (_T("Showing"));
		}
		mPreviewWnd->RedrawWindow ();
	}
}

void CPropPageCharSel::StopCharacter ()
{
	if	(IsWindow (mPreviewWnd->GetSafeHwnd ()))
	{
		mPreviewWnd->Stop ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageCharSel::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return TRUE;
}

void CPropPageCharSel::OnDestroy()
{
	SafeFreeSafePtr (mPreviewWnd);
	CPropertyPage::OnDestroy();
}

void CPropPageCharSel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	if	(bShow)
	{
		StartCharacter ();
	}
	else
	{
		StopCharacter ();
	}
}

/////////////////////////////////////////////////////////////////////////////

HBRUSH CPropPageCharSel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH lBrush = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if	(pWnd->GetSafeHwnd() == mCharPreview.GetSafeHwnd())
	{
		lBrush = GetSysColorBrush (COLOR_WINDOW);
	}
	return lBrush;
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageCharSel::OnNext()
{
	if	(mFileNdx < mFiles.Files().GetUpperBound())
	{
		mFileNdx++;
		ShowCharacter ();
		SetModified	(mFileNdx != mFiles.FindDefChar());
	}
}

void CPropPageCharSel::OnBack()
{
	if	(mFileNdx > 0)
	{
		mFileNdx--;
		ShowCharacter ();
		SetModified	(mFileNdx != mFiles.FindDefChar());
	}
}
