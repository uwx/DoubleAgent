#include "StdAfx.h"
#include <shlwapi.h>
#include "SabotageTest.h"
#include "SabotageTestDlg.h"
#include "AgentPreviewWnd.h"
#include "UiState.h"
#include "NotifyLock.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "LocalizeEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal|LogHighVolume
#define	_LOG_AGENT_CALLS		LogNormal|LogTimeMs|LogHighVolume
#define	_LOG_CHAR_CALLS			LogAlways|LogTimeMs|LogHighVolume
//#define	_LOG_CHAR_CALLS_EX	LogAlways|LogTimeMs|LogHighVolume
#define	_LOG_NOTIFY				LogNormal|LogTimeMs
#endif

#ifndef	_LOG_AGENT_CALLS
#define	_LOG_AGENT_CALLS		LogNormal|LogTimeMs
#endif
#ifndef	_LOG_CHAR_CALLS
#define	_LOG_CHAR_CALLS			LogNormal|LogTimeMs
#endif
#ifndef	_LOG_CHAR_CALLS_EX
#define	_LOG_CHAR_CALLS_EX		LogDetails|LogTimeMs
#endif
#ifndef	_LOG_NOTIFY
#define	_LOG_NOTIFY				LogDetails|LogTimeMs
#endif

#define	_KEEP_INITIAL_SERVER	0

/////////////////////////////////////////////////////////////////////////////

static const int	sSabotageShowEvent = 1;
static const int	sSabotageHideEvent = 2;
static const int	sSabotageActivateEvent = 3;
static const int	sSabotageClickEvent = 4;
static const int	sSabotageDblClickEvent = 5;
static const int	sSabotageCommandEvent = 6;
static const int	sSabotageMoveEvent = 7;
static const int	sSabotageDragStartEvent = 8;
static const int	sSabotageDragEndEvent = 9;
static const int	sSabotageAnimateStartEvent = 10;
static const int	sSabotageAnimateEndEvent = 11;
static const int	sSabotageSpeakStartEvent = 12;
static const int	sSabotageSpeakEndEvent = 13;
static const int	sSabotageThinkStartEvent = 14;
static const int	sSabotageThinkEndEvent = 15;
static const int	sSabotageIdleStartEvent = 16;
static const int	sSabotageIdleEndEvent = 17;
static const int	sSabotageBalloonShowEvent = 18;
static const int	sSabotageBalloonHideEvent = 19;
static const int	sSabotageListenStartEvent = 20;
static const int	sSabotageListenEndEvent = 21;

static const int	sSabotageMinEvent = sSabotageShowEvent;
static const int	sSabotageMaxEvent = sSabotageListenEndEvent;

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileCharacter = _T("Character");
static LPCTSTR	sProfileCharacterCount = _T("Characters");
static LPCTSTR	sProfileUseControl = _T("UseControl");
static LPCTSTR	sProfileControlContained = _T("ControlContained");
static LPCTSTR	sProfileControlStandalone = _T("ControlStandalone");
static LPCTSTR	sProfileUseServer = _T("UseServer");
static LPCTSTR	sProfileSoundOn = _T("Sound");
static LPCTSTR	sProfileIdleOn = _T("Idle");
static LPCTSTR	sProfileGesture = _T("Gesture");
static LPCTSTR	sProfileSabotage = _T("Sabotage");
static LPCTSTR	sProfileSuperSabotage = _T("SuperSabotage");
static LPCTSTR	sProfileRestartSabotage = _T("RestartSabotage");

static LPCTSTR	sControlCharacterIdPopup = _T("Popup");
static LPCTSTR	sControlCharacterIdContained = _T("Contained");

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSabotageTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSabotageTestDlg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ACTIVATEAPP()
	ON_BN_CLICKED(IDC_SHOW, OnShowCharacter)
	ON_BN_CLICKED(IDC_USE_CONTROL, OnControlMode)
	ON_BN_CLICKED(IDC_CONTROL_CONTAINED, OnControlMode)
	ON_BN_CLICKED(IDC_CONTROL_STANDALONE, OnControlMode)
	ON_BN_CLICKED(IDC_SUPER_SABOTAGE, OnSabotageMode)
	ON_BN_CLICKED(IDC_RESTART_SABOTAGE, OnSabotageMode)
	ON_BN_CLICKED(IDC_CHARACTERS_01, OnCharacterCount)
	ON_BN_CLICKED(IDC_CHARACTERS_02, OnCharacterCount)
	ON_BN_CLICKED(IDC_CHARACTERS_05, OnCharacterCount)
	ON_BN_CLICKED(IDC_CHARACTERS_10, OnCharacterCount)
	ON_BN_CLICKED(IDC_SOUND_ON, OnSoundOn)
	ON_BN_CLICKED(IDC_IDLE_ON, OnIdleOn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, OnItemChangedCharacterList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CHARACTER_LIST, OnItemActivateCharacterList)
	ON_LBN_SELCHANGE(IDC_GESTURES, OnSelChangeGestures)
	ON_LBN_DBLCLK(IDC_GESTURES, OnDblClkGestures)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MOVE, OnMoveButton)
	ON_BN_CLICKED(IDC_SPEAK, OnSpeakButton)
	ON_BN_CLICKED(IDC_THINK, OnThinkButton)
	ON_BN_CLICKED(IDC_LISTEN, OnListenButton)
	ON_BN_CLICKED(IDC_SABOTAGE00, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE01, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE02, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE03, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE04, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE05, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE06, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE07, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE08, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE09, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE10, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE11, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE12, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE13, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE14, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE15, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE16, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE17, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE18, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE19, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE20, OnSabotageNum)
	ON_BN_CLICKED(IDC_SABOTAGE21, OnSabotageNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSabotageTestDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSabotageTestDlg)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_ActivateInput, CSabotageTestDlg::OnCtlActivateInput, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_DeactivateInput, CSabotageTestDlg::OnCtlDeactivateInput, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_Click, CSabotageTestDlg::OnCtlClick, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_DblClick, CSabotageTestDlg::OnCtlDblClick, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_DragStart, CSabotageTestDlg::OnCtlDragStart, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_DragComplete, CSabotageTestDlg::OnCtlDragComplete, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_Show, CSabotageTestDlg::OnCtlShow, VTS_BSTR VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_Hide, CSabotageTestDlg::OnCtlHide, VTS_BSTR VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_RequestStart, CSabotageTestDlg::OnCtlRequestStart, VTS_DISPATCH)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_RequestComplete, CSabotageTestDlg::OnCtlRequestComplete, VTS_DISPATCH)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_Command, CSabotageTestDlg::OnCtlCommand, VTS_DISPATCH)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_IdleStart, CSabotageTestDlg::OnCtlIdleStart, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_IdleComplete, CSabotageTestDlg::OnCtlIdleComplete, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_IdleStart, CSabotageTestDlg::OnCtlIdleStart, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_Move, CSabotageTestDlg::OnCtlMove, VTS_BSTR VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_Size, CSabotageTestDlg::OnCtlSize, VTS_BSTR VTS_I2 VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_BalloonShow, CSabotageTestDlg::OnCtlBalloonShow, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_BalloonHide, CSabotageTestDlg::OnCtlBalloonHide, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_ListenStart, CSabotageTestDlg::OnCtlListenStart, VTS_BSTR)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_ListenComplete, CSabotageTestDlg::OnCtlListenComplete, VTS_BSTR VTS_I2)
	ON_EVENT(CSabotageTestDlg, IDC_CONTROL, DISPID_AgentEvents_ActiveClientChange, CSabotageTestDlg::OnCtlActiveClientChange, VTS_BSTR VTS_BOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CSabotageTestDlg, CDialog)
	INTERFACE_PART(CSabotageTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CSabotageTestDlg, DaSvrNotifySink)

/////////////////////////////////////////////////////////////////////////////

CSabotageTestDlg::CSabotageTestDlg(CWnd* pParent)
:	CDialog(CSabotageTestDlg::IDD, pParent),
	mSelCharacterNdx (-1),
	mNotifySinkId (0),
	mServerCharacterId (0),
	mExitCommandId (0),
	mRepeatTimer (0),
	mCycleTimer (0),
	mLoadReqID (0),
	mShowReqID (0),
	mPlayReqID (0),
	mMoveReqID (),
	mSpeakReqID (0),
	mThinkReqID (0)
{
	//{{AFX_DATA_INIT(CSabotageTestDlg)
	mSabotageNum = 0;
	//}}AFX_DATA_INIT
	EnableAutomation ();
}

CSabotageTestDlg::~CSabotageTestDlg()
{
	FreeServerCharacters ();
	FreeAgentServers ();
	FreeControlCharacters ();
	FreeAgentControls ();
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSabotageTestDlg)
	DDX_Control(pDX, IDC_IDLE_ON, mIdleOn);
	DDX_Control(pDX, IDC_SOUND_ON, mSoundOn);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDC_USE_CONTROL, mUseControl);
	DDX_Control(pDX, IDC_CONTROL_CONTAINED, mControlContained);
	DDX_Control(pDX, IDC_CONTROL_STANDALONE, mControlStandalone);
	DDX_Control(pDX, IDC_CHARACTERS_01, mCharacterCount1);
	DDX_Control(pDX, IDC_CHARACTERS_02, mCharacterCount2);
	DDX_Control(pDX, IDC_CHARACTERS_05, mCharacterCount5);
	DDX_Control(pDX, IDC_CHARACTERS_10, mCharacterCount10);
	DDX_Control(pDX, IDC_USE_SERVER, mUseServer);
	DDX_Control(pDX, IDC_SUPER_SABOTAGE, mSuperSabotage);
	DDX_Control(pDX, IDC_RESTART_SABOTAGE, mRestartSabotage);
	DDX_Control(pDX, IDC_SHOW, mShowButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	DDX_Control(pDX, IDC_GESTURES, mGestures);
	DDX_Control(pDX, IDC_MOVE, mMoveButton);
	DDX_Control(pDX, IDC_SPEAK, mSpeakButton);
	DDX_Control(pDX, IDC_THINK, mThinkButton);
	DDX_Control(pDX, IDC_LISTEN, mListenButton);
	DDX_Radio(pDX, IDC_SABOTAGE00, mSabotageNum);
	//}}AFX_DATA_MAP

	if	(pDX->m_bSaveAndValidate)
	{
		AfxGetApp()->WriteProfileInt (sProfileKey, sProfileSabotage, mSabotageNum);
	}
}

BOOL CSabotageTestDlg::OnInitDialog()
{
	bool	lAutoStart;

	mSabotageNum = AfxGetApp()->GetProfileInt (sProfileKey, sProfileSabotage, mSabotageNum);

	CDialog::OnInitDialog();

	GetAgentServers ();
	ShowCharacters ();
#if	!_KEEP_INITIAL_SERVER	
	FreeAgentServers ();
#endif	

	LoadConfig ();
	lAutoStart = CommandLineConfig ();
	ShowControlMode ();
	ShowSabotageMode ();
	ShowCharacterState ();

	if	(lAutoStart)
	{
		mCycleTimer = SetTimer ((UINT_PTR)&mCycleTimer, 500, NULL);
	}
	else
	{
		ShowConfigCharacter ();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::SabotageEvent ()
{
	LogMessage (LogIfActive, _T("*** SabotageEvent ***"));
	
	if	(mSuperSabotage.GetCheck())
	{
		SaveConfig ();
		if	(mRestartSabotage.GetCheck())
		{
			tSS <STARTUPINFO, DWORD>	lStartupInfo;
			tS <PROCESS_INFORMATION>	lProcessInfo;

			CreateProcess (NULL, GetCommandLine(), NULL, NULL, FALSE, DETACHED_PROCESS|CREATE_NEW_PROCESS_GROUP|CREATE_DEFAULT_ERROR_MODE|NORMAL_PRIORITY_CLASS, NULL, NULL, &lStartupInfo, &lProcessInfo); 
			if	(lProcessInfo.hThread)
			{
				CloseHandle (lProcessInfo.hThread);
			}
			if	(lProcessInfo.hProcess)
			{
				CloseHandle (lProcessInfo.hProcess);
			}
		}
		ExitProcess (0);
	}
	else
	{
		ShowCharacterState ();

		if	(mServerCharacter.GetCount() > 0)
		{
			LogMessage (LogNormal, _T("FreeServerCharacters"));
			FreeServerCharacters ();
			LogMessage (LogNormal, _T("FreeServerCharacters Done"));

			ShowCharacterState ();
		}

		if	(mServer.GetCount() > 0)
		{
			LogMessage (LogNormal, _T("FreeAgentServers"));
			FreeAgentServers ();
			LogMessage (LogNormal, _T("FreeAgentServers Done"));

			ShowCharacterState ();
		}

		if	(mControlCharacter.GetCount() > 0)
		{
			LogMessage (LogNormal, _T("FreeControlCharacters"));
			FreeControlCharacters ();
			LogMessage (LogNormal, _T("FreeControlCharacters Done"));

			ShowCharacterState ();
		}
#if	FALSE
//TODO
		if	(
				(mControl.GetCount() > 0)
			&&	(!mControlContained.GetCheck ())
			)
		{
			LogMessage (LogNormal, _T("FreeAgentControls"));
			FreeAgentControls ();
			LogMessage (LogNormal, _T("FreeAgentControls Done"));

			ShowCharacterState ();
		}
#endif		
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::ShowModeSelection ()
{
	if	(
			(mServerCharacter.GetCount() > 0)
		||	(mControlCharacter.GetCount() > 0)
		)
	{
		mUseServer.EnableWindow (FALSE);
		mUseControl.EnableWindow (FALSE);
		mControlContained.EnableWindow (FALSE);
		mControlStandalone.EnableWindow (FALSE);
	}
	else
	{
		mUseServer.EnableWindow (TRUE);
		mUseControl.EnableWindow (TRUE);
		ShowControlMode ();
	}
}

void CSabotageTestDlg::ShowControlMode ()
{
	if	(mUseControl.GetCheck())
	{
		mControlContained.EnableWindow (mUseControl.IsWindowEnabled());
		if	(mControlContained.GetCheck())
		{
			mControlStandalone.SetCheck (TRUE);
			mControlStandalone.EnableWindow (FALSE);
		}
		else
		{
			mControlStandalone.EnableWindow (mUseControl.IsWindowEnabled());
		}
	}
	else
	{
		mControlContained.SetCheck (FALSE);
		mControlStandalone.SetCheck (FALSE);
		mControlContained.EnableWindow (FALSE);
		mControlStandalone.EnableWindow (FALSE);
	}
}

void CSabotageTestDlg::ShowSabotageMode ()
{
	if	(mSuperSabotage.GetCheck())
	{
		mRestartSabotage.EnableWindow (TRUE);
	}
	else
	{
		mRestartSabotage.EnableWindow (FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::ShowCharacters ()
{
	IDaSvrCharacterFilesPtr	lCharacterFiles;
	tSafeArrayPtr			lFilePaths;
	VARTYPE					lFilePathType;
	long					lLowerBound, lUpperBound, lNdx;
	CRect					lClientRect;

	mCharacterList.InsertColumn (0, _T("Path"));
	mCharacterList.GetClientRect (&lClientRect);
	lClientRect.right -= GetSystemMetrics (SM_CXVSCROLL);

	if	(
			(mServer.GetCount() > 0)
		&&	(mServer[0])
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer[0]->get_CharacterFiles (&lCharacterFiles))))
		)
	{
#if	TRUE
		lCharacterFiles->put_Filter (FilesFilter_PathMask);
#endif
		if	(
				(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, lCharacterFiles->get_FilePaths (lFilePaths.Free()))))
			&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, SafeArrayGetVartype (lFilePaths, &lFilePathType))))
			&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, SafeArrayGetLBound (lFilePaths, 1, &lLowerBound))))
			&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, SafeArrayGetUBound (lFilePaths, 1, &lUpperBound))))
			)
	{
			for	(lNdx = lLowerBound; lNdx < lUpperBound; lNdx++)
			{
				_variant_t	lFilePath;

				lFilePath.vt = lFilePathType;
				SafeArrayGetElement (lFilePaths, &lNdx, &V_BYREF(&lFilePath));
				mCharacterList.InsertItem (0, CString ((BSTR)(_bstr_t)lFilePath));
			}
		}
	}

	mCharacterList.SetColumnWidth (0, lClientRect.Width());
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::SelectCharacter (int pCharacterNdx, bool pShow)
{
	bool	lRet = false;

	if	(
			(pCharacterNdx >= 0)
		&&	(pCharacterNdx < mCharacterList.GetItemCount ())
		)
	{
		mSelCharacterNdx = pCharacterNdx;
		{
			CNotifyLock	lLock (m_hWnd);
			mCharacterList.SetItemState (mSelCharacterNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		mCharacterList.EnsureVisible (mSelCharacterNdx, FALSE);
		mCharacterList.RedrawWindow ();

		if	(pShow)
		{
			lRet = ShowCharacter (mCharacterList.GetItemText (mSelCharacterNdx, 0));
		}
		else
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSabotageTestDlg::ShowCharacter (LPCTSTR pCharacterPath)
{
	bool	lRet = false;

	if	(mSelCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
	{
		FreeServerCharacters ();
//		FreeAgentServers ();
		FreeControlCharacters ();
		CharacterIsVisible (false);
	}

	ShowControlMode ();

	if	(
			(pCharacterPath)
		&&	(pCharacterPath[0])
		)
	{
		mSelCharacterPath = pCharacterPath;

		if	(
				(mUseServer.GetCheck())
			&&	(LoadServerCharacters (-1))
			)
		{
			lRet = true;
		}
		if	(
				(mUseControl.GetCheck())
			&&	(LoadControlCharacters (-1))
			)
		{
			lRet = true;
		}
	}
	else
	{
		mSelCharacterPath.Empty ();
	}

	ShowGestures ();
	ShowCharacterState ();
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::ShowGestures ()
{
	CString	lSelGesture;

	if	(mGestures.GetCount() > 0)
	{
		lSelGesture = GetSelectedGesture ();
	}
	else
	{
		lSelGesture = AfxGetApp()->GetProfileString (sProfileKey, sProfileGesture);
	}

	mGestures.SetRedraw (FALSE);
	mGestures.ResetContent ();

	try
	{
		tSafeArrayPtr	lAnimations;
		VARTYPE			lAnimationType;
		long			lLowerBound, lUpperBound, lNdx;
		CRect			lClientRect;

		mGestures.AddString (_T(""));

		if	(
				(
					(
						(mServerCharacter.GetCount() > 0)
					&&	(mServerCharacter[0])
					&&	(SUCCEEDED (LogComErr (LogNormal, mServerCharacter[0]->get_Animations (lAnimations.Free()))))
					)
				||	(
						(mControlCharacter.GetCount() > 0)
					&&	(mControlCharacter[0])
					&&	(SUCCEEDED (LogComErr (LogNormal, mControlCharacter[0]->get_Animations (lAnimations.Free()))))
					)
				)
			&&	(SUCCEEDED (LogComErr (LogDetails, SafeArrayGetVartype (lAnimations, &lAnimationType))))
			&&	(SUCCEEDED (LogComErr (LogDetails, SafeArrayGetLBound (lAnimations, 1, &lLowerBound))))
			&&	(SUCCEEDED (LogComErr (LogDetails, SafeArrayGetUBound (lAnimations, 1, &lUpperBound))))
			)
		{
			for	(lNdx = lLowerBound; lNdx < lUpperBound; lNdx++)
			{
				_variant_t	lAnimation;

				lAnimation.vt = lAnimationType;
				SafeArrayGetElement (lAnimations, &lNdx, &V_BYREF(&lAnimation));
				mGestures.AddString (CString ((BSTR)(_bstr_t)lAnimation));
			}
		}
	}
	catch AnyExceptionDebug

	mGestures.SetCurSel (max (mGestures.FindStringExact (1, lSelGesture), 0));
	mGestures.SetRedraw (TRUE);
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::ShowSelectedGesture (bool pStopFirst)
{
	bool	lRet = false;
	CString	lSelGesture = GetSelectedGesture ();
	HRESULT	lResult;
	INT_PTR	lNdx;

	if	(lSelGesture.IsEmpty())
	{
		Stop ();
	}
	else
	if	(IsCharacterVisible ())
	{
		if	(pStopFirst)
		{
			Stop ();
		}
		else
		{
			for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
			{
				if	(mServerCharacter[lNdx])
				{
					if	(
							(lNdx < (INT_PTR)mPlayReqID.GetCount())
						&&	(mPlayReqID[lNdx] > 0)
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mServerCharacter[lNdx]->Stop (mPlayReqID[lNdx]));
						mPlayReqID[lNdx] = 0;
					}
					if	(
							(lNdx < (INT_PTR)mMoveReqID.GetCount())
						&&	(mMoveReqID[lNdx] > 0)
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mServerCharacter[lNdx]->Stop (mMoveReqID[lNdx]));
						mMoveReqID[lNdx] = 0;
					}
					if	(
							(lNdx < (INT_PTR)mSpeakReqID.GetCount())
						&&	(mSpeakReqID[lNdx] > 0)
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mServerCharacter[lNdx]->Stop (mSpeakReqID[lNdx]));
						mSpeakReqID[lNdx] = 0;
					}
					if	(
							(lNdx < (INT_PTR)mThinkReqID.GetCount())
						&&	(mThinkReqID[lNdx] > 0)
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mServerCharacter[lNdx]->Stop (mThinkReqID[lNdx]));
						mThinkReqID[lNdx] = 0;
					}
				}
			}

			for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
			{
				if	(mControlCharacter[lNdx])
				{
					if	(
							(lNdx < (INT_PTR)mPlayRequest.GetCount())
						&&	(mPlayRequest[lNdx])
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mControlCharacter[lNdx]->Stop (_variant_t((LPDISPATCH)mPlayRequest[lNdx].GetInterfacePtr())));
						mPlayRequest[lNdx] = NULL;
					}
					if	(
							(lNdx < (INT_PTR)mPlayRequest.GetCount())
						&&	(mMoveRequest[lNdx])
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mControlCharacter[lNdx]->Stop (_variant_t((LPDISPATCH)mMoveRequest[lNdx].GetInterfacePtr())));
						mMoveRequest[lNdx] = NULL;
					}
					if	(
							(lNdx < (INT_PTR)mPlayRequest.GetCount())
						&&	(mSpeakRequest[lNdx])
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mControlCharacter[lNdx]->Stop (_variant_t((LPDISPATCH)mSpeakRequest[lNdx].GetInterfacePtr())));
						mSpeakRequest[lNdx] = NULL;
					}
					if	(
							(lNdx < (INT_PTR)mPlayRequest.GetCount())
						&&	(mThinkRequest[lNdx])
						)
					{
						LogComErr (_LOG_CHAR_CALLS, mControlCharacter[lNdx]->Stop (_variant_t((LPDISPATCH)mThinkRequest[lNdx].GetInterfacePtr())));
						mThinkRequest[lNdx] = NULL;
					}
				}
			}
		}

		mPlayReqID.RemoveAll ();
		mPlayReqID.SetCount (mServerCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(mServerCharacter[lNdx])
			{
				lResult = mServerCharacter[lNdx]->Play (_bstr_t(lSelGesture), &mPlayReqID[lNdx]);
				if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Play [%s] [%d]"), mServerCharacterId[lNdx], lSelGesture, mPlayReqID[lNdx])))
				{
					lRet = true;
				}
			}
		}

		mPlayRequest.RemoveAll();
		mPlayRequest.SetCount (mControlCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(mControlCharacter[lNdx])
			{
				lResult = mControlCharacter[lNdx]->Play (_bstr_t(lSelGesture), &mPlayRequest[lNdx]);
				if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Play [%s] [%d]"), mControlCharacterId[lNdx], lSelGesture, mPlayRequest[lNdx].ID)))
				{
					lRet = true;
				}
			}
		}
	}

	ShowCharacterState ();
	return lRet;
}

CString CSabotageTestDlg::GetSelectedGesture ()
{
	CString	lGestureName;
	int		lCurSel = mGestures.GetCurSel ();

	if	(lCurSel > 0)
	{
		mGestures.GetText (lCurSel, lGestureName);
	}
	return lGestureName;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::IsAnimating ()
{
	INT_PTR	lNdx;
	
	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(
				(mServerCharacter[lNdx])
			&&	(
					(
						(lNdx < (INT_PTR)mLoadReqID.GetCount())
					&&	(mLoadReqID[lNdx] > 0)
					)
				||	(
						(lNdx < (INT_PTR)mShowReqID.GetCount())
					&&	(mShowReqID[lNdx] > 0)
					)
				||	(
						(lNdx < (INT_PTR)mPlayReqID.GetCount())
					&&	(mPlayReqID[lNdx] > 0)
					)
				||	(
						(lNdx < (INT_PTR)mMoveReqID.GetCount())
					&&	(mMoveReqID[lNdx] > 0)
					)
				||	(
						(lNdx < (INT_PTR)mSpeakReqID.GetCount())
					&&	(mSpeakReqID[lNdx] > 0)
					)
				||	(
						(lNdx < (INT_PTR)mThinkReqID.GetCount())
					&&	(mThinkReqID[lNdx] > 0)
					)
				)
			)
		{
			return true;
		}
	}

	for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
	{
		if	(
				(mControlCharacter[lNdx])
			&&	(
					(
						(lNdx < (INT_PTR)mLoadRequest.GetCount())
					&&	(mLoadRequest[lNdx])
					&&	(
							(mLoadRequest[lNdx].Status == RequestStatus_Pending)
						||	(mLoadRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (INT_PTR)mShowRequest.GetCount())
					&&	(mShowRequest[lNdx])
					&&	(
							(mShowRequest[lNdx].Status == RequestStatus_Pending)
						||	(mShowRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (INT_PTR)mPlayRequest.GetCount())
					&&	(mPlayRequest[lNdx])
					&&	(
							(mPlayRequest[lNdx].Status == RequestStatus_Pending)
						||	(mPlayRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (INT_PTR)mMoveRequest.GetCount())
					&&	(mMoveRequest[lNdx])
					&&	(
							(mMoveRequest[lNdx].Status == RequestStatus_Pending)
						||	(mMoveRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (INT_PTR)mSpeakRequest.GetCount())
					&&	(mSpeakRequest[lNdx])
					&&	(
							(mSpeakRequest[lNdx].Status == RequestStatus_Pending)
						||	(mSpeakRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (INT_PTR)mThinkRequest.GetCount())
					&&	(mThinkRequest[lNdx])
					&&	(
							(mThinkRequest[lNdx].Status == RequestStatus_Pending)
						||	(mThinkRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				)
			)
		{
			return true;
		}
	}
	return false;
}

bool CSabotageTestDlg::Stop ()
{
	bool	lRet = false;
	INT_PTR	lNdx;

	if	(mRepeatTimer)
	{
		KillTimer (mRepeatTimer);
		mRepeatTimer = 0;
		lRet = true;
	}

	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(
				(mServerCharacter[lNdx] != NULL)
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mServerCharacter[lNdx]->StopAll (StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility), _T("[%d] StopAll"), mServerCharacterId[lNdx])))
			)
		{
			lRet = true;
		}
	}
	mShowReqID.RemoveAll();
	mPlayReqID.RemoveAll();
	mMoveReqID.RemoveAll();
	mSpeakReqID.RemoveAll();
	mThinkReqID.RemoveAll();

	for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
	{
		if	(
				(mControlCharacter[lNdx] != NULL)
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mControlCharacter[lNdx]->StopAll (_variant_t((long)(StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility),VT_I4)), _T("[%s] StopAll"), mControlCharacterId[lNdx])))
			)
		{
			lRet = true;
		}
	}
	mShowRequest.RemoveAll();
	mPlayRequest.RemoveAll();
	mMoveRequest.RemoveAll();
	mSpeakRequest.RemoveAll();
	mThinkRequest.RemoveAll();

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK GetUsedRects (HWND pWindow, LPARAM pLparam)
{
	CAtlTypeArray<CRect> *	lUsedRects = (CAtlTypeArray<CRect> *) pLparam;
	CRect					lUsedRect;

	if	(
			(pWindow != GetShellWindow ())
		&&	(pWindow != GetDesktopWindow ())
		&&	(IsWindowVisible (pWindow))
		&&	(!IsZoomed (pWindow))
		&&	(GetWindowRect (pWindow, &lUsedRect))
		&&	(!lUsedRect.IsRectEmpty ())
		)
	{
		lUsedRects->Add (lUsedRect);
	}
	return TRUE;
}

CPoint CSabotageTestDlg::GetInitialPos (const CSize & pInitialSize)
{
	CPoint					lInitialPos;
	CRect					lBounds (pInitialSize.cx, pInitialSize.cy, GetSystemMetrics(SM_CXSCREEN)-pInitialSize.cx, GetSystemMetrics(SM_CYSCREEN)-pInitialSize.cy);
	CAtlTypeArray<CRect>	lUsedRects;

	EnumWindows (GetUsedRects, (LPARAM)&lUsedRects);

	for	(lInitialPos.y = lBounds.top; lInitialPos.y <= lBounds.bottom; lInitialPos.y += pInitialSize.cy)
	{
		for	(lInitialPos.x = lBounds.right; lInitialPos.x >= lBounds.left; lInitialPos.x -= pInitialSize.cy)
		{
			CRect	lRect (lInitialPos, pInitialSize);
			CRect	lIntersect;
			int		lNdx;

			for	(lNdx = (int)lUsedRects.GetCount()-1; lNdx >= 0; lNdx--)
			{
				if	(lIntersect.IntersectRect (lRect, lUsedRects [lNdx]))
				{
					break;
				}
			}
			if	(lNdx < 0)
			{
				return lInitialPos;
			}
		}
	}
	return CPoint (0,0);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::GetAgentServers (INT_PTR pServerCount)
{
	HRESULT	lResult;
	INT_PTR	lNdx;
	
	if	(pServerCount <= 0)
	{
		pServerCount = 1;
	}
	if	((INT_PTR)mServer.GetCount() < pServerCount)
	{
		mServer.SetCount (pServerCount);
	}
	mNotifySinkId.SetCount (mServer.GetCount());
	
	for	(lNdx = 0; lNdx < (INT_PTR)mServer.GetCount(); lNdx++)
	{
		if	(!mServer[lNdx])
		{
			IUnknownPtr	lUnknown;

			lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown);
			if	(LogIsActive (_LOG_AGENT_CALLS))
			{
				LogComErrAnon (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] CoCreateInstance"), lNdx);
			}
			mServer[lNdx] = lUnknown;

			if	(mServer[lNdx])
			{
				mServer[lNdx].CharacterStyle |= CharacterStyle_Smoothed;
				mServer[lNdx].CharacterStyle &= ~CharacterStyle_IconShown;
				lResult = mServer[lNdx]->Register (&m_xDaSvrNotifySink, &mNotifySinkId[lNdx]);
				if	(LogIsActive (_LOG_AGENT_CALLS))
				{
					LogComErrAnon (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] Register [%d]"), lNdx, mNotifySinkId[lNdx]);
				}
			}
		}
	}
}

void CSabotageTestDlg::FreeAgentServers (INT_PTR pServerCount)
{
	HRESULT	lResult;
	INT_PTR	lNdx;
	
	if	(pServerCount <= 0)
	{
		pServerCount = 0;
	}
	pServerCount = min (pServerCount, (INT_PTR)mServer.GetCount());
	
	for	(lNdx = pServerCount; lNdx < (INT_PTR)mServer.GetCount(); lNdx++)
	{
		if	(
				(mServer[lNdx])
			&&	(lNdx < (INT_PTR)mNotifySinkId.GetCount())
			&&	(mNotifySinkId[lNdx])
			)
		{
			try
			{
				lResult = mServer[lNdx]->Unregister (mNotifySinkId[lNdx]);
				if	(LogIsActive (_LOG_AGENT_CALLS))
				{
					LogComErrAnon (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] Unregister [%d]"), lNdx, mNotifySinkId[lNdx]);
				}
			}
			catch AnyExceptionSilent
		}
	}

	mServer.SetCount (pServerCount);
	mNotifySinkId.SetCount (mServer.GetCount());
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::LoadServerCharacters (INT_PTR pCharacterCount)
{
	bool	lRet = false;
	HRESULT	lResult;
	INT_PTR	lNdx;
	INT_PTR	lServerNdx;

	if	(pCharacterCount == 0)
	{
		pCharacterCount = 1;
	}
	else
	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetServerCharacterCount ();
	}

	GetAgentServers (pCharacterCount);
	
	if	(
			(mServer.GetCount() > 0)
		&&	(mServer[0])
		&&	(!mSelCharacterPath.IsEmpty ())
		)
	{
		if	((INT_PTR)mServerCharacter.GetCount() < pCharacterCount)
		{
			mServerCharacter.SetCount (pCharacterCount);
		}
		mLoadReqID.SetCount (mServerCharacter.GetCount());
		mExitCommandId.SetCount (mServerCharacter.GetCount());
		mServerCharacterId.SetCount (mServerCharacter.GetCount());
		mCharacterServer.SetCount (mServerCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(
					(lNdx < (INT_PTR)mServer.GetCount())
				&&	(mServer[lNdx])
				)
			{
				lServerNdx = lNdx;
			}
			else
			{
				lServerNdx = 0;
			}
		
			if	(
					(!mServerCharacter[lNdx])
				&&	(mServer[lServerNdx])
				)
			{
				mLoadReqID[lNdx] = 0;
				mExitCommandId[lNdx] = 0;
				mServerCharacterId[lNdx] = 0;
				mCharacterServer[lNdx] = mServer[lServerNdx];

				lResult = mCharacterServer[lNdx]->Load (_variant_t(mSelCharacterPath), &mServerCharacterId[lNdx], &mLoadReqID[lNdx]);

				if	(LogIsActive (_LOG_AGENT_CALLS))
				{
					LogComErrAnon (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] Load [%d] [%s] as [%d]"), lNdx, mLoadReqID[lNdx], mSelCharacterPath, mServerCharacterId[lNdx]);
				}
				if	(SUCCEEDED (lResult))
				{
					if	(LoadedServerCharacter (lNdx))
					{
						lRet = true;
					}
				}
				else
				{
					mServerCharacter[lNdx] = NULL;
					mServerCharacterId[lNdx] = 0;
					mCharacterServer[lNdx] = NULL;
				}
			}
		}
		
		if	(
				(lRet)
			&&	(
					(mCycleTimer)
				||	(mShowButton.GetCheck())
				)
			)
		{
			lRet = ShowServerCharacters ();
		}
	}
	return lRet;
}

bool CSabotageTestDlg::LoadedServerCharacter (INT_PTR pNdx)
{
	bool	lRet = false;
	long	lReqID = 0;
	CPoint	lCharPos;

	if	(
			(pNdx >= 0)
		&&	(pNdx < (INT_PTR)mServerCharacter.GetCount())
		)
	{
		if	(
				(mServer.GetCount() > 0)
			&&	(mServer[0])
			&&	(mServerCharacterId[pNdx] != 0)
			&&	(mServerCharacter[pNdx] == NULL)
			&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mCharacterServer[pNdx]->get_Character (mServerCharacterId[pNdx], &mServerCharacter[pNdx]), _T("get_Character"))))
			)
		{
			if	(mIdleOn.GetCheck())
			{
				mServerCharacter[pNdx].Style |= CharacterStyle_IdleEnabled;
			}
			else
			{
				mServerCharacter[pNdx].Style &= ~CharacterStyle_IdleEnabled;
			}
			if	(mSoundOn.GetCheck())
			{
				mServerCharacter[pNdx].Style |= CharacterStyle_SoundEffects;
			}
			else
			{
				mServerCharacter[pNdx].Style &= ~CharacterStyle_SoundEffects;
			}
		}

		if	(pNdx == 0)
		{
			ShowGestures ();
		}

		if	(mServerCharacter[pNdx] != NULL)
		{
			CPoint	lCharPos;
			CSize	lCharSize;

			if	(
					(SUCCEEDED (mServerCharacter[pNdx]->GetSize (&lCharSize.cx, &lCharSize.cy)))
				&&	(
						(lCharSize.cx > 200)
					||	(lCharSize.cy > 200)
					)
				)
			{
				while	(
							(lCharSize.cx > 200)
						||	(lCharSize.cy > 200)
						)
				{
					lCharSize.cx = MulDiv (lCharSize.cx, 3, 4);
					lCharSize.cy = MulDiv (lCharSize.cy, 3, 4);
				}
				mServerCharacter[pNdx]->SetSize (lCharSize.cx, lCharSize.cy);
			}

			if	(!mServerCharacter[pNdx].Visible)
			{
				int	lSabotageNum = mSabotageNum;

				mSabotageNum = -1;
				lCharPos = GetInitialPos (lCharSize);
				mServerCharacter[pNdx]->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
				mSabotageNum = lSabotageNum;
			}
		}

		if	(mServerCharacter[pNdx])
		{
			IDaSvrCommands2Ptr	lCommands (mServerCharacter[pNdx]);

			lCommands.Caption = _bstr_t("Sabotage");
			lCommands.VoiceGrammar = _bstr_t("sabotage");
			lCommands.GlobalVoiceCommandsEnabled = true;

			mExitCommandId.SetCount (mServerCharacter.GetCount());
			if	(mExitCommandId[pNdx] == 0)
			{
				lCommands->AddEx (_bstr_t("Exit"), _bstr_t("exit"), _bstr_t("Exit"), TRUE, TRUE, 0, &mExitCommandId[pNdx]);
			}
		}

		if	(mServerCharacter[pNdx])
		{
			lRet = true;
		}
		if	(pNdx < (INT_PTR)mLoadReqID.GetCount())
		{
			mLoadReqID[pNdx] = 0;
		}
	}

	return lRet;
}

bool CSabotageTestDlg::FreeServerCharacters (INT_PTR pCharacterCount)
{
	bool	lRet = false;
	HRESULT	lResult;
	INT_PTR	lNdx;

	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetServerCharacterCount ();
	}
	pCharacterCount = min (pCharacterCount, (INT_PTR)mServerCharacter.GetCount());

	for	(lNdx = pCharacterCount; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		mServerCharacter[lNdx] = NULL;

		if	(
				(lNdx < (INT_PTR)mCharacterServer.GetCount())
			&&	(lNdx < (INT_PTR)mServerCharacterId.GetCount())
			&&	(mCharacterServer[lNdx])
			&&	(mServerCharacterId[lNdx])
			)
		{
			lResult = mCharacterServer[lNdx]->Unload (mServerCharacterId[lNdx]);
			if	(LogIsActive (_LOG_AGENT_CALLS))
			{
				LogComErrAnon (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] Unload [%d]"), lNdx, mServerCharacterId[lNdx]);
			}
		}
		lRet = true;
	}

	mServerCharacter.SetCount (pCharacterCount);
	mServerCharacterId.SetCount (mServerCharacter.GetCount());
	mCharacterServer.SetCount (mServerCharacter.GetCount());

	mLoadReqID.SetCount (mServerCharacter.GetCount());
	mShowReqID.SetCount (mServerCharacter.GetCount());
	mPlayReqID.SetCount (mServerCharacter.GetCount());
	mMoveReqID.SetCount (mServerCharacter.GetCount());
	mSpeakReqID.SetCount (mServerCharacter.GetCount());
	mThinkReqID.SetCount (mServerCharacter.GetCount());
	
	FreeAgentServers (max (pCharacterCount,1));
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::ShowServerCharacters ()
{
	bool	lRet = false;
	HRESULT	lResult;
	INT_PTR	lNdx;

	mShowReqID.RemoveAll ();
	mShowReqID.SetCount (mServerCharacter.GetCount());
		
	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mServerCharacter[lNdx])
		{
			lResult = mServerCharacter[lNdx]->Show (FALSE, &mShowReqID[lNdx]);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mServerCharacterId[lNdx], mShowReqID[lNdx])))
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

bool CSabotageTestDlg::HideServerCharacters ()
{
	bool	lRet = false;
	HRESULT	lResult;
	INT_PTR	lNdx;

	mShowReqID.RemoveAll ();
	mShowReqID.SetCount (mServerCharacter.GetCount());

	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mServerCharacter[lNdx])
		{
			lResult = mServerCharacter[lNdx]->Hide (FALSE, &mShowReqID[lNdx]);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mServerCharacterId[lNdx], mShowReqID[lNdx])))
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CSabotageTestDlg::GetServerCharacterId (INT_PTR pNdx) const
{
	if	(
			(pNdx >= 0)
		&&	(pNdx < (INT_PTR)mServerCharacterId.GetCount())
		)
	{
		return mServerCharacterId[pNdx];
	}
	return -1;
}

INT_PTR CSabotageTestDlg::GetServerCharacterCount () const
{
	if	(mCharacterCount1.m_hWnd)
	{
		return mCharacterCount10.GetCheck()?10:mCharacterCount5.GetCheck()?5:mCharacterCount2.GetCheck()?2:1;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::GetAgentControls (INT_PTR pControlCount)
{
	CWnd *	lPlacer;
	INT_PTR	lNdx;
	
	if	(
			(lPlacer = GetDlgItem (IDC_CONTROL))
		&&	(
				(mControlWnd.GetCount() <= 0)
			||	(lPlacer->GetSafeHwnd() != mControlWnd[0]->GetSafeHwnd())
			)
		)
	{
		lPlacer->GetWindowRect (mControlRect);
		lPlacer->DestroyWindow();
		ScreenToClient (mControlRect);
	}

	if	(pControlCount <= 0)
	{
		pControlCount = 1;
	}
	if	(mControlContained.GetCheck())
	{
		pControlCount = max (pControlCount, 1);
	}
	if	((INT_PTR)mControl.GetCount() < pControlCount)
	{
		mControl.SetCount (pControlCount);
	}
	mControlWnd.SetCount (max (mControlWnd.GetCount(), mControl.GetCount()));
	
	for	(lNdx = 0; lNdx < (INT_PTR)mControl.GetCount(); lNdx++)
	{
		if	(
				(!mControlWnd[lNdx]->GetSafeHwnd())
			&&	(mControlWnd[lNdx] = new CWnd)
			&&	(mControlWnd[lNdx]->CreateControl (__uuidof(DaControl), _T(""), WS_CHILD|WS_CLIPSIBLINGS, mControlRect, this, IDC_CONTROL+(UINT)lNdx))
			)
		{
			mControl[lNdx] = mControlWnd[lNdx]->GetControlSite()->m_pObject;
		}
		if	(mControl[lNdx])
		{
			mControl[lNdx].CharacterStyle |= CharacterStyle_Smoothed;
			mControl[lNdx].CharacterStyle &= ~CharacterStyle_IconShown;

			if	(
					(mControlContained.GetCheck ())
				||	(mControlStandalone.GetCheck ())
				)
			{
				mControl[lNdx].AutoConnect = false;
				mControl[lNdx].Connected = false;
			}
			else
			{
				mControl[lNdx].AutoConnect = true;
			}
		}
	}
}

void CSabotageTestDlg::FreeAgentControls (INT_PTR pControlCount)
{
	INT_PTR	lNdx;

	if	(pControlCount <= 0)
	{
		pControlCount = 0;
	}
	pControlCount = min (pControlCount, (INT_PTR)mControl.GetCount());
	mControl.SetCount (pControlCount);
	
	for	(lNdx = pControlCount; lNdx < (INT_PTR)mControlWnd.GetCount(); lNdx++)
	{
		if	(mControlWnd[lNdx]->GetSafeHwnd())
		{
			mControlWnd[lNdx]->DestroyWindow ();
		}
	}
	mControlWnd.SetCount (pControlCount);
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::LoadControlCharacters (INT_PTR pCharacterCount)
{
	bool					lRet = false;
	HRESULT					lResult;
	IDaCtlCharacters2Ptr	lCharacters;
	INT_PTR					lNdx;
	INT_PTR					lControlNdx;

	if	(pCharacterCount == 0)
	{
		pCharacterCount = 1;
	}
	else
	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetControlCharacterCount ();
	}

	GetAgentControls (pCharacterCount);

	if	(
			(mControl.GetCount() > 0)
		&&	(mControl[0])
		&&	(!mSelCharacterPath.IsEmpty ())
		)
	{
		if	((INT_PTR)mControlCharacter.GetCount() < pCharacterCount)
		{
			mControlCharacter.SetCount (pCharacterCount);
		}

		mLoadRequest.SetCount (mControlCharacter.GetCount());
		mExitCommandName.SetSize (mControlCharacter.GetCount());
		mControlCharacterId.SetSize (mControlCharacter.GetCount());
		mCharacterControl.SetCount (mControlCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(
					(lNdx < (INT_PTR)mControl.GetCount())
				&&	(mControl[lNdx])
				)
			{
				lControlNdx = lNdx;
			}
			else
			{
				lControlNdx = 0;
			}

			if	(
					(!mControlCharacter [lNdx])
				&&	(mControl[lControlNdx])
				&&	(lCharacters = mControl[lControlNdx].Characters)
				)
			{
				if	(mControlContained.GetCheck())
				{
					mControlCharacterId[lNdx] = sControlCharacterIdContained;
				}
				else
				{
					mControlCharacterId[lNdx] = sControlCharacterIdPopup;
				}
				mCharacterControl[lNdx] = mControl[lControlNdx];
				mLoadRequest[lNdx] = NULL;
				mExitCommandName[lNdx].Empty();

				lResult = lCharacters->Load (_bstr_t(mControlCharacterId[lNdx]), _variant_t(mSelCharacterPath), &mLoadRequest[lNdx]);
				if	(LogIsActive (_LOG_AGENT_CALLS))
				{
					LogComErr (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] Load [%d] [%s] as [%s]"), lNdx, mLoadRequest[lNdx].ID, mSelCharacterPath, mControlCharacterId[lNdx]);
				}

				if	(SUCCEEDED (lResult))
				{
					if	(LoadedControlCharacter (lNdx))
					{
						lRet = true;
					}
				}
				else
				{
					mControlCharacter[lNdx] = NULL;
					mControlCharacterId[lNdx].Empty ();
					mCharacterControl[lNdx] = NULL;
				}
			}
		}

		if	(
				(lRet)				
			&&	(
					(mCycleTimer)
				||	(mShowButton.GetCheck())
				)
			)
		{
			lRet = ShowControlCharacters ();
		}
	}

	return lRet;
}

bool CSabotageTestDlg::LoadedControlCharacter (INT_PTR pNdx)
{
	bool					lRet = false;
	IDaCtlCharacters2Ptr	lCharacters;

	if	(
			(pNdx >= 0)
		&&	(pNdx < (INT_PTR)mControlCharacter.GetCount())
		)
	{
		if	(
				(!mControlCharacterId[pNdx].IsEmpty())
			&&	(!mControlCharacter[pNdx])
			&&	(mCharacterControl[pNdx])
			&&	(lCharacters = mCharacterControl[pNdx].Characters)
			&&	(mControlCharacter[pNdx] = lCharacters.Item [_bstr_t(mControlCharacterId[pNdx])])
			)
		{
			mControlCharacter[pNdx].IdleEnabled = mIdleOn.GetCheck() ? true : false;
			mControlCharacter[pNdx].SoundEffectsEnabled = mSoundOn.GetCheck() ? true : false;
		}

		ShowGestures ();

		if	(mControlCharacter[pNdx])
		{
			CSize	lCharSize (mControlCharacter[pNdx].Width, mControlCharacter[pNdx].Height);
			CPoint	lCharPos;

			if	(
					(lCharSize.cx > 200)
				||	(lCharSize.cy > 200)
				)
			{
				while	(
							(lCharSize.cx > 200)
						||	(lCharSize.cy > 200)
						)
				{
					lCharSize.cx = MulDiv (lCharSize.cx, 3, 4);
					lCharSize.cy = MulDiv (lCharSize.cy, 3, 4);
				}
				mControlCharacter[pNdx].Width = (short)lCharSize.cx;
				mControlCharacter[pNdx].Height = (short)lCharSize.cy;
			}

			if	(
					(mControlCharacterId[pNdx].CompareNoCase (sControlCharacterIdPopup) == 0)
				&&	(!mControlCharacter[pNdx].Visible)
				)
			{
				int	lSabotageNum = mSabotageNum;

				mSabotageNum = -1;
				lCharPos = GetInitialPos (lCharSize);
				mControlCharacter[pNdx].Left = (short)lCharPos.x;
				mControlCharacter[pNdx].Top = (short)lCharPos.y;
				mSabotageNum = lSabotageNum;
			}

			if	(
					(pNdx == 0)
				&&	(mControlContained.GetCheck())
				)
			{
				mControl[pNdx].ControlCharacter = mControlCharacter[pNdx];
				if	(mControlWnd[pNdx]->GetSafeHwnd())
				{
					mControlWnd[pNdx]->ShowWindow (SW_SHOWNA);
				}
			}
			else
			{
				mControl[pNdx].ControlCharacter = NULL;
				if	(mControlWnd[pNdx]->GetSafeHwnd())
				{
					mControlWnd[pNdx]->ShowWindow (SW_HIDE);
				}
			}
		}

		if	(mControlCharacter[pNdx])
		{
			IDaCtlCommands2Ptr	lCommands (mControlCharacter[pNdx].Commands);
			IDaCtlCommand2Ptr	lCommand;

			lCommands.Caption = _bstr_t("Sabotage");
			lCommands.VoiceCaption = _bstr_t("Sabotage");
			lCommands.VoiceGrammar = _bstr_t("sabotage");
			lCommands.GlobalVoiceCommandsEnabled = true;

			if	(mExitCommandName[pNdx].IsEmpty())
			{
				mExitCommandName[pNdx] = _T("exit");
				lCommands->Add (_bstr_t(mExitCommandName[pNdx]), _variant_t(_bstr_t("Exit")), _variant_t(_bstr_t("exit")), _variant_t(VARIANT_TRUE,VT_BOOL), _variant_t(VARIANT_TRUE,VT_BOOL), &lCommand);
			}
		}

		if	(mControlCharacter[pNdx])
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSabotageTestDlg::FreeControlCharacters (INT_PTR pCharacterCount)
{
	bool					lRet = false;
	IDaCtlCharacters2Ptr	lCharacters;
	HRESULT					lResult;
	INT_PTR					lNdx;

	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetControlCharacterCount ();
	}
	pCharacterCount = min (pCharacterCount, (INT_PTR)mControlCharacter.GetCount());
	
	for	(lNdx = pCharacterCount; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx] != NULL)
		{
			lRet = true;
		}
		mControlCharacter[lNdx] = NULL;

		if	(
				(!mControlCharacterId[lNdx].IsEmpty())
			&&	(mCharacterControl[lNdx])
			&&	(lCharacters = mCharacterControl[lNdx].Characters)
			)
		{
			lResult = lCharacters->Unload (_bstr_t(mControlCharacterId[lNdx]));
			if	(LogIsActive (_LOG_AGENT_CALLS))
			{
				LogComErr (MinLogLevel(_LOG_AGENT_CALLS,LogAlways), lResult, _T("[%d] Unload [%s] as [%s]"), lNdx, mSelCharacterPath, mControlCharacterId[lNdx]);
			}
			if	(SUCCEEDED (lResult))
			{
				lRet = true;
			}
		}
		mControlCharacterId[lNdx].Empty ();
	}
	
	mControlCharacter.SetCount (pCharacterCount);
	mControlCharacterId.SetSize (mControlCharacter.GetCount());
	mCharacterControl.SetCount (mControlCharacter.GetCount());

	mLoadRequest.SetCount (mControlCharacter.GetCount());
	mShowRequest.SetCount (mControlCharacter.GetCount());
	mPlayRequest.SetCount (mControlCharacter.GetCount());
	mMoveRequest.SetCount (mControlCharacter.GetCount());
	mSpeakRequest.SetCount (mControlCharacter.GetCount());
	mThinkRequest.SetCount (mControlCharacter.GetCount());

	FreeAgentControls (max (pCharacterCount,1));
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::ShowControlCharacters ()
{
	bool	lRet = false;
	HRESULT	lResult;
	INT_PTR	lNdx;

	mShowRequest.RemoveAll();
	mShowRequest.SetCount (mControlCharacter.GetCount());

	for	(lNdx = 0; lNdx < (INT_PTR)mControl.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx])
		{
			lResult = mControlCharacter[lNdx]->Show (_variant_t (VARIANT_FALSE,VT_BOOL), &mShowRequest[lNdx]);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Show [%d]"), mControlCharacterId[lNdx], mShowRequest[lNdx].ID)))
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

bool CSabotageTestDlg::HideControlCharacters ()
{
	bool	lRet = false;
	HRESULT	lResult;
	INT_PTR	lNdx;

	mShowRequest.RemoveAll();
	mShowRequest.SetCount (mControlCharacter.GetCount());

	for	(lNdx = 0; lNdx < (INT_PTR)mControl.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx])
		{
			lResult = mControlCharacter[lNdx]->Hide (_variant_t (VARIANT_FALSE,VT_BOOL), &mShowRequest[lNdx]);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Hide [%d]"), mControlCharacterId[lNdx], mShowRequest[lNdx].ID)))
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CString CSabotageTestDlg::GetControlCharacterId (INT_PTR pNdx) const
{
	if	(
			(pNdx >= 0)
		&&	(pNdx < (INT_PTR)mControlCharacterId.GetCount())
		)
	{
		return mControlCharacterId[pNdx];
	}
	return CString();
}

INT_PTR CSabotageTestDlg::GetControlCharacterCount () const
{
	if	(
			(mCharacterCount1.m_hWnd)
		&&	(!mControlContained.GetCheck())
		)
	{
		return mCharacterCount10.GetCheck()?10:mCharacterCount5.GetCheck()?5:mCharacterCount2.GetCheck()?2:1;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::IsCharacterVisible ()
{
	INT_PTR	lNdx;

	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(
				(mServerCharacter[lNdx] != NULL)
			&&	(mServerCharacter[lNdx].Visible)
			)
		{
			return true;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
	{
		if	(
				(mControlCharacter[lNdx] != NULL)
			&&	(mControlCharacter[lNdx].Visible)
			)
		{
			return true;
		}
	}
	return false;
}

void CSabotageTestDlg::CharacterIsVisible (bool pVisible)
{
	long	lSoundOn = 0;
	long	lIdleOn = 0;

	if	(pVisible)
	{
		mShowButton.SetCheck (TRUE);
		mSpeakButton.EnableWindow (TRUE);
		mThinkButton.EnableWindow (TRUE);
		mListenButton.EnableWindow (TRUE);
	}
	else
	{
		mShowButton.SetCheck (FALSE);
		mSpeakButton.EnableWindow (FALSE);
		mThinkButton.EnableWindow (FALSE);
		mListenButton.EnableWindow (FALSE);
	}

	if	(
			(mServerCharacter.GetCount() > 0)
		&&	(mServerCharacter[0])
		)
	{
		mSoundOn.SetCheck ((mServerCharacter[0].Style & CharacterStyle_SoundEffects) ? TRUE : FALSE);
		mSoundOn.EnableWindow (TRUE);
	}
	else
	if	(
			(mControlCharacter.GetCount() > 0)
		&&	(mControlCharacter[0])
		)
	{
		mSoundOn.SetCheck (mControlCharacter[0].SoundEffectsEnabled ? TRUE : FALSE);
		mSoundOn.EnableWindow (TRUE);
	}
	else
	{
		mSoundOn.EnableWindow (FALSE);
	}

	if	(
			(mServerCharacter.GetCount() > 0)
		&&	(mServerCharacter[0])
		)
	{
		mIdleOn.SetCheck ((mServerCharacter[0].Style & CharacterStyle_IdleEnabled) ? TRUE : FALSE);
		mIdleOn.EnableWindow (TRUE);
	}
	else
	if	(
			(mControlCharacter.GetCount() > 0)
		&&	(mControlCharacter[0])
		)
	{
		mIdleOn.SetCheck (mControlCharacter[0].IdleEnabled ? TRUE : FALSE);
		mIdleOn.EnableWindow (TRUE);
	}
	else
	{
		mIdleOn.EnableWindow (FALSE);
	}
}

void CSabotageTestDlg::ShowCharacterState ()
{
	if	(
			(mServerCharacter.GetCount() > 0)
		&&	(mServerCharacter[0])
		)
	{
		mMoveButton.EnableWindow (TRUE);
		mCancelButton.EnableWindow (TRUE);
	}
	else
	if	(
			(mControlCharacter.GetCount() > 0)
		&&	(mControlCharacter[0])
		)
	{
		mMoveButton.EnableWindow (mControlCharacterId[0].CompareNoCase(sControlCharacterIdPopup)==0);
		mCancelButton.EnableWindow (TRUE);
	}
	else
	{
		mMoveButton.EnableWindow (FALSE);
		mCancelButton.EnableWindow (FALSE);
	}

	ShowModeSelection ();
	CharacterIsVisible (IsCharacterVisible ());
}


/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	mUseControl.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileUseControl, mUseControl.GetCheck()) ? TRUE : FALSE);
	mControlContained.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileControlContained, mControlContained.GetCheck()) ? TRUE : FALSE);
	mControlStandalone.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileControlStandalone, mControlStandalone.GetCheck()) ? TRUE : FALSE);
	mCharacterCount10.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharacterCount, 0) == 10);
	mCharacterCount5.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharacterCount, 0) == 5);
	mCharacterCount2.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharacterCount, 0) == 2);
	mCharacterCount1.SetCheck (!mCharacterCount10.GetCheck() && !mCharacterCount5.GetCheck() && !mCharacterCount2.GetCheck());
	mUseServer.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileUseServer, mUseServer.GetCheck()) ? TRUE : FALSE);
	mSoundOn.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileSoundOn, mSoundOn.GetCheck()) ? TRUE : FALSE);
	mIdleOn.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIdleOn, mIdleOn.GetCheck()) ? TRUE : FALSE);
	mSuperSabotage.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileSuperSabotage, mSuperSabotage.GetCheck()) ? TRUE : FALSE);
	mRestartSabotage.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRestartSabotage, mRestartSabotage.GetCheck()) ? TRUE : FALSE);

	mSelCharacterNdx = -1;
	mSelCharacterPath = lApp->GetProfileString (sProfileKey, sProfileCharacter);

	GetWindowRect (&lWinRect);
	lWinRect.OffsetRect (lApp->GetProfileInt (sProfileKey, sProfilePosX, lWinRect.left) - lWinRect.left, lApp->GetProfileInt (sProfileKey, sProfilePosY, lWinRect.top) - lWinRect.top);
	MoveWindow (&lWinRect);
}

void CSabotageTestDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	lApp->WriteProfileInt (sProfileKey, sProfileUseControl, mUseControl.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileControlContained, mControlContained.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileControlStandalone, mControlStandalone.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileCharacterCount, (int)GetServerCharacterCount());
	lApp->WriteProfileInt (sProfileKey, sProfileUseServer, mUseServer.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileSoundOn, mSoundOn.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIdleOn, mIdleOn.GetCheck());
	lApp->WriteProfileString (sProfileKey, sProfileCharacter, mSelCharacterPath);
	lApp->WriteProfileString (sProfileKey, sProfileGesture, GetSelectedGesture());
	lApp->WriteProfileInt (sProfileKey, sProfileSuperSabotage, mSuperSabotage.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileRestartSabotage, mRestartSabotage.GetCheck());

	if	(!IsIconic ())
	{
		GetWindowRect (&lWinRect);
		lApp->WriteProfileInt (sProfileKey, sProfilePosX, lWinRect.left);
		lApp->WriteProfileInt (sProfileKey, sProfilePosY, lWinRect.top);
	}
}

bool CSabotageTestDlg::CommandLineConfig ()
{
	bool	lRet = false;
	int		lArgNdx;

	for (lArgNdx = 1; lArgNdx < __argc; lArgNdx++)
	{
		CString	lArg (__targv [lArgNdx]);

		if	(
				(lArg [0] == _T('-'))
			||	(lArg [0] == _T('/'))
			)
		{
			lArg.Delete (0);

			if	(
					(lArg.CompareNoCase (_T("Server")) == 0)
				||	(lArg.CompareNoCase (_T("Server+")) == 0)
				)
			{
				mUseServer.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Server-")) == 0)
			{
				mUseServer.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Control")) == 0)
				||	(lArg.CompareNoCase (_T("Control+")) == 0)
				)
			{
				mUseControl.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Control-")) == 0)
			{
				mUseControl.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Contained")) == 0)
				||	(lArg.CompareNoCase (_T("Contained+")) == 0)
				)
			{
				mControlContained.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Contained-")) == 0)
			{
				mControlContained.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Standalone")) == 0)
				||	(lArg.CompareNoCase (_T("Standalone+")) == 0)
				)
			{
				mControlStandalone.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Standalone-")) == 0)
			{
				mControlStandalone.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Sound")) == 0)
				||	(lArg.CompareNoCase (_T("Sound+")) == 0)
				)
			{
				mSoundOn.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Sound-")) == 0)
			{
				mSoundOn.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Super")) == 0)
				||	(lArg.CompareNoCase (_T("Super+")) == 0)
				)
			{
				mSuperSabotage.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Super-")) == 0)
			{
				mSuperSabotage.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("SuperRestart")) == 0)
				||	(lArg.CompareNoCase (_T("SuperRestart+")) == 0)
				||	(lArg.CompareNoCase (_T("Restart")) == 0)
				||	(lArg.CompareNoCase (_T("Restart+")) == 0)
				)
			{
				mRestartSabotage.SetCheck (TRUE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("SuperRestart-")) == 0)
				||	(lArg.CompareNoCase (_T("Restart-")) == 0)
				)
			{
				mRestartSabotage.SetCheck (FALSE);
			}
			else
			if	(lArg.CompareNoCase (_T("Count")) == 0)
			{
				long	lCount;
				
				if	(
						(lArgNdx < __argc-1)
					&&	(LclParseNumber (__targv [lArgNdx+1], lCount))
					)
				{
					lArgNdx++;
					if	(lCount >= 10)
					{
						CheckRadioButton (IDC_CHARACTERS_01, IDC_CHARACTERS_10, IDC_CHARACTERS_10);
					}
					else
					if	(lCount >= 5)
					{
						CheckRadioButton (IDC_CHARACTERS_01, IDC_CHARACTERS_10, IDC_CHARACTERS_05);
					}
					else
					if	(lCount >= 2)
					{
						CheckRadioButton (IDC_CHARACTERS_01, IDC_CHARACTERS_10, IDC_CHARACTERS_02);
					}
					else
					if	(lCount >= 1)
					{
						CheckRadioButton (IDC_CHARACTERS_01, IDC_CHARACTERS_10, IDC_CHARACTERS_01);
					}
				}
			}
			else
			if	(lArg.CompareNoCase (_T("Show")) == 0)
			{
				if	(!mSelCharacterPath.IsEmpty())
				{
					mShowButton.SetCheck (TRUE);
				}
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Start")) == 0)
				||	(lArg.CompareNoCase (_T("Go")) == 0)
				)
			{
				lRet = true;
			}
		}
	}

	if	(lRet)
	{
		CString	lTitle (_T("Sabotage"));

		mUseServer.EnableWindow (FALSE);
		mUseControl.EnableWindow (FALSE);
		mControlContained.EnableWindow (FALSE);
		mControlStandalone.EnableWindow (FALSE);

		if	(mUseServer.GetCheck())
		{
			lTitle += _T(" Server");
		}
		if	(mUseControl.GetCheck())
		{
			lTitle += _T(" Control");
			if	(mControlContained.GetCheck())
			{
				lTitle += _T(" (Contained)");
			}
			else
			if	(mControlStandalone.GetCheck())
			{
				lTitle += _T(" (Standalone)");
			}
		}
#ifdef	_WIN64
		lTitle += _T(" x64");
#else
		lTitle += _T(" x86");
#endif
		SetWindowText (lTitle);

		LogStop ();
		LogStart (false, lTitle+_T(".log"));
		LogMessage (LogNormal|LogTime, _T("Start %s"), lTitle);
	}

	if	(
			(!mSelCharacterPath.IsEmpty())
		&&	(mShowButton.GetCheck ())
		)
	{
		ShowCharacter (mSelCharacterPath);
	}
	return lRet;
}

bool CSabotageTestDlg::ShowConfigCharacter ()
{
	CWinApp *	lApp = AfxGetApp ();
	CString		lCharacterPath;

	lCharacterPath = lApp->GetProfileString (sProfileKey, sProfileCharacter);
	if	(!lCharacterPath.IsEmpty ())
	{
		tS <LVFINDINFO>	lFindInfo;

		lFindInfo.flags = LVFI_STRING;
		lFindInfo.psz = lCharacterPath;
		lFindInfo.vkDirection = VK_NEXT;
		if	(SelectCharacter (ListView_FindItem (mCharacterList.m_hWnd, -1, &lFindInfo), false))
		{
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnCancel()
{
	if	(mCycleTimer)
	{
		KillTimer (mCycleTimer);
	}
	mCycleTimer = 0;

	if	(!Stop ())
	{
		MessageBeep (MB_OK);
	}
}

void CSabotageTestDlg::OnOK()
{
}

void CSabotageTestDlg::OnClose()
{
	Stop ();
	UpdateData (TRUE);
	CDialog::EndDialog (IDOK);
}

void CSabotageTestDlg::OnDestroy()
{
	Stop ();
	SaveConfig ();
	FreeServerCharacters ();
	FreeAgentServers ();
	FreeControlCharacters ();
	FreeAgentControls ();

	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW	lNotify = (LPNMLISTVIEW)pNMHDR;

	if	(
			(lNotify->uChanged & LVIF_STATE)
		&&	(!(lNotify->uOldState & LVIS_SELECTED))
		&&	(lNotify->uNewState & LVIS_SELECTED)
		)
	{
		if	(SelectCharacter (lNotify->iItem, true))
		{
			ShowSelectedGesture ();
		}
	}
}

void CSabotageTestDlg::OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE*	lNotify = (NMITEMACTIVATE*)pNMHDR;

	if	(SelectCharacter (lNotify->iItem, true))
	{
		ShowSelectedGesture ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnSelChangeGestures()
{
	ShowSelectedGesture ();
}

void CSabotageTestDlg::OnDblClkGestures()
{
	if	(mGestures.GetCurSel () > 0)
	{
		ShowSelectedGesture (true);
		mRepeatTimer = SetTimer ((UINT_PTR)&mRepeatTimer, 100, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnShowCharacter()
{
	if	(IsCharacterVisible ())
	{
		mShowButton.SetCheck (FALSE);

		if	(HideServerCharacters ())
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (false);
#endif
		}
		if	(HideControlCharacters ())
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (false);
#endif
		}
	}
	else
	{
		mShowButton.SetCheck (TRUE);

		if	(SelectCharacter (mSelCharacterNdx, true))
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (true);
#endif
		}
	}
	ShowCharacterState ();
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnMoveButton()
{
	Stop ();
	if	(IsCharacterVisible ())
	{
		HRESULT	lResult;
		CPoint	lCharPos;
		CSize	lCharSize;
		CRect	lCharRect;
		INT_PTR	lNdx;

		mMoveReqID.RemoveAll();
		mMoveReqID.SetCount (mServerCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(mServerCharacter[lNdx])
			{
				lCharPos.x = mServerCharacter[lNdx].Left;
				lCharPos.y = mServerCharacter[lNdx].Top;
				lCharSize.cx = mServerCharacter[lNdx].Width;
				lCharSize.cy = mServerCharacter[lNdx].Height;
				lCharRect = CRect (lCharPos, lCharSize);

				if	(
						(lCharRect.left <= 0)
					||	(lCharRect.right >= GetSystemMetrics(SM_CXSCREEN)-1)
					)
				{
					OffsetRect (lCharRect, (GetSystemMetrics(SM_CXSCREEN)/2)-lCharRect.CenterPoint().x, 0);
				}
				else
				if	(lCharRect.CenterPoint().x > GetSystemMetrics(SM_CXSCREEN)/2)
				{
					OffsetRect (lCharRect, GetSystemMetrics(SM_CXSCREEN)-lCharRect.right, 0);
				}
				else
				{
					OffsetRect (lCharRect, -lCharRect.left, 0);
				}

				lResult = mServerCharacter[lNdx]->MoveTo ((short)lCharRect.left, (short)lCharRect.top, 1000, &mMoveReqID[lNdx]);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveTo [%d %d] [%d]"), mServerCharacterId[lNdx], lCharRect.left, lCharRect.top, mMoveReqID[lNdx]);
			}
		}

		mMoveRequest.RemoveAll();
		mMoveRequest.SetCount (mControlCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(
					(mControlCharacter[lNdx])
				&&	(mControlCharacterId[lNdx].CompareNoCase(sControlCharacterIdPopup)==0)
				)
			{
				lCharPos.x = mControlCharacter[lNdx].Left;
				lCharPos.y = mControlCharacter[lNdx].Top;
				lCharSize.cx = mControlCharacter[lNdx].Width;
				lCharSize.cy = mControlCharacter[lNdx].Height;
				lCharRect = CRect (lCharPos, lCharSize);

				if	(
						(lCharRect.left <= 0)
					||	(lCharRect.right >= GetSystemMetrics(SM_CXSCREEN)-1)
					)
				{
					OffsetRect (lCharRect, (GetSystemMetrics(SM_CXSCREEN)/2)-lCharRect.CenterPoint().x, 0);
				}
				else
				if	(lCharRect.CenterPoint().x > GetSystemMetrics(SM_CXSCREEN)/2)
				{
					OffsetRect (lCharRect, GetSystemMetrics(SM_CXSCREEN)-lCharRect.right, 0);
				}
				else
				{
					OffsetRect (lCharRect, -lCharRect.left, 0);
				}

				lResult = mControlCharacter[lNdx]->MoveTo ((short)lCharRect.left, (short)lCharRect.top, _variant_t(), &mMoveRequest[lNdx]);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] MoveTo [%d %d] [%d]"), mControlCharacterId[lNdx], lCharRect.left, lCharRect.top, mMoveRequest[lNdx].ID);
			}
		}

		ShowCharacterState ();
	}
	else
	{
		MessageBeep (MB_OK);
	}
}

void CSabotageTestDlg::OnSpeakButton()
{
	Stop ();
	if	(IsCharacterVisible ())
	{
		HRESULT	lResult;
		INT_PTR	lNdx;

		mSpeakReqID.RemoveAll();
		mSpeakReqID.SetCount (mServerCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(mServerCharacter[lNdx])
			{
				lResult = mServerCharacter[lNdx]->Speak (_bstr_t("Now is the winter of our discontent."), NULL, &mSpeakReqID[lNdx]);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d]"), mServerCharacterId[lNdx], mSpeakReqID[lNdx]);
			}
		}

		mSpeakRequest.RemoveAll();
		mSpeakRequest.SetCount (mControlCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(mControlCharacter[lNdx])
			{
				lResult = mControlCharacter[lNdx]->Speak (_variant_t(_bstr_t("Now is the winter of our discontent.")), _variant_t(), &mSpeakRequest[lNdx]);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Speak [%d]"), mControlCharacterId[lNdx], mSpeakRequest[lNdx].ID);
			}
		}

		ShowCharacterState ();
	}
	else
	{
		MessageBeep (MB_OK);
	}
}

void CSabotageTestDlg::OnThinkButton()
{
	Stop ();
	if	(IsCharacterVisible ())
	{
		HRESULT	lResult;
		INT_PTR	lNdx;

		mThinkReqID.RemoveAll();
		mThinkReqID.SetCount (mServerCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(mServerCharacter[lNdx])
			{
				lResult = mServerCharacter[lNdx]->Think (_bstr_t("Now is the winter of our discontent."), &mThinkReqID[lNdx]);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d]"), mServerCharacterId[lNdx], mThinkReqID[lNdx]);
			}
		}

		mThinkRequest.RemoveAll();
		mThinkRequest.SetCount (mControlCharacter.GetCount());

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(mControlCharacter[lNdx])
			{
				lResult = mControlCharacter[lNdx]->Think (_bstr_t("Now is the winter of our discontent."), &mThinkRequest[lNdx]);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Think [%d]"), mControlCharacterId[lNdx], mThinkRequest[lNdx].ID);
			}
		}

		ShowCharacterState ();
	}
	else
	{
		MessageBeep (MB_OK);
	}
}

void CSabotageTestDlg::OnListenButton()
{
	Stop ();
	if	(IsCharacterVisible ())
	{
		HRESULT	lResult;
		INT_PTR	lNdx;

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(mServerCharacter[lNdx])
			{
				IDaSvrSettingsPtr	lSettings (mCharacterServer[lNdx].Settings);
				long				lListen = (lSettings.AudioStatus != AudioStatus_CharacterListening);

				lResult = mServerCharacter[lNdx]->Listen (lListen);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Listen [%d]"), mServerCharacterId[lNdx], lListen);
			}
		}

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(mControlCharacter[lNdx])
			{
				IDaCtlSettingsPtr	lSettings (mCharacterControl[lNdx].Settings);
				VARIANT_BOOL		lListen = (lSettings.AudioStatus != AudioStatus_CharacterListening) ? VARIANT_TRUE : VARIANT_FALSE;
				VARIANT_BOOL		lListening = VARIANT_FALSE;

				lResult = mControlCharacter[lNdx]->Listen (lListen, &lListening);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Listen [%d] [%d]"), mControlCharacterId[lNdx], lListen, lListening);
			}
		}

		ShowCharacterState ();
	}
	else
	{
		MessageBeep (MB_OK);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnSabotageNum()
{
	UpdateData (TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnControlMode()
{
	ShowControlMode ();
}

void CSabotageTestDlg::OnSabotageMode()
{
	ShowSabotageMode ();
}

void CSabotageTestDlg::OnCharacterCount()
{
	if	(mUseServer.GetCheck())
	{
		FreeServerCharacters (-1);
		LoadServerCharacters (-1);
	}
	if	(mUseControl.GetCheck())
	{
		FreeControlCharacters (-1);
		LoadControlCharacters (-1);
	}
}

void CSabotageTestDlg::OnSoundOn()
{
	INT_PTR	lNdx;

	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mServerCharacter[lNdx])
		{
			mServerCharacter[lNdx].Style ^= CharacterStyle_SoundEffects;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx])
		{
			mControlCharacter[lNdx].SoundEffectsEnabled = !mControlCharacter[lNdx].SoundEffectsEnabled;
		}
	}
	ShowCharacterState ();
}

void CSabotageTestDlg::OnIdleOn()
{
	INT_PTR	lNdx;

	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mServerCharacter[lNdx])
		{
			mServerCharacter[lNdx].Style ^= CharacterStyle_IdleEnabled;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx])
		{
			mControlCharacter[lNdx].IdleEnabled = !mControlCharacter[lNdx].IdleEnabled;
		}
	}
	ShowCharacterState ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if	(
			(mRepeatTimer)
		&&	(nIDEvent == mRepeatTimer)
		)
	{
		if	(
				(IsCharacterVisible())
			&&	(!IsAnimating())
			)
		{
			ShowSelectedGesture (false);
		}
	}

	if	(
			(mCycleTimer)
		&&	(nIDEvent == mCycleTimer)
		)
	{
		if	(
				(!GetQueueStatus (QS_INPUT))
			&&	(IsCharacterVisible())
			&&	(!IsAnimating ())
			)
		{
			if	(mSabotageNum == sSabotageMoveEvent)	
			{
				OnMoveButton ();
			}
			else
			if	(
					(mSabotageNum == sSabotageSpeakStartEvent)	
				||	(mSabotageNum == sSabotageSpeakEndEvent)	
				)
			{
				OnSpeakButton ();
			}
			else
			if	(
					(mSabotageNum == sSabotageThinkStartEvent)	
				||	(mSabotageNum == sSabotageThinkEndEvent)	
				)
			{
				OnThinkButton ();
			}
			else
			if	(
					(mSabotageNum == sSabotageListenStartEvent)	
				||	(mSabotageNum == sSabotageListenEndEvent)	
				)
			{
				OnListenButton ();
			}
		}
#if	FALSE	
		if	(
				(!GetQueueStatus (QS_INPUT))
			&&	(!IsAnimating ())
			&&	(mCharacterList.GetItemCount() > 0)
			)
		{
			do
			{
				while	(++mSelCharacterNdx < mCharacterList.GetItemCount ())
				{
					PrimeMessagePump ();
					if	(SelectCharacter (mSelCharacterNdx, true))
					{
						break;
					}
					if	(!mCycleTimer)
					{
						break;
					}
				}
				if	(!mCycleTimer)
				{
					break;
				}
			}
			while	(mSelCharacterNdx < mCharacterList.GetItemCount());
		}

		if	(
				(mCycleTimer)
			&&	(mCharacterList.GetItemCount() <= 0)
			)
		{
			KillTimer (mCycleTimer);
			mCycleTimer = 0;
			Stop ();
		}

		if	(
				(mCycleTimer)
			&&	(mCharacterList.GetItemCount() > 0)
			&&	(mSelCharacterNdx >= mCharacterList.GetItemCount()-1)
			)
		{
			PrimeMessagePump ();
			mSelCharacterNdx = -1;
		}
#endif		
	}

	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	if	(bActive)
	{
		INT_PTR	lNdx;

		for	(lNdx = 0; lNdx < (INT_PTR)mServerCharacter.GetCount(); lNdx++)
		{
			if	(
					(mServerCharacter[lNdx])
				&&	(mServerCharacter[lNdx].Visible)
				)
			{
				LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mServerCharacterId[lNdx], bActive, mServerCharacter[lNdx].ActiveState, mServerCharacter[lNdx].OtherClientCount);
				LogComErr (_LOG_CHAR_CALLS, mServerCharacter[lNdx]->put_ActiveState (ActiveState_Active), _T("[%d] put_ActiveState ActiveState_Active"), mServerCharacterId[lNdx]);
				LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd]"), mServerCharacterId[lNdx], bActive, mServerCharacter[lNdx].ActiveState);
				break;
			}
		}

		for	(lNdx = 0; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
		{
			if	(
					(mControlCharacter[lNdx])
				&&	(mControlCharacter[lNdx].Visible)
				&&	(mControlCharacterId[lNdx].CompareNoCase(sControlCharacterIdPopup)==0)
				)
			{
				LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%s] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mControlCharacterId[lNdx], bActive, mControlCharacter[lNdx].ActiveState, mControlCharacter[lNdx].HasOtherClients);
				LogComErr (_LOG_CHAR_CALLS, mControlCharacter[lNdx]->Activate (_variant_t((short)ActiveState_Active,VT_I2), NULL), _T("[%s] put_ActiveState ActiveState_Active"), mControlCharacterId[lNdx]);
				LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%s] ActivateApp [%u] Active [%hd]"), mControlCharacterId[lNdx], bActive, mControlCharacter[lNdx].ActiveState);
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Command"), pThis->GetServerCharacterId(pThis->mExitCommandId.Find(CommandID)), pThis->m_dwRef);
#endif
	if	(pThis->mSabotageNum == sSabotageCommandEvent)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(pThis->mExitCommandId.Find (CommandID) >= 0)
	{
		PostQuitMessage (0);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ActivateInputState (long CharacterID, long Activated)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Activated);
#endif
	if	(
			(
				(Activated == ActiveState_Active)
			||	(Activated == ActiveState_InputActive)
			)
		&&	(pThis->mSabotageNum == sSabotageActivateEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Restart"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Shutdown"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	VisibilityCauseType lCause = (VisibilityCauseType)-1;
	INT_PTR				lNdx = pThis->mServerCharacterId.Find (CharacterID);
	if	(
			(lNdx >= 0)
		&&	(pThis->mServerCharacter[lNdx] != NULL)
		)
	{
		pThis->mServerCharacter[lNdx]->get_VisibilityCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->GetServerCharacterId(lNdx), pThis->m_dwRef, CharacterID, Visible, Cause, lCause);
#endif
	if	(
			(Visible)
		&&	(pThis->mSabotageNum == sSabotageShowEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(!Visible)
		&&	(pThis->mSabotageNum == sSabotageHideEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	{
		pThis->ShowCharacterState ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Click (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(pThis->mSabotageNum == sSabotageClickEvent)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::DblClick (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(pThis->mSabotageNum == sSabotageDblClickEvent)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::DragStart (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(pThis->mSabotageNum == sSabotageDragStartEvent)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::DragComplete (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(pThis->mSabotageNum == sSabotageDragEndEvent)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::RequestStart (long RequestID)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mServerCharacterId[0], pThis->m_dwRef, RequestID);
#endif
	if	(
			(pThis->mPlayReqID.Find (RequestID) >= 0)
		&&	(pThis->mSabotageNum == sSabotageAnimateStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(pThis->mSpeakReqID.Find (RequestID) >= 0)
		&&	(pThis->mSabotageNum == sSabotageSpeakStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(pThis->mThinkReqID.Find (RequestID) >= 0)
		&&	(pThis->mSabotageNum == sSabotageThinkStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::RequestComplete (long RequestID, long Result)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mServerCharacterId[0], pThis->m_dwRef, RequestID, Result);
#endif
	if	(
			(pThis->mPlayReqID.Find (RequestID) >= 0)
		&&	(pThis->mSabotageNum == sSabotageAnimateEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(pThis->mSpeakReqID.Find (RequestID) >= 0)
		&&	(pThis->mSabotageNum == sSabotageSpeakEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(pThis->mThinkReqID.Find (RequestID) >= 0)
		&&	(pThis->mSabotageNum == sSabotageThinkEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	
	INT_PTR	lReqNdx;

	if	((lReqNdx = pThis->mShowReqID.Find (RequestID)) >= 0)
	{
		pThis->mShowReqID[lReqNdx] = 0;
	}
	if	((lReqNdx = pThis->mPlayReqID.Find (RequestID)) >= 0)
	{
		pThis->mPlayReqID[lReqNdx] = 0;
	}
	if	((lReqNdx = pThis->mMoveReqID.Find (RequestID)) >= 0)
	{
		pThis->mMoveReqID[lReqNdx] = 0;
	}
	if	((lReqNdx = pThis->mSpeakReqID.Find (RequestID)) >= 0)
	{
		pThis->mSpeakReqID[lReqNdx] = 0;
	}
	if	((lReqNdx = pThis->mThinkReqID.Find (RequestID)) >= 0)
	{
		pThis->mThinkReqID[lReqNdx] = 0;
	}
	if	(
			((lReqNdx = pThis->mLoadReqID.Find (RequestID)) >= 0)
		&&	(pThis->LoadedServerCharacter (lReqNdx))
		&&	(
				(pThis->mCycleTimer)
			||	(pThis->mShowButton.GetCheck())
			)
		)
	{
		pThis->ShowServerCharacters ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::BookMark (long BookMarkID)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->GetServerCharacterId(0), pThis->m_dwRef, BookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Start);
#endif
	if	(
			(Start)
		&&	(pThis->mSabotageNum == sSabotageIdleStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(!Start)
		&&	(pThis->mSabotageNum == sSabotageIdleEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	{
		pThis->ShowCharacterState ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Move (long CharacterID, long x, long y, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	MoveCauseType	lCause = (MoveCauseType)-1;
	INT_PTR			lNdx = pThis->mServerCharacterId.Find (CharacterID);
	if	(
			(lNdx >= 0)
		&&	(pThis->mServerCharacter[lNdx])
		)
	{
		pThis->mServerCharacter[lNdx]->get_MoveCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->GetServerCharacterId(lNdx), pThis->m_dwRef, CharacterID, x, y, Cause, lCause);
#endif
	INT_PTR	lReqNdx = pThis->mServerCharacterId.Find (CharacterID);

	if	(
			(lReqNdx >= 0)
		&&	(lReqNdx < (INT_PTR)pThis->mMoveReqID.GetCount())
		&&	(pThis->mMoveReqID[lReqNdx] > 0)
		&&	(pThis->mSabotageNum == sSabotageMoveEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Size (long CharacterID, long Width, long Height)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Width, Height);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Visible);
#endif
	if	(
			(Visible)
		&&	(pThis->mSabotageNum == sSabotageBalloonShowEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(!Visible)
		&&	(pThis->mSabotageNum == sSabotageBalloonHideEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	{
		pThis->ShowCharacterState ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ListeningState [%d] [%d] [%d]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Listening, Cause);
#endif
	if	(
			(Listening)
		&&	(pThis->mSabotageNum == sSabotageListenStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(!Listening)
		&&	(pThis->mSabotageNum == sSabotageListenEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	{
		pThis->ShowCharacterState ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR CharGUID)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->GetServerCharacterId(0), pThis->m_dwRef, CharGUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->GetServerCharacterId(pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Status);
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnCtlActivateInput(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlActivateInput [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
	if	(mSabotageNum == sSabotageActivateEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDeactivateInput(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDeactivateInput [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
}

void CSabotageTestDlg::OnCtlClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlClick [%s] [%d] [%d] [%d %d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageClickEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDblClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDblClick [%s] [%d] [%d] [%d %d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageDblClickEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDragStart (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDragStart [%s] [%d] [%d] [%d %d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageDragStartEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDragComplete (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDragComplete [%s] [%d] [%d] [%d %d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageDragEndEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlShow (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlShow [%s] [%d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Cause);
#endif
	if	(mSabotageNum == sSabotageShowEvent)
	{
		SabotageEvent ();
	}
	else
	{
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnCtlHide (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlHide [%s] [%d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Cause);
#endif
	if	(mSabotageNum == sSabotageHideEvent)
	{
		SabotageEvent ();
	}
	else
	{
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnCtlRequestStart (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr	lRequest (Request);
	bool				lRequestFound = false;
	INT_PTR				lNdx;
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlRequestStart [%d] [%d] [%8.8X] [%ls]"), GetControlCharacterId(0), lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
#endif
	if	(mSabotageNum == sSabotageAnimateStartEvent)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mPlayRequest.GetCount(); lNdx++)
		{
			if	(
					(mPlayRequest[lNdx])
				&&	(mPlayRequest[lNdx].ID == lRequest.ID)
				)
			{
				lRequestFound = true;
				break;
			}
		}
	}
	else
	if	(mSabotageNum == sSabotageSpeakStartEvent)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mSpeakRequest.GetCount(); lNdx++)
		{
			if	(
					(mSpeakRequest[lNdx])
				&&	(mSpeakRequest[lNdx].ID == lRequest.ID)
				)
			{
				lRequestFound = true;
				break;
			}
		}
	}
	else
	if	(mSabotageNum == sSabotageThinkStartEvent)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mThinkRequest.GetCount(); lNdx++)
		{
			if	(
					(mThinkRequest[lNdx])
				&&	(mThinkRequest[lNdx].ID == lRequest.ID)
				)
			{
				lRequestFound = true;
				break;
			}
		}
	}
	if	(lRequestFound)		
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlRequestComplete (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr	lRequest (Request);
	bool				lRequestFound = false;
	INT_PTR				lNdx;
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlRequestComplete [%d] [%d] [%8.8X] [%ls]"), GetControlCharacterId(0), lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
#endif
	if	(mSabotageNum == sSabotageAnimateEndEvent)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mPlayRequest.GetCount(); lNdx++)
		{
			if	(
					(mPlayRequest[lNdx])
				&&	(mPlayRequest[lNdx].ID == lRequest.ID)
				)
			{
				lRequestFound = true;
				break;
			}
		}
	}
	else
	if	(mSabotageNum == sSabotageSpeakEndEvent)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mSpeakRequest.GetCount(); lNdx++)
		{
			if	(
					(mSpeakRequest[lNdx])
				&&	(mSpeakRequest[lNdx].ID == lRequest.ID)
				)
			{
				lRequestFound = true;
				break;
			}
		}
	}
	else
	if	(mSabotageNum == sSabotageThinkEndEvent)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mThinkRequest.GetCount(); lNdx++)
		{
			if	(
					(mThinkRequest[lNdx])
				&&	(mThinkRequest[lNdx].ID == lRequest.ID)
				)
			{
				lRequestFound = true;
				break;
			}
		}
	}

	for	(lNdx = 0; lNdx < (INT_PTR)mLoadRequest.GetCount(); lNdx++)
	{
		if	(
				(mLoadRequest[lNdx])
			&&	(mLoadRequest[lNdx].ID == lRequest.ID)
			)
		{
			mLoadRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mShowRequest.GetCount(); lNdx++)
	{
		if	(
				(mShowRequest[lNdx])
			&&	(mShowRequest[lNdx].ID == lRequest.ID)
			)
		{
			mShowRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mPlayRequest.GetCount(); lNdx++)
	{
		if	(
				(mPlayRequest[lNdx])
			&&	(mPlayRequest[lNdx].ID == lRequest.ID)
			)
		{
			mPlayRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mMoveRequest.GetCount(); lNdx++)
	{
		if	(
				(mMoveRequest[lNdx])
			&&	(mMoveRequest[lNdx].ID == lRequest.ID)
			)
		{
			mMoveRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mSpeakRequest.GetCount(); lNdx++)
	{
		if	(
				(mSpeakRequest[lNdx])
			&&	(mSpeakRequest[lNdx].ID == lRequest.ID)
			)
		{
			mSpeakRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (INT_PTR)mThinkRequest.GetCount(); lNdx++)
	{
		if	(
				(mThinkRequest[lNdx])
			&&	(mThinkRequest[lNdx].ID == lRequest.ID)
			)
		{
			mThinkRequest[lNdx] = NULL;
		}
	}

	if	(lRequestFound)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlCommand (IDaCtlUserInput* UserInput)
{
	IDaCtlUserInputPtr lUserInput (UserInput);
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlCommand [%ls] [%ls]"), GetControlCharacterId(0), (BSTR)lUserInput.CharacterID, (BSTR)lUserInput.Name);
#endif
	if	(mSabotageNum == sSabotageCommandEvent)
	{
		SabotageEvent ();
	}
	else
	if	(FindString (mExitCommandName, CString ((BSTR)lUserInput.Name)) >= 0)
	{
		PostQuitMessage (0);
	}
}

void CSabotageTestDlg::OnCtlIdleStart (LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlIdleStart [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
	if	(mSabotageNum == sSabotageIdleStartEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlIdleComplete (LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlIdleComplete [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
	if	(mSabotageNum == sSabotageIdleEndEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlMove (LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlMove [%s] [%d %d] [%d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, X, Y, Cause);
#endif
	if	(mSabotageNum == sSabotageMoveEvent)
	{
		INT_PTR	lNdx;
		
		for	(lNdx = 0; lNdx < (INT_PTR)mMoveRequest.GetCount(); lNdx++)
		{
			if	(mMoveRequest[lNdx])
			{
				SabotageEvent ();
				break;
			}
		}
	}
}

void CSabotageTestDlg::OnCtlSize (LPCTSTR CharacterID, short Width, short Height)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlSize [%s] [%d %d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Width, Height);
#endif
}

void CSabotageTestDlg::OnCtlBalloonShow(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlBalloonShow [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
	if	(mSabotageNum == sSabotageBalloonShowEvent)
	{
		SabotageEvent ();
	}
	else
	{
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnCtlBalloonHide(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlBalloonHide [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
	if	(mSabotageNum == sSabotageBalloonHideEvent)
	{
		SabotageEvent ();
	}
	else
	{
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnCtlListenStart(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlListenStart [%s]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID);
#endif
	if	(mSabotageNum == sSabotageListenStartEvent)
	{
		SabotageEvent ();
	}
	else
	{
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnCtlListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlListenComplete [%s] [%d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Cause);
#endif
	if	(mSabotageNum == sSabotageListenEndEvent)
	{
		SabotageEvent ();
	}
	else
	{
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnCtlActiveClientChange (LPCTSTR CharacterID, BOOL Active)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlActiveClientChange [%s] [%d]"), GetControlCharacterId(FindString(mControlCharacterId,CharacterID)), CharacterID, Active);
#endif
}
