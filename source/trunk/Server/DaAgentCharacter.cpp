/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaServer.h"
#include "DaCore.h"
#include "DaAgentCharacter.h"
#include "DaAgentBalloon.h"
#include "DaAgentCommands.h"
#include "DaAgentNotify.h"
#include "DaAnimationNames.h"
#include "AgentPopupWnd.h"
#include "AgentBalloonWnd.h"
#include "AgentListeningWnd.h"
#include "VoiceCommandsWnd.h"
#include "ListeningState.h"
#include "QueuedPrepare.h"
#include "FileDownload.h"
#include "SapiVoice.h"
#include "SapiVoiceCache.h"
#include "Sapi5Input.h"
#include "SapiInputCache.h"
#include "Registry.h"
#include "Localize.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Character"),LogVerbose,true)&0xFFFF|LogHighVolume)
//#define	_DEBUG_LANGUAGE		LogNormal
//#define	_DEBUG_ACTIVE		LogNormal
#define	_DEBUG_PREPARE			LogNormal
//#define	_DEBUG_LISTEN		LogNormal
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentCharacter, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentCharacter, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentCharacter::CDaAgentCharacter (long pCharID, CAgentFile * pFile, CAgentFileCache & pUsedFileCache, IDaNotify & pNotify)
:	mCharID (pCharID),
	mLangID (LANG_USER_DEFAULT),
	mFile (NULL),
	mUsedFileCache (pUsedFileCache),
	mNotify (pNotify),
	mWnd (NULL),
	mSapiVoice (NULL),
	mSapiInput (NULL),
	mIdleOn (true),
	mAutoPopupMenu (true),
	mAgentBalloon (NULL),
	mAgentCommands (NULL),
	mInNotify (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgentCharacter::CDaAgentCharacter (%d) [%ls]"), this, m_dwRef, mCharID, AfxGetModuleState()->m_nObjectCount, ((pFile) ? (BSTR)pFile->GetPath() : NULL));
	}
#endif
	AfxOleLockApp();

	if	(FAILED (SetLangID (GetUserDefaultUILanguage ())))
	{
		SetLangID (MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT));
	}
#ifdef	_DEBUG_LANGUAGE
	LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaAgentCharacter Default LangID [%4.4hX]"), this, mCharID, mLangID);
#endif
	mNotify._RegisterInternalNotify (this, true);

	if	(mFile = pFile)
	{
		if	(!mUsedFileCache.AddFileClient (mFile, this))
		{
			mUsedFileCache.CacheFile (mFile, this);
		}
		if	(!TheServerApp->AddFileClient (mFile, this))
		{
			TheServerApp->CacheFile (mFile, this);
		}
		OpenFile ();
	}

	EnableAutomation();
	EnableTypeLib();
}

CDaAgentCharacter::~CDaAgentCharacter()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgentCharacter::~CDaAgentCharacter (%d)"), this, m_dwRef, mCharID, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgentCharacter::~CDaAgentCharacter (%d) Done"), this, m_dwRef, mCharID, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

void CDaAgentCharacter::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgentCharacter::Terminate [%u %u] [%ls]"), this, m_dwRef, mCharID, pFinal, pAbandonned, (mFile ? (BSTR)mFile->GetPath() : NULL));
			if	(mWnd->GetSafeHwnd())
			{
				LogMessage (_LOG_INSTANCE, _T("[%p] [%d] [%u]   PopupWnd [%p(%u)] [%d]"), this, m_dwRef, mCharID, mWnd, mWnd->m_dwRef, mWnd->GetCharID());
			}
		}
#endif
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					ExternalDisconnect ();
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		try
		{
			if	(IsClientActive ())
			{
				SetClientActive (false, false);
			}
		}
		catch AnyExceptionDebug

		if	(
				(mFile)
			&&	(GetClientCount (mCharID) <= 0)
			)
		{
			try
			{
				if	(
						(mWnd)
					&&	(mWnd->GetBalloonWnd())
					)
				{
					TheServerApp->RemoveFileClient (mFile, mWnd->GetBalloonWnd());
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(mWnd)
				{
					TheServerApp->RemoveFileClient (mFile, mWnd);
				}
			}
			catch AnyExceptionDebug
		}

		if	(GetClientCount (mCharID) <= 0)
		{
			ReleaseSapiVoice ();
			ReleaseSapiInput ();
		}

		try
		{
			if	(mWnd)
			{
				mWnd->Detach (mCharID, &mNotify);
			}
			mWnd = NULL;
		}
		catch AnyExceptionDebug

		if	(!mPrepares.IsEmpty ())
		{
			try
			{
				mPrepares.RemoveAll ();
			}
			catch AnyExceptionDebug
		}

		if	(mAgentCommands != NULL)
		{
			try
			{
				CDaAgentCommands *	lCommands;

				if	(
						(pFinal)
					&&	(pAbandonned)
					&&	(lCommands = GetCommandsObj (false))
					)
				{
					lCommands->Terminate (pFinal, pAbandonned);
					lCommands->OnFinalRelease ();
				}
				else
				if	(pFinal)
				{
					mAgentCommands->Release();
				}
			}
			catch AnyExceptionSilent

			if	(pFinal)
			{
				mAgentCommands = NULL;
			}
		}

		if	(mAgentBalloon != NULL)
		{
			try
			{
				CDaAgentBalloon *	lBalloon;

				if	(
						(pFinal)
					&&	(pAbandonned)
					&&	(lBalloon = GetBalloonObj (false))
					)
				{
					lBalloon->Terminate (pFinal, pAbandonned);
					lBalloon->OnFinalRelease ();
				}
				else
				if	(pFinal)
				{
					mAgentBalloon->Release ();
				}
			}
			catch AnyExceptionSilent

			if	(pFinal)
			{
				mAgentBalloon = NULL;
			}
		}

		SafeFreeSafePtr (mWndRefHolder);

		try
		{
			if	(mFile)
			{
				mNotify._RegisterInternalNotify (this, false);
			}
		}
		catch AnyExceptionDebug
		try
		{
			if	(mFile)
			{
				mUsedFileCache.RemoveFileClient (mFile, this, false);
			}
		}
		catch AnyExceptionDebug
		try
		{
			if	(mFile)
			{
				TheServerApp->RemoveFileClient (mFile, this);
			}
		}
		catch AnyExceptionDebug

#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgentCharacter::Terminate [%u %u] Done"), this, m_dwRef, mCharID, pFinal, pAbandonned);
		}
#endif
		mFile = NULL;
	}
}

void CDaAgentCharacter::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgentCharacter::OnFinalRelease [%u]"), this, m_dwRef, mCharID, IsInNotify());
	}
#endif
	if	(IsInNotify() == 0)
	{
		Terminate (false);
		CCmdTarget::OnFinalRelease();
	}
}

/////////////////////////////////////////////////////////////////////////////

LPUNKNOWN CDaAgentCharacter::GetInterfaceHook(const void* iid)
{
	LPUNKNOWN	lRet = NULL;
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] [%d] CDaAgentCharacter::QueryInterface [%s]"), this, m_dwRef, mCharID, CGuidStr::GuidName(*(GUID*)iid));
#endif
	if	(
			(IsEqualIID (*(const IID *)iid, __uuidof (IDaSvrBalloon)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentBalloon)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentBalloonEx)))
		)
	{
		CDaAgentBalloon *	lBalloon = NULL;

		if	(
				(mAgentBalloon == NULL)
			&&	(mFile)
			&&	(lBalloon = new CDaAgentBalloon (mCharID, mWnd, mFile, mNotify))
			)
		{
			lBalloon->SetLangID (mLangID);
			mAgentBalloon = lBalloon->GetIDispatch (FALSE);
		}
		if	(
				(mAgentBalloon)
			&&	(SUCCEEDED (mAgentBalloon->QueryInterface (*(const IID *)iid, (void**)&lRet)))
			)
		{
			InterlockedDecrement (&m_dwRef);
		}
	}
	else
	if	(
			(IsEqualIID (*(const IID *)iid, __uuidof (IDaSvrCommands)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentCommands)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentCommandsEx)))
		)
	{
		CDaAgentCommands *	lCommands = NULL;

		if	(
				(mAgentCommands == NULL)
			&&	(lCommands = new CDaAgentCommands (mCharID, mNotify))
			)
		{
			lCommands->SetLangID (mLangID);
			mAgentCommands = lCommands->GetIDispatch (FALSE);
		}
		if	(
				(mAgentCommands)
			&&	(SUCCEEDED (mAgentCommands->QueryInterface (*(const IID *)iid, (void**)&lRet)))
			)
		{
			InterlockedDecrement (&m_dwRef);
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::PreNotify ()
{
	if	(
			(this)
		&&	(m_dwRef > 0)
		)
	{
		mInNotify++;
		return true;
	}
	return false;
}

bool CDaAgentCharacter::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		if	(
				(mInNotify == 0)
			&&	(m_dwRef == 0)
			)
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive (_LOG_INSTANCE))
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%d] CDaAgent PostNotify -> OnFinalRelease"), this, m_dwRef, mCharID);
			}
#endif
			OnFinalRelease ();
			return false;
		}
		return true;
	}
	return false;
}

UINT CDaAgentCharacter::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BSTR CDaAgentCharacter::GetName () const
{
	CAgentFileName *	lFileName;

	if	(
			(mFile)
		&&	(lFileName = mFile->FindName (mLangID))
		)
	{
		return lFileName->mName;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

void CDaAgentCharacter::OpenFile ()
{
	tPtr <CAgentPopupWnd>	lNewWnd;
	CObTypeArray <CObject>	lFileClients;
	int						lClientNdx;

	if	(
			(mFile)
		&&	(TheServerApp->GetFileClients (mFile, lFileClients))
		)
	{
		for	(lClientNdx = 0; lClientNdx <= lFileClients.GetUpperBound(); lClientNdx++)
		{
			if	(
					(mWnd = DYNAMIC_DOWNCAST (CAgentPopupWnd, lFileClients [lClientNdx]))
				&&	(mWnd->GetSafeHwnd ())
				)
			{
				break;
			}
			mWnd = NULL;
		}
	}

	if	(
			(mFile)
		&&	(!mWnd->GetSafeHwnd())
		&&	(lNewWnd = (CAgentPopupWnd *) CAgentPopupWnd::CreateObject())
		&&	(lNewWnd->Create (NULL))
		)
	{
		lNewWnd->ModifyStyle (WS_CAPTION|WS_THICKFRAME|WS_SYSMENU, 0, SWP_FRAMECHANGED);
		lNewWnd->ModifyStyleEx (0, WS_EX_TOOLWINDOW);

		if	(SUCCEEDED (lNewWnd->Open (mFile)))
		{
			lNewWnd->EnableSound (lNewWnd->IsSoundEnabled());
			mWnd = lNewWnd.Detach ();
		}
	}

	if	(
			(mFile)
		&&	(mWnd->GetSafeHwnd())
		)
	{
		TheServerApp->AddFileClient (mFile, mWnd);
		mWndRefHolder = mWnd->GetIDispatch (FALSE);
		if	(GetActiveClient () <= 0)
		{
			SetClientActive (true, false);
		}
		else
		{
			mWnd->Attach (mCharID, &mNotify, false);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::IsVisible (bool pOrIsShowing) const
{
	bool	lRet = false;

	if	(
			(mWnd->GetSafeHwnd())
		&&	(
				(mWnd->GetStyle() & WS_VISIBLE)
			||	(
					(pOrIsShowing)
				&&	(mWnd->IsShowingQueued ())
				)
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaAgentCharacter::IsShowing () const
{
	bool	lRet = false;

	if	(
			(mWnd->GetSafeHwnd())
		&&	(
				(mWnd->IsShowingQueued ())
			||	(mWnd->IsShowingState (_T("SHOWING")))
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaAgentCharacter::IsHiding () const
{
	bool	lRet = false;

	if	(
			(mWnd->GetSafeHwnd())
		&&	(
				(mWnd->IsHidingQueued ())
			||	(mWnd->IsShowingState (_T("HIDING")))
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CDaAgentCharacter::Show (bool pFast, bool pImmediate)
{
	long	lReqID = 0;

	if	(mWnd->GetSafeHwnd ())
	{
		if	(pImmediate)
		{
			mWnd->ShowPopup (mCharID, ProgramShowed);
		}
		else
		{
			lReqID = mWnd->QueueShow (mCharID, pFast);
			mWnd->ActivateQueue (true);
		}
	}
	return lReqID;
}

long CDaAgentCharacter::Hide (bool pFast, bool pImmediate)
{
	long	lReqID = 0;

	StopListening (false, LSCOMPLETE_CAUSE_CLIENTDEACTIVATED);

	if	(mWnd->GetSafeHwnd ())
	{
		if	(pImmediate)
		{
			mWnd->HidePopup (mCharID, ProgramHid);
		}
		else
		{
			lReqID = mWnd->QueueHide (mCharID, pFast);
			mWnd->ActivateQueue (true);
		}
	}
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::IsInputActive () const
{
	bool	lRet = false;

	if	(
			(mWnd->GetSafeHwnd ())
		&&	(mWnd->GetCharID () == GetCharID())
		&&	(mWnd->GetLastActive () == mWnd->GetSafeHwnd ())
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaAgentCharacter::IsClientActive () const
{
	bool	lRet = false;

	if	(
			(mWnd->GetSafeHwnd ())
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		lRet = true;
	}
	return lRet;
}

long CDaAgentCharacter::GetActiveClient () const
{
	long	lRet = 0;

	if	(mWnd->GetSafeHwnd ())
	{
		lRet = mWnd->GetCharID ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::SetClientActive (bool pActive, bool pInputActive)
{
	bool				lRet = false;
	long				lPrevCharId = 0;
	CDaAgentCharacter *	lNextCharacter = NULL;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

#ifdef	_DEBUG_ACTIVE
	LogMessage (_DEBUG_ACTIVE, _T("[%d] SetClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listening [%d]"), mCharID, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify._GetActiveCharacter(), (TheServerApp->GetListenCharacter()?TheServerApp->GetListenCharacter()->GetCharID():0));
#endif
	if	(mWnd->GetSafeHwnd())
	{
		lPrevCharId = mWnd->GetCharID ();
	}

	if	(
			(pActive)
		&&	(lPrevCharId != mCharID)
		)
	{
		if	(
				(mWnd->GetSafeHwnd())
			&&	(mWnd->Attach (mCharID, &mNotify, true))
			)
		{
			if	(lBalloonWnd = GetBalloonWnd (false))
			{
				lBalloonWnd->SetOptions (mFile->GetBalloon(), GetBalloonInterface (false), mLangID);
			}
			lNextCharacter = this;
			lRet = true;
		}
	}
	else
	if	(
			(!pActive)
		&&	(lPrevCharId == mCharID)
		)
	{
		try
		{
			CObTypeArray <CObject>	lFileClients;
			int						lClientNdx;

			if	(
					(mFile)
				&&	(TheServerApp->GetFileClients (mFile, lFileClients))
				)
			{
				for	(lClientNdx = 0; lClientNdx <= lFileClients.GetUpperBound(); lClientNdx++)
				{
					if	(
							(lNextCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
						&&	(lNextCharacter->GetCharID() != mCharID)
						)
					{
						break;
					}
					lNextCharacter = NULL;
				}
			}

			if	(
					(lNextCharacter)
				&&	(lNextCharacter->mWnd == mWnd)
				&&	(mWnd->Attach (lNextCharacter->mCharID, &lNextCharacter->mNotify, true))
				)
			{
				if	(lBalloonWnd = lNextCharacter->GetBalloonWnd (false))
				{
					lBalloonWnd->SetOptions (lNextCharacter->mFile->GetBalloon(), lNextCharacter->GetBalloonInterface (false), lNextCharacter->mLangID);
				}
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}

	if	(
			(pActive)
		&&	(pInputActive)
		&&	(mWnd->GetSafeHwnd())
		&&	(mWnd->GetStyle() & WS_VISIBLE)
		)
	{
		mWnd->SetLastActive (mWnd->GetSafeHwnd());
	}
#ifdef	_DEBUG_ACTIVE
	LogMessage (_DEBUG_ACTIVE, _T("[%d] %u  ClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listening [%d]"), mCharID, lRet, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), mNotify._GetActiveClient(mCharID), mNotify._GetActiveCharacter(), (TheServerApp->GetListenCharacter()?TheServerApp->GetListenCharacter()->GetCharID():0));
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

int CDaAgentCharacter::GetClientCount (int pSkipCharID) const
{
	int	lRet = -1;

	try
	{
		CObTypeArray <CObject>	lFileClients;
		int						lClientNdx;
		CDaAgentCharacter *		lCharacter;

		if	(
				(mFile)
			&&	(TheServerApp->GetFileClients (mFile, lFileClients))
			)
		{
			lRet = 0;

			for	(lClientNdx = 0; lClientNdx <= lFileClients.GetUpperBound(); lClientNdx++)
			{
				if	(
						(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
					&&	(
							(pSkipCharID <= 0)
						||	(lCharacter->GetCharID() != pSkipCharID)
						)
					)
				{
					lRet++;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgentCharacter::SetLangID (LANGID pLangID)
{
	HRESULT	lResult = S_OK;
	LANGID	lLangID;

	if	(pLangID == LANG_USER_DEFAULT)
	{
#ifdef	_DEBUG
		pLangID = LANGIDFROMLCID (GetThreadLocale ());
#else
		pLangID = GetUserDefaultUILanguage();
#endif
	}
	else
	if	(pLangID == LANG_SYSTEM_DEFAULT)
	{
		pLangID = GetSystemDefaultUILanguage();
	}

	lLangID = pLangID;

	if	(
			(lLangID == 0)
		||	(
				(!::IsValidLocale (MAKELCID (lLangID, SORT_DEFAULT), LCID_SUPPORTED))
#ifdef	_DEBUG
			&&	(GetProfileDebugInt(_T("DebugAnyLanguage")) <= 0)
#endif
			)
		)
	{
#ifdef	_DEBUG_LANGUAGE
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaAgentCharacter LangID [%4.4hX] not supported"), this, mCharID, pLangID);
#endif
		lResult = E_INVALIDARG;
	}
	else
	if	(
			(PRIMARYLANGID (lLangID) == LANG_ENGLISH)
		||	(CLocalize::GetMuiModule (lLangID))
		||	(CLocalize::GetMuiModule (lLangID = MAKELANGID (PRIMARYLANGID (lLangID), SUBLANG_DEFAULT)))
		||	(CLocalize::GetMuiModule (lLangID = MAKELANGID (PRIMARYLANGID (lLangID), SUBLANG_NEUTRAL)))
		)
	{
#ifdef	_DEBUG_LANGUAGE
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaAgentCharacter LangID from [%4.4hX] to [%4.4hX]"), this, mCharID, mLangID, lLangID);
#endif
		if	(
				(mWnd)
			&&	(mWnd->GetCharID() == mCharID)
			)
		{
			mWnd->RemoveQueuedSpeak (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("SetLangID"));
			mWnd->RemoveQueuedThink (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("SetLangID"));
		}
		ReleaseSapiVoice ();
		ReleaseSapiInput ();

		mLangID = lLangID;
		PropagateLangID ();
	}
	else
	{
#ifdef	_DEBUG_LANGUAGE
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaAgentCharacter LangID [%4.4hX] not installed"), this, mCharID, pLangID);
#endif
		lResult = AGENTERR_LANGUAGENOTFOUND;
	}
	return lResult;
}

void CDaAgentCharacter::PropagateLangID ()
{
	if	(mFile)
	{
		try
		{
			CDaAgentBalloon *		lBalloon = NULL;
			CDaAgentCommands *		lCommands = NULL;
			CAgentBalloonWnd *		lBalloonWnd;
			CVoiceCommandsWnd *		lVoiceCommandsWnd;
			CAgentListeningWnd *	lListeningWnd;

			if	(lCommands = GetCommandsObj (false))
			{
				lCommands->SetLangID (mLangID);
			}

			if	(
					(lVoiceCommandsWnd = TheServerApp->GetVoiceCommandsWnd (false))
				&&	(lVoiceCommandsWnd->GetSafeHwnd ())
				&&	(lVoiceCommandsWnd->GetCharID () == mCharID)
				&&	(lVoiceCommandsWnd->GetLangID () != mLangID)
				)
			{
				lVoiceCommandsWnd->SetLangID (mLangID);

				if	(lCommands)
				{
					lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				}
				else
				{
					lVoiceCommandsWnd->ShowGlobalCommands ();
				}
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
			}

			if	(
					(lListeningWnd = GetListeningWnd (false))
				&&	(lListeningWnd->GetCharID () == mCharID)
				&&	(lListeningWnd->GetLangID () != mLangID)
				)
			{
				lListeningWnd->SetLangID (mLangID);
			}

			if	(
					(mWnd)
				&&	(mWnd->GetCharID() == mCharID)
				)
			{
				mWnd->SetNotifyIconName (mLangID);
			}

			if	(lBalloon = GetBalloonObj (false))
			{
				lBalloon->SetLangID (mLangID);
			}

			if	(
					(lBalloonWnd = GetBalloonWnd (false))
				&&	(lBalloonWnd->GetCharID() == mCharID)
				&&	(lBalloonWnd->GetLangID() != mLangID)
				)
			{
				lBalloonWnd->SetOptions (mFile->GetBalloon(), GetBalloonInterface (false), mLangID);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::IsSpeaking () const
{
	bool			lRet = false;
	CSapiVoice *	lVoice;

//
//	Note that if the voice is shared by multiple characters, we don't check
//	which character is speaking.  That's ok because this method is used
//	to check if ANY character is speaking, so it doesn't matter.
//
	if	(
			(lVoice = const_cast <CDaAgentCharacter *> (this)->GetSapiVoice (false))
		&&	(lVoice->SafeIsSpeaking ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CSapiVoice * CDaAgentCharacter::GetSapiVoice (bool pCreateObject, LPCTSTR pVoiceName)
{
	if	(
			(!mSapiVoice)
		&&	(pCreateObject)
		&&	(mFile)
		)
	{
		try
		{
			CSapiVoiceCache *	lVoiceCache;
			CString				lVoiceName (pVoiceName);

			if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			{
				if	(lVoiceName.IsEmpty ())
				{
					CAgentFileTts	lFileTts;

					lFileTts = mFile->GetTts ();
					lFileTts.mLanguage = mLangID;
					mSapiVoice = lVoiceCache->GetAgentVoice (lFileTts, true);
				}
				else
				{
					mSapiVoice = lVoiceCache->GetAgentVoice (lVoiceName, mLangID, true);
				}

				if	(mSapiVoice)
				{
					if	(!mSapiVoice->SafeIsPrepared ())
					{
						mSapiVoice->PrepareToSpeak ();
					}
					if	(!lVoiceCache->CacheVoice (mSapiVoice, this))
					{
						lVoiceCache->AddVoiceClient (mSapiVoice, this);
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
	return mSapiVoice;
}

void CDaAgentCharacter::ReleaseSapiVoice ()
{
	try
	{
		CSapiVoiceCache *	lVoiceCache;

		if	(
				(mSapiVoice)
			&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			)
		{
			lVoiceCache->RemoveVoiceClient (mSapiVoice, this);
		}
	}
	catch AnyExceptionDebug

	mSapiVoice = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::IsListening () const
{
	bool	lRet = false;

	if	(
			(mListeningState)
		&&	(mListeningState->IsListening ())
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaAgentCharacter::IsHearing () const
{
	bool	lRet = false;

	if	(
			(mListeningState)
		&&	(mListeningState->IsHearing ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgentCharacter::StartListening (bool pManual)
{
	HRESULT	lResult = S_OK;

	try
	{
#ifdef	_DEBUG_LISTEN
		if	(
				(pManual)
			||	(!mListeningState)
			||	(!mListeningState->IsAutomatic ())
			)
		{
			LogMessage (_DEBUG_LISTEN, _T("[%d] StartListening Manual [%u] Listening [%u] Active [%u] Enabled [%u]"), mCharID, pManual, IsListening(), (mNotify._GetActiveCharacter()==GetCharID()), CDaSpeechInputConfig().LoadConfig().mEnabled);
		}
#endif

		if	(
				(!pManual)
			&&	(mListeningState)
			&&	(mListeningState->IsAutomatic ())
			)
		{
			lResult = mListeningState->KeepListening (pManual);
		}
		else
		if	(!mWnd->GetSafeHwnd())
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(
				(pManual)
			?	(mNotify._GetActiveCharacter() != GetCharID())
			:	(!IsClientActive ())
			)
		{
			lResult = AGENTERR_CHARACTERNOTACTIVE;
		}
		else
		if	(
				(mListeningState)
			||	(mListeningState = new CListeningState (*this))
			)
		{
			lResult = mListeningState->StartListening (pManual);
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDaAgentCharacter::StopListening (bool pManual, long pCause)
{
	HRESULT	lResult = S_FALSE;

	try
	{
		if	(mListeningState)
		{
#ifdef	_DEBUG_LISTEN
			LogMessage (_DEBUG_LISTEN, _T("[%d] StopListening Manual [%u] Listening [%u] Active [%u]"), mCharID, pManual, IsListening(), (mNotify._GetActiveCharacter()==GetCharID()));
#endif
			lResult = mListeningState->StopListening (pManual, pCause);
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCharacter::TransferListeningState (CDaAgentCharacter * pOtherCharacter)
{
	if	(
			(pOtherCharacter)
		&&	(pOtherCharacter != this)
		&&	(IsListening ())
		&&	(pOtherCharacter->GetSapiInput (true))
		)
	{
#ifdef	_DEBUG_LISTEN
		LogMessage (_DEBUG_LISTEN, _T("[%d] TransferListening [%d]"), mCharID, pOtherCharacter->GetCharID());
#endif
		if	(
				(pOtherCharacter->mListeningState)
			||	(pOtherCharacter->mListeningState = new CListeningState (*pOtherCharacter))
			)
		{
			mListeningState->TransferState (pOtherCharacter->mListeningState);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::ShowListeningState (bool pShow)
{
	bool	lRet = false;

#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%d] ShowListeningState [%u] IsVisible [%u] IsShowing [%u] IsHiding {%u]"), mCharID, pShow, IsVisible(false), IsShowing(), IsHiding());
#endif
	if	(mWnd->GetSafeHwnd ())
	{
		if	(pShow)
		{
			if	(
					(IsVisible (false))
				&&	(!IsShowing ())
				&&	(!IsHiding ())
				)
			{
				StopAll (STOP_TYPE_PLAY|STOP_TYPE_MOVE|STOP_TYPE_SPEAK|STOP_TYPE_PREPARE, AGENTREQERR_INTERRUPTEDLISTENKEY);
				if	(mWnd->QueueState (0, _T("LISTENING")))
				{
					mWnd->ActivateQueue (true);
					lRet = true;
				}
			}
		}
		else
		{
			if	(mWnd->RemoveQueuedState (0, _T("LISTENING")))
			{
				lRet = true;
			}
			if	(mWnd->IsShowingState (_T("LISTENING")))
			{
				mWnd->ShowGesture (NULL);
				lRet = true;
			}
			mWnd->ActivateQueue (true);
		}
	}
#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%d] ShowListeningState [%u] Done [%u]"), mCharID, pShow, lRet);
#endif
	return lRet;
}

bool CDaAgentCharacter::ShowHearingState (bool pShow)
{
	bool	lRet = false;

#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%d] ShowHearingState [%u] IsVisible [%u] IsShowing [%u] IsHiding {%u]"), mCharID, pShow, IsVisible(false), IsShowing(), IsHiding());
#endif
	if	(mWnd->GetSafeHwnd ())
	{
		if	(pShow)
		{
			if	(
					(IsVisible (false))
				&&	(!IsShowing ())
				&&	(!IsHiding ())
				)
			{
				StopAll (STOP_TYPE_PLAY|STOP_TYPE_MOVE|STOP_TYPE_SPEAK|STOP_TYPE_PREPARE, AGENTREQERR_INTERRUPTEDHEARING);

				if	(mWnd->QueueState (0, _T("HEARING")))
				{
					mWnd->ActivateQueue (true);
					lRet = true;
				}
			}
		}
		else
		{
			if	(mWnd->RemoveQueuedState (0, _T("HEARING")))
			{
				lRet = true;
			}
			if	(mWnd->IsShowingState (_T("HEARING")))
			{
				mWnd->ShowGesture (NULL);
				lRet = true;
			}
			mWnd->ActivateQueue (true);
		}
	}
#ifdef	_DEBUG_LISTEN
	LogMessage (_DEBUG_LISTEN, _T("[%d] ShowHearingState [%u] Done [%u]"), mCharID, pShow, lRet);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CDaAgentCharacter::GetListeningStatus ()
{
	long	lStatus = LISTEN_STATUS_CANLISTEN;

	if	(!CDaSpeechInputConfig().LoadConfig().mEnabled)
	{
		lStatus = LISTEN_STATUS_SPEECHDISABLED;
	}
	else
	if	(mNotify._GetActiveCharacter() != GetCharID())
	{
		lStatus = LISTEN_STATUS_NOTACTIVE;
	}
	else
	if	(!GetSapiInput (true))
	{
		lStatus = LISTEN_STATUS_COULDNTINITIALIZESPEECH;
	}
	return lStatus;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CSapi5Input * CDaAgentCharacter::GetSapiInput (bool pCreateObject, LPCTSTR pEngineName)
{
	if	(
			(!mSapiInput)
		&&	(pCreateObject)
		)
	{
		try
		{
			CSapiInputCache *	lInputCache;
			CString				lEngineName (pEngineName);

			if	(lInputCache = CSapiInputCache::GetStaticInstance ())
			{
				if	(lEngineName.IsEmpty ())
				{
					mSapiInput = lInputCache->GetAgentInput (mLangID, false);
				}
				else
				{
					mSapiInput = lInputCache->GetAgentInput (lEngineName, mLangID, false);
				}

				if	(mSapiInput)
				{
					if	(!mSapiInput->SafeIsPrepared ())
					{
						mSapiInput->PrepareToListen ();
					}
					if	(!lInputCache->CacheInput (mSapiInput, this))
					{
						lInputCache->AddInputClient (mSapiInput, this);
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
	return mSapiInput;
}

void CDaAgentCharacter::ReleaseSapiInput ()
{
	SafeFreeSafePtr (mListeningState);

	try
	{
		CSapiInputCache *	lInputCache;

		if	(
				(mSapiInput)
			&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
			)
		{
			lInputCache->RemoveInputClient (mSapiInput, this);
		}
	}
	catch AnyExceptionDebug

	mSapiInput = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgentCharacter::StopAll (long pStopTypes, HRESULT pReqStatus)
{
	HRESULT	lResult = S_FALSE;
	bool	lExcludeActive = false;

	if	(pStopTypes & STOP_TYPE_PLAY)
	{
		if	(mWnd->ClearQueuedStates (mCharID, pReqStatus, _T("StopAll"), lExcludeActive, _T("SHOWING"), _T("HIDING"), NULL))
		{
			lResult = S_OK;
		}
		if	(mWnd->ClearQueuedGestures (mCharID, pReqStatus, _T("StopAll"), lExcludeActive, _T("SHOWING"), _T("HIDING"), NULL))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & STOP_TYPE_SPEAK)
	{
		if	(mWnd->RemoveQueuedSpeak (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
		if	(mWnd->RemoveQueuedThink (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
//
//	The balloon may still be visible even if there are no more Speak or Think actions queued.  Either the balloon
//	is not AUTOHIDE, or it's AUTOHIDE timer is set and it will disappear soon.  We could hide it immediately if
//	we wanted, but for now we won't.
//
//		CAgentBalloonWnd *	lBalloonWnd;
//		if	(
//				(lBalloonWnd = GetBalloonWnd (false))
//			&&	(lBalloonWnd->IsAutoHide ())
//			)
//		{
//			lBalloonWnd->HideBalloon (true);
//		}
	}

	if	(pStopTypes & STOP_TYPE_MOVE)
	{
		if	(mWnd->RemoveQueuedMove (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & STOP_TYPE_VISIBLE)
	{
		if	(mWnd->RemoveQueuedShow (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
		if	(mWnd->RemoveQueuedHide (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & STOP_TYPE_PREPARE)
	{
		if	(mWnd->RemoveQueuedPrepare (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(
			(pStopTypes & STOP_TYPE_NONQUEUEDPREPARE)
		&&	(!mPrepares.IsEmpty ())
		)
	{
		try
		{
			POSITION			lPos;
			long				lReqID;
			CQueuedPrepare *	lPrepare;

			for	(lPos = mPrepares.GetStartPosition(); lPos;)
			{
				mPrepares.GetNextAssoc (lPos, lReqID=0, lPrepare=NULL);

				if	(
						(lPrepare)
					&&	(!lPrepare->IsComplete ())
					)
				{
					try
					{
						lPrepare->CancelDownloads ();
					}
					catch AnyExceptionSilent

#ifdef	_DEBUG_REQUESTS
					LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestStart    [%d]"), mCharID, lReqID);
#endif
					PreNotify ();
					try
					{
						mNotify.RequestComplete (lReqID, pReqStatus);
					}
					catch AnyExceptionSilent
					PostNotify ();
				}
			}
		}
		catch AnyExceptionDebug
	}

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgentCharacter::DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID)
{
	HRESULT					lResult = E_FAIL;
	tPtr <CQueuedPrepare>	lPrepare;

#ifdef	_DEBUG_PREPARE
	LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaAgentCharacter Prepare [%d %u] [%s]"), this, mCharID, pType, pQueue, pName);
#endif

	if	(
			(mWnd)
		&&	(mFile)
		)
	{
		if	(
				(
					(pType == PREPARE_ANIMATION)
				||	(pType == PREPARE_STATE)
				)
			&&	(!mFile->IsAcfFile ())
			)
		{
			lResult = S_FALSE;
		}
		else
		if	(
				(pType != PREPARE_ANIMATION)
			&&	(pType != PREPARE_STATE)
			&&	(pType != PREPARE_WAVE)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		{
			pReqID = mNotify.NextReqID ();

			if	(!pQueue)
			{
#ifdef	_DEBUG_REQUESTS
				LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestStart    [%d]"), mCharID, pReqID);
#endif
				PreNotify ();
				try
				{
					mNotify.RequestStart (pReqID);
				}
				catch AnyExceptionSilent
				PostNotify ();
			}

			if	(lPrepare = CQueuedPrepare::CreateObject (mCharID, pReqID))
			{
				if	(pType == PREPARE_ANIMATION)
				{
					lResult = lPrepare->PutAnimationNames (mFile, pName, &mNotify);
				}
				else
				if	(pType == PREPARE_STATE)
				{
					lResult = lPrepare->PutStateNames (mFile, pName, &mNotify);
				}
				else
				{
					lResult = lPrepare->PutSoundUrl (mFile, pName, &mNotify);
				}

				if	(pQueue)
				{
					if	(SUCCEEDED (lResult))
					{
						mWnd->PutQueuedAction (lPrepare.Detach ());
					}
				}
				else
				{
					mPrepares.SetAt (pReqID, lPrepare);

					if	(lResult == S_OK)
					{
						lPrepare->StartDownloads ();
#ifdef	_DEBUG_PREPARE
						LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaAgentCharacter Preparing [%p] [%d] [%s]"), this, mCharID, lPrepare.Ptr(), lPrepare->mReqID, (BSTR)lPrepare->GetAnimationNames());
#endif
					}
					else
					{
						mPrepares.RemoveKey (pReqID);
					}
					lPrepare.Detach ();
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}

			if	(
					(!pQueue)
				&&	(FAILED (lResult))
				)
			{
#ifdef	_DEBUG_REQUESTS
				LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestComplete [%d] [%8.8X]"), mCharID, pReqID, lResult);
#endif
				PreNotify ();
				try
				{
					mNotify.RequestComplete (pReqID, lResult);
				}
				catch AnyExceptionSilent
				PostNotify ();
			}
		}
	}
	return lResult;
}

bool CDaAgentCharacter::_OnDownloadComplete (CFileDownload * pDownload)
{
	bool	lRet = false;

	if	(pDownload)
	{
		try
		{
			POSITION			lPos;
			long				lReqID;
			CQueuedPrepare *	lPrepare;
			HRESULT				lResult = S_OK;

#ifdef	_DEBUG_PREPARE
			LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaAgentCharacter _OnDownloadComplete [%ls]"), this, mCharID, (BSTR)pDownload->GetURL());
#endif
			for	(lPos = mPrepares.GetStartPosition(); lPos;)
			{
				mPrepares.GetNextAssoc (lPos, lReqID=0, lPrepare=NULL);

				if	(
						(lPrepare)
					&&	(lPrepare->FindDownload (pDownload))
					)
				{
					lRet = true;
					break;
				}
			}

			if	(lRet)
			{
				if	(!lPrepare->IsSoundDownload ())
				{
					lResult = lPrepare->PutAnimation (mFile, pDownload);
#ifdef	_DEBUG_PREPARE
					LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] [%d] CDaAgentCharacter PutAnimation [%ls] [%ls]"), this, mCharID, (BSTR)pDownload->GetURL(), (BSTR)pDownload->GetCacheName());
#endif
				}

				if	(!lPrepare->IsComplete ())
				{
					lPrepare->StartDownloads ();
				}

				if	(lPrepare->IsComplete ())
				{
#ifdef	_DEBUG_PREPARE
					LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaAgentCharacter Prepare Complete [%p] [%d] [%ls]"), this, mCharID, lPrepare, lPrepare->mReqID, (BSTR)lPrepare->GetAnimationNames());
#endif
#ifdef	_DEBUG_REQUESTS
					LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestComplete [%d] [%8.8X]"), mCharID, lPrepare->mReqID, lResult);
#endif
					PreNotify ();
					try
					{
						mNotify.RequestComplete (lPrepare->mReqID, lResult);
					}
					catch AnyExceptionSilent
					PostNotify ();
				}
			}
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

CFileDownload * CDaAgentCharacter::_FindSoundDownload (LPCTSTR pSoundUrl)
{
	CFileDownload *	lRet = NULL;

	try
	{
		POSITION			lPos;
		long				lReqID;
		CQueuedPrepare *	lPrepare;
		CFileDownload *		lDownload;

		for	(lPos = mPrepares.GetStartPosition(); lPos;)
		{
			mPrepares.GetNextAssoc (lPos, lReqID=0, lPrepare=NULL);

			if	(
					(lPrepare)
				&&	(lPrepare->IsSoundDownload ())
				&&	(lDownload = lPrepare->GetDownload ())
				&&	(CString (lDownload->GetURL()).CompareNoCase (pSoundUrl) == 0)
				)
			{
				lRet = lDownload;
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCharacter::_OnCharacterNameChanged (long pCharID)
{
	BSTR	lName;

	if	(
			(mWnd->GetSafeHwnd())
		&&	(mWnd->GetCharID() == GetCharID())
		&&	(lName = GetName ())
		)
	{
		mWnd->SetNotifyIconName (CString (lName), mLangID);
	}
}

void CDaAgentCharacter::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	CVoiceCommandsWnd *	lVoiceCommandsWnd;

#ifdef	_DEBUG_ACTIVE
	LogMessage (_DEBUG_ACTIVE, _T("[%d] OnCharacterActive [%d] InputActive [%d] Inactive [%d] InputInactive [%d] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ActiveClient [%d] InputActive [%d] Listening [%d]"), mCharID, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify._GetActiveCharacter(), (TheServerApp->GetListenCharacter()?TheServerApp->GetListenCharacter()->GetCharID():0));
#endif

	if	(pActiveCharID == mCharID)
	{
		PropagateLangID ();
		if	(mWnd->GetSafeHwnd ())
		{
			mWnd->EnableIdle (mIdleOn);
		}
	}

	if	(
			(pInactiveCharID == mCharID)
		&&	(pActiveCharID != GetActiveClient ())
		)
	{
		CDaAgentCharacter *	lListeningCharacter = NULL;

		if	(
				(IsListening ())
			&&	(
					(
						(pInputActiveCharID > 0)
					&&	(pInputActiveCharID == pActiveCharID)
					&&	(lListeningCharacter = TheServerApp->GetAppCharacter (pInputActiveCharID))
					)
				||	(
						(lListeningCharacter = TheServerApp->GetListenCharacter ())
					)
				)
			&&	(lListeningCharacter != this)
			)
		{
			TransferListeningState (lListeningCharacter);
		}

		if	(lListeningCharacter != this)
		{
			StopListening (false, LSCOMPLETE_CAUSE_CLIENTDEACTIVATED);
		}
	}

	if	(
			(
				(pActiveCharID == mCharID)
			||	(pInactiveCharID == mCharID)
			)
		&&	(lVoiceCommandsWnd = TheServerApp->GetVoiceCommandsWnd (false))
		&&	(lVoiceCommandsWnd->GetSafeHwnd ())
		)
	{
		CDaAgentCommands *	lCommands;

		if	(
				(pActiveCharID == mCharID)
			&&	(pInputActiveCharID == mCharID)
			)
		{
			if	(lVoiceCommandsWnd = TheServerApp->GetVoiceCommandsWnd (true, mCharID))
			{
				lVoiceCommandsWnd->SetLangID (mLangID);

				if	(lCommands = GetCommandsObj (true))
				{
					lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				}
				else
				{
					lVoiceCommandsWnd->ShowGlobalCommands ();
				}
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
			}
		}
		else
		{
			if	(lVoiceCommandsWnd->GetCharID() == mCharID)
			{
				lVoiceCommandsWnd->SetCharacter (0);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::_OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	if	(pCharID == mCharID)
	{
		if	(mAutoPopupMenu)
		{
			DoContextMenu (pOwner, pPosition);
		}
		return true;
	}
	return false;
}

bool CDaAgentCharacter::_OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	if	(pCharID == mCharID)
	{
		if	(mAutoPopupMenu)
		{
			DoDefaultCommand (pOwner, pPosition);
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::DoContextMenu (HWND pOwner, const CPoint & pPosition)
{
	CDaAgentCommands *	lCommands;
	CDaAgentCommand *	lCommand;
	USHORT				lCommandId;

	if	(
			(lCommands = GetCommandsObj (true))
		&&	(lCommandId = lCommands->DoContextMenu (pOwner, pPosition))
		)
	{
		if	(
				(!DoMenuCommand (lCommandId))
			&&	(lCommand = lCommands->GetCommand (lCommandId))
			&&	(lCommand->mEnabled)
			)
		{
			PreNotify ();
			try
			{
				mNotify.Command ((long)lCommandId, NULL);
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::DoDefaultCommand (HWND pOwner, const CPoint & pPosition)
{
//
//	MS Agent leaves default command processing to the calling application
//
#ifndef	_STRICT_COMPATIBILITY
	CDaAgentCommands *	lCommands;
	USHORT				lCommand;

	if	(
			(lCommands = GetCommandsObj (true))
		&&	(lCommand = lCommands->mDefaultId)
		)
	{
		PreNotify ();
		try
		{
			mNotify.Command ((long)lCommand, NULL);
		}
		catch AnyExceptionDebug
		PostNotify ();
		return true;
	}
#endif
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgentCharacter::DoMenuCommand (USHORT pCommandId)
{
	bool				lRet = false;
	CDaAgentCommands *	lCommands;

	if	(lCommands = GetCommandsObj (true))
	{
		if	(pCommandId == lCommands->mHideCharacterCmdId)
		{
			StopAll (STOP_TYPE_PLAY|STOP_TYPE_MOVE|STOP_TYPE_SPEAK|STOP_TYPE_PREPARE, AGENTREQERR_INTERRUPTEDUSER);
			mWnd->QueueHide (mCharID);
			mWnd->ActivateQueue (true);
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCharacterCmdId)
		{
			StopAll (STOP_TYPE_PLAY|STOP_TYPE_MOVE|STOP_TYPE_SPEAK|STOP_TYPE_PREPARE, AGENTREQERR_INTERRUPTEDUSER);
			mWnd->QueueShow (mCharID);
			mWnd->ActivateQueue (true);
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mHideCommandsCmdId)
		{
			CVoiceCommandsWnd *	lVoiceCommandsWnd;

			if	(lVoiceCommandsWnd = TheServerApp->GetVoiceCommandsWnd (false))
			{
				lVoiceCommandsWnd->Hide ();
			}
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCommandsCmdId)
		{
			CVoiceCommandsWnd *	lVoiceCommandsWnd;

			if	(lVoiceCommandsWnd = TheServerApp->GetVoiceCommandsWnd (true, mCharID))
			{
				lVoiceCommandsWnd->SetLangID (mLangID);
				lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
				lVoiceCommandsWnd->Show ();
			}
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAgentCommands * CDaAgentCharacter::GetCommandsObj (bool pCreateObject)
{
	CDaAgentCommands *	lRet = NULL;
	IDaSvrCommandsPtr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetControllingUnknown ();
	}
	if	(mAgentCommands != NULL)
	{
		lRet = (CDaAgentCommands *)CCmdTarget::FromIDispatch (mAgentCommands);
	}
	return lRet;
}

CDaAgentBalloon * CDaAgentCharacter::GetBalloonObj (bool pCreateObject)
{
	CDaAgentBalloon *	lRet = NULL;
	IDaSvrBalloonPtr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetControllingUnknown ();
	}
	if	(mAgentBalloon != NULL)
	{
		lRet = (CDaAgentBalloon *)CCmdTarget::FromIDispatch (mAgentBalloon);
	}
	return lRet;
}

IDaSvrBalloon * CDaAgentCharacter::GetBalloonInterface (bool pCreateObject)
{
	IDaSvrBalloonPtr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetControllingUnknown ();
	}
	else
	{
		lInterface = mAgentBalloon;
	}
	return lInterface;
}

/////////////////////////////////////////////////////////////////////////////

CAgentPopupWnd * CDaAgentCharacter::GetAgentWnd ()
{
	return mWnd;
}

CAgentBalloonWnd * CDaAgentCharacter::GetBalloonWnd (bool pCreateObject)
{
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	(mWnd->GetSafeHwnd ())
	{
		lBalloonWnd = mWnd->GetBalloonWnd ();

		if	(
				(!lBalloonWnd->GetSafeHwnd ())
			&&	(pCreateObject)
			)
		{
			IDaSvrBalloonPtr	lBalloonInterface (GetControllingUnknown ());

			if	(
					(lBalloonInterface->GetEnabled (NULL) == S_OK)
				&&	(lBalloonWnd = mWnd->GetBalloonWnd (true))
				&&	(lBalloonWnd->GetSafeHwnd ())
				)
			{
				mWnd->GetBalloonWnd()->SetOptions (mFile->GetBalloon(), lBalloonInterface, mLangID);
			}
		}

		if	(!lBalloonWnd->GetSafeHwnd ())
		{
			lBalloonWnd = NULL;
		}
	}
	return lBalloonWnd;
}

CAgentListeningWnd * CDaAgentCharacter::GetListeningWnd (bool pCreateObject)
{
	CAgentListeningWnd *	lListeningWnd = NULL;

	if	(
			(mWnd->GetSafeHwnd ())
		&&	(
				(!pCreateObject)
			||	(CDaSpeechInputConfig().LoadConfig().mListeningTip)
			)
		)
	{
		lListeningWnd = mWnd->GetListeningWnd (pCreateObject);

		if	(lListeningWnd->GetSafeHwnd ())
		{
			if	(pCreateObject)
			{
				lListeningWnd->Attach (mCharID, CString (GetName()));
				lListeningWnd->SetLangID (mLangID);
			}
		}
		else
		{
			lListeningWnd = NULL;
		}
	}
	return lListeningWnd;
}

/////////////////////////////////////////////////////////////////////////////

LPVOID CDaAgentCharacter::FindOtherRequest (long pReqID, CDaAgentCharacter *& pOtherCharacter)
{
	LPVOID			lRet = NULL;
	CAgentFile *	lFile;
	INT_PTR			lFileNdx;

	pOtherCharacter = NULL;

	for	(lFileNdx = 0; lFile = mUsedFileCache.GetCachedFile (lFileNdx); lFileNdx++)
	{
		CObTypeArray <CObject>	lFileClients;
		INT_PTR					lClientNdx;
		CDaAgentCharacter *		lCharacter;

		if	(mUsedFileCache.GetFileClients (lFile, lFileClients))
		{
			for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
			{
				if	(
						(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
					&&	(lCharacter->mWnd)
					&&	(lRet = lCharacter->mWnd->FindQueuedAction (pReqID))
					)
				{
					pOtherCharacter = lCharacter;
					break;
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCharacter::_OnOptionsChanged ()
{
	CDaSpeechInputConfig	lInputConfig;
	CAgentListeningWnd *	lListeningWnd;
	CAgentBalloonWnd *		lBalloonWnd;

	lInputConfig.LoadConfig ();

	if	(!lInputConfig.mEnabled)
	{
		StopListening (false, LSCOMPLETE_CAUSE_USERDISABLED);
		ReleaseSapiVoice ();
	}
	if	(
			(!lInputConfig.mListeningTip)
		&&	(lListeningWnd = GetListeningWnd (false))
		)
	{
		lListeningWnd->Detach (mCharID);
	}

	if	(
			(mWnd->GetSafeHwnd())
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		mWnd->EnableSound (mWnd->IsSoundEnabled());
	}

	if	(
			(mFile)
		&&	(lBalloonWnd = GetBalloonWnd (false))
		&&	(lBalloonWnd->GetCharID() == GetCharID())
		)
	{
		IDaSvrBalloonPtr	lBalloonInterface (GetControllingUnknown ());

		if	(lBalloonInterface->GetEnabled (NULL) == S_FALSE)
		{
			lBalloonWnd->HideBalloon (true);
			mWnd->RemoveQueuedThink (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("OptionsChanged"));
		}
		lBalloonWnd->SetOptions (mFile->GetBalloon(), lBalloonInterface, mLangID);
	}
}

void CDaAgentCharacter::_OnDefaultCharacterChanged ()
{
	StopListening (false, LSCOMPLETE_CAUSE_DEFAULTCHARCHANGE);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentCharacter, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentCharacter)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetVisible", DISPID_IAgentCharacter_GetVisible, DspGetVisible, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetPosition", DISPID_IAgentCharacter_SetPosition, DspSetPosition, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetPosition", DISPID_IAgentCharacter_GetPosition, DspGetPosition, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetSize", DISPID_IAgentCharacter_SetSize, DspSetSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetSize", DISPID_IAgentCharacter_GetSize, DspGetSize, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetName", DISPID_IAgentCharacter_GetName, DspGetName, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetDescription", DISPID_IAgentCharacter_GetDescription, DspGetDescription, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetTTSSpeed", DISPID_IAgentCharacter_GetTTSSpeed, DspGetTTSSpeed, VT_EMPTY, VTS_PI2)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetTTSPitch", DISPID_IAgentCharacter_GetTTSPitch, DspGetTTSPitch, VT_EMPTY, VTS_PI2)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Activate", DISPID_IAgentCharacter_Activate, DspActivate, VT_EMPTY, VTS_I2)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetIdleOn", DISPID_IAgentCharacter_SetIdleOn, DspSetIdleOn, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetIdleOn", DISPID_IAgentCharacter_GetIdleOn, DspGetIdleOn, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Prepare", DISPID_IAgentCharacter_Prepare, DspPrepare, VT_EMPTY, VTS_I4 VTS_BSTR VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Play", DISPID_IAgentCharacter_Play, DspPlay, VT_EMPTY, VTS_BSTR VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Stop", DISPID_IAgentCharacter_Stop, DspStop, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "StopAll", DISPID_IAgentCharacter_StopAll, DspStopAll, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Wait", DISPID_IAgentCharacter_Wait, DspWait, VT_EMPTY, VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Interrupt", DISPID_IAgentCharacter_Interrupt, DspInterrupt, VT_EMPTY, VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Show", DISPID_IAgentCharacter_Show, DspShow, VT_EMPTY, VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Hide", DISPID_IAgentCharacter_Hide, DspHide, VT_EMPTY, VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Speak", DISPID_IAgentCharacter_Speak, DspSpeak, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "MoveTo", DISPID_IAgentCharacter_MoveTo, DspMoveTo, VT_EMPTY, VTS_I2 VTS_I2 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GestureAt", DISPID_IAgentCharacter_GestureAt, DspGestureAt, VT_EMPTY, VTS_I2 VTS_I2 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetMoveCause", DISPID_IAgentCharacter_GetMoveCause, DspGetMoveCause, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetVisibilityCause", DISPID_IAgentCharacter_GetVisibilityCause, DspGetVisibilityCause, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "HasOtherClients", DISPID_IAgentCharacter_HasOtherClients, DspHasOtherClients, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetSoundEffectsOn", DISPID_IAgentCharacter_SetSoundEffectsOn, DspSetSoundEffectsOn, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetSoundEffectsOn", DISPID_IAgentCharacter_GetSoundEffectsOn, DspGetSoundEffectsOn, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetName", DISPID_IAgentCharacter_SetName, DspSetName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetDescription", DISPID_IAgentCharacter_SetDescription, DspSetDescription, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetExtraData", DISPID_IAgentCharacter_GetExtraData, DspGetExtraData, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "ShowPopupMenu", DISPID_IAgentCharacterEx_ShowPopupMenu, DspShowPopupMenu, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetAutoPopupMenu", DISPID_IAgentCharacterEx_SetAutoPopupMenu, DspSetAutoPopupMenu, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetAutoPopupMenu", DISPID_IAgentCharacterEx_GetAutoPopupMenu, DspGetAutoPopupMenu, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetHelpFileName", DISPID_IAgentCharacterEx_GetHelpFileName, DspGetHelpFileName, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetHelpFileName", DISPID_IAgentCharacterEx_SetHelpFileName, DspSetHelpFileName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetHelpModeOn", DISPID_IAgentCharacterEx_SetHelpModeOn, DspSetHelpModeOn, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetHelpModeOn", DISPID_IAgentCharacterEx_GetHelpModeOn, DspGetHelpModeOn, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetHelpContextID", DISPID_IAgentCharacterEx_SetHelpContextID, DspSetHelpContextID, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetHelpContextID", DISPID_IAgentCharacterEx_GetHelpContextID, DspGetHelpContextID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetActive", DISPID_IAgentCharacterEx_GetActive, DspGetActive, VT_EMPTY, VTS_PI2)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Listen", DISPID_IAgentCharacterEx_Listen, DspListen, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetLanguageID", DISPID_IAgentCharacterEx_SetLanguageID, DspSetLanguageID, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetLanguageID", DISPID_IAgentCharacterEx_GetLanguageID, DspGetLanguageID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetTTSModeID", DISPID_IAgentCharacterEx_GetTTSModeID, DspGetTTSModeID, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetTTSModeID", DISPID_IAgentCharacterEx_SetTTSModeID, DspSetTTSModeID, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetSRModeID", DISPID_IAgentCharacterEx_GetSRModeID, DspGetSRModeID, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "SetSRModeID", DISPID_IAgentCharacterEx_SetSRModeID, DspSetSRModeID, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetGUID", DISPID_IAgentCharacterEx_GetGUID, DspGetGUID, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetOriginalSize", DISPID_IAgentCharacterEx_GetOriginalSize, DspGetOriginalSize, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "Think", DISPID_IAgentCharacterEx_Think, DspThink, VT_EMPTY, VTS_BSTR VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetVersion", DISPID_IAgentCharacterEx_GetVersion, DspGetVersion, VT_EMPTY, VTS_PI2 VTS_PI2)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetAnimationNames", DISPID_IAgentCharacterEx_GetAnimationNames, DspGetAnimationNames, VT_EMPTY, VTS_PUNKNOWN)
	DISP_FUNCTION_ID(CDaAgentCharacter, "GetSRStatus", DISPID_IAgentCharacterEx_GetSRStatus, DspGetSRStatus, VT_EMPTY, VTS_PI4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentCharacter, CCmdTarget)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(IDaSvrCharacter), Character)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(IAgentCharacter), Character)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(IAgentCharacterEx), Character)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAgentCharacter, __uuidof(IStdMarshalInfo), StdMarshalInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentCharacter, Character)
IMPLEMENT_DISPATCH_IID(CDaAgentCharacter, __uuidof(IDaSvrCharacter))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentCharacter, __uuidof(IDaSvrCharacter))
IMPLEMENT_IUNKNOWN(CDaAgentCharacter, StdMarshalInfo)

BEGIN_SUPPORTERRORINFO(CDaAgentCharacter)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCharacter, __uuidof(IDaSvrCharacter))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCharacter, __uuidof(IAgentCharacter))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCharacter, __uuidof(IAgentCharacterEx))
END_SUPPORTERRORINFO(CDaAgentCharacter)

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XStdMarshalInfo::GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid)
{
	METHOD_PROLOGUE(CDaAgentCharacter, StdMarshalInfo)
	if	(!pClsid)
	{
		return E_POINTER;
	}
	*pClsid = __uuidof(DaServerHandler);
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CDaAgentCharacter::XStdMarshalInfo::GetClassForHandler [%8.8X] [%s]"), pThis, pThis->m_dwRef, dwDestContext, CGuidStr::GuidName(*pClsid));
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetPosition (long lLeft, long lTop)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetPosition [%d %d]"), pThis, pThis->m_dwRef, pThis->mCharID, lLeft, lTop);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetPosition"), _T("%d\t%d"), lLeft, lTop);
#endif
#ifdef	_STRICT_COMPATIBILITY
		if	(!pThis->mWnd->MovePopup (CPoint (lLeft, lTop), 0, ProgramMoved, true))
#else
		if	(!pThis->mWnd->MovePopup (CPoint (lLeft, lTop), 0, ProgramMoved))
#endif
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetPosition"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetPosition (long *plLeft, long *plTop)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetPosition"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		pThis->mWnd->GetWindowRect (&lWindowRect);
		if	(plLeft)
		{
			(*plLeft) = lWindowRect.left;
		}
		if	(plTop)
		{
			(*plTop) = lWindowRect.top;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetPosition"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetSize (long lWidth, long lHeight)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetSize"), _T("%d\t%d"), lWidth, lHeight);
#endif
		if	(!pThis->mWnd->SizePopup (CSize (lWidth, lHeight), 0))
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = S_FALSE;
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetSize (long *plWidth, long *plHeight)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		pThis->mWnd->GetWindowRect (&lWindowRect);
		if	(plWidth)
		{
			(*plWidth) = lWindowRect.Width();
		}
		if	(plHeight)
		{
			(*plHeight) = lWindowRect.Height();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetOriginalSize (long *plWidth, long *plHeight)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetOriginalSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		CSize	lImageSize = pThis->mFile->GetImageSize ();

		if	(plWidth)
		{
			(*plWidth) = lImageSize.cx;
		}
		if	(plHeight)
		{
			(*plHeight) = lImageSize.cy;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetOriginalSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetGUID (BSTR *pbszID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetGUID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszID)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*pbszID) = ((CString)CGuidStr (pThis->mFile->GetGuid())).AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetGUID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetVersion(short *psMajor, short *psMinor)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetVersion"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		if	(psMajor)
		{
			(*psMajor) = HIWORD (pThis->mFile->GetVersion());
		}
		if	(psMinor)
		{
			(*psMinor) = LOWORD (pThis->mFile->GetVersion());
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetVersion"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetName (BSTR *pbszName)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!pbszName)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(pThis->mFile->GetNames().GetSize() <= 0)
			&&	(!pThis->mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = pThis->mFile->FindName (pThis->mLangID))
		{
			(*pbszName) = tBstrPtr (lFileName->mName).Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetName (BSTR bszName)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CString				lName (bszName);
	CAgentFileName *	lFileName;

	lName.TrimLeft ();
	lName.TrimRight ();

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetName"), _T("%ls"), bszName);
#endif
		if	(lFileName = pThis->mFile->FindName (pThis->mLangID))
		{
			if	(CString ((BSTR)lFileName->mName) != lName)
			{
				lFileName->mName = lName.AllocSysString ();
				TheServerApp->_OnCharacterNameChanged (pThis->mCharID);
			}
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetDescription (BSTR *pbszDescription)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetDescription"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!pbszDescription)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(pThis->mFile->GetNames().GetSize() <= 0)
			&&	(!pThis->mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = pThis->mFile->FindName (pThis->mLangID))
		{
			(*pbszDescription) = tBstrPtr (lFileName->mDesc1).Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetDescription"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetDescription (BSTR bszDescription)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetDescription"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CString				lDescription (bszDescription);
	CAgentFileName *	lFileName;

	lDescription.TrimLeft ();
	lDescription.TrimRight ();

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetDescription"), _T("%ls"), bszDescription);
#endif
		if	(lFileName = pThis->mFile->FindName (pThis->mLangID))
		{
			lFileName->mDesc1 = lDescription.AllocSysString ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetDescription"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetLanguageID (long langid)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetLanguageID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
#ifdef	_DEBUG_LANGUAGE
	LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaAgentCharacter SetLanguageID [%4.4hX]"), pThis, pThis->mCharID, langid);
#endif
	HRESULT	lResult = S_OK;
	LANGID	lLangID = LOWORD (langid);

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetLanguageID"), _T("%d"), langid);
#endif
		if	(pThis->mLangID != lLangID)
		{
			lResult = pThis->SetLangID (lLangID);
			if	(SUCCEEDED (lResult))
			{
				TheServerApp->_OnCharacterNameChanged (pThis->mCharID);
			}
		}
#ifndef	_STRICT_COMPATIBILITY
		else
		{
			lResult = S_FALSE;
		}
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetLanguageID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetLanguageID (long *plangid)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetLanguageID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plangid)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*plangid) = (long)pThis->mLangID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetLanguageID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetIdleOn (long bOn)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetIdleOn [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bOn);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetIdleOn"), _T("%d"), bOn);
#endif
		pThis->mIdleOn = (bOn!=FALSE);
		if	(
				(pThis->mWnd->GetSafeHwnd())
			&&	(pThis->mWnd->GetCharID() == pThis->mCharID)
			)
		{
			pThis->mWnd->EnableIdle (pThis->mIdleOn);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetIdleOn"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetIdleOn (long *pbOn)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetIdleOn"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbOn)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*pbOn) = (pThis->mIdleOn!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetIdleOn"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetSoundEffectsOn (long bOn)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetSoundEffectsOn [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bOn);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetSoundEffectsOn"), _T("%d"), bOn);
#endif
		if	(pThis->mWnd->GetSafeHwnd())
		{
			if	(pThis->mWnd->EnableSound (bOn!=FALSE))
			{
				lResult = S_OK;
			}
#ifdef	_STRICT_COMPATIBILITY
			lResult = S_OK;
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetSoundEffectsOn [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bOn);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetSoundEffectsOn (long *pbOn)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSoundEffectsOn"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbOn)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(pThis->mWnd->GetSafeHwnd())
	{
		(*pbOn) = (pThis->mWnd->IsSoundEnabled()!=false);
	}
	else
	{
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSoundEffectsOn"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetExtraData (BSTR *pbszExtraData)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetExtraData"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!pbszExtraData)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(pThis->mFile->GetNames().GetSize() <= 0)
			&&	(!pThis->mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = pThis->mFile->FindName (pThis->mLangID))
		{
			(*pbszExtraData) = tBstrPtr (lFileName->mDesc2).Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetExtraData"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetAnimationNames (IUnknown **punkEnum)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetAnimationNames"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CDaAnimationNames *	lAnimationNames;

	if	(!punkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*punkEnum) = NULL;

		if	(!pThis->mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lAnimationNames = new CDaAnimationNames (*pThis->mFile))
		{
			(*punkEnum) = lAnimationNames->GetControllingUnknown ();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetAnimationNames"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Activate (short sState)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Activate [%hd]"), pThis, pThis->m_dwRef, pThis->mCharID, sState);
#endif
#ifdef	_DEBUG_ACTIVE
	LogMessage (_DEBUG_ACTIVE, _T("[%d] Activate [%hd]"), pThis->mCharID, sState);
#endif
	HRESULT	lResult = S_OK;

	if	(sState == ACTIVATE_NOTACTIVE)
	{
		if	(!pThis->SetClientActive (false, false))
		{
			if	(pThis->mWnd->GetSafeHwnd())
			{
				lResult = S_FALSE;
			}
			else
			{
				lResult = E_INVALIDARG;
			}
		}
	}
	else
	if	(!pThis->mWnd->GetSafeHwnd())
	{
#ifdef	_STRICT_COMPATIBILITY
		lResult = E_INVALIDARG;
#else
		lResult = AGENTERR_CHARACTERINVALID;
#endif
	}
	else
	if	(sState == ACTIVATE_ACTIVE)
	{
		pThis->SetClientActive (true, false);
	}
	else
	if	(sState == ACTIVATE_INPUTACTIVE)
	{
		if	(pThis->mWnd->GetStyle() & WS_VISIBLE)
		{
			pThis->SetClientActive (true, true);
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Activate"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetActive (short *psState)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetActive"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	short	lState = ACTIVATE_NOTACTIVE;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		if	(pThis->IsClientActive ())
		{
			if	(pThis->mWnd->GetSafeHwnd() == CAgentPopupWnd::GetLastActive())
			{
				lState = ACTIVATE_INPUTACTIVE;
			}
			else
			{
				lState = ACTIVATE_ACTIVE;
			}
		}
		else
		{
			lResult = S_FALSE;
		}
		if	(psState)
		{
			(*psState) = lState;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetActive"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::HasOtherClients (long *plNumOtherClients)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::HasOtherClients"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	long	lClientCount = 0;

	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lClientCount = pThis->GetClientCount (pThis->mCharID);
		if	(lClientCount == 0)
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTWARNING_ONLYCLIENT;
#endif
		}
		else
		if	(lClientCount < 0)
		{
			lClientCount = 0;
			lResult = E_FAIL;
		}
		if	(plNumOtherClients)
		{
			(*plNumOtherClients) = lClientCount;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::HasOtherClients"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Show (long bFast, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Show [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bFast);
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lReqID = pThis->Show (bFast!=0);
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Show"), _T("%d\t%d"), bFast, lReqID);
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Show"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Hide (long bFast, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Hide [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bFast);
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lReqID = pThis->Hide (bFast!=0);
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Hide"), _T("%d\t%d"), bFast, lReqID);
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Hide"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetVisible (long *pbVisible)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbVisible)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*pbVisible) = pThis->IsVisible (true) ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetVisibilityCause (long *pdwCause)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetVisibilityCause"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCause)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*pdwCause) = pThis->mNotify._GetVisibilityCause (pThis->mCharID);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetVisibilityCause"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Prepare (long dwType, BSTR bszName, long bQueue, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Prepare [%d] [%ls] [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, dwType, bszName, bQueue);
#endif
	HRESULT	lResult;
	long	lReqID = 0;

	if	(!bszName)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lResult = pThis->DoPrepare (dwType, CString (bszName), (bQueue != 0), lReqID);
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Prepare"), _T("%d\t%ls\t%d\t%d"), dwType, bszName, bQueue, lReqID);
#endif
	}

	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Prepare"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Play (BSTR bszAnimation, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Play [%s]"), pThis, pThis->m_dwRef, pThis->mCharID, DebugStr(CString(bszAnimation)));
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!bszAnimation)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lReqID = pThis->mWnd->QueueGesture (pThis->mCharID, bszAnimation);
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Play"), _T("%ls\t%d"), bszAnimation, lReqID);
#endif
		if	(lReqID)
		{
			pThis->mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Play"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Stop (long dwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Stop"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	LPVOID				lRequest = NULL;
	CDaAgentCharacter *	lOtherCharacter = NULL;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(dwReqID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Stop"), _T("%d"), dwReqID);
#endif
		if	(lRequest = pThis->mWnd->FindQueuedAction (dwReqID))
		{
			if	(!pThis->mWnd->RemoveQueuedAction ((CQueuedAction *) lRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Stop")))
			{
				lResult = AGENTREQERR_REMOVED;
			}
		}
		else
		if	(
				(lRequest = pThis->FindOtherRequest (dwReqID, lOtherCharacter))
			&&	(lOtherCharacter != pThis)
			)
		{
			lResult = AGENTREQERR_CANTSTOPOTHERS;
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Stop"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::StopAll (long lTypes)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::StopAll [%8.8X]"), pThis, pThis->m_dwRef, pThis->mCharID, lTypes);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("StopAll"), _T("0x%8.8X"), lTypes);
#endif
		lResult = pThis->StopAll (lTypes, AGENTREQERR_INTERRUPTEDCODE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::StopAll"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::MoveTo (short x, short y, long lSpeed, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::MoveTo"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(pThis->mWnd->IsDragging())
	{
		lResult = AGENTERR_CANTMOVEDURINGDRAG;
	}
	else
	if	(lSpeed > 0)
	{
		lReqID = pThis->mWnd->QueueMove (pThis->mCharID, CPoint (x, y), lSpeed);
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("MoveTo"), _T("%hd\t%hd\t%d\t%d"), x, y, lSpeed, lReqID);
#endif
		if	(lReqID)
		{
			pThis->mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = SetPosition (x, y);
		}
	}
	else
	{
		lResult = SetPosition (x, y);
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::MoveTo"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetMoveCause (long *pdwCause)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetMoveCause"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCause)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*pdwCause) = pThis->mNotify._GetMoveCause (pThis->mCharID);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetMoveCause"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GestureAt (short x, short y, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GestureAt [%hd %hd]"), pThis, pThis->m_dwRef, pThis->mCharID, x, y);
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		CPoint	lOffset (x, y);
		CRect	lWinRect;
		CString	lStateName;

		pThis->mWnd->GetWindowRect (&lWinRect);
		lOffset -= lWinRect.CenterPoint ();

		if	(lOffset.x < 0)
		{
			if	(lOffset.y < lOffset.x)
			{
				lStateName = _T("GESTURINGUP");
			}
			else
			if	(lOffset.y > -lOffset.x)
			{
				lStateName = _T("GESTURINGDOWN");
			}
			else
			{
				lStateName = _T("GESTURINGRIGHT");
			}
		}
		else
		{
			if	(lOffset.y > lOffset.x)
			{
				lStateName = _T("GESTURINGDOWN");
			}
			else
			if	(lOffset.y < -lOffset.x)
			{
				lStateName = _T("GESTURINGUP");
			}
			else
			{
				lStateName = _T("GESTURINGLEFT");
			}
		}

		lReqID = pThis->mWnd->QueueState (pThis->mCharID, lStateName);
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("GestureAt"), _T("%hd\t%hd\t%d"), x, y, lReqID);
#endif
		if	(lReqID)
		{
			pThis->mWnd->ActivateQueue (true);
		}
#ifndef	_STRICT_COMPATIBILITY
		else
		{
			lResult = AGENTERR_STATENOTFOUND;
		}
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GestureAt"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Think (BSTR bszText, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Think [%s]"), pThis, pThis->m_dwRef, pThis->mCharID, DebugStr(bszText));
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		IDaSvrBalloonPtr	lAgentBalloon (pThis->GetControllingUnknown ());

		if	(lAgentBalloon->GetEnabled (NULL) == S_OK)
		{
			pThis->GetBalloonWnd (true);
			lReqID = pThis->mWnd->QueueThink (pThis->mCharID, CString (bszText));
			pThis->mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_NOBALLOON;
		}
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Think"), _T("%s\t%d"), EncodeTraceString(bszText), lReqID);
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Think"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Wait (long dwReqID, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Wait"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	LPVOID				lOtherRequest = NULL;
	CDaAgentCharacter *	lOtherCharacter = NULL;
	long				lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(dwReqID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
		if	(lOtherRequest = pThis->FindOtherRequest (dwReqID, lOtherCharacter))
		{
			if	(
					(lOtherCharacter == pThis)
				||	(lOtherCharacter->mWnd == pThis->mWnd)
				)
			{
				lResult = AGENTREQERR_CANTWAITONSELF;
			}
			else
			{
				lReqID = pThis->mWnd->QueueWait (pThis->mCharID, lOtherCharacter->mCharID, dwReqID);
				pThis->mWnd->ActivateQueue (true);
			}
#ifdef	_TRACE_CHARACTER_ACTIONS
			TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Wait"), _T("%d\t%d"), dwReqID, lReqID);
#endif
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Wait"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Interrupt (long dwReqID, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Interrupt"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	LPVOID				lOtherRequest = NULL;
	CDaAgentCharacter *	lOtherCharacter = NULL;
	long				lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(dwReqID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
		if	(lOtherRequest = pThis->FindOtherRequest (dwReqID, lOtherCharacter))
		{
			if	(
					(lOtherCharacter == pThis)
				||	(lOtherCharacter->mWnd == pThis->mWnd)
				)
			{
				lResult = AGENTREQERR_CANTINTERRUPTSELF;
			}
			else
			{
				lReqID = pThis->mWnd->QueueInterrupt (pThis->mCharID, lOtherCharacter->mCharID, dwReqID);
				pThis->mWnd->ActivateQueue (true);
			}
#ifdef	_TRACE_CHARACTER_ACTIONS
			TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Interrupt"), _T("%d\t%d"), dwReqID, lReqID);
#endif
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Interrupt"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::ShowPopupMenu (short x, short y)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::ShowPopupMenu"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(pThis->mNotify._GetActiveCharacter() != pThis->GetCharID())
	{
		lResult = AGENTERR_CHARACTERNOTACTIVE;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("ShowPopupMenu"), _T("%hd\t%hd"), x, y);
#endif
		if	(!pThis->DoContextMenu (pThis->mWnd->GetSafeHwnd(), CPoint (x, y)))
		{
			lResult = S_FALSE;
		}
	}
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::ShowPopupMenu"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetAutoPopupMenu (long bAutoPopupMenu)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetAutoPopupMenu [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bAutoPopupMenu);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
#ifndef	_STRICT_COMPATIBILITY
	if	(pThis->mAutoPopupMenu == (bAutoPopupMenu != FALSE))
	{
		lResult = S_FALSE;
	}
	else
#endif
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetAutoPopupMenu"), _T("%d"), bAutoPopupMenu);
#endif
		pThis->mAutoPopupMenu = (bAutoPopupMenu != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetAutoPopupMenu"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetAutoPopupMenu (long *pbAutoPopupMenu)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetAutoPopupMenu"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = pThis->mAutoPopupMenu ? S_OK : S_FALSE;
#ifdef _STRICT_COMPATIBILITY
	lResult = S_OK;
#endif

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(pbAutoPopupMenu)
	{
		(*pbAutoPopupMenu) = (pThis->mAutoPopupMenu!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetAutoPopupMenu"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetHelpFileName (BSTR *pbszName)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetHelpFileName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
	if	(pbszName)
	{
		(*pbszName) = NULL;
	}
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetHelpFileName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetHelpFileName (BSTR bszName)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetHelpFileName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetHelpFileName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetHelpModeOn (long bHelpModeOn)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetHelpModeOn"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetHelpModeOn"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetHelpModeOn (long *pbHelpModeOn)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetHelpModeOn"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
	if	(pbHelpModeOn)
	{
		(*pbHelpModeOn) = FALSE;
	}
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetHelpModeOn"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetHelpContextID (long ulID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetHelpContextID (long *pulID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
	if	(pulID)
	{
		(*pulID) = 0;
	}
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetTTSSpeed (long *pdwSpeed)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetTTSSpeed"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwSpeed)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
#ifndef	_WIN64
	if	(
			(pThis->GetSapiVoice (true))
		&&	(pThis->mSapiVoice->SafeIsValid () == 4)
		)
	{
		(*pdwSpeed) = CDaAudioOutputConfig().ApplyVoiceRate (pThis->mFile->GetTts().mSpeed, 4);
	}
	else
#endif
	{
		(*pdwSpeed) = CDaAudioOutputConfig().ApplyVoiceRate (pThis->mFile->GetTts().mSpeed);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetTTSSpeed"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetTTSPitch (short *pwPitch)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetTTSPitch"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pwPitch)
	{
		lResult = E_POINTER;
	}
	else
	if	(!pThis->mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
#ifndef	_WIN64
	if	(
			(pThis->GetSapiVoice (true))
		&&	(pThis->mSapiVoice->SafeIsValid () == 4)
		)
	{
		(*pwPitch) = pThis->mSapiVoice->GetPitch ();
	}
	else
#endif
	{
		(*pwPitch) = pThis->mFile->GetTts().mPitch;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetTTSPitch"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetTTSModeID (BSTR *pbszModeID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetTTSModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->GetSapiVoice (true))
	{
		(*pbszModeID) = pThis->mSapiVoice->GetUniqueId().Detach();
	}
	else
	{
		(*pbszModeID) = CString().AllocSysString();
#ifndef	_STRICT_COMPATIBILITY
		lResult = AGENTWARNING_TTSENGINENOTFOUND;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetTTSModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetTTSModeID (BSTR bszModeID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetTTSModeID [%ls]"), pThis, pThis->m_dwRef, pThis->mCharID, bszModeID);
#endif
	HRESULT				lResult = E_FAIL;
	CSapiVoiceCache *	lVoiceCache;

	if	(!CDaAudioOutputConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		try
		{
			CSapiVoice *	lPrevVoice = pThis->mSapiVoice;

#ifdef	_TRACE_CHARACTER_ACTIONS
			TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetTTSModeID"), _T("%ls"), bszModeID);
#endif
#ifdef	_STRICT_COMPATIBILITY
			try
			{
				if	(
						(pThis->mWnd->NextQueuedAction (pThis->mCharID))
					&&	(pThis->mWnd->IsSpeakQueued (pThis->mCharID) == pThis->mWnd->NextQueuedAction (pThis->mCharID))
					)
				{
					pThis->mWnd->RemoveQueuedAction (pThis->mWnd->NextQueuedAction (pThis->mCharID), AGENTREQERR_INTERRUPTEDCODE, _T("SetTTSModeID"));
				}
				if	(pThis->mSapiVoice)
				{
					pThis->mSapiVoice->Stop ();
				}
			}
			catch AnyExceptionDebug
#endif
			pThis->mSapiVoice = NULL;

			if	(pThis->GetSapiVoice (true, CString (bszModeID)))
			{
				pThis->mWnd->UpdateQueuedSpeak (pThis->mCharID, pThis->mSapiVoice);

				if	(
						(lPrevVoice)
					&&	(lPrevVoice != pThis->mSapiVoice)
					)
				{
					lVoiceCache->RemoveVoiceClient (lPrevVoice, pThis);
				}
				lResult = S_OK;
			}
			else
			{
				pThis->mSapiVoice = lPrevVoice;
#ifdef	_STRICT_COMPATIBILITY_NOT
				lResult = AGENTERR_TTSLANGUAGENOTFOUND;
#else
				lResult = AGENTWARNING_TTSENGINENOTFOUND;
#endif
			}
		}
		catch AnyExceptionDebug
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetTTSModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Speak (BSTR bszText, BSTR bszUrl, long *pdwReqID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Speak [%s] [%s]"), pThis, pThis->m_dwRef, pThis->mCharID, DebugStr(CString(bszText)), DebugStr(CString(bszUrl)));
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!pThis->mWnd->GetSafeHwnd())
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!CDaAudioOutputConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
	if	(
			(
				(bszUrl == NULL)
			||	(*bszUrl == NULL)
			)
		&&	(
				(bszText == NULL)
			||	(*bszText == NULL)
			)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		IDaSvrBalloonPtr	lAgentBalloon (pThis->GetControllingUnknown ());
		bool				lShowBalloon = (lAgentBalloon->GetEnabled (NULL) == S_OK);
		CString				lText (bszText);
		CString				lSoundUrl (bszUrl);
		CSapiVoice *		lVoice = NULL;
//
//	MS Agent shows the speech balloon silently when the character is listening.
//	For now, we'll just stop listening.
//
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("PreSpeak"), _T("%s\t%s"), EncodeTraceString(bszText), DebugStr(bszUrl));
#endif
		pThis->StopListening (false, LSCOMPLETE_CAUSE_CLIENTDEACTIVATED);

		if	(lSoundUrl.IsEmpty ())
		{
			lVoice = pThis->GetSapiVoice (true);
			if	(!lVoice)
			{
				lResult = AGENTERR_TTSLANGUAGENOTFOUND;
			}
		}
		else
		{
			lVoice = pThis->GetSapiVoice (false);
			if	(lText.IsEmpty ())
			{
				lShowBalloon = false;
			}

			if	(pThis->mFile->IsRelativeFilePath (lSoundUrl))
			{
				lSoundUrl = pThis->mFile->ParseRelativePath (lSoundUrl);
			}
			else
			{
				lSoundUrl = pThis->mFile->ParseFilePath (lSoundUrl);
			}
			if	(pThis->mFile->IsProperFilePath (lSoundUrl))
			{
				if	(
						(PathIsURL (lSoundUrl))
					&&	(!pThis->_FindSoundDownload (lSoundUrl))
					)
				{
					lResult = pThis->DoPrepare (PREPARE_WAVE, lSoundUrl, false, lReqID);
				}
			}
			else
			{
				lResult = E_INVALIDARG;
			}
		}

		if	(SUCCEEDED (lResult))
		{
			if	(lShowBalloon)
			{
				pThis->GetBalloonWnd (true);
			}
			lReqID = pThis->mWnd->QueueSpeak (pThis->mCharID, lText, lSoundUrl, lVoice, lShowBalloon);
#ifdef	_TRACE_CHARACTER_ACTIONS
			TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Speak"), _T("%s\t%s\t%d"), EncodeTraceString(bszText), DebugStr(bszUrl), lReqID);
#endif
			pThis->mWnd->ActivateQueue (true);
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Speak"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetSRModeID (BSTR *pbszModeID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSRModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->GetSapiInput (true))
	{
		(*pbszModeID) = pThis->mSapiInput->GetEngineName().Detach();
	}
	else
	{
		(*pbszModeID) = CString().AllocSysString();
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSRModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::SetSRModeID (BSTR bszModeID)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetSRModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = E_FAIL;
	CSapiInputCache *	lInputCache;

	if	(!CDaAudioOutputConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
	if	(lInputCache = CSapiInputCache::GetStaticInstance ())
	{
		try
		{
			CSapi5Input *	lPrevInput = pThis->mSapiInput;

#ifdef	_TRACE_CHARACTER_ACTIONS
			TheServerApp->TraceCharacterAction (pThis->mCharID, _T("SetSRModeID"), _T("%ls"), bszModeID);
#endif
			pThis->mSapiInput = NULL;

			if	(pThis->GetSapiInput (true, CString (bszModeID)))
			{
				pThis->StopListening (false, LSCOMPLETE_CAUSE_CLIENTDEACTIVATED);
				SafeFreeSafePtr (pThis->mListeningState);

				if	(
						(lPrevInput)
					&&	(lPrevInput != pThis->mSapiInput)
					)
				{
					lInputCache->RemoveInputClient (lPrevInput, pThis);
				}
				lResult = S_OK;
			}
			else
			{
				pThis->mSapiInput = lPrevInput;
				lResult = AGENTERR_SRLANGUAGENOTFOUND;
			}
		}
		catch AnyExceptionDebug
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::SetSRModeID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::Listen (long bListen)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Listen [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bListen);
#endif
	HRESULT	lResult;

#ifdef	_TRACE_CHARACTER_ACTIONS
	TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Listen"), _T("%d"), bListen);
#endif
	if	(bListen)
	{
		lResult = pThis->StartListening (true);
	}
	else
	{
		lResult = pThis->StopListening (true, LSCOMPLETE_CAUSE_PROGRAMDISABLED);
	}
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::Listen [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bListen);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCharacter::XCharacter::GetSRStatus (long *plStatus)
{
	METHOD_PROLOGUE(CDaAgentCharacter, Character)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSRStatus"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plStatus)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*plStatus) = pThis->GetListeningStatus ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentCharacter::XCharacter::GetSRStatus"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCharacter::DspGetVisible(long * Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetVisible"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetPosition(long Left, long Top)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetPosition"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetPosition (Left, Top);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetPosition(long * Left, long * Top)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetPosition"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetPosition (Left, Top);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetSize(long Width, long Height)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetSize (Width, Height);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetSize(long * Width, long * Height)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetSize (Width, Height);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetName(BSTR * Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetName (Name);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetDescription(BSTR * Description)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetDescription"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetDescription (Description);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetTTSSpeed(short * Speed)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetTTSSpeed"), this, m_dwRef, mCharID);
#endif
	long	lSpeed = 0;
	HRESULT	lResult = m_xCharacter.GetTTSSpeed (&lSpeed);
	(*Speed) = (short)lSpeed;
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetTTSPitch(short * Pitch)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetTTSPitch"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetTTSPitch (Pitch);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspActivate(short State)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspActivate"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Activate (State);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetIdleOn(long On)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetIdleOn"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetIdleOn (On);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetIdleOn(long * On)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetIdleOn"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetIdleOn (On);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspPrepare(long Type, LPCTSTR Name, long Queue, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspPrepare"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Prepare (Type, _bstr_t(Name), Queue, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspPlay(LPCTSTR Animation, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspPlay"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Play (_bstr_t(Animation), ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspStop(long ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspStop"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Stop (ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspWait(long WaitForReqID, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspWait"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Wait (WaitForReqID, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspStopAll(long Types)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspStopAll"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.StopAll (Types);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspInterrupt(long InterruptReqID, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspInterrupt"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Interrupt (InterruptReqID, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspShow(long Fast, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspShow"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Show (Fast, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspHide(long Fast, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspHide"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Hide (Fast, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSpeak(LPCTSTR Text, LPCTSTR Url, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSpeak"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Speak (_bstr_t(Text), _bstr_t(Url), ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspMoveTo(short x, short y, long Speed, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspMoveTo"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.MoveTo (x, y, Speed, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGestureAt(short x, short y, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGestureAt"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GestureAt (x, y, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetMoveCause(long * Cause)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetMoveCause"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetMoveCause (Cause);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetVisibilityCause(long * Cause)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetVisibilityCause"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetVisibilityCause (Cause);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspHasOtherClients(long * NumOtherClients)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspHasOtherClients"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.HasOtherClients (NumOtherClients);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetSoundEffectsOn(long On)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetSoundEffectsOn"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetSoundEffectsOn (On);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetSoundEffectsOn(long * On)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetSoundEffectsOn"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetSoundEffectsOn (On);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetName(LPCTSTR Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetName (_bstr_t(Name));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetDescription(LPCTSTR Description)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetDescription"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetDescription (_bstr_t(Description));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetExtraData(BSTR * ExtraData)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetExtraData"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetExtraData (ExtraData);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspShowPopupMenu(short x, short y)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspShowPopupMenu"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.ShowPopupMenu (x, y);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetAutoPopupMenu(long AutoPopupMenu)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetAutoPopupMenu"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetAutoPopupMenu (AutoPopupMenu);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetAutoPopupMenu(long * AutoPopupMenu)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetAutoPopupMenu"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetAutoPopupMenu (AutoPopupMenu);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetHelpFileName(BSTR * Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetHelpFileName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetHelpFileName (Name);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetHelpFileName(LPCTSTR Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetHelpFileName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetHelpFileName (_bstr_t(Name));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetHelpModeOn(long HelpModeOn)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetHelpModeOn"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetHelpModeOn (HelpModeOn);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetHelpModeOn(long * HelpModeOn)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetHelpModeOn"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetHelpModeOn (HelpModeOn);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetHelpContextID(long ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetHelpContextID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetHelpContextID(long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetHelpContextID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetActive(short * State)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetActive"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetActive (State);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspListen(long Listen)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspListen"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Listen (Listen);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetLanguageID(long langid)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetLanguageID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetLanguageID (langid);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetLanguageID(long * langid)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetLanguageID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetLanguageID (langid);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetTTSModeID(BSTR * ModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetTTSModeID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetTTSModeID (ModeID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetTTSModeID(LPCTSTR ModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetTTSModeID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetTTSModeID (_bstr_t(ModeID));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetSRModeID(BSTR * ModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetSRModeID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetSRModeID (ModeID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspSetSRModeID(LPCTSTR ModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspSetSRModeID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.SetSRModeID (_bstr_t(ModeID));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetGUID(BSTR * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetGUID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetGUID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetOriginalSize(long * Width, long * Height)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetOriginalSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetOriginalSize (Width, Height);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspThink(LPCTSTR Text, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspThink"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.Think (_bstr_t(Text), ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetVersion(short * Major, short * Minor)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetVersion"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetVersion (Major, Minor);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetAnimationNames(LPUNKNOWN * Enum)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetAnimationNames"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetAnimationNames (Enum);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCharacter::DspGetSRStatus(long * Status)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%d] CDaAgentCharacter::DspGetSRStatus"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCharacter.GetSRStatus (Status);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
