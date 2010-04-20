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
//#define	_LOG_AGENT_CALLS	LogAlways|LogTimeMs|LogHighVolume
//#define	_LOG_CHAR_CALLS		LogAlways|LogTimeMs|LogHighVolume
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

#define	_ALWAYS_RELOAD

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

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileCharPosX = _T("CharLeft");
static LPCTSTR	sProfileCharPosY = _T("CharTop");
static LPCTSTR	sProfileCharacter = _T("Character");
static LPCTSTR	sProfileGesture = _T("Gesture");
static LPCTSTR	sProfileState = _T("State");
static LPCTSTR	sProfileSabotage = _T("Sabotage");

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSabotageTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSabotageTestDlg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ACTIVATEAPP()
	ON_BN_CLICKED(IDC_POPUP, OnPopup)
	ON_BN_CLICKED(IDC_SOUND_ON, OnSoundOn)
	ON_BN_CLICKED(IDC_IDLE_ON, OnIdleOn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, OnItemChangedCharacterList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CHARACTER_LIST, OnItemActivateCharacterList)
	ON_LBN_SELCHANGE(IDC_GESTURES, OnSelChangeGestures)
	ON_LBN_DBLCLK(IDC_GESTURES, OnDblClkGestures)
	ON_LBN_SELCHANGE(IDC_STATES, OnSelChangeStates)
	ON_LBN_DBLCLK(IDC_STATES, OnDblClkStates)
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CSabotageTestDlg, CDialog)
	INTERFACE_PART(CSabotageTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CSabotageTestDlg, DaSvrNotifySink)

/////////////////////////////////////////////////////////////////////////////

CSabotageTestDlg::CSabotageTestDlg(CWnd* pParent)
:	CDialog(CSabotageTestDlg::IDD, pParent),
	mNotifySinkId (0),
	mCharacterId (0),
	mExitCommandId (0),
	mRepeatTimer (0),
	mTimerCount (0),
	mLoadReqID (0),
	mLastAnimationReqID (0),
	mHidingStateReqID (0),
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
	FreeAgentCharacter ();
	FreeAgentServer ();
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSabotageTestDlg)
	DDX_Control(pDX, IDC_IDLE_ON, mIdleEnabled);
	DDX_Control(pDX, IDC_SOUND_ON, mSoundOn);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDC_POPUP, mPopupButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	DDX_Control(pDX, IDC_STATES, mStates);
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
	mSabotageNum = AfxGetApp()->GetProfileInt (sProfileKey, sProfileSabotage, mSabotageNum);

	CDialog::OnInitDialog();

	GetWindowText (mWinTitle);
	GetAgentServer ();
	ShowCharacters ();
	LoadConfig ();
	ShowCharacterState ();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
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

bool CSabotageTestDlg::ShowCharacter (LPCTSTR pCharacterPath)
{
	bool	lRet = false;

#ifndef	_ALWAYS_RELOAD
	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
#endif
	{
		FreeAgentCharacter ();
/**/	FreeAgentServer ();
		CharacterIsVisible (false);
	}

	if	(
			(pCharacterPath)
		&&	(pCharacterPath[0])
		)
	{
		lRet = true;
	}

	if	(lRet)
	{
		mCharacterPath = pCharacterPath;
	}
	else
	{
		mCharacterPath.Empty ();
	}

	ShowGestures ();
	ShowStates ();

	if	(
			(lRet)
		&&	(mPopupButton.GetCheck())
		)
	{
		ShowAgentCharacter ();
	}
	ShowCharacterState ();

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::ShowGestures ()
{
	CString	lSelGesture = GetSelGesture ();

	mGestures.SetRedraw (FALSE);
	mGestures.ResetContent ();

	try
	{
		tPtr <CAgentFile>	lAgentFile;

		if	(
				(!mCharacterPath.IsEmpty ())
			&&	(
					(mCharacter != NULL)
				||	(!PathIsURL (mCharacterPath))
				)
			&&	(lAgentFile = CAgentFile::CreateInstance())
			&&	(SUCCEEDED (lAgentFile->Open (mCharacterPath)))
			)
		{
			int	lNdx;

			mGestures.AddString (_T(""));
			lAgentFile->ReadGestures();

			for	(lNdx = 0; lNdx <= lAgentFile->GetGestures().mNames.GetUpperBound (); lNdx++)
			{
				mGestures.AddString (lAgentFile->GetGestures().mNames.GetAt (lNdx));
			}

			if	(
					(mCharacter != NULL)
				&&	(PathIsURL (mCharacterPath))
				)
			{
#if	FALSE
				CString			lGestureName;
				CStringArray	lGestureNames;
				BOOL			lQueuePrepare = FALSE;
				long			lReqID;

				for	(lNdx = 1; lNdx < mGestures.GetCount(); lNdx++)
				{
					mGestures.GetText (lNdx, lGestureName);
					lGestureNames.Add (lGestureName);
					if	(lNdx % 10 == 1)
					{
						lGestureName = JoinStringArray (lGestureNames, _T(","));
						lGestureNames.RemoveAll ();
						mCharacter->Prepare (PREPARE_Sabotage, _bstr_t(lGestureName), lQueuePrepare, &lReqID);
					}
					if	(lNdx >= 20)
					{
						lQueuePrepare = TRUE;
					}
					if	(lNdx >= 40)
					{
						break;
					}
				}
#endif
			}
		}
	}
	catch AnyExceptionDebug

	mGestures.SetCurSel (max (mGestures.FindStringExact (1, lSelGesture), 0));
	mGestures.SetRedraw (TRUE);
}

void CSabotageTestDlg::ShowStates ()
{
	CString	lSelState = GetSelState ();

	mStates.SetRedraw (FALSE);
	mStates.ResetContent ();

	try
	{
		tPtr <CAgentFile>	lAgentFile;

		if	(
				(!mCharacterPath.IsEmpty ())
			&&	(
					(mCharacter != NULL)
				||	(!PathIsURL (mCharacterPath))
				)
			&&	(lAgentFile = CAgentFile::CreateInstance())
			&&	(SUCCEEDED (lAgentFile->Open (mCharacterPath)))
			)
		{
			int	lNdx;

			mStates.AddString (_T(""));
			lAgentFile->ReadStates();

			for	(lNdx = 0; lNdx <= lAgentFile->GetStates().mNames.GetUpperBound (); lNdx++)
			{
				mStates.AddString (lAgentFile->GetStates().mNames.GetAt (lNdx));
			}

			if	(
					(mCharacter != NULL)
				&&	(PathIsURL (mCharacterPath))
				)
			{
#if	FALSE
				CString			lStateName;
				CStringArray	lStateNames;
				BOOL			lQueuePrepare = FALSE;
				long			lReqID;

				for	(lNdx = 1; lNdx < mStates.GetCount(); lNdx++)
				{
					mStates.GetText (lNdx, lStateName);
					lStateNames.Add (lStateName);
					if	(lNdx % 10 == 1)
					{
						lStateName = JoinStringArray (lStateNames, _T(","));
						lStateNames.RemoveAll ();
						mCharacter->Prepare (PrepareResource_State, _bstr_t(lStateName), lQueuePrepare, &lReqID);
					}
					if	(lNdx >= 20)
					{
						lQueuePrepare = TRUE;
					}
					if	(lNdx >= 40)
					{
						break;
					}
				}
#endif
			}
		}
	}
	catch AnyExceptionDebug

	mStates.SetCurSel (max (mStates.FindStringExact (1, lSelState), 0));
	mStates.SetRedraw (TRUE);
}

/////////////////////////////////////////////////////////////////////////////

CString CSabotageTestDlg::GetSelGesture ()
{
	CString	lSelGesture;
	int		lCurSel = mGestures.GetCurSel ();

	if	(lCurSel > 0)
	{
		mGestures.GetText (lCurSel, lSelGesture);
	}
	return lSelGesture;
}

CString CSabotageTestDlg::GetSelState ()
{
	CString	lSelState;
	int		lCurSel = mStates.GetCurSel ();

	if	(lCurSel > 0)
	{
		mStates.GetText (lCurSel, lSelState);
	}
	return lSelState;
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::ShowSelectedAnimation ()
{
	if	(
			(mGestures.GetCurSel () > 0)
		&&	(ShowSelGesture (false))
		)
	{
		mStates.SetCurSel (0);
	}
	else
	{
		mGestures.SetCurSel (0);
	}

	if	(
			(mStates.GetCurSel () > 0)
		&&	(ShowSelState (false))
		)
	{
		mGestures.SetCurSel (0);
	}
	else
	{
		mStates.SetCurSel (0);
	}

	if	(
			(mGestures.GetCurSel () <= 0)
		&&	(mStates.GetCurSel () <= 0)
		)
	{
		Stop ();
		ShowDefaultAnimation ();
	}
}

void CSabotageTestDlg::ShowDefaultAnimation ()
{
	ShowCharacterState ();
}

bool CSabotageTestDlg::ShowSelGesture (bool pStopFirst)
{
	bool	lRet = false;
	CString	lSelGesture = GetSelGesture ();
	HRESULT	lResult;

	if	(!lSelGesture.IsEmpty())
	{
		if	(IsCharacterVisible ())
		{
			if	(pStopFirst)
			{
				Stop ();
			}
			else
			{
				if	(mLastAnimationReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mLastAnimationReqID));
					mLastAnimationReqID = 0;
				}
				if	(mMoveReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mMoveReqID));
					mMoveReqID = 0;
				}
				if	(mHidingStateReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mHidingStateReqID));
					mHidingStateReqID = 0;
				}
			}

			lResult = mCharacter->Play (_bstr_t(lSelGesture), &mLastAnimationReqID);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Play [%s] [%d]"), mCharacterId, lSelGesture, mLastAnimationReqID)))
			{
				lRet = true;
			}
		}
	}
	ShowCharacterState ();
	return lRet;
}

bool CSabotageTestDlg::ShowSelState (bool pStopFirst)
{
	bool	lRet = false;
	CString	lSelState = GetSelState ();
	bool	lCharacterVisible = IsCharacterVisible ();

	if	(!lSelState.IsEmpty())
	{
		if	(lCharacterVisible)
		{
			HRESULT	lResult = S_FALSE;
			CRect	lCharRect;

			if	(pStopFirst)
			{
				Stop ();
			}
			else
			{
				if	(mLastAnimationReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mLastAnimationReqID));
					mLastAnimationReqID = 0;
				}
				if	(mMoveReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mMoveReqID));
					mMoveReqID = 0;
				}
				if	(mHidingStateReqID > 0)
				{
					LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mHidingStateReqID));
					mHidingStateReqID = 0;
				}
			}

			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetSize (&lCharRect.right, &lCharRect.bottom), _T("[%d] GetSize"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetPosition (&lCharRect.left, &lCharRect.top), _T("[%d] GetPosition"), mCharacterId);
			lCharRect.right += lCharRect.left;
			lCharRect.bottom += lCharRect.top;

			if	(lSelState.CompareNoCase (_T("GESTURINGDOWN")) == 0)
			{
				lResult = mCharacter->GestureAt ((short)lCharRect.CenterPoint().x, (short)lCharRect.bottom, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureDown [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("GESTURINGUP")) == 0)
			{
				lResult = mCharacter->GestureAt ((short)lCharRect.CenterPoint().x, (short)lCharRect.top, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureUp [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("GESTURINGRIGHT")) == 0)
			{
				lResult = mCharacter->GestureAt ((short)lCharRect.left, (short)lCharRect.CenterPoint().y, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureRight [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("GESTURINGLEFT")) == 0)
			{
				lResult = mCharacter->GestureAt ((short)lCharRect.right, (short)lCharRect.CenterPoint().y, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureLeft [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGDOWN")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top+lCharRect.Height()*2, 1000, &mMoveReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveDown [%d]"), mCharacterId, mMoveReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGUP")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top-lCharRect.Height()*2, 1000, &mMoveReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveUp [%d]"), mCharacterId, mMoveReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGRIGHT")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left-lCharRect.Width()*2, (short)lCharRect.top, 1000, &mMoveReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveRight [%d]"), mCharacterId, mMoveReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGLEFT")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left+lCharRect.Width()*2, (short)lCharRect.top, 1000, &mMoveReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveLeft [%d]"), mCharacterId, mMoveReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("HIDING")) == 0)
			{
				lResult = mCharacter->Hide (FALSE, &mHidingStateReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, mHidingStateReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("SHOWING")) == 0)
			{
				lResult = mCharacter->Hide (TRUE, &mLastAnimationReqID);
				lResult = mCharacter->Show (FALSE, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId, mLastAnimationReqID);
			}

			if	(lResult == S_OK)
			{
				lRet = true;
			}

			if	(
					(lSelState.CompareNoCase (_T("IDLINGLEVEL1")) == 0)
				||	(lSelState.CompareNoCase (_T("IDLINGLEVEL2")) == 0)
				||	(lSelState.CompareNoCase (_T("IDLINGLEVEL3")) == 0)
				)
			{
				if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mCharacter->SetIdleOn (TRUE), _T("[%d] SetIdleOn"), mCharacterId)))
				{
					lRet = true;
				}
			}
			else
			{
				LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->SetIdleOn (FALSE), _T("[%d] SetIdleOff"), mCharacterId);
			}
		}
	}
	ShowCharacterState ();
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::IsAnimating ()
{
	if	(
			(mCharacter != NULL)
		&&	(
				(mLastAnimationReqID > 0)
			||	(mHidingStateReqID > 0)
			||	(mMoveReqID > 0)
			||	(mSpeakReqID > 0)
			||	(mThinkReqID > 0)
			)
		)
	{
		return true;;
	}
	return false;
}

bool CSabotageTestDlg::Stop ()
{
	bool	lRet = false;
	long	lReqID = 0;

	if	(mRepeatTimer)
	{
		KillTimer (mRepeatTimer);
		mRepeatTimer = 0;
		lRet = true;
	}

	if	(mCharacter != NULL)
	{
		LogComErr (_LOG_CHAR_CALLS, mCharacter->StopAll (StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility), _T("[%d] StopAll"), mCharacterId);
	}

	mTimerCount = 0;
	mLastAnimationReqID = 0;
	mSpeakReqID = 0;
	mThinkReqID = 0;
	return lRet;
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

bool CSabotageTestDlg::ShowAgentCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	GetAgentServer ();
	mLoadReqID = 0;

	if	(
			(mServer != NULL)
		&&	(mCharacterId == 0)
		&&	(!mCharacterPath.IsEmpty ())
		)
	{
		mExitCommandId = 0;
		lResult = mServer->Load (_variant_t(mCharacterPath), &mCharacterId, &mLoadReqID);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("Load [%d] [%s] as [%d]"), mLoadReqID, mCharacterPath, mCharacterId);
	}

	if	(lRet = LoadedAgentCharacter ())
	{
		mLoadReqID = 0;
	}
	return lRet;
}

bool CSabotageTestDlg::LoadedAgentCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;
	long	lReqID = 0;
	CString	lWinTitle;
	CPoint	lCharPos;

	if	(
			(mServer != NULL)
		&&	(mCharacterId != 0)
		&&	(mCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer->get_Character (mCharacterId, &mCharacter), _T("GetCharacterEx"))))
		)
	{
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->SetIdleOn (FALSE), _T("[%d] SetIdleOff"), mCharacterId);
	}

	if	(
			(mCharacter != NULL)
		&&	(mSabotageNum != sSabotageMoveEvent)
		)
	{
		if	(SUCCEEDED (mCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			lCharPos.x = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			lCharPos.y = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
			mCharacter->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
		}
	}

	if	(mCharacter != NULL)
	{
		IDaSvrCommands2Ptr	lCommands (mCharacter);

		lCommands->SetCaption (_bstr_t("Sabotage"));
		lCommands->SetVoice (_bstr_t("sabotage"));
		lCommands->SetGlobalVoiceCommandsEnabled (TRUE);

		if	(mExitCommandId == 0)
		{
			lCommands->AddEx (_bstr_t("Exit"), _bstr_t("exit"), _bstr_t("Exit"), TRUE, TRUE, 0, &mExitCommandId);
		}
	}

	if	(mCharacter != NULL)
	{
		lResult = mCharacter->Show (FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}

	if	(mCharacter != NULL)
	{
		lWinTitle.Format (_T("%s [%d]"), mWinTitle, mCharacterId);
		SetWindowText (lWinTitle);
	}
	return lRet;
}

bool CSabotageTestDlg::HideAgentCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	CPoint	lCharPos;
	HRESULT	lResult;

	if	(
			(mCharacter != NULL)
		&&	(IsCharacterVisible ())
		)
	{
		if	(SUCCEEDED (mCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
		}
		lResult = mCharacter->Hide (FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSabotageTestDlg::FreeAgentCharacter ()
{
	bool	lRet = false;
	CPoint	lCharPos;

	if	(mCharacter != NULL)
	{
		if	(SUCCEEDED (mCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
		}
		mCharacter = NULL;

		if	(
				(mServer != NULL)
			&&	(mCharacterId != 0)
			)
		{
			LogComErr (_LOG_AGENT_CALLS, mServer->Unload (mCharacterId), _T("Unload [%d]"), mCharacterId);
		}

		mCharacterId = 0;
		SetWindowText (mWinTitle);
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CSabotageTestDlg::IsCharacterVisible ()
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;

	if	(
			(mCharacter != NULL)
		&&	(SUCCEEDED (mCharacter->get_Visible (&lVisible)))
		&&	(lVisible)
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
		mPopupButton.SetCheck (TRUE);
		mSpeakButton.EnableWindow (TRUE);
		mThinkButton.EnableWindow (TRUE);
		mListenButton.EnableWindow (TRUE);
	}
	else
	{
		mPopupButton.SetCheck (FALSE);
		mSpeakButton.EnableWindow (FALSE);
		mThinkButton.EnableWindow (FALSE);
		mListenButton.EnableWindow (FALSE);
	}

	if	(mCharacter != NULL)
	{
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetSoundEffectsOn (&lSoundOn), _T("[%d] GetSoundEffectsOn"), mCharacterId);
		mSoundOn.SetCheck (lSoundOn ? TRUE : FALSE);
		mSoundOn.EnableWindow (TRUE);
	}
	else
	{
		mSoundOn.EnableWindow (FALSE);
		mSoundOn.SetCheck (FALSE);
	}

	if	(mCharacter != NULL)
	{
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetIdleOn (&lIdleOn), _T("[%d] GetIdleOn"), mCharacterId);
		mIdleEnabled.SetCheck (lIdleOn ? TRUE : FALSE);
		mIdleEnabled.EnableWindow (TRUE);
	}
	else
	{
		mIdleEnabled.EnableWindow (FALSE);
		mIdleEnabled.SetCheck (FALSE);
	}
}

void CSabotageTestDlg::ShowCharacterState ()
{
	if	(mCharacter != NULL)
	{
//		mPopupButton.EnableWindow (TRUE);
		mMoveButton.EnableWindow (TRUE);
		mCancelButton.EnableWindow (TRUE);
	}
	else
	{
//		mPopupButton.EnableWindow (FALSE);
		mMoveButton.EnableWindow (FALSE);
		mCancelButton.EnableWindow (FALSE);
	}

	CharacterIsVisible (IsCharacterVisible ());
}

void CSabotageTestDlg::SabotageEvent ()
{
	ShowCharacterState ();

	LogMessage (LogNormal, _T("FreeAgentCharacter"));
	FreeAgentCharacter ();
	LogMessage (LogNormal, _T("FreeAgentCharacter Done"));

	ShowCharacterState ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	if	(ShowCharacter (lApp->GetProfileString (sProfileKey, sProfileCharacter, mCharacterPath)))
	{
		tS <LVFINDINFO>	lFindInfo;
		int				lFoundNdx;

		lFindInfo.flags = LVFI_STRING;
		lFindInfo.psz = mCharacterPath;
		lFindInfo.vkDirection = VK_NEXT;
		lFoundNdx = ListView_FindItem (mCharacterList.m_hWnd, -1, &lFindInfo);
		if	(lFoundNdx >= 0)
		{
			mCharacterList.SetItemState (lFoundNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}

	mGestures.SetCurSel (max (mGestures.FindStringExact (1, lApp->GetProfileString (sProfileKey, sProfileGesture, GetSelGesture())), 0));
	mStates.SetCurSel (max (mStates.FindStringExact (1, lApp->GetProfileString (sProfileKey, sProfileState, GetSelState())), 0));
	ShowSelectedAnimation ();

	GetWindowRect (&lWinRect);
	lWinRect.OffsetRect (lApp->GetProfileInt (sProfileKey, sProfilePosX, lWinRect.left) - lWinRect.left, lApp->GetProfileInt (sProfileKey, sProfilePosY, lWinRect.top) - lWinRect.top);
	MoveWindow (&lWinRect);
}

void CSabotageTestDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	lApp->WriteProfileString (sProfileKey, sProfileCharacter, mCharacterPath);
	lApp->WriteProfileString (sProfileKey, sProfileGesture, GetSelGesture());
	lApp->WriteProfileString (sProfileKey, sProfileState, GetSelState());

	if	(!IsIconic ())
	{
		GetWindowRect (&lWinRect);
		lApp->WriteProfileInt (sProfileKey, sProfilePosX, lWinRect.left);
		lApp->WriteProfileInt (sProfileKey, sProfilePosY, lWinRect.top);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnCancel()
{
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
	UpdateData (TRUE);
	CDialog::EndDialog (IDOK);
}

void CSabotageTestDlg::OnDestroy()
{
	SaveConfig ();
	FreeAgentCharacter ();
	FreeAgentServer ();

	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW	lNotify = (LPNMLISTVIEW)pNMHDR;
	CString			lCharacterPath;

	if	(
			(lNotify->uChanged & LVIF_STATE)
		&&	(!(lNotify->uOldState & LVIS_SELECTED))
		&&	(lNotify->uNewState & LVIS_SELECTED)
		)
	{
		lCharacterPath = mCharacterList.GetItemText (lNotify->iItem, 0);
		if	(
				(
					(mCharacter == NULL)
				||	(lCharacterPath.CompareNoCase (mCharacterPath) != 0)
				)
			&&	(ShowCharacter (lCharacterPath))
			)
		{
			ShowSelectedAnimation ();
		}
	}
}

void CSabotageTestDlg::OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE*	lNotify = (NMITEMACTIVATE*)pNMHDR;

	if	(ShowCharacter (mCharacterList.GetItemText (lNotify->iItem, 0)))
	{
		ShowSelectedAnimation ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnSelChangeGestures()
{
	if	(mGestures.GetCurSel () > 0)
	{
		mStates.SetCurSel (0);
	}
	ShowSelectedAnimation ();
}

void CSabotageTestDlg::OnDblClkGestures()
{
	if	(mGestures.GetCurSel () > 0)
	{
		mStates.SetCurSel (0);
		ShowSelectedAnimation ();
		mRepeatTimer = SetTimer ((UINT_PTR)&mRepeatTimer, 100, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnSelChangeStates()
{
	if	(mStates.GetCurSel () > 0)
	{
		mGestures.SetCurSel (0);
	}
	ShowSelectedAnimation ();
}

void CSabotageTestDlg::OnDblClkStates()
{
	if	(mStates.GetCurSel () > 0)
	{
		mGestures.SetCurSel (0);
		ShowSelectedAnimation ();
		mRepeatTimer = SetTimer ((UINT_PTR)&mRepeatTimer, 100, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnPopup()
{
	if	(IsCharacterVisible ())
	{
		if	(HideAgentCharacter ())
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (false);
#endif
		}
	}
	else
	{
#ifdef	_ALWAYS_RELOAD
		ShowCharacter (CString((LPCTSTR)mCharacterPath));
#endif
		if	(ShowAgentCharacter ())
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
		CRect	lCharRect;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetSize (&lCharRect.right, &lCharRect.bottom), _T("[%d] GetSize"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetPosition (&lCharRect.left, &lCharRect.top), _T("[%d] GetPosition"), mCharacterId);
		lCharRect.right += lCharRect.left;
		lCharRect.bottom += lCharRect.top;

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
		lResult = mCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top, 1000, &mMoveReqID);
		LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveTo [%d %d] [%d]"), mCharacterId, lCharRect.left, lCharRect.top, mMoveReqID);
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

		lResult = mCharacter->Speak (_bstr_t("Now is the winter of our discontent."), NULL, &mSpeakReqID);
		LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d]"), mCharacterId, mSpeakReqID);
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

		lResult = mCharacter->Think (_bstr_t("Now is the winter of our discontent."), &mThinkReqID);
		LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d]"), mCharacterId, mThinkReqID);
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
		HRESULT					lResult;
		IDaSvrAudioOutputPtr	lOutputProperties (mServer);
		long					lTTSStatus;

		LogComErr (_LOG_CHAR_CALLS, lOutputProperties->GetStatus (&lTTSStatus));
		lResult = mCharacter->Listen (lTTSStatus != AudioStatus_CharacterListening);
		LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Listen [%d]"), mCharacterId, (lTTSStatus != AudioStatus_CharacterListening));
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

void CSabotageTestDlg::OnSoundOn()
{
	if	(mCharacter)
	{
		long	lSoundOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetSoundEffectsOn (&lSoundOn), _T("[%d] GetSoundEffectsOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mCharacter->SetSoundEffectsOn (lSoundOn ? FALSE : TRUE), _T("[%d] SetSoundEffectsOn"), mCharacterId);
		ShowCharacterState ();
	}
}

void CSabotageTestDlg::OnIdleOn()
{
	if	(mCharacter)
	{
		long	lIdleOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetIdleOn (&lIdleOn), _T("[%d] GetIdleOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mCharacter->SetIdleOn (lIdleOn ? FALSE : TRUE), _T("[%d] SetIdleOn"), mCharacterId);
		ShowCharacterState ();
	}
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
		if	(!IsAnimating())
		{
			ShowSelGesture (false);
			ShowSelState (false);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////

void CSabotageTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	if	(mCharacter != NULL)
	{
		ActiveStateType	lActive = (ActiveStateType)-1;
		long			lOtherClients = -1;

		if	(bActive)
		{
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->get_ActiveState (&lActive), _T("[%d] get_ActiveState"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->get_OtherClientCount (&lOtherClients), _T("[%d] get_OtherClientCount"), mCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mCharacterId, bActive, lActive, lOtherClients);

			LogComErr (_LOG_CHAR_CALLS, mCharacter->put_ActiveState (ActiveState_Active), _T("[%d] put_ActiveState ActiveState_Active"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->get_ActiveState (&lActive), _T("[%d] get_ActiveState"), mCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd]"), mCharacterId, bActive, lActive);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::GetTypeInfoCount(UINT *pctinfo)
{
	METHOD_PROLOGUE_EX_(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSabotageTestDlg::XDaSvrNotifySink::GetTypeInfoCount"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfoCount (pctinfo);
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	METHOD_PROLOGUE_EX_(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSabotageTestDlg::XDaSvrNotifySink::GetTypeInfo"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfo (iTInfo, lcid, ppTInfo);
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	METHOD_PROLOGUE_EX_(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSabotageTestDlg::XDaSvrNotifySink::GetIDsOfNames"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	METHOD_PROLOGUE_EX(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSabotageTestDlg::XDaSvrNotifySink::Invoke [%8.8X (%u)]"), pThis, pThis->m_dwRef, dispIdMember, dispIdMember);
#endif
	return pThis->GetIDispatch(FALSE)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Command"), pThis->mCharacterId, pThis->m_dwRef);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Activated);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Restart"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Shutdown"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	VisibilityCauseType lCause = (VisibilityCauseType)-1;
	if	(pThis->mCharacter != NULL)
	{
		pThis->mCharacter->get_VisibilityCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Visible, Cause, lCause);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mCharacterId, pThis->m_dwRef, RequestID);
#endif
	if	(
			(
				(RequestID == pThis->mLastAnimationReqID)
			||	(RequestID == pThis->mHidingStateReqID)
			)
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

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::RequestComplete (long RequestID, long hrStatus)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, RequestID, hrStatus);
#endif
	if	(
			(
				(RequestID == pThis->mLastAnimationReqID)
			||	(RequestID == pThis->mHidingStateReqID)
			)
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

	if	(RequestID == pThis->mLastAnimationReqID)
	{
		pThis->mLastAnimationReqID = 0;
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
	if	(RequestID == pThis->mHidingStateReqID)
	{
		pThis->mHidingStateReqID = 0;
		LogComErr (_LOG_CHAR_CALLS, pThis->mCharacter->Show (TRUE, &RequestID), _T("[%d] Show"), pThis->mCharacterId);
	}
	if	(RequestID == pThis->mLoadReqID)
	{
		pThis->mLoadReqID = 0;
		pThis->LoadedAgentCharacter ();

		if	(PathIsURL (pThis->mCharacterPath))
		{
			pThis->ShowGestures ();
			pThis->ShowStates ();
			pThis->ShowCharacterState ();
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::BookMark (long dwBookMarkID)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwBookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Start);
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
	if	(pThis->mCharacter != NULL)
	{
		pThis->mCharacter->get_MoveCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, x, y, Cause, lCause);
#endif
	if	(pThis->mSabotageNum == sSabotageMoveEvent)
	{
		pThis->SabotageEvent ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::Size (long CharacterID, long Width, long Height)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Width, Height);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Visible);
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
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ListeningState"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	pThis->ShowCharacterState ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR bszGUID)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mCharacterId, pThis->m_dwRef, bszGUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSabotageTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CSabotageTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSabotageTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Status);
#endif
	return S_OK;
}
