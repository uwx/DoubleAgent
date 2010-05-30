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
#include "PropertyPage.h"
#include "LayoutTools.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CPropPageCopyright : public CAtlPropertyPage, protected CLayoutTools
{
	DECLARE_DLL_OBJECT(CPropPageCopyright)
protected:
	CPropPageCopyright();
public:
	virtual ~CPropPageCopyright();
	static CPropPageCopyright * CreateInstance ();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_COPYRIGHT };
	CContainedWindow	mVersionTitle;
	CContainedWindow	mProductVersion;
	CContainedWindow	mProductName;
	CContainedWindow	mCopyright;
	CContainedWindow	mIconControl;
	CContainedWindow	mMaVersionTitle;
	CContainedWindow	mMaProductVersion;
	CContainedWindow	mMaProductName;
	CContainedWindow	mMaIcon;
	CContainedWindow	mMaCopyright;

// Overrides
protected:
	virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnDrawIcon (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnLinkClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BEGIN_MSG_MAP(CPropPageCopyright)
		MESSAGE_HANDLER (WM_DRAWITEM, OnDrawIcon)
		NOTIFY_HANDLER (IDC_PROPPAGE_CPR_LICENSELINK, NM_CLICK, OnLinkClick)
		NOTIFY_HANDLER (IDC_PROPPAGE_CPR_REFLINK, NM_CLICK, OnLinkClick)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void InitFonts ();
	void ShowDaVersion ();
	void ShowMaVersion ();

protected:
	CFontHandle	mLargeFont;
	CIconHandle	mIcon;

protected:
	class CLicenseDlg : public CDialogImpl <CLicenseDlg>
	{
	public:
		enum { IDD = IDD_LICENSE };

		BEGIN_MSG_MAP(CLicenseDlg)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_CLOSE, OnClose)
			COMMAND_HANDLER(IDOK, BN_CLICKED, OnOk)
		END_MSG_MAP()

		LRESULT OnInitDialog (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
		LRESULT OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
		LRESULT OnOk (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

		CAxWindow mLicenseText;
		CAtlString mLicenseURL;
	};
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
