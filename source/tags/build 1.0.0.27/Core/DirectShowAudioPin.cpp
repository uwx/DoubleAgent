#include "StdAfx.h"
#include "DirectShowAudioPin.h"
#include "DirectSoundDelay.h"
#include "DirectShowFilter.h"
#include "AgentFile.h"
#ifdef	_DEBUG
#include "DirectShowTrace.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE	LogNormal
//#define	_DEBUG_NOFILTERCACHE
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectShowAudioPin, CDirectShowPinPull)

CDirectShowAudioPin::CDirectShowAudioPin (CDirectShowFilter & pFilter, long pSoundNdx)
:	CDirectShowPinPull (pFilter),
	mFilterPins (pFilter.mOutputPins),
	mFilterChain (pFilter.GetFilterGraph ()),
	mSoundNdx (pSoundNdx),
	mSoundDuration (0)
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowAudioPin::CDirectShowAudioPin (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	mName.Format (_T("Sound (%d)"), mSoundNdx);
	mFilterPins.Add (this);
}

CDirectShowAudioPin::~CDirectShowAudioPin ()
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectShowAudioPin::~CDirectShowAudioPin (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	mFilterPins.Remove (this);

	if	(
			(mWaveParser != NULL)
		&&	(mAudioRender != NULL)
		&&	(mFilterChain != NULL)
		)
	{
		try
		{
			LogVfwErr (LogNormal, mFilterChain->StopChain (mWaveParser, mAudioRender));
#ifdef	_DEBUG_NOFILTERCACHE			
			LogVfwErr (LogNormal, mFilterChain->RemoveChain (mWaveParser, mAudioRender));
#else			
			LogVfwErr (LogNormal, MoveFiltersToCache (mWaveParser, mAudioRender, mFilterChain, mDelayFilter->SafeGetFilter(), NULL));
#endif			
		}
		catch AnyExceptionSilent
	}

	try
	{
		if	(mConnection != NULL)
		{
			LogVfwErr (LogNormal, mConnection->Disconnect ());
			LogVfwErr (LogNormal, Disconnect ());
		}
	}
	catch AnyExceptionSilent

	SafeFreeSafePtr (mAudioRender);
	SafeFreeSafePtr (mWaveParser);
	SafeFreeSafePtr (mDelayFilter);
	SafeFreeSafePtr (mFilterChain);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowAudioPin::ConnectFilters (LPCVOID pSound, long pSoundSize)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		IGraphBuilderPtr	lGraphBuilder (mFilter.GetFilterGraph ());
		CString				lFilterName;
		tMediaTypePtr		lMediaType;
		IPinPtr				lWaveInPin;
		IPinPtr				lWaveOutPin;
		IPinPtr				lRenderInPin;

		SetInputBuffer (pSound, pSoundSize);

		if	(lMediaType.Attach (GetSoundMediaType (pSound, pSoundSize)))
		{
			mMediaTypes.RemoveAll ();
			mMediaTypes.Add (lMediaType.Detach());
		}
		
		if	(
				(SUCCEEDED (lResult))
			&&	(mWaveParser == NULL)
			)
		{
			lFilterName.Format (_T("Wave Parser (%d)"), mSoundNdx);

#ifdef	_DEBUG_NOFILTERCACHE			
			if	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(WaveParser), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mWaveParser))))
#else
			LogComErr (LogNormal, GetFilterFromCache (__uuidof(WaveParser), lGraphBuilder, &mWaveParser));
			if	(
					(mWaveParser != NULL)
				||	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(WaveParser), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mWaveParser))))
				)
#endif			
			{
				if	(FAILED (lResult = LogVfwErr (LogNormal, lGraphBuilder->AddFilter (mWaveParser, lFilterName))))
				{
					SafeFreeSafePtr (mWaveParser);
				}
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(mAudioRender == NULL)
			)
		{
			lFilterName.Format (_T("Audio Render (%d)"), mSoundNdx);

#ifdef	_DEBUG_NOFILTERCACHE			
			if	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_DSoundRender, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mAudioRender))))
#else
			LogComErr (LogNormal, GetFilterFromCache (CLSID_DSoundRender, lGraphBuilder, &mAudioRender));
			if	(
					(mAudioRender != NULL)
				||	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_DSoundRender, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mAudioRender))))
				)
#endif			
			{
#ifdef	_DEBUG_NOT			
				mAudioRender = (new CTraceFilter(mAudioRender, _T("AudioRender"), LogIfActive))->GetControllingUnknown();
#endif
				if	(FAILED (lResult = LogVfwErr (LogNormal, lGraphBuilder->AddFilter (mAudioRender, lFilterName))))
				{
					SafeFreeSafePtr (mAudioRender);
				}
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(mWaveParser != NULL)
			&&	(mAudioRender != NULL)
			&&	(!mDelayFilter)
			)
		{
			if	(
					(SUCCEEDED (lResult = GetFilterPins (mWaveParser, &lWaveInPin, NULL)))
				&&	(lWaveInPin != NULL)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, lGraphBuilder->ConnectDirect (lWaveInPin, GetInterface(), NULL))))
				&&	(SUCCEEDED (lResult = GetFilterPins (mWaveParser, NULL, &lWaveOutPin)))
				&&	(SUCCEEDED (lResult = GetFilterPins (mAudioRender, &lRenderInPin, NULL)))
				&&	(lWaveOutPin != NULL)
				&&	(lRenderInPin != NULL)
				)
			{
				lResult = LogVfwErr (LogNormal, lGraphBuilder->Connect (lWaveOutPin, lRenderInPin));
#ifdef	_DEBUG_NOT
				(new CTraceSamples ())->ConnectBefore (lGraphBuilder, mAudioRender).LogSamples();
#endif
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(mSoundDuration <= 0)
			)
		{
			IMediaSeekingPtr	lMediaSeeking (mAudioRender);

			if	(lMediaSeeking != NULL)
			{
				LogVfwErr (LogNormal, lMediaSeeking->GetDuration (&mSoundDuration));
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(mWaveParser != NULL)
			&&	(mAudioRender != NULL)
			&&	(!mDelayFilter)
			)
		{
			lFilterName.Format (_T("Delay (%d)"), mSoundNdx);

			if	(
					(mDelayFilter = new CDirectSoundDelay (lFilterName))
//				&&	(SUCCEEDED (lResult = GetFilterPins (mWaveParser, NULL, &lWaveOutPin)))
//				&&	(lWaveOutPin != NULL)
//				&&	(SUCCEEDED (lResult = mDelayFilter->ConnectAfter (lGraphBuilder, mWaveParser, lWaveOutPin)))
				&&	(SUCCEEDED (lResult = GetFilterPins (mAudioRender, &lRenderInPin, NULL)))
				&&	(lRenderInPin != NULL)
				&&	(SUCCEEDED (lResult = mDelayFilter->ConnectBefore (lGraphBuilder, mAudioRender, lRenderInPin)))
				)
			{
#ifdef	_DEBUG_NOT
				LogFilter (LogDebug, mWaveParser, false, _T("Connected"));
				LogFilter (LogDebug, mAudioRender, false, _T("Connected"));
				LogFilter (LogDebug, mDelayFilter->GetFilter(), false, _T("Connected"));
#endif
			}
			else
			if	(SUCCEEDED (lResult))
			{
				lResult = E_FAIL;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowAudioPin::CueSound (REFERENCE_TIME pStartTime)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);
	
	try
	{
		if	(
				(mSoundDuration > 0)
			&&	(mDelayFilter.Ptr())
			)
		{
			lResult = mDelayFilter->AddSoundCue (pStartTime);
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowAudioPin::BeginOutput ()
{
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			if	(mDelayFilter.Ptr())
			{
				mDelayFilter->Enable (false);
				mDelayFilter->ClearSoundCues ();
			}
		}
		catch AnyExceptionSilent
	}
	return CDirectShowPinPull::BeginOutput ();
}

HRESULT CDirectShowAudioPin::EndOutput ()
{
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			if	(mDelayFilter.Ptr())
			{
				mDelayFilter->Enable (false);
				mDelayFilter->ClearSoundCues ();
			}
		}
		catch AnyExceptionSilent
	}
	return CDirectShowPinPull::EndOutput ();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowAudioPin::BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	HRESULT		lResult = CDirectShowPinPull::BeginOutputStream (pStartTime, pEndTime, pRate);
	CSingleLock	lLock (&mStateLock, TRUE);
	
	try
	{
		IPinPtr				lWaveOutPin;
		IMediaSeekingPtr	lMediaSeeking;
		REFERENCE_TIME		lStartTime = 0;
		REFERENCE_TIME		lStopTime = mSoundDuration;
		
		if	(mDelayFilter.Ptr())
		{
			mDelayFilter->Enable (true);
		}
		
		if	(
				(mSoundDuration > 0)
			&&	(mWaveParser != NULL)
			&&	(SUCCEEDED (lResult = GetFilterPins (mWaveParser, NULL, &lWaveOutPin))) 
			)
		{
			lMediaSeeking = lWaveOutPin;
			
			if	(lMediaSeeking == NULL)
			{
				lResult = E_NOINTERFACE;
			}
			else
			{
				lResult = LogVfwErr (LogNormal, lMediaSeeking->SetPositions (&lStartTime, AM_SEEKING_AbsolutePositioning|AM_SEEKING_Segment, &lStopTime, AM_SEEKING_IncrementalPositioning|AM_SEEKING_Segment));
			}
		}
	}
	catch AnyExceptionDebug
	
	return lResult;
}
