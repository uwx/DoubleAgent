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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropPageSpeech, CPropertyPage)

BEGIN_MESSAGE_MAP(CPropPageSpeech, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageSpeech)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PROPPAGE_SR_ENABLED, OnSpeechEnabled)
	ON_BN_CLICKED(IDC_PROPPAGE_SR_PROMPT, OnSpeechPrompt)
	ON_BN_CLICKED(IDC_PROPPAGE_SR_TIPS, OnSpeechTips)
	ON_EN_CHANGE(IDC_PROPPAGE_SR_HOTKEY, OnHotKeyChange)
	ON_EN_CHANGE(IDC_PROPPAGE_SR_HOTKEY_DELAY, OnHotKeyDelayChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropPageSpeech::CPropPageSpeech()
:	CPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageSpeech::CPropPageSpeech"), this);
#endif
	//{{AFX_DATA_INIT(CPropPageSpeech)
	//}}AFX_DATA_INIT

	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD))
	{
		m_psp.dwFlags |= PSP_DLGINDIRECT;
		m_psp.pszTitle = (LPCTSTR) (m_strCaption = mPropPageFix.GetTemplateCaption (m_psp.pResource));
	}
}

CPropPageSpeech::~CPropPageSpeech()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageSpeech::~CPropPageSpeech"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageSpeech::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSpeech)
	DDX_Control(pDX, IDC_PROPPAGE_SR_TIPS, mSpeechTipsEnabled);
	DDX_Control(pDX, IDC_PROPPAGE_SR_PROMPT, mSpeechPromptEnabled);
	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_DELAY, mHotKeyDelay);
	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_DELAY_SPIN, mHotKeySpin);
	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY, mHotKey);
	DDX_Control(pDX, IDC_PROPPAGE_SR_ENABLED, mSpeechEnabled);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE1, mHotKeyTitle1);
//	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE2, mHotKeyTitle2);
//	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE3, mHotKeyTitle3);
//	DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE4, mHotKeyTitle4);

//
//	KLUDGE
//
	if	(
			(!pDX->m_bSaveAndValidate)
		&&	(!mHotKeyTitle1.m_hWnd)
		)
	{
		HWND	lWnd;

		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE1))
		{
			DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE1, mHotKeyTitle1);
		}
		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE2))
		{
			DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE2, mHotKeyTitle2);
		}
		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE3))
		{
			DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE3, mHotKeyTitle3);
		}
		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_SR_HOTKEY_TITLE4))
		{
			DDX_Control(pDX, IDC_PROPPAGE_SR_HOTKEY_TITLE4, mHotKeyTitle4);
		}

		for	(lWnd = ::GetWindow (mSpeechEnabled.m_hWnd, GW_HWNDNEXT); lWnd; lWnd = ::GetWindow (lWnd, GW_HWNDNEXT))
		{
			if	((short)::GetDlgCtrlID (lWnd) == IDC_STATIC)
			{
				if	(!mHotKeyTitle1.m_hWnd)
				{
					mHotKeyTitle1.SubclassWindow (lWnd);
				}
				else
				if	(!mHotKeyTitle2.m_hWnd)
				{
					mHotKeyTitle2.SubclassWindow (lWnd);
				}
				else
				if	(!mHotKeyTitle3.m_hWnd)
				{
					mHotKeyTitle3.SubclassWindow (lWnd);
				}
				else
				if	(!mHotKeyTitle4.m_hWnd)
				{
					mHotKeyTitle4.SubclassWindow (lWnd);
				}
				else
				{
					break;
				}
			}
		}
	}
//
//	END KLUDGE
//

	if	(pDX->m_bSaveAndValidate)
	{
		mSpeechConfig.RegisterHotKey (false);

		mSpeechConfig.mEnabled = mSpeechEnabled.GetCheck () ? true : false;
		mSpeechConfig.mListeningTip = mSpeechTipsEnabled.GetCheck () ? true : false;
		mSpeechConfig.mListeningPrompt = mSpeechPromptEnabled.GetCheck () ? true : false;
		mSpeechConfig.mHotKey = (WORD) mHotKey.GetHotKey ();
		mSpeechConfig.mHotKeyDelay = min (max (GetDlgItemInt (mHotKeyDelay.GetDlgCtrlID())*1000, mSpeechConfig.mHotKeyDelayMin), mSpeechConfig.mHotKeyDelayMax);

		mSpeechConfig.SaveConfig ();
	}
	else
	{
		mSpeechConfig.LoadConfig ();

		mSpeechEnabled.SetCheck (mSpeechConfig.mEnabled ? TRUE : FALSE);
		mSpeechTipsEnabled.SetCheck (mSpeechConfig.mListeningTip ? TRUE : FALSE);
		mSpeechPromptEnabled.SetCheck (mSpeechConfig.mListeningPrompt ? TRUE : FALSE);
		mHotKey.SetHotKey (LOBYTE (mSpeechConfig.mHotKey), HIBYTE (mSpeechConfig.mHotKey));
		SetDlgItemInt (mHotKeyDelay.GetDlgCtrlID(), mSpeechConfig.mHotKeyDelay/1000, FALSE);
		mHotKeySpin.SetRange ((short)(mSpeechConfig.mHotKeyDelayMin/1000), (short)(mSpeechConfig.mHotKeyDelayMax/1000));

		EnableControls ();
	}
}

void CPropPageSpeech::EnableControls ()
{
	if	(mSpeechEnabled.GetCheck ())
	{
		mSpeechTipsEnabled.EnableWindow (TRUE);
		mSpeechPromptEnabled.EnableWindow (TRUE);
		mHotKey.EnableWindow (TRUE);
		mHotKeyTitle1.EnableWindow (TRUE);
		mHotKeyTitle2.EnableWindow (TRUE);
		if	(mHotKey.GetHotKey ())
		{
			mHotKeyDelay.EnableWindow (TRUE);
			mHotKeySpin.EnableWindow (TRUE);
			mHotKeyTitle3.EnableWindow (TRUE);
			mHotKeyTitle4.EnableWindow (TRUE);
		}
		else
		{
			mHotKeyDelay.EnableWindow (FALSE);
			mHotKeySpin.EnableWindow (FALSE);
			mHotKeyTitle3.EnableWindow (FALSE);
			mHotKeyTitle4.EnableWindow (FALSE);
		}
	}
	else
	{
		mSpeechTipsEnabled.EnableWindow (FALSE);
		mSpeechPromptEnabled.EnableWindow (FALSE);
		mHotKeyDelay.EnableWindow (FALSE);
		mHotKeySpin.EnableWindow (FALSE);
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

HBRUSH CPropPageSpeech::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if	(
			(pWnd == &mHotKeyDelay)
		&&	(!mHotKeyDelay.IsWindowEnabled ())
		)
	{
		pDC->SetBkColor (GetSysColor (COLOR_3DFACE));
		return GetSysColorBrush (COLOR_3DFACE);
	}
	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageSpeech::OnSpeechEnabled()
{
	SetModified ();
	EnableControls ();
}

void CPropPageSpeech::OnSpeechPrompt()
{
	SetModified ();
	EnableControls ();
}

void CPropPageSpeech::OnSpeechTips()
{
	SetModified ();
	EnableControls ();
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageSpeech::OnHotKeyChange()
{
	SetModified ();
	EnableControls ();
}

void CPropPageSpeech::OnHotKeyDelayChange()
{
	SetModified ();
}
