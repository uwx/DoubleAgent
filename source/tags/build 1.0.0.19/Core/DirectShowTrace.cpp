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
#include "DirectShowTrace.h"
#include "DirectShowEnums.h"
#include "GuidStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_COM			LogNormal
//#define	_DEBUG_COM_QI		LogNormal
//#define	_DEBUG_QUERIES		LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_BASEFILTER		LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_MEDIASEEKING		LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_PIN				LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_MEMINPUTPIN		LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_ASYNCREADER		LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_PINCONNECTION	LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_PINFLOWCONTROL	LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_RESULTS			LogNormal|LogHighVolume|LogTimeMs

//#define	_LOG_ALLOCATOR		LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_SAMPLES			LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_SEEKING			LogNormal|LogHighVolume|LogTimeMs
//#define	_TRACE_SAMPLE_TRACER	LogIfActive

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CTraceSamples, CCmdTarget)
	INTERFACE_PART(CTraceSamples, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CTraceSamples, __uuidof(ISampleGrabberCB), Trace)
END_INTERFACE_MAP()

BEGIN_INTERFACE_MAP(CTraceFilter, CCmdTarget)
	INTERFACE_PART(CTraceFilter, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CTraceFilter, __uuidof(_ITraceFilter), _TraceFilter)
	INTERFACE_PART(CTraceFilter, __uuidof(IPersist), BaseFilter)
	INTERFACE_PART(CTraceFilter, __uuidof(IBaseFilter), BaseFilter)
	INTERFACE_PART(CTraceFilter, __uuidof(IMediaFilter), BaseFilter)
	INTERFACE_AGGREGATE (CTraceFilter, mUnknown)
END_INTERFACE_MAP()

BEGIN_INTERFACE_MAP(CTracePins, CCmdTarget)
	INTERFACE_PART(CTracePins, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CTracePins, __uuidof(_ITracePins), _TracePins)
	INTERFACE_PART(CTracePins, __uuidof(IEnumPins), Enum)
	INTERFACE_AGGREGATE (CTracePins, mUnknown)
END_INTERFACE_MAP()

BEGIN_INTERFACE_MAP(CTracePin, CCmdTarget)
	INTERFACE_PART(CTracePin, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CTracePin, __uuidof(_ITracePin), _TracePin)
	INTERFACE_PART(CTracePin, __uuidof(IPin), Pin)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CTraceSamples, Trace)
IMPLEMENT_IUNKNOWN(CTraceFilter, _TraceFilter)
IMPLEMENT_IUNKNOWN(CTraceFilter, BaseFilter)
IMPLEMENT_IUNKNOWN(CTraceFilter, MediaSeeking)
IMPLEMENT_IUNKNOWN(CTraceFilter, SampleGrabber)
IMPLEMENT_IUNKNOWN(CTraceFilter, FileSourceFilter)
IMPLEMENT_IUNKNOWN(CTraceFilter, AMFilterMiscFlags)
IMPLEMENT_IUNKNOWN(CTracePins, Enum)
IMPLEMENT_IUNKNOWN(CTracePins, _TracePins)
IMPLEMENT_IUNKNOWN(CTracePin, Pin)
IMPLEMENT_IUNKNOWN(CTracePin, _TracePin)
IMPLEMENT_IUNKNOWN(CTracePin, MemInputPin)
IMPLEMENT_IUNKNOWN(CTracePin, AsyncReader)
IMPLEMENT_IUNKNOWN(CTracePin, MediaSeeking)
IMPLEMENT_IUNKNOWN(CTracePin, PinConnection)
IMPLEMENT_IUNKNOWN(CTracePin, PinFlowControl)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTraceSamples, CCmdTarget)

CTraceSamples::CTraceSamples (LPCTSTR pFilterName)
:	mFilterName (pFilterName)
{
	if	(mFilterName.IsEmpty())
	{
		mFilterName = _T("TraceSamples");
	}
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::CTraceSamples (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleLockApp ();

	EnableAggregation ();
	m_dwRef = 0;

#ifdef	_TRACE_SAMPLE_TRACER
	// Does not work yet
	if	(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (__uuidof(SampleGrabber), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mGrabberFilter))))
	{
		mGrabberFilter = (new CTraceFilter(mGrabberFilter, _T("Grabber"), _TRACE_SAMPLE_TRACER))->GetControllingUnknown();
	}
#endif
}

CTraceSamples::~CTraceSamples( )
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::~CTraceSamples (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	Disconnect ();
	AfxOleUnlockApp ();
}

void CTraceSamples::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceSamples::OnFinalRelease"), this);
#endif
	CCmdTarget::OnFinalRelease ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CTraceSamples & CTraceSamples::ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin, AM_MEDIA_TYPE * pMediaType)
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
			&&	(SUCCEEDED (lResult = LogComErr (LogNormal, lDownstreamPin->ConnectedTo (&lUpstreamPin))))
			&&	(lUpstreamPin != NULL)
			)
		{
			if	(
					(
						(mGrabberFilter != NULL)
					||	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(SampleGrabber), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mGrabberFilter))))
					)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pFilterGraph->AddFilter (mGrabberFilter, mFilterName))))
				)
			{
				mGrabber = mGrabberFilter;
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGrabber->SetMediaType (pMediaType))))
				&&	(SUCCEEDED (lResult = GetFilterPins (mGrabberFilter, &lInputPin, &lOutputPin)))
				&&	(lInputPin != NULL)
				&&	(lOutputPin != NULL)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, lDownstreamPin->Disconnect ())))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, lUpstreamPin->Disconnect ())))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pFilterGraph->ConnectDirect (lUpstreamPin, lInputPin, pMediaType))))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pFilterGraph->ConnectDirect (lOutputPin, lDownstreamPin, pMediaType))))
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

CTraceSamples & CTraceSamples::ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin, AM_MEDIA_TYPE * pMediaType)
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

CTraceSamples & CTraceSamples::Disconnect ()
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
			lResult = LogVfwErr (LogNormal, lFilterInfo.pGraph->RemoveFilter (lFilter));
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

CTraceSamples & CTraceSamples::LogSamples (UINT pLogLevel, bool pOneShot)
{
	try
	{
		HRESULT	lResult = E_FAIL;

		mLogLevelSample = pLogLevel;
		if	(
				(mGrabber != NULL)
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGrabber->SetCallback (&m_xTrace, 0))))
			)
		{
			LogVfwErr (LogNormal, mGrabber->SetOneShot (pOneShot!=false));
		}
	}
	catch AnyExceptionDebug

	return *this;
}

CTraceSamples & CTraceSamples::DumpSamples (UINT pByteCount, UINT pLogLevel, bool pOneShot)
{
	try
	{
		HRESULT	lResult = E_FAIL;

		mLogLevelDump = pLogLevel;
		mDumpSize = pByteCount;

		if	(
				(mGrabber != NULL)
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGrabber->SetCallback (&m_xTrace, 1))))
			)
		{
			LogVfwErr (LogNormal, mGrabber->SetOneShot (pOneShot!=false));
		}
	}
	catch AnyExceptionDebug

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceSamples::XTrace::SampleCB (double SampleTime, IMediaSample *pSample)
{
	METHOD_PROLOGUE(CTraceSamples, Trace)
	try
	{
		LogMediaSample (pThis->mLogLevelSample, pSample, _T("[%s] At [%f]"), pThis->mFilterName, SampleTime);
	}
	catch AnyExceptionSilent
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CTraceSamples::XTrace::BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen)
{
	METHOD_PROLOGUE(CTraceSamples, Trace)
	try
	{
		LogMessage (pThis->mLogLevelDump, _T("[%s] At [%f] of [%d]"), pThis->mFilterName, SampleTime, BufferLen);
		LogDump (pThis->mLogLevelDump, pBuffer, min (BufferLen, pThis->mDumpSize), _T("  "));
	}
	catch AnyExceptionSilent
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CTraceFilter, CCmdTarget)

CTraceFilter::CTraceFilter (LPUNKNOWN pUnknown, LPCTSTR pFilterName, UINT pLogLevelPins)
:	mFilterName (pFilterName),
	mLogLevelPins (pLogLevelPins)
{
	Aggregate (pUnknown);
	if	(mFilterName.IsEmpty())
	{
		mFilterName = _T("TraceFilter");
	}
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::CTraceFilter (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleLockApp ();

	EnableAggregation ();
	EnableAutomation ();
	//m_dwRef = 0;
}

CTraceFilter::~CTraceFilter ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::~CTraceFilter (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleUnlockApp ();
}

void CTraceFilter::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTraceFilter::OnFinalRelease"), this);
#endif
	CCmdTarget::OnFinalRelease ();
}

LPUNKNOWN CTraceFilter::GetInterfaceHook(const void* iid)
{
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] %s::QueryInterface [%s]"), this, m_dwRef, mFilterName, CGuidStr::GuidName(*(GUID*)iid));
#endif
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IMediaSeeking)))
		&&	(mInnerMediaSeeking != NULL)
		)
	{
		return &m_xMediaSeeking;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(ISampleGrabber)))
		&&	(mInnerSampleGrabber != NULL)
		)
	{
		return &m_xSampleGrabber;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IFileSourceFilter)))
		&&	(mInnerFileSourceFilter != NULL)
		)
	{
		return &m_xFileSourceFilter;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IAMFilterMiscFlags)))
		&&	(mInnerAMFilterMiscFlags != NULL)
		)
	{
		return &m_xAMFilterMiscFlags;
	}
	return NULL;
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

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::GetClassID (CLSID *pClassID)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XBaseFilter::GetClassID"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT lResult = pThis->mInnerBaseFilter->GetClassID (pClassID);

#ifdef	_LOG_RESULTS
	LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::GetClassID"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::Stop (void)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%u)] %s::XBaseFilter::Stop"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->Stop ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::Stop"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::Pause (void)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%u)] %s::XBaseFilter::Pause"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->Pause ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::Pause"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::Run (REFERENCE_TIME tStart)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%u)] %s::XBaseFilter::Run [%f]"), pThis, pThis->m_dwRef, pThis->mFilterName, RefTimeSec(tStart));
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->Run (tStart);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::Run"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::GetState (DWORD dwMilliSecsTimeout, FILTER_STATE *State)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XBaseFilter::GetState"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->GetState (dwMilliSecsTimeout, State);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::GetState"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::SetSyncSource (IReferenceClock *pClock)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%u)] %s::XBaseFilter::SetSyncSource [%p]"), pThis, pThis->m_dwRef, pThis->mFilterName, pClock);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->SetSyncSource (pClock);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::SetSyncSource"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::GetSyncSource (IReferenceClock **pClock)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XBaseFilter::GetSyncSource"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->GetSyncSource (pClock);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::GetSyncSource"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::EnumPins (IEnumPins **ppEnum)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XBaseFilter::EnumPins"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT			lResult;
	IEnumPinsPtr	lEnum;

	lResult = pThis->mInnerBaseFilter->EnumPins (&lEnum);
	lEnum = (new CTracePins (lEnum, this, pThis->mLogLevelPins))->GetControllingUnknown ();
	*ppEnum = lEnum.Detach();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::EnumPins"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::FindPin (LPCWSTR Id, IPin **ppPin)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%u)] %s::XBaseFilter::FindPin"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->FindPin (Id, ppPin);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::FindPin"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::QueryFilterInfo (FILTER_INFO *pInfo)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XBaseFilter::QueryFilterInfo"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->QueryFilterInfo (pInfo);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::QueryFilterInfo"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::JoinFilterGraph (IFilterGraph *pGraph, LPCWSTR pName)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_BASEFILTER
	LogMessage (_DEBUG_BASEFILTER, _T("[%p(%u)] %s::XBaseFilter::JoinFilterGraph [%p] [%ls]"), pThis, pThis->m_dwRef, pThis->mFilterName, pGraph, pName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->JoinFilterGraph (pGraph, pName);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XBaseFilter::JoinFilterGraph"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XBaseFilter::QueryVendorInfo (LPWSTR *pVendorInfo)
{
	METHOD_PROLOGUE(CTraceFilter, BaseFilter)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XBaseFilter::QueryVendorInfo"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerBaseFilter->QueryVendorInfo (pVendorInfo);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%u)] %s::XBaseFilter::QueryVendorInfo"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetCapabilities (DWORD *pCapabilities)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::MediaSeeking::GetCapabilities"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetCapabilities"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::CheckCapabilities (DWORD *pCapabilities)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::CheckCapabilities"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->CheckCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::CheckCapabilities"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::IsFormatSupported (const GUID *pFormat)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::IsFormatSupported"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->IsFormatSupported (pFormat);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%u)] %s::XMediaSeeking::IsFormatSupported"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::QueryPreferredFormat (GUID *pFormat)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::QueryPreferredFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->QueryPreferredFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::QueryPreferredFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetTimeFormat (GUID *pFormat)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%u)] %s::XMediaSeeking::GetTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::IsUsingTimeFormat (const GUID *pFormat)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::IsUsingTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->IsUsingTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::IsUsingTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::SetTimeFormat (const GUID *pFormat)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%u)] %s::XMediaSeeking::SetTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->SetTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::SetTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%u)] %s::XMediaSeeking::ConvertTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->ConvertTimeFormat (pTarget, pTargetFormat, Source, pSourceFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::ConvertTimeFormat"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetDuration (LONGLONG *pDuration)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%u)] %s::XMediaSeeking::GetDuration"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetDuration (pDuration);
#ifdef	_LOG_SEEKING
	if	(pDuration)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Duration [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pDuration));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetDuration"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetStopPosition (LONGLONG *pStop)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::GetStopPosition"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetStopPosition (pStop);
#ifdef	_LOG_SEEKING
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetStopPosition"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetCurrentPosition (LONGLONG *pCurrent)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::GetCurrentPosition"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetCurrentPosition (pCurrent);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pCurrent));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetCurrentPosition"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%u)] %s::XMediaSeeking::SetPositions"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f] [%s]"), pThis, pThis->mFilterName, RefTimeSec (*pCurrent), SeekingFlagsStr(dwCurrentFlags));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f] [%s]"), pThis, pThis->mFilterName, RefTimeSec (*pStop), SeekingFlagsStr(dwStopFlags));
	}
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->SetPositions (pCurrent, dwCurrentFlags, pStop, dwStopFlags);

#ifdef	_LOG_SEEKING
	if	(
			(pCurrent)
		&&	(dwCurrentFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pCurrent));
	}
	if	(
			(pStop)
		&&	(dwStopFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::SetPositions"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetPositions (LONGLONG *pCurrent, LONGLONG *pStop)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::GetPositions"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetPositions (pCurrent, pStop);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pCurrent));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetPositions"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::GetAvailable"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetAvailable (pEarliest, pLatest);
#ifdef	_LOG_SEEKING
	if	(pEarliest)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Earliest [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pEarliest));
	}
	if	(pLatest)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Latest   [%f]"), pThis, pThis->mFilterName, RefTimeSec (*pLatest));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetAvailable"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::SetRate (double dRate)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (_DEBUG_MEDIASEEKING, _T("[%p(%u)] %s::XMediaSeeking::SetRate"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->SetRate (dRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::SetRate"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetRate (double *pdRate)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::GetRate"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetRate (pdRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetRate"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XMediaSeeking::GetPreroll (LONGLONG *pllPreroll)
{
	METHOD_PROLOGUE(CTraceFilter, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (_DEBUG_QUERIES, _T("[%p(%u)] %s::XMediaSeeking::GetPreroll"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetPreroll (pllPreroll);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (_DEBUG_QUERIES, lResult, _T("[%p(%u)] %s::XMediaSeeking::GetPreroll"), pThis, pThis->m_dwRef, pThis->mFilterName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::SetOneShot (BOOL OneShot)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->SetOneShot (OneShot);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::SetMediaType (const AM_MEDIA_TYPE *pType)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->SetMediaType (pType);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::GetConnectedMediaType ( AM_MEDIA_TYPE *pType)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->GetConnectedMediaType (pType);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::SetBufferSamples (BOOL BufferThem)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->SetBufferSamples (BufferThem);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::GetCurrentBuffer (long *pBufferSize, long *pBuffer)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->GetCurrentBuffer (pBufferSize, pBuffer);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::GetCurrentSample (IMediaSample **ppSample)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->GetCurrentSample (ppSample);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XSampleGrabber::SetCallback (ISampleGrabberCB *pCallback, long WhichMethodToCallback)
{
	METHOD_PROLOGUE(CTraceFilter, SampleGrabber)
	return pThis->mInnerSampleGrabber->SetCallback (pCallback, WhichMethodToCallback);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTraceFilter::XFileSourceFilter::Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTraceFilter, FileSourceFilter)
	return pThis->mInnerFileSourceFilter->Load (pszFileName, pmt);
}

HRESULT STDMETHODCALLTYPE CTraceFilter::XFileSourceFilter::GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTraceFilter, FileSourceFilter)
	return pThis->mInnerFileSourceFilter->GetCurFile (ppszFileName, pmt);
}

ULONG STDMETHODCALLTYPE CTraceFilter::XAMFilterMiscFlags::GetMiscFlags ()
{
	METHOD_PROLOGUE(CTraceFilter, AMFilterMiscFlags)
	return pThis->mInnerAMFilterMiscFlags->GetMiscFlags ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTracePins, CCmdTarget)

CTracePins::CTracePins (LPUNKNOWN pUnknown, IBaseFilter * pFilter, UINT pLogLevelPins)
:	mFilter (pFilter),
	mLogLevelPins (pLogLevelPins),
	mUnknown (pUnknown),
	mInnerEnum (pUnknown)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::CTracePins (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleLockApp ();

	EnableAggregation();
	//m_dwRef = 0;
}

CTracePins::~CTracePins ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::~CTracePins (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleUnlockApp ();
}

void CTracePins::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CTracePins::OnFinalRelease"), this);
#endif
	CCmdTarget::OnFinalRelease ();
}

LPUNKNOWN CTracePins::GetInterfaceHook(const void* iid)
{
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CTracePins::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePins::XEnum::Next (ULONG cPins, IPin **ppPins, ULONG *pcFetched)
{
	METHOD_PROLOGUE(CTracePins, Enum)

	HRESULT	lResult;
	ULONG	lFetched = 0;
	ULONG	lNdx;

	lResult = pThis->mInnerEnum->Next (cPins, ppPins, &lFetched);
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
			lPin = (new CTracePin (ppPins [lNdx], pThis->mFilter, pThis->mLogLevelPins))->GetControllingUnknown ();
			ppPins [lNdx] = lPin.Detach();
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePins::XEnum::Skip (ULONG cPins)
{
	METHOD_PROLOGUE(CTracePins, Enum)
	return pThis->mInnerEnum->Skip (cPins);
}

HRESULT STDMETHODCALLTYPE CTracePins::XEnum::Reset ()
{
	METHOD_PROLOGUE(CTracePins, Enum)
	return pThis->mInnerEnum->Reset ();
}

HRESULT STDMETHODCALLTYPE CTracePins::XEnum::Clone (IEnumPins **ppEnum)
{
	METHOD_PROLOGUE(CTracePins, Enum)

	HRESULT			lResult;
	IEnumPinsPtr	lClone;

	if	(SUCCEEDED (lResult = pThis->mInnerEnum->Clone (&lClone)))
	{
		(*ppEnum) = lClone = (new CTracePins (lClone, pThis->mFilter))->GetControllingUnknown();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTracePin, CCmdTarget)

CTracePin::CTracePin (LPUNKNOWN pUnknown, IBaseFilter * pFilter, UINT pLogLevel)
:	mFilter (pFilter),
	mLogLevel (pLogLevel),
	mUnknown (pUnknown),
	mInnerPin (pUnknown),
	mInnerMemInputPin (pUnknown),
	mInnerAsyncReader (pUnknown),
	mInnerMediaSeeking (pUnknown),
	mInnerPinConnection (pUnknown),
	mInnerPinFlowControl (pUnknown)
{
	ASSERT(mInnerPin != NULL);
	mName = PinIdStr (mInnerPin);
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::CTracePin (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleLockApp ();

	EnableAggregation();
	//m_dwRef = 0;
}

CTracePin::~CTracePin ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::~CTracePin (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	AfxOleUnlockApp ();
}

void CTracePin::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (MaxLogLevel (_DEBUG_INSTANCE,mLogLevel), _T("[%p] CTracePin::OnFinalRelease"), this);
#endif
	CCmdTarget::OnFinalRelease ();
}

LPUNKNOWN CTracePin::GetInterfaceHook(const void* iid)
{
#ifdef	_DEBUG_COM
	LogMessage (MaxLogLevel (_DEBUG_COM,mLogLevel), _T("[%p(%u)] CTracePin::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IMemInputPin)))
		&&	(mInnerMemInputPin != NULL)
		)
	{
		return &m_xMemInputPin;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IAsyncReader)))
		&&	(mInnerAsyncReader != NULL)
		)
	{
		return &m_xAsyncReader;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IMediaSeeking)))
		&&	(mInnerMediaSeeking != NULL)
		)
	{
		return &m_xMediaSeeking;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IPinConnection)))
		&&	(mInnerPinConnection != NULL)
		)
	{
		return &m_xPinConnection;
	}
	else
	if	(
			(IsEqualGUID (*(GUID*)iid, __uuidof(IPinFlowControl)))
		&&	(mInnerPinFlowControl != NULL)
		)
	{
		return &m_xPinFlowControl;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPin::Connect (IPin *pReceivePin, const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::Connect [%p] [%p]"), pThis, pThis->m_dwRef, pThis->mName, pReceivePin, pmt);
#endif

	HRESULT			lResult;
	_ITracePinPtr	lTracePin (pReceivePin);
	IPinPtr			lReceivePin;
	UINT			lLogLevel;

	if	(lTracePin == NULL)
	{
		lLogLevel = ((pThis->mLogLevel & 0xFF) <= LogIfActive) ? pThis->mLogLevel : LogVerbose;
		lReceivePin = (new CTracePin (pReceivePin, NULL, lLogLevel))->GetControllingUnknown();
		pReceivePin = lReceivePin.Detach ();
	}
	lResult = pThis->mInnerPin->Connect (pReceivePin, pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::Connect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::ReceiveConnection"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT			lResult;
	_ITracePinPtr	lTracePin (pConnector);
	IPinPtr			lConnector;
	UINT			lLogLevel;

	if	(lTracePin == NULL)
	{
		lLogLevel = ((pThis->mLogLevel & 0xFF) <= LogIfActive) ? pThis->mLogLevel : LogVerbose;
		lConnector = (new CTracePin (pConnector, NULL, lLogLevel))->GetControllingUnknown();
		pConnector = lConnector.Detach ();
	}
	lResult = pThis->mInnerPin->ReceiveConnection (pConnector, pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::ReceiveConnection"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::Disconnect (void)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::Disconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->Disconnect ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::Disconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPin::ConnectedTo (IPin **pPin)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::ConnectedTo"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT			lResult;
	_ITracePinPtr	lTracePin;
	IPinPtr			lPin;
	UINT			lLogLevel;

	lResult = pThis->mInnerPin->ConnectedTo (pPin);
	if	(
			(pPin)
		&&	(*pPin)
		)
	{
		lTracePin = *pPin;
		if	(lTracePin == NULL)
		{
			lLogLevel = ((pThis->mLogLevel & 0xFF) <= LogIfActive) ? pThis->mLogLevel : LogVerbose;
			lPin = (new CTracePin (*pPin, NULL, lLogLevel))->GetControllingUnknown();
			*pPin = lPin.Detach ();
		}
	}

#ifdef	_LOG_RESULTS
	if	(lResult != VFW_E_NOT_CONNECTED)
	{
		LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::ConnectedTo"), pThis, pThis->m_dwRef, pThis->mName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::ConnectionMediaType (AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::ConnectionMediaType"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->ConnectionMediaType (pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::ConnectionMediaType"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPin::QueryPinInfo (PIN_INFO *pInfo)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::QueryPinInfo"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->QueryPinInfo (pInfo);

	if	(
			(pInfo)
		&&	(pThis->mFilter)
		)
	{
		if	(pInfo->pFilter)
		{
			pInfo->pFilter->Release();
		}
		pInfo->pFilter = pThis->mFilter;
		pInfo->pFilter->AddRef ();
	}

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::QueryPinInfo"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::QueryDirection (PIN_DIRECTION *pPinDir)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::QueryDirection"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->QueryDirection (pPinDir);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::QueryDirection"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::QueryId (LPWSTR *Id)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::QueryId"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->QueryId (Id);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::QueryId"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::QueryAccept (const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::QueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->QueryAccept (pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::QueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPin::EnumMediaTypes (IEnumMediaTypes **ppEnum)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::EnumMediaTypes"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->EnumMediaTypes (ppEnum);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::EnumMediaTypes"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::QueryInternalConnections (IPin **apPin, ULONG *nPin)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::QueryInternalConnections"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->QueryInternalConnections (apPin, nPin);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::QueryInternalConnections"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPin::NewSegment (REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::NewSegment [%f - %f] [%f]"), pThis, pThis->m_dwRef, pThis->mName, RefTimeSec(tStart), RefTimeSec(tStop), dRate);
#endif

	HRESULT	lResult = pThis->mInnerPin->NewSegment (tStart, tStop, dRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::NewSegment"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::EndOfStream (void)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::EndOfStream"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->EndOfStream ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::EndOfStream"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPin::BeginFlush (void)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->BeginFlush ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPin::EndFlush (void)
{
	METHOD_PROLOGUE(CTracePin, Pin)
#ifdef	_DEBUG_PIN
	LogMessage (MaxLogLevel (_DEBUG_PIN,pThis->mLogLevel), _T("[%p(%u)] %s::XPin::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPin->EndFlush ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPin::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XMemInputPin::GetAllocator (IMemAllocator **ppAllocator)
{
	METHOD_PROLOGUE(CTracePin, MemInputPin)
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), _T("[%p(%u)] %s::XMemInputPin::GetAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMemInputPin->GetAllocator (ppAllocator);
#ifdef	_LOG_ALLOCATOR
	if	(*ppAllocator)
	{
		LogMemAllocator (_LOG_ALLOCATOR, *ppAllocator);
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMemInputPin::GetAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMemInputPin::NotifyAllocator (IMemAllocator *pAllocator, BOOL bReadOnly)
{
	METHOD_PROLOGUE(CTracePin, MemInputPin)
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), _T("[%p(%u)] %s::XMemInputPin::NotifyAllocator [%p] ReadOnly [%d]"), pThis, pThis->m_dwRef, pThis->mName, pAllocator, bReadOnly);
#endif

#ifdef	_LOG_ALLOCATOR
	if	(pAllocator)
	{
		LogMemAllocator (_LOG_ALLOCATOR, pAllocator);
	}
#endif
	HRESULT	lResult = pThis->mInnerMemInputPin->NotifyAllocator (pAllocator, bReadOnly);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMemInputPin::NotifyAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMemInputPin::GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps)
{
	METHOD_PROLOGUE(CTracePin, MemInputPin)
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), _T("[%p(%u)] %s::XMemInputPin::GetAllocatorRequirements"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMemInputPin->GetAllocatorRequirements (pProps);
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
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMemInputPin::GetAllocatorRequirements"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMemInputPin::Receive (IMediaSample *pSample)
{
	METHOD_PROLOGUE(CTracePin, MemInputPin)
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), _T("[%p(%u)] %s::XMemInputPin::Receive"), pThis, pThis->m_dwRef, pThis->mName);
	//LogMediaSampleId (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), pSample, _T("[%p(%u)] %s"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMemInputPin->Receive (pSample);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMemInputPin::Receive"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMemInputPin::ReceiveMultiple (IMediaSample **pSamples, long nSamples, long *nSamplesProcessed)
{
	METHOD_PROLOGUE(CTracePin, MemInputPin)
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), _T("[%p(%u)] %s::XMemInputPin::ReceiveMultiple"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMemInputPin->ReceiveMultiple (pSamples, nSamples, nSamplesProcessed);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMemInputPin::ReceiveMultiple"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMemInputPin::ReceiveCanBlock (void)
{
	METHOD_PROLOGUE(CTracePin, MemInputPin)
#ifdef	_DEBUG_MEMINPUTPIN
	LogMessage (MaxLogLevel (_DEBUG_MEMINPUTPIN,pThis->mLogLevel), _T("[%p(%u)] %s::XMemInputPin::ReceiveCanBlock"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMemInputPin->ReceiveCanBlock ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMemInputPin::ReceiveCanBlock"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::RequestAllocator (IMemAllocator *pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator **ppActual)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::RequestAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->RequestAllocator (pPreferred, pProps, ppActual);
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
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::RequestAllocator"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::Request (IMediaSample *pSample, DWORD_PTR dwUser)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::Request"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->Request (pSample, dwUser);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::Request"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::WaitForNext (DWORD dwTimeout, IMediaSample **ppSample, DWORD_PTR *pdwUser)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::WaitForNext"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->WaitForNext (dwTimeout, ppSample, pdwUser);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::WaitForNext"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::SyncReadAligned (IMediaSample *pSample)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::SyncReadAligned"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->SyncReadAligned (pSample);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::SyncReadAligned"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::SyncRead (LONGLONG llPosition, LONG lLength, BYTE *pBuffer)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::SyncRead"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->SyncRead (llPosition, lLength, pBuffer);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::SyncRead"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::Length (LONGLONG *pTotal, LONGLONG *pAvailable)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::Length"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->Length (pTotal, pAvailable);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::Length"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::BeginFlush (void)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->BeginFlush ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::BeginFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XAsyncReader::EndFlush (void)
{
	METHOD_PROLOGUE(CTracePin, AsyncReader)
#ifdef	_DEBUG_ASYNCREADER
	LogMessage (MaxLogLevel (_DEBUG_ASYNCREADER,pThis->mLogLevel), _T("[%p(%u)] %s::XAsyncReader::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerAsyncReader->EndFlush ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XAsyncReader::EndFlush"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetCapabilities (DWORD *pCapabilities)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetCapabilities"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetCapabilities"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::CheckCapabilities (DWORD *pCapabilities)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::CheckCapabilities"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->CheckCapabilities (pCapabilities);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::CheckCapabilities"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::IsFormatSupported (const GUID *pFormat)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::IsFormatSupported"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->IsFormatSupported (pFormat);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::IsFormatSupported"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::QueryPreferredFormat (GUID *pFormat)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::QueryPreferredFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->QueryPreferredFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::QueryPreferredFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetTimeFormat (GUID *pFormat)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetTimeFormat   (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::IsUsingTimeFormat (const GUID *pFormat)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::IsUsingTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->IsUsingTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::IsUsingTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::SetTimeFormat (const GUID *pFormat)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::SetTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->SetTimeFormat (pFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::SetTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetDuration (LONGLONG *pDuration)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetDuration"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetDuration (pDuration);
#ifdef	_LOG_SEEKING
	if	(pDuration)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Duration [%f]"), pThis, pThis->mName, RefTimeSec (*pDuration));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetDuration"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetStopPosition (LONGLONG *pStop)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetStopPosition"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetStopPosition (pStop);
#ifdef	_LOG_SEEKING
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), pThis, pThis->mName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetStopPosition"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetCurrentPosition (LONGLONG *pCurrent)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetCurrentPosition"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetCurrentPosition (pCurrent);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), pThis, pThis->mName, RefTimeSec (*pCurrent));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetCurrentPosition"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::ConvertTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->ConvertTimeFormat (pTarget, pTargetFormat, Source, pSourceFormat);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::ConvertTimeFormat"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::SetPositions"), pThis, pThis->m_dwRef, pThis->mName);
#endif

#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f] [%s]"), pThis, pThis->mName, RefTimeSec (*pCurrent), SeekingFlagsStr(dwCurrentFlags));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f] [%s]"), pThis, pThis->mName, RefTimeSec (*pStop), SeekingFlagsStr(dwStopFlags));
	}
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->SetPositions (pCurrent, dwCurrentFlags, pStop, dwStopFlags);

#ifdef	_LOG_SEEKING
	if	(
			(pCurrent)
		&&	(dwCurrentFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), pThis, pThis->mName, RefTimeSec (*pCurrent));
	}
	if	(
			(pStop)
		&&	(dwStopFlags & AM_SEEKING_ReturnTime)
		)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), pThis, pThis->mName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::SetPositions"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetPositions (LONGLONG *pCurrent, LONGLONG *pStop)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetPositions"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetPositions (pCurrent, pStop);
#ifdef	_LOG_SEEKING
	if	(pCurrent)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] CurrPosition [%f]"), pThis, pThis->mName, RefTimeSec (*pCurrent));
	}
	if	(pStop)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] StopPosition [%f]"), pThis, pThis->mName, RefTimeSec (*pStop));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetPositions"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetAvailable"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetAvailable (pEarliest, pLatest);
#ifdef	_LOG_SEEKING
	if	(pEarliest)
	{
		LogMessage (_LOG_SEEKING, _T("[%s] [%p] Earliest [%f]"), pThis, pThis->mName, RefTimeSec (*pEarliest));
	}
	if	(pLatest)
	{
		LogMessage (_LOG_SEEKING, _T("[%p] [%s] Latest   [%f]"), pThis, pThis->mName, RefTimeSec (*pLatest));
	}
#endif

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetAvailable"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::SetRate (double dRate)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_MEDIASEEKING
	LogMessage (MaxLogLevel (_DEBUG_MEDIASEEKING,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::SetRate"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->SetRate (dRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::SetRate"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetRate (double *pdRate)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetRate"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetRate (pdRate);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetRate"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XMediaSeeking::GetPreroll (LONGLONG *pllPreroll)
{
	METHOD_PROLOGUE(CTracePin, MediaSeeking)
#ifdef	_DEBUG_QUERIES
	LogMessage (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), _T("[%p(%u)] %s::XMediaSeeking::GetPreroll"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerMediaSeeking->GetPreroll (pllPreroll);

#ifdef	_DEBUG_QUERIES
	LogVfwErrAnon (MaxLogLevel (_DEBUG_QUERIES,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XMediaSeeking::GetPreroll"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CTracePin::XPinConnection::DynamicQueryAccept (const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CTracePin, PinConnection)
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,pThis->mLogLevel), _T("[%p(%u)] %s::XPinConnection::DynamicQueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPinConnection->DynamicQueryAccept (pmt);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPinConnection::DynamicQueryAccept"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPinConnection::NotifyEndOfStream (HANDLE hNotifyEvent)
{
	METHOD_PROLOGUE(CTracePin, PinConnection)
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,pThis->mLogLevel), _T("[%p(%u)] %s::XPinConnection::NotifyEndOfStream"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPinConnection->NotifyEndOfStream (hNotifyEvent);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPinConnection::NotifyEndOfStream"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPinConnection::IsEndPin (void)
{
	METHOD_PROLOGUE(CTracePin, PinConnection)
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,pThis->mLogLevel), _T("[%p(%u)] %s::XPinConnection::IsEndPin"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPinConnection->IsEndPin ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPinConnection::IsEndPin"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPinConnection::DynamicDisconnect (void)
{
	METHOD_PROLOGUE(CTracePin, PinConnection)
#ifdef	_DEBUG_PINCONNECTION
	LogMessage (MaxLogLevel (_DEBUG_PINCONNECTION,pThis->mLogLevel), _T("[%p(%u)] %s::XPinConnection::DynamicDisconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPinConnection->DynamicDisconnect ();

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPinConnection::DynamicDisconnect"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CTracePin::XPinFlowControl::Block (DWORD dwBlockFlags, HANDLE hEvent)
{
	METHOD_PROLOGUE(CTracePin, PinFlowControl)
#ifdef	_DEBUG_PINFLOWCONTROL
	LogMessage (MaxLogLevel (_DEBUG_PINFLOWCONTROL,pThis->mLogLevel), _T("[%p(%u)] %s::XPinFlowControl::Block"), pThis, pThis->m_dwRef, pThis->mName);
#endif

	HRESULT	lResult = pThis->mInnerPinFlowControl->Block (dwBlockFlags, hEvent);

#ifdef	_LOG_RESULTS
	LogVfwErrAnon (MaxLogLevel (_LOG_RESULTS,pThis->mLogLevel), lResult, _T("[%p(%u)] %s::XPinFlowControl::Block"), pThis, pThis->m_dwRef, pThis->mName);
#endif
	return lResult;
}

