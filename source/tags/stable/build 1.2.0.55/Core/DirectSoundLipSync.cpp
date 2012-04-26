/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "DirectSoundLipSync.h"
#include "DirectShowFilter.h"
#include "DirectShowSource.h"
#include "AgentStreamInfo.h"
#include "VfwErr.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "DirectShowTrace.h"
#endif

#pragma warning (disable: 4355)

#ifdef	_DEBUG
//#define	_DEBUG_CONNECT	LogNormal
//#define	_DEBUG_FORMAT	LogNormal
#define	_DEBUG_LIP_SYNC		(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_DirectShowFilter"),LogVerbose,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

CDirectSoundLipSync::CDirectSoundLipSync ()
:	mDuration (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectSoundLipSync::CDirectSoundLipSync (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectSoundLipSync::~CDirectSoundLipSync ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectSoundLipSync::~CDirectSoundLipSync (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Disconnect ();
}

void CDirectSoundLipSync::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectSoundLipSync::~CDirectSoundLipSync (%d) [%8.8X %8.8X]"), this, _CoreAnchor.Module.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Disconnect ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundLipSync::Connect (IGraphBuilder * pGraphBuilder, LPCTSTR pWaveFileName, CAgentStreamInfo* pStreamInfo)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(
				(pGraphBuilder)
			&&	(pWaveFileName)
			&&	(pWaveFileName[0])
			)
		{
			CAtlString	lFilterName;

			SetAgentStreamInfo (pStreamInfo);
			mFilterName = PathFindFileName (pWaveFileName);
#ifdef	_DEBUG_CONNECT
			LogWaveSound (_DEBUG_CONNECT, pWaveFileName, _T("CDirectSoundLipSync"));
#endif
			if	(
					(SUCCEEDED (lResult))
				&&	(mGrabberFilter == NULL)
				)
			{
				lFilterName.Format (_T("%s Lip Sync"), mFilterName);
				lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (__uuidof(SampleGrabber), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mGrabberFilter));

				if	(
						(SUCCEEDED (lResult))
					&&	(FAILED (lResult = LogVfwErr (LogNormal|LogTime, pGraphBuilder->AddFilter (mGrabberFilter, lFilterName))))
					)
				{
					SafeFreeSafePtr (mGrabberFilter);
				}
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(mWaveParser == NULL)
				)
			{
				lFilterName.Format (_T("%s Wave Parser"), mFilterName);
				lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (__uuidof(WaveParser), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mWaveParser));

				if	(
						(SUCCEEDED (lResult))
					&&	(FAILED (lResult = LogVfwErr (LogNormal|LogTime, pGraphBuilder->AddFilter (mWaveParser, lFilterName))))
					)
				{
					SafeFreeSafePtr (mWaveParser);
				}
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(mAudioRender == NULL)
				)
			{
				lFilterName.Format (_T("%s Audio Render"), mFilterName);
				lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_DSoundRender, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mAudioRender));

				if	(
						(SUCCEEDED (lResult))
					&&	(FAILED (lResult = LogVfwErr (LogNormal|LogTime, pGraphBuilder->AddFilter (mAudioRender, lFilterName))))
					)
				{
					SafeFreeSafePtr (mAudioRender);
				}
			}

			if	(SUCCEEDED (lResult))
			{
				if	(PathIsURL (pWaveFileName))
				{
					lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_URLReader, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mFileSource));
				}
				else
				{
					lResult = LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_AsyncReader, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void**) &mFileSource));
				}

				if	(SUCCEEDED (lResult))
				{
					IFileSourceFilterPtr	lFileSource (mFileSource);
					tMallocPtr <OLECHAR>	lFileName;

					if	(lFileSource == NULL)
					{
						lResult = E_FAIL;
					}
					else
					if	(lFileName = AtlAllocTaskOleString (pWaveFileName))
					{
						lResult = LogVfwErr (LogNormal|LogTime, lFileSource->Load (lFileName, NULL));
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}

				if	(
						(SUCCEEDED (lResult))
					&&	(FAILED (lResult = LogVfwErr (LogNormal|LogTime, pGraphBuilder->AddFilter (mFileSource, pWaveFileName))))
					)
				{
					SafeFreeSafePtr (mFileSource);
				}
			}

			if	(SUCCEEDED (lResult))
			{
				mFilterChain = pGraphBuilder;
				mGrabber = mGrabberFilter;

				if	(
						(mFilterChain == NULL)
					||	(mGrabber == NULL)
					)
				{
					lResult = E_FAIL;
				}
			}

			if	(SUCCEEDED (lResult))
			{
				IPinPtr				lFileOutPin;
				IPinPtr				lWaveInPin;
				IPinPtr				lWaveOutPin;
				IPinPtr				lGrabberInPin;
				tMediaTypePtr		lFileMediaType;
				AM_MEDIA_TYPE_Safe	lGrabberMediaType;

				try
				{
					if	(
							(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, GetFilterPin (mFileSource, PINDIR_OUTPUT, MEDIATYPE_Stream, &lFileOutPin, lFileMediaType.Free()))))
						&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, GetFilterPins (mWaveParser, &lWaveInPin, NULL))))
						)
					{
						lResult = LogVfwErr (LogNormal|LogTime, pGraphBuilder->ConnectDirect (lFileOutPin, lWaveInPin, lFileMediaType));
					}
					if	(
							(SUCCEEDED (lResult))
						&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, GetFilterPins (mWaveParser, NULL, &lWaveOutPin))))
						&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal|LogTime, GetFilterPins (mGrabberFilter, &lGrabberInPin, NULL))))
						)
					{
						lResult = LogVfwErr (LogNormal|LogTime, pGraphBuilder->ConnectDirect (lWaveOutPin, lGrabberInPin, NULL));
					}
					if	(SUCCEEDED (lResult))
					{
						lResult = LogVfwErr (LogNormal|LogTime, ConnectFilters (pGraphBuilder, mAudioRender, mGrabberFilter, false));

						if	(
								(SUCCEEDED (lResult))
							&&	(SUCCEEDED (LogVfwErr (LogNormal|LogTime, GetFilterPins (mGrabberFilter, &lGrabberInPin, NULL))))
							&&	(SUCCEEDED (LogVfwErr (LogNormal|LogTime, lGrabberInPin->ConnectionMediaType (&lGrabberMediaType))))
							)
						{
							IMediaSeekingPtr	lMediaSeeking (mAudioRender);

							MoDuplicateMediaType ((DMO_MEDIA_TYPE **)mMediaType.Free(), lGrabberMediaType);
							if	(lMediaSeeking != NULL)
							{
								LogVfwErr (LogNormal|LogTime, lMediaSeeking->GetDuration (&mDuration));
							}
#ifdef	_DEBUG_FORMAT
							LogMediaType (_DEBUG_FORMAT, lGrabberMediaType, _T("CDirectSoundLipSync [%s]"), pWaveFileName);
							LogMessage (_DEBUG_FORMAT, _T("  Duration [%f]"), RefTimeSec (mDuration));
#endif
						}
					}
				}
				catch AnyExceptionDebug
			}

			if	(SUCCEEDED (lResult))
			{
				LogVfwErr (LogNormal|LogTime, mGrabber->SetCallback (this, 1));
				LogVfwErr (LogNormal|LogTime, mGrabber->SetOneShot (FALSE));
			}

#ifdef	_DEBUG_CONNECT
			if	(SUCCEEDED (lResult))
			{
				LogFilter (_DEBUG_CONNECT, mFileSource);
				LogFilter (_DEBUG_CONNECT, mWaveParser);
				LogFilter (_DEBUG_CONNECT, mGrabberFilter);
				LogFilter (_DEBUG_CONNECT, mAudioRender);
			}
#endif

			if	(FAILED (lResult))
			{
				Disconnect (pGraphBuilder);
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectSoundLipSync::Disconnect (IGraphBuilder * pGraphBuilder)
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		IGraphBuilderPtr	lGraphBuilder (pGraphBuilder);
		FILTER_INFO_Safe	lFilterInfo;

		if	(lGraphBuilder == NULL)
		{
			lGraphBuilder = mFilterChain;
		}

		if	(
				(lGraphBuilder == NULL)
			&&	(mGrabberFilter != NULL)
			&&	(SUCCEEDED (lResult = mGrabberFilter->QueryFilterInfo (&lFilterInfo)))
			)
		{
			lGraphBuilder = lFilterInfo.pGraph;
		}

		if	(
				(mFileSource != NULL)
			&&	(mAudioRender != NULL)
			&&	(mFilterChain != NULL)
			)
		{
			try
			{
				LogVfwErr (LogVerbose|LogTime, mFilterChain->StopChain (mFileSource, mAudioRender));
				LogVfwErr (LogVerbose|LogTime, mFilterChain->RemoveChain (mFileSource, mAudioRender));
			}
			catch AnyExceptionDebug
		}

		if	(lGraphBuilder != NULL)
		{
			if	(mFileSource != NULL)
			{
				LogVfwErr (LogDetails|LogTime, lGraphBuilder->RemoveFilter (mFileSource));
			}
			if	(mWaveParser != NULL)
			{
				LogVfwErr (LogDetails|LogTime, lGraphBuilder->RemoveFilter (mWaveParser));
			}
			if	(mAudioRender != NULL)
			{
				LogVfwErr (LogDetails|LogTime, lGraphBuilder->RemoveFilter (mAudioRender));
			}
			if	(mGrabberFilter != NULL)
			{
				lResult = LogVfwErr (LogDetails|LogTime, lGraphBuilder->RemoveFilter (mGrabberFilter));
			}
		}
		else
		if	(SUCCEEDED (lResult))
		{
			lResult = S_FALSE;
		}

		SafeFreeSafePtr (mGrabber);
		SafeFreeSafePtr (mGrabberFilter);
		SafeFreeSafePtr (mWaveParser);
		SafeFreeSafePtr (mAudioRender);
		SafeFreeSafePtr (mFilterChain);
		SafeFreeSafePtr (mMediaType);
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundLipSync::Start ()
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(
				(mFileSource != NULL)
			&&	(mAudioRender != NULL)
			&&	(mFilterChain != NULL)
			)
		{
			lResult = LogVfwErr (LogNormal|LogTime, mFilterChain->StartChain (mFileSource, mAudioRender));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectSoundLipSync::Stop ()
{
	HRESULT		lResult = S_FALSE;
	CLockMutex	lLock (mStateLock);

	try
	{
		if	(
				(mFileSource != NULL)
			&&	(mAudioRender != NULL)
			&&	(mFilterChain != NULL)
			)
		{
			lResult = LogVfwErr (LogNormal|LogTime, mFilterChain->StopChain (mFileSource, mAudioRender));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectSoundLipSync::SampleCB (double SampleTime, IMediaSample*pSample)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDirectSoundLipSync::BufferCB (double SampleTime, BYTE*pBuffer, long BufferLen)
{
	if	(BufferLen >= 2)
	{
		try
		{
			long				lBufferVal = 0;
			long				lNdx;
			CAgentStreamInfo*	lStreamInfo;

			for	(lNdx = 0; lNdx <= min (BufferLen/2, 7); lNdx++)
			{
				lBufferVal += (long)((LPWORD) pBuffer) [lNdx];
			}
			if	(lBufferVal <= 3000)
			{
				lNdx = 0;
			}
			else
			{
				lNdx = MulDiv (MulDiv (lBufferVal, 2, lNdx), 1, USHRT_MAX) + 1;
			}

#ifdef	_DEBUG_LIP_SYNC
			if	(LogIsActive (_DEBUG_LIP_SYNC))
			{
				LogMessage (_DEBUG_LIP_SYNC, _T("Buffer [%d] at [%f] [%4d] Value [%f] Mouth [%d] [%s]"), BufferLen, SampleTime, (long)(SampleTime*Ns100PerSec/MsPer100Ns), (float)(long)lBufferVal/(float)(long)USHRT_MAX, lNdx, MouthOverlayStr((short)lNdx));
			}
#endif
			if	(lStreamInfo = GetAgentStreamInfo ())
			{
				lStreamInfo->SetMouthOverlay ((short)lNdx, (long)(SampleTime*Ns100PerSec/MsPer100Ns));
			}
		}
		catch AnyExceptionSilent
	}
	return S_OK;
}
