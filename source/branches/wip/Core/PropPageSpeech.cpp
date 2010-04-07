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
#include "PropPageSpeech.h"
#include "Localize.h"

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CPropPageSpeech)

CPropPageSpeech::CPropPageSpeech()
:	CAtlPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageSpeech::CPropPageSpeech"), this);
#endif
	if	(CLocalize::LoadDialog (mTemplate, IDD))
	{
		mPsp.pResource = (PROPSHEETPAGE_RESOURCE)GlobalLock (mTemplate);
		mPsp.hInstance = NULL;
		mPsp.dwFlags |= PSP_DLGINDIRECT;
	}
}

CPropPageSpeech::~CPropPageSpeech()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageSpeech::~CPropPageSpeech"), this);
#endif
}

CPropPageSpeech * CPropPageSpeech::CreateInstance ()
{
	return new CPropPageSpeech;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageSpeech::OnInitDialog ()
{
	HWND	lWnd;

	mSpeechTipsEnabled.Attach	(GetDlgItem (IDC_PROPPAGE_SR_TIPS));
	mSpeechPromptEnabled.Attach	(GetDlgItem (IDC_PROPPAGE_SR_PROMPT));
	mHotKeyDelay.Attach			(GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_DELAY));
	mHotKeyDelaySpin.Attach		(GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_DELAY_SPIN));
	mHotKey.Attach				(GetDlgItem (IDC_PROPPAGE_SR_HOTKEY));
	mSpeechEnabled.Attach		(GetDlgItem (IDC_PROPPAGE_SR_ENABLED));

//
//KLUDGE - for handling resources where these controls are IDC_STATIC
//
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE1))
	{
		mHotKeyTitle1.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE1));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE2))
	{
		mHotKeyTitle2.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE2));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE3))
	{
		mHotKeyTitle3.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE3));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE4))
	{
		mHotKeyTitle4.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE4));
	}

	for	(lWnd = ::GetWindow (mSpeechEnabled.m_hWnd, GW_HWNDNEXT); lWnd; lWnd = ::GetWindow (lWnd, GW_HWNDNEXT))
	{
		if	((short)::GetDlgCtrlID (lWnd) == IDC_STATIC)
		{
			if	(!mHotKeyTitle1.m_hWnd)
			{
				mHotKeyTitle1.Attach (lWnd);
			}
			else
			if	(!mHotKeyTitle2.m_hWnd)
			{
				mHotKeyTitle2.Attach (lWnd);
			}
			else
			if	(!mHotKeyTitle3.m_hWnd)
			{
				mHotKeyTitle3.Attach (lWnd);
			}
			else
			if	(!mHotKeyTitle4.m_hWnd)
			{
				mHotKeyTitle4.Attach (lWnd);
			}
			else
			{
				break;
			}
		}
	}
//
//END KLUDGE
//
	mSpeechConfig.LoadConfig ();

	Button_SetCheck (mSpeechEnabled, mSpeechConfig.mEnabled ? TRUE : FALSE);
	Button_SetCheck (mSpeechTipsEnabled, mSpeechConfig.mListeningTip ? TRUE : FALSE);
	Button_SetCheck (mSpeechPromptEnabled, mSpeechConfig.mListeningPrompt ? TRUE : FALSE);
	mHotKey.SendMessage (HKM_SETHOTKEY, (WPARAM)mSpeechConfig.mHotKey);
	SetDlgItemInt (mHotKeyDelay.GetDlgCtrlID(), mSpeechConfig.mHotKeyDelay/1000, FALSE);
	mHotKeyDelaySpin.SendMessage (UDM_SETRANGE32, (short)(mSpeechConfig.mHotKeyDelayMin/1000), (short)(mSpeechConfig.mHotKeyDelayMax/1000));

	EnableControls ();
	return TRUE;
}

LRESULT CPropPageSpeech::OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	mSpeechConfig.RegisterHotKey (false);

	mSpeechConfig.mEnabled = Button_GetCheck (mSpeechEnabled) ? true : false;
	mSpeechConfig.mListeningTip = Button_GetCheck (mSpeechTipsEnabled) ? true : false;
	mSpeechConfig.mListeningPrompt = Button_GetCheck (mSpeechPromptEnabled) ? true : false;
	mSpeechConfig.mHotKey = (WORD) mHotKey.SendMessage (HKM_GETHOTKEY);
	mSpeechConfig.mHotKeyDelay = min (max (GetDlgItemInt (mHotKeyDelay.GetDlgCtrlID())*1000, mSpeechConfig.mHotKeyDelayMin), mSpeechConfig.mHotKeyDelayMax);

	mSpeechConfig.SaveConfig ();
	return 0;
}

void CPropPageSpeech::EnableControls ()
{
	if	(Button_GetCheck (mSpeechEnabled))
	{
		mSpeechTipsEnabled.EnableWindow (TRUE);
		mSpeechPromptEnabled.EnableWindow (TRUE);
		mHotKey.EnableWindow (TRUE);
		mHotKeyTitle1.EnableWindow (TRUE);
		mHotKeyTitle2.EnableWindow (TRUE);
		if	(mHotKey.SendMessage (HKM_GETHOTKEY))
		{
			mHotKeyDelaySpin.EnableWindow (TRUE);
			mHotKeyDelay.EnableWindow (TRUE);
			mHotKeyTitle3.EnableWindow (TRUE);
			mHotKeyTitle4.EnableWindow (TRUE);
		}
		else
		{
			mHotKeyDelaySpin.EnableWindow (FALSE);
			mHotKeyDelay.EnableWindow (FALSE);
			mHotKeyTitle3.EnableWindow (FALSE);
			mHotKeyTitle4.EnableWindow (FALSE);
		}
	}
	else
	{
		mSpeechTipsEnabled.EnableWindow (FALSE);
		mSpeechPromptEnabled.EnableWindow (FALSE);
		mHotKeyDelay.EnableWindow (FALSE);
		mHotKeyDelaySpin.EnableWindow (FALSE);
		mHotKey.EnableWindow (FALSE);
		mHotKeyTitle1.EnableWindow (FALSE);
		mHotKeyTitle2.EnableWindow (FALSE);
		mHotKeyTitle3.EnableWindow (FALSE);
		mHotKeyTitle4.EnableWindow (FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageSpeech::OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	(
			((HWND)lParam == mHotKeyDelay.m_hWnd)
		&&	(!mHotKeyDelay.IsWindowEnabled ())
		)
	{
		::SetBkColor ((HDC)wParam, GetSysColor (COLOR_3DFACE));
		lResult = (LRESULT) GetSysColorBrush (COLOR_3DFACE);
	}
	else
	{
		bHandled = CAtlPropertyPage::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageSpeech::OnSpeechEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageSpeech::OnSpeechPrompt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageSpeech::OnSpeechTips(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageSpeech::OnHotKeyChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageSpeech::OnHotKeyDelayChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	return 0;
}
