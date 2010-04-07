#include "StdAfx.h"
#include <shlwapi.h>
#include <psapi.h>
#include "StressTest.h"
#include "StressTestDlg.h"
#include "DaServerOdl.h"
#include "AgentPreviewWnd.h"
#include "Elapsed.h"
#include "UiState.h"
#include "GuidStr.h"
#include "DebugProcess.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_NOTIFY	LogNormal|LogTimeMs
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CStressTestDlg, CDialog)
	//{{AFX_MSG_MAP(CStressTestDlg)
	ON_WM_ACTIVATEAPP()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, OnItemChangedCharacterList)
	ON_LBN_SELCHANGE(IDC_GESTURES, OnSelChangeGestures)
	ON_BN_CLICKED(IDC_RANDOM_STOP0, OnRandomStop)
	ON_BN_CLICKED(IDC_RANDOM_STOP1, OnRandomStop)
	ON_BN_CLICKED(IDC_RANDOM_STOP2, OnRandomStop)
	ON_BN_CLICKED(IDC_RANDOM_STOP3, OnRandomStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CStressTestDlg, CDialog)
	INTERFACE_PART(CStressTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////

CStressTestDlg::CStressTestDlg(CWnd* pParent)
:	CDialog(CStressTestDlg::IDD, pParent),
	mNotifySinkId (0),
	mCharacterId (0),
	mCharacterNdx (-1),
	mCharacterPos (0, 0),
	mCharacterAutoPos (-1),
	mGestureNdx (-1),
	mGestureReqId (0),
	mSpeechReqId (0),
	mCycleNum (0),
	mTimer (0),
	mRandomStopTimer (0)
{
	//{{AFX_DATA_INIT(CStressTestDlg)
	//}}AFX_DATA_INIT
	EnableAutomation ();
}

CStressTestDlg::~CStressTestDlg()
{
	FreeAgentCharacter ();
	FreeAgentServer ();
}

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressTestDlg)
	DDX_Control(pDX, IDC_STRESS_REPEAT, mStressRepeat);
	DDX_Control(pDX, IDC_STRESS_PREVIEW, mStressPreview);
	DDX_Control(pDX, IDC_STRESS_CHARACTER, mStressCharacter);
	DDX_Control(pDX, IDC_RANDOM_STOP0, mRandomStop0);
	DDX_Control(pDX, IDC_RANDOM_STOP1, mRandomStop1);
	DDX_Control(pDX, IDC_RANDOM_STOP2, mRandomStop2);
	DDX_Control(pDX, IDC_RANDOM_STOP3, mRandomStop3);
	DDX_Control(pDX, IDC_STRESS_SOUND, mStressSound);
	DDX_Control(pDX, IDC_STRESS_SPEAK, mStressSpeak);
	DDX_Control(pDX, IDC_CHARACTER_PREVIEW, mPreviewWnd);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDC_CHARACTER_NAME, mCharacterNameEdit);
	DDX_Control(pDX, IDOK, mOkButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	DDX_Control(pDX, IDC_GESTURES, mGestures);
	//}}AFX_DATA_MAP
}

BOOL CStressTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetAgentServer ();
	ShowCharacters ();
	LoadConfig ();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::ShowCharacters ()
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
		&&	(SUCCEEDED (LogComErr (LogDetails, mServer->GetCharacterFiles (&lCharacterFiles))))
		)
	{
#if	TRUE
		lCharacterFiles->put_Filter (FilesFilter_PathMask);
#endif
		if	(
				(SUCCEEDED (LogComErr (LogDetails, lCharacterFiles->get_FilePaths (lFilePaths.Free()))))
			&&	(SUCCEEDED (LogComErr (LogDetails, SafeArrayGetVartype (lFilePaths, &lFilePathType))))
			&&	(SUCCEEDED (LogComErr (LogDetails, SafeArrayGetLBound (lFilePaths, 1, &lLowerBound))))
			&&	(SUCCEEDED (LogComErr (LogDetails, SafeArrayGetUBound (lFilePaths, 1, &lUpperBound))))
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

	mCharacterPath.Empty ();
	mCharacterNdx = -1;
	mGestureNdx = -1;
}

/////////////////////////////////////////////////////////////////////////////

bool CStressTestDlg::ShowCharacter (int pCharacterNdx)
{
	bool	lRet = false;

	if	(
			(pCharacterNdx >= 0)
		&&	(pCharacterNdx < mCharacterList.GetItemCount ())
		)
	{
		mCharacterNdx = pCharacterNdx;
		mCharacterList.SetItemState (mCharacterNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		mCharacterList.RedrawWindow ();

		lRet = ShowCharacter (mCharacterList.GetItemText (mCharacterNdx, 0));
	}
	return lRet;
}

bool CStressTestDlg::ShowCharacter (LPCTSTR pCharacterPath)
{
	bool	lRet = false;

	CDebugProcess().LogWorkingSetInline (LogNormal|LogHighVolume|LogTime);
	CDebugProcess().LogAddressSpaceInline (LogNormal|LogHighVolume|LogTime);
	CDebugProcess().LogGuiResourcesInline (LogNormal|LogHighVolume|LogTime);
	LogMessage (LogNormal|LogTime, _T("Show Character [%d %d] %s"), mCycleNum, mCharacterNdx, pCharacterPath);

	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
	{
		FreeAgentCharacter ();
		FreeAgentServer ();
	}

	if	(
			(pCharacterPath)
		&&	(pCharacterPath[0])
		)
	{
		try
		{
			CAgentPreviewWnd *	lPreview;
			CRect				lClientRect;
			CRect				lWindowRect;

			SafeFreeSafePtr (mAgentWnd);

			if	(mStressPreview.GetCheck())
			{
				if	(
						(mAgentWnd = CAgentPreviewWnd::CreateInstance())
					&&	(mAgentWnd->Create (mPreviewWnd.m_hWnd))
					&&	(mAgentWnd->IsWindow ())
					)
				{
					mAgentWnd->ShowWindow (SW_HIDE);

					if	(mAgentWnd->Open (pCharacterPath))
					{
						if	(lPreview = dynamic_cast <CAgentPreviewWnd *> (mAgentWnd.Ptr()))
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
			else
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}

	if	(lRet)
	{
		mCharacterPath = pCharacterPath;

		if	(
				(mStressCharacter.GetCheck ())
			&&	(!ShowAgentCharacter ())
			)
		{
			lRet = false;
		}
	}
	else
	{
		mCharacterPath.Empty ();
	}

	ShowCharacterDetails ();
	ShowGestures ();
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::ShowCharacterDetails ()
{
	CString	lName;

	if	(!mCharacterPath.IsEmpty ())
	{
		CAgentFileName *	lFileName;

		if	(
				(mAgentWnd)
			&&	(mAgentWnd->IsWindow())
			&&	(mAgentWnd->GetAgentFile())
			&&	(lFileName = mAgentWnd->GetAgentFile()->FindName ())
			)
		{
			lName = lFileName->mName;
		}
	}

	mCharacterNameEdit.SetWindowText (lName);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::ShowGestures ()
{
	mGestures.SetRedraw (FALSE);
	mGestures.ResetContent ();

	try
	{
		tPtr <CAgentFile>	lAgentFile;

		if	(
				(!mCharacterPath.IsEmpty ())
			&&	(lAgentFile = CAgentFile::CreateInstance())
			&&	(SUCCEEDED (lAgentFile->Open (mCharacterPath)))
			)
		{
			int	lNdx;

			if	(lAgentFile->ReadGestures())
			{
				for	(lNdx = 0; lNdx <= lAgentFile->GetGestures().mNames.GetUpperBound (); lNdx++)
				{
					mGestures.AddString (lAgentFile->GetGestures().mNames.GetAt (lNdx));
				}
			}
		}
	}
	catch AnyExceptionDebug

	mGestures.SetCurSel (-1);
	mGestures.SetRedraw (TRUE);
	mGestureNdx = -1;
}

/////////////////////////////////////////////////////////////////////////////

bool CStressTestDlg::ShowGesture (int pGestureNdx)
{
	bool	lRet = false;
	CString	lGestureName;

	if	(
			(pGestureNdx >= 0)
		&&	(pGestureNdx < mGestures.GetCount())
		)
	{
		mGestureNdx = pGestureNdx;
		mGestures.SetCurSel (mGestureNdx);
		mGestures.RedrawWindow ();
		mGestures.GetText (mGestureNdx, lGestureName);
		lRet = ShowGesture (lGestureName);
	}
	return lRet;
}

static CString GestureNameSpeech (LPCTSTR pGestureName)
{
	CString	lRet (pGestureName);
	int		lNdx;

	for	(lNdx = 0; lNdx < lRet.GetLength(); lNdx++)
	{
		if	(lRet [lNdx] == _T('_'))
		{
			lRet.SetAt (lNdx, _T(' '));
		}
		else
		if	(
				(lNdx > 1)
			&&	(
					(
						(_istupper (lRet [lNdx]))
					&&	(!_istupper (lRet [lNdx-1]))
					)
				||	(
						(_istdigit (lRet [lNdx]))
					&&	(!_istdigit (lRet [lNdx-1]))
					&&	(!_istpunct (lRet [lNdx-1]))
					)
				)
			&&	(!_istspace (lRet [lNdx-1]))
			)
		{
			lRet.Insert (lNdx, _T(" "));
		}
	}
	return lRet;
}

bool CStressTestDlg::ShowGesture (LPCTSTR pGestureName)
{
	bool	lRet = false;

	mSpeechReqId = 0;
	mGestureReqId = 0;

	if	(
			(pGestureName)
		&&	(pGestureName[0])
		)
	{
		LogMessage (LogDetails|LogTime, _T("  Show %s Gesture %s"), PathFindFileName(mCharacterPath), pGestureName);

		if	(
				(mStressCharacter.GetCheck())
			&&	(mCharacter != NULL)
			)
		{
			mCharacter->SetSoundEffectsOn (mStressSound.GetCheck ());
			if	(
					(
						(!mStressSpeak.GetCheck())
					||	(SUCCEEDED (LogComErr (LogNormal, mCharacter->Speak (_bstr_t(GestureNameSpeech(pGestureName)), NULL, &mSpeechReqId))))
					)
				&&	(SUCCEEDED (LogComErr (LogNormal, mCharacter->Play (_bstr_t(pGestureName), &mGestureReqId))))
				)
			{
				lRet = true;
			}
		}
		if	(
				(mStressPreview.GetCheck())
			&&	(mAgentWnd)
			&&	(mAgentWnd->IsWindow ())
			)
		{
			mAgentWnd->EnableSound (mStressSound.GetCheck() ? true : false);
			if	(mAgentWnd->ShowAnimation (pGestureName))
			{
				lRet = true;
			}
		}
		if	(lRet)
		{
			mGestureStartTime = GetTickCount ();
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CStressTestDlg::IsAnimating ()
{
	if	(
			(mCharacter != NULL)
		&&	(
				(mGestureReqId > 0)
			||	(mSpeechReqId > 0)
			)
		)
	{
		return true;
	}
	if	(
			(mAgentWnd)
		&&	(mAgentWnd->IsWindow())
		&&	(!mAgentWnd->IsAnimationComplete ())
		)
	{
		return true;
	}
	return false;
}

bool CStressTestDlg::Stop ()
{
	bool	lRet = false;

	if	(
			(mCharacter != NULL)
		&&	(SUCCEEDED (LogComErr (LogNormal, mCharacter->StopAll (StopType_Play|StopType_Move|StopType_Speak), _T("[%d] StopAll"), mCharacterId)))
		)
	{
		lRet = true;
	}
	if	(
			(mAgentWnd)
		&&	(mAgentWnd->IsWindow ())
		&&	(mAgentWnd->Stop () == S_OK)
		)
	{
		lRet = true;
	}

	mGestureReqId = 0;
	mSpeechReqId = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::GetAgentServer ()
{
	if	(mServer == NULL)
	{
		IUnknownPtr	lUnknown;

		LogComErr (LogNormal, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("CoCreateInstance"));
		mServer = lUnknown;

		if	(mServer != NULL)
		{
			LogComErr (LogNormal, mServer->Register (&m_xDaSvrNotifySink, &mNotifySinkId), _T("Register"));
			LogComErr (LogNormal, mServer->put_CharacterStyle (CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled));
		}
	}
}

void CStressTestDlg::FreeAgentServer ()
{
	if	(
			(mNotifySinkId)
		&&	(mServer != NULL)
		)
	{
		try
		{
			LogComErr (LogNormal, mServer->Unregister (mNotifySinkId), _T("Unregister"));
		}
		catch AnyExceptionSilent
	}

	mNotifySinkId = 0;
	SafeFreeSafePtr (mServer);
}

bool CStressTestDlg::ShowAgentCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;
	CString	lWinTitle;

	GetAgentServer ();

	if	(mCharacter != NULL)
	{
		mCharacter->GetPosition (&mCharacterPos.x, &mCharacterPos.y);
	}

	if	(
			(mServer != NULL)
		&&	(mCharacterId == 0)
		&&	(!mCharacterPath.IsEmpty ())
		)
	{
		lResult = mServer->Load (_variant_t(mCharacterPath), &mCharacterId, &lReqID);
		LogComErr (LogNormal, lResult, _T("Load [%d] [%s] as [%d]"), lReqID, mCharacterPath, mCharacterId);
	}

	if	(
			(mServer != NULL)
		&&	(mCharacterId != 0)
		&&	(mCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (LogNormal, mServer->GetCharacterEx (mCharacterId, &mCharacter), _T("GetCharacterEx"))))
		)
	{
		LogComErr (LogDetails, mCharacter->SetIdleOn (FALSE), _T("[%d] SetIdleOff"), mCharacterId);
	}

	if	(mCharacter != NULL)
	{
		CSize	lCharSize;
		
		if	(
				(SUCCEEDED (mCharacter->GetSize (&lCharSize.cx, &lCharSize.cy)))
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
			mCharacter->SetSize (lCharSize.cx, lCharSize.cy);
		}
#if	FALSE
		if	(mCharacterAutoPos < 0)
		{
			CSize	lCharacterSize;

			mCharacterAutoPos = mCharacterId-256;
			if	(SUCCEEDED (mCharacter->GetSize (&lCharacterSize.cx, &lCharacterSize.cy)))
			{
				mCharacterPos.x = GetSystemMetrics (SM_CXSCREEN) - lCharacterSize.cx;
				mCharacterPos.y = mCharacterAutoPos * 100;
				mCharacterPos.x = min (max (mCharacterPos.x, 0), GetSystemMetrics (SM_CXSCREEN)-50);
				mCharacterPos.y = min (max (mCharacterPos.y, 0), GetSystemMetrics (SM_CYSCREEN)-50);
			}
		}
#endif
		mCharacter->MoveTo ((short)mCharacterPos.x, (short)mCharacterPos.y, 0, &lReqID);
		lResult = mCharacter->Show (TRUE, &lReqID);
		if	(SUCCEEDED (LogComErr (LogNormal, lResult, _T("[%d] Show [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	if	(!lRet)
	{
		SafeFreeSafePtr (mCharacter);
		SafeFreeSafePtr (mServer);
		OnCancel ();
	}
	return lRet;
}

bool CStressTestDlg::HideAgentCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;

	if	(mCharacter != NULL)
	{
		lResult = mCharacter->Hide (FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (LogNormal, lResult, _T("[%d] Hide [%d]"), mCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CStressTestDlg::FreeAgentCharacter ()
{
	bool	lRet = false;

	if	(mCharacter != NULL)
	{
		mCharacter->GetPosition (&mCharacterPos.x, &mCharacterPos.y);
		mCharacter = NULL;

		if	(
				(mServer != NULL)
			&&	(mCharacterId != 0)
			)
		{
			LogComErr (LogNormal, mServer->Unload (mCharacterId), _T("Unload [%d]"), mCharacterId);
		}

		lRet = true;
	}

	mCharacterId = 0;
	mGestureReqId = 0;
	mSpeechReqId = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileCharPosX = _T("CharLeft");
static LPCTSTR	sProfileCharPosY = _T("CharTop");
static LPCTSTR	sProfileStressPreview = _T("StressPreview");
static LPCTSTR	sProfileStressCharacter = _T("StressCharacter");
static LPCTSTR	sProfileStressRepeat = _T("StressRepeat");
static LPCTSTR	sProfileStressSpeak = _T("StressSpeak");
static LPCTSTR	sProfileStressSound = _T("StressSound");
static LPCTSTR	sProfileRandomStop = _T("RandomStop");

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	mStressPreview.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressPreview, mStressPreview.GetCheck()) ? TRUE : FALSE);
	mStressCharacter.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressCharacter, mStressCharacter.GetCheck()) ? TRUE : FALSE);
	mStressRepeat.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressRepeat, mStressRepeat.GetCheck()) ? TRUE : FALSE);
	mStressSpeak.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressSpeak, mStressSpeak.GetCheck()) ? TRUE : FALSE);
	mStressSound.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressSound, mStressSound.GetCheck()) ? TRUE : FALSE);
	mRandomStop0.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, !mRandomStop0.GetCheck()) ? FALSE : TRUE);
	mRandomStop1.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, mRandomStop1.GetCheck() ? 1 : 0) == 1);
	mRandomStop2.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, mRandomStop2.GetCheck() ? 2 : 0) == 2);
	mRandomStop3.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, mRandomStop3.GetCheck() ? 3 : 0) == 3);

	GetWindowRect (&lWinRect);
	lWinRect.OffsetRect (lApp->GetProfileInt (sProfileKey, sProfilePosX, lWinRect.left) - lWinRect.left, lApp->GetProfileInt (sProfileKey, sProfilePosY, lWinRect.top) - lWinRect.top);
	lWinRect.OffsetRect (min (GetSystemMetrics(SM_CXSCREEN)-lWinRect.right, 0), min (GetSystemMetrics(SM_CYSCREEN)-lWinRect.bottom, 0));
	lWinRect.OffsetRect (max (-lWinRect.left, 0), max (-lWinRect.top, 0));
	MoveWindow (&lWinRect);

	mCharacterPos.x = lApp->GetProfileInt (sProfileKey, sProfileCharPosX, mCharacterPos.x);
	mCharacterPos.y = lApp->GetProfileInt (sProfileKey, sProfileCharPosY, mCharacterPos.y);
	mCharacterPos.x = min (max (mCharacterPos.x, 0), GetSystemMetrics (SM_CXSCREEN)-50);
	mCharacterPos.y = min (max (mCharacterPos.y, 0), GetSystemMetrics (SM_CYSCREEN)-50);

	if	(mStressCharacter.GetCheck())
	{
		GetAgentServer ();
	}
}

void CStressTestDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	lApp->WriteProfileInt (sProfileKey, sProfileStressPreview, mStressPreview.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileStressCharacter, mStressCharacter.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileStressRepeat, mStressRepeat.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileStressSpeak, mStressSpeak.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileStressSound, mStressSound.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileRandomStop, mRandomStop1.GetCheck() ? 1 : mRandomStop2.GetCheck() ? 2 : mRandomStop3.GetCheck() ? 3 : 0);

	if	(!IsIconic ())
	{
		GetWindowRect (&lWinRect);
		lApp->WriteProfileInt (sProfileKey, sProfilePosX, lWinRect.left);
		lApp->WriteProfileInt (sProfileKey, sProfilePosY, lWinRect.top);
	}
	if	(
			(mCharacter != NULL)
		&&	(SUCCEEDED (mCharacter->GetPosition (&mCharacterPos.x, &mCharacterPos.y)))
		)
	{
		lApp->WriteProfileInt (sProfileKey, sProfileCharPosX, mCharacterPos.x);
		lApp->WriteProfileInt (sProfileKey, sProfileCharPosY, mCharacterPos.y);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::OnOK()
{
	mTimer = SetTimer ((UINT_PTR)&mTimer, 100, NULL);
	OnRandomStop ();
}

void CStressTestDlg::OnCancel()
{
	if	(mTimer)
	{
		KillTimer (mTimer);
	}
	mTimer = 0;
	if	(mRandomStopTimer)
	{
		KillTimer (mRandomStopTimer);
	}
	mRandomStopTimer = 0;
}

void CStressTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if	(
			(mTimer)
		&&	(nIDEvent == mTimer)
		)
	{
		if	(
				(IsAnimating ())
			&&	(TicksElapsed (mGestureStartTime, 10000))
			)
		{
			Stop ();
		}

		if	(!IsAnimating ())
		{
			do
			{
				bool	lGestureShown = false;

				while	(++mGestureNdx < mGestures.GetCount ())
				{
					if	(lGestureShown = ShowGesture (mGestureNdx))
					{
						break;
					}
					if	(!mTimer)
					{
						break;
					}
				}
				if	(lGestureShown)
				{
					break;
				}
				while	(++mCharacterNdx < mCharacterList.GetItemCount ())
				{
					if	(ShowCharacter (mCharacterNdx))
					{
						break;
					}
					if	(!mTimer)
					{
						break;
					}
				}
				if	(!mTimer)
				{
					break;
				}
			}
			while	(mCharacterNdx < mCharacterList.GetItemCount ());
		}

		if	(
				(mTimer)
			&&	(mCharacterNdx >= mCharacterList.GetItemCount ())
			&&	(mGestureNdx >= mGestures.GetCount ())
			)
		{
			PrimeMessagePump ();
			mCycleNum++;
			mCharacterNdx = -1;
			mGestureNdx = -1;

			if	(!mStressRepeat.GetCheck ())
			{
				KillTimer (mTimer);
				mTimer = 0;
				OnRandomStop ();
			}
		}
	}
	else
	if	(
			(mRandomStopTimer)
		&&	(nIDEvent == mRandomStopTimer)
		)
	{
		LogMessage (LogDetails|LogTime, _T("  Random stop [%d %d]"), mGestureReqId, mSpeechReqId);
		Stop ();
		OnRandomStop ();
		OnTimer (mTimer);
	}
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW	lNotify = (LPNMLISTVIEW)pNMHDR;

	if	(
			(lNotify->uChanged & LVIF_STATE)
		&&	(!(lNotify->uOldState & LVIS_SELECTED))
		&&	(lNotify->uNewState & LVIS_SELECTED)
		)
	{
		if	(mCharacterNdx != lNotify->iItem)
		{
			ShowCharacter (lNotify->iItem);
		}
	}
}

void CStressTestDlg::OnSelChangeGestures()
{
	if	(mGestureNdx != mGestures.GetCurSel ())
	{
		ShowGesture (mGestures.GetCurSel ());
	}
}

void CStressTestDlg::OnRandomStop()
{
	if	(
			(mTimer)
		&&	(
				(mRandomStop1.GetCheck ())
			||	(mRandomStop2.GetCheck ())
			||	(mRandomStop3.GetCheck ())
			)
		)
	{
		DWORD	lRandomTime;

		if	(mRandomStop3.GetCheck ())
		{
			lRandomTime = ((DWORD)rand() % 500) + 100;
		}
		else
		if	(mRandomStop2.GetCheck ())
		{
			lRandomTime = ((DWORD)rand() % 5000) + 500;
		}
		else
		{
			lRandomTime = ((DWORD)rand() % 50000) + 500;
		}

		//LogMessage (LogNormal|LogTime, _T("Starting Random stop [%u]"), lRandomTime);
		mRandomStopTimer = SetTimer ((UINT_PTR)&mRandomStopTimer, lRandomTime, NULL);
	}
	else
	{
		if	(mRandomStopTimer)
		{
			KillTimer (mRandomStopTimer);
		}
		mRandomStopTimer = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if	(nID != SC_SCREENSAVE)
	{
		CDialog::OnSysCommand (nID, lParam);
	}
}

void CStressTestDlg::OnClose()
{
	if	(
			(mAgentWnd)
		&&	(mAgentWnd->IsWindow ())
		)
	{
		mAgentWnd->Close ();
	}
	CDialog::EndDialog (IDOK);
}

void CStressTestDlg::OnDestroy()
{
	SaveConfig ();
	FreeAgentCharacter ();
	FreeAgentServer ();

	if	(
			(mAgentWnd)
		&&	(mAgentWnd->IsWindow ())
		)
	{
		mAgentWnd->Close ();
	}
	CDialog::OnDestroy();
}

void CStressTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);
	if	(
			(mAgentWnd)
		&&	(mAgentWnd->IsWindow ())
		)
	{
		mAgentWnd->MakeActiveMedia (bActive!=FALSE);
	}
	if	(
			(bActive)
		&&	(mCharacter != NULL)
		)
	{
		LogComErr (LogNormal, mCharacter->Activate (ActiveType_InputActive), _T("[%d] Activate ActiveType_Active"), mCharacterId);
	}
}

/////////////////////////////////////////////////////////////////////////////

HBRUSH CStressTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if	(pWnd == &mCharacterNameEdit)
	{
		pDC->SetBkColor (GetSysColor (COLOR_WINDOW));
		return GetSysColorBrush (COLOR_WINDOW);
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::AddRef ()
{
	METHOD_PROLOGUE_EX_(CStressTestDlg, DaSvrNotifySink)
	ULONG lRet = pThis->ExternalAddRef ();
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::AddRef"), pThis, lRet);
#endif
	return lRet;
}

ULONG STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Release ()
{
	METHOD_PROLOGUE_EX_(CStressTestDlg, DaSvrNotifySink)
	ULONG lRet = pThis->ExternalRelease ();
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::Release"), pThis, lRet);
#endif
	return lRet;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX(CStressTestDlg, DaSvrNotifySink)
	HRESULT	lResult = pThis->ExternalQueryInterface(&iid, ppvObj);
#ifdef	_DEBUG_COM
	LogComErr (_DEBUG_COM, lResult, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::QueryInterface [%s]"), pThis, pThis->m_dwRef, CGuidStr::GuidName(iid));
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::GetTypeInfoCount(UINT *pctinfo)
{
	METHOD_PROLOGUE_EX_(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::GetTypeInfoCount"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfoCount (pctinfo);
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	METHOD_PROLOGUE_EX_(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::GetTypeInfo"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfo (iTInfo, lcid, ppTInfo);
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	METHOD_PROLOGUE_EX_(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::GetIDsOfNames"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	METHOD_PROLOGUE_EX(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p] [%u] CStressTestDlg::XDaSvrNotifySink::Invoke [%8.8X (%u)]"), pThis, pThis->m_dwRef, dispIdMember, dispIdMember);
#endif
	return pThis->GetIDispatch(FALSE)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Command (long dwCommandID, IUnknown *punkUserInput)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Command"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ActivateInputState (long dwCharID, long bActivated)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bActivated);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Restart"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Shutdown"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::VisibleState (long dwCharID, long bVisible, long dwCause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	long lCause = -1;
	if	(pThis->mCharacter != NULL)
	{
		pThis->mCharacter->GetVisibilityCause (&lCause);
	}
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bVisible, dwCause, lCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Click (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DblClick (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DragStart (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DragComplete (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::RequestStart (long dwRequestID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwRequestID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::RequestComplete (long dwRequestID, long hrStatus)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, dwRequestID, hrStatus);
#endif
	if	(dwRequestID == pThis->mGestureReqId)
	{
		pThis->mGestureReqId = 0;
	}
	if	(dwRequestID == pThis->mSpeechReqId)
	{
		pThis->mSpeechReqId = 0;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::BookMark (long dwBookMarkID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::BookMark"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Idle (long dwCharID, long bStart)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bStart);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Move (long dwCharID, long x, long y, long dwCause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	long lCause = -1;
	if	(pThis->mCharacter != NULL)
	{
		pThis->mCharacter->GetMoveCause (&lCause);
	}
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, x, y, dwCause, lCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Size (long dwCharID, long lWidth, long lHeight)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, lWidth, lHeight);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::BalloonVisibleState (long dwCharID, long bVisible)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bVisible);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::HelpComplete (long dwCharID, long dwCommandID, long dwCause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ListeningState (long dwCharID, long bListening, long dwCause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ListeningState"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR bszGUID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mCharacterId, pThis->m_dwRef, bszGUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ActiveClientChange (long dwCharID, long lStatus)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, lStatus);
#endif
	return S_OK;
}
