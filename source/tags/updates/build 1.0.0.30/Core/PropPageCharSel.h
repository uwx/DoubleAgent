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
#ifndef PROPPAGECHARSEL_H_INCLUDED_
#define PROPPAGECHARSEL_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "DaCoreRes.h"
#include "PropPageFix.h"
#include "AgentFiles.h"
#include "AgentPreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CPropPageCharSel : public CPropertyPage
{
protected:
	CPropPageCharSel();
public:
	virtual ~CPropPageCharSel();
	DECLARE_DYNCREATE(CPropPageCharSel)

// Dialog Data
	//{{AFX_DATA(CPropPageCharSel)
	enum { IDD = IDD_PROPPAGE_CHARSEL };
	CButton	mNextButton;
	CButton	mBackButton;
	CStatic	mCharPreview;
	CStatic	mCharTitle;
	CStatic	mCharNameTitle;
	CStatic	mCharName;
	CStatic	mCharDesc;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CPropPageCharSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPageCharSel)
	afx_msg void OnNext();
	afx_msg void OnBack();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ShowCharacter ();
	void StartCharacter ();
	void StopCharacter ();

protected:
	CPropPageFix			mPropPageFix;
	CAgentFiles				mFiles;
	int						mFileNdx;
	tPtr <CAgentPreviewWnd>	mPreviewWnd;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPPAGECHARSEL_H_INCLUDED_
