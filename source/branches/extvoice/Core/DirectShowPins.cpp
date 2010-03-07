/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DirectShowPins.h"
#include "DirectShowEnums.h"
#include "DirectShowFilter.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM				LogNormal|LogHighVolume
//#define	_DEBUG_CONNECTION		LogNormal
//#define	_DEBUG_DYNCONNECTION	LogNormal|LogHighVolume
//#define	_DEBUG_ALLOCATOR		LogNormal
//#define	_DEBUG_QUERIES			LogNormal|LogHighVolume
//#define	_DEBUG_STREAM			LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM_EX		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM_DATA		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STATE			LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_PULL				LogNormal|LogTimeMs
//#define	_TRACE_PULL_DATA		LogNormal|LogHighVolume|LogTimeMs
//#define	_TRACE_PULL_STATE		LogNormal|LogHighVolume|LogTimeMs |LogToCache
//#define	_TRACE_PULL_BLOCK		LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_DirectShow"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

static const LONGLONG	sPullTimeScale = 10000000;

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectShowPin, CCmdTarget)
	INTERFACE_PART(CDirectShowPin, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectShowPin, __uuidof(IPin), Pin)
END_INTERFACE_MAP()

BEGIN_INTERFACE_MAP(CDirectShowPinIn, CDirectShowPin)
	INTERFACE_PART(CDirectShowPinIn, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectShowPinIn, __uuidof(IMemInputPin), MemInput)
	INTERFACE_PART(CDirectShowPinIn, __uuidof(IPinConnection), PinConnection)
END_INTERFACE_MAP()

BEGIN_INTERFACE_MAP(CDirectShowPinPull, CDirectShowPin)
	INTERFACE_PART(CDirectShowPinPull, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectShowPinPull, __uuidof(IAsyncReader), AsyncReader)
	INTERFACE_PART(CDirectShowPinPull, __uuidof(IPinFlowControl), FlowControl)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN_OWNED(CDirectShowPin, Pin)
IMPLEMENT_IUNKNOWN_OWNED(CDirectShowPinIn, MemInput)
IMPLEMENT_IUNKNOWN_OWNED(CDirectShowPinIn, PinConnection)
IMPLEMENT_IUNKNOWN_OWNED(CDirectShowPinPull, AsyncReader)
IMPLEMENT_IUNKNOWN_OWNED(CDirectShowPinPull, FlowControl)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowPin, CCmdTarget)

CDirectShowPin::CDirectShowPin (CDirectShowFilter & pFilter, PIN_DIRECTION pDirection, LPCTSTR pName, LPCTSTR pUniqueId)
:	mFilter (pFilter),
	mUniqueId (pUniqueId),
	mName (pName),
	mDirection (pDirection),
	mIsFlushing (false),
	mIsEndOfStream (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPin::CDirectShowPin (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	//AfxOleLockApp (); // No reference counting

	EnableAggregation();
	LogVfwErr (LogIfActive, MoDuplicateMediaType (mMediaType.Free(), &AM_MEDIA_TYPE_Safe()));
}

CDirectShowPin::~CDirectShowPin ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPin::~CDirectShowPin (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	SafeFreeSafePtr (mAllocator);
	SafeFreeSafePtr (mConnection);
	m_dwRef = 0;
	//AfxOleUnlockApp (); // No reference counting
}

void CDirectShowPin::OnFinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%s] [%p] CDirectShowPin::OnFinalRelease !!!"), mName, this);
	}
#endif
	// Should never happed - this filter is owned by another object and not reference counted
	//CCmdTarget::OnFinalRelease ()
}

LPUNKNOWN CDirectShowPin::GetInterfaceHook(const void* iid)
{
	LPUNKNOWN	lInterface = NULL;

#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CDirectShowPin::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	if	(
			(mDirection == PINDIR_OUTPUT)
		&&	(IsEqualGUID (*(GUID*)iid, __uuidof(IMediaSeeking)))
		)
	{
		mFilter.ExternalQueryInterface (iid, (void**)&lInterface);
	}
	return lInterface;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IPin * CDirectShowPin::SafeGetConnection () const
{
	IPin *	lRet = NULL;

	if	(this)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		lRet = mConnection;
	}
	return lRet;
}

IMemAllocator * CDirectShowPin::SafeGetAllocator () const
{
	IMemAllocator *	lRet = NULL;

	if	(this)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		lRet = mAllocator;
	}
	return lRet;
}

bool CDirectShowPin::SafeIsFlushing () const
{
	bool	lRet = false;

	if	(this)
	{
		CSingleLock	lLock (&mDataLock, TRUE);
		lRet = mIsFlushing;
	}
	return lRet;
}

bool CDirectShowPin::SafeIsEndOfStream () const
{
	bool	lRet = false;

	if	(this)
	{
		CSingleLock	lLock (&mDataLock, TRUE);
		lRet = mIsEndOfStream;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPin::CanConnect (IPin *pReceivePin)
{
	HRESULT			lResult = S_OK;
	PIN_DIRECTION	lPinDirection;

	if	(!pReceivePin)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(SUCCEEDED (lResult = pReceivePin->QueryDirection (&lPinDirection)))
		&&	(
				(
					(lPinDirection == PINDIR_INPUT)
				&&	(mDirection != PINDIR_OUTPUT)
				)
			||	(
					(lPinDirection == PINDIR_OUTPUT)
				&&	(mDirection != PINDIR_INPUT)
				)
			)
		)
	{
		lResult = VFW_E_INVALID_DIRECTION;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			if	(mConnection != NULL)
			{
				lResult = VFW_E_ALREADY_CONNECTED;
			}
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPin::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = VFW_E_NO_ACCEPTABLE_TYPES;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(pMediaType)
		{
#ifdef	_DEBUG_CONNECTION
			LogMediaType (_DEBUG_CONNECTION, *pMediaType, _T("[%s] [%p] ProposeConnection to [%p] [%s] from [%s]"), mName, this, pReceivePin, PinIdStr(pReceivePin), PinIdStr(GetInterface()));
#endif
			if	(
					(SUCCEEDED (pReceivePin->ReceiveConnection (GetInterface(), pMediaType)))
				&&	(SUCCEEDED (lResult = MoDuplicateMediaType (mMediaType.Free(), pMediaType)))
				)
			{
				mConnection = pReceivePin;
				mFilter.OnPinConnected (this);
				lResult = S_OK;
#ifdef	_DEBUG_CONNECTION
				LogMediaType (_DEBUG_CONNECTION, *mMediaType, _T("[%s] [%p] Connected to [%p] [%s] from [%s]"), mName, this, pReceivePin, PinIdStr(pReceivePin), PinIdStr(GetInterface()));
#endif
			}
		}
		else
		{
			int						lMediaTypeNdx;
			const AM_MEDIA_TYPE *	lMediaType;

			for	(lMediaTypeNdx = 0; lMediaTypeNdx <= mMediaTypes.GetUpperBound(); lMediaTypeNdx++)
			{
				if	(
						(lMediaType = mMediaTypes [lMediaTypeNdx])
					&&	(Connect (pReceivePin, lMediaType) == S_OK)
					)
				{
					lResult = S_OK;
					break;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPin::ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
#ifdef	_DEBUG_CONNECTION
		LogMediaType (_DEBUG_CONNECTION, *pMediaType, _T("[%s] [%p] ReceiveConnection from [%p] [%s] to [%s]"), mName, this, pConnector, PinIdStr(pConnector), PinIdStr(GetInterface()));
#endif
		if	(SUCCEEDED (lResult = MoDuplicateMediaType (mMediaType.Free(), pMediaType)))
		{
			mConnection = pConnector;
			mFilter.OnPinConnected (this);
			lResult = S_OK;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPin::Disconnect ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		SafeFreeSafePtr (mAllocator);
		if	(mConnection != NULL)
		{
			SafeFreeSafePtr (mConnection);
			MoDuplicateMediaType (mMediaType.Free(), &AM_MEDIA_TYPE_Safe());
			mFilter.OnPinDisconnected (this);
			lResult = S_OK;
		}
		mIsEndOfStream = false;
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPin::BeginOutput ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = false;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_OUTPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::EndOutput ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = false;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_OUTPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::GetOutputSample (IMediaSample ** pSample, const REFERENCE_TIME * pStartTime, const REFERENCE_TIME * pEndTime, DWORD pFlags)
{
	return E_NOTIMPL;
}

HRESULT CDirectShowPin::PutOutputSample (IMediaSample * pSample, ULONG pSampleSize)
{
	return E_NOTIMPL;
}

HRESULT CDirectShowPin::BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return S_FALSE;
}

HRESULT CDirectShowPin::EndOutputStream ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsEndOfStream = true;
	}
	catch AnyExceptionSilent

	return S_FALSE;
}

HRESULT CDirectShowPin::BeginOutputFlush ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = true;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return S_FALSE;
}

HRESULT CDirectShowPin::EndOutputFlush ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = false;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPin::BeginInput ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = false;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_INPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::EndInput ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = false;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_INPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::PeekInputSample (IMediaSample ** pSample, DWORD pLockTime)
{
	(*pSample) = NULL;
	return (mDirection == PINDIR_INPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::GetInputSample (IMediaSample ** pSample, DWORD pLockTime)
{
	(*pSample) = NULL;
	return (mDirection == PINDIR_INPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::BeginInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_INPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::EndInputStream ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsEndOfStream = true;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_INPUT) ? S_FALSE : E_UNEXPECTED;
}

HRESULT CDirectShowPin::BeginInputFlush ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = true;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_INPUT) ? E_NOTIMPL : E_UNEXPECTED;
}

HRESULT CDirectShowPin::EndInputFlush ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = false;
		mIsEndOfStream = false;
	}
	catch AnyExceptionSilent

	return (mDirection == PINDIR_INPUT) ? E_NOTIMPL : E_UNEXPECTED;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::XPin::Connect [%p] [%p]"), pThis, pThis->m_dwRef, pThis->mName, pReceivePin, pmt);
#endif
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&pThis->mStateLock, TRUE);

	try
	{
		lResult = pThis->CanConnect (pReceivePin);
		if	(
				(SUCCEEDED (lResult))
			&&	(pmt)
			)
		{
			lResult = QueryAccept (pmt);
		}
		if	(lResult == S_OK)
		{
			lResult = pThis->Connect (pReceivePin, pmt);
			if	(FAILED (lResult))
			{
				pThis->Disconnect ();
			}
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::XPin::Connect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::XPin::ReceiveConnection"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!pConnector)
		||	(!pmt)
		)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
//
//	Removed to allow for Pull pin connection
//
//			if	(pThis->mDirection != PINDIR_INPUT)
//			{
//				lResult = E_UNEXPECTED;
//			}
			if	(SUCCEEDED (lResult))
			{
				lResult = pThis->CanConnect (pConnector);
			}
			if	(SUCCEEDED (lResult))
			{
				lResult = QueryAccept (pmt);

				if	(lResult == S_OK)
				{
					lResult = pThis->ReceiveConnection (pConnector, pmt);
					if	(FAILED (lResult))
					{
						pThis->Disconnect ();
					}
				}
				else
				{
					lResult = VFW_E_TYPE_NOT_ACCEPTED;
				}
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::XPin::ReceiveConnection"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::Disconnect (void)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::XPin::Disconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = pThis->Disconnect ();

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::XPin::Disconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::ConnectedTo (IPin **pPin)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XPin::ConnectedTo"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pPin)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			if	(*pPin = pThis->mConnection)
			{
				(*pPin)->AddRef ();
			}
			else
			{
				lResult = VFW_E_NOT_CONNECTED;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::XPin::ConnectedTo"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::ConnectionMediaType (AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XPin::ConnectionMediaType"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pmt)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			if	(
					(pThis->mConnection != NULL)
				&&	(pThis->mMediaType.Ptr())
				)
			{
				lResult = MoCopyMediaType ((DMO_MEDIA_TYPE*)pmt, (DMO_MEDIA_TYPE*)pThis->mMediaType);
			}
			else
			{
				lResult = VFW_E_NOT_CONNECTED;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::XPin::ConnectionMediaType"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::QueryPinInfo (PIN_INFO *pInfo)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XPin::QueryPinInfo"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pInfo)
	{
		lResult = E_POINTER;
	}
	else
	{
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
		CSingleLock	lLock (&pThis->mStateLock, TRUE);
#endif
		try
		{
			IBaseFilterPtr	lBaseFilter (pThis->mFilter.GetFilter ());

			pInfo->pFilter = lBaseFilter.Detach ();
			pInfo->dir = pThis->mDirection;
			wcsncpy (pInfo->achName, pThis->mName, sizeof(pInfo->achName)/sizeof(WCHAR));
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::XPin::QueryPinInfo"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::QueryDirection (PIN_DIRECTION *pPinDir)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XPin::QueryDirection"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pPinDir)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pPinDir) = pThis->mDirection;
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::XPin::QueryDirection"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::QueryId (LPWSTR *Id)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XPin::QueryId"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!Id)
	{
		lResult = E_POINTER;
	}
	else
	{
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
		CSingleLock	lLock (&pThis->mStateLock, TRUE);
#endif
		try
		{
			(*Id) = AfxAllocTaskWideString (pThis->mUniqueId);
			if	(*Id == NULL)
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::XPin::QueryId"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::QueryAccept (const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::XPin::QueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pmt)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			int				lMediaTypeNdx;
			AM_MEDIA_TYPE *	lMediaType;

#ifdef	_DEBUG_CONNECTION
			LogMediaType (_DEBUG_CONNECTION, *pmt, _T("[%s] [%p] QueryAccept to [%s]"), pThis->mName, pThis, PinIdStr(this));
#endif
			for	(lMediaTypeNdx = 0; lMediaTypeNdx <= pThis->mMediaTypes.GetUpperBound (); lMediaTypeNdx++)
			{
				if	(
						(lMediaType = pThis->mMediaTypes [lMediaTypeNdx])
					&&	(IsEqualGUID (lMediaType->majortype, pmt->majortype))
					&&	(
							(IsEqualGUID (lMediaType->subtype, pmt->subtype))
						||	(IsEqualGUID (lMediaType->subtype, GUID_NULL))
						)
					&&	(
							(IsEqualGUID (lMediaType->formattype, pmt->formattype))
						||	(IsEqualGUID (lMediaType->formattype, FORMAT_None))
						)
					)
				{
#ifdef	_DEBUG_CONNECTION
					LogMediaType (_DEBUG_CONNECTION, *lMediaType, _T("[%s] [%p] CanAccept to [%s]"), pThis->mName, pThis, PinIdStr(this));
#endif
					lResult = S_OK;
				}
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::XPin::QueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::EnumMediaTypes (IEnumMediaTypes **ppEnum)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XPin::EnumMediaTypes"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			CEnumMediaTypes *	lEnum;
			IEnumMediaTypesPtr	lInterface;

			if	(lEnum = new CEnumMediaTypes (pThis->mMediaTypes, pThis->GetControllingUnknown()))
			{
				lInterface = lEnum->GetControllingUnknown ();
				lEnum->ExternalRelease ();
				(*ppEnum) = lInterface.Detach ();
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::XPin::EnumMediaTypes"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::QueryInternalConnections (IPin **apPin, ULONG *nPin)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_CONNECTION_NOT
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::XPin::QueryInternalConnections"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = E_NOTIMPL;

#ifdef	_DEBUG_CONNECTION_NOT
	LogVfwErrAnon (_DEBUG_CONNECTION_NOT, lResult, _T("[%p(%d)] %s::XPin::QueryInternalConnections"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::NewSegment (REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::XPin::NewSegment [%f - %f] [%f]"), pThis, pThis->m_dwRef, pThis->mName, RefTimeSec(tStart), RefTimeSec(tStop), dRate);
#endif
	HRESULT	lResult = S_FALSE;

	try
	{
		if	(pThis->mDirection == PINDIR_INPUT)
		{
			lResult = pThis->BeginInputStream (tStart, tStop, dRate);
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::XPin::NewSegment"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::EndOfStream (void)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::XPin::EndOfStream"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = E_UNEXPECTED;

	try
	{
		if	(pThis->mDirection == PINDIR_INPUT)
		{
			lResult = pThis->EndInputStream ();
		}
		else
		if	(pThis->mDirection == PINDIR_OUTPUT)
		{
			IPinPtr	lConnection = pThis->SafeGetConnection ();

			if	(lConnection != NULL)
			{
				try
				{
					LogVfwErr (LogVerbose, lConnection->EndOfStream ());
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::XPin::EndOfStream"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::BeginFlush (void)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::XPin::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = pThis->BeginInputFlush ();

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::XPin::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::XPin::EndFlush (void)
{
	METHOD_PROLOGUE(CDirectShowPin, Pin)
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::XPin::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = pThis->EndInputFlush ();

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::XPin::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowPinIn, CDirectShowPin)

CDirectShowPinIn::CDirectShowPinIn (CDirectShowFilter & pFilter, LPCTSTR pName, LPCTSTR pUniqueId, UINT pDesiredSampleCount, UINT pMaxSampleCount)
:	CDirectShowPin (pFilter, PINDIR_INPUT, pName, pUniqueId),
	mDesiredSampleCount (max (pDesiredSampleCount, 1)),
	mMaxSampleCount (max (pDesiredSampleCount, pMaxSampleCount)),
	mReadOnlySamples (false),
	mEosNotifyEvent (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPinIn::CDirectShowPinIn (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinIn::~CDirectShowPinIn ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPinOut::~CDirectShowPinOut (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	mSamples.RemoveAll ();
	SafeFreeSafePtr (mCacheAllocator);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinIn::ProvideAllocator ()
{
	HRESULT	lResult = S_OK;

	if	(mAllocator == NULL)
	{
		if	(
				(mMediaType.Ptr ())
			&&	(mMediaType->lSampleSize > 0)
			)
		{
			tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
			tS <ALLOCATOR_PROPERTIES>	lActualMem;

			mReadOnlySamples = false;

			lRequiredMem.cBuffers = max (mMaxSampleCount, max (mDesiredSampleCount, 1));
			lRequiredMem.cbBuffer = mMediaType->lSampleSize;
			lRequiredMem.cbAlign = 8;

			if	(
					(mFilter.mInputPins.GetSize() == 1)
				&&	(mFilter.mOutputPins.GetSize() == 1)
				)
			{
				IMemInputPinPtr		lDownstreamTransport = mFilter.mOutputPins[0]->SafeGetConnection();
				IMemAllocatorPtr	lDownstreamAllocator = mFilter.mOutputPins[0]->SafeGetAllocator ();

				if	(
						(lDownstreamAllocator != NULL)
					&&	(SUCCEEDED (lDownstreamAllocator->GetProperties (lActualMem.Clear())))
					&&	(lActualMem.cBuffers >= lRequiredMem.cBuffers)
					&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
					)
				{
#ifdef	_DEBUG_ALLOCATOR
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Downstream [%s] Actual "), PinIdStr(mFilter.mOutputPins[0]->SafeGetConnection()));
#endif
					mAllocator = lDownstreamAllocator;
				}
				else
				if	(
						(lDownstreamTransport != NULL)
					&&	(SUCCEEDED (lDownstreamTransport->GetAllocatorRequirements (lActualMem.Clear())))
					)
				{
#ifdef	_DEBUG_ALLOCATOR
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Downstream [%s] Requirements "), PinIdStr(mFilter.mOutputPins[0]->SafeGetConnection()));
#endif
					lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, lActualMem.cBuffers);
					lRequiredMem.cbBuffer = max (lRequiredMem.cbBuffer, lActualMem.cbBuffer);
					lRequiredMem.cbAlign = max (lRequiredMem.cbAlign, lActualMem.cbAlign);
				}
			}

			if	(mAllocator == NULL)
			{
#ifdef	_DEBUG_ALLOCATOR
				LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Request "), mName, this);
#endif
				if	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&mAllocator))))
				{
					lResult = LogVfwErr (LogNormal, mAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()));
				}
			}
#ifdef	_DEBUG_ALLOCATOR
			LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("[%s] [%p]   Actual  "), mName, this);
#endif
		}
		else
		{
			lResult = VFW_E_NO_TYPES;
		}
	}
	return lResult;
}

HRESULT CDirectShowPinIn::ReceiveAllocator (IMemAllocator * pAllocator, bool pReadOnly)
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
		tS <ALLOCATOR_PROPERTIES>	lActualMem;

		mReadOnlySamples = pReadOnly;
		mAllocator = pAllocator;
		SafeFreeSafePtr (mCacheAllocator);

#ifdef	_DEBUG_ALLOCATOR
		LogMemAllocator (_DEBUG_ALLOCATOR, mAllocator, _T("[%s] [%p]   Allocator"), mName, this);
#endif
		if	(
				(mDesiredSampleCount > 1)
			&&	(SUCCEEDED (mAllocator->GetProperties (&lActualMem)))
			&&	(lActualMem.cBuffers < (long)mDesiredSampleCount)
			)
		{
			lRequiredMem = lActualMem;
			lRequiredMem.cBuffers = mDesiredSampleCount;

			if	(
					(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&mCacheAllocator))))
				&&	(SUCCEEDED (LogVfwErr (LogNormal, mCacheAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()))))
				)
			{
#ifdef	_DEBUG_ALLOCATOR
				LogMemAllocator (_DEBUG_ALLOCATOR, mCacheAllocator, _T("[%s] [%p]   CacheAllocator"), mName, this);
#endif
			}
			else
			{
				SafeFreeSafePtr (mCacheAllocator);
			}
		}
	}
	catch AnyExceptionDebug

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinIn::ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(SUCCEEDED (lResult = CDirectShowPin::ReceiveConnection (pConnector, pMediaType)))
		{
			lResult = ProvideAllocator ();
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinIn::Disconnect ()
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		mIsFlushing = true;
		EmptyCache ();
		SafeFreeSafePtr (mCacheAllocator);
		mReadOnlySamples = false;
		mIsFlushing = false;
	}
	catch AnyExceptionDebug

	return CDirectShowPin::Disconnect ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

INT_PTR CDirectShowPinIn::GetCachedSampleCount () const
{
	CSingleLock	lLock (&mDataLock, TRUE);
	return mSamples.GetSize();
}

HRESULT CDirectShowPinIn::PeekInputSample (IMediaSample ** pSample, DWORD pLockTime)
{
	HRESULT	lResult = VFW_E_TIMEOUT;

	if	(mDataLock.Lock (pLockTime))
	{
		try
		{
			(*pSample) = NULL;
			if	(mSamples.GetSize () > 0)
			{
				if	(*pSample = mSamples [0])
				{
					(*pSample)->AddRef ();
					lResult = S_OK;
				}
				else
				{
					lResult = E_FAIL;
				}
			}
			else
			if	(mIsEndOfStream)
			{
				lResult = VFW_S_NO_MORE_ITEMS;
			}
			else
			{
				lResult = S_FALSE;
			}
		}
		catch AnyExceptionDebug

		mDataLock.Unlock ();
	}
	return lResult;
}

HRESULT CDirectShowPinIn::GetInputSample (IMediaSample ** pSample, DWORD pLockTime)
{
	HRESULT	lResult = VFW_E_TIMEOUT;

	if	(mDataLock.Lock (pLockTime))
	{
		try
		{
			(*pSample) = NULL;
			if	(mSamples.GetSize () > 0)
			{
				*pSample = mSamples [0].Detach ();
				mSamples.RemoveAt (0);

				if	(mMaxSampleSemaphore)
				{
					mMaxSampleSemaphore->Unlock ();
				}
				lResult = S_OK;
			}
			else
			if	(mIsEndOfStream)
			{
				lResult = VFW_S_NO_MORE_ITEMS;
			}
			else
			{
				lResult = S_FALSE;
			}
		}
		catch AnyExceptionDebug

		mDataLock.Unlock ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinIn::PutInputSample (IMediaSample * pSample, DWORD pLockTime)
{
	HRESULT	lResult = VFW_E_TIMEOUT;

	if	(mDataLock.Lock (pLockTime))
	{
		try
		{
			if	(
					(mMaxSampleCount > 0)
				&&	(!mMaxSampleSemaphore)
				)
			{
				mMaxSampleSemaphore = new CSemaphore ((long)mMaxSampleCount, (long)mMaxSampleCount);
			}

			if	(
					(mIsFlushing)
				||	(mIsEndOfStream)
				)
			{
#ifdef	_DEBUG_STREAM
				LogMediaSampleId (_DEBUG_STREAM, pSample, _T("[%s] [%u] Sample rejected Flushing [%d] EndOfStream [%d]"), mName, m_dwRef, mIsFlushing, mIsEndOfStream);
#endif
				lResult = S_FALSE;
			}
			else
			if	(mFilter.GetState() == State_Stopped)
			{
#ifdef	_DEBUG_STREAM
				LogMediaSampleId (_DEBUG_STREAM, pSample, _T("[%s] [%u] Sample rejected for stopped filter"), mName, m_dwRef);
#endif
				lResult = VFW_E_WRONG_STATE;
			}
			else
			{
				DWORD			lWait = WAIT_OBJECT_0;
				HANDLE			lSemaphore;
				IMediaSamplePtr	lCacheSample;

				if	(
						(mMaxSampleSemaphore)
					&&	(lSemaphore = (*mMaxSampleSemaphore))
					)
				{
					mDataLock.Unlock ();
					lWait = WaitForSingleObject (lSemaphore, pLockTime);
					mDataLock.Lock ();
				}

				if	(lWait == WAIT_OBJECT_0)
				{
#ifdef	_DEBUG_STREAM_DATA
					LogMediaSampleId (_DEBUG_STREAM_DATA, pSample, _T("[%s] [%u] Sample [%d Max %d]"), mName, m_dwRef, mSamples.GetSize(), mMaxSampleCount);
#endif
					if	(
							(mCacheAllocator != NULL)
						&&	(SUCCEEDED (CDirectShowUtils::DuplicateSample (pSample, mCacheAllocator, &lCacheSample)))
						)
					{
						mSamples.Add (lCacheSample);
					}
					else
					{
						mSamples.Add (pSample);
					}
					mFilter.OnGotInputSample (this);
					lResult = S_OK;
				}
				else
				{
#ifdef	_DEBUG_STREAM
					LogMediaSampleId (_DEBUG_STREAM, pSample, _T("[%s] [%u] Sample rejected MaxSamples [%d] Flushing [%d] EndOfStream [%d]"), mName, m_dwRef, mMaxSampleCount);
#endif
				}
			}
		}
		catch AnyExceptionDebug

		mDataLock.Unlock ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinIn::BeginInput ()
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsFlushing = true;
		EmptyCache ();
		if	(mCacheAllocator != NULL)
		{
			LogVfwErr (LogNormal, mCacheAllocator->Commit ());
		}
		mIsEndOfStream = false;
		mIsFlushing = false;
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinIn::EndInput ()
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mIsEndOfStream = false;
		mIsFlushing = false;
		EmptyCache ();
		if	(mCacheAllocator != NULL)
		{
			LogVfwErr (LogNormal, mCacheAllocator->Decommit ());
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinIn::BeginInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%f] BeginInputStream [%f - %f] of [%f]"), mName, RefTimeSec(mFilter.GetStreamTime(mFilter.GetState())), RefTimeSec(pStartTime), RefTimeSec(pEndTime), RefTimeSec(mFilter.GetDuration()));
#endif
		mIsEndOfStream = false;
		mFilter.OnStartInputStream (pStartTime, pEndTime, pRate);
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinIn::EndInputStream ()
{
	HRESULT		lResult = S_OK;
	INT_PTR		lSampleCount;

	{
		CSingleLock	lLock (&mDataLock, TRUE);

		try
		{
#ifdef	_DEBUG_STATE
			LogMessage (_DEBUG_STATE, _T("[%s] [%f] EndInputStream (Queued {%d])"), mName, RefTimeSec(mFilter.GetStreamTime(mFilter.GetState())), mSamples.GetSize ());
#endif
			mIsEndOfStream = true;
			lSampleCount = mSamples.GetSize();
		}
		catch AnyExceptionDebug
	}

	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			if	(
					(lSampleCount <= 0)
				&&	(mEosNotifyEvent)
				)
			{
#ifdef	_DEBUG_DYNCONNECTION
				LogMessage (_DEBUG_DYNCONNECTION, _T("[%s] [%f] NotifyEos [%p]"), mName, RefTimeSec(mFilter.GetStreamTime(mFilter.GetState())), mEosNotifyEvent);
#endif
				::SetEvent (mEosNotifyEvent);
			}
		}
		catch AnyExceptionDebug
	}

	mFilter.OnEndInputStream (lSampleCount);

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinIn::BeginInputFlush ()
{
	HRESULT				lResult = S_OK;
	IMemAllocatorPtr	lAllocator = SafeGetAllocator ();
	CSingleLock			lLock (&mDataLock, TRUE);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%f] BeginInputFlush"), mName, RefTimeSec(mFilter.GetStreamTime(mFilter.GetState())));
#endif
		mIsFlushing = true;
		mFilter.BeginFlush ();
		EmptyCache ();
		if	(lAllocator != NULL)
		{
			LogVfwErr (LogNormal, lAllocator->Decommit ());
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinIn::EndInputFlush ()
{
	HRESULT				lResult = S_OK;
	IMemAllocatorPtr	lAllocator = SafeGetAllocator ();
	CSingleLock			lLock (&mDataLock, TRUE);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%f] EndInputFlush"), mName, RefTimeSec(mFilter.GetStreamTime(mFilter.GetState())));
#endif
		mIsEndOfStream = false;
		if	(lAllocator != NULL)
		{
			LogVfwErr (LogNormal, lAllocator->Commit ());
		}
		mFilter.EndFlush ();
		mIsFlushing = false;
	}
	catch AnyExceptionDebug
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowPinIn::EmptyCache ()
{
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
#ifdef	_DEBUG_STREAM_DATA
		if	(mSamples.GetSize() > 0)
		{
			LogMessage (_DEBUG_STREAM_DATA, _T("[%s] [%u] Discard [%d] samples Flushing [%d] EndOfStream [%d]"), mName, m_dwRef, mSamples.GetSize(), mIsFlushing, mIsEndOfStream);
		}
#endif
		mSamples.RemoveAll ();
		if	(mMaxSampleSemaphore)
		{
			mMaxSampleSemaphore->Unlock (mMaxSampleCount);
		}
		SafeFreeSafePtr (mMaxSampleSemaphore);
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XMemInput::GetAllocator (IMemAllocator **ppAllocator)
{
	METHOD_PROLOGUE(CDirectShowPinIn, MemInput)
#ifdef	_DEBUG_ALLOCATOR
	LogMessage (_DEBUG_ALLOCATOR, _T("[%p(%d)] %s::XMemInput::GetAllocator [%p]"), pThis, pThis->m_dwRef, pThis->mName, ppAllocator);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppAllocator)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			lResult = pThis->ProvideAllocator ();
			if	(
					(SUCCEEDED (lResult))
				&&	(*ppAllocator = pThis->mAllocator)
				)
			{
				(*ppAllocator)->AddRef ();
			}
			else
			if	(SUCCEEDED (lResult))
			{
				lResult = VFW_E_NO_ALLOCATOR;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon ((lResult==VFW_E_NO_TYPES)?MaxLogLevel(_LOG_RESULTS,LogVerbose):_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMemInput::GetAllocator"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XMemInput::NotifyAllocator (IMemAllocator *pAllocator, BOOL bReadOnly)
{
	METHOD_PROLOGUE(CDirectShowPinIn, MemInput)
#ifdef	_DEBUG_ALLOCATOR
	LogMessage (_DEBUG_ALLOCATOR, _T("[%p(%d)] %s::XMemInput::NotifyAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pAllocator)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = pThis->ReceiveAllocator (pAllocator, (bReadOnly!=FALSE));
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMemInput::NotifyAllocator"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XMemInput::GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps)
{
	METHOD_PROLOGUE(CDirectShowPinIn, MemInput)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XMemInput::GetAllocatorRequirements"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pProps)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			memset (pProps, 0, sizeof(*pProps));
			if	(pThis->mAllocator != NULL)
			{
				lResult = pThis->mAllocator->GetProperties (pProps);
			}
			if	(pProps->cbAlign <= 1)
			{
				pProps->cbAlign = 8;
			}
#ifdef	_DEBUG_ALLOCATOR
			LogAllocatorProps (_DEBUG_ALLOCATOR, *pProps, _T("[%s] [%p]   Requirements"), pThis->mName, pThis);
#endif
		}
		catch AnyExceptionDebug
	}

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XMemInput::Receive (IMediaSample *pSample)
{
	METHOD_PROLOGUE(CDirectShowPinIn, MemInput)
#ifdef	_DEBUG_STREAM_EX
	LogMessage (_DEBUG_STREAM_EX, _T("[%p(%d)] %s::XMemInput::Receive"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pSample)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = pThis->PutInputSample (pSample, 500);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon ((lResult==VFW_E_WRONG_STATE)?MaxLogLevel(_LOG_RESULTS,LogVerbose):_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMemInput::Receive [%s]"), pThis, pThis->m_dwRef, pThis->mName, FilterStateStr(pThis->mFilter.GetState()));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XMemInput::ReceiveMultiple (IMediaSample **pSamples, long nSamples, long *nSamplesProcessed)
{
	METHOD_PROLOGUE(CDirectShowPinIn, MemInput)
#ifdef	_DEBUG_STREAM_EX
	LogMessage (_DEBUG_STREAM_EX, _T("[%p(%d)] %s::XMemInput::ReceiveMultiple [%d]"), pThis, pThis->m_dwRef, pThis->mName, nSamples);
#endif
	HRESULT	lResult = S_OK;

	if	(nSamplesProcessed)
	{
		(*nSamplesProcessed) = 0;
	}

	if	(nSamples <= 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!pSamples)
	{
		lResult = E_POINTER;
	}
	else
	{
		long	lSampleNdx;

		for	(lSampleNdx = 0; lSampleNdx < nSamples; lSampleNdx++)
		{
			if	(SUCCEEDED (lResult = pThis->PutInputSample (pSamples [lSampleNdx], 500)))
			{
				if	(nSamplesProcessed)
				{
					(*nSamplesProcessed)++;
				}
			}
			else
			{
				break;
			}
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMemInput::ReceiveMultiple"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XMemInput::ReceiveCanBlock (void)
{
	METHOD_PROLOGUE(CDirectShowPinIn, MemInput)
#ifdef	_DEBUG_ALLOCATOR
	LogMessage (_DEBUG_ALLOCATOR, _T("[%p(%d)] %s::XPin::ReceiveCanBlock"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XPinConnection::DynamicQueryAccept (const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowPinIn, PinConnection)
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::XPinConnection::DynamicQueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return pThis->m_xPin.QueryAccept (pmt);
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XPinConnection::NotifyEndOfStream (HANDLE hNotifyEvent)
{
	METHOD_PROLOGUE(CDirectShowPinIn, PinConnection)
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::XPinConnection::NotifyEndOfStream [%p]"), pThis, pThis->m_dwRef, pThis->mName, hNotifyEvent);
#endif
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&pThis->mStateLock);

	try
	{
		if	(hNotifyEvent)
		{
			pThis->mEosNotifyEvent = hNotifyEvent;
			lResult = S_OK;
		}
		else
		{
			if	(pThis->mEosNotifyEvent)
			{
				lResult = S_OK;
			}
			pThis->mEosNotifyEvent = NULL;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XPinConnection::IsEndPin (void)
{
	METHOD_PROLOGUE(CDirectShowPinIn, PinConnection)
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::XPinConnection::IsEndPin"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return (pThis->mFilter.mOutputPins.GetSize() <= 0) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::XPinConnection::DynamicDisconnect (void)
{
	METHOD_PROLOGUE(CDirectShowPinIn, PinConnection)
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::XPinConnection::DynamicDisconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_FALSE;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowPinOut, CDirectShowPin)

CDirectShowPinOut::CDirectShowPinOut (CDirectShowFilter & pFilter, LPCTSTR pName, LPCTSTR pUniqueId, UINT pDesiredSampleCount)
:	CDirectShowPin (pFilter, PINDIR_OUTPUT, pName, pUniqueId),
	mDesiredSampleCount (pDesiredSampleCount),
	mSampleSize (0),
	mReadOnlySamples (true)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPinOut::CDirectShowPinOut (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinOut::~CDirectShowPinOut ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPinOut::~CDirectShowPinOut (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinOut::CanConnect (IPin *pReceivePin)
{
	HRESULT	lResult = CDirectShowPin::CanConnect (pReceivePin);

	if	(SUCCEEDED (lResult))
	{
		lResult = VFW_E_NO_ALLOCATOR;

		try
		{
			IMemInputPinPtr		lTransport (pReceivePin);
			IMemAllocatorPtr	lAllocator;

			if	(lTransport == NULL)
			{
				lResult = VFW_E_NO_TRANSPORT;
			}
			else
			{
				if	(
						(SUCCEEDED (lTransport->GetAllocator (&lAllocator)))
					&&	(lAllocator != NULL)
					)
				{
					lResult = S_OK;
				}
			}
#ifdef	_DEBUG_ALLOCATOR_NOT
			if	(FAILED (lResult))
			{
				LogVfwErrAnon (LogNormal, lResult, PinIdStr (pReceivePin));
			}
#endif
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowPinOut::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		lResult = CDirectShowPin::Connect (pReceivePin, pMediaType);

		if	(
				(SUCCEEDED (lResult))
			&&	(pMediaType)
			)
		{
			lResult = NegotiateAllocator (pMediaType);
		}
 	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinOut::Disconnect ()
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(mAllocator != NULL)
		{
			LogVfwErr (LogVerbose, mAllocator->Decommit ());
		}
	}
	catch AnyExceptionDebug

	return CDirectShowPin::Disconnect ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinOut::NegotiateAllocator (const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = VFW_E_NOT_SAMPLE_CONNECTION;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		IMemInputPinPtr				lTransport (mConnection);
		IMemAllocatorPtr			lAllocator;
		tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
		tS <ALLOCATOR_PROPERTIES>	lActualMem;

#ifdef	_DEBUG_ALLOCATOR
		LogMessage (_DEBUG_ALLOCATOR, _T("[%s] [%p] NegotiateAllocator"), mName, this);
#endif
		if	(lTransport!= NULL)
		{
#ifdef	_DEBUG_ALLOCATOR
			LogMemAllocator (_DEBUG_ALLOCATOR, lTransport, _T("[%s] [%p]   Current "), mName, this);
#endif
			if	(SUCCEEDED (lTransport->GetAllocatorRequirements (lRequiredMem.Clear())))
			{
#ifdef	_DEBUG_ALLOCATOR
				LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Required"), mName, this);
#endif
			}

			if	(
					(mSampleSize == 0)
				&&	(pMediaType)
				)
			{
				mSampleSize = pMediaType->lSampleSize;
			}
			lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, max (mDesiredSampleCount, 1));
			lRequiredMem.cbBuffer = mSampleSize;
			if	(lRequiredMem.cbAlign <= 1)
			{
				lRequiredMem.cbAlign = 8;
			}

			if	(
					(mFilter.mInputPins.GetSize() == 1)
				&&	(mFilter.mOutputPins.GetSize() == 1)
				)
			{
				IMemInputPinPtr		lUpstreamTransport = mFilter.mInputPins[0]->GetInterface();
				IMemAllocatorPtr	lUpstreamAllocator = mFilter.mInputPins[0]->SafeGetAllocator ();

				if	(
						(lUpstreamAllocator != NULL)
					&&	(SUCCEEDED (lUpstreamAllocator->GetProperties (lActualMem.Clear())))
					&&	(lActualMem.cBuffers >= lRequiredMem.cBuffers)
					&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
					)
				{
#ifdef	_DEBUG_ALLOCATOR
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Upstream [%s] Actual "), PinIdStr(mFilter.mInputPins[0]->GetInterface()));
#endif
					lAllocator = lUpstreamAllocator;
				}
				else
				if	(
						(lUpstreamTransport != NULL)
					&&	(SUCCEEDED (lUpstreamTransport->GetAllocatorRequirements (lActualMem.Clear())))
					)
				{
#ifdef	_DEBUG_ALLOCATOR
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Upstream [%s] Requirements "), PinIdStr(mFilter.mInputPins[0]->GetInterface()));
#endif
					lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, lActualMem.cBuffers);
					lRequiredMem.cbBuffer = max (lRequiredMem.cbBuffer, lActualMem.cbBuffer);
					lRequiredMem.cbAlign = max (lRequiredMem.cbAlign, lActualMem.cbAlign);
				}
			}

			if	(lAllocator == NULL)
			{
#ifdef	_DEBUG_ALLOCATOR
				LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Request "), mName, this);
#endif
			}

			if	(
					(
						(lAllocator != NULL)
					||	(SUCCEEDED (LogVfwErr (LogNormal, lResult = lTransport->GetAllocator (&lAllocator))))
					)
				&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = lAllocator->GetProperties (lActualMem.Clear()))))
				&&	(
						(
							(lActualMem.cBuffers >= lRequiredMem.cBuffers)
						&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
						)
					||	(
							(SUCCEEDED (LogVfwErr (LogNormal, lResult = lAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()))))
						&&	(lActualMem.cBuffers >= lRequiredMem.cBuffers)
						&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
						)
					||	(
							(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&lAllocator))))
						&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = lAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()))))
						&&	(lActualMem.cBuffers >= lRequiredMem.cBuffers)
						&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
						)
					)
				&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = lTransport->NotifyAllocator (lAllocator, (mReadOnlySamples!=false)))))
				)
			{
				mAllocator = lAllocator;
#ifdef	_DEBUG_ALLOCATOR
				LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("[%s] [%p]   Actual  "), mName, this);
				LogMemAllocator (_DEBUG_ALLOCATOR, lAllocator, _T("[%s] [%p]   Actual  "), mName, this);
#endif
			}
			else
			if	(SUCCEEDED (lResult))
			{
				lResult = VFW_E_NO_ALLOCATOR;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinOut::BeginOutput ()
{
	HRESULT		lResult = VFW_E_NO_ALLOCATOR;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
#ifdef	_DEBUG_ALLOCATOR
		LogMessage (_DEBUG_ALLOCATOR, _T("[%s] [%p] BeginOutput"), mName, this);
#endif
		if	(mAllocator != NULL)
		{
			lResult = LogVfwErr (LogNormal, mAllocator->Commit ());
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinOut::EndOutput ()
{
	HRESULT		lResult = VFW_E_NOT_SAMPLE_CONNECTION;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
#ifdef	_DEBUG_ALLOCATOR
		LogMessage (_DEBUG_ALLOCATOR, _T("[%s] [%p] EndOutput"), mName, this);
#endif
		if	(mAllocator != NULL)
		{
			LogVfwErr (LogNormal, lResult = mAllocator->Decommit ());
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinOut::GetOutputSample (IMediaSample ** pSample, const REFERENCE_TIME * pStartTime, const REFERENCE_TIME * pEndTime, DWORD pFlags)
{
	HRESULT				lResult = VFW_E_NO_ALLOCATOR;
	IMemAllocatorPtr	lAllocator = SafeGetAllocator ();

	try
	{
		pFlags &= AM_GBF_NOWAIT;
		if	(lAllocator != NULL)
		{
			LogVfwErr (LogVerbose, lResult = lAllocator->GetBuffer (pSample, const_cast <REFERENCE_TIME *> (pStartTime), const_cast <REFERENCE_TIME *> (pEndTime), pFlags));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinOut::PutOutputSample (IMediaSample * pSample, ULONG pSampleSize)
{
	HRESULT				lResult = VFW_E_NO_ALLOCATOR;
	IMemInputPinPtr		lTransport = SafeGetConnection ();
	IMemAllocatorPtr	lAllocator = SafeGetAllocator ();

	try
	{
		if	(
				(pSample != NULL)
			&&	(lTransport != NULL)
			&&	(lAllocator != NULL)
			)
		{
			ULONG	lSampleSize = pSampleSize;

			if	(lSampleSize == 0)
			{
				lSampleSize = pSample->GetSize ();
			}
			if	(lSampleSize != 0)
			{
				LogVfwErr (LogNormal, pSample->SetActualDataLength (pSampleSize), _T("SetActualDataLength [%u]"), pSampleSize);
			}
#ifdef	_DEBUG_STREAM_DATA
			LogMediaSampleId (_DEBUG_STREAM_DATA, pSample, _T("[%s] [%u] Sample"), mName, m_dwRef);
#endif
			LogVfwErr (LogVerbose, lResult = lTransport->Receive (pSample));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinOut::BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] NewSegment [%f - %f] from [%s] to [%s]"), mName, m_dwRef, RefTimeSec(pStartTime), RefTimeSec(pEndTime), PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		if	(SUCCEEDED (LogVfwErr (LogNormal, lConnection->NewSegment (pStartTime, pEndTime, pRate))))
		{
			lResult = S_OK;
		}
	}
	return lResult;
}

HRESULT CDirectShowPinOut::EndOutputStream ()
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] EndOfStream from [%s] to [%s]"), mName, m_dwRef, PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		if	(SUCCEEDED (LogVfwErr (LogVerbose, lConnection->EndOfStream ())))
		{
			lResult = S_OK;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinOut::BeginOutputFlush ()
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] BeginFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		lResult = LogVfwErr (LogVerbose, lConnection->BeginFlush ());
	}
	return lResult;
}

HRESULT CDirectShowPinOut::EndOutputFlush ()
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] EndFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		lResult = LogVfwErr (LogVerbose, lConnection->EndFlush ());
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowPinPull, CDirectShowPin)

CDirectShowPinPull::CDirectShowPinPull (CDirectShowFilter & pFilter, LPCTSTR pName, LPCTSTR pUniqueId)
:	CDirectShowPin (pFilter, PINDIR_OUTPUT, pName, pUniqueId),
	mInputBuffer (NULL),
	mInputBufferSize (0),
	mBlockingEvent (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPinPull::CDirectShowPinPull (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinPull::~CDirectShowPinPull ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowPinPull::~CDirectShowPinPull (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::CanConnect (IPin *pReceivePin)
{
	HRESULT	lResult = CDirectShowPin::CanConnect (pReceivePin);

	if	(SUCCEEDED (lResult))
	{
		try
		{
			IMemInputPinPtr	lTransport (pReceivePin);

			if	(lTransport != NULL)
			{
				lResult = VFW_E_NO_TRANSPORT;
			}
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowPinPull::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		lResult = pReceivePin->Connect (&m_xPin, pMediaType);
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinPull::Disconnect ()
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		BlockStream (false);
		mRequests.RemoveAll ();
		mRequestEvent.SetEvent ();
	}
	catch AnyExceptionDebug

	return CDirectShowPin::Disconnect ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::UseAllocator (IMemAllocator * pAllocator)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
		tS <ALLOCATOR_PROPERTIES>	lActualMem;

		if	(
				(pAllocator)
			&&	(SUCCEEDED (pAllocator->GetProperties (lRequiredMem.Clear())))
			)
		{
#ifdef	_DEBUG_ALLOCATOR
			LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Request "), mName, this);
#endif
			lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, 1);
			lRequiredMem.cbAlign = max (lRequiredMem.cbAlign, 1);
			if	(mMediaType.Ptr())
			{
				lRequiredMem.cbBuffer = max (lRequiredMem.cbBuffer, mMediaType->lSampleSize);
			}

			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()))))
			{
				mAllocator = pAllocator;
#ifdef	_DEBUG_ALLOCATOR
				LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Request "), mName, this);
				LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("[%s] [%p]   Actual  "), mName, this);
#endif
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinPull::GetAllocator (const ALLOCATOR_PROPERTIES * pRequest)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&mAllocator))))
		{
			tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
			tS <ALLOCATOR_PROPERTIES>	lActualMem;

			if	(pRequest)
			{
				lRequiredMem = (*pRequest);
#ifdef	_DEBUG_ALLOCATOR
				LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Request "), mName, this);
#endif
			}

			lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, 1);
			lRequiredMem.cbAlign = max (lRequiredMem.cbAlign, 1);
			if	(mMediaType.Ptr())
			{
				lRequiredMem.cbBuffer = max (lRequiredMem.cbBuffer, mMediaType->lSampleSize);
			}
			lResult = LogVfwErr (LogNormal, mAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()));

#ifdef	_DEBUG_ALLOCATOR
			LogAllocatorProps (_DEBUG_ALLOCATOR, lRequiredMem, _T("[%s] [%p]   Request "), mName, this);
			LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("[%s] [%p]   Actual  "), mName, this);
#endif
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::SetInputBuffer (LPCVOID pBuffer, ULONG pBufferSize, bool pCopyBuffer)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mInputBuffer = pBuffer;
		mInputBufferSize = pBufferSize;

		if	(
				(pBuffer)
			&&	(pBufferSize)
			&&	(pCopyBuffer)
			)
		{
			if	(mInputBufferCopy = new BYTE [pBufferSize])
			{
				memcpy ((LPBYTE)mInputBufferCopy, pBuffer, pBufferSize);
				mInputBuffer = (LPBYTE)mInputBufferCopy;
#ifdef	_TRACE_PULL_DATA
				LogMessage (_TRACE_PULL_DATA, _T("[%s] [%p] InputBuffer [%u]"), mName, this, mInputBufferSize);
				LogDump (_TRACE_PULL_DATA, (LPVOID)mInputBuffer, min (mInputBufferSize, 64), _T("  "));
#endif
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinPull::FillSample (IMediaSample * pSample)
{
	HRESULT			lResult;
	REFERENCE_TIME	lStartTime;
	REFERENCE_TIME	lEndTime;
	LPBYTE			lBuffer;
	long			lLength;

	if	(
			(SUCCEEDED (lResult = pSample->GetTime (&lStartTime, &lEndTime)))
		&&	(SUCCEEDED (lResult = pSample->GetPointer (&lBuffer)))
		)
	{
#ifdef	_TRACE_PULL_DATA
		LogMediaSample (_TRACE_PULL_DATA, pSample, _T("[%s] [%p] FillSample"), mName, this);
#endif
		lStartTime /= sPullTimeScale;
		lEndTime /=	sPullTimeScale;
		lLength = (long)(lEndTime-lStartTime);

		if	(SUCCEEDED (lResult = FillBuffer (lBuffer, lStartTime, lLength)))
		{
			LogVfwErr (LogNormal, pSample->SetActualDataLength (lLength));

			if	(lResult == S_FALSE)
			{
				lStartTime *= sPullTimeScale;
				lEndTime = lStartTime + ((REFERENCE_TIME)lLength * sPullTimeScale);
				LogVfwErr (LogNormal, pSample->SetTime (&lStartTime, &lEndTime));
				mIsEndOfStream = true;
			}
#ifdef	_TRACE_PULL_DATA
			LogMediaSample (_TRACE_PULL_DATA, pSample, _T("[%s] [%p] FillSample done (eos %u)"), mName, this, mIsEndOfStream);
#endif
		}
	}
	return lResult;
}

HRESULT CDirectShowPinPull::FillBuffer (LPBYTE pBuffer, LONGLONG pStart, long & pLength)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		if	(
				(pStart < 0)
			||	(pLength <= 0)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(pStart > (LONGLONG)mInputBufferSize)
		{
			lResult = HRESULT_FROM_WIN32 (ERROR_HANDLE_EOF);
		}
		else
		{
			if	(pStart + pLength >= (LONGLONG)mInputBufferSize)
			{
				pLength = (long)(mInputBufferSize-pStart);
				lResult = S_FALSE;
			}
			memcpy (pBuffer, ((LPBYTE)mInputBuffer)+pStart, pLength);
#ifdef	_TRACE_PULL_DATA
			LogMessage (_TRACE_PULL_DATA, _T("[%s] [%p] FillBuffer [%I64d] [%d]"), mName, this, pStart, pLength);
			LogDump (_TRACE_PULL_DATA, pBuffer, min (pLength, 64), _T("  "));
#endif
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::ValidateSampleAlign (IMediaSample *pSample)
{
	HRESULT			lResult;
	REFERENCE_TIME	lStartTime;
	REFERENCE_TIME	lEndTime;

	if	(SUCCEEDED (lResult = pSample->GetTime (&lStartTime, &lEndTime)))
	{
		lResult = ValidateSampleAlign (lStartTime, lEndTime);
	}
	return lResult;
}

HRESULT CDirectShowPinPull::ValidateSampleAlign (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime)
{
	HRESULT	lResult = S_OK;

	if	(pStartTime/sPullTimeScale > (LONGLONG)mInputBufferSize)
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_HANDLE_EOF);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::BlockStream (bool pBlock, HANDLE pBlockingEvent)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mBlockingLock, TRUE);

	try
	{
		if	(!SafeGetConnection ())
		{
			lResult = VFW_E_NOT_CONNECTED;
		}
		else
		if	(pBlock)
		{
			if	(mBlockingMutex.Lock (0))
			{
				mBlockingMutex.Unlock ();

				if	(mBlockingMutex.Unlock ())
				{
					mBlockingMutex.Lock ();
					lResult = VFW_E_PIN_ALREADY_BLOCKED_ON_THIS_THREAD;
				}
				else
				if	(pBlockingEvent)
				{
					mBlockingEvent = pBlockingEvent;
					lResult = S_OK;
#ifdef	_TRACE_PULL_BLOCK
					LogMessage (_TRACE_PULL_BLOCK, _T("[%s] BlockStream [%p]"), mName, mBlockingEvent);
#endif
				}
				else
				{
					if	(
							(BlockStreamAsync ())
						||	(mBlockingMutex.Lock (0))
						)
					{
#ifdef	_TRACE_PULL_BLOCK
						LogMessage (_TRACE_PULL_BLOCK, _T("[%s] BlockStream <Blocked>"), mName);
#endif
						mRequestEvent.SetEvent ();
						lResult = S_OK;
					}
					else
					{
						lResult = VFW_E_PIN_ALREADY_BLOCKED;
					}
				}
			}
			else
			{
				lResult = VFW_E_PIN_ALREADY_BLOCKED;
			}
		}
		else
		{
			if	(mBlockingEvent)
			{
				lResult = S_OK;
			}
			mBlockingEvent = NULL;
			mBlockingMutex.Unlock ();
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

bool CDirectShowPinPull::BlockStreamAsync ()
{
	bool		lRet = false;
	CSingleLock	lLock (&mBlockingLock, TRUE);

	try
	{
		if	(mBlockingEvent)
		{
			if	(mBlockingMutex.Lock (0))
			{
				lRet = true;
			}
			if	(::SetEvent (mBlockingEvent))
			{
#ifdef	_TRACE_PULL_BLOCK
				LogMessage (_TRACE_PULL_BLOCK, _T("[%s] BlockStreamAsync <Blocked>"), mName);
#endif
				mRequestEvent.SetEvent ();
			}
			else
			{
#ifdef	_TRACE_PULL_BLOCK
				LogWinErr (MinLogLevel(_TRACE_PULL_BLOCK,LogAlways), GetLastError(), _T("[%s] BlockStreamAsync <Failed>"), mName);
#endif
				if	(lRet)
				{
					mBlockingMutex.Unlock ();
				}
				lRet = false;
			}
			mBlockingEvent = NULL;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

bool CDirectShowPinPull::IsStreamBlocked ()
{
	bool		lRet = false;
	CSingleLock	lLock (&mBlockingLock, TRUE);

	try
	{
		if	(mBlockingMutex.Unlock ())
		{
			mBlockingMutex.Lock (0);
			lRet = true;
#ifdef	_TRACE_PULL_BLOCK
			LogMessage (_TRACE_PULL_BLOCK, _T("[%s] StreamIsBlocked"), mName);
#endif
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::EndOutputStream ()
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] EndOfStream from [%s] to [%s]"), mName, m_dwRef, PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		if	(SUCCEEDED (LogVfwErr (LogVerbose, lConnection->EndOfStream ())))
		{
			lResult = S_OK;
		}
	}
	return lResult;
}

HRESULT CDirectShowPinPull::BeginOutputFlush ()
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_TRACE_PULL_STATE
		LogMessage (_TRACE_PULL_STATE, _T("[%s] [%u] BeginFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		lResult = LogVfwErr (LogVerbose, lConnection->BeginFlush ());
	}

	return lResult;
}

HRESULT CDirectShowPinPull::EndOutputFlush ()
{
	HRESULT	lResult = VFW_E_NOT_CONNECTED;
	IPinPtr	lConnection = SafeGetConnection ();

	if	(lConnection != NULL)
	{
#ifdef	_TRACE_PULL_STATE
		LogMessage (_TRACE_PULL_STATE, _T("[%s] [%u] EndFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(GetInterface()), PinIdStr(lConnection));
#endif
		lResult = LogVfwErr (LogVerbose, lConnection->EndFlush ());
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowPinPull::EndPullStream ()
{
	return S_OK; // The documentation for IAsyncReader says to return S_FALSE, but that does not work.
}

HRESULT CDirectShowPinPull::BeginPullFlush ()
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
#ifdef	_TRACE_PULL_STATE
		LogMessage (_TRACE_PULL_STATE, _T("[%s] BeginPullFlush"), mName);
#endif
		mIsFlushing = true;
		mRequestEvent.SetEvent ();
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinPull::EndPullFlush ()
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
#ifdef	_TRACE_PULL_STATE
		LogMessage (_TRACE_PULL_STATE, _T("[%s] EndPullFlush"), mName);
#endif
		mRequests.RemoveAll ();
		mIsEndOfStream = false;
		mIsFlushing = false;
		BlockStreamAsync ();
		mRequestEvent.ResetEvent ();
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::RequestAllocator (IMemAllocator *pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator **ppActual)
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::RequestAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppActual)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mStateLock, TRUE);

		try
		{
			lResult = pThis->UseAllocator (pPreferred);

			if	(lResult != S_OK)
			{
				lResult = pThis->GetAllocator (pProps);
			}
			if	(SUCCEEDED (lResult))
			{
				if	(*ppActual = pThis->mAllocator)
				{
					(*ppActual)->AddRef ();
				}
				else
				{
					lResult = VFW_E_NO_ALLOCATOR;
				}
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XAsyncReader::RequestAllocator"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::Request (IMediaSample *pSample, DWORD_PTR dwUser)
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::Request"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = E_FAIL;

	if	(!pSample)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&pThis->mDataLock, TRUE);

		try
		{
			if	(pThis->mIsFlushing)
			{
#ifdef	_TRACE_PULL_STATE
				LogMessage (_TRACE_PULL_STATE, _T("[%s] Request [%p] [%p] <Flushing>"), pThis->mName, pSample, dwUser);
#endif
				pThis->mRequestEvent.SetEvent ();
				lResult = VFW_E_WRONG_STATE;
			}
			else
			if	(SUCCEEDED (lResult = pThis->ValidateSampleAlign (pSample)))
			{
				tS <PullRequest>	lRequest;
#ifdef	_TRACE_PULL_STATE
				LogMessage (_TRACE_PULL_STATE, _T("[%s] Request [%p] [%p]"), pThis->mName, pSample, dwUser);
#endif
				lRequest.mSample = pSample;
				lRequest.mUserVal = dwUser;
				pThis->mRequests.Add (lRequest);
				pThis->mRequestEvent.SetEvent ();
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_TRACE_PULL_STATE
	LogVfwErrAnon (MinLogLevel (_TRACE_PULL_STATE,LogAlways), lResult, _T("[%s] Request [%p] [%p]"), pThis->mName, pSample, dwUser);
#endif
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (MaxLogLevel(_LOG_RESULTS,LogVerbose), lResult, _T("[%p(%d)] %s::XAsyncReader::Request"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::WaitForNext (DWORD dwTimeout, IMediaSample **ppSample,DWORD_PTR *pdwUser)
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::WaitForNext [%d]"), pThis, pThis->m_dwRef, pThis->mName, dwTimeout);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!ppSample)
	{
		lResult = E_POINTER;
	}
	else
	{
		DWORD				lWait = WAIT_ABANDONED;
		HANDLE				lRequestEvent = NULL;
		tS <PullRequest>	lRequest;
		bool				lEndOfStream = false;

		*ppSample = NULL;
		if	(pdwUser)
		{
			*pdwUser = 0;
		}
#ifdef	_TRACE_PULL_STATE
		LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%s]"), pThis->mName, FilterStateStr(pThis->mFilter.GetState()));
#endif

		if	(pThis->mDataLock.Lock (dwTimeout))
		{
			try
			{
				if	(
						(pThis->BlockStreamAsync ())
					||	(pThis->IsStreamBlocked ())
					)
				{
#ifdef	_TRACE_PULL_BLOCK
					LogMessage (_TRACE_PULL_BLOCK, _T("[%s] WaitForNext [%d] <Blocked>"), pThis->mName, pThis->mRequests.GetSize());
#endif
					pThis->mRequestEvent.SetEvent ();
					lResult = VFW_E_TIMEOUT;
				}
				else
				if	(pThis->mIsFlushing)
				{
					pThis->mRequestEvent.SetEvent ();
				}
				else
				{
					pThis->mRequestEvent.ResetEvent ();
					lRequestEvent = pThis->mRequestEvent;
				}
				lEndOfStream = pThis->mIsEndOfStream;
			}
			catch AnyExceptionSilent

			pThis->mDataLock.Unlock ();
		}
		else
		{
			lResult = VFW_E_TIMEOUT;
		}

		if	(!lRequestEvent)
		{
			if	(SUCCEEDED (lResult))
			{
#ifdef	_TRACE_PULL_STATE
				LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <Flushing>"), pThis->mName, pThis->mRequests.GetSize());
#endif
				lResult = VFW_E_WRONG_STATE;
			}
		}
		else
		if	(lEndOfStream)
		{
#ifdef	_TRACE_PULL_STATE
			LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <EndOfStream>"), pThis->mName, pThis->mRequests.GetSize());
#endif
			lResult = S_FALSE;
		}
		else
		{
#ifdef	_TRACE_PULL_STATE
			LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] ..."), pThis->mName, pThis->mRequests.GetSize());
#endif
			try
			{
				lWait = WaitForSingleObject (lRequestEvent, dwTimeout);
			}
			catch AnyExceptionSilent

#ifdef	_TRACE_PULL_STATE
			LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] done [%d]"), pThis->mName, pThis->mRequests.GetSize(), lWait);
#endif
		}

		if	(
				(lResult != VFW_E_TIMEOUT)
			&&	(pThis->mDataLock.Lock (dwTimeout))
			)
		{
			try
			{
				pThis->mRequestEvent.SetEvent ();

				if	(
						(pThis->BlockStreamAsync ())
					||	(pThis->IsStreamBlocked ())
					)
				{
#ifdef	_TRACE_PULL_BLOCK
					LogMessage (_TRACE_PULL_BLOCK, _T("[%s] WaitForNext [%d] <Blocked>"), pThis->mName, pThis->mRequests.GetSize());
#endif
#ifdef	_TRACE_PULL_STATE
					LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <Blocked>"), pThis->mName, pThis->mRequests.GetSize());
#endif
					lResult = VFW_E_TIMEOUT;
				}
				else
				{
					if	(pThis->mRequests.GetSize() > 0)
					{
						lRequest = pThis->mRequests [0];
						pThis->mRequests.RemoveAt (0);
					}
#ifdef	_TRACE_PULL_STATE
					if	(lRequest.mSample == NULL)
					{
						LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <NoRequest>"), pThis->mName, pThis->mRequests.GetSize());
					}
#endif
				}

				if	(
						(SUCCEEDED (lResult))
					&&	(!pThis->mIsEndOfStream)
					)
				{
					if	(pThis->mIsFlushing)
					{
#ifdef	_TRACE_PULL_STATE
						LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <Flushed>"), pThis->mName, pThis->mRequests.GetSize());
#endif
						lResult = VFW_E_TIMEOUT;
					}
					else
					if	(lWait == WAIT_OBJECT_0)
					{
						if	(lRequest.mSample == NULL)
						{
							lResult = S_FALSE;
						}
						else
						{
							lResult = pThis->FillSample (lRequest.mSample);
#ifdef	_TRACE_PULL_STATE
							LogMediaSampleId (_TRACE_PULL_STATE, lRequest.mSample, _T("[%s] WaitForNext"), pThis->mName);
#endif
						}
					}
					else
					{
#ifdef	_TRACE_PULL_STATE
						LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <TimeOut>"), pThis->mName, pThis->mRequests.GetSize());
#endif
						lResult = VFW_E_TIMEOUT;
					}
				}

				if	(
						(SUCCEEDED (lResult))
					&&	(pThis->mIsEndOfStream)
					)
				{
#ifdef	_TRACE_PULL_STATE
					LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <EndOfStream>"), pThis->mName, pThis->mRequests.GetSize());
#endif
					lResult = pThis->EndPullStream ();
				}

				if	(*ppSample = lRequest.mSample)
				{
					(*ppSample)->AddRef ();
				}
				if	(pdwUser)
				{
					*pdwUser = lRequest.mUserVal;
				}
			}
			catch AnyExceptionDebug

			pThis->mDataLock.Unlock ();
		}
		else
		{
			lResult = VFW_E_TIMEOUT;
		}
	}

#ifdef	_TRACE_PULL_STATE
	LogVfwErrAnon (MinLogLevel (_TRACE_PULL_STATE,LogAlways), lResult, _T("[%s] WaitForNext [%p] [%p]"), pThis->mName, (ppSample ? *ppSample : NULL), (pdwUser ? *pdwUser : NULL));
#endif

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (MaxLogLevel(_LOG_RESULTS,LogVerbose), lResult, _T("[%p(%d)] %s::XAsyncReader::WaitForNext"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::SyncReadAligned (IMediaSample *pSample)
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::SyncReadAligned"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult;

	if	(!pSample)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = pThis->ValidateSampleAlign (pSample)))
	{
		lResult = pThis->FillSample (pSample);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XAsyncReader::SyncReadAligned"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::SyncRead (LONGLONG llPosition, LONG lLength, BYTE *pBuffer)
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::SyncRead [%I64d] [%d] [%p]"), pThis, pThis->m_dwRef, pThis->mName, llPosition, lLength, pBuffer);
#endif
	HRESULT	lResult;

	if	(!pBuffer)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = pThis->FillBuffer (pBuffer, llPosition, lLength);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XAsyncReader::SyncRead"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::Length (LONGLONG *pTotal, LONGLONG *pAvailable)
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::XAsyncReader::Length [%u]"), pThis, pThis->m_dwRef, pThis->mName, pThis->mInputBufferSize);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mAllocator == NULL)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		CSingleLock	lLock (&pThis->mDataLock, TRUE);

		try
		{
			if	(pTotal)
			{
				*pTotal = (LONGLONG)pThis->mInputBufferSize;
			}
			if	(pAvailable)
			{
				*pAvailable = (LONGLONG)pThis->mInputBufferSize;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XAsyncReader::Length"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::BeginFlush ()
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = pThis->BeginPullFlush ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XAsyncReader::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XAsyncReader::EndFlush ()
{
	METHOD_PROLOGUE(CDirectShowPinPull, AsyncReader)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XAsyncReader::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	HRESULT	lResult = pThis->EndPullFlush ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XAsyncReader::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::XFlowControl::Block (DWORD dwBlockFlags, HANDLE hEvent)
{
	METHOD_PROLOGUE(CDirectShowPinPull, FlowControl)
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::XFlowControl::Block"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->BlockStream (((dwBlockFlags & AM_PIN_FLOW_CONTROL_BLOCK) != 0), hEvent);

#ifdef	_TRACE_PULL_BLOCK
	LogVfwErrAnon (MinLogLevel(_TRACE_PULL_BLOCK,LogAlways), lResult, _T("[%p(%d)] %s::XFlowControl::Block [%u] [%p]"), pThis, pThis->m_dwRef, pThis->mName, dwBlockFlags, hEvent);
#endif
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XFlowControl::Block"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}
