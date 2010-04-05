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
#ifndef PROPSHEETBASE_H_INCLUDED_
#define PROPSHEETBASE_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////
//
//	We're using this instead of MFC CPropSheet because it has too much
//	unwanted crap.  For example, it applies the property sheet font
//	to its pages, and we want the opposite.
//
/////////////////////////////////////////////////////////////////////////////

class CPropSheetBase : public CDialog
{
public:
	CPropSheetBase (UINT pCaptionId, CWnd* pParentWnd = NULL);
	virtual ~CPropSheetBase();
	DECLARE_DYNAMIC(CPropSheetBase)

// Attributes
public:

// Operations
public:
	bool Create ();
	INT_PTR DoModal();

// Overrides
	//{{AFX_VIRTUAL(CPropSheetBase)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropSheetBase)
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnApply();
	afx_msg void OnSysCommand(UINT nID, LPARAM);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static int CALLBACK PropSheetCallbackModeless (HWND hwnd, UINT message, LPARAM lParam);
	static int CALLBACK PropSheetCallbackModal (HWND hwnd, UINT message, LPARAM lParam);

	virtual void LoadConfig ();
	virtual void SaveConfig (int pSheetResult);
	virtual void OnChanged ();
	virtual void OnApplied ();

protected:
	CString								mCaption;
	tS <PROPSHEETHEADER_MFC_VER>		mPsh;
	tArrayPtr <PROPSHEETPAGE_MFC_VER>	mPsp;
	COwnPtrArray <CPropertyPage>		mPages;
	bool								mPageChanged;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPSHEETBASE_H_INCLUDED_
