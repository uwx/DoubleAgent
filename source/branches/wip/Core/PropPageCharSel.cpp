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
#include "Localize.h"

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CPropPageCharSel)

CPropPageCharSel::CPropPageCharSel()
:	CAtlPropertyPage (IDD),
	mFileNdx (0)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCharSel::CPropPageCharSel"), this);
#endif
	mCaption = CLocalize::LoadString (IDS_PROPPAGE_CHARSEL);
	mPsp.pszTitle = (LPCTSTR)mCaption;
	mPsp.dwFlags |= PSP_USETITLE;

	if	(CLocalize::LoadDialog (mTemplate, IDD))
	{
		mPsp.pResource = (PROPSHEETPAGE_RESOURCE)GlobalLock (mTemplate);
		mPsp.hInstance = NULL;
		mPsp.dwFlags |= PSP_DLGINDIRECT;
	}
}

CPropPageCharSel::~CPropPageCharSel()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageCharSel::~CPropPageCharSel"), this);
#endif
}

CPropPageCharSel * CPropPageCharSel::CreateInstance ()
{
	return new CPropPageCharSel;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageCharSel::OnInitDialog()
{
	mNextButton.Attach		(GetDlgItem (IDC_PROPPAGE_CHARSEL_NEXT));
	mBackButton.Attach		(GetDlgItem (IDC_PROPPAGE_CHARSEL_BACK));
	mCharPreview.Attach		(GetDlgItem (IDC_PROPPAGE_CHARSEL_PREVIEW));
	mCharTitle.Attach		(GetDlgItem (IDC_PROPPAGE_CHARSEL_TITLE));
	mCharNameTitle.Attach	(GetDlgItem (IDC_PROPPAGE_CHARSEL_NAME_TITLE));
	mCharName.Attach		(GetDlgItem (IDC_PROPPAGE_CHARSEL_NAME));
	mCharDesc.Attach		(GetDlgItem (IDC_PROPPAGE_CHARSEL_DESC));

	if	(mFiles.Files().GetCount() <= 0)
	{
		mFiles.Load ();
	}
	mFileNdx = mFiles.FindDefChar ();
	mFileNdx = max (mFileNdx, 0);
	ShowCharacter ();

	return TRUE;
}

LRESULT CPropPageCharSel::OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	CAgentFile *	lFile;

	if	(
			(lFile = mFiles.Files() (mFileNdx))
		&&	(CAtlString ((BSTR)mFiles.GetDefCharPath ()).CompareNoCase (CAtlString ((BSTR)lFile->GetPath())) != 0)
		&&	(SUCCEEDED (LogComErr (LogIfActive, mFiles.SetDefCharPath (CAtlString ((BSTR)lFile->GetPath())))))
		)
	{
		SetModified (FALSE);
		try
		{
			DWORD	lTargets = BSM_APPLICATIONS;
			long	lResult;

			lResult = BroadcastSystemMessage (BSF_FORCEIFHUNG|BSF_POSTMESSAGE, &lTargets, DA_BROADCAST_DEFCHAR_CHANGED, 0, 0);
#ifdef	_DEBUG
			LogMessage (LogNormal, _T("DA_BROADCAST_DEFCHAR_CHANGED [%d]"), lResult);
#endif
		}
		catch AnyExceptionSilent
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
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
		mCharName.SetWindowText (CAtlString ((BSTR)lFileName->mName));
		mCharDesc.SetWindowText (CAtlString ((BSTR)lFileName->mDesc1));
	}
	else
	{
		mCharName.SetWindowText (_T(""));
		mCharDesc.SetWindowText (_T(""));
	}

	SafeFreeSafePtr (mPreviewWnd);

	if	(
			(lFile)
		&&	(mPreviewWnd = CAgentPreviewWnd::CreateInstance())
		&&	(mPreviewWnd->Create (mCharPreview.m_hWnd))
		&&	(mPreviewWnd->Open (CAtlString ((BSTR)lFile->GetPath())))
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

	mNextButton.EnableWindow (mFileNdx < (INT_PTR)mFiles.Files().GetCount()-1);
	mBackButton.EnableWindow (mFileNdx > 0);
}

void CPropPageCharSel::StartCharacter ()
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

void CPropPageCharSel::StopCharacter ()
{
	if	(
			(mPreviewWnd)
		&&	(mPreviewWnd->IsWindow ())
		)
	{
		mPreviewWnd->Stop ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageCharSel::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	SafeFreeSafePtr (mPreviewWnd);
	bHandled = FALSE;
	return 0;
}

LRESULT CPropPageCharSel::OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT lResult = 0;

	if	(!(bHandled = CAtlPropertyPage::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(wParam)
	{
		StartCharacter ();
	}
	else
	{
		StopCharacter ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageCharSel::OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CPropPageCharSel::OnNext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if	(mFileNdx < (INT_PTR)mFiles.Files().GetCount()-1)
	{
		mFileNdx++;
		ShowCharacter ();
		SetModified	(mFileNdx != mFiles.FindDefChar());
	}
	return 0;
}

LRESULT CPropPageCharSel::OnBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if	(mFileNdx > 0)
	{
		mFileNdx--;
		ShowCharacter ();
		SetModified	(mFileNdx != mFiles.FindDefChar());
	}
	return 0;
}
