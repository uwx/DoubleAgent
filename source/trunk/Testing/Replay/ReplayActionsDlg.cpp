#include "StdAfx.h"
#include <shlwapi.h>
#include "ReplayActions.h"
#include "ReplayActionsDlg.h"
#include "DaCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CReplayActionsDlg::CReplayActionsDlg (CWnd* pParent)
:	CDialog(CReplayActionsDlg::IDD, pParent),
	mCharacterId (0),
	mPlayTimer (0)
{
	//{{AFX_DATA_INIT(CReplayActionsDlg)
	//}}AFX_DATA_INIT
}

CReplayActionsDlg::~CReplayActionsDlg ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CReplayActionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplayActionsDlg)
	DDX_Control(pDX, IDC_STOP_REPLAY, mStopButton);
	DDX_Control(pDX, IDC_START_REPLAY, mStartButton);
	DDX_Control(pDX, IDC_REPEAT_REPLAY, mRepeatButton);
	DDX_Control(pDX, IDC_OPEN_FILE, mOpenButton);
	DDX_Control(pDX, IDC_ACTION_LIST, mActionList);
	//}}AFX_DATA_MAP
}

BOOL CReplayActionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LogComErr (LogNormal, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_SERVER, __uuidof(IDaServer), (void**)&mServer), _T("CoCreateInstance"));

	LoadConfig ();
	if	(
			(!mDefaultFileName.IsEmpty ())
		&&	(mActionFile = new CActionFile)
		&&	(!mActionFile->Open (mDefaultFileName))
		)
	{
		mActionFile = NULL;
	}
	ShowState ();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CReplayActionsDlg::ShowState ()
{
	CString	lWinTitle;

	lWinTitle.LoadString (AFX_IDS_APP_TITLE);

	if	(mActionFile)
	{
		lWinTitle.Format (_T("%s - %s"), CString((LPCTSTR)lWinTitle), mActionFile->GetFileTitle());
		mOpenButton.EnableWindow (FALSE);
		mStartButton.EnableWindow (mPlayTimer == 0);
		mStopButton.EnableWindow (mPlayTimer != 0);
	}
	else
	{
		mOpenButton.EnableWindow (TRUE);
		mStartButton.EnableWindow (FALSE);
		mStopButton.EnableWindow (FALSE);
	}

	SetWindowText (lWinTitle);
}

/////////////////////////////////////////////////////////////////////////////

bool CReplayActionsDlg::Start ()
{
	bool			lRet = false;
	CActionLine *	lLine;

	if	(
			(mActionFile)
		&&	(lLine = mActionFile->PeekNextLine ())
		)
	{
		mActionList.DeleteAllItems ();
		mStartTimeOffset = NULL;
		mReqIdMap.RemoveAll ();
		lRet = true;
	}
	return lRet;
}

bool CReplayActionsDlg::Restart ()
{
	bool	lRet = false;

	if	(mActionFile)
	{
		tPtr <CActionFile>	lActionFile;

		if	(
				(lActionFile = new CActionFile)
			&&	(lActionFile->Open (mActionFile->GetFilePath ()))
			)
		{
			mActionFile = lActionFile.Detach ();
			if	(mRepeatButton.GetCheck ())
			{
				lRet = Start ();
			}
		}
		else
		{
			mActionFile = NULL;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CReplayActionsDlg::LoadCharacter (LPCTSTR pCharacterName)
{
	HRESULT	lResult = E_UNEXPECTED;
	CString	lCharacterName (pCharacterName);
	int		lCharPos;
	long	lReqID;

	UnloadCharacter ();

	PathStripPath (lCharacterName.GetBuffer (lCharacterName.GetLength ()));
	PathRemoveExtension (lCharacterName.GetBuffer (lCharacterName.GetLength ()));
	lCharacterName.ReleaseBuffer ();
	lCharPos = lCharacterName.Find (_T('('));
	if	(lCharPos > 0)
	{
		lCharacterName = lCharacterName.Left (lCharPos);
	}
	PathAddExtension (lCharacterName.GetBuffer (MAX_PATH), _T(".acs"));
	lCharacterName.ReleaseBuffer ();

	if	(
			(mServer != NULL)
		&&	(SUCCEEDED (lResult = LogComErr (LogNormal, mServer->Load (_variant_t(lCharacterName), &mCharacterId, &lReqID), _T("Load [%s]"), lCharacterName)))
		&&	(SUCCEEDED (lResult = LogComErr (LogNormal, mServer->GetCharacterEx (mCharacterId, &mCharacter), _T("GetCharacterEx"))))
		)
	{
		LogComErr (LogNormal, mCharacter->SetIdleOn (FALSE));
	}
	else
	{
		UnloadCharacter ();
	}
	return lResult;
}

HRESULT CReplayActionsDlg::UnloadCharacter ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(mCharacter != NULL)
	{
		mCharacter = NULL;
	}
	if	(
			(mServer != NULL)
		&&	(mCharacterId != 0)
		)
	{
		lResult = LogComErr (LogNormal, mServer->Unload (mCharacterId), _T("Unload [%d]"), mCharacterId);
	}
	mCharacterId = 0;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CReplayActionsDlg::RunActions ()
{
	bool	lRet = false;

	if	(mActionFile)
	{
		CActionLine *		lNextLine = NULL;
		tPtr <CActionLine>	lLine;

		while	(lNextLine = mActionFile->PeekNextLine ())
		{
			CFileTime	lCurrTime = CFileTime::GetCurrentTime();
			CFileTime	lPlayTime;

			lRet = true;

			if	(mStartTimeOffset)
			{
				lPlayTime = lNextLine->mTime + *mStartTimeOffset;
				if	(
						(lPlayTime > lCurrTime)
					&&	((lPlayTime - lCurrTime).GetTimeSpan () / CFileTime::Millisecond) >= 120
					)
				{
					break;
				}
			}

			if	(lLine = mActionFile->GetNextLine ())
			{
				if	(mStartTimeOffset)
				{
					lPlayTime = lLine->mTime + *mStartTimeOffset;
					if	(
							(lPlayTime > lCurrTime)
						&&	(((lPlayTime - lCurrTime).GetTimeSpan() / CFileTime::Millisecond) > 10)
						)
					{
						Sleep ((DWORD) ((lPlayTime - lCurrTime).GetTimeSpan() / CFileTime::Millisecond));
					}
				}
				else
				{
					mStartTimeOffset = new CFileTimeSpan (lCurrTime - lLine->mTime);
				}

				RunAction (lLine);
				mActionList.ShowAction (lLine);
			}
		}
	}
	return	lRet;
}

HRESULT CReplayActionsDlg::RunAction (CActionLine * pAction)
{
	HRESULT	lResult = E_UNEXPECTED;
	long	lReqID = 0;

	if	(pAction->mAction.CompareNoCase (_T("Load")) == 0)
	{
		lResult = LoadCharacter (pAction->mValues [1]);
	}
	else
	if	(pAction->mAction.CompareNoCase (_T("Unload")) == 0)
	{
		lResult = UnloadCharacter ();
	}
	else
	{
		if	(mCharacter == NULL)
		{
			LoadCharacter (mActionFile->GetFileName ());
		}

		if	(mCharacter == NULL)
		{
			lResult = E_FAIL;
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetPosition")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetPosition (_ttol(pAction->mValues[0]), _ttol(pAction->mValues[1])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetSize")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetSize (_ttol(pAction->mValues[0]), _ttol(pAction->mValues[1])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetName")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetName (_bstr_t(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetDescription")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetDescription (_bstr_t(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetLanguageID")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetLanguageID (_ttol(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetIdleOn")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetIdleOn (_ttol(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetSoundEffectsOn")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetSoundEffectsOn (_ttol(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Show")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 2));
			lResult = LogComErr (LogNormal, mCharacter->Show (_ttol(pAction->mValues[0]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[1]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Hide")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 2));
			lResult = LogComErr (LogNormal, mCharacter->Hide (_ttol(pAction->mValues[0]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[1]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Prepare")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 4));
			lResult = LogComErr (LogNormal, mCharacter->Prepare (_ttol(pAction->mValues[0]), _bstr_t(pAction->mValues[1]), _ttol(pAction->mValues[2]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[3]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Play")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 2));
			lResult = LogComErr (LogNormal, mCharacter->Play (_bstr_t(pAction->mValues[0]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[1]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Stop")) == 0)
		{
			long	lReqIdFrom;
			long	lReqIdTo;

			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
			lReqIdFrom = _ttol(pAction->mValues[0]);
			if	(
					(mReqIdMap.Lookup (lReqIdFrom, lReqIdTo))
				&&	(lReqIdTo > 0)
				)
			{
				lResult = LogComErr (LogNormal, mCharacter->Stop (lReqIdTo));
			}
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("StopAll")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->StopAll (_tcstoul(pAction->mValues[0], NULL, 16)));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("MoveTo")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 4));
			lResult = LogComErr (LogNormal, mCharacter->MoveTo ((short)_ttol(pAction->mValues[0]), (short)_ttol(pAction->mValues[1]), _ttol(pAction->mValues[2]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[3]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("GestureAt")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 3));
			lResult = LogComErr (LogNormal, mCharacter->GestureAt ((short)_ttol(pAction->mValues[0]), (short)_ttol(pAction->mValues[1]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[2]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Think")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 2));
			pAction->mValues[0] = DecodeTraceString (pAction->mValues[0]);
			lResult = LogComErr (LogNormal, mCharacter->Think (_bstr_t(pAction->mValues[0]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[1]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Wait")) == 0)
		{
			lResult = LogComErr (LogNormal, E_NOTIMPL);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Interrupt")) == 0)
		{
			lResult = LogComErr (LogNormal, E_NOTIMPL);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("ShowPopupMenu")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->ShowPopupMenu ((short)_ttol(pAction->mValues[0]), (short)_ttol(pAction->mValues[1])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetAutoPopupMenu")) == 0)
		{
			lResult = LogComErr (LogNormal, mCharacter->SetAutoPopupMenu (_ttol(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetTTSModeID")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
			lResult = LogComErr (LogNormal, mCharacter->SetTTSModeID (_bstr_t(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Speak")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 3));
			pAction->mValues[0] = DecodeTraceString (pAction->mValues[0]);
			lResult = LogComErr (LogNormal, mCharacter->Speak (_bstr_t(pAction->mValues[0]), _bstr_t(pAction->mValues[1]), &lReqID));
			mReqIdMap.SetAt (_ttol(pAction->mValues[2]), lReqID);
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("SetSRModeID")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
			lResult = LogComErr (LogNormal, mCharacter->SetSRModeID (_bstr_t(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Listen")) == 0)
		{
			pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
			lResult = LogComErr (LogNormal, mCharacter->Listen (_ttol(pAction->mValues[0])));
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Balloon:SetNumLines")) == 0)
		{
			IDaSvrBalloonPtr	lBalloon (mCharacter);

			if	(lBalloon != NULL)
			{
				pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
				lResult = LogComErr (LogNormal, lBalloon->SetNumLines (_ttol(pAction->mValues[0])));
			}
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Balloon:SetNumCharsPerLine")) == 0)
		{
			IDaSvrBalloonPtr	lBalloon (mCharacter);

			if	(lBalloon != NULL)
			{
				pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
				lResult = LogComErr (LogNormal, lBalloon->SetNumCharsPerLine (_ttol(pAction->mValues[0])));
			}
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Balloon:SetFontName")) == 0)
		{
			IDaSvrBalloonPtr	lBalloon (mCharacter);

			if	(lBalloon != NULL)
			{
				pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
				lResult = LogComErr (LogNormal, lBalloon->SetFontName (_bstr_t(pAction->mValues[0])));
			}
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Balloon:SetFontSize")) == 0)
		{
			IDaSvrBalloonPtr	lBalloon (mCharacter);

			if	(lBalloon != NULL)
			{
				pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
				lResult = LogComErr (LogNormal, lBalloon->SetFontSize (_ttol(pAction->mValues[0])));
			}
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Balloon:SetFontCharSet")) == 0)
		{
			IDaSvrBalloonPtr	lBalloon (mCharacter);

			if	(lBalloon != NULL)
			{
				pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
				lResult = LogComErr (LogNormal, lBalloon->SetFontCharSet ((short)_ttol(pAction->mValues[0])));
			}
		}
		else
		if	(pAction->mAction.CompareNoCase (_T("Balloon:SetVisible")) == 0)
		{
			IDaSvrBalloonPtr	lBalloon (mCharacter);

			if	(lBalloon != NULL)
			{
				pAction->mValues.SetSize (max (pAction->mValues.GetSize(), 1));
				lResult = LogComErr (LogNormal, lBalloon->SetVisible (_ttol(pAction->mValues[0])));
			}
		}
		else
		{
			lResult = E_NOTIMPL;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileKey = _T("Settings");
static LPCTSTR	sProfilePosX = _T("Left");
static LPCTSTR	sProfilePosY = _T("Top");
static LPCTSTR	sProfileRepeat = _T("Repeat");

/////////////////////////////////////////////////////////////////////////////

void CReplayActionsDlg::LoadConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	mRepeatButton.SetCheck (lApp->GetProfileInt (sProfileKey, sProfileRepeat, mRepeatButton.GetCheck()) ? TRUE : FALSE);

	GetWindowRect (&lWinRect);
	lWinRect.OffsetRect (lApp->GetProfileInt (sProfileKey, sProfilePosX, lWinRect.left) - lWinRect.left, lApp->GetProfileInt (sProfileKey, sProfilePosY, lWinRect.top) - lWinRect.top);
	lWinRect.OffsetRect (min (GetSystemMetrics(SM_CXSCREEN)-lWinRect.right, 0), min (GetSystemMetrics(SM_CYSCREEN)-lWinRect.bottom, 0));
	lWinRect.OffsetRect (max (-lWinRect.left, 0), max (-lWinRect.top, 0));
	MoveWindow (&lWinRect);
}

void CReplayActionsDlg::SaveConfig ()
{
	CWinApp *	lApp = AfxGetApp ();
	CRect		lWinRect;

	lApp->WriteProfileInt (sProfileKey, sProfileRepeat, mRepeatButton.GetCheck());

	GetWindowRect (&lWinRect);
	lApp->WriteProfileInt (sProfileKey, sProfilePosX, lWinRect.left);
	lApp->WriteProfileInt (sProfileKey, sProfilePosY, lWinRect.top);
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CReplayActionsDlg, CDialog)
	//{{AFX_MSG_MAP(CReplayActionsDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPEN_FILE, OnOpenFile)
	ON_BN_CLICKED(IDC_START_REPLAY, OnStartReplay)
	ON_BN_CLICKED(IDC_STOP_REPLAY, OnStopReplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CReplayActionsDlg::OnDestroy()
{
	SaveConfig ();
	UnloadCharacter ();
	SafeFreeSafePtr (mServer);
	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////

void CReplayActionsDlg::OnOpenFile()
{
	CFileDialog	lFileDialog (TRUE, _T(".txt"), mDefaultFileName, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("Text Files|*.txt|All Files|*.*||"), this);

	if	(lFileDialog.DoModal () == IDOK)
	{
		mActionFile = new CActionFile;
		if	(mActionFile->Open (lFileDialog.GetPathName ()))
		{
			mDefaultFileName = lFileDialog.GetPathName ();
		}
		else
		{
			mActionFile = NULL;
		}
		ShowState ();
	}
}

void CReplayActionsDlg::OnStartReplay()
{
	if	(
			(Start ())
		||	(Restart ())
		)
	{
		mPlayTimer = SetTimer ((UINT_PTR)&mPlayTimer, 100, NULL);
		ShowState ();
		OnTimer (mPlayTimer);
	}
}

void CReplayActionsDlg::OnStopReplay()
{
	if	(mPlayTimer)
	{
		KillTimer (mPlayTimer);
		mPlayTimer = 0;
	}
	ShowState ();
}

/////////////////////////////////////////////////////////////////////////////

void CReplayActionsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if	(
			(mPlayTimer)
		&&	(nIDEvent == mPlayTimer)
		)
	{
		if	(!RunActions ())
		{
			if	(!Restart ())
			{
				KillTimer (mPlayTimer);
				mPlayTimer = 0;
			}
			ShowState ();
		}
	}
	else
	{
		CDialog::OnTimer (nIDEvent);
	}
}
