#pragma once
#include "DaServerInterface.h"
#include "AgentPreviewWnd.h"

_COM_SMARTPTR_TYPEDEF (IDaSvrAudioOutput, __uuidof(IDaSvrAudioOutput));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechInput, __uuidof(IDaSvrSpeechInput));

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
	CButton	mFormattedButton;
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

	BEGIN_INTERFACE_PART(DaSvrNotifySink, IDaSvrNotifySink2)
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
		HRESULT STDMETHODCALLTYPE SpeechStart (long CharacterID, IDaSvrFormattedText *FormattedText);
		HRESULT STDMETHODCALLTYPE SpeechEnd (long CharacterID, IDaSvrFormattedText *FormattedText, VARIANT_BOOL Stopped);
		HRESULT STDMETHODCALLTYPE SpeechWord (long CharacterID, IDaSvrFormattedText *FormattedText, long WordIndex);
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
	IDaSvrTTSEnginesPtr			mTTSEngines;
	IDaSvrSREnginesPtr			mSREngines;
	int							mTTSModeAdded;
	int							mSRModeAdded;
	UINT_PTR					mStatusTimer;
};

/////////////////////////////////////////////////////////////////////////////
