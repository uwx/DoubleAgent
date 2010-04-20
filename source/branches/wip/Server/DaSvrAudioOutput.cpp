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
#include "DaSvrAudioOutput.h"
#include "DaGlobalConfig.h"
#include "DaSvrCharacter.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrAudioOutput::DaSvrAudioOutput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAudioOutput::DaSvrAudioOutput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrAudioOutput::~DaSvrAudioOutput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAudioOutput::~DaSvrAudioOutput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrAudioOutput (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrAudioOutput * DaSvrAudioOutput::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrAudioOutput> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrAudioOutput>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrAudioOutput::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
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

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void DaSvrAudioOutput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAudioOutput::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrAudioOutput::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAudioOutput::OnClientEnded"), this, m_dwRef);
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

long DaSvrAudioOutput::GetAudioStatus ()
{
	long	Status = AudioStatus_Available;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			DaSvrCharacter *					lCharacter;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
					{
						if	(lCharacter->IsSpeaking ())
						{
							Status = AudioStatus_CharacterSpeaking;
						}
						else
						if	(lCharacter->IsHearing ())
						{
							Status = AudioStatus_UserSpeaking;
						}
						else
						if	(lCharacter->IsListening ())
						{
							Status = AudioStatus_CharacterListening;
						}
					}
				}
				if	(Status != AudioStatus_Available)
				{
					break;
				}
			}
			if	(Status != AudioStatus_Available)
			{
				break;
			}
		}

		if	(
				(Status == AudioStatus_Available)
			&&	(!CDaSettingsConfig().LoadConfig().mTtsEnabled)
			)
		{
			Status = AudioStatus_Disabled;
		}
	}
	catch AnyExceptionDebug

	return Status;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrAudioOutput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrAudioOutput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentAudioOutputProperties), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentAudioOutputPropertiesEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrAudioOutput::GetEnabled (long *Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAudioOutput::GetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mTtsEnabled ? S_OK : S_FALSE;

	if	(Enabled)
	{
		(*Enabled) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrAudioOutput::GetEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrAudioOutput::GetUsingSoundEffects (long *UsingSoundEffects)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAudioOutput::GetUsingSoundEffects"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mEffectsEnabled ? S_OK : S_FALSE;

	if	(UsingSoundEffects)
	{
		(*UsingSoundEffects) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrAudioOutput::GetUsingSoundEffects"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrAudioOutput::GetStatus (long *Status)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAudioOutput::GetStatus"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!Status)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Status) = GetAudioStatus ();
	}

	PutServerError (lResult, __uuidof(IDaSvrAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrAudioOutput::GetStatus"), this, m_dwRef);
	}
#endif
	return lResult;
}
