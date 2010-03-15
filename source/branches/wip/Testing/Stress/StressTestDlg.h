#ifndef STRESSTESTDLG_H_INCLUDED_
#define STRESSTESTDLG_H_INCLUDED_
#pragma once

#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));

/////////////////////////////////////////////////////////////////////////////

class CStressTestDlg : public CDialog
{
public:
	CStressTestDlg(CWnd* pParent = NULL);
	~CStressTestDlg();

// Dialog Data
	//{{AFX_DATA(CStressTestDlg)
	enum { IDD = IDD_STRESSTEST_DIALOG };
	CButton	mRandomStop0;
	CButton	mRandomStop1;
	CButton	mRandomStop2;
	CButton	mRandomStop3;
	CButton	mStressRepeat;
	CButton	mStressPreview;
	CButton	mStressCharacter;
	CButton	mStressSound;
	CButton	mStressSpeak;
	CStatic	mPreviewWnd;
	CListCtrl	mCharacterList;
	CEdit	mCharacterNameEdit;
	CButton	mOkButton;
	CButton	mCancelButton;
	CListBox	mGestures;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStressTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CStressTestDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelChangeGestures();
	afx_msg void OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRandomStop();
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

protected:
	void ShowCharacters ();
	void ShowCharacterDetails ();
	void ShowGestures ();

	bool ShowCharacter (int pCharacterNdx);
	bool ShowCharacter (LPCTSTR pCharacterPath);
	bool ShowGesture (int pGestureNdx);
	bool ShowGesture (LPCTSTR pGestureName);

	bool IsAnimating ();
	bool Stop ();

	void GetAgentServer ();
	bool ShowAgentCharacter ();
	bool HideAgentCharacter ();
	bool ReleaseAgentCharacter ();

	void LoadConfig ();
	void SaveConfig ();

protected:
	tPtr <CAgentPreviewWnd>	mAgentWnd;
	CString					mCharacterPath;
	CPoint					mCharacterPos;
	int						mCharacterAutoPos;
	IDaServer2Ptr			mServer;
	long					mNotifySinkId;
	IDaSvrCharacterPtr		mCharacter;
	long					mCharacterId;
	int						mCycleNum;
	UINT_PTR				mTimer;
	UINT_PTR				mRandomStopTimer;
	int						mCharacterNdx;
	int						mGestureNdx;
	DWORD					mGestureStartTime;
	long					mGestureReqId;
	long					mSpeechReqId;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // STRESSTESTDLG_H_INCLUDED_
