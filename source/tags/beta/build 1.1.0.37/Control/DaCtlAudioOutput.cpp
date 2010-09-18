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
#include "DaControlMod.h"
#include "DaCtlAudioOutput.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlAudioOutput::DaCtlAudioOutput ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::DaCtlAudioOutput (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlAudioOutput::~DaCtlAudioOutput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::~DaCtlAudioOutput (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mAudioOutput != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput Attached [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), mOwner->mAudioOutput.GetInterfacePtr());
		}
	}
	catch AnyExceptionSilent
#endif

	Terminate (true);
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlAudioOutput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::FinalRelease [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void DaCtlAudioOutput::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::Terminate [%u] [%p(%d)]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
		}
#endif
#endif

		Disconnect (pFinal);
		mOwner = NULL;

#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

void DaCtlAudioOutput::Disconnect (bool pFinal)
{
	if	(pFinal)
	{
		mServerObject.Detach ();
	}
	else
	{
		SafeFreeSafePtr (mServerObject);
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlAudioOutput::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
		mServerObject = mOwner->mServer;
		if	(!mServerObject)
		{
			lResult = E_FAIL;
		}
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::SetOwner (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	return lResult;
}

DaControl * DaCtlAudioOutput::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlAudioOutput::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlAudioOutput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlAudioOutput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlAudioObject), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlAudioObjectEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlAudioOutput::get_Enabled (VARIANT_BOOL *AudioEnabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::get_Enabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;
	long	lEnabled = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetEnabled (&lEnabled);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(AudioEnabled)
	{
		(*AudioEnabled) = lEnabled ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::get_Enabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlAudioOutput::get_SoundEffects (VARIANT_BOOL *SoundEffects)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::get_SoundEffects"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;
	long	lSoundEffects = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetUsingSoundEffects (&lSoundEffects);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(SoundEffects)
	{
		(*SoundEffects) = lSoundEffects ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::get_SoundEffects"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlAudioOutput::get_Status (short *Available)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::get_Status"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;
	long	Status = 0;

	if	(!Available)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetStatus (&Status);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*Available) = (short)Status;
	}

	PutControlError (lResult, __uuidof(IDaCtlAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlAudioOutput::get_Status"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
