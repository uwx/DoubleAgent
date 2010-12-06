#include "StdAfx.h"
#include <shlwapi.h>
#include <psapi.h>
#include "StressTest.h"
#include "StressTestDlg.h"
#include "Elapsed.h"
#include "UiState.h"
#include "GuidStr.h"
#include "DebugProcess.h"
#include "DebugWin.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_NOTIFY	LogNormal|LogTimeMs|LogHighVolume
#endif

static LPCTSTR	sControlCharacterIdPopup = _T("Popup");
static LPCTSTR	sControlCharacterIdContained = _T("Contained");

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CStressTestDlg, CDialog)
	//{{AFX_MSG_MAP(CStressTestDlg)
	ON_WM_ACTIVATEAPP()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, OnItemChangedCharacterList)
	ON_LBN_SELCHANGE(IDC_GESTURES, OnSelChangeGestures)
	ON_BN_CLICKED(IDC_STRESS_CONTROL, OnControlMode)
	ON_BN_CLICKED(IDC_STRESS_CONTAINED, OnControlMode)
	ON_BN_CLICKED(IDC_STRESS_STANDALONE, OnControlMode)
	ON_BN_CLICKED(IDC_RANDOM_STOP0, OnRandomStop)
	ON_BN_CLICKED(IDC_RANDOM_STOP1, OnRandomStop)
	ON_BN_CLICKED(IDC_RANDOM_STOP2, OnRandomStop)
	ON_BN_CLICKED(IDC_RANDOM_STOP3, OnRandomStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CStressTestDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CStressTestDlg)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_ActivateInput, CStressTestDlg::OnCtlActivateInput, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_DeactivateInput, CStressTestDlg::OnCtlDeactivateInput, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_Click, CStressTestDlg::OnCtlClick, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_DblClick, CStressTestDlg::OnCtlDblClick, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_DragStart, CStressTestDlg::OnCtlDragStart, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_DragComplete, CStressTestDlg::OnCtlDragComplete, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_Show, CStressTestDlg::OnCtlShow, VTS_BSTR VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_Hide, CStressTestDlg::OnCtlHide, VTS_BSTR VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_RequestStart, CStressTestDlg::OnCtlRequestStart, VTS_DISPATCH)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_RequestComplete, CStressTestDlg::OnCtlRequestComplete, VTS_DISPATCH)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_Command, CStressTestDlg::OnCtlCommand, VTS_DISPATCH)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_IdleStart, CStressTestDlg::OnCtlIdleStart, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_IdleComplete, CStressTestDlg::OnCtlIdleComplete, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_IdleStart, CStressTestDlg::OnCtlIdleStart, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_Move, CStressTestDlg::OnCtlMove, VTS_BSTR VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_Size, CStressTestDlg::OnCtlSize, VTS_BSTR VTS_I2 VTS_I2)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_BalloonShow, CStressTestDlg::OnCtlBalloonShow, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_BalloonHide, CStressTestDlg::OnCtlBalloonHide, VTS_BSTR)
	ON_EVENT(CStressTestDlg, IDC_CONTROL_PLACER, DISPID_AgentEvents_ActiveClientChange, CStressTestDlg::OnCtlActiveClientChange, VTS_BSTR VTS_BOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CStressTestDlg, CDialog)
	INTERFACE_PART(CStressTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CStressTestDlg, DaSvrNotifySink)

/////////////////////////////////////////////////////////////////////////////

CStressTestDlg::CStressTestDlg(CWnd* pParent)
:	CDialog(CStressTestDlg::IDD, pParent),
	mServerSinkId (0),
	mServerCharacterId (0),
	mCharacterNdx (-1),
	mGestureNdx (-1),
	mShowReqId (0),
	mGestureReqId (0),
	mSpeechReqId (0),
	mCycleNum (0),
	mCycleTimer (0),
	mRandomStopTimer (0)
{
	//{{AFX_DATA_INIT(CStressTestDlg)
	//}}AFX_DATA_INIT
	EnableAutomation ();
}

CStressTestDlg::~CStressTestDlg()
{
	FreeServerCharacter ();
	FreeAgentServer ();
	FreeControlCharacter ();
	FreeAgentControl ();
}

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressTestDlg)
	DDX_Control(pDX, IDC_STRESS_REPEAT, mStressRepeat);
	DDX_Control(pDX, IDC_STRESS_CONTROL, mStressControl);
	DDX_Control(pDX, IDC_STRESS_CONTAINED, mControlContained);
	DDX_Control(pDX, IDC_STRESS_STANDALONE, mControlStandalone);
	DDX_Control(pDX, IDC_STRESS_SERVER, mStressServer);
	DDX_Control(pDX, IDC_RANDOM_STOP0, mRandomStop0);
	DDX_Control(pDX, IDC_RANDOM_STOP1, mRandomStop1);
	DDX_Control(pDX, IDC_RANDOM_STOP2, mRandomStop2);
	DDX_Control(pDX, IDC_RANDOM_STOP3, mRandomStop3);
	DDX_Control(pDX, IDC_STRESS_SOUND, mStressSound);
	DDX_Control(pDX, IDC_STRESS_SPEAK, mStressSpeak);
	DDX_Control(pDX, IDC_CONTROL_PLACER, mControlPlacer);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDOK, mOkButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	DDX_Control(pDX, IDC_GESTURES, mGestures);
	//}}AFX_DATA_MAP
}

BOOL CStressTestDlg::OnInitDialog()
{
	bool	lAutoStart;

	CDialog::OnInitDialog();

	ModifyStyleEx (0, WS_EX_APPWINDOW);

	GetAgentServer ();
	ShowCharacters ();
	FreeAgentServer ();

	LoadConfig ();
	lAutoStart = CommandLineConfig ();
	ShowControlMode ();

	if	(lAutoStart)
	{
		PostMessage (WM_COMMAND, IDOK);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::ShowModeSelection ()
{
	if	(
			(mServerCharacter)
		||	(mControlCharacter)
		)
	{
		mStressServer.EnableWindow (FALSE);
		mStressControl.EnableWindow (FALSE);
		mControlContained.EnableWindow (FALSE);
		mControlStandalone.EnableWindow (FALSE);
	}
	else
	{
		mStressServer.EnableWindow (TRUE);
		mStressControl.EnableWindow (TRUE);
		ShowControlMode ();
	}
}

void CStressTestDlg::ShowControlMode ()
{
	if	(mStressControl.GetCheck())
	{
		mControlContained.EnableWindow (mStressControl.IsWindowEnabled());
		if	(mControlContained.GetCheck())
		{
			mControlStandalone.SetCheck (TRUE);
			mControlStandalone.EnableWindow (FALSE);
		}
		else
		{
			mControlStandalone.EnableWindow (mStressControl.IsWindowEnabled());
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
		&&	(SUCCEEDED (LogComErr (LogDetails, mServer->get_CharacterFiles (&lCharacterFiles))))
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
		mCharacterList.EnsureVisible (mCharacterNdx, FALSE);
		mCharacterList.RedrawWindow ();

		lRet = ShowCharacter (mCharacterList.GetItemText (mCharacterNdx, 0));
	}
	return lRet;
}

bool CStressTestDlg::ShowCharacter (LPCTSTR pCharacterPath)
{
	bool	lRet = false;

#if	FALSE
	CDebugProcess().LogWorkingSetInline (LogNormal|LogTime|LogHighVolume);
	CDebugProcess().LogAddressSpaceInline (LogNormal|LogTime|LogHighVolume);
	CDebugProcess().LogGuiResourcesInline (LogNormal|LogTime|LogHighVolume);
#endif
	LogMessage (LogNormal|LogTime, _T("Show Character [%d %d] %s"), mCycleNum, mCharacterNdx, pCharacterPath);

	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
	{
		FreeServerCharacter ();
		FreeAgentServer ();
		FreeControlCharacter ();
	}

	ShowControlMode ();

	if	(
			(pCharacterPath)
		&&	(pCharacterPath[0])
		)
	{
		mCharacterPath = pCharacterPath;

		if	(
				(mStressServer.GetCheck ())
			&&	(ShowServerCharacter ())
			)
		{
			lRet = true;
		}

		if	(
				(mStressControl.GetCheck ())
			&&	(ShowControlCharacter ())
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
	ShowModeSelection ();
	return lRet;
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
		tSafeArrayPtr	lAnimations;
		VARTYPE			lAnimationType;
		long			lLowerBound, lUpperBound, lNdx;
		CRect			lClientRect;

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
	//lRet.Insert (0, _T("Show \\mrk=1\\gesture -\\mrk=2\\ "));
	return lRet;
}

bool CStressTestDlg::ShowGesture (LPCTSTR pGestureName)
{
	bool	lRet = false;

	mGestureReqId = 0;
	mSpeechReqId = 0;
	mGestureRequest = NULL;
	mSpeechRequest = NULL;

	if	(
			(pGestureName)
		&&	(pGestureName[0])
		)
	{
		LogMessage (LogDetails|LogTime, _T("  Show %s Gesture %s"), PathFindFileName(mCharacterPath), pGestureName);

		if	(
				(mStressServer.GetCheck())
			&&	(mServerCharacter != NULL)
			)
		{
			if	(mStressSound.GetCheck ())
			{
				mServerCharacter.Style |= CharacterStyle_SoundEffects;
			}
			else
			{
				mServerCharacter.Style &= ~CharacterStyle_SoundEffects;
			}
			if	(
					(
						(mStressSpeak.GetCheck())
					?	(SUCCEEDED (LogComErr (LogNormal|LogTime, mServerCharacter->Speak (_bstr_t(GestureNameSpeech(pGestureName)), NULL, &mSpeechReqId))))
					:	(SUCCEEDED (LogComErr (LogNormal|LogTime, mServerCharacter->Think (_bstr_t(GestureNameSpeech(pGestureName)), &mSpeechReqId))))
					)
				&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, mServerCharacter->Play (_bstr_t(pGestureName), &mGestureReqId))))
				)
			{
				//LogMessage (LogNormal, _T("[%s] Gesture [%s] SpeechReqId [%d] GestureReqId [%d]"), mCharacterPath, pGestureName, mSpeechReqId, mGestureReqId);
				lRet = true;
			}
		}

		if	(
				(mStressControl.GetCheck())
			&&	(mControlCharacter != NULL)
			)
		{
			if	(mStressSound.GetCheck ())
			{
				mControlCharacter.SoundEffectsEnabled = true;
			}
			else
			{
				mControlCharacter.SoundEffectsEnabled = false;
			}
			if	(
					(
						(mStressSpeak.GetCheck())
					?	(SUCCEEDED (LogComErr (LogNormal|LogTime, mControlCharacter->Speak (_variant_t(GestureNameSpeech(pGestureName)), _variant_t(), &mSpeechRequest))))
					:	(SUCCEEDED (LogComErr (LogNormal|LogTime, mControlCharacter->Think (_bstr_t(GestureNameSpeech(pGestureName)), &mSpeechRequest))))
					)
				&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, mControlCharacter->Play (_bstr_t(pGestureName), &mGestureRequest))))
				)
			{
				//LogMessage (LogNormal, _T("[%s] Gesture [%s] SpeechReqId [%d] GestureReqId [%d]"), mCharacterPath, pGestureName, mSpeechRequest.ID, mGestureRequest.ID);
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
			(mServerCharacter != NULL)
		&&	(
				(!mServerCharacter.Visible)
			||	(mShowReqId > 0)
			||	(mGestureReqId > 0)
			||	(mSpeechReqId > 0)
			)
		)
	{
		return true;
	}

	if	(
			(mControlCharacter != NULL)
		&&	(
				(!mControlCharacter.Visible)
			||	(
					(mShowRequest != NULL)
				&&	(
						(mShowRequest.Status == RequestStatus_Pending)
					||	(mShowRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mGestureRequest != NULL)
				&&	(
						(mGestureRequest.Status == RequestStatus_Pending)
					||	(mGestureRequest.Status == RequestStatus_InProgress)
					)
				)
			||	(
					(mSpeechRequest != NULL)
				&&	(
						(mSpeechRequest.Status == RequestStatus_Pending)
					||	(mSpeechRequest.Status == RequestStatus_InProgress)
					)
				)
			)
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
			(mServerCharacter != NULL)
		&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, mServerCharacter->StopAll (StopAll_Everything), _T("[%d] StopAll"), mServerCharacterId)))
		)
	{
		lRet = true;
	}
	mShowReqId = 0;
	mGestureReqId = 0;
	mSpeechReqId = 0;

	if	(
			(mControlCharacter != NULL)
		&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, mControlCharacter->StopAll (_variant_t((long)(StopAll_Everything),VT_I4)), _T("[%s] StopAll"), mControlCharacterId)))
		)
	{
		lRet = true;
	}
	mShowRequest = NULL;
	mGestureRequest = NULL;
	mSpeechRequest = NULL;

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

		LogComErr (LogNormal|LogTime, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("CoCreateInstance"));
		mServer = lUnknown;

		if	(mServer != NULL)
		{
			LogComErr (LogNormal|LogTime, mServer->Register (&m_xDaSvrNotifySink, &mServerSinkId), _T("Register"));
			mServer.CharacterStyle = CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_Smoothed;
		}
	}
}

void CStressTestDlg::FreeAgentServer ()
{
	if	(
			(mServerSinkId)
		&&	(mServer != NULL)
		)
	{
		try
		{
			LogComErr (LogNormal|LogTime, mServer->Unregister (mServerSinkId), _T("Unregister"));
		}
		catch AnyExceptionSilent
	}

	mServerSinkId = 0;
	SafeFreeSafePtr (mServer);
}

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::GetAgentControl ()
{
	if	(mControlPlacer.m_hWnd)
	{
		mControlRect = ChildWndRect (mControlPlacer);
		mControlPlacer.DestroyWindow();
	}
	if	(
			(mControlWnd.m_hWnd == NULL)
		&&	(mControlWnd.CreateControl (__uuidof(DaControl), _T(""), WS_CHILD|WS_CLIPSIBLINGS, mControlRect, this, IDC_CONTROL_PLACER))
		)
	{
		mControl = mControlWnd.GetControlSite()->m_pObject;
	}
	if	(mControl)
	{
		mControl.CharacterStyle = CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_Smoothed;

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

void CStressTestDlg::FreeAgentControl ()
{
	SafeFreeSafePtr (mControl);
	if	(mControlWnd.m_hWnd)
	{
		mControlWnd.DestroyWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CStressTestDlg::ShowServerCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;
	CString	lWinTitle;

	GetAgentServer ();

	if	(
			(mServer != NULL)
		&&	(mServerCharacterId == 0)
		&&	(!mCharacterPath.IsEmpty ())
		)
	{
		lResult = mServer->Load (_variant_t(mCharacterPath), &mServerCharacterId, &lReqID);
		LogComErr (LogNormal|LogTime, lResult, _T("Load [%d] [%s] as [%d]"), lReqID, mCharacterPath, mServerCharacterId);
	}

	if	(
			(mServer != NULL)
		&&	(mServerCharacterId != 0)
		&&	(mServerCharacter == NULL)
		&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, mServer->get_Character (mServerCharacterId, &mServerCharacter), _T("GetCharacter"))))
		)
	{
		mServerCharacter.Style &= ~CharacterStyle_IdleEnabled;
		if	(mStressSound.GetCheck ())
		{
			mServerCharacter.Style |= CharacterStyle_SoundEffects;
		}
		else
		{
			mServerCharacter.Style &= ~CharacterStyle_SoundEffects;
		}
	}

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
			lCharPos = GetInitialPos (lCharSize);
			mServerCharacter->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
		}

		mShowReqId = 0;
		lResult = mServerCharacter->Show (FALSE, &mShowReqId);
		if	(SUCCEEDED (LogComErr (LogNormal|LogTime, lResult, _T("[%d] Show [%d]"), mServerCharacterId, mShowReqId)))
		{
			lRet = true;
		}
	}
	if	(!lRet)
	{
		FreeServerCharacter ();
		FreeAgentServer ();
		OnCancel ();
	}
	return lRet;
}

bool CStressTestDlg::HideServerCharacter ()
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;

	if	(mServerCharacter != NULL)
	{
		lResult = mServerCharacter->Hide (FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (LogNormal|LogTime, lResult, _T("[%d] Hide [%d]"), mServerCharacterId, lReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CStressTestDlg::FreeServerCharacter ()
{
	bool	lRet = false;

	mShowReqId = 0;
	mGestureReqId = 0;
	mSpeechReqId = 0;

	if	(mServerCharacter != NULL)
	{
		LogMessage (LogNormal|LogTime, _T("Free Character %s"), (BSTR)mServerCharacter.FilePath);
		lRet = true;
	}
	mServerCharacter = NULL;

	if	(
			(mServerCharacterId != 0)
		&&	(mServer != NULL)
		&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, mServer->Unload (mServerCharacterId), _T("Unload [%d]"), mServerCharacterId)))
		)
	{
		lRet = true;
	}
	mServerCharacterId = 0;

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CStressTestDlg::ShowControlCharacter ()
{
	bool					lRet = false;
	HRESULT					lResult;
	IDaCtlCharacters2Ptr	lCharacters;

	GetAgentControl ();

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
		lResult = lCharacters->Load (_bstr_t(mControlCharacterId), _variant_t(mCharacterPath), NULL);
		LogComErr (LogNormal|LogTime, lResult, _T("Load [%s] as [%s]"), mCharacterPath, mControlCharacterId);
	}

	if	(
			(mControl != NULL)
		&&	(!mControlCharacterId.IsEmpty())
		&&	(mControlCharacter == NULL)
		&&	(lCharacters = mControl.Characters)
		&&	(mControlCharacter = lCharacters.Item [_bstr_t(mControlCharacterId)])
		)
	{
		mControlCharacter.IdleEnabled = false;
		mControlCharacter.SoundEffectsEnabled = mStressSound.GetCheck() ? true : false;
	}

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
			lCharPos = GetInitialPos (lCharSize);
			mControlCharacter.Left = (short)lCharPos.x;
			mControlCharacter.Top = (short)lCharPos.y;
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

		mShowRequest = NULL;
		lResult = mControlCharacter->Show (_variant_t (VARIANT_FALSE,VT_BOOL), &mShowRequest);
		if	(SUCCEEDED (LogComErr (LogNormal|LogTime, lResult, _T("[%s] Show [%d]"), mControlCharacterId, mShowRequest.ID)))
		{
			lRet = true;
		}
	}
	if	(!lRet)
	{
		SafeFreeSafePtr (mControlCharacter);
		OnCancel ();
	}

	return lRet;
}

bool CStressTestDlg::HideControlCharacter ()
{
	bool	lRet = false;
	HRESULT	lResult;

	if	(mControlCharacter)
	{
		lResult = mControlCharacter->Hide (_variant_t (VARIANT_FALSE,VT_BOOL), NULL);
		if	(SUCCEEDED (LogComErr (LogNormal|LogTime, lResult, _T("[%s] Hide [%d]"), mControlCharacterId)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CStressTestDlg::FreeControlCharacter ()
{
	bool					lRet = false;
	IDaCtlCharacters2Ptr	lCharacters;

	mShowRequest = NULL;
	mGestureRequest = NULL;
	mSpeechRequest = NULL;

	if	(mControlCharacter != NULL)
	{
		LogMessage (LogNormal|LogTime, _T("Free Character %s"), (BSTR)mControlCharacter.FilePath);
		lRet = true;
	}
	mControlCharacter = NULL;

	if	(
			(!mControlCharacterId.IsEmpty())
		&&	(mControl != NULL)
		&&	(lCharacters = mControl.Characters)
		&&	(SUCCEEDED (LogComErr (LogNormal|LogTime, lCharacters->Unload (_bstr_t(mControlCharacterId)), _T("Unload [%s]"), mControlCharacterId)))
		)
	{
		lRet = true;
	}
	mControlCharacterId.Empty ();

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileStressControl = _T("StressControl");
static LPCTSTR	sProfileControlContained = _T("ControlContained");
static LPCTSTR	sProfileControlStandalone = _T("ControlStandalone");
static LPCTSTR	sProfileStressServer = _T("StressServer");
static LPCTSTR	sProfileStressRepeat = _T("StressRepeat");
static LPCTSTR	sProfileStressSpeak = _T("StressSpeak");
static LPCTSTR	sProfileStressSound = _T("StressSound");
static LPCTSTR	sProfileRandomStop = _T("RandomStop");

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	mStressControl.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressControl, mStressControl.GetCheck()) ? TRUE : FALSE);
	mControlContained.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileControlContained, mControlContained.GetCheck()) ? TRUE : FALSE);
	mControlStandalone.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileControlStandalone, mControlStandalone.GetCheck()) ? TRUE : FALSE);
	mStressServer.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStressServer, mStressServer.GetCheck()) ? TRUE : FALSE);
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
}

void CStressTestDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	lApp->WriteProfileInt (sProfileKey, sProfileStressControl, mStressControl.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileControlContained, mControlContained.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileControlStandalone, mControlStandalone.GetCheck());
	lApp->WriteProfileInt (sProfileKey, sProfileStressServer, mStressServer.GetCheck());
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
}

bool CStressTestDlg::CommandLineConfig ()
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
				mStressServer.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Server-")) == 0)
			{
				mStressServer.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Control")) == 0)
				||	(lArg.CompareNoCase (_T("Control+")) == 0)
				)
			{
				mStressControl.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Control-")) == 0)
			{
				mStressControl.SetCheck (FALSE);
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
					(lArg.CompareNoCase (_T("Repeat")) == 0)
				||	(lArg.CompareNoCase (_T("Repeat+")) == 0)
				)
			{
				mStressRepeat.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Repeat-")) == 0)
			{
				mStressRepeat.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Speak")) == 0)
				||	(lArg.CompareNoCase (_T("Speak+")) == 0)
				)
			{
				mStressSpeak.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Speak-")) == 0)
			{
				mStressSpeak.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Sound")) == 0)
				||	(lArg.CompareNoCase (_T("Sound+")) == 0)
				)
			{
				mStressSound.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("Sound-")) == 0)
			{
				mStressSound.SetCheck (FALSE);
			}
			else
			if	(
					(lArg.CompareNoCase (_T("Random")) == 0)
				&&	(lArgNdx < __argc-1)
				)
			{
				int lRandomStop = _ttoi(__targv [++lArgNdx]);

				mRandomStop0.SetCheck (lRandomStop == 0);
				mRandomStop1.SetCheck (lRandomStop == 1);
				mRandomStop2.SetCheck (lRandomStop == 2);
				mRandomStop3.SetCheck (lRandomStop == 3);
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
		CString	lTitle (_T("Stress"));

		mStressServer.EnableWindow (FALSE);
		mStressControl.EnableWindow (FALSE);
		mControlContained.EnableWindow (FALSE);
		mControlStandalone.EnableWindow (FALSE);

		if	(mStressServer.GetCheck())
		{
			lTitle += _T(" Server");
		}
		if	(mStressControl.GetCheck())
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
#if	FALSE
		LogStop ();
		LogStart (false, lTitle+_T(".log"));
#endif		
		LogMessage (LogNormal|LogTime, _T("Start %s"), lTitle);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
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

CPoint CStressTestDlg::GetInitialPos (const CSize & pInitialSize)
{
	CPoint					lInitialPos;
	CRect					lBounds (100, 100, GetSystemMetrics(SM_CXSCREEN)-pInitialSize.cx, GetSystemMetrics(SM_CYSCREEN)-pInitialSize.cy);
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

void CStressTestDlg::OnOK()
{
	if	(mCharacterList.GetSelectedCount() == 0)
	{
		mCharacterList.SetItemState (0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	mCycleTimer = SetTimer ((UINT_PTR)&mCycleTimer, 200, NULL);
	OnRandomStop ();
}

void CStressTestDlg::OnCancel()
{
	Stop ();

	if	(mCycleTimer)
	{
		KillTimer (mCycleTimer);
	}
	mCycleTimer = 0;
	if	(mRandomStopTimer)
	{
		KillTimer (mRandomStopTimer);
	}
	mRandomStopTimer = 0;
}

void CStressTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if	(
			(mCycleTimer)
		&&	(nIDEvent == mCycleTimer)
		)
	{
		if	(
				(IsAnimating ())
			&&	(TicksElapsed (mGestureStartTime, 10000))
			)
		{
			Stop ();
		}

		if	(
				(!GetQueueStatus (QS_INPUT))
			&&	(!IsAnimating ())
			&&	(mCharacterList.GetItemCount() > 0)
			&&	(mGestures.GetCount() > 0)
			)
		{
			do
			{
				bool	lGestureShown = false;

				while	(++mGestureNdx < mGestures.GetCount())
				{
					PrimeMessagePump ();
					if	(lGestureShown = ShowGesture (mGestureNdx))
					{
						break;
					}
					if	(!mCycleTimer)
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
					PrimeMessagePump ();
					if	(ShowCharacter (mCharacterNdx))
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
			OnRandomStop ();
		}

		if	(
				(mCycleTimer)
			&&	(mCharacterList.GetItemCount() > 0)
			&&	(mCharacterNdx >= mCharacterList.GetItemCount()-1)
			&&	(mGestureNdx >= mGestures.GetCount())
			)
		{
			PrimeMessagePump ();
			mCycleNum++;
			mCharacterNdx = -1;
			mGestureNdx = -1;

			if	(!mStressRepeat.GetCheck ())
			{
				KillTimer (mCycleTimer);
				mCycleTimer = 0;
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
#if	FALSE
		if	(
				(!mStressSpeak.GetCheck())
			||	(
					(mServerCharacter)
				&&	(mSpeechReqId > 0)
				)
			||	(
					(mControlCharacter)
				&&	(mSpeechRequest)
				&&	(mSpeechRequest.Status != RequestStatus_Pending)
				)
			)
#endif
		{
			if	(mServerCharacter)
			{
				LogMessage (LogDetails|LogTime, _T("  Random stop [%d %d %d]"), mShowReqId, mGestureReqId, mSpeechReqId);
			}
			if	(mControlCharacter)
			{
				LogMessage (LogDetails|LogTime, _T("  Random stop [%d %d] [%d %d] [%d %d]"), mShowRequest.ID, mShowRequest.Status, mGestureRequest.ID, mGestureRequest.Status, mSpeechRequest.ID, mSpeechRequest.Status);
			}
			Stop ();
			OnRandomStop ();
			OnTimer (mCycleTimer);
		}
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

void CStressTestDlg::OnControlMode()
{
	ShowControlMode ();
}

void CStressTestDlg::OnRandomStop()
{
	if	(
			(mCycleTimer)
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
			lRandomTime = ((DWORD)rand() % 500) + 500;
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
	CDialog::EndDialog (IDOK);
}

void CStressTestDlg::OnDestroy()
{
	SaveConfig ();
	FreeServerCharacter ();
	FreeAgentServer ();
	FreeControlCharacter ();
	FreeAgentControl ();

	CDialog::OnDestroy();
}

void CStressTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);
#if	FALSE
	if	(
			(bActive)
		&&	(mServerCharacter != NULL)
		)
	{
		LogComErr (LogNormal|LogTime, mServerCharacter->Activate (ActiveState_InputActive), _T("[%d] Activate ActiveState_Active"), mServerCharacterId);
	}
#endif
#if	FALSE
	if	(
			(bActive)
		&&	(mControlCharacter != NULL)
		)
	{
		LogComErr (LogNormal|LogTime, mControlCharacter->Activate (_variant_t((short)ActiveState_InputActive,VT_I4), NULL), _T("[%s] Activate ActiveState_Active"), mControlCharacterId);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Command"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ActivateInputState (long CharacterID, long Activated)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Activated);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Restart"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Shutdown"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	long lCause = -1;
	if	(pThis->mServerCharacter != NULL)
	{
		pThis->mServerCharacter->GetVisibilityCause (&lCause);
	}
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Visible, Cause, lCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Click (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#else
	LogMessage (LogNormal|LogTime, _T("Click [%d]"), CharacterID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DblClick (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	pThis->BringWindowToTop ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DragStart (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DragComplete (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::RequestStart (long RequestID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, RequestID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::RequestComplete (long RequestID, long Result)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mServerCharacterId, pThis->m_dwRef, RequestID, Result);
#endif
	if	(RequestID == pThis->mShowReqId)
	{
		pThis->mShowReqId = 0;
	}
	if	(RequestID == pThis->mGestureReqId)
	{
		pThis->mGestureReqId = 0;
	}
	if	(RequestID == pThis->mSpeechReqId)
	{
		pThis->mSpeechReqId = 0;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::BookMark (long BookMarkID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::BookMark"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Start);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Move (long CharacterID, long x, long y, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	long lCause = -1;
	if	(pThis->mServerCharacter != NULL)
	{
		pThis->mServerCharacter->GetMoveCause (&lCause);
	}
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, x, y, Cause, lCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Size (long CharacterID, long Width, long Height)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Width, Height);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Visible);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ListeningState"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR GUID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mServerCharacterId, pThis->m_dwRef, GUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->mServerCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mServerCharacterId, pThis->m_dwRef, CharacterID, Status);
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::OnCtlActivateInput(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlActivateInput [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CStressTestDlg::OnCtlDeactivateInput(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDeactivateInput [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CStressTestDlg::OnCtlClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlClick [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#else
	LogMessage (LogNormal|LogTime, _T("Click [%s]"), CharacterID);
#endif
}

void CStressTestDlg::OnCtlDblClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDblClick [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
	BringWindowToTop ();
}

void CStressTestDlg::OnCtlDragStart (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDragStart [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
}

void CStressTestDlg::OnCtlDragComplete (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDragComplete [%s] [%d] [%d] [%d %d]"), mControlCharacterId, CharacterID, Button, Shift, X, Y);
#endif
}

void CStressTestDlg::OnCtlShow (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlShow [%s] [%d]"), mControlCharacterId, CharacterID, Cause);
#endif
}

void CStressTestDlg::OnCtlHide (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlHide [%s] [%d]"), mControlCharacterId, CharacterID, Cause);
#endif
}

void CStressTestDlg::OnCtlRequestStart (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr lRequest (Request);
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlRequestStart [%d] [%d] [%8.8X] [%ls]"), mControlCharacterId, lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
#endif
}

void CStressTestDlg::OnCtlRequestComplete (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr lRequest (Request);
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlRequestComplete [%d] [%d] [%8.8X] [%ls]"), mControlCharacterId, lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
#endif
	if	(
			(mShowRequest)
		&&	(mShowRequest.ID == lRequest.ID)
		)
	{
		mShowRequest = NULL;
	}
	if	(
			(mGestureRequest)
		&&	(mGestureRequest.ID == lRequest.ID)
		)
	{
		mGestureRequest = NULL;
	}
	if	(
			(mSpeechRequest)
		&&	(mSpeechRequest.ID == lRequest.ID)
		)
	{
		mSpeechRequest = NULL;
	}
}

void CStressTestDlg::OnCtlCommand (IDaCtlUserInput* UserInput)
{
	IDaCtlUserInputPtr lUserInput (UserInput);
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlCommand [%ls] [%ls]"), mControlCharacterId, (BSTR)lUserInput.CharacterID, (BSTR)lUserInput.Name);
#endif
}

void CStressTestDlg::OnCtlIdleStart (LPCTSTR CharacterID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlIdleStart [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CStressTestDlg::OnCtlIdleComplete (LPCTSTR CharacterID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlIdleComplete [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CStressTestDlg::OnCtlMove (LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlMove [%s] [%d %d] [%d]"), mControlCharacterId, CharacterID, X, Y, Cause);
#endif
}

void CStressTestDlg::OnCtlSize (LPCTSTR CharacterID, short Width, short Height)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlSize [%s] [%d %d]"), mControlCharacterId, CharacterID, Width, Height);
#endif
}

void CStressTestDlg::OnCtlBalloonShow(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlBalloonShow [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CStressTestDlg::OnCtlBalloonHide(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlBalloonHide [%s]"), mControlCharacterId, CharacterID);
#endif
}

void CStressTestDlg::OnCtlActiveClientChange (LPCTSTR CharacterID, BOOL Active)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlActiveClientChange [%s] [%d]"), mControlCharacterId, CharacterID, Active);
#endif
}
