/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DirectSoundPinPush.h"
#include "DirectSoundConvert.h"
#include "DirectShowFilter.h"
#include "AgentFile.h"
#ifdef	_DEBUG
#include "DirectShowTrace.h"
#include "DebugProcess.h"
#endif

#pragma warning (disable: 4355)

#ifdef	_DEBUG
//#define	_DEBUG_CUES				LogNormal
//#define	_LOG_FAILED_FORMATS		LogNormal
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
//#define	_TRACE_THREADS			(GetProfileDebugInt(_T("TraceThreads"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE				(GetProfileDebugInt(_T("LogInstance_DirectShowPin"),LogVerbose,true)&0xFFFF|LogTime)
#endif

#define	_NO_FILTER_CACHE

#ifndef	_LOG_FAILED_FORMATS
#define	_LOG_FAILED_FORMATS	LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

CDirectSoundPinPush::CDirectSoundPinPush ()
:	mFilterPins (NULL),
	mSoundNdx (-1),
	mConvertCache (NULL),
	mCueAsyncStart (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectSoundPinPush::CDirectSoundPinPush [%p] (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), mConvertCache, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectSoundPinPush::~CDirectSoundPinPush ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectSoundPinPush::~CDirectSoundPinPush [%p] (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), mConvertCache, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	NotGatedInstance<CDirectSoundPinPush> (this);
	DisconnectFilters (false);
	if	(mFilterPins)
	{
		mFilterPins->Remove (this);
	}
}

/////////////////////////////////////////////////////////////////////////////

CDirectSoundPinPush & CDirectSoundPinPush::Initialize (CDirectShowFilter & pFilter, CDirectSoundConvertCache & pConvertCache, long pSoundNdx)
{
	CDirectShowPinOut::Initialize (pFilter);
	InitMediaSeeking (*this, pFilter, 0, AM_SEEKING_CanDoSegments);

	mFilterPins = &pFilter.mOutputPins;
	const_cast <long &> (mSoundNdx) = pSoundNdx;
	mConvertCache = &pConvertCache;

	mName.Format (_T("Sound (%d)"), mSoundNdx);
	mFilterPins->Add (this);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectSoundPinPush::Initialize [%p] (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), mConvertCache, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundPinPush::ConvertSound (LPCVOID pSound, long pSoundSize)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mDataLock);

	try
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::ConvertSound"), this);
		}
#endif
		CDirectSoundConvert *	lConvert;
		LPVOID					lBuffer = NULL;
		ULONG					lBufferSize = 0;
		ULONG					lDataLength = 0;
		tPtr <WAVEFORMATEX>		lInputFormat;
		LPWAVEFORMATEX			lOutputFormat;
		long					lSoundFormatSize;

		if	(
				(lConvert = mConvertCache->GetCachedConvert (mSoundNdx))
			&&	(lConvert->GetInputBuffer ((LPCVOID&)lBuffer, lBufferSize))
			&&	(
					(lBuffer != pSound)
				||	(lBufferSize != pSoundSize)
				)
			)
		{
			mConvertCache->RemovedCachedConvert (mSoundNdx);
		}

		if	(
				(!mConvertCache->GetCachedConvert (mSoundNdx))
			&&	(lConvert = new CDirectSoundConvert)
			)
		{
			mConvertCache->CacheSoundConvert (lConvert, mSoundNdx);
		}

		if	(lConvert = mConvertCache->GetCachedConvert (mSoundNdx))
		{
			if	(!lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength))
			{
				lInputFormat = GetSoundFormat (pSound, pSoundSize, &lSoundFormatSize, LogNormal);
				lConvert->SetInputFormat (lInputFormat);
				lConvert->SetInputBuffer (pSound, pSoundSize);

				if	(
						(
							(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, lConvert->DeriveOutputFormat (), _T("%s format"), mName)))
						&&	(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, lConvert->ConvertSound (), _T("%s convert"), mName)))
						)
					||	(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, lConvert->StashSound (), _T("%s stash"), mName)))
					)
				{
					lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength);
				}
				else
				{
					mConvertCache->RemovedCachedConvert (mSoundNdx);
				}
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(lConvert = mConvertCache->GetCachedConvert (mSoundNdx))
			&&	(lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength))
			&&	(lConvert->GetOutputFormat (lOutputFormat))
			)
		{
			tMediaTypePtr	lMediaType;

			lSoundFormatSize = sizeof(WAVEFORMATEX) + lOutputFormat->cbSize;

			if	(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), lSoundFormatSize)))
			{
				lMediaType->majortype = MEDIATYPE_Audio;
				lMediaType->subtype = MEDIASUBTYPE_PCM;
				lMediaType->formattype = FORMAT_WaveFormatEx;
				lMediaType->bFixedSizeSamples = FALSE;
				lMediaType->bTemporalCompression = FALSE;
				lMediaType->lSampleSize = lDataLength;
				memcpy (lMediaType->pbFormat, lOutputFormat, lSoundFormatSize);

				mMediaTypes.RemoveAll ();
				mMediaTypes.Add (lMediaType.Detach());
			}
		}
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::ConvertSound Done"), this);
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundPinPush::ConnectFilters ()
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::ConnectFilters"), this);
		}
#endif
		IGraphBuilderPtr	lGraphBuilder (mFilter->GetFilterGraph ());
		CAtlString			lFilterName;
		IPinPtr				lRenderInPin;

		if	(
				(SUCCEEDED (lResult))
			&&	(mAudioRender == NULL)
			)
		{
			lFilterName.Format (_T("Audio Render (%d)"), mSoundNdx);

#ifdef	_NO_FILTER_CACHE
			if	(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_DSoundRender, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mAudioRender))))
#else
			LogComErr (LogNormal|LogTime, GetFilterFromCache (CLSID_DSoundRender, lGraphBuilder, &mAudioRender));
			if	(
					(mAudioRender != NULL)
				||	(SUCCEEDED (lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_DSoundRender, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &mAudioRender))))
				)
#endif
			{
#ifdef	_DEBUG_NOT
				mAudioRender = (new CTraceFilter(mAudioRender, _T("AudioRender"), LogIfActive))->GetControllingUnknown();
#endif
				if	(FAILED (lResult = LogVfwErr (LogNormal|LogTime, lGraphBuilder->AddFilter (mAudioRender, lFilterName))))
				{
					SafeFreeSafePtr (mAudioRender);
				}
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(mAudioRender != NULL)
			)
		{
			if	(
					(SUCCEEDED (lResult = GetFilterPins (mAudioRender, &lRenderInPin, NULL)))
				&&	(lRenderInPin != NULL)
				)
			{
				if	(FAILED (lResult = LogVfwErr (LogNormal|LogTime, lGraphBuilder->Connect (this, lRenderInPin))))
				{
#ifdef	_LOG_FAILED_FORMATS
					LogFilterPin (_LOG_FAILED_FORMATS, this, true, _T("AudioOut -> RenderIn"));
#endif
				}
#ifdef	_DEBUG_NOT
				else
				{
					(new CTraceSamples (_T("Trace ")+lFilterName))->ConnectBefore (lGraphBuilder, mAudioRender).LogSamples();
				}
#endif
			}

			if	(FAILED (lResult))
			{
				lGraphBuilder->RemoveFilter (mAudioRender);
				SafeFreeSafePtr (mAudioRender);
			}
		}
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::ConnectFilters Done"), this);
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectSoundPinPush::DisconnectFilters (bool pCacheUnusedFilter)
{
	NotGatedInstance<CDirectSoundPinPush> (this);

	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::DisconnectFilters"), this);
		}
#endif
		if	(
				(mAudioRender != NULL)
			&&	(mFilter->GetFilterGraph ())
			)
		{
			try
			{
				IFilterGraphPtr	lFilterGraph (mFilter->GetFilterGraph ());
				IPinPtr			lRenderInPin;

				LogVfwErr (LogNormal|LogTime, mAudioRender->Stop ());
#ifdef	_NO_FILTER_CACHE
				LogVfwErr (LogNormal|LogTime, lResult = lFilterGraph->RemoveFilter (mAudioRender));
#else
				if	(pCacheUnusedFilter)
				{
					LogVfwErr (LogNormal|LogTime, lResult = MoveFilterToCache (mAudioRender, lFilterGraph));
				}
				else
				{
					LogVfwErr (LogNormal|LogTime, lResult = lFilterGraph->RemoveFilter (mAudioRender));
				}
#endif
			}
			catch AnyExceptionSilent
		}

		try
		{
			if	(mConnection != NULL)
			{
				LogVfwErr (LogNormal|LogTime, mConnection->Disconnect ());
				LogVfwErr (LogNormal|LogTime, Disconnect ());
			}
		}
		catch AnyExceptionSilent

		SafeFreeSafePtr (mAudioRender);
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::DisconnectFilters Done"), this);
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundPinPush::CueSound (REFERENCE_TIME pStartTime)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mDataLock);

	try
	{
		mCueTimes.Add (pStartTime);
#ifdef	_DEBUG_CUES
		LogMessage (_DEBUG_CUES, _T("[%s] CueSound [%d] at [%f]"), mName, mCueTimes.GetCount()-1, RefTimeSec(pStartTime));
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectSoundPinPush::StreamCuedSound (INT_PTR pCueNdx, bool pSynchronous)
{
	HRESULT					lResult = S_FALSE;
	CDirectSoundConvert *	lConvert;
	LPVOID					lBuffer = NULL;
	ULONG					lBufferSize = 0;
	ULONG					lDataLength = 0;
	REFERENCE_TIME			lStartTime = 0;
	REFERENCE_TIME			lEndTime = 0;

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::StreamCuedSound"), this);
	}
#endif
	if	(lConvert = mConvertCache->GetCachedConvert (mSoundNdx))
	{
		CLockMutex	lLock (mDataLock);

		try
		{
			if	(
					(pCueNdx >= 0)
				&&	(pCueNdx < (INT_PTR)mCueTimes.GetCount())
				&&	(lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength))
				)
			{
				lStartTime = mCueTimes [pCueNdx];
				lEndTime = lStartTime + GetDuration () - 1;
			}
		}
		catch AnyExceptionDebug
	}

	if	(lEndTime > lStartTime)
	{
		try
		{
			IMediaSamplePtr	lSample;
			long			lSampleSize;
			LPBYTE			lSampleBuffer;

#ifdef	_DEBUG_CUES
			LogMessage (_DEBUG_CUES, _T("[%s] StreamCuedSound [%d] Start [%f] End [%f] Duration [%f] Length [%u]"), mName, pCueNdx, RefTimeSec(lStartTime), RefTimeSec(lEndTime), RefTimeSec(GetDuration()), lDataLength);
#endif
			SetTimes (lStartTime, lEndTime);

			if	(
					(SUCCEEDED (lResult = GetOutputSample (&lSample, &lStartTime, &lEndTime, pSynchronous?0:AM_GBF_NOWAIT)))
				&&	(lSample != NULL)
				&&	(lSampleSize = lSample->GetSize ())
				&&	(SUCCEEDED (lResult = lSample->GetPointer (&lSampleBuffer)))
				)
			{
				lDataLength = min (lDataLength, lSampleSize);
				memcpy (lSampleBuffer, ((LPBYTE)lBuffer)+lBufferSize-lDataLength, lDataLength);
				lSample->SetTime (&lStartTime, &lEndTime);
				lSample->SetDiscontinuity (TRUE);
				lResult = PutOutputSample (lSample, lDataLength);
			}
			else
			if	(SUCCEEDED (lResult))
			{
				lResult = S_FALSE;
			}
		}
		catch AnyExceptionDebug
	}
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::StreamCuedSound Done"), this);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LONGLONG CDirectSoundPinPush::GetDuration ()
{
	LONGLONG	lRet = 0;
	CLockMutex	lLock (mDataLock);

	try
	{
		CDirectSoundConvert *	lConvert;
		LPVOID					lBuffer = NULL;
		ULONG					lBufferSize = 0;
		ULONG					lDataLength = 0;
		LPWAVEFORMATEX			lSoundFormat;

		if	(
				(lConvert = mConvertCache->GetCachedConvert (mSoundNdx))
			&&	(lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength))
			&&	(lConvert->GetOutputFormat (lSoundFormat))
			)
		{
			lRet = (LONGLONG) ((double)(short)lSoundFormat->wBitsPerSample / 8.0 * (double)lDataLength / (double)(long)lSoundFormat->nSamplesPerSec * Ns100PerSec);
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundPinPush::BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	HRESULT	lResult;
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::BeginOutputStream"), this);
	}
#endif
	NotGatedInstance<CDirectSoundPinPush> (this);

	if	(
			(SUCCEEDED (lResult = CDirectShowPinOut::BeginOutputStream (pStartTime, pEndTime, pRate)))
		&&	(mCueTimes.GetCount() > 0)
		)
	{
		lResult = StreamCuedSound (0, false);
		if	(lResult == S_OK)
		{
			mCueAsyncStart = 1;
		}
		else
		{
			mCueAsyncStart = 0;
		}
		lResult = S_OK;

		if	((INT_PTR)mCueTimes.GetCount() > mCueAsyncStart)
		{
#ifdef	_TRACE_THREADS
			LogMessage (_TRACE_THREADS, _T("CDirectSoundPinPush::QueueUserWorkItem"));
#endif
			QueueUserWorkItem (StreamProc, PutGatedInstance<CDirectSoundPinPush> (this), WT_EXECUTELONGFUNCTION);
		}
	}
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundPinPush::BeginOutputStream Done"), this);
	}
#endif
	return lResult;
}

DWORD WINAPI CDirectSoundPinPush::StreamProc (LPVOID pThreadParameter)
{
	CDirectSoundPinPush *	lThis = NULL;
	HRESULT					lResult = S_FALSE;
	INT_PTR					lCueNdx = 1;

#ifdef	_TRACE_THREADS
	LogMessage (_TRACE_THREADS, _T("CDirectSoundPinPush::StreamProc"));
#endif

	if	(LockGatedInstance<CDirectSoundPinPush> (pThreadParameter, lThis))
	{
		lCueNdx = lThis->mCueAsyncStart;
		FreeGatedInstance<CDirectSoundPinPush> (pThreadParameter, lThis);
	}

	while	(LockGatedInstance<CDirectSoundPinPush> (pThreadParameter, lThis))
	{
		try
		{
			lResult = lThis->StreamCuedSound (lCueNdx, false);
		}
		catch AnyExceptionDebug

		FreeGatedInstance<CDirectSoundPinPush> (pThreadParameter, lThis);

		if	(lResult == S_OK)
		{
			lCueNdx++;
		}
		else
		if	(lResult == VFW_E_TIMEOUT)
		{
			SleepEx (100, TRUE);
		}
		else
		{
			break;
		}
	}

#ifdef	_TRACE_THREADS
	LogMessage (_TRACE_THREADS, _T("CDirectSoundPinPush::StreamProc End"));
#endif
	return 0;
}

