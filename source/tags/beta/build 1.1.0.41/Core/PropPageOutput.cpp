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
#include <commdlg.h>
#include "DaCore.h"
#include "PropPageOutput.h"
#include "Localize.h"

#pragma comment (lib,"comdlg32.lib")
#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CPropPageOutput)

CPropPageOutput::CPropPageOutput()
:	CAtlPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageOutput::CPropPageOutput"), this);
#endif
	if	(CLocalize::LoadDialog (mTemplate, IDD))
	{
		mPsp.pResource = (PROPSHEETPAGE_RESOURCE)GlobalLock (mTemplate);
		mPsp.hInstance = NULL;
		mPsp.dwFlags |= PSP_DLGINDIRECT;
	}
}

CPropPageOutput::~CPropPageOutput()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageOutput::~CPropPageOutput"), this);
#endif
}

CPropPageOutput * CPropPageOutput::CreateInstance ()
{
	return new CPropPageOutput;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageOutput::OnInitDialog ()
{
	HWND	lWnd;

	mBalloonFontCustom.Attach	(GetDlgItem (IDC_PROPPAGE_BALLOON_FONT_CUSTOM));
	mTtsSpeed.Attach			(GetDlgItem (IDC_PROPPAGE_TTS_SPEED));
	mTtsEnabled.Attach		(GetDlgItem (IDC_PROPPAGE_TTS_ENABLED));
	mSoundEnabled.Attach		(GetDlgItem (IDC_PROPPAGE_SOUND_ENABLED));
	mBalloonFontDefault.Attach	(GetDlgItem (IDC_PROPPAGE_BALLOON_FONT_DEFAULT));
	mBalloonEnabled.Attach		(GetDlgItem (IDC_PROPPAGE_BALLOON_ENABLED));
	mBalloonFontChoose.Attach	(GetDlgItem (IDC_PROPPAGE_BALLOON_FONT_CHOOSE));

//
//KLUDGE - for handling resources where these controls are IDC_STATIC
//
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_SLOW))
	{
		mTtsSpeedSlow.Attach (GetDlgItem (IDC_PROPPAGE_TTS_SPEED_SLOW));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_FAST))
	{
		mTtsSpeedFast.Attach (GetDlgItem (IDC_PROPPAGE_TTS_SPEED_FAST));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_TITLE))
	{
		mTtsSpeedTitle.Attach (GetDlgItem (IDC_PROPPAGE_TTS_SPEED_TITLE));
	}
	if	(::GetDlgItem (m_hWnd, IDC_PROPPAGE_TTS_SPEED_DEFAULT))
	{
		mTtsSpeedDefault.Attach (GetDlgItem (IDC_PROPPAGE_TTS_SPEED_DEFAULT));
	}

	for	(lWnd = ::GetWindow (mTtsEnabled.m_hWnd, GW_HWNDNEXT); lWnd; lWnd = ::GetWindow (lWnd, GW_HWNDNEXT))
	{
		if	((short)::GetDlgCtrlID (lWnd) == IDC_STATIC)
		{
			if	(!mTtsSpeedTitle.m_hWnd)
			{
				mTtsSpeedTitle.Attach (lWnd);
			}
			else
			if	(!mTtsSpeedSlow.m_hWnd)
			{
				mTtsSpeedSlow.Attach (lWnd);
			}
			else
			if	(!mTtsSpeedFast.m_hWnd)
			{
				mTtsSpeedFast.Attach (lWnd);
			}
			else
			if	(!mTtsSpeedDefault.m_hWnd)
			{
				mTtsSpeedDefault.Attach (lWnd);
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
	mBalloonConfig.LoadConfig ();
	mSettingsConfig.LoadConfig ();

	Button_SetCheck (mBalloonEnabled, mBalloonConfig.mEnabled ? TRUE : FALSE);
	Button_SetCheck (mBalloonFontDefault, mBalloonConfig.mFont.Ptr() ? FALSE : TRUE);
	Button_SetCheck (mBalloonFontCustom, mBalloonConfig.mFont.Ptr() ? TRUE : FALSE);
	Button_SetCheck (mTtsEnabled, mSettingsConfig.mTtsEnabled ? TRUE : FALSE);
	mTtsSpeed.SendMessage (TBM_SETRANGE, TRUE, MAKELPARAM(mSettingsConfig.mTtsSpeedMin, mSettingsConfig.mTtsSpeedMax));
	mTtsSpeed.SendMessage (TBM_SETPOS, TRUE, (LPARAM)mSettingsConfig.mTtsSpeed);
	Button_SetCheck (mSoundEnabled, mSettingsConfig.mEffectsEnabled ? TRUE : FALSE);

	EnableControls ();
	return TRUE;
}

LRESULT CPropPageOutput::OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	mBalloonConfig.mEnabled = Button_GetCheck (mBalloonEnabled) ? true : false;
	mSettingsConfig.mTtsEnabled = Button_GetCheck (mTtsEnabled) ? true : false;
	mSettingsConfig.mTtsSpeed = (USHORT) mTtsSpeed.SendMessage (TBM_GETPOS);
	mSettingsConfig.mEffectsEnabled = Button_GetCheck (mSoundEnabled) ? true : false;

	mBalloonConfig.SaveConfig ();
	mSettingsConfig.SaveConfig ();

	return 0;
}

void CPropPageOutput::EnableControls ()
{
	mBalloonFontDefault.EnableWindow (Button_GetCheck (mBalloonEnabled) ? TRUE : FALSE);
	mBalloonFontCustom.EnableWindow (Button_GetCheck (mBalloonEnabled) ? TRUE : FALSE);
	mBalloonFontChoose.EnableWindow ((Button_GetCheck (mBalloonEnabled) && Button_GetCheck (mBalloonFontCustom)) ? TRUE : FALSE);

	mTtsSpeed.EnableWindow (Button_GetCheck (mTtsEnabled) ? TRUE : FALSE);
	mTtsSpeedSlow.EnableWindow (Button_GetCheck (mTtsEnabled) ? TRUE : FALSE);
	mTtsSpeedTitle.EnableWindow (Button_GetCheck (mTtsEnabled) ? TRUE : FALSE);
	mTtsSpeedFast.EnableWindow (Button_GetCheck (mTtsEnabled) ? TRUE : FALSE);
	mTtsSpeedDefault.EnableWindow (Button_GetCheck (mTtsEnabled) ? TRUE : FALSE);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageOutput::OnBalloonEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageOutput::OnBalloonFontDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
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
	return 0;
}

LRESULT CPropPageOutput::OnBalloonFontCustom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
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
	return 0;
}

LRESULT CPropPageOutput::OnBalloonFontChoose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	tSS <CHOOSEFONT, DWORD>	lChooseFont;
	tPtr <LOGFONT>			lFont;

	lChooseFont.hwndOwner = m_hWnd;
	lChooseFont.Flags = CF_EFFECTS|CF_SCREENFONTS|CF_NOVECTORFONTS|CF_NOVERTFONTS;

	if	(mBalloonConfig.mFont)
	{
		lFont = new tS <LOGFONT> (*mBalloonConfig.mFont);
		lChooseFont.Flags |= CF_INITTOLOGFONTSTRUCT;
	}
	else
	if	(mBalloonConfig.mLastFont)
	{
		lFont = new tS <LOGFONT> (*mBalloonConfig.mLastFont);
		lChooseFont.Flags |= CF_INITTOLOGFONTSTRUCT;
	}
	else
	{
		lFont = new tS<LOGFONT>;
	}
	lChooseFont.lpLogFont = lFont;

	if	(mBalloonConfig.mFgColor)
	{
		lChooseFont.rgbColors = *mBalloonConfig.mFgColor;
	}
	else
	if	(mBalloonConfig.mLastFgColor)
	{
		lChooseFont.rgbColors = *mBalloonConfig.mLastFgColor;
	}
	else
	{
		lChooseFont.rgbColors = GetSysColor (COLOR_WINDOW);
	}

	if	(ChooseFont (&lChooseFont))
	{
		if	(lFont)
		{
			mBalloonConfig.mFont = new tS <LOGFONT> (*lFont);
		}
		mBalloonConfig.mFgColor = new COLORREF;
		(*mBalloonConfig.mFgColor) = lChooseFont.rgbColors;

		SetModified ();
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageOutput::OnTtsEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	EnableControls ();
	return 0;
}

LRESULT CPropPageOutput::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	(
			(wParam == SB_ENDSCROLL)
		&&	((HWND)lParam == mTtsSpeed.m_hWnd)
		)
	{
		SetModified ();
	}

	bHandled = CAtlPropertyPage::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageOutput::OnSoundEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetModified ();
	return 0;
}
