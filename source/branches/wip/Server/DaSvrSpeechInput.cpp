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
#include "DaSvrSpeechInput.h"
#include "DaGlobalConfig.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechInput::DaSvrSpeechInput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::DaSvrSpeechInput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSpeechInput::~DaSvrSpeechInput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::~DaSvrSpeechInput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrSpeechInput (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechInput * DaSvrSpeechInput::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSpeechInput> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSpeechInput>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSpeechInput::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrSpeechInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrSpeechInput::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::OnClientEnded"), this, m_dwRef);
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

STDMETHODIMP DaSvrSpeechInput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrSpeechInput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentSpeechInputProperties), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetInstalled (long *pbInstalled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetInstalled"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pbInstalled)
	{
		(*pbInstalled) = FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetInstalled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetEnabled (long *pbEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSpeechInputConfig().mEnabled ? S_OK : S_FALSE;

	if	(pbEnabled)
	{
		(*pbEnabled) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetHotKey (BSTR *pbszHotCharKey)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetHotKey"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pbszHotCharKey)
	{
//**/TODO GetKeyNameText()
		(*pbszHotCharKey) = CString().AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetHotKey"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetLCID (unsigned long *plcidCurrent)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetLCID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(plcidCurrent)
	{
		(*plcidCurrent) = 0;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetLCID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetEngine (BSTR *pbszEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetEngine"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pbszEngine)
	{
		(*pbszEngine) = CString().AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::SetEngine (BSTR bszEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::SetEngine"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::SetEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetListeningTip (long *pbListeningTip)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetListeningTip"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSpeechInputConfig().mListeningTip ? S_OK : S_FALSE;

	if	(pbListeningTip)
	{
		(*pbListeningTip) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetListeningTip"), this, m_dwRef);
	}
#endif
	return lResult;
}
