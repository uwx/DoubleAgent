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
#include "DaCore.h"
#include <shlwapi.h>
#include "DirectShowSource.h"
#include "AgentStreamInfo.h"
#include "Registry.h"
#include "GuidStr.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#include "DebugProcess.h"
#endif

#pragma warning (disable: 4355)

#ifdef	_DEBUG
//#define	_DEBUG_COM				LogNormal|LogHighVolume
//#define	_DEBUG_INTERFACE		LogNormal
//#define	_DEBUG_INTERFACE_EX		LogNormal|LogHighVolume
//#define	_DEBUG_SAMPLES			LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_AUDIO			LogNormal|LogHighVolume
//#define	_DEBUG_AUDIO_FILTERS	LogNormal
#define	_LOG_FILE_LOAD				(GetProfileDebugInt(_T("LogFileLoad"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE				(GetProfileDebugInt(_T("LogInstance_DirectShowFilter"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS				(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
//#define	_TRACE_RESOURCES_EX		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif
#ifndef	_LOG_FILE_LOAD
#define	_LOG_FILE_LOAD	LogVerbose+1
#endif

/////////////////////////////////////////////////////////////////////////////

//
//	For now we're creating this object directly, so it's not registered
//
//IMPLEMENT_OLECREATE_EX(CDirectShowSource, _DSFILTER_PROGID_VER, 0x1147E563, 0xA208, 0x11DE, 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2)
//
//BOOL CDirectShowSource::CDirectShowSourceFactory::UpdateRegistry (BOOL bRegister)
//{
//	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_DSFILTER_PROGID_NAME)))
//	{
//		if	(bRegister)
//		{
//			RegisterProgIdVer (_T(_DSFILTER_PROGID), _T(_DSFILTER_PROGID_VER), _T(_DSFILTER_PROGID_NAME));
//			RegisterServer ();
//			RegisterFreeThreaded (true);
//			RegisterDefCategory ();
//		}
//		else
//		{
//			UnregisterProgIdVer (_T(_DSFILTER_PROGID), _T(_DSFILTER_PROGID_VER));
//			UnregisterServer ();
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectShowSource::CDirectShowSource()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowSource::CDirectShowSource (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowSource::~CDirectShowSource()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowSource::~CDirectShowSource (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Terminate ();
}

HRESULT CDirectShowSource::FinalConstruct ()
{
	InitMediaSeeking (*this, *this, 0, AM_SEEKING_CanGetCurrentPos | AM_SEEKING_CanGetStopPos | AM_SEEKING_CanDoSegments | AM_SEEKING_Source);

	return S_OK;
}

void CDirectShowSource::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowSource::~FinalRelease (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Terminate ();
}

void CDirectShowSource::Terminate ()
{
	if	(this)
	{
		try
		{
			EndClock ();

			{
				CLockMutex	lLock (mStateLock);
				try
				{
					SafeFreeSafePtr (mVideoOutPin);
					mAudioOutPins.RemoveAll ();
				}
				catch AnyExceptionSilent
			}
			{
				CLockMutex	lLock (mDataLock);
				try
				{
					CAgentStreamUtils::SetAgentFile (NULL, this);
				}
				catch AnyExceptionSilent
			}

			CDirectShowFilter::Terminate ();
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

const GUID & CDirectShowSource::GetClassID ()
{
	static const GUID lClassID = __uuidof(CDirectShowSource);
	return lClassID;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSource::SetFilterName (LPCWSTR pFilterName)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(pFilterName)
		&&	(wcstoul (pFilterName, NULL, 10) == 0)
		&&	(!CAgentStreamUtils::GetAgentFile ())
		)
	{
		IMediaEventSinkPtr	lEventSink (mFilterGraph);

		if	(lEventSink != NULL)
		{
			lEventSink->Notify (EC_OPENING_FILE, 1, 0);
		}

		if	(SUCCEEDED (lResult = OpenFile (CAtlString (pFilterName))))
		{
			ReadFile ();
		}

		if	(lEventSink != NULL)
		{
			lEventSink->Notify (EC_OPENING_FILE, 0, 0);
		}
	}
	return lResult;
}

CAtlString CDirectShowSource::GetFilterName ()
{
	CAtlString		lFilterName;
	CAgentFile *	lAgentFile;
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
	CLockMutex		lLock (mStateLock);
#endif
	try
	{
		if	(lAgentFile = CAgentStreamUtils::GetAgentFile ())
		{
			lFilterName = (BSTR)lAgentFile->GetPath();
		}
	}
	catch AnyExceptionDebug

	return lFilterName;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSource::OpenFile (LPCTSTR pFileName)
{
	HRESULT		lResult = S_OK;
	CLockMutex	lLock (mStateLock);

	try
	{
		CAgentFile *	lAgentFile;
		CAtlString		lFileName = CAgentFile::ParseFilePath (pFileName);

		lAgentFile = CAgentStreamUtils::GetAgentFile ();

		if	(lFileName.IsEmpty ())
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(
				(lAgentFile)
			&&	(lFileName.CompareNoCase (CAtlString ((BSTR)lAgentFile->GetPath())) != 0)
			)
		{
			lResult = E_FAIL;
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(!lAgentFile)
			)
		{
			if	(lAgentFile = _AtlModule.FindCachedFile (lFileName))
			{
				CAgentStreamUtils::SetAgentFile (lAgentFile, this);
			}
			else
			if	(lAgentFile = CAgentFile::CreateInstance())
			{
				lAgentFile->SetDownloadMode (false, false, false);
				lResult = lAgentFile->Open (lFileName, _LOG_FILE_LOAD);

				if	(SUCCEEDED (lResult))
				{
					CAgentStreamUtils::SetAgentFile (lAgentFile, this);
				}
				else
				{
					delete lAgentFile;
					lAgentFile = NULL;
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

void CDirectShowSource::ReadFile ()
{
	CLockMutex	lLock (mStateLock);

	try
	{
		CAgentFile *		lAgentFile;
		CAgentStreamInfo *	lStreamInfo;

		if	(lAgentFile = CAgentStreamUtils::GetAgentFile ())
		{
			if	(lAgentFile->GetNames().GetCount() <= 0)
			{
				lAgentFile->ReadNames (true, _LOG_FILE_LOAD);
			}
			GetFileImages (_LOG_FILE_LOAD);
			GetFileSounds (_LOG_FILE_LOAD);

			if	(lStreamInfo = new CComObjectNoLock<CAgentStreamInfo>)
			{
				lStreamInfo->Initialize (lAgentFile);
				CAgentStreamUtils::SetAgentStreamInfo (lStreamInfo);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::InitializePins ()
{
	CAgentFile *		lAgentFile;
	tMediaTypePtr		lMediaType;
	GUID				lMediaSubtype;
	CAtlString			lPinName;
	CSize				lImageSize;
	UINT				lImageFormatSize;
	tArrayPtr <BYTE>	lImageFormatBuffer;
	BITMAPINFO *		lImageFormat;
	VIDEOINFOHEADER *	lVideoInfo;

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::InitializePins"), this);
	}
#endif
	if	(mBkColor)
	{
		lPinName = _T("RGB8");
		lMediaSubtype = MEDIASUBTYPE_RGB8;
	}
	else
	{
		lPinName = _T("ARGB32");
		lMediaSubtype = MEDIASUBTYPE_ARGB32;
	}

	if	(lAgentFile = CAgentStreamUtils::GetAgentFile ())
	{
		lImageSize = lAgentFile->GetImageSize();
		lImageFormatSize = lAgentFile->GetImageFormat (NULL, NULL, (IsEqualGUID (lMediaSubtype, MEDIASUBTYPE_ARGB32)?true:false));
		if	(lImageFormatBuffer = new BYTE [lImageFormatSize])
		{
			lImageFormat = (BITMAPINFO*)lImageFormatBuffer.Ptr();
		}

		if	(
				(lImageFormat)
			&&	(lAgentFile->GetImageFormat (lImageFormat, NULL, (IsEqualGUID (lMediaSubtype, MEDIASUBTYPE_ARGB32)?true:false)))
			&&	(mVideoOutPin = new CComObjectNoLock <CDirectShowPinOut>)
			)
		{
			mVideoOutPin->AddRef ();
			mVideoOutPin->Initialize (*this, _T("Animation Out"), lPinName);

			if	(IsEqualGUID (lMediaSubtype, MEDIASUBTYPE_ARGB32))
			{
				if	(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), sizeof(VIDEOINFOHEADER)+lImageFormatSize-sizeof(BITMAPINFOHEADER))))
				{
					lMediaType->majortype = MEDIATYPE_Video;
					lMediaType->subtype = lMediaSubtype;
					lMediaType->formattype = FORMAT_VideoInfo;
					lMediaType->bFixedSizeSamples = FALSE;
					lMediaType->bTemporalCompression = FALSE;
					lMediaType->lSampleSize = lAgentFile->GetImageBits (NULL, NULL, true);

					lVideoInfo = (VIDEOINFOHEADER*)lMediaType->pbFormat;
					SetRect (&lVideoInfo->rcSource, 0, 0, lImageSize.cx, lImageSize.cy);
					SetRect (&lVideoInfo->rcTarget, 0, 0, lImageSize.cx, lImageSize.cy);
					memcpy (&lVideoInfo->bmiHeader, lImageFormat, lImageFormatSize);
					lVideoInfo->dwBitRate = 0;
					lVideoInfo->dwBitErrorRate = 0;
					lVideoInfo->AvgTimePerFrame = MsPer100Ns;

					mVideoOutPin->mMediaTypes.Add (lMediaType.Detach());
				}
			}
			else
			{
				SetPaletteBkColor (lImageFormat, lAgentFile->GetTransparency(), *mBkColor);

				if	(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), sizeof(VIDEOINFOHEADER)+lImageFormatSize-sizeof(BITMAPINFOHEADER))))
				{
					lMediaType->majortype = MEDIATYPE_Video;
					lMediaType->subtype = lMediaSubtype;
					lMediaType->formattype = FORMAT_VideoInfo;
					lMediaType->bFixedSizeSamples = FALSE;
					lMediaType->bTemporalCompression = FALSE;
					lMediaType->lSampleSize = lAgentFile->GetImageBits (NULL, NULL);

					lVideoInfo = (VIDEOINFOHEADER*)lMediaType->pbFormat;
					SetRect (&lVideoInfo->rcSource, 0, 0, lImageSize.cx, lImageSize.cy);
					SetRect (&lVideoInfo->rcTarget, 0, 0, lImageSize.cx, lImageSize.cy);
					memcpy (&lVideoInfo->bmiHeader, lImageFormat, lImageFormatSize);
					lVideoInfo->dwBitRate = 0;
					lVideoInfo->dwBitErrorRate = 0;
					lVideoInfo->AvgTimePerFrame = MsPer100Ns;

					mVideoOutPin->mMediaTypes.Add (lMediaType.Detach());
				}
			}

			mOutputPins.Add (mVideoOutPin);
		}
	}
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::InitializePins Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::OnJoinedFilterGraph ()
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::OnJoinedFilterGraph"), this);
	}
#endif

	CDirectShowFilter::OnJoinedFilterGraph ();
	SetTimes (0, GetDuration());

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::OnJoinedFilterGraph Done"), this);
	}
#endif
}

void CDirectShowSource::OnLeftFilterGraph ()
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::OnLeftFilterGraph"), this);
	}
#endif

	Terminate ();
	CDirectShowFilter::OnLeftFilterGraph ();

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::OnLeftFilterGraph Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSource::OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState)
{
	HRESULT	lResult = CDirectShowFilter::OnStateChanged (pOldState, pNewState);

#ifdef	_DEBUG_DIRECT_SHOW
	if	(
			(pNewState == State_Stopped)
		&&	(pOldState != State_Stopped)
		)
	{
		LogMessage (_DEBUG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Stopped> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
	else
	if	(
			(pNewState == State_Paused)
		&&	(pOldState == State_Stopped)
		)
	{
		LogMessage (_DEBUG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Started> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
	else
	if	(
			(pNewState == State_Running)
		&&	(pOldState == State_Paused)
		)
	{
		LogMessage (_DEBUG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Running> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags)
{
	CDirectShowSeekingImpl::OnTimesChanged (pCurrTime, pCurrentFlags, pStopTime, pStopFlags);

	if	(mVideoOutPin.Ptr ())
	{
		CLockMutex	lLock (mStateLock);

		try
		{
			if	((pStopFlags & AM_SEEKING_NoFlush) == 0)
			{
#ifdef	_DEBUG_SAMPLES
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Flush VideoOut"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
#endif
				Flush ();
			}
			if	(pStopFlags & AM_SEEKING_Segment)
			{
#ifdef	_DEBUG_SAMPLES
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Restart VideoOut"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
#endif
				StartOutputStreams ();
			}

			if	(
					(mState == State_Running)
				&&	(!IsClockStarted())
				)
			{
				StartClock (sDefaultClockInterval);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSource::StartOutputStreams ()
{
	CueAudioSegments ();
	return CDirectShowFilter::StartOutputStreams ();
}

void CDirectShowSource::OnClockPulse ()
{
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowSource::OnClockPulse"), this);
	}
#endif
	if	(!PutVideoFrame ())
	{
		if	(mVideoOutPin)
		{
			mVideoOutPin->EndOutputStream ();
		}

#ifdef	_DEBUG_DIRECT_SHOW
		LogMessage (_DEBUG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Complete> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
		StopClock ();
	}
#ifdef	_TRACE_RESOURCES_EX
	if	(LogIsActive (_TRACE_RESOURCES_EX))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CDirectShowSource::OnClockPulse Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDirectShowSource::PutVideoFrame ()
{
	bool				lRet = false;
	REFERENCE_TIME		lStreamTime;
	REFERENCE_TIME		lCurrTime;
	REFERENCE_TIME		lStopTime;
	REFERENCE_TIME		lDuration = 0;
	CAgentStreamInfo *	lStreamInfo;

	GetTimes (lCurrTime, lStopTime);
	lStreamTime = GetStreamTime(mState);

	if	(lCurrTime < lStopTime)
	{
		if	(
				(lCurrTime > lStreamTime - 0.001)
			&&	(lStreamInfo = CAgentStreamUtils::GetAgentStreamInfo())
			&&	(lStreamInfo->GetSpeakingDuration () > 0)
			)
		{
			//
			//	When a WAV file is speaking, we don't want to queue too many frames in advance.
			//	This allows the lip-sync filter to get mouth positions queued up BEFORE the
			//	corresponding frames are prepared.
			//
			lRet = true;
		}
		else
		if	(PutVideoSample (lCurrTime, lStopTime) == S_OK)
		{
			if	(CAgentStreamUtils::GetAgentStreamInfo ())
			{
				lDuration = GetDuration ();
			}
			SetTimes (lCurrTime, lStopTime, lDuration);
		}
		lRet = true;
	}
	else
	if	(
			(lStreamInfo = CAgentStreamUtils::GetAgentStreamInfo())
		&&	(lStreamInfo->GetSpeakingDuration () != 0)
		)
	{
		lRet = true;
	}
	return lRet;
}

HRESULT CDirectShowSource::PutVideoSample (REFERENCE_TIME & pSampleTime, REFERENCE_TIME pStopTime)
{
	HRESULT					lResult = S_FALSE;
	REFERENCE_TIME			lStartTime = pSampleTime;
	REFERENCE_TIME			lEndTime = pSampleTime;
	REFERENCE_TIME			lMediaStartTime;
	REFERENCE_TIME			lMediaEndTime;
	IMediaSamplePtr			lSample;
	UINT					lSampleSize = 0;
	LPBYTE					lSampleBuffer = NULL;
	long					lAnimationNdx = -1;
	long					lFrameNdx = -1;
	const CAgentFileFrame *	lFrame = NULL;
	short					lMouthOverlayNdx = -1;
	CAgentFile *			lAgentFile;
	BYTE					lTransparency;
	bool					l32BitSamples = false;

	if	(mDataLock.Lock (100))
	{
		try
		{
			CAgentStreamInfo *			lStreamInfo;
			CAnimationSequence *		lAnimationSequence;
			const CAgentFileAnimation *	lAnimation;
			long						lFrameDuration;
			long						lSpeakingDuration = 0;
			long						lTimeNdx = (long)(lStartTime / MsPer100Ns);

			if	(
					(mVideoOutPin->mMediaType)
				&&	(IsEqualGUID (mVideoOutPin->mMediaType->subtype, MEDIASUBTYPE_ARGB32))
				)
			{
				l32BitSamples = true;
			}

			if	(
					(lAgentFile = CAgentStreamUtils::GetAgentFile())
				&&	(lStreamInfo = CAgentStreamUtils::GetAgentStreamInfo())
				)
			{
				lTransparency = lAgentFile->GetTransparency();
				lStreamInfo->Lock ();

				try
				{
					lAnimationSequence = lStreamInfo->GetAnimationSequence ();
					lSpeakingDuration = lStreamInfo->GetSpeakingDuration (true);

					//
					//	When mouth animation is started, a stream is built containing only a single frame.
					//	So, when animating speech, we always use frame 0 (ignoring the TimeNdx).
					//
					if	(
							(lAnimationSequence)
						&&	(lStreamInfo->CalcSequenceAnimationFrameNdx (&lAnimationNdx, &lFrameNdx, (lSpeakingDuration > 0) ? 0 : lTimeNdx, true) == S_OK)
						&&	(lAnimation = lAgentFile->GetAnimation (lAnimationNdx))
						&&	(lFrameNdx >= 0)
						&&	(lFrameNdx < (long)(short)lAnimation->mFrameCount)
						)
					{
						lFrame = &lAnimation->mFrames [lFrameNdx];
						lFrameDuration = (long)(short)lFrame->mDuration;
						if	(lSpeakingDuration > 0)
						{
							//
							//	For TTS speech, we use an arbitrarily long duration because we don't know the actual duration in advance.
							//	The stream will be restarted for each mouth movement, so the duration is not relevant.
							//
							//	For WAV speech, we use the actual duration of the speech sound stream.
							//	The WAV lip-sync filter will queue mouth positions, by TimeNdx, into StreamInfo.
							//	To keep the lips moving, we'll use a frame duration of at most 5 milliseconds.
							//
							if	(lStreamInfo->GetSpeakingDuration () < 0)
							{
								lFrameDuration = max (lFrameDuration, lSpeakingDuration);
							}
							else
							{
								lFrameDuration = min (max (lFrameDuration, 2), 5);
							}
						}

						if	(lSpeakingDuration > 0)
						{
							lMouthOverlayNdx = lStreamInfo->GetMouthOverlay (lTimeNdx);
						}
						lMediaStartTime = lTimeNdx;
						lMediaEndTime = lTimeNdx + lFrameDuration;
						lEndTime = lStartTime + ((LONGLONG)lFrameDuration * MsPer100Ns);
						lEndTime = min (lEndTime, pStopTime);
					}
#ifdef	_DEBUG_SAMPLES
					else
					if	(!lAnimationSequence)
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No AnimationSequence"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
					}
					else
					if	(!lFrame)
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No more frames [%f]"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(lStartTime));
					}
#endif
				}
				catch AnyExceptionSilent

				lStreamInfo->Unlock ();
			}
#ifdef	_DEBUG_SAMPLES
			else
			{
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No StreamInfo"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
			}
#endif
		}
		catch AnyExceptionDebug

		mDataLock.Unlock ();
	}
#ifdef	_DEBUG_SAMPLES
	else
	{
		LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Mutex lock failed (a)"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
	}
#endif

	if	(
			(lFrame)
		&&	(lEndTime >= lStartTime)
		)
	{
		if	(
				(lAgentFile = CAgentStreamUtils::GetAgentFile())
			&&	(mVideoOutPin)
			&&	(SUCCEEDED (lResult = mVideoOutPin->GetOutputSample (&lSample, &lStartTime, &lEndTime/*, 0*/)))
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, lSample->GetPointer (&lSampleBuffer))))
			)
		{
			LogVfwErr (LogNormal, lSample->SetTime (&lStartTime, &lEndTime), _T("SetTime [%f (%I64d)] [%f (%I64d)]"), RefTimeSec(lStartTime), lStartTime, RefTimeSec(lEndTime), lEndTime);
			LogVfwErr (LogNormal, lSample->SetMediaTime (&lMediaStartTime, &lMediaEndTime), _T("SetMediaTime [%I64d] [%I64d]"), lMediaStartTime, lMediaEndTime);

			if	(mDataLock.Lock (100))
			{
				try
				{
#ifdef	_DEBUG_SAMPLES
					if	(lFrame->mImageCount > 0)
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Frame [%3.3d] [%3.3d] Overlay [%hd] at MediaTime [%I64d - %I64d] TimeStamp [%f - %f]"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)), lAnimationNdx, lFrameNdx, lMouthOverlayNdx, lMediaStartTime, lMediaEndTime, RefTimeSec(lStartTime), RefTimeSec(lEndTime));
					}
					else
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Frame [%3.3d] [%3.3d] Empty at MediaTime [%I64d - %I64d] TimeStamp [%f - %f]"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)), lAnimationNdx, lFrameNdx, lMediaStartTime, lMediaEndTime, RefTimeSec(lStartTime), RefTimeSec(lEndTime));
					}
#endif
					if	(lFrame->mImageCount > 0)
					{
						lSampleSize = lAgentFile->GetFrameBits (lSampleBuffer, *lFrame, l32BitSamples, mBkColor, lMouthOverlayNdx);
					}
					else
					{
						lSampleSize = lAgentFile->GetImageBits (NULL, NULL, l32BitSamples);
						if	(l32BitSamples)
						{
							memset (lSampleBuffer, 0, lSampleSize);
						}
						else
						{
							memset (lSampleBuffer, lTransparency, lSampleSize);
						}
					}
				}
				catch AnyExceptionDebug

				mDataLock.Unlock ();
			}
#ifdef	_DEBUG_SAMPLES
			else
			{
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Mutex lock failed (b)"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
			}
#endif
			if	(SUCCEEDED (lResult))
			{
				lResult = S_FALSE;
			}
			if	(
					(SUCCEEDED (lResult))
				&&	(lSampleBuffer)
				&&	(lSampleSize)
				)
			{
				lResult = mVideoOutPin->PutOutputSample (lSample, lSampleSize);
				pSampleTime = lEndTime;
			}
#ifdef	_DEBUG_SAMPLES
			else
			{
				if	(
						(lSampleBuffer)
					&&	(lSampleSize)
					)
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Frame discarded"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
				}
				else
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No frame"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
				}
			}
#endif
		}
#ifdef	_DEBUG_SAMPLES_NOT
		else
		{
			LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No output buffer"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)));
		}
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDirectShowSource::CueAudioSegments ()
{
	bool					lRet = false;
	CAgentStreamInfo *		lStreamInfo;
	CAnimationSequence *	lAnimationSequence;

	if	(lStreamInfo = CAgentStreamUtils::GetAgentStreamInfo())
	{
		lStreamInfo->Lock ();

		try
		{
			if	(lAnimationSequence = lStreamInfo->GetAnimationSequence ())
			{
				lRet = CueAudioSegments (lAnimationSequence);
			}
		}
		catch AnyExceptionSilent

		lStreamInfo->Unlock ();
	}
	return lRet;
}

bool CDirectShowSource::CueAudioSegments (CAnimationSequence * pAnimationSequence)
{
	bool		lRet = false;
	CLockMutex	lLock (mStateLock);

	try
	{
		INT_PTR	lAudioNdx;

		if	(pAnimationSequence)
		{
			for	(lAudioNdx = 0; lAudioNdx < (INT_PTR)pAnimationSequence->mAudio.GetCount(); lAudioNdx++)
			{
				CSeqAudioSegment &		lAudioSegment = pAnimationSequence->mAudio [lAudioNdx];
				CDirectSoundPinPush *	lAudioPin = mAudioOutPins [lAudioSegment.mSoundNdx];

				if	(
						(lAudioPin)
					&&	(SUCCEEDED (lAudioPin->CueSound ((LONGLONG)lAudioSegment.mStartTime * MsPer100Ns)))
					)
				{
					lRet = true;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::ConnectSequenceAudio ()
{
	CAgentStreamInfo *		lStreamInfo;
	CAnimationSequence *	lAnimationSequence;

	if	(lStreamInfo = CAgentStreamUtils::GetAgentStreamInfo())
	{
		lStreamInfo->Lock ();

		try
		{
			if	(lAnimationSequence = lStreamInfo->GetAnimationSequence ())
			{
				ConnectSequenceAudio (lAnimationSequence);
			}
		}
		catch AnyExceptionSilent

		lStreamInfo->Unlock ();
	}
}

void CDirectShowSource::ConnectSequenceAudio (CAnimationSequence * pAnimationSequence)
{
	CLockMutex	lLock (mStateLock);

	try
	{
		long					lAudioNdx;
		CDirectSoundPinPush *	lAudioPin;

		try
		{
			for	(lAudioNdx = 0; lAudioNdx < (long)mAudioOutPins.GetCount(); lAudioNdx++)
			{
				if	(lAudioPin = mAudioOutPins [lAudioNdx])
				{
					lAudioPin->DisconnectFilters ();
				}
			}
		}
		catch AnyExceptionDebug

		mAudioOutPins.RemoveAll ();

		if	(pAnimationSequence)
		{
			for	(lAudioNdx = 0; lAudioNdx < (INT_PTR)pAnimationSequence->mAudio.GetCount(); lAudioNdx++)
			{
				CSeqAudioSegment &	lAudioSegment = pAnimationSequence->mAudio [lAudioNdx];

				if	(lAudioPin = ConnectSequenceSound (lAudioSegment.mSoundNdx))
				{
					lAudioSegment.mEndTime = lAudioSegment.mStartTime + (long)((lAudioPin->GetDuration()+MsPer100Ns-1)/MsPer100Ns); // Rounding up
#ifdef	_DEBUG_AUDIO
					LogMessage (_DEBUG_AUDIO, _T("  Cue [%2d] Frame [%2d %2d] Start [%6u] End [%6u] Sound [%2u]"), lAudioNdx, lAudioSegment.mAnimationNdx, lAudioSegment.mFrameNdx, lAudioSegment.mStartTime, lAudioSegment.mEndTime, lAudioSegment.mSoundNdx);
#endif
				}
			}

			CAgentStreamInfo::ResyncAudioVideo (pAnimationSequence);
		}

#ifdef	_DEBUG_AUDIO_FILTERS
		IGraphBuilderPtr lGraphBuilder (mFilterGraph);
		LogFilterStates (_DEBUG_AUDIO_FILTERS, lGraphBuilder, true, _T("CueSequenceAudio"));
#endif
#ifdef	_DEBUG_DIRECT_SHOW
		if	(
				(pAnimationSequence)
			&&	(pAnimationSequence->mAudio.GetCount() > 0)
			)
		{
			LogMessage (_DEBUG_DIRECT_SHOW, _T("  [%f] DirectShow Sounds <%d Ready> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), mAudioOutPins.GetCount(), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
		}
#endif
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

CDirectSoundPinPush * CDirectShowSource::ConnectSequenceSound (long pSoundNdx)
{
	CDirectSoundPinPush *	lRet = NULL;
	CDirectSoundPinPush *	lAudioPin = mAudioOutPins [pSoundNdx];
	CAgentFile *			lAgentFile;
	long					lSoundSize;
	LPCVOID					lSound = NULL;

	if	(
			(!lAudioPin)
		&&	(lAgentFile = CAgentStreamUtils::GetAgentFile ())
		&&	(lSoundSize = lAgentFile->GetSoundSize (pSoundNdx))
		&&	(lSound = lAgentFile->GetSound (pSoundNdx))
		&&	(lAudioPin = new CComObjectNoLock <CDirectSoundPinPush>)
		)
	{
		lAudioPin->AddRef ();
		lAudioPin->Initialize (*this, *this, pSoundNdx);
		mAudioOutPins [pSoundNdx] = lAudioPin;

		if	(
				(SUCCEEDED (lAudioPin->ConvertSound (lSound, lSoundSize)))
			&&	(SUCCEEDED (lAudioPin->ConnectFilters ()))
			)
		{
			lRet = lAudioPin;
		}
		else
		{
			mAudioOutPins.RemoveKey (pSoundNdx);
		}
	}
	else
	{
		lRet = lAudioPin;
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::GetSeekingTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime)
{
	GetTimes (pCurrTime, pStopTime);
}

LONGLONG CDirectShowSource::GetDuration ()
{
	LONGLONG			lDuration = 0;
	CAgentStreamInfo *	lStreamInfo;
	long				lSequenceDuration;

	if	(lStreamInfo = CAgentStreamUtils::GetAgentStreamInfo ())
	{
		lSequenceDuration = lStreamInfo->GetSpeakingDuration (true);

		if	(lSequenceDuration > 0)
		{
			lDuration = lSequenceDuration;
		}
		else
		if	(lStreamInfo->GetSequenceDuration (&lSequenceDuration) == S_OK)
		{
			lDuration = lSequenceDuration;
		}
		else
		if	(lStreamInfo->GetMaxSequenceDuration (&lSequenceDuration) == S_OK)
		{
			lDuration = lSequenceDuration;
		}
	}
	else
	{
		lDuration = CalcFileDuration ();
	}
	return lDuration * MsPer100Ns;
}

LONGLONG CDirectShowSource::GetPreroll ()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::GetAgentFile (ULONG_PTR *pAgentFile)
{
	HRESULT	lResult = S_OK;

	if	(!pAgentFile)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAgentFile) = (ULONG_PTR) CAgentStreamUtils::GetAgentFile ();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSource::SetAgentFile (ULONG_PTR pAgentFile)
{
	HRESULT		lResult = S_OK;
	CLockMutex	lLock (mStateLock);

	try
	{
		CAgentFile *	lAgentFile;

		if	(lAgentFile = (CAgentFile *)pAgentFile)
		{
			CAgentStreamUtils::SetAgentFile (lAgentFile, this);
			ReadFile ();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::GetAgentStreamInfo (ULONG_PTR *pAgentStreamInfo)
{
	HRESULT	lResult = S_OK;

	if	(!pAgentStreamInfo)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAgentStreamInfo) = (ULONG_PTR) CAgentStreamUtils::GetAgentStreamInfo ();
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSource::SetAgentStreamInfo (ULONG_PTR pAgentStreamInfo)
{
	if	(pAgentStreamInfo)
	{
		CAgentStreamUtils::SetAgentStreamInfo ((CAgentStreamInfo *) pAgentStreamInfo);
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::GetBkColor (COLORREF *pBkColor)
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

HRESULT STDMETHODCALLTYPE CDirectShowSource::SetBkColor (const COLORREF *pBkColor)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		CAgentFile *		lAgentFile;
		AM_MEDIA_TYPE *		lMediaType;
		int					lMediaTypeNdx;
		VIDEOINFOHEADER *	lVideoInfo;
		BITMAPINFO *		lImageFormat;

		if	(pBkColor)
		{
			mBkColor = new COLORREF;
			*mBkColor = *pBkColor;
		}
		else
		{
			mBkColor = NULL;
		}

		if	(
				(lAgentFile = CAgentStreamUtils::GetAgentFile ())
			&&	(mFilterGraph)
			&&	(mVideoOutPin)
			)
		{
			for	(lMediaTypeNdx = 0; lMediaTypeNdx < (int)mVideoOutPin->mMediaTypes.GetCount(); lMediaTypeNdx++)
			{
				if	(
						(lMediaType = mVideoOutPin->mMediaTypes [lMediaTypeNdx])
					&&	(IsEqualGUID (lMediaType->subtype, MEDIASUBTYPE_RGB8))
					&&	(lMediaType = mVideoOutPin->mMediaTypes [lMediaTypeNdx])
					&&	(lVideoInfo = (VIDEOINFOHEADER*)lMediaType->pbFormat)
					&&	(lImageFormat = (BITMAPINFO*)&lVideoInfo->bmiHeader)
					)
				{
					SetPaletteBkColor (lImageFormat, lAgentFile->GetTransparency(), (mBkColor.Ptr()) ? *mBkColor : GetSysColor (COLOR_WINDOW));

					if	(
							(mVideoOutPin->mMediaType)
						&&	(IsEqualGUID (mVideoOutPin->mMediaType->subtype, lMediaType->subtype))
						)
					{
						IGraphConfigPtr	lGraphConfig (mFilterGraph);
						IPinPtr			lInputPin (mVideoOutPin->SafeGetConnection ());

						if	(
								(lInputPin != NULL)
							&&	(mState == State_Stopped)
							&&	(SUCCEEDED (LogVfwErr (LogNormal, lInputPin->Disconnect ())))
							)
						{
							LogVfwErr (LogNormal, mVideoOutPin->Disconnect ());
							LogVfwErr (LogNormal, mVideoOutPin->Connect (lInputPin, lMediaType));
							lResult = S_OK;
						}
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::SegmentDurationChanged ()
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CLockMutex	lLock (mDataLock);

	try
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::SegmentDurationChanged"), this);
		}
#endif
		IMediaEventSinkPtr	lEventSink (mFilterGraph);
		REFERENCE_TIME		lDuration;

		ConnectSequenceAudio ();
		lDuration = GetDuration ();
		SetTimes (mCurrTime, mStopTime, lDuration);

		if	(lEventSink != NULL)
		{
			lResult = lEventSink->Notify (EC_LENGTH_CHANGED, 0, 0);
		}

#ifdef	_DEBUG_SAMPLES
		LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] SegmentDurationChanged [%f - %f] of [%f] Speaking [%d]"), AtlTypeName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(lDuration), CAgentStreamUtils::GetAgentStreamInfo()->GetSpeakingDuration());
#endif
#ifdef	_DEBUG_DIRECT_SHOW
		LogMessage (_DEBUG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Ready> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowSource::SegmentDurationChanged Done"), this);
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] %s::FileSource::Load [%ls]"), this, max(m_dwRef,-1), AtlTypeName(this), pszFileName);
#endif
	HRESULT		lResult = E_FAIL;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(CAgentStreamUtils::GetAgentFile ())
		{
			lResult = E_UNEXPECTED;
		}
		else
		if	(SUCCEEDED (lResult = OpenFile (CAtlString (pszFileName))))
		{
			ReadFile ();
		}
	}
	catch AnyExceptionDebug

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Load"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSource::GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] %s::GetCurFile"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = E_FAIL;

	if	(!ppszFileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		CAgentFile *	lAgentFile;
		CLockMutex		lLock (mStateLock);

		try
		{
			if	(lAgentFile = CAgentStreamUtils::GetAgentFile ())
			{
				(*ppszFileName) = AtlAllocTaskOleString ((BSTR)lAgentFile->GetPath ());
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetCurFile"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE CDirectShowSource::GetMiscFlags ()
{
#ifdef	_DEBUG_INTERFACE_EX
	LogMessage (_DEBUG_INTERFACE_EX, _T("[%p(%d)] %s::GetMiscFlags"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	return AM_FILTER_MISC_FLAGS_IS_SOURCE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::Reconfigure (PVOID pvContext, DWORD dwFlags)
{
#ifdef	_DEBUG_INTERFACE_EX
	LogMessage (_DEBUG_INTERFACE_EX, _T("[%p(%d)] %s::Reconfigure"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = S_OK;

	try
	{
		//lResult = ReconnectCallback (dwFlags);
	}
	catch AnyExceptionDebug

	return lResult;
}
