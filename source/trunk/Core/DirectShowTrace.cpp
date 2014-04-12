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
#include "DirectShowTrace.h"
#include "DirectShowEnums.h"
#include "GuidStr.h"

//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_QUERIES		LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_BASEFILTER		LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_MEDIASEEKING		LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_PIN				LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_MEMINPUTPIN		LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_ASYNCREADER		LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_PINCONNECTION	LogNormal|LogTimeMs|LogHighVolume
#define	_DEBUG_PINFLOWCONTROL	LogNormal|LogTimeMs|LogHighVolume
#define	_LOG_RESULTS			LogNormal|LogTimeMs|LogHighVolume

//#define	_LOG_ALLOCATOR		LogNormal|LogTimeMs|LogHighVolume
#define	_LOG_SAMPLES			LogNormal|LogTimeMs|LogHighVolume
#define	_LOG_SEEKING			LogNormal|LogTimeMs|LogHighVolume
//#define	_TRACE_SAMPLE_TRACER	LogIfActive

/////////////////////////////////////////////////////////////////////////////

CTraceSamples::CTraceSamples ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::CTraceSamples (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
#ifdef	_TRACE_SAMPLE_TRACER
	// Does not work yet
	if	(SUCCEEDED (LogComErr (LogNormal|LogTime, CoCreateInstance (__uuidof(SampleGrabber), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mGrabberFilter))))
	{
		mGrabberFilter = (new CTraceFilter(mGrabberFilter, _T("Grabber"), _TRACE_SAMPLE_TRACER))->GetControllingUnknown();
	}
#endif
}

CTraceSamples::~CTraceSamples ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::~CTraceSamples (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
	Disconnect ();
}

CTraceSamples& CTraceSamples::Initialize (LPCTSTR pFilterName)
{
	mFilterName = pFilterName;
	if	(mFilterName.IsEmpty())
	{
		mFilterName = _T("TraceSamples");
	}
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::Initialize (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
	return *this;
}

void CTraceSamples::FinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::FinalRelease"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CTraceSamples& CTraceSamples::ConnectBefore (IFilterGraph* pFilterGraph, IBaseFilter* pDownstreamFilter, IPin* pDownstreamPin, AM_MEDIA_TYPE* pMediaType)
{
	try
	{
		HRESULT				lResult = E_FAIL;
		IPinPtr				lDownstreamPin (pDownstreamPin);
		IPinPtr				lUpstreamPin;
		IPinPtr				lInputPin;
		IPinPtr				lOutputPin;
		AM_MEDIA_TYPE_Safe	lMediaType;

		if	(lDownstreamPin == NULL)
		{
			if	(pMediaType)
			{
				lResult = GetFilterPin (pDownstreamFilter, PINDIR_INPUT, pMediaType->majortype, &lDownstreamPin);
			}
			else
			{
				lResult = GetFilterPins (pDownstreamFilter, &lDownstreamPin, NULL);
			}
		}
		if	(
				(lDownstreamPin != NULL)
			&&	(!pMediaType)
			&&	(SUCCEEDED (lDownstreamPin->ConnectionMediaType (&lMediaType)))
			)
		{
			pMediaType = &lMediaType;
		}

		if	(
				(lDownstreamPin != NULL)
			&&	(pMediaType != NULL)
			&&	(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, lDownstreamPin->ConnectedTo (&lUpstreamPin))))
			&&	(lUpstreamPin != NULL)
			)
		{
			if	(
					(
						(mGrabberFilter != NULL)
					||	(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (__uuidof(SampleGrabber), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mGrabberFilter))))
					)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, pFilterGraph->AddFilter (mGrabberFilter, mFilterName))))
				)
			{
				mGrabber = mGrabberFilter;
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, mGrabber->SetMediaType (pMediaType))))
				&&	(SUCCEEDED (lResult = GetFilterPins (mGrabberFilter, &lInputPin, &lOutputPin)))
				&&	(lInputPin != NULL)
				&&	(lOutputPin != NULL)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, lDownstreamPin->Disconnect ())))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, lUpstreamPin->Disconnect ())))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, pFilterGraph->ConnectDirect (lUpstreamPin, lInputPin, pMediaType))))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, pFilterGraph->ConnectDirect (lOutputPin, lDownstreamPin, pMediaType))))
				)
			{
			}
			else
			{
				SafeFreeSafePtr (mGrabber);
			}
		}
	}
	catch AnyExceptionDebug

	return *this;
}

CTraceSamples& CTraceSamples::ConnectAfter (IFilterGraph* pFilterGraph, IBaseFilter* pUpstreamFilter, IPin* pUpstreamPin, AM_MEDIA_TYPE* pMediaType)
{
	try
	{
		HRESULT			lResult = E_FAIL;
		IPinPtr			lUpstreamPin (pUpstreamPin);
		IPinPtr			lDownstreamPin;
		PIN_INFO_Safe	lDownstreamPinInfo;

		if	(lUpstreamPin == NULL)
		{
			if	(pMediaType)
			{
				lResult = GetFilterPin (pUpstreamFilter, PINDIR_OUTPUT, pMediaType->majortype, &lUpstreamPin);
			}
			else
			{
				lResult = GetFilterPins (pUpstreamFilter, NULL, &lUpstreamPin);
			}
		}

		if	(
				(lUpstreamPin != NULL)
			&&	(SUCCEEDED (lResult = lUpstreamPin->ConnectedTo (&lDownstreamPin)))
			&&	(lDownstreamPin != NULL)
			&&	(SUCCEEDED (lResult = lDownstreamPin->QueryPinInfo (&lDownstreamPinInfo)))
			&&	(lDownstreamPinInfo.pFilter != NULL)
			)
		{
			ConnectBefore (pFilterGraph, lDownstreamPinInfo.pFilter, lDownstreamPin);
		}
	}
	catch AnyExceptionDebug

	return *this;
}

CTraceSamples& CTraceSamples::Disconnect ()
{
	try
	{
		HRESULT				lResult = S_FALSE;
		IBaseFilterPtr		lFilter (mGrabberFilter);
		FILTER_INFO_Safe	lFilterInfo;

		SafeFreeSafePtr (mGrabberFilter);

		if	(
				(lFilter != NULL)
			&&	(SUCCEEDED (lResult = lFilter->QueryFilterInfo (&lFilterInfo)))
			&&	(lFilterInfo.pGraph != NULL)
			)
		{
			lResult = LogVfwErr (LogNormal|LogTime, lFilterInfo.pGraph->RemoveFilter (lFilter));
		}
		else
		if	(SUCCEEDED (lResult))
		{
			lResult = S_FALSE;
		}
	}
	catch AnyExceptionDebug

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CTraceSamples& CTraceSamples::LogSamples (UINT pLogLevel, bool pOneShot)
{
	try
	{
		HRESULT	lResult = E_FAIL;

		mLogLevelSample = pLogLevel;
		if	(
				(mGrabber != NULL)
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, mGrabber->SetCallback (this, 0))))
			)
		{
			LogVfwErr (LogNormal|LogTime, mGrabber->SetOneShot (pOneShot!=false));
		}
	}
	catch AnyExceptionDebug

	return *this;
}

CTraceSamples& CTraceSamples::DumpSamples (UINT pByteCount, UINT pLogLevel, bool pOneShot)
{
	try
	{
		HRESULT	lResult = E_FAIL;

		mLogLevelDump = pLogLevel;
		mDumpSize = pByteCount;

		if	(
				(mGrabber != NULL)
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, mGrabber->SetCallback (this, 1))))
			)
		{
			LogVfwErr (LogNormal|LogTime, mGrabber->SetOneShot (pOneShot!=false));
		}
	}
	catch AnyExceptionDebug

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceSamples::SampleCB (double SampleTime, IMediaSample*pSample)
{
	try
	{
		LogMediaSample (mLogLevelSample, pSample, _T("[%s] At [%f]"), mFilterName, SampleTime);
	}
	catch AnyExceptionSilent
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CTraceSamples::BufferCB (double SampleTime, BYTE*pBuffer, long BufferLen)
{
	try
	{
		LogMessage (mLogLevelDump, _T("[%s] At [%f] of [%d]"), mFilterName, SampleTime, BufferLen);
		LogDump (mLogLevelDump, pBuffer, min (BufferLen, mDumpSize), _T("  "));
	}
	catch AnyExceptionSilent
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CTraceFilter::CTraceFilter ()
:	mLogLevelPins (LogVerbose|LogTime)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::CTraceFilter (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
}

CTraceFilter::~CTraceFilter ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::~CTraceFilter (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
}

CTraceFilter& CTraceFilter::Initialize (LPUNKNOWN pUnknown, LPCTSTR pFilterName, UINT pLogLevelPins)
{
	mFilterName = pFilterName;
	if	(mFilterName.IsEmpty())
	{
		mFilterName = _T("TraceFilter");
	}
	mLogLevelPins = pLogLevelPins;
	Aggregate (pUnknown);

#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::Initialize (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
	return *this;
}

void CTraceFilter::FinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::FinalRelease"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI CTraceFilter::CheckInternalInterfaces (void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw)
{
	HRESULT			lResult = S_FALSE;
	CTraceFilter *	lThis = (CTraceFilter *) pv;

	if	(
			(IsEqualGUID (riid, __uuidof(IMediaSeeking)))
		&&	(lThis->mInnerMediaSeeking == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(ISampleGrabber)))
		&&	(lThis->mInnerSampleGrabber == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(IFileSourceFilter)))
		&&	(lThis->mInnerFileSourceFilter == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(IAMFilterMiscFlags)))
		&&	(lThis->mInnerAMFilterMiscFlags == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	return lResult;
}

void CTraceFilter::Aggregate (LPUNKNOWN pUnknown)
{
	mUnknown = pUnknown;
	mInnerBaseFilter = pUnknown;
	mInnerMediaSeeking = pUnknown;
	mInnerSampleGrabber = pUnknown;
	mInnerFileSourceFilter = pUnknown;
	mInnerAMFilterMiscFlags = pUnknown;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::GetClassID (CLSID *pClassID)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetClassID"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT lResult = mInnerBaseFilter->GetClassID (pClassID);

#ifdef	_LOG_RESULTS
	LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetClassID"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::Stop (void)
{
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%d)] %s::Stop"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->Stop ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Stop"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::Pause (void)
{
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%d)] %s::Pause"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->Pause ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Pause"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::Run (REFERENCE_TIME tStart)
{
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%d)] %s::Run [%f]"), this, max(m_dwRef,-1), mFilterName, RefTimeSec(tStart));
#endif

	HRESULT	lResult = mInnerBaseFilter->Run (tStart);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Run"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetState (DWORD dwMilliSecsTimeout, FILTER_STATE *State)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetState"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->GetState (dwMilliSecsTimeout, State);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetState"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetSyncSource (IReferenceClock *pClock)
{
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%d)] %s::SetSyncSource [%p]"), this, max(m_dwRef,-1), mFilterName, pClock);
#endif

	HRESULT	lResult = mInnerBaseFilter->SetSyncSource (pClock);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SetSyncSource"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetSyncSource (IReferenceClock **pClock)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetSyncSource"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->GetSyncSource (pClock);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetSyncSource"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::EnumPins (IEnumPins **ppEnum)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::EnumPins"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT			lResult;
	IEnumPinsPtr	lEnum;

	lResult = mInnerBaseFilter->EnumPins (&lEnum);
	lEnum = (new CComObject<CTracePins>)->Initialize (lEnum, this, mLogLevelPins).GetControllingUnknown ();
	*ppEnum = lEnum.Detach();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::EnumPins"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::FindPin (LPCWSTR Id, IPin**ppPin)
{
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%d)] %s::FindPin"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->FindPin (Id, ppPin);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::FindPin"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::QueryFilterInfo (FILTER_INFO *pInfo)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::QueryFilterInfo"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->QueryFilterInfo (pInfo);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::QueryFilterInfo"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::JoinFilterGraph (IFilterGraph*pGraph, LPCWSTR pName)
{
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%d)] %s::JoinFilterGraph [%p] [%ls]"), this, max(m_dwRef,-1), mFilterName, pGraph, pName);
#endif

	HRESULT	lResult = mInnerBaseFilter->JoinFilterGraph (pGraph, pName);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::JoinFilterGraph"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::QueryVendorInfo (LPWSTR *pVendorInfo)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::QueryVendorInfo"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerBaseFilter->QueryVendorInfo (pVendorInfo);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::QueryVendorInfo"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::GetCapabilities (DWORD *pCapabilities)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::MediaSeeking::GetCapabilities"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetCapabilities"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::CheckCapabilities (DWORD *pCapabilities)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::CheckCapabilities"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->CheckCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::CheckCapabilities"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::IsFormatSupported (const GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::IsFormatSupported"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->IsFormatSupported (pFormat);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::IsFormatSupported"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::QueryPreferredFormat (GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::QueryPreferredFormat"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->QueryPreferredFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::QueryPreferredFormat"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetTimeFormat (GUID *pFormat)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::GetTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::IsUsingTimeFormat (const GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::IsUsingTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->IsUsingTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::IsUsingTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetTimeFormat (const GUID *pFormat)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%d)] %s::SetTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->SetTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SetTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%d)] %s::ConvertTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->ConvertTimeFormat (pTarget, pTargetFormat, Source, pSourceFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::ConvertTimeFormat"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetDuration (LONGLONG *pDuration)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%d)] %s::GetDuration"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetDuration (pDuration);
#ifdef	_LOG_SEEKING
	if	(pDuration)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Duration [%f]"), this, mFilterName, RefTimeSec (*pDuration));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetDuration"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetStopPosition (LONGLONG *pStop)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetStopPosition"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetStopPosition (pStop);
#ifdef	_LOG_SEEKING
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), this, mFilterName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetStopPosition"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetCurrentPosition (LONGLONG *pCurrent)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetCurrentPosition"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetCurrentPosition (pCurrent);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), this, mFilterName, RefTimeSec (*pCurrent));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetCurrentPosition"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%d)] %s::SetPositions"), this, max(m_dwRef,-1), mFilterName);
#endif

#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f] [%s]"), this, mFilterName, RefTimeSec (*pCurrent), SeekingFlagsStr(dwCurrentFlags));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f] [%s]"), this, mFilterName, RefTimeSec (*pStop), SeekingFlagsStr(dwStopFlags));
	}
#endif

	HRESULT	lResult = mInnerMediaSeeking->SetPositions (pCurrent, dwCurrentFlags, pStop, dwStopFlags);

#ifdef	_LOG_SEEKING
	if	(
			(pCurrent)
		&&	(dwCurrentFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), this, mFilterName, RefTimeSec (*pCurrent));
	}
	if	(
			(pStop)
		&&	(dwStopFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), this, mFilterName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SetPositions"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetPositions (LONGLONG *pCurrent, LONGLONG *pStop)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetPositions"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetPositions (pCurrent, pStop);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), this, mFilterName, RefTimeSec (*pCurrent));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), this, mFilterName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetPositions"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetAvailable"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetAvailable (pEarliest, pLatest);
#ifdef	_LOG_SEEKING
	if	(pEarliest)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Earliest [%f]"), this, mFilterName, RefTimeSec (*pEarliest));
	}
	if	(pLatest)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Latest   [%f]"), this, mFilterName, RefTimeSec (*pLatest));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetAvailable"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetRate (double dRate)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%d)] %s::SetRate"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->SetRate (dRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SetRate"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetRate (double *pdRate)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetRate"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetRate (pdRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetRate"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetPreroll (LONGLONG *pllPreroll)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%d)] %s::GetPreroll"), this, max(m_dwRef,-1), mFilterName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetPreroll (pllPreroll);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%d)] %s::GetPreroll"), this, max(m_dwRef,-1), mFilterName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::SetOneShot (BOOL OneShot)
{
	return mInnerSampleGrabber->SetOneShot (OneShot);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetMediaType (const AM_MEDIA_TYPE *pType)
{
	return mInnerSampleGrabber->SetMediaType (pType);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetConnectedMediaType ( AM_MEDIA_TYPE *pType)
{
	return mInnerSampleGrabber->GetConnectedMediaType (pType);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetBufferSamples (BOOL BufferThem)
{
	return mInnerSampleGrabber->SetBufferSamples (BufferThem);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetCurrentBuffer (long *pBufferSize, long *pBuffer)
{
	return mInnerSampleGrabber->GetCurrentBuffer (pBufferSize, pBuffer);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetCurrentSample (IMediaSample**ppSample)
{
	return mInnerSampleGrabber->GetCurrentSample (ppSample);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::SetCallback (ISampleGrabberCB *pCallback, long WhichMethodToCallback)
{
	return mInnerSampleGrabber->SetCallback (pCallback, WhichMethodToCallback);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt)
{
	return mInnerFileSourceFilter->Load (pszFileName, pmt);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt)
{
	return mInnerFileSourceFilter->GetCurFile (ppszFileName, pmt);
}

ULONG STDMETHODCALLTYPE CTraceFilter::GetMiscFlags ()
{
	return mInnerAMFilterMiscFlags->GetMiscFlags ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CTracePins::CTracePins ()
:	mFilter (NULL),
	mLogLevelPins (LogVerbose|LogTime),
	mUnknown (NULL)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::CTracePins (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
}

CTracePins::~CTracePins ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::~CTracePins (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
}

CTracePins& CTracePins::Initialize (LPUNKNOWN pUnknown, IBaseFilter* pFilter, UINT pLogLevelPins)
{
	mFilter = pFilter;
	mLogLevelPins = pLogLevelPins;
	mUnknown = pUnknown;
	mInnerEnum = pUnknown;

#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::Initialize (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
	return *this;
}

void CTracePins::FinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::FinalRelease"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePins::Next (ULONG cPins, IPin**ppPins, ULONG *pcFetched)
{
	HRESULT	lResult;
	ULONG	lFetched = 0;
	ULONG	lNdx;

	lResult = mInnerEnum->Next (cPins, ppPins, &lFetched);
	if	(pcFetched)
	{
		*pcFetched = lFetched;
	}
	for	(lNdx = 0; lNdx < lFetched; lNdx++)
	{
		_ITracePinPtr	lTracePin (ppPins [lNdx]);
		IPinPtr			lPin;

		if	(lTracePin == NULL)
		{
			lPin = (new CComObject<CTracePin>)->Initialize (ppPins [lNdx], mFilter, mLogLevelPins).GetControllingUnknown ();
			ppPins [lNdx] = lPin.Detach();
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePins::Skip (ULONG cPins)
{
	return mInnerEnum->Skip (cPins);
}

HRESULT STDMETHODCALLTYPE CTracePins::Reset ()
{
	return mInnerEnum->Reset ();
}

HRESULT STDMETHODCALLTYPE CTracePins::Clone (IEnumPins **ppEnum)
{
	HRESULT			lResult;
	IEnumPinsPtr	lClone;

	if	(SUCCEEDED (lResult = mInnerEnum->Clone (&lClone)))
	{
		(*ppEnum) = lClone = (new CComObject<CTracePins>)->Initialize (lClone, mFilter).GetControllingUnknown();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CTracePin::CTracePin ()
:	mFilter (NULL),
	mLogLevel (LogVerbose|LogTime),
	mUnknown (NULL)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::CTracePin (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
}

CTracePin::~CTracePin ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::~CTracePin (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
}

CTracePin& CTracePin::Initialize (LPUNKNOWN pUnknown, IBaseFilter* pFilter, UINT pLogLevel)
{
	mFilter = pFilter;
	mLogLevel = pLogLevel;
	mUnknown = pUnknown;
	mInnerPin = pUnknown;
	mInnerMemInputPin = pUnknown;
	mInnerAsyncReader = pUnknown;
	mInnerMediaSeeking = pUnknown;
	mInnerPinConnection = pUnknown;
	mInnerPinFlowControl = pUnknown;

	ATLASSERT(mInnerPin != NULL);
	mName = PinIdStr (mInnerPin);
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::Initialize (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
#endif
	return *this;
}

void CTracePin::FinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::FinalRelease"), this);
#endif
}

HRESULT WINAPI CTracePin::CheckInternalInterfaces (void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw)
{
	HRESULT		lResult = S_FALSE;
	CTracePin *	lThis = (CTracePin *) pv;

	if	(
			(IsEqualGUID (riid, __uuidof(IMemInputPin)))
		&&	(lThis->mInnerMemInputPin == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(IAsyncReader)))
		&&	(lThis->mInnerAsyncReader == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(IMediaSeeking)))
		&&	(lThis->mInnerMediaSeeking == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(IPinConnection)))
		&&	(lThis->mInnerPinConnection == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	else
	if	(
			(IsEqualGUID (riid, __uuidof(IPinFlowControl)))
		&&	(lThis->mInnerPinFlowControl == NULL)
		)
	{
		lResult = E_NOINTERFACE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::Connect [%p] [%p]"), this, max(m_dwRef,-1), mName, pReceivePin, pmt);
#endif

	HRESULT			lResult;
	_ITracePinPtr	lTracePin (pReceivePin);
	IPinPtr			lReceivePin;
	UINT			lLogLevel;

	if	(lTracePin == NULL)
	{
		lLogLevel = ((mLogLevel & 0xFF) <= LogIfActive) ? mLogLevel : LogVerbose|LogTime;
		lReceivePin = (new CComObject<CTracePin>)->Initialize (pReceivePin, NULL, lLogLevel).GetControllingUnknown();
		pReceivePin = lReceivePin.Detach ();
	}
	lResult = mInnerPin->Connect (pReceivePin, pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::Connect"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::ReceiveConnection"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT			lResult;
	_ITracePinPtr	lTracePin (pConnector);
	IPinPtr			lConnector;
	UINT			lLogLevel;

	if	(lTracePin == NULL)
	{
		lLogLevel = ((mLogLevel & 0xFF) <= LogIfActive) ? mLogLevel : LogVerbose|LogTime;
		lConnector = (new CComObject<CTracePin>)->Initialize (pConnector, NULL, lLogLevel).GetControllingUnknown();
		pConnector = lConnector.Detach ();
	}
	lResult = mInnerPin->ReceiveConnection (pConnector, pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::ReceiveConnection"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::Disconnect (void)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::Disconnect"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->Disconnect ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::Disconnect"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::ConnectedTo (IPin**pPin)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::ConnectedTo"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT			lResult;
	_ITracePinPtr	lTracePin;
	IPinPtr			lPin;
	UINT			lLogLevel;

	lResult = mInnerPin->ConnectedTo (pPin);
	if	(
			(pPin)
		&&	(*pPin)
		)
	{
		lTracePin = *pPin;
		if	(lTracePin == NULL)
		{
			lLogLevel = ((mLogLevel & 0xFF) <= LogIfActive) ? mLogLevel : LogVerbose|LogTime;
			lPin = (new CComObject<CTracePin>)->Initialize (*pPin, NULL, lLogLevel).GetControllingUnknown();
			*pPin = lPin.Detach ();
		}
	}

#ifdef	_LOG_RESULTS
	if	(lResult != VFW_E_NOT_CONNECTED)
	{
		LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::ConnectedTo"), this, max(m_dwRef,-1), mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::ConnectionMediaType (AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::ConnectionMediaType"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->ConnectionMediaType (pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::ConnectionMediaType"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::QueryPinInfo (PIN_INFO *pInfo)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::QueryPinInfo"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->QueryPinInfo (pInfo);

	if	(
			(pInfo)
		&&	(mFilter)
		)
	{
		if	(pInfo->pFilter)
		{
			pInfo->pFilter->Release();
		}
		pInfo->pFilter = mFilter;
		pInfo->pFilter->AddRef ();
	}

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::QueryPinInfo"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::QueryDirection (PIN_DIRECTION *pPinDir)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::QueryDirection"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->QueryDirection (pPinDir);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::QueryDirection"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::QueryId (LPWSTR *Id)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::QueryId"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->QueryId (Id);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::QueryId"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::QueryAccept (const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::QueryAccept"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->QueryAccept (pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::QueryAccept"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::EnumMediaTypes (IEnumMediaTypes **ppEnum)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::EnumMediaTypes"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->EnumMediaTypes (ppEnum);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::EnumMediaTypes"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::QueryInternalConnections (IPin**apPin, ULONG *nPin)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::QueryInternalConnections"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->QueryInternalConnections (apPin, nPin);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), lResult, _T("[%p(%d)] %s::QueryInternalConnections"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::NewSegment (REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::NewSegment [%f - %f] [%f]"), this, max(m_dwRef,-1), mName, RefTimeSec(tStart), RefTimeSec(tStop), dRate);
#endif

	HRESULT	lResult = mInnerPin->NewSegment (tStart, tStop, dRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::NewSegment"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::EndOfStream (void)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::EndOfStream"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->EndOfStream ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::EndOfStream"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::BeginFlush (void)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::BeginFlush"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->BeginFlush ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::BeginFlush"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::EndFlush (void)
{
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,mLogLevel), _T("[%p(%d)] %s::EndFlush"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPin->EndFlush ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::EndFlush"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::GetAllocator (IMemAllocator**ppAllocator)
{
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), _T("[%p(%d)] %s::GetAllocator"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMemInputPin->GetAllocator (ppAllocator);
#ifdef	_LOG_ALLOCATOR
	if	(*ppAllocator)
	{
		LogMemAllocator (_LOG_ALLOCATOR, *ppAllocator);
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetAllocator"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::NotifyAllocator (IMemAllocator*pAllocator, BOOL bReadOnly)
{
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), _T("[%p(%d)] %s::NotifyAllocator [%p] ReadOnly [%d]"), this, max(m_dwRef,-1), mName, pAllocator, bReadOnly);
#endif

#ifdef	_LOG_ALLOCATOR
	if	(pAllocator)
	{
		LogMemAllocator (_LOG_ALLOCATOR, pAllocator);
	}
#endif
	HRESULT	lResult = mInnerMemInputPin->NotifyAllocator (pAllocator, bReadOnly);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::NotifyAllocator"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps)
{
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), _T("[%p(%d)] %s::GetAllocatorRequirements"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMemInputPin->GetAllocatorRequirements (pProps);
#ifdef	_LOG_ALLOCATOR
	if	(
			(SUCCEEDED (lResult))
		&&	(pProps)
		)
	{
		LogAllocatorProps (_LOG_ALLOCATOR, *pProps);
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetAllocatorRequirements"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::Receive (IMediaSample*pSample)
{
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), _T("[%p(%d)] %s::Receive"), this, max(m_dwRef,-1), mName);
	//LogMediaSampleId (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), pSample, _T("[%p(%d)] %s"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMemInputPin->Receive (pSample);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::Receive"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::ReceiveMultiple (IMediaSample**pSamples, long nSamples, long *nSamplesProcessed)
{
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), _T("[%p(%d)] %s::ReceiveMultiple"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMemInputPin->ReceiveMultiple (pSamples, nSamples, nSamplesProcessed);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::ReceiveMultiple"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::ReceiveCanBlock (void)
{
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,mLogLevel), _T("[%p(%d)] %s::ReceiveCanBlock"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMemInputPin->ReceiveCanBlock ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::ReceiveCanBlock"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::RequestAllocator (IMemAllocator*pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator**ppActual)
{
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,mLogLevel), _T("[%p(%d)] %s::RequestAllocator"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerAsyncReader->RequestAllocator (pPreferred, pProps, ppActual);
#ifdef	_LOG_ALLOCATOR
	if	(pPreferred)
	{
		LogMemAllocator (_LOG_ALLOCATOR, pPreferred, _T("Preferred"));
	}
	if	(pProps)
	{
		LogAllocatorProps (_LOG_ALLOCATOR, *pProps, _T("Request"));
	}
	if	(
			(ppActual)
		&&	(*ppActual)
		)
	{
		LogMemAllocator (_LOG_ALLOCATOR, *ppActual, _T("Actual"));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::RequestAllocator"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::Request (IMediaSample*pSample, DWORD_PTR dwUser)
{
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,mLogLevel), _T("[%p(%d)] %s::Request"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerAsyncReader->Request (pSample, dwUser);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::Request"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::WaitForNext (DWORD dwTimeout, IMediaSample**ppSample, DWORD_PTR *pdwUser)
{
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,mLogLevel), _T("[%p(%d)] %s::WaitForNext"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerAsyncReader->WaitForNext (dwTimeout, ppSample, pdwUser);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::WaitForNext"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::SyncReadAligned (IMediaSample*pSample)
{
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,mLogLevel), _T("[%p(%d)] %s::SyncReadAligned"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerAsyncReader->SyncReadAligned (pSample);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::SyncReadAligned"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::SyncRead (LONGLONG llPosition, LONG lLength, BYTE*pBuffer)
{
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,mLogLevel), _T("[%p(%d)] %s::SyncRead"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerAsyncReader->SyncRead (llPosition, lLength, pBuffer);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::SyncRead"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::Length (LONGLONG *pTotal, LONGLONG *pAvailable)
{
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,mLogLevel), _T("[%p(%d)] %s::Length"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerAsyncReader->Length (pTotal, pAvailable);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::Length"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::GetCapabilities (DWORD *pCapabilities)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::GetCapabilities"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetCapabilities"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::CheckCapabilities (DWORD *pCapabilities)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::CheckCapabilities"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->CheckCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::CheckCapabilities"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::IsFormatSupported (const GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::IsFormatSupported"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->IsFormatSupported (pFormat);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), lResult, _T("[%p(%d)] %s::IsFormatSupported"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::QueryPreferredFormat (GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::QueryPreferredFormat"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->QueryPreferredFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::QueryPreferredFormat"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetTimeFormat (GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::GetTimeFormat"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetTimeFormat   (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetTimeFormat"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::IsUsingTimeFormat (const GUID *pFormat)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::IsUsingTimeFormat"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->IsUsingTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::IsUsingTimeFormat"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::SetTimeFormat (const GUID *pFormat)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::SetTimeFormat"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->SetTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::SetTimeFormat"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetDuration (LONGLONG *pDuration)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::GetDuration"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetDuration (pDuration);
#ifdef	_LOG_SEEKING
	if	(pDuration)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Duration [%f]"), this, mName, RefTimeSec (*pDuration));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetDuration"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetStopPosition (LONGLONG *pStop)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::GetStopPosition"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetStopPosition (pStop);
#ifdef	_LOG_SEEKING
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), this, mName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetStopPosition"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetCurrentPosition (LONGLONG *pCurrent)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::GetCurrentPosition"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetCurrentPosition (pCurrent);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), this, mName, RefTimeSec (*pCurrent));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetCurrentPosition"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::ConvertTimeFormat"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->ConvertTimeFormat (pTarget, pTargetFormat, Source, pSourceFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::ConvertTimeFormat"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::SetPositions"), this, max(m_dwRef,-1), mName);
#endif

#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f] [%s]"), this, mName, RefTimeSec (*pCurrent), SeekingFlagsStr(dwCurrentFlags));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f] [%s]"), this, mName, RefTimeSec (*pStop), SeekingFlagsStr(dwStopFlags));
	}
#endif

	HRESULT	lResult = mInnerMediaSeeking->SetPositions (pCurrent, dwCurrentFlags, pStop, dwStopFlags);

#ifdef	_LOG_SEEKING
	if	(
			(pCurrent)
		&&	(dwCurrentFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), this, mName, RefTimeSec (*pCurrent));
	}
	if	(
			(pStop)
		&&	(dwStopFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), this, mName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::SetPositions"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetPositions (LONGLONG *pCurrent, LONGLONG *pStop)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::GetPositions"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetPositions (pCurrent, pStop);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), this, mName, RefTimeSec (*pCurrent));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), this, mName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetPositions"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::GetAvailable"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetAvailable (pEarliest, pLatest);
#ifdef	_LOG_SEEKING
	if	(pEarliest)
	{
		LogMessage (_LOG_SEEKING, _T("[%s] [%p] Earliest [%f]"), this, mName, RefTimeSec (*pEarliest));
	}
	if	(pLatest)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Latest   [%f]"), this, mName, RefTimeSec (*pLatest));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetAvailable"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::SetRate (double dRate)
{
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,mLogLevel), _T("[%p(%d)] %s::SetRate"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->SetRate (dRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::SetRate"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetRate (double *pdRate)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::GetRate"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetRate (pdRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::GetRate"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::GetPreroll (LONGLONG *pllPreroll)
{
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), _T("[%p(%d)] %s::GetPreroll"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerMediaSeeking->GetPreroll (pllPreroll);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (MaxLogLevel (_DEBUG_QUERIES,mLogLevel), lResult, _T("[%p(%d)] %s::GetPreroll"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::DynamicQueryAccept (const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,mLogLevel), _T("[%p(%d)] %s::DynamicQueryAccept"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPinConnection->DynamicQueryAccept (pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::DynamicQueryAccept"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::NotifyEndOfStream (HANDLE hNotifyEvent)
{
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,mLogLevel), _T("[%p(%d)] %s::NotifyEndOfStream"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPinConnection->NotifyEndOfStream (hNotifyEvent);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::NotifyEndOfStream"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::IsEndPin (void)
{
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,mLogLevel), _T("[%p(%d)] %s::IsEndPin"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPinConnection->IsEndPin ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::IsEndPin"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::DynamicDisconnect (void)
{
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,mLogLevel), _T("[%p(%d)] %s::DynamicDisconnect"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPinConnection->DynamicDisconnect ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::DynamicDisconnect"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::Block (DWORD dwBlockFlags, HANDLE hEvent)
{
#ifdef	_DEBUG_PINFLOWCONTROL
	LogMessage (MaxLogLevel (_DEBUG_PINFLOWCONTROL,mLogLevel), _T("[%p(%d)] %s::Block"), this, max(m_dwRef,-1), mName);
#endif

	HRESULT	lResult = mInnerPinFlowControl->Block (dwBlockFlags, hEvent);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,mLogLevel), lResult, _T("[%p(%d)] %s::Block"), this, max(m_dwRef,-1), mName);
#endif
	return lResult;
}

