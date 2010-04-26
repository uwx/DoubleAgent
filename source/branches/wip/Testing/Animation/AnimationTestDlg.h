#pragma once
#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet2, __uuidof(IDaSvrPropertySheet2));

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
	CButton	mIdleEnabled;
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
	CButton	mSmoothNone;
	CButton	mSmoothEdges;
	CButton	mSmoothFull;
	CButton	mIconShown;
	CButton	mIconOnLoad;
	CButton	mIconGenerated;
	CButton	mIconClipped;
	CButton	mIconIdentified;
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
	afx_msg void OnSmooth();
	afx_msg void OnIconOnLoad();
	afx_msg void OnIconShown();
	afx_msg void OnIconGenerated();
	afx_msg void OnIconIdentified();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(DaSvrNotifySink, IDaSvrNotifySink)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE Command (long CommandID, IDaSvrUserInput2 *UserInput);
		HRESULT STDMETHODCALLTYPE ActivateInputState (long CharacterID, long Activated);
		HRESULT STDMETHODCALLTYPE Restart (void);
		HRESULT STDMETHODCALLTYPE Shutdown (void);
		HRESULT STDMETHODCALLTYPE VisibleState (long CharacterID, long Visible, long Cause);
		HRESULT STDMETHODCALLTYPE Click (long CharacterID, short Keys, long x, long y);
		HRESULT STDMETHODCALLTYPE DblClick (long CharacterID, short Keys, long x, long y);
		HRESULT STDMETHODCALLTYPE DragStart (long CharacterID, short Keys, long x, long y);
		HRESULT STDMETHODCALLTYPE DragComplete (long CharacterID, short Keys, long x, long y);
		HRESULT STDMETHODCALLTYPE RequestStart (long RequestID);
		HRESULT STDMETHODCALLTYPE RequestComplete (long RequestID, long Result);
		HRESULT STDMETHODCALLTYPE BookMark (long BookMarkID);
		HRESULT STDMETHODCALLTYPE Idle (long CharacterID, long Start);
		HRESULT STDMETHODCALLTYPE Move (long CharacterID, long x, long y, long Cause);
		HRESULT STDMETHODCALLTYPE Size (long CharacterID, long Width, long Height);
		HRESULT STDMETHODCALLTYPE BalloonVisibleState (long CharacterID, long Visible);
		HRESULT STDMETHODCALLTYPE HelpComplete (long CharacterID, long CommandID, long Cause);
		HRESULT STDMETHODCALLTYPE ListeningState (long CharacterID, long Listening, long Cause);
		HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR CharGUID);
		HRESULT STDMETHODCALLTYPE AgentPropertyChange(void);
		HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status);
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
	void SetCharacterIcon();

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
