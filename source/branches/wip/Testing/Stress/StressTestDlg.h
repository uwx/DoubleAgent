#pragma once
#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////

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
	void ShowCharacterDetails ();
	void ShowGestures ();

	bool ShowCharacter (int pCharacterNdx);
	bool ShowCharacter (LPCTSTR pCharacterPath);
	bool ShowGesture (int pGestureNdx);
	bool ShowGesture (LPCTSTR pGestureName);

	bool IsAnimating ();
	bool Stop ();

	void GetAgentServer ();
	void FreeAgentServer ();
	bool ShowAgentCharacter ();
	bool HideAgentCharacter ();
	bool FreeAgentCharacter ();

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
