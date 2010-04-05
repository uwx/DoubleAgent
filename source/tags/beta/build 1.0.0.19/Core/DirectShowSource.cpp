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
#ifdef	_DEBUG
#include "BitmapDebugger.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
#define	_LOG_INSTANCE				(GetProfileDebugInt(_T("LogInstance_DirectShow"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS				(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif
#ifndef	_LOG_FILE_LOAD
#define	_LOG_FILE_LOAD	LogVerbose+1
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDirectShowSource, CDirectShowFilter)
//
//	For now we're creating this object directly, so it's not registered
//
//IMPLEMENT_OLECREATE_EX(CDirectShowSource, _DSFILTER_PROGID, 0x1147E563, 0xA208, 0x11DE, 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2)
//
//BOOL CDirectShowSource::CDirectShowSourceFactory::UpdateRegistry (BOOL bRegister)
//{
//	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_DSFILTER_PROGID_NAME)))
//	{
//		if	(bRegister)
//		{
//			RegisterServer ();
//			RegisterBothThreaded (true);
//			RegisterDefCategory ();
//		}
//		else
//		{
//			UnregisterServer ();
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectShowSource, CDirectShowFilter)
	INTERFACE_PART(CDirectShowSource, __uuidof(IFileSourceFilter), FileSource)
	INTERFACE_PART(CDirectShowSource, __uuidof(IAMFilterMiscFlags), FilterMiscFlags)
	INTERFACE_PART(CDirectShowSource, __uuidof(IGraphConfigCallback), ConfigCallback)
	INTERFACE_PART(CDirectShowSource, __uuidof(IMediaSeeking), MediaSeeking)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CDirectShowSource, FileSource)
IMPLEMENT_IUNKNOWN(CDirectShowSource, FilterMiscFlags)
IMPLEMENT_IUNKNOWN(CDirectShowSource, ConfigCallback)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectShowSource::CDirectShowSource()
:	CDirectShowSeeking (*(CCmdTarget*)this, *(CDirectShowClock*)this)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowSource::CDirectShowSource (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation ();
	mSeekingCaps |= AM_SEEKING_CanGetCurrentPos | AM_SEEKING_CanGetStopPos | AM_SEEKING_CanDoSegments | AM_SEEKING_Source;
}

CDirectShowSource::~CDirectShowSource()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowSource::~CDirectShowSource (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Terminate ();
	AfxOleUnlockApp ();
}

void CDirectShowSource::Terminate ()
{
	if	(this)
	{
		try
		{
			EndClock ();

			{
				CSingleLock	lLock (&mStateLock, TRUE);
				try
				{
					SafeFreeSafePtr (mVideoOutPin);
					mAudioOutPins.RemoveAll ();
				}
				catch AnyExceptionSilent
			}
			{
				CSingleLock	lLock (&mDataLock, TRUE);
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
		&&	(!GetAgentFile ())
		)
	{
		IMediaEventSinkPtr	lEventSink (mFilterGraph);

		if	(lEventSink != NULL)
		{
			lEventSink->Notify (EC_OPENING_FILE, 1, 0);
		}

		if	(SUCCEEDED (lResult = OpenFile (CString (pFilterName))))
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

CString CDirectShowSource::GetFilterName ()
{
	CString			lFilterName;
	CAgentFile *	lAgentFile;
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
	CSingleLock		lLock (&mStateLock, TRUE);
#endif
	try
	{
		if	(lAgentFile = GetAgentFile ())
		{
			lFilterName = (BSTR)lAgentFile->GetPath();
		}
	}
	catch AnyExceptionDebug

	return lFilterName;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSource::SetAgentFile (CAgentFile * pAgentFile)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(pAgentFile)
		{
			CAgentStreamUtils::SetAgentFile (pAgentFile, this);
			if	(pAgentFile)
			{
				ReadFile ();
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowSource::OpenFile (LPCTSTR pFileName)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		CAgentFile *	lAgentFile;
		CString			lFileName = CAgentFile::ParseFilePath (pFileName);

		lAgentFile = GetAgentFile ();

		if	(lFileName.IsEmpty ())
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(
				(lAgentFile)
			&&	(lFileName.CompareNoCase (CString ((BSTR)lAgentFile->GetPath())) != 0)
			)
		{
			lResult = E_FAIL;
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(!lAgentFile)
			)
		{
			if	(lAgentFile = TheCoreApp->FindCachedFile (lFileName))
			{
				CAgentStreamUtils::SetAgentFile (lAgentFile, this);
			}
			else
			if	(lAgentFile = (CAgentFile *)CAgentFile::CreateObject())
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
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		CAgentFile *			lAgentFile;
		CAgentStreamInfo *		lStreamInfo;
		_IAgentStreamInfoPtr	lInterface;

		if	(lAgentFile = GetAgentFile ())
		{
			if	(lAgentFile->GetNames().GetSize() <= 0)
			{
				lAgentFile->ReadNames (true, _LOG_FILE_LOAD);
			}
			GetFileImages (_LOG_FILE_LOAD);
			GetFileSounds (_LOG_FILE_LOAD);

			if	(lStreamInfo = new CAgentStreamInfo (lAgentFile, GetControllingUnknown()))
			{
				lInterface.Attach (lStreamInfo);
				SetAgentStreamInfo (lInterface);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowSource::SetBkColor (const COLORREF * pBkColor)
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		CAgentFile *		lAgentFile;
		AM_MEDIA_TYPE *		lMediaType;
		VIDEOINFOHEADER *	lVideoInfo;
		BITMAPINFO *		lImageFormat;
		BYTE				lTransparency;
		COLORREF			lTransparentColor;

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
				(lAgentFile = GetAgentFile ())
			&&	(mFilterGraph)
			&&	(mVideoOutPin)
			&&	(mVideoOutPin->mMediaTypes.GetSize() > 0)
			&&	(lMediaType = mVideoOutPin->mMediaTypes [0])
			&&	(lVideoInfo = (VIDEOINFOHEADER*)lMediaType->pbFormat)
			&&	(lImageFormat = (BITMAPINFO*)&lVideoInfo->bmiHeader)
			)
		{
			lTransparency = lAgentFile->GetTransparency();
			lTransparentColor = (mBkColor.Ptr()) ? *mBkColor : GetSysColor (COLOR_WINDOW);

			((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbRed = GetRValue(lTransparentColor);
			((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbGreen = GetGValue(lTransparentColor);
			((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbBlue = GetBValue(lTransparentColor);

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
					LogVfwErr (LogNormal, mVideoOutPin->GetInterface()->Disconnect ());
					LogVfwErr (LogNormal, mVideoOutPin->GetInterface()->Connect (lInputPin, lMediaType));
				}
			}
		}
	}
	catch AnyExceptionDebug

	return true;
}

const COLORREF * CDirectShowSource::GetBkColor () const
{
	CSingleLock	lLock (&mStateLock, TRUE);
	return mBkColor;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::InitializePins ()
{
	CAgentFile *		lAgentFile;
	tMediaTypePtr		lMediaType;
	CSize				lImageSize;
	UINT				lImageFormatSize;
	tArrayPtr <BYTE>	lImageFormatBuffer;
	BITMAPINFO *		lImageFormat;
	CString				lPinName;
	bool				l32BitSamples;
	VIDEOINFOHEADER *	lVideoInfo;

	if	(mBkColor)
	{
		l32BitSamples = false;
		lPinName = _T("RGB8");
	}
	else
	{
		l32BitSamples = true;
		lPinName = _T("ARGB32");
	}

	if	(lAgentFile = GetAgentFile ())
	{
		lImageSize = lAgentFile->GetImageSize();
		lImageFormatSize = lAgentFile->GetImageFormat (NULL, NULL, l32BitSamples);
		if	(lImageFormatBuffer = new BYTE [lImageFormatSize])
		{
			lImageFormat = (BITMAPINFO*)lImageFormatBuffer.Ptr();
		}

		if	(
				(lImageFormat)
			&&	(lAgentFile->GetImageFormat (lImageFormat, NULL, l32BitSamples))
			&&	(mVideoOutPin = new CDirectShowPinOut (*this, _T("Animation Out"), lPinName))
			)
		{
			if	(l32BitSamples)
			{
				if	(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), sizeof(VIDEOINFOHEADER)+lImageFormatSize-sizeof(BITMAPINFOHEADER))))
				{
					lMediaType->majortype = MEDIATYPE_Video;
					lMediaType->subtype = MEDIASUBTYPE_ARGB32;
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
				BYTE		lTransparency = lAgentFile->GetTransparency();
				COLORREF	lTransparentColor = *mBkColor;

				((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbRed = GetRValue(lTransparentColor);
				((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbGreen = GetGValue(lTransparentColor);
				((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbBlue = GetBValue(lTransparentColor);

				if	(SUCCEEDED (MoCreateMediaType (lMediaType.Free(), sizeof(VIDEOINFOHEADER)+lImageFormatSize-sizeof(BITMAPINFOHEADER))))
				{
					lMediaType->majortype = MEDIATYPE_Video;
					lMediaType->subtype = MEDIASUBTYPE_RGB8;
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
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::OnJoinedFilterGraph ()
{
	CDirectShowFilter::OnJoinedFilterGraph ();
	SetTimes (0, GetDuration());
}

void CDirectShowSource::OnLeftFilterGraph ()
{
	Terminate ();
	CDirectShowFilter::OnLeftFilterGraph ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSource::OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState)
{
	HRESULT	lResult = CDirectShowFilter::OnStateChanged (pOldState, pNewState);

#ifdef	_LOG_DIRECT_SHOW
	if	(
			(pNewState == State_Stopped)
		&&	(pOldState != State_Stopped)
		)
	{
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Stopped> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
	else
	if	(
			(pNewState == State_Paused)
		&&	(pOldState == State_Stopped)
		)
	{
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Started> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
	else
	if	(
			(pNewState == State_Running)
		&&	(pOldState == State_Paused)
		)
	{
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Running> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u)]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSource::OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags)
{
	CDirectShowSeeking::OnTimesChanged (pCurrTime, pCurrentFlags, pStopTime, pStopFlags);

	if	(mVideoOutPin.Ptr ())
	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			if	((pStopFlags & AM_SEEKING_NoFlush) == 0)
			{
#ifdef	_DEBUG_SAMPLES
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Flush VideoOut"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
#endif
				Flush ();
			}
			if	(pStopFlags & AM_SEEKING_Segment)
			{
#ifdef	_DEBUG_SAMPLES
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Restart VideoOut"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
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
	if	(!PutVideoFrame ())
	{
		if	(mVideoOutPin)
		{
			mVideoOutPin->EndOutputStream ();
		}

#ifdef	_LOG_DIRECT_SHOW
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Complete> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
		StopClock ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDirectShowSource::PutVideoFrame ()
{
	bool			lRet = false;
	REFERENCE_TIME	lCurrTime;
	REFERENCE_TIME	lStopTime;
	REFERENCE_TIME	lDuration = 0;

	GetTimes (lCurrTime, lStopTime);

	if	(lCurrTime < lStopTime)
	{
		if	(PutVideoSample (lCurrTime, lStopTime) == S_OK)
		{
			if	(GetAgentStreamInfo ())
			{
				lDuration = GetDuration ();
			}
			SetTimes (lCurrTime, lStopTime, lDuration);
		}
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
					(lAgentFile = GetAgentFile())
				&&	(lStreamInfo = GetAgentStreamInfo())
				)
			{
				lTransparency = lAgentFile->GetTransparency();
				lStreamInfo->Lock ();

				try
				{
					lAnimationSequence = lStreamInfo->GetAnimationSequence ();
					lSpeakingDuration = lStreamInfo->GetSpeakingDuration ();
					lSpeakingDuration = min (lSpeakingDuration, 5);

					if	(
							(lAnimationSequence)
						&&	(lStreamInfo->CalcSequenceAnimationFrameNdx (&lAnimationNdx, &lFrameNdx, (lSpeakingDuration > 0) ? 0 : lTimeNdx, true) == S_OK)
						&&	(lAnimation = lAgentFile->GetAnimation (lAnimationNdx))
						&&	(lFrameNdx >= 0)
						&&	(lFrameNdx < (long)(short)lAnimation->mFrameCount)
						)
					{
						lFrame = &lAnimation->mFrames [lFrameNdx];

						lMediaStartTime = lTimeNdx;
						lMouthOverlayNdx = lStreamInfo->GetMouthOverlay (lTimeNdx);

						if	(lSpeakingDuration > 0)
						{
							lEndTime = lStartTime + ((LONGLONG)lSpeakingDuration * MsPer100Ns);
							lMediaEndTime = lTimeNdx + lSpeakingDuration;
						}
						else
						{
							lEndTime = lStartTime + (((LONGLONG)(long)(short)lFrame->mDuration) * MsPer100Ns);
							lMediaEndTime = lTimeNdx + (long)(short)lFrame->mDuration;
						}
						lEndTime = min (lEndTime, pStopTime);
					}
#ifdef	_DEBUG_SAMPLES
					else
					if	(!lAnimationSequence)
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No AnimationSequence"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
					}
					else
					if	(!lFrame)
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No more frames [%f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(lStartTime));
					}
#endif
				}
				catch AnyExceptionSilent

				lStreamInfo->Unlock ();
			}
#ifdef	_DEBUG_SAMPLES
			else
			{
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No StreamInfo"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
			}
#endif
		}
		catch AnyExceptionDebug

		mDataLock.Unlock ();
	}
#ifdef	_DEBUG_SAMPLES
	else
	{
		LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Mutex lock failed (a)"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
	}
#endif

	if	(
			(lFrame)
		&&	(lEndTime >= lStartTime)
		)
	{
		if	(
				(lAgentFile = GetAgentFile())
			&&	(mVideoOutPin)
			&&	(SUCCEEDED (lResult = mVideoOutPin->GetOutputSample (&lSample, &lStartTime, &lEndTime, 0)))
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
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Frame [%3.3d] [%3.3d] Overlay [%hd] at MediaTime [%I64d - %I64d] TimeStamp [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), lAnimationNdx, lFrameNdx, lMouthOverlayNdx, lMediaStartTime, lMediaEndTime, RefTimeSec(lStartTime), RefTimeSec(lEndTime));
					}
					else
					{
						LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Frame [%3.3d] [%3.3d] Empty at MediaTime [%I64d - %I64d] TimeStamp [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), lAnimationNdx, lFrameNdx, lMediaStartTime, lMediaEndTime, RefTimeSec(lStartTime), RefTimeSec(lEndTime));
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
				LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Mutex lock failed (b)"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
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
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] Frame discarded"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
				}
				else
				{
					LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No frame"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
				}
			}
#endif
		}
#ifdef	_DEBUG_SAMPLES_NOT
		else
		{
			LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] No output buffer"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)));
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

	if	(lStreamInfo = GetAgentStreamInfo())
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
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		INT_PTR	lAudioNdx;

		if	(pAnimationSequence)
		{
			for	(lAudioNdx = 0; lAudioNdx <= pAnimationSequence->mAudio.GetUpperBound (); lAudioNdx++)
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

	if	(lStreamInfo = GetAgentStreamInfo())
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
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		INT_PTR					lAudioNdx;
		CDirectSoundPinPush *	lAudioPin;
		long					lSoundNdx;
		POSITION				lPos;

		for	(lPos = mAudioOutPins.GetStartPosition(); lPos;)
		{
			mAudioOutPins.GetNextAssoc (lPos, lSoundNdx, lAudioPin);
			if	(lAudioPin)
			{
				lAudioPin->DisconnectFilters ();
			}
		}
		mAudioOutPins.RemoveAll ();

		if	(pAnimationSequence)
		{
			for	(lAudioNdx = 0; lAudioNdx <= pAnimationSequence->mAudio.GetUpperBound (); lAudioNdx++)
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
#ifdef	_LOG_DIRECT_SHOW
		if	(
				(pAnimationSequence)
			&&	(pAnimationSequence->mAudio.GetSize() > 0)
			)
		{
			LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Sounds <%d Ready> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u]"), RefTimeSec(GetReferenceTime()), mAudioOutPins.GetSize(), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
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
		&&	(lAgentFile = GetAgentFile ())
		&&	(lSoundSize = lAgentFile->GetSoundSize (pSoundNdx))
		&&	(lSound = lAgentFile->GetSound (pSoundNdx))
		&&	(lAudioPin = new CDirectSoundPinPush (*this, *this, pSoundNdx))
		)
	{
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
	LONGLONG			lDuration;
	CAgentStreamInfo *	lStreamInfo;
	long				lSequenceDuration;

	if	(lStreamInfo = GetAgentStreamInfo ())
	{
		lSequenceDuration = lStreamInfo->GetSpeakingDuration ();

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
		{
			lDuration = lStreamInfo->MaxSequenceDuration ();
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

HRESULT	CDirectShowSource::SegmentDurationChanged ()
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CSingleLock	lLock (&mDataLock, TRUE);

	try
	{
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
		LogMessage (_DEBUG_SAMPLES, _T("[%s] [%p] [%f] SegmentDurationChanged [%f - %f] of [%f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(lDuration));
#endif
#ifdef	_LOG_DIRECT_SHOW
		LogMessage (_LOG_DIRECT_SHOW, _T("  [%f] DirectShow Source <Ready> (Duration [%f] Curr [%f] Stop [%f]) [%s (%u %u]"), RefTimeSec(GetReferenceTime()), RefTimeSec(GetDuration()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), FilterStateStr(mState), IsClockStarted(), IsClockSet());
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::XFileSource::Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowSource, FileSource)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] %s::FileSource::Load [%ls]"), pThis, pThis->m_dwRef, ObjClassName(pThis), pszFileName);
#endif
	HRESULT		lResult = E_FAIL;
	CSingleLock	lLock (&pThis->mStateLock, TRUE);

	try
	{
		if	(pThis->GetAgentFile ())
		{
			lResult = E_UNEXPECTED;
		}
		else
		if	(SUCCEEDED (lResult = pThis->OpenFile (CString (pszFileName))))
		{
			pThis->ReadFile ();
		}
	}
	catch AnyExceptionDebug

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XFileSource::Load"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSource::XFileSource::GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt)
{
	METHOD_PROLOGUE(CDirectShowSource, FileSource)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] %s::XFileSource::GetCurFile"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = E_FAIL;

	if	(!ppszFileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		CAgentFile *	lAgentFile;
		CSingleLock		lLock (&pThis->mStateLock, TRUE);

		try
		{
			if	(lAgentFile = pThis->GetAgentFile ())
			{
				(*ppszFileName) = AfxAllocTaskOleString ((BSTR)lAgentFile->GetPath ());
			}
		}
		catch AnyExceptionDebug
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] %s::XFileSource::GetCurFile"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE CDirectShowSource::XFilterMiscFlags::GetMiscFlags ()
{
	METHOD_PROLOGUE(CDirectShowSource, FilterMiscFlags)
#ifdef	_DEBUG_INTERFACE_EX
	LogMessage (_DEBUG_INTERFACE_EX, _T("[%p(%u)] %s::XFilterMiscFlags::GetMiscFlags"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	return AM_FILTER_MISC_FLAGS_IS_SOURCE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSource::XConfigCallback::Reconfigure (PVOID pvContext, DWORD dwFlags)
{
	METHOD_PROLOGUE(CDirectShowSource, ConfigCallback)
#ifdef	_DEBUG_INTERFACE_EX
	LogMessage (_DEBUG_INTERFACE_EX, _T("[%p(%u)] %s::XConfigCallback::Reconfigure"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = S_OK;

	try
	{
		//lResult = pThis->ReconnectCallback (dwFlags);
	}
	catch AnyExceptionDebug

	return lResult;
}
