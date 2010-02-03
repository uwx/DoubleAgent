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
#include "DirectSoundPinPush.h"
#include "DirectSoundConvert.h"
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
#pragma warning (disable: 4355)

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE			LogNormal
//#define	_DEBUG_CUES				LogNormal
//#define	_DEBUG_NOFILTERCACHE
//#define	_LOG_FAILED_FORMATS		LogNormal
#endif

#ifndef	_LOG_FAILED_FORMATS
#define	_LOG_FAILED_FORMATS	LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectSoundPinPush, CDirectShowPinOut)
	INTERFACE_PART(CDirectSoundPinPush, __uuidof(IMediaSeeking), MediaSeeking)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDirectSoundPinPush, CDirectShowPinOut)

CDirectSoundPinPush::CDirectSoundPinPush (CDirectShowFilter & pFilter, CDirectSoundConvertCache & pConvertCache, long pSoundNdx)
:	CDirectShowPinOut (pFilter),
	CDirectShowSeeking (*(CCmdTarget*)this, pFilter),
	mFilterPins (pFilter.mOutputPins),
	mSoundNdx (pSoundNdx),
	mConvertCache (pConvertCache)
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectSoundPinPush::CDirectSoundPinPush (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	mSeekingCaps |= AM_SEEKING_CanDoSegments;
	mName.Format (_T("Sound (%d)"), mSoundNdx);
	mFilterPins.Add (this);
}

CDirectSoundPinPush::~CDirectSoundPinPush ()
{
#ifdef	_DEBUG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_DEBUG_INSTANCE, _T("[%p] CDirectSoundPinPush::~CDirectSoundPinPush (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	NotGatedInstance (this);
	DisconnectFilters (false);
	mFilterPins.Remove (this);
}

/////////////////////////////////////////////////////////////////////////////

LPUNKNOWN CDirectSoundPinPush::GetInterfaceHook(const void* iid)
{
	return CCmdTarget::GetInterfaceHook (iid); // Ensure that IMediaSeeking is not delegated to the filter.
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundPinPush::ConvertSound (LPCVOID pSound, long pSoundSize)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		CDirectSoundConvert *	lConvert;
		LPVOID					lBuffer = NULL;
		ULONG					lBufferSize = 0;
		ULONG					lDataLength = 0;
		tPtr <WAVEFORMATEX>		lInputFormat;
		LPWAVEFORMATEX			lOutputFormat;
		long					lSoundFormatSize;

		if	(
				(lConvert = mConvertCache.GetCachedConvert (mSoundNdx))
			&&	(lConvert->GetInputBuffer ((LPCVOID&)lBuffer, lBufferSize))
			&&	(
					(lBuffer != pSound)
				||	(lBufferSize != pSoundSize)
				)
			)
		{
			mConvertCache.RemovedCachedConvert (mSoundNdx);
		}

		if	(
				(!mConvertCache.GetCachedConvert (mSoundNdx))
			&&	(lConvert = new CDirectSoundConvert)
			)
		{
			mConvertCache.CacheSoundConvert (lConvert, mSoundNdx);
		}

		if	(lConvert = mConvertCache.GetCachedConvert (mSoundNdx))
		{
			if	(!lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength))
			{
				lInputFormat = GetSoundFormat (pSound, pSoundSize, &lSoundFormatSize, LogNormal);
				lConvert->SetInputFormat (lInputFormat);
				lConvert->SetInputBuffer (pSound, pSoundSize);

				if	(
						(
							(SUCCEEDED (lResult = LogComErr (LogNormal, lConvert->DeriveOutputFormat (), _T("%s format"), mName)))
						&&	(SUCCEEDED (lResult = LogComErr (LogNormal, lConvert->ConvertSound (), _T("%s convert"), mName)))
						)
					||	(SUCCEEDED (lResult = LogComErr (LogNormal, lConvert->StashSound (), _T("%s stash"), mName)))
					)
				{
					lConvert->GetOutputBuffer (lBuffer, lBufferSize, lDataLength);
				}
				else
				{
					mConvertCache.RemovedCachedConvert (mSoundNdx);
				}
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(lConvert = mConvertCache.GetCachedConvert (mSoundNdx))
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
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundPinPush::ConnectFilters ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		IGraphBuilderPtr	lGraphBuilder (mFilter.GetFilterGraph ());
		CString				lFilterName;
		IPinPtr				lRenderInPin;

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
			&&	(mAudioRender != NULL)
			)
		{
			if	(
					(SUCCEEDED (lResult = GetFilterPins (mAudioRender, &lRenderInPin, NULL)))
				&&	(lRenderInPin != NULL)
				)
			{
				if	(FAILED (lResult = LogVfwErr (LogNormal, lGraphBuilder->Connect (GetInterface(), lRenderInPin))))
				{
#ifdef	_LOG_FAILED_FORMATS
					LogFilterPin (_LOG_FAILED_FORMATS, GetInterface(), true, _T("AudioOut -> RenderIn"));
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
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectSoundPinPush::DisconnectFilters (bool pCacheUnusedFilter)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(
				(mAudioRender != NULL)
			&&	(mFilter.GetFilterGraph ())
			)
		{
			try
			{
				IFilterGraphPtr	lFilterGraph (mFilter.GetFilterGraph ());

				LogVfwErr (LogNormal, mAudioRender->Stop ());
#ifdef	_DEBUG_NOFILTERCACHE
				LogVfwErr (LogNormal, lResult = lFilterGraph->RemoveFilter (mAudioRender));
#else
				if	(pCacheUnusedFilter)
				{
					LogVfwErr (LogNormal, lResult = MoveFilterToCache (mAudioRender, lFilterGraph));
				}
				else
				{
					LogVfwErr (LogNormal, lResult = lFilterGraph->RemoveFilter (mAudioRender));
				}
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
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		mCueTimes.Add (pStartTime);
#ifdef	_DEBUG_CUES
		LogMessage (_DEBUG_CUES, _T("[%s] CueSound [%d] at [%f]"), mName, mCueTimes.GetUpperBound(), RefTimeSec(pStartTime));
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectSoundPinPush::StreamCuedSound (INT_PTR pCueNdx)
{
	HRESULT					lResult = S_FALSE;
	CDirectSoundConvert *	lConvert;
	LPVOID					lBuffer = NULL;
	ULONG					lBufferSize = 0;
	ULONG					lDataLength = 0;
	REFERENCE_TIME			lStartTime = 0;
	REFERENCE_TIME			lEndTime = 0;

	if	(lConvert = mConvertCache.GetCachedConvert (mSoundNdx))
	{
		CSingleLock	lLock (&mDataLock, TRUE);

		try
		{
			if	(
					(pCueNdx >= 0)
				&&	(pCueNdx <= mCueTimes.GetUpperBound ())
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
					(SUCCEEDED (lResult = GetOutputSample (&lSample, &lStartTime, &lEndTime, 0)))
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

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LONGLONG CDirectSoundPinPush::GetDuration ()
{
	LONGLONG	lRet = 0;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		CDirectSoundConvert *	lConvert;
		LPVOID					lBuffer = NULL;
		ULONG					lBufferSize = 0;
		ULONG					lDataLength = 0;
		LPWAVEFORMATEX			lSoundFormat;

		if	(
				(lConvert = mConvertCache.GetCachedConvert (mSoundNdx))
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

	if	(
			(SUCCEEDED (lResult = CDirectShowPinOut::BeginOutputStream (pStartTime, pEndTime, pRate)))
		&&	(SUCCEEDED (lResult = StreamCuedSound (0)))
		&&	(mCueTimes.GetSize() > 1)
		)
	{
		QueueUserWorkItem (StreamProc, PutGatedInstance (this), WT_EXECUTELONGFUNCTION);
	}
	return lResult;
}

DWORD WINAPI CDirectSoundPinPush::StreamProc (LPVOID pThreadParameter)
{
	CDirectSoundPinPush *	lThis = NULL;
	HRESULT					lResult = S_FALSE;
	INT_PTR					lCueNdx = 1;

	while	(LockGatedInstance (pThreadParameter, lThis, 100))
	{
		try
		{
			lResult = lThis->StreamCuedSound (lCueNdx);
		}
		catch AnyExceptionDebug

		FreeGatedInstance (pThreadParameter, lThis);

		if	(lResult == S_OK)
		{
			lCueNdx++;
		}
		else
		{
			break;
		}
	}
	return 0;
}

