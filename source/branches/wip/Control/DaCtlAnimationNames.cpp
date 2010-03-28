/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControl.h"
#include "DaCtlAnimationNames.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
/////////////////////////////////////////////////////////////////////////////

CDaCtlAnimationNames::CDaCtlAnimationNames ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames::CDaCtlAnimationNames (%d)"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

CDaCtlAnimationNames::~CDaCtlAnimationNames ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames::~CDaCtlAnimationNames (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mAnimationNames != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames Attached [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mAnimationNames.GetInterfacePtr());
		}
	}
	catch AnyExceptionSilent
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlAnimationNames::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames::FinalRelease (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void CDaCtlAnimationNames::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames::Terminate [%u] [%p(%u)]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
		}
#endif
#endif
		if	(pFinal)
		{
			mOwner = NULL;
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames::Terminate [%u] Done [%d]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlAnimationNames::SetOwner (CDaCtlCharacter * pOwner)
{
	if	(
			(mOwner = pOwner)
		&&	(mOwner->mServerObject != NULL)
		)
	{
		IUnknownPtr	lServerObject;
		mOwner->mServerObject->GetAnimationNames (&lServerObject);
		mServerObject = lServerObject;
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlAnimationNames::SetOwner (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

CDaCtlCharacter * CDaCtlAnimationNames::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCtlAnimationNames::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlAnimationNames::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlAnimationNames), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlAnimationNames), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlAnimationNames::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlAnimationNames::get__NewEnum"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(mServerObject == NULL)
	{
		lResult = AGENTCTLERROR_SERVERINIT;
	}
	else
	{
		mServerObject->AddRef ();
		(*ppunkEnum) = mServerObject;
	}

	PutControlError (lResult, __uuidof(IDaCtlAnimationNames));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlAnimationNames::get__NewEnum"), this, m_dwRef);
	}
#endif
	return lResult;
}
