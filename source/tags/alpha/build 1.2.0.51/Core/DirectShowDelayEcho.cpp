#include "StdAfx.h"
#include "DirectShowDelayEcho.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_COM			LogNormal
//#define	_DEBUG_CUES			LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_SEEKING		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM		LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_RESULTS			LogNormal|LogHighVolume|LogTimeMs
#endif

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectShowDelayEcho, CCmdTarget)
	INTERFACE_PART(CDirectShowDelayEcho, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectShowDelayEcho, __uuidof(ISampleGrabberCB), Grabber)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CDirectShowDelayEcho, Grabber)

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowDelayEcho, CCmdTarget)

CDirectShowDelayEcho::CDirectShowDelayEcho (LPCTSTR pFilterName)
:	mFilterName (pFilterName),
	mCueNdx (-1),
	mCueDuration (0)
{
	if	(mFilterName.IsEmpty())
	{
		mFilterName = _T("DelayEcho");
	}
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowDelayEcho::CDirectShowDelayEcho (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	//AfxOleLockApp (); // No reference counting

	EnableAggregation ();
}

CDirectShowDelayEcho::~CDirectShowDelayEcho( )
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowDelayEcho::~CDirectShowDelayEcho (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	Disconnect ();
	m_dwRef = 0;
	//AfxOleUnlockApp (); // No reference counting
}

void CDirectShowDelayEcho::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowDelayEcho::OnFinalRelease"), this);
#endif
	// Should never happed - this filter is owned by another object and not reference counted
	//CCmdTarget::OnFinalRelease ()
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IBaseFilter * CDirectShowDelayEcho::SafeGetFilter ()
{
	if	(this)
	{
		return mGrabberFilter;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin, AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
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

	return lResult;
}

HRESULT CDirectShowDelayEcho::ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin, AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
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

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::Disconnect ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		IBaseFilterPtr		lFilter (mGrabberFilter);
		FILTER_INFO_Safe	lFilterInfo;
		
		mEchoEvent.ResetEvent ();
		mEchoWaitHandle.Close ();

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

	return lResult;
}

HRESULT CDirectShowDelayEcho::Enable ()
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(
				(mGrabber != NULL)
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGrabber->SetCallback (&m_xGrabber, 0))))
			)
		{
			LogVfwErr (LogNormal, mGrabber->SetOneShot (FALSE));
		}
		else
		{
			mEchoEvent.ResetEvent ();
			mEchoWaitHandle.Close ();
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
	
void CDirectShowDelayEcho::AddCue (REFERENCE_TIME pStartTime)
{
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		mStartTimes.Add (pStartTime);
#ifdef	_DEBUG_CUES
		LogMessage (_DEBUG_CUES, _T("[%s] AddCue [%f]"), mFilterName, RefTimeSec(pStartTime));
#endif		
	}
	catch AnyExceptionSilent
}

void CDirectShowDelayEcho::ClearCues ()
{
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		mStartTimes.RemoveAll ();
		mCueNdx = -1;
		mCueDuration = 0;
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::CueDelay ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		mEchoEvent.ResetEvent ();
		mEchoWaitHandle.Close ();
		mCueNdx = -1;

		if	(mStartTimes.GetSize () > 0)
		{
			IMediaSeekingPtr	lMediaSeeking;
			LONGLONG			lDuration = -1;
			REFERENCE_TIME		lStartTime;
			REFERENCE_TIME		lStopTime;

#ifdef	_DEBUG_CUES
			LogMessage (_DEBUG_CUES, _T("[%s] CueDelay [%d] of [%d] at [%f]"), mFilterName, mCueNdx+2, mStartTimes.GetSize(), RefTimeSec(mStartTimes[0]));
#endif		
			if	(
					(SUCCEEDED (lResult = GetUpstreamSeeking (&lMediaSeeking)))
				&&	(lMediaSeeking != NULL)
				&&	(SUCCEEDED (lResult = lMediaSeeking->GetDuration (&lDuration)))
				&&	(lDuration > 0)
				)
			{
				lStartTime = 0;
				lStopTime = lDuration;
#ifdef	_DEBUG_SEEKING
				LogMessage (_DEBUG_SEEKING, _T("[%s] SetPositions [%f - %f] of [%f]"), mFilterName, RefTimeSec(lStartTime), RefTimeSec(lStopTime), RefTimeSec(lDuration));
#endif		
				mDataLock.Unlock ();
				try
				{
					lResult = LogVfwErr (LogNormal, lMediaSeeking->SetPositions (&lStartTime, AM_SEEKING_AbsolutePositioning|AM_SEEKING_Segment, &lStopTime, AM_SEEKING_IncrementalPositioning|AM_SEEKING_Segment));
				}
				catch AnyExceptionDebug
				mDataLock.Lock ();
				
				if	(SUCCEEDED (lResult))
				{
#ifdef	_DEBUG_SEEKING
					LogMediaSeekingPos (_DEBUG_SEEKING, lMediaSeeking, _T("[%s] SetPositions"), mFilterName);
#endif		
					mCueNdx = 0;
					mCueDuration = lDuration;
				}
			}
		}

		if	(
				(mCueNdx >= 0)
			&&	(mStartTimes.GetSize () > 1)
			)
		{
			RegisterWaitForSingleObject (&mEchoWaitHandle, mEchoEvent, EchoCallback, this, INFINITE, WT_EXECUTEDEFAULT);
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

HRESULT CDirectShowDelayEcho::CueEcho ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		if	(
				(mCueNdx >= 0)
			&&	(mCueNdx < mStartTimes.GetUpperBound ())
			&&	(mCueDuration > 0)
			)
		{
			IMediaSeekingPtr	lMediaSeeking;
			REFERENCE_TIME		lStartTime = 0;
			REFERENCE_TIME		lStopTime = mCueDuration;

#ifdef	_DEBUG_CUES
			LogMessage (_DEBUG_CUES, _T("[%s] CueEcho [%d] of [%d] at [%f] of [%f]"), mFilterName, mCueNdx+2, mStartTimes.GetSize(), mStartTimes[mCueNdx+1], RefTimeSec(mCueDuration));
#endif		
			if	(
					(SUCCEEDED (lResult = GetUpstreamSeeking (&lMediaSeeking)))
				&&	(lMediaSeeking != NULL)
				)
			{
#ifdef	_DEBUG_SEEKING
				LogMessage (_DEBUG_SEEKING, _T("[%s] SetPositions [%f - %f] of [%f] (NoFlush)"), mFilterName, RefTimeSec(lStartTime), RefTimeSec(lStopTime), RefTimeSec(mCueDuration));
#endif		
				mDataLock.Unlock ();
				try
				{
//
//	Note - AM_SEEKING_NoFlush is required so we don't step on previous samples, but it introduces an offset in the sample times.
//	       This requires the use of mCueDuration to correct for the offsets.
//				
					lResult = LogVfwErr (LogNormal, lMediaSeeking->SetPositions (&lStartTime, AM_SEEKING_AbsolutePositioning|AM_SEEKING_Segment|AM_SEEKING_NoFlush, &lStopTime, AM_SEEKING_IncrementalPositioning|AM_SEEKING_Segment|AM_SEEKING_NoFlush));
				}
				catch AnyExceptionDebug
				mDataLock.Lock ();
				
				if	(SUCCEEDED (lResult))
				{
#ifdef	_DEBUG_SEEKING
					LogMediaSeekingPos (_DEBUG_SEEKING, lMediaSeeking, _T("[%s] SetPositions"), mFilterName);
#endif		
					mCueNdx++;
				}
			}
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);
	
	try
	{
		IEnumPinsPtr		lEnumPins;
		IPinPtr				lInputPin;
		IPinPtr				lUpstreamPin;
		IMediaSeekingPtr	lMediaSeeking;

		if	(
				(mGrabberFilter != NULL)	
			&&	(SUCCEEDED (lResult = mGrabberFilter->EnumPins (&lEnumPins)))
			&&	(lEnumPins != NULL)
			&&	(lEnumPins->Next (1, &lInputPin, NULL) == S_OK)
			&&	(SUCCEEDED (lResult = lInputPin->ConnectedTo (&lUpstreamPin)))
			)
		{
			lMediaSeeking = lUpstreamPin;
			if	(lMediaSeeking != NULL)
			{
				(*pMediaSeeking) = lMediaSeeking.Detach();
				lResult = S_OK;
			}
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

bool CDirectShowDelayEcho::IsEndOfStream (double pSampleTime, IMediaSample * pMediaSample)
{
	bool		lRet = false;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		IMediaSeekingPtr	lMediaSeeking;
		REFERENCE_TIME		lStartTime = -1;
		REFERENCE_TIME		lEndTime = -1;
		REFERENCE_TIME		lStopTime = -1;

		if	(mCueNdx >= 0)
		{
			lStopTime = mCueDuration * (LONGLONG)(mCueNdx+1);
		}		
		
		if	(
				(lStopTime > 0)
			&&	(SUCCEEDED (GetUpstreamSeeking (&lMediaSeeking)))
			&&	(lMediaSeeking != NULL)
			&&	(SUCCEEDED (pMediaSample->GetTime (&lStartTime, &lEndTime)))
			&&	(lEndTime >= lStopTime-100) // End time of the last sample can be slightly less than the duration
			)
		{
#ifdef	_DEBUG_STREAM
			LogMessage (_DEBUG_STREAM, _T("[%s] IsEndOfStream [%d] [%f of %f]"), mFilterName, mCueNdx, RefTimeSec(lEndTime), RefTimeSec(lStopTime));
#endif		
			lRet = true;
		}
	}
	catch AnyExceptionDebug
	
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::DelaySample (double pSampleTime, IMediaSample * pMediaSample)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
#ifdef	_DEBUG_STREAM
		LogMediaSampleId (_DEBUG_STREAM, pMediaSample, _T("[%s] Sample at [%f]"), mFilterName, pSampleTime);
#endif	
		if	(
				(mCueNdx >= 0)
			&&	(mCueNdx <= mStartTimes.GetUpperBound ())
			)
		{
			REFERENCE_TIME	lDelayTime = mStartTimes [mCueNdx];
			REFERENCE_TIME	lStartTime;
			REFERENCE_TIME	lEndTime;
			
			if	(mCueNdx > 0)
			{
				lDelayTime -= mCueDuration * (LONGLONG)mCueNdx;
			}

			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pMediaSample->GetTime (&lStartTime, &lEndTime))))
			{
				lStartTime += lDelayTime;
				lEndTime += lDelayTime;
				
				if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pMediaSample->SetTime (&lStartTime, &lEndTime))))
				{
#ifdef	_DEBUG_STREAM
					LogMediaSampleId (_DEBUG_STREAM, pMediaSample, _T("[%s] Sample delayed [%d] [%f]"), mFilterName, mCueNdx+1, RefTimeSec(lDelayTime));
#endif	
				}
			}
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

void CALLBACK CDirectShowDelayEcho::EchoCallback (PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	CDirectShowDelayEcho *	lThis = (CDirectShowDelayEcho *) lpParameter;
	
	lThis->CueEcho ();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowDelayEcho::XGrabber::SampleCB (double SampleTime, IMediaSample *pSample)
{
	METHOD_PROLOGUE(CDirectShowDelayEcho, Grabber)

	try
	{
		bool	lIsEndOfStream = pThis->IsEndOfStream (SampleTime, pSample);
		
		pThis->DelaySample (SampleTime, pSample);		
		
		if	(lIsEndOfStream)
		{
			pThis->mEchoEvent.SetEvent ();
		}
	}
	catch AnyExceptionDebug
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDirectShowDelayEcho::XGrabber::BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen)
{
	METHOD_PROLOGUE(CDirectShowDelayEcho, Grabber)
	return S_OK;
}
