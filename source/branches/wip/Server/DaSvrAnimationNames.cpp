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
#include "DaSvrAnimationNames.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE	(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrAnimationNames::DaSvrAnimationNames ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::DaSvrAnimationNames (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrAnimationNames::~DaSvrAnimationNames ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::~DaSvrAnimationNames (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrAnimationNames * DaSvrAnimationNames::CreateInstance (CAgentFile & pAgentFile, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrAnimationNames> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrAnimationNames>::CreateInstance (&lInstance))))
	{
		LogComErr (LogIfActive, lInstance->CDaCmnAnimationNames::Initialize (pAgentFile));
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrAnimationNames::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrAnimationNames::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrAnimationNames::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::OnClientEnded"), this, max(m_dwRef,-1));
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

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrAnimationNames), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::Next (ULONG celt, VARIANT *rgVar, ULONG *pCeltFetched)
{
	return CDaCmnAnimationNames::mEnumVARIANT.Next (celt, rgVar, pCeltFetched);
}

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::Skip (ULONG celt)
{
	return CDaCmnAnimationNames::mEnumVARIANT.Skip (celt);
}

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::Reset (void)
{
	return CDaCmnAnimationNames::mEnumVARIANT.Reset ();
}

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::Clone(IEnumVARIANT** ppEnum)
{
	HRESULT								lResult;
	CComObject<DaSvrAnimationNames> *	lClone = NULL;

	if	(
			(SUCCEEDED (LogComErr (LogIfActive, lResult = CComObject<DaSvrAnimationNames>::CreateInstance (&lClone))))
		&&	(SUCCEEDED (LogComErr (LogIfActive, lResult = lClone->CDaCmnAnimationNames::Initialize (*this, GetUnknown()))))
		)
	{
		lResult = lClone->_InternalQueryInterface (__uuidof(IEnumVARIANT), (void**)ppEnum);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::get_Count (long *Value)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAnimationNames::get_Count"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnAnimationNames::get_Count (Value);

	PutServerError (lResult, __uuidof(IDaSvrAnimationNames));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrAnimationNames::get_Count"), this, max(m_dwRef,-1), mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::get_Item (long Index, BSTR *AnimationName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAnimationNames::get_Item"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnAnimationNames::get_Item (Index, AnimationName);

	PutServerError (lResult, __uuidof(IDaSvrAnimationNames));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrAnimationNames::get_Item"), this, max(m_dwRef,-1), mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::get__NewEnum (IUnknown ** EnumVariant)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAnimationNames::get__NewEnum"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnAnimationNames::get__NewEnum (EnumVariant);

	PutServerError (lResult, __uuidof(IDaSvrAnimationNames));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrAnimationNames::get__NewEnum"), this, max(m_dwRef,-1), mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrAnimationNames::get_All (SAFEARRAY **Array)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrAnimationNames::get_All"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnAnimationNames::get_All (Array);

	PutServerError (lResult, __uuidof(IDaSvrAnimationNames));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrAnimationNames::get_All"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
