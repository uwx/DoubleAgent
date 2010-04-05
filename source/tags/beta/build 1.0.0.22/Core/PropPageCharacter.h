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
#ifndef PROPPAGECHARACTER_H_INCLUDED_
#define PROPPAGECHARACTER_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "DaCoreRes.h"
#include "PropPageFix.h"
#include "FormLayout.h"
#include "AgentFile.h"
#include "AgentPreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CPropPageCharacter : public CPropertyPage, protected CFormLayout
{
protected:
	CPropPageCharacter();
public:
	virtual ~CPropPageCharacter();
	DECLARE_DYNCREATE(CPropPageCharacter)

// Dialog Data
	//{{AFX_DATA(CPropPageCharacter)
	enum { IDD = IDD_PROPPAGE_CHARACTER };
	CEdit	mCharTtsModeID;
	CEdit	mCharStandard;
	CStatic	mCharPreview;
	CStatic	mCharName;
	CStatic	mCharInvalid;
	CEdit	mCharGuid;
	CEdit	mCharFileVer;
	CStatic	mCharDesc;
	//}}AFX_DATA

// Attributes
	tPtr <CAgentFile>	mAgentFile;

// Operations
	HRESULT AddSheetPage (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam);

// Overrides
	//{{AFX_VIRTUAL(CPropPageCharacter)
	protected:
	virtual void OnFinalRelease();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPageCharacter)
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static UINT CALLBACK PropPageCallback (HWND pWnd, UINT pMsgId, LPPROPSHEETPAGE pPropPage);

	void NoCharacter ();
	void ShowCharacter ();
	void ShowPreview ();
	void StartPreview ();
	void StopPreview ();

protected:
	CPropPageFix			mPropPageFix;
	tPtr <CAgentPreviewWnd>	mPreviewWnd;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPPAGECHARACTER_H_INCLUDED_
