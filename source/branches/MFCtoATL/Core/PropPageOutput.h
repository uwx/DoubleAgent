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
#pragma once
#include "DaCoreExp.h"
#include "DaCoreRes.h"
#include "DaGlobalConfig.h"
#include "PropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CPropPageOutput : public CAtlPropertyPage
{
	DECLARE_DLL_OBJECT(CPropPageOutput)
protected:
	CPropPageOutput();
public:
	virtual ~CPropPageOutput();
	static CPropPageOutput * CreateInstance ();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_OUTPUT };
	CContainedWindow	mSpeechSpeedSlow;
	CContainedWindow	mSpeechSpeedFast;
	CContainedWindow	mSpeechSpeedDefault;
	CContainedWindow	mBalloonFontCustom;
	CContainedWindow	mSpeechSpeedTitle;
	CContainedWindow	mSpeechSpeed;
	CContainedWindow	mSpeechEnabled;
	CContainedWindow	mSoundEnabled;
	CContainedWindow	mBalloonFontDefault;
	CContainedWindow	mBalloonEnabled;
	CContainedWindow	mBalloonFontChoose;

// Overrides
protected:
	virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnBalloonEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnBalloonFontDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnBalloonFontCustom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnBalloonFontChoose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnSpeechEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnSoundEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnHScroll (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CPropPageOutput)
		NOTIFY_CODE_HANDLER(PSN_APPLY, OnApply)
		COMMAND_HANDLER(IDC_PROPPAGE_BALLOON_ENABLED, BN_CLICKED, OnBalloonEnabled)
		COMMAND_HANDLER(IDC_PROPPAGE_BALLOON_FONT_DEFAULT, BN_CLICKED, OnBalloonFontDefault)
		COMMAND_HANDLER(IDC_PROPPAGE_BALLOON_FONT_CUSTOM, BN_CLICKED, OnBalloonFontCustom)
		COMMAND_HANDLER(IDC_PROPPAGE_BALLOON_FONT_CHOOSE, BN_CLICKED, OnBalloonFontChoose)
		COMMAND_HANDLER(IDC_PROPPAGE_TTS_ENABLED, BN_CLICKED, OnSpeechEnabled)
		COMMAND_HANDLER(IDC_PROPPAGE_SOUND_ENABLED, BN_CLICKED, OnSoundEnabled)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void EnableControls ();

protected:
	CGlobalHandle			mTemplate;
	CDaBalloonConfig		mBalloonConfig;
	CDaAudioOutputConfig	mAudioConfig;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
