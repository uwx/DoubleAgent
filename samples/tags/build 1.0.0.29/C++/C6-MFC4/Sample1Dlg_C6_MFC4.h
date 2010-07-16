//{{AFX_INCLUDES()
#include "MsAgentCtl.h"
#include "MsAgentSvr.h"
#include "DoubleAgentCtl.h"
#include "DoubleAgentSvr.h"
//}}AFX_INCLUDES
#if !defined(AFX_SAMPLE1DLG_H__4BE466E2_AE80_4451_B114_04CFB0C6A302__INCLUDED_)
#define AFX_SAMPLE1DLG_H__4BE466E2_AE80_4451_B114_04CFB0C6A302__INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CSample1Dlg : public CDialog
{
public:
	CSample1Dlg(CWnd* pParent = NULL);
	~CSample1Dlg();

// Dialog Data
	//{{AFX_DATA(CSample1Dlg)
	enum { IDD = IDD_SAMPLE1_DIALOG };
	CButton	mUnloadMsServerChar;
	CButton	mUnloadMsControlChar;
	CButton	mUnloadDaServerChar;
	CButton	mUnloadDaControlChar;
	CButton	mShowMsServerOptions;
	CButton	mShowMsServerChars;
	CButton	mShowMsServerChar;
	CButton	mShowMsControlOptions;
	CButton	mShowMsControlChars;
	CButton	mShowMsControlChar;
	CButton	mShowDaServerOptions;
	CButton	mShowDaServerChars;
	CButton	mShowDaServerChar;
	CButton	mShowDaControlOptions;
	CButton	mShowDaControlChars;
	CButton	mShowDaControlChar;
	CButton	mLoadMsServerChar;
	CButton	mLoadMsControlChar;
	CButton	mLoadDaServerChar;
	CButton	mLoadDaControlChar;
	CButton	mHideMsServerChar;
	CButton	mHideMsControlChar;
	CButton	mHideDaServerChar;
	CButton	mHideDaControlChar;
	CButton	mShowCharMs;
	CButton	mShowCharDa;
	IAgentCtlEx	mMsControl;
	IDaControl	mDaControl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSample1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CSample1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowControlMs(LPCTSTR CharacterID, short Cause);
	afx_msg void OnHideControlMs(LPCTSTR CharacterID, short Cause);
	afx_msg void OnShowControlDa(LPCTSTR CharacterID, short Cause);
	afx_msg void OnHideControlDa(LPCTSTR CharacterID, short Cause);
	afx_msg void OnLoadMsServerChar();
	afx_msg void OnUnloadMsServerChar();
	afx_msg void OnShowMsServerChar();
	afx_msg void OnHideMsServerChar();
	afx_msg void OnShowMsServerOptions();
	afx_msg void OnShowMsServerChars();
	afx_msg void OnLoadMsControlChar();
	afx_msg void OnUnloadMsControlChar();
	afx_msg void OnShowMsControlChar();
	afx_msg void OnHideMsControlChar();
	afx_msg void OnShowMsControlOptions();
	afx_msg void OnShowMsControlChars();
	afx_msg void OnLoadDaServerChar();
	afx_msg void OnUnloadDaServerChar();
	afx_msg void OnShowDaServerChar();
	afx_msg void OnHideDaServerChar();
	afx_msg void OnShowDaServerOptions();
	afx_msg void OnShowDaServerChars();
	afx_msg void OnLoadDaControlChar();
	afx_msg void OnUnloadDaControlChar();
	afx_msg void OnShowDaControlChar();
	afx_msg void OnHideDaControlChar();
	afx_msg void OnShowDaControlOptions();
	afx_msg void OnShowDaControlChars();
	afx_msg void OnAppAbout();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	const CString			mDaCharacterFile;
	const CString			mDaCharacterName;
	const CString			mMsCharacterFile;
	const CString			mMsCharacterName;
	IAgentCtlCharacterEx	mMsControlChar;
	IDaCtlCharacter			mDaControlChar;
	IAgentEx				mMsServer;
	IAgentCharacterEx		mMsServerChar;
	long					mMsServerCharId;
	IDaServer				mDaServer;
	IDaSvrCharacter			mDaServerChar;
	long					mDaServerCharId;

	void SetDaServerButtons();
	void SetDaControlButtons();
	void SetMsServerButtons();
	void SetMsControlButtons();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLE1DLG_H__4BE466E2_AE80_4451_B114_04CFB0C6A302__INCLUDED_)
