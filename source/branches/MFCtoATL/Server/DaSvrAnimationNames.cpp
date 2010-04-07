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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::DaSvrAnimationNames (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrAnimationNames::~DaSvrAnimationNames ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::~DaSvrAnimationNames (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrAnimationNames * DaSvrAnimationNames::CreateInstance (CAgentFile & pAgentFile)
{
	CComObject<DaSvrAnimationNames> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrAnimationNames>::CreateInstance (&lInstance))))
	{
		const CAgentFileGestures &	lGestures = pAgentFile.GetGestures ();
		INT_PTR						lNdx;

		if	(lGestures.mNames.GetSize() <= 0)
		{
			pAgentFile.ReadGestures ();
		}
		if	(lInstance->mAnimationNames = new CComVariant [lGestures.mNames.GetCount()+1])
		{
			for	(lNdx = 0; lNdx < (INT_PTR)lGestures.mNames.GetCount(); lNdx++)
			{
				lInstance->mAnimationNames [lNdx] = lGestures.mNames [lNdx];
			}
		}

		LogComErr (LogIfActive, lInstance->Init (&(lInstance->mAnimationNames[0]), &(lInstance->mAnimationNames[(INT_PTR)lGestures.mNames.GetCount()]), NULL));
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
	}
}

void DaSvrAnimationNames::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrAnimationNames::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrAnimationNames::Clone(IEnumVARIANT** ppEnum)
{
	HRESULT								lResult;
	CComObject<DaSvrAnimationNames> *	lClone = NULL;

	if	(
			(SUCCEEDED (LogComErr (LogIfActive, lResult = CComObject<DaSvrAnimationNames>::CreateInstance (&lClone))))
		&&	(SUCCEEDED (LogComErr (LogIfActive, lResult = lClone->Init (m_begin, m_end, GetUnknown()))))
		)
	{
		lClone->m_iter = m_iter;
		lResult = lClone->_InternalQueryInterface (__uuidof(IEnumVARIANT), (void**)ppEnum);
	}
	return lResult;
}
