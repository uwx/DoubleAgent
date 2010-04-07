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
#include "DaSvrRecognitionEngines.h"
#include "DaSvrRecognitionEngine.h"
#include "SapiInputCache.h"
#include "Sapi5Inputs.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrRecognitionEngines::DaSvrRecognitionEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngines::DaSvrRecognitionEngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrRecognitionEngines::~DaSvrRecognitionEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngines::~DaSvrRecognitionEngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrRecognitionEngines * DaSvrRecognitionEngines::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrRecognitionEngines> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrRecognitionEngines>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrRecognitionEngines::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrRecognitionEngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngines::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrRecognitionEngines::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngines::OnClientEnded"), this, m_dwRef);
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

void DaSvrRecognitionEngines::UseAllInputs ()
{
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;

	if	(
			(lInputCache = CSapiInputCache::GetStaticInstance ())
		&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs())
		)
	{
		mSapi5Inputs.Copy (*lSapi5Inputs);
	}
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrRecognitionEngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrRecognitionEngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngines::get_Item (long Index, IDaSvrRecognitionEngine **RecognitionEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngines::get_Item"), this, m_dwRef);
#endif
	HRESULT						lResult = S_OK;
	DaSvrRecognitionEngine *	lRecognitionEngine = NULL;
	IDaSvrRecognitionEnginePtr	lInterface;

	if	(!RecognitionEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngine) = NULL;

		if	(Index < 0)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(Index <= mSapi5Inputs.GetUpperBound ())
		{
			if	(lRecognitionEngine = DaSvrRecognitionEngine::CreateInstance (mSapi5Inputs [Index], mClientMutexName))
			{
				lInterface = lRecognitionEngine->GetControllingUnknown();
				(*RecognitionEngine) = lInterface.Detach();
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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngines::get_Item"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngines::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngines::get_Count"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mSapi5Inputs.GetSize ();
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngines::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}
