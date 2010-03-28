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
#include "DaCore.h"
#include "DirectShowPins.h"
#include "DirectShowEnums.h"
#include "DirectShowFilter.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#endif

#ifdef	_DEBUG
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
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_DirectShowPin"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

static const LONGLONG	sPullTimeScale = 10000000;

/////////////////////////////////////////////////////////////////////////////

CDirectShowPin::CDirectShowPin (PIN_DIRECTION pDirection)
:	mFilter (NULL),
	mDirection (pDirection),
	mIsFlushing (false),
	mIsEndOfStream (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPin::CDirectShowPin (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPin::~CDirectShowPin ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPin::~CDirectShowPin (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	SafeFreeSafePtr (mAllocator);
	SafeFreeSafePtr (mConnection);
}

/////////////////////////////////////////////////////////////////////////////

CDirectShowPin & CDirectShowPin::Initialize (CDirectShowFilter & pFilter, PIN_DIRECTION pDirection, LPCTSTR pName, LPCTSTR pUniqueId)
{
	mFilter = &pFilter;
	mUniqueId = pUniqueId;
	mName = pName;
	const_cast <PIN_DIRECTION &> (mDirection) = pDirection;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPin::Initialize (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	LogVfwErr (LogIfActive, MoDuplicateMediaType (mMediaType.Free(), &AM_MEDIA_TYPE_Safe()));
	
	return *this;
}

void CDirectShowPin::FinalRelease ()
{
#ifdef	_DEBUG
	if	(LogIsActive())
	{
		try
		{
			if	(
					(mFilter)
				&&	(
						(mFilter->mInputPins.Find (this) >= 0)
					||	(mFilter->mInputPins.Find (this) >= 0)
					)
				)
			{
				LogMessage (LogIfActive, _T("[%p(%d)] CDirectShowPin::FinalRelease [%s] !!!"), this, m_dwRef, mName);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI CDirectShowPin::DelegateIMediaSeeking (void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw)
{
	HRESULT				lResult = S_FALSE;
	CDirectShowPin *	lThis = (CDirectShowPin *) pv;	

	(*ppv) = NULL;
	
	if	(
			(lThis->mDirection == PINDIR_OUTPUT)
		&&	(lThis->mFilter)
		)
	{
		lResult = lThis->mFilter->QueryInterface (riid, ppv);
	}
	return lResult;
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

HRESULT CDirectShowPin::InternalConnect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = VFW_E_NO_ACCEPTABLE_TYPES;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(pMediaType)
		{
#ifdef	_DEBUG_CONNECTION
			LogMediaType (_DEBUG_CONNECTION, *pMediaType, _T("[%s] [%p] ProposeConnection to [%p] [%s] from [%s]"), mName, this, pReceivePin, PinIdStr(pReceivePin), PinIdStr(this));
#endif
			if	(
					(SUCCEEDED (pReceivePin->ReceiveConnection (this, pMediaType)))
				&&	(SUCCEEDED (lResult = MoDuplicateMediaType (mMediaType.Free(), pMediaType)))
				)
			{
				mConnection = pReceivePin;
				mFilter->OnPinConnected (this);
				lResult = S_OK;
#ifdef	_DEBUG_CONNECTION
				LogMediaType (_DEBUG_CONNECTION, *mMediaType, _T("[%s] [%p] Connected to [%p] [%s] from [%s]"), mName, this, pReceivePin, PinIdStr(pReceivePin), PinIdStr(this));
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
					&&	(InternalConnect (pReceivePin, lMediaType) == S_OK)
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

HRESULT CDirectShowPin::InternalReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
#ifdef	_DEBUG_CONNECTION
		LogMediaType (_DEBUG_CONNECTION, *pMediaType, _T("[%s] [%p] ReceiveConnection from [%p] [%s] to [%s]"), mName, this, pConnector, PinIdStr(pConnector), PinIdStr(this));
#endif
		if	(SUCCEEDED (lResult = MoDuplicateMediaType (mMediaType.Free(), pMediaType)))
		{
			mConnection = pConnector;
			mFilter->OnPinConnected (this);
			lResult = S_OK;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPin::InternalDisconnect ()
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
			mFilter->OnPinDisconnected (this);
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

HRESULT STDMETHODCALLTYPE CDirectShowPin::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::Connect [%p] [%p]"), this, m_dwRef, mName, pReceivePin, pmt);
#endif
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		lResult = CanConnect (pReceivePin);
		if	(
				(SUCCEEDED (lResult))
			&&	(pmt)
			)
		{
			lResult = QueryAccept (pmt);
		}
		if	(lResult == S_OK)
		{
			lResult = InternalConnect (pReceivePin, pmt);
			if	(FAILED (lResult))
			{
				InternalDisconnect ();
			}
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::Connect"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::ReceiveConnection"), this, m_dwRef, mName);
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
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			if	(SUCCEEDED (lResult))
			{
				lResult = CanConnect (pConnector);
			}
			if	(SUCCEEDED (lResult))
			{
				lResult = QueryAccept (pmt);

				if	(lResult == S_OK)
				{
					lResult = InternalReceiveConnection (pConnector, pmt);
					if	(FAILED (lResult))
					{
						InternalDisconnect ();
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
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::ReceiveConnection"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::Disconnect (void)
{
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::Disconnect"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = InternalDisconnect ();

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::Disconnect"), this, m_dwRef, mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::ConnectedTo (IPin **pPin)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::ConnectedTo"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pPin)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			if	(*pPin = mConnection)
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
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::ConnectedTo"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::ConnectionMediaType (AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::ConnectionMediaType"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pmt)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			if	(
					(mConnection != NULL)
				&&	(mMediaType.Ptr())
				)
			{
				lResult = MoCopyMediaType ((DMO_MEDIA_TYPE*)pmt, (DMO_MEDIA_TYPE*)mMediaType);
			}
			else
			{
				lResult = VFW_E_NOT_CONNECTED;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::ConnectionMediaType"), this, m_dwRef, mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::QueryPinInfo (PIN_INFO *pInfo)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::QueryPinInfo"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pInfo)
	{
		lResult = E_POINTER;
	}
	else
	{
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
		CSingleLock	lLock (&mStateLock, TRUE);
#endif
		try
		{
			IBaseFilterPtr	lBaseFilter (mFilter);

			pInfo->pFilter = lBaseFilter.Detach ();
			pInfo->dir = mDirection;
			wcsncpy (pInfo->achName, mName, sizeof(pInfo->achName)/sizeof(WCHAR));
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::QueryPinInfo"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::QueryDirection (PIN_DIRECTION *pPinDir)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::QueryDirection"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pPinDir)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pPinDir) = mDirection;
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::QueryDirection"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::QueryId (LPWSTR *Id)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::QueryId"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!Id)
	{
		lResult = E_POINTER;
	}
	else
	{
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
		CSingleLock	lLock (&mStateLock, TRUE);
#endif
		try
		{
			(*Id) = AfxAllocTaskWideString (mUniqueId);
			if	(*Id == NULL)
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::QueryId"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::QueryAccept (const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_CONNECTION
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::QueryAccept"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pmt)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			int				lMediaTypeNdx;
			AM_MEDIA_TYPE *	lMediaType;

#ifdef	_DEBUG_CONNECTION
			LogMediaType (_DEBUG_CONNECTION, *pmt, _T("[%s] [%p] QueryAccept to [%s]"), mName, this, PinIdStr(this));
#endif
			for	(lMediaTypeNdx = 0; lMediaTypeNdx <= mMediaTypes.GetUpperBound (); lMediaTypeNdx++)
			{
				if	(
						(lMediaType = mMediaTypes [lMediaTypeNdx])
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
					LogMediaType (_DEBUG_CONNECTION, *lMediaType, _T("[%s] [%p] CanAccept to [%s]"), mName, this, PinIdStr(this));
#endif
					lResult = S_OK;
				}
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_CONNECTION
	LogVfwErrAnon (_DEBUG_CONNECTION, lResult, _T("[%p(%d)] %s::QueryAccept"), this, m_dwRef, mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::EnumMediaTypes (IEnumMediaTypes **ppEnum)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::EnumMediaTypes"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			CComObject <CEnumMediaTypes> *	lEnum = NULL;
			IEnumMediaTypesPtr				lInterface;

			if	(SUCCEEDED (lResult = CComObject <CEnumMediaTypes>::CreateInstance (&lEnum)))
			{
				lEnum->Initialize (mMediaTypes, GetUnknown());
				lInterface = lEnum;
				(*ppEnum) = lInterface.Detach ();
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::EnumMediaTypes"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::QueryInternalConnections (IPin **apPin, ULONG *nPin)
{
#ifdef	_DEBUG_CONNECTION_NOT
	LogMessage (_DEBUG_CONNECTION, _T("[%p(%d)] %s::QueryInternalConnections"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = E_NOTIMPL;

#ifdef	_DEBUG_CONNECTION_NOT
	LogVfwErrAnon (_DEBUG_CONNECTION_NOT, lResult, _T("[%p(%d)] %s::QueryInternalConnections"), this, m_dwRef, mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::NewSegment (REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::NewSegment [%f - %f] [%f]"), this, m_dwRef, mName, RefTimeSec(tStart), RefTimeSec(tStop), dRate);
#endif
	HRESULT	lResult = S_FALSE;

	try
	{
		if	(mDirection == PINDIR_INPUT)
		{
			lResult = BeginInputStream (tStart, tStop, dRate);
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::NewSegment"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::EndOfStream (void)
{
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::EndOfStream"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = E_UNEXPECTED;

	try
	{
		if	(mDirection == PINDIR_INPUT)
		{
			lResult = EndInputStream ();
		}
		else
		if	(mDirection == PINDIR_OUTPUT)
		{
			IPinPtr	lConnection = SafeGetConnection ();

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
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::EndOfStream"), this, m_dwRef, mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPin::BeginFlush (void)
{
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::BeginFlush"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = BeginInputFlush ();

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::BeginFlush"), this, m_dwRef, mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPin::EndFlush (void)
{
#ifdef	_DEBUG_STREAM
	LogMessage (_DEBUG_STREAM, _T("[%p(%d)] %s::EndFlush"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = EndInputFlush ();

#ifdef	_DEBUG_STREAM
	LogVfwErrAnon (_DEBUG_STREAM, lResult, _T("[%p(%d)] %s::EndFlush"), this, m_dwRef, mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectShowPinIn::CDirectShowPinIn ()
:	CDirectShowPin (PINDIR_INPUT),
	mDesiredSampleCount (1),
	mMaxSampleCount (1),
	mReadOnlySamples (false),
	mEosNotifyEvent (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinIn::CDirectShowPinIn (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinIn::~CDirectShowPinIn ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinOut::~CDirectShowPinOut (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	mSamples.RemoveAll ();
	SafeFreeSafePtr (mCacheAllocator);
}

CDirectShowPinIn & CDirectShowPinIn::Initialize (CDirectShowFilter & pFilter, LPCTSTR pName, LPCTSTR pUniqueId, UINT pDesiredSampleCount, UINT pMaxSampleCount)
{
	CDirectShowPin::Initialize (pFilter, PINDIR_INPUT, pName, pUniqueId);
	
	const_cast <UINT &> (mDesiredSampleCount) = max (pDesiredSampleCount, 1);
	const_cast <UINT &> (mMaxSampleCount) = max (pDesiredSampleCount, pMaxSampleCount);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinIn::Initialize (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	return *this;
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
					(mFilter->mInputPins.GetSize() == 1)
				&&	(mFilter->mOutputPins.GetSize() == 1)
				)
			{
				IMemInputPinPtr		lDownstreamTransport = mFilter->mOutputPins[0]->SafeGetConnection();
				IMemAllocatorPtr	lDownstreamAllocator = mFilter->mOutputPins[0]->SafeGetAllocator ();

				if	(
						(lDownstreamAllocator != NULL)
					&&	(SUCCEEDED (lDownstreamAllocator->GetProperties (lActualMem.Clear())))
					&&	(lActualMem.cBuffers >= lRequiredMem.cBuffers)
					&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
					)
				{
#ifdef	_DEBUG_ALLOCATOR
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Downstream [%s] Actual "), PinIdStr(mFilter->mOutputPins[0]->SafeGetConnection()));
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
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Downstream [%s] Requirements "), PinIdStr(mFilter->mOutputPins[0]->SafeGetConnection()));
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

HRESULT CDirectShowPinIn::InternalReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(SUCCEEDED (lResult = CDirectShowPin::InternalReceiveConnection (pConnector, pMediaType)))
		{
			lResult = ProvideAllocator ();
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinIn::InternalDisconnect ()
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

	return CDirectShowPin::InternalDisconnect ();
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
			if	(mFilter->GetState() == State_Stopped)
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
					mFilter->OnGotInputSample (this);
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%f] BeginInputStream [%f - %f]"), mName, RefTimeSec(mFilter->GetStreamTime(mFilter->GetState())), RefTimeSec(pStartTime), RefTimeSec(pEndTime));
#endif
		mIsEndOfStream = false;
		mFilter->OnStartInputStream (pStartTime, pEndTime, pRate);
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
			LogMessage (_DEBUG_STATE, _T("[%s] [%f] EndInputStream (Queued {%d])"), mName, RefTimeSec(mFilter->GetStreamTime(mFilter->GetState())), mSamples.GetSize ());
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
				LogMessage (_DEBUG_DYNCONNECTION, _T("[%s] [%f] NotifyEos [%p]"), mName, RefTimeSec(mFilter->GetStreamTime(mFilter->GetState())), mEosNotifyEvent);
#endif
				::SetEvent (mEosNotifyEvent);
			}
		}
		catch AnyExceptionDebug
	}

	mFilter->OnEndInputStream (lSampleCount);

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
		LogMessage (_DEBUG_STATE, _T("[%s] [%f] BeginInputFlush"), mName, RefTimeSec(mFilter->GetStreamTime(mFilter->GetState())));
#endif
		mIsFlushing = true;
		mFilter->BeginFlush ();
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%f] EndInputFlush"), mName, RefTimeSec(mFilter->GetStreamTime(mFilter->GetState())));
#endif
		mIsEndOfStream = false;
		if	(lAllocator != NULL)
		{
			LogVfwErr (LogNormal, lAllocator->Commit ());
		}
		mFilter->EndFlush ();
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

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::GetAllocator (IMemAllocator **ppAllocator)
{
#ifdef	_DEBUG_ALLOCATOR
	LogMessage (_DEBUG_ALLOCATOR, _T("[%p(%d)] %s::GetAllocator [%p]"), this, m_dwRef, mName, ppAllocator);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppAllocator)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			lResult = ProvideAllocator ();
			if	(
					(SUCCEEDED (lResult))
				&&	(*ppAllocator = mAllocator)
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
		LogVfwErrAnon ((lResult==VFW_E_NO_TYPES)?MaxLogLevel(_LOG_RESULTS,LogVerbose):_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetAllocator"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::NotifyAllocator (IMemAllocator *pAllocator, BOOL bReadOnly)
{
#ifdef	_DEBUG_ALLOCATOR
	LogMessage (_DEBUG_ALLOCATOR, _T("[%p(%d)] %s::NotifyAllocator"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pAllocator)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = ReceiveAllocator (pAllocator, (bReadOnly!=FALSE));
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::NotifyAllocator"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetAllocatorRequirements"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pProps)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			memset (pProps, 0, sizeof(*pProps));
			if	(mAllocator != NULL)
			{
				lResult = mAllocator->GetProperties (pProps);
			}
			if	(pProps->cbAlign <= 1)
			{
				pProps->cbAlign = 8;
			}
#ifdef	_DEBUG_ALLOCATOR
			LogAllocatorProps (_DEBUG_ALLOCATOR, *pProps, _T("[%s] [%p]   Requirements"), mName, this);
#endif
		}
		catch AnyExceptionDebug
	}

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::Receive (IMediaSample *pSample)
{
#ifdef	_DEBUG_STREAM_EX
	LogMessage (_DEBUG_STREAM_EX, _T("[%p(%d)] %s::Receive"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pSample)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = PutInputSample (pSample, 500);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (((lResult==VFW_E_WRONG_STATE)||(lResult==VFW_E_TIMEOUT))?MaxLogLevel(_LOG_RESULTS,LogVerbose):_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Receive [%s]"), this, m_dwRef, mName, FilterStateStr(mFilter->GetState()));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::ReceiveMultiple (IMediaSample **pSamples, long nSamples, long *nSamplesProcessed)
{
#ifdef	_DEBUG_STREAM_EX
	LogMessage (_DEBUG_STREAM_EX, _T("[%p(%d)] %s::ReceiveMultiple [%d]"), this, m_dwRef, mName, nSamples);
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
			if	(SUCCEEDED (lResult = PutInputSample (pSamples [lSampleNdx], 500)))
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
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::ReceiveMultiple"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::ReceiveCanBlock (void)
{
#ifdef	_DEBUG_ALLOCATOR
	LogMessage (_DEBUG_ALLOCATOR, _T("[%p(%d)] %s::ReceiveCanBlock"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::DynamicQueryAccept (const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::DynamicQueryAccept"), this, m_dwRef, mName);
#endif
	return CDirectShowPin::QueryAccept (pmt);
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::NotifyEndOfStream (HANDLE hNotifyEvent)
{
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::NotifyEndOfStream [%p]"), this, m_dwRef, mName, hNotifyEvent);
#endif
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock);

	try
	{
		if	(hNotifyEvent)
		{
			mEosNotifyEvent = hNotifyEvent;
			lResult = S_OK;
		}
		else
		{
			if	(mEosNotifyEvent)
			{
				lResult = S_OK;
			}
			mEosNotifyEvent = NULL;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::IsEndPin (void)
{
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::IsEndPin"), this, m_dwRef, mName);
#endif
	return (mFilter->mOutputPins.GetSize() <= 0) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinIn::DynamicDisconnect (void)
{
#ifdef	_DEBUG_DYNCONNECTION
	LogMessage (_DEBUG_DYNCONNECTION, _T("[%p(%d)] %s::DynamicDisconnect"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_FALSE;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectShowPinOut::CDirectShowPinOut ()
:	CDirectShowPin (PINDIR_OUTPUT),
	mDesiredSampleCount (1),
	mSampleSize (0),
	mReadOnlySamples (true)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinOut::CDirectShowPinOut (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinOut::~CDirectShowPinOut ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinOut::~CDirectShowPinOut (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinOut & CDirectShowPinOut::Initialize (CDirectShowFilter & pFilter, LPCTSTR pName, LPCTSTR pUniqueId, UINT pDesiredSampleCount)
{
	CDirectShowPin::Initialize (pFilter, PINDIR_OUTPUT, pName, pUniqueId);
	
	const_cast <UINT &> (mDesiredSampleCount) = pDesiredSampleCount;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinOut::Initialize (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	return *this;
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

HRESULT CDirectShowPinOut::InternalConnect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		lResult = CDirectShowPin::InternalConnect (pReceivePin, pMediaType);

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

HRESULT CDirectShowPinOut::InternalDisconnect ()
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

	return CDirectShowPin::InternalDisconnect ();
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
					(mFilter->mInputPins.GetSize() == 1)
				&&	(mFilter->mOutputPins.GetSize() == 1)
				)
			{
				IMemInputPinPtr		lUpstreamTransport (mFilter->mInputPins[0]);
				IMemAllocatorPtr	lUpstreamAllocator (mFilter->mInputPins[0]->SafeGetAllocator ());

				if	(
						(lUpstreamAllocator != NULL)
					&&	(SUCCEEDED (lUpstreamAllocator->GetProperties (lActualMem.Clear())))
					&&	(lActualMem.cBuffers >= lRequiredMem.cBuffers)
					&&	(lActualMem.cbBuffer >= lRequiredMem.cbBuffer)
					)
				{
#ifdef	_DEBUG_ALLOCATOR
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Upstream [%s] Actual "), PinIdStr(mFilter->mInputPins[0]));
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
					LogAllocatorProps (_DEBUG_ALLOCATOR, lActualMem, _T("Upstream [%s] Requirements "), PinIdStr(mFilter->mInputPins[0]));
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] NewSegment [%f - %f] from [%s] to [%s]"), mName, m_dwRef, RefTimeSec(pStartTime), RefTimeSec(pEndTime), PinIdStr(this), PinIdStr(lConnection));
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] EndOfStream from [%s] to [%s]"), mName, m_dwRef, PinIdStr(this), PinIdStr(lConnection));
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] BeginFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(this), PinIdStr(lConnection));
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] EndFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(this), PinIdStr(lConnection));
#endif
		lResult = LogVfwErr (LogVerbose, lConnection->EndFlush ());
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectShowPinPull::CDirectShowPinPull ()
:	CDirectShowPin (PINDIR_OUTPUT),
	mInputBuffer (NULL),
	mInputBufferSize (0),
	mBlockingEvent (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinPull::CDirectShowPinPull (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinPull::~CDirectShowPinPull ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinPull::~CDirectShowPinPull (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowPinPull & CDirectShowPinPull::Initialize (CDirectShowFilter & pFilter, LPCTSTR pName, LPCTSTR pUniqueId)
{
	CDirectShowPin::Initialize (pFilter, PINDIR_OUTPUT, pName, pUniqueId);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowPinPull::Initialize (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	return *this;
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

HRESULT CDirectShowPinPull::InternalConnect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		lResult = pReceivePin->Connect (this, pMediaType);
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowPinPull::InternalDisconnect ()
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		BlockStream (false);
		mRequests.RemoveAll ();
		mRequestEvent.SetEvent ();
	}
	catch AnyExceptionDebug

	return CDirectShowPin::InternalDisconnect ();
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
		LogMessage (_DEBUG_STATE, _T("[%s] [%u] EndOfStream from [%s] to [%s]"), mName, m_dwRef, PinIdStr(this), PinIdStr(lConnection));
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
		LogMessage (_TRACE_PULL_STATE, _T("[%s] [%u] BeginFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(this), PinIdStr(lConnection));
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
		LogMessage (_TRACE_PULL_STATE, _T("[%s] [%u] EndFlush from [%s] to [%s]"), mName, m_dwRef, PinIdStr(this), PinIdStr(lConnection));
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

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::RequestAllocator (IMemAllocator *pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator **ppActual)
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::RequestAllocator"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppActual)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			lResult = UseAllocator (pPreferred);

			if	(lResult != S_OK)
			{
				lResult = GetAllocator (pProps);
			}
			if	(SUCCEEDED (lResult))
			{
				if	(*ppActual = mAllocator)
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
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::RequestAllocator"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::Request (IMediaSample *pSample, DWORD_PTR dwUser)
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::Request"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = E_FAIL;

	if	(!pSample)
	{
		lResult = E_POINTER;
	}
	else
	{
		CSingleLock	lLock (&mDataLock, TRUE);

		try
		{
			if	(mIsFlushing)
			{
#ifdef	_TRACE_PULL_STATE
				LogMessage (_TRACE_PULL_STATE, _T("[%s] Request [%p] [%p] <Flushing>"), mName, pSample, dwUser);
#endif
				mRequestEvent.SetEvent ();
				lResult = VFW_E_WRONG_STATE;
			}
			else
			if	(SUCCEEDED (lResult = ValidateSampleAlign (pSample)))
			{
				tS <PullRequest>	lRequest;
#ifdef	_TRACE_PULL_STATE
				LogMessage (_TRACE_PULL_STATE, _T("[%s] Request [%p] [%p]"), mName, pSample, dwUser);
#endif
				lRequest.mSample = pSample;
				lRequest.mUserVal = dwUser;
				mRequests.Add (lRequest);
				mRequestEvent.SetEvent ();
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_TRACE_PULL_STATE
	LogVfwErrAnon (MinLogLevel (_TRACE_PULL_STATE,LogAlways), lResult, _T("[%s] Request [%p] [%p]"), mName, pSample, dwUser);
#endif
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (MaxLogLevel(_LOG_RESULTS,LogVerbose), lResult, _T("[%p(%d)] %s::Request"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::WaitForNext (DWORD dwTimeout, IMediaSample **ppSample,DWORD_PTR *pdwUser)
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::WaitForNext [%d]"), this, m_dwRef, mName, dwTimeout);
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
		LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%s]"), mName, FilterStateStr(mFilter->GetState()));
#endif

		if	(mDataLock.Lock (dwTimeout))
		{
			try
			{
				if	(
						(BlockStreamAsync ())
					||	(IsStreamBlocked ())
					)
				{
#ifdef	_TRACE_PULL_BLOCK
					LogMessage (_TRACE_PULL_BLOCK, _T("[%s] WaitForNext [%d] <Blocked>"), mName, mRequests.GetSize());
#endif
					mRequestEvent.SetEvent ();
					lResult = VFW_E_TIMEOUT;
				}
				else
				if	(mIsFlushing)
				{
					mRequestEvent.SetEvent ();
				}
				else
				{
					mRequestEvent.ResetEvent ();
					lRequestEvent = mRequestEvent;
				}
				lEndOfStream = mIsEndOfStream;
			}
			catch AnyExceptionSilent

			mDataLock.Unlock ();
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
				LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <Flushing>"), mName, mRequests.GetSize());
#endif
				lResult = VFW_E_WRONG_STATE;
			}
		}
		else
		if	(lEndOfStream)
		{
#ifdef	_TRACE_PULL_STATE
			LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <EndOfStream>"), mName, mRequests.GetSize());
#endif
			lResult = S_FALSE;
		}
		else
		{
#ifdef	_TRACE_PULL_STATE
			LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] ..."), mName, mRequests.GetSize());
#endif
			try
			{
				lWait = WaitForSingleObject (lRequestEvent, dwTimeout);
			}
			catch AnyExceptionSilent

#ifdef	_TRACE_PULL_STATE
			LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] done [%d]"), mName, mRequests.GetSize(), lWait);
#endif
		}

		if	(
				(lResult != VFW_E_TIMEOUT)
			&&	(mDataLock.Lock (dwTimeout))
			)
		{
			try
			{
				mRequestEvent.SetEvent ();

				if	(
						(BlockStreamAsync ())
					||	(IsStreamBlocked ())
					)
				{
#ifdef	_TRACE_PULL_BLOCK
					LogMessage (_TRACE_PULL_BLOCK, _T("[%s] WaitForNext [%d] <Blocked>"), mName, mRequests.GetSize());
#endif
#ifdef	_TRACE_PULL_STATE
					LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <Blocked>"), mName, mRequests.GetSize());
#endif
					lResult = VFW_E_TIMEOUT;
				}
				else
				{
					if	(mRequests.GetSize() > 0)
					{
						lRequest = mRequests [0];
						mRequests.RemoveAt (0);
					}
#ifdef	_TRACE_PULL_STATE
					if	(lRequest.mSample == NULL)
					{
						LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <NoRequest>"), mName, mRequests.GetSize());
					}
#endif
				}

				if	(
						(SUCCEEDED (lResult))
					&&	(!mIsEndOfStream)
					)
				{
					if	(mIsFlushing)
					{
#ifdef	_TRACE_PULL_STATE
						LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <Flushed>"), mName, mRequests.GetSize());
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
							lResult = FillSample (lRequest.mSample);
#ifdef	_TRACE_PULL_STATE
							LogMediaSampleId (_TRACE_PULL_STATE, lRequest.mSample, _T("[%s] WaitForNext"), mName);
#endif
						}
					}
					else
					{
#ifdef	_TRACE_PULL_STATE
						LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <TimeOut>"), mName, mRequests.GetSize());
#endif
						lResult = VFW_E_TIMEOUT;
					}
				}

				if	(
						(SUCCEEDED (lResult))
					&&	(mIsEndOfStream)
					)
				{
#ifdef	_TRACE_PULL_STATE
					LogMessage (_TRACE_PULL_STATE, _T("[%s] WaitForNext [%d] <EndOfStream>"), mName, mRequests.GetSize());
#endif
					lResult = EndPullStream ();
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

			mDataLock.Unlock ();
		}
		else
		{
			lResult = VFW_E_TIMEOUT;
		}
	}

#ifdef	_TRACE_PULL_STATE
	LogVfwErrAnon (MinLogLevel (_TRACE_PULL_STATE,LogAlways), lResult, _T("[%s] WaitForNext [%p] [%p]"), mName, (ppSample ? *ppSample : NULL), (pdwUser ? *pdwUser : NULL));
#endif

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (MaxLogLevel(_LOG_RESULTS,LogVerbose), lResult, _T("[%p(%d)] %s::WaitForNext"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::SyncReadAligned (IMediaSample *pSample)
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::SyncReadAligned"), this, m_dwRef, mName);
#endif
	HRESULT	lResult;

	if	(!pSample)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = ValidateSampleAlign (pSample)))
	{
		lResult = FillSample (pSample);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SyncReadAligned"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::SyncRead (LONGLONG llPosition, LONG lLength, BYTE *pBuffer)
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::SyncRead [%I64d] [%d] [%p]"), this, m_dwRef, mName, llPosition, lLength, pBuffer);
#endif
	HRESULT	lResult;

	if	(!pBuffer)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = FillBuffer (pBuffer, llPosition, lLength);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SyncRead"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::Length (LONGLONG *pTotal, LONGLONG *pAvailable)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::Length [%u]"), this, m_dwRef, mName, mInputBufferSize);
#endif
	HRESULT	lResult = S_OK;

	if	(mAllocator == NULL)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		CSingleLock	lLock (&mDataLock, TRUE);

		try
		{
			if	(pTotal)
			{
				*pTotal = (LONGLONG)mInputBufferSize;
			}
			if	(pAvailable)
			{
				*pAvailable = (LONGLONG)mInputBufferSize;
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Length"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::BeginFlush ()
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::BeginFlush"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = BeginPullFlush ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::BeginFlush"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::EndFlush ()
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::EndFlush"), this, m_dwRef, mName);
#endif
	HRESULT	lResult = EndPullFlush ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::EndFlush"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowPinPull::Block (DWORD dwBlockFlags, HANDLE hEvent)
{
#ifdef	_DEBUG_PULL
	LogMessage (_DEBUG_PULL, _T("[%p(%d)] %s::Block"), this, m_dwRef, mName);
#endif

	HRESULT	lResult = BlockStream (((dwBlockFlags & AM_PIN_FLOW_CONTROL_BLOCK) != 0), hEvent);

#ifdef	_TRACE_PULL_BLOCK
	LogVfwErrAnon (MinLogLevel(_TRACE_PULL_BLOCK,LogAlways), lResult, _T("[%p(%d)] %s::Block [%u] [%p]"), this, m_dwRef, mName, dwBlockFlags, hEvent);
#endif
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Block"), this, m_dwRef, mName);
	}
#endif
	return lResult;
}
