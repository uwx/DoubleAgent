#pragma once
#include "DaServerInterface.h"
#include "DaControlInterface.h"
#include "FormLayout.h"

/////////////////////////////////////////////////////////////////////////////

class CStressTestDlg : public CDialog, public CFormLayout
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
	CButton	mRandomStopAll;
	CButton	mStressRepeat;
	CButton	mStressControl;
	CButton	mControlContained;
	CButton	mControlStandalone;
	CButton	mStressServer;
	CButton	mStressSound;
	CButton	mStressSpeak;
	CStatic	mControlPlacer;
	CListCtrl	mCharacterList;
	CButton	mCharacterCount1;
	CButton	mCharacterCount2;
	CButton	mCharacterCount5;
	CButton	mCharacterCount10;
	CButton	mOkButton;
	CButton	mCancelButton;
	CListBox	mGestures;
	CStatic mCycleIndicator;
	//}}AFX_DATA

// Operations
	void LoadConfig ();
	void SaveConfig ();
	bool CommandLineConfig ();

// Overrides
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelChangeGestures();
	afx_msg void OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnControlMode();
	afx_msg void OnCharacterCount();
	afx_msg void OnRandomStop();
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
		HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR GUID);
		HRESULT STDMETHODCALLTYPE AgentPropertyChange(void);
		HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status);
	END_INTERFACE_PART(DaSvrNotifySink)
	DECLARE_INTERFACE_MAP()

protected:
	void ShowModeSelection ();
	void ShowControlMode ();
	void ShowCharacters ();
	void ShowGestures ();

	bool ShowCharacter (int pCharacterNdx);
	bool ShowCharacter (LPCTSTR pCharacterPath);
	bool ShowGesture (int pGestureNdx);
	bool ShowGesture (LPCTSTR pGestureName);

	bool IsAnimating ();
	bool Stop (bool pRandom = false);
	CPoint GetInitialPos (const CSize & pInitialSize);

	void GetAgentServers (int pServerCount = 0);
	void FreeAgentServers (int pServerCount = 0);
	int GetServerMultiple () const;

	void GetAgentControls (int pControlCount = 0);
	void FreeAgentControls (int pControlCount = 0);
	int GetControlMultiple () const;

	bool ShowServerCharacters (int pCharacterCount = 0);
	bool HideServerCharacters ();
	bool FreeServerCharacters (int pCharacterCount = 0);
	long GetServerCharacterId (int pNdx) const;

	bool ShowControlCharacters (int pCharacterCount = 0);
	bool HideControlCharacters ();
	bool FreeControlCharacters (int pCharacterCount = 0);
	CString GetControlCharacterId (int pNdx) const;

protected:
	int																									mCharacterSelNdx;
	CString																								mCharacterPath;
	int																									mGestureSelNdx;
	DWORD																								mGestureStartTime;
	CAtlArrayEx <IDaServer2Ptr, CComPtrElementTraits <IDaServer2Ptr, IDaServer2> >						mServer;
	CAtlTypeArray <long>																				mServerSinkId;
	CAtlArrayEx <IDaSvrCharacter2Ptr, CComPtrElementTraits <IDaSvrCharacter2Ptr, IDaSvrCharacter2> >	mServerCharacter;
	CAtlTypeArray <long>																				mServerCharacterId;
	CAtlArrayEx <IDaServer2Ptr, CComPtrElementTraits <IDaServer2Ptr, IDaServer2> >						mCharacterServer;
	CAtlArrayEx <IDaControl2Ptr, CComPtrElementTraits <IDaControl2Ptr, IDaControl2> >					mControl;
	CAtlArrayEx <IDaCtlCharacter2Ptr, CComPtrElementTraits <IDaCtlCharacter2Ptr, IDaCtlCharacter2> >	mControlCharacter;
	CStringArray																						mControlCharacterId;
	CAtlOwnPtrArray <CWnd>																				mControlWnd;
	CAtlArrayEx <IDaControl2Ptr, CComPtrElementTraits <IDaControl2Ptr, IDaControl2> >					mCharacterControl;
	CRect																								mControlRect;
	CAtlTypeArray <long>																				mShowReqId;
	CAtlTypeArray <long>																				mGestureReqId;
	CAtlTypeArray <long>																				mSpeechReqId;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mShowRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mGestureRequest;
	CAtlArrayEx <IDaCtlRequestPtr, CComPtrElementTraits <IDaCtlRequestPtr, IDaCtlRequest> >				mSpeechRequest;
	int																									mCycleNum;
	UINT_PTR																							mCycleTimer;
	UINT_PTR																							mRandomStopTimer;
};

/////////////////////////////////////////////////////////////////////////////
