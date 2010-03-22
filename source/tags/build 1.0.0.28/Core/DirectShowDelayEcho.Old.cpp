#include "StdAfx.h"
#include "DirectShowDelayEcho.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE	LogNormal
#define	_DEBUG_CUES		LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_SAMPLES	LogNormal|LogHighVolume|LogTimeMs
#define	_DEBUG_STREAM	LogNormal|LogHighVolume|LogTimeMs
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowDelayEcho, CDirectShowFilter)

CDirectShowDelayEcho::CDirectShowDelayEcho (LPCTSTR pFilterName, IPin * pDurationSourcePin)
:	mFilterName (pFilterName),
	mDurationSourcePin (pDurationSourcePin)
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowDelayEcho::CDirectShowDelayEcho (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation ();
	mSeekingCaps |= AM_SEEKING_CanDoSegments;
}

CDirectShowDelayEcho::~CDirectShowDelayEcho( )
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowDelayEcho::~CDirectShowDelayEcho (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Disconnect ();
	AfxOleUnlockApp ();
}

void CDirectShowDelayEcho::OnFinalRelease ()
{
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("[%p] CDirectShowDelayEcho::OnFinalRelease!!!"), this); //Should never happen - instance is owned by AudioPin
#endif
	CDirectShowFilter::OnFinalRelease ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin, AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lMutexLock (&mMutex, TRUE);

	try
	{
		IPinPtr				lDownstreamPin (pDownstreamPin);
		IPinPtr				lUpstreamPin;
		AM_MEDIA_TYPE_Safe	lMediaType;
		tMediaTypePtr		lPinMediaType;

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
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pFilterGraph->AddFilter (GetFilter(), mFilterName))))
			)
		{
			if	(SUCCEEDED (MoDuplicateMediaType (lPinMediaType.Free(), pMediaType)))
			{
				mInputPin->mMediaTypes.Add (lPinMediaType.Detach ());
			}
			if	(SUCCEEDED (MoDuplicateMediaType (lPinMediaType.Free(), pMediaType)))
			{
				mOutputPin->mMediaTypes.Add (lPinMediaType.Detach ());
			}

			if	(
					(SUCCEEDED (lResult = LogVfwErr (LogNormal, lDownstreamPin->Disconnect ())))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, lUpstreamPin->Disconnect ())))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pFilterGraph->ConnectDirect (lUpstreamPin, mInputPin->GetInterface(), pMediaType))))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pFilterGraph->ConnectDirect (mOutputPin->GetInterface(), lDownstreamPin, pMediaType))))
				)
			{
#ifdef	_DEBUG_NOT
				LogFilter (LogDebug, GetFilter(), true, _T("Connected"));
#endif
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowDelayEcho::ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin, AM_MEDIA_TYPE * pMediaType)
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lMutexLock (&mMutex, TRUE);

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
			lResult = ConnectBefore (pFilterGraph, lDownstreamPinInfo.pFilter, lDownstreamPin);
		}
		else
		if	(SUCCEEDED (lResult))
		{
			lResult = E_FAIL;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowDelayEcho::Disconnect ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lMutexLock (&mMutex, TRUE);

	try
	{
		if	(mFilterGraph != NULL)
		{
			lResult = LogVfwErr (LogNormal, mFilterGraph->RemoveFilter (GetFilter()));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowDelayEcho::AddCue (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime)
{
	CSingleLock	lMutexLock (&mMutex, TRUE);

	try
	{
#ifdef	_DEBUG_CUES
		LogMessage (_DEBUG_CUES, _T("[%s] [%p] AddCue [%f - %f]"), mFilterName, this, RefTimeSec(pStartTime), RefTimeSec(pEndTime));
#endif
		mStartTimes.Add (pStartTime);
		mEndTimes.Add (pEndTime);
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

const GUID & CDirectShowDelayEcho::GetClassID ()
{
	return __uuidof (CDirectShowDelayEcho);
}

HRESULT CDirectShowDelayEcho::SetFilterName (LPCWSTR pFilterName)
{
	mFilterName = pFilterName;
	return S_OK;
}

CString CDirectShowDelayEcho::GetFilterName ()
{
	return mFilterName;
}

/////////////////////////////////////////////////////////////////////////////

LONGLONG CDirectShowDelayEcho::GetDuration ()
{
	//LONGLONG			lDuration = -1;
	//IMediaSeekingPtr	lMediaSeeking (mDurationSourcePin);

	//if	(lMediaSeeking != NULL)
	//{
	//	lMediaSeeking->GetDuration (&lDuration);
	//}
	//return lDuration;
	return CDirectShowFilter::GetDuration();
}

void CDirectShowDelayEcho::InitializePins ()
{
	if	(mOutputPin = new CDirectShowPinOut (*this, _T("Delay Out")))
	{
		mOutputPins.Add (mOutputPin);
	}
	if	(mInputPin = new CDirectShowPinIn (*this, _T("Delay In"), NULL, 30))
	{
		mInputPins.Add (mInputPin);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowDelayEcho::BeginFlush ()
{
	CSingleLock	lMutexLock (&mMutex, TRUE);

	try
	{
		LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] BeginFlush [%s]"), mFilterName, this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState));
		mSampleCache.RemoveAll ();
		mCacheAllocator = NULL;
		CDirectShowFilter::BeginFlush ();
	}
	catch AnyExceptionDebug
}

void CDirectShowDelayEcho::OnStartInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] OnStartInputStream [%s]"), mFilterName, this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState));
	CDirectShowFilter::OnStartInputStream (pStartTime, pEndTime, pRate);
}

void CDirectShowDelayEcho::OnEndInputStream (long pPendingSamples)
{
	LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] OnEndInputStream [%s] [%d]"), mFilterName, this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), pPendingSamples);
	CDirectShowFilter::OnEndInputStream (pPendingSamples);
}

void CDirectShowDelayEcho::OnGotInputSample (CDirectShowPin * pPin)
{
	LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] OnGotInputSample [%s]"), mFilterName, this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState));

//	if	(mState == State_Running)
	{
		IMediaSamplePtr	lMediaSample;
		while (mInputPin->GetInputSample (&lMediaSample) == S_OK)
		{
			int lPriority = ::GetThreadPriority (::GetCurrentThread ());

			//IMediaSamplePtr	lOutputSample;
			//LogVfwErr (LogNormal, mOutputPin->GetOutputSample (&lOutputSample));
			//LogVfwErr (LogNormal, CopySample (lMediaSample, lOutputSample));
			//lOutputSample = lMediaSample;
			LogSample (_DEBUG_SAMPLES, lMediaSample, _T("[%s] [%p] [%f] Sample"), mFilterName, this, RefTimeSec(GetStreamTime(mState)));
			LogVfwErr (LogNormal, mOutputPin->PutOutputSample (lMediaSample));

			int lNewPriority = ::GetThreadPriority (::GetCurrentThread ());
			//LogMessage (LogDebugFast, _T("Priority [%d] [%d]"), lPriority, lNewPriority);
			//::SetThreadPriority (::GetCurrentThread(), lPriority);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::SetClock (REFERENCE_TIME pReferenceTime, REFERENCE_TIME pDelay)
{
	return S_FALSE;
}
HRESULT CDirectShowDelayEcho::StartClock (REFERENCE_TIME pInterval)
{
	return S_FALSE;
}
HRESULT CDirectShowDelayEcho::StopClock ()
{
	return S_FALSE;
}

void CDirectShowDelayEcho::OnClockPulse ()
{
#if FALSE
	CSingleLock	lMutexLock (&mMutex, TRUE);

	try
	{
		HRESULT			lResult = S_FALSE;
		IMediaSamplePtr	lMediaSample;
		REFERENCE_TIME	lStartTime;
		REFERENCE_TIME	lEndTime;
		REFERENCE_TIME	lDelayTime = mStartTimes [0];

//			&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mInputPin->PeekInputSample (&lSample))))
//		if ((lResult = mInputPin->GetInputSample (&lMediaSample)) == S_OK)
		if	(false)
		{
			if	(SUCCEEDED (LogVfwErr (LogNormal, lMediaSample->GetTime (&lStartTime, &lEndTime))))
			{
				//mCurrTime = lStartTime;
				//lStartTime += lDelayTime;
				//lEndTime += lDelayTime;
				//LogVfwErr (LogNormal, lMediaSample->SetTime (&lStartTime, &lEndTime));
			}
#ifdef	_DEBUG_SAMPLES
			LogSample (_DEBUG_SAMPLES, lMediaSample, _T("[%s] [%p] [%f] Sample [%f - %f] Delay [%f]"), mFilterName, this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(lStartTime-lDelayTime), RefTimeSec(lEndTime-lDelayTime), RefTimeSec(lDelayTime));
#endif
//			IMediaSamplePtr	lOutputSample;
//			if	(SUCCEEDED (LogVfwErr (LogNormal, mOutputPin->GetOutputSample (&lOutputSample, &lStartTime, &lEndTime))))
			{
//				CopySample (lMediaSample, lOutputSample);
//LogSample (_DEBUG_SAMPLES, lOutputSample, _T("Output"));

				int lPriority = ::GetThreadPriority (::GetCurrentThread ());

//				mOutputPin->PutOutputSample (lOutputSample);
//				mOutputPin->PutOutputSample (lMediaSample);

				int lNewPriority = ::GetThreadPriority (::GetCurrentThread ());
//LogMessage (LogDebugFast, _T("Priority [%d] [%d]"), lPriority, lNewPriority);
				::SetThreadPriority (::GetCurrentThread(), lPriority);
			}
//LogMessage (_DEBUG_SAMPLES, _T("PutOutputSample done"));
		}

//		if	(lResult == VFW_S_NO_MORE_ITEMS)
		if	(true)
		{
//LogMessage (LogDebug, _T("--- EndOfStream ---"));
//			StopOutputStreams ();
//LogMessage (LogDebug, _T("--- StopClock ---"));
			//StopClock ();
//LogMessage (LogDebug, _T("--- StopClock Done ---"));
		}
	}
	catch AnyExceptionDebug
#endif	
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::CopySample (IMediaSample * pSample, IMediaSample * pCopy)
{
	HRESULT			lResult;
	REFERENCE_TIME	lStartTime;
	REFERENCE_TIME	lEndTime;
	tMediaTypePtr	lMediaType;
	long			lDataSize;
	long			lDataLength;
	LPBYTE			lBuffer;
	IMediaSamplePtr	lCopy;
	long			lCopySize;
	LPBYTE			lCopyBuffer;

	if	(
			(lDataSize = pSample->GetSize ())
		&&	(lDataLength = pSample->GetActualDataLength ())
		&&	(SUCCEEDED (lResult = pSample->GetTime (&lStartTime, &lEndTime)))
		&&	(SUCCEEDED (lResult = pSample->GetMediaType (lMediaType.Free())))
		&&	(lCopySize = pCopy->GetSize ())
		&&	(lCopySize >= lDataSize)
		&&	(SUCCEEDED (lResult = pCopy->SetTime (&lStartTime, &lEndTime)))
		&&	(SUCCEEDED (lResult = pCopy->SetMediaType (lMediaType)))
		&&	(SUCCEEDED (lResult = pCopy->SetDiscontinuity (pSample->IsDiscontinuity()==S_OK)))
		&&	(SUCCEEDED (lResult = pCopy->SetPreroll (pSample->IsPreroll()==S_OK)))
		&&	(SUCCEEDED (lResult = pCopy->SetSyncPoint (pSample->IsSyncPoint()==S_OK)))
		&&	(SUCCEEDED (lResult = pSample->GetPointer (&lBuffer)))
		&&	(SUCCEEDED (lResult = pCopy->GetPointer (&lCopyBuffer)))
		)
	{
		memcpy (lCopyBuffer, lBuffer, lDataLength);
		lResult = pCopy->SetActualDataLength (lDataLength);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#if FALSE

HRESULT CDirectShowDelayEcho::BeginInput ()
{
	CSingleLock	lLock (&mCriticalSection, TRUE);

	try
	{
		IPinPtr	lOutputPin;
		IPinPtr	lDownstreamPin;

		mSampleCache.RemoveAll ();
		mCacheAllocator = NULL;

		if	(mStartTimes.GetSize () > 0)
		{
#ifdef	_DEBUG_CUES
			LogMessage (_DEBUG_CUES, _T("[%s] [%p] BeginInput [%f - %f]"), mFilterName, this, RefTimeSec(mStartTimes[0]), RefTimeSec(mEndTimes[0]));
#endif
			if	(
					(SUCCEEDED (GetFilterPins (mGrabberFilter, NULL, &lOutputPin)))
				&&	(lOutputPin != NULL)
				&&	(SUCCEEDED (lOutputPin->ConnectedTo (&lDownstreamPin)))
				&&	(lDownstreamPin != NULL)
				)
			{
				try
				{
#ifdef	_DEBUG_CUES
					LogMessage (_DEBUG_CUES, _T("[%s] [%p] Send NewSegment [%f - %f]"), mFilterName, this, RefTimeSec(mStartTimes[0]), RefTimeSec(mEndTimes[0]));
#endif
					LogVfwErr (LogNormal, lDownstreamPin->NewSegment (mStartTimes[0], mEndTimes[0], 1.0));
				}
				catch AnyExceptionSilent

				if	(mStartTimes.GetSize () > 1)
				{
					IMemInputPinPtr				lTransport (lDownstreamPin);
					IMemAllocatorPtr			lDownstreamAllocator;
					tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
					tS <ALLOCATOR_PROPERTIES>	lActualMem;

					if	(
							(lMemInputPin != NULL)
						&&	(SUCCEEDED (lMemInputPin->GetAllocator (&lDownstreamAllocator)))
						)
					{
						lDownstreamAllocator->GetProperties (lRequiredMem.Clear());
					}

					lRequiredMem.cbAlign = max (lRequiredMem.cbAlign, 1);
					lRequiredMem.cbBuffer = max (lRequiredMem.cbBuffer, 256);
					lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, 2);

					if	(
							(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&mCacheAllocator))))
						&&	(SUCCEEDED (LogVfwErr (LogNormal, mCacheAllocator->SetProperties (&lRequiredMem, lActualMem.Clear()))))
						&&	(SUCCEEDED (LogVfwErr (LogNormal, mCacheAllocator->Commit ())))
						)
					{
#ifdef	_DEBUG_SAMPLES
						LogMemAllocator (_DEBUG_SAMPLES, mCacheAllocator, _T("[%s] [%p] Cache Allocator"), mFilterName, this);
#endif
						mCacheDoneEvent.ResetEvent ();
						RegisterWaitForSingleObject (&mCacheDoneWaitHandle, mCacheDoneEvent, StreamCachedSamples, this, INFINITE, WT_EXECUTEDEFAULT);
					}
					else
					{
						mCacheAllocator = NULL;
					}
				}
			}
		}
#ifdef	_DEBUG_CUES
		else
		{
			LogMessage (_DEBUG_CUES, _T("[%s] [%p] BeginInput <no cues>"), mFilterName, this);
		}
#endif
	}
	catch AnyExceptionDebug

	return S_OK;
}

HRESULT CDirectShowDelayEcho::EndInput ()
{
	CSingleLock	lLock (&mCriticalSection, TRUE);

	try
	{
		mSampleCache.RemoveAll ();
		mCacheAllocator = NULL;
	}
	catch AnyExceptionDebug

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowDelayEcho::CacheSample (IMediaSample * pSample)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mCriticalSection, TRUE);

	try
	{
		IMediaSamplePtr	lDuplicate;
		REFERENCE_TIME	lStartTime;
		REFERENCE_TIME	lEndTime;

		if	(
				(SUCCEEDED (DuplicateSample (pSample, &lDuplicate)))
			&&	(lDuplicate != NULL)
			)
		{
#ifdef	_DEBUG_SAMPLES
			LogSample (_DEBUG_SAMPLES, lDuplicate, _T("[%s] [%p] Cache Sample"), mFilterName, this);
#endif
			if	(mSampleCache.GetSize() <= 0)
			{
				lDuplicate->SetDiscontinuity (TRUE);
			}
			mSampleCache.Add (lDuplicate.Detach ());

			if	(
					(SUCCEEDED (pSample->GetTime (&lStartTime, &lEndTime)))
				&&	(((((lEndTime+MsPer100Ns-1)/MsPer100Ns)*MsPer100Ns) - lStartTime) >= (mEndTimes[0] - mStartTimes[0]))
				)
			{
				lResult = VFW_S_NO_MORE_ITEMS;
			}
			else
			{
				lResult = S_OK;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowDelayEcho::DuplicateSample (IMediaSample * pSample, IMediaSample ** pDuplicate)
{
	HRESULT			lResult;
	REFERENCE_TIME	lStartTime;
	REFERENCE_TIME	lEndTime;
	tMediaTypePtr	lMediaType;
	long			lDataSize;
	long			lDataLength;
	LPBYTE			lBuffer;
	IMediaSamplePtr	lDuplicate;
	long			lDuplicateSize;
	LPBYTE			lDuplicateBuffer;

	(*pDuplicate) = NULL;

	if	(
			(mCacheAllocator != NULL)
		&&	(lDataSize = pSample->GetSize ())
		&&	(lDataLength = pSample->GetActualDataLength ())
		&&	(SUCCEEDED (lResult = pSample->GetTime (&lStartTime, &lEndTime)))
		&&	(SUCCEEDED (lResult = pSample->GetMediaType (lMediaType.Free())))
		&&	(SUCCEEDED (lResult = mCacheAllocator->GetBuffer (&lDuplicate, &lStartTime, &lEndTime, AM_GBF_NOWAIT)))
		&&	(lDuplicate != NULL)
		)
	{
		if	(
				(lDuplicateSize = lDuplicate->GetSize ())
			&&	(lDuplicateSize >= lDataSize)
			&&	(SUCCEEDED (lResult = lDuplicate->SetTime (&lStartTime, &lEndTime)))
			&&	(SUCCEEDED (lResult = lDuplicate->SetMediaType (lMediaType)))
			&&	(SUCCEEDED (lResult = pSample->GetPointer (&lBuffer)))
			&&	(SUCCEEDED (lResult = lDuplicate->GetPointer (&lDuplicateBuffer)))
			)
		{
			memcpy (lDuplicateBuffer, lBuffer, lDataLength);
			if	(SUCCEEDED (lResult = lDuplicate->SetActualDataLength (lDataLength)))
			{
				(*pDuplicate) = lDuplicate.Detach ();
			}
		}
	}
	return lResult;
}

void CALLBACK CDirectShowDelayEcho::StreamCachedSamples (PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	CDirectShowDelayEcho *	lThis = (CDirectShowDelayEcho*)lpParameter;

	if	(!TimerOrWaitFired)
	{
#ifdef	_DEBUG_SAMPLES
	LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] StreamCachedSamples"), lThis->mFilterName, lThis);
#endif
		try
		{
			IBaseFilterPtr	lFilter;
			FILTER_STATE	lFilterState;
			IPinPtr			lOutputPin;
			IPinPtr			lDownstreamPin;
			IMemInputPinPtr	lTransport;
			long			lCueNdx = 0;

			{
				CSingleLock	lLock (&lThis->mCriticalSection, TRUE);
				try
				{
					lFilter = lThis->mGrabberFilter;

					if	(
							(lCueNdx <= lThis->mStartTimes.GetUpperBound ())
						&&	(SUCCEEDED (GetFilterPins (lThis->mGrabberFilter, NULL, &lOutputPin)))
						&&	(lOutputPin != NULL)
						&&	(SUCCEEDED (lOutputPin->ConnectedTo (&lDownstreamPin)))
						&&	(lDownstreamPin != NULL)
						)
					{
						lInputPin = lDownstreamPin;
					}
				}
				catch AnyExceptionSilent
			}

			while	(
						(lInputPin != NULL)
					&&	(SUCCEEDED (lFilter->GetState (1, &lFilterState)))
					&&	(lFilterState != State_Stopped)
					)
			{
				long			lCacheNdx = -1;
				IMediaSamplePtr	lCachedSample;
				REFERENCE_TIME	lCueStartTime;
				REFERENCE_TIME	lCueEndTime;
				REFERENCE_TIME	lSampleStartTime;
				REFERENCE_TIME	lSampleEndTime;

				lCueNdx++;

				do
				{
					lCachedSample = NULL;
					lCacheNdx++;

					{
						CSingleLock	lLock (&lThis->mCriticalSection, TRUE);
						try
						{
							if	(lCueNdx <= lThis->mStartTimes.GetUpperBound ())
							{
								lCueStartTime = lThis->mStartTimes [lCueNdx];
								lCueEndTime = lThis->mEndTimes [lCueNdx];
								if	(lCacheNdx <= lThis->mSampleCache.GetUpperBound ())
								{
									lThis->DuplicateSample (lThis->mSampleCache [lCacheNdx], &lCachedSample);
								}
							}
							else
							{
								lInputPin = NULL;
							}
						}
						catch AnyExceptionDebug
					}

					if	(
							(lCachedSample != NULL)
						&&	(SUCCEEDED (lCachedSample->GetTime (&lSampleStartTime, &lSampleEndTime)))
						)
					{
						try
						{
							if	(lCacheNdx == 0)
							{
#ifdef	_DEBUG_CUES
								LogMessage (_DEBUG_CUES, _T("[%s] [%p] Send NewSegment [%f - %f]"), lThis->mFilterName, lThis, RefTimeSec(lCueStartTime), RefTimeSec(lCueEndTime));
#endif
								LogVfwErr (LogNormal, lDownstreamPin->NewSegment (lCueStartTime, lCueEndTime, 1.0));
							}

							lSampleStartTime += lCueStartTime;
							lSampleEndTime += lCueStartTime;
							LogVfwErr (LogNormal, lCachedSample->SetTime (&lSampleStartTime, &lSampleEndTime));
#ifdef	_DEBUG_SAMPLES
							LogSample (_DEBUG_SAMPLES, lCachedSample, _T("[%s] [%p] FromCache [%f - %f] Delay [%f]"), lThis->mFilterName, lThis, RefTimeSec(lSampleStartTime-lCueStartTime), RefTimeSec(lSampleEndTime-lCueStartTime), RefTimeSec(lCueStartTime));
#endif
							LogVfwErr (LogNormal, lInputPin->Receive (lCachedSample));
						}
						catch AnyExceptionSilent
					}
				}	while (lCachedSample != NULL);

				if	(lInputPin == NULL)
				{
#ifdef	_DEBUG_CUES
					LogFilterPin (_DEBUG_CUES, lDownstreamPin, _T("[%s] [%p] Send EndOfStream"), lThis->mFilterName, lThis);
#endif
					LogVfwErr (LogNormal, lDownstreamPin->EndOfStream ());
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowDelayEcho::XSampleDelay::SampleCB (double SampleTime, IMediaSample *pSample)
{
	METHOD_PROLOGUE(CDirectShowDelayEcho, SampleDelay)

	CSingleLock	lLock (&pThis->mCriticalSection, TRUE);

	try
	{
		REFERENCE_TIME	lStartTime;
		REFERENCE_TIME	lEndTime;
		REFERENCE_TIME	lDelayTime;
		HRESULT			lCacheResult;

		if	(
				(pThis->mStartTimes.GetSize() > 0)
			&&	(SUCCEEDED (LogVfwErr (LogNormal, pSample->GetTime (&lStartTime, &lEndTime))))
			)
		{
			if	(pThis->mStartTimes.GetSize() > 1)
			{
				lCacheResult = pThis->CacheSample (pSample);
			}
			lDelayTime = pThis->mStartTimes [0];
			lStartTime += lDelayTime;
			lEndTime += lDelayTime;
			LogVfwErr (LogNormal, pSample->SetTime (&lStartTime, &lEndTime));

#ifdef	_DEBUG_SAMPLES
			LogSample (_DEBUG_SAMPLES, pSample, _T("[%s] [%p] SampleCB [%f] [%f - %f] Delay [%f]"), pThis->mFilterName, pThis, SampleTime, RefTimeSec(lStartTime-lDelayTime), RefTimeSec(lEndTime-lDelayTime), RefTimeSec(lDelayTime));
#endif
			if	(lCacheResult == VFW_S_NO_MORE_ITEMS)
			{
				pThis->mCacheDoneEvent.SetEvent ();
#ifdef	_DEBUG_SAMPLES
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Cache Done"), pThis->mFilterName, pThis);
#endif
			}
		}
	}
	catch AnyExceptionSilent

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDirectShowDelayEcho::XSampleDelay::BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen)
{
	METHOD_PROLOGUE(CDirectShowDelayEcho, SampleDelay)
#ifdef	_TRACE_SAMPLES
	LogMessage (_TRACE_SAMPLES, _T("[%f] [%s] === BufferCB [%d] ==="), SampleTime, pThis->mFilterName, BufferLen);
#endif
	return S_OK;
}
#endif