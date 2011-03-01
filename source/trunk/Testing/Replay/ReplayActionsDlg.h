#pragma once
#include "DaServerOdl.h"
#include "ActionList.h"
#include "ActionFile.h"
#include "FormLayout.h"

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrBalloon2, __uuidof(IDaSvrBalloon2));

/////////////////////////////////////////////////////////////////////////////

class CReplayActionsDlg : public CDialog, protected CFormLayout
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID);
	afx_msg void OnOpenFile();
	afx_msg void OnStartReplay();
	afx_msg void OnStopReplay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void RecalcLayout ();
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
	IDaServer2Ptr					mServer;
	IDaSvrCharacter2Ptr				mCharacter;
	long							mCharacterId;
	UINT_PTR						mPlayTimer;
	tPtr <CFileTimeSpan>			mStartTimeOffset;
	CMap <long, long, long, long>	mReqIdMap;
};

/////////////////////////////////////////////////////////////////////////////
