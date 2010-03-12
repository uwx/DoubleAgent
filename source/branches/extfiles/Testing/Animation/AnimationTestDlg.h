#ifndef ANIMATIONTESTDLG_H_INCLUDED_
#define ANIMATIONTESTDLG_H_INCLUDED_
#pragma once

#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));

/////////////////////////////////////////////////////////////////////////////

class CAnimationTestDlg : public CDialog
{
public:
	CAnimationTestDlg(CWnd* pParent = NULL);
	~CAnimationTestDlg();

// Dialog Data
	//{{AFX_DATA(CAnimationTestDlg)
	enum { IDD = IDD_ANIMATIONTEST_DIALOG };
	CButton	mAutoPopup;
	CButton	mFastShowHide;
	CButton	mIdleOn;
	CButton	mAnimateBoth;
	CButton	mAgentPropsButton;
	CButton	mCharPropsButton;
	CButton	mSoundOn;
	CStatic	mPreviewWnd;
	CListCtrl	mCharacterList;
	CEdit	mCharacterNameEdit;
	CEdit	mCharacterDescEdit;
	CButton	mPopupButton;
	CButton	mCancelButton;
	CListBox	mStates;
	CListBox	mGestures;
	CButton	mAllStates;
	CButton	mAllGestures;
	CButton	mSizeNormal;
	CButton	mSizeLarge;
	CButton	mSizeSmall;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAnimationTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAnimationTestDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnAllGestures();
	afx_msg void OnAllStates();
	afx_msg void OnPopup();
	afx_msg void OnSoundOn();
	afx_msg void OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeGestures();
	afx_msg void OnDblClkGestures();
	afx_msg void OnSelChangeStates();
	afx_msg void OnDblClkStates();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAgentProps();
	afx_msg void OnCharProps();
	afx_msg void OnIdleOn();
	afx_msg void OnAutoPopup();
	afx_msg void OnSizeNormal();
	afx_msg void OnSizeLarge();
	afx_msg void OnSizeSmall();
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
	bool ShowCharacter (LPCTSTR pCharacterPath);
	void ShowCharacterDetails ();
	void ShowGestures ();
	void ShowStates ();
	CString GetSelGesture ();
	CString GetSelState ();
	void ShowSelectedAnimation ();
	void ShowDefaultAnimation ();
	bool ShowSelGesture (bool pStopFirst = true);
	bool ShowSelState (bool pStopFirst = true);
	bool ShowAllGestures ();
	bool ShowAllStates ();

	bool IsAnimating ();
	bool Stop ();

	void GetAgentServer ();
	bool ShowAgentCharacter ();
	bool HideAgentCharacter ();
	bool ReleaseAgentCharacter ();
	bool LoadedAgentCharacter ();
	bool IsCharacterVisible ();
	void CharacterIsVisible (bool pVisible);
	void ShowCharacterState ();
	void SetCharacterSize();

	void LoadConfig ();
	void SaveConfig ();

protected:
	tPtr <CAgentPreviewWnd>	mAgentWnd;
	CString					mWinTitle;
	CString					mCharacterPath;
	IDaServer2Ptr			mServer;
	long					mNotifySinkId;
	IDaSvrCharacter2Ptr		mCharacter;
	long					mCharacterId;
	UINT_PTR				mRepeatTimer;
	UINT_PTR				mAllGesturesTimer;
	UINT_PTR				mAllStatesTimer;
	UINT					mTimerCount;
	long					mLoadReqID;
	long					mHideReqID;
	long					mLastAnimationReqID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // ANIMATIONTESTDLG_H_INCLUDED_
