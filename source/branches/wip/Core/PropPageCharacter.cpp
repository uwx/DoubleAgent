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
#include <AfxPriv.h>
#include "DaCore.h"
#include "PropPageCharacter.h"
#include "AgentPreviewWnd.h"
#include "Localize.h"
#include "GuidStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_PROPSHEET	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropPageCharacter, CPropertyPage)

BEGIN_MESSAGE_MAP(CPropPageCharacter, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageCharacter)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropPageCharacter::CPropPageCharacter()
:	CPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::CPropPageCharacter (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	//{{AFX_DATA_INIT(CPropPageCharacter)
	//}}AFX_DATA_INIT

	m_strCaption = CLocalize::LoadString (IDS_PROPPAGE_CHARACTER);
	m_psp.pszTitle = (LPCTSTR) m_strCaption;
	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD))
	{
		m_psp.dwFlags |= PSP_DLGINDIRECT;
		m_psp.pResource = mPropPageFix.SetTemplateCaption (m_psp.pResource, m_psp.pszTitle);
	}

	EnableAggregation();
}

CPropPageCharacter::~CPropPageCharacter()
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::~CPropPageCharacter (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleUnlockApp ();
}

void CPropPageCharacter::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::OnFinalRelease"), this, m_dwRef);
#endif
	CCmdTarget::OnFinalRelease ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CPropPageCharacter::AddSheetPage (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam)
{
	HRESULT						lResult = E_FAIL;
	HPROPSHEETPAGE				lPage;
	tS <PROPSHEETPAGE_MFC_VER>	lPsp;

	lPsp = m_psp;
	lPsp.lParam = (LPARAM) this;
	lPsp.pfnCallback = PropPageCallback;

#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p(%d)] CPropPageCharacter::CreatePropertySheetPage"), this, m_dwRef);
#endif
	if	(lPage = CreatePropertySheetPage ((PROPSHEETPAGE*) &lPsp))
	{
		if	((*pAddPageFunc) (lPage, pLparam))
		{
			lResult = S_OK;
		}
		else
		{
#ifdef	_DEBUG_PROPSHEET
			LogMessage (_DEBUG_PROPSHEET, _T("[%p(%d)] CPropPageCharacter::DestroyPropertySheetPage"), this, m_dwRef);
#endif
			DestroyPropertySheetPage (lPage);
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

#ifdef	_DEBUG_PROPSHEET
	LogComErrAnon (MinLogLevel(_DEBUG_PROPSHEET,LogAlways), lResult, _T("[%p(%d)] CPropPageCharacter::AddSheetPage"), this, m_dwRef);
#endif
	return lResult;
}

UINT CALLBACK CPropPageCharacter::PropPageCallback (HWND pWnd, UINT pMsgId, LPPROPSHEETPAGE pPropPage)
{
	UINT					lRet = 0;
	CPropPageCharacter *	lThis = (CPropPageCharacter *) pPropPage->lParam;

	pPropPage->lParam = lThis->m_psp.lParam;

	try
	{
		lRet = (*(lThis->m_psp.pfnCallback)) (pWnd, pMsgId, pPropPage);
	}
	catch AnyExceptionSilent

	pPropPage->lParam = (LPARAM) lThis;

	if	(pMsgId == PSPCB_RELEASE)
	{
#ifdef	_DEBUG_PROPSHEET
		try
		{
//			LogMessage (_DEBUG_PROPSHEET, _T("[%p(%d)] CPropPageCharacter::PropPageCallback PSPCB_RELEASE"), lThis, lThis->m_dwRef);
			LogMessage (_DEBUG_PROPSHEET, _T("[%p] CPropPageCharacter::PropPageCallback PSPCB_RELEASE"), lThis);
		}
		catch AnyExceptionSilent
#endif
		try
		{
			lThis->ExternalRelease();
		}
		catch AnyExceptionSilent

#ifdef	_DEBUG_PROPSHEET
		try
		{
			LogMessage (_DEBUG_PROPSHEET, _T("[%p] CPropPageCharacter::PropPageCallback Released"), lThis);
		}
		catch AnyExceptionSilent
#endif
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageCharacter::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageCharacter)
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_TTSMODEID, mCharTtsModeID);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_STANDARD, mCharStandard);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_PREVIEW, mCharPreview);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_NAME, mCharName);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_INVALID, mCharInvalid);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_GUID, mCharGuid);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_FILEVER, mCharFileVer);
	DDX_Control(pDX, IDC_PROPPAGE_CHARACTER_DESC, mCharDesc);
	//}}AFX_DATA_MAP

	if	(!pDX->m_bSaveAndValidate)
	{
		if	(mAgentFile.Ptr())
		{
			ShowCharacter ();
			ShowPreview ();
		}
		else
		{
			NoCharacter ();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageCharacter::OnInitDialog()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::OnInitDialog"), this, m_dwRef);
#endif
	return CPropertyPage::OnInitDialog ();
}

void CPropPageCharacter::OnDestroy ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::OnDestroy"), this, m_dwRef);
#endif
	SafeFreeSafePtr (mPreviewWnd);
	CPropertyPage::OnDestroy();
}

void CPropPageCharacter::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	if	(bShow)
	{
		StartPreview ();
	}
	else
	{
		StopPreview ();
	}
}

/////////////////////////////////////////////////////////////////////////////

HBRUSH CPropPageCharacter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH lBrush = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if	(pWnd->GetSafeHwnd() == mCharPreview.GetSafeHwnd())
	{
		lBrush = GetSysColorBrush (COLOR_WINDOW);
	}
	return lBrush;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageCharacter::NoCharacter ()
{
	mCharName.ShowWindow (SW_HIDE);
	mCharDesc.ShowWindow (SW_HIDE);
	mCharPreview.ShowWindow (SW_HIDE);
	mCharInvalid.ShowWindow (SW_SHOWNA);

	mCharGuid.SetWindowText (_T(""));
	mCharFileVer.SetWindowText (_T(""));
	mCharStandard.SetWindowText (_T(""));
	mCharTtsModeID.SetWindowText (_T(""));
	mCharInvalid.SetWindowText (CLocalize::LoadString (IDS_PROPPAGE_CHARACTER_INVALID));
}

void CPropPageCharacter::ShowCharacter ()
{
	CAgentFileName *	lName;
	DWORD				lVersion = mAgentFile->GetVersion();
	CString				lVersionStr;
	CString				lGuid (CGuidStr (mAgentFile->GetGuid()));
	CString				lTtsModeID ((BSTR)mAgentFile->GetTts().mModeId);

	mCharName.ShowWindow (SW_SHOWNA);
	mCharDesc.ShowWindow (SW_SHOWNA);
	mCharPreview.ShowWindow (SW_SHOWNA);
	mCharInvalid.ShowWindow (SW_HIDE);

	if	(lName = mAgentFile->FindName ())
	{
		mCharName.SetWindowText (CString ((BSTR)lName->mName));
		mCharDesc.SetWindowText (CString ((BSTR)lName->mDesc1));
	}

	lVersionStr.Format (_T("%u.%2.2u"), HIWORD(lVersion), LOWORD(lVersion));
	if	(lTtsModeID.IsEmpty())
	{
		lTtsModeID = CLocalize::LoadString (IDS_PROPPAGE_CHARACTER_TTSNONE);
	}

	mCharFileVer.SetWindowText (lVersionStr);
	mCharGuid.SetWindowText (lGuid);
	mCharTtsModeID.SetWindowText (lTtsModeID);

	if	(mAgentFile->GetStyle() & CharStyleStandard)
	{
		mCharStandard.SetWindowText (_T("2.0"));
	}
	else
	{
		mCharStandard.SetWindowText (CLocalize::LoadString (IDS_PROPPAGE_CHARACTER_NONSTANDARD));
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageCharacter::ShowPreview ()
{
	CRect	lClientRect;
	CRect	lPreviewRect;

	if	(
			(mPreviewWnd = (CAgentPreviewWnd *) CAgentPreviewWnd::CreateObject())
		&&	(mPreviewWnd->Create (mCharPreview.m_hWnd))
		&&	(mPreviewWnd->Open (CString ((BSTR)mAgentFile->GetPath())))
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
			StartPreview ();
		}
	}
	else
	{
		SafeFreeSafePtr (mPreviewWnd);
	}
}

void CPropPageCharacter::StartPreview ()
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

void CPropPageCharacter::StopPreview ()
{
	if	(IsWindow (mPreviewWnd->GetSafeHwnd ()))
	{
		mPreviewWnd->Stop ();
	}
}
