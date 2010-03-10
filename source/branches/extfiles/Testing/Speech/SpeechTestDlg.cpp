#include "StdAfx.h"
#include <shlwapi.h>
#include <AfxPriv.h>
#include "SpeechTest.h"
#include "SpeechTestDlg.h"
#include "Registry.h"
#include "..\Core\Sapi5Voices.h"
#include "..\Core\Sapi5Inputs.h"
#ifndef	_WIN64
#include "..\Core\Sapi4Voices.h"
#endif
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
//#define	_LOG_AGENT_CALLS	LogNormal|LogTimeMs
//#define	_LOG_CHAR_CALLS		LogAlways|LogTimeMs|LogHighVolume
//#define	_LOG_CHAR_CALLS_EX	LogAlways|LogTimeMs|LogHighVolume
//#define	_LOG_NOTIFY			LogNormal|LogTimeMs
//#define	_LOG_COMMANDS		LogNormal|LogTimeMs
#endif

#ifndef	_LOG_AGENT_CALLS
#define	_LOG_AGENT_CALLS		LogDetails|LogTimeMs
#endif
#ifndef	_LOG_CHAR_CALLS
#define	_LOG_CHAR_CALLS			LogDetails|LogTimeMs
#endif
#ifndef	_LOG_CHAR_CALLS_EX
#define	_LOG_CHAR_CALLS_EX		LogDetails|LogTimeMs
#endif
#ifndef	_LOG_NOTIFY
#define	_LOG_NOTIFY				LogDetails|LogTimeMs
#endif
#ifndef	_LOG_COMMANDS
#define	_LOG_COMMANDS			LogDetails|LogTimeMs
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSpeechTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeechTestDlg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ACTIVATEAPP()
	ON_BN_CLICKED(IDC_SHOW_CHAR, OnShowChar)
	ON_BN_CLICKED(IDC_SHOW_CHAR2, OnShowChar2)
	ON_BN_CLICKED(IDC_THINK, OnThink)
	ON_BN_CLICKED(IDC_SPEAK, OnSpeak)
	ON_BN_CLICKED(IDC_LISTEN, OnListen)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, OnItemChangedCharacterList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CHARACTER_LIST, OnItemActivateCharacterList)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AGENT_PROPS, OnAgentProps)
	ON_BN_CLICKED(IDC_CHAR_PROPS, OnCharProps)
	ON_BN_CLICKED(IDC_BALLOON_VISIBLE, OnBalloonVisible)
	ON_BN_CLICKED(IDC_BALLOON_AUTOPACE, OnBalloonAutoPace)
	ON_BN_CLICKED(IDC_BALLOON_AUTOHIDE, OnBalloonAutoHide)
	ON_BN_CLICKED(IDC_BALLOON_AUTOSIZE, OnBalloonAutoSize)
	ON_CBN_SELENDOK(IDC_SPEECH_WAVE, OnSelEndOkSpeechWave)
	ON_CBN_SELENDOK(IDC_TTS_MODE, OnSelEndOkTTSModes)
	ON_CBN_SELENDOK(IDC_SR_MODE, OnSelEndOkSRModes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CSpeechTestDlg, CDialog)
	INTERFACE_PART(CSpeechTestDlg, __uuidof(IDaSvrNotifySink), DaSvrNotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN (CSpeechTestDlg, DaSvrNotifySink)

/////////////////////////////////////////////////////////////////////////////

CSpeechTestDlg::CSpeechTestDlg(CWnd* pParent)
:	CDialog(CSpeechTestDlg::IDD, pParent),
	mNotifySinkId (0),
	mLoadReqID (0),
	mActiveChar (-1),
	mTTSModeAdded (-1),
	mSRModeAdded (-1)
{
	//{{AFX_DATA_INIT(CSpeechTestDlg)
	mCharacterId[0] = 0;
	mCharacterId[1] = 0;
	//}}AFX_DATA_INIT
	EnableAutomation ();
}

CSpeechTestDlg::~CSpeechTestDlg()
{
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeechTestDlg)
	DDX_Control(pDX, IDC_BALLOON_AUTOSIZE, mBalloonAutoSize);
	DDX_Control(pDX, IDC_BALLOON_AUTOPACE, mBalloonAutoPace);
	DDX_Control(pDX, IDC_BALLOON_AUTOHIDE, mBalloonAutoHide);
	DDX_Control(pDX, IDC_BALLOON_VISIBLE, mBalloonVisible);
	DDX_Control(pDX, IDC_AGENT_PROPS, mAgentPropsButton);
	DDX_Control(pDX, IDC_CHAR_PROPS, mCharPropsButton);
	DDX_Control(pDX, IDC_BALLOON_TEXT2, mBalloonTextEdit2);
	DDX_Control(pDX, IDC_SPEECH_WAVE, mSpeechWave);
	DDX_Control(pDX, IDC_THINK, mThinkButton);
	DDX_Control(pDX, IDC_SPEAK, mSpeakButton);
	DDX_Control(pDX, IDC_LISTEN, mListenButton);
	DDX_Control(pDX, IDC_BALLOON_TEXT1, mBalloonTextEdit1);
	DDX_Control(pDX, IDC_CHARACTER_LIST, mCharacterList);
	DDX_Control(pDX, IDC_SHOW_CHAR, mShowCharButton);
	DDX_Control(pDX, IDC_SHOW_CHAR2, mShowChar2Button);
	DDX_Control(pDX, IDC_TTS_MODE, mTTSModes);
	DDX_Control(pDX, IDC_TTS_STATUS, mTTSStatus);
	DDX_Control(pDX, IDC_SR_MODE, mSRModes);
	DDX_Control(pDX, IDC_SR_STATUS, mSRStatus);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
	//}}AFX_DATA_MAP
}

BOOL CSpeechTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetWindowText (mWinTitle);
	GetAgentServer ();
	ShowCharacters ();
	ShowTTSModes ();
	ShowSRModes ();
	LoadConfig ();
	ShowCharacterState ();

	mStatusTimer = SetTimer ((UINT_PTR)&mStatusTimer, 500, NULL);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::ShowCharacters ()
{
	IDaSvrCharacterFilesPtr	lCharacterFiles;
	tSafeArrayPtr			lFilePaths;
	VARTYPE					lFilePathType;
	long					lLowerBound, lUpperBound, lNdx;
	CRect					lClientRect;

	mCharacterList.InsertColumn (0, _T("Path"));

	if	(
			(mServer != NULL)
		&&	(SUCCEEDED (LogComErr (_LOG_AGENT_CALLS, mServer->GetCharacterFiles (&lCharacterFiles))))
		)
	{
#if	TRUE
		lCharacterFiles->put_Filter (FILES_PATH_MASK);
#endif
#if	FALSE
		lCharacterFiles->put_Filter (FILES_PATH_MASK|FILES_EXCLUDE_NONSPEAKING);
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

	mCharacterList.GetClientRect (&lClientRect);
	mCharacterList.SetColumnWidth (0, lClientRect.Width());
}

/////////////////////////////////////////////////////////////////////////////

bool CSpeechTestDlg::ShowCharacter (INT_PTR pCharNdx, LPCTSTR pCharacterPath)
{
	bool	lRet = false;
	bool	lShowAgentChar = IsCharacterVisible (pCharNdx);

	if	(
			(pCharNdx >= 0)
		&&	(pCharNdx <= 1)
		)
	{
		if	(mCharacterPath[pCharNdx].CompareNoCase (CString (pCharacterPath)) != 0)
		{
			ReleaseAgentCharacter (pCharNdx);
			CharacterIsVisible (pCharNdx, false);
		}

		mCharacterPath[pCharNdx] = pCharacterPath;

		if	(lShowAgentChar)
		{
			ShowAgentCharacter (pCharNdx);
		}
		ShowCharacterState (pCharNdx);
		lRet = true;
	}
	return lRet;
}

bool CSpeechTestDlg::Stop (INT_PTR pCharNdx)
{
	bool	lRet = false;

	if	(pCharNdx < 0)
	{
		if	(Stop (0))
		{
			lRet = true;
		}
		if	(Stop (1))
		{
			lRet = true;
		}
	}
	else
	{
		long	lReqID = 0;

		if	(
				(mCharacter[pCharNdx] != NULL)
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mCharacter[pCharNdx]->StopAll (STOP_TYPE_PLAY|STOP_TYPE_SPEAK|STOP_TYPE_MOVE|STOP_TYPE_VISIBLE), _T("[%d] StopAll"), mCharacterId)))
			)
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::GetAgentServer ()
{
	if	(mServer == NULL)
	{
		IUnknownPtr	lUnknown;

		LogComErr (_LOG_AGENT_CALLS, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown), _T("CoCreateInstance"));
		if	(lUnknown != NULL)
		{
			mServer = lUnknown;
		}
		if	(mServer != NULL)
		{
			LogComErr (_LOG_AGENT_CALLS, mServer->Register (&m_xDaSvrNotifySink, &mNotifySinkId), _T("Register"));
		}
	}
}

bool CSpeechTestDlg::ShowAgentCharacter (INT_PTR pCharNdx)
{
	bool	lRet = false;
	HRESULT	lResult;

	GetAgentServer ();
	mLoadReqID = 0;

	if	(
			(mServer != NULL)
		&&	(pCharNdx >= 0)
		&&	(pCharNdx <= 1)
		&&	(mCharacterId[pCharNdx] == 0)
		&&	(!mCharacterPath[pCharNdx].IsEmpty ())
		)
	{
		lResult = mServer->Load (_variant_t(mCharacterPath[pCharNdx]), &mCharacterId[pCharNdx], &mLoadReqID);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("Load [%d] [%s] as [%d]"), mLoadReqID, mCharacterPath[pCharNdx], mCharacterId[pCharNdx]);
	}

	if	(lRet = LoadedAgentCharacter (pCharNdx))
	{
		mLoadReqID = 0;
	}
	return lRet;
}

bool CSpeechTestDlg::LoadedAgentCharacter (INT_PTR pCharNdx)
{
	bool	lRet = false;
	HRESULT	lResult;
	long	lReqID = 0;
	CString	lWinTitle;

	if	(
			(pCharNdx < 0)
		&&	(mCharacter [0] == NULL)
		)
	{
		pCharNdx = 0;
	}
	else
	if	(
			(pCharNdx < 0)
		&&	(mCharacter [1] == NULL)
		)
	{
		pCharNdx = 1;
	}

	if	(
			(mServer != NULL)
		&&	(pCharNdx >= 0)
		&&	(pCharNdx <= 1)
		)
	{
		if	(
				(mCharacterId[pCharNdx] != 0)
			&&	(mCharacter[pCharNdx] == NULL)
			)
		{
			LogComErr (_LOG_AGENT_CALLS, mServer->GetCharacterEx (mCharacterId[pCharNdx], &mCharacter[pCharNdx]), _T("GetCharacterEx"));

			if	(mCharacter[pCharNdx] != NULL)
			{
				IDaSvrCommandsPtr	lCommands (mCharacter[pCharNdx]);
				IDaSvrCommandPtr	lCommand;
				CString				lCommandName;
				long				lCommandId;
				long				lConfidenceThreshold;
				tBstrPtr			lConfidenceText;
				tBstrPtr			lFontName;
				long				lFontSize;

				if	(pCharNdx == 0)
				{
					LogComErr (_LOG_AGENT_CALLS, mCharacter[pCharNdx]->SetLanguageID (MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_CAN)));
					lCommands->SetCaption (_bstr_t("Test Speech"));
					lCommands->SetVoice (_bstr_t("test speech"));
					lCommands->SetGlobalVoiceCommandsEnabled (TRUE);
					lCommands->GetFontName (lFontName.Free());
					lCommands->GetFontSize (&lFontSize);
					//lCommands->SetVisible (FALSE);
					//LogMessage (LogDebug, _T("Font [%s] [%d]"), (BSTR)lFontName, lFontSize);
					lCommands->SetFontSize (10);
				}
				else
				{
					//lCommands->SetGlobalVoiceCommandsEnabled (FALSE);
				}
				lCommandName.Format (_T("Test %d"), mCharacterId[pCharNdx]);
				lCommands->AddEx (_bstr_t(lCommandName), _bstr_t("test* command"), _bstr_t(lCommandName), TRUE, TRUE, 0, &lCommandId);
				lCommands->GetCommandEx (lCommandId, &lCommand);
				lCommand->GetConfidenceThreshold (&lConfidenceThreshold);
				lCommand->GetConfidenceText (lConfidenceText.Free());
#ifdef	_LOG_COMMANDS
				LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lConfidenceThreshold, (BSTR)lConfidenceText);
#endif
				if	(pCharNdx == 0)
				{
					lCommands->Add (_bstr_t("Two"), _bstr_t("command test+ two"), TRUE, FALSE, &lCommandId);
					lCommands->GetCommandEx (lCommandId, &lCommand);
					lCommand->SetConfidenceThreshold (50);
					lCommand->SetConfidenceText (_bstr_t("I thought I heard..."));
					lCommand->GetConfidenceThreshold (&lConfidenceThreshold);
					lCommand->GetConfidenceText (lConfidenceText.Free());
#ifdef	_LOG_COMMANDS
					LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lConfidenceThreshold, (BSTR)lConfidenceText);
#endif
					lCommands->Add (_bstr_t("Three"), _bstr_t("command … three"), TRUE, FALSE, &lCommandId);
					lCommands->GetCommandEx (lCommandId, &lCommand);
					lCommand->GetConfidenceThreshold (&lConfidenceThreshold);
					lCommand->GetConfidenceText (lConfidenceText.Free());
#ifdef	_LOG_COMMANDS
					LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lConfidenceThreshold, (BSTR)lConfidenceText);
#endif
					lCommands->Add (_bstr_t("Four"), _bstr_t("(command number)* four"), TRUE, FALSE, &lCommandId);
					lCommands->GetCommandEx (lCommandId, &lCommand);
					lCommand->GetConfidenceThreshold (&lConfidenceThreshold);
					lCommand->GetConfidenceText (lConfidenceText.Free());
#ifdef	_LOG_COMMANDS
					LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lConfidenceThreshold, (BSTR)lConfidenceText);
#endif
				}
			}

			if	(
					(mCharacter[pCharNdx] != NULL)
				&&	(mCharacterPos[pCharNdx].Ptr())
				)
			{
				mCharacter[pCharNdx]->SetPosition (mCharacterPos[pCharNdx]->x, mCharacterPos[pCharNdx]->y);
			}
		}

		if	(mCharacter[pCharNdx] != NULL)
		{
			long	lSpeed = 0;
			short	lPitch = 0;

			mCharacter[pCharNdx]->GetTTSSpeed (&lSpeed);
			mCharacter[pCharNdx]->GetTTSPitch (&lPitch);
			LogMessage (LogNormal, _T("TTSSpeed [%d] TTSPitch [%hd]"), lSpeed, lPitch);
		}

		if	(mCharacter[pCharNdx] != NULL)
		{
			LogComErr (_LOG_CHAR_CALLS, mCharacter[pCharNdx]->SetIdleOn (TRUE));
			lResult = mCharacter[pCharNdx]->Show (FALSE, &lReqID);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId[pCharNdx], lReqID)))
			{
				lRet = true;
			}

			lWinTitle.Format (_T("%s [%d %d]"), mWinTitle, mCharacterId[0], mCharacterId[1]);
			SetWindowText (lWinTitle);
			lRet = true;
		}
	}

	ShowCharacterState ();
	return lRet;
}

bool CSpeechTestDlg::HideAgentCharacter (INT_PTR pCharNdx)
{
	bool	lRet = false;
	long	lReqID = 0;
	HRESULT	lResult;

	if	(
			(pCharNdx >= 0)
		&&	(pCharNdx <= 1)
		&&	(mCharacter[pCharNdx] != NULL)
		&&	(IsCharacterVisible (pCharNdx))
		)
	{
		lResult = mCharacter[pCharNdx]->Hide (FALSE, &lReqID);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Hide [%d]"), mCharacterId[pCharNdx], lReqID)))
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSpeechTestDlg::ReleaseAgentCharacter (INT_PTR pCharNdx)
{
	bool	lRet = false;

	if	(pCharNdx < 0)
	{
		if	(ReleaseAgentCharacter (0))
		{
			lRet = true;
		}
		if	(ReleaseAgentCharacter (1))
		{
			lRet = true;
		}
	}
	else
	if	(
			(pCharNdx >= 0)
		&&	(pCharNdx <= 1)
		&&	(mCharacter[pCharNdx] != NULL)
		)
	{
		mCharacterPos[pCharNdx] = new CPoint (0,0);
		mCharacter[pCharNdx]->GetPosition (&mCharacterPos[pCharNdx]->x, &mCharacterPos[pCharNdx]->y);

		mCharacter[pCharNdx] = NULL;

		if	(
				(mServer != NULL)
			&&	(mCharacterId[pCharNdx] != 0)
			)
		{
			LogComErr (_LOG_AGENT_CALLS, mServer->Unload (mCharacterId[pCharNdx]), _T("Unload [%d]"), mCharacterId[pCharNdx]);
		}

		mCharacterId[pCharNdx] = 0;
		if	(IsWindow (m_hWnd))
		{
			CString	lWinTitle;
			lWinTitle.Format (_T("%s [%d %d]"), mWinTitle, mCharacterId[0], mCharacterId[1]);
			SetWindowText (lWinTitle);
		}
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CSpeechTestDlg::IsCharacterVisible (INT_PTR pCharNdx)
{
	bool	lRet = false;
	long	lVisible = FALSE;

	if	(pCharNdx < 0)
	{
		if	(IsCharacterVisible (0))
		{
			lRet = true;
		}
		if	(IsCharacterVisible (1))
		{
			lRet = true;
		}
	}
	else
	if	(
			(pCharNdx >= 0)
		&&	(pCharNdx <= 1)
		&&	(mCharacter[pCharNdx] != NULL)
		&&	(SUCCEEDED (mCharacter[pCharNdx]->GetVisible (&lVisible)))
		&&	(lVisible)
		)
	{
		lRet = true;
	}
	return lRet;
}

void CSpeechTestDlg::CharacterIsVisible (INT_PTR pCharNdx, bool pVisible)
{
	if	(pCharNdx == 0)
	{
		mShowCharButton.SetCheck (pVisible ? TRUE : FALSE);
	}
	if	(pCharNdx == 1)
	{
		mShowChar2Button.SetCheck (pVisible ? TRUE : FALSE);
	}

	if	(mCharacter[0] != NULL)
	{
		mShowChar2Button.EnableWindow (TRUE);
	}
	else
	{
		mShowChar2Button.EnableWindow (FALSE);
	}

	pCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
	if	(mCharacter[pCharNdx] == NULL)
	{
		pCharNdx = -1;
	}

	if	(pCharNdx >= 0)
	{
		IDaSvrBalloonPtr	lBalloon (mCharacter[pCharNdx]);
		tBstrPtr			lTTSMode;
		CString				lTTSModeStr;
		tBstrPtr			lSRMode;
		CString				lSRModeStr;

		mBalloonTextEdit1.EnableWindow (TRUE);
		mBalloonTextEdit2.EnableWindow (TRUE);
		mSpeechWave.EnableWindow (TRUE);
		mBalloonVisible.EnableWindow (TRUE);
		mCancelButton.EnableWindow (TRUE);
		mThinkButton.EnableWindow (TRUE);
		mSpeakButton.EnableWindow (TRUE);
		mListenButton.EnableWindow (TRUE);
		mBalloonAutoHide.EnableWindow (TRUE);
		mBalloonAutoPace.EnableWindow (TRUE);
		mBalloonAutoSize.EnableWindow (TRUE);
		mBalloonVisible.EnableWindow (TRUE);

		if	(lBalloon != NULL)
		{
			long	lStyle;
			long	lVisible;

			LogComErr (_LOG_CHAR_CALLS_EX, lBalloon->GetStyle (&lStyle), _T("[%d] Balloon->GetStyle"), mCharacterId[pCharNdx]);
			mBalloonAutoHide.SetCheck ((lStyle & BALLOON_STYLE_AUTOHIDE) != 0);
			mBalloonAutoPace.SetCheck ((lStyle & BALLOON_STYLE_AUTOPACE) != 0);
			mBalloonAutoSize.SetCheck ((lStyle & BALLOON_STYLE_SIZETOTEXT) != 0);
			LogComErr (_LOG_CHAR_CALLS_EX, lBalloon->GetVisible (&lVisible), _T("[%d] Balloon->GetVisible"), mCharacterId[pCharNdx]);
			mBalloonVisible.SetCheck (lVisible ? TRUE : FALSE);
		}
		else
		{
			mBalloonAutoHide.SetCheck (FALSE);
			mBalloonAutoPace.SetCheck (FALSE);
			mBalloonAutoSize.SetCheck (FALSE);
			mBalloonVisible.SetCheck (FALSE);
		}

		mTTSModes.EnableWindow (TRUE);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[pCharNdx]->GetTTSModeID (lTTSMode.Free()))))
		{
			lTTSModeStr = (BSTR)lTTSMode;
		}
#ifdef	_WIN64
		LogMessage (LogDebug, _T("Character [%d] TTSModeID [%s] [%d] [%d]"), mCharacterId[pCharNdx], lTTSModeStr, mSapi5Voices->FindVoiceId (lTTSModeStr), mSapi5Voices->FindVoiceName (lTTSModeStr));
#else
		LogMessage (LogDebug, _T("Character [%d] TTSModeID [%s] Sapi5 [%d] [%d] Sapi4 [%d]"), mCharacterId[pCharNdx], lTTSModeStr, mSapi5Voices->FindVoiceId (lTTSModeStr), mSapi5Voices->FindVoiceName (lTTSModeStr), mSapi4Voices->FindModeId (CGuidStr::Parse(lTTSModeStr)));
#endif
		if	(lTTSModeStr.IsEmpty())
		{
			if	(mTTSModeAdded >= 0)
			{
				mTTSModes.DeleteString (mTTSModeAdded);
				mTTSModeAdded = -1;
			}
			mTTSModes.SetCurSel (-1);
		}
		else
		{
			int	lModeNdx = (int)mSapi5Voices->FindVoiceId (lTTSModeStr);

			if	(lModeNdx < 0)
			{
				lModeNdx = (int)mSapi5Voices->FindVoiceName (lTTSModeStr);
			}
#ifndef	_WIN64
			if	(lModeNdx < 0)
			{
				lModeNdx = (int)mSapi4Voices->FindModeId (CGuidStr::Parse(lTTSModeStr));
				if	(lModeNdx >= 0)
				{
					lModeNdx += (int)mSapi5Voices->GetSize();
				}
			}
#endif
			mTTSModes.SetCurSel (lModeNdx);
			if	(lModeNdx < 0)
			{
				CString	lAddedStr;

				lTTSModeStr = _T("<")+lTTSModeStr+_T(">");
				if	(mTTSModeAdded >= 0)
				{
					mTTSModes.GetLBText (mTTSModeAdded, lAddedStr);
				}
				if	(
						(mTTSModeAdded >= 0)
					&&	(lAddedStr == lTTSModeStr)
					)
				{
					mTTSModes.SetCurSel (mTTSModeAdded);
				}
				else
				{
					if	(mTTSModeAdded >= 0)
					{
						mTTSModes.DeleteString (mTTSModeAdded);
					}
					mTTSModeAdded = mTTSModes.AddString (lTTSModeStr);
					mTTSModes.SetCurSel (mTTSModeAdded);
				}
			}
		}

		mSRModes.EnableWindow (TRUE);
		if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[pCharNdx]->GetSRModeID (lSRMode.Free()))))
		{
			lSRModeStr = (BSTR)lSRMode;
		}
		LogMessage (LogDebug, _T("Character [%d] SRModeID [%s] [%d] [%d]"), mCharacterId[pCharNdx], lSRModeStr, mSapi5Inputs->FindEngineName (lSRModeStr), mSapi5Inputs->FindEngineId (lSRModeStr));
		if	(lSRModeStr.IsEmpty ())
		{
			if	(mSRModeAdded >= 0)
			{
				mSRModes.DeleteString (mSRModeAdded);
				mSRModeAdded = -1;
			}
			mSRModes.SetCurSel (-1);
		}
		else
		{
			mSRModes.SetCurSel ((int)mSapi5Inputs->FindEngineName (lSRModeStr));
			if	(mSRModes.GetCurSel() < 0)
			{
				mSRModes.SetCurSel ((int)mSapi5Inputs->FindEngineId (lSRModeStr));
			}
			if	(mSRModes.GetCurSel() < 0)
			{
				CString	lAddedStr;

				lSRModeStr = _T("<")+lSRModeStr+_T(">");
				if	(mSRModeAdded >= 0)
				{
					mSRModes.GetLBText (mSRModeAdded, lAddedStr);
				}
				if	(
						(mSRModeAdded >= 0)
					&&	(lAddedStr == lSRModeStr)
					)
				{
					mSRModes.SetCurSel (mSRModeAdded);
				}
				else
				{
					if	(mSRModeAdded >= 0)
					{
						mSRModes.DeleteString (mSRModeAdded);
					}
					mSRModeAdded = mSRModes.AddString (lSRModeStr);
					mSRModes.SetCurSel (mSRModeAdded);
				}
			}
		}

		ShowTTSStatus ();
		ShowSRStatus ();
	}
	else
	{
		mBalloonTextEdit1.EnableWindow (FALSE);
		mBalloonTextEdit2.EnableWindow (FALSE);
		mSpeechWave.EnableWindow (FALSE);
		mBalloonVisible.EnableWindow (FALSE);
		mCancelButton.EnableWindow (FALSE);
		mThinkButton.EnableWindow (FALSE);
		mSpeakButton.EnableWindow (FALSE);
		mListenButton.EnableWindow (mCharacter[0] != NULL);

		mBalloonAutoHide.EnableWindow (FALSE);
		mBalloonAutoHide.SetCheck (FALSE);
		mBalloonAutoPace.EnableWindow (FALSE);
		mBalloonAutoPace.SetCheck (FALSE);
		mBalloonAutoSize.EnableWindow (FALSE);
		mBalloonAutoSize.SetCheck (FALSE);
		mBalloonVisible.EnableWindow (FALSE);
		mBalloonVisible.SetCheck (FALSE);

		mTTSModes.EnableWindow (mCharacter[0] != NULL);
		mSRModes.EnableWindow (mCharacter[0] != NULL);
	}
}

void CSpeechTestDlg::ShowCharacterState (INT_PTR pCharNdx)
{
	if	(pCharNdx < 0)
	{
		ShowCharacterState (0);
		ShowCharacterState (1);
	}
	else
	{
		CharacterIsVisible (pCharNdx, IsCharacterVisible (pCharNdx));
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::ShowTTSModes ()
{
	if	(mTTSModes.m_hWnd)
	{
		INT_PTR	lNdx;

		mTTSModes.ResetContent ();
		mTTSModeAdded = -1;

		if	(mSapi5Voices = (CSapi5Voices *) CSapi5Voices::CreateObject())
		{
			for	(lNdx = 0; lNdx <= mSapi5Voices->GetUpperBound(); lNdx++)
			{
				CSapi5VoiceInfo * lSapi5VoiceInfo = mSapi5Voices->GetAt (lNdx);
				mTTSModes.AddString (CString (lSapi5VoiceInfo->mVoiceName));
			}
		}
#ifndef	_WIN64
		if	(mSapi4Voices = (CSapi4Voices *) CSapi4Voices::CreateObject())
		{
			for	(lNdx = 0; lNdx <= mSapi4Voices->GetUpperBound(); lNdx++)
			{
				CSapi4VoiceInfo * lSapi4VoiceInfo = mSapi4Voices->GetAt (lNdx);
				mTTSModes.AddString (CString (lSapi4VoiceInfo->mVoiceName) + _T(" *"));
			}
		}
#endif
		mTTSModes.SetCurSel (-1);
	}
}

void CSpeechTestDlg::ShowTTSStatus ()
{
	if	(mTTSStatus.m_hWnd)
	{
		IDaSvrAudioOutputPropertiesPtr	lOutputProperties (mServer);
		long							lTTSStatus;
		CString							lStatusStr;

		if	(
				(lOutputProperties != NULL)
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, lOutputProperties->GetStatus (&(lTTSStatus=0)))))
			)
		{
			lStatusStr.Format (_T("%d"), lTTSStatus);

			switch (lTTSStatus)
			{
				case AUDIO_STATUS_AVAILABLE:			lStatusStr += _T(" AVAILABLE"); break;
				case AUDIO_STATUS_NOAUDIO:				lStatusStr += _T(" NOAUDIO"); break;
				case AUDIO_STATUS_CANTOPENAUDIO:		lStatusStr += _T(" CANTOPENAUDIO"); break;
				case AUDIO_STATUS_USERSPEAKING:			lStatusStr += _T(" USERSPEAKING"); break;
				case AUDIO_STATUS_CHARACTERSPEAKING:	lStatusStr += _T(" CHARACTERSPEAKING"); break;
				case AUDIO_STATUS_SROVERRIDEABLE:		lStatusStr += _T(" SROVERRIDEABLE"); break;
				case AUDIO_STATUS_ERROR:				lStatusStr += _T(" ERROR"); break;
			}
		}
		AfxSetWindowText (mTTSStatus, lStatusStr);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::ShowSRModes ()
{
	if	(mSRModes.m_hWnd)
	{
		INT_PTR				lNdx;
		CSapi5InputInfo *	lInputInfo;

		mSRModes.ResetContent ();
		mSRModeAdded = -1;

		if	(mSapi5Inputs = (CSapi5Inputs *) CSapi5Inputs::CreateObject())
		{
			for	(lNdx = 0; lNdx <= mSapi5Inputs->GetUpperBound(); lNdx++)
			{
				lInputInfo = mSapi5Inputs->GetAt (lNdx);
				mSRModes.AddString (CString (lInputInfo->mEngineName));
			}
		}
		mSRModes.SetCurSel (-1);
	}
}

void CSpeechTestDlg::ShowSRStatus ()
{
	if	(mSRStatus.m_hWnd)
	{
		INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
		long	lSRStatus;
		CString	lStatusStr;

		if	(
				(lCharNdx >= 0)
			&&	(mCharacter[lCharNdx])
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[lCharNdx]->GetSRStatus (&(lSRStatus=0)))))
			)
		{
			lStatusStr.Format (_T("%d"), lSRStatus);

			switch (lSRStatus)
			{
				case LISTEN_STATUS_CANLISTEN:				lStatusStr += _T(" CANLISTEN"); break;
				case LISTEN_STATUS_NOAUDIO:					lStatusStr += _T(" NOAUDIO"); break;
				case LISTEN_STATUS_NOTACTIVE:				lStatusStr += _T(" NOTACTIVE"); break;
				case LISTEN_STATUS_CANTOPENAUDIO:			lStatusStr += _T(" CANTOPENAUDIO"); break;
				case LISTEN_STATUS_COULDNTINITIALIZESPEECH:	lStatusStr += _T(" COULDNTINITIALIZESPEECH"); break;
				case LISTEN_STATUS_SPEECHDISABLED:			lStatusStr += _T(" SPEECHDISABLED"); break;
				case LISTEN_STATUS_ERROR:					lStatusStr += _T(" ERROR"); break;
			}
		}

		AfxSetWindowText (mSRStatus, lStatusStr);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileCharacter = _T("Character");
static LPCTSTR	sProfileShowChar = _T("Show");
static LPCTSTR	sProfileText1 = _T("Text");
static LPCTSTR	sProfileText2 = _T("Text2");
static LPCTSTR	sProfileWaveFile = _T("WavFile");
static LPCTSTR	sProfileWaveFiles = _T("WavFiles");
static LPCTSTR	sProfileCharPosX[2] = {_T("Left1"), _T("Left2")};
static LPCTSTR	sProfileCharPosY[2] = {_T("Top1"), _T("Top2")};

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;
	CString		lBalloonText;

	mCharacterPos[0] = new CPoint (0,0);
	mCharacterPos[0]->x = lApp->GetProfileInt (sProfileKey, sProfileCharPosX[0], mCharacterPos[0]->x);
	mCharacterPos[0]->y = lApp->GetProfileInt (sProfileKey, sProfileCharPosY[0], mCharacterPos[0]->y);
	mCharacterPos[1] = new CPoint (0,0);
	mCharacterPos[1]->x = lApp->GetProfileInt (sProfileKey, sProfileCharPosX[1], mCharacterPos[1]->x);
	mCharacterPos[1]->y = lApp->GetProfileInt (sProfileKey, sProfileCharPosY[1], mCharacterPos[1]->y);

	if	(ShowCharacter (0, lApp->GetProfileString (sProfileKey, sProfileCharacter, mCharacterPath[0])))
	{
		tS <LVFINDINFO>	lFindInfo;
		int				lFoundNdx;

		lFindInfo.flags = LVFI_STRING;
		lFindInfo.psz = mCharacterPath[0];
		lFindInfo.vkDirection = VK_NEXT;
		lFoundNdx = ListView_FindItem (mCharacterList.m_hWnd, -1, &lFindInfo);
		if	(lFoundNdx >= 0)
		{
			mCharacterList.SetItemState (lFoundNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}

		if	(lApp->GetProfileInt (sProfileKey, sProfileShowChar, mShowCharButton.GetCheck()))
		{
			ShowAgentCharacter (0);
		}
	}

	mBalloonTextEdit1.GetWindowText (lBalloonText);
	lBalloonText = lApp->GetProfileString (sProfileKey, sProfileText1, lBalloonText);
	mBalloonTextEdit1.SetWindowText (lBalloonText);

	mBalloonTextEdit2.GetWindowText (lBalloonText);
	lBalloonText = lApp->GetProfileString (sProfileKey, sProfileText2, lBalloonText);
	mBalloonTextEdit2.SetWindowText (lBalloonText);

	LoadWaveFiles ();

	GetWindowRect (&lWinRect);
	lWinRect.OffsetRect (lApp->GetProfileInt (sProfileKey, sProfilePosX, lWinRect.left) - lWinRect.left, lApp->GetProfileInt (sProfileKey, sProfilePosY, lWinRect.top) - lWinRect.top);
	MoveWindow (&lWinRect);
}

void CSpeechTestDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CPoint		lCharPos;
	CRect		lWinRect;
	CString		lBalloonText;

	if	(IsCharacterVisible (0))
	{
		if	(SUCCEEDED (mCharacter[0]->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			lApp->WriteProfileInt (sProfileKey, sProfileCharPosX[0], lCharPos.x);
			lApp->WriteProfileInt (sProfileKey, sProfileCharPosY[0], lCharPos.y);
		}
	}
	if	(IsCharacterVisible (1))
	{
		if	(SUCCEEDED (mCharacter[1]->GetPosition (&lCharPos.x, &lCharPos.y)))
		{
			lApp->WriteProfileInt (sProfileKey, sProfileCharPosX[1], lCharPos.x);
			lApp->WriteProfileInt (sProfileKey, sProfileCharPosY[1], lCharPos.y);
		}
	}

	lApp->WriteProfileInt (sProfileKey, sProfileShowChar, mShowCharButton.GetCheck());
	lApp->WriteProfileString (sProfileKey, sProfileCharacter, mCharacterPath[0]);

	mBalloonTextEdit1.GetWindowText (lBalloonText);
	lApp->WriteProfileString (sProfileKey, sProfileText1, lBalloonText);
	mBalloonTextEdit2.GetWindowText (lBalloonText);
	lApp->WriteProfileString (sProfileKey, sProfileText2, lBalloonText);

	SaveWaveFiles ();

	if	(!IsIconic ())
	{
		GetWindowRect (&lWinRect);
		lApp->WriteProfileInt (sProfileKey, sProfilePosX, lWinRect.left);
		lApp->WriteProfileInt (sProfileKey, sProfilePosY, lWinRect.top);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::LoadWaveFiles ()
{
	CRegKey	lProfileKey;

	if	(lProfileKey.Attach (AfxGetApp()->GetAppRegistryKey()))
	{
		CRegString	lWaveFile (lProfileKey, sProfileWaveFile);
		CRegStrings	lWaveFiles (lProfileKey, sProfileWaveFiles);
		int			lNdx;

		mSpeechWave.ResetContent ();
		for	(lNdx = 0; lNdx <= lWaveFiles.Value().GetUpperBound(); lNdx++)
		{
			mSpeechWave.AddString (lWaveFiles.Value() [lNdx]);
		}
		mSpeechWave.InsertString (0, _T(""));
		mSpeechWave.SetWindowText (lWaveFile.Value());
	}
}

void CSpeechTestDlg::SaveWaveFiles ()
{
	CRegKey	lProfileKey;

	if	(lProfileKey.Attach (AfxGetApp()->GetAppRegistryKey()))
	{
		CRegString	lWaveFile (lProfileKey, sProfileWaveFile, true);
		CRegStrings	lWaveFiles (lProfileKey, sProfileWaveFiles, true);
		CString		lFileName;
		int			lNdx;

		lWaveFile.Update (GetWaveFile ());

		lWaveFiles.Value().RemoveAll ();
		for	(lNdx = 0; lNdx < mSpeechWave.GetCount(); lNdx++)
		{
			mSpeechWave.GetLBText (lNdx, lFileName);
			if	(!lFileName.IsEmpty ())
			{
				AddUniqueString (lWaveFiles.Value(), lFileName);
			}
		}
		lWaveFiles.Update ();
	}
}

CString CSpeechTestDlg::GetWaveFile ()
{
	CString	lFileName;
	int		lSelNdx;

	mSpeechWave.GetWindowText (lFileName);
	lFileName.TrimLeft ();
	lFileName.TrimRight ();

	if	(lFileName.IsEmpty())
	{
		mSpeechWave.SetCurSel (0);
	}
	else
	{
		lSelNdx = mSpeechWave.FindStringExact (1, lFileName);
		if	(lSelNdx > 1)
		{
			mSpeechWave.DeleteString (lSelNdx);
		}
		if	(lSelNdx != 1)
		{
			lSelNdx = mSpeechWave.InsertString (1, lFileName);
		}
		mSpeechWave.SetCurSel (lSelNdx);
	}

	return lFileName;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnCancel()
{
	if	(!Stop ())
	{
		MessageBeep (MB_OK);
	}
}

void CSpeechTestDlg::OnOK()
{
}

void CSpeechTestDlg::OnClose()
{
	CDialog::EndDialog (IDOK);
}

void CSpeechTestDlg::OnDestroy()
{
	SaveConfig ();
	ReleaseAgentCharacter (-1);
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
	SafeFreeSafePtr (mCharacter[0]);
	SafeFreeSafePtr (mCharacter[1]);
	SafeFreeSafePtr (mServer);

	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnActivateApp(BOOL bActive, _MFC_ACTIVATEAPP_PARAM2 dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	if	(bActive)
	{
		if	(mCharacter[0] != NULL)
		{
			if	(mActiveChar == mCharacterId[1])
			{
				LogComErr (_LOG_CHAR_CALLS, mCharacter[0]->Activate (ACTIVATE_ACTIVE), _T("[%d] Activate ACTIVATE_ACTIVE"), mCharacterId[0]);
			}
			else
			{
				LogComErr (_LOG_CHAR_CALLS, mCharacter[0]->Activate (ACTIVATE_INPUTACTIVE), _T("[%d] Activate ACTIVATE_INPUTACTIVE"), mCharacterId[0]);
			}
		}
		if	(mCharacter[1] != NULL)
		{
			if	(mActiveChar == mCharacterId[1])
			{
				LogComErr (_LOG_CHAR_CALLS, mCharacter[1]->Activate (ACTIVATE_INPUTACTIVE), _T("[%d] Activate ACTIVATE_INPUTACTIVE"), mCharacterId[1]);
			}
			else
			{
				LogComErr (_LOG_CHAR_CALLS, mCharacter[1]->Activate (ACTIVATE_ACTIVE), _T("[%d] Activate ACTIVATE_ACTIVE"), mCharacterId[1]);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnItemChangedCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
//	LPNMLISTVIEW	lNotify = (LPNMLISTVIEW)pNMHDR;
//	CString			lCharacterPath;
//
//	if	(
//			(lNotify->uChanged & LVIF_STATE)
//		&&	(!(lNotify->uOldState & LVIS_SELECTED))
//		&&	(lNotify->uNewState & LVIS_SELECTED)
//		)
//	{
//		lCharacterPath = mCharacterList.GetItemText (lNotify->iItem, 0);
//		if	(lCharacterPath.CompareNoCase (mCharacterPath[0]) != 0)
//		{
//			ShowCharacter (0, lCharacterPath);
//		}
//	}
}

void CSpeechTestDlg::OnItemActivateCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnShowChar()
{
	if	(IsCharacterVisible (0))
	{
		if	(HideAgentCharacter (0))
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (0, false);
#endif
		}
	}
	else
	{
		POSITION	lSelection = mCharacterList.GetFirstSelectedItemPosition();
		int			lItem = mCharacterList.GetNextSelectedItem (lSelection);

		if	(lItem >= 0)
		{
			ShowCharacter (0, mCharacterList.GetItemText (lItem, 0));
			if	(ShowAgentCharacter (0))
			{
#ifdef	_DEBUG_NOT
				CharacterIsVisible (0, true);
#endif
			}
		}
	}
	ShowCharacterState ();
}

void CSpeechTestDlg::OnShowChar2()
{
	if	(IsCharacterVisible (1))
	{
		if	(HideAgentCharacter (1))
		{
#ifdef	_DEBUG_NOT
			CharacterIsVisible (1, false);
#endif
		}
	}
	else
	if	(mCharacter[0] != NULL)
	{
		POSITION	lSelection = mCharacterList.GetFirstSelectedItemPosition();
		int			lItem = mCharacterList.GetNextSelectedItem (lSelection);

		if	(lItem >= 0)
		{
			if	(
					(
						(!ShowCharacter (1, mCharacterList.GetItemText (lItem, 0)))
					||	(!ShowAgentCharacter (1))
					)
				&&	(
						(!ShowCharacter (1, mCharacterList.GetItemText (lItem+1, 0)))
					||	(!ShowAgentCharacter (1))
					)
				&&	(lItem > 0)
				)
			{
				ShowCharacter (1, mCharacterList.GetItemText (lItem-1, 0));
			}
			if	(ShowAgentCharacter (1))
			{
#ifdef	_DEBUG_NOT
				CharacterIsVisible (1, true);
#endif
			}
		}
	}
	ShowCharacterState ();
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnThink()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(
			(lCharNdx >= 0)
		&&	(IsCharacterVisible (lCharNdx))
		)
	{
		CString	lText;
		HRESULT	lResult;
		long	lReqID;

		mBalloonTextEdit1.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			lResult = mCharacter[lCharNdx]->Think (_bstr_t(lText), &lReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
		}
		mBalloonTextEdit2.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			lResult = mCharacter[lCharNdx]->Think (_bstr_t(lText), &lReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
		}
	}
	else
	{
		MessageBeep (0);
	}
}

void CSpeechTestDlg::OnSpeak()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(
			(lCharNdx >= 0)
		&&	(IsCharacterVisible (lCharNdx))
		)
	{
		CString	lText;
		CString	lWaveFile = GetWaveFile ();
		HRESULT	lResult;
		long	lReqID;

		mBalloonTextEdit1.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			lResult = mCharacter[lCharNdx]->Speak (_bstr_t(lText), _bstr_t(lWaveFile), &lReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d] [%s] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText), lWaveFile);
		}
		mBalloonTextEdit2.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			lResult = mCharacter[lCharNdx]->Speak (_bstr_t(lText), NULL, &lReqID);
			LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
		}
	}
	else
	{
		MessageBeep (0);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnSelEndOkSpeechWave()
{
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnBalloonAutoPace()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		IDaSvrBalloonPtr	lBalloon (mCharacter[lCharNdx]);

		if	(lBalloon != NULL)
		{
			long	lStyle;

			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, lBalloon->GetStyle (&lStyle), _T("[%d] Balloon->GetStyle"), mCharacterId[lCharNdx])))
			{
				lStyle ^= BALLOON_STYLE_AUTOPACE;
				LogComErr (_LOG_CHAR_CALLS, lBalloon->SetStyle (lStyle), _T("[%d] Balloon->SetStyle"), mCharacterId[lCharNdx]);
			}
			ShowCharacterState ();
		}
	}
}

void CSpeechTestDlg::OnBalloonAutoHide()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		IDaSvrBalloonPtr	lBalloon (mCharacter[lCharNdx]);

		if	(lBalloon != NULL)
		{
			long	lStyle;

			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, lBalloon->GetStyle (&lStyle), _T("[%d] Balloon->GetStyle"), mCharacterId[lCharNdx])))
			{
				lStyle ^= BALLOON_STYLE_AUTOHIDE;
				LogComErr (_LOG_CHAR_CALLS, lBalloon->SetStyle (lStyle), _T("[%d] Balloon->SetStyle"), mCharacterId[lCharNdx]);
			}
			ShowCharacterState ();
		}
	}
}

void CSpeechTestDlg::OnBalloonAutoSize()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		IDaSvrBalloonPtr	lBalloon (mCharacter[lCharNdx]);

		if	(lBalloon != NULL)
		{
			long	lStyle;

			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, lBalloon->GetStyle (&lStyle), _T("[%d] Balloon->GetStyle"), mCharacterId[lCharNdx])))
			{
				lStyle ^= BALLOON_STYLE_SIZETOTEXT;
				LogComErr (_LOG_CHAR_CALLS, lBalloon->SetStyle (lStyle), _T("[%d] Balloon->SetStyle"), mCharacterId[lCharNdx]);
			}
			ShowCharacterState ();
		}
	}
}

void CSpeechTestDlg::OnBalloonVisible()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		IDaSvrBalloonPtr	lBalloon (mCharacter[lCharNdx]);

		if	(lBalloon != NULL)
		{
			long	lVisible;

			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, lBalloon->GetVisible (&lVisible), _T("[%d] Balloon->GetVisible"), mCharacterId[lCharNdx])))
			{
				LogComErr (_LOG_CHAR_CALLS, lBalloon->SetVisible (!lVisible), _T("[%d] Balloon->SetVisible"), mCharacterId[lCharNdx]);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnListen()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(
			(lCharNdx >= 0)
		&&	(IsCharacterVisible (lCharNdx))
		)
	{
		HRESULT		lResult;
		tBstrPtr	lSRModeID;
		long		lSRStatus = 0;

		lResult = mCharacter[lCharNdx]->GetSRModeID (lSRModeID.Free());
		LogComErr (LogAlways, lResult, _T("[%d] GetSRModeID [%ls]"), mCharacterId[lCharNdx], (BSTR)lSRModeID);

		lResult = mCharacter[lCharNdx]->GetSRStatus (&(lSRStatus=0));
		LogComErr (LogAlways, lResult, _T("[%d] GetSRStatus [%d]"), mCharacterId[lCharNdx], lSRStatus);

		lResult = mCharacter[lCharNdx]->Listen (mListenButton.GetCheck() ? TRUE : FALSE);
		LogComErr (LogAlways, lResult, _T("[%d] Listen [%d]"), mCharacterId[lCharNdx], mListenButton.GetCheck() ? TRUE : FALSE);

		lResult = mCharacter[lCharNdx]->GetSRStatus (&(lSRStatus=0));
		LogComErr (LogAlways, lResult, _T("[%d] GetSRStatus [%d]"), mCharacterId[lCharNdx], lSRStatus);
	}
	else
	{
		MessageBeep (0);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnSelEndOkTTSModes()
{
	INT_PTR				lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
	int					lSelMode = mTTSModes.GetCurSel ();
	CSapi5VoiceInfo *	lSapi5VoiceInfo;

	if	(
			(lSelMode >= 0)
		&&	(lSelMode != mTTSModeAdded)
		&&	(mSapi5Voices.Ptr())
		&&	(mCharacter [lCharNdx] != NULL)
		)
	{
		if	(
				(mTTSModeAdded >= 0)
			&&	(lSelMode >	mTTSModeAdded)
			)
		{
			lSelMode--;
		}
		if	(
				(lSelMode <= mSapi5Voices->GetUpperBound())
			&&	(lSapi5VoiceInfo = mSapi5Voices->GetAt (lSelMode))
			)
		{
			LogComErr (LogAlways, mCharacter [lCharNdx]->SetTTSModeID (lSapi5VoiceInfo->mProduct), _T("SetTTSModeID [%ls]"), (BSTR)lSapi5VoiceInfo->mProduct);
			ShowCharacterState (lCharNdx);
		}
#ifndef	_WIN64
		else
		{
			CSapi4VoiceInfo *	lSapi4VoiceInfo;

			lSelMode -= mSapi5Voices->GetSize();
			if	(
					(lSelMode >= 0)
				&&	(lSelMode <= mSapi4Voices->GetUpperBound())
				&&	(lSapi4VoiceInfo = mSapi4Voices->GetAt (lSelMode))
				)
			{
				LogComErr (LogAlways, mCharacter [lCharNdx]->SetTTSModeID (_bstr_t((CString)CGuidStr(lSapi4VoiceInfo->mModeId))), _T("SetTTSModeID [%s]"), (CString)CGuidStr(lSapi4VoiceInfo->mModeId));
				ShowCharacterState (lCharNdx);
			}
		}
#endif
	}
}

void CSpeechTestDlg::OnSelEndOkSRModes()
{
	INT_PTR				lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
	int					lSelMode = mSRModes.GetCurSel ();
	CSapi5InputInfo *	lInputInfo;

	if	(
			(lSelMode >= 0)
		&&	(lSelMode != mSRModeAdded)
		&&	(mSapi5Inputs.Ptr())
		&&	(mCharacter [lCharNdx] != NULL)
		)
	{
		if	(
				(mSRModeAdded >= 0)
			&&	(lSelMode >	mSRModeAdded)
			)
		{
			lSelMode--;
		}
		if	(
				(lSelMode <= mSapi5Inputs->GetUpperBound())
			&&	(lInputInfo = mSapi5Inputs->GetAt (lSelMode))
			)
		{
			LogComErr (LogAlways, mCharacter [lCharNdx]->SetSRModeID (lInputInfo->mEngineName), _T("SetSRModeID [%ls]"), (BSTR)lInputInfo->mEngineName);
			ShowCharacterState (lCharNdx);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnAgentProps()
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

void CSpeechTestDlg::OnCharProps()
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

void CSpeechTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if	(nIDEvent == mStatusTimer)
	{
		ShowTTSStatus ();
		ShowSRStatus ();
	}
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfoCount(UINT *pctinfo)
{
	METHOD_PROLOGUE_EX_(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfoCount"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfoCount (pctinfo);
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	METHOD_PROLOGUE_EX_(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfo"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfo (iTInfo, lcid, ppTInfo);
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	METHOD_PROLOGUE_EX_(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CSpeechTestDlg::XDaSvrNotifySink::GetIDsOfNames"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	METHOD_PROLOGUE_EX(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CSpeechTestDlg::XDaSvrNotifySink::Invoke [%8.8X (%u)]"), pThis, pThis->m_dwRef, dispIdMember, dispIdMember);
#endif
	return pThis->GetIDispatch(FALSE)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Command (long dwCommandID, IUnknown *punkUserInput)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Command [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCommandID);
#endif
#ifdef	_LOG_COMMANDS
	IDaSvrUserInputPtr	lUserInput (punkUserInput);
	LogMessage (_LOG_COMMANDS, _T("Command [%d]"), dwCommandID);
	if	(lUserInput != NULL)
	{
		long		lItemCount = 0;
		long		lItemNdx;
		_variant_t	lItemIds;
		_variant_t	lItemConfidences;
		_variant_t	lItemTexts;

		LogComErr (_LOG_COMMANDS, lUserInput->GetCount (&lItemCount));
		LogMessage (_LOG_COMMANDS, _T("  UserInput [%d]"), lItemCount);

		for	(lItemNdx = 0; lItemNdx < lItemCount; lItemNdx++)
		{
			long		lItemId = -1;
			long		lItemConfidence = -1;
			tBstrPtr	lItemText;

			LogComErr (_LOG_COMMANDS, lUserInput->GetItemID (lItemNdx, &lItemId));
			LogComErr (_LOG_COMMANDS, lUserInput->GetItemConfidence (lItemNdx, &lItemConfidence));
			LogComErr (_LOG_COMMANDS, lUserInput->GetItemText (lItemNdx, lItemText.Free()));
			LogMessage (_LOG_COMMANDS, _T("    Item [%d] Id [%d] Text [%ls] Confidence [%d]"), lItemNdx, lItemId, (BSTR)lItemText, lItemConfidence);
		}

		if	(SUCCEEDED (LogComErr (_LOG_COMMANDS, lUserInput->GetAllItemData (&lItemIds, &lItemConfidences, &lItemTexts))))
		{
			long lBounds [6];

			SafeArrayGetLBound (V_ARRAY(&lItemIds), 1, lBounds);
			SafeArrayGetUBound (V_ARRAY(&lItemIds), 1, lBounds+1);
			SafeArrayGetLBound (V_ARRAY(&lItemConfidences), 1, lBounds+2);
			SafeArrayGetUBound (V_ARRAY(&lItemConfidences), 1, lBounds+3);
			SafeArrayGetLBound (V_ARRAY(&lItemTexts), 1, lBounds+4);
			SafeArrayGetUBound (V_ARRAY(&lItemTexts), 1, lBounds+5);
			LogMessage (_LOG_COMMANDS, _T("  Bounds [%d %d] [%d %d] [%d %d]"), lBounds[0], lBounds[1], lBounds[2], lBounds[3], lBounds[4], lBounds[5]);

			for	(lItemNdx = 0; lItemNdx < lItemCount; lItemNdx++)
			{
				long	lItemId = -1;
				long	lItemConfidence = -1;
				BSTR	lItemText = NULL;

				SafeArrayGetElement (V_ARRAY(&lItemIds), &lItemNdx, &lItemId);
				SafeArrayGetElement (V_ARRAY(&lItemConfidences), &lItemNdx, &lItemConfidence);
				SafeArrayGetElement (V_ARRAY(&lItemTexts), &lItemNdx, &lItemText);

				LogMessage (_LOG_COMMANDS, _T("    Item [%d] Id [%d] Text [%ls] Confidence [%d]"), lItemNdx, lItemId, lItemText, lItemConfidence);
			}
		}
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::ActivateInputState (long dwCharID, long bActivated)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bActivated);
#endif
	if	(bActivated)
	{
		if	(pThis->mActiveChar != dwCharID)
		{
			pThis->mActiveChar = dwCharID;
			pThis->ShowCharacterState ();
		}
	}
	else
	{
		if	(pThis->mActiveChar == dwCharID)
		{
			pThis->mActiveChar = 0;
			pThis->ShowCharacterState ();
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Restart"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Shutdown"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::VisibleState (long dwCharID, long bVisible, long dwCause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bVisible, dwCause);
#endif
	pThis->ShowCharacterState ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Click (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DblClick (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DragStart (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DragComplete (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::RequestStart (long dwRequestID)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwRequestID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::RequestComplete (long dwRequestID, long hrStatus)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, dwRequestID, hrStatus);
#endif
	if	(dwRequestID == pThis->mLoadReqID)
	{
		pThis->mLoadReqID = 0;
		pThis->LoadedAgentCharacter (-1);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::BookMark (long dwBookMarkID)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwBookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Idle (long dwCharID, long bStart)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bStart);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Move (long dwCharID, long x, long y, long dwCause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, x, y, dwCause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Size (long dwCharID, long lWidth, long lHeight)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, lWidth, lHeight);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::BalloonVisibleState (long dwCharID, long bVisible)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bVisible);
#endif
	pThis->mBalloonVisible.SetCheck (bVisible ? TRUE : FALSE);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::HelpComplete (long dwCharID, long dwCommandID, long dwCause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::ListeningState (long dwCharID, long bListening, long dwCause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	CString	lCauseStr;
	switch (dwCause)
	{
		case LSCOMPLETE_CAUSE_PROGRAMDISABLED:		lCauseStr = _T("PROGRAMDISABLED"); break;
		case LSCOMPLETE_CAUSE_PROGRAMTIMEDOUT:		lCauseStr = _T("PROGRAMTIMEDOUT"); break;
		case LSCOMPLETE_CAUSE_USERTIMEDOUT:			lCauseStr = _T("USERTIMEDOUT"); break;
		case LSCOMPLETE_CAUSE_USERRELEASEDKEY:		lCauseStr = _T("USERRELEASEDKEY"); break;
		case LSCOMPLETE_CAUSE_USERUTTERANCEENDED:	lCauseStr = _T("USERUTTERANCEENDED"); break;
		case LSCOMPLETE_CAUSE_CLIENTDEACTIVATED:	lCauseStr = _T("CLIENTDEACTIVATED"); break;
		case LSCOMPLETE_CAUSE_DEFAULTCHARCHANGE:	lCauseStr = _T("DEFAULTCHARCHANGE"); break;
		case LSCOMPLETE_CAUSE_USERDISABLED:			lCauseStr = _T("USERDISABLED"); break;
		default:									lCauseStr.Format (_T("%d"), dwCause); break;
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::ListeningState [%d] [%d] cause [%d] [%s]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, bListening, dwCause, lCauseStr);
#endif
	if	(!bListening)
	{
		pThis->mListenButton.SetCheck (FALSE);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR bszGUID)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mCharacterId, pThis->m_dwRef, bszGUID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::AgentPropertyChange"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	if	(pThis->mCharacter[0] != NULL)
	{
		long	lSpeed = 0;
		short	lPitch = 0;

		pThis->mCharacter[0]->GetTTSSpeed (&lSpeed);
		pThis->mCharacter[0]->GetTTSPitch (&lPitch);
		LogMessage (LogNormal, _T("TTSSpeed [%d] TTSPitch [%hd]"), lSpeed, lPitch);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::ActiveClientChange (long dwCharID, long lStatus)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, dwCharID, lStatus);
#endif
	return S_OK;
}
