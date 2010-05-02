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
#include "DaSvrSettings.h"
#include "DaGlobalConfig.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSettings::DaSvrSettings ()
:	CDaCmnSettings (&_AtlModule)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::DaSvrSettings (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSettings::~DaSvrSettings ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::~DaSvrSettings (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrSettings (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSettings * DaSvrSettings::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSettings> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSettings>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSettings::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrSettings::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrSettings::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::OnClientEnded"), this, m_dwRef);
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

STDMETHODIMP DaSvrSettings::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrSettings), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SoundEffectsEnabled (VARIANT_BOOL *SoundEffectsEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_SoundEffectsEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_SoundEffectsEnabled (SoundEffectsEnabled);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_SoundEffectsEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_BalloonEnabled (VARIANT_BOOL *BalloonEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_BalloonEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_BalloonEnabled (BalloonEnabled);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_BalloonEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_BalloonFont (IFontDisp **BalloonFont)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_BalloonFont"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_BalloonFont (BalloonFont);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_BalloonFont"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_TTSEnabled (VARIANT_BOOL *TTSEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_TTSEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_TTSEnabled (TTSEnabled);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_TTSEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_TTSSpeed (short *TTSSpeed)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_TTSSpeed"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_TTSSpeed (TTSSpeed);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_TTSSpeed"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SREnabled (VARIANT_BOOL *SREnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SREnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_SREnabled (SREnabled);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SREnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRHotKey (BSTR *SRHotKey)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SRHotKey"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_SRHotKey (SRHotKey);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SRHotKey"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRHotKeyTime (short *SRHotKeyTime)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_SRHotKeyTime"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_SRHotKeyTime (SRHotKeyTime);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_SRHotKeyTime"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRListeningTip (VARIANT_BOOL *SRListeningTip)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningTip"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_SRListeningTip (SRListeningTip);

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningTip"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRListeningPrompt (VARIANT_BOOL *SRListeningPrompt)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningPrompt"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_SRListeningPrompt (SRListeningPrompt);

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningPrompt"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_AudioStatus (AudioStatusType *AudioStatus)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_AudioStatus"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnSettings::get_AudioStatus (AudioStatus);

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_AudioStatus"), this, m_dwRef);
	}
#endif
	return lResult;
}
