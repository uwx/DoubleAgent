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
#include "PropPageCharacter.h"
#include "AgentPreviewWnd.h"
#include "Localize.h"
#include "GuidStr.h"

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_PROPSHEET	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CPropPageCharacter)

CPropPageCharacter::CPropPageCharacter()
:	CAtlPropertyPage (IDD)
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::CPropPageCharacter (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	mCaption = CLocalize::LoadString (IDS_PROPPAGE_CHARACTER);
	mPsp.pszTitle = (LPCTSTR)mCaption;
	mPsp.dwFlags |= PSP_USETITLE;

	if	(CLocalize::LoadDialog (mTemplate, IDD))
	{
		mPsp.pResource = (PROPSHEETPAGE_RESOURCE)GlobalLock (mTemplate);
		mPsp.hInstance = NULL;
		mPsp.dwFlags |= PSP_DLGINDIRECT;
	}
}

CPropPageCharacter::~CPropPageCharacter()
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::~CPropPageCharacter (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

void CPropPageCharacter::FinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::FinalRelease"), this, m_dwRef);
#endif
}

CPropPageCharacter * CPropPageCharacter::CreateInstance ()
{
	CComObject<CPropPageCharacter> *	lInstance = NULL;
	LogComErr (LogIfActive, CComObject<CPropPageCharacter>::CreateInstance (&lInstance));
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CPropPageCharacter::AddSheetPage (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam)
{
	HRESULT				lResult = E_FAIL;
	HPROPSHEETPAGE		lPage;
	tS <PROPSHEETPAGE>	lPsp;

	lPsp = mPsp;
	lPsp.lParam = (LPARAM) this;
	lPsp.pfnCallback = PropPageCallback;

#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p(%d)] CPropPageCharacter::CreatePropertySheetPage"), this, m_dwRef);
#endif
	if	(lPage = CreatePropertySheetPage (&lPsp))
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

	pPropPage->lParam = lThis->mPsp.lParam;

	try
	{
		lRet = (*(lThis->mPsp.pfnCallback)) (pWnd, pMsgId, pPropPage);
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
			lThis->Release ();
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

BOOL CPropPageCharacter::OnInitDialog()
{
	mCharTtsModeID.Attach	(GetDlgItem (IDC_PROPPAGE_CHARACTER_TTSMODEID));
	mCharStandard.Attach	(GetDlgItem (IDC_PROPPAGE_CHARACTER_STANDARD));
	mCharPreview.Attach		(GetDlgItem (IDC_PROPPAGE_CHARACTER_PREVIEW));
	mCharName.Attach		(GetDlgItem (IDC_PROPPAGE_CHARACTER_NAME));
	mCharInvalid.Attach		(GetDlgItem (IDC_PROPPAGE_CHARACTER_INVALID));
	mCharGuid.Attach		(GetDlgItem (IDC_PROPPAGE_CHARACTER_GUID));
	mCharFileVer.Attach		(GetDlgItem (IDC_PROPPAGE_CHARACTER_FILEVER));
	mCharDesc.Attach		(GetDlgItem (IDC_PROPPAGE_CHARACTER_DESC));

	if	(mAgentFile.Ptr())
	{
		ShowCharacter ();
		ShowPreview ();
	}
	else
	{
		NoCharacter ();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageCharacter::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CPropPageCharacter::OnDestroy"), this, m_dwRef);
#endif
	SafeFreeSafePtr (mPreviewWnd);
	bHandled = FALSE;
	return 0;
}

LRESULT CPropPageCharacter::OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT lResult = 0;

	if	(!(bHandled = CAtlPropertyPage::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(wParam)
	{
		StartPreview ();
	}
	else
	{
		StopPreview ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageCharacter::OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	((HWND)lParam == mCharPreview.m_hWnd)
	{
		lResult = (LRESULT) GetSysColorBrush (COLOR_WINDOW);
	}
	else
	{
		bHandled = FALSE;
	}
	return lResult;
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
	CAtlString			lVersionStr;
	CAtlString			lGuid (CGuidStr (mAgentFile->GetGuid()));
	CAtlString			lTtsModeID ((BSTR)mAgentFile->GetTts().mModeId);

	mCharName.ShowWindow (SW_SHOWNA);
	mCharDesc.ShowWindow (SW_SHOWNA);
	mCharPreview.ShowWindow (SW_SHOWNA);
	mCharInvalid.ShowWindow (SW_HIDE);

	if	(lName = mAgentFile->FindName ())
	{
		mCharName.SetWindowText (CAtlString ((BSTR)lName->mName));
		mCharDesc.SetWindowText (CAtlString ((BSTR)lName->mDesc1));
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
			(mPreviewWnd = CAgentPreviewWnd::CreateInstance())
		&&	(mPreviewWnd->Create (mCharPreview.m_hWnd))
		&&	(mPreviewWnd->Open (CAtlString ((BSTR)mAgentFile->GetPath())))
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
	if	(
			(mPreviewWnd)
		&&	(mPreviewWnd->IsWindow ())
		)
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
	if	(
			(mPreviewWnd)
		&&	(mPreviewWnd->IsWindow ())
		)
	{
		mPreviewWnd->Stop ();
	}
}
