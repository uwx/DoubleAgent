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
#include "UiState.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
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
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_FILTERS		LogNormal|LogHighVolume
//#define	_DEBUG_EVENTS		LogNormal|LogHighVolume|LogTimeMs
//#define	_LOG_GRAPH_BUILDER	LogNormal
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

IMPLEMENT_DYNCREATE(CDirectShowWnd, CWnd)

BEGIN_MESSAGE_MAP(CDirectShowWnd, CWnd)
	//{{AFX_MSG_MAP(CDirectShowWnd)
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_REGISTERED_MESSAGE(mEventMsg, OnMediaEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CDirectShowWnd::CDirectShowWnd()
:	mAutoSize (true),
	mAutoRewind (false),
	mAlphaBlended (false)
{
#ifdef	_DEBUG
	EnableAggregation ();
#endif
}

CDirectShowWnd::~CDirectShowWnd()
{
	Close ();

	if	(IsWindow (m_hWnd))
	{
		try
		{
			DestroyWindow ();
		}
		catch AnyExceptionSilent
	}
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
			lResult = Initialize (pFileName);
			if	(SUCCEEDED (lResult))
			{
				lResult = PrepareGraph (pFileName);
			}
			if	(SUCCEEDED (lResult))
			{
				lResult = GraphPrepared (pFileName);
			}
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
				try
				{
					mGraphBuilder->Abort ();
					mGraphBuilder->SetLogFile (NULL);
				}
				catch AnyExceptionSilent
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
	Stop ();
}

void CDirectShowWnd::Closed ()
{
	try
	{
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
		SafeFreeSafePtr (mGraphBuilder);
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
			(IsWindow (m_hWnd))
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
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
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowWnd::Stop (DWORD pWaitForCompletion)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(IsWindow (m_hWnd))
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
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
			(IsWindow (m_hWnd))
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
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
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowWnd::Resume (DWORD pWaitForCompletion)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(IsWindow (m_hWnd))
		&&	(mMediaControl != NULL)
		)
	{
		try
		{
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
			(IsWindow (m_hWnd))
		&&	(mMediaSeeking != NULL)
		)
	{
		try
		{
			LONGLONG	lCurrPosition = 0;
			LONGLONG	lStopPosition = 0;

			if	(SUCCEEDED (LogVfwErr (LogNormal, mMediaSeeking->SetPositions (&lCurrPosition, AM_SEEKING_AbsolutePositioning, &lStopPosition, AM_SEEKING_AbsolutePositioning))))
			{
#ifdef	_DEBUG_NOT
				LogMediaSeeking (LogDebug, mMediaSeeking, _T("[%p] Rewind"), this);
#endif
				lRet = true;
			}
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
			(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
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
	HRESULT	lResult = E_FAIL;
	CString	lFileName (pFileName);

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
						CString	lLogFilePath (gLogFileName);
						CString	lLogFileName (pFileName);

						if	(PathIsURL (pFileName))
						{
							DWORD lParsedSize;
							CoInternetParseUrl (pFileName, PARSE_SCHEMA, 0, lLogFileName.GetBuffer(MAX_PATH), MAX_PATH, &lParsedSize, 0);
							lLogFileName.ReleaseBuffer ();
							lLogFileName = CString(pFileName).Mid (lLogFileName.GetLength()+1);
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
	HRESULT	lResult = E_FAIL;
	CString	lFileName (pFileName);

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

HRESULT CDirectShowWnd::AutoSizeWindow ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(IsWindow (m_hWnd))
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
				GetParent()->ScreenToClient (&lWinRect);
			}
			MoveWindow (lWinRect);
			GetClientRect (&lClientRect);
		}

		if	(mVMRWindowlessControl9 != NULL)
		{
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRWindowlessControl9->SetVideoPosition (&lVideoRect, &lClientRect))))
			{
				Invalidate ();
			}
		}
		else
		if	(mVMRWindowlessControl7 != NULL)
		{
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRWindowlessControl7->SetVideoPosition (&lVideoRect, &lClientRect))))
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
			LogVfwErr (LogNormal, lResult = mBasicVideo->SetDestinationPosition (lVideoRect.left, lVideoRect.top, lVideoRect.Width (), lVideoRect.Height ()));
		}
	}
	return lResult;
}

HRESULT CDirectShowWnd::AutoSizeVideo (bool pKeepAspectRatio)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(IsWindow (m_hWnd))
	{
		CRect	lClientRect;
		CSize	lVideoSize (0, 0);
		CRect	lVideoRect;
		CSize	lAspectRatio;

		GetClientRect (&lClientRect);
		lVideoSize = GetVideoSize ();
		lVideoRect.SetRect (0, 0, lVideoSize.cx, lVideoSize.cy);

		if	(mVMRWindowlessControl9 != NULL)
		{
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRWindowlessControl9->SetVideoPosition (&lVideoRect, &lClientRect))))
			{
				Invalidate ();
			}
		}
		else
		if	(mVMRWindowlessControl7 != NULL)
		{
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mVMRWindowlessControl7->SetVideoPosition (&lVideoRect, &lClientRect))))
			{
				Invalidate ();
			}
		}
		else
		if	(mVideoWindow != NULL)
		{
			LogVfwErr (LogNormal, lResult = mVideoWindow->SetWindowPosition (lClientRect.left, lClientRect.top, lClientRect.Width (), lClientRect.Height ()));
		}
		else
		if	(mBasicVideo != NULL)
		{
			LogVfwErr (LogNormal, lResult = mBasicVideo->SetDestinationPosition (lClientRect.left, lClientRect.top, lClientRect.Width (), lClientRect.Height ()));
		}
	}
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
	}
	else
	if	(mBasicVideo != NULL)
	{
		mBasicVideo->GetDestinationPosition (&lVideoRect.left, &lVideoRect.top, &lVideoRect.right, &lVideoRect.bottom);
	}
	return lVideoRect;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::OnDestroy()
{
	Close ();
	CWnd::OnDestroy ();
}

void CDirectShowWnd::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	Default ();
	// Skip CWnd response - avoids MFC assertions for custom menus
}

LRESULT CDirectShowWnd::OnDisplayChange(WPARAM wParam, LPARAM lParam)
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
	return CWnd::OnDisplayChange (wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::OnPaint()
{
	CPaintDC lPaintDC (this);
	PrintClient (&lPaintDC, PRF_CLIENT);
}

BOOL CDirectShowWnd::OnEraseBkgnd(CDC *pDC)
{
	if	(EraseWindow (pDC, GetEraseColor()))
	{
		return TRUE;
	}
	else
	{
		return CWnd::OnEraseBkgnd (pDC);
	}
}

LRESULT CDirectShowWnd::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	CDC *	lDC = CDC::FromHandle ((HDC)wParam);

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

	return Default();
}

/////////////////////////////////////////////////////////////////////////////

COLORREF CDirectShowWnd::GetEraseColor ()
{
	return GetSysColor (COLOR_WINDOW);
}

bool CDirectShowWnd::EraseWindow (CDC * pDC, COLORREF pBkColor)
{
	CRect	lVideoRect;
	CRect	lClientRect;

	GetClientRect (&lClientRect);

	if	(!mAlphaBlended)
	{
		lVideoRect = GetVideoRect ();
	}
	if	(
			(mAlphaBlended)
		||	(lVideoRect.IsRectEmpty ())
		)
	{
		pDC->FillSolidRect (&lClientRect, pBkColor);
	}
	else
	if	(!lVideoRect.EqualRect (&lClientRect))
	{
		pDC->SaveDC ();
		pDC->ExcludeClipRect (&lVideoRect);
		pDC->FillSolidRect (&lClientRect, pBkColor);
		pDC->RestoreDC (-1);
	}
	return true;
}

bool CDirectShowWnd::PaintWindow (CDC * pDC)
{
	if	(mVMRWindowlessControl9 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl9->RepaintVideo (m_hWnd, *pDC));
		return true;
	}
	else
	if	(mVMRWindowlessControl7 != NULL)
	{
		LogVfwErr (LogNormal, mVMRWindowlessControl7->RepaintVideo (m_hWnd, *pDC));
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CDirectShowWnd::OnMediaEvent (WPARAM wParam, LPARAM lParam)
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

FILTER_STATE CDirectShowWnd::GetState (CString * pStateStr)
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
		CString			lPrefix;
		CString			lTitle;
		HRESULT			lResult;
		OAFilterState	lState;
		CString			lStateStr;
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
				LogMessage (pLogLevel, _T("%s%sState %s At [%I64d] to [%I64d] of [%I64d]"), lPrefix, lTitle, lStateStr, lCurrPos/MsPer100Ns, lStopPos/MsPer100Ns, lDuration/MsPer100Ns);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%sState %s"), lPrefix, lTitle, lStateStr);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowWnd::LogStatus (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lPrefix;
		CString	lTitle;

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
					LogMessage (pLogLevel, _T("%s     Visible       [%d] WndVisible [%d]"), lPrefix, lVisible, (IsWindow (m_hWnd) ? ((GetStyle () & WS_VISIBLE) != 0) : -1));
				}
				if	(SUCCEEDED (mVideoWindow->GetWindowPosition (&lPos.x, &lPos.y, &lSize.cx, &lSize.cy)))
				{
					if	(IsWindow (m_hWnd))
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
