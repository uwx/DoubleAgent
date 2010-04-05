#ifndef REPLAYACTIONSDLG_H_INCLUDED_
#define REPLAYACTIONSDLG_H_INCLUDED_
#pragma once

#include "DaServerOdl.h"
#include "ActionList.h"
#include "ActionFile.h"

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrBalloon, __uuidof(IDaSvrBalloon));

/////////////////////////////////////////////////////////////////////////////

class CReplayActionsDlg : public CDialog
{
// Construction
public:
	CReplayActionsDlg (CWnd* pParent = NULL);
	~CReplayActionsDlg ();

// Dialog Data
	//{{AFX_DATA(CReplayActionsDlg)
	enum { IDD = IDD_REPLAYACTIONS_DIALOG };
	CButton	mStopButton;
	CButton	mStartButton;
	CButton	mRepeatButton;
	CButton	mOpenButton;
	CActionList	mActionList;
	//}}AFX_DATA

// Attributes
public:
	CString	mDefaultFileName;

	//{{AFX_VIRTUAL(CReplayActionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CReplayActionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOpenFile();
	afx_msg void OnStartReplay();
	afx_msg void OnStopReplay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ShowState ();
	bool Start ();
	bool Restart ();
	bool RunActions ();
	HRESULT RunAction (CActionLine * pAction);
	HRESULT LoadCharacter (LPCTSTR pCharacterName);
	HRESULT UnloadCharacter ();
	void LoadConfig ();
	void SaveConfig ();

protected:
	tPtr <CActionFile>				mActionFile;
	IDaServerPtr					mServer;
	IDaSvrCharacterPtr				mCharacter;
	long							mCharacterId;
	UINT_PTR						mPlayTimer;
	tPtr <CFileTimeSpan>			mStartTimeOffset;
	CMap <long, long, long, long>	mReqIdMap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

/////////////////////////////////////////////////////////////////////////////

#endif // REPLAYACTIONSDLG_H_INCLUDED_
