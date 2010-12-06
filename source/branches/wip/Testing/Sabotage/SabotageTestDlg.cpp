#include "StdAfx.h"
#include <shlwapi.h>
#include "SabotageTest.h"
#include "SabotageTestDlg.h"
#include "AgentPreviewWnd.h"
#include "UiState.h"
#include "NotifyLock.h"
#include "GuidStr.h"
#include "StringArrayEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal|LogHighVolume
#define	_LOG_AGENT_CALLS	LogAlways|LogTimeMs|LogHighVolume
#define	_LOG_CHAR_CALLS		LogAlways|LogTimeMs|LogHighVolume
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
static LPCTSTR	sProfileUseControl = _T("UseControl");
static LPCTSTR	sProfileControlContained = _T("ControlContained");
static LPCTSTR	sProfileControlStandalone = _T("ControlStandalone");
static LPCTSTR	sProfileUseServer = _T("UseServer");
static LPCTSTR	sProfileSoundOn = _T("Sound");
static LPCTSTR	sProfileIdleOn = _T("Idle");
static LPCTSTR	sProfileGesture = _T("Gesture");
static LPCTSTR	sProfileSabotage = _T("Sabotage");

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
	mCharacterNdx (-1),
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
	FreeServerCharacter ();
	FreeAgentServer ();
	FreeControlCharacter ();
	FreeAgentControl ();
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
	DDX_Control(pDX, IDC_USE_SERVER, mUseServer);
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

	GetAgentServer ();
	ShowCharacters ();
	FreeAgentServer ();

	LoadConfig ();
	lAutoStart = CommandLineConfig ();
	ShowControlMode ();
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
	ShowCharacterState ();

	if	(mServerCharacter)
	{
		LogMessage (LogNormal, _T("FreeServerCharacter"));
		FreeServerCharacter ();
		LogMessage (LogNormal, _T("FreeServerCharacter Done"));

		ShowCharacterState ();
	}

	if	(mServer)
	{
		LogMessage (LogNormal, _T("FreeAgentServer"));
		FreeAgentServer ();
		LogMessage (LogNormal, _T("FreeAgentServer Done"));

		ShowCharacterState ();
	}

	if	(mControlCharacter)
	{
		LogMessage (LogNormal, _T("FreeControlCharacter"));
		FreeControlCharacter ();
		LogMessage (LogNormal, _T("FreeControlCharacter Done"));

		ShowCharacterState ();
	}

	//if	(mControl)
	//{
	//	LogMessage (LogNormal, _T("FreeAgentControl"));
	//	FreeAgentControl ();
	//	LogMessage (LogNormal, _T("FreeAgentControl Done"));

	//	ShowCharacterState ();
	//}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::ShowModeSelection ()
{
	if	(
			(mServerCharacter)
		||	(mControlCharacter)
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
			(mServer != NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer->get_CharacterFiles (&lCharacterFiles))))
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
		mCharacterNdx = pCharacterNdx;
		{
			CNotifyLock	lLock (m_hWnd);
			mCharacterList.SetItemState (mCharacterNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		mCharacterList.EnsureVisible (mCharacterNdx, FALSE);
		mCharacterList.RedrawWindow ();

		if	(pShow)
		{
			lRet = ShowCharacter (mCharacterList.GetItemText (mCharacterNdx, 0));
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

	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
	{
		FreeServerCharacter ();
//		FreeAgentServer ();
		FreeControlCharacter ();
		CharacterIsVisible (false);
	}

	ShowControlMode ();

	if	(
			(pCharacterPath)
		&&	(pCharacterPath[0])
		)
	{
		mCharacterPath = pCharacterPath;

		if	(
				(mUseServer.GetCheck())
			&&	(LoadServerCharacter ())
			)
		{
			lRet = true;
		}
		if	(
				(mUseControl.GetCheck())
			&&	(LoadControlCharacter ())
			)
		{
			lRet = true;
		}
	}
	else
	{
		mCharacterPath.Empty ();
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
						(mServerCharacter != NULL)
					&&	(SUCCEEDED (LogComErr (LogNormal, mServerCharacter->get_Animations (lAnimations.Free()))))
					)
				||	(
						(mControlCharacter != NULL)
					&&	(SUCCEEDED (LogComErr (LogNormal, mControlCharacter->get_Animations (lAnimations.Free()))))
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
			if	(mServerCharacter)
			{
				if	(mPlayReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mServerCharacter->Stop (mPlayReqID));
					mPlayReqID = 0;
				}
				if	(mMoveReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mServerCharacter->Stop (mMoveReqID));
					mMoveReqID = 0;
				}
				if	(mSpeakReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mServerCharacter->Stop (mSpeakReqID));
					mSpeakReqID = 0;
				}
				if	(mThinkReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mServerCharacter->Stop (mThinkReqID));
					mThinkReqID = 0;
				}
			}
			if	(mControlCharacter)
			{
				if	(mPlayRequest)
				{
					LogComErr (_LOG_CHAR_CALLS, mControlCharacter->Stop (_variant_t((LPDISPATCH)mPlayRequest.GetInterfacePtr())));
					mPlayRequest = NULL;
				}
				if	(mMoveRequest)
				{
					LogComErr (_LOG_CHAR_CALLS, mControlCharacter->Stop (_variant_t((LPDISPATCH)mMoveRequest.GetInterfacePtr())));
					mMoveRequest = NULL;
				}
				if	(mSpeakRequest)
				{
					LogComErr (_LOG_CHAR_CALLS, mControlCharacter->Stop (_variant_t((LPDISPATCH)mSpeakRequest.GetInterfacePtr())));
					mSpeakRequest = NULL;
				}
				if	(mThinkRequest)
				{
					LogComErr (_LOG_CHAR_CALLS, mControlCharacter->Stop (_variant_t((LPDISPATCH)mThinkRequest.GetInterfacePtr())));
					mThinkRequest = NULL;
				}
			}
		}

		if	(mServerCharacter)
		{
			lResult = mServerCharacter->Play (_bstr_t(lSelGesture), &mPlayReqID);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Play [%s] [%d]"), mServerCharacterId, lSelGesture, mPlayReqID)))
			{
				lRet = true;
			}
		}

		if	(mControlCharacter)
		{
			lResult = mControlCharacter->Play (_bstr_t(lSelGesture), &mPlayRequest);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Play [%s] [%d]"), mControlCharacterId, lSelGesture, mPlayRequest.ID)))
			{
				lRet = true;
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
	if	(
			(mServerCharacter != NULL)
		&&	(
				(mLoadReqID > 0)
			||	(mShowReqID > 0)
			||	(mPlayReqID > 0)
			||	(mMoveReqID > 0)
			||	(mSpeakReqID > 0)
			||	(mThinkReqID > 0)
			)
		)
	{
		return true;
	}

	if	(
			(mControlCharacter != NULL)
		&&	(
				(
					(mLoadRequest != NULL)
				&&	(
						(mLoadRequest.Status == RequestStatus_Pending)
					||	(mLoadRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mShowRequest != NULL)
				&&	(
						(mShowRequest.Status == RequestStatus_Pending)
					||	(mShowRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mPlayRequest != NULL)
				&&	(
						(mPlayRequest.Status == RequestStatus_Pending)
					||	(mPlayRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mMoveRequest != NULL)
				&&	(
						(mMoveRequest.Status == RequestStatus_Pending)
					||	(mMoveRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mSpeakRequest != NULL)
				&&	(
						(mSpeakRequest.Status == RequestStatus_Pending)
					||	(mSpeakRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mThinkRequest != NULL)
				&&	(
						(mThinkRequest.Status == RequestStatus_Pending)
					||	(mThinkRequest.Status == RequestStatus_InProgress)
					)
				)
			)
		)
	{
		return true;
	}
	return false;
}

bool CSabotageTestDlg::Stop ()
{
	bool	lRet = false;

	if	(mRepeatTimer)
	{
		KillTimer (mRepeatTimer);
		mRepeatTimer = 0;
		lRet = true;
	}

	if	(
			(mServerCharacter != NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mServerCharacter->StopAll (StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility), _T("[%d] StopAll"), mServerCharacterId)))
		)
	{
		lRet = true;
	}
	mShowReqID = 0;
	mPlayReqID = 0;
	mMoveReqID = 0;
	mSpeakReqID = 0;
	mThinkReqID = 0;

	if	(
			(mControlCharacter != NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mControlCharacter->StopAll (_variant_t((long)(StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility),VT_I4)), _T("[%s] StopAll"), mControlCharacterId)))
		)
	{
		lRet = true;
	}
	mShowRequest = NULL;
	mPlayRequest = NULL;
	mMoveRequest = NULL;
	mSpeakRequest = NULL;
	mThinkRequest = NULL;

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

void CSabotageTestDlg::GetAgentServer ()
{
	if	(mServer == NULL)
	{
		IUnknownPtr	lUnknown;

		LogComErr (_LOG_AGENT_CALLS, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("CoCreateInstance"));
		mServer = lUnknown;

		if	(mServer != NULL)
		{
			mServer.CharacterStyle |= CharacterStyle_Smoothed;
			mServer.CharacterStyle &= ~CharacterStyle_IconShown;

			LogComErr (_LOG_AGENT_CALLS, mServer->Register (&m_xDaSvrNotifySink, &mNotifySinkId), _T("Register"));
		}
	}
}

void CSabotageTestDlg::FreeAgentServer ()
{
	if	(
			(mNotifySinkId)
		&&	(mServer != NULL)
		)
	{
		try
		{
			LogComErr (_LOG_AGENT_CALLS, mServer->Unregister (mNotifySinkId), _T("Unregister"));
		}
		catch AnyExceptionSilent
	}

	mNotifySinkId = 0;
	SafeFreeSafePtr (mServer);
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::LoadServerCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	GetAgentServer ();
	mLoadReqID = 0;

	if	(
			(mServer != NULL)
		&&	(mServerCharacterId == 0)
		&&	(!mCharacterPath.IsEmpty ())
		)
	{
		mExitCommandId = 0;
		lResult = mServer->Load (_variant_t(mCharacterPath), &mServerCharacterId, &mLoadReqID);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("Load [%d] [%s] as [%d]"), mLoadReqID, mCharacterPath, mServerCharacterId);
	}

	if	(
			(lRet = LoadedServerCharacter ())
		&&	(
				(mCycleTimer)
			||	(mShowButton.GetCheck())
			)
		)
	{
		lRet = ShowServerCharacter ();
	}
	return lRet;
}

bool CSabotageTestDlg::LoadedServerCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	CPoint	lCharPos;

	if	(
			(mServer != NULL)
		&&	(mServerCharacterId != 0)
		&&	(mServerCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer->get_Character (mServerCharacterId, &mServerCharacter), _T("GetCharacterEx"))))
		)
	{
		if	(mIdleOn.GetCheck())
		{
			mServerCharacter.Style |= CharacterStyle_IdleEnabled;
		}
		else
		{
			mServerCharacter.Style &= ~CharacterStyle_IdleEnabled;
		}
		if	(mSoundOn.GetCheck())
		{
			mServerCharacter.Style |= CharacterStyle_SoundEffects;
		}
		else
		{
			mServerCharacter.Style &= ~CharacterStyle_SoundEffects;
		}
	}

	ShowGestures ();

	if	(mServerCharacter != NULL)
	{
		CPoint	lCharPos;
		CSize	lCharSize;

		if	(
				(SUCCEEDED (mServerCharacter->GetSize (&lCharSize.cx, &lCharSize.cy)))
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
			mServerCharacter->SetSize (lCharSize.cx, lCharSize.cy);
		}

		if	(!mServerCharacter.Visible)
		{
			int	lSabotageNum = mSabotageNum;

			mSabotageNum = -1;
			lCharPos = GetInitialPos (lCharSize);
			mServerCharacter->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
			mSabotageNum = lSabotageNum;
		}
	}

	if	(mServerCharacter != NULL)
	{
		IDaSvrCommands2Ptr	lCommands (mServerCharacter);

		lCommands.Caption = _bstr_t("Sabotage");
		lCommands.VoiceGrammar = _bstr_t("sabotage");
		lCommands.GlobalVoiceCommandsEnabled = true;

		if	(mExitCommandId == 0)
		{
			lCommands->AddEx (_bstr_t("Exit"), _bstr_t("exit"), _bstr_t("Exit"), TRUE, TRUE, 0, &mExitCommandId);
		}
	}

	if	(mServerCharacter != NULL)
	{
		lRet = true;
	}
	mLoadReqID = 0;

	return lRet;
}

bool CSabotageTestDlg::FreeServerCharacter ()
{
	bool	lRet = false;
	CPoint	lCharPos;

	if	(mServerCharacter != NULL)
	{
		mServerCharacter = NULL;

		if	(
				(mServer != NULL)
			&&	(mServerCharacterId != 0)
			)
		{
			LogComErr (_LOG_AGENT_CALLS, mServer->Unload (mServerCharacterId), _T("Unload [%d]"), mServerCharacterId);
		}

		mServerCharacterId = 0;
		lRet = true;
	}

	mLoadReqID = 0;
	mShowReqID = 0;
	mPlayReqID = 0;
	mMoveReqID = 0;
	mSpeakReqID = 0;
	mThinkReqID = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::ShowServerCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	if	(mServerCharacter != NULL)
	{
		mShowReqID = 0;
		lResult = mServerCharacter->Show (FALSE, &mShowReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mServerCharacterId, mShowReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSabotageTestDlg::HideServerCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	if	(mServerCharacter != NULL)
	{
		mShowReqID = 0;
		lResult = mServerCharacter->Hide (FALSE, &mShowReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mServerCharacterId, mShowReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::GetAgentControl ()
{
	CWnd *	lPlacer;
	if	(
			(lPlacer = GetDlgItem (IDC_CONTROL))
		&&	(lPlacer->GetSafeHwnd() != mControlWnd.m_hWnd)
		)
	{
		lPlacer->GetWindowRect (mControlRect);
		lPlacer->DestroyWindow();
		ScreenToClient (mControlRect);
	}

	if	(
			(mControlWnd.m_hWnd == NULL)
		&&	(mControlWnd.CreateControl (__uuidof(DaControl), _T(""), WS_CHILD|WS_CLIPSIBLINGS, mControlRect, this, IDC_CONTROL))
		)
	{
		mControl = mControlWnd.GetControlSite()->m_pObject;
	}
	if	(mControl)
	{
		mControl.CharacterStyle |= CharacterStyle_Smoothed;
		mControl.CharacterStyle &= ~CharacterStyle_IconShown;

		if	(
				(mControlContained.GetCheck ())
			||	(mControlStandalone.GetCheck ())
			)
		{
			mControl.AutoConnect = false;
			mControl.Connected = false;
		}
		else
		{
			mControl.AutoConnect = true;
		}
	}
}

void CSabotageTestDlg::FreeAgentControl ()
{
	SafeFreeSafePtr (mControl);
	if	(mControlWnd.m_hWnd)
	{
		mControlWnd.DestroyWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::LoadControlCharacter ()
{
	bool					lRet = false;
	HRESULT					lResult;
	IDaCtlCharacters2Ptr	lCharacters;

	GetAgentControl ();
	mLoadRequest = NULL;

	if	(
			(mControl != NULL)
		&&	(mControlCharacterId.IsEmpty())
		&&	(!mCharacterPath.IsEmpty ())
		&&	(lCharacters = mControl.Characters)
		)
	{
		if	(mControlContained.GetCheck())
		{
			mControlCharacterId = sControlCharacterIdContained;
		}
		else
		{
			mControlCharacterId = sControlCharacterIdPopup;
		}

		mExitCommandName.Empty();
		lResult = lCharacters->Load (_bstr_t(mControlCharacterId), _variant_t(mCharacterPath), &mLoadRequest);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("Load [%d] [%s] as [%s]"), mLoadRequest.ID, mCharacterPath, mControlCharacterId);
	}

	if	(lRet = LoadedControlCharacter ())
	{
		if	(
				(mCycleTimer)
			||	(mShowButton.GetCheck())
			)
		{
			lRet = ShowControlCharacter ();
		}
	}
	else
	{
		SafeFreeSafePtr (mControlCharacter);
		mControlCharacterId.Empty ();
	}

	return lRet;
}

bool CSabotageTestDlg::LoadedControlCharacter ()
{
	bool					lRet = false;
	IDaCtlCharacters2Ptr	lCharacters;

	if	(
			(mControl != NULL)
		&&	(!mControlCharacterId.IsEmpty())
		&&	(mControlCharacter == NULL)
		&&	(lCharacters = mControl.Characters)
		&&	(mControlCharacter = lCharacters.Item [_bstr_t(mControlCharacterId)])
		)
	{
		mControlCharacter.IdleEnabled = mIdleOn.GetCheck() ? true : false;
		mControlCharacter.SoundEffectsEnabled = mSoundOn.GetCheck() ? true : false;
	}

	ShowGestures ();

	if	(mControlCharacter != NULL)
	{
		CSize	lCharSize (mControlCharacter.Width, mControlCharacter.Height);
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
			mControlCharacter.Width = (short)lCharSize.cx;
			mControlCharacter.Height = (short)lCharSize.cy;
		}

		if	(
				(mControlCharacterId.CompareNoCase (sControlCharacterIdPopup) == 0)
			&&	(!mControlCharacter.Visible)
			)
		{
			int	lSabotageNum = mSabotageNum;

			mSabotageNum = -1;
			lCharPos = GetInitialPos (lCharSize);
			mControlCharacter.Left = (short)lCharPos.x;
			mControlCharacter.Top = (short)lCharPos.y;
			mSabotageNum = lSabotageNum;
		}

		if	(mControlContained.GetCheck())
		{
			mControl.ControlCharacter = mControlCharacter;
			mControlWnd.ShowWindow (SW_SHOWNA);
		}
		else
		{
			mControlWnd.ShowWindow (SW_HIDE);
		}
	}

	if	(mControlCharacter != NULL)
	{
		IDaCtlCommands2Ptr	lCommands (mControlCharacter.Commands);
		IDaCtlCommand2Ptr	lCommand;

		lCommands.Caption = _bstr_t("Sabotage");
		lCommands.VoiceCaption = _bstr_t("Sabotage");
		lCommands.VoiceGrammar = _bstr_t("sabotage");
		lCommands.GlobalVoiceCommandsEnabled = true;

		if	(mExitCommandName.IsEmpty())
		{
			mExitCommandName = _T("exit");
			lCommands->Add (_bstr_t(mExitCommandName), _variant_t(_bstr_t("Exit")), _variant_t(_bstr_t("exit")), _variant_t(VARIANT_TRUE,VT_BOOL), _variant_t(VARIANT_TRUE,VT_BOOL), &lCommand);
		}
	}

	if	(mControlCharacter != NULL)
	{
		lRet = true;
	}
	return lRet;
}

bool CSabotageTestDlg::FreeControlCharacter ()
{
	bool					lRet = false;
	IDaCtlCharacters2Ptr	lCharacters;

	if	(mControlCharacter != NULL)
	{
		lRet = true;
	}
	mControlCharacter = NULL;

	if	(
			(!mControlCharacterId.IsEmpty())
		&&	(mControl != NULL)
		&&	(lCharacters = mControl.Characters)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, lCharacters->Unload (_bstr_t(mControlCharacterId)))))
		)
	{
		lRet = true;
	}
	mControlCharacterId.Empty ();

	mLoadRequest = NULL;
	mShowRequest = NULL;
	mPlayRequest = NULL;
	mMoveRequest = NULL;
	mSpeakRequest = NULL;
	mThinkRequest = NULL;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::ShowControlCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	if	(mControlCharacter)
	{
		lResult = mControlCharacter->Show (_variant_t (VARIANT_FALSE,VT_BOOL), &mShowRequest);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Show [%d]"), mControlCharacterId, mShowRequest.ID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSabotageTestDlg::HideControlCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	if	(mControlCharacter)
	{
		lResult = mControlCharacter->Hide (_variant_t (VARIANT_FALSE,VT_BOOL), &mShowRequest);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Hide [%d]"), mControlCharacterId, mShowRequest.ID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::IsCharacterVisible ()
{
	if	(
			(
				(mServerCharacter != NULL)
			&&	(mServerCharacter.Visible)
			)
		||	(
				(mControlCharacter != NULL)
			&&	(mControlCharacter.Visible)
			)
		)
	{
		return true;
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

	if	(mServerCharacter != NULL)
	{
		mSoundOn.SetCheck ((mServerCharacter.Style & CharacterStyle_SoundEffects) ? TRUE : FALSE);
		mSoundOn.EnableWindow (TRUE);
	}
	else
	if	(mControlCharacter != NULL)
	{
		mSoundOn.SetCheck (mControlCharacter.SoundEffectsEnabled ? TRUE : FALSE);
		mSoundOn.EnableWindow (TRUE);
	}
	else
	{
		mSoundOn.EnableWindow (FALSE);
	}

	if	(mServerCharacter != NULL)
	{
		mIdleOn.SetCheck ((mServerCharacter.Style & CharacterStyle_IdleEnabled) ? TRUE : FALSE);
		mIdleOn.EnableWindow (TRUE);
	}
	else
	if	(mControlCharacter != NULL)
	{
		mIdleOn.SetCheck (mControlCharacter.IdleEnabled ? TRUE : FALSE);
		mIdleOn.EnableWindow (TRUE);
	}
	else
	{
		mIdleOn.EnableWindow (FALSE);
	}
}

void CSabotageTestDlg::ShowCharacterState ()
{
	if	(mServerCharacter != NULL)
	{
		mMoveButton.EnableWindow (TRUE);
		mCancelButton.EnableWindow (TRUE);
	}
	else
	if	(mControlCharacter != NULL)
	{
		mMoveButton.EnableWindow (mControlCharacterId.CompareNoCase(sControlCharacterIdPopup)==0);
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
	mUseServer.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileUseServer, mUseServer.GetCheck()) ? TRUE : FALSE);
	mSoundOn.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileSoundOn, mSoundOn.GetCheck()) ? TRUE : FALSE);
	mIdleOn.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIdleOn, mIdleOn.GetCheck()) ? TRUE : FALSE);

	mCharacterNdx = -1;
	mCharacterPath = lApp->GetProfileString (sProfileKey, sProfileCharacter);

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
	lApp->WriteProfileInt (sProfileKey, sProfileUseServer, mUseServer.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileSoundOn, mSoundOn.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIdleOn, mIdleOn.GetCheck());
	lApp->WriteProfileString (sProfileKey, sProfileCharacter, mCharacterPath);
	lApp->WriteProfileString (sProfileKey, sProfileGesture, GetSelectedGesture());

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
	FreeServerCharacter ();
	FreeAgentServer ();
	FreeControlCharacter ();
	FreeAgentControl ();

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

		if	(HideServerCharacter ())
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (false);
#endif
		}
		if	(HideControlCharacter ())
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (false);
#endif
		}
	}
	else
	{
		mShowButton.SetCheck (TRUE);

		if	(SelectCharacter (mCharacterNdx, true))
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

		if	(mServerCharacter)
		{
			lCharPos.x = mServerCharacter.Left;
			lCharPos.y = mServerCharacter.Top;
			lCharSize.cx = mServerCharacter.Width;
			lCharSize.cy = mServerCharacter.Height;
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

			mMoveReqID = 0;
			lResult = mServerCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top, 1000, &mMoveReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveTo [%d %d] [%d]"), mServerCharacterId, lCharRect.left, lCharRect.top, mMoveReqID);
		}

		if	(
				(mControlCharacter)
			&&	(mControlCharacterId.CompareNoCase(sControlCharacterIdPopup)==0)
			)
		{
			lCharPos.x = mControlCharacter.Left;
			lCharPos.y = mControlCharacter.Top;
			lCharSize.cx = mControlCharacter.Width;
			lCharSize.cy = mControlCharacter.Height;
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

			lResult = mControlCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top, _variant_t(), &mMoveRequest);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] MoveTo [%d %d] [%d]"), mControlCharacterId, lCharRect.left, lCharRect.top, mMoveRequest.ID);
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

		if	(mServerCharacter)
		{
			mSpeakReqID = 0;
			lResult = mServerCharacter->Speak (_bstr_t("Now is the winter of our discontent."), NULL, &mSpeakReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d]"), mServerCharacterId, mSpeakReqID);
		}

		if	(mControlCharacter)
		{
			lResult = mControlCharacter->Speak (_variant_t(_bstr_t("Now is the winter of our discontent.")), _variant_t(), &mSpeakRequest);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Speak [%d]"), mControlCharacterId, mSpeakRequest.ID);
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

		if	(mServerCharacter)
		{
			mThinkReqID = 0;
			lResult = mServerCharacter->Think (_bstr_t("Now is the winter of our discontent."), &mThinkReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d]"), mServerCharacterId, mThinkReqID);
		}

		if	(mControlCharacter)
		{
			lResult = mControlCharacter->Think (_bstr_t("Now is the winter of our discontent."), &mThinkRequest);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Think [%d]"), mControlCharacterId, mThinkRequest.ID);
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

		if	(mServerCharacter)
		{
			IDaSvrSettingsPtr	lSettings (mServer.Settings);
			long				lListen = (lSettings.AudioStatus != AudioStatus_CharacterListening);

			lResult = mServerCharacter->Listen (lListen);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Listen [%d]"), mServerCharacterId, lListen);
		}

		if	(mControlCharacter)
		{
			IDaCtlSettingsPtr	lSettings (mControl.Settings);
			VARIANT_BOOL		lListen = (lSettings.AudioStatus != AudioStatus_CharacterListening) ? VARIANT_TRUE : VARIANT_FALSE;
			VARIANT_BOOL		lListening = VARIANT_FALSE;

			lResult = mControlCharacter->Listen (lListen, &lListening);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%s] Listen [%d] [%d]"), mControlCharacterId, lListen, lListening);
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

void CSabotageTestDlg::OnSoundOn()
{
	if	(mServerCharacter)
	{
		mServerCharacter.Style ^= CharacterStyle_SoundEffects;
	}
	if	(mControlCharacter)
	{
		mControlCharacter.SoundEffectsEnabled = !mControlCharacter.SoundEffectsEnabled;
	}
	ShowCharacterState ();
}

void CSabotageTestDlg::OnIdleOn()
{
	if	(mServerCharacter)
	{
		mServerCharacter.Style ^= CharacterStyle_IdleEnabled;
	}
	if	(mControlCharacter)
	{
		mControlCharacter.IdleEnabled = !mControlCharacter.IdleEnabled;
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
			&&	(!IsAnimating ())
			&&	(mCharacterList.GetItemCount() > 0)
			)
		{
			do
			{
				while	(++mCharacterNdx < mCharacterList.GetItemCount ())
				{
					PrimeMessagePump ();
					if	(SelectCharacter (mCharacterNdx, true))
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
			while	(mCharacterNdx < mCharacterList.GetItemCount());
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
			&&	(mCharacterNdx >= mCharacterList.GetItemCount()-1)
			)
		{
			PrimeMessagePump ();
			mCharacterNdx = -1;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	if	(bActive)
	{
		if	(
				(mServerCharacter != NULL)
			&&	(mServerCharacter.Visible)
			)
		{
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mServerCharacterId, bActive, mServerCharacter.ActiveState, mServerCharacter.OtherClientCount);
			LogComErr (_LOG_CHAR_CALLS, mServerCharacter->put_ActiveState (ActiveState_Active), _T("[%d] put_ActiveState ActiveState_Active"), mServerCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd]"), mServerCharacterId, bActive, mServerCharacter.ActiveState);
		}

		if	(
				(mControlCharacter)
			&&	(mControlCharacter.Visible)
			&&	(mControlCharacterId.CompareNoCase(sControlCharacterIdPopup)==0)
			)
		{
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%s] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mControlCharacterId, bActive, mControlCharacter.ActiveState, mControlCharacter.HasOtherClients);
			LogComErr (_LOG_CHAR_CALLS, mControlCharacter->Activate (_variant_t((short)ActiveState_Active,VT_I2), NULL), _T("[%s] put_ActiveState ActiveState_Active"), mControlCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%s] ActivateApp [%u] Active [%hd]"), mControlCharacterId, bActive, mControlCharacter.ActiveState);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Command"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	if	(pThis->mSabotageNum == sSabotageCommandEvent)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(CommandID == pThis->mExitCommandId)
	{
		PostQuitMessage (0);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ActivateInputState (long CharacterID, long Activated)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Activated);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Restart"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Shutdown"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	VisibilityCauseType lCause = (VisibilityCauseType)-1;
	if	(pThis->mServerCharacter != NULL)
	{
		pThis->mServerCharacter->get_VisibilityCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Visible, Cause, lCause);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, RequestID);
#endif
	if	(
			(RequestID == pThis->mPlayReqID)
		&&	(pThis->mSabotageNum == sSabotageAnimateStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(RequestID == pThis->mSpeakReqID)
		&&	(pThis->mSabotageNum == sSabotageSpeakStartEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(RequestID == pThis->mThinkReqID)
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mServerCharacterId, pThis->m_dwRef, RequestID, Result);
#endif
	if	(
			(RequestID == pThis->mPlayReqID)
		&&	(pThis->mSabotageNum == sSabotageAnimateEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(RequestID == pThis->mSpeakReqID)
		&&	(pThis->mSabotageNum == sSabotageSpeakEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}
	else
	if	(
			(RequestID == pThis->mThinkReqID)
		&&	(pThis->mSabotageNum == sSabotageThinkEndEvent)
		)
	{
		pThis->SabotageEvent ();
	}

	if	(RequestID == pThis->mShowReqID)
	{
		pThis->mShowReqID = 0;
	}
	if	(RequestID == pThis->mPlayReqID)
	{
		pThis->mPlayReqID = 0;
	}
	if	(RequestID == pThis->mMoveReqID)
	{
		pThis->mMoveReqID = 0;
	}
	if	(RequestID == pThis->mSpeakReqID)
	{
		pThis->mSpeakReqID = 0;
	}
	if	(RequestID == pThis->mThinkReqID)
	{
		pThis->mThinkReqID = 0;
	}
	if	(
			(RequestID == pThis->mLoadReqID)
		&&	(pThis->LoadedServerCharacter ())
		&&	(
				(pThis->mCycleTimer)
			||	(pThis->mShowButton.GetCheck())
			)
		)
	{
		pThis->ShowControlCharacter ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::BookMark (long BookMarkID)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, BookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Start);
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
	MoveCauseType lCause = (MoveCauseType)-1;
	if	(pThis->mServerCharacter != NULL)
	{
		pThis->mServerCharacter->get_MoveCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, x, y, Cause, lCause);
#endif
	if	(
			(pThis->mMoveReqID > 0)
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Width, Height);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Visible);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ListeningState [%d] [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Listening, Cause);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mServerCharacterId, pThis->m_dwRef, CharGUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Status);
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnCtlActivateInput(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlActivateInput [%s]"), mControlCharacterId, CharacterID);
#endif
	if	(mSabotageNum == sSabotageActivateEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDeactivateInput(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDeactivateInput [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CSabotageTestDlg::OnCtlClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlClick [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageClickEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDblClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDblClick [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageDblClickEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDragStart (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDragStart [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageDragStartEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlDragComplete (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlDragComplete [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
	if	(mSabotageNum == sSabotageDragEndEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlShow (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlShow [%s] [%d]"), mControlCharacterId, CharacterID, Cause);
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
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlHide [%s] [%d]"), mControlCharacterId, CharacterID, Cause);
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
	IDaCtlRequestPtr lRequest (Request);
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlRequestStart [%d] [%d] [%8.8X] [%ls]"), mControlCharacterId, lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
#endif
	if	(
			(mPlayRequest)
		&&	(mPlayRequest.ID == lRequest.ID)
		&&	(mSabotageNum == sSabotageAnimateStartEvent)
		)
	{
		SabotageEvent ();
	}
	if	(
			(mSpeakRequest)
		&&	(mSpeakRequest.ID == lRequest.ID)
		&&	(mSabotageNum == sSabotageSpeakStartEvent)
		)
	{
		SabotageEvent ();
	}
	if	(
			(mThinkRequest)
		&&	(mThinkRequest.ID == lRequest.ID)
		&&	(mSabotageNum == sSabotageThinkStartEvent)
		)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlRequestComplete (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr lRequest (Request);
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlRequestComplete [%d] [%d] [%8.8X] [%ls]"), mControlCharacterId, lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
#endif
	if	(
			(mPlayRequest)
		&&	(mPlayRequest.ID == lRequest.ID)
		&&	(mSabotageNum == sSabotageAnimateEndEvent)
		)
	{
		SabotageEvent ();
	}
	if	(
			(mSpeakRequest)
		&&	(mSpeakRequest.ID == lRequest.ID)
		&&	(mSabotageNum == sSabotageSpeakEndEvent)
		)
	{
		SabotageEvent ();
	}
	if	(
			(mThinkRequest)
		&&	(mThinkRequest.ID == lRequest.ID)
		&&	(mSabotageNum == sSabotageThinkEndEvent)
		)
	{
		SabotageEvent ();
	}

	if	(
			(mLoadRequest)
		&&	(mLoadRequest.ID == lRequest.ID)
		)
	{
		mLoadRequest = NULL;
	}
	if	(
			(mShowRequest)
		&&	(mShowRequest.ID == lRequest.ID)
		)
	{
		mShowRequest = NULL;
	}
	if	(
			(mPlayRequest)
		&&	(mPlayRequest.ID == lRequest.ID)
		)
	{
		mPlayRequest = NULL;
	}
	if	(
			(mMoveRequest)
		&&	(mMoveRequest.ID == lRequest.ID)
		)
	{
		mMoveRequest = NULL;
	}
	if	(
			(mSpeakRequest)
		&&	(mSpeakRequest.ID == lRequest.ID)
		)
	{
		mSpeakRequest = NULL;
	}
	if	(
			(mThinkRequest)
		&&	(mThinkRequest.ID == lRequest.ID)
		)
	{
		mThinkRequest = NULL;
	}
}

void CSabotageTestDlg::OnCtlCommand (IDaCtlUserInput* UserInput)
{
	IDaCtlUserInputPtr lUserInput (UserInput);
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlCommand [%ls] [%ls]"), mControlCharacterId, (BSTR)lUserInput.CharacterID, (BSTR)lUserInput.Name);
#endif
	if	(mSabotageNum == sSabotageCommandEvent)
	{
		SabotageEvent ();
	}
	else
	if	(CString ((BSTR)lUserInput.Name).CollateNoCase (mExitCommandName) == 0)
	{
		PostQuitMessage (0);
	}
}

void CSabotageTestDlg::OnCtlIdleStart (LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlIdleStart [%s]"), mControlCharacterId, CharacterID);
#endif
	if	(mSabotageNum == sSabotageIdleStartEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlIdleComplete (LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlIdleComplete [%s]"), mControlCharacterId, CharacterID);
#endif
	if	(mSabotageNum == sSabotageIdleEndEvent)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlMove (LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlMove [%s] [%d %d] [%d]"), mControlCharacterId, CharacterID, X, Y, Cause);
#endif
	if	(
			(mMoveRequest)
		&&	(mSabotageNum == sSabotageMoveEvent)
		)
	{
		SabotageEvent ();
	}
}

void CSabotageTestDlg::OnCtlSize (LPCTSTR CharacterID, short Width, short Height)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlSize [%s] [%d %d]"), mControlCharacterId, CharacterID, Width, Height);
#endif
}

void CSabotageTestDlg::OnCtlBalloonShow(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlBalloonShow [%s]"), mControlCharacterId, CharacterID);
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
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlBalloonHide [%s]"), mControlCharacterId, CharacterID);
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
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlListenStart [%s]"), mControlCharacterId, CharacterID);
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
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlListenComplete [%s] [%d]"), mControlCharacterId, CharacterID, Cause);
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
	LogMessage (_LOG_NOTIFY, _T("[%s] CSabotageTestDlg::OnCtlActiveClientChange [%s] [%d]"), mControlCharacterId, CharacterID, Active);
#endif
}
