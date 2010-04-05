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
#ifndef PROPPAGEOUTPUT_H_INCLUDED_
#define PROPPAGEOUTPUT_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "PropPageFix.h"
#include "DaGlobalConfig.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CPropPageOutput : public CPropertyPage
{
protected:
	CPropPageOutput();
public:
	virtual ~CPropPageOutput();
	DECLARE_DYNCREATE(CPropPageOutput)

// Dialog Data
	//{{AFX_DATA(CPropPageOutput)
	enum { IDD = IDD_PROPPAGE_OUTPUT };
	CStatic	mSpeechSpeedSlow;
	CStatic	mSpeechSpeedFast;
	CStatic	mSpeechSpeedDefault;
	CButton	mBalloonFontCustom;
	CStatic	mSpeechSpeedTitle;
	CSliderCtrl	mSpeechSpeed;
	CButton	mSpeechEnabled;
	CButton	mSoundEnabled;
	CButton	mBalloonFontDefault;
	CButton	mBalloonEnabled;
	CButton	mBalloonFontChoose;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CPropPageOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPageOutput)
	afx_msg void OnBalloonEnabled();
	afx_msg void OnBalloonFontDefault();
	afx_msg void OnBalloonFontCustom();
	afx_msg void OnBalloonFontChoose();
	afx_msg void OnSpeechEnabled();
	afx_msg void OnSoundEnabled();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void EnableControls ();

protected:
	CPropPageFix			mPropPageFix;
	CDaBalloonConfig		mBalloonConfig;
	CDaAudioOutputConfig	mAudioConfig;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPPAGEOUTPUT_H_INCLUDED_
