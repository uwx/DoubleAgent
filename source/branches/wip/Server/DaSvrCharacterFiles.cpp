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
#include "DaServerApp.h"
#include "DaSvrCharacterFiles.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF,_LOG_INSTANCE)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#define	_DEBUG_ABANDONED	_LOG_ABANDONED
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacterFiles::DaSvrCharacterFiles ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::DaSvrCharacterFiles (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCharacterFiles::~DaSvrCharacterFiles ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::~DaSvrCharacterFiles (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrCharacterFiles (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacterFiles * DaSvrCharacterFiles::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrCharacterFiles> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrCharacterFiles>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize ();
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrCharacterFiles::Terminate (bool pFinal, bool pAbandonned)
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
			if	(pAbandonned)
			{
#ifdef	_DEBUG_ABANDONED
				if	(LogIsActive (_DEBUG_ABANDONED))
				{
					LogMessage (_DEBUG_ABANDONED, _T("[%p(%d)] DaSvrCharacterFiles SKIP CoDisconnectObject"), this, max(m_dwRef,-1));
				}
#endif
			}
			else
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

void DaSvrCharacterFiles::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, !_VerifyClientLifetime());
}

void DaSvrCharacterFiles::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrCharacterFiles::OnClientEnded"), this, max(m_dwRef,-1));
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

STDMETHODIMP DaSvrCharacterFiles::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrCharacterFiles), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_FilePaths"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::get_FilePaths (FilePaths);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_FilePaths"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_SearchPath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::get_SearchPath (SearchPath);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_SearchPath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::put_SearchPath (BSTR SearchPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::put_SearchPath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::put_SearchPath (SearchPath);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::put_SearchPath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultSearchPath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::get_DefaultSearchPath (DefaultSearchPath);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultSearchPath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_Filter (long *Filter)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_Filter"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::get_Filter (Filter);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_Filter"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::put_Filter (long Filter)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::put_Filter [%8.8X]"), this, max(m_dwRef,-1), Filter);
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::put_Filter (Filter);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::put_Filter"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_DefaultFilePath (BSTR *DefaultFilePath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFilePath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::get_DefaultFilePath (DefaultFilePath);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFilePath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_DefaultFileName (BSTR *DefaultFileName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFileName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCharacterFiles::get_DefaultFileName (DefaultFileName);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFileName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
