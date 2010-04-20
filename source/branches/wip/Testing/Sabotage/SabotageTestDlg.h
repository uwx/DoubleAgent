#pragma once
#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaSvrAudioOutput, __uuidof(IDaSvrAudioOutput));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommands2, __uuidof(IDaSvrCommands2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommand2, __uuidof(IDaSvrCommand2));

/////////////////////////////////////////////////////////////////////////////

class CSabotageTestDlg : public CDialog
{
public:
	CSabotageTestDlg(CWnd* pParent = NULL);
	~CSabotageTestDlg();

// Dialog Data
	//{{AFX_DATA(CSabotageTestDlg)
	enum { IDD = IDD_SABOTAGETEST_DIALOG };
	CButton	mIdleEnabled;
	CButton	mSoundOn;
	CListCtrl	mCharacterList;
	CButton	mPopupButton;
	CButton	mCancelButton;
	CListBox	mStates;
	CListBox	mGestures;
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

// Implementation
protected:
	//{{AFX_MSG(CSabotageTestDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnPopup();
	afx_msg void OnSoundOn();
	afx_msg void OnIdleOn();
	afx_msg void OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeGestures();
	afx_msg void OnDblClkGestures();
	afx_msg void OnSelChangeStates();
	afx_msg void OnDblClkStates();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMoveButton();
	afx_msg void OnSpeakButton();
	afx_msg void OnThinkButton();
	afx_msg void OnListenButton();
	afx_msg void OnSabotageNum();
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
		HRESULT STDMETHODCALLTYPE RequestComplete (long RequestID, long hrStatus);
		HRESULT STDMETHODCALLTYPE BookMark (long dwBookMarkID);
		HRESULT STDMETHODCALLTYPE Idle (long CharacterID, long Start);
		HRESULT STDMETHODCALLTYPE Move (long CharacterID, long x, long y, long Cause);
		HRESULT STDMETHODCALLTYPE Size (long CharacterID, long Width, long Height);
		HRESULT STDMETHODCALLTYPE BalloonVisibleState (long CharacterID, long Visible);
		HRESULT STDMETHODCALLTYPE HelpComplete (long CharacterID, long CommandID, long Cause);
		HRESULT STDMETHODCALLTYPE ListeningState (long CharacterID, long Listening, long Cause);
		HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR bszGUID);
		HRESULT STDMETHODCALLTYPE AgentPropertyChange(void);
		HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status);
	END_INTERFACE_PART(DaSvrNotifySink)

	DECLARE_INTERFACE_MAP()

protected:
	void ShowCharacters ();
	bool ShowCharacter (LPCTSTR pCharacterPath);
	void ShowGestures ();
	void ShowStates ();
	CString GetSelGesture ();
	CString GetSelState ();
	void ShowSelectedAnimation ();
	void ShowDefaultAnimation ();
	bool ShowSelGesture (bool pStopFirst = true);
	bool ShowSelState (bool pStopFirst = true);

	bool IsAnimating ();
	bool Stop ();
	void SabotageEvent ();

	void GetAgentServer ();
	void FreeAgentServer ();
	bool ShowAgentCharacter ();
	bool HideAgentCharacter ();
	bool FreeAgentCharacter ();
	bool LoadedAgentCharacter ();
	bool IsCharacterVisible ();
	void CharacterIsVisible (bool pVisible);
	void ShowCharacterState ();

	void LoadConfig ();
	void SaveConfig ();

protected:
	CString					mWinTitle;
	CString					mCharacterPath;
	IDaServer2Ptr			mServer;
	long					mNotifySinkId;
	IDaSvrCharacter2Ptr		mCharacter;
	long					mCharacterId;
	long					mExitCommandId;
	UINT_PTR				mRepeatTimer;
	UINT					mTimerCount;
	long					mLoadReqID;
	long					mLastAnimationReqID;
	long					mHidingStateReqID;
	long					mMoveReqID;
	long					mSpeakReqID;
	long					mThinkReqID;
};

/////////////////////////////////////////////////////////////////////////////
