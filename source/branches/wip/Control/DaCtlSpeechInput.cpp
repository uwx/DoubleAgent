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
#include "DaCtlSpeechInput.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
/////////////////////////////////////////////////////////////////////////////

DaCtlSpeechInput::DaCtlSpeechInput ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::DaCtlSpeechInput (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlSpeechInput::~DaCtlSpeechInput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::~DaCtlSpeechInput (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mSpeechInput != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput Attached [%p]]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mSpeechInput.GetInterfacePtr());
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

void DaCtlSpeechInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::FinalRelease [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void DaCtlSpeechInput::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::Terminate [%u] [%p(%u)]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlSpeechInput::SetOwner (DaControl * pOwner)
{
	if	(mOwner = pOwner)
	{
		mServerObject = mOwner->mServer;
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::SetOwner (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

DaControl * DaCtlSpeechInput::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlSpeechInput::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlSpeechInput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlSpeechInput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlSpeechInput), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::get_Enabled (VARIANT_BOOL *VoiceEnabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Enabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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
	if	(VoiceEnabled)
	{
		(*VoiceEnabled) = lEnabled ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Enabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::get_Language (BSTR *Language)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Language"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;
	if	(Language)
	{
		(*Language) = NULL;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Language"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::get_HotKey (BSTR *HotKey)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_HotKey"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!HotKey)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*HotKey) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetHotKey (HotKey);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_HotKey"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::get_Installed (VARIANT_BOOL *VoiceInstalled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Installed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;
	if	(VoiceInstalled)
	{
		(*VoiceInstalled) = VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Installed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::get_Engine (BSTR *Engine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Engine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;
	if	(Engine)
	{
		(*Engine) = NULL;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_Engine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::put_Engine (BSTR Engine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::put_Engine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetEngine (Engine);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::put_Engine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSpeechInput::get_ListeningTip (VARIANT_BOOL *ListeningTip)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_ListeningTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lListeningTip = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetListeningTip (&lListeningTip);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(ListeningTip)
	{
		(*ListeningTip) = lListeningTip ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSpeechInput::get_ListeningTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
