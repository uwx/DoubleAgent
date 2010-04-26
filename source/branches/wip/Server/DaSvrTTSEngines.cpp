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
#include "DaSvrTTSEngines.h"
#include "DaSvrTTSEngine.h"
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

DaSvrTTSEngines::DaSvrTTSEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::DaSvrTTSEngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrTTSEngines::~DaSvrTTSEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::~DaSvrTTSEngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrTTSEngines * DaSvrTTSEngines::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrTTSEngines> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrTTSEngines>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrTTSEngines::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrTTSEngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrTTSEngines::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::OnClientEnded"), this, m_dwRef);
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

void DaSvrTTSEngines::UseAllVoices ()
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

STDMETHODIMP DaSvrTTSEngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrTTSEngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrTTSEngines::get_Item (long Index, IDaSvrTTSEngine **TTSEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSEngines::get_Item"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	DaSvrTTSEngine *	lTTSEngine = NULL;
	IDaSvrTTSEnginePtr	lInterface;

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(Index < 0)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(Index < (long)mSapi5Voices.GetCount())
		{
			if	(lTTSEngine = DaSvrTTSEngine::CreateInstance (mSapi5Voices [Index], mClientMutexName))
			{
				lInterface = lTTSEngine->GetControllingUnknown();
				(*TTSEngine) = lInterface.Detach();
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
#ifndef	_WIN64
		else
		if	(Index - (long)mSapi5Voices.GetCount() < (long)mSapi4Voices.GetCount())
		{
			Index -= mSapi5Voices.GetCount();
			if	(lTTSEngine = DaSvrTTSEngine::CreateInstance (mSapi4Voices [Index], mClientMutexName))
			{
				lInterface = lTTSEngine->GetControllingUnknown();
				(*TTSEngine) = lInterface.Detach();
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

	PutServerError (lResult, __uuidof(IDaSvrTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSEngines::get_Item"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSEngines::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSEngines::get_Count"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mSapi5Voices.GetCount();
#ifndef	_WIN64
		(*Count) += (long)mSapi4Voices.GetCount();
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSEngines::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrTTSEngines::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSEngines::get__NewEnum"), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <CComVariant>	lArray;
	IEnumVARIANTPtr			lInterface;
	IDaSvrTTSEnginePtr		lEngine;
	long					lCount = 0;
	long					lNdx;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(
				(lEnum = new CComObject <CEnumVARIANT>)
			&&	(SUCCEEDED (get_Count (&lCount)))
			&&	(lArray = new CComVariant [lCount+1])
			)
		{
			for	(lNdx = 0; lNdx < (INT_PTR)lCount; lNdx++)
			{
				get_Item (lNdx, &lEngine);
				lArray [lNdx] = (LPDISPATCH)lEngine;
			}
			if	(SUCCEEDED (lResult = lEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)lCount]), (LPDISPATCH)this, AtlFlagCopy)))
			{
				lInterface = lEnum.Detach ();
				(*ppunkEnum) = lInterface.Detach ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSEngines::get__NewEnum"), this, m_dwRef);
	}
#endif
	return lResult;
}
