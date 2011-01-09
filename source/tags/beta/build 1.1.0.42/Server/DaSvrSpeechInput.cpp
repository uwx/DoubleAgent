/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechInput::DaSvrSpeechInput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::DaSvrSpeechInput (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSpeechInput::~DaSvrSpeechInput ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::~DaSvrSpeechInput (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
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

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrSpeechInput>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSpeechInput::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}

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

void DaSvrSpeechInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechInput::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrSpeechInput::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrSpeechInput::OnClientEnded"), this, max(m_dwRef,-1));
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

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetInstalled (long *Installed)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetInstalled"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(Installed)
	{
		(*Installed) = FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetInstalled"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetEnabled (long *Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetEnabled"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaSettingsConfig().mSrEnabled ? S_OK : S_FALSE;

	if	(Enabled)
	{
		(*Enabled) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetEnabled"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetHotKey (BSTR *HotKey)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetHotKey"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(HotKey)
	{
		CDaSettingsConfig	lConfig;
		CAtlString			lKeyName;
		CAtlString			lModName;
		UINT				lHotKeyCode;
		long				lHotKeyScan;

		lConfig.LoadConfig ();
		if	(
				(lHotKeyCode = LOBYTE (lConfig.mSrHotKey))
			&&	(lHotKeyScan = MapVirtualKey (lHotKeyCode, MAPVK_VK_TO_VSC))
			&&	(GetKeyNameText (lHotKeyScan<<16, lKeyName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
			)
		{
			lKeyName.ReleaseBuffer ();

			if	(
					(HIBYTE (lConfig.mSrHotKey) & HOTKEYF_ALT)
				&&	(lHotKeyScan = MapVirtualKey (VK_MENU, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
			if	(
					(HIBYTE (lConfig.mSrHotKey) & HOTKEYF_SHIFT)
				&&	(lHotKeyScan = MapVirtualKey (VK_SHIFT, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
			if	(
					(HIBYTE (lConfig.mSrHotKey) & HOTKEYF_CONTROL)
				&&	(lHotKeyScan = MapVirtualKey (VK_CONTROL, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
		}
		else
		{
			lKeyName.Empty ();
		}
		(*HotKey) = lKeyName.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetHotKey"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetLCID (unsigned long *LCIDCurrent)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetLCID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(LCIDCurrent)
	{
		(*LCIDCurrent) = 0;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetLCID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetEngine (BSTR *Engine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetEngine"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(Engine)
	{
		(*Engine) = CAtlString().AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetEngine"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::SetEngine (BSTR Engine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::SetEngine"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::SetEngine"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechInput::GetListeningTip (long *ListeningTip)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::GetListeningTip"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaSettingsConfig().mSrListeningTip ? S_OK : S_FALSE;

	if	(ListeningTip)
	{
		(*ListeningTip) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::GetListeningTip"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
