// Sample1Dlg.h : header file
//

#pragma once

#include <comutil.h>
#include "CAgentEx.h" 
#include "CAgentCharacterEx.h"
#include "CAgentCtlEx.h"
#include "CAgentCtlCharacterEx.h"
#include "CDaServer.h" 
#include "CDaSvrCharacter.h"
#include "CDaControl.h"
#include "CDaCtlCharacter.h"
#include "afxwin.h"

// CSample1Dlg dialog
class CSample1Dlg : public CDialog
{
// Construction
public:
	CSample1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SAMPLE1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	const CString			mDaCharacterFile;
	const CString			mDaCharacterName;
	const CString			mMsCharacterFile;
	const CString			mMsCharacterName;
	CAgentCtlCharacterEx	mMsControlChar;
	CDaCtlCharacter			mDaControlChar;
	CAgentEx				mMsServer;
	CAgentCharacterEx		mMsServerChar;
	long					mMsServerCharId;
	CDaServer				mDaServer;
	CDaSvrCharacter			mDaServerChar;
	long					mDaServerCharId;
	
	virtual BOOL OnInitDialog();
	void SetMsControlButtons(void);
	void SetMsServerButtons(void);
	void SetDaControlButtons(void);
	void SetDaServerButtons(void);

	void OnShowMsChar(LPCTSTR CharacterID, short Cause);
	void OnHideMsChar(LPCTSTR CharacterID, short Cause);
	void OnShowDaChar(LPCTSTR CharacterID, short Cause);
	void OnHideDaChar(LPCTSTR CharacterID, short Cause);

	afx_msg void OnBnClickedLoadMsServerChar();
	afx_msg void OnBnClickedUnloadMsServerChar();
	afx_msg void OnBnClickedShowMsServerChar();
	afx_msg void OnBnClickedHideMsServerChar();
	afx_msg void OnBnClickedShowMsServerOptions();
	afx_msg void OnBnClickedShowMsServerChars();
	afx_msg void OnBnClickedLoadMsControlChar();
	afx_msg void OnBnClickedUnloadMsControlChar();
	afx_msg void OnBnClickedShowMsControlChar();
	afx_msg void OnBnClickedHideMsControlChar();
	afx_msg void OnBnClickedShowMsControlOptions();
	afx_msg void OnBnClickedShowMsControlChars();
	afx_msg void OnBnClickedLoadDaServerChar();
	afx_msg void OnBnClickedUnloadDaServerChar();
	afx_msg void OnBnClickedShowDaServerChar();
	afx_msg void OnBnClickedHideDaServerChar();
	afx_msg void OnBnClickedShowDaServerOptions();
	afx_msg void OnBnClickedShowDaServerChars();
	afx_msg void OnBnClickedLoadDaControlChar();
	afx_msg void OnBnClickedUnloadDaControlChar();
	afx_msg void OnBnClickedShowDaControlChar();
	afx_msg void OnBnClickedHideDaControlChar();
	afx_msg void OnBnClickedShowDaControlOptions();
	afx_msg void OnBnClickedShowDaControlChars();

public:
	CButton mLoadMsServerChar;
	CButton mUnloadMsServerChar;
	CButton mShowMsServerChar;
	CButton mHideMsServerChar;
	CButton mShowMsServerOptions;
	CButton mShowMsServerChars;
	CButton mLoadDaServerChar;
	CButton mUnloadDaServerChar;
	CButton mShowDaServerChar;
	CButton mHideDaServerChar;
	CButton mShowDaServerOptions;
	CButton mShowDaServerChars;
	CButton mLoadMsControlChar;
	CButton mUnloadMsControlChar;
	CButton mShowMsControlChar;
	CButton mHideMsControlChar;
	CButton mShowMsControlOptions;
	CButton mShowMsControlChars;
	CButton mLoadDaControlChar;
	CButton mUnloadDaControlChar;
	CButton mShowDaControlChar;
	CButton mHideDaControlChar;
	CButton mShowDaControlOptions;
	CButton mShowDaControlChars;
	CDaControl mDaControl;
	CAgentCtlEx mMsControl;
};
