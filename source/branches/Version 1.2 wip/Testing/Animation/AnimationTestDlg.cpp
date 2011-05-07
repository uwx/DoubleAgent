#include "StdAfx.h"
#include <shlwapi.h>
#include "AnimationTest.h"
#include "AnimationTestDlg.h"
#include "AgentFiles.h"
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
static LPCTSTR	sProfileIdleEnabled = _T("IdleEnabled");
static LPCTSTR	sProfileSoundOn = _T("SoundEnabled");
static LPCTSTR	sProfileAutoPopup = _T("AutoPopup");
static LPCTSTR	sProfileCharSize = _T("CharSize");
static LPCTSTR	sProfileCharSmooth = _T("CharSmooth");
static LPCTSTR	sProfileIconShown = _T("IconShown");
static LPCTSTR	sProfileIconOnLoad = _T("IconOnLoad");
static LPCTSTR	sProfileIconGenerated = _T("IconGenerated");
static LPCTSTR	sProfileIconClipped = _T("IconClipped");
static LPCTSTR	sProfileIconIdentified = _T("IconIdentified");
static LPCTSTR	sProfileUseMsAgent = _T("UseMsAgent");

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
	ON_BN_CLICKED(IDC_SMOOTH_NONE, OnSmooth)
	ON_BN_CLICKED(IDC_SMOOTH_EDGES, OnSmooth)
	ON_BN_CLICKED(IDC_SMOOTH_FULL, OnSmooth)
	ON_BN_CLICKED(IDC_ICON_SHOWONLOAD, OnIconOnLoad)
	ON_BN_CLICKED(IDC_ICON_SHOW, OnIconShown)
	ON_BN_CLICKED(IDC_ICON_GENERATE, OnIconGenerated)
	ON_BN_CLICKED(IDC_ICON_IDENTITY, OnIconIdentified)
	ON_BN_CLICKED(IDC_USE_MSAGENT, OnUseMsAgent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CAnimationTestDlg, CDialog)
	INTERFACE_PART(CAnimationTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
	INTERFACE_PART(CAnimationTestDlg, __uuidof(IAgentNotifySinkEx), DaSvrNotifySink)
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
	DDX_Control(pDX, IDC_IDLE_ON, mIdleEnabled);
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
	DDX_Control(pDX, IDC_SMOOTH_NONE, mSmoothNone);
	DDX_Control(pDX, IDC_SMOOTH_EDGES, mSmoothEdges);
	DDX_Control(pDX, IDC_SMOOTH_FULL, mSmoothFull);
	DDX_Control(pDX, IDC_ICON_SHOW, mIconShown);
	DDX_Control(pDX, IDC_ICON_SHOWONLOAD, mIconOnLoad);
	DDX_Control(pDX, IDC_ICON_GENERATE, mIconGenerated);
	DDX_Control(pDX, IDC_ICON_GENERATE_CLIP, mIconClipped);
	DDX_Control(pDX, IDC_ICON_IDENTITY, mIconIdentified);
	DDX_Control(pDX, IDC_USE_MSAGENT, mUseMsAgent);
	//}}AFX_DATA_MAP
}

BOOL CAnimationTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetWindowText (mWinTitle);
	GetAgentServer ();
	ShowCharacters ();
	LoadConfig ();
	GetAgentServer (mUseMsAgent.GetCheck() ? true : false);
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

	if	(
			(mDaServer)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mDaServer->get_CharacterFiles (&lCharacterFiles))))
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

/**/	mCharacterList.InsertItem (0, _T("C:\\DoubleAgent\\_Test\\Chars\\TestChar1.acs"));
/**/	mCharacterList.InsertItem (0, _T("C:\\DoubleAgent\\_Test\\Chars\\TestChar1Copy.acs"));
/**/	mCharacterList.InsertItem (0, _T("C:\\DoubleAgent\\_Test\\Chars\\Empty.acs"));
/**/	mCharacterList.InsertItem (0, _T("C:\\DoubleAgent\\_Test\\Chars\\EmptyCopy.acs"));
/**/	mCharacterList.InsertItem (0, _T("C:\\DoubleAgent\\_Test\\Chars\\MerlinCopy.acs"));
/**/	mCharacterList.InsertItem (0, _T("file://C:/DoubleAgent/_Test/Chars/TestChar1.acf"));
		mCharacterList.InsertItem (0, _T("<default>"));
	}

	mCharacterList.SetColumnWidth (0, lClientRect.Width());
	mCharacterList.RedrawWindow ();
}

bool CAnimationTestDlg::ShowCharacter (LPCTSTR pCharacterPath)
{
	bool	lRet = false;

#ifndef	_ALWAYS_RELOAD
	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
#endif
	{
		FreeAgentCharacter ();
		CharacterIsVisible (false);
	}

	if	(pCharacterPath)
	{
		if	(
				(pCharacterPath[0])
			&&	(PathIsURL (pCharacterPath))
			)
		{
			lRet = true;
		}
		else
		if	(mMsServer)
		{
			lRet = true;
		}
		else
		{
			try
			{
				CAgentPreviewWnd*	lPreview;
				CRect				lClientRect;

				SafeFreeSafePtr (mAgentPreviewWnd);
				mPreviewWnd.GetClientRect (&lClientRect);

				if	(
						(mAgentPreviewWnd = CAgentPreviewWnd::CreateInstance())
					&&	(mAgentPreviewWnd->Create (mPreviewWnd.m_hWnd, &lClientRect))
					&&	(mAgentPreviewWnd->IsWindow ())
					)
				{
					mAgentPreviewWnd->ShowWindow (SW_HIDE);

					if	(
							(pCharacterPath[0])
						?	(mAgentPreviewWnd->Open (pCharacterPath))
						:	(mAgentPreviewWnd->Open (CAgentFiles::GetDefCharPath()))
						)
					{
						if	(lPreview = dynamic_cast <CAgentPreviewWnd*> (mAgentPreviewWnd.Ptr()))
						{
							lPreview->SetBkColor (GetSysColor (COLOR_WINDOW));
						}
						mAgentPreviewWnd->ShowWindow (SW_SHOWNA);
						lRet = true;
					}
					else
					{
						SafeFreeSafePtr (mAgentPreviewWnd);
					}
				}
				else
				{
					SafeFreeSafePtr (mAgentPreviewWnd);
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
	tBstrPtr	lName;
	tBstrPtr	lDesc;

	if	(mDaCharacter)
	{
		mDaCharacter->GetName (lName.Free());
		mDaCharacter->GetDescription (lDesc.Free());
	}
	else
	if	(mMsCharacter)
	{
		mMsCharacter->GetName (lName.Free());
		mMsCharacter->GetDescription (lDesc.Free());
	}

	mCharacterNameEdit.SetWindowText (CString ((BSTR)lName));
	mCharacterDescEdit.SetWindowText (CString ((BSTR)lDesc));
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::ShowGestures ()
{
	CString	lSelGesture = GetSelGesture ();

	mGestures.SetRedraw (FALSE);
	mGestures.ResetContent ();

	try
	{
		CString				lCharacterPath (mCharacterPath);
		tPtr <CAgentFile>	lAgentFile;

		if	(lCharacterPath.IsEmpty ())
		{
			lCharacterPath = CAgentFiles::GetDefCharPath ();
		}
		if	(
				(!lCharacterPath.IsEmpty ())
			&&	(
					(mDaCharacter)
				||	(mMsCharacter)
				||	(!PathIsURL (mCharacterPath))
				)
			&&	(lAgentFile = CAgentFile::CreateInstance (lCharacterPath))
			&&	(SUCCEEDED (lAgentFile->Open (lCharacterPath)))
			)
		{
			INT_PTR	lNdx;

			mGestures.AddString (_T(""));

			for	(lNdx = 0; lNdx < (INT_PTR)lAgentFile->Gestures.mNames.GetCount(); lNdx++)
			{
				mGestures.AddString (lAgentFile->Gestures.mNames.GetAt (lNdx));
			}

			if	(
					(
						(mDaCharacter)
					||	(mMsCharacter)
					)
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
						mDaCharacter->Prepare (PrepareResource_Animation, _bstr_t(lGestureName), lQueuePrepare, &lReqID);
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
		CString				lCharacterPath (mCharacterPath);
		tPtr <CAgentFile>	lAgentFile;

		if	(lCharacterPath.IsEmpty ())
		{
			lCharacterPath = CAgentFiles::GetDefCharPath ();
		}
		if	(
				(!lCharacterPath.IsEmpty ())
			&&	(
					(mDaCharacter)
				||	(mMsCharacter)
				||	(!PathIsURL (mCharacterPath))
				)
			&&	(lAgentFile = CAgentFile::CreateInstance (lCharacterPath))
			&&	(SUCCEEDED (lAgentFile->Open (lCharacterPath)))
			)
		{
			INT_PTR	lNdx;

			mStates.AddString (_T(""));

			for	(lNdx = 0; lNdx < (INT_PTR)lAgentFile->States.mNames.GetCount(); lNdx++)
			{
				mStates.AddString (lAgentFile->States.mNames.GetAt (lNdx));
			}

			if	(
					(
						(mDaCharacter)
					||	(mMsCharacter)
					)
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
						mDaCharacter->Prepare (PrepareResource_State, _bstr_t(lStateName), lQueuePrepare, &lReqID);
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
				if	(mDaCharacter)
				{
					LogComErr (_LOG_CHAR_CALLS, mDaCharacter->Stop (mLastAnimationReqID));
				}
				else
				if	(mMsCharacter)
				{
					LogComErr (_LOG_CHAR_CALLS, mMsCharacter->Stop (mLastAnimationReqID));
				}
				mLastAnimationReqID = 0;
			}

			if	(mDaCharacter)
			{
				lResult = mDaCharacter->Play (_bstr_t(lSelGesture), &mLastAnimationReqID);
			}
			else
			if	(mMsCharacter)
			{
				lResult = mMsCharacter->Play (_bstr_t(lSelGesture), &mLastAnimationReqID);
			}
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Play [%s] [%d]"), mCharacterId, lSelGesture, mLastAnimationReqID)))
			{
				lRet = true;
#if	FALSE
				if	(lSelGesture.CompareNoCase (_T("DoMagic1")) == 0)
				{
					LogMessage (LogDebug, _T("DoMagic1 [%d]"), mLastAnimationReqID);
					if	(mDaCharacter)
					{
						LogComErr (_LOG_CHAR_CALLS, mDaCharacter->Play (_bstr_t(_T("DoMagic2")), &mLastAnimationReqID));
					}
					LogMessage (LogDebug, _T("DoMagic2 [%d]"), mLastAnimationReqID);
					if	(mDaCharacter)
					{
						LogComErr (_LOG_CHAR_CALLS, mDaCharacter->Play (_bstr_t(_T("RestPose")), &mLastAnimationReqID));
					}
					LogMessage (LogDebug, _T("RestPose [%d]"), mLastAnimationReqID);
				}
#endif
#if	TRUE
				if	(lSelGesture.CompareNoCase (_T("DoMagic1")) == 0)
				{
					if	(mDaCharacter)
					{
						mDaCharacter->Play (_bstr_t("DoMagic2"), &mLastAnimationReqID);
						mDaCharacter->Play (_bstr_t("RestPose"), &mLastAnimationReqID);
					}
					else
					if	(mMsCharacter)
					{
						mMsCharacter->Play (_bstr_t("DoMagic2"), &mLastAnimationReqID);
						mMsCharacter->Play (_bstr_t("RestPose"), &mLastAnimationReqID);
					}
				}
				else
				if	(lSelGesture.CompareNoCase (_T("DoMagic2")) == 0)
				{
					if	(mDaCharacter)
					{
						mDaCharacter->Play (_bstr_t("RestPose"), &mLastAnimationReqID);
					}
					else
					if	(mMsCharacter)
					{
						mMsCharacter->Play (_bstr_t("RestPose"), &mLastAnimationReqID);
					}
				}
#endif
			}
		}

		if	(
				(!mMsServer)
			&&	(!mMsCharacter)
			&&	(
					(!IsCharacterVisible ())
				||	(mAnimateBoth.GetCheck())
				)
			&&	(mAgentPreviewWnd)
			&&	(mAgentPreviewWnd->IsWindow ())
			)
		{
			lRet = mAgentPreviewWnd->ShowGesture (lSelGesture, NULL, true, pStopFirst);

			if	(
					(!lRet)
				||	(mAgentPreviewWnd->IsPaused())
				||	(mAgentPreviewWnd->IsStopped())
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
				if	(mDaCharacter)
				{
					LogComErr (_LOG_CHAR_CALLS, mDaCharacter->Stop (mLastAnimationReqID));
				}
				else
				if	(mMsCharacter)
				{
					LogComErr (_LOG_CHAR_CALLS, mMsCharacter->Stop (mLastAnimationReqID));
				}
				mLastAnimationReqID = 0;
			}

			if	(mDaCharacter)
			{
				LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GetSize (&lCharRect.right, &lCharRect.bottom), _T("[%d] GetSize"), mCharacterId);
				LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GetPosition (&lCharRect.left, &lCharRect.top), _T("[%d] GetPosition"), mCharacterId);
			}
			else
			if	(mMsCharacter)
			{
				LogComErr (_LOG_CHAR_CALLS_EX, mMsCharacter->GetSize (&lCharRect.right, &lCharRect.bottom), _T("[%d] GetSize"), mCharacterId);
				LogComErr (_LOG_CHAR_CALLS_EX, mMsCharacter->GetPosition (&lCharRect.left, &lCharRect.top), _T("[%d] GetPosition"), mCharacterId);
			}
			lCharRect.right += lCharRect.left;
			lCharRect.bottom += lCharRect.top;

			if	(lSelState.CompareNoCase (_T("GESTURINGDOWN")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->GestureAt ((short)lCharRect.CenterPoint().x, (short)lCharRect.bottom, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->GestureAt ((short)lCharRect.CenterPoint().x, (short)lCharRect.bottom, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureDown [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("GESTURINGUP")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->GestureAt ((short)lCharRect.CenterPoint().x, (short)lCharRect.top, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->GestureAt ((short)lCharRect.CenterPoint().x, (short)lCharRect.top, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureUp [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("GESTURINGRIGHT")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->GestureAt ((short)lCharRect.left, (short)lCharRect.CenterPoint().y, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->GestureAt ((short)lCharRect.left, (short)lCharRect.CenterPoint().y, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureRight [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("GESTURINGLEFT")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->GestureAt ((short)lCharRect.right, (short)lCharRect.CenterPoint().y, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->GestureAt ((short)lCharRect.right, (short)lCharRect.CenterPoint().y, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] GestureLeft [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGDOWN")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top+lCharRect.Height()*2, 1000, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top+lCharRect.Height()*2, 1000, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveDown [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGUP")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top-lCharRect.Height()*2, 1000, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->MoveTo ((short)lCharRect.left, (short)lCharRect.top-lCharRect.Height()*2, 1000, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveUp [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGRIGHT")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->MoveTo ((short)lCharRect.left-lCharRect.Width()*2, (short)lCharRect.top, 1000, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->MoveTo ((short)lCharRect.left-lCharRect.Width()*2, (short)lCharRect.top, 1000, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveRight [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("MOVINGLEFT")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->MoveTo ((short)lCharRect.left+lCharRect.Width()*2, (short)lCharRect.top, 1000, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->MoveTo ((short)lCharRect.left+lCharRect.Width()*2, (short)lCharRect.top, 1000, &mLastAnimationReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] MoveLeft [%d]"), mCharacterId, mLastAnimationReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("HIDING")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->Hide (FALSE, &mHideReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->Hide (FALSE, &mHideReqID);
				}
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, mHideReqID);
			}
			else
			if	(lSelState.CompareNoCase (_T("SHOWING")) == 0)
			{
				if	(mDaCharacter)
				{
					lResult = mDaCharacter->Hide (TRUE, &mLastAnimationReqID);
					lResult = mDaCharacter->Show (FALSE, &mLastAnimationReqID);
				}
				else
				if	(mMsCharacter)
				{
					lResult = mMsCharacter->Hide (TRUE, &mLastAnimationReqID);
					lResult = mMsCharacter->Show (FALSE, &mLastAnimationReqID);
				}
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
				if	(mDaCharacter)
				{
					if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mDaCharacter->SetIdleOn (TRUE), _T("[%d] SetIdleOn"), mCharacterId)))
					{
						lRet = true;
					}
				}
				else
				if	(mMsCharacter)
				{
					if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mMsCharacter->SetIdleOn (TRUE), _T("[%d] SetIdleOn"), mCharacterId)))
					{
						lRet = true;
					}
				}
			}
			else
			{
				if	(mDaCharacter)
				{
					LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->SetIdleOn (FALSE), _T("[%d] SetIdleOff"), mCharacterId);
				}
				else
				if	(mMsCharacter)
				{
					LogComErr (_LOG_CHAR_CALLS_EX, mMsCharacter->SetIdleOn (FALSE), _T("[%d] SetIdleOff"), mCharacterId);
				}
			}
		}

		if	(
				(!mMsServer)
			&&	(!mMsCharacter)
			&&	(
					(!lCharacterVisible)
				||	(mAnimateBoth.GetCheck())
				)
			&&	(mAgentPreviewWnd)
			)
		{
			lRet = mAgentPreviewWnd->ShowState (lSelState, true, pStopFirst);
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
			(
				(mDaCharacter)
			||	(mMsCharacter != NULL)
			)
		&&	(mLastAnimationReqID > 0)
		)
	{
		return true;;
	}
	if	(
			(!mMsServer)
		&&	(mAgentPreviewWnd)
		&&	(mAgentPreviewWnd->IsWindow ())
		&&	(!mAgentPreviewWnd->IsAnimationComplete ())
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
			(!mMsServer)
		&&	(mAgentPreviewWnd)
		&&	(mAgentPreviewWnd->IsWindow ())
		&&	(mAgentPreviewWnd->Stop () == S_OK)
		)
	{
		lRet = true;
	}

	if	(mDaCharacter)
	{
		LogComErr (_LOG_CHAR_CALLS, mDaCharacter->StopAll (StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility), _T("[%d] StopAll"), mCharacterId);
	}
	else
	if	(mMsCharacter != NULL)
	{
		LogComErr (_LOG_CHAR_CALLS, mMsCharacter->StopAll (StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility), _T("[%d] StopAll"), mCharacterId);
	}

	mTimerCount = 0;
	mLastAnimationReqID = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::GetAgentServer (bool pMsAgent)
{
	if	(pMsAgent)
	{
		if	(mMsServer == NULL)
		{
			IUnknownPtr	lUnknown;

			FreeAgentServer ();
			LogComErr (_LOG_AGENT_CALLS, CoCreateInstance (__uuidof(AgentServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("CoCreateInstance"));
			mMsServer = lUnknown;

			//if	(mMsServer != NULL)
			//{
			//	LogComErr (_LOG_AGENT_CALLS, mMsServer->Register (&m_xDaSvrNotifySink, &mNotifySinkId), _T("Register"));
			//}
		}
	}
	else
	{
		if	(mDaServer == NULL)
		{
			IUnknownPtr	lUnknown;

			FreeAgentServer ();
			LogComErr (_LOG_AGENT_CALLS, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("CoCreateInstance"));
			mDaServer = lUnknown;

			if	(mDaServer)
			{
				LogComErr (_LOG_AGENT_CALLS, mDaServer->Register (&m_xDaSvrNotifySink, &mNotifySinkId), _T("Register"));
			}
		}
	}
}

void CAnimationTestDlg::FreeAgentServer ()
{
	if	(
			(mDaServer)
		&&	(mNotifySinkId)
		)
	{
		try
		{
			LogComErr (_LOG_AGENT_CALLS, mDaServer->Unregister (mNotifySinkId), _T("Unregister"));
		}
		catch AnyExceptionDebug
	}
	else
	if	(
			(mMsServer)
		&&	(mNotifySinkId)
		)
	{
		try
		{
			LogComErr (_LOG_AGENT_CALLS, mMsServer->Unregister (mNotifySinkId), _T("Unregister"));
		}
		catch AnyExceptionDebug
	}
	mNotifySinkId = 0;

	SafeFreeSafePtr (mMsServer);
	SafeFreeSafePtr (mDaServer);
}

/////////////////////////////////////////////////////////////////////////////

bool CAnimationTestDlg::ShowAgentCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	GetAgentServer (mUseMsAgent.GetCheck() ? true : false);
	mLoadReqID = 0;

	if	(mDaServer)
	{
		LogComErr (_LOG_AGENT_CALLS, mDaServer->put_CharacterStyle	((mSoundOn.GetCheck()?CharacterStyle_SoundEffects:0)
																	|(mIdleEnabled.GetCheck()?CharacterStyle_IdleEnabled:0)
																	|(mAutoPopup.GetCheck()?CharacterStyle_AutoPopupMenu:0)
																	|(mIconOnLoad.GetCheck()?CharacterStyle_IconShown:0)
																	|(mSmoothFull.GetCheck()?CharacterStyle_Smoothed:mSmoothEdges.GetCheck()?CharacterStyle_SmoothEdges:0)
																	));
	}

	if	(
			(mDaServer)
		&&	(mCharacterId == 0)
		)
	{
		_variant_t	lProvider;

		if	(!mCharacterPath.IsEmpty ())
		{
			lProvider = mCharacterPath;
		}
		lResult = mDaServer->Load (lProvider, &mCharacterId, &mLoadReqID);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("Load [%d] [%s] as [%d]"), mLoadReqID, mCharacterPath, mCharacterId);
	}
	else
	if	(
			(mMsServer != NULL)
		&&	(mCharacterId == 0)
		)
	{
		_variant_t	lProvider;

		if	(!mCharacterPath.IsEmpty ())
		{
			lProvider = mCharacterPath;
		}
		lResult = mMsServer->Load (lProvider, &mCharacterId, &mLoadReqID);
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

	IDaSvrCharacterPtr		lDaCharacter;
	IAgentCharacterExPtr	lMsCharacter;

	if	(
			(mDaServer)
		&&	(mCharacterId != 0)
		&&	(mDaCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mDaServer->GetCharacterEx (mCharacterId, &lDaCharacter), _T("GetCharacterEx"))))
		)
	{
		mDaCharacter = lDaCharacter;
	}
	else
	if	(
			(mMsServer != NULL)
		&&	(mCharacterId != 0)
		&&	(mMsCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mMsServer->GetCharacterEx (mCharacterId, &lMsCharacter), _T("GetCharacterEx"))))
		)
	{
		mMsCharacter = lMsCharacter;
	}

	if	(mDaCharacter)
	{
//		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GenerateIcon ());
//		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->SetIdleOn (mIdleEnabled.GetCheck()?TRUE:FALSE), _T("[%d] SetIdleOn"), mCharacterId);
//		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->SetSoundEffectsOn (mSoundOn.GetCheck()?TRUE:FALSE), _T("[%d] SetSoundEffectsOn"), mCharacterId);
//		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->SetAutoPopupMenu (mAutoPopup.GetCheck()?TRUE:FALSE), _T("[%d] SetSoundEffectsOn"), mCharacterId);

		if	(SUCCEEDED (mDaCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			lCharPos.x = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			lCharPos.y = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
			mDaCharacter->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
		}
		SetCharacterSize ();
		SetCharacterIcon ();

		lResult = mDaCharacter->Show (mFastShowHide.GetCheck() ? TRUE : FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}

		lWinTitle.Format (_T("%s [%d]"), mWinTitle, mCharacterId);
		SetWindowText (lWinTitle);
		lRet = true;
	}
	else
	if	(mMsCharacter != NULL)
	{
		if	(SUCCEEDED (mMsCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			lCharPos.x = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			lCharPos.y = AfxGetApp()->GetProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
			mMsCharacter->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
		}
		SetCharacterSize ();

		lResult = mMsCharacter->Show (mFastShowHide.GetCheck() ? TRUE : FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}

		lWinTitle.Format (_T("%s [%d] (Native)"), mWinTitle, mCharacterId);
		SetWindowText (lWinTitle);
		lRet = true;
	}

	ShowCharacterDetails ();
	return lRet;
}

bool CAnimationTestDlg::HideAgentCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	CPoint	lCharPos;
	HRESULT	lResult;

	if	(
			(mDaCharacter)
		&&	(IsCharacterVisible ())
		)
	{
		if	(SUCCEEDED (mDaCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
		}
		lResult = mDaCharacter->Hide (mFastShowHide.GetCheck() ? TRUE : FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	else
	if	(
			(mMsCharacter != NULL)
		&&	(IsCharacterVisible ())
		)
	{
		if	(SUCCEEDED (mMsCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
		}
		lResult = mMsCharacter->Hide (mFastShowHide.GetCheck() ? TRUE : FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CAnimationTestDlg::FreeAgentCharacter ()
{
	bool	lRet = false;
	CPoint	lCharPos;

	if	(mDaCharacter)
	{
		if	(SUCCEEDED (mDaCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
		}
		mDaCharacter = NULL;

		if	(
				(mDaServer)
			&&	(mCharacterId != 0)
			)
		{
			LogComErr (_LOG_AGENT_CALLS, mDaServer->Unload (mCharacterId), _T("Unload [%d]"), mCharacterId);
		}
		lRet = true;
	}
	if	(mMsCharacter != NULL)
	{
		if	(SUCCEEDED (mMsCharacter->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosX, lCharPos.x);
			AfxGetApp()->WriteProfileInt (sProfileKey, sProfileCharPosY, lCharPos.y);
		}
		mMsCharacter = NULL;

		if	(
				(mMsServer != NULL)
			&&	(mCharacterId != 0)
			)
		{
			LogComErr (_LOG_AGENT_CALLS, mMsServer->Unload (mCharacterId), _T("Unload [%d]"), mCharacterId);
		}
		lRet = true;
	}
	mCharacterId = 0;
	if	(lRet)
	{
		SetWindowText (mWinTitle);
	}
	return lRet;
}

void CAnimationTestDlg::SetCharacterSize()
{
	CSize	lCharSize;

	if	(
			(mDaCharacter)
		&&	(SUCCEEDED (mDaCharacter->GetOriginalSize (&lCharSize.cx, &lCharSize.cy)))
		)
	{
		if	(mSizeSmall.GetCheck ())
		{
			mDaCharacter->SetSize (MulDiv (lCharSize.cx, 2, 3), MulDiv (lCharSize.cy, 2, 3));
		}
		else
		if	(mSizeLarge.GetCheck ())
		{
			mDaCharacter->SetSize (MulDiv (lCharSize.cx, 3, 2), MulDiv (lCharSize.cy, 3, 2));
		}
		else
		{
			mDaCharacter->SetSize (lCharSize.cx, lCharSize.cy);
		}
	}
	else
	if	(
			(mMsCharacter != NULL)
		&&	(SUCCEEDED (mMsCharacter->GetOriginalSize (&lCharSize.cx, &lCharSize.cy)))
		)
	{
		if	(mSizeSmall.GetCheck ())
		{
			mMsCharacter->SetSize (MulDiv (lCharSize.cx, 2, 3), MulDiv (lCharSize.cy, 2, 3));
		}
		else
		if	(mSizeLarge.GetCheck ())
		{
			mMsCharacter->SetSize (MulDiv (lCharSize.cx, 3, 2), MulDiv (lCharSize.cy, 3, 2));
		}
		else
		{
			mMsCharacter->SetSize (lCharSize.cx, lCharSize.cy);
		}
	}
}

void CAnimationTestDlg::SetCharacterIcon()
{
	long	lCharacterStyle;

	if	(mDaCharacter)
	{
		if	(mIconIdentified.GetCheck())
		{
			tBstrPtr	lGUID;
			LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->get_UniqueID (lGUID.Free()));
			LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->put_IconIdentity (lGUID));
		}
		else
		{
			LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->put_IconIdentity (_bstr_t("")));
		}

		if	(mIconGenerated.GetCheck())
		{
			CSize	lCharSize;

			if	(
					(mIconClipped.GetCheck ())
				&&	(SUCCEEDED (mDaCharacter->GetOriginalSize (&lCharSize.cx, &lCharSize.cy)))
				)
			{
				LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GenerateIcon (0, 0, lCharSize.cx, lCharSize.cy/2));
			}
			else
			{
				LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GenerateIcon ());
			}
		}

		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->get_Style (&lCharacterStyle));
		if	(mIconShown.GetCheck ())
		{
			lCharacterStyle |= CharacterStyle_IconShown;
		}
		else
		{
			lCharacterStyle &= ~CharacterStyle_IconShown;
		}
		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->put_Style (lCharacterStyle));
	}

#if	FALSE
	CString		lIconTip;
	lIconTip.Format (_T("%u Animation Test"), GetCurrentProcessId());
	LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->put_IconTip (_bstr_t(lIconTip)));
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CAnimationTestDlg::IsCharacterVisible ()
{
	if	(mDaCharacter)
	{
		VARIANT_BOOL	lVisible = VARIANT_FALSE;

		if	(
				(SUCCEEDED (mDaCharacter->get_Visible (&lVisible)))
			&&	(lVisible)
			)
		{
			return true;
		}
	}
	else
	if	(mMsCharacter)
	{
		long	lVisible = FALSE;

		if	(
				(SUCCEEDED (mMsCharacter->GetVisible (&lVisible)))
			&&	(lVisible)
			)
		{
			return true;
		}
	}
	return false;
}

void CAnimationTestDlg::CharacterIsVisible (bool pVisible)
{
	if	(pVisible)
	{
		mPopupButton.SetCheck (TRUE);
		mAnimateBoth.EnableWindow (mMsCharacter == NULL);
	}
	else
	{
		mPopupButton.SetCheck (FALSE);
		mAnimateBoth.EnableWindow (FALSE);
	}

	if	(mDaCharacter)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->get_Style (&lCharacterStyle), _T("[%d] get_Style"), mCharacterId);

		mSoundOn.SetCheck ((lCharacterStyle & CharacterStyle_SoundEffects) ? TRUE : FALSE);
		mIdleEnabled.SetCheck ((lCharacterStyle & CharacterStyle_IdleEnabled) ? TRUE : FALSE);
		mAutoPopup.SetCheck ((lCharacterStyle & CharacterStyle_AutoPopupMenu) ? TRUE : FALSE);
		mIconShown.SetCheck ((lCharacterStyle & CharacterStyle_IconShown) ? TRUE : FALSE);
		mSmoothNone.SetCheck ((lCharacterStyle & CharacterStyle_Smoothed) ? FALSE : TRUE);
		mSmoothEdges.SetCheck ((lCharacterStyle & CharacterStyle_Smoothed) == CharacterStyle_SmoothEdges);
		mSmoothFull.SetCheck ((lCharacterStyle & CharacterStyle_Smoothed) == CharacterStyle_Smoothed);

		mSoundOn.EnableWindow (TRUE);
		mIdleEnabled.EnableWindow (TRUE);
		mAutoPopup.EnableWindow (TRUE);
		mIconShown.EnableWindow (TRUE);
		mSmoothNone.EnableWindow (FALSE);
		mSmoothEdges.EnableWindow (FALSE);
		mSmoothFull.EnableWindow (FALSE);
	}
	else
	if	(mMsCharacter != NULL)
	{
		long	lSoundOn = FALSE;
		long	lIdleOn = FALSE;
		long	lAutoPop = FALSE;

		mMsCharacter->GetSoundEffectsOn (&lSoundOn);
		mMsCharacter->GetIdleOn (&lIdleOn);
		mMsCharacter->GetAutoPopupMenu (&lAutoPop);

		mSoundOn.SetCheck (lSoundOn ? TRUE : FALSE);
		mIdleEnabled.SetCheck (lIdleOn ? TRUE : FALSE);
		mAutoPopup.SetCheck (lAutoPop ? TRUE : FALSE);
		mIconShown.SetCheck (FALSE);
		mSmoothNone.SetCheck (FALSE);
		mSmoothEdges.SetCheck (FALSE);
		mSmoothFull.SetCheck (FALSE);

		mSoundOn.EnableWindow (TRUE);
		mIdleEnabled.EnableWindow (TRUE);
		mAutoPopup.EnableWindow (TRUE);
		mIconShown.EnableWindow (FALSE);
		mSmoothNone.EnableWindow (FALSE);
		mSmoothEdges.EnableWindow (FALSE);
		mSmoothFull.EnableWindow (FALSE);
	}
	else
	if	(mDaServer)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_AGENT_CALLS, mDaServer->get_CharacterStyle (&lCharacterStyle), _T("get_CharacterStyle"));

		mSoundOn.SetCheck ((lCharacterStyle & CharacterStyle_SoundEffects) ? TRUE : FALSE);
		mIdleEnabled.SetCheck ((lCharacterStyle & CharacterStyle_IdleEnabled) ? TRUE : FALSE);
		mAutoPopup.SetCheck ((lCharacterStyle & CharacterStyle_AutoPopupMenu) ? TRUE : FALSE);
		mIconOnLoad.SetCheck ((lCharacterStyle & CharacterStyle_IconShown) ? TRUE : FALSE);
		mSmoothNone.SetCheck ((lCharacterStyle & CharacterStyle_Smoothed) ? FALSE : TRUE);
		mSmoothEdges.SetCheck ((lCharacterStyle & CharacterStyle_Smoothed) == CharacterStyle_SmoothEdges);
		mSmoothFull.SetCheck ((lCharacterStyle & CharacterStyle_Smoothed) == CharacterStyle_Smoothed);
		mSoundOn.EnableWindow (TRUE);

		mIdleEnabled.EnableWindow (TRUE);
		mAutoPopup.EnableWindow (TRUE);
		mIconShown.EnableWindow (TRUE);
		mSmoothNone.EnableWindow (TRUE);
		mSmoothEdges.EnableWindow (TRUE);
		mSmoothFull.EnableWindow (TRUE);
	}
	else
	{
		mSoundOn.SetCheck (FALSE);
		mIdleEnabled.SetCheck (FALSE);
		mAutoPopup.SetCheck (FALSE);
		mSmoothNone.SetCheck (FALSE);
		mSmoothEdges.SetCheck (FALSE);
		mSmoothFull.SetCheck (FALSE);

		mSoundOn.EnableWindow (FALSE);
		mIdleEnabled.EnableWindow (FALSE);
		mAutoPopup.EnableWindow (FALSE);
		mSmoothNone.EnableWindow (FALSE);
		mSmoothEdges.EnableWindow (FALSE);
		mSmoothFull.EnableWindow (FALSE);
	}
}

void CAnimationTestDlg::ShowCharacterState ()
{
	if	(
			(!mMsServer)
		&&	(mAgentPreviewWnd)
		&&	(mAgentPreviewWnd->IsWindow ())
		)
	{
		mAgentPreviewWnd->EnableSound (mDaCharacter == NULL);
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
	int			lCharSize = lApp->GetProfileInt (sProfileKey, sProfileCharSize, 0);
	int			lCharSmooth = lApp->GetProfileInt (sProfileKey, sProfileCharSmooth, 0);

	mSizeNormal.SetCheck (lCharSize == 0);
	mSizeLarge.SetCheck (lCharSize > 0);
	mSizeSmall.SetCheck (lCharSize < 0);
	mSmoothNone.SetCheck (lCharSmooth == 0);
	mSmoothEdges.SetCheck (lCharSmooth == 1);
	mSmoothFull.SetCheck (lCharSmooth == 2);

	mAnimateBoth.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileBoth, mAnimateBoth.GetCheck()) ? TRUE : FALSE);
	mFastShowHide.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileFast, mFastShowHide.GetCheck()) ? TRUE : FALSE);
	mIdleEnabled.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIdleEnabled, mIdleEnabled.GetCheck()) ? TRUE : FALSE);
	mSoundOn.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileSoundOn, mSoundOn.GetCheck()) ? TRUE : FALSE);
	mAutoPopup.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileAutoPopup, mAutoPopup.GetCheck()) ? TRUE : FALSE);
	mIconShown.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconShown, mIconShown.GetCheck()) ? TRUE : FALSE);
	mIconOnLoad.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconOnLoad, mIconOnLoad.GetCheck()) ? TRUE : FALSE);
	mIconGenerated.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconGenerated, mIconGenerated.GetCheck()) ? TRUE : FALSE);
	mIconClipped.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconClipped, mIconClipped.GetCheck()) ? TRUE : FALSE);
	mIconIdentified.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileIconIdentified, mIconIdentified.GetCheck()) ? TRUE : FALSE);
	mUseMsAgent.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileUseMsAgent, mUseMsAgent.GetCheck()) ? TRUE : FALSE);

	if	(mDaServer)
	{
		LogComErr (_LOG_AGENT_CALLS, mDaServer->put_CharacterStyle	((mSoundOn.GetCheck()?CharacterStyle_SoundEffects:0)
																	|(mIdleEnabled.GetCheck()?CharacterStyle_IdleEnabled:0)
																	|(mAutoPopup.GetCheck()?CharacterStyle_AutoPopupMenu:0)
																	|(mIconOnLoad.GetCheck()?CharacterStyle_IconShown:0)
																	|(mSmoothFull.GetCheck()?CharacterStyle_Smoothed:mSmoothEdges.GetCheck()?CharacterStyle_SmoothEdges:0)
																	));
	}
	if	(ShowCharacter (lApp->GetProfileString (sProfileKey, sProfileCharacter, mCharacterPath)))
	{
		tS <LVFINDINFO>	lFindInfo;
		int				lFoundNdx;

		if	(mCharacterPath.IsEmpty ())
		{
			lFoundNdx = 0;
		}
		else
		{
			lFindInfo.flags = LVFI_STRING;
			lFindInfo.psz = mCharacterPath;
			lFindInfo.vkDirection = VK_NEXT;
			lFoundNdx = ListView_FindItem (mCharacterList.m_hWnd, -1, &lFindInfo);
		}
		if	(lFoundNdx >= 0)
		{
			mCharacterList.SetItemState (lFoundNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}

//	mGestures.SetCurSel (max (mGestures.FindStringExact (1, lApp->GetProfileString (sProfileKey, sProfileGesture, GetSelGesture())), 0));
//	mStates.SetCurSel (max (mStates.FindStringExact (1, lApp->GetProfileString (sProfileKey, sProfileState, GetSelState())), 0));
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
	lApp->WriteProfileInt (sProfileKey, sProfileIdleEnabled, mIdleEnabled.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileSoundOn, mSoundOn.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileAutoPopup, mAutoPopup.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileCharSize, mSizeSmall.GetCheck()?-1:mSizeLarge.GetCheck()?1:0);
	lApp->WriteProfileInt (sProfileKey, sProfileCharSmooth, mSmoothFull.GetCheck()?2:mSmoothEdges.GetCheck()?1:0);
	lApp->WriteProfileInt (sProfileKey, sProfileIconShown, mIconShown.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconOnLoad, mIconOnLoad.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconGenerated, mIconGenerated.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconClipped, mIconClipped.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileIconIdentified, mIconIdentified.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileUseMsAgent, mUseMsAgent.GetCheck());

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
	if	(
			(mAgentPreviewWnd)
		&&	(mAgentPreviewWnd->IsWindow ())
		)
	{
		mAgentPreviewWnd->Close ();
	}
	CDialog::EndDialog (IDOK);
}

void CAnimationTestDlg::OnDestroy()
{
	SaveConfig ();
	FreeAgentCharacter ();
	FreeAgentServer ();

	if	(
			(mAgentPreviewWnd)
		&&	(mAgentPreviewWnd->IsWindow ())
		)
	{
		mAgentPreviewWnd->Close ();
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
		if	(lNotify->iItem == 0)
		{
			lCharacterPath.Empty ();
		}
		else
		{
			lCharacterPath = mCharacterList.GetItemText (lNotify->iItem, 0);
		}
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
	CString			lCharacterPath;

	if	(lNotify->iItem == 0)
	{
		lCharacterPath.Empty ();
	}
	else
	{
		lCharacterPath = mCharacterList.GetItemText (lNotify->iItem, 0);
	}
	if	(ShowCharacter (lCharacterPath))
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
	if	(mDaServer)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->get_CharacterStyle (&lCharacterStyle));
		if	(mSoundOn.GetCheck())
		{
			lCharacterStyle |= CharacterStyle_SoundEffects;
		}
		else
		{
			lCharacterStyle &= ~CharacterStyle_SoundEffects;
		}
		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->put_CharacterStyle (lCharacterStyle));
	}

	if	(mDaCharacter)
	{
		long	lSoundOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GetSoundEffectsOn (&lSoundOn), _T("[%d] GetSoundEffectsOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mDaCharacter->SetSoundEffectsOn (mSoundOn.GetCheck() ? TRUE : FALSE), _T("[%d] SetSoundEffectsOn"), mCharacterId);
	}
	else
	if	(mMsCharacter)
	{
		long	lSoundOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mMsCharacter->GetSoundEffectsOn (&lSoundOn), _T("[%d] GetSoundEffectsOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mMsCharacter->SetSoundEffectsOn (mSoundOn.GetCheck() ? TRUE : FALSE), _T("[%d] SetSoundEffectsOn"), mCharacterId);
	}
	ShowCharacterState ();
}

void CAnimationTestDlg::OnIdleOn()
{
	if	(mDaServer)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->get_CharacterStyle (&lCharacterStyle));
		if	(mIdleEnabled.GetCheck())
		{
			lCharacterStyle |= CharacterStyle_IdleEnabled;
		}
		else
		{
			lCharacterStyle &= ~CharacterStyle_IdleEnabled;
		}
		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->put_CharacterStyle (lCharacterStyle));
	}

	if	(mDaCharacter)
	{
		long	lIdleOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GetIdleOn (&lIdleOn), _T("[%d] GetIdleOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mDaCharacter->SetIdleOn (mIdleEnabled.GetCheck() ? TRUE : FALSE), _T("[%d] SetIdleOn"), mCharacterId);
	}
	else
	if	(mMsCharacter)
	{
		long	lIdleOn = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mMsCharacter->GetIdleOn (&lIdleOn), _T("[%d] GetIdleOn"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mMsCharacter->SetIdleOn (mIdleEnabled.GetCheck() ? TRUE : FALSE), _T("[%d] SetIdleOn"), mCharacterId);
	}
	ShowCharacterState ();
}

void CAnimationTestDlg::OnAutoPopup()
{
	if	(mDaServer)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->get_CharacterStyle (&lCharacterStyle));
		if	(mAutoPopup.GetCheck())
		{
			lCharacterStyle |= CharacterStyle_AutoPopupMenu;
		}
		else
		{
			lCharacterStyle &= ~CharacterStyle_AutoPopupMenu;
		}
		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->put_CharacterStyle (lCharacterStyle));
	}

	if	(mDaCharacter)
	{
		long	lAutoPopup = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->GetAutoPopupMenu (&lAutoPopup), _T("[%d] GetAutoPopupMenu"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mDaCharacter->SetAutoPopupMenu (mAutoPopup.GetCheck() ? TRUE : FALSE), _T("[%d] SetAutoPopupMenu"), mCharacterId);
	}
	else
	if	(mMsCharacter)
	{
		long	lAutoPopup = 0;

		LogComErr (_LOG_CHAR_CALLS_EX, mMsCharacter->GetAutoPopupMenu (&lAutoPopup), _T("[%d] GetAutoPopupMenu"), mCharacterId);
		LogComErr (_LOG_CHAR_CALLS, mMsCharacter->SetAutoPopupMenu (mAutoPopup.GetCheck() ? TRUE : FALSE), _T("[%d] SetAutoPopupMenu"), mCharacterId);
	}
	ShowCharacterState ();
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

void CAnimationTestDlg::OnSmooth()
{
	if	(mDaServer)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->get_CharacterStyle (&lCharacterStyle));
		if	(mSmoothFull.GetCheck())
		{
			lCharacterStyle |= CharacterStyle_Smoothed;
		}
		else
		if	(mSmoothEdges.GetCheck())
		{
			lCharacterStyle &= ~CharacterStyle_Smoothed;
			lCharacterStyle |= CharacterStyle_SmoothEdges;
		}
		else
		{
			lCharacterStyle &= ~CharacterStyle_Smoothed;
		}
		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->put_CharacterStyle (lCharacterStyle));
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnIconOnLoad()
{
	if	(mDaServer)
	{
		long	lCharacterStyle;

		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->get_CharacterStyle (&lCharacterStyle));
		if	(mIconOnLoad.GetCheck())
		{
			lCharacterStyle |= CharacterStyle_IconShown;
		}
		else
		{
			lCharacterStyle &= ~CharacterStyle_IconShown;
		}
		LogComErr (_LOG_CHAR_CALLS_EX, mDaServer->put_CharacterStyle (lCharacterStyle));
	}
	ShowCharacterState ();
}

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

void CAnimationTestDlg::OnUseMsAgent()
{
	FreeAgentCharacter ();
	FreeAgentServer ();
	GetAgentServer (mUseMsAgent.GetCheck() ? true : false);
	ShowCharacterState ();
}

/////////////////////////////////////////////////////////////////////////////

void CAnimationTestDlg::OnAgentProps()
{
	HRESULT		lResult;

	if	(mDaServer)
	{
		IDaSvrPropertySheet2Ptr	lPropSheet (mDaServer);
		VARIANT_BOOL			lVisible = VARIANT_FALSE;

		if	(lPropSheet != NULL)
		{
			AllowSetForegroundWindow (ASFW_ANY);
			LogComErr (_LOG_AGENT_CALLS, lPropSheet->get_Visible (&lVisible), _T("PropertySheet->get_Visible"));
			lResult = lPropSheet->put_Visible (lVisible?VARIANT_FALSE:VARIANT_TRUE);
			LogComErr (_LOG_AGENT_CALLS, lResult, _T("PropertySheet->put_Visible"));
		}
	}
	else
	if	(mMsServer)
	{
		IAgentPropertySheetPtr	lPropSheet (mMsServer);
		long					lVisible = FALSE;

		if	(lPropSheet != NULL)
		{
			AllowSetForegroundWindow (ASFW_ANY);
			LogComErr (_LOG_AGENT_CALLS, lPropSheet->GetVisible (&lVisible), _T("PropertySheet->GetVisible"));
			lResult = lPropSheet->SetVisible (lVisible?FALSE:TRUE);
			LogComErr (_LOG_AGENT_CALLS, lResult, _T("PropertySheet->SetVisible"));
		}
	}
}

void CAnimationTestDlg::OnCharProps()
{
	HRESULT	lResult;

	if	(mDaServer)
	{
		AllowSetForegroundWindow (ASFW_ANY);
		lResult = mDaServer->ShowDefaultCharacterProperties (0, 0, TRUE);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("ShowDefaultCharacterProperties"));
	}
	else
	if	(mMsServer)
	{
		AllowSetForegroundWindow (ASFW_ANY);
		lResult = mMsServer->ShowDefaultCharacterProperties (0, 0, TRUE);
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

	if	(mDaCharacter)
	{
		ActiveStateType	lActive = (ActiveStateType)-1;
		long			lOtherClients = -1;

		if	(bActive)
		{
			LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->get_ActiveState (&lActive), _T("[%d] get_ActiveState"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->get_OtherClientCount (&lOtherClients), _T("[%d] get_OtherClientCount"), mCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd] OtherClients [%d]"), mCharacterId, bActive, lActive, lOtherClients);

			LogComErr (_LOG_CHAR_CALLS, mDaCharacter->put_ActiveState (ActiveState_InputActive), _T("[%d] put_ActiveState ActiveState_InputActive"), mCharacterId);
			LogComErr (_LOG_CHAR_CALLS_EX, mDaCharacter->get_ActiveState (&lActive), _T("[%d] get_ActiveState"), mCharacterId);
			LogMessage (MaxLogLevel(LogNormal,_LOG_CHAR_CALLS), _T("[%d] ActivateApp [%u] Active [%hd]"), mCharacterId, bActive, lActive);
		}
	}
	else
	if	(mMsCharacter)
	{
	}
	if	(
			(mAgentPreviewWnd)
		&&	(mAgentPreviewWnd->IsWindow ())
		)
	{
		mAgentPreviewWnd->MakeActiveMedia (bActive!=FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfoCount(UINT*pctinfo)
{
	METHOD_PROLOGUE_EX_(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfoCount"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfoCount (pctinfo);
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	METHOD_PROLOGUE_EX_(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CAnimationTestDlg::XDaSvrNotifySink::GetTypeInfo"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfo (iTInfo, lcid, ppTInfo);
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	METHOD_PROLOGUE_EX_(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CAnimationTestDlg::XDaSvrNotifySink::GetIDsOfNames"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr)
{
	METHOD_PROLOGUE_EX(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CAnimationTestDlg::XDaSvrNotifySink::Invoke [%8.8X (%u)]"), pThis, pThis->m_dwRef, dispIdMember, dispIdMember);
#endif
	return pThis->GetIDispatch(FALSE)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Command"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::ActivateInputState (long CharacterID, long Activated)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Activated);
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

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	VisibilityCauseType lCause = (VisibilityCauseType)-1;
	if	(pThis->mDaCharacter)
	{
		pThis->mDaCharacter->get_VisibilityCause (&lCause);
	}
	else
	if	(pThis->mMsCharacter != NULL)
	{
		pThis->mMsCharacter->GetVisibilityCause ((long*)&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Visible, Cause, lCause);
#endif
	pThis->ShowCharacterState ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Click (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DblClick (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DragStart (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DragComplete (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::RequestStart (long RequestID)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mCharacterId, pThis->m_dwRef, RequestID);
#endif
//	if	(
//			(pThis->mDaCharacter)
//		&&	(RequestID > 101)
//		)
//	{
//		pThis->mDaCharacter->Stop (RequestID);
//	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::RequestComplete (long RequestID, long Result)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, RequestID, Result);
#endif
	if	(RequestID == pThis->mLastAnimationReqID)
	{
		pThis->mLastAnimationReqID = 0;
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
	if	(RequestID == pThis->mHideReqID)
	{
		pThis->mHideReqID = 0;
		if	(pThis->mDaCharacter)
		{
			LogComErr (_LOG_CHAR_CALLS, pThis->mDaCharacter->Show (TRUE, &RequestID), _T("[%d] Show"), pThis->mCharacterId);
		}
		else
		if	(pThis->mMsCharacter)
		{
			LogComErr (_LOG_CHAR_CALLS, pThis->mMsCharacter->Show (TRUE, &RequestID), _T("[%d] Show"), pThis->mCharacterId);
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::BookMark (long BookMarkID)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->mCharacterId, pThis->m_dwRef, BookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Start);
#endif
//	if	(Start)
//	{
//		pThis->FreeAgentCharacter ();
//	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Move (long CharacterID, long x, long y, long Cause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	MoveCauseType lCause = (MoveCauseType)-1;
	if	(pThis->mDaCharacter)
	{
		pThis->mDaCharacter->get_MoveCause (&lCause);
	}
	else
	if	(pThis->mMsCharacter != NULL)
	{
		pThis->mMsCharacter->GetMoveCause ((long*)&lCause);
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, x, y, Cause, lCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::Size (long CharacterID, long Width, long Height)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Width, Height);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Visible);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::ListeningState"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR CharGUID)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mCharacterId, pThis->m_dwRef, CharGUID);
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

HRESULT STDMETHODCALLTYPE CAnimationTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CAnimationTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CAnimationTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Status);
#endif
	return S_OK;
}
