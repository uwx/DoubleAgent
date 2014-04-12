/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_DEBUG_ABANDONED	_LOG_ABANDONED
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

DaSvrAnimationNames * DaSvrAnimationNames::CreateInstance (CAgentFile& pAgentFile, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrAnimationNames>*	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrAnimationNames>::CreateInstance (&lInstance))))
	{
		LogComErr (LogIfActive|LogTime, lInstance->CDaCmnAnimationNames::Initialize (pAgentFile));
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrAnimationNames::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
		if	(pAbandonned)
		{
			Disconnect (pAbandonned);
		}
		if	(pFinal)
		{
			Disconnect (pAbandonned);
		}
	}
}

void DaSvrAnimationNames::Disconnect (bool pAbandonned)
{
	if	(m_dwRef > 0)
	{
		if	(pAbandonned)
		{
			m_dwRef = SHRT_MIN;
		}
		__try
		{
			LogComErr (LogIfActive, CoDisconnectObject (GetUnknown(), 0));
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__, EXCEPTION_EXECUTE_HANDLER))
		{}
	}
	if	(pAbandonned)
	{
		m_dwRef = min (m_dwRef, SHRT_MIN);
	}
	else
	{
		m_dwRef = min (m_dwRef, 0);
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
	Terminate (false, !_VerifyClientLifetime());
}

void DaSvrAnimationNames::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrAnimationNames::OnClientEnded"), this, max(m_dwRef,-1));
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
#pragma page()
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
	CComObject<DaSvrAnimationNames>*	lClone = NULL;

	if	(
			(SUCCEEDED (LogComErr (LogIfActive|LogTime, lResult = CComObject<DaSvrAnimationNames>::CreateInstance (&lClone))))
		&&	(SUCCEEDED (LogComErr (LogIfActive|LogTime, lResult = lClone->CDaCmnAnimationNames::Initialize (*this, GetUnknown()))))
		)
	{
		lResult = lClone->_InternalQueryInterface (__uuidof(IEnumVARIANT), (void**)ppEnum);
	}
	return lResult;
}
