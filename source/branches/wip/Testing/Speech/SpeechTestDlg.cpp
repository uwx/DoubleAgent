#include "StdAfx.h"
#include <shlwapi.h>
#include <AfxPriv.h>
#include "SpeechTest.h"
#include "SpeechTestDlg.h"
#include "Registry.h"
#include "UiState.h"
#include "NotifyLock.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "DebugStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal|LogHighVolume
//#define	_LOG_AGENT_CALLS	LogNormal|LogTimeMs
#define	_LOG_CHAR_CALLS		LogAlways|LogTimeMs|LogHighVolume
//#define	_LOG_CHAR_CALLS_EX	LogAlways|LogTimeMs|LogHighVolume
#define	_LOG_NOTIFY			LogNormal|LogTimeMs|LogHighVolume
//#define	_LOG_COMMANDS		LogNormal|LogTimeMs
#endif

#ifndef	_LOG_TTS_MODES
#define	_LOG_TTS_MODES			LogDetails
#endif
#ifndef	_LOG_SR_MODES
#define	_LOG_SR_MODES			LogDetails
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

#define	_APPEND_FORMATTED

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
	ON_BN_CLICKED(IDC_BALLOON_PARTIAL, OnBalloonPartialLines)
	ON_CBN_SELENDOK(IDC_SPEECH_WAVE, OnSelEndOkSpeechWave)
	ON_CBN_SELENDOK(IDC_TTS_MODE, OnSelEndOkTTSModes)
	ON_CBN_SELENDOK(IDC_SR_MODE, OnSelEndOkSRModes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CSpeechTestDlg, CDialog)
	INTERFACE_PART(CSpeechTestDlg, __uuidof(IDaSvrNotifySink2), DaSvrNotifySink)
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
	DDX_Control(pDX, IDC_BALLOON_PARTIAL, mBalloonPartialLines);
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
	DDX_Control(pDX, IDC_TEST_FORMATTED, mFormattedButton);
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
	mCharacterList.GetClientRect (&lClientRect);
	lClientRect.right -= GetSystemMetrics (SM_CXVSCROLL);

	if	(mServer != NULL)
	{
		lCharacterFiles = mServer.CharacterFiles;
#if	TRUE
		lCharacterFiles.Filter = FilesFilter_PathMask;
#endif
#if	FALSE
		lCharacterFiles.Filter = (FilesFilter_PathMask|FilesFilter_ExcludeNonSpeaking);
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

	mCharacterList.InsertItem (0, _T("<default>"));
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
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mCharacter[pCharNdx]->StopAll (StopAll_Play|StopAll_Speak|StopAll_Move|StopAll_Visibility), _T("[%d] StopAll"), mCharacterId)))
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
			LogComErr (LogNormal, mServer->put_CharacterStyle (CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_AutoPopupMenu|CharacterStyle_IconShown|CharacterStyle_SmoothEdges));
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
		)
	{
		variant_t	lProvider;

		if	(!mCharacterPath[pCharNdx].IsEmpty ())
		{
			lProvider = mCharacterPath[pCharNdx];
		}
		lResult = mServer->Load (lProvider, &mCharacterId[pCharNdx], &mLoadReqID);
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
			//LogComErr (_LOG_AGENT_CALLS, mServer->get_Character (mCharacterId[pCharNdx], &mCharacter[pCharNdx]), _T("GetCharacterEx"));
			mCharacter[pCharNdx] = mServer.Character [mCharacterId[pCharNdx]];

			if	(mCharacter[pCharNdx] != NULL)
			{
				IDaSvrCommands2Ptr	lCommands  = mCharacter[pCharNdx].Commands;
				IDaSvrCommand2Ptr	lCommand;
				CString				lCommandName;
				long				lCommandId;
				long				lClientCount = 0;

				lClientCount = mCharacter[pCharNdx].OtherClientCount;

				if	(pCharNdx == 0)
				{
					mCharacter[pCharNdx].LanguageID = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_CAN);
					lCommands.Caption = (lClientCount==0)?_bstr_t("My Application"):_bstr_t("Another Application **");
					lCommands.VoiceGrammar = _bstr_t("test speech");
					lCommands.GlobalVoiceCommandsEnabled = true;
					//lCommands.Visible = FALSE;
					//lCommands.FontSize = 10;
				}
				else
				{
					//lCommands->SetGlobalVoiceCommandsEnabled (FALSE);
				}
				//lCommandName.Format (_T("Test %d"), mCharacterId[pCharNdx]);
				lCommandName = _T("My First Command *");
				lCommands->AddEx (_bstr_t(lCommandName), _bstr_t("test* command"), _bstr_t(lCommandName), TRUE, TRUE, 0, &lCommandId);
				lCommand = lCommands.Command [lCommandId];
#ifdef	_LOG_COMMANDS
				LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lCommand.ConfidenceThreshold, (BSTR)lCommand.ConfidenceText);
#endif
				if	(pCharNdx == 0)
				{
					lCommands->Add (_bstr_t("My Second Command"), _bstr_t("command test+ two"), TRUE, TRUE/*, FALSE*/, &lCommandId);
					lCommand = lCommands.Command [lCommandId];
					lCommand.ConfidenceThreshold = 50;
					lCommand.ConfidenceText = _bstr_t("I thought I heard...");
#ifdef	_LOG_COMMANDS
					LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lCommand.ConfidenceThreshold, (BSTR)lCommand.ConfidenceText);
#endif
					lCommands->Add (_bstr_t("My Third Command"), _bstr_t("command … three"), TRUE, TRUE/*, FALSE*/, &lCommandId);
					lCommand = lCommands.Command [lCommandId];
#ifdef	_LOG_COMMANDS
					LogMessage (_LOG_COMMANDS, _T("[%d] Command [%d] added [%d %ls]"), pCharNdx, lCommandId, lCommand.ConfidenceThreshold, (BSTR)lCommand.ConfidenceText);
#endif
				}
			}

			if	(mCharacter[pCharNdx] != NULL)
			{
				CSize	lCharSize;

				if	(
						(SUCCEEDED (mCharacter[pCharNdx]->GetSize (&lCharSize.cx, &lCharSize.cy)))
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
					mCharacter[pCharNdx]->SetSize (lCharSize.cx, lCharSize.cy);
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
			IDaSvrTTSEnginePtr	lTTSEngine;
			IDaSvrTTSEnginesPtr	lTTSEngines;
			long				lNdx;

			lTTSEngine = mCharacter[pCharNdx].TTSEngine;
//			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[pCharNdx]->get_TTSEngine (TRUE, &lTTSEngine))
//			LogComErr (_LOG_CHAR_CALLS_EX, mServer->GetCharacterTTSEngine (_variant_t(mCharacterPath[pCharNdx]), &lTTSEngine));
			if	(lTTSEngine)
			{
				long	lTTSSpeed = 0;
				short	lTTSPitch = 0;

				mCharacter[pCharNdx]->GetTTSSpeed (&lTTSSpeed);
				mCharacter[pCharNdx]->GetTTSPitch (&lTTSPitch);
				LogMessage (LogNormal, _T("Character [%d] Default TTSModeID [%s] TTSSpeed [%d] TTSPitch [%hd]"), mCharacterId[pCharNdx], (BSTR)lTTSEngine.TTSModeID, lTTSSpeed, lTTSPitch);
			}

			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[pCharNdx]->FindTTSEngines (0, &lTTSEngines));
//			LogComErr (_LOG_CHAR_CALLS_EX, mServer->FindCharacterTTSEngines (_variant_t(mCharacterPath[pCharNdx]), 0x040C, &lTTSEngines));
//			LogComErr (_LOG_CHAR_CALLS_EX, mServer->FindTTSEngines (0x040C, SpeechGender_Female, &lTTSEngines));
			if	(lTTSEngines)
			{
				LogMessage (LogNormal, _T("Character [%d] TTSEngines [%d]"), mCharacterId[pCharNdx], lTTSEngines.Count);
				for	(lNdx = 0; lNdx < lTTSEngines.Count; lNdx++)
				{
					lTTSEngine =  lTTSEngines.Item [lNdx];
					LogMessage (LogNormal, _T("  TTSEngine [%ls] [%ls]"), (BSTR)lTTSEngine.TTSModeID, (BSTR)lTTSEngine.DisplayName);
				}
			}
		}

		if	(mCharacter[pCharNdx] != NULL)
		{
			IDaSvrSREnginePtr	lSREngine;
			IDaSvrSREnginesPtr	lSREngines;
			long				lNdx;

			lSREngine = mCharacter[pCharNdx].SREngine;
//			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[pCharNdx]->get_SREngine (TRUE, &lSREngine));
//			LogComErr (_LOG_CHAR_CALLS_EX, mServer->GetCharacterSREngine (_variant_t(mCharacterPath[pCharNdx]), &lSREngine));
			if	(lSREngine)
			{
				LogMessage (LogNormal, _T("Character [%d] Default SRModeID [%s]"), mCharacterId[pCharNdx], (BSTR)lSREngine.SRModeID);
			}

			LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[pCharNdx]->FindSREngines (0, &lSREngines));
//			LogComErr (_LOG_CHAR_CALLS_EX, mServer->FindCharacterSREngines (_variant_t(mCharacterPath[pCharNdx]), 0x0809, &lSREngines));
//			LogComErr (_LOG_CHAR_CALLS_EX, mServer->FindSREngines (0x0809, &lSREngines));
			if	(lSREngines)
			{
				LogMessage (LogNormal, _T("Character [%d] SREngines [%d]"), mCharacterId[pCharNdx], lSREngines.Count);
				for	(lNdx = 0; lNdx < lSREngines.Count; lNdx++)
				{
					lSREngine = lSREngines.Item [lNdx];
					LogMessage (LogNormal, _T("  SREngine [%ls] [%ls]"), (BSTR)lSREngine.SRModeID, (BSTR)lSREngine.DisplayName);
				}
			}
		}

		if	(mCharacter[pCharNdx] != NULL)
		{
			mCharacter[pCharNdx].Style |= CharacterStyle_IdleEnabled;
			lResult = mCharacter[pCharNdx]->Show (FALSE, &lReqID);
			if	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Show [%d]"), mCharacterId[pCharNdx], lReqID)))
			{
				mActiveChar = mCharacterId[pCharNdx];
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
		&&	(mCharacter[pCharNdx].Visible)
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
		IDaSvrBalloon2Ptr	lBalloon = mCharacter[pCharNdx].Balloon;
		CString				lTTSModeStr;
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
			mBalloonAutoHide.SetCheck ((lBalloon.Style & BalloonStyle_AutoHide) != 0);
			mBalloonAutoPace.SetCheck ((lBalloon.Style & BalloonStyle_AutoPace) != 0);
			mBalloonAutoSize.SetCheck ((lBalloon.Style & BalloonStyle_SizeToText) != 0);
			mBalloonPartialLines.SetCheck ((lBalloon.Style & BalloonStyle_ShowPartialLines) != 0);
			mBalloonVisible.SetCheck (lBalloon.Visible ? TRUE : FALSE);
			mBalloonPartialLines.EnableWindow ((lBalloon.Style & BalloonStyle_SizeToText) == 0);
		}
		else
		{
			mBalloonAutoHide.SetCheck (FALSE);
			mBalloonAutoPace.SetCheck (FALSE);
			mBalloonAutoSize.SetCheck (FALSE);
			mBalloonPartialLines.SetCheck (FALSE);
			mBalloonVisible.SetCheck (FALSE);
			mBalloonPartialLines.EnableWindow (TRUE);
		}

		mTTSModes.EnableWindow (TRUE);
		lTTSModeStr = (BSTR)mCharacter[pCharNdx].TTSModeID;
		LogMessage (LogNormal, _T("Character [%d] TTSModeID [%s] [%d]"), mCharacterId[pCharNdx], lTTSModeStr, FindTTSModeID (lTTSModeStr));

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
			int	lModeNdx = FindTTSModeID (lTTSModeStr);

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
		lSRModeStr = (BSTR)mCharacter[pCharNdx].SRModeID;
		LogMessage (LogNormal, _T("Character [%d] SRModeID [%s] [%d]"), mCharacterId[pCharNdx], lSRModeStr, FindSRModeID (lSRModeStr));

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
			int	lModeNdx = FindSRModeID (lSRModeStr);

			mSRModes.SetCurSel (lModeNdx);
			if	(lModeNdx < 0)
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
		mBalloonPartialLines.EnableWindow (FALSE);
		mBalloonPartialLines.SetCheck (FALSE);
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
		mTTSModes.ResetContent ();
		mTTSModeAdded = -1;
		mTTSEngines = mServer.TTSEngines;
	
		if	(mTTSEngines)
		{
			IDaSvrTTSEnginePtr	lTTSEngine;
			long				lNdx;

#ifdef	_LOG_TTS_MODES
			if	(LogIsActive (_LOG_TTS_MODES))
			{
				LogMessage (_LOG_TTS_MODES, _T("TTSEngines [%d]"), mTTSEngines.Count);
			}
#endif
			for	(lNdx = 0; lNdx < mTTSEngines.Count; lNdx++)
			{
				CString	lModeName;
				CString	lGenderName;

				lTTSEngine = mTTSEngines.Item [lNdx];
				lGenderName = (lTTSEngine.Gender==SpeechGender_Male)?_T("Male"):(lTTSEngine.Gender==SpeechGender_Female)?_T("Female"):_T("Neutral");
				lModeName.Format (_T("%-30ls (%s) %ls"), (BSTR)lTTSEngine.DisplayName, lGenderName, (BSTR)lTTSEngine.LanguageName);
				mTTSModes.AddString (lModeName);

#ifdef	_LOG_TTS_MODES
				if	(LogIsActive (_LOG_TTS_MODES))
				{
					short	lVersionMajor;
					short	lVersionMinor;

					lTTSEngine->GetVersion (&lVersionMajor, &lVersionMinor);
					LogMessage (_LOG_TTS_MODES, _T("  TTSEngine [%2d] [%ls] [%ls] [%hd.%hd] [%s] [%4.4X (%ls)] [%ls]"), lNdx, (BSTR)lTTSEngine.TTSModeID, (BSTR)lTTSEngine.DisplayName, lVersionMajor, lVersionMinor, lGenderName, lTTSEngine.LanguageID, (BSTR)lTTSEngine.LanguageName, (BSTR)lTTSEngine.Manufacturer);
				}
#endif
			}
		}
		mTTSModes.SetCurSel (-1);
	}
}

void CSpeechTestDlg::ShowTTSStatus ()
{
	if	(mTTSStatus.m_hWnd)
	{
		IDaSvrAudioOutputPtr	lAudioOutput (mServer);
		long					lTTSStatus;
		CString					lStatusStr;

		if	(
				(lAudioOutput != NULL)
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, lAudioOutput->GetStatus (&(lTTSStatus=0)))))
			)
		{
			lStatusStr.Format (_T("%d"), lTTSStatus);

			switch (lTTSStatus)
			{
				case AudioStatus_Available:				lStatusStr += _T(" Available"); break;
				case AudioStatus_Disabled:				lStatusStr += _T(" Disabled"); break;
				case AUDIO_STATUS_CANTOPENAUDIO:		lStatusStr += _T(" CANTOPENAUDIO"); break;
				case AudioStatus_UserSpeaking:			lStatusStr += _T(" UserSpeaking"); break;
				case AudioStatus_CharacterSpeaking:		lStatusStr += _T(" CharacterSpeaking"); break;
				case AudioStatus_CharacterListening:	lStatusStr += _T(" CharacterListening"); break;
				case AudioStatus_Error:					lStatusStr += _T(" Error"); break;
			}
		}
		AfxSetWindowText (mTTSStatus, lStatusStr);
	}
}

/////////////////////////////////////////////////////////////////////////////

int CSpeechTestDlg::FindTTSModeID (LPCTSTR pTTSModeID)
{
	CString				lFindModeID (pTTSModeID);
	IDaSvrTTSEnginePtr	lTTSEngine;
	long				lNdx;

	if	(mTTSEngines != NULL)
	{
		for	(lNdx = 0; lNdx < mTTSEngines.Count; lNdx++)
		{
			lTTSEngine = mTTSEngines.Item [lNdx];
			if	(
					(lFindModeID.CompareNoCase (CString ((BSTR)lTTSEngine.TTSModeID)) == 0)
				||	(lFindModeID.CompareNoCase (CString ((BSTR)lTTSEngine.DisplayName)) == 0)
				)
			{
				return lNdx;
			}
		}
	}
	return -1;
}

CString CSpeechTestDlg::GetTTSModeID (INT_PTR pTTSModeNdx)
{
	IDaSvrTTSEnginePtr	lTTSEngine;

	if	(mTTSEngines != NULL)
	{
		lTTSEngine = mTTSEngines.Item [(long)pTTSModeNdx];
		if	(lTTSEngine != NULL)
		{
			return CString ((BSTR)lTTSEngine.TTSModeID);
		}
	}
	return CString();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::ShowSRModes ()
{
	if	(mSRModes.m_hWnd)
	{
		mSRModes.ResetContent ();
		mSRModeAdded = -1;
		mSREngines = mServer.SREngines;

		if	(mSREngines)
		{
			IDaSvrSREnginePtr	lSREngine;
			long				lNdx;

#ifdef	_LOG_SR_MODES
			if	(LogIsActive (_LOG_SR_MODES))
			{
				LogMessage (_LOG_SR_MODES, _T("SREngines [%d]"), mSREngines.Count);
			}
#endif

			for	(lNdx = 0; lNdx < mSREngines.Count; lNdx++)
			{
				lSREngine = mSREngines.Item [lNdx];
				mSRModes.AddString (CString ((BSTR)lSREngine.DisplayName));

#ifdef	_LOG_SR_MODES
				if	(LogIsActive (_LOG_SR_MODES))
				{
					short	lVersionMajor;
					short	lVersionMinor;

					lSREngine->GetVersion (&lVersionMajor, &lVersionMinor);
					LogMessage (_LOG_SR_MODES, _T("  SREngine [%2d] [%ls] [%ls] [%hd.%hd] [%4.4X (%ls)] [%ls]"), lNdx, (BSTR)lSREngine.SRModeID, (BSTR)lSREngine.DisplayName, lVersionMajor, lVersionMinor, lSREngine.LanguageID, (BSTR)lSREngine.LanguageName, (BSTR)lSREngine.Manufacturer);
		}
#endif
			}
		}

		mSRModes.SetCurSel (-1);
	}
}

void CSpeechTestDlg::ShowSRStatus ()
{
	if	(mSRStatus.m_hWnd)
	{
		INT_PTR					lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
		ListeningStatusType		lSRStatus;
		CString					lStatusStr;
		IDaSvrSpeechInputPtr	lSpeechInput (mServer);
		long					lEnabled = 0;
		long					lListeningTip = 0;
		tBstrPtr				lHotKey;

		lSpeechInput->GetEnabled (&lEnabled);
		lSpeechInput->GetListeningTip (&lListeningTip);
		lSpeechInput->GetHotKey (lHotKey.Free ());

		if	(
				(lCharNdx >= 0)
			&&	(mCharacter[lCharNdx])
			&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS_EX, mCharacter[lCharNdx]->get_ListeningStatus (&(lSRStatus=(ListeningStatusType)-1)))))
			)
		{
			lStatusStr.Format (_T("%d"), lSRStatus);

			switch (lSRStatus)
			{
				case ListeningStatus_Available:			lStatusStr += _T(" Available"); break;
				case LISTEN_STATUS_NOAUDIO:				lStatusStr += _T(" NOAUDIO"); break;
				case ListeningStatus_CharacterInactive:	lStatusStr += _T(" CharacterInactive"); break;
				case LISTEN_STATUS_CANTOPENAUDIO:		lStatusStr += _T(" CANTOPENAUDIO"); break;
				case ListeningStatus_InitializeFailed:	lStatusStr += _T(" InitializeFailed"); break;
				case ListeningStatus_InputDisabled:		lStatusStr += _T(" InputDisabled"); break;
				case ListeningStatus_Error:				lStatusStr += _T(" Error"); break;
			}

			if	(lEnabled)
			{
				lStatusStr += _T(" (Enabled)");
			}
			else
			{
				lStatusStr += _T(" (Disabled)");
			}
			if	(lListeningTip)
			{
				lStatusStr += _T(" (Tip)");
			}
			if	(!CString (lHotKey).IsEmpty ())
			{
				lStatusStr += _T(" (") + CString (lHotKey) + _T(")");
			}
		}

		AfxSetWindowText (mSRStatus, lStatusStr);
	}
}

/////////////////////////////////////////////////////////////////////////////

int CSpeechTestDlg::FindSRModeID (LPCTSTR pSRModeID)
{
	CString				lFindModeID (pSRModeID);
	IDaSvrSREnginePtr	lSREngine;
	tBstrPtr			lDisplayName;
	tBstrPtr			lSRModeID;
	long				lNdx;

	if	(mSREngines != NULL)
	{
		for	(lNdx = 0; (mSREngines->get_Item (lNdx, &lSREngine) == S_OK); lNdx++)
		{
			if	(
					(
						(SUCCEEDED (lSREngine->get_SRModeID (lSRModeID.Free())))
					&&	(lFindModeID.CompareNoCase (CString (lSRModeID)) == 0)
					)
				||	(
						(SUCCEEDED (lSREngine->get_DisplayName (lDisplayName.Free())))
					&&	(lFindModeID.CompareNoCase (CString (lDisplayName)) == 0)
					)
				)
			{
				return lNdx;
			}
		}
	}
	return -1;
}

CString CSpeechTestDlg::GetSRModeID (INT_PTR pSRModeNdx)
{
	IDaSvrSREnginePtr	lSREngine;

	if	(mSREngines != NULL)
	{
		lSREngine = mSREngines.Item [(long)pSRModeNdx];
		if	(lSREngine != NULL)
		{
			return CString ((BSTR)lSREngine.SRModeID);
		}
	}
	return CString();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileCharacter = _T("Character");
static LPCTSTR	sProfileShowChar = _T("Show");
static LPCTSTR	sProfileFormatted = _T("Formatted");
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

		if	(mCharacterPath[0].IsEmpty ())
		{
			lFoundNdx = 0;
		}
		else
		{
			lFindInfo.flags = LVFI_STRING;
			lFindInfo.psz = mCharacterPath[0];
			lFindInfo.vkDirection = VK_NEXT;
			lFoundNdx = ListView_FindItem (mCharacterList.m_hWnd, -1, &lFindInfo);
		}
		if	(lFoundNdx >= 0)
		{
			mCharacterList.SetItemState (lFoundNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}

		if	(lApp->GetProfileInt (sProfileKey, sProfileShowChar, mShowCharButton.GetCheck()))
		{
			ShowAgentCharacter (0);
		}
	}

	mFormattedButton.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileFormatted, mFormattedButton.GetCheck()) ? TRUE : FALSE);

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
	lApp->WriteProfileInt (sProfileKey, sProfileFormatted, mFormattedButton.GetCheck());

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
	CRegKeyEx	lProfileKey;

	if	(lProfileKey.Attach (AfxGetApp()->GetAppRegistryKey()))
	{
		CRegString	lWaveFile (lProfileKey, sProfileWaveFile);
		CRegStrings	lWaveFiles (lProfileKey, sProfileWaveFiles);
		INT_PTR		lNdx;

		mSpeechWave.ResetContent ();
		for	(lNdx = 0; lNdx < lWaveFiles.Value().GetCount(); lNdx++)
		{
			mSpeechWave.AddString (lWaveFiles.Value() [lNdx]);
		}
		mSpeechWave.InsertString (0, _T(""));
		mSpeechWave.SetWindowText (lWaveFile.Value());
	}
}

void CSpeechTestDlg::SaveWaveFiles ()
{
	CRegKeyEx	lProfileKey;

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
	SafeFreeSafePtr (mTTSEngines);
	SafeFreeSafePtr (mSREngines);
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
				LogComErrAnon (_LOG_CHAR_CALLS, mCharacter[0]->Activate (ActiveState_Active), _T("[%d] Activate ActiveState_Active"), mCharacterId[0]);
			}
			else
			{
				LogComErrAnon (_LOG_CHAR_CALLS, mCharacter[0]->Activate (ActiveState_InputActive), _T("[%d] Activate ActiveState_InputActive"), mCharacterId[0]);
			}
		}
		if	(mCharacter[1] != NULL)
		{
			if	(mActiveChar == mCharacterId[1])
			{
				LogComErrAnon (_LOG_CHAR_CALLS, mCharacter[1]->Activate (ActiveState_InputActive), _T("[%d] Activate ActiveState_InputActive"), mCharacterId[1]);
			}
			else
			{
				LogComErrAnon (_LOG_CHAR_CALLS, mCharacter[1]->Activate (ActiveState_Active), _T("[%d] Activate ActiveState_Active"), mCharacterId[1]);
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
			ShowCharacter (0, (lItem > 0) ? mCharacterList.GetItemText (lItem, 0) : _T(""));
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
						(!ShowCharacter (1, (lItem > 0) ? mCharacterList.GetItemText (lItem, 0) : _T("")))
					||	(!ShowAgentCharacter (1))
					)
				&&	(
						(!ShowCharacter (1, mCharacterList.GetItemText (lItem+1, 0)))
					||	(!ShowAgentCharacter (1))
					)
				&&	(lItem > 0)
				)
			{
				ShowCharacter (1, (lItem > 1) ? mCharacterList.GetItemText (lItem-1, 0) : _T(""));
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
		CString					lText;
		IDaSvrFormattedTextPtr	lFormattedText;
		HRESULT					lResult;
		long					lReqID;

		mBalloonTextEdit1.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			if	(mFormattedButton.GetCheck())
			{
				if	(
						(SUCCEEDED (lResult = mCharacter[lCharNdx]->NewFormattedText (&lFormattedText)))
					&&	(SUCCEEDED (lResult = lFormattedText->Parse (_bstr_t(lText))))
					)
				{
#ifdef	_APPEND_FORMATTED
					{
						CString					lText2;
						IDaSvrFormattedTextPtr	lFormattedText2;

						mBalloonTextEdit2.GetWindowText (lText2);
						if	(
								(!lText2.IsEmpty())
							&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mCharacter[lCharNdx]->NewFormattedText (&lFormattedText2), _T("NewFormattedText"))))
							&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lFormattedText2->Parse (_bstr_t(lText2)), _T("Parse"))))
							)
						{
							LogComErr (_LOG_CHAR_CALLS, lFormattedText->Append (lFormattedText2), _T("Append"));
						}
					}
#endif
					lResult = mCharacter[lCharNdx]->ThinkFormatted (lFormattedText, &lReqID);
					LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] ThinkFormatted [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
					if	(LogIsActive (_LOG_CHAR_CALLS))
					{ 
						LogMessage (_LOG_CHAR_CALLS, _T("   Count     [%d] Index [%d] Word [%ls]"), lFormattedText.WordCount, lFormattedText.WordIndex, (BSTR)lFormattedText.DisplayWord[lFormattedText.WordIndex]);
						LogMessage (_LOG_CHAR_CALLS, _T("   Display   [%ls]"), DebugStr((BSTR)lFormattedText.DisplayText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Raw       [%ls]"), DebugStr((BSTR)lFormattedText.RawText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Parsed    [%ls]"), DebugStr((BSTR)lFormattedText.ParsedText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Speech    [%ls]"), DebugStr((BSTR)lFormattedText.SpeechText));
					}
				}
			}
			else
			{
				lResult = mCharacter[lCharNdx]->Think (_bstr_t(lText), &lReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
			}
		}

		mBalloonTextEdit2.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			if	(mFormattedButton.GetCheck())
			{
#ifndef	_APPEND_FORMATTED
				if	(
						(SUCCEEDED (lResult = mCharacter[lCharNdx]->NewFormattedText (&lFormattedText)))
					&&	(SUCCEEDED (lResult = lFormattedText->Parse (_bstr_t(lText))))
					)
				{
					lResult = mCharacter[lCharNdx]->ThinkFormatted (lFormattedText, &lReqID);
					LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] ThinkFormatted [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
					if	(LogIsActive (_LOG_CHAR_CALLS))
					{ 
						LogMessage (_LOG_CHAR_CALLS, _T("   Count [%d] Index [%d] Word [%ls]"), lFormattedText.WordCount, lFormattedText.WordIndex, (BSTR)lFormattedText.DisplayWord[lFormattedText.WordIndex]);
						LogMessage (_LOG_CHAR_CALLS, _T("   Display   [%ls]"), DebugStr((BSTR)lFormattedText.DisplayText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Text      [%ls]"), DebugStr((BSTR)lFormattedText.Text));
						LogMessage (_LOG_CHAR_CALLS, _T("   Formatted [%ls]"), DebugStr((BSTR)lFormattedText.FormattedText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Speech    [%ls]"), DebugStr((BSTR)lFormattedText.SpeechText));
					}
				}
#endif
			}
			else
			{
				lResult = mCharacter[lCharNdx]->Think (_bstr_t(lText), &lReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Think [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
			}
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
		CString					lText;
		CString					lWaveFile = GetWaveFile ();
		IDaSvrFormattedTextPtr	lFormattedText;
		HRESULT					lResult;
		long					lReqID;

		mBalloonTextEdit1.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			if	(mFormattedButton.GetCheck())
			{
				if	(
						(SUCCEEDED (lResult = mCharacter[lCharNdx]->NewFormattedText (&lFormattedText)))
					&&	(SUCCEEDED (lResult = lFormattedText->Parse (_bstr_t(lText))))
					)
				{
#ifdef	_APPEND_FORMATTED
					{
						CString					lText2;
						IDaSvrFormattedTextPtr	lFormattedText2;

						mBalloonTextEdit2.GetWindowText (lText2);
						if	(
								(!lText2.IsEmpty())
							&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, mCharacter[lCharNdx]->NewFormattedText (&lFormattedText2), _T("NewFormattedText"))))
							&&	(SUCCEEDED (LogComErr (_LOG_CHAR_CALLS, lFormattedText2->Parse (_bstr_t(lText2)), _T("Parse"))))
							)
						{
							LogComErr (_LOG_CHAR_CALLS, lFormattedText->Append (lFormattedText2), _T("Append"));
						}
					}
#endif
					lResult = mCharacter[lCharNdx]->SpeakFormatted (lFormattedText, &lReqID);
					LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] SpeakFormatted [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
					if	(LogIsActive (_LOG_CHAR_CALLS))
					{ 
						LogMessage (_LOG_CHAR_CALLS, _T("   Count     [%d] Index [%d] Word [%ls]"), lFormattedText.WordCount, lFormattedText.WordIndex, (BSTR)lFormattedText.DisplayWord[lFormattedText.WordIndex]);
						LogMessage (_LOG_CHAR_CALLS, _T("   Display   [%ls]"), DebugStr((BSTR)lFormattedText.DisplayText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Raw       [%ls]"), DebugStr((BSTR)lFormattedText.RawText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Parsed    [%ls]"), DebugStr((BSTR)lFormattedText.ParsedText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Speech    [%ls]"), DebugStr((BSTR)lFormattedText.SpeechText));
					}
				}
			}
			else
			{
				lResult = mCharacter[lCharNdx]->Speak (_bstr_t(lText), _bstr_t(lWaveFile), &lReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d] [%s] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText), lWaveFile);
			}
		}

		mBalloonTextEdit2.GetWindowText (lText);
		if	(!lText.IsEmpty())
		{
			if	(mFormattedButton.GetCheck())
			{
#ifndef	_APPEND_FORMATTED
				if	(
						(SUCCEEDED (lResult = mCharacter[lCharNdx]->NewFormattedText (&lFormattedText)))
					&&	(SUCCEEDED (lResult = lFormattedText->Parse (_bstr_t(lText))))
					)
				{
					lResult = mCharacter[lCharNdx]->SpeakFormatted (lFormattedText, &lReqID);
					LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] SpeakFormatted [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
					if	(LogIsActive (_LOG_CHAR_CALLS))
					{ 
						LogMessage (_LOG_CHAR_CALLS, _T("   Count [%d] Index [%d] Word [%ls]"), lFormattedText.WordCount, lFormattedText.WordIndex, (BSTR)lFormattedText.DisplayWord[lFormattedText.WordIndex]);
						LogMessage (_LOG_CHAR_CALLS, _T("   Display   [%ls]"), DebugStr((BSTR)lFormattedText.DisplayText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Text      [%ls]"), DebugStr((BSTR)lFormattedText.Text));
						LogMessage (_LOG_CHAR_CALLS, _T("   Formatted [%ls]"), DebugStr((BSTR)lFormattedText.FormattedText));
						LogMessage (_LOG_CHAR_CALLS, _T("   Speech    [%ls]"), DebugStr((BSTR)lFormattedText.SpeechText));
					}
				}
#endif
			}
			else
			{
				lResult = mCharacter[lCharNdx]->Speak (_bstr_t(lText), NULL, &lReqID);
				LogComErr (_LOG_CHAR_CALLS, lResult, _T("[%d] Speak [%d] [%s]"), mCharacterId[lCharNdx], lReqID, DebugStr(lText));
			}
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
		mCharacter[lCharNdx].Balloon.Style ^= BalloonStyle_AutoPace;
		ShowCharacterState ();
	}
}

void CSpeechTestDlg::OnBalloonAutoHide()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		mCharacter[lCharNdx].Balloon.Style ^= BalloonStyle_AutoHide;
		ShowCharacterState ();
	}
}

void CSpeechTestDlg::OnBalloonAutoSize()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		mCharacter[lCharNdx].Balloon.Style ^= BalloonStyle_SizeToText;
		ShowCharacterState ();
	}
}

void CSpeechTestDlg::OnBalloonPartialLines()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		mCharacter[lCharNdx].Balloon.Style ^= BalloonStyle_ShowPartialLines;
		ShowCharacterState ();
	}
}

void CSpeechTestDlg::OnBalloonVisible()
{
	INT_PTR	lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);

	if	(lCharNdx >= 0)
	{
		mCharacter[lCharNdx].Balloon.Visible = !mCharacter[lCharNdx].Balloon.Visible;
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
		HRESULT					lResult;
		tBstrPtr				lSRModeID;
		ListeningStatusType		lSRStatus;

		lResult = mCharacter[lCharNdx]->get_SRModeID (lSRModeID.Free());
		LogComErr (LogAlways, lResult, _T("[%d] get_SRModeID [%ls]"), mCharacterId[lCharNdx], (BSTR)lSRModeID);

		lResult = mCharacter[lCharNdx]->get_ListeningStatus (&(lSRStatus=(ListeningStatusType)-1));
		LogComErr (LogAlways, lResult, _T("[%d] get_ListeningStatus [%d]"), mCharacterId[lCharNdx], lSRStatus);

		lResult = mCharacter[lCharNdx]->Listen (mListenButton.GetCheck() ? TRUE : FALSE);
		LogComErr (LogAlways, lResult, _T("[%d] Listen [%d]"), mCharacterId[lCharNdx], mListenButton.GetCheck() ? TRUE : FALSE);

		lResult = mCharacter[lCharNdx]->get_ListeningStatus (&(lSRStatus=(ListeningStatusType)-1));
		LogComErr (LogAlways, lResult, _T("[%d] get_ListeningStatus [%d]"), mCharacterId[lCharNdx], lSRStatus);
	}
	else
	{
		MessageBeep (0);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnSelEndOkTTSModes()
{
	INT_PTR		lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
	int			lSelMode = mTTSModes.GetCurSel ();
	CString		lTTSModeID;

	if	(
			(lSelMode >= 0)
		&&	(lSelMode != mTTSModeAdded)
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
		lTTSModeID = GetTTSModeID (lSelMode);
		if	(!lTTSModeID.IsEmpty ())
		{
			LogComErr (LogAlways, mCharacter [lCharNdx]->put_TTSModeID (_bstr_t(lTTSModeID)), _T("SetTTSModeID [%s]"), lTTSModeID);
			ShowCharacterState (lCharNdx);
		}
			}
}

void CSpeechTestDlg::OnSelEndOkSRModes()
{
	INT_PTR		lCharNdx = (mActiveChar == mCharacterId[0] ? 0 : mActiveChar == mCharacterId[1] ? 1 : -1);
	int			lSelMode = mSRModes.GetCurSel ();
	CString		lSRModeID;

	if	(
			(lSelMode >= 0)
		&&	(lSelMode != mSRModeAdded)
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
		lSRModeID = GetSRModeID (lSelMode);
		if	(!lSRModeID.IsEmpty ())
		{
			LogComErr (LogAlways, mCharacter [lCharNdx]->put_SRModeID (_bstr_t(lSRModeID)), _T("SetSRModeID [%s]"), lSRModeID);
			ShowCharacterState (lCharNdx);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSpeechTestDlg::OnAgentProps()
{
	IDaSvrPropertySheet2Ptr	lPropSheet (mServer);
	VARIANT_BOOL			lVisible = VARIANT_FALSE;
	HRESULT					lResult;

	if	(lPropSheet != NULL)
	{
		AllowSetForegroundWindow (ASFW_ANY);
		LogComErr (_LOG_AGENT_CALLS, lPropSheet->get_Visible (&lVisible), _T("PropertySheet->get_Visible"));
		lResult = lPropSheet->put_Visible (lVisible?VARIANT_FALSE:VARIANT_TRUE);
		LogComErr (_LOG_AGENT_CALLS, lResult, _T("PropertySheet->put_Visible"));
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
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfoCount"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfoCount (pctinfo);
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	METHOD_PROLOGUE_EX_(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSpeechTestDlg::XDaSvrNotifySink::GetTypeInfo"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetTypeInfo (iTInfo, lcid, ppTInfo);
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	METHOD_PROLOGUE_EX_(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSpeechTestDlg::XDaSvrNotifySink::GetIDsOfNames"), pThis, pThis->m_dwRef);
#endif
	return pThis->GetIDispatch(FALSE)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	METHOD_PROLOGUE_EX(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CSpeechTestDlg::XDaSvrNotifySink::Invoke [%8.8X (%u)]"), pThis, pThis->m_dwRef, dispIdMember, dispIdMember);
#endif
	return pThis->GetIDispatch(FALSE)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Command [%d]"), pThis->mCharacterId, pThis->m_dwRef, CommandID);
#endif
#ifdef	_LOG_COMMANDS
	LogMessage (_LOG_COMMANDS, _T("Command [%d]"), CommandID);
	if	(UserInput != NULL)
	{
		long		lItemCount = 0;
		long		lItemNdx;
		_variant_t	lItemIds;
		_variant_t	lItemConfidences;
		_variant_t	lItemTexts;

		LogComErr (_LOG_COMMANDS, UserInput->get_Count (&lItemCount));
		LogMessage (_LOG_COMMANDS, _T("  UserInput [%d]"), lItemCount);

		for	(lItemNdx = 0; lItemNdx < lItemCount; lItemNdx++)
		{
			long		lItemId = -1;
			long		lItemConfidence = -1;
			tBstrPtr	lItemText;

			LogComErr (_LOG_COMMANDS, UserInput->get_ItemCommandID (lItemNdx, &lItemId));
			LogComErr (_LOG_COMMANDS, UserInput->get_ItemConfidence (lItemNdx, &lItemConfidence));
			LogComErr (_LOG_COMMANDS, UserInput->get_ItemText (lItemNdx, lItemText.Free()));
			LogMessage (_LOG_COMMANDS, _T("    Item [%d] Id [%d] Text [%ls] Confidence [%d]"), lItemNdx, lItemId, (BSTR)lItemText, lItemConfidence);
		}

		if	(SUCCEEDED (LogComErr (_LOG_COMMANDS, UserInput->GetAllItemData (&lItemIds, &lItemConfidences, &lItemTexts))))
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

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::ActivateInputState (long CharacterID, long Activated)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::ActivateInputState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Activated);
#endif
	if	(Activated)
	{
		if	(pThis->mActiveChar != CharacterID)
		{
			pThis->mActiveChar = CharacterID;
			pThis->ShowCharacterState ();
		}
	}
	else
	{
		if	(pThis->mActiveChar == CharacterID)
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

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::VisibleState [%d] [%d] cause [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Visible, Cause);
#endif
	pThis->ShowCharacterState ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Click (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Click [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DblClick (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DblClick [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DragStart (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DragStart [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DragComplete (long CharacterID, short Keys, long x, long y)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Keys, x, y);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::RequestStart (long RequestID)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::RequestStart [%d]"), pThis->mCharacterId, pThis->m_dwRef, RequestID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::RequestComplete (long RequestID, long Result)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::RequestComplete [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, RequestID, Result);
#endif
	if	(RequestID == pThis->mLoadReqID)
	{
		pThis->mLoadReqID = 0;
		pThis->LoadedAgentCharacter (-1);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::BookMark (long BookMarkID)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::BookMark [%d]"), pThis->mCharacterId, pThis->m_dwRef, BookMarkID);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Idle (long CharacterID, long Start)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Idle [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Start);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Move (long CharacterID, long x, long y, long Cause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Move [%d] [%d %d] cause [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, x, y, Cause);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::Size (long CharacterID, long Width, long Height)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::Size [%d] [%d %d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Width, Height);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::BalloonVisibleState [%d] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Visible);
#endif
	pThis->mBalloonVisible.SetCheck (Visible ? TRUE : FALSE);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::HelpComplete"), pThis->mCharacterId, pThis->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	CString	lCauseStr;
	switch (Cause)
	{
		case ListenComplete_ProgramDisabled:			lCauseStr = _T("ProgramDisabled"); break;
		case ListenComplete_ProgramTimedOut:			lCauseStr = _T("ProgramTimedOut"); break;
		case ListenComplete_UserTimedOut:				lCauseStr = _T("UserTimedOut"); break;
		case ListenComplete_UserReleasedKey:			lCauseStr = _T("UserReleasedKey"); break;
		case ListenComplete_UserSpeechEnded:			lCauseStr = _T("UserSpeechEnded"); break;
		case ListenComplete_CharacterClientDeactivated:	lCauseStr = _T("CharacterClientDeactivated"); break;
		case ListenComplete_DefaultCharacterChanged:	lCauseStr = _T("DefaultCharacterChanged"); break;
		case ListenComplete_UserDisabled:				lCauseStr = _T("UserDisabled"); break;
		default:										lCauseStr.Format (_T("%d"), Cause); break;
	}
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::ListeningState [%d] [%d] cause [%d] [%s]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Listening, Cause, lCauseStr);
#endif
	if	(!Listening)
	{
		pThis->mListenButton.SetCheck (FALSE);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::DefaultCharacterChange (BSTR CharGUID)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::DefaultCharacterChange [%ls]"), pThis->mCharacterId, pThis->m_dwRef, CharGUID);
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
		long	Speed = 0;
		short	lPitch = 0;

		pThis->mCharacter[0]->GetTTSSpeed (&Speed);
		pThis->mCharacter[0]->GetTTSPitch (&lPitch);
		LogMessage (LogNormal, _T("TTSSpeed [%d] TTSPitch [%hd]"), Speed, lPitch);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::ActiveClientChange (long CharacterID, long Status)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::ActiveClientChange [%d] [%8.8X]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, Status);
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::SpeechStart (long CharacterID, IDaSvrFormattedText *FormattedText)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::SpeechStart [%d] [%p]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, FormattedText);
	IDaSvrFormattedTextPtr	lText (FormattedText);
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Count     [%d] Index [%d] Word [%ls]"), pThis->mCharacterId, pThis->m_dwRef, lText.WordCount, lText.WordIndex, (BSTR)lText.DisplayWord[lText.WordIndex]);
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Display   [%ls]"), pThis->mCharacterId, pThis->m_dwRef, DebugStr((BSTR)lText.DisplayText));
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Raw       [%ls]"), pThis->mCharacterId, pThis->m_dwRef, DebugStr((BSTR)lText.RawText));
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Parsed    [%ls]"), pThis->mCharacterId, pThis->m_dwRef, DebugStr((BSTR)lText.ParsedText));
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Speech    [%ls]"), pThis->mCharacterId, pThis->m_dwRef, DebugStr((BSTR)lText.SpeechText));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::SpeechEnd (long CharacterID, IDaSvrFormattedText *FormattedText, VARIANT_BOOL Stopped)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::SpeechEnd   [%d] [%p] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, FormattedText, Stopped);
	IDaSvrFormattedTextPtr	lText (FormattedText);
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Count     [%d] Index [%d] Word [%ls]"), pThis->mCharacterId, pThis->m_dwRef, lText.WordCount, lText.WordIndex, (BSTR)lText.DisplayWord[lText.WordIndex]);
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Display   [%ls]"), pThis->mCharacterId, pThis->m_dwRef, DebugStr((BSTR)lText.DisplayText));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSpeechTestDlg::XDaSvrNotifySink::SpeechWord (long CharacterID, IDaSvrFormattedText *FormattedText, long WordIndex)
{
	METHOD_PROLOGUE(CSpeechTestDlg, DaSvrNotifySink)
#ifdef	_LOG_NOTIFY
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u] CSpeechTestDlg::XDaSvrNotifySink::SpeechWord  [%d] [%p] [%d]"), pThis->mCharacterId, pThis->m_dwRef, CharacterID, FormattedText, WordIndex);
	IDaSvrFormattedTextPtr	lText (FormattedText);
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Count     [%d] Index [%d] Word [%ls]"), pThis->mCharacterId, pThis->m_dwRef, lText.WordCount, lText.WordIndex, (BSTR)lText.DisplayWord[lText.WordIndex]);
	LogMessage (_LOG_NOTIFY, _T("[%d] [%u]   Display   [%ls]"), pThis->mCharacterId, pThis->m_dwRef, DebugStr((BSTR)lText.DisplayText));
#endif
	return S_OK;
}
