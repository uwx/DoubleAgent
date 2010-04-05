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
#ifndef PROPPAGESPEECH_H_INCLUDED_
#define PROPPAGESPEECH_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "PropPageFix.h"
#include "DaGlobalConfig.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CPropPageSpeech : public CPropertyPage
{
protected:
	CPropPageSpeech();
public:
	virtual ~CPropPageSpeech();
	DECLARE_DYNCREATE(CPropPageSpeech)

// Dialog Data
	//{{AFX_DATA(CPropPageSpeech)
	enum { IDD = IDD_PROPPAGE_SPEECH };
	CButton	mSpeechTipsEnabled;
	CButton	mSpeechPromptEnabled;
	CStatic	mHotKeyTitle4;
	CStatic	mHotKeyTitle3;
	CStatic	mHotKeyTitle2;
	CStatic	mHotKeyTitle1;
	CEdit	mHotKeyDelay;
	CSpinButtonCtrl	mHotKeySpin;
	CHotKeyCtrl	mHotKey;
	CButton	mSpeechEnabled;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CPropPageSpeech)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPageSpeech)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSpeechEnabled();
	afx_msg void OnSpeechPrompt();
	afx_msg void OnSpeechTips();
	afx_msg void OnHotKeyChange();
	afx_msg void OnHotKeyDelayChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void EnableControls ();

protected:
	CPropPageFix			mPropPageFix;
	CDaSpeechInputConfig	mSpeechConfig;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPPAGESPEECH_H_INCLUDED_
