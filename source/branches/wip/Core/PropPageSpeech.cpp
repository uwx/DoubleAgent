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

	mSrTipsEnabled.Attach	(GetDlgItem (IDC_PROPPAGE_SR_TIPS));
	mSrPromptEnabled.Attach	(GetDlgItem (IDC_PROPPAGE_SR_PROMPT));
	mSrHotKeyDelay.Attach			(GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_DELAY));
	mSrHotKeyDelaySpin.Attach		(GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_DELAY_SPIN));
	mSrHotKey.Attach				(GetDlgItem (IDC_PROPPAGE_SR_HOTKEY));
	mSrEnabled.Attach		(GetDlgItem (IDC_PROPPAGE_SR_ENABLED));

//
//KLUDGE - for handling resources where these controls are IDC_STATIC
//
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE1))
	{
		mSrHotKeyTitle1.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE1));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE2))
	{
		mSrHotKeyTitle2.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE2));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE3))
	{
		mSrHotKeyTitle3.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE3));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE4))
	{
		mSrHotKeyTitle4.Attach (GetDlgItem (IDC_PROPPAGE_SR_HOTKEY_TITLE4));
	}

	for	(lWnd = ::GetWindow (mSrEnabled.m_hWnd, GW_HWNDNEXT); lWnd; lWnd = ::GetWindow (lWnd, GW_HWNDNEXT))
	{
		if	((short)::GetDlgCtrlID (lWnd) == IDC_STATIC)
		{
			if	(!mSrHotKeyTitle1.m_hWnd)
			{
				mSrHotKeyTitle1.Attach (lWnd);
			}
			else
			if	(!mSrHotKeyTitle2.m_hWnd)
			{
				mSrHotKeyTitle2.Attach (lWnd);
			}
			else
			if	(!mSrHotKeyTitle3.m_hWnd)
			{
				mSrHotKeyTitle3.Attach (lWnd);
			}
			else
			if	(!mSrHotKeyTitle4.m_hWnd)
			{
				mSrHotKeyTitle4.Attach (lWnd);
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
	mSettingsConfig.LoadConfig ();

	Button_SetCheck (mSrEnabled, mSettingsConfig.mSrEnabled ? TRUE : FALSE);
	Button_SetCheck (mSrTipsEnabled, mSettingsConfig.mSrListeningTip ? TRUE : FALSE);
	Button_SetCheck (mSrPromptEnabled, mSettingsConfig.mSrListeningPrompt ? TRUE : FALSE);
	mSrHotKey.SendMessage (HKM_SETHOTKEY, (WPARAM)mSettingsConfig.mSrHotKey);
	SetDlgItemInt (mSrHotKeyDelay.GetDlgCtrlID(), mSettingsConfig.mSrHotKeyDelay/1000, FALSE);
	mSrHotKeyDelaySpin.SendMessage (UDM_SETRANGE32, (short)(mSettingsConfig.mSrHotKeyDelayMin/1000), (short)(mSettingsConfig.mSrHotKeyDelayMax/1000));

	EnableControls ();
	return TRUE;
}

LRESULT CPropPageSpeech::OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	mSettingsConfig.mSrEnabled = Button_GetCheck (mSrEnabled) ? true : false;
	mSettingsConfig.mSrListeningTip = Button_GetCheck (mSrTipsEnabled) ? true : false;
	mSettingsConfig.mSrListeningPrompt = Button_GetCheck (mSrPromptEnabled) ? true : false;
	mSettingsConfig.mSrHotKey = (WORD) mSrHotKey.SendMessage (HKM_GETHOTKEY);
	mSettingsConfig.mSrHotKeyDelay = min (max (GetDlgItemInt (mSrHotKeyDelay.GetDlgCtrlID())*1000, mSettingsConfig.mSrHotKeyDelayMin), mSettingsConfig.mSrHotKeyDelayMax);

	mSettingsConfig.SaveConfig ();
	return 0;
}

void CPropPageSpeech::EnableControls ()
{
	if	(Button_GetCheck (mSrEnabled))
	{
		mSrTipsEnabled.EnableWindow (TRUE);
		mSrPromptEnabled.EnableWindow (TRUE);
		mSrHotKey.EnableWindow (TRUE);
		mSrHotKeyTitle1.EnableWindow (TRUE);
		mSrHotKeyTitle2.EnableWindow (TRUE);
		if	(mSrHotKey.SendMessage (HKM_GETHOTKEY))
		{
			mSrHotKeyDelaySpin.EnableWindow (TRUE);
			mSrHotKeyDelay.EnableWindow (TRUE);
			mSrHotKeyTitle3.EnableWindow (TRUE);
			mSrHotKeyTitle4.EnableWindow (TRUE);
		}
		else
		{
			mSrHotKeyDelaySpin.EnableWindow (FALSE);
			mSrHotKeyDelay.EnableWindow (FALSE);
			mSrHotKeyTitle3.EnableWindow (FALSE);
			mSrHotKeyTitle4.EnableWindow (FALSE);
		}
	}
	else
	{
		mSrTipsEnabled.EnableWindow (FALSE);
		mSrPromptEnabled.EnableWindow (FALSE);
		mSrHotKeyDelay.EnableWindow (FALSE);
		mSrHotKeyDelaySpin.EnableWindow (FALSE);
		mSrHotKey.EnableWindow (FALSE);
		mSrHotKeyTitle1.EnableWindow (FALSE);
		mSrHotKeyTitle2.EnableWindow (FALSE);
		mSrHotKeyTitle3.EnableWindow (FALSE);
		mSrHotKeyTitle4.EnableWindow (FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageSpeech::OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	(
			((HWND)lParam == mSrHotKeyDelay.m_hWnd)
		&&	(!mSrHotKeyDelay.IsWindowEnabled ())
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

LRESULT CPropPageSpeech::OnSrEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageSpeech::OnSrPrompt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageSpeech::OnSrTips(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageSpeech::OnSrHotKeyChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageSpeech::OnSrHotKeyDelayChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	return 0;
}
