#include "StdAfx.h"
#include "DirectSoundDelay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_COM			LogNormal
//#define	_DEBUG_CUES			LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_CACHE		LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_RESULTS			LogNormal|LogHighVolume|LogTimeMs
#endif

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectSoundDelay, CCmdTarget)
	INTERFACE_PART(CDirectSoundDelay, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectSoundDelay, __uuidof(ISampleGrabberCB), Grabber)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CDirectSoundDelay, Grabber)

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectSoundDelay, CCmdTarget)

CDirectSoundDelay::CDirectSoundDelay (LPCTSTR pFilterName)
:	mFilterName (pFilterName),
	mCueNdx (-1),
	mCueDuration (0)
{
	if	(mFilterName.IsEmpty())
	{
		mFilterName = _T("DelayRepeat");
	}
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectSoundDelay::CDirectSoundDelay (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	//AfxOleLockApp (); // No reference counting

	EnableAggregation ();
}

CDirectSoundDelay::~CDirectSoundDelay( )
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectSoundDelay::~CDirectSoundDelay (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	Disconnect ();
	m_dwRef = 0;
	//AfxOleUnlockApp (); // No reference counting
}

void CDirectSoundDelay::OnFinalRelease ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectSoundDelay::OnFinalRelease"), this);
#endif
	// Should never happed - this filter is owned by another object and not reference counted
	//CCmdTarget::OnFinalRelease ()
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IBaseFilter * CDirectSoundDelay::SafeGetFilter ()
{
	if	(this)
	{
		return mGrabberFilter;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundDelay::ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin, AM_MEDIA_TYPE * pMediaType)
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

HRESULT CDirectSoundDelay::ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin, AM_MEDIA_TYPE * pMediaType)
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

HRESULT CDirectSoundDelay::Disconnect ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		IBaseFilterPtr		lFilter (mGrabberFilter);
		FILTER_INFO_Safe	lFilterInfo;

		StopClock ();
		SafeFreeSafePtr (mClock);
		ClearSoundCues ();
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

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundDelay::Enable (bool pEnable)
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(mGrabber != NULL)
		{
			if	(pEnable)
			{
				if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGrabber->SetCallback (&m_xGrabber, 0))))
				{
					LogVfwErr (LogNormal, mGrabber->SetOneShot (FALSE));
				}
			}
			else
			{
				lResult = LogVfwErr (LogNormal, mGrabber->SetCallback (NULL, 0));
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundDelay::AddSoundCue (REFERENCE_TIME pStartTime)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		if	(mCueDuration <= 0)
		{
			IPinPtr				lInputPin;
			IPinPtr				lUpstreamPin;
			IMediaSeekingPtr	lMediaSeeking;
			
			if	(
					(mGrabberFilter != NULL)
				&&	(SUCCEEDED (GetFilterPins (mGrabberFilter, &lInputPin, NULL)))
				&&	(lInputPin != NULL)
				&&	(SUCCEEDED (lResult = lInputPin->ConnectedTo (&lUpstreamPin)))
				)
			{
				lMediaSeeking = lUpstreamPin;

				if	(lMediaSeeking != NULL)
				{
					lResult = LogVfwErr (LogNormal, lMediaSeeking->GetDuration (&mCueDuration));
				}
			}
		}

		if	(
				(mCueDuration > 0)
			&&	(mCueTimes.AddUnique (pStartTime) >= 0)
			)
		{
#ifdef	_DEBUG_CUES
			LogMessage (_DEBUG_CUES, _T("[%s] Cue [%d] [%f]"), mFilterName, mCueTimes.GetUpperBound(), RefTimeSec(pStartTime));
#endif		
			mCueNdx = max (mCueNdx, 0);
			lResult = S_OK;
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

void CDirectSoundDelay::ClearSoundCues ()
{
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		StopClock ();
		mCueNdx = -1;
		mCueDuration = 0;
		mCueTimes.RemoveAll ();
		mSampleCache.RemoveAll ();
		mSampleCacheAllocator = NULL;
	}
	catch AnyExceptionSilent
}

INT_PTR CDirectSoundDelay::GetSoundCueCount () const
{
	CSingleLock	lLock (&mDataLock, TRUE);
	return mCueTimes.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundDelay::DelaySample (double pSampleTime, IMediaSample * pMediaSample)
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
			&&	(mCueDuration > 0)
			)
		{
			REFERENCE_TIME		lDelayTime = mCueTimes [mCueNdx];
			REFERENCE_TIME		lStopTime = mCueDuration;
			REFERENCE_TIME		lStartTime;
			REFERENCE_TIME		lEndTime;
			
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pMediaSample->GetTime (&lStartTime, &lEndTime))))
			{
				lStartTime += lDelayTime;
				lEndTime += lDelayTime;
				
				if	(SUCCEEDED (LogVfwErr (LogNormal, pMediaSample->SetTime (&lStartTime, &lEndTime))))
				{
#ifdef	_DEBUG_STREAM
					LogMediaSampleId (_DEBUG_STREAM, pMediaSample, _T("[%s]   Delayed [%d] [%f]"), mFilterName, mCueNdx, RefTimeSec(lDelayTime));
#endif	
				}

				if	(lEndTime >= lStopTime-100) // End time of the last sample can be slightly less than the duration
				{
					lResult = VFW_S_NO_MORE_ITEMS;
				}
			}
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundDelay::CacheSample (IMediaSample * pMediaSample)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		IMediaSamplePtr	lMediaSample;
		
		if	(mSampleCacheAllocator == NULL)
		{
			tS <ALLOCATOR_PROPERTIES>	lRequest;
			tS <ALLOCATOR_PROPERTIES>	lActual;
			REFERENCE_TIME				lStartTime;
			REFERENCE_TIME				lEndTime;
			
			lRequest.cbAlign = 1;
			lRequest.cbBuffer = pMediaSample->GetSize ();

			if	(
					(SUCCEEDED (pMediaSample->GetTime (&lStartTime, &lEndTime)))
				&&	(lEndTime > lStartTime)
				)
			{
				lRequest.cBuffers = (long)(mCueDuration / (lEndTime - lStartTime));
				lRequest.cBuffers = min ((lRequest.cBuffers+1) * (long)mCueTimes.GetSize(), 200);
			}			
			lRequest.cBuffers = max (lRequest.cBuffers, (long)mCueTimes.GetSize());

			if	(
					(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&mSampleCacheAllocator))))
				&&	(SUCCEEDED (LogVfwErr (LogNormal, mSampleCacheAllocator->SetProperties (&lRequest, lActual.Clear()))))
				&&	(SUCCEEDED (LogVfwErr (LogNormal, mSampleCacheAllocator->Commit ())))
				)
			{
#ifdef	_DEBUG_CACHE
				LogMemAllocator (_DEBUG_CACHE, mSampleCacheAllocator, _T("[%s] CacheAllocator"), mFilterName);
#endif	
			}
			else
			{
				SafeFreeSafePtr (mSampleCacheAllocator);
			}
		}
		if	(
				(mSampleCacheAllocator != NULL)
			&&	(SUCCEEDED (DuplicateSample (pMediaSample, mSampleCacheAllocator, &lMediaSample)))
			)
		{
#ifdef	_DEBUG_CACHE
			LogMediaSampleId (_DEBUG_CACHE, lMediaSample, _T("[%s] Cache [%d]"), mFilterName, mSampleCache.GetSize());
#endif	
			mSampleCache.Add (lMediaSample);
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundDelay::RepeatSamples ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		if	(
				(mCueNdx >= 0)
			&&	(mCueNdx < mCueTimes.GetUpperBound ())
			&&	(mSampleCache.GetSize() > 0)
			)
		{
			if	(
					(mClock == NULL)
				&&	(mGrabberFilter != NULL)
				)
			{
				mGrabberFilter->GetSyncSource (&mClock);
			}

			lResult = StartClock (sDefaultClockInterval);
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

void CDirectSoundDelay::OnClockPulse ()
{
	int	lPriority = GetThreadPriority (GetCurrentThread ());

	try
	{
		REFERENCE_TIME		lStartTime = 0;
		REFERENCE_TIME		lStopTime = 0;
		IPinPtr				lInputPin;
		IPinPtr				lUpstreamPin;
		IMemInputPinPtr		lMemInputPin;

		if	(mClock != NULL)
		{
			CSingleLock	lLock (&mDataLock, TRUE);
			
			try
			{
				if	(mCueNdx < mCueTimes.GetUpperBound ())
				{
					mCueNdx++;
					lStartTime = mCueTimes [mCueNdx];
					lStopTime = lStartTime + mCueDuration;
				}
			}
			catch AnyExceptionDebug
		}

		if	(lStopTime > lStartTime)
		{
			CSingleLock	lLock (&mStateLock, TRUE);
			
			try
			{
				if	(
						(mGrabberFilter != NULL)
					&&	(SUCCEEDED (GetFilterPins (mGrabberFilter, &lInputPin, NULL)))
					&&	(lInputPin != NULL)
					&&	(SUCCEEDED (LogVfwErr (LogNormal, lInputPin->ConnectedTo (&lUpstreamPin))))
					)
				{
					lMemInputPin = lInputPin;
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(mClock != NULL)
		{
#ifdef	_DEBUG_STREAM
			LogMessage (_DEBUG_STREAM, _T("[%s] All cues complete"), mFilterName);
#endif	
			StopClock ();
		}
		
		if	(
				(lStopTime > lStartTime)
			&&	(lMemInputPin != NULL)
			&&	(lUpstreamPin != NULL)
			)
		{
			CSingleLock	lLock (&mDataLock, TRUE);
			
			try
			{
				if	(mCueNdx <= mCueTimes.GetUpperBound ())
				{
					INT_PTR			lCacheNdx;
					IMediaSamplePtr	lMediaSample;

					try
					{
						lUpstreamPin->NewSegment (lStartTime, lStopTime, 1.0);
					}
					catch AnyExceptionSilent
			
					for	(lCacheNdx = 0; lCacheNdx <= mSampleCache.GetUpperBound(); lCacheNdx++)
					{
#ifdef	_DEBUG_CACHE
						LogMediaSampleId (_DEBUG_CACHE, mSampleCache [lCacheNdx], _T("[%s] From cache"), mFilterName);
#endif	
						if	(SUCCEEDED (LogVfwErr (LogNormal|LogTimeMs, DuplicateSample (mSampleCache [lCacheNdx], mSampleCacheAllocator, &lMediaSample, 0))))
						{
							mDataLock.Unlock ();
							try
							{
								LogVfwErr (LogNormal|LogTimeMs, lMemInputPin->Receive (lMediaSample), _T("Receive"));
							}
							catch AnyExceptionSilent
							mDataLock.Lock ();
						}
						else
						{
							break;
						}
					}
				}
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
	
	SetThreadPriority (GetCurrentThread(), lPriority);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectSoundDelay::XGrabber::SampleCB (double SampleTime, IMediaSample *pSample)
{
	METHOD_PROLOGUE(CDirectSoundDelay, Grabber)

	try
	{
		HRESULT	lResult;
		INT_PTR	lCueNdx = -1;
		INT_PTR	lCueCount = 0;
		
		{
			CSingleLock	lLock (&pThis->mDataLock, TRUE);
			
			try
			{
				lCueNdx = pThis->mCueNdx;
				lCueCount = pThis->mCueTimes.GetSize();
			}
			catch AnyExceptionSilent
		}

		if	(
				(lCueNdx == 0)			
			&&	(lCueCount > 1)
			)
		{
			pThis->CacheSample (pSample);
		}
	
		lResult = pThis->DelaySample (SampleTime, pSample);

		if	(
				(lCueNdx == 0)		
			&&	(lResult == VFW_S_NO_MORE_ITEMS)
			)
		{
			pThis->RepeatSamples ();
		}
	}
	catch AnyExceptionDebug
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDirectSoundDelay::XGrabber::BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen)
{
	METHOD_PROLOGUE(CDirectSoundDelay, Grabber)
	return S_OK;
}
