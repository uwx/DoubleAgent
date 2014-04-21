/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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

class CPropPageSpeech : public CAtlPropertyPage
{
	DECLARE_DLL_OBJECT_EX(CPropPageSpeech, _DACORE_IMPEXP)
protected:
	CPropPageSpeech();
public:
	_DACORE_IMPEXP virtual ~CPropPageSpeech();
	_DACORE_IMPEXP static CPropPageSpeech * CreateInstance ();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SPEECH };
	CContainedWindow	mSrTipsEnabled;
	CContainedWindow	mSrPromptEnabled;
	CContainedWindow	mSrHotKeyTitle4;
	CContainedWindow	mSrHotKeyTitle3;
	CContainedWindow	mSrHotKeyTitle2;
	CContainedWindow	mSrHotKeyTitle1;
	CContainedWindow	mSrHotKeyDelay;
	CContainedWindow	mSrHotKeyDelaySpin;
	CContainedWindow	mSrHotKey;
	CContainedWindow	mSrEnabled;

// Overrides
protected:
	_DACORE_IMPEXP virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSrEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSrPrompt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSrTips(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSrHotKeyChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSrHotKeyDelayChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CPropPageSpeech)
		NOTIFY_CODE_HANDLER(PSN_APPLY, OnApply)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_ENABLED, BN_CLICKED, OnSrEnabled)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_PROMPT, BN_CLICKED, OnSrPrompt)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_TIPS, BN_CLICKED, OnSrTips)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_HOTKEY, EN_CHANGE, OnSrHotKeyChange)
		COMMAND_HANDLER(IDC_PROPPAGE_SR_HOTKEY_DELAY, EN_CHANGE, OnSrHotKeyDelayChange)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void EnableControls ();

protected:
	CGlobalHandle		mTemplate;
	CDaSettingsConfig	mSettingsConfig;
};

/////////////////////////////////////////////////////////////////////////////
