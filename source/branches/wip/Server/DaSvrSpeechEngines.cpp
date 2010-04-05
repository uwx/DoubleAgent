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
#include "DaSvrSpeechEngines.h"
#include "DaSvrSpeechEngine.h"
#include "SapiVoiceCache.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechEngines::DaSvrSpeechEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngines::DaSvrSpeechEngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSpeechEngines::~DaSvrSpeechEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngines::~DaSvrSpeechEngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechEngines * DaSvrSpeechEngines::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSpeechEngines> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSpeechEngines>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSpeechEngines::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrSpeechEngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngines::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrSpeechEngines::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngines::OnClientEnded"), this, m_dwRef);
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

void DaSvrSpeechEngines::UseAllVoices ()
{
	CSapiVoiceCache *	lVoiceCache;

	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		CSapi5Voices *	lSapi5Voices;

		if	(lSapi5Voices = lVoiceCache->GetSapi5Voices())
		{
			mSapi5Voices.Copy (*lSapi5Voices);
		}
#ifndef	_WIN64
		CSapi4Voices *	lSapi4Voices;

		if	(lSapi4Voices = lVoiceCache->GetSapi4Voices())
		{
			mSapi4Voices.Copy (*lSapi4Voices);
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrSpeechEngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrSpeechEngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngines::get_Item (long Index, IDaSvrSpeechEngine **SpeechEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngines::get_Item"), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	DaSvrSpeechEngine *	lSpeechEngine = NULL;
	IDaSvrSpeechEnginePtr	lInterface;

	if	(!SpeechEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngine) = NULL;

		if	(Index < 0)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(Index <= mSapi5Voices.GetUpperBound ())
		{
			if	(lSpeechEngine = DaSvrSpeechEngine::CreateInstance (mSapi5Voices [Index], mClientMutexName))
			{
				lInterface = lSpeechEngine->GetControllingUnknown();
				(*SpeechEngine) = lInterface.Detach();
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
#ifndef	_WIN64
		else
		if	(Index - mSapi5Voices.GetSize() <= mSapi4Voices.GetUpperBound ())
		{
			Index -= mSapi5Voices.GetSize();
			if	(lSpeechEngine = DaSvrSpeechEngine::CreateInstance (mSapi4Voices [Index], mClientMutexName))
			{
				lInterface = lSpeechEngine->GetControllingUnknown();
				(*SpeechEngine) = lInterface.Detach();
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
#endif
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngines::get_Item"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngines::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngines::get_Count"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mSapi5Voices.GetSize ();
#ifndef	_WIN64
		(*Count) += (long)mSapi4Voices.GetSize ();
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngines::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}
