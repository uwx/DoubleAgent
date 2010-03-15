#include "StdAfx.h"
#include <shlwapi.h>
#include "AnimationTest.h"
#include "AnimationTestDlg.h"
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

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileCharPosX = _T("CharLeft");
static LPCTSTR	sProfileCharPosY = _T("CharTop");
static LPCTSTR	sProfileCharacter = _T("Character");
static LPCTSTR	sProfileGesture = _T("Gesture");
static LPCTSTR	sProfileState = _T("State");
static LPCTSTR	sProfileBoth = _T("Both");
static LPCTSTR	sProfileFast = _T("Fast");
static LPCTSTR	sProfileCharSize = _T("CharSize");
static LPCTSTR	sProfileIconShown = _T("IconShown");
static LPCTSTR	sProfileIconOnLoad = _T("IconOnLoad");
static LPCTSTR	sProfileIconGenerated = _T("IconGenerated");
static LPCTSTR	sProfileIconClipped = _T("IconClipped");
static LPCTSTR	sProfileIconIdentified = _T("IconIdentified");

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAnimationTestDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimationTestDlg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ACTIVATEAPP()
	ON_BN_CLICKED(IDC_ALL_GESTURES, OnAllGestures)
	ON_BN_CLICKED(IDC_ALL_STATES, OnAllStates)
	ON_BN_CLICKED(IDC_POPUP, OnPopup)
	ON_BN_CLICKED(IDC_SOUND_ON, OnSoundOn)
	ON_BN_CLICKED(IDC_AUTO_POPUP, OnAutoPopup)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, OnItemChangedCharacterList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CHARACTER_LIST, OnItemActivateCharacterList)
	ON_LBN_SELCHANGE(IDC_GESTURES, OnSelChangeGestures)
	ON_LBN_DBLCLK(IDC_GESTURES, OnDblClkGestures)
	ON_LBN_SELCHANGE(IDC_STATES, OnSelChangeStates)
	ON_LBN_DBLCLK(IDC_STATES, OnDblClkStates)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AGENT_PROPS, OnAgentProps)
	ON_BN_CLICKED(IDC_CHAR_PROPS, OnCharProps)
	ON_BN_CLICKED(IDC_IDLE_ON, OnIdleOn)
	ON_BN_CLICKED(IDC_SIZE_NORMAL, OnSizeNormal)
	ON_BN_CLICKED(IDC_SIZE_SMALL, OnSizeSmall)
	ON_BN_CLICKED(IDC_SIZE_LARGE, OnSizeLarge)
	ON_BN_CLICKED(IDC_ICON_SHOW, OnIconShown)
	ON_BN_CLICKED(IDC_ICON_GENERATE, OnIconGenerated)
	ON_BN_CLICKED(IDC_ICON_IDENTITY, OnIconIdentified)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CAnimationTestDlg, CDialog)
	INTERFACE_PART(CAnimationTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CAnimationTestDlg, DaSvrNotifySink)

/////////////////////////////////////////////////////////////////////////////

CAnimationTestDlg::CAnimationTestDlg(CWnd* pParent)
:	CDialog(CAnimationTestDlg::IDD, pParent),
	mNotifySinkId (0),
	mCharacterId (0),
	mRepeatTimer (0),
	mAllGesturesTimer (0),
	mAllStatesTimer (0),
	mTimerCount (0),
	mLoadReqID (0),
	mHideReqID (0),
	mLastAnimationReqID (0)
{
	//{{AFX_DATA_INIT(CAnimationTestDlg)
	//}}AFX_DATA_INIT
	EnableAutomation ();
}

CAnimationTestDlg::~CAnimationTestDlg()
{
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimationTestDlg)
	DDX_Control(pDX, IDC_AUTO_POPUP, mAutoPopup);
	DDX_Control(pDX, IDC_FAST_SHOWHIDE, mFastShowHide);
	DDX_Control(pDX, IDC_IDLE_ON, mIdleOn);
	DDX_Control(pDX, IDC_ANIMATE_BOTH, mAnimateBoth);
	DDX_Control(pDX, IDC_AGENT_PROPS, mAgentPropsButton);
	DDX_Control(pDX, IDC_CHAR_PROPS, mCharPropsButton);
	DDX_Control(pDX, IDC_SOUND_ON, mSoundOn);
	DDX_Control(pDX, IDC_CHARACTER_PREVIEW, mPreviewWnd);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDC_CHARACTER_NAME, mCharacterNameEdit);
	DDX_Control(pDX, IDC_CHARACTER_DESC, mCharacterDescEdit);
	DDX_Control(pDX, IDC_POPUP, mPopupButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	DDX_Control(pDX, IDC_STATES, mStates);
	DDX_Control(pDX, IDC_GESTURES, mGestures);
	DDX_Control(pDX, IDC_ALL_STATES, mAllStates);
	DDX_Control(pDX, IDC_ALL_GESTURES, mAllGestures);
	DDX_Control(pDX, IDC_SIZE_NORMAL, mSizeNormal);
	DDX_Control(pDX, IDC_SIZE_LARGE, mSizeLarge);
	DDX_Control(pDX, IDC_SIZE_SMALL, mSizeSmall);
	DDX_Control(pDX, IDC_ICON_SHOW, mIconShown);
	DDX_Control(pDX, IDC_ICON_SHOWONLOAD, mIconOnLoad);
	DDX_Control(pDX, IDC_ICON_GENERATE, mIconGenerated);
	DDX_Control(pDX, IDC_ICON_GENERATE_CLIP, mIconClipped);
	DDX_Control(pDX, IDC_ICON_IDENTITY, mIconIdentified);
	//}}AFX_DATA_MAP
}

BOOL CAnimationTestDlg::OnInitDialog()
{
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

void CAnimationTestDlg::ShowCharacters ()
{
	IDaSvrCharacterFilesPtr	lCharacterFiles;
	tSafeArrayPtr			lFilePaths;
	VARTYPE					lFilePathType;
	long					lLowerBound, lUpperBound, lNdx;
	CRect					lClientRect;

	mCharacterList.InsertColumn (0, _T("Path"));
	mCharacterList.GetClientRect (&lClientRect);
	lClientRect.right -= GetSystemMetrics (SM_CXVSCROLL);

	//CDC	lCompactDC;
	//lCompactDC.CreateCompatibleDC (NULL);
	//lCompactDC.SelectObject (mCharacterList.GetFont ());

	if	(
			(mServer != NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer->GetCharacterFiles (&lCharacterFiles))))
		)
	{
#if	TRUE
		lCharacterFiles->put_Filter (FILES_PATH_MASK);
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
				CString		lFilePathStr;

				lFilePath.vt = lFilePathType;
				SafeArrayGetElement (lFilePaths, &lNdx, &V_BYREF(&lFilePath));
				lFilePathStr = (BSTR)(_bstr_t)lFilePath;

				//PathCompactPath (lCompactDC, lFilePathStr.GetBuffer (MAX_PATH), lClientRect.Width()-4);
				//lFilePathStr.ReleaseBuffer ();
				mCharacterList.InsertItem (0, lFilePathStr);
			}
		}
	}

	mCharacterList.SetColumnWidth (0, lClientRect.Width());
}

bool CAnimationTestDlg::ShowCharacter (LPCTSTR pCharacterPath)
{
	bool	lRet = false;

#ifndef	_ALWAYS_RELOAD
	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
#endif
	{
		ReleaseAgentCharacter ();
		CharacterIsVisible (false);
	}

	if	(
			(pCharacterPath)
		&&	(pCharacterPath[0])
		)
	{
		if	(PathIsURL (pCharacterPath))
		{
			lRet = true;
		}
		else
		{
			try
			{
				CAgentPreviewWnd *	lPreview;
				CRect				lClientRect;
				CRect				lWindowRect;

				SafeFreeSafePtr (mAgentWnd);

				if	(
						(mAgentWnd = (CAgentPreviewWnd *) CAgentPreviewWnd::CreateObject())
					&&	(mAgentWnd->Create (mPreviewWnd.m_hWnd))
					&&	(IsWindow (mAgentWnd->GetSafeHwnd ()))
					)
				{
					mAgentWnd->ShowWindow (SW_HIDE);

					if	(mAgentWnd->Open (pCharacterPath))
					{
						if	(lPreview = DYNAMIC_DOWNCAST (CAgentPreviewWnd, mAgentWnd.Ptr()))
						{
							lPreview->SetBkColor (GetSysColor (COLOR_WINDOW));
						}

						mPreviewWnd.GetClientRect (&lClientRect);
						mAgentWnd->GetWindowRect (&lWindowRect);
						mPreviewWnd.ScreenToClient (&lWindowRect);
						lWindowRect.OffsetRect (lClientRect.CenterPoint().x - lWindowRect.CenterPoint().x, lClientRect.CenterPoint().y - lWindowRect.CenterPoint().y);
						mAgentWnd->MoveWindow (&lWindowRect);
						mAgentWnd->ShowWindow (SW_SHOWNA);
						lRet = true;
					}
					else
					{
						SafeFreeSafePtr (mAgentWnd);
					}
				}
				else
				{
					SafeFreeSafePtr (mAgentWnd);
				}
			}
			catch AnyExceptionDebug
		}
	}

	if	(lRet)
	{
		mCharacterPath = pCharacterPath;
	}
	else
	{
		mCharacterPath.Empty ();
	}

	ShowCharacterDetails ();
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

void CAnimationTestDlg::ShowCharacterDetails ()
{
	CString	lName;
	CString	lDesc;

	if	(!mCharacterPath.IsEmpty ())
	{
		CAgentFileName *	lFileName;

		if	(
				(mAgentWnd->GetSafeHwnd())
			&&	(mAgentWnd->GetAgentFile())
			&&	(lFileName = mAgentWnd->GetAgentFile()->FindName ())
			)
		{
			lName = lFileName->mName;
			lDesc = lFileName->mDesc1;
		}
	}

	mCharacterNameEdit.SetWindowText (lName);
	mCharacterDescEdit.SetWindowText (lDesc);
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::ShowGestures ()
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
			&&	(lAgentFile = (CAgentFile *)CAgentFile::CreateObject())
			&&	(SUCCEEDED (lAgentFile->Open (mCharacterPath)))
			)
		{
			int	lNdx;

			mGestures.AddString (_T(""));
			lAgentFile->ReadGestures();

			for	(lNdx = 0; lNdx <= lAgentFile->GetGestures().OrderArray().GetUpperBound (); lNdx++)
			{
				mGestures.AddString (lAgentFile->GetGestures().OrderArray().GetAt (lNdx));
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
						mCharacter->Prepare (PREPARE_ANIMATION, _bstr_t(lGestureName), lQueuePrepare, &lReqID);
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

void CAnimationTestDlg::ShowStates ()
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
			&&	(lAgentFile = (CAgentFile *)CAgentFile::CreateObject())
			&&	(SUCCEEDED (lAgentFile->Open (mCharacterPath)))
			)
		{
			int	lNdx;

			mStates.AddString (_T(""));
			lAgentFile->ReadStates();

			for	(lNdx = 0; lNdx <= lAgentFile->GetStates().OrderArray().GetUpperBound (); lNdx++)
			{
				mStates.AddString (lAgentFile->GetStates().OrderArray().GetAt (lNdx));
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
						mCharacter->Prepare (PREPARE_STATE, _bstr_t(lStateName), lQueuePrepare, &lReqID);
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

CString CAnimationTestDlg::GetSelGesture ()
{
	CString	lSelGesture;
	int		lCurSel = mGestures.GetCurSel ();

	if	(lCurSel > 0)
	{
		mGestures.GetText (lCurSel, lSelGesture);
	}
	return lSelGesture;
}

CString CAnimationTestDlg::GetSelState ()
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

void CAnimationTestDlg::ShowSelectedAnimation ()
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

void CAnimationTestDlg::ShowDefaultAnimation ()
{
	ShowCharacterState ();
}

bool CAnimationTestDlg::ShowSelGesture (bool pStopFirst)
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
			if	(mLastAnimationReqID > 0)
			{
				LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mLastAnimationReqID));
				mLastAnimationReqID = 0;
			}

			lResult = mCharacter->Play (_bstr_t(lSelGesture), &mLastAnimationReqID);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Play [%s] [%d]"), mCharacterId, lSelGesture, mLastAnimationReqID)))
			{
				lRet = true;
			}
		}

		if	(
				(
					(!IsCharacterVisible ())
				||	(mAnimateBoth.GetCheck())
				)
			&&	(mAgentWnd->GetSafeHwnd())
			)
		{
			lRet = mAgentWnd->ShowGesture (lSelGesture, NULL, true, pStopFirst);

			if	(
					(!lRet)
				||	(mAgentWnd->IsPaused())
				||	(mAgentWnd->IsStopped())
				)
			{
				MessageBeep (MB_OK);
			}
		}
	}
	ShowCharacterState ();
	return lRet;
}

bool CAnimationTestDlg::ShowSelState (bool pStopFirst)
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
			if	(mLastAnimationReqID > 0)
			{
				LogComErr (_LOG_CHAR_CALLS, mCharacter->Stop (mLastAnimationReqID));
				mLastAnimationReqID = 0;
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
				lResult = mCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top+lCharRect.Height()*2, 1000, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveDown [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGUP")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top-lCharRect.Height()*2, 1000, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveUp [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGRIGHT")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left-lCharRect.Width()*2, (short)lCharRect.top, 1000, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveRight [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGLEFT")) == 0)
			{
				lResult = mCharacter->MoveTo ((short)lCharRect.left+lCharRect.Width()*2, (short)lCharRect.top, 1000, &mLastAnimationReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveLeft [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("HIDING")) == 0)
			{
				lResult = mCharacter->Hide (FALSE, &mHideReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, mHideReqID);
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

		if	(
				(!lCharacterVisible)
			||	(mAnimateBoth.GetCheck())
			)
		{
			lRet = mAgentWnd->ShowState (lSelState, true, pStopFirst);
		}
	}
	ShowCharacterState ();
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAnimationTestDlg::ShowAllGestures ()
{
	Stop ();
	mGestures.SetCurSel (0);
	mStates.SetCurSel (0);
	mTimerCount = 0;
	mLastAnimationReqID = 0;
	mAllGesturesTimer = SetTimer ((UINT_PTR)&mAllGesturesTimer, 100, NULL);

	return true;
}

bool CAnimationTestDlg::ShowAllStates ()
{
	Stop ();
	mGestures.SetCurSel (0);
	mStates.SetCurSel (0);
	mTimerCount = 0;
	mLastAnimationReqID = 0;
	mAllStatesTimer = SetTimer ((UINT_PTR)&mAllStatesTimer, 100, NULL);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAnimationTestDlg::IsAnimating ()
{
	if	(
			(mCharacter != NULL)
		&&	(mLastAnimationReqID > 0)
		)
	{
		return true;;
	}
	if	(
			(IsWindow (mAgentWnd->GetSafeHwnd()))
		&&	(!mAgentWnd->IsAnimationComplete ())
		)
	{
		return true;
	}
	return false;
}

bool CAnimationTestDlg::Stop ()
{
	bool	lRet = false;
	long	lReqID = 0;

	if	(mRepeatTimer)
	{
		KillTimer (mRepeatTimer);
		mRepeatTimer = 0;
		lRet = true;
	}
	if	(mAllGesturesTimer)
	{
		KillTimer (mAllGesturesTimer);
		mAllGesturesTimer = 0;
		mGestures.SetCurSel (0);
		lRet = true;
	}
	if	(mAllStatesTimer)
	{
		KillTimer (mAllStatesTimer);
		mAllStatesTimer = 0;
		mStates.SetCurSel (0);
		lRet = true;
	}

	if	(
			(IsWindow (mAgentWnd->GetSafeHwnd ()))
		&&	(mAgentWnd->Stop () == S_OK)
		)
	{
		lRet = true;
	}

	if	(mCharacter != NULL)
	{
		LogComErr (_LOG_CHAR_CALLS, mCharacter->StopAll (STOP_TYPE_PLAY|STOP_TYPE_SPEAK|STOP_TYPE_MOVE|STOP_TYPE_VISIBLE), _T("[%d] StopAll"), mCharacterId);
	}

	mTimerCount = 0;
	mLastAnimationReqID = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::GetAgentServer ()
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

bool CAnimationTestDlg::ShowAgentCharacter ()
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
		LogComErr (_LOG_AGENT_CALLS, mServer->put_IsCharacterIconShown (mIconOnLoad.GetCheck()?TRUE:FALSE));
		lResult = mServer->Load (_variant_t(mCharacterPath), &mCharacterId, &mLoadReqID);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("Load [%d] [%s] as [%d]"), mLoadReqID, mCharacterPath, mCharacterId);
	}

	if	(lRet = LoadedAgentCharacter ())
	{
		mLoadReqID = 0;
	}
	return lRet;
}

bool CAnimationTestDlg::LoadedAgentCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;
	long	lReqID = 0;
	CString	lWinTitle;
	CPoint	lCharPos;

	IDaSvrCharacterPtr	lCharacter;

	if	(
			(mServer != NULL)
		&&	(mCharacterId != 0)
		&&	(mCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer->GetCharacterEx (mCharacterId, &lCharacter), _T("GetCharacterEx"))))
		)
	{
		mCharacter = lCharacter;
	}

	if	(mCharacter != NULL)
	{
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->SetIdleOn (mIdleOn.GetCheck()?TRUE:FALSE), _T("[%d] SetIdleOn"), mCharacterId);

		if	(SUCCEEDED (mCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			lCharPos.x = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			lCharPos.y = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
			mCharacter->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
		}
		SetCharacterSize ();
		SetCharacterIcon ();

		lResult = mCharacter->Show (mFastShowHide.GetCheck() ? TRUE : FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}

		lWinTitle.Format (_T("%s [%d]"), mWinTitle, mCharacterId);
		SetWindowText (lWinTitle);
		lRet = true;
	}
	return lRet;
}

bool CAnimationTestDlg::HideAgentCharacter ()
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
		lResult = mCharacter->Hide (mFastShowHide.GetCheck() ? TRUE : FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CAnimationTestDlg::ReleaseAgentCharacter ()
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

void CAnimationTestDlg::SetCharacterSize()
{
	CSize	lCharSize;

	if	(
			(mCharacter != NULL)
		&&	(SUCCEEDED (mCharacter->GetOriginalSize (&lCharSize.cx, &lCharSize.cy)))
		)
	{
		if	(mSizeSmall.GetCheck ())
		{
			mCharacter->SetSize (MulDiv (lCharSize.cx, 2, 3), MulDiv (lCharSize.cy, 2, 3));
		}
		else
		if	(mSizeLarge.GetCheck ())
		{
			mCharacter->SetSize (MulDiv (lCharSize.cx, 3, 2), MulDiv (lCharSize.cy, 3, 2));
		}
		else
		{
			mCharacter->SetSize (lCharSize.cx, lCharSize.cy);
		}
	}
}

void CAnimationTestDlg::SetCharacterIcon()
{
	if	(mIconIdentified.GetCheck())
	{
		tBstrPtr	lGUID;
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetGUID (lGUID.Free()));
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->put_IconIdentity (lGUID));
	}
	else
	{
		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->put_IconIdentity (_T("")));
	}

	if	(mIconGenerated.GetCheck())
	{
		CSize	lCharSize;
		
		if	(
				(mIconClipped.GetCheck ())
			&&	(SUCCEEDED (mCharacter->GetOriginalSize (&lCharSize.cx, &lCharSize.cy)))
			)
		{
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GenerateIcon (0, 0, lCharSize.cx, lCharSize.cy/2));
		}
		else
		{
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GenerateIcon ());
		}
	}

	LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->put_IsIconShown (mIconShown.GetCheck()?TRUE:FALSE));

#if	FALSE
	CString		lIconTip;
	lIconTip.Format (_T("%u Animation Test"), GetCurrentProcessId());
	LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->put_IconTip (_bstr_t(lIconTip)));
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CAnimationTestDlg::IsCharacterVisible ()
{
	long	lVisible = FALSE;

	if	(
			(mCharacter != NULL)
		&&	(SUCCEEDED (mCharacter->GetVisible (&lVisible)))
		&&	(lVisible)
		)
	{
		return true;
	}
	return false;
}

void CAnimationTestDlg::CharacterIsVisible (bool pVisible)
{
	if	(pVisible)
	{
		mPopupButton.SetCheck (TRUE);
		mAnimateBoth.EnableWindow (TRUE);
	}
	else
	{
		mPopupButton.SetCheck (FALSE);
		mAnimateBoth.EnableWindow (FALSE);
	}

	if	(mCharacter != NULL)
	{
		long	lSoundOn = 0;
		long	lIdleOn = 0;
		long	lAutoPopup = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetSoundEffectsOn (&lSoundOn), _T("[%d] GetSoundEffectsOn"), mCharacterId);
		mSoundOn.SetCheck (lSoundOn ? TRUE : FALSE);
		mSoundOn.EnableWindow (TRUE);

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetIdleOn (&lIdleOn), _T("[%d] GetIdleOn"), mCharacterId);
		mIdleOn.SetCheck (lIdleOn ? TRUE : FALSE);
		mIdleOn.EnableWindow (TRUE);

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetAutoPopupMenu (&lAutoPopup), _T("[%d] GetAutoPopupMenu"), mCharacterId);
		mAutoPopup.SetCheck (lAutoPopup ? TRUE : FALSE);
		mAutoPopup.EnableWindow (TRUE);
	}
	else
	{
		mSoundOn.EnableWindow (FALSE);
		mSoundOn.SetCheck (FALSE);
		mIdleOn.EnableWindow (FALSE);
		mIdleOn.SetCheck (FALSE);
		mAutoPopup.EnableWindow (FALSE);
		mAutoPopup.SetCheck (FALSE);
	}
}

void CAnimationTestDlg::ShowCharacterState ()
{
	if	(mAgentWnd->GetSafeHwnd ())
	{
		mAgentWnd->EnableSound (mCharacter == NULL);
	}
	CharacterIsVisible (IsCharacterVisible ());
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	mSizeNormal.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharSize, 0) == 0);
	mSizeSmall.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharSize, 0) < 0);
	mSizeLarge.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharSize, 0) > 0);

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

//	mGestures.SetCurSel (max (mGestures.FindStringExact (1, lApp->GetProfileString (sProfileKey, sProfileGesture, GetSelGesture())), 0));
//	mStates.SetCurSel (max (mStates.FindStringExact (1, lApp->GetProfileString (sProfileKey, sProfileState, GetSelState())), 0));
	mAnimateBoth.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileBoth, mAnimateBoth.GetCheck()) ? TRUE : FALSE);
	mFastShowHide.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileFast, mFastShowHide.GetCheck()) ? TRUE : FALSE);
	mIconShown.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconShown, mIconShown.GetCheck()) ? TRUE : FALSE);
	mIconOnLoad.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconOnLoad, mIconOnLoad.GetCheck()) ? TRUE : FALSE);
	mIconGenerated.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconGenerated, mIconGenerated.GetCheck()) ? TRUE : FALSE);
	mIconClipped.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconClipped, mIconClipped.GetCheck()) ? TRUE : FALSE);
	mIconIdentified.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconIdentified, mIconIdentified.GetCheck()) ? TRUE : FALSE);
	ShowSelectedAnimation ();

	GetWindowRect (&lWinRect);
	lWinRect.OffsetRect (lApp->GetProfileInt (sProfileKey, sProfilePosX, lWinRect.left) - lWinRect.left, lApp->GetProfileInt (sProfileKey, sProfilePosY, lWinRect.top) - lWinRect.top);
	MoveWindow (&lWinRect);
}

void CAnimationTestDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	lApp->WriteProfileString (sProfileKey, sProfileCharacter, mCharacterPath);
	lApp->WriteProfileString (sProfileKey, sProfileGesture, GetSelGesture());
	lApp->WriteProfileString (sProfileKey, sProfileState, GetSelState());
	lApp->WriteProfileInt (sProfileKey, sProfileBoth, mAnimateBoth.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileFast, mFastShowHide.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileCharSize, mSizeSmall.GetCheck()?-1:mSizeLarge.GetCheck()?1:0);
	lApp->WriteProfileInt (sProfileKey, sProfileIconShown, mIconShown.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconOnLoad, mIconOnLoad.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconGenerated, mIconGenerated.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconClipped, mIconClipped.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconIdentified, mIconIdentified.GetCheck());

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

void CAnimationTestDlg::OnCancel()
{
	if	(!Stop ())
	{
		MessageBeep (MB_OK);
	}
}

void CAnimationTestDlg::OnOK()
{
}

void CAnimationTestDlg::OnClose()
{
	if	(IsWindow (mAgentWnd->GetSafeHwnd ()))
	{
		mAgentWnd->Close ();
	}
	CDialog::EndDialog (IDOK);
}

void CAnimationTestDlg::OnDestroy()
{
	SaveConfig ();
	ReleaseAgentCharacter ();
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
	SafeFreeSafePtr (mCharacter);
	SafeFreeSafePtr (mServer);

	if	(IsWindow (mAgentWnd->GetSafeHwnd ()))
	{
		mAgentWnd->Close ();
	}
	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////

HBRUSH CAnimationTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if	(
			(pWnd == &mCharacterNameEdit)
		||	(pWnd == &mCharacterDescEdit)
		)
	{
		pDC->SetBkColor (GetSysColor (COLOR_WINDOW));
		return GetSysColorBrush (COLOR_WINDOW);
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
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
				(lCharacterPath.CompareNoCase (mCharacterPath) != 0)
			&&	(ShowCharacter (lCharacterPath))
			)
		{
			ShowSelectedAnimation ();
		}
	}
}

void CAnimationTestDlg::OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE*	lNotify = (NMITEMACTIVATE*)pNMHDR;

	if	(ShowCharacter (mCharacterList.GetItemText (lNotify->iItem, 0)))
	{
		ShowSelectedAnimation ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnSelChangeGestures()
{
	if	(mGestures.GetCurSel () > 0)
	{
		mStates.SetCurSel (0);
	}
	ShowSelectedAnimation ();
}

void CAnimationTestDlg::OnDblClkGestures()
{
	if	(mGestures.GetCurSel () > 0)
	{
		mStates.SetCurSel (0);
		ShowSelectedAnimation ();
		mRepeatTimer = SetTimer ((UINT_PTR)&mRepeatTimer, 100, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnSelChangeStates()
{
	if	(mStates.GetCurSel () > 0)
	{
		mGestures.SetCurSel (0);
	}
	ShowSelectedAnimation ();
}

void CAnimationTestDlg::OnDblClkStates()
{
	if	(mStates.GetCurSel () > 0)
	{
		mGestures.SetCurSel (0);
		ShowSelectedAnimation ();
		mRepeatTimer = SetTimer ((UINT_PTR)&mRepeatTimer, 100, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnAllGestures()
{
	ShowAllGestures ();
}

void CAnimationTestDlg::OnAllStates()
{
	ShowAllStates ();
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnPopup()
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

void CAnimationTestDlg::OnSoundOn()
{
	if	(mCharacter)
	{
		long	lSoundOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetSoundEffectsOn (&lSoundOn), _T("[%d] GetSoundEffectsOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mCharacter->SetSoundEffectsOn (lSoundOn ? FALSE : TRUE), _T("[%d] SetSoundEffectsOn"), mCharacterId);
		ShowCharacterState ();
	}
}

void CAnimationTestDlg::OnIdleOn()
{
	if	(mCharacter)
	{
		long	lIdleOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetIdleOn (&lIdleOn), _T("[%d] GetIdleOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mCharacter->SetIdleOn (lIdleOn ? FALSE : TRUE), _T("[%d] SetIdleOn"), mCharacterId);
		ShowCharacterState ();
	}
}

void CAnimationTestDlg::OnAutoPopup()
{
	if	(mCharacter)
	{
		long	lAutoPopup = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetAutoPopupMenu (&lAutoPopup), _T("[%d] GetAutoPopupMenu"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mCharacter->SetAutoPopupMenu (lAutoPopup ? FALSE : TRUE), _T("[%d] SetAutoPopupMenu"), mCharacterId);
		ShowCharacterState ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnSizeNormal()
{
	SetCharacterSize ();
}

void CAnimationTestDlg::OnSizeSmall()
{
	SetCharacterSize ();
}

void CAnimationTestDlg::OnSizeLarge()
{
	SetCharacterSize ();
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnIconShown()
{
	SetCharacterIcon ();
}

void CAnimationTestDlg::OnIconGenerated()
{
	SetCharacterIcon ();
}

void CAnimationTestDlg::OnIconIdentified()
{
	SetCharacterIcon ();
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnAgentProps()
{
	IDaSvrPropertySheetPtr	lPropSheet (mServer);
	long					lVisible = 0;
	HRESULT					lResult;

	if	(lPropSheet != NULL)
	{
		AllowSetForegroundWindow (ASFW_ANY);
		LogComErr (_LOG_AGENT_CALLS, lPropSheet->GetVisible (&lVisible), _T("PropertySheet->GetVisible"));
		lResult = lPropSheet->SetVisible (!lVisible);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("PropertySheet->SetVisible"));
	}
}

void CAnimationTestDlg::OnCharProps()
{
	if	(mServer != NULL)
	{
		HRESULT	lResult;

		AllowSetForegroundWindow (ASFW_ANY);
		lResult = mServer->ShowDefaultCharacterProperties (0, 0, TRUE);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("ShowDefaultCharacterProperties"));
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnTimer(UINT_PTR nIDEvent)
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
	else
	if	(
			(mAllGesturesTimer)
		&&	(nIDEvent == mAllGesturesTimer)
		)
	{
		if	(
				(!IsAnimating())
			||	(++mTimerCount >= 200)
			)
		{
			int	lCurSel = mGestures.GetCurSel ();

			if	(lCurSel < mGestures.GetCount()-1)
			{
				mTimerCount = 0;
				mGestures.SetCurSel (lCurSel+1);
				ShowSelGesture (false);
			}
			else
			{
				mGestures.SetCurSel (0);
				Stop ();
			}
		}
	}
	else
	if	(
			(mAllStatesTimer)
		&&	(nIDEvent == mAllStatesTimer)
		)
	{
		if	(
				(!IsAnimating())
			||	(++mTimerCount >= 200)
			)
		{
			int	lCurSel = mStates.GetCurSel ();

			if	(lCurSel < mStates.GetCount()-1)
			{
				mTimerCount = 0;
				mStates.SetCurSel (lCurSel+1);
				ShowSelState (false);
			}
			else
			{
				mStates.SetCurSel (0);
				Stop ();
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	if	(mCharacter != NULL)
	{
		short	lActive = -1;
		long	lOtherClients = -1;

		if	(bActive)
		{
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetActive (&lActive), _T("[%d] GetActive"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->HasOtherClients (&lOtherClients), _T("[%d] HasOtherClients"), mCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mCharacterId, bActive, lActive, lOtherClients);

			LogComErr (_LOG_CHAR_CALLS, mCharacter->Activate (ACTIVATE_INPUTACTIVE), _T("[%d] Activate ACTIVATE_INPUTACTIVE"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter->GetActive (&lActive), _T("[%d] GetActive"), mCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd]"), mCharacterId, bActive, lActive);
		}
	}
	if	(IsWindow (mAgentWnd->GetSafeHwnd ()))
	{
		mAgentWnd->MakeActiveMedia (bActive!=FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfoCount(UINT *pctinfo)
{
	METHOD_PROLOGUE_EX_(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfoCount"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfoCount (pctinfo);
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	METHOD_PROLOGUE_EX_(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfo"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfo (iTInfo, lcid, ppTInfo);
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	METHOD_PROLOGUE_EX_(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CAnimationTestDlg::XDaSvrNotifySink::GetIDsOfNames"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	METHOD_PROLOGUE_EX(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CAnimationTestDlg::XDaSvrNotifySink::Invoke [%8.8X (%u)]"), pThis, pThis->m_dwRef, dispIdMember, dispIdMember);
#endif
	return pThis->GetIDispatch(FALSE)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Command (long dwCommandID, IUnknown *punkUserInput)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Command"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::ActivateInputState (long dwCharID, long bActivated)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bActivated);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Restart"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Shutdown"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::VisibleState (long dwCharID, long bVisible, long dwCause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	long lCause = -1;
	if	(pThis->mCharacter != NULL)
	{
		pThis->mCharacter->GetVisibilityCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bVisible, dwCause, lCause);
#endif
	pThis->ShowCharacterState ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Click (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DblClick (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DragStart (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DragComplete (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::RequestStart (long dwRequestID)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwRequestID);
#endif
//	if	(
//			(pThis->mCharacter != NULL)
//		&&	(dwRequestID > 101)
//		)
//	{
//		pThis->mCharacter->Stop (dwRequestID);
//	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::RequestComplete (long dwRequestID, long hrStatus)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, dwRequestID, hrStatus);
#endif
	if	(dwRequestID == pThis->mLastAnimationReqID)
	{
		pThis->mLastAnimationReqID = 0;
	}
	if	(dwRequestID == pThis->mLoadReqID)
	{
		pThis->mLoadReqID = 0;
		pThis->LoadedAgentCharacter ();

		if	(PathIsURL (pThis->mCharacterPath))
		{
			pThis->ShowCharacterDetails ();
			pThis->ShowGestures ();
			pThis->ShowStates ();
			pThis->ShowCharacterState ();
		}
	}
	if	(dwRequestID == pThis->mHideReqID)
	{
		pThis->mHideReqID = 0;
		LogComErr (_LOG_CHAR_CALLS, pThis->mCharacter->Show (TRUE, &dwRequestID), _T("[%d] Show"), pThis->mCharacterId);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::BookMark (long dwBookMarkID)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwBookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Idle (long dwCharID, long bStart)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bStart);
#endif
//	if	(bStart)
//	{
//		pThis->ReleaseAgentCharacter ();
//	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Move (long dwCharID, long x, long y, long dwCause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	long lCause = -1;
	if	(pThis->mCharacter != NULL)
	{
		pThis->mCharacter->GetMoveCause (&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, x, y, dwCause, lCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Size (long dwCharID, long lWidth, long lHeight)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, lWidth, lHeight);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::BalloonVisibleState (long dwCharID, long bVisible)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bVisible);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::HelpComplete (long dwCharID, long dwCommandID, long dwCause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::ListeningState (long dwCharID, long bListening, long dwCause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::ListeningState"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR bszGUID)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mCharacterId, pThis->m_dwRef, bszGUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::ActiveClientChange (long dwCharID, long lStatus)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, lStatus);
#endif
	return S_OK;
}
