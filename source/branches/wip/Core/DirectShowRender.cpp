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
#include <uuids.h>
#include "DaCore.h"
#include "DirectShowRender.h"
#include "ImageTools.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#include "UseGdiPlus.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#include "LocalizeEx.h"
#endif

#pragma warning (disable: 4355)

/////////////////////////////////////////////////////////////////////////////
/*
	This is a simple frame rendering filter.  It's used in place of any of
	the existing DirectShow video rendering filters for a couple of reasons.

	First, the DirectShow renderers use DirectDraw surfaces for rendering.
	This causes a problem when the DirectDraw surface is not configured
	for transparency.  The DirectShow Video Mixing Renderer (VMR) does
	support transparency, but only for overlays, and it does not support
	stand-alone overlays.

	Second, the DirectDraw surface used by the DirectShow VMR is more-or-less
	independent of the GDI.  The surface is clipped to a containing window,
	but there are slight latencies in the synchronization and that can have
	some weird effects (like when you minimize a video window and the video
	stays visible longer than the window).

	We could mess with the VMR's DirectDraw surface to get it to support
	transparency, but it's just easier to make our own video renderer
	that simply BLTs frames to the window using GDI.  We use small frames
	and a low frame rate so there are no performance issues.

	Also, this allows us to implement some rudimentary image processing
	(such as supersampled smoothing).
*/
/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
//#define	_DEBUG_CONNECTION	LogNormal
//#define	_DEBUG_ALLOCATOR	LogNormal
//#define	_DEBUG_STREAM		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM_EX	LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_SAMPLES		LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_MISSED_SAMPLES		LogNormal|LogHighVolume
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_DirectShowFilter"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
//#define	_TRACE_RESOURCES	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
//#define	_TRACE_RESOURCES_EX	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

#ifndef	_LOG_MISSED_SAMPLES
#define	_LOG_MISSED_SAMPLES		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

CDirectShowRender::CDirectShowRender()
:	mRenderWnd (NULL),
	mSmoothing (0),
	mSourceRect (0,0,0,0),
	mRenderRect (0,0,0,0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowRender::CDirectShowRender (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowRender::~CDirectShowRender()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowRender::~CDirectShowRender (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

HRESULT CDirectShowRender::FinalConstruct ()
{
	InitMediaSeeking (*this, *this, 0, AM_SEEKING_CanGetCurrentPos | AM_SEEKING_CanGetStopPos | AM_SEEKING_CanDoSegments);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

const GUID & CDirectShowRender::GetClassID ()
{
	return GUID_NULL;
}

HRESULT CDirectShowRender::SetFilterName (LPCWSTR pFilterName)
{
	mFilterName = pFilterName;
	return S_OK;
}

CAtlString CDirectShowRender::GetFilterName ()
{
	return mFilterName;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::GetSeekingTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime)
{
	GetTimes (pCurrTime, pStopTime);
}

bool CDirectShowRender::GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking)
{
	return CDirectShowFilter::GetUpstreamSeeking (pMediaSeeking);
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::InitializePins ()
{
	tMediaTypePtr		lMediaType;
	VIDEOINFOHEADER *	lVideoInfo;
	CAtlString			lPinName;

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::InitializePins"), this);
	}
#endif
	if	(mBkColor)
	{
		lPinName = _T("RGB32");
	}
	else
	{
		lPinName = _T("ARGB32");
	}

	if	(
			(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), sizeof(VIDEOINFOHEADER))))
		&&	(mInputPin = new CComObjectNoLock <CDirectShowPinIn>)
		)
	{
		mInputPin->AddRef ();
		mInputPin->Initialize (*this, _T("Animation In"), lPinName, 16);

		lMediaType->majortype = MEDIATYPE_Video;
		if	(mBkColor)
		{
			lMediaType->subtype = MEDIASUBTYPE_RGB32;
		}
		else
		{
			lMediaType->subtype = MEDIASUBTYPE_ARGB32;
		}
		lMediaType->formattype = FORMAT_VideoInfo;
		lMediaType->bFixedSizeSamples = FALSE;
		lMediaType->bTemporalCompression = FALSE;
		lMediaType->lSampleSize = 0;

		lVideoInfo = (VIDEOINFOHEADER*)lMediaType->pbFormat;
		memset (lVideoInfo, 0, sizeof (VIDEOINFOHEADER));
		lVideoInfo->bmiHeader.biSize = sizeof (lVideoInfo->bmiHeader);
		lVideoInfo->bmiHeader.biBitCount = 32;
		lVideoInfo->bmiHeader.biPlanes = 1;
		lVideoInfo->bmiHeader.biCompression = BI_RGB;

		mInputPin->mMediaTypes.Add (lMediaType.Detach());
		mInputPins.Add (mInputPin);
	}
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::InitializePins Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::OnJoinedFilterGraph ()
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnJoinedFilterGraph"), this);
	}
#endif

	CDirectShowFilter::OnJoinedFilterGraph ();
	SetTimes (0, GetDuration());

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnJoinedFilterGraph Done"), this);
	}
#endif
}

void CDirectShowRender::OnLeftFilterGraph ()
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnLeftFilterGraph"), this);
	}
#endif
	try
	{
		IMediaEventSinkPtr	lEventSink (mFilterGraph);

		if	(
				(IsWindow (mRenderWnd))
			&&	(lEventSink != NULL)
			)
		{
			LogVfwErr (LogNormal, lEventSink->Notify (EC_WINDOW_DESTROYED, (LONG_PTR)(IBaseFilter*)this, 0));
		}
	}
	catch AnyExceptionSilent

	CDirectShowFilter::OnLeftFilterGraph ();
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnLeftFilterGraph Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowRender::OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState)
{
	HRESULT	lResult = CDirectShowFilter::OnStateChanged (pOldState, pNewState);

#ifdef	_LOG_DIRECT_SHOW
	if	(
			(pNewState == State_Stopped)
		&&	(pOldState != State_Stopped)
		)
	{
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Render <Stopped> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
	else
	if	(
			(pNewState == State_Paused)
		&&	(pOldState == State_Stopped)
		)
	{
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Render <Started> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
	else
	if	(
			(pNewState == State_Running)
		&&	(pOldState == State_Paused)
		)
	{
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Render <Running> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
#endif
	return lResult;
}

void CDirectShowRender::OnPinConnected (CDirectShowPin * pPin)
{
	VIDEOINFOHEADER *	lVideoInfo;
	IMediaEventSinkPtr	lEventSink;

	if	(
			(pPin == mInputPin)
		&&	(pPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pPin->mMediaType->pbFormat)
		)
	{
		mSourceRect.SetRect (0, 0, lVideoInfo->bmiHeader.biWidth, lVideoInfo->bmiHeader.biHeight);
	}

	if	(
			(IsWindow (mRenderWnd))
		&&	(pPin == mInputPin)
		)
	{
		lEventSink = mInputPin->SafeGetConnection ();

		if	(lEventSink != NULL)
		{
			LogVfwErr (LogNormal, lEventSink->Notify (EC_NOTIFY_WINDOW, (LONG_PTR)mRenderWnd, 0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::OnStartInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
#ifdef	_DEBUG_SAMPLES
	LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] OnStartInputStream [%f - %f] [%s (%u %u)]"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(pStartTime), RefTimeSec(pEndTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnStartInputStream"), this);
	}
#endif

	CDirectShowFilter::OnStartInputStream (pStartTime, pEndTime, pRate);

#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnStartInputStream Done"), this);
	}
#endif
}

void CDirectShowRender::OnEndInputStream (INT_PTR pPendingSamples)
{
#ifdef	_DEBUG_SAMPLES
	LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] OnEndInputStream [%d] [%s (%u %u)]"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)), pPendingSamples, FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnEndInputStream"), this);
	}
#endif

	CDirectShowFilter::OnEndInputStream (pPendingSamples);

	if	(pPendingSamples <= 0)
	{
#ifdef	_LOG_DIRECT_SHOW
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Render <EndOfStream> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
		try
		{
			IMediaEventSinkPtr	lEventSink (mFilterGraph);

			if	(lEventSink != NULL)
			{
				LogVfwErr (LogNormal, lEventSink->Notify (EC_COMPLETE, S_OK, 0));
			}
		}
		catch AnyExceptionSilent
	}
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnEndInputStream Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::OnClockPulse ()
{
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnClockPulse"), this);
	}
#endif
	HANDLE	lThread = GetCurrentThread ();
	int		lThreadPriority = GetThreadPriority (lThread);

	SetThreadPriority (lThread, THREAD_PRIORITY_HIGHEST);

	try
	{
		HRESULT			lResult;
		REFERENCE_TIME	lReferenceTime = GetReferenceTime ();
		REFERENCE_TIME	lStreamTime = GetStreamTime(mState);
		REFERENCE_TIME	lSampleTime;
		REFERENCE_TIME	lNextSampleTime;
		IMediaSamplePtr	lSample;

#ifdef	_DEBUG_STREAM_EX
		LogMessage (_DEBUG_STREAM_EX, _T("[%s] [%p] [%f] OnClockPulse [%s]"), AtlTypeName(this), this, RefTimeSec(lStreamTime), FilterStateStr(mState));
#endif
		lResult = GetInputSample (lStreamTime, lSample, lSampleTime, lNextSampleTime);

		if	(
				(lResult == S_OK)
			&&	(lNextSampleTime <= lStreamTime)
			)
		{
			mCurrTime = lSampleTime;

			if	(GetSampleImage (lSample))
			{
#ifdef	_DEBUG_SAMPLES
				LogMediaSampleId (_DEBUG_SAMPLES, lSample, _T("[%s] [%p] [%f] GotSampleImage [%s] for [%p]"), AtlTypeName(this), this, RefTimeSec(lStreamTime), FormatSize(mImageBuffer.GetBitmapSize()), mRenderWnd);
#endif
				DrawSampleImage ();
			}
			lResult = GetNextSampleTime (lStreamTime, lNextSampleTime);
		}

		if	(
				(lResult == S_OK)
			&&	(mState == State_Running)
			&&	(lStreamTime >= 0)
			&&	(lNextSampleTime > lStreamTime)
			)
		{
#ifdef	_DEBUG_SAMPLES_NOT
			LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Delay [%f] until [%f]"), AtlTypeName(this), this, RefTimeSec(lStreamTime), RefTimeSec(lNextSampleTime-lStreamTime), RefTimeSec(lNextSampleTime));
#endif
			SetClock (lReferenceTime, lNextSampleTime - GetStreamTime(mState));
		}
		else
		if	(lResult == VFW_S_NO_MORE_ITEMS)
		{
			SetTimes (mStopTime, mStopTime);
			StopClock ();
			mInputPin->EndInputStream ();
#ifdef	_DEBUG_STREAM
			LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] EndOfStream"), AtlTypeName(this), this, RefTimeSec(lStreamTime));
#endif
#ifdef	_LOG_DIRECT_SHOW
			LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Render <Complete> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
		}
		else
		if	(
				(mState != State_Stopped)
			&&	(!IsClockStarted ())
			)
		{
#ifdef	_LOG_DIRECT_SHOW
			LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Render <Start> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
			StartClock (sDefaultClockInterval);
		}
	}
	catch AnyExceptionDebug

	SetThreadPriority (lThread, lThreadPriority);
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnClockPulse Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowRender::GetNextSampleTime (REFERENCE_TIME pStreamTime, REFERENCE_TIME & pNextSampleTime)
{
	HRESULT		lResult = E_UNEXPECTED;
	CLockMutex	lLock (mDataLock);

	try
	{
		IMediaSamplePtr	lSample;
		REFERENCE_TIME	lSampleStartTime;
		REFERENCE_TIME	lSampleEndTime;

		pNextSampleTime = -1;

		if	(
				(mInputPin)
			&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mInputPin->PeekInputSample (&lSample))))
			&&	(lSample != NULL)
			&&	(SUCCEEDED (lSample->GetTime (&lSampleStartTime, &lSampleEndTime)))
			)
		{
			pNextSampleTime = lSampleStartTime;
			lResult = S_OK;
		}
		else
		if	(
				(SUCCEEDED (lResult))
			&&	(lResult != VFW_S_NO_MORE_ITEMS)
			)
		{
			lResult = S_FALSE;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowRender::GetInputSample (REFERENCE_TIME pStreamTime, IMediaSamplePtr & pSample, REFERENCE_TIME & pSampleTime, REFERENCE_TIME & pNextSampleTime)
{
	HRESULT		lResult = E_UNEXPECTED;
	CLockMutex	lLock (mDataLock);

	try
	{
		REFERENCE_TIME	lSampleStartTime;
		REFERENCE_TIME	lSampleEndTime;
		REFERENCE_TIME	lStreamEndTime = (pStreamTime - MsPer100Ns*100); // Allow samples to be up 100 milliseconds late

		pSampleTime = -1;
		pNextSampleTime = -1;

		if	(mInputPin)
		{
			while	(
						(SUCCEEDED (LogVfwErr (LogDetails, lResult = mInputPin->PeekInputSample (&pSample))))
					&&	(pSample != NULL)
					&&	(SUCCEEDED (pSample->GetTime (&lSampleStartTime, &lSampleEndTime)))
					)
			{
#ifdef	_DEBUG_STREAM_EX
				LogMessage (_DEBUG_STREAM_EX, _T("[%s] [%p] [%f] PeekSample [%f - %f]"), AtlTypeName(this), this, RefTimeSec(pStreamTime), RefTimeSec(lSampleStartTime), RefTimeSec(lSampleEndTime));
#endif
				if	(
						(lSampleEndTime < lStreamEndTime)
					&&	(lSampleStartTime > 0)						// Always allow the first sample, even if it's late
					&&	(lSampleEndTime < mStopTime)	// Always allow the last sample, even if it's late
					&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mInputPin->GetInputSample (&pSample))))
					)
				{
#ifdef	_LOG_MISSED_SAMPLES
					if	(LogIsActive())
					{
						LogMessage (_LOG_MISSED_SAMPLES|LogHighVolume|LogTimeMs, _T("[%s] [%p] [%f] Discard late sample [%f - %f] Cache [%d] Priority [%d]"), AtlTypeName(this), this, RefTimeSec(pStreamTime), RefTimeSec(lSampleStartTime), RefTimeSec(lSampleEndTime), mInputPin->GetCachedSampleCount(), GetThreadPriority(GetCurrentThread()));
					}
#endif
					SafeFreeSafePtr (pSample);
				}
				else
				{
					break;
				}
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(lResult != VFW_S_NO_MORE_ITEMS)
				)
			{
				lResult = S_FALSE;
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(lResult != VFW_S_NO_MORE_ITEMS)
				&&	(pSample != NULL)
				)
			{
				pSampleTime = lSampleStartTime;

				if	(mState == State_Running)
				{
					if	(lSampleStartTime > pStreamTime)
					{
#ifdef	_DEBUG_STREAM_EX
						LogMessage (_DEBUG_STREAM_EX, _T("[%s] [%p] [%f] Delay sample [%f - %f]"), AtlTypeName(this), this, RefTimeSec(pStreamTime), RefTimeSec(lSampleStartTime), RefTimeSec(lSampleEndTime));
#endif
						pNextSampleTime = lSampleStartTime;
						lResult = S_OK;
					}
					else
					if	(
							(lSampleEndTime >= lStreamEndTime)
						&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mInputPin->GetInputSample (&pSample))))
						&&	(pSample != NULL)
						)
					{
						if	(SUCCEEDED (pSample->GetTime (&lSampleStartTime, &lSampleEndTime)))
						{
							pSampleTime = lSampleStartTime;
						}
						lResult = S_OK;
					}
				}
			}

#ifdef	_DEBUG_STREAM
			if	(lResult == VFW_S_NO_MORE_ITEMS)
			{
				LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] NoMoreItems"), AtlTypeName(this), this, RefTimeSec(pStreamTime));
			}
#endif
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowRender::GetSampleImage (IMediaSample * pSample)
{
	bool		lRet = false;
	CLockMutex	lLock (mDataLock);

	try
	{
		VIDEOINFOHEADER *	lVideoInfo;
		LPBYTE				lSampleBuffer = NULL;
		long				lSampleSize;

		if	(
				(mInputPin)
			&&	(mInputPin->mMediaType.Ptr())
			&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
			&&	(lSampleSize = pSample->GetActualDataLength ())
			&&	(SUCCEEDED (pSample->GetPointer (&lSampleBuffer)))
			&&	(lSampleBuffer)
			)
		{
			CSize	lImageSize (lVideoInfo->bmiHeader.biWidth, lVideoInfo->bmiHeader.biHeight);

			if	(
					(mImageBuffer.GetImageSize () == lImageSize)
				||	(mImageBuffer.CreateBuffer (lImageSize, true))
				)
			{
				mImageBuffer.EndBuffer ();
				memcpy (GetImageBits (*mImageBuffer.mImage), lSampleBuffer, lSampleSize);
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::DrawSampleImage (HDC pDC, const RECT *pTargetRect)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mDataLock);

	try
	{
		CSize	lImageSize = mImageBuffer.GetImageSize ();
		bool	lImageHasAlpha = false;
		CRect	lTargetRect;
		HDC		lRenderDC;

		if	(
				(mImageBuffer.GetImage ())
			&&	(
					(lRenderDC = pDC)
				||	(
						(IsWindow (mRenderWnd))
					&&	(lRenderDC = ::GetDC (mRenderWnd))
					)
				)
			)
		{
			if	(
					(mInputPin)
				&&	(mInputPin->mMediaType)
				&&	(IsEqualGUID (mInputPin->mMediaType->subtype, MEDIASUBTYPE_ARGB32))
				)
			{
				lImageHasAlpha = true;
			}
			if	(pTargetRect)
			{
				lTargetRect.CopyRect (pTargetRect);
			}
			else
			if	(IsWindow (mRenderWnd))
			{
				::GetClientRect (mRenderWnd, &lTargetRect);
			}
			else
			{
				lTargetRect.SetRect (0, 0, lImageSize.cx, lImageSize.cy);
			}

			if	(lImageHasAlpha)
			{
				bool				lUpdateLayered = false;
				tPtr <CImageBuffer>	lWorkBuffer;
				BLENDFUNCTION		lBlend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

				if	(
						(WindowFromDC (lRenderDC) == mRenderWnd)
					&&	(GetWindowLong (mRenderWnd, GWL_EXSTYLE) & WS_EX_LAYERED)
					)
				{
					lUpdateLayered = true;
				}

				if	(
						(lWorkBuffer = ScaleImage (lImageSize, lTargetRect))
					||	(lWorkBuffer = SmoothImage (lImageSize, lTargetRect))
					)
				{
					if	(lUpdateLayered)
					{
						::UpdateLayeredWindow (mRenderWnd, lRenderDC, NULL, &lTargetRect.Size(), *lWorkBuffer->mDC, &CPoint (0,0), 0, &lBlend, ULW_ALPHA);
					}
					else
					{
						::AlphaBlend (lRenderDC, lTargetRect.left, lTargetRect.top, lTargetRect.Width(), lTargetRect.Height(), *lWorkBuffer->mDC, 0, 0, lImageSize.cx, lImageSize.cy, lBlend);
					}
				}
				else
				if	(mImageBuffer.StartBuffer ())
				{
					if	(
							(lUpdateLayered)
						&&	(lTargetRect.Size() != lImageSize)
						&&	(lWorkBuffer = new CImageBuffer)
						&&	(lWorkBuffer->CreateBuffer (lTargetRect.Size(), true))
						)
					{
						::SetStretchBltMode (*lWorkBuffer->mDC, COLORONCOLOR);
						::StretchBlt (*lWorkBuffer->mDC, 0, 0, lTargetRect.Width(), lTargetRect.Height(), mImageBuffer.GetDC(), 0, 0, lImageSize.cx, lImageSize.cy, SRCCOPY);
						::UpdateLayeredWindow (mRenderWnd, lRenderDC, NULL, &lTargetRect.Size(), *lWorkBuffer->mDC, &CPoint (0,0), 0, &lBlend, ULW_ALPHA);
					}
					else
					if	(lUpdateLayered)
					{
						::UpdateLayeredWindow (mRenderWnd, lRenderDC, NULL, &lImageSize, mImageBuffer.GetDC(), &CPoint (0,0), 0, &lBlend, ULW_ALPHA);
					}
					else
					{
						::AlphaBlend (lRenderDC, lTargetRect.left, lTargetRect.top, lTargetRect.Width(), lTargetRect.Height(), mImageBuffer.GetDC(), 0, 0, lImageSize.cx, lImageSize.cy, lBlend);
					}
				}
#ifdef	_DEBUG_SAMPLES
				else
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Image buffer [%d %d] failed"), AtlTypeName(this), this, lImageSize.cx, lImageSize.cy);
				}
#endif
			}
			else
			{
				if	(mImageBuffer.StartBuffer ())
				{
					if	(lTargetRect.Size() == lImageSize)
					{
						::BitBlt (lRenderDC, lTargetRect.left, lTargetRect.top, lImageSize.cx, lImageSize.cy, mImageBuffer.GetDC(), 0, 0, SRCCOPY);
					}
					else
					{
						::SetStretchBltMode (lRenderDC, HALFTONE);
						::StretchBlt (lRenderDC, lTargetRect.left, lTargetRect.top, lTargetRect.Width(), lTargetRect.Height(), mImageBuffer.GetDC(), 0, 0, lImageSize.cx, lImageSize.cy, SRCCOPY);
					}
				}
#ifdef	_DEBUG_SAMPLES
				else
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Image buffer [%d %d] failed"), AtlTypeName(this), this, lImageSize.cx, lImageSize.cy);
				}
#endif
			}
			mImageBuffer.EndBuffer ();

			if	(
					(lRenderDC)
				&&	(lRenderDC != pDC)
				)
			{
				::ReleaseDC (mRenderWnd, lRenderDC);
			}
			lResult = S_OK;
		}
#ifdef	_DEBUG_SAMPLES
		else
		{
			LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Unable to get Render DC"), AtlTypeName(this), this);
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

CImageBuffer * CDirectShowRender::ScaleImage (const CSize & pImageSize, const CRect & pTargetRect)
{
	tPtr <CImageBuffer>	lTargetBuffer;

	if	(
			(mImageBuffer.GetImage())
		&&	(pTargetRect.Size () != pImageSize)
		&&	(lTargetBuffer = new CImageBuffer)
		&&	(lTargetBuffer->CreateBuffer (pTargetRect.Size(), true))
		)
	{
		Gdiplus::Bitmap		lBitmap (pImageSize.cx, pImageSize.cy, pImageSize.cx*4, PixelFormat32bppPARGB, GetImageBits(*mImageBuffer.mImage));
		Gdiplus::Graphics	lGraphics (*lTargetBuffer->mDC);

		lGraphics.Clear (Gdiplus::Color (0,0,0,0));
		lGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
		lGraphics.SetCompositingQuality (Gdiplus::CompositingQualityHighQuality);
		lGraphics.SetInterpolationMode (Gdiplus::InterpolationModeHighQualityBilinear);
		lGraphics.SetPixelOffsetMode (Gdiplus::PixelOffsetModeHighQuality);
		lGraphics.DrawImage (&lBitmap, 0, pTargetRect.Height(), pTargetRect.Width(), -pTargetRect.Height());

		return lTargetBuffer.Detach ();
	}
	return NULL;
}

CImageBuffer * CDirectShowRender::SmoothImage (const CSize & pImageSize, const CRect & pTargetRect)
{
	tPtr <CImageBuffer>	lTargetBuffer;

	if	(
			(mSmoothing & (RenderSmoothEdges|RenderSmoothAll))
		&&	(mImageBuffer.GetImage())
		&&	(pTargetRect.Size () == pImageSize)
		&&	(lTargetBuffer = new CImageBuffer)
		&&	(lTargetBuffer->CreateBuffer (pTargetRect.Size(), true))
		)
	{
		lTargetBuffer->EndBuffer ();

		LPBYTE	lSrcBits = GetImageBits (*mImageBuffer.mImage);
		LPBYTE	lDstBits = GetImageBits (*lTargetBuffer->mImage);
		long	lBitmapPitch = abs(mImageBuffer.mImage->GetPitch());
		CPoint	lSrcPixel;
		CPoint	lDstPixel;
		long	lDstNdx;
		long	lSrcNdx;
		bool	lEdgesOnly = ((mSmoothing & RenderSmoothAll) != RenderSmoothAll);

		for	(lDstPixel.y = 0; lDstPixel.y < pImageSize.cy; lDstPixel.y++)
		{
			lDstNdx = (pImageSize.cy-lDstPixel.y-1) * lBitmapPitch;

			for	(lDstPixel.x = 0; lDstPixel.x < pImageSize.cx; lDstPixel.x++, lDstNdx += 4)
			{
				if	(
						(lEdgesOnly)
					&&	(lSrcBits [lDstNdx+3])
					)
				{
					*(PDWORD)(lDstBits+lDstNdx) = *(PDWORD)(lSrcBits+lDstNdx); 
				}
				else
				{
					ULONG	lCount = 0;
					ULONG	lAlpha = 0;
					ULONG	lRed = 0;
					ULONG	lGreen = 0;
					ULONG	lBlue = 0;

					for	(lSrcPixel.y = max (lDstPixel.y-1, 0); lSrcPixel.y < min (lDstPixel.y+2, pImageSize.cy); lSrcPixel.y++)
					{
						lSrcNdx = (pImageSize.cy-lSrcPixel.y-1) * lBitmapPitch;

						for	(lSrcPixel.x = max (lDstPixel.x-1, 0), lSrcNdx += lSrcPixel.x*4; lSrcPixel.x < min (lDstPixel.x+2, pImageSize.cx); lSrcPixel.x++, lSrcNdx += 4)
						{
							const ULONG	lNearWeight = 6;
							const ULONG	lEdgeWeight = 3;
							ULONG		lWeight = 1;
							
							if	(lSrcPixel.x == lDstPixel.x)
							{
								lWeight *= lNearWeight;
							}
							else
							if	(
									(lSrcBits [lSrcNdx+3])
								&&	(!lSrcBits [lDstNdx+3])
								)
							{
								lWeight *= lEdgeWeight;
							}

							if	(lSrcPixel.y == lDstPixel.y)
							{
								lWeight *= lNearWeight;
							}
							else
							if	(
									(lSrcBits [lSrcNdx+3])
								&&	(!lSrcBits [lDstNdx+3])
								)
							{
								lWeight *= lEdgeWeight;
							}

							if	(lSrcBits [lSrcNdx+3])
							{
								lBlue += (ULONG)lSrcBits [lSrcNdx] * lWeight;
								lGreen += (ULONG)lSrcBits [lSrcNdx+1] * lWeight;
								lRed += (ULONG)lSrcBits [lSrcNdx+2] * lWeight;
								lAlpha += (ULONG)lSrcBits [lSrcNdx+3] * lWeight;
							}
							
							if	(
									(lSrcBits [lDstNdx+3])
								?	(lSrcBits [lSrcNdx+3])
								:	(true)
								)
							{
								lCount += lWeight;
							}
						}
					}

					if	(lCount > 0)					
					{
						lDstBits [lDstNdx] = (BYTE)(lBlue/lCount); 
						lDstBits [lDstNdx+1] = (BYTE)(lGreen/lCount); 
						lDstBits [lDstNdx+2] = (BYTE)(lRed/lCount); 
						lDstBits [lDstNdx+3] = (BYTE)(lAlpha/lCount); 
					}
				}
			}
		}
		
		lTargetBuffer->StartBuffer ();
		return lTargetBuffer.Detach ();
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetRenderWnd (HWND *pRenderWnd)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(pRenderWnd)
		{
			(*pRenderWnd) = mRenderWnd;
		}
		if	(mRenderWnd)
		{
			lResult = S_OK;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetRenderWnd (HWND pRenderWnd)
{
	HRESULT		lResult = S_OK;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(mRenderWnd)
		{
			lResult = E_UNEXPECTED;
		}
		else
		if	(!IsWindow (pRenderWnd))
		{
			lResult = E_INVALIDARG;
		}
		else
		{
			mRenderWnd = pRenderWnd;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetBkColor (COLORREF *pBkColor)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(pBkColor)
		{
			if	(mBkColor)
			{
				(*pBkColor) = (*mBkColor);
			}
			else
			{
				(*pBkColor) = 0;
			}
		}
		if	(mBkColor)
		{
			lResult = S_OK;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetBkColor (const COLORREF *pBkColor)
{
	HRESULT		lResult = S_OK;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(pBkColor)
		{
			mBkColor = new COLORREF;
			*mBkColor = *pBkColor;
			mUseGdiplus = NULL;
		}
		else
		{
			mBkColor = NULL;
			mUseGdiplus = new CUseGdiplus;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetSmoothing (DWORD *pSmoothing)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(!pSmoothing)
		{
			lResult = E_POINTER;
		}
		else
		{
			(*pSmoothing) = mSmoothing;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetSmoothing (DWORD pSmoothing)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(mSmoothing != pSmoothing)
		{
			lResult = S_OK;
		}
		mSmoothing = pSmoothing;
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetImageSize (long *pImageWidth, long *pImageHeight)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		CSize	lImageSize (0,0);

		if	(mImageBuffer.mImage)
		{
			lImageSize = mImageBuffer.GetImageSize ();
			lResult = S_OK;
		}
		if	(pImageWidth)
		{
			(*pImageWidth) = lImageSize.cx;
		}
		if	(pImageHeight)
		{
			(*pImageHeight) = lImageSize.cy;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_AvgTimePerFrame (REFTIME *pAvgTimePerFrame)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pAvgTimePerFrame)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		*pAvgTimePerFrame = RefTimeSec (lVideoInfo->AvgTimePerFrame);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_BitRate (long *pBitRate)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pBitRate)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		*pBitRate = lVideoInfo->dwBitRate;
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_BitErrorRate (long *pBitErrorRate)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pBitErrorRate)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		*pBitErrorRate = lVideoInfo->dwBitErrorRate;
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_VideoWidth (long *pVideoWidth)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pVideoWidth)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		*pVideoWidth = lVideoInfo->bmiHeader.biWidth;
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_VideoHeight (long *pVideoHeight)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pVideoHeight)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		*pVideoHeight = lVideoInfo->bmiHeader.biHeight;
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_SourceLeft (long SourceLeft)
{
	HRESULT	lResult = S_OK;

	mSourceRect.OffsetRect (SourceLeft - mSourceRect.left, 0);
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_SourceLeft (long *pSourceLeft)
{
	HRESULT	lResult = S_OK;

	if	(!pSourceLeft)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceLeft = mSourceRect.left;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_SourceWidth (long SourceWidth)
{
	HRESULT	lResult = S_OK;

	if	(SourceWidth < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		mSourceRect.right = mSourceRect.left + SourceWidth;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_SourceWidth (long *pSourceWidth)
{
	HRESULT	lResult = S_OK;

	if	(!pSourceWidth)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceWidth = mSourceRect.Width();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_SourceTop (long SourceTop)
{
	HRESULT	lResult = S_OK;

	mSourceRect.OffsetRect (0, SourceTop - mSourceRect.top);
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_SourceTop (long *pSourceTop)
{
	HRESULT	lResult = S_OK;

	if	(!pSourceTop)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceTop = mSourceRect.top;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_SourceHeight (long SourceHeight)
{
	HRESULT	lResult = S_OK;

	if	(SourceHeight < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		mSourceRect.bottom = mSourceRect.top + SourceHeight;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_SourceHeight (long *pSourceHeight)
{
	HRESULT	lResult = S_OK;

	if	(!pSourceHeight)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceHeight = mSourceRect.Height ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_DestinationLeft (long DestinationLeft)
{
	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	mSourceRect.OffsetRect (DestinationLeft - mSourceRect.left, 0);
	if	(IsWindow (mRenderWnd))
	{
		::GetClientRect (mRenderWnd, &lClientRect);
		::IntersectRect (&mRenderRect, &mRenderRect, &lClientRect);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_DestinationLeft (long *pDestinationLeft)
{
	HRESULT	lResult = S_OK;

	if	(!pDestinationLeft)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationLeft = mRenderRect.left;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_DestinationWidth (long DestinationWidth)
{
	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	if	(DestinationWidth < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		mRenderRect.right = mRenderRect.left + DestinationWidth;
		if	(IsWindow (mRenderWnd))
		{
			::GetClientRect (mRenderWnd, &lClientRect);
			::IntersectRect (&mRenderRect, &mRenderRect, &lClientRect);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_DestinationWidth (long *pDestinationWidth)
{
	HRESULT	lResult = S_OK;

	if	(!pDestinationWidth)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationWidth = mRenderRect.Width();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_DestinationTop (long DestinationTop)
{
	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	mRenderRect.OffsetRect (0, DestinationTop - mRenderRect.top);
	if	(IsWindow (mRenderWnd))
	{
		::GetClientRect (mRenderWnd, &lClientRect);
		::IntersectRect (&mRenderRect, &mRenderRect, &lClientRect);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_DestinationTop (long *pDestinationTop)
{
	HRESULT	lResult = S_OK;

	if	(!pDestinationTop)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationTop = mRenderRect.top;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::put_DestinationHeight (long DestinationHeight)
{
	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	if	(DestinationHeight < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		mRenderRect.bottom = mRenderRect.top + DestinationHeight;
		if	(IsWindow (mRenderWnd))
		{
			::GetClientRect (mRenderWnd, &lClientRect);
			::IntersectRect (&mRenderRect, &mRenderRect, &lClientRect);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::get_DestinationHeight (long *pDestinationHeight)
{
	HRESULT	lResult = S_OK;

	if	(!pDestinationHeight)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationHeight = mRenderRect.Height();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetSourcePosition (long Left, long Top, long Width, long Height)
{
	HRESULT	lResult = S_OK;

	if	(
			(Width < 0)
		||	(Height < 0)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		mSourceRect.SetRect (Left, Top, Left+Width, Top+Height);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetSourcePosition (long *pLeft, long *pTop, long *pWidth, long *pHeight)
{
	HRESULT	lResult = S_OK;

	if	(pLeft)
	{
		*pLeft = mSourceRect.left;
	}
	if	(pTop)
	{
		*pTop = mSourceRect.top;
	}
	if	(pWidth)
	{
		*pWidth = mSourceRect.Width();
	}
	if	(pHeight)
	{
		*pHeight = mSourceRect.Height();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetDefaultSourcePosition (void)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		mSourceRect.SetRect (0, 0, lVideoInfo->bmiHeader.biWidth, lVideoInfo->bmiHeader.biHeight);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetDestinationPosition (long Left, long Top, long Width, long Height)
{
	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	if	(
			(Width < 0)
		||	(Height < 0)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		mRenderRect.SetRect (Left, Top, Left+Width, Top+Height);
		if	(IsWindow (mRenderWnd))
		{
			::GetClientRect (mRenderWnd, &lClientRect);
			::IntersectRect (&mRenderRect, &mRenderRect, &lClientRect);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetDestinationPosition (long *pLeft, long *pTop, long *pWidth, long *pHeight)
{
	HRESULT	lResult = S_OK;

	if	(pLeft)
	{
		*pLeft = mRenderRect.left;
	}
	if	(pTop)
	{
		*pTop = mRenderRect.top;
	}
	if	(pWidth)
	{
		*pWidth = mRenderRect.Width();
	}
	if	(pHeight)
	{
		*pHeight = mRenderRect.Height();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::SetDefaultDestinationPosition (void)
{
	HRESULT	lResult = S_OK;

	if	(IsWindow (mRenderWnd))
	{
		::GetClientRect (mRenderWnd, &mRenderRect);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetVideoSize (long *pWidth, long *pHeight)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		if	(pWidth)
		{
			*pWidth = lVideoInfo->bmiHeader.biWidth;
		}
		if	(pHeight)
		{
			*pHeight = lVideoInfo->bmiHeader.biHeight;
		}
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetVideoPaletteEntries (long StartIndex, long Entries, long *pRetrieved, long *pPalette)
{
	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		lResult = E_NOTIMPL;
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::GetCurrentImage (long *pBufferSize,long *pDIBImage)
{
	HRESULT	lResult = E_NOTIMPL;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::IsUsingDefaultSource (void)
{
	HRESULT				lResult = S_FALSE;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(mInputPin)
		&&	(mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)mInputPin->mMediaType->pbFormat)
		)
	{
		if	(
				(mSourceRect.left == 0)
			&&	(mSourceRect.top == 0)
			&&	(mSourceRect.Width() == lVideoInfo->bmiHeader.biWidth)
			&&	(mSourceRect.Height() == lVideoInfo->bmiHeader.biHeight)
			)
		{
			lResult = S_OK;
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::IsUsingDefaultDestination (void)
{
	HRESULT	lResult = S_FALSE;
	CRect	lClientRect;

	if	(IsWindow (mRenderWnd))
	{
		::GetClientRect (mRenderWnd, &lClientRect);
		if	(::EqualRect (&mRenderRect, &lClientRect))
		{
			lResult = S_OK;
		}
	}
	return lResult;
}
