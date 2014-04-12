/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCore.h"
#include "FileDownload.h"
#include "EventNotify.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_BINDING	LogNormal
//#define	_DEBUG_PROGRESS	LogNormal
//#define	_DEBUG_ASYNC	LogNormal
//#define	_TRACE_THREADS	(GetProfileDebugInt(_T("TraceThreads"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Download"),LogVerbose,true)&0xFFFF|LogTime)
//#define	_LOG_STATUS		LogNormal
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

#ifndef	_LOG_STATUS
#define	_LOG_STATUS		LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CFileDownload)

CFileDownload::CFileDownload ()
:	mUserData (0),
	mNotify (NULL),
	mNotifyThreadId (0),
	mBindFlags (0),
	mDownloadSize (0),
	mDownloadProgress (0),
	mDownloadStarted (false),
	mDownloadCancelling (false),
	mDownloadComplete (S_FALSE)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CFileDownload::CFileDownload (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	m_dwRef = 1;
}

CFileDownload::~CFileDownload ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CFileDownload::~CFileDownload (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	CancelDownload ();
	m_dwRef = 0;
}

CFileDownload* CFileDownload::CreateInstance (LPCTSTR pURL)
{
	CFileDownload*	lInstance;

	if	(lInstance = new CComObjectNoLock <CFileDownload>)
	{
		lInstance->mURL = pURL;
		lInstance->SetBindFlags (BINDF_RESYNCHRONIZE | BINDF_GETFROMCACHE_IF_NET_FAIL);
	}
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

tBstrPtr CFileDownload::GetURL () const
{
	BSTR	lRet = NULL;
	CLockCS	lLock (mLock);
	try
	{
		lRet = mURL.AllocSysString ();
	}
	catch AnyExceptionSilent
	return lRet;
}

tBstrPtr CFileDownload::GetCacheName () const
{
	BSTR	lRet = NULL;
	CLockCS	lLock (mLock);
	try
	{
		lRet = mCacheName.AllocSysString ();
	}
	catch AnyExceptionSilent
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

DWORD CFileDownload::GetBindFlags () const
{
	CLockCS	lLock (mLock);
	return mBindFlags;
}

ULONG CFileDownload::GetDownloadSize () const
{
	CLockCS	lLock (mLock);
	return mDownloadSize;
}

ULONG CFileDownload::GetDownloadProgress () const
{
	CLockCS	lLock (mLock);
	return mDownloadProgress;
}

/////////////////////////////////////////////////////////////////////////////

bool CFileDownload::IsDownloadStarting () const
{
	bool	lRet = false;

	if	(this)
	{
		CLockCS	lLock (mLock);

		try
		{
			if	(
					(!mDownloadStarted)
				&&	(mDownloadComplete != S_FALSE)
				)
			{
				if	(FindGatedInstance<CFileDownload> (const_cast<CFileDownload*>(this)))
				{
					lRet = true;
				}
			}
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

bool CFileDownload::IsDownloadStarted () const
{
	bool	lRet = false;

	if	(this)
	{
		CLockCS	lLock (mLock);
		lRet = mDownloadStarted;
	}
	return lRet;
}

bool CFileDownload::IsDownloadCancelling () const
{
	bool	lRet = false;

	if	(this)
	{
		CLockCS	lLock (mLock);
		lRet = mDownloadCancelling;
	}
	return lRet;
}

HRESULT CFileDownload::IsDownloadComplete () const
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
		CLockCS	lLock (mLock);
		lResult = mDownloadComplete;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DWORD CFileDownload::SetBindFlags (DWORD pBindFlags)
{
	mBindFlags = BINDF_NO_UI | BINDF_SILENTOPERATION;
	mBindFlags |= pBindFlags & (BINDF_GETNEWESTVERSION | BINDF_RESYNCHRONIZE | BINDF_GETFROMCACHE_IF_NET_FAIL | BINDF_FWD_BACK | BINDF_SILENTOPERATION | BINDF_IGNORESECURITYPROBLEM | BINDF_ENFORCERESTRICTED);
	return mBindFlags;
}

DWORD CFileDownload::SetReloadMode (bool pReload)
{
	if	(pReload)
	{
		mBindFlags |= BINDF_GETNEWESTVERSION;
		mBindFlags &= ~(BINDF_RESYNCHRONIZE | BINDF_GETFROMCACHE_IF_NET_FAIL | BINDF_FWD_BACK);
	}
	else
	{
		mBindFlags |= BINDF_FWD_BACK;
		mBindFlags &= ~(BINDF_GETNEWESTVERSION | BINDF_RESYNCHRONIZE | BINDF_GETFROMCACHE_IF_NET_FAIL);
	}
	return mBindFlags;
}

DWORD CFileDownload::SetResynchronizeMode (bool pResynchronize)
{
	if	(pResynchronize)
	{
		mBindFlags |= BINDF_RESYNCHRONIZE | BINDF_GETFROMCACHE_IF_NET_FAIL;
		mBindFlags &= ~(BINDF_GETNEWESTVERSION | BINDF_FWD_BACK);
	}
	else
	{
		mBindFlags |= BINDF_FWD_BACK;
		mBindFlags &= ~(BINDF_GETNEWESTVERSION | BINDF_RESYNCHRONIZE | BINDF_GETFROMCACHE_IF_NET_FAIL);
	}
	return mBindFlags;
}

DWORD CFileDownload::SetSecurityMode (bool pEnforeSecurity)
{
	if	(pEnforeSecurity)
	{
		mBindFlags &= ~BINDF_IGNORESECURITYPROBLEM;
	}
	else
	{
		mBindFlags |= BINDF_IGNORESECURITYPROBLEM;
	}
	return mBindFlags;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CFileDownload::Download (LPUNKNOWN pActiveXContext, CEventNotify* pNotify)
{
	HRESULT		lResult;
	CAtlString	lCacheName;

	{
		CLockCS	lLock (mLock);
		try
		{
			mBindFlags &= ~(BINDF_ASYNCHRONOUS | BINDF_DIRECT_READ | BINDF_NOWRITECACHE | BINDF_PRAGMA_NO_CACHE);
			mBindFlags |= BINDF_NEEDFILE;
			mDownloadSize = 0;
			mDownloadProgress = 0;
			mDownloadStarted = false;
			mDownloadCancelling = false;
			mDownloadComplete = S_FALSE;
			mCacheName.Empty ();

			SafeFreeSafePtr (mBindStatusCallback);
			SafeFreeSafePtr (mBindStatusMarshall);
			SafeFreeSafePtr (mContextMarshall);
			if	(mNotify = pNotify)
			{
				mNotifyThreadId = GetCurrentThreadId ();
				mBindFlags |= BINDF_NO_UI | BINDF_SILENTOPERATION;	// Just to ensure no UI from the worker thread
			}
			else
			{
				mNotifyThreadId = 0;
				mDownloadStarted = true;
			}
		}
		catch AnyExceptionSilent
	}

	if	(pNotify)
	{
		{
			CLockCS	lLock (mLock);
			try
			{
				if	(pActiveXContext)
				{
					LogComErr (LogNormal|LogTime, CoMarshalInterThreadInterfaceInStream (__uuidof(IUnknown), pActiveXContext, &mContextMarshall));
				}
				lResult = LogComErr (LogNormal|LogTime, CoMarshalInterThreadInterfaceInStream (__uuidof(IBindStatusCallback), (IBindStatusCallback*)this, &mBindStatusMarshall));
			}
			catch AnyExceptionSilent
		}

		if	(SUCCEEDED (lResult))
		{
			if	(QueueUserWorkItem (AsyncThreadProc, PutGatedInstance<CFileDownload> (this), WT_EXECUTELONGFUNCTION))
			{
				lResult = S_FALSE;
			}
			else
			{
				lResult = HRESULT_FROM_WIN32 (GetLastError ());
				if	(SUCCEEDED (lResult))
				{
					lResult = E_FAIL;
				}
			}
		}
	}
	else
	{
		lResult = LogComErr (LogNormal|LogTime, URLDownloadToCacheFile (pActiveXContext, mURL, lCacheName.GetBuffer(MAX_PATH), MAX_PATH, 0, this));
		lCacheName.ReleaseBuffer ();

		{
			CLockCS	lLock (mLock);
			try
			{
				mDownloadStarted = false;
				if	(SUCCEEDED (lResult))
				{
					mCacheName = lCacheName;
				}
			}
			catch AnyExceptionSilent
		}
	}

	if	(FAILED (lResult))
	{
		CLockCS	lLock (mLock);
		try
		{
			mDownloadComplete = lResult;
		}
		catch AnyExceptionSilent
	}
	return lResult;
}

bool CFileDownload::CancelDownload ()
{
	bool	lRet = false;

	if	(this)
	{
		{
			CLockCS	lLock (mLock);
			try
			{
				if	(
						(!mURL.IsEmpty ())
					&&	(mDownloadComplete == S_FALSE)
					)
				{
					mDownloadCancelling = true;
					lRet = true;
#ifdef	_DEBUG_BINDING
					LogMessage (_DEBUG_BINDING, _T("[%p] Cancelling [%s]"), this, mURL);
#endif
				}
			}
			catch AnyExceptionSilent
		}

		NotGatedInstance<CFileDownload> (this);

		{
			CLockCS	lLock (mLock);
			try
			{
				SafeFreeSafePtr (mBindStatusCallback);
				SafeFreeSafePtr (mContextMarshall);
				SafeFreeSafePtr (mBindStatusMarshall);
#ifdef	_DEBUG_BINDING
				if	(lRet)
				{
					LogMessage (_DEBUG_BINDING, _T("[%p] Cancelled [%s]"), this, mURL);
				}
#endif
			}
			catch AnyExceptionSilent
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

DWORD WINAPI CFileDownload::AsyncThreadProc (LPVOID lpParameter)
{
	HRESULT			lResult = E_FAIL;
	CFileDownload*	lThis = NULL;
	CAtlString		lURL;
	IUnknownPtr		lActiveXContext;
	CAtlString		lCacheName;

	CoInitialize (NULL);

#ifdef	_TRACE_THREADS
	LogMessage (_TRACE_THREADS, _T("CFileDownload::AsyncThreadProc"));
#endif
#ifdef	_DEBUG_ASYNC
	LogMessage (_DEBUG_ASYNC, _T("[%p] AsyncThreadProc"), lpParameter);
#endif

	if	(LockGatedInstance<CFileDownload> (lpParameter, lThis, 100))
	{
		try
		{
			CLockCS	lLock (lThis->mLock);
			try
			{
				if	(lThis->mContextMarshall != NULL)
				{
					if	(SUCCEEDED (LogComErr (LogNormal|LogTime, CoGetInterfaceAndReleaseStream (lThis->mContextMarshall, __uuidof(IUnknown), (void**) &lActiveXContext))))
					{
						lThis->mContextMarshall.Detach ();
					}
				}
				if	(SUCCEEDED (LogComErr (LogNormal|LogTime, CoGetInterfaceAndReleaseStream (lThis->mBindStatusMarshall, __uuidof(IBindStatusCallback), (void**) &lThis->mBindStatusCallback))))
				{
					lThis->mBindStatusMarshall.Detach ();
					lURL = lThis->mURL;
					lThis->mDownloadStarted = true;
				}
			}
			catch AnyExceptionSilent
		}
		catch AnyExceptionDebug

		FreeGatedInstance<CFileDownload> (lpParameter, lThis);
	}
#ifdef	_DEBUG_ASYNC
	else
	{
		LogMessage (_DEBUG_ASYNC, _T("[%p] LockGatedInstance failed"), lpParameter);
	}
#endif

	if	(!lURL.IsEmpty ())
	{
		if	(LockGatedInstance<CFileDownload> (lpParameter, lThis, 100))
		{
			try
			{
				lResult = LogComErr (LogNormal|LogTime, URLDownloadToCacheFile (lActiveXContext, lURL, lCacheName.GetBuffer(MAX_PATH), MAX_PATH, 0, lThis), lURL);
				lCacheName.ReleaseBuffer ();
			}
			catch AnyExceptionDebug

			FreeGatedInstance<CFileDownload> (lpParameter, lThis);
		}
#ifdef	_DEBUG_ASYNC
		else
		{
			LogMessage (_DEBUG_ASYNC, _T("[%p] LockGatedInstance failed"), lpParameter);
		}
#endif
	}

	if	(LockGatedInstance<CFileDownload> (lpParameter, lThis, 100))
	{
		IBindStatusCallbackPtr	lBindStatusCallback;

		try
		{
			CLockCS	lLock (lThis->mLock);
			try
			{
				lThis->mDownloadStarted = false;
				if	(SUCCEEDED (lResult))
				{
					lThis->mCacheName = lCacheName;
				}
				else
				{
					lThis->mDownloadComplete = lResult;
				}
				lBindStatusCallback.Attach (lThis->mBindStatusCallback.Detach());
			}
			catch AnyExceptionSilent
		}
		catch AnyExceptionDebug

		FreeGatedInstance<CFileDownload> (lpParameter, lThis);
		SafeFreeSafePtr (lBindStatusCallback); // Must do this outside all locks to avoid a race condition with the notify target thread.
	}
#ifdef	_DEBUG_ASYNC
	else
	{
		LogMessage (_DEBUG_ASYNC, _T("[%p] LockGatedInstance failed"), lpParameter);
	}
#endif

#ifdef	_DEBUG_ASYNC
	LogMessage (_DEBUG_ASYNC, _T("[%p] AsyncThreadProc Done [%s]"), lpParameter, lURL);
#endif
#ifdef	_TRACE_THREADS
	LogMessage (_TRACE_THREADS, _T("CFileDownload::AsyncThreadProc End"));
#endif

	CoUninitialize ();
	return ERROR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CFileDownload::OnProgress (ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::OnProgress"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	IBindStatusCallbackPtr	lBindStatusCallback;
	CEventNotify*			lNotify = NULL;

	{
		CLockCS	lLock (mLock);
		try
		{
			if	(GetCurrentThreadId() == mNotifyThreadId)
			{
				lNotify = mNotify;
			}
			else
			{
				lBindStatusCallback = mBindStatusCallback;
			}
		}
		catch AnyExceptionSilent
	}

#ifdef	_DEBUG_PROGRESS
	CAtlString	lStatusStr;

    switch (ulStatusCode)
    {
		case BINDSTATUS_BEGINDOWNLOADDATA:		lStatusStr = _T("BEGINDOWNLOADDATA"); break;
	    case BINDSTATUS_DOWNLOADINGDATA:		lStatusStr = _T("DOWNLOADINGDATA"); break;
		case BINDSTATUS_ENDDOWNLOADDATA:		lStatusStr = _T("ENDDOWNLOADDATA"); break;
		case BINDSTATUS_FINDINGRESOURCE:		lStatusStr = _T("FINDINGRESOURCE"); break;
		case BINDSTATUS_CONNECTING:				lStatusStr = _T("CONNECTING"); break;
		case BINDSTATUS_SENDINGREQUEST:			lStatusStr = _T("SENDINGREQUEST"); break;
		case BINDSTATUS_MIMETYPEAVAILABLE:		lStatusStr = _T("MIMETYPEAVAILABLE"); break;
		case BINDSTATUS_CACHEFILENAMEAVAILABLE:	lStatusStr = _T("CACHEFILENAMEAVAILABLE"); break;
		case BINDSTATUS_USINGCACHEDCOPY:		lStatusStr = _T("USINGCACHEDCOPY"); break;
		case BINDSTATUS_P3P_HEADER:				lStatusStr = _T("P3P_HEADER"); break;
		case BINDSTATUS_COOKIE_STATE_ACCEPT:	lStatusStr = _T("COOKIE_STATE_ACCEPT"); break;
		case BINDSTATUS_COOKIE_SENT:			lStatusStr = _T("COOKIE_SENT"); break;
		case BINDSTATUS_REDIRECTING:			lStatusStr = _T("REDIRECTING"); break;
		default:								lStatusStr.Format (_T("[%u]"), ulStatusCode);
	}

    switch (ulStatusCode)
    {
		case BINDSTATUS_BEGINDOWNLOADDATA:
		{
			LogMessage (_DEBUG_PROGRESS, _T("  %s [%u] of [%u] [%ls]"), lStatusStr, ulProgress, ulProgressMax, szStatusText);
		}	break;
	    case BINDSTATUS_DOWNLOADINGDATA:
		{
			LogMessage (_DEBUG_PROGRESS, _T("  %s [%u] of [%u] [%ls]"), lStatusStr, ulProgress, ulProgressMax, szStatusText);
		}	break;
		case BINDSTATUS_ENDDOWNLOADDATA:
		{
			LogMessage (_DEBUG_PROGRESS, _T("  %s [%u] of [%u] [%ls]"), lStatusStr, ulProgress, ulProgressMax, szStatusText);
		}	break;
		default:
		{
			LogMessage (_DEBUG_PROGRESS, _T("  %s [%ls]"), lStatusStr, szStatusText);
		}
	}
#endif

	{
		CLockCS	lLock (mLock);
		try
		{
			if	(
					(ulStatusCode == BINDSTATUS_DOWNLOADINGDATA)
				&&	(mDownloadCancelling)
				)
			{
#ifdef	_DEBUG_BINDING
				LogMessage (_DEBUG_BINDING, _T("[%p] Cancelled [%s]"), this, mURL);
#endif
				lResult = mDownloadComplete = E_ABORT;
			}

			if	(ulStatusCode == BINDSTATUS_CACHEFILENAMEAVAILABLE)
			{
				mCacheName = szStatusText;
#ifdef	_DEBUG_BINDING
				LogMessage (_DEBUG_BINDING, _T("[%p] Caching [%s] to [%s]"), this, mURL, mCacheName);
#endif
			}
			else
			if	(
					(ulStatusCode == BINDSTATUS_BEGINDOWNLOADDATA)
				||	(ulStatusCode == BINDSTATUS_DOWNLOADINGDATA)
				||	(ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
				)
			{
				mDownloadProgress = ulProgress;
				mDownloadSize = ulProgressMax;
			}
			if	(ulStatusCode == BINDSTATUS_BEGINDOWNLOADDATA)
			{
				mDownloadStarted = true;
				mDownloadComplete = S_FALSE;
			}
			if	(ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
			{
				if	(SUCCEEDED (mDownloadComplete))
				{
					mDownloadComplete = S_OK;
				}
#ifdef	_DEBUG_BINDING
				if	(LogIsActive (_DEBUG_BINDING))
				{
					LogComErrAnon (MinLogLevel(_DEBUG_BINDING,LogAlways), mDownloadComplete, _T("[%p] Complete [%s] in [%s]"), this, mURL, mCacheName);
				}
#endif
				if	(mNotify == lNotify)
				{
					mNotify = NULL;
				}
			}
		}
		catch AnyExceptionSilent
	}

	if	(ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
	{
		if	(lBindStatusCallback != NULL)
		{
			//
			//	Redirect to the main thread for the complete notification
			//
			lResult = lBindStatusCallback->OnProgress (ulProgress, ulProgressMax, ulStatusCode, szStatusText);
		}
		else
		if	(lNotify != NULL)
		{
			try
			{
				lNotify->_DownloadComplete (this);
			}
			catch AnyExceptionSilent
		}
	}

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CFileDownload::OnStartBinding (DWORD dwReserved, IBinding *pib)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::OnStartBinding"), this, max(m_dwRef,-1));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFileDownload::OnStopBinding (HRESULT hresult, LPCWSTR szError)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::OnStopBinding"), this, max(m_dwRef,-1));
	LogComErrAnon (_LOG_STATUS, hresult, _T("[%ls]"), szError);
#endif
	if	(FAILED (hresult))
	{
		CLockCS	lLock (mLock);
		try
		{
			mDownloadComplete = hresult;
		}
		catch AnyExceptionSilent
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFileDownload::GetBindInfo (DWORD *grfBINDF, BINDINFO *pbindinfo)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::GetBindInfo"), this, max(m_dwRef,-1));
#endif

	if	(grfBINDF)
	{
		CLockCS	lLock (mLock);
		try
		{
			*grfBINDF = mBindFlags;
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_BINDING
	if	(grfBINDF)
	{
		CAtlString	lFlagsStr;

		if	(*grfBINDF & BINDF_ASYNCHRONOUS)
		{
			lFlagsStr += _T("BINDF_ASYNCHRONOUS ");
		}
		if	(*grfBINDF & BINDF_ASYNCSTORAGE)
		{
			lFlagsStr += _T("BINDF_ASYNCSTORAGE ");
		}
		if	(*grfBINDF & BINDF_NOPROGRESSIVERENDERING)
		{
			lFlagsStr += _T("BINDF_NOPROGRESSIVERENDERING ");
		}
		if	(*grfBINDF & BINDF_OFFLINEOPERATION)
		{
			lFlagsStr += _T("BINDF_OFFLINEOPERATION ");
		}
		if	(*grfBINDF & BINDF_GETNEWESTVERSION)
		{
			lFlagsStr += _T("BINDF_GETNEWESTVERSION ");
		}
		if	(*grfBINDF & BINDF_NOWRITECACHE)
		{
			lFlagsStr += _T("BINDF_NOWRITECACHE ");
		}
		if	(*grfBINDF & BINDF_NEEDFILE)
		{
			lFlagsStr += _T("BINDF_NEEDFILE ");
		}
		if	(*grfBINDF & BINDF_PULLDATA)
		{
			lFlagsStr += _T("BINDF_PULLDATA ");
		}
		if	(*grfBINDF & BINDF_IGNORESECURITYPROBLEM)
		{
			lFlagsStr += _T("BINDF_IGNORESECURITYPROBLEM ");
		}
		if	(*grfBINDF & BINDF_RESYNCHRONIZE)
		{
			lFlagsStr += _T("BINDF_RESYNCHRONIZE ");
		}
		if	(*grfBINDF & BINDF_HYPERLINK)
		{
			lFlagsStr += _T("BINDF_HYPERLINK ");
		}
		if	(*grfBINDF & BINDF_NO_UI)
		{
			lFlagsStr += _T("BINDF_NO_UI ");
		}
		if	(*grfBINDF & BINDF_SILENTOPERATION)
		{
			lFlagsStr += _T("BINDF_SILENTOPERATION ");
		}
		if	(*grfBINDF & BINDF_PRAGMA_NO_CACHE)
		{
			lFlagsStr += _T("BINDF_PRAGMA_NO_CACHE ");
		}
		if	(*grfBINDF & BINDF_GETCLASSOBJECT)
		{
			lFlagsStr += _T("BINDF_GETCLASSOBJECT ");
		}
		if	(*grfBINDF & BINDF_FREE_THREADED)
		{
			lFlagsStr += _T("BINDF_FREE_THREADED ");
		}
		if	(*grfBINDF & BINDF_DIRECT_READ)
		{
			lFlagsStr += _T("BINDF_DIRECT_READ ");
		}
		if	(*grfBINDF & BINDF_FORMS_SUBMIT)
		{
			lFlagsStr += _T("BINDF_FORMS_SUBMIT ");
		}
		if	(*grfBINDF & BINDF_GETFROMCACHE_IF_NET_FAIL)
		{
			lFlagsStr += _T("BINDF_GETFROMCACHE_IF_NET_FAIL ");
		}
		if	(*grfBINDF & BINDF_FROMURLMON)
		{
			lFlagsStr += _T("BINDF_FROMURLMON ");
		}
		if	(*grfBINDF & BINDF_FWD_BACK)
		{
			lFlagsStr += _T("BINDF_FWD_BACK ");
		}
		if	(*grfBINDF & BINDF_PREFERDEFAULTHANDLER)
		{
			lFlagsStr += _T("BINDF_PREFERDEFAULTHANDLER ");
		}
		if	(*grfBINDF & BINDF_ENFORCERESTRICTED)
		{
			lFlagsStr += _T("BINDF_ENFORCERESTRICTED ");
		}
		lFlagsStr.TrimRight ();
		LogMessage (_DEBUG_BINDING, _T("[%p] Binding [%8.8X] [%s] [%s]"), this, *grfBINDF, lFlagsStr, mURL);
	}
#endif
#ifdef	_DEBUG_BINDING_NOT
	if	(pbindinfo)
	{
		CAtlString	lFlagsStr;
		CAtlString	lVerbStr;
		CAtlString	lCodePageStr;

		lFlagsStr.Format (_T("%8.8X"), pbindinfo->grfBindInfoF);
		if	(pbindinfo->grfBindInfoF & BINDINFOF_URLENCODESTGMEDDATA)
		{
			lFlagsStr += _T(" URLENCODESTGMEDDATA");
		}
		if	(pbindinfo->grfBindInfoF & BINDINFOF_URLENCODEDEXTRAINFO)
		{
			lFlagsStr += _T(" URLENCODEDEXTRAINFO");
		}

		switch (pbindinfo->dwBindVerb)
		{
			case BINDVERB_GET:		lVerbStr = _T("GET"); break;
			case BINDVERB_PUT:		lVerbStr = _T("PUT"); break;
			case BINDVERB_POST:		lVerbStr = _T("POST"); break;
			case BINDVERB_CUSTOM:	lVerbStr.Format (_T("Custom %ls"), pbindinfo->szCustomVerb); break;
			default:				lVerbStr.Format (_T("%u"), pbindinfo->dwBindVerb);
		}

		switch (pbindinfo->dwCodePage)
		{
			case CP_ACP:		lCodePageStr = _T("ANSI"); break;
			case CP_OEMCP:		lCodePageStr = _T("OEM"); break;
			case CP_THREAD_ACP:	lCodePageStr = _T("Thread ANSI"); break;
			case CP_UTF7:		lCodePageStr = _T("UTF7"); break;
			case CP_UTF8:		lCodePageStr = _T("UTF8"); break;
			default:			lCodePageStr.Format (_T("%u"), pbindinfo->dwCodePage);
		}

		LogMessage (_DEBUG_BINDING, _T("BindInfo [%u]"), pbindinfo->cbSize);
		LogMessage (_DEBUG_BINDING, _T("  Flags      [%s]"), lFlagsStr);
		LogMessage (_DEBUG_BINDING, _T("  CodePage   [%s]"), lCodePageStr);
		LogMessage (_DEBUG_BINDING, _T("  StgmedData [%u]"), pbindinfo->cbstgmedData);
		LogMessage (_DEBUG_BINDING, _T("  Extra      [%ls]"), pbindinfo->szExtraInfo);
		LogMessage (_DEBUG_BINDING, _T("  IID        [%s]"), CGuidStr::GuidName (pbindinfo->iid));
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CFileDownload::GetPriority (LONG *pnPriority)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::GetPriority"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!pnPriority)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pnPriority = THREAD_PRIORITY_NORMAL;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CFileDownload::OnDataAvailable (DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::OnDataAvailable"), this, max(m_dwRef,-1));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFileDownload::OnObjectAvailable (REFIID riid, IUnknown *punk)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::OnObjectAvailable"), this, max(m_dwRef,-1));
#endif
	return LogComErr (LogNormal|LogTime, E_NOTIMPL);
}

HRESULT STDMETHODCALLTYPE CFileDownload::OnLowResource (DWORD reserved)
{
#ifdef	_LOG_STATUS
	LogMessage (_LOG_STATUS, _T("[%p(%d)] CFileDownload::OnLowResource"), this, max(m_dwRef,-1));
#endif
	return S_OK;
}
