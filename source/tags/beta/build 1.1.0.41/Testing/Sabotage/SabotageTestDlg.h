#pragma once
#include "DaServerInterface.h"
#include "DaControlInterface.h"

/////////////////////////////////////////////////////////////////////////////

class CSabotageTestDlg : public CDialog
{
public:
	CSabotageTestDlg(CWnd* pParent = NULL);
	~CSabotageTestDlg();

// Dialog Data
	//{{AFX_DATA(CSabotageTestDlg)
	enum { IDD = IDD_SABOTAGETEST_DIALOG };
	CButton	mIdleOn;
	CButton	mSoundOn;
	CListCtrl	mCharacterList;
	CButton	mShowButton;
	CButton	mCancelButton;
	CListBox	mGestures;
	CButton	mUseControl;
	CButton	mControlContained;
	CButton	mControlStandalone;
	CButton	mUseServer;
	CButton	mMoveButton;
	CButton	mSpeakButton;
	CButton	mThinkButton;
	CButton	mListenButton;
	int mSabotageNum;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSabotageTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Operations
	void LoadConfig ();
	void SaveConfig ();
	bool CommandLineConfig ();
		bool ShowConfigCharacter ();

// Implementation
protected:
	//{{AFX_MSG(CSabotageTestDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnShowCharacter();
	afx_msg void OnControlMode();
	afx_msg void OnSoundOn();
	afx_msg void OnIdleOn();
	afx_msg void OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeGestures();
	afx_msg void OnDblClkGestures();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMoveButton();
	afx_msg void OnSpeakButton();
	afx_msg void OnThinkButton();
	afx_msg void OnListenButton();
	afx_msg void OnSabotageNum();
	afx_msg void OnCtlActivateInput(LPCTSTR CharacterID);
	afx_msg void OnCtlDeactivateInput(LPCTSTR CharacterID);
	afx_msg void OnCtlClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y);
	afx_msg void OnCtlDblClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y);
	afx_msg void OnCtlDragStart (LPCTSTR CharacterID, short Button, short Shift, short X, short Y);
	afx_msg void OnCtlDragComplete (LPCTSTR CharacterID, short Button, short Shift, short X, short Y);
	afx_msg void OnCtlShow (LPCTSTR CharacterID, VisibilityCauseType Cause);
	afx_msg void OnCtlHide (LPCTSTR CharacterID, VisibilityCauseType Cause);
	afx_msg void OnCtlRequestStart (IDaCtlRequest* Request);
	afx_msg void OnCtlRequestComplete (IDaCtlRequest* Request);
	afx_msg void OnCtlCommand (IDaCtlUserInput* UserInput);
	afx_msg void OnCtlIdleStart (LPCTSTR CharacterID);
	afx_msg void OnCtlIdleComplete (LPCTSTR CharacterID);
	afx_msg void OnCtlMove (LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause);
	afx_msg void OnCtlSize (LPCTSTR CharacterID, short Width, short Height);
	afx_msg void OnCtlBalloonShow(LPCTSTR CharacterID);
	afx_msg void OnCtlBalloonHide(LPCTSTR CharacterID);
	afx_msg void OnCtlListenStart(LPCTSTR CharacterID);
	afx_msg void OnCtlListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause);
	afx_msg void OnCtlActiveClientChange (LPCTSTR CharacterID, BOOL Active);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

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
	void ShowModeSelection ();
	void ShowControlMode ();
	void ShowCharacters ();
	bool SelectCharacter (int pCharacterNdx, bool pShow);
	bool ShowCharacter (LPCTSTR pCharacterPath);
	void ShowGestures ();
	bool ShowSelectedGesture (bool pStopFirst = true);
	CString GetSelectedGesture ();

	bool IsAnimating ();
	bool Stop ();
	CPoint GetInitialPos (const CSize & pInitialSize);
	void SabotageEvent ();

	void GetAgentServer ();
	void FreeAgentServer ();
	bool LoadServerCharacter ();
	bool LoadedServerCharacter ();
	bool FreeServerCharacter ();
	bool ShowServerCharacter ();
	bool HideServerCharacter ();

	void GetAgentControl ();
	void FreeAgentControl ();
	bool LoadControlCharacter ();
	bool FreeControlCharacter ();
	bool LoadedControlCharacter ();
	bool ShowControlCharacter ();
	bool HideControlCharacter ();

	bool IsCharacterVisible ();
	void CharacterIsVisible (bool pVisible);
	void ShowCharacterState ();

protected:
	int						mCharacterNdx;
	CString					mCharacterPath;
	IDaServer2Ptr			mServer;
	long					mNotifySinkId;
	IDaSvrCharacter2Ptr		mServerCharacter;
	long					mServerCharacterId;
	IDaControl2Ptr			mControl;
	CWnd					mControlWnd;
	CRect					mControlRect;
	IDaCtlCharacter2Ptr		mControlCharacter;
	CString					mControlCharacterId;
	long					mLoadReqID;
	long					mShowReqID;
	long					mPlayReqID;
	long					mMoveReqID;
	long					mSpeakReqID;
	long					mThinkReqID;
	long					mExitCommandId;
	IDaCtlRequestPtr		mLoadRequest;
	IDaCtlRequestPtr		mShowRequest;
	IDaCtlRequestPtr		mPlayRequest;
	IDaCtlRequestPtr		mMoveRequest;
	IDaCtlRequestPtr		mSpeakRequest;
	IDaCtlRequestPtr		mThinkRequest;
	CString					mExitCommandName;
	UINT_PTR				mRepeatTimer;
	UINT_PTR				mCycleTimer;
};

/////////////////////////////////////////////////////////////////////////////
