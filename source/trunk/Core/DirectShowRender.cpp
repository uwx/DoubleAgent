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
#include "DirectShowRender.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#define GDIPVER 0x0110
#include "UseGdiPlus.h"
#ifdef	_DEBUG
#include "BitmapDebugger.h"
#include "DebugStr.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
//#define	_DEBUG_COM			LogNormal|LogHighVolume
//#define	_DEBUG_CONNECTION	LogNormal
//#define	_DEBUG_ALLOCATOR	LogNormal
//#define	_DEBUG_STREAM		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM_EX	LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_SAMPLES		LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_MISSED_SAMPLES		LogNormal|LogHighVolume
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_DirectShow"),LogVerbose,true)&0xFFFF)
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

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectShowRender, CDirectShowFilter)
	INTERFACE_PART(CDirectShowRender, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectShowRender, __uuidof(IBasicVideo), BasicVideo)
	INTERFACE_PART(CDirectShowRender, __uuidof(IMediaSeeking), MediaSeeking)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDirectShowRender, BasicVideo)

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CDirectShowRender, CDirectShowFilter)

CDirectShowRender::CDirectShowRender()
:	CDirectShowSeeking (*(CCmdTarget*)this, *(CDirectShowClock*)this),
	mSourceRect (0,0,0,0),
	mRenderRect (0,0,0,0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowRender::CDirectShowRender (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	mSeekingCaps |= AM_SEEKING_CanGetCurrentPos | AM_SEEKING_CanGetStopPos | AM_SEEKING_CanDoSegments;
}

CDirectShowRender::~CDirectShowRender()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowRender::~CDirectShowRender (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
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

CString CDirectShowRender::GetFilterName ()
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
	CString				lPinName;

#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::InitializePins"), this);
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
			(mInputPin = new CDirectShowPinIn (*this, _T("Animation In"), lPinName, 16))
		&&	(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), sizeof(VIDEOINFOHEADER))))
		)
	{
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
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::InitializePins Done"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::OnJoinedFilterGraph ()
{
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnJoinedFilterGraph"), this);
#endif

	CDirectShowFilter::OnJoinedFilterGraph ();
	SetTimes (0, GetDuration());

#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnJoinedFilterGraph Done"), this);
#endif
}

void CDirectShowRender::OnLeftFilterGraph ()
{
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnLeftFilterGraph"), this);
#endif
	try
	{
		IMediaEventSinkPtr	lEventSink (mFilterGraph);

		if	(
				(IsWindow (mRenderWnd))
			&&	(lEventSink != NULL)
			)
		{
			LogVfwErr (LogNormal, lEventSink->Notify (EC_WINDOW_DESTROYED, (LONG_PTR)&m_xBaseFilter, 0));
		}
	}
	catch AnyExceptionSilent

	CDirectShowFilter::OnLeftFilterGraph ();
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowRender::OnLeftFilterGraph Done"), this);
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
#ifdef	_TRACE_RESOURCES_EX
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnStartInputStream"), this);
#endif

	CDirectShowFilter::OnStartInputStream (pStartTime, pEndTime, pRate);

#ifdef	_TRACE_RESOURCES_EX
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnStartInputStream Done"), this);
#endif
}

void CDirectShowRender::OnEndInputStream (INT_PTR pPendingSamples)
{
#ifdef	_TRACE_RESOURCES_EX
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnEndInputStream"), this);
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
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnEndInputStream Done"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowRender::OnClockPulse ()
{
#ifdef	_TRACE_RESOURCES_EX
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnClockPulse"), this);
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
		LogMessage (_DEBUG_STREAM_EX, _T("[%s] [%p] [%f] OnClockPulse [%s]"), ObjClassName(this), this, RefTimeSec(lStreamTime), FilterStateStr(mState));
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
				LogMediaSampleId (_DEBUG_SAMPLES, lSample, _T("[%s] [%p] [%f] GotSampleImage [%s] for [%p]"), ObjClassName(this), this, RefTimeSec(lStreamTime), FormatSize(mImageBuffer.GetBitmapSize()), mRenderWnd);
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
			LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Delay [%f] until [%f]"), ObjClassName(this), this, RefTimeSec(lStreamTime), RefTimeSec(lNextSampleTime-lStreamTime), RefTimeSec(lNextSampleTime));
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
			LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] EndOfStream"), ObjClassName(this), this, RefTimeSec(lStreamTime));
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
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowRender::OnClockPulse Done"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowRender::GetNextSampleTime (REFERENCE_TIME pStreamTime, REFERENCE_TIME & pNextSampleTime)
{
	HRESULT		lResult = E_UNEXPECTED;
	CSingleLock	lLock (&mDataLock, TRUE);

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
	CSingleLock	lLock (&mDataLock, TRUE);

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
				LogMessage (_DEBUG_STREAM_EX, _T("[%s] [%p] [%f] PeekSample [%f - %f]"), ObjClassName(this), this, RefTimeSec(pStreamTime), RefTimeSec(lSampleStartTime), RefTimeSec(lSampleEndTime));
#endif
				if	(
						(lSampleEndTime < lStreamEndTime)
					&&	(lSampleStartTime > 0)				// Always allow the first sample, even if it's late
					&&	(lSampleEndTime < mStopTime)		// Always allow the last sample, even if it's late
					&&	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mInputPin->GetInputSample (&pSample))))
					)
				{
#ifdef	_LOG_MISSED_SAMPLES
					if	(LogIsActive())
					{
						LogMessage (_LOG_MISSED_SAMPLES|LogHighVolume|LogTimeMs, _T("[%s] [%p] [%f] Discard late sample [%f - %f] Cache [%d] Priority [%d]"), ObjClassName(this), this, RefTimeSec(pStreamTime), RefTimeSec(lSampleStartTime), RefTimeSec(lSampleEndTime), mInputPin->GetCachedSampleCount(), GetThreadPriority(GetCurrentThread()));
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
						LogMessage (_DEBUG_STREAM_EX, _T("[%s] [%p] [%f] Delay sample [%f - %f]"), ObjClassName(this), this, RefTimeSec(pStreamTime), RefTimeSec(lSampleStartTime), RefTimeSec(lSampleEndTime));
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
				LogMessage (_DEBUG_STREAM, _T("[%s] [%p] [%f] NoMoreItems"), ObjClassName(this), this, RefTimeSec(pStreamTime));
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
	CSingleLock	lLock (&mDataLock, TRUE);

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
					(mImageBuffer.GetBitmapSize () == lImageSize)
				||	(mImageBuffer.CreateBuffer (lImageSize, true))
				)
			{
				memcpy (mImageBuffer.mBitmapBits, lSampleBuffer, lSampleSize);
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

bool CDirectShowRender::SetBkColor (const COLORREF * pBkColor)
{
	CSingleLock	lLock (&mStateLock, TRUE);

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
	catch AnyExceptionSilent

	return true;
}

const COLORREF * CDirectShowRender::GetBkColor () const
{
	CSingleLock	lLock (&mStateLock, TRUE);
	return mBkColor;
}

CSize CDirectShowRender::GetImageSize () const
{
	CSingleLock	lLock (&mDataLock, TRUE);
	return mImageBuffer.GetBitmapSize ();
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowRender::DrawSampleImage (HDC pDC, const RECT * pTargetRect)
{
	bool		lRet = false;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
		CSize	lImageSize = mImageBuffer.GetBitmapSize ();
		bool	lImageHasAlpha = false;
		CRect	lTargetRect;
		HDC		lRenderDC;

		if	(
				(lRenderDC = pDC)
			||	(
					(IsWindow (mRenderWnd))
				&&	(lRenderDC = ::GetDC (mRenderWnd))
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
				bool					lUpdateLayered = false;
				tPtr <CBitmapBuffer>	lWorkBuffer;
				BLENDFUNCTION			lBlend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

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
						::UpdateLayeredWindow (mRenderWnd, lRenderDC, NULL, &lTargetRect.Size(), lWorkBuffer->mDC, &CPoint (0,0), 0, &lBlend, ULW_ALPHA);
					}
					else
					{
						::AlphaBlend (lRenderDC, lTargetRect.left, lTargetRect.top, lTargetRect.Width(), lTargetRect.Height(), lWorkBuffer->mDC, 0, 0, lImageSize.cx, lImageSize.cy, lBlend);
					}
				}
				else
				if	(mImageBuffer.StartBuffer ())
				{
					if	(
							(lUpdateLayered)
						&&	(lTargetRect.Size() != lImageSize)
						&&	(lWorkBuffer = new CBitmapBuffer)
						&&	(lWorkBuffer->CreateBuffer (lTargetRect.Size(), true))
						)
					{
						::SetStretchBltMode (lWorkBuffer->mDC, COLORONCOLOR);
						::StretchBlt (lWorkBuffer->mDC, 0, 0, lTargetRect.Width(), lTargetRect.Height(), mImageBuffer.mDC, 0, 0, lImageSize.cx, lImageSize.cy, SRCCOPY);
						::UpdateLayeredWindow (mRenderWnd, lRenderDC, NULL, &lTargetRect.Size(), lWorkBuffer->mDC, &CPoint (0,0), 0, &lBlend, ULW_ALPHA);
					}
					else
					if	(lUpdateLayered)
					{
						::UpdateLayeredWindow (mRenderWnd, lRenderDC, NULL, &lImageSize, mImageBuffer.mDC, &CPoint (0,0), 0, &lBlend, ULW_ALPHA);
					}
					else
					{
						::AlphaBlend (lRenderDC, lTargetRect.left, lTargetRect.top, lTargetRect.Width(), lTargetRect.Height(), mImageBuffer.mDC, 0, 0, lImageSize.cx, lImageSize.cy, lBlend);
					}
				}
#ifdef	_DEBUG_SAMPLES
				else
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Image buffer [%d %d] failed"), ObjClassName(this), this, lImageSize.cx, lImageSize.cy);
				}
#endif
			}
			else
			{
				if	(mImageBuffer.StartBuffer ())
				{
					if	(lTargetRect.Size() == lImageSize)
					{
						::BitBlt (lRenderDC, lTargetRect.left, lTargetRect.top, lImageSize.cx, lImageSize.cy, mImageBuffer.mDC, 0, 0, SRCCOPY);
					}
					else
					{
						::SetStretchBltMode (lRenderDC, HALFTONE);
						::StretchBlt (lRenderDC, lTargetRect.left, lTargetRect.top, lTargetRect.Width(), lTargetRect.Height(), mImageBuffer.mDC, 0, 0, lImageSize.cx, lImageSize.cy, SRCCOPY);
					}
				}
#ifdef	_DEBUG_SAMPLES
				else
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Image buffer [%d %d] failed"), ObjClassName(this), this, lImageSize.cx, lImageSize.cy);
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
			lRet = true;
		}
#ifdef	_DEBUG_SAMPLES
		else
		{
			LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] Unable to get Render DC"), ObjClassName(this), this);
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CBitmapBuffer * CDirectShowRender::ScaleImage (const CSize & pImageSize, const CRect & pTargetRect)
{
	tPtr <CBitmapBuffer>	lTargetBuffer;

	if	(
			(pTargetRect.Size () != pImageSize)
		&&	(lTargetBuffer = new CBitmapBuffer)
		&&	(lTargetBuffer->CreateBuffer (pTargetRect.Size(), true))
		)
	{
		Gdiplus::Bitmap		lBitmap (pImageSize.cx, pImageSize.cy, pImageSize.cx*4, PixelFormat32bppPARGB, mImageBuffer.mBitmapBits);
		Gdiplus::Graphics	lGraphics (lTargetBuffer->mDC);

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

CBitmapBuffer * CDirectShowRender::SmoothImage (const CSize & pImageSize, const CRect & pTargetRect)
{
	tPtr <CBitmapBuffer>	lTargetBuffer;

	if	(
			(pTargetRect.Size () == pImageSize)
#ifdef	_DEBUG
		&&	(GetProfileDebugInt(_T("DebugDisableSmoothing")) <= 0)
#endif
		&&	(lTargetBuffer = new CBitmapBuffer)
		&&	(lTargetBuffer->CreateBuffer (pTargetRect.Size(), true))
		)
	{
#if	TRUE
		Gdiplus::Bitmap		lBitmap (pImageSize.cx, pImageSize.cy, pImageSize.cx*4, PixelFormat32bppPARGB, mImageBuffer.mBitmapBits);
		Gdiplus::Graphics	lGraphics (lTargetBuffer->mDC);
		Gdiplus::RectF		lSrcRect (0.0f, 0.0f, (float)pImageSize.cx, (float)pImageSize.cy);
		Gdiplus::RectF		lDstRect (0.0f, (float)pTargetRect.Height(), (float)pTargetRect.Width(), -(float)pTargetRect.Height());

		lGraphics.Clear (Gdiplus::Color (0,0,0,0));
		lGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
		lGraphics.SetCompositingQuality (Gdiplus::CompositingQualityHighQuality);
		lGraphics.SetInterpolationMode (Gdiplus::InterpolationModeHighQualityBilinear);
		lGraphics.SetPixelOffsetMode (Gdiplus::PixelOffsetModeHighQuality);

		lDstRect.Offset (0.5f, 0.5f);
		lGraphics.DrawImage (&lBitmap, lDstRect, lSrcRect, Gdiplus::UnitPixel);
		return lTargetBuffer.Detach ();
#endif
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_AvgTimePerFrame (REFTIME *pAvgTimePerFrame)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pAvgTimePerFrame)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_BitRate (long *pBitRate)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pBitRate)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_BitErrorRate (long *pBitErrorRate)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pBitErrorRate)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_VideoWidth (long *pVideoWidth)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pVideoWidth)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_VideoHeight (long *pVideoHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(!pVideoHeight)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_SourceLeft (long SourceLeft)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	pThis->mSourceRect.OffsetRect (SourceLeft - pThis->mSourceRect.left, 0);
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_SourceLeft (long *pSourceLeft)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pSourceLeft)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceLeft = pThis->mSourceRect.left;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_SourceWidth (long SourceWidth)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(SourceWidth < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		pThis->mSourceRect.right = pThis->mSourceRect.left + SourceWidth;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_SourceWidth (long *pSourceWidth)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pSourceWidth)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceWidth = pThis->mSourceRect.Width();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_SourceTop (long SourceTop)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	pThis->mSourceRect.OffsetRect (0, SourceTop - pThis->mSourceRect.top);
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_SourceTop (long *pSourceTop)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pSourceTop)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceTop = pThis->mSourceRect.top;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_SourceHeight (long SourceHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(SourceHeight < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		pThis->mSourceRect.bottom = pThis->mSourceRect.top + SourceHeight;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_SourceHeight (long *pSourceHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pSourceHeight)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pSourceHeight = pThis->mSourceRect.Height ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_DestinationLeft (long DestinationLeft)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	pThis->mSourceRect.OffsetRect (DestinationLeft - pThis->mSourceRect.left, 0);
	if	(IsWindow (pThis->mRenderWnd))
	{
		::GetClientRect (pThis->mRenderWnd, &lClientRect);
		::IntersectRect (&pThis->mRenderRect, &pThis->mRenderRect, &lClientRect);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_DestinationLeft (long *pDestinationLeft)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pDestinationLeft)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationLeft = pThis->mRenderRect.left;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_DestinationWidth (long DestinationWidth)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	if	(DestinationWidth < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		pThis->mRenderRect.right = pThis->mRenderRect.left + DestinationWidth;
		if	(IsWindow (pThis->mRenderWnd))
		{
			::GetClientRect (pThis->mRenderWnd, &lClientRect);
			::IntersectRect (&pThis->mRenderRect, &pThis->mRenderRect, &lClientRect);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_DestinationWidth (long *pDestinationWidth)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pDestinationWidth)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationWidth = pThis->mRenderRect.Width();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_DestinationTop (long DestinationTop)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	pThis->mRenderRect.OffsetRect (0, DestinationTop - pThis->mRenderRect.top);
	if	(IsWindow (pThis->mRenderWnd))
	{
		::GetClientRect (pThis->mRenderWnd, &lClientRect);
		::IntersectRect (&pThis->mRenderRect, &pThis->mRenderRect, &lClientRect);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_DestinationTop (long *pDestinationTop)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pDestinationTop)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationTop = pThis->mRenderRect.top;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::put_DestinationHeight (long DestinationHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;
	CRect	lClientRect;

	if	(DestinationHeight < 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		pThis->mRenderRect.bottom = pThis->mRenderRect.top + DestinationHeight;
		if	(IsWindow (pThis->mRenderWnd))
		{
			::GetClientRect (pThis->mRenderWnd, &lClientRect);
			::IntersectRect (&pThis->mRenderRect, &pThis->mRenderRect, &lClientRect);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::get_DestinationHeight (long *pDestinationHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(!pDestinationHeight)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDestinationHeight = pThis->mRenderRect.Height();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::SetSourcePosition (long Left, long Top, long Width, long Height)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

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
		pThis->mSourceRect.SetRect (Left, Top, Left+Width, Top+Height);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::GetSourcePosition (long *pLeft, long *pTop, long *pWidth, long *pHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(pLeft)
	{
		*pLeft = pThis->mSourceRect.left;
	}
	if	(pTop)
	{
		*pTop = pThis->mSourceRect.top;
	}
	if	(pWidth)
	{
		*pWidth = pThis->mSourceRect.Width();
	}
	if	(pHeight)
	{
		*pHeight = pThis->mSourceRect.Height();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::SetDefaultSourcePosition (void)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
		)
	{
		pThis->mSourceRect.SetRect (0, 0, lVideoInfo->bmiHeader.biWidth, lVideoInfo->bmiHeader.biHeight);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::SetDestinationPosition (long Left, long Top, long Width, long Height)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

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
		pThis->mRenderRect.SetRect (Left, Top, Left+Width, Top+Height);
		if	(IsWindow (pThis->mRenderWnd))
		{
			::GetClientRect (pThis->mRenderWnd, &lClientRect);
			::IntersectRect (&pThis->mRenderRect, &pThis->mRenderRect, &lClientRect);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::GetDestinationPosition (long *pLeft, long *pTop, long *pWidth, long *pHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(pLeft)
	{
		*pLeft = pThis->mRenderRect.left;
	}
	if	(pTop)
	{
		*pTop = pThis->mRenderRect.top;
	}
	if	(pWidth)
	{
		*pWidth = pThis->mRenderRect.Width();
	}
	if	(pHeight)
	{
		*pHeight = pThis->mRenderRect.Height();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::SetDefaultDestinationPosition (void)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_OK;

	if	(IsWindow (pThis->mRenderWnd))
	{
		::GetClientRect (pThis->mRenderWnd, &pThis->mRenderRect);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::GetVideoSize (long *pWidth, long *pHeight)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::GetVideoPaletteEntries (long StartIndex, long Entries, long *pRetrieved, long *pPalette)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_OK;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
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

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::GetCurrentImage (long *pBufferSize,long *pDIBImage)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = E_NOTIMPL;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::IsUsingDefaultSource (void)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT				lResult = S_FALSE;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(
			(pThis->mInputPin)
		&&	(pThis->mInputPin->mMediaType)
		&&	(lVideoInfo = (VIDEOINFOHEADER *)pThis->mInputPin->mMediaType->pbFormat)
		)
	{
		if	(
				(pThis->mSourceRect.left == 0)
			&&	(pThis->mSourceRect.top == 0)
			&&	(pThis->mSourceRect.Width() == lVideoInfo->bmiHeader.biWidth)
			&&	(pThis->mSourceRect.Height() == lVideoInfo->bmiHeader.biHeight)
			)
		{
			lResult = S_OK;
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowRender::XBasicVideo::IsUsingDefaultDestination (void)
{
	METHOD_PROLOGUE(CDirectShowRender, BasicVideo)

	HRESULT	lResult = S_FALSE;
	CRect	lClientRect;

	if	(IsWindow (pThis->mRenderWnd))
	{
		::GetClientRect (pThis->mRenderWnd, &lClientRect);
		if	(::EqualRect (&pThis->mRenderRect, &lClientRect))
		{
			lResult = S_OK;
		}
	}
	return lResult;
}
