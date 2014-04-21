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
	CButton	mCharacterCount1;
	CButton	mCharacterCount2;
	CButton	mCharacterCount5;
	CButton	mCharacterCount10;
	CButton	mSuperSabotage;
	CButton	mRestartSabotage;
	CButton	mSuperSabotageNow;
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
	afx_msg void OnSabotageMode();
	afx_msg void OnSabotageNow();
	afx_msg void OnCharacterCount();
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
	void ShowSabotageMode ();
	void ShowCharacters ();
	bool SelectCharacter (int pCharacterNdx, bool pShow);
	bool ShowCharacter (LPCTSTR pCharacterPath);
	void ShowGestures ();
	bool ShowSelectedGesture (bool pStopFirst = true);
	CString GetSelectedGesture ();

	bool IsAnimating ();
	bool Stop ();
	CPoint GetInitialPos (const CSize& pInitialSize);
	void SabotageEvent ();

	void GetAgentServers (INT_PTR pServerCount = 0);
	void FreeAgentServers (INT_PTR pServerCount = 0);
	bool LoadServerCharacters (INT_PTR pCharacterCount = 0);
	bool LoadedServerCharacter (INT_PTR pNdx);
	bool FreeServerCharacters (INT_PTR pCharacterCount = 0);
	bool ShowServerCharacters ();
	bool HideServerCharacters ();
	long GetServerCharacterId (INT_PTR pNdx) const;
	INT_PTR GetServerCharacterCount () const;

	void GetAgentControls (INT_PTR pControlCount = 0);
	void FreeAgentControls (INT_PTR pControlCount = 0);
	bool LoadControlCharacters (INT_PTR pCharacterCount = 0);
	bool LoadedControlCharacter (INT_PTR pNdx);
	bool FreeControlCharacters (INT_PTR pCharacterCount = 0);
	bool ShowControlCharacters ();
	bool HideControlCharacters ();
	CString GetControlCharacterId (INT_PTR pNdx) const;
	INT_PTR GetControlCharacterCount () const;

	bool IsCharacterVisible ();
	void CharacterIsVisible (bool pVisible);
	void ShowCharacterState ();

protected:
	int																									mSelCharacterNdx;
	CString																								mSelCharacterPath;
	CAtlArrayEx <IDaServer2Ptr, CComPtrElementTraits <IDaServer2Ptr, IDaServer2> >						mServer;
	CAtlTypeArray <long>																				mNotifySinkId;
	CAtlArrayEx <IDaSvrCharacter2Ptr, CComPtrElementTraits <IDaSvrCharacter2Ptr, IDaSvrCharacter2> >	mServerCharacter;
	CAtlTypeArray <long>																				mServerCharacterId;
	CAtlArrayEx <IDaServer2Ptr, CComPtrElementTraits <IDaServer2Ptr, IDaServer2> >						mCharacterServer;
	CAtlArrayEx <IDaControl2Ptr, CComPtrElementTraits <IDaControl2Ptr, IDaControl2> >					mControl;
	CAtlArrayEx <IDaCtlCharacter2Ptr, CComPtrElementTraits <IDaCtlCharacter2Ptr, IDaCtlCharacter2> >	mControlCharacter;
	CStringArray																						mControlCharacterId;
	CAtlOwnPtrArray <CWnd>																				mControlWnd;
	CRect																								mControlRect;
	CAtlArrayEx <IDaControl2Ptr, CComPtrElementTraits <IDaControl2Ptr, IDaControl2> >					mCharacterControl;
	CAtlTypeArray <long>																				mLoadReqID;
	CAtlTypeArray <long>																				mShowReqID;
	CAtlTypeArray <long>																				mPlayReqID;
	CAtlTypeArray <long>																				mMoveReqID;
	CAtlTypeArray <long>																				mSpeakReqID;
	CAtlTypeArray <long>																				mThinkReqID;
	CAtlTypeArray <long>																				mExitCommandId;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mLoadRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mShowRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mPlayRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mMoveRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mSpeakRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mThinkRequest;
	CStringArray																						mExitCommandName;
	UINT_PTR																							mRepeatTimer;
	UINT_PTR																							mCycleTimer;
};

/////////////////////////////////////////////////////////////////////////////
