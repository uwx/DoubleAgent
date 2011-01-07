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
#include "DaSvrSREngine.h"
#include "Sapi5Inputs.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED			MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSREngine::DaSvrSREngine ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngine::DaSvrSREngine (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSREngine::~DaSvrSREngine ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngine::~DaSvrSREngine (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSREngine * DaSvrSREngine::CreateInstance (CSapi5InputInfo * pInputInfo, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSREngine> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrSREngine>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize (pInputInfo);
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSREngine::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrSREngine::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSREngine::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrSREngine::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrSREngine::OnClientEnded"), this, max(m_dwRef,-1));
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

STDMETHODIMP DaSvrSREngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrSREngine), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_SRModeID (BSTR *SRModeID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_SRModeID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_SRModeID (SRModeID);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_SRModeID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_DisplayName (BSTR *DisplayName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_DisplayName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_DisplayName (DisplayName);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_DisplayName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_Manufacturer (BSTR *Manufacturer)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_Manufacturer"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_Manufacturer (Manufacturer);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_Manufacturer"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSREngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::GetVersion"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::GetVersion (MajorVersion, MinorVersion);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::GetVersion"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_LanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_LanguageID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_LanguageID (LanguageID);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_LanguageID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_LanguageName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_LanguageName (EnglishName, LanguageName);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_LanguageName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_LanguageIDs (SAFEARRAY **LanguageIDs)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_LanguageIDs"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_LanguageIDs (LanguageIDs);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_LanguageIDs"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}


HRESULT STDMETHODCALLTYPE DaSvrSREngine::get_LanguageNames (VARIANT_BOOL EnglishNames, SAFEARRAY **LanguageNames)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSREngine::get_LanguageNames"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnSREngine::get_LanguageNames (EnglishNames, LanguageNames);

	PutServerError (lResult, __uuidof(IDaSvrSREngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSREngine::get_LanguageNames"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
