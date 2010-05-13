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
#include "DaServerApp.h"
#include "DaSvrCharacter.h"
#include "DaSvrCharacter.h"
#include "DaSvrBalloon.h"
#include "DaSvrCommands.h"
#include "DaSvrAnimationNames.h"
#include "DaSvrUserInput.h"
#include "DaSvrTTSEngine.h"
#include "DaSvrTTSEngines.h"
#include "DaSvrSREngine.h"
#include "DaSvrSREngines.h"
#include "DaActionTrace.h"
#include "AgentPopupWnd.h"
#include "AgentBalloonWnd.h"
#include "AgentListeningWnd.h"
#include "VoiceCommandsWnd.h"
#include "ListeningState.h"
#include "QueuedPrepare.h"
#include "FileDownload.h"
#include "SapiVoice.h"
#include "Registry.h"
#include "Localize.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Character"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_LISTEN			(GetProfileDebugInt(_T("DebugListen"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacter::DaSvrCharacter ()
:	mSvrBalloon (NULL),
	mSvrCommands (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::DaSvrCharacter (%d)"), this, m_dwRef, mCharID, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCharacter::~DaSvrCharacter()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::~DaSvrCharacter (%d)"), this, m_dwRef, mCharID, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacter * DaSvrCharacter::CreateInstance (long pCharID, CEventNotify * pNotify, _IListeningAnchor * pListeningAnchor, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrCharacter> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrCharacter>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize (pCharID, pNotify, pListeningAnchor);
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::CreateInstance (%d)"), lInstance, lInstance->m_dwRef, lInstance->mCharID, _AtlModule.GetLockCount());
		}
#endif
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::Terminate [%u %u] [%ls]"), this, m_dwRef, mCharID, pFinal, pAbandonned, (mFile ? (BSTR)mFile->GetPath() : NULL));
			if	(
					(mWnd)
				&&	(mWnd->IsWindow ())
				)
			{
				LogMessage (_LOG_INSTANCE, _T("[%p] [%d] [%u]   PopupWnd [%p(%d)] [%d]"), this, m_dwRef, mCharID, mWnd, mWnd->m_dwRef, mWnd->GetCharID());
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
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		if	(mSvrCommands != NULL)
		{
			try
			{
				DaSvrCommands *	lCommands;

				if	(
						(pFinal)
					&&	(pAbandonned)
					&&	(lCommands = dynamic_cast <DaSvrCommands *> (mSvrCommands))
					)
				{
					lCommands->Terminate (pFinal, pAbandonned);
					try
					{
						delete lCommands;
					}
					catch AnyExceptionSilent
				}
				else
				if	(pFinal)
				{
					mSvrCommands->Release();
				}
			}
			catch AnyExceptionSilent

			if	(pFinal)
			{
				mSvrCommands = NULL;
			}
		}

		if	(mSvrBalloon != NULL)
		{
			try
			{
				DaSvrBalloon *	lBalloon;

				if	(
						(pFinal)
					&&	(pAbandonned)
					&&	(lBalloon = dynamic_cast <DaSvrBalloon *> (mSvrBalloon))
					)
				{
					lBalloon->Terminate (pFinal, pAbandonned);
					try
					{
						delete lBalloon;
					}
					catch AnyExceptionSilent
				}
				else
				if	(pFinal)
				{
					mSvrBalloon->Release ();
				}
			}
			catch AnyExceptionSilent

			if	(pFinal)
			{
				mSvrBalloon = NULL;
			}
		}

		CDaCmnCharacter::Terminate ();

#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::Terminate [%u %u] Done"), this, m_dwRef, mCharID, pFinal, pAbandonned);
		}
#endif
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::FinalRelease [%u]"), this, m_dwRef, mCharID, IsInNotify());
	}
#endif
	Terminate (false);
}

bool DaSvrCharacter::CanFinalRelease ()
{
	return (IsInNotify() == 0);
}

void DaSvrCharacter::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::OnClientEnded"), this, m_dwRef, mCharID);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI DaSvrCharacter::DelegateIDaSvrBalloon (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT				lResult = E_NOINTERFACE;
	DaSvrCharacter *	lThis = (DaSvrCharacter *) pv;
	DaSvrBalloon *		lBalloon = NULL;

	if	(
			(lThis->mSvrBalloon == NULL)
		&&	(lThis->mFile)
		&&	(lBalloon = DaSvrBalloon::CreateInstance (lThis->mCharID, lThis->mFile, lThis->mWnd))
		)
	{
		lBalloon->SetLangID (lThis->mLangID);
		lThis->mSvrBalloon = lBalloon->GetControllingUnknown();
		lThis->mSvrBalloon->AddRef ();
	}
	if	(lThis->mSvrBalloon)
	{
		lResult = lThis->mSvrBalloon->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaSvrCharacter::DelegateIDaSvrCommands (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT				lResult = E_NOINTERFACE;
	DaSvrCharacter *	lThis = (DaSvrCharacter *) pv;
	DaSvrCommands *		lCommands = NULL;

	if	(
			(lThis->mSvrCommands == NULL)
		&&	(lCommands = DaSvrCommands::CreateInstance (lThis->mCharID, lThis->mNotify))
		)
	{
		lCommands->SetLangID (lThis->mLangID);
		lThis->mSvrCommands = lCommands->GetControllingUnknown();
		lThis->mSvrCommands->AddRef ();
	}
	if	(lThis->mSvrCommands)
	{
		lResult = lThis->mSvrCommands->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::_PreNotify ()
{
	return (m_dwRef > 0);
}

bool DaSvrCharacter::_PostNotify ()
{
	if	(
			(CanFinalRelease ())
		&&	(HasFinalReleased ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaServer PostNotify -> Release"), this, m_dwRef, mCharID);
		}
#endif
		m_dwRef = 1;
		Release ();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCmnCommands * DaSvrCharacter::GetCommands (bool pCreateObject)
{
	CDaCmnCommands *		lRet = NULL;
	IDaSvrCommandsPtr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetUnknown ();
	}
	if	(mSvrCommands != NULL)
	{
		lRet = dynamic_cast <DaSvrCommands *> (mSvrCommands);
	}
	return lRet;
}

CDaCmnBalloon * DaSvrCharacter::GetBalloon (bool pCreateObject)
{
	CDaCmnBalloon *		lRet = NULL;
	IDaSvrBalloon2Ptr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetUnknown ();
	}
	if	(mSvrBalloon != NULL)
	{
		lRet = dynamic_cast <DaSvrBalloon *> (mSvrBalloon);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::NotifyVoiceCommand (USHORT pCommandId, ISpRecoResult * pRecoResult, bool pGlobalCommand)
{
	DaSvrUserInput *		lUserInput;
	IDaSvrUserInput2Ptr		lNotifyUserInput;

	if	(lUserInput = DaSvrUserInput::CreateInstance (pRecoResult, pGlobalCommand))
	{
		lNotifyUserInput = lUserInput->GetControllingUnknown();
	}

#ifdef	_STRICT_COMPATIBILITY
	mNotify->Command ((long)pCommandId, lNotifyUserInput);
#else
	if	(pGlobalCommand)
	{
		mNotify->Command (0, lNotifyUserInput);
	}
	else
	{
		mNotify->Command ((long)pCommandId, lNotifyUserInput);
	}
#endif

	SafeFreeSafePtr (lNotifyUserInput);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrCharacter::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrCharacter2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrCharacter), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCharacter), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCharacterEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid)
{
	if	(!pClsid)
	{
		return E_POINTER;
	}
	*pClsid = __uuidof(DaServerHandler);
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] DaSvrCharacter::GetClassForHandler [%8.8X] [%p] [%s]"), this, m_dwRef, dwDestContext, pvDestContext, CGuidStr::GuidName(*pClsid));
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Balloon (IDaSvrBalloon2 **Balloon)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Balloon"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	IDaSvrBalloon2Ptr	lSvrBalloon;

	if	(!Balloon)
	{
		lResult = E_POINTER;
	}
	else
	{
		GetBalloon (true);
		lSvrBalloon = mSvrBalloon;
		(*Balloon) = lSvrBalloon.Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Balloon"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Commands (IDaSvrCommands2 **Commands)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Commands"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	IDaSvrCommands2Ptr	lSvrCommands;

	if	(!Commands)
	{
		lResult = E_POINTER;
	}
	else
	{
		GetCommands (true);
		lSvrCommands = mSvrCommands;
		(*Commands) = lSvrCommands.Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Commands"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetPosition (long Left, long Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetPosition [%d %d]"), this, m_dwRef, mCharID, Left, Top);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::SetPosition (Left, Top);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("SetPosition"), _T("%d\t%d"), Left, Top);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetPosition"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetPosition (long *Left, long *Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetPosition"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetPosition (Left, Top);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetPosition"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSize (long Width, long Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::SetSize (Width, Height);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("SetSize"), _T("%d\t%d"), Width, Height);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetSize (Width, Height);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetOriginalSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetOriginalSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetOriginalSize (Width, Height);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetOriginalSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetGUID (BSTR *CharGUID)
{
	return get_UniqueID (CharGUID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVersion (short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetVersion"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetVersion (MajorVersion, MinorVersion);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetVersion"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetName (BSTR *Name)
{
	return get_Name (Name);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetName (BSTR Name)
{
	return put_Name (Name);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetDescription (BSTR *Description)
{
	return get_Description (Description);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetDescription (BSTR Description)
{
	return put_Description (Description);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetLanguageID (long LanguageID)
{
	return put_LanguageID (LanguageID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetLanguageID (long *LanguageID)
{
	return get_LanguageID (LanguageID);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetIdleOn (long On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetIdleOn [%d]"), this, m_dwRef, mCharID, On);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetIdleOn"), _T("%d"), On);
#endif
		if	(On)
		{
			lResult = SetStyle (0, CharacterStyle_IdleEnabled);
		}
		else
		{
			lResult = SetStyle (CharacterStyle_IdleEnabled, 0);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetIdleOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetIdleOn (long *On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetIdleOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = IsIdleEnabled() ? S_OK : S_FALSE;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!On)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*On) = (IsIdleEnabled()!=false);
#ifdef _STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetIdleOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSoundEffectsOn (long On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetSoundEffectsOn [%d]"), this, m_dwRef, mCharID, On);
	}
#endif
	HRESULT	lResult = S_FALSE;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetSoundEffectsOn"), _T("%d"), On);
#endif
		if	(On)
		{
			lResult = SetStyle (0, CharacterStyle_SoundEffects);
		}
		else
		{
			lResult = SetStyle (CharacterStyle_SoundEffects, 0);
		}
#ifdef	_STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetSoundEffectsOn [%d]"), this, m_dwRef, mCharID, On);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSoundEffectsOn (long *On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSoundEffectsOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = IsSoundEnabled() ? S_OK : S_FALSE;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!On)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*On) = (IsSoundEnabled(true)!=false);
#ifdef _STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSoundEffectsOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetExtraData (BSTR *ExtraData)
{
	return get_ExtraData (ExtraData);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetAnimationNames (IUnknown **punkEnum)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetAnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_OK;
	IDaSvrAnimationNames *	lInterface = NULL;

	if	(!punkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = get_AnimationNames (&lInterface);
		(*punkEnum) = lInterface;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetAnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_AnimationNames (IDaSvrAnimationNames **AnimationNames)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_AnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_OK;
	DaSvrAnimationNames *	lAnimationNames;
	IDaSvrAnimationNamesPtr	lInterface;

	if	(!AnimationNames)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AnimationNames) = NULL;

		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lAnimationNames = DaSvrAnimationNames::CreateInstance (*mFile))
		{
			lInterface = lAnimationNames->GetControllingUnknown ();
			(*AnimationNames) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_AnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Activate (short State)
{
	return put_ActiveState ((ActiveStateType)State);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetActive (short *State)
{
	return get_ActiveState ((ActiveStateType*)State);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::HasOtherClients (long *OtherClientCount)
{
	return get_OtherClientCount (OtherClientCount);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Show (long Fast, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Show [%d]"), this, m_dwRef, mCharID, Fast);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Show (Fast, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Show"), _T("%d\t%d"), Fast, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Show"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Hide (long Fast, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Hide [%d]"), this, m_dwRef, mCharID, Fast);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Hide (Fast, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Hide"), _T("%d\t%d"), Fast, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Hide"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible;
	HRESULT			lResult = get_Visible (&lVisible);

	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVisibilityCause (long *Cause)
{
	VisibilityCauseType	lCause;
	HRESULT				lResult = get_VisibilityCause (&lCause);

	if	(Cause)
	{
		(*Cause) = (long)lCause;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Prepare (long Type, BSTR Name, long Queue, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Prepare [%d] [%ls] [%d]"), this, m_dwRef, mCharID, Type, Name, Queue);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Prepare (Type, Name, Queue, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Prepare"), _T("%d\t%ls\t%d\t%d"), Type, Name, Queue, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Prepare"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Play (BSTR Animation, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Play [%s]"), this, m_dwRef, mCharID, DebugStr(CString(Animation)));
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Play (Animation, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Play"), _T("%ls\t%d"), Animation, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Play"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Stop (long RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Stop"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Stop (RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Stop"), _T("%d"), RequestID);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Stop"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::StopAll (long Types)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::StopAll [%8.8X]"), this, m_dwRef, mCharID, Types);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::StopAll (Types);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("StopAll"), _T("0x%8.8X"), Types);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::StopAll"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::MoveTo (short x, short y, long Speed, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::MoveTo"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::MoveTo (x, y, Speed, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("MoveTo"), _T("%hd\t%hd\t%d\t%d"), x, y, Speed, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::MoveTo"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetMoveCause (long *Cause)
{
	MoveCauseType	lCause;
	HRESULT			lResult = get_MoveCause (&lCause);

	if	(Cause)
	{
		(*Cause) = (long)lCause;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GestureAt (short x, short y, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GestureAt [%hd %hd]"), this, m_dwRef, mCharID, x, y);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GestureAt (x, y, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("GestureAt"), _T("%hd\t%hd\t%d"), x, y, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GestureAt"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Think (BSTR Text, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Think [%s]"), this, m_dwRef, mCharID, DebugStr(Text));
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Think (Text, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Think"), _T("%s\t%d"), EncodeTraceString(Text), (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Think"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Wait (long WaitForRequestID, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Wait"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Wait (WaitForRequestID, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Wait"), _T("%d\t%d"), RequestID, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Wait"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Interrupt (long InterruptRequestID, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Interrupt"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Interrupt (InterruptRequestID, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Interrupt"), _T("%d\t%d"), RequestID, (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Interrupt"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::ShowPopupMenu (short x, short y)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::ShowPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::ShowPopupMenu (x, y);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("ShowPopupMenu"), _T("%hd\t%hd"), x, y);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::ShowPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetAutoPopupMenu (long AutoPopupMenu)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetAutoPopupMenu [%d]"), this, m_dwRef, mCharID, AutoPopupMenu);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetAutoPopupMenu"), _T("%d"), AutoPopupMenu);
#endif
		if	(AutoPopupMenu)
		{
			lResult = SetStyle (0, CharacterStyle_AutoPopupMenu);
		}
		else
		{
			lResult = SetStyle (CharacterStyle_AutoPopupMenu, 0);
		}
#ifdef	_STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetAutoPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetAutoPopupMenu (long *AutoPopupMenu)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetAutoPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = IsAutoPopupMenu() ? S_OK : S_FALSE;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!AutoPopupMenu)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*AutoPopupMenu) = (IsAutoPopupMenu()!=false);
#ifdef _STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetAutoPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpFileName (BSTR *Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(Name)
	{
		(*Name) = NULL;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpFileName (BSTR Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpModeOn (long HelpModeOn)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpModeOn (long *HelpModeOn)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(HelpModeOn)
	{
		(*HelpModeOn) = FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpContextID (long ContextID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpContextID (long *ContextID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(ContextID)
	{
		(*ContextID) = 0;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSSpeed (long *Speed)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSSpeed"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetTTSSpeed (Speed);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSSpeed"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSPitch (short *Pitch)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSPitch"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetTTSPitch (Pitch);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSPitch"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSModeID (BSTR *ModeID)
{
	return get_TTSModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetTTSModeID (BSTR ModeID)
{
	return put_TTSModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Speak (BSTR Text, BSTR Url, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Speak [%s] [%s]"), this, m_dwRef, mCharID, DebugStr(CString(Text)), DebugStr(CString(Url)));
	}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("PreSpeak"), _T("%s\t%s"), EncodeTraceString(Text), DebugStr(Url));
#endif

	HRESULT	lResult = CDaCmnCharacter::Speak (Text, Url, RequestID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Speak"), _T("%s\t%s\t%d"), EncodeTraceString(Text), DebugStr(Url), (RequestID?*RequestID:0));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Speak"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSRModeID (BSTR *ModeID)
{
	return get_SRModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSRModeID (BSTR ModeID)
{
	return put_SRModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Listen (long Listen)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Listen [%d]"), this, m_dwRef, mCharID, Listen);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::Listen (Listen);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Listen"), _T("%d"), Listen);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Listen [%d]"), this, m_dwRef, mCharID, Listen);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSRStatus (long *Status)
{
	ListeningStatusType	lStatus;
	HRESULT				lResult = get_ListeningStatus (&lStatus);

	if	(Status)
	{
		(*Status) = (long)lStatus;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_CharacterID (long *CharacterID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_CharacterID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_CharacterID (CharacterID);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_CharacterID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_UniqueID (BSTR *CharGUID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_UniqueID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_UniqueID (CharGUID);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_UniqueID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Name (BSTR *Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Name"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Name (Name);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Name"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Name (BSTR Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Name"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Name (Name);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Name"), _T("%ls"), Name);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Name"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Description (BSTR *Description)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Description"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Description (Description);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Description"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Description (BSTR Description)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Description"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Description (Description);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Description"), _T("%ls"), Description);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Description"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_ExtraData (BSTR *ExtraData)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_ExtraData"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_ExtraData (ExtraData);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_ExtraData"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_FileName (BSTR *FileName)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_FileName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_FileName (FileName);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_FileName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_FilePath (BSTR *FilePath)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_FilePath"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_FilePath (FilePath);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_FilePath"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_LanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_LanguageID (LanguageID);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_LanguageID (long LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_LanguageID (LanguageID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_LanguageID"), _T("%d"), LanguageID);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_TTSModeID (BSTR *TTSModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_TTSModeID (TTSModeID);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_TTSModeID (BSTR TTSModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_TTSModeID [%ls]"), this, m_dwRef, mCharID, TTSModeID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_TTSModeID (TTSModeID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_TTSModeID"), _T("%ls"), TTSModeID);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_TTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_SRModeID (BSTR *SRModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_SRModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_SRModeID (SRModeID);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_SRModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_SRModeID (BSTR SRModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_SRModeID [%ls]"), this, m_dwRef, mCharID, SRModeID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_SRModeID (SRModeID);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_SRModeID"), _T("%ls"), SRModeID);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_SRModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Left (short *Left)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Left"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Left (Left);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Left"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Left (short Left)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Left [%hd]"), this, m_dwRef, mCharID, Left);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Left (Left);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Left"), _T("%hd"), Left);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Left"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Top (short *Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Top"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Top (Top);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Top"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Top (short Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Top [%hd]"), this, m_dwRef, mCharID, Top);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Top (Top);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Top"), _T("%hd"), Top);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Top"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Width (short *Width)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Width"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Width (Width);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Width"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Width (short Width)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Width [%hd]"), this, m_dwRef, mCharID, Width);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Width (Width);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Width"), _T("%hd"), Width);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Width"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Height (short *Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Height"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Height (Height);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Height"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Height (short Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Height [%hd]"), this, m_dwRef, mCharID, Height);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Height (Height);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Height"), _T("%hd"), Height);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Height"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_OriginalWidth (short *OriginalWidth)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalWidth"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_OriginalWidth (OriginalWidth);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalWidth"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_OriginalHeight (short *OriginalHeight)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalHeight"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_OriginalHeight (OriginalHeight);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalHeight"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Visible"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Visible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_ActiveState (ActiveStateType *ActiveState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_ActiveState"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_ActiveState (ActiveState);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_ActiveState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_ActiveState (ActiveStateType ActiveState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_ActiveState [%hd]"), this, m_dwRef, mCharID, ActiveState);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_ActiveState (ActiveState);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_ActiveState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IdleState (VARIANT_BOOL *IdleState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IdleState"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_IdleState (IdleState);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IdleState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_OtherClientCount (long *OtherClientCount)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_OtherClientCount"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_OtherClientCount (OtherClientCount);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_OtherClientCount"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_MoveCause (MoveCauseType *MoveCause)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_MoveCause"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_MoveCause (MoveCause);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_MoveCause"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_VisibilityCause (VisibilityCauseType *VisibilityCause)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_VisibilityCause"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_VisibilityCause (VisibilityCause);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_VisibilityCause"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_ListeningStatus (ListeningStatusType *ListeningStatus)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_ListeningStatus"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_ListeningStatus (ListeningStatus);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_ListeningStatus"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_HasIcon (VARIANT_BOOL *HasIcon)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_HasIcon"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_HasIcon (HasIcon);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_HasIcon"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Style (long *Style)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Style"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_Style (Style);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Style"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Style (long Style)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconState [%u]"), this, m_dwRef, mCharID, Style);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_Style (Style);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Style"), _T("%8.8X"), Style);
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconVisible (VARIANT_BOOL *IconVisible)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconVisible"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_IconVisible (IconVisible);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GenerateIcon (long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GenerateIcon"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GenerateIcon (ClipLeft, ClipTop, ClipWidth, ClipHeight);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GenerateIcon"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetIconIdentity (GUID *IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetIconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::GetIconIdentity (IconIdentity);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetIconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetIconIdentity (const GUID *IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetIconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::SetIconIdentity (IconIdentity);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("SetIconIdentity"), _T("%s"), (CString)CGuidStr(IconIdentity?*IconIdentity:GUID_NULL));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetIconIdentity"), this, m_dwRef, mCharID);
	}

#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconIdentity (BSTR *IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_IconIdentity (IconIdentity);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_IconIdentity (BSTR IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_IconIdentity (IconIdentity);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_IconIdentity"), _T("%s"), CString(IconIdentity));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconTip (BSTR *IconTip)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::get_IconTip (IconTip);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_IconTip (BSTR IconTip)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = CDaCmnCharacter::put_IconTip (IconTip);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_IconTip"), _T("%s"), CString(IconTip));
#endif

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_TTSEngine (VARIANT_BOOL GetDefault, IDaSvrTTSEngine **TTSEngine)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSEngine"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <DaSvrTTSEngine>	lTTSEngine = NULL;
	IDaSvrTTSEnginePtr		lInterface;

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(lTTSEngine = DaSvrTTSEngine::CreateInstance ((CSapi5VoiceInfo*)NULL, mClientMutexName))
		{
			if	(
					(GetDefault)
				?	(lTTSEngine->Initialize (mFile))
				:	(lTTSEngine->Initialize (GetSapiVoice (true)))
				)
			{
				lInterface = lTTSEngine.Detach()->GetControllingUnknown();
				(*TTSEngine) = lInterface.Detach();
				lResult = S_OK;
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSEngine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::FindTTSEngines (long LanguageID, IDaSvrTTSEngines **TTSEngines)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::FindTTSEngines"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <DaSvrTTSEngines>	lTTSEngines;
	IDaSvrTTSEnginesPtr		lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(lTTSEngines = DaSvrTTSEngines::CreateInstance (mClientMutexName))
		{
			if	(SUCCEEDED (lResult = lTTSEngines->UseTheseVoices (mFile, (LANGID)LanguageID, SpeechGender_Neutral)))
			{
				lInterface = lTTSEngines.Detach()->GetControllingUnknown();
				(*TTSEngines) = lInterface.Detach();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::FindTTSEngines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_SREngine (VARIANT_BOOL GetDefault, IDaSvrSREngine **SREngine)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_SREngine"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <DaSvrSREngine>	lSREngine = NULL;
	IDaSvrSREnginePtr		lInterface;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(lSREngine = DaSvrSREngine::CreateInstance (NULL, mClientMutexName))
		{
			if	(
					(GetDefault)
				?	(lSREngine->Initialize (mFile))
				:	(lSREngine->Initialize (GetSapiInput (true)))
				)
			{
				lInterface = lSREngine.Detach()->GetControllingUnknown();
				(*SREngine) = lInterface.Detach();
				lResult = S_OK;
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_SREngine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::FindSREngines (long LanguageID, IDaSvrSREngines **SREngines)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::FindSREngines"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <DaSvrSREngines>	lSREngines;
	IDaSvrSREnginesPtr		lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(lSREngines = DaSvrSREngines::CreateInstance (mClientMutexName))
		{
			if	(SUCCEEDED (lResult = lSREngines->UseTheseInputs (mFile, (LANGID)LanguageID)))
			{
				lInterface = lSREngines.Detach()->GetControllingUnknown();
				(*SREngines) = lInterface.Detach();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::FindSREngines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
