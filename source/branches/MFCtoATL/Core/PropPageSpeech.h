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

class _DACORE_IMPEXP CPropPageSpeech : public CAtlPropertyPage
{
	DECLARE_DLL_OBJECT(CPropPageSpeech)
protected:
	CPropPageSpeech();
public:
	virtual ~CPropPageSpeech();
	static CPropPageSpeech * CreateInstance ();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SPEECH };
	CContainedWindow	mSpeechTipsEnabled;
	CContainedWindow	mSpeechPromptEnabled;
	CContainedWindow	mHotKeyTitle4;
	CContainedWindow	mHotKeyTitle3;
	CContainedWindow	mHotKeyTitle2;
	CContainedWindow	mHotKeyTitle1;
	CContainedWindow	mHotKeyDelay;
	CContainedWindow	mHotKeyDelaySpin;
	CContainedWindow	mHotKey;
	CContainedWindow	mSpeechEnabled;

// Overrides
protected:
	virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpeechEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnSpeechPrompt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnSpeechTips(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnHotKeyChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnHotKeyDelayChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CPropPageSpeech)
		NOTIFY_CODE_HANDLER(PSN_APPLY, OnApply)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_ENABLED, BN_CLICKED, OnSpeechEnabled)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_PROMPT, BN_CLICKED, OnSpeechPrompt)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_TIPS, BN_CLICKED, OnSpeechTips)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_HOTKEY, EN_CHANGE, OnHotKeyChange)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_HOTKEY_DELAY, EN_CHANGE, OnHotKeyDelayChange)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void EnableControls ();

protected:
	CGlobalHandle			mTemplate;
	CDaSpeechInputConfig	mSpeechConfig;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
