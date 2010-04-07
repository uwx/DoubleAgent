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
#include "PropertyPage.h"
#include "AgentFiles.h"
#include "AgentPreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CPropPageCharSel : public CAtlPropertyPage
{
	DECLARE_DLL_OBJECT(CPropPageCharSel)
protected:
	CPropPageCharSel();
public:
	virtual ~CPropPageCharSel();
	static CPropPageCharSel * CreateInstance ();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CHARSEL };
	CContainedWindow	mNextButton;
	CContainedWindow	mBackButton;
	CContainedWindow	mCharPreview;
	CContainedWindow	mCharTitle;
	CContainedWindow	mCharNameTitle;
	CContainedWindow	mCharName;
	CContainedWindow	mCharDesc;

// Overrides
protected:
	virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

	BEGIN_MSG_MAP(CPropPageSpeech)
		NOTIFY_CODE_HANDLER(PSN_APPLY, OnApply)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
		COMMAND_HANDLER(IDC_PROPPAGE_CHARSEL_NEXT, BN_CLICKED, OnNext)
		COMMAND_HANDLER(IDC_PROPPAGE_CHARSEL_BACK, BN_CLICKED, OnBack)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void ShowCharacter ();
	void StartCharacter ();
	void StopCharacter ();

protected:
	CGlobalHandle			mTemplate;
	CAgentFiles				mFiles;
	int						mFileNdx;
	tPtr <CAgentPreviewWnd>	mPreviewWnd;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
