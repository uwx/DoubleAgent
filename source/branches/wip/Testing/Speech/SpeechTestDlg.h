#ifndef SPEECHTESTDLG_H_INCLUDED_
#define SPEECHTESTDLG_H_INCLUDED_
#pragma once

#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaSvrBalloon, __uuidof(IDaSvrBalloon));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommands, __uuidof(IDaSvrCommands));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommand, __uuidof(IDaSvrCommand));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));
_COM_SMARTPTR_TYPEDEF (IDaSvrAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties));
_COM_SMARTPTR_TYPEDEF (IDaSvrUserInput, __uuidof(IDaSvrUserInput));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines));
_COM_SMARTPTR_TYPEDEF (IDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine));
_COM_SMARTPTR_TYPEDEF (IDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines));

/////////////////////////////////////////////////////////////////////////////

class CSpeechTestDlg : public CDialog
{
public:
	CSpeechTestDlg(CWnd* pParent = NULL);
	~CSpeechTestDlg();

// Dialog Data
	//{{AFX_DATA(CSpeechTestDlg)
	enum { IDD = IDD_SPEECHTEST_DIALOG };
	CButton	mBalloonAutoSize;
	CButton	mBalloonAutoPace;
	CButton	mBalloonAutoHide;
	CButton	mBalloonPartialLines;
	CButton	mBalloonVisible;
	CButton	mAgentPropsButton;
	CButton	mCharPropsButton;
	CEdit	mBalloonTextEdit1;
	CEdit	mBalloonTextEdit2;
	CComboBox mSpeechWave;
	CButton	mThinkButton;
	CButton	mSpeakButton;
	CButton	mListenButton;
	CListCtrl	mCharacterList;
	CButton	mShowCharButton;
	CButton	mShowChar2Button;
	CButton	mCancelButton;
	CComboBox mTTSModes;
	CComboBox mSRModes;
	CStatic mTTSStatus;
	CStatic mSRStatus;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSpeechTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CSpeechTestDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnShowChar();
	afx_msg void OnShowChar2();
	afx_msg void OnThink();
	afx_msg void OnSpeak();
	afx_msg void OnListen();
	afx_msg void OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAgentProps();
	afx_msg void OnCharProps();
	afx_msg void OnBalloonVisible();
	afx_msg void OnBalloonAutoPace();
	afx_msg void OnBalloonAutoHide();
	afx_msg void OnBalloonAutoSize();
	afx_msg void OnBalloonPartialLines();
	afx_msg void OnSelEndOkSpeechWave();
	afx_msg void OnSelEndOkTTSModes();
	afx_msg void OnSelEndOkSRModes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(DaSvrNotifySink, IDaSvrNotifySink)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE Command (long dwCommandID, IUnknown *punkUserInput);
		HRESULT STDMETHODCALLTYPE ActivateInputState (long dwCharID, long bActivated);
		HRESULT STDMETHODCALLTYPE Restart (void);
		HRESULT STDMETHODCALLTYPE Shutdown (void);
		HRESULT STDMETHODCALLTYPE VisibleState (long dwCharID, long bVisible, long dwCause);
		HRESULT STDMETHODCALLTYPE Click (long dwCharID, short fwKeys, long x, long y);
		HRESULT STDMETHODCALLTYPE DblClick (long dwCharID, short fwKeys, long x, long y);
		HRESULT STDMETHODCALLTYPE DragStart (long dwCharID, short fwKeys, long x, long y);
		HRESULT STDMETHODCALLTYPE DragComplete (long dwCharID, short fwKeys, long x, long y);
		HRESULT STDMETHODCALLTYPE RequestStart (long dwRequestID);
		HRESULT STDMETHODCALLTYPE RequestComplete (long dwRequestID, long hrStatus);
		HRESULT STDMETHODCALLTYPE BookMark (long dwBookMarkID);
		HRESULT STDMETHODCALLTYPE Idle (long dwCharID, long bStart);
		HRESULT STDMETHODCALLTYPE Move (long dwCharID, long x, long y, long dwCause);
		HRESULT STDMETHODCALLTYPE Size (long dwCharID, long lWidth, long lHeight);
		HRESULT STDMETHODCALLTYPE BalloonVisibleState (long dwCharID, long bVisible);
		HRESULT STDMETHODCALLTYPE HelpComplete (long dwCharID, long dwCommandID, long dwCause);
		HRESULT STDMETHODCALLTYPE ListeningState (long dwCharID, long bListening, long dwCause);
		HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR bszGUID);
		HRESULT STDMETHODCALLTYPE AgentPropertyChange(void);
		HRESULT STDMETHODCALLTYPE ActiveClientChange (long dwCharID, long lStatus);
	END_INTERFACE_PART(DaSvrNotifySink)

	DECLARE_INTERFACE_MAP()

	void ShowCharacters ();
	bool ShowCharacter (INT_PTR pCharNdx, LPCTSTR pCharacterPath);

	bool Stop (INT_PTR pCharNdx = -1);

	void GetAgentServer ();
	bool ShowAgentCharacter (INT_PTR pCharNdx);
	bool HideAgentCharacter (INT_PTR pCharNdx);
	bool ReleaseAgentCharacter (INT_PTR pCharNdx);
	bool LoadedAgentCharacter (INT_PTR pCharNdx);
	bool IsCharacterVisible (INT_PTR pCharNdx = -1);
	void CharacterIsVisible (INT_PTR pCharNdx, bool pVisible);
	void ShowCharacterState (INT_PTR pCharNdx = -1);

	void ShowTTSModes ();
	void ShowTTSStatus ();
	int FindTTSModeID (LPCTSTR pTTSModeID);
	CString GetTTSModeID (INT_PTR pTTSModeNdx);

	void ShowSRModes ();
	void ShowSRStatus ();
	int FindSRModeID (LPCTSTR pSRModeID);
	CString GetSRModeID (INT_PTR pSRModeNdx);

	void LoadConfig ();
	void SaveConfig ();
	void LoadWaveFiles ();
	void SaveWaveFiles ();
	CString GetWaveFile ();

protected:
	CString						mWinTitle;
	IDaServer2Ptr				mServer;
	long						mNotifySinkId;
	CString						mCharacterPath [2];
	long						mCharacterId [2];
	IDaSvrCharacter2Ptr			mCharacter [2];
	tPtr <CPoint>				mCharacterPos[2];
	long						mLoadReqID;
	long						mActiveChar;
	IDaSvrSpeechEnginesPtr		mSpeechEngines;
	IDaSvrRecognitionEnginesPtr	mRecognitionEngines;
	int							mTTSModeAdded;
	int							mSRModeAdded;
	UINT_PTR					mStatusTimer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // SPEECHTESTDLG_H_INCLUDED_
