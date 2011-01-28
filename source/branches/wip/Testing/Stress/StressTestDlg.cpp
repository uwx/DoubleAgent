#include "StdAfx.h"
#include <shlwapi.h>
#include <psapi.h>
#include "StressTest.h"
#include "StressTestDlg.h"
#include "Elapsed.h"
#include "UiState.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "LocalizeEx.h"
#include "DebugProcess.h"
#include "DebugWin.h"
#include "DebugStr.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_LOG_AGENT_CALLS		LogNormal|LogTimeMs|LogHighVolume
//#define	_LOG_CHAR_CALLS			LogNormal|LogTimeMs|LogHighVolume
//#define	_LOG_NOTIFY				LogNormal|LogTimeMs|LogHighVolume
#endif

#ifndef	_LOG_AGENT_CALLS
#define	_LOG_AGENT_CALLS		LogNormal|LogTimeMs
#endif
#ifndef	_LOG_CHAR_CALLS
#define	_LOG_CHAR_CALLS			LogNormal|LogTimeMs
#endif
#ifndef	_LOG_NOTIFY
#define	_LOG_NOTIFY				LogDetails|LogTimeMs
#endif
#ifndef	_LOG_PROGRESS
#define	_LOG_PROGRESS			LogNormal|LogTime
#endif
#ifndef	_LOG_PROGRESS_MORE
#define	_LOG_PROGRESS_MORE		LogDetails|LogTime
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
	ON_BN_CLICKED(IDC_CHARACTERS_01, OnCharacterCount)
	ON_BN_CLICKED(IDC_CHARACTERS_02, OnCharacterCount)
	ON_BN_CLICKED(IDC_CHARACTERS_05, OnCharacterCount)
	ON_BN_CLICKED(IDC_CHARACTERS_10, OnCharacterCount)
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
	mCharacterSelNdx (-1),
	mGestureSelNdx (-1),
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
	FreeServerCharacters ();
	FreeAgentServers ();
	FreeControlCharacters ();
	FreeAgentControls ();
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
	DDX_Control(pDX, IDC_RANDOM_STOP_ALL, mRandomStopAll);
	DDX_Control(pDX, IDC_STRESS_SOUND, mStressSound);
	DDX_Control(pDX, IDC_STRESS_SPEAK, mStressSpeak);
	DDX_Control(pDX, IDC_CONTROL_PLACER, mControlPlacer);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDC_CHARACTERS_01, mCharacterCount1);
	DDX_Control(pDX, IDC_CHARACTERS_02, mCharacterCount2);
	DDX_Control(pDX, IDC_CHARACTERS_05, mCharacterCount5);
	DDX_Control(pDX, IDC_CHARACTERS_10, mCharacterCount10);
	DDX_Control(pDX, IDOK, mOkButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	DDX_Control(pDX, IDC_GESTURES, mGestures);
	DDX_Control(pDX, IDC_CYCLE_INDICATOR, mCycleIndicator);
	//}}AFX_DATA_MAP
}

BOOL CStressTestDlg::OnInitDialog()
{
	bool	lAutoStart;

	CDialog::OnInitDialog();

	ModifyStyleEx (0, WS_EX_APPWINDOW);

	GetAgentServers ();
	ShowCharacters ();
	FreeAgentServers ();

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
			(mServerCharacter.GetCount() > 0)
		||	(mControlCharacter.GetCount() > 0)
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

	mCharacterPath.Empty ();
	mCharacterSelNdx = -1;
	mGestureSelNdx = -1;
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
		mCharacterSelNdx = pCharacterNdx;
		mCharacterList.SetItemState (mCharacterSelNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		mCharacterList.EnsureVisible (mCharacterSelNdx, FALSE);
		mCharacterList.RedrawWindow ();

		lRet = ShowCharacter (mCharacterList.GetItemText (mCharacterSelNdx, 0));
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
	LogMessage (_LOG_PROGRESS, _T("Show Character [%d %d] %s"), mCycleNum, mCharacterSelNdx, pCharacterPath);

	if	(mCharacterPath.CompareNoCase (CString (pCharacterPath)) != 0)
	{
		FreeServerCharacters ();
		FreeAgentServers ();
		FreeControlCharacters ();
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
			&&	(ShowServerCharacters (-1))
			)
		{
			lRet = true;
		}

		if	(
				(mStressControl.GetCheck ())
			&&	(ShowControlCharacters (-1))
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
		HRESULT			lResult = S_FALSE;
		tSafeArrayPtr	lAnimations;
		VARTYPE			lAnimationType;
		long			lLowerBound, lUpperBound, lNdx;
		CRect			lClientRect;

		if	(
				(
					(
						(mServerCharacter.GetCount() > 0)
					&&	(mServerCharacter[0])
					&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mServerCharacter[0]->get_Animations (lAnimations.Free()))))
					)
				||	(
						(mControlCharacter.GetCount() > 0)
					&&	(mControlCharacter[0])
					&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mControlCharacter[0]->get_Animations (lAnimations.Free()))))
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

		CheckServerOk (lResult);
	}
	catch AnyExceptionDebug

	mGestures.SetCurSel (-1);
	mGestures.SetRedraw (TRUE);
	mGestureSelNdx = -1;
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
		mGestureSelNdx = pGestureNdx;
		mGestures.SetCurSel (mGestureSelNdx);
		mGestures.RedrawWindow ();
		mGestures.GetText (mGestureSelNdx, lGestureName);
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
	HRESULT	lResult = S_FALSE;
	int		lNdx;

	mGestureReqId.RemoveAll();
	mSpeechReqId.RemoveAll();
	mGestureRequest.RemoveAll();
	mSpeechRequest.RemoveAll();

	if	(
			(pGestureName)
		&&	(pGestureName[0])
		)
	{
		LogMessage (_LOG_PROGRESS_MORE, _T("  Show %s Gesture %s"), PathFindFileName(mCharacterPath), pGestureName);

		if	(
				(mStressServer.GetCheck())
			&&	(mServerCharacter.GetCount() > 0)
			)
		{
			mGestureReqId.SetCount (mServerCharacter.GetCount());
			mSpeechReqId.SetCount (mServerCharacter.GetCount());

			for	(lNdx = 0; lNdx < (int)mServerCharacter.GetCount(); lNdx++)
			{
				if	(mServerCharacter[lNdx])
				{
					if	(mStressSound.GetCheck ())
					{
						mServerCharacter[lNdx].Style |= CharacterStyle_SoundEffects;
					}
					else
					{
						mServerCharacter[lNdx].Style &= ~CharacterStyle_SoundEffects;
					}
					if	(
							(
								(mStressSpeak.GetCheck())
							?	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mServerCharacter[lNdx]->Speak (_bstr_t(GestureNameSpeech(pGestureName)), NULL, &mSpeechReqId[lNdx]))))
							:	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mServerCharacter[lNdx]->Think (_bstr_t(GestureNameSpeech(pGestureName)), &mSpeechReqId[lNdx]))))
							)
						&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mServerCharacter[lNdx]->Play (_bstr_t(pGestureName), &mGestureReqId[lNdx]))))
						)
					{
						//LogMessage (LogNormal, _T("[%s] Gesture [%s] SpeechReqId [%d] GestureReqId [%d]"), mCharacterPath, pGestureName, mSpeechReqId[lNdx], mGestureReqId[lNdx]);
						lRet = true;
					}
					if	(!CheckServerOk (lResult))
					{
						break;
					}
				}
			}
		}

		if	(
				(mStressControl.GetCheck())
			&&	(mControlCharacter.GetCount() > 0)
			)
		{
			mGestureRequest.SetCount (mControlCharacter.GetCount());
			mSpeechRequest.SetCount (mControlCharacter.GetCount());

			for	(lNdx = 0; lNdx < (int)mControlCharacter.GetCount(); lNdx++)
			{
				if	(mControlCharacter[lNdx])
				{
					if	(mStressSound.GetCheck ())
					{
						mControlCharacter[lNdx].SoundEffectsEnabled = true;
					}
					else
					{
						mControlCharacter[lNdx].SoundEffectsEnabled = false;
					}
					if	(
							(
								(mStressSpeak.GetCheck())
							?	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mControlCharacter[lNdx]->Speak (_variant_t(GestureNameSpeech(pGestureName)), _variant_t(), &mSpeechRequest[lNdx]))))
							:	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mControlCharacter[lNdx]->Think (_bstr_t(GestureNameSpeech(pGestureName)), &mSpeechRequest[lNdx]))))
							)
						&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mControlCharacter[lNdx]->Play (_bstr_t(pGestureName), &mGestureRequest[lNdx]))))
						)
					{
						//LogMessage (LogNormal, _T("[%s] Gesture [%s] SpeechReqId [%d] GestureReqId [%d]"), mCharacterPath, pGestureName, mSpeechRequest[lNdx].ID, mGestureRequest[lNdx].ID);
						lRet = true;
					}
					if	(!CheckServerOk (lResult))
					{
						break;
					}
				}
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
	int	lNdx;

	for	(lNdx = 0; lNdx < (int)mServerCharacter.GetCount(); lNdx++)
	{
		if	(
				(mServerCharacter[lNdx])
			&&	(
					(!mServerCharacter[lNdx].Visible)
				||	(
						(lNdx < (int)mShowReqId.GetCount())
					&&	(mShowReqId[lNdx] > 0)
					)
				||	(
						(lNdx < (int)mGestureReqId.GetCount())
					&&	(mGestureReqId[lNdx] > 0)
					)
				||	(
						(lNdx < (int)mSpeechReqId.GetCount())
					&&	(mSpeechReqId[lNdx] > 0)
					)
				)
			)
		{
			return true;
		}
	}

	for	(lNdx = 0; lNdx < (int)mControlCharacter.GetCount(); lNdx++)
	{
		if	(
				(mControlCharacter[lNdx])
			&&	(
					(!mControlCharacter[lNdx].Visible)
				||	(
						(lNdx < (int)mShowRequest.GetCount())
					&&	(mShowRequest[lNdx])
					&&	(
							(mShowRequest[lNdx].Status == RequestStatus_Pending)
						||	(mShowRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (int)mGestureRequest.GetCount())
					&&	(mGestureRequest[lNdx])
					&&	(
							(mGestureRequest[lNdx].Status == RequestStatus_Pending)
						||	(mGestureRequest[lNdx].Status == RequestStatus_InProgress)
						)
					)
				||	(
						(lNdx < (int)mSpeechRequest.GetCount())
					&&	(mSpeechRequest[lNdx])
					&&	(
							(mSpeechRequest[lNdx].Status == RequestStatus_Pending)
						||	(mSpeechRequest[lNdx].Status == RequestStatus_InProgress)
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

bool CStressTestDlg::Stop (bool pRandom)
{
	bool	lRet = false;
	bool	lDone = false;
	HRESULT	lResult = S_FALSE;
	int		lNdx;

	mShowReqId.SetCount (mServerCharacter.GetCount());
	mGestureReqId.SetCount (mServerCharacter.GetCount());
	mSpeechReqId.SetCount (mServerCharacter.GetCount());

	for	(lNdx = 0; (lNdx < (int)mServerCharacter.GetCount()) && (!lDone); lNdx++)
	{
		mShowReqId[lNdx] = 0;
		mGestureReqId[lNdx] = 0;
		mSpeechReqId[lNdx] = 0;

		if	(
				(mServerCharacter[lNdx])
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mServerCharacter[lNdx]->StopAll (StopAll_Everything), _T("[%d] [%d] StopAll"), lNdx, mServerCharacterId[lNdx])))
			)
		{
			lRet = true;

			if	(
					(pRandom)
				&&	(lResult == S_OK)
				&&	(!mRandomStopAll.GetCheck())
				)
			{
				lDone = true;
			}
		}
		
		CheckServerOk (lResult);
	}

	mShowRequest.SetCount (mControlCharacter.GetCount());
	mGestureRequest.SetCount (mControlCharacter.GetCount());
	mSpeechRequest.SetCount (mControlCharacter.GetCount());

	for	(lNdx = 0; (lNdx < (int)mControlCharacter.GetCount()) && (!lDone); lNdx++)
	{
		mShowRequest[lNdx] = NULL;
		mGestureRequest[lNdx] = NULL;
		mSpeechRequest[lNdx] = NULL;

		if	(
				(mControlCharacter[lNdx])
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult = mControlCharacter[lNdx]->StopAll (_variant_t((long)(StopAll_Everything),VT_I4)), _T("[%d] [%s] StopAll"), lNdx, mControlCharacterId[lNdx])))
			)
		{
			lRet = true;

			if	(
					(pRandom)
				&&	(lResult == S_OK)
				&&	(!mRandomStopAll.GetCheck())
				)
			{
				lDone = true;
			}
		}

		CheckServerOk (lResult);
	}

	mCycleIndicator.ShowWindow (IsAnimating()?TRUE:FALSE);
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CStressTestDlg::GetAgentServers (int pServerCount)
{
	HRESULT	lResult = S_FALSE;
	int		lNdx;

	if	(pServerCount <= 0)
	{
		pServerCount = 1;
	}
	if	((int)mServer.GetCount() < pServerCount)
	{
		mServer.SetCount (pServerCount);
	}
	mServerSinkId.SetCount (mServer.GetCount());

	for	(lNdx = 0; lNdx < (int)mServer.GetCount(); lNdx++)
	{
		if	(!mServer[lNdx])
		{
			IUnknownPtr	lUnknown;

			lResult = LogComErr (_LOG_AGENT_CALLS, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("[%d] CoCreateInstance"), lNdx);
			mServer[lNdx] = lUnknown;

			if	(mServer[lNdx] != NULL)
			{
				lResult = LogComErr (_LOG_AGENT_CALLS, mServer[lNdx]->Register (&m_xDaSvrNotifySink, &mServerSinkId[lNdx]), _T("[%d] Register"), lNdx);
				mServer[lNdx].CharacterStyle = CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_Smoothed;
			}
		}
		if	(!CheckServerOk (lResult))
		{
			break;
		}
	}
	return lResult;
}

HRESULT CStressTestDlg::FreeAgentServers (int pServerCount)
{
	HRESULT	lResult = S_FALSE;
	int		lNdx;

	if	(pServerCount <= 0)
	{
		pServerCount = 0;
	}
	pServerCount = min (pServerCount, (int)mServer.GetCount());

	for	(lNdx = pServerCount; lNdx < (int)mServer.GetCount(); lNdx++)
	{
		if	(
				(mServer[lNdx])
			&&	(lNdx < (int)mServerSinkId.GetCount())
			&&	(mServerSinkId[lNdx])
			)
		{
			try
			{
				lResult = LogComErr (_LOG_AGENT_CALLS, mServer[lNdx]->Unregister (mServerSinkId[lNdx]), _T("[%d] Unregister"), lNdx);
			}
			catch AnyExceptionSilent

			CheckServerOk (lResult);
		}
	}

	mServer.SetCount (pServerCount);
	mServerSinkId.SetCount (mServer.GetCount());
	return lResult;
}

int CStressTestDlg::GetServerMultiple () const
{
	if	(mCharacterCount1.m_hWnd)
	{
		return mCharacterCount10.GetCheck()?10:mCharacterCount5.GetCheck()?5:mCharacterCount2.GetCheck()?2:1;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::GetAgentControls (int pControlCount)
{
	HRESULT	lResult = S_FALSE;
	int		lNdx;

	if	(mControlPlacer.m_hWnd)
	{
		mControlRect = ChildWndRect (mControlPlacer);
		mControlPlacer.DestroyWindow();
	}

	if	(pControlCount <= 0)
	{
		pControlCount = 1;
	}
	if	(mControlContained.GetCheck())
	{
		pControlCount = max (pControlCount, 1);
	}
	if	((int)mControl.GetCount() < pControlCount)
	{
		mControl.SetCount (pControlCount);
	}
	mControlWnd.SetCount (max (mControlWnd.GetCount(), mControl.GetCount()));

	for	(lNdx = 0; lNdx < (int)mControl.GetCount(); lNdx++)
	{
		if	(
				(!mControlWnd[lNdx]->GetSafeHwnd())
			&&	(mControlWnd[lNdx] = new CWnd)
			&&	(mControlWnd[lNdx]->CreateControl (__uuidof(DaControl), _T(""), WS_CHILD|WS_CLIPSIBLINGS, mControlRect, this, IDC_CONTROL_PLACER+(UINT)lNdx))
			)
		{
			mControl[lNdx] = mControlWnd[lNdx]->GetControlSite()->m_pObject;
		}
		if	(mControl[lNdx])
		{
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
			mControl[lNdx].CharacterStyle = CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_Smoothed;
		}
	}
}

void CStressTestDlg::FreeAgentControls (int pControlCount)
{
	HRESULT	lResult = S_FALSE;
	int		lNdx;

	if	(pControlCount <= 0)
	{
		pControlCount = 0;
	}
	pControlCount = min (pControlCount, (int)mControl.GetCount());
	mControl.SetCount (pControlCount);

	for	(lNdx = pControlCount; lNdx < (int)mControlWnd.GetCount(); lNdx++)
	{
		if	(mControlWnd[lNdx]->GetSafeHwnd())
		{
			mControlWnd[lNdx]->DestroyWindow ();
		}
	}
	mControlWnd.SetCount (pControlCount);
}

int CStressTestDlg::GetControlMultiple () const
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

bool CStressTestDlg::ShowServerCharacters (int pCharacterCount)
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;
	CString	lWinTitle;
	int		lNdx;
	int		lServerNdx;

	if	(pCharacterCount == 0)
	{
		pCharacterCount = 1;
	}
	else
	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetServerMultiple ();
	}

	if	(
			(CheckServerOk (GetAgentServers (pCharacterCount)))
		&&	(mServer.GetCount() > 0)
		&&	(mServer[0])
		&&	(!mCharacterPath.IsEmpty ())
		)
	{
		if	((int)mServerCharacter.GetCount() < pCharacterCount)
		{
			mServerCharacter.SetCount (pCharacterCount);
		}
		mServerCharacterId.SetCount (mServerCharacter.GetCount());
		mCharacterServer.SetCount (mServerCharacter.GetCount());
		mShowReqId.SetCount (mServerCharacter.GetCount());

		for	(lNdx = 0; lNdx < (int)mServerCharacter.GetCount(); lNdx++)
		{
			if	(
					(lNdx < (int)mServer.GetCount())
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
				mServerCharacterId[lNdx] = 0;
				mCharacterServer[lNdx] = mServer[lServerNdx];

				lResult = mCharacterServer[lNdx]->Load (_variant_t(mCharacterPath), &mServerCharacterId[lNdx], &lReqID);
				LogComErr (_LOG_AGENT_CALLS, lResult, _T("[%d] Load [%d] [%s] as [%d]"), lNdx, lReqID, mCharacterPath, mServerCharacterId[lNdx]);

				if	(FAILED (lResult))
				{
					mServerCharacterId[lNdx] = 0;
					mCharacterServer[lNdx] = NULL;
				}
			}

			if	(
					(mServerCharacterId[lNdx] != 0)
				&&	(!mServerCharacter[lNdx])
				&&	(mCharacterServer[lNdx])
				&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, lResult = mCharacterServer[lNdx]->get_Character (mServerCharacterId[lNdx], &mServerCharacter[lNdx]), _T("[%d] GetCharacter [%d]"), lNdx, mServerCharacterId[lNdx])))
				)
			{
				mServerCharacter[lNdx].Style &= ~CharacterStyle_IdleEnabled;
				if	(mStressSound.GetCheck ())
				{
					mServerCharacter[lNdx].Style |= CharacterStyle_SoundEffects;
				}
				else
				{
					mServerCharacter[lNdx].Style &= ~CharacterStyle_SoundEffects;
				}
			}

			if	(mServerCharacter[lNdx])
			{
				CPoint	lCharPos;
				CSize	lCharSize;

				if	(
						(SUCCEEDED (mServerCharacter[lNdx]->GetSize (&lCharSize.cx, &lCharSize.cy)))
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
					mServerCharacter[lNdx]->SetSize (lCharSize.cx, lCharSize.cy);
				}

				if	(!mServerCharacter[lNdx].Visible)
				{
					lCharPos = GetInitialPos (lCharSize);
					lResult = mServerCharacter[lNdx]->MoveTo ((short)lCharPos.x, (short)lCharPos.y, 0, &lReqID);
				}

				mShowReqId[lNdx] = 0;
				if	(SUCCEEDED (lResult))
				{
					lResult = mServerCharacter[lNdx]->Show (FALSE, &mShowReqId[lNdx]);
					if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] [%d] Show [%d]"), lNdx, mServerCharacterId[lNdx], mShowReqId[lNdx])))
					{
						lRet = true;
					}
				}
			}
			if	(!CheckServerOk (lResult))
			{
				break;
			}
		}
	}

	if	(
			(mServerCharacter.GetCount() > 0)
		&&	(!mServerCharacter[mServerCharacter.GetCount()-1])
		)
	{
		FreeServerCharacters ();
		FreeAgentServers ();
		OnCancel ();
	}
	return lRet;
}

bool CStressTestDlg::HideServerCharacters ()
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;
	int		lNdx;

	for	(lNdx = 0; lNdx < (int)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mServerCharacter[lNdx])
		{
			lResult = mServerCharacter[lNdx]->Hide (FALSE, &lReqID);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] [%d] Hide [%d]"), lNdx, mServerCharacterId[lNdx], lReqID)))
			{
				lRet = true;
			}
			CheckServerOk (lResult);
		}
	}
	return lRet;
}

bool CStressTestDlg::FreeServerCharacters (int pCharacterCount)
{
	bool	lRet = false;
	HRESULT	lResult;
	int		lNdx;

	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetServerMultiple ();
	}
	pCharacterCount = min (pCharacterCount, (int)mServerCharacter.GetCount());

	for	(lNdx = pCharacterCount; lNdx < (int)mServerCharacter.GetCount(); lNdx++)
	{
		if	(mServerCharacter[lNdx] != NULL)
		{
			LogMessage (_LOG_PROGRESS, _T("[%d] Free Character [%d] %s"), lNdx, mServerCharacterId[lNdx], (BSTR)mServerCharacter[lNdx].FilePath);
			lRet = true;
		}
		mServerCharacter[lNdx] = NULL;

		if	(
				(mServerCharacterId[lNdx] != 0)
			&&	(mCharacterServer[lNdx])
			&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, lResult = mCharacterServer[lNdx]->Unload (mServerCharacterId[lNdx]), _T("[%d] Unload [%d]"), lNdx, mServerCharacterId[lNdx])))
			)
		{
			lRet = true;
		}
		mServerCharacterId[lNdx] = 0;
		mCharacterServer[lNdx] = 0;

		CheckServerOk (lResult);
	}

	mServerCharacter.SetCount (pCharacterCount);
	mServerCharacterId.SetCount (mServerCharacter.GetCount());
	mCharacterServer.SetCount (mServerCharacter.GetCount());

	mShowReqId.SetCount (mServerCharacter.GetCount());
	mGestureReqId.SetCount (mServerCharacter.GetCount());
	mSpeechReqId.SetCount (mServerCharacter.GetCount());

	FreeAgentServers (max (pCharacterCount,1));
	return lRet;
}

long CStressTestDlg::GetServerCharacterId (int pNdx) const
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CStressTestDlg::ShowControlCharacters (int pCharacterCount)
{
	bool					lRet = false;
	HRESULT					lResult;
	IDaCtlCharacters2Ptr	lCharacters;
	int						lNdx;
	int						lControlNdx;

	if	(pCharacterCount == 0)
	{
		pCharacterCount = 1;
	}
	else
	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetControlMultiple ();
	}

	GetAgentControls (pCharacterCount);

	if	(
			(mControl.GetCount() > 0)
		&&	(mControl[0])
		&&	(!mCharacterPath.IsEmpty ())
		)
	{
		if	((int)mControlCharacter.GetCount() < pCharacterCount)
		{
			mControlCharacter.SetCount (pCharacterCount);
		}
		mControlCharacterId.SetSize (mControlCharacter.GetCount());
		mCharacterControl.SetCount (mControlCharacter.GetCount());
		mShowRequest.SetCount (mControlCharacter.GetCount());

		for	(lNdx = 0; lNdx < (int)mControlCharacter.GetCount(); lNdx++)
		{
			if	(
					(lNdx < (int)mControl.GetCount())
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

				lResult = lCharacters->Load (_bstr_t(mControlCharacterId[lNdx]), _variant_t(mCharacterPath), NULL);
				LogComErr (_LOG_AGENT_CALLS, lResult, _T("[%d] Load [%s] as [%s]"), lNdx, mCharacterPath, mControlCharacterId[lNdx]);

				if	(FAILED (lResult))
				{
					mControlCharacterId[lNdx].Empty();
					mCharacterControl[lNdx] = NULL;
				}
			}

			if	(
					(!mControlCharacterId[lNdx].IsEmpty())
				&&	(!mControlCharacter[lNdx])
				&&	(mCharacterControl[lNdx])
				&&	(lCharacters = mCharacterControl[lNdx].Characters)
				&&	(mControlCharacter[lNdx] = lCharacters.Item [_bstr_t(mControlCharacterId[lNdx])])
				)
			{
				mControlCharacter[lNdx].IdleEnabled = false;
				mControlCharacter[lNdx].SoundEffectsEnabled = mStressSound.GetCheck() ? true : false;
			}

			if	(mControlCharacter[lNdx])
			{
				CSize	lCharSize (mControlCharacter[lNdx].Width, mControlCharacter[lNdx].Height);
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
					mControlCharacter[lNdx].Width = (short)lCharSize.cx;
					mControlCharacter[lNdx].Height = (short)lCharSize.cy;
				}

				if	(
						(mControlCharacterId[lNdx].CompareNoCase (sControlCharacterIdPopup) == 0)
					&&	(!mControlCharacter[lNdx].Visible)
					)
				{
					lCharPos = GetInitialPos (lCharSize);
					mControlCharacter[lNdx].Left = (short)lCharPos.x;
					mControlCharacter[lNdx].Top = (short)lCharPos.y;
				}

				if	(mControlContained.GetCheck())
				{
					mCharacterControl[lNdx].ControlCharacter = mControlCharacter[lNdx];
					mControlWnd[lNdx]->ShowWindow (SW_SHOWNA);
				}
				else
				{
					mCharacterControl[lNdx].ControlCharacter = NULL;
					mControlWnd[lNdx]->ShowWindow (SW_HIDE);
				}

				mShowRequest[lNdx] = NULL;
				lResult = mControlCharacter[lNdx]->Show (_variant_t (VARIANT_FALSE,VT_BOOL), &mShowRequest[lNdx]);
				if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] [%s] Show [%d]"), lNdx, mControlCharacterId[lNdx], mShowRequest[lNdx].ID)))
				{
					lRet = true;
				}
			}
			if	(!CheckServerOk (lResult))
			{
				break;
			}
		}
	}

	if	(
			(mControlCharacter.GetCount() > 0)
		&&	(!mControlCharacter [mControlCharacter.GetCount()-1])
		)
	{
		FreeControlCharacters ();
		OnCancel ();
	}
	return lRet;
}

bool CStressTestDlg::HideControlCharacters ()
{
	bool	lRet = false;
	HRESULT	lResult;
	int		lNdx;

	for	(lNdx = 0; lNdx < (int)mControlCharacter.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx])
		{
			lResult = mControlCharacter[lNdx]->Hide (_variant_t (VARIANT_FALSE,VT_BOOL), NULL);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] [%s] Hide"), lNdx, mControlCharacterId[lNdx])))
			{
				lRet = true;
			}
			CheckServerOk (lResult);
		}
	}
	return lRet;
}

bool CStressTestDlg::FreeControlCharacters (int pCharacterCount)
{
	bool					lRet = false;
	IDaCtlCharacters2Ptr	lCharacters;
	HRESULT					lResult;
	int						lNdx;

	if	(pCharacterCount < 0)
	{
		pCharacterCount = GetControlMultiple ();
	}
	pCharacterCount = min (pCharacterCount, (INT_PTR)mControlCharacter.GetCount());

	mShowRequest.RemoveAll();
	mGestureRequest.RemoveAll();
	mSpeechRequest.RemoveAll();

	for	(lNdx = pCharacterCount; lNdx < (INT_PTR)mControlCharacter.GetCount(); lNdx++)
	{
		if	(mControlCharacter[lNdx])
		{
			LogMessage (_LOG_PROGRESS, _T("[%d] Free Character %s"), lNdx, (BSTR)mControlCharacter[lNdx].FilePath);
			lRet = true;
		}
		mControlCharacter[lNdx] = NULL;

		if	(
				(!mControlCharacterId[lNdx].IsEmpty())
			&&	(mCharacterControl[lNdx])
			&&	(lCharacters = mCharacterControl[lNdx].Characters)
			&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, lResult = lCharacters->Unload (_bstr_t(mControlCharacterId[lNdx])), _T("[%d] Unload [%s]"), lNdx, mControlCharacterId[lNdx])))
			)
		{
			lRet = true;
		}
		mControlCharacterId[lNdx].Empty ();
		mCharacterControl[lNdx] = NULL;

		CheckServerOk (lResult);
	}

	mControlCharacter.SetCount (pCharacterCount);
	mControlCharacterId.SetSize (mControlCharacter.GetCount());
	mCharacterControl.SetCount (mControlCharacter.GetCount());

	FreeAgentControls (max (pCharacterCount,1));
	return lRet;
}

CString CStressTestDlg::GetControlCharacterId (int pNdx) const
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
static LPCTSTR	sProfileStopAll = _T("RandomStopAll");
static LPCTSTR	sProfileCharacterCount = _T("CharacterCount");

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
	mCharacterCount10.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharacterCount, 0) == 10);
	mCharacterCount5.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharacterCount, 0) == 5);
	mCharacterCount2.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileCharacterCount, 0) == 2);
	mCharacterCount1.SetCheck (!mCharacterCount10.GetCheck() && !mCharacterCount5.GetCheck() && !mCharacterCount2.GetCheck());
	mRandomStop0.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, !mRandomStop0.GetCheck()) ? FALSE : TRUE);
	mRandomStop1.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, mRandomStop1.GetCheck() ? 1 : 0) == 1);
	mRandomStop2.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, mRandomStop2.GetCheck() ? 2 : 0) == 2);
	mRandomStop3.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRandomStop, mRandomStop3.GetCheck() ? 3 : 0) == 3);
	mRandomStopAll.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileStopAll, mRandomStopAll.GetCheck()) ? TRUE : FALSE);

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
	lApp->WriteProfileInt (sProfileKey, sProfileCharacterCount, GetServerMultiple());
	lApp->WriteProfileInt (sProfileKey, sProfileRandomStop, mRandomStop1.GetCheck() ? 1 : mRandomStop2.GetCheck() ? 2 : mRandomStop3.GetCheck() ? 3 : 0);
	lApp->WriteProfileInt (sProfileKey, sProfileStopAll, mRandomStopAll.GetCheck());

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
					(lArg.CompareNoCase (_T("Count")) == 0)
				&&	(lArgNdx < __argc-1)
				)
			{
				long	lCount;

				if	(LclParseNumber (__targv [lArgNdx+1], lCount))
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
			if	(
					(lArg.CompareNoCase (_T("Random")) == 0)
				&&	(lArgNdx < __argc-1)
				)
			{
				long lRandomStop;

				if	(LclParseNumber (__targv [lArgNdx+1], lRandomStop))
				{
					lArgNdx++;
					mRandomStop0.SetCheck (lRandomStop == 0);
					mRandomStop1.SetCheck (lRandomStop == 1);
					mRandomStop2.SetCheck (lRandomStop == 2);
					mRandomStop3.SetCheck (lRandomStop == 3);
				}
			}
			else
			if	(
					(lArg.CompareNoCase (_T("RandomAll")) == 0)
				||	(lArg.CompareNoCase (_T("RandomAll+")) == 0)
				)
			{
				mRandomStopAll.SetCheck (TRUE);
			}
			else
			if	(lArg.CompareNoCase (_T("RandomAll-")) == 0)
			{
				mRandomStopAll.SetCheck (FALSE);
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
		LogMessage (_LOG_PROGRESS, _T("Start %s"), lTitle);
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

bool CStressTestDlg::CheckServerOk (HRESULT pResult)
{
	if	(
			(FAILED (pResult))
		&&	(HRESULT_CODE (pResult) == RPC_S_SERVER_UNAVAILABLE)
		)
	{
		LogComErrAnon (LogIfActive, pResult, _T("*** Stopping ***"));
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
		return false;
	}
	return true;
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

				while	(++mGestureSelNdx < mGestures.GetCount())
				{
					PrimeMessagePump ();
					if	(lGestureShown = ShowGesture (mGestureSelNdx))
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
				while	(++mCharacterSelNdx < mCharacterList.GetItemCount ())
				{
					PrimeMessagePump ();
					if	(ShowCharacter (mCharacterSelNdx))
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
			while	(mCharacterSelNdx < mCharacterList.GetItemCount());
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
			&&	(mCharacterSelNdx >= mCharacterList.GetItemCount()-1)
			&&	(mGestureSelNdx >= mGestures.GetCount())
			)
		{
			PrimeMessagePump ();
			mCycleNum++;
			mCharacterSelNdx = -1;
			mGestureSelNdx = -1;

			if	(!mStressRepeat.GetCheck ())
			{
				KillTimer (mCycleTimer);
				mCycleTimer = 0;
				OnRandomStop ();
			}
		}

		mCycleIndicator.ShowWindow (IsAnimating()?TRUE:FALSE);
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
			if	(mServerCharacter.GetCount() > 0)
			{
				LogMessage (_LOG_PROGRESS_MORE, _T("  Random stop [%d %d %d]"), (mShowReqId.GetCount()>0)?mShowReqId[0]:-1, (mGestureReqId.GetCount()>0)?mGestureReqId[0]:-1, (mSpeechReqId.GetCount()>0)?mSpeechReqId[0]:-1);
			}
			if	(mControlCharacter.GetCount() > 0)
			{
				LogMessage (_LOG_PROGRESS_MORE, _T("  Random stop [%d %d] [%d %d] [%d %d]"), (mShowRequest.GetCount()>0)?mShowRequest[0].ID:-1, (mShowRequest.GetCount()>0)?mShowRequest[0].Status:-1, (mGestureRequest.GetCount()>0)?mGestureRequest[0].ID:-1, (mGestureRequest.GetCount()>0)?mGestureRequest[0].Status:-1, (mSpeechRequest.GetCount()>0)?mSpeechRequest[0].ID:-1, (mSpeechRequest.GetCount()>0)?mSpeechRequest[0].Status:-1);
			}
			Stop (true);
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
		if	(mCharacterSelNdx != lNotify->iItem)
		{
			ShowCharacter (lNotify->iItem);
		}
	}
}

void CStressTestDlg::OnSelChangeGestures()
{
	if	(mGestureSelNdx != mGestures.GetCurSel ())
	{
		ShowGesture (mGestures.GetCurSel ());
	}
}

void CStressTestDlg::OnControlMode()
{
	ShowControlMode ();
}

void CStressTestDlg::OnCharacterCount()
{
	if	(mStressServer.GetCheck())
	{
		FreeServerCharacters (-1);
		if	(mCycleTimer)
		{
			ShowServerCharacters (-1);
		}
	}
	if	(mStressControl.GetCheck())
	{
		FreeControlCharacters (-1);
		if	(mCycleTimer)
		{
			ShowControlCharacters (-1);
		}
	}
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

		//LogMessage (_LOG_PROGRESS_MORE, _T("Starting Random stop [%u]"), lRandomTime);
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
	FreeServerCharacters ();
	FreeAgentServers ();
	FreeControlCharacters ();
	FreeAgentControls ();

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
		LogComErr (_LOG_PROGRESS, mServerCharacter->Activate (ActiveState_InputActive), _T("[%d] Activate ActiveState_Active"), mServerCharacterId);
	}
#endif
#if	FALSE
	if	(
			(bActive)
		&&	(mControlCharacter != NULL)
		)
	{
		LogComErr (_LOG_PROGRESS, mControlCharacter->Activate (_variant_t((short)ActiveState_InputActive,VT_I4), NULL), _T("[%s] Activate ActiveState_Active"), mControlCharacterId);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Command"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ActivateInputState (long CharacterID, long Activated)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Activated);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Restart"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Shutdown"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		long	lCause = -1;
		int		lNdx = (int)pThis->mServerCharacterId.Find (CharacterID);
		if	(
				(lNdx < (int)pThis->mServerCharacter.GetCount())
			&&	(pThis->mServerCharacter[lNdx])
			)
		{
			pThis->mServerCharacter[lNdx]->GetVisibilityCause (&lCause);
		}
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d] [%d]"), pThis->GetServerCharacterId(lNdx), pThis->m_dwRef, CharacterID, Visible, Cause, lCause);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Click (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
	}
#else
	LogMessage (_LOG_PROGRESS, _T("Click [%d]"), CharacterID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DblClick (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
	}
#endif
	pThis->BringWindowToTop ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DragStart (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DragComplete (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Keys, x, y);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::RequestStart (long RequestID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->GetServerCharacterId(0), pThis->m_dwRef, RequestID);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::RequestComplete (long RequestID, long Result)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->GetServerCharacterId(0), pThis->m_dwRef, RequestID, Result);
	}
#endif
	int	lNdx;

	for	(lNdx = 0; lNdx < (int)pThis->mShowReqId.GetCount(); lNdx++)
	{
		if	(RequestID == pThis->mShowReqId[lNdx])
		{
			pThis->mShowReqId[lNdx] = 0;
		}
	}
	for	(lNdx = 0; lNdx < (int)pThis->mGestureReqId.GetCount(); lNdx++)
	{
		if	(RequestID == pThis->mGestureReqId[lNdx])
		{
			pThis->mGestureReqId[lNdx] = 0;
		}
	}
	for	(lNdx = 0; lNdx < (int)pThis->mSpeechReqId.GetCount(); lNdx++)
	{
		if	(RequestID == pThis->mSpeechReqId[lNdx])
		{
			pThis->mSpeechReqId[lNdx] = 0;
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::BookMark (long BookMarkID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::BookMark"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Start);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Move (long CharacterID, long x, long y, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		long	lCause = -1;
		int		lNdx = (int)pThis->mServerCharacterId.Find (CharacterID);
		if	(
				(lNdx < (int)pThis->mServerCharacter.GetCount())
			&&	(pThis->mServerCharacter[lNdx])
			)
		{
			pThis->mServerCharacter[lNdx]->GetMoveCause (&lCause);
		}
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d] [%d]"), pThis->GetServerCharacterId(lNdx), pThis->m_dwRef, CharacterID, x, y, Cause, lCause);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::Size (long CharacterID, long Width, long Height)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Width, Height);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Visible);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ListeningState"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR GUID)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->GetServerCharacterId(0), pThis->m_dwRef, GUID);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->GetServerCharacterId(0), pThis->m_dwRef);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CStressTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CStressTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CStressTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->GetServerCharacterId((int)pThis->mServerCharacterId.Find(CharacterID)), pThis->m_dwRef, CharacterID, Status);
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CStressTestDlg::OnCtlActivateInput(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlActivateInput [%s]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID);
	}
#endif
}

void CStressTestDlg::OnCtlDeactivateInput(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDeactivateInput [%s]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID);
	}
#endif
}

void CStressTestDlg::OnCtlClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlClick [%s] [%d] [%d] [%d %d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
	}
#else
	LogMessage (_LOG_PROGRESS, _T("Click [%s]"), CharacterID);
#endif
}

void CStressTestDlg::OnCtlDblClick (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDblClick [%s] [%d] [%d] [%d %d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
	}
#endif
	BringWindowToTop ();
}

void CStressTestDlg::OnCtlDragStart (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDragStart [%s] [%d] [%d] [%d %d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
	}
#endif
}

void CStressTestDlg::OnCtlDragComplete (LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlDragComplete [%s] [%d] [%d] [%d %d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Button, Shift, X, Y);
	}
#endif
}

void CStressTestDlg::OnCtlShow (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlShow [%s] [%d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Cause);
	}
#endif
}

void CStressTestDlg::OnCtlHide (LPCTSTR CharacterID, VisibilityCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlHide [%s] [%d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Cause);
	}
#endif
}

void CStressTestDlg::OnCtlRequestStart (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr lRequest (Request);
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlRequestStart [%d] [%d] [%8.8X] [%ls]"), GetControlCharacterId(0), lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
	}
#endif
}

void CStressTestDlg::OnCtlRequestComplete (IDaCtlRequest* Request)
{
	IDaCtlRequestPtr	lRequest (Request);
	int					lNdx;
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlRequestComplete [%d] [%d] [%8.8X] [%ls]"), GetControlCharacterId(0), lRequest.ID, lRequest.Status, lRequest.Number, (BSTR)lRequest.Description);
	}
#endif
	for	(lNdx = 0; lNdx < (int)mShowRequest.GetCount(); lNdx++)
	{
		if	(
				(mShowRequest[lNdx])
			&&	(mShowRequest[lNdx].ID == lRequest.ID)
			)
		{
			mShowRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (int)mGestureRequest.GetCount(); lNdx++)
	{
		if	(
				(mGestureRequest[lNdx])
			&&	(mGestureRequest[lNdx].ID == lRequest.ID)
			)
		{
			mGestureRequest[lNdx] = NULL;
		}
	}
	for	(lNdx = 0; lNdx < (int)mSpeechRequest.GetCount(); lNdx++)
	{
		if	(
				(mSpeechRequest[lNdx])
			&&	(mSpeechRequest[lNdx].ID == lRequest.ID)
			)
		{
			mSpeechRequest[lNdx] = NULL;
		}
	}
}

void CStressTestDlg::OnCtlCommand (IDaCtlUserInput* UserInput)
{
	IDaCtlUserInputPtr lUserInput (UserInput);
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlCommand [%ls] [%ls]"), GetControlCharacterId(0), (BSTR)lUserInput.CharacterID, (BSTR)lUserInput.Name);
	}
#endif
}

void CStressTestDlg::OnCtlIdleStart (LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlIdleStart [%s]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID);
	}
#endif
}

void CStressTestDlg::OnCtlIdleComplete (LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlIdleComplete [%s]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID);
	}
#endif
}

void CStressTestDlg::OnCtlMove (LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlMove [%s] [%d %d] [%d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, X, Y, Cause);
	}
#endif
}

void CStressTestDlg::OnCtlSize (LPCTSTR CharacterID, short Width, short Height)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlSize [%s] [%d %d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Width, Height);
	}
#endif
}

void CStressTestDlg::OnCtlBalloonShow(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlBalloonShow [%s]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID);
	}
#endif
}

void CStressTestDlg::OnCtlBalloonHide(LPCTSTR CharacterID)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlBalloonHide [%s]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID);
	}
#endif
}

void CStressTestDlg::OnCtlActiveClientChange (LPCTSTR CharacterID, BOOL Active)
{
#ifdef	_LOG_NOTIFY
	if	(LogIsActive (_LOG_NOTIFY))
	{
		LogMessage (_LOG_NOTIFY, _T("[%s] CStressTestDlg::OnCtlActiveClientChange [%s] [%d]"), GetControlCharacterId((int)FindString(mControlCharacterId,CharacterID)), CharacterID, Active);
	}
#endif
}
