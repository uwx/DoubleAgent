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
#ifndef PROPPAGECOPYRIGHT_H_INCLUDED_
#define PROPPAGECOPYRIGHT_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "PropPageFix.h"
#include "FormLayout.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CPropPageCopyright : public CPropertyPage, protected CFormLayout
{
protected:
	CPropPageCopyright();
public:
	virtual ~CPropPageCopyright();
	DECLARE_DYNCREATE(CPropPageCopyright)

// Dialog Data
	//{{AFX_DATA(CPropPageCopyright)
	enum { IDD = IDD_PROPPAGE_COPYRIGHT };
	CStatic	mVersionTitle;
	CStatic	mProductVersion;
	CStatic	mProductName;
	CStatic	mCopyright;
	CStatic	mIcon;
	CStatic	mMaVersionTitle;
	CStatic	mMaProductVersion;
	CStatic	mMaProductName;
	CStatic	mMaIcon;
	CStatic	mMaCopyright;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CPropPageCopyright)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPageCopyright)
	afx_msg void OnLinkClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitFonts ();
	void ShowDaVersion ();
	void ShowMaVersion ();

protected:
	CPropPageFix	mPropPageFix;
	CFont			mLargeFont;

protected:
	class CLicenseDlg : public CDialog
	{
	public:
		CLicenseDlg (CWnd * pParentWnd);
		virtual BOOL OnInitDialog ();
		class CHtmlView * mLicenseText;
		CString mLicenseURL;
	};
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPPAGECOPYRIGHT_H_INCLUDED_
