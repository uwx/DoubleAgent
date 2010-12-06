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

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED			MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrTTSEngines::DaSvrTTSEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::DaSvrTTSEngines (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrTTSEngines::~DaSvrTTSEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::~DaSvrTTSEngines (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrTTSEngines * DaSvrTTSEngines::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrTTSEngines> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrTTSEngines>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrTTSEngines::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		SafeFreeSafePtr (mCachedEnum);

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}

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
	}
}

void DaSvrTTSEngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSEngines::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrTTSEngines::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrTTSEngines::OnClientEnded"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, true);
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrTTSEngines::InitEnumVariant (CEnumVARIANTImpl * pEnum)
{
	HRESULT					lResult = S_FALSE;
	tArrayPtr <CComVariant>	lArray;
	IDaSvrTTSEnginePtr		lEngine;
	long					lCount = 0;
	long					lNdx;

	if	(
			(SUCCEEDED (get_Count (&lCount)))
		&&	(lArray = new CComVariant [lCount+1])
		)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)lCount; lNdx++)
		{
			get_Item (lNdx, &lEngine);
			lArray [lNdx] = (LPDISPATCH)lEngine;
		}
		lResult = pEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)lCount]), (LPDISPATCH)this, AtlFlagCopy);
	}
	return lResult;
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSEngines::get_Item [%d]"), this, max(m_dwRef,-1), Index);
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
		{
			CSapi5VoiceInfo *	lSapi5VoiceInfo;

			if	(lSapi5VoiceInfo = GetSapi5VoiceAt (Index))
			{
				if	(lTTSEngine = DaSvrTTSEngine::CreateInstance (lSapi5VoiceInfo, mClientMutexName))
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
			{
				CSapi4VoiceInfo *	lSapi4VoiceInfo;

				if	(lSapi4VoiceInfo = GetSapi4VoiceAt (Index))
				{
					if	(lTTSEngine = DaSvrTTSEngine::CreateInstance (lSapi4VoiceInfo, mClientMutexName))
					{
						lInterface = lTTSEngine->GetControllingUnknown();
						(*TTSEngine) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lResult = E_INVALIDARG;
				}
			}
#else
			else
			{
				lResult = E_INVALIDARG;
			}
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrTTSEngines));
#ifdef	_LOG_RESULTS
	if	(
			(LogIsActive (_LOG_RESULTS))
		&&	(lResult != E_INVALIDARG)
		)
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSEngines::get_Item"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSEngines::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSEngines::get_Count"), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSEngines::get_Count"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrTTSEngines::get__NewEnum (IUnknown **EnumVariant)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSEngines::get__NewEnum"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	tPtr <CComObject<CEnumVARIANT> >	lEnumVariant;
	IEnumVARIANTPtr						lInterface;

	if	(!EnumVariant)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*EnumVariant) = NULL;

		if	(
				(SUCCEEDED (lResult = CComObject<CEnumVARIANT>::CreateInstance (lEnumVariant.Free())))
			&&	(SUCCEEDED (lResult = InitEnumVariant (lEnumVariant)))
			)
		{
			lInterface = lEnumVariant.Detach();
			(*EnumVariant) = lInterface.Detach();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSEngines::get__NewEnum"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
