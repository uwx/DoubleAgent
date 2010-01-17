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
#include "PropPageOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropPageOutput, CPropertyPage)

BEGIN_MESSAGE_MAP(CPropPageOutput, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOutput)
	ON_BN_CLICKED(IDC_PROPPAGE_BALLOON_ENABLED, OnBalloonEnabled)
	ON_BN_CLICKED(IDC_PROPPAGE_BALLOON_FONT_DEFAULT, OnBalloonFontDefault)
	ON_BN_CLICKED(IDC_PROPPAGE_BALLOON_FONT_CUSTOM, OnBalloonFontCustom)
	ON_BN_CLICKED(IDC_PROPPAGE_BALLOON_FONT_CHOOSE, OnBalloonFontChoose)
	ON_BN_CLICKED(IDC_PROPPAGE_TTS_ENABLED, OnSpeechEnabled)
	ON_BN_CLICKED(IDC_PROPPAGE_SOUND_ENABLED, OnSoundEnabled)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropPageOutput::CPropPageOutput()
:	CPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageOutput::CPropPageOutput"), this);
#endif
	//{{AFX_DATA_INIT(CPropPageOutput)
	//}}AFX_DATA_INIT

	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD))
	{
		m_psp.dwFlags |= PSP_DLGINDIRECT;
		m_psp.pszTitle = (LPCTSTR) (m_strCaption = mPropPageFix.GetTemplateCaption (m_psp.pResource));
	}
}

CPropPageOutput::~CPropPageOutput()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageOutput::~CPropPageOutput"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageOutput::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOutput)
	DDX_Control(pDX, IDC_PROPPAGE_BALLOON_FONT_CUSTOM, mBalloonFontCustom);
	DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED, mSpeechSpeed);
	DDX_Control(pDX, IDC_PROPPAGE_TTS_ENABLED, mSpeechEnabled);
	DDX_Control(pDX, IDC_PROPPAGE_SOUND_ENABLED, mSoundEnabled);
	DDX_Control(pDX, IDC_PROPPAGE_BALLOON_FONT_DEFAULT, mBalloonFontDefault);
	DDX_Control(pDX, IDC_PROPPAGE_BALLOON_ENABLED, mBalloonEnabled);
	DDX_Control(pDX, IDC_PROPPAGE_BALLOON_FONT_CHOOSE, mBalloonFontChoose);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_SLOW, mSpeechSpeedSlow);
//	DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_FAST, mSpeechSpeedFast);
//	DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_TITLE, mSpeechSpeedTitle);
//	DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_DEFAULT, mSpeechSpeedDefault);

//
//	KLUDGE
//
	if	(
			(!pDX->m_bSaveAndValidate)
		&&	(!mSpeechSpeedTitle.m_hWnd)
		)
	{
		HWND	lWnd;

		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_SLOW))
		{
			DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_SLOW, mSpeechSpeedSlow);
		}
		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_FAST))
		{
			DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_FAST, mSpeechSpeedFast);
		}
		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_TITLE))
		{
			DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_TITLE, mSpeechSpeedTitle);
		}
		if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_DEFAULT))
		{
			DDX_Control(pDX, IDC_PROPPAGE_TTS_SPEED_DEFAULT, mSpeechSpeedDefault);
		}

		for	(lWnd = ::GetWindow (mSpeechEnabled.m_hWnd, GW_HWNDNEXT); lWnd; lWnd = ::GetWindow (lWnd, GW_HWNDNEXT))
		{
			if	((short)::GetDlgCtrlID (lWnd) == IDC_STATIC)
			{
				if	(!mSpeechSpeedTitle.m_hWnd)
				{
					mSpeechSpeedTitle.SubclassWindow (lWnd);
				}
				else
				if	(!mSpeechSpeedSlow.m_hWnd)
				{
					mSpeechSpeedSlow.SubclassWindow (lWnd);
				}
				else
				if	(!mSpeechSpeedFast.m_hWnd)
				{
					mSpeechSpeedFast.SubclassWindow (lWnd);
				}
				else
				if	(!mSpeechSpeedDefault.m_hWnd)
				{
					mSpeechSpeedDefault.SubclassWindow (lWnd);
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
		mBalloonConfig.mEnabled = mBalloonEnabled.GetCheck () ? true : false;
		mAudioConfig.mTtsEnabled = mSpeechEnabled.GetCheck () ? true : false;
		mAudioConfig.mSpeechSpeed = mSpeechSpeed.GetPos ();
		mAudioConfig.mEffectsEnabled = mSoundEnabled.GetCheck () ? true : false;

		mBalloonConfig.SaveConfig ();
		mAudioConfig.SaveConfig ();
	}
	else
	{
		mBalloonConfig.LoadConfig ();
		mAudioConfig.LoadConfig ();

		mBalloonEnabled.SetCheck (mBalloonConfig.mEnabled ? TRUE : FALSE);
		mBalloonFontDefault.SetCheck (mBalloonConfig.mFont.Ptr() ? FALSE : TRUE);
		mBalloonFontCustom.SetCheck (mBalloonConfig.mFont.Ptr() ? TRUE : FALSE);
		mSpeechEnabled.SetCheck (mAudioConfig.mTtsEnabled ? TRUE : FALSE);
		mSpeechSpeed.SetRange (mAudioConfig.mSpeechSpeedMin, mAudioConfig.mSpeechSpeedMax);
		mSpeechSpeed.SetPos (mAudioConfig.mSpeechSpeed);
		mSoundEnabled.SetCheck (mAudioConfig.mEffectsEnabled ? TRUE : FALSE);

		EnableControls ();
	}
}

void CPropPageOutput::EnableControls ()
{
	mBalloonFontDefault.EnableWindow (mBalloonEnabled.GetCheck () ? TRUE : FALSE);
	mBalloonFontCustom.EnableWindow (mBalloonEnabled.GetCheck () ? TRUE : FALSE);
	mBalloonFontChoose.EnableWindow ((mBalloonEnabled.GetCheck () && mBalloonFontCustom.GetCheck ()) ? TRUE : FALSE);

	mSpeechSpeed.EnableWindow (mSpeechEnabled.GetCheck () ? TRUE : FALSE);
	mSpeechSpeedSlow.EnableWindow (mSpeechEnabled.GetCheck () ? TRUE : FALSE);
	mSpeechSpeedTitle.EnableWindow (mSpeechEnabled.GetCheck () ? TRUE : FALSE);
	mSpeechSpeedFast.EnableWindow (mSpeechEnabled.GetCheck () ? TRUE : FALSE);
	mSpeechSpeedDefault.EnableWindow (mSpeechEnabled.GetCheck () ? TRUE : FALSE);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageOutput::OnBalloonEnabled()
{
	SetModified ();
	EnableControls ();
}

void CPropPageOutput::OnBalloonFontDefault()
{
	if	(mBalloonConfig.mFont)
	{
		mBalloonConfig.mLastFont = mBalloonConfig.mFont.Detach();
	}
	if	(mBalloonConfig.mFgColor)
	{
		mBalloonConfig.mLastFgColor = mBalloonConfig.mFgColor.Detach();
	}
	SetModified ();
	EnableControls ();
}

void CPropPageOutput::OnBalloonFontCustom()
{
	if	(!mBalloonConfig.mFont)
	{
		mBalloonConfig.mFont = mBalloonConfig.mLastFont.Detach();
	}
	if	(!mBalloonConfig.mFgColor)
	{
		mBalloonConfig.mFgColor = mBalloonConfig.mLastFgColor.Detach();
	}
	SetModified ();
	EnableControls ();
}

void CPropPageOutput::OnBalloonFontChoose()
{
	CFontDialog	lFontDialog;

	if	(mBalloonConfig.mFont)
	{
		memcpy (&lFontDialog.m_lf, mBalloonConfig.mFont.Ptr(), sizeof (LOGFONT));
		lFontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	}
	else
	if	(mBalloonConfig.mLastFont)
	{
		memcpy (&lFontDialog.m_lf, mBalloonConfig.mLastFont.Ptr(), sizeof (LOGFONT));
		lFontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	}

	if	(mBalloonConfig.mFgColor)
	{
		lFontDialog.m_cf.rgbColors = *mBalloonConfig.mFgColor;
	}
	else
	if	(mBalloonConfig.mLastFgColor)
	{
		lFontDialog.m_cf.rgbColors = *mBalloonConfig.mLastFgColor;
	}
	else
	{
		lFontDialog.m_cf.rgbColors = GetSysColor (COLOR_WINDOW);
	}

	if	(lFontDialog.DoModal () == IDOK)
	{
		mBalloonConfig.mFont = new LOGFONT;
		memcpy (mBalloonConfig.mFont.Ptr(), &lFontDialog.m_lf, sizeof (LOGFONT));
		mBalloonConfig.mFgColor = new COLORREF;
		(*mBalloonConfig.mFgColor) = lFontDialog.GetColor ();

		SetModified ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageOutput::OnSpeechEnabled()
{
	SetModified ();
	EnableControls ();
}

void CPropPageOutput::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	if	(
			(nSBCode == SB_ENDSCROLL)
		&&	(pScrollBar->GetSafeHwnd() == mSpeechSpeed.m_hWnd)
		)
	{
		SetModified ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageOutput::OnSoundEnabled()
{
	SetModified ();
}
