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
#include "DaSvrSREngines.h"
#include "DaSvrSREngine.h"
#include "SapiInputCache.h"
#include "Sapi5Inputs.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSREngines::DaSvrSREngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngines::DaSvrSREngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSREngines::~DaSvrSREngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngines::~DaSvrSREngines (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSREngines * DaSvrSREngines::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSREngines> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSREngines>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSREngines::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrSREngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngines::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrSREngines::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngines::OnClientEnded"), this, m_dwRef);
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

void DaSvrSREngines::UseAllInputs ()
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

STDMETHODIMP DaSvrSREngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrSREngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSREngines::get_Item (long Index, IDaSvrSREngine **SREngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngines::get_Item"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	DaSvrSREngine *		lSREngine = NULL;
	IDaSvrSREnginePtr	lInterface;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(Index < 0)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(Index <= mSapi5Inputs.GetUpperBound ())
		{
			if	(lSREngine = DaSvrSREngine::CreateInstance (mSapi5Inputs [Index], mClientMutexName))
			{
				lInterface = lSREngine->GetControllingUnknown();
				(*SREngine) = lInterface.Detach();
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

	PutServerError (lResult, __uuidof(IDaSvrSREngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngines::get_Item"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSREngines::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngines::get_Count"), this, m_dwRef);
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

	PutServerError (lResult, __uuidof(IDaSvrSREngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngines::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSREngines::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngines::get__NewEnum"), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <CComVariant>	lArray;
	IEnumVARIANTPtr			lInterface;
	IDaSvrSREnginePtr		lEngine;
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

	PutServerError (lResult, __uuidof(IDaSvrSREngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngines::get__NewEnum"), this, m_dwRef);
	}
#endif
	return lResult;
}
