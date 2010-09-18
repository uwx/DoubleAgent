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
#include <shlwapi.h>
#include "DirectShowWnd.h"
#include "DirectShowPins.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#include "Elapsed.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#endif

#define INITGUID
#include <guiddef.h>
#include <uuids.h>
#include <Qnetwork.h>
#include <mmreg.h>
#include <mpegtype.h>
#include <dmodshow.h>
#include <dmoreg.h>
#undef	INITGUID

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "dmoguids.lib")

#ifdef	_DEBUG
//#define	_DEBUG_FILTERS		LogNormal|LogHighVolume
//#define	_DEBUG_EVENTS		LogNormal|LogHighVolume|LogTimeMs
//#define	_LOG_GRAPH_BUILDER	LogNormal
#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_GRAPH_BUILDER
#define	_LOG_GRAPH_BUILDER	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IMediaEventSink, __uuidof(IMediaEventSink));
_COM_SMARTPTR_TYPEDEF (IGraphConfig, __uuidof(IGraphConfig));
_COM_SMARTPTR_TYPEDEF (IFilterGraph, __uuidof(IFilterGraph));
_COM_SMARTPTR_TYPEDEF (IEnumFilters, __uuidof(IEnumFilters));
_COM_SMARTPTR_TYPEDEF (IBaseFilter, __uuidof(IBaseFilter));
_COM_SMARTPTR_TYPEDEF (IEnumPins, __uuidof(IEnumPins));
_COM_SMARTPTR_TYPEDEF (IEnumMediaTypes, __uuidof(IEnumMediaTypes));
_COM_SMARTPTR_TYPEDEF (IPin, __uuidof(IPin));

/////////////////////////////////////////////////////////////////////////////

const UINT	CDirectShowWnd::mEventMsg = RegisterWindowMessage (_T("1147E563-A208-11DE-ABF2-002421116FB2"));

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CDirectShowWnd)

CDirectShowWnd::CDirectShowWnd()
:	mAutoSize (true),
	mAutoRewind (false),
	mAlphaSmoothing (0)
{
}

CDirectShowWnd::~CDirectShowWnd()
{
	Close ();

	if	(IsWindow ())
	{
		try
		{
			DestroyWindow ();
		}
		catch AnyExceptionSilent
	}
	else
	if	(m_hWnd)
	{
		Detach ();
	}
}

CDirectShowWnd * CDirectShowWnd::CreateInstance ()
{
	return new CDirectShowWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDirectShowWnd::Open (LPCTSTR pFileName)
{
	bool	lRet = false;

	try
	{
		Opening (pFileName);
	}
	catch AnyExceptionSilent

	try
	{
		if	(SUCCEEDED (_Open (pFileName)))
		{
#ifdef	_DEBUG_FILTERS
			LogState (_DEBUG_FILTERS);
			LogStatus (_DEBUG_FILTERS);
#endif
			lRet = true;
		}
		else
		{
			_Close ();
		}
	}
	catch AnyExceptionSilent

	if	(lRet)
	{
		try
		{
			Opened ();
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

HRESULT CDirectShowWnd::_Open (LPCTSTR pFileName)
{
	HRESULT	lResult = E_FAIL;

	if	(this)
	{
		try
		{
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::_Open [%s]"), this, pFileName);
#endif
			lResult = Initialize (pFileName);
			if	(SUCCEEDED (lResult))
			{
				lResult = PrepareGraph (pFileName);
			}
			if	(SUCCEEDED (lResult))
			{
				lResult = GraphPrepared (pFileName);
			}
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::_Open Done [%s]"), this, pFileName);
#endif
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::Close ()
{
	try
	{
		Closing ();
	}
	catch AnyExceptionSilent

	_Close ();

	try
	{
		Closed ();
	}
	catch AnyExceptionSilent
}

void CDirectShowWnd::_Close ()
{
	if	(this)
	{
		try
		{
			if	(mGraphBuilder)
			{
#ifdef	_TRACE_RESOURCES
				CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::_Close"), this);
#endif
				try
				{
					mGraphBuilder->Abort ();
					mGraphBuilder->SetLogFile (NULL);
				}
				catch AnyExceptionSilent
#ifdef	_TRACE_RESOURCES
				CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::_Close Done"), this);
#endif
			}

			mLogFile.Close ();
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::Opening (LPCTSTR pFileName)
{
	_Close ();
}

void CDirectShowWnd::Opened ()
{}

void CDirectShowWnd::Closing ()
{
#ifdef	_TRACE_RESOURCES
	if	(mGraphBuilder)
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Closing"), this);
	}
#endif
	Stop ();
}

void CDirectShowWnd::Closed ()
{
	try
	{
#ifdef	_TRACE_RESOURCES
		bool lTraceClosed = false;
		if	(mGraphBuilder)
		{
			lTraceClosed = true;
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Closed"), this);
		}
#endif
		SafeFreeSafePtr (mMediaControl);
		SafeFreeSafePtr (mMediaSeeking);
		SafeFreeSafePtr (mMediaEvent);
		SafeFreeSafePtr (mBasicVideo);
		SafeFreeSafePtr (mBasicAudio);
		SafeFreeSafePtr (mVideoWindow);
		SafeFreeSafePtr (mVMRFilterConfig7);
		SafeFreeSafePtr (mVMRWindowlessControl7);
		SafeFreeSafePtr (mVMRFilterConfig9);
		SafeFreeSafePtr (mVMRWindowlessControl9);
#ifdef	_DEBUG_NOT
		LogFilterStates (LogDebug, true, _T("Closed"));
		LogFilterCache (LogDebug, mGraphBuilder, _T("Closed"));
#endif
		EmptyFilterCache (mGraphBuilder);
		SafeFreeSafePtr (mGraphBuilder);

#ifdef	_TRACE_RESOURCES
		if	(lTraceClosed)
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Closed Done"), this);
		}
#endif
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::Start (DWORD pWaitForCompletion)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Start"), this);
#endif
			DWORD			lStartTime = GetTickCount ();
			OAFilterState	lState;

			if	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mMediaControl->Run ())))
			{
				if	(pWaitForCompletion)
				{
					do
					{
						if	(
								(SUCCEEDED (lResult = mMediaControl->GetState (10, &lState)))
							&&	(lResult != VFW_S_STATE_INTERMEDIATE)
							)
						{
							if	(lState == State_Running)
							{
								lResult = S_OK;
								break;
							}
							else
							{
								lResult = S_FALSE;
							}
						}
						Sleep (min (max ((long) pWaitForCompletion - ElapsedTicks (lStartTime), 10), 100));

					}	while (!TicksElapsed (lStartTime, pWaitForCompletion));
#ifdef	_DEBUG
					if	(lState != State_Running)
					{
						LogMessage (LogDebug, _T("Not running after %ums"), pWaitForCompletion);
					}
#endif
				}
			}
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Start Done"), this);
#endif
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowWnd::Stop (DWORD pWaitForCompletion)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Stop"), this);
#endif
			DWORD			lStartTime = GetTickCount ();
			OAFilterState	lState;

			if	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mMediaControl->Stop ())))
			{
				if	(pWaitForCompletion)
				{
					do
					{
						if	(
								(SUCCEEDED (lResult = mMediaControl->GetState (10, &lState)))
							&&	(lResult != VFW_S_STATE_INTERMEDIATE)
							)
						{
							if	(lState == State_Stopped)
							{
								lResult = S_OK;
								break;
							}
							else
							{
								lResult = S_FALSE;
							}
						}
						Sleep (min (max ((long) pWaitForCompletion - ElapsedTicks (lStartTime), 10), 100));

					}	while (!TicksElapsed (lStartTime, pWaitForCompletion));
#ifdef	_DEBUG
					if	(lState != State_Stopped)
					{
						LogMessage (LogDebug, _T("Not stopped after %ums"), pWaitForCompletion);
					}
#endif
				}
			}
			if	(mAutoRewind)
			{
				Rewind ();
			}
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Stop Done"), this);
#endif
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::Pause (DWORD pWaitForCompletion)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Pause"), this);
#endif
			DWORD			lStartTime = GetTickCount ();
			OAFilterState	lState;

			if	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mMediaControl->Pause ())))
			{
				if	(pWaitForCompletion)
				{
					do
					{
						if	(
								(SUCCEEDED (lResult = mMediaControl->GetState (10, &lState)))
							&&	(lResult != VFW_S_STATE_INTERMEDIATE)
							)
						{
							if	(lState == State_Paused)
							{
								lResult = S_OK;
								break;
							}
							else
							{
								lResult = S_FALSE;
							}
						}
						Sleep (min (max ((long) pWaitForCompletion - ElapsedTicks (lStartTime), 10), 100));

					}	while (!TicksElapsed (lStartTime, pWaitForCompletion));
#ifdef	_DEBUG
					if	(lState != State_Paused)
					{
						LogMessage (LogDebug, _T("Not paused after %ums"), pWaitForCompletion);
					}
#endif
				}
			}
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Pause Done"), this);
#endif
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowWnd::Resume (DWORD pWaitForCompletion)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Resume"), this);
#endif
			DWORD			lStartTime = GetTickCount ();
			OAFilterState	lState;

			if	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mMediaControl->Run ())))
			{
				if	(pWaitForCompletion)
				{
					do
					{
						if	(
								(SUCCEEDED (lResult = mMediaControl->GetState (10, &lState)))
							&&	(lResult != VFW_S_STATE_INTERMEDIATE)
							)
						{
							if	(lState == State_Running)
							{
								lResult = S_OK;
								break;
							}
							else
							{
								lResult = S_FALSE;
							}
						}
						Sleep (min (max ((long) pWaitForCompletion - ElapsedTicks (lStartTime), 10), 100));

					}	while (!TicksElapsed (lStartTime, pWaitForCompletion));
#ifdef	_DEBUG
					if	(lState != State_Running)
					{
						LogMessage (LogDebug, _T("Not resumed after %ums"), pWaitForCompletion);
					}
#endif
				}
			}
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Resume Done"), this);
#endif
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowWnd::Rewind ()
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(mMediaSeeking != NULL)
		)
	{
		try
		{
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Rewind"), this);
#endif
			LONGLONG	lCurrPosition = 0;
			LONGLONG	lStopPosition = 0;

			if	(SUCCEEDED (LogVfwErr (LogNormal, mMediaSeeking->SetPositions (&lCurrPosition, AM_SEEKING_AbsolutePositioning, &lStopPosition, AM_SEEKING_AbsolutePositioning))))
			{
#ifdef	_DEBUG_NOT
				LogMediaSeeking (LogDebug, mMediaSeeking, _T("[%p] Rewind"), this);
#endif
				lRet = true;
			}
#ifdef	_TRACE_RESOURCES
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowWnd::Rewind Done"), this);
#endif
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDirectShowWnd::IsPlaying (bool pIncludePause) const
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
			HRESULT			lResult;
			OAFilterState	lState;
			int				lRetryCount = 10;

			do
			{
				lResult = mMediaControl->GetState (10, &lState);
			}
			while	(
						(FAILED (lResult))
					&&	(--lRetryCount >= 0)
					);

			if	(
					(SUCCEEDED (lResult))
				&&	(
						(lState == State_Running)
					||	(
							(pIncludePause)
						&&	(lState == State_Paused)
						)
					)
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

bool CDirectShowWnd::IsPaused () const
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
			HRESULT			lResult;
			OAFilterState	lState;
			int				lRetryCount = 10;

			do
			{
				lResult = mMediaControl->GetState (10, &lState);
			}
			while	(
						(FAILED (lResult))
					&&	(--lRetryCount >= 0)
					);

			if	(
					(SUCCEEDED (lResult))
				&&	(lState == State_Paused)
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

bool CDirectShowWnd::IsStopped () const
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
			HRESULT			lResult;
			OAFilterState	lState;
			int				lRetryCount = 10;

			do
			{
				lResult = mMediaControl->GetState (10, &lState);
			}
			while	(
						(FAILED (lResult))
					&&	(--lRetryCount >= 0)
					);

			if	(
					(SUCCEEDED (lResult))
				&&	(lState == State_Stopped)
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowWnd::IsEndOfStream ()
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(mMediaSeeking != NULL)
		)
	{
		try
		{
			LONGLONG	lDuration = 0;
			LONGLONG	lCurrPos = 0;
			LONGLONG	lStopPos = -1;

			if	(
					(SUCCEEDED (mMediaSeeking->GetDuration (&lDuration)))
				&&	(SUCCEEDED (mMediaSeeking->GetPositions (&lCurrPos, &lStopPos)))
				&&	(
						(lCurrPos >= lDuration)
					||	(lCurrPos >= lStopPos)
					)
				)
			{
#ifdef	_DEBUG_EVENTS_NOT
				LogMessage (_DEBUG_EVENTS, _T("[%p] IsEndOfStream [%f - %f] of [%f]"), this, RefTimeSec(lCurrPos), RefTimeSec(lStopPos), RefTimeSec(lDuration));
#endif
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowWnd::IsVideoVisible ()
{
	bool	lRet = false;
	long	lVisible = 0;

	if	(
			(IsWindow ())
		&&	(mVideoWindow != NULL)
		&&	(SUCCEEDED (mVideoWindow->get_Visible (&lVisible)))
		&&	(lVisible)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDirectShowWnd::SetVideoVisible (bool pVisible)
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(mVideoWindow != NULL)
		&&	(SUCCEEDED (LogVfwErr (LogNormal, mVideoWindow->put_Visible (pVisible?OATRUE:OAFALSE))))
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LONGLONG CDirectShowWnd::GetDuration ()
{
	LONGLONG	lRet = 0;

	if	(mMediaSeeking != NULL)
	{
		mMediaSeeking->GetDuration (&lRet);
	}
	return lRet;
}

LONGLONG CDirectShowWnd::GetPosition ()
{
	LONGLONG	lRet = 0;

	if	(mMediaSeeking != NULL)
	{
		mMediaSeeking->GetCurrentPosition (&lRet);
	}
	return lRet;
}

LONGLONG CDirectShowWnd::GetStop ()
{
	LONGLONG	lRet = 0;

	if	(mMediaSeeking != NULL)
	{
		mMediaSeeking->GetStopPosition (&lRet);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::Initialize (LPCTSTR pFileName)
{
	HRESULT		lResult = E_FAIL;
	CAtlString	lFileName (pFileName);

	try
	{
		lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_FilterGraphNoThread, NULL, CLSCTX_INPROC_SERVER, __uuidof (IGraphBuilder), (void **) &mGraphBuilder));

		if	(SUCCEEDED (lResult))
		{
#ifdef	_LOG_GRAPH_BUILDER
			if	(pFileName)
			{
				try
				{
					mLogFile.Close ();

					if	(LogIsActive (_LOG_GRAPH_BUILDER))
					{
						CAtlString	lLogFilePath (gLogFileName);
						CAtlString	lLogFileName (pFileName);

						if	(PathIsURL (pFileName))
						{
							DWORD lParsedSize;
							CoInternetParseUrl (pFileName, PARSE_SCHEMA, 0, lLogFileName.GetBuffer(MAX_PATH), MAX_PATH, &lParsedSize, 0);
							lLogFileName.ReleaseBuffer ();
							lLogFileName = CAtlString(pFileName).Mid (lLogFileName.GetLength()+1);
							lLogFileName.TrimLeft (_T('/'));
							lLogFileName = PathFindFileName (lLogFileName);
						}
						else
						{
							PathStripPath (lLogFileName.GetBuffer (lLogFileName.GetLength ()));
						}
						PathRemoveExtension (lLogFileName.GetBuffer (lLogFileName.GetLength ()));
						lLogFileName.ReleaseBuffer ();
						PathRemoveFileSpec (lLogFilePath.GetBuffer (MAX_PATH));
						PathAppend (lLogFilePath.GetBuffer (MAX_PATH), lLogFileName);
						PathAddExtension (lLogFilePath.GetBuffer (MAX_PATH), _T(".log"));
						lLogFilePath.ReleaseBuffer ();

						mLogFile = CreateFile (lLogFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

						if	(mLogFile.SafeIsValid ())
						{
							LogVfwErr (_LOG_GRAPH_BUILDER, mGraphBuilder->SetLogFile ((DWORD_PTR)(HANDLE)mLogFile));
						}
					}
				}
				catch AnyExceptionDebug
			}
#endif
			mMediaEvent = mGraphBuilder;
			if	(mMediaEvent != NULL)
			{
				LogVfwErr (LogNormal, mMediaEvent->SetNotifyWindow ((OAHWND) m_hWnd, mEventMsg, 0));
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::PrepareGraph (LPCTSTR pFileName)
{
	return E_NOTIMPL;
}

HRESULT CDirectShowWnd::PrepareGraphWindowed (IBaseFilter ** pFilter)
{
	HRESULT			lResult;
	IBaseFilterPtr	lVideoRenderFilter;

	(*pFilter) = NULL;
	SafeFreeSafePtr (mVMRFilterConfig7);
	SafeFreeSafePtr (mVMRWindowlessControl7);
	SafeFreeSafePtr (mVMRFilterConfig9);
	SafeFreeSafePtr (mVMRWindowlessControl9);

	lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_VideoRendererDefault, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &lVideoRenderFilter));
	if	(
			(SUCCEEDED (lResult))
		&&	(lVideoRenderFilter != NULL)
		)
	{
		lResult = LogVfwErr (LogNormal, mGraphBuilder->AddFilter (lVideoRenderFilter, L"Video Renderer (Windowed)"));
		if	(SUCCEEDED (lResult))
		{
			(*pFilter) = lVideoRenderFilter.Detach ();
		}
	}
	return lResult;
}

HRESULT CDirectShowWnd::PrepareGraphWindowless (IBaseFilter ** pFilter)
{
	HRESULT			lResult;
	IBaseFilterPtr	lVideoRenderFilter;

	(*pFilter) = NULL;
	SafeFreeSafePtr (mVMRFilterConfig7);
	SafeFreeSafePtr (mVMRWindowlessControl7);
	SafeFreeSafePtr (mVMRFilterConfig9);
	SafeFreeSafePtr (mVMRWindowlessControl9);

#if	FALSE
	lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &lVideoRenderFilter));
	if	(
			(SUCCEEDED (lResult))
		&&	(lVideoRenderFilter != NULL)
		)
	{
		mVMRFilterConfig9 = lVideoRenderFilter;

		if	(mVMRFilterConfig9 == NULL)
		{
			lResult = E_NOINTERFACE;
		}
		if	(
				(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGraphBuilder->AddFilter (lVideoRenderFilter, L"Video Renderer (Windowless-9)"))))
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRFilterConfig9->SetRenderingMode (VMRMode_Windowless))))
			)
		{
			mVMRWindowlessControl9 = mVMRFilterConfig9;
		}
		if	(FAILED (lResult))
		{
			mVMRFilterConfig9 = NULL;
			mVMRWindowlessControl9 = NULL;
		}
	}

	if	(FAILED (lResult))
#endif
	{
		lVideoRenderFilter = NULL;
		lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &lVideoRenderFilter));
		if	(
				(SUCCEEDED (lResult))
			&&	(lVideoRenderFilter != NULL)
			)
		{
			mVMRFilterConfig7 = lVideoRenderFilter;

			if	(mVMRFilterConfig7 == NULL)
			{
				lResult = E_NOINTERFACE;
			}

			if	(
					(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGraphBuilder->AddFilter (lVideoRenderFilter, L"Video Renderer (Windowless-7)"))))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRFilterConfig7->SetRenderingMode (VMRMode_Windowless))))
				)
			{
				mVMRWindowlessControl7 = mVMRFilterConfig7;
			}
			if	(FAILED (lResult))
			{
				mVMRFilterConfig7 = NULL;
				mVMRWindowlessControl7 = NULL;
			}
		}
	}
	if	(SUCCEEDED (lResult))
	{
		(*pFilter) = lVideoRenderFilter.Detach ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::GraphPrepared (LPCTSTR pFileName)
{
	HRESULT		lResult = E_FAIL;
	CAtlString	lFileName (pFileName);

	try
	{
		mBasicAudio = mGraphBuilder;
		if	(mBasicVideo == NULL)
		{
			mBasicVideo = mGraphBuilder;
		}
		mMediaControl = mGraphBuilder;
		mMediaSeeking = mGraphBuilder;

		if	(mMediaSeeking != NULL)
		{
			LogVfwErr (LogNormal, mMediaSeeking->SetTimeFormat (&TIME_FORMAT_MEDIA_TIME));
		}

		if	(mBasicVideo != NULL)
		{
			if	(SUCCEEDED (lResult = InitVideoWindow ()))
			{
				lResult = S_OK;
			}

#ifdef	_DEBUG_FILTERS
			LogFilters (_DEBUG_FILTERS, false, _T("Final Filter Graph"));
#endif
		}
		else
		{
			lResult = E_NOINTERFACE;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::InitVideoWindow ()
{
	HRESULT	lResult = S_OK;

	try
	{
		long						lVisible;
		CSize						lVideoSize;
		CSize						lAspectRatio;
		const COLORREF				lBorderColor = RGB (0,0,0);
		const VMR_ASPECT_RATIO_MODE	lAspectRatioMode = VMR_ARMODE_NONE;

		mVideoWindow = NULL;

		if	(mBasicVideo == NULL)
		{
			lResult = E_NOINTERFACE;
		}
		if	(SUCCEEDED (lResult))
		{
			if	(mVMRWindowlessControl9 != NULL)
			{
				lResult = LogVfwErr (LogNormal, mVMRWindowlessControl9->GetNativeVideoSize (&lVideoSize.cx, &lVideoSize.cy, &lAspectRatio.cx, &lAspectRatio.cy));

				if	(
						(SUCCEEDED (lResult))
					&&	(
							(lVideoSize.cx <= 0)
						||	(lVideoSize.cx <= 0)
						)
					)
				{
					lResult = E_NOINTERFACE;
				}
				if	(SUCCEEDED (lResult))
				{
					LogVfwErr (LogNormal, mVMRWindowlessControl9->SetVideoClippingWindow (m_hWnd));
					LogVfwErr (LogNormal, mVMRWindowlessControl9->SetBorderColor (lBorderColor));
					LogVfwErr (LogNormal, mVMRWindowlessControl9->SetAspectRatioMode (lAspectRatioMode));
				}
				else
				{
					SafeFreeSafePtr (mVMRFilterConfig9);
					SafeFreeSafePtr (mVMRWindowlessControl9);
				}
			}
			else
			if	(mVMRWindowlessControl7 != NULL)
			{
				lResult = LogVfwErr (LogNormal, mVMRWindowlessControl7->GetNativeVideoSize (&lVideoSize.cx, &lVideoSize.cy, &lAspectRatio.cx, &lAspectRatio.cy));

				if	(
						(SUCCEEDED (lResult))
					&&	(
							(lVideoSize.cx <= 0)
						||	(lVideoSize.cx <= 0)
						)
					)
				{
					lResult = E_NOINTERFACE;
				}
				if	(SUCCEEDED (lResult))
				{
					LogVfwErr (LogNormal, mVMRWindowlessControl7->SetVideoClippingWindow (m_hWnd));
					LogVfwErr (LogNormal, mVMRWindowlessControl7->SetBorderColor (lBorderColor));
					LogVfwErr (LogNormal, mVMRWindowlessControl7->SetAspectRatioMode (lAspectRatioMode));
				}
				else
				{
					SafeFreeSafePtr (mVMRFilterConfig7);
					SafeFreeSafePtr (mVMRWindowlessControl7);
				}
			}
			else
			{
				mVideoWindow = mGraphBuilder;
				if	(
						(mVideoWindow != NULL)
					&&	(SUCCEEDED (mVideoWindow->get_Visible (&lVisible)))
					)
				{
					LogVfwErr (LogNormal, mVideoWindow->put_Owner ((OAHWND) m_hWnd));
					LogVfwErr (LogNormal, mVideoWindow->put_WindowStyle (WS_CHILD|WS_CLIPSIBLINGS));
					LogVfwErr (LogNormal, mVideoWindow->put_AutoShow (OATRUE));
				}
				else
				{
					mVideoWindow = NULL;
				}
			}
		}

		if	(SUCCEEDED (lResult))
		{
			if	(mAutoSize)
			{
				lResult = AutoSizeWindow ();
			}
			else
			{
				lResult = AutoSizeVideo ();
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

CSize CDirectShowWnd::GetVideoSize ()
{
	CSize	lVideoSize (0, 0);
	CSize	lAspectRatio;

	if	(mVMRWindowlessControl9 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl9->GetNativeVideoSize (&lVideoSize.cx, &lVideoSize.cy, &lAspectRatio.cx, &lAspectRatio.cy));
	}
	else
	if	(mVMRWindowlessControl7 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl7->GetNativeVideoSize (&lVideoSize.cx, &lVideoSize.cy, &lAspectRatio.cx, &lAspectRatio.cy));
	}
	else
	if	(mVideoWindow != NULL)
	{
		LogVfwErr (LogNormal, mBasicVideo->GetVideoSize (&lVideoSize.cx, &lVideoSize.cy));
	}
	else
	if	(mBasicVideo != NULL)
	{
		LogVfwErr (LogNormal, mBasicVideo->GetVideoSize (&lVideoSize.cx, &lVideoSize.cy));
	}
	return lVideoSize;
}

CRect CDirectShowWnd::GetVideoRect ()
{
	CRect	lVideoRect (0,0,0,0);
	CRect	lSourceRect;
	long	lVideoVisible = OATRUE;

	if	(mVMRWindowlessControl9 != NULL)
	{
		mVMRWindowlessControl9->GetVideoPosition (&lSourceRect, &lVideoRect);
	}
	else
	if	(mVMRWindowlessControl7 != NULL)
	{
		mVMRWindowlessControl7->GetVideoPosition (&lSourceRect, &lVideoRect);
	}
	else
	if	(
			(mVideoWindow != NULL)
		&&	(SUCCEEDED (mVideoWindow->get_Visible (&lVideoVisible)))
		&&	(lVideoVisible)
		)
	{
		mVideoWindow->GetWindowPosition (&lVideoRect.left, &lVideoRect.top, &lVideoRect.right, &lVideoRect.bottom);
		lVideoRect.right += lVideoRect.left;
		lVideoRect.bottom += lVideoRect.top;
	}
	else
	if	(mBasicVideo != NULL)
	{
		mBasicVideo->GetDestinationPosition (&lVideoRect.left, &lVideoRect.top, &lVideoRect.right, &lVideoRect.bottom);
		lVideoRect.right += lVideoRect.left;
		lVideoRect.bottom += lVideoRect.top;
	}
	return lVideoRect;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::SetVideoRect (const CRect & pVideoRect)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(IsWindow ())
	{
		CRect	lVideoRect (pVideoRect);

#ifdef	_DEBUG_NOT
		GetClientRect (&lClientRect);
		GetWindowRect (&lWinRect);
		LogMessage (LogDebug, _T("[%p] SetVideoRect Window [%s] Client [%s] Video [%s]"), this, FormatRect(lWinRect), FormatRect(lClientRect), FormatRect(lVideoRect));
#endif
		if	(mVMRWindowlessControl9 != NULL)
		{
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRWindowlessControl9->SetVideoPosition (&CRect (CPoint (0, 0), GetVideoSize()), &lVideoRect))))
			{
				Invalidate ();
			}
		}
		else
		if	(mVMRWindowlessControl7 != NULL)
		{
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRWindowlessControl7->SetVideoPosition (&CRect (CPoint (0, 0), GetVideoSize()), &lVideoRect))))
			{
				Invalidate ();
			}
		}
		else
		if	(mVideoWindow != NULL)
		{
			LogVfwErr (LogNormal, lResult = mVideoWindow->SetWindowPosition (lVideoRect.left, lVideoRect.top, lVideoRect.Width (), lVideoRect.Height ()));
		}
		else
		if	(mBasicVideo != NULL)
		{
			if	(SUCCEEDED (LogVfwErr (LogNormal, lResult = mBasicVideo->SetDestinationPosition (lVideoRect.left, lVideoRect.top, lVideoRect.Width (), lVideoRect.Height ()))))
			{
				Invalidate ();
			}
		}
	}
	return lResult;
}

HRESULT CDirectShowWnd::CenterVideo (const CSize * pVideoSize)
{
	HRESULT	lResult = S_FALSE;

	if	(IsWindow ())
	{
		CRect	lVideoRect;
		CRect	lClientRect;

		if	(pVideoSize)
		{
			if	(
					(pVideoSize->cx > 0)
				&&	(pVideoSize->cy > 0)
				)
			{
				lVideoRect.SetRect (0, 0, pVideoSize->cx, pVideoSize->cy);
			}
			else
			{
				lResult = E_INVALIDARG;
			}
		}
		else
		{
			lVideoRect = GetVideoRect();
			if	(lVideoRect.IsRectEmpty ())
			{
				lVideoRect = CRect (CPoint (0, 0), GetVideoSize());
			}
		}

		GetClientRect (&lClientRect);
		lVideoRect.OffsetRect (lClientRect.CenterPoint().x - lVideoRect.CenterPoint().x, lClientRect.CenterPoint().y - lVideoRect.CenterPoint().y);
		lResult = SetVideoRect (lVideoRect);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowWnd::AutoSizeWindow ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(IsWindow ())
	{
		CSize	lVideoSize;
		CRect	lVideoRect;
		CRect	lClientRect;
		CRect	lWinRect;

		GetClientRect (&lClientRect);
		GetWindowRect (&lWinRect);
		lVideoSize = GetVideoSize ();
		lVideoRect.SetRect (0, 0, lVideoSize.cx, lVideoSize.cy);
#ifdef	_DEBUG_NOT
		LogMessage (LogDebug, _T("[%p] AutoSizeWindow Window [%s] Client [%s] Video [%s]"), this, FormatRect(lWinRect), FormatRect(lClientRect), FormatRect(lVideoRect));
#endif
		if	(
				(lClientRect.Width() != lVideoSize.cx)
			||	(lClientRect.Height() != lVideoSize.cy)
			)
		{
			lWinRect.right = lWinRect.left + lVideoSize.cx + lWinRect.Width() - lClientRect.Width();
			lWinRect.bottom = lWinRect.top + lVideoSize.cy + lWinRect.Height() - lClientRect.Height();

			if	(GetStyle () & WS_CHILD)
			{
				GetParent().ScreenToClient (&lWinRect);
			}
			MoveWindow (lWinRect);
		}
		lResult = SetVideoRect (lVideoRect);
	}
	return lResult;
}

HRESULT CDirectShowWnd::AutoSizeVideo (bool pKeepAspectRatio)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(IsWindow ())
	{
		CRect	lTargetRect;

		GetClientRect (&lTargetRect);

		if	(pKeepAspectRatio)
		{
			CSize	lSourceSize = GetVideoSize ();
			CSize	lTargetSize = lTargetRect.Size ();

			if	(MulDiv (lSourceSize.cx, 100, lTargetSize.cx) > MulDiv (lSourceSize.cy, 100, lTargetSize.cy))
			{
				lTargetSize.cy = MulDiv (lTargetSize.cx, lSourceSize.cy, lSourceSize.cx);
			}
			else
			{
				lTargetSize.cx = MulDiv (lTargetSize.cy, lSourceSize.cx, lSourceSize.cy);
			}
			lTargetRect.OffsetRect ((lTargetRect.Width()-lTargetSize.cx)/2, (lTargetRect.Height()-lTargetSize.cy)/2);
			lTargetRect = CRect (lTargetRect.TopLeft(), lTargetSize);
		}
		lResult = SetVideoRect (lTargetRect);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

DWORD CDirectShowWnd::SetAlphaSmoothing (DWORD pAlphaSmoothing)
{
	DWORD	lRet = mAlphaSmoothing;

	if	(!m_hWnd)
	{
		mAlphaSmoothing = pAlphaSmoothing;
	}
	return lRet;
}

DWORD CDirectShowWnd::GetAlphaSmoothing () const
{
	return mAlphaSmoothing;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CDirectShowWnd::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	Close ();
	return DefWindowProc ();
}

LRESULT CDirectShowWnd::OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if	(mVMRWindowlessControl9 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl9->DisplayModeChanged ());
	}
	else
	if	(mVMRWindowlessControl7 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl7->DisplayModeChanged ());
	}
	return DefWindowProc ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CDirectShowWnd::OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	tS <PAINTSTRUCT>	lPaintStruct;
	CMemDCHandle		lPaintDC;

	if	(lPaintDC = BeginPaint (&lPaintStruct))
	{
		PrintClient (lPaintDC, PRF_CLIENT);
		EndPaint (&lPaintStruct);
	}
	lPaintDC.Detach ();
	return 0;
}

LRESULT CDirectShowWnd::OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if	(EraseWindow ((HDC)wParam, GetEraseColor()))
	{
		return TRUE;
	}
	else
	{
		return DefWindowProc ();
	}
}

LRESULT CDirectShowWnd::OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	HDC	lDC = (HDC)wParam;

	if	(lParam & PRF_ERASEBKGND)
	{
		EraseWindow (lDC, GetEraseColor());
	}
	if	(
			(lParam & PRF_CLIENT)
		&&	(PaintWindow (lDC))
		)
	{
		return 0;
	}
	return DefWindowProc();
}

/////////////////////////////////////////////////////////////////////////////

COLORREF CDirectShowWnd::GetEraseColor ()
{
	return GetSysColor (COLOR_WINDOW);
}

bool CDirectShowWnd::EraseWindow (HDC pDC, COLORREF pBkColor)
{
	CRect	lVideoRect;
	CRect	lClientRect;

	GetClientRect (&lClientRect);

	if	(!GetAlphaSmoothing())
	{
		lVideoRect = GetVideoRect ();
	}
	if	(
			(GetAlphaSmoothing())
		||	(lVideoRect.IsRectEmpty ())
		)
	{
		FillSolidRect (pDC, &lClientRect, pBkColor);
	}
	else
	if	(!lVideoRect.EqualRect (&lClientRect))
	{
		SaveDC (pDC);
		ExcludeClipRect (pDC, lVideoRect.left, lVideoRect.top, lVideoRect.right, lVideoRect.bottom);
		FillSolidRect (pDC, &lClientRect, pBkColor);
		RestoreDC (pDC, -1);
	}
	return true;
}

bool CDirectShowWnd::PaintWindow (HDC pDC)
{
	if	(mVMRWindowlessControl9 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl9->RepaintVideo (m_hWnd, pDC));
		return true;
	}
	else
	if	(mVMRWindowlessControl7 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl7->RepaintVideo (m_hWnd, pDC));
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CDirectShowWnd::OnMediaEvent (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_EVENTS
	if	(mMediaEvent != NULL)
	{
		LogMediaEvent (_DEBUG_EVENTS, mMediaEvent, _T("[%p] OnMediaEvent"), this);
	}
#endif
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

FILTER_STATE CDirectShowWnd::GetState (CAtlString * pStateStr)
{
	OAFilterState	lState = -1;
	HRESULT			lResult;

	if	(mMediaControl != NULL)
	{
		lResult = mMediaControl->GetState (10, &lState);
		if	(
				(SUCCEEDED (lResult))
			&&	(pStateStr)
			)
		{
			*pStateStr = FilterStateStr (lState, (lResult == VFW_S_STATE_INTERMEDIATE));
		}
	}
	return (FILTER_STATE) lState;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::LogState (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		CAtlString		lPrefix;
		CAtlString		lTitle;
		HRESULT			lResult;
		OAFilterState	lState;
		CAtlString		lStateStr;
		LONGLONG		lDuration = -1;
		LONGLONG		lCurrPos = -1;
		LONGLONG		lStopPos = -1;

		if	(pFormat)
		{
			va_list lArgPtr;
			va_start (lArgPtr, pFormat);
			_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
			lTitle.ReleaseBuffer ();
		}
		if	(!lTitle.IsEmpty ())
		{
			lTitle += _T(" ");
		}

		lPrefix.Format (_T("[%p] "), this);
		lResult = mMediaControl->GetState (10, &lState);

		if	(SUCCEEDED (lResult))
		{
			lStateStr = FilterStateStr (lState, (lResult == VFW_S_STATE_INTERMEDIATE));

			if	(
					(mMediaSeeking != NULL)
				&&	(SUCCEEDED (mMediaSeeking->GetDuration (&lDuration)))
				&&	(SUCCEEDED (mMediaSeeking->GetPositions (&lCurrPos, &lStopPos)))
				)
			{
				LogMessage (pLogLevel, _T("%s%State %s At [%f] to [%f] of [%f]"), lPrefix, lTitle, lStateStr, RefTimeSec(lCurrPos), RefTimeSec(lStopPos), RefTimeSec(lDuration));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%State %s"), lPrefix, lTitle, lStateStr);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::LogStatus (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		CAtlString	lPrefix;
		CAtlString	lTitle;

		if	(pFormat)
		{
			va_list lArgPtr;
			va_start (lArgPtr, pFormat);
			_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
			lTitle.ReleaseBuffer ();
		}
		else
		{
			lTitle = _T("Status");
		}

		lPrefix.Format (_T("[%p]"), this);
		LogMessage (pLogLevel, _T("%s %s"), lPrefix, lTitle);

		try
		{
			if	(mMediaControl != NULL)
			{
				HRESULT			lResult;
				OAFilterState	lState;

				LogMessage (pLogLevel, _T("%s   IMediaControl"), lPrefix);
				lResult = mMediaControl->GetState (10, &lState);
				if	(SUCCEEDED (lResult))
				{
					LogMessage (pLogLevel, _T("%s     %s"), lPrefix, FilterStateStr (lState, (lResult == VFW_S_STATE_INTERMEDIATE)));
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(mBasicAudio != NULL)
			{
				long	lVolume = 0;

				LogMessage (pLogLevel, _T("%s   IBasicAudio"), lPrefix);

				if	(SUCCEEDED (mBasicAudio->get_Volume (&lVolume)))
				{
					LogMessage (pLogLevel, _T("%s     Volume        [%d]"), lPrefix, lVolume);
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(mBasicVideo != NULL)
			{
				CPoint	lSrcPos;
				CSize	lSrcSize;
				CPoint	lDstPos;
				CSize	lDstSize;
				CSize	lVidSize;

				LogMessage (pLogLevel, _T("%s   IBasicVideo"), lPrefix);

				if	(SUCCEEDED (mBasicVideo->GetSourcePosition (&lSrcPos.x, &lSrcPos.y, &lSrcSize.cx, &lSrcSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     Source        [%d %d] [%d %d] Default [%u]"), lPrefix, lSrcPos.x, lSrcPos.y, lSrcSize.cx, lSrcSize.cy, (mBasicVideo->IsUsingDefaultSource () == S_OK));
				}
				if	(SUCCEEDED (mBasicVideo->GetDestinationPosition (&lDstPos.x, &lDstPos.y, &lDstSize.cx, &lDstSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     Destination   [%d %d] [%d %d] Default [%u]"), lPrefix, lDstPos.x, lDstPos.y, lDstSize.cx, lDstSize.cy, (mBasicVideo->IsUsingDefaultDestination () == S_OK));
				}
				if	(SUCCEEDED (mBasicVideo->GetVideoSize (&lVidSize.cx, &lVidSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     Video Size    [%d %d]"), lPrefix, lVidSize.cx, lVidSize.cy);
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(mVideoWindow != NULL)
			{
				long	lAutoShow = 1;
				long	lCursorHidden = 1;
				long	lVisible = 1;
				OAHWND	lOwner = NULL;
				CPoint	lPos;
				CSize	lSize;
				CSize	lMinSize (0, 0);
				CSize	lMaxSize (0, 0);

				LogMessage (pLogLevel, _T("%s   IVideoWindow"), lPrefix);

				if	(SUCCEEDED (mVideoWindow->get_Owner (&lOwner)))
				{
					LogMessage (pLogLevel, _T("%s     Owner         [%p] ThisWnd [%p]"), lPrefix, lOwner, m_hWnd);
				}
				if	(SUCCEEDED (mVideoWindow->get_Visible (&lVisible)))
				{
					LogMessage (pLogLevel, _T("%s     Visible       [%d] WndVisible [%d]"), lPrefix, lVisible, (IsWindow () ? ((GetStyle () & WS_VISIBLE) != 0) : -1));
				}
				if	(SUCCEEDED (mVideoWindow->GetWindowPosition (&lPos.x, &lPos.y, &lSize.cx, &lSize.cy)))
				{
					if	(IsWindow ())
					{
						ScreenToClient (&lPos);
					}
					LogMessage (pLogLevel, _T("%s     Position      [%d %d] Size [%d %d]"), lPrefix, lPos.x, lPos.y, lSize.cx, lSize.cy);
				}
				if	(SUCCEEDED (mVideoWindow->GetMinIdealImageSize (&lMinSize.cx, &lMinSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     MinIdealSize  [%d %d]"), lPrefix, lMinSize.cx, lMinSize.cy);
				}
				if	(SUCCEEDED (mVideoWindow->GetMaxIdealImageSize (&lMaxSize.cx, &lMaxSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     MaxIdealSize  [%d %d]"), lPrefix, lMaxSize.cx, lMaxSize.cy);
				}
				if	(SUCCEEDED (mVideoWindow->get_AutoShow (&lAutoShow)))
				{
					LogMessage (pLogLevel, _T("%s     AutoShow      [%d]"), lPrefix, lAutoShow);
				}
				if	(SUCCEEDED (mVideoWindow->IsCursorHidden (&lCursorHidden)))
				{
					LogMessage (pLogLevel, _T("%s     CursorHidden  [%d]"), lPrefix, lCursorHidden);
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(mVMRWindowlessControl9 != NULL)
			{
				CRect		lSrcRect;
				CRect		lDstRect;
				CSize		lMinSize (0, 0);
				CSize		lMaxSize (0, 0);
				COLORREF	lBorderColor;
				DWORD		lAspectMode;

				LogMessage (pLogLevel, _T("%s   IVMRWindowlessControl9"), lPrefix);

				if	(SUCCEEDED (mVMRWindowlessControl9->GetVideoPosition (&lSrcRect, &lDstRect)))
				{
					LogMessage (pLogLevel, _T("%s     Position      [%d %d] Size [%d %d] of [%d %d]"), lPrefix, lDstRect.left, lDstRect.top, lDstRect.Width (), lDstRect.Height (), lSrcRect.Width (), lSrcRect.Height ());
				}
				if	(SUCCEEDED (mVMRWindowlessControl9->GetMinIdealVideoSize (&lMinSize.cx, &lMinSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     MinIdealSize  [%d %d]"), lPrefix, lMinSize.cx, lMinSize.cy);
				}
				if	(SUCCEEDED (mVMRWindowlessControl9->GetMaxIdealVideoSize (&lMaxSize.cx, &lMaxSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     MaxIdealSize  [%d %d]"), lPrefix, lMaxSize.cx, lMaxSize.cy);
				}
				if	(SUCCEEDED (mVMRWindowlessControl9->GetBorderColor (&lBorderColor)))
				{
					LogMessage (pLogLevel, _T("%s     BorderColor   [%2.2X %2.2X %2.2X]"), lPrefix, GetRValue (lBorderColor), GetGValue (lBorderColor), GetBValue (lBorderColor));
				}
				if	(SUCCEEDED (mVMRWindowlessControl9->GetAspectRatioMode (&lAspectMode)))
				{
					LogMessage (pLogLevel, _T("%s     AspectRatio   [%s]"), lPrefix, (lAspectMode == VMR_ARMODE_LETTER_BOX) ? _T("Letter box") : _T("Stretch"));
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(mVMRWindowlessControl7 != NULL)
			{
				CRect		lSrcRect;
				CRect		lDstRect;
				CSize		lMinSize (0, 0);
				CSize		lMaxSize (0, 0);
				COLORREF	lBorderColor;
				DWORD		lAspectMode;

				LogMessage (pLogLevel, _T("%s   IVMRWindowlessControl"), lPrefix);

				if	(SUCCEEDED (mVMRWindowlessControl7->GetVideoPosition (&lSrcRect, &lDstRect)))
				{
					LogMessage (pLogLevel, _T("%s     Position      [%d %d] Size [%d %d] of [%d %d]"), lPrefix, lDstRect.left, lDstRect.top, lDstRect.Width (), lDstRect.Height (), lSrcRect.Width (), lSrcRect.Height ());
				}
				if	(SUCCEEDED (mVMRWindowlessControl7->GetMinIdealVideoSize (&lMinSize.cx, &lMinSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     MinIdealSize  [%d %d]"), lPrefix, lMinSize.cx, lMinSize.cy);
				}
				if	(SUCCEEDED (mVMRWindowlessControl7->GetMaxIdealVideoSize (&lMaxSize.cx, &lMaxSize.cy)))
				{
					LogMessage (pLogLevel, _T("%s     MaxIdealSize  [%d %d]"), lPrefix, lMaxSize.cx, lMaxSize.cy);
				}
				if	(SUCCEEDED (mVMRWindowlessControl7->GetBorderColor (&lBorderColor)))
				{
					LogMessage (pLogLevel, _T("%s     BorderColor   [%2.2X %2.2X %2.2X]"), lPrefix, GetRValue (lBorderColor), GetGValue (lBorderColor), GetBValue (lBorderColor));
				}
				if	(SUCCEEDED (mVMRWindowlessControl7->GetAspectRatioMode (&lAspectMode)))
				{
					LogMessage (pLogLevel, _T("%s     AspectRatio   [%s]"), lPrefix, (lAspectMode == VMR_ARMODE_LETTER_BOX) ? _T("Letter box") : _T("Stretch"));
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(mMediaSeeking != NULL)
			{
				DWORD		lCapFlags = 0;
				GUID		lTimeFormat;
				LONGLONG	lDuration;
				LONGLONG	lCurrPosition;
				LONGLONG	lStopPosition;
				double		lRate;
				LONGLONG	lPreroll;

				LogMessage (pLogLevel, _T("%s   IMediaSeeking"), lPrefix);

				if	(SUCCEEDED (mMediaSeeking->GetCapabilities (&lCapFlags)))
				{
					LogMessage (pLogLevel, _T("%s     SeekingCaps   [%8.8X] [%s]"), lPrefix, lCapFlags, SeekingCapsStr(lCapFlags));
				}

				if	(SUCCEEDED (mMediaSeeking->GetTimeFormat (&lTimeFormat)))
				{
					LogMessage (pLogLevel, _T("%s     TimeFormat    [%s]"), lPrefix, CGuidStr::GuidName(lTimeFormat));
				}
				if	(SUCCEEDED (mMediaSeeking->GetDuration (&lDuration)))
				{
					LogMessage (pLogLevel, _T("%s     Duration      [%f (%I64d)]"), lPrefix, RefTimeSec (lDuration), lDuration);
				}
				if	(SUCCEEDED (mMediaSeeking->GetCurrentPosition (&lCurrPosition)))
				{
					LogMessage (pLogLevel, _T("%s     Position      [%f (%I64d)]"), lPrefix, RefTimeSec (lCurrPosition), lCurrPosition);
				}
				if	(SUCCEEDED (mMediaSeeking->GetStopPosition (&lStopPosition)))
				{
					LogMessage (pLogLevel, _T("%s     StopPosition  [%f (%I64d)]"), lPrefix, RefTimeSec (lStopPosition), lStopPosition);
				}
				if	(SUCCEEDED (mMediaSeeking->GetPreroll (&lPreroll)))
				{
					LogMessage (pLogLevel, _T("%s     Preroll       [%f (%I64d)]"), lPrefix, RefTimeSec (lPreroll), lPreroll);
				}
				if	(SUCCEEDED (mMediaSeeking->GetRate (&lRate)))
				{
					LogMessage (pLogLevel, _T("%s     Rate          [%f]"), lPrefix, lRate);
				}
			}
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::LogFilters (UINT pLogLevel, bool pEnumPinTypes, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			va_list lArgPtr;
			TCHAR	lFormat [4096];

			if	(pFormat)
			{
				va_start (lArgPtr, pFormat);
				_vsntprintf (lFormat, sizeof(lFormat)/sizeof(TCHAR), pFormat, lArgPtr);
				pFormat = lFormat;
			}
			::LogFilters (pLogLevel, mGraphBuilder, pEnumPinTypes, pFormat);
		}
		catch AnyExceptionSilent
	}
}

void CDirectShowWnd::LogFilterStates (UINT pLogLevel, bool pEnumPins, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			va_list lArgPtr;
			TCHAR	lFormat [4096];

			if	(pFormat)
			{
				va_start (lArgPtr, pFormat);
				_vsntprintf (lFormat, sizeof(lFormat)/sizeof(TCHAR), pFormat, lArgPtr);
				pFormat = lFormat;
			}
			::LogFilterStates (pLogLevel, mGraphBuilder, pEnumPins, pFormat);
		}
		catch AnyExceptionSilent
	}
}
