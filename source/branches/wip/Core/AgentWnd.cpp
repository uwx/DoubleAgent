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
#include <intrin.h>
#include <shlwapi.h>
#include "EventNotify.h"
#include "AgentWnd.h"
#include "AgentStreamInfo.h"
#include "DirectShowSource.h"
#include "DirectShowRender.h"
#include "DaGlobalConfig.h"
#include "StringArrayEx.h"
#include "MallocPtr.h"
#include "GuidStr.h"
#include "Elapsed.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_FILTER_PREPARE	(GetProfileDebugInt(_T("DebugFilterPrepare"),LogDetails,true)&0xFFFF)
#define	_DEBUG_FILTER_SEGMENTS	(GetProfileDebugInt(_T("DebugFilterSegments"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_IDLE				(GetProfileDebugInt(_T("DebugIdle"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_FILE_NAMES			(GetProfileDebugInt(_T("LogFileNames"),LogDetails,true)&0xFFFF|LogTimeMs)
#define	_LOG_FILE_LOAD			(GetProfileDebugInt(_T("LogFileLoad"),LogVerbose,true)&0xFFFF)
#define	_LOG_ANIMATE_OPS		(GetProfileDebugInt(_T("LogAnimateOps"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_QUEUE_OPS			(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_CYCLES		(GetProfileDebugInt(_T("LogQueueCycles"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
//#define	_TRACE_RESOURCES_EX		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

//#define	_TRACE_BUSY_TIME	60000

#ifndef	_LOG_FILE_LOAD
#define	_LOG_FILE_LOAD	LogVerbose+1
#endif
#ifndef	_LOG_ANIMATE_OPS
#define	_LOG_ANIMATE_OPS	LogDetails
#endif
#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS		LogDetails
#endif
#ifndef	_LOG_QUEUE_CYCLES
//#define	_LOG_QUEUE_CYCLES	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentWnd)

CAgentWnd::CAgentWnd ()
:	mQueueTimer (0),
	mQueueTimeMin (10),
	mQueueTimeMax (200),
	mQueueTimeDefault (100),
	mQueueBusy (0),
	mIdleLevel (-1),
	mIdleTimer (0),
	mIdleStarted (false),
	mEnableSoundFlag (true),
	mVideoRenderType (GUID_NULL)
{
	mStateTraceData.QuadPart = 0;
#ifdef	_DEBUG
	if	(GetProfileDebugInt(_T("DebugDisableSound")) > 0)
	{
		mEnableSoundFlag = false;
	}
#endif
	SetBkColor (GetSysColor (COLOR_WINDOW));
}

CAgentWnd::~CAgentWnd ()
{
	if	(IsWindow ())
	{
		try
		{
			ShowWindow (SW_HIDE);
			Close ();
		}
		catch AnyExceptionSilent
	}

	if	(IsWindow ())
	{
		try
		{
			ShowWindow (SW_HIDE);
			DestroyWindow ();
		}
		catch AnyExceptionSilent
	}
	else
	if	(m_hWnd)
	{
		Detach ();
	}

	SetAgentFile (NULL, this);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::Open (CAgentFile * pAgentFile)
{
	bool			lRet = false;
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile())
	{
		Close ();
	}

	if	(pAgentFile)
	{
		if	(lAgentFile = _AtlModule.FindCachedFile (pAgentFile->GetGuid()))
		{
			SetAgentFile (lAgentFile, this);
		}
		else
		if	(lAgentFile = CAgentFile::CreateInstance())
		{
			SetAgentFile (lAgentFile, NULL);
			_AtlModule.CacheFile (lAgentFile, this);

			lAgentFile->SetDownloadMode (false, false, false);
			if	(SUCCEEDED (lAgentFile->Open (pAgentFile->GetPath())))
			{
				lAgentFile->ReadNames (true);
			}
		}

		if	(lAgentFile = GetAgentFile())
		{
			lRet = CDirectShowWnd::Open (lAgentFile->GetPath());
		}
	}
	if	(!lRet)
	{
		Closed ();
	}
	return lRet;
}

bool CAgentWnd::Open (LPCTSTR pFileName)
{
	bool			lRet = false;
	CAgentFile *	lAgentFile;
	CAtlString		lFileName (pFileName);

	if	(lAgentFile = GetAgentFile())
	{
		Close ();
	}

	lFileName.TrimLeft ();
	lFileName.TrimRight ();
	PathUnquoteSpaces (lFileName.GetBuffer (lFileName.GetLength ()));
	lFileName.ReleaseBuffer ();
	lFileName.TrimLeft ();
	lFileName.TrimRight ();

	if	(!lFileName.IsEmpty ())
	{
		if	(lAgentFile = _AtlModule.FindCachedFile (lFileName))
		{
			SetAgentFile (lAgentFile, this);
		}
		else
		if	(
				(lAgentFile = CAgentFile::CreateInstance())
			&&	(SUCCEEDED (lAgentFile->Open (lFileName, _LOG_FILE_LOAD)))
			)
		{
			SetAgentFile (lAgentFile, NULL);
			_AtlModule.CacheFile (lAgentFile, this);
			lAgentFile->ReadNames (true, _LOG_FILE_LOAD);
		}
		else
		if	(lAgentFile)
		{
			delete lAgentFile;
		}
	}

	if	(lAgentFile = GetAgentFile ())
	{
		lRet = CDirectShowWnd::Open (lFileName);
	}
	if	(!lRet)
	{
		Closed ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentWnd::Opening (LPCTSTR pFileName)
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentWnd::Opening [%p] [%s]"), this, m_hWnd, pFileName);
	}
#endif
#ifdef	_LOG_FILE_NAMES
	if	(LogIsActive (_LOG_FILE_NAMES))
	{
		LogMessage (_LOG_FILE_NAMES, _T("[%p] %s::Opening [%s] File [%p] [%ls]"), m_hWnd, AtlTypeName(this), pFileName, GetAgentFile(), (GetAgentFile() ? (BSTR)GetAgentFile()->GetPath() : (BSTR)NULL));
	}
#endif
	SafeFreeSafePtr (mSourceFilter);
	SafeFreeSafePtr (mRenderFilter);
	CDirectShowWnd::Opening (pFileName);
}

void CAgentWnd::Opened ()
{
#ifdef	_LOG_FILE_NAMES
	if	(LogIsActive (_LOG_FILE_NAMES))
	{
		LogMessage (_LOG_FILE_NAMES, _T("[%p] %s::Opened File [%p] [%ls]"), m_hWnd, AtlTypeName(this), GetAgentFile(), (GetAgentFile() ? (BSTR)GetAgentFile()->GetPath() : (BSTR)NULL));
	}
#endif
//
//	Make an initial empty animation sequence
//
	CAgentStreamInfo *	lStreamInfo;

	if	(
			(IsWindow ())
		&&	(lStreamInfo = GetAgentStreamInfo())
		&&	(SUCCEEDED (lStreamInfo->NewAnimationSequence ()))
		)
	{
		lStreamInfo->ClearSequenceAudio ();
		AnimationSequenceChanged ();
	}
	CDirectShowWnd::Opened ();

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentWnd::Opened [%p]"), this, m_hWnd);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentWnd::Closing ()
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentWnd::Closing [%p]"), this, m_hWnd);
	}
#endif
#ifdef	_LOG_FILE_NAMES
	if	(
			(GetAgentFile ())
		&&	(LogIsActive (_LOG_FILE_NAMES))
		)
	{
		LogMessage (_LOG_FILE_NAMES, _T("[%p] %s::Closing File [%p] [%ls]"), m_hWnd, AtlTypeName(this), GetAgentFile(), (GetAgentFile() ? (BSTR)GetAgentFile()->GetPath() : (BSTR)NULL));
	}
#endif

	CDirectShowWnd::Closing ();
}

void CAgentWnd::Closed ()
{
#ifdef	_LOG_FILE_NAMES
	if	(
			(GetAgentFile ())
		&&	(LogIsActive (_LOG_FILE_NAMES))
		)
	{
		LogMessage (_LOG_FILE_NAMES, _T("[%p] %s::Closed File [%p] [%ls]"), m_hWnd, AtlTypeName(this), GetAgentFile(), (GetAgentFile() ? (BSTR)GetAgentFile()->GetPath() : (BSTR)NULL));
	}
#endif
	if	(mGraphBuilder != NULL)
	{
		IBaseFilterPtr	lSourceFilter;
		IBaseFilterPtr	lRenderFilter;

		try
		{
			if	(lSourceFilter = mSourceFilter)
			{
				mGraphBuilder->RemoveFilter (lSourceFilter);
			}
		}
		catch AnyExceptionSilent

		try
		{
			if	(lRenderFilter = mRenderFilter)
			{
				mGraphBuilder->RemoveFilter (lRenderFilter);
			}
		}
		catch AnyExceptionSilent
	}

	SafeFreeSafePtr (mSourceFilter);
	SafeFreeSafePtr (mRenderFilter);
	SafeFreeSafePtr (mSystemClock);
	SetAgentFile (NULL, this);
	MakeActiveMedia (false);
	CDirectShowWnd::Closed ();

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentWnd::Closed [%p]"), this, m_hWnd);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentWnd::Start (DWORD pWaitForCompletion)
{
	CAgentStreamInfo *	lStreamInfo;

	ClearQueuedActions (-1, AGENTREQERR_INTERRUPTEDUSER, _T("Start"));
	ClearAnimations ();

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(SUCCEEDED (lStreamInfo->NewAnimationSequence ()))
		)
	{
		lStreamInfo->SequenceAll ();
		if	(IsSoundEnabled ())
		{
			lStreamInfo->CueSequenceAudio ();
		}
	}
	AnimationSequenceChanged ();

	return PlayFromTo (0, GetDurationMs(), true, pWaitForCompletion);
}

HRESULT CAgentWnd::Stop (DWORD pWaitForCompletion)
{
	HRESULT	lResult = CDirectShowWnd::Stop (pWaitForCompletion);

	ClearAnimations ();
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentWnd::PrepareGraph (LPCTSTR pFileName)
{
	HRESULT		lResult = E_NOTIMPL;
	CAtlString	lFileName (pFileName);

	try
	{
		tMallocPtr <WCHAR>						lFileName = AtlAllocTaskWideString (pFileName);
		tPtr <CComObject <CDirectShowSource> >	lDirectShowSource;
		tPtr <CComObject <CDirectShowRender> >	lDirectShowRender;
		IFileSourceFilterPtr					lFileSource;
		IBaseFilterPtr							lAgentFilter;
		IBaseFilterPtr							lRenderFilter;
		IPinPtr									lRenderPin;

		SafeFreeSafePtr (mSourceFilter);
		SafeFreeSafePtr (mRenderFilter);
		mVideoRenderType = GUID_NULL;

		if	(SUCCEEDED (lResult = CComObject<CDirectShowSource>::CreateInstance (lDirectShowSource.Free())))
		{
			lAgentFilter = lDirectShowSource->GetControllingUnknown ();

			if	(GetAgentFile())
			{
				lDirectShowSource->SetAgentFile ((ULONG_PTR) GetAgentFile());
				lResult = S_OK;
			}
			else
			{
				lFileSource = lDirectShowSource->GetControllingUnknown ();
				lResult = lFileSource->Load (lFileName, NULL);
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = CComObject<CDirectShowRender>::CreateInstance (lDirectShowRender.Free())))
			)
		{
			lRenderFilter = lDirectShowRender->GetControllingUnknown ();
		}

		if	(SUCCEEDED (lResult))
		{
			if	(mAlphaSmoothing)
			{
				lDirectShowSource->SetBkColor (NULL);
				lDirectShowRender->SetBkColor (NULL);
			}
			else
			{
				lDirectShowSource->SetBkColor (mBkColor);
				lDirectShowRender->SetBkColor (mBkColor);
			}
			lDirectShowRender->SetSmoothing (mAlphaSmoothing);
			lDirectShowRender->SetRenderWnd (m_hWnd);
		}

//
//	By default, DirectShow uses the audio renderer as its reference clock.  Because audio renderers come and go in the filter
//	graph (depending on whether an animation sequence has sound or not), we need to manually set the reference clock so it
//	will stay consistent despite changes to the filter graph.
//
		if	(SUCCEEDED (lResult))
		{
			IMediaFilterPtr		lMediaFilter (mGraphBuilder);
			IReferenceClockPtr	lSystemClock;

			if	(
					(lMediaFilter != NULL)
				&&	(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (CLSID_SystemClock, NULL, CLSCTX_SERVER, __uuidof (IReferenceClock), (void **) &lSystemClock))))
				&&	(SUCCEEDED (LogVfwErr (LogNormal, lMediaFilter->SetSyncSource (lSystemClock))))
				)
			{
#ifdef	_DEBUG_NOT
				IReferenceClockTimerControlPtr	lClockControl (lSystemClock);
				REFERENCE_TIME					lTimerResolution;

				if	(
						(lClockControl != NULL)
					&&	(SUCCEEDED (lClockControl->GetDefaultTimerResolution (&lTimerResolution)))
					)
				{
					LogMessage (LogNormal, _T("Timer resolution [%f (%I64d)]"), RefTimeSec(lTimerResolution), lTimerResolution);
				}
#endif
				mSystemClock = lSystemClock;
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGraphBuilder->AddFilter (lAgentFilter, lFileName))))
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGraphBuilder->AddFilter (lRenderFilter, L"Animation Render"))))
			)
		{
			lResult = ConnectFilters (mGraphBuilder, lRenderFilter, lAgentFilter, true, NULL, NULL, &lRenderPin);

			if	(FAILED (lResult))
			{
				lResult = ConnectFilters (mGraphBuilder, lRenderFilter, lAgentFilter, false, NULL, NULL, &lRenderPin);
			}

			if	(FAILED (lResult))
			{
				IBaseFilterPtr	lColorFilter;

				if	(
						(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_Colour, NULL, CLSCTX_SERVER, __uuidof (IBaseFilter), (void **) &lColorFilter))))
					&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mGraphBuilder->AddFilter (lColorFilter, L"Color Space Converter"))))
					)
				{
					lResult = ConnectFilters (mGraphBuilder, lColorFilter, lAgentFilter, true);
					if	(SUCCEEDED (lResult))
					{
						lResult = ConnectFilters (mGraphBuilder, lRenderFilter, lColorFilter, true, NULL, NULL, &lRenderPin);
					}
				}
			}
		}

#ifdef	_DEBUG_FILTER_PREPARE
		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (GetRenderType (lRenderPin, mVideoRenderType)))
			)
		{
			LogMessage (_DEBUG_FILTER_PREPARE, _T("[%p] VideoRenderType [%s]"), this, CGuidStr::GuidName (mVideoRenderType));
		}
#endif

		if	(SUCCEEDED (lResult))
		{
			ULONG_PTR	lAgentFile = NULL;
			ULONG_PTR	lAgentStreamInfo = NULL;

			if	(SUCCEEDED (lResult = lDirectShowSource->GetAgentFile (&lAgentFile)))
			{
				SetAgentFile ((CAgentFile *)lAgentFile, this);
			}
			if	(SUCCEEDED (lResult = lDirectShowSource->GetAgentStreamInfo (&lAgentStreamInfo)))
			{
				SetAgentStreamInfo ((CAgentStreamInfo *)lAgentStreamInfo);
			}
			mBasicVideo = lRenderFilter; // Overrides QueryInterface on the filter graph - see CDirectShowWnd::GraphPrepared
		}

		mSourceFilter = lDirectShowSource.Detach ();
		mRenderFilter = lDirectShowRender.Detach ();

		SafeFreeSafePtr (lRenderFilter);
		SafeFreeSafePtr (lAgentFilter);
		SafeFreeSafePtr (lFileSource);

#ifdef	_DEBUG_FILTER_PREPARE
		if	(SUCCEEDED (lResult))
		{
			LogFilters (_DEBUG_FILTER_PREPARE, true, _T("PrepareGraph"));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

COLORREF CAgentWnd::GetEraseColor ()
{
	if	(mBkColor)
	{
		return (*mBkColor);
	}
	else
	{
		return CDirectShowWnd::GetEraseColor ();
	}
}

bool CAgentWnd::PaintWindow (HDC pDC)
{
	if	(mRenderFilter)
	{
		return (mRenderFilter->DrawSampleImage (pDC) == S_OK);
	}
	else
	{
		return CDirectShowWnd::PaintWindow (pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentWnd::NextReqID () const
{
	CEventNotify *	lNotify;

	return (lNotify = mNotify (0)) ? lNotify->NextReqID() : -1;
}

/////////////////////////////////////////////////////////////////////////////

SAFEARRAY * CAgentWnd::GetStateNames ()
{
	tSafeArrayPtr	lRet;

	if	(GetAgentFile())
	{
		const CAtlStringArray &	lStateNames = GetFileStates().mNames;
		long					lNdx;

		if	(
				(lStateNames.GetCount() > 0)
			&&	(lRet = SafeArrayCreateVector (VT_BSTR, 0, (ULONG)lStateNames.GetCount()))
			)
		{
			for	(lNdx = 0; lNdx < (long)lStateNames.GetCount(); lNdx++)
			{
				SafeArrayPutElement (lRet, &lNdx, lStateNames [lNdx].AllocSysString());
			}
		}
	}
	return lRet.Detach();
}

SAFEARRAY * CAgentWnd::GetGestureNames ()
{
	tSafeArrayPtr	lRet;

	if	(GetAgentFile())
	{
		const CAtlStringArray &	lGestureNames = GetFileGestures().mNames;
		long					lNdx;

		if	(
				(lGestureNames.GetCount() > 0)
			&&	(lRet = SafeArrayCreateVector (VT_BSTR, 0, (ULONG)lGestureNames.GetCount()))
			)
		{
			for	(lNdx = 0; lNdx < (long)lGestureNames.GetCount(); lNdx++)
			{
				SafeArrayPutElement (lRet, &lNdx, lGestureNames [lNdx].AllocSysString());
			}
		}
	}
	return lRet.Detach();
}

SAFEARRAY * CAgentWnd::GetAnimationNames ()
{
	tSafeArrayPtr	lRet;

	if	(GetAgentFile())
	{
		const CAgentFileGestures &	lGestures = GetFileGestures();
		long						lNdx;

		if	(
				(lGestures.mAnimations.GetCount() > 0)
			&&	(lRet = SafeArrayCreateVector (VT_BSTR, 0, (ULONG)lGestures.mAnimations.GetCount()))
			)
		{
			for	(lNdx = 0; lNdx < (long)lGestures.mAnimations.GetCount(); lNdx++)
			{
				SafeArrayPutElement (lRet, &lNdx, SysAllocString (lGestures.mAnimations [lNdx]->mName));
			}
		}
	}
	return lRet.Detach();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::IsSoundEnabled (bool pIgnoreGlobalConfig) const
{
	if	(
			(mEnableSoundFlag)
		&&	(
				(pIgnoreGlobalConfig)
			||	(CDaSettingsConfig().LoadConfig().mEffectsEnabled)
			)
		)
	{
		return true;
	}
	return false;
}

bool CAgentWnd::EnableSound (bool pEnable)
{
	if	(mEnableSoundFlag != pEnable)
	{
		mEnableSoundFlag = pEnable;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::SetBkColor (COLORREF pBkColor)
{
	mBkColor = new COLORREF;
	(*mBkColor) = pBkColor;

	if	(IsWindow ())
	{
		if	(mSourceFilter)
		{
			if	(mAlphaSmoothing)
			{
				mSourceFilter->SetBkColor (NULL);
			}
			else
			{
				mSourceFilter->SetBkColor (mBkColor);
			}
		}
		if	(mRenderFilter)
		{
			if	(mAlphaSmoothing)
			{
				mRenderFilter->SetBkColor (NULL);
			}
			else
			{
				mRenderFilter->SetBkColor (mBkColor);
			}
		}
		return true;
	}
	return false;
}

bool CAgentWnd::GetBkColor (COLORREF & pBkColor)
{
	if	(mBkColor)
	{
		pBkColor = (*mBkColor);
		return true;
	}
	return false;
}

bool CAgentWnd::ResetBkColor ()
{
	COLORREF	lDefaultBkColor = GetSysColor (COLOR_WINDOW);

	mBkColor = NULL;

	if	(IsWindow ())
	{
		if	(mSourceFilter)
		{
			if	(mAlphaSmoothing)
			{
				mSourceFilter->SetBkColor (NULL);
			}
			else
			{
				mSourceFilter->SetBkColor (&lDefaultBkColor);
			}
		}
		if	(mRenderFilter)
		{
			if	(mAlphaSmoothing)
			{
				mRenderFilter->SetBkColor (NULL);
			}
			else
			{
				mRenderFilter->SetBkColor (&lDefaultBkColor);
			}
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::ShowState (LPCTSTR pStateName, bool pStopIdle, bool pClearQueue)
{
	bool		lRet = false;
	CAtlString	lStateName (pStateName);

#ifdef	_LOG_ANIMATE_OPS
	if	(LogIsActive (_LOG_ANIMATE_OPS))
	{
		LogMessage (_LOG_ANIMATE_OPS, _T("ShowState [%s] StopIdle [%u] ClearQueue [%u]"), pStateName, pStopIdle, pClearQueue);
	}
#endif
	lStateName.TrimLeft ();
	lStateName.TrimRight ();

	if	(pStopIdle)
	{
		StopIdle (_T("ShowState"));
	}
	if	(pClearQueue)
	{
		Stop ();
#ifdef	_LOG_ANIMATE_OPS
		if	(LogIsActive (_LOG_ANIMATE_OPS))
		{
			LogMessage (_LOG_ANIMATE_OPS, _T("Stop at [%d] of [%d %d] for State [%s]"), GetCurrPosMs(), GetStopPosMs(), GetDurationMs(), pStateName);
		}
#endif
		ClearQueuedActions (-1, AGENTREQERR_INTERRUPTEDUSER, _T("ShowState"));
	}

	if	(ShowStateGestures (0, lStateName, false))
	{
		lRet = true;
	}

	if	(lRet)
	{
		ActivateQueue (true);
	}
#ifdef	_LOG_ANIMATE_OPS
	if	(
			(!lRet)
		&&	(LogIsActive (_LOG_ANIMATE_OPS))
		)
	{
		LogMessage (_LOG_ANIMATE_OPS, _T("ShowState [%s] failed"), lStateName);
	}
#endif
	return lRet;
}

bool CAgentWnd::ShowStateGestures (long pCharID, LPCTSTR pStateName, bool pQueuedState)
{
	bool	lRet = false;

	if	(GetAgentFile())
	{
		CAtlString				lStateName (pStateName);
		const CAtlStringArray *	lGestures = GetFileStates()(lStateName);
		CAtlString				lGestureName;
		INT_PTR					lNdx;

		if	(
				(lGestures)
			&&	(lGestures->GetCount() > 0)
			)
		{
#ifdef	_LOG_ANIMATE_OPS
			if	(LogIsActive (_LOG_ANIMATE_OPS))
			{
				LogMessage (_LOG_ANIMATE_OPS, _T("ShowState [%s] Gestures [%s]"), lStateName, JoinStringArray (*lGestures, _T(" ")));
			}
#endif
			if	(pQueuedState)
			{
				for	(lNdx = 0; lNdx < (INT_PTR)lGestures->GetCount(); lNdx++)
				{
					if	(QueueGesture (pCharID, lGestures->GetAt (lNdx), lStateName))
					{
						lRet = true;
					}
				}
			}
			else
			{
				for	(lNdx = 1; lNdx < (INT_PTR)lGestures->GetCount(); lNdx++)
				{
					QueueGesture (pCharID, lGestures->GetAt (lNdx), lStateName);
				}
				if	(ShowGesture (lGestures->GetAt (0), lStateName, false, false))
				{
					lRet = true;
				}
				else
				{
					ClearQueuedGestures (pCharID, AGENTREQERR_INTERRUPTEDUSER);
				}
			}
		}
	}
	return lRet;
}

bool CAgentWnd::IsShowingState (LPCTSTR pStateName)
{
	bool				lRet = false;
	CAgentStreamInfo *	lStreamInfo = NULL;
	tBstrPtr			lAnimationSource;

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(SUCCEEDED (lStreamInfo->GetAnimationSource (lAnimationSource.Free())))
		&&	(CAtlString ((BSTR)lAnimationSource).CompareNoCase (pStateName) == 0)
		&&	(!IsAnimationComplete ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::ShowGesture (LPCTSTR pGestureName, LPCTSTR pForState, bool pStopIdle, bool pClearQueue)
{
	bool				lRet = false;
	CAgentFile *		lAgentFile;
	CAgentStreamInfo *	lStreamInfo = NULL;
	CAtlString			lGestureName (pGestureName);
	long				lAnimationNdx = -1;
	long				lPrevAnimationNdx = -1;
	long				lPrevFrameNdx = -1;
	long				lAnimationDuration = -1;
	bool				lReturnContinues = false;
	bool				lReturnBranches = false;
	bool				lReturnExits = false;
	long				lSilentFrameCount = 0;

//
//	Show with blank name and not ClearQueue plays the exit animation of the previous animation
//
#ifdef	_LOG_ANIMATE_OPS
	if	(LogIsActive (_LOG_ANIMATE_OPS))
	{
		LogMessage (_LOG_ANIMATE_OPS, _T("ShowGesture [%s] StopIdle [%u] ClearQueue [%u]"), pGestureName, pStopIdle, pClearQueue);
	}
#endif
	lGestureName.TrimLeft ();
	lGestureName.TrimRight ();

	if	(pStopIdle)
	{
		StopIdle (_T("ShowGesture"));
	}
	if	(pClearQueue)
	{
		Stop ();
#ifdef	_LOG_ANIMATE_OPS
		if	(LogIsActive (_LOG_ANIMATE_OPS))
		{
			LogMessage (_LOG_ANIMATE_OPS, _T("Stop at [%d] of [%d %d] for Gesture [%s]"), GetCurrPosMs(), GetStopPosMs(), GetDurationMs(), pGestureName);
		}
#endif
		ClearQueuedActions (-1, AGENTREQERR_INTERRUPTEDUSER, _T("ShowGesture"));
	}

	if	(
			(lAgentFile = GetAgentFile())
		&&	(GetFileGestures().mAnimations.GetCount() > 0)
		&&	(lStreamInfo = GetAgentStreamInfo())
		)
	{
		long	lPosition = GetCurrPosMs ();
		bool	lWasPlaying = AnimationSequenceChanging (true);

		if	(
				(pClearQueue)
			||	(lWasPlaying)
			)
		{
			long	lDuration = 0;

			if	(
					(SUCCEEDED (lStreamInfo->GetAnimationIndex (&lPrevAnimationNdx)))
				&&	(lPrevAnimationNdx >= 0)
				&&	(SUCCEEDED (lStreamInfo->GetSequenceDuration (&lDuration)))
				&&	(lPosition < lDuration)
				)
			{
				lStreamInfo->CalcSequenceAnimationFrameNdx (&lPrevAnimationNdx, &lPrevFrameNdx, lPosition, true);
				lReturnExits = true;
//
//	Interrupted - play exit sequence
//
#ifdef	_LOG_ANIMATE_OPS
				if	(LogIsActive (_LOG_ANIMATE_OPS))
				{
					LogMessage (_LOG_ANIMATE_OPS, _T("  Exit [%s] at [%d] from [%d %d]"), GetFileGestures().mAnimations[lPrevAnimationNdx]->mName, lPosition, lPrevAnimationNdx, lPrevFrameNdx);
				}
#endif
			}
		}

		if	(!lReturnExits)
		{
			const CAgentFileAnimation *	lPrevAnimation;
			long						lPosition = 0;

			if	(
					(SUCCEEDED (lStreamInfo->GetAnimationIndex (&lPrevAnimationNdx)))
				&&	(lPrevAnimationNdx >= 0)
				&&	(lPrevAnimation = lAgentFile->GetAnimation (lPrevAnimationNdx))
				&&	(lPrevAnimation->mReturnType == 1)
				)
			{
				if	(lWasPlaying)
				{
					lPosition = GetCurrPosMs();
				}
				lStreamInfo->TruncateAnimationLoop (lPosition);
				lStreamInfo->CalcSequenceAnimationFrameNdx (&lPrevAnimationNdx, &lPrevFrameNdx, LONG_MAX, true);
				lReturnContinues = true;
//
//	Play previous animation's return (exit branching)
//
#ifdef	_LOG_ANIMATE_OPS
				if	(LogIsActive (_LOG_ANIMATE_OPS))
				{
					LogMessage (_LOG_ANIMATE_OPS, _T("  Continue [%s] at [%d] from [%d %d]"), GetFileGestures().mAnimations[lPrevAnimationNdx]->mName, lPosition, lPrevAnimationNdx, lPrevFrameNdx);
				}
#endif
			}
			else
			if	(
					(SUCCEEDED (lStreamInfo->GetAnimationIndex (&lPrevAnimationNdx)))
				&&	(lPrevAnimationNdx >= 0)
				&&	(lPrevAnimation = lAgentFile->GetAnimation (lPrevAnimationNdx))
				&&	(lPrevAnimation->mReturnType == 0)
				&&	((lPrevAnimationNdx = (long)lAgentFile->FindGesture (lPrevAnimation->mReturnName)) >= 0)
				)
			{
				lReturnContinues = true;
				lReturnBranches = true;
//
//	Play previous animation's return (return animation)
//
#ifdef	_LOG_ANIMATE_OPS
				if	(LogIsActive (_LOG_ANIMATE_OPS))
				{
					LogMessage (_LOG_ANIMATE_OPS, _T("  Return [%s] as [%s]"), GetFileGestures().mAnimations[lPrevAnimationNdx]->mName, lPrevAnimation->mReturnName);
				}
#endif
			}
			else
			{
				lPrevAnimationNdx = -1;
			}
		}

		lStreamInfo->ClearAnimationSequences();

		if	(SUCCEEDED (lStreamInfo->NewAnimationSequence ()))
		{
			if	(lPrevAnimationNdx >= 0)
			{
				if	(lReturnExits)
				{
					lStreamInfo->SequenceAnimationExit (lPrevAnimationNdx, lPrevFrameNdx);
					lStreamInfo->GetSequenceFrameCount (&lSilentFrameCount);
				}
				else
				if	(lReturnBranches)
				{
					lStreamInfo->SequenceAnimation (lPrevAnimationNdx);
				}
				else
				{
					lStreamInfo->SequenceAnimationExit (lPrevAnimationNdx, lPrevFrameNdx);
				}
#ifdef	_LOG_ANIMATE_OPS_NOT
				if	(LogIsActive (_LOG_ANIMATE_OPS))
				{
					lStreamInfo->LogAnimationSequence (_LOG_ANIMATE_OPS, _T("  Sequenced [%s] from [%d %d]"), GetFileGestures()[lPrevAnimationNdx]->mName, lPrevAnimationNdx, lPrevFrameNdx);
				}
#endif
			}

			if	(
					(
						(lGestureName.IsEmpty ())
					&&	(SUCCEEDED (lStreamInfo->GetSequenceDuration (&lAnimationDuration)))
					&&	(lAnimationDuration > 0)
					&&	(SUCCEEDED (lStreamInfo->SetAnimationIndex (-1)))
					)
				||	(
						((lAnimationNdx = (long)lAgentFile->FindAnimation (lGestureName)) >= 0)
					&&	(SUCCEEDED (lStreamInfo->SequenceAnimation (lAnimationNdx)))
					&&	(SUCCEEDED (lStreamInfo->GetSequenceDuration (&lAnimationDuration)))
					&&	(lAnimationDuration > 0)
					&&	(SUCCEEDED (lStreamInfo->SetAnimationIndex (lAnimationNdx)))
					)
				)
			{
				if	(pForState)
				{
					lStreamInfo->SetAnimationSource (_bstr_t(pForState));
				}
				if	(IsSoundEnabled ())
				{
					lStreamInfo->CueSequenceAudio (lSilentFrameCount);
				}
				else
				{
					lStreamInfo->ClearSequenceAudio ();
				}
				AnimationSequenceChanged ();
				lStreamInfo->GetSequenceDuration (&lAnimationDuration);

#ifdef	_LOG_ANIMATE_OPS
				if	(LogIsActive (_LOG_ANIMATE_OPS))
				{
					long lFrameCount = 0;
					lStreamInfo->GetSequenceFrameCount (&lFrameCount);
					//lStreamInfo->LogAnimationSequence (_LOG_ANIMATE_OPS, _T("ShowGesture [%s] Duration [%d] Continuing [%u] Exiting [%u] Frames [%d] SilentFrames [%d]"), lGestureName, lAnimationDuration, lReturnContinues, lReturnExits, lFrameCount, lSilentFrameCount);
					LogMessage (_LOG_ANIMATE_OPS, _T("ShowGesture [%s] Duration [%d] Continuing [%u] Exiting [%u] Frames [%d] SilentFrames [%d]"), lGestureName, lAnimationDuration, lReturnContinues, lReturnExits, lFrameCount, lSilentFrameCount);
				}
#endif
#ifdef	_DEBUG
				if	(GetDurationMs () != lAnimationDuration)
				{
					LogMessage (LogDebug, _T("!!! Duration [%d] [%d]"), lAnimationDuration, GetDurationMs());
				}
#endif
				if	(SUCCEEDED (PlayFromTo (0, lAnimationDuration, true)))
				{
					lRet = true;
				}
			}
		}
	}

	if	(lRet)
	{
		ActivateQueue (true);
	}
#ifdef	_LOG_ANIMATE_OPS
	if	(
			(!lRet)
		&&	(LogIsActive (_LOG_ANIMATE_OPS))
		)
	{
		LogMessage (_LOG_ANIMATE_OPS, _T("ShowGesture [%s] failed [%d] [%d] [%p]"), lGestureName, lAnimationNdx, lAnimationDuration, lStreamInfo);
	}
#endif
	return lRet;
}

bool CAgentWnd::IsShowingGesture (LPCTSTR pGestureName, LPCTSTR pForState)
{
	bool				lRet = false;
	CAgentStreamInfo *	lStreamInfo = NULL;
	tBstrPtr			lAnimationName;
	tBstrPtr			lAnimationSource;

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(SUCCEEDED (lStreamInfo->GetAnimationName (lAnimationName.Free())))
		&&	(
				(pGestureName)
			?	(CAtlString ((BSTR)lAnimationName).CompareNoCase (pGestureName) == 0)
			:	(!CAtlString ((BSTR)lAnimationName).IsEmpty ())
			)
		&&	(
				(!pForState)
			||	(
					(SUCCEEDED (lStreamInfo->GetAnimationSource (lAnimationSource.Free())))
				&&	(CAtlString ((BSTR)lAnimationSource).CompareNoCase (pForState) == 0)
				)
			)
		&&	(!IsAnimationComplete ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::ShowAnimation (LPCTSTR pAnimationName, bool pStopIdle, bool pClearQueue)
{
	bool				lRet = false;
	CAgentFile *		lAgentFile;
	CAgentStreamInfo *	lStreamInfo = NULL;
	CAtlString			lAnimationName (pAnimationName);
	long				lAnimationNdx = -1;
	long				lAnimationDuration;

#ifdef	_LOG_ANIMATE_OPS
	if	(LogIsActive (_LOG_ANIMATE_OPS))
	{
		LogMessage (_LOG_ANIMATE_OPS, _T("ShowAnimation [%s] StopIdle [%u] ClearQueue [%u]"), pAnimationName, pStopIdle, pClearQueue);
	}
#endif
	lAnimationName.TrimLeft ();
	lAnimationName.TrimRight ();

	if	(pStopIdle)
	{
		StopIdle (_T("ShowAnimation"));
	}
	if	(pClearQueue)
	{
		Stop ();
#ifdef	_LOG_ANIMATE_OPS
		if	(LogIsActive (_LOG_ANIMATE_OPS))
		{
			LogMessage (_LOG_ANIMATE_OPS, _T("Stop at [%d] of [%d %d] for Animation [%s]"), GetCurrPosMs(), GetStopPosMs(), GetDurationMs(), pAnimationName);
		}
#endif
		ClearQueuedActions (-1, AGENTREQERR_INTERRUPTEDUSER, _T("ShowAnimation"));
	}

	if	(
			(lAgentFile = GetAgentFile())
		&&	((lAnimationNdx = (long)lAgentFile->FindAnimation (lAnimationName)) >= 0)
		&&	(lStreamInfo = GetAgentStreamInfo())
		)
	{
		AnimationSequenceChanging (true);
		lStreamInfo->ClearAnimationSequences();

		if	(SUCCEEDED (lStreamInfo->NewAnimationSequence ()))
		{
			if	(
					(SUCCEEDED (lStreamInfo->SequenceAnimation (lAnimationNdx)))
				&&	(SUCCEEDED (lStreamInfo->GetSequenceDuration (&lAnimationDuration)))
				&&	(lAnimationDuration > 0)
				&&	(SUCCEEDED (lStreamInfo->SetAnimationIndex (lAnimationNdx)))
				)
			{
//
//	This function includes the animation exit sequence
//
				lStreamInfo->SequenceAnimationExit (lAnimationNdx, -1);
				if	(IsSoundEnabled ())
				{
					lStreamInfo->CueSequenceAudio ();
				}
				else
				{
					lStreamInfo->ClearSequenceAudio ();
				}
				AnimationSequenceChanged ();
				lStreamInfo->GetSequenceDuration (&lAnimationDuration);
#ifdef	_LOG_ANIMATE_OPS
				if	(LogIsActive (_LOG_ANIMATE_OPS))
				{
					LogMessage (_LOG_ANIMATE_OPS, _T("ShowAnimation [%s] Duration [%d]"), lAnimationName, lAnimationDuration);
				}
#endif
#ifdef	_DEBUG
				if	(GetDurationMs () != lAnimationDuration)
				{
					LogMessage (LogDebug, _T("!!! Duration [%d] [%d]"), lAnimationDuration, GetDurationMs());
				}
#endif
				if	(SUCCEEDED (PlayFromTo (0, lAnimationDuration, true)))
				{
					lRet = true;
				}
			}
			else
			{
				lStreamInfo->EndAnimationSequence ();
				AnimationSequenceChanged ();
			}
		}
	}

	if	(lRet)
	{
		ActivateQueue (true);
	}
#ifdef	_LOG_ANIMATE_OPS
	if	(
			(!lRet)
		&&	(LogIsActive (_LOG_ANIMATE_OPS))
		)
	{
		LogMessage (_LOG_ANIMATE_OPS, _T("ShowAnimation [%s] failed [%d] [%p]"), lAnimationName, lAnimationNdx, lStreamInfo);
	}
#endif
	return lRet;
}

bool CAgentWnd::IsShowingAnimation (LPCTSTR pAnimationName)
{
	bool				lRet = false;
	CAgentStreamInfo *	lStreamInfo = NULL;
	tBstrPtr			lAnimationName;

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(SUCCEEDED (lStreamInfo->GetAnimationName (lAnimationName.Free())))
		&&	(CAtlString ((BSTR)lAnimationName).CompareNoCase (pAnimationName) == 0)
		&&	(!IsAnimationComplete ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::IsAnimationComplete (bool pPauseAtEndOfStream)
{
	bool	lRet = false;

#ifdef	_TRACE_BUSY_TIME
	if	(mStateTraceData.HighPart <= 0)
	{
		mStateTraceData.LowPart = GetTickCount();
	}
#endif

	if	(IsStopped ())
	{
		lRet = true;
	}
	else
	if	(IsEndOfStream ())
	{
		if	(pPauseAtEndOfStream)
		{
#ifdef	_DEBUG_FILTER_SEGMENTS
			LogMessage (_DEBUG_FILTER_SEGMENTS, _T("[%p] [%s] Pause for EndOfStream"), this, AtlTypeName(this));
#endif
			Pause ();
		}
		lRet = true;
	}
	else
	if	(IsPaused ())
	{
		Resume ();
	}

#ifdef	_TRACE_BUSY_TIME
	if	(lRet)
	{
		mStateTraceData.HighPart = 0;
	}
	else
	if	(
			(++mStateTraceData.HighPart >= 100)
		&&	(ElapsedTicks (mStateTraceData.LowPart) >= _TRACE_BUSY_TIME)
		)
	{
		if	(LogIsActive ())
		{
			LogState (LogIfActive|LogHighVolume, _T("BusyTime [%u] Cycles [%d] Queue [%u] Timer [%u]"), ElapsedTicks (mStateTraceData.LowPart), mStateTraceData.HighPart, mQueue.GetSize(), IsQueueActive());
		}
		mStateTraceData.HighPart = 0;
	}
#endif
	return lRet;
}

bool CAgentWnd::AnimationSequenceChanging (bool pStopNow, bool pStopAtEndOfStream)
{
	bool	lRet = false;

	if	(!IsStopped ())
	{
		if	(
				(!IsPaused ())
			&&	(!IsEndOfStream ())
			)
		{
			lRet = true;
		}

		if	(
				(pStopNow)
			||	(
					(pStopAtEndOfStream)
				&&	(IsEndOfStream ())
				)
			)
		{
#ifdef	_DEBUG_FILTER_SEGMENTS
			LogMessage (_DEBUG_FILTER_SEGMENTS, _T("[%p] [%s] Stop for AnimationSequenceChanging"), this, AtlTypeName(this));
#endif
			CDirectShowWnd::Stop ();
		}
	}
	return lRet; // true if was running
}

void CAgentWnd::AnimationSequenceChanged ()
{
	if	(mSourceFilter)
	{
		mSourceFilter->SegmentDurationChanged ();
	}
#ifdef	_DEBUG_FILTER_SEGMENTS
	LogMediaSeeking (_DEBUG_FILTER_SEGMENTS, mMediaSeeking, _T("[%p] [%s] AnimationSequenceChanged"), this, AtlTypeName(this));
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::ClearAnimations ()
{
	CAgentStreamInfo *	lStreamInfo;

	if	(lStreamInfo = GetAgentStreamInfo())
	{
		lStreamInfo->ClearAnimationSequences();
		lStreamInfo->SetAnimationIndex (-1);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentWnd::GetCurrPosMs ()
{
	long		lRet = -1;
	LONGLONG	lPosition = -1;

	if	(
			(mMediaSeeking != NULL)
		&&	(SUCCEEDED (mMediaSeeking->GetCurrentPosition (&lPosition)))
		)
	{
		lRet = (long)(lPosition / MsPer100Ns);
	}
	return lRet;
}

long CAgentWnd::GetStopPosMs ()
{
	long		lRet = -1;
	LONGLONG	lPosition = -1;

	if	(
			(mMediaSeeking != NULL)
		&&	(SUCCEEDED (mMediaSeeking->GetStopPosition (&lPosition)))
		)
	{
		lRet = (long)(lPosition / MsPer100Ns);
	}
	return lRet;
}

long CAgentWnd::GetDurationMs ()
{
	long		lRet = 0;
	LONGLONG	lDuration = -1;

	if	(
			(mMediaSeeking != NULL)
		&&	(SUCCEEDED (mMediaSeeking->GetDuration (&lDuration)))
		)
	{
		lRet = (long)(lDuration / MsPer100Ns);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentWnd::PlayFromTo (long pStartPosMs, long pStopPosMs, bool pRestart, DWORD pWaitForCompletion)
{
	HRESULT		lResult = E_UNEXPECTED;

	if	(mMediaSeeking != NULL)
	{
		LONGLONG	lCurrPosition = (LONGLONG)pStartPosMs * MsPer100Ns;
		LONGLONG	lStopPosition = (LONGLONG)pStopPosMs * MsPer100Ns;

		if	(pRestart)
		{
			lResult = CDirectShowWnd::Stop (pWaitForCompletion);
		}

		if	(
				(pRestart)
			?	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mMediaSeeking->SetPositions (&lCurrPosition, AM_SEEKING_AbsolutePositioning, &lStopPosition, AM_SEEKING_AbsolutePositioning))))
				)
			:	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mMediaSeeking->SetPositions (&lCurrPosition, AM_SEEKING_AbsolutePositioning|AM_SEEKING_Segment|AM_SEEKING_NoFlush, &lStopPosition, AM_SEEKING_AbsolutePositioning|AM_SEEKING_Segment|AM_SEEKING_NoFlush))))
			)
		{
#ifdef	_DEBUG_FILTER_SEGMENTS
			LogMediaSeeking (_DEBUG_FILTER_SEGMENTS, mMediaSeeking, _T("[%p] [%s] PlayFromTo [%d - %d] Restart [%u] Wait [%u]"), this, AtlTypeName(this), pStartPosMs, pStopPosMs, pRestart, pWaitForCompletion);
#endif
			if	(
					(!pRestart)
				&&	(IsPlaying (false))
				)
			{
				lResult = S_OK;
			}
			else
			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, Resume (pWaitForCompletion), _T("PlayFromTo [%d - %d] Restart [%u] Wait [%u]"), pStartPosMs, pStopPosMs, pRestart, pWaitForCompletion)))
			{
#ifdef	_DEBUG_NOT
//TODO - use preroll for complex animations (excess sound preparation time)???
				if	(lResult != S_OK)
				{
					LogVfwErrAnon (LogAlways, lResult, _T("PlayFromTo [%d - %d] Restart [%u] Wait [%u]"), pStartPosMs, pStopPosMs, pRestart, pWaitForCompletion);
					LogState (LogDebugFast);
					//LogFilterStates (LogDebugFast);
					MessageBeep (MB_ICONINFORMATION);
				}
#endif
				lResult = S_OK;
			}
		}
		if	(FAILED (lResult))
		{
#ifdef	_DEBUG
			LogMessage (LogNormal, _T("[%p] [%s] Resume failed"), this, AtlTypeName(this));
#endif
			CDirectShowWnd::Stop (0);
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::UpdateActiveMedia ()
{
	bool	lRet = false;
	HWND	lForegroundWindow;
	DWORD	lProcessId = 0;
	DWORD	lForegroundProcessId = 0;
	HWND	lActiveWindow;

	if	(
			(IsWindow ())
		&&	(lForegroundWindow = ::GetForegroundWindow ())
		&&	(
				(lForegroundWindow == m_hWnd)
			||	(::IsChild (lForegroundWindow, m_hWnd))
			||	(
					(GetWindowThreadProcessId (m_hWnd, &lProcessId))
				&&	(GetWindowThreadProcessId (lForegroundWindow, &lForegroundProcessId))
				&&	(lForegroundProcessId == lProcessId)
				&&	(lActiveWindow = ::GetActiveWindow ())
				&&	(
						(lActiveWindow == m_hWnd)
					||	(::IsChild (lActiveWindow, m_hWnd))
					)
				)
			)
		)
	{
		lRet = MakeActiveMedia (true);
	}
	return lRet;
}

bool CAgentWnd::MakeActiveMedia (bool pActive)
{
	bool				lRet = false;
	IMediaEventSinkPtr	lEventSink (mGraphBuilder);
	IBaseFilterPtr		lRenderFilter;

	if	(
			(lEventSink != NULL)
		&&	(lRenderFilter = mRenderFilter)
		)
	{
		try
		{
			if	(pActive)
			{
				if	(
						(IsWindow ())
					&&	(SUCCEEDED (lEventSink->Notify (EC_ACTIVATE, TRUE, (LONG_PTR)lRenderFilter.GetInterfacePtr())))
					)
				{
					lRet = true;
				}
			}
			else
			{
				if	(SUCCEEDED (lEventSink->Notify (EC_ACTIVATE, FALSE, (LONG_PTR)lRenderFilter.GetInterfacePtr())))
				{
					lRet = true;
				}
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentWnd::QueueState (long pCharID, LPCTSTR pStateName)
{
	long					lReqID = 0;
	CQueuedState *			lQueuedState = NULL;
	CAtlString				lStateName (pStateName);
	const CAtlStringArray *	lGestures;

	lStateName.TrimLeft ();
	lStateName.TrimRight ();
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueState [%d] [%s]"), this, m_dwRef, pCharID, lStateName);
	}
#endif

	if	(
			(GetAgentFile ())
		&&	(lGestures = GetFileStates()(lStateName))
		&&	(lGestures->GetCount() > 0)
		&&	(lQueuedState = new CQueuedState (lStateName, pCharID, lReqID=(pCharID?NextReqID():-1)))
		)
	{
		lQueuedState->mGestures.Copy (*lGestures);
		mQueue.AddTail (lQueuedState);
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueState [%d] [%s] as [%p] [%d]"), this, m_dwRef, pCharID, lStateName, lQueuedState, lReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueState [%d] [%s] failed"), this, m_dwRef, pCharID, lStateName);
		}
#endif
	}
	return lReqID;
}

bool CAgentWnd::DoQueuedState ()
{
	tPtr <CQueuedState>	lQueuedState;

	if	(lQueuedState = (CQueuedState *) mQueue.GetNextAction (QueueActionState))
	{
		if	(lQueuedState->mStarted)
		{
			mQueue.RemoveHead ();
			lQueuedState->NotifyComplete (mNotify);
		}
		else
		{
			lQueuedState->NotifyStarted (mNotify);

			if	(mQueue.GetNextAction (QueueActionState) == lQueuedState)
			{
				mQueue.RemoveHead ();

				if	((GetStyle () & WS_VISIBLE) == 0)
				{
					lQueuedState->NotifyComplete (mNotify, AGENTERR_CHARACTERNOTVISIBLE);
				}
				else
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					mQueue.PushQueue (lQueue);

					if	(ShowStateGestures (lQueuedState->mCharID, lQueuedState->mStateName, true))
					{
						mQueue.AddTail (lQueuedState.Detach());
					}
					mQueue.PopQueue (lQueue);
				}
			}
			else
			{
				lQueuedState.Detach (); // Was deleted during NotifyStarted
			}
		}
		return true;
	}
	return false;
}

void CAgentWnd::AbortQueuedState (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedState *		lQueuedState;
	CQueuedGesture *	lQueuedGesture;

	if	(
			(lQueuedState = (CQueuedState *) pQueuedAction)
		&&	(lQueuedState->mStarted)
		)
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] AbortQueuedState [%d] [%s] as [%p] [%d]"), this, m_dwRef, lQueuedState->mCharID, lQueuedState->mStateName, lQueuedState, lQueuedState->mReqID);
		}
#endif

		while	(
					(lQueuedGesture = (CQueuedGesture *) mQueue.GetNextAction (QueueActionGesture))
				&&	(lQueuedGesture->mCharID == lQueuedState->mCharID)
				&&	(lQueuedGesture->mStateName.CompareNoCase (lQueuedState->mStateName) == 0)
				&&	(RemoveQueuedGesture (lQueuedGesture, pReqStatus, pReason))
				)
		{;}
	}
}

/////////////////////////////////////////////////////////////////////////////

long CAgentWnd::QueueGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState)
{
	long				lReqID = 0;
	CAgentFile *		lAgentFile;
	CQueuedGesture *	lQueuedGesture = NULL;
	CAtlString			lGestureName (pGestureName);

	lGestureName.TrimLeft ();
	lGestureName.TrimRight ();
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueGesture [%d] [%s]"), this, m_dwRef, pCharID, lGestureName);
	}
#endif

	if	(
			(lAgentFile = GetAgentFile())
		&&	(lAgentFile->FindGesture (lGestureName) >= 0)
		&&	(lQueuedGesture = new CQueuedGesture (lGestureName, pCharID, lReqID=(pCharID?NextReqID():-1)))
		)
	{
		lQueuedGesture->mStateName = pForState;
		mQueue.AddTail (lQueuedGesture);
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			if	(lQueuedGesture->mStateName.IsEmpty())
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueGesture [%d] [%s] as [%p] [%d]"), this, m_dwRef, pCharID, lGestureName, lQueuedGesture, lReqID);
			}
			else
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueGesture [%d] [%s] State [%s] as [%p] [%d]"), this, m_dwRef, pCharID, lGestureName, lQueuedGesture->mStateName, lQueuedGesture, lReqID);
			}
		}
#endif
	}
	else
	{
		lReqID = 0;
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueGesture [%d] [%s] failed"), this, m_dwRef, pCharID, lGestureName);
		}
#endif
	}
	return lReqID;
}

bool CAgentWnd::DoQueuedGesture ()
{
	tPtr <CQueuedGesture>	lQueuedGesture;

	if	(lQueuedGesture = (CQueuedGesture *) mQueue.GetNextAction (QueueActionGesture))
	{
		if	(lQueuedGesture->mStarted)
		{
			mQueue.RemoveHead ();
			lQueuedGesture->NotifyComplete (mNotify);
		}
		else
		{
			lQueuedGesture->NotifyStarted (mNotify);

			if	(mQueue.GetNextAction (QueueActionGesture) == lQueuedGesture)
			{
				mQueue.RemoveHead ();

				if	((GetStyle () & WS_VISIBLE) == 0)
				{
					if	(lQueuedGesture->mReqID > 0)
					{
						lQueuedGesture->NotifyComplete (mNotify, AGENTERR_CHARACTERNOTVISIBLE);
					}
				}
				else
				{
					if	(ShowGesture (lQueuedGesture->mGestureName, lQueuedGesture->mStateName))
					{
						mQueue.AddHead (lQueuedGesture.Detach());
					}
					else
					if	(lQueuedGesture->mReqID > 0)
					{
						lQueuedGesture->NotifyComplete (mNotify, AGENTERR_ANIMATIONNOTFOUND);
					}
				}
			}
			else
			{
				lQueuedGesture.Detach (); // Was deleted during NotifyStarted
			}
		}
		return true;
	}
	return false;
}

void CAgentWnd::AbortQueuedGesture (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedGesture *	lQueuedGesture;

	if	(lQueuedGesture = (CQueuedGesture *) pQueuedAction)
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] AbortQueuedGesture [%d] [%s] as [%p] [%d] Started [%u] Animating [%u]"), this, m_dwRef, lQueuedGesture->mCharID, lQueuedGesture->mGestureName, lQueuedGesture, lQueuedGesture->mReqID, lQueuedGesture->mStarted, !IsAnimationComplete());
		}
#endif
		if	(lQueuedGesture->mStarted)
		{
			if	(!ShowGesture (NULL))
			{
				Stop ();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentWnd::FindQueuedAction (long pReqID)
{
	CQueuedAction *	lRet = NULL;
	CQueuedAction *	lQueuedAction;
	POSITION		lPos;

	if	(
			(pReqID > 0)
		&&	(!mQueue.IsEmpty ())
		)
	{
		for	(lPos = mQueue.GetHeadPosition (); lPos;)
		{
			if	(
					(lQueuedAction = mQueue.GetNext (lPos))
				&&	(lQueuedAction->mReqID == pReqID)
				)
			{
				lRet = lQueuedAction;
				break;
			}
		}
	}
	return lRet;
}

CQueuedAction * CAgentWnd::NextQueuedAction (long pCharID)
{
	return mQueue.FindNextAction (pCharID);
}

UINT CAgentWnd::HasQueuedActions (long pCharID)
{
	return mQueue.HasActions (pCharID);
}

bool CAgentWnd::HasQueuedStates (long pCharID)
{
	return (mQueue.GetCharAction (QueueActionState, pCharID) != NULL);
}

bool CAgentWnd::HasQueuedGestures (long pCharID)
{
	return (mQueue.GetCharAction (QueueActionGesture, pCharID) != NULL);
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentWnd::FindQueuedState (long pCharID, LPCTSTR pStateName)
{
	CQueuedState *	lQueuedState = NULL;

	if	(!mQueue.IsEmpty ())
	{
		POSITION	lPos;
		CAtlString	lStateName (pStateName);

		lStateName.TrimLeft ();
		lStateName.TrimRight ();

		for	(lPos = mQueue.GetHeadPosition (); lPos;)
		{
			if	(
					(lQueuedState = (CQueuedState *) mQueue.GetNext (lPos))
				&&	(lQueuedState->mAction == QueueActionState)
				&&	(
						(pCharID < 0)
					||	(lQueuedState->mCharID == pCharID)
					)
				&&	(
						(!pStateName)
					||	(lQueuedState->mStateName.CompareNoCase (lStateName) == 0)
					)
				)
			{
				break;
			}
			lQueuedState = NULL;
		}
	}
	return lQueuedState;
}

CQueuedAction * CAgentWnd::FindQueuedGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState)
{
	CQueuedGesture *	lQueuedGesture = NULL;

	if	(!mQueue.IsEmpty ())
	{
		POSITION	lPos;
		CAtlString	lGestureName (pGestureName);
		CAtlString	lForState (pForState);

		lGestureName.TrimLeft ();
		lGestureName.TrimRight ();
		lForState.TrimLeft ();
		lForState.TrimRight ();

		for	(lPos = mQueue.GetHeadPosition (); lPos;)
		{
			if	(
					(lQueuedGesture = (CQueuedGesture *) mQueue.GetNext (lPos))
				&&	(lQueuedGesture->mAction == QueueActionGesture)
				&&	(
						(pCharID < 0)
					||	(lQueuedGesture->mCharID == pCharID)
					)
				&&	(
						(!pGestureName)
					||	(lQueuedGesture->mGestureName.CompareNoCase (lGestureName) == 0)
					)
				&&	(
						(!pForState)
					||	(lQueuedGesture->mStateName.CompareNoCase (lForState) == 0)
					)
				)
			{
				break;
			}
			lQueuedGesture = NULL;
		}
	}
	return lQueuedGesture;
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CAgentWnd::GetQueuedState (CQueuedAction * pQueuedState)
{
	CQueuedState *	lQueuedState = NULL;
	POSITION		lPos;

	if	(
			(lPos = mQueue.Find (pQueuedState))
		&&	(lQueuedState = (CQueuedState *) mQueue.GetAt (lPos))
		&&	(lQueuedState->mAction == QueueActionState)
		)
	{
		return lQueuedState->mStateName.AllocSysString();
	}
	return tBstrPtr();
}

tBstrPtr CAgentWnd::GetQueuedGesture (CQueuedAction * pQueuedGesture)
{
	CQueuedGesture *	lQueuedGesture = NULL;
	POSITION			lPos;

	if	(
			(lPos = mQueue.Find (pQueuedGesture))
		&&	(lQueuedGesture = (CQueuedGesture *) mQueue.GetAt (lPos))
		&&	(lQueuedGesture->mAction == QueueActionGesture)
		)
	{
		return lQueuedGesture->mGestureName.AllocSysString();
	}
	return tBstrPtr();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::RemoveQueuedState (CQueuedAction * pQueuedState, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool			lRet = false;
	CQueuedState *	lQueuedState = NULL;
	POSITION		lPos;

	if	(
			(pQueuedState)
		&&	(lPos = mQueue.Find (pQueuedState))
		&&	(lQueuedState = (CQueuedState *) mQueue.GetAt (lPos))
		&&	(lQueuedState->mAction == QueueActionState)
		)
	{
		lRet = RemoveQueuedAction (pQueuedState, pReqStatus, pReason);
	}
	return lRet;
}

bool CAgentWnd::RemoveQueuedState (long pCharID, LPCTSTR pStateName, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool				lRet = false;
	CQueuedAction *		lQueuedState;
	CQueuedGesture *	lQueuedGesture;

	if	(
			(lQueuedState = FindQueuedState (pCharID, pStateName))
		&&	(RemoveQueuedAction (lQueuedState, pReqStatus, pReason))
		)
	{
		lRet = true;
	}
	else
	{
		while	(
					(lQueuedGesture = (CQueuedGesture *) mQueue.GetNextAction (QueueActionGesture))
				&&	(
						(pCharID < 0)
					||	(lQueuedGesture->mCharID == pCharID)
					)
				&&	(lQueuedGesture->mStateName.CompareNoCase (pStateName) == 0)
				&&	(RemoveQueuedGesture (lQueuedGesture, pReqStatus, pReason))
				)
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CAgentWnd::RemoveQueuedGesture (CQueuedAction * pQueuedGesture, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool				lRet = false;
	CQueuedGesture *	lQueuedGesture = NULL;
	POSITION			lPos;

	if	(
			(pQueuedGesture)
		&&	(lPos = mQueue.Find (pQueuedGesture))
		&&	(lQueuedGesture = (CQueuedGesture *) mQueue.GetAt (lPos))
		&&	(lQueuedGesture->mAction == QueueActionGesture)
		)
	{
		lRet = RemoveQueuedAction (pQueuedGesture, pReqStatus, pReason);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::PutQueuedAction (CQueuedAction * pQueuedAction)
{
	bool	lRet = false;

	if	(pQueuedAction)
	{
		mQueue.AddTail (pQueuedAction);
		ActivateQueue (true);
		lRet = true;
	}
	return lRet;
}

bool CAgentWnd::RemoveQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool					lRet = false;
	tPtr <CQueuedAction>	lQueuedAction = NULL;
	POSITION				lPos;

	if	(
			(pQueuedAction)
		&&	(lPos = mQueue.Find (pQueuedAction))
		&&	(lQueuedAction = mQueue.GetAt (lPos))
		)
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] RemoveQueuedAction [%p] [%d] [%d] [%d] for [%p] [%s] left [%d]"), this, m_dwRef, pQueuedAction, lQueuedAction->mAction, lQueuedAction->mCharID, lQueuedAction->mReqID, pReqStatus, pReason, mQueue.GetCount()-1);
		}
#endif
		mQueue.RemoveAt (lPos);
		AbortQueuedAction (lQueuedAction, pReqStatus, pReason);

		if	(pReqStatus)
		{
			lQueuedAction->NotifyComplete (mNotify, pReqStatus);
		}
		lRet = true;
	}

#ifdef	_LOG_QUEUE_OPS
	if	(
			(!lRet)
#ifndef	_DEBUG
		&&	(pQueuedAction)
#endif
		&&	(LogIsActive (_LOG_QUEUE_OPS))
		)
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] RemoveQueuedAction [%p] failed"), this, m_dwRef, pQueuedAction);
	}
#endif
	return lRet;
}

void CAgentWnd::AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedAction *	lQueuedAction;

	if	(lQueuedAction = (CQueuedAction *) pQueuedAction)
	{
		if	(lQueuedAction->mAction == QueueActionState)
		{
			AbortQueuedState (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionGesture)
		{
			AbortQueuedGesture (pQueuedAction, pReqStatus, pReason);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::ClearQueuedActions (long pCharID, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool	lRet = false;

	if	(!mQueue.IsEmpty ())
	{
		if	(pCharID >= 0)
		{
			CQueuedAction *	lQueuedAction;
			POSITION		lPos;
			bool			lDeleted;

#ifdef	_LOG_QUEUE_OPS
			if	(LogIsActive (_LOG_QUEUE_OPS))
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] ClearQueuedActions for [%d] [%8.8X] [%s]"), this, m_dwRef, pCharID, pReqStatus, pReason);
			}
#endif
			do
			{
				lDeleted = false;
				for	(lPos = mQueue.GetHeadPosition (); lPos;)
				{
					if	(
							(lQueuedAction = mQueue.GetNext (lPos))
						&&	(lQueuedAction->mCharID == pCharID)
						)
					{
						if	(RemoveQueuedAction (lQueuedAction, pReqStatus, pReason))
						{
							lRet = true;
							lDeleted = true;
						}
						break;
					}
				}
			}
			while (lDeleted);
		}
		else
		{
#ifdef	_LOG_QUEUE_OPS
			if	(LogIsActive (_LOG_QUEUE_OPS))
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Clear [%d] QueuedActions [%8.8X] [%s]"), this, m_dwRef, mQueue.GetCount(), pReqStatus, pReason);
			}
#endif
			mQueue.DeleteAll ();
			lRet = true;
		}
	}

	if	(lRet)
	{
		ActivateQueue (false);
	}
	return lRet;
}

bool CAgentWnd::ClearQueuedStates (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive, LPCTSTR pExcludeState, ...)
{
	bool				lRet = false;
	CQueuedAction *		lQueuedAction;
	CQueuedState *		lQueuedState;
	POSITION			lPos;
	bool				lDeleted;
	CAtlStringArray		lExcludeStates;

	if	(!mQueue.IsEmpty ())
	{
		if	(pExcludeState)
		{
			va_list lArgPtr;
			LPCTSTR	lExcludeState;

			va_start (lArgPtr, pExcludeState);
			for	(lExcludeState = pExcludeState; lExcludeState; lExcludeState = va_arg (lArgPtr, LPCTSTR))
			{
				AddSortedString (lExcludeStates, lExcludeState);
			}
			va_end (lArgPtr);
		}
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] ClearQueuedStates for [%d] [%8.8X] [%s] exclude [%u] [%s]"), this, m_dwRef, pCharID, pReqStatus, pReason, pExcludeActive, JoinStringArray(lExcludeStates,_T(", ")));
		}
#endif
		do
		{
			lDeleted = false;
			for	(lPos = mQueue.GetHeadPosition (); lPos;)
			{
				if	(
						(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionState)
					&&	(
							(pCharID < 0)
						||	(lQueuedAction->mCharID == pCharID)
						)
					&&	(lQueuedState = (CQueuedState *) lQueuedAction)
					)
				{
					if	(
							(pExcludeActive)
						&&	(lQueuedState->mStarted)
						)
					{
						continue;
					}
					if	(
							(lExcludeStates.GetCount() > 0)
						&&	(FindSortedString (lExcludeStates, lQueuedState->mStateName) >= 0)
						)
					{
						continue;
					}
					if	(RemoveQueuedAction (lQueuedAction, pReqStatus, pReason))
					{
						lRet = true;
						lDeleted = true;
					}
					break;
				}
			}
		}
		while (lDeleted);
	}

	if	(mQueue.IsEmpty ())
	{
		ActivateQueue (false);
	}
	return lRet;
}

bool CAgentWnd::ClearQueuedGestures (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive, LPCTSTR pExcludeState, ...)
{
	bool				lRet = false;
	CQueuedAction *		lQueuedAction;
	CQueuedGesture *	lQueuedGesture;
	POSITION			lPos;
	bool				lDeleted;
	CAtlStringArray		lExcludeStates;

	if	(!mQueue.IsEmpty ())
	{
		if	(pExcludeState)
		{
			va_list lArgPtr;
			LPCTSTR	lExcludeState;

			va_start (lArgPtr, pExcludeState);
			for	(lExcludeState = pExcludeState; lExcludeState; lExcludeState = va_arg (lArgPtr, LPCTSTR))
			{
				AddSortedString (lExcludeStates, lExcludeState);
			}
			va_end (lArgPtr);
		}
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] ClearQueuedGestures for [%d] [%8.8X] [%s] exclude [%u] [%s]"), this, m_dwRef, pCharID, pReqStatus, pReason, pExcludeActive, JoinStringArray(lExcludeStates,_T(", ")));
		}
#endif
		do
		{
			lDeleted = false;
			for	(lPos = mQueue.GetHeadPosition (); lPos;)
			{
				if	(
						(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionGesture)
					&&	(
							(pCharID < 0)
						||	(lQueuedAction->mCharID == pCharID)
						)
					&&	(lQueuedGesture = (CQueuedGesture *) lQueuedAction)
					)
				{
					if	(
							(pExcludeActive)
						&&	(
								(lQueuedGesture->mStarted)
							||	(!lQueuedGesture->mStateName.IsEmpty ()) // Assumes that ClearQueuedStates was used for state gestures
							)
						)
					{
						continue;
					}
					if	(
							(lExcludeStates.GetCount() > 0)
						&&	(FindSortedString (lExcludeStates, lQueuedGesture->mStateName) >= 0)
						)
					{
						continue;
					}
					if	(RemoveQueuedAction (lQueuedAction, pReqStatus, pReason))
					{
						lRet = true;
						lDeleted = true;
					}
					break;
				}
			}
		}
		while (lDeleted);
	}

	if	(mQueue.IsEmpty ())
	{
		ActivateQueue (false);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

UINT CAgentWnd::IsQueueBusy () const
{
	UINT	lRet = 0;
	if	(this)
	{
#ifdef	_WIN64
		lRet = InterlockedAnd ((long*)&mQueueBusy, 0xFFFFFFFF);
#else
		lRet = _InterlockedAnd ((long*)&mQueueBusy, 0xFFFFFFFF);
#endif
	}
	return lRet;
}

int CAgentWnd::_PreDoQueue ()
{
	long	lQueueBusy;
#ifdef	_WIN64
	lQueueBusy = InterlockedIncrement ((long*)&mQueueBusy);
#else
	lQueueBusy = _InterlockedIncrement ((long*)&mQueueBusy);
#endif
	if	(lQueueBusy < 0)
	{
		lQueueBusy = 1;
#ifdef	_WIN64
		InterlockedExchange ((long*)&mQueueBusy, lQueueBusy);
#else
		_InterlockedExchange ((long*)&mQueueBusy, lQueueBusy);
#endif
#ifdef	_DEBUG
		LogMessage (LogIfActive, _T("PreDoQueue error [%d]"), lQueueBusy);
#endif
		lQueueBusy = 1;
	}
	return (int)lQueueBusy;
}

int CAgentWnd::_PostDoQueue ()
{
	long	lQueueBusy;
#ifdef	_WIN64
	lQueueBusy = InterlockedDecrement ((long*)&mQueueBusy);
#else
	lQueueBusy = _InterlockedDecrement ((long*)&mQueueBusy);
#endif
	if	(lQueueBusy < 0)
	{
		lQueueBusy = 0;
#ifdef	_WIN64
		InterlockedExchange ((long*)&mQueueBusy, lQueueBusy);
#else
		_InterlockedExchange ((long*)&mQueueBusy, lQueueBusy);
#endif
#ifdef	_DEBUG
		LogMessage (LogIfActive, _T("PostDoQueue [%d] without PreDoQueue!"), lQueueBusy);
#endif
		lQueueBusy = 0;
	}
	return (int)lQueueBusy;
}

int CAgentWnd::PreDoQueue ()
{
	if	(this)
	{
		return _PreDoQueue ();
	}
	return -1;
}

int CAgentWnd::PostDoQueue ()
{
	if	(this)
	{
		return _PostDoQueue ();
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

UINT_PTR CAgentWnd::IsQueueActive () const
{
	if	(IsWindow ())
	{
		return mQueueTimer;
	}
	return 0;
}

UINT_PTR CAgentWnd::ActivateQueue (bool pImmediate, DWORD pQueueTime)
{
	UINT_PTR			lRet = 0;
	CAgentStreamInfo *	lStreamInfo = NULL;

	if	(IsWindow ())
	{
		DWORD	lQueueTime = 0;

		if	(pQueueTime)
		{
			lQueueTime = min (max (pQueueTime, mQueueTimeMin), mQueueTimeMax);
		}
		else
		if	(!mQueue.IsEmpty())
		{
			lQueueTime = mQueueTimeDefault;
		}
		else
		if	(
				(lStreamInfo = GetAgentStreamInfo())
			&&	(lStreamInfo->GetSequenceLoop (NULL) == S_OK)
			)
		{
			lQueueTime = mQueueTimeDefault;
		}

		if	(lQueueTime)
		{
			if	(
					(!mQueueTimer)
				||	(mQueueTime != lQueueTime)
				)
			{
#ifdef	_TRACE_RESOURCES_EX
				if	(LogIsActive (_TRACE_RESOURCES_EX))
				{
					CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::SetQueueTimer [%u]"), this, mQueueTimer);
				}
#endif

				mQueueTimer = SetTimer ((UINT_PTR)&mQueueTimer, mQueueTime=lQueueTime, NULL);

#ifdef	_TRACE_RESOURCES_EX
				if	(LogIsActive (_TRACE_RESOURCES_EX))
				{
					CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::SetQueueTimer [%u] Done"), this, mQueueTimer);
				}
#endif
#ifdef	_LOG_QUEUE_CYCLES
				if	(LogIsActive (_LOG_QUEUE_CYCLES))
				{
					LogMessage (_LOG_QUEUE_CYCLES, _T("Queue [%u] Activated [%u]"), mQueueTimer, mQueueTime);
				}
#endif
			}
			if	(pImmediate)
			{
#ifdef	_LOG_QUEUE_CYCLES
				if	(LogIsActive (_LOG_QUEUE_CYCLES))
				{
					LogMessage (_LOG_QUEUE_CYCLES, _T("Queue [%u] Immediate"), mQueueTimer);
				}
#endif
				PostMessage (WM_TIMER, mQueueTimer);
			}
			lRet = mQueueTimer;
		}
		else
		{
			SuspendQueue ();
		}
	}
	else
	{
		SuspendQueue ();
	}
	return lRet;
}

UINT_PTR CAgentWnd::SuspendQueue ()
{
	UINT_PTR	lRet = 0;

	if	(
			(IsWindow ())
		&&	(mQueueTimer)
		)
	{
#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::KillQueueTimer [%u]"), this, mQueueTimer);
		}
#endif

		lRet = mQueueTimer;
		KillTimer (mQueueTimer);

#ifdef	_TRACE_RESOURCES_EX
		if	(LogIsActive (_TRACE_RESOURCES_EX))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::KillQueueTimer Done"), this);
		}
#endif
#ifdef	_LOG_QUEUE_CYCLES
		if	(LogIsActive (_LOG_QUEUE_CYCLES))
		{
			LogMessage (_LOG_QUEUE_CYCLES, _T("Queue [%u] Suspended [%u]"), mQueueTimer, m_dwRef);
		}
#endif
	}
	mQueueTimer = 0;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::DoAnimationQueue ()
{
	bool	lRet = false;

	if	(IsAnimationComplete ())
	{
		CQueuedAction *	lNextAction = mQueue.GetNextAction ();

#ifdef	_LOG_QUEUE_CYCLES
		if	(LogIsActive (_LOG_QUEUE_CYCLES))
		{
			LogMessage (_LOG_QUEUE_CYCLES, _T("Queue Timer [%u] [%u] Actions [%d] Ref [%u] Visible [%u]"), mQueueTimer, mQueueTime, mQueue.GetCount(), m_dwRef, IsWindowVisible());
		}
#endif

		if	(DoAnimationLoop ())
		{
#ifdef	_LOG_QUEUE_CYCLES
			if	(LogIsActive (_LOG_QUEUE_CYCLES))
			{
				LogMessage (_LOG_QUEUE_CYCLES, _T("Animation looped"));
			}
#endif
			ActivateQueue (true);
		}
		else
		if	(
				(DoQueuedState ())
			||	(DoQueuedGesture ())
			)
		{
			ActivateQueue ((mQueue.GetNextAction () != lNextAction));
		}
		else
		if	(mQueue.IsEmpty ())
		{
			SuspendQueue ();
		}

		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::DoAnimationLoop ()
{
	bool				lRet = false;
	CAgentStreamInfo *	lStreamInfo;
	long				lAnimationNdx;

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(lStreamInfo->GetSequenceLoop (NULL) == S_OK)
		&&	(lStreamInfo->GetAnimationIndex (&(lAnimationNdx=-1)) == S_OK)
		&&	(lStreamInfo->SequenceAnimationLoop (lAnimationNdx) == S_OK)
		)
	{
		long	lSequenceDuration = 0;

		if	(IsSoundEnabled ())
		{
			lStreamInfo->CueSequenceAudio ();
		}
		else
		{
			lStreamInfo->ClearSequenceAudio ();
		}
#ifdef	_DEBUG_FILTER_SEGMENTS
		LogMessage (_DEBUG_FILTER_SEGMENTS, _T("[%p] [%s] Sequenced [%s] loop [%d]"), this, AtlTypeName(this), GetFileGestures().mAnimations[lAnimationNdx]->mName, lStreamInfo->GetAnimationSequence()->mLoopDuration);
#endif
		AnimationSequenceChanged ();
		lStreamInfo->GetSequenceDuration (&lSequenceDuration);
#ifdef	_DEBUG
		if	(GetDurationMs () != lSequenceDuration)
		{
			LogMessage (LogDebug, _T("!!! Duration [%d] [%d]"), lSequenceDuration, GetDurationMs());
		}
#endif
		PlayFromTo (0, lSequenceDuration, false);
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentWnd::OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	(
			(mQueueTimer)
		&&	(wParam == mQueueTimer)
		)
	{
		if	(PreDoQueue () > 0)
		{
			try
			{
				DoAnimationQueue ();
			}
			catch AnyExceptionSilent
			PostDoQueue ();
		}
	}
	else
	if	(
			(mIdleTimer)
		&&	(wParam == mIdleTimer)
#ifdef	_DEBUG
		&&	(GetProfileDebugInt(_T("DebugDisableIdle")) <= 0)
#endif
		)
	{
		if	(PreDoQueue () > 0)
		{
			try
			{
				DoIdle ();
			}
			catch AnyExceptionSilent
			PostDoQueue ();
		}
	}
	else
	{
		bHandled = CDirectShowWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	}
	return 0;
}

LRESULT CAgentWnd::OnMediaEvent (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#if	FALSE
	ActivateQueue (false, mQueueTimeMin);
#else
	if	(
			(mQueueTimer != NULL)
		&&	(mMediaEvent != NULL)
		)
	{
		long		lEventCode;
		LONG_PTR	lEventParam1 = NULL;
		LONG_PTR	lEventParam2 = NULL;

		while	(mMediaEvent->GetEvent (&lEventCode, &lEventParam1, &lEventParam2, 0) == S_OK)
		{
			if	(
					(lEventCode == EC_COMPLETE)
				||	(lEventCode == EC_END_OF_SEGMENT)
				)
			{
				ActivateQueue (false, mQueueTimeMin);
				break;
			}
		}
	}
#endif
	bHandled = CDirectShowWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentWnd::EnableIdle (bool pEnable)
{
	bool	lRet = false;

	if	(pEnable)
	{
		if	(mIdleLevel < 0)
		{
			lRet = true;
		}
		mIdleLevel = max (mIdleLevel, 0);

#ifdef	_DEBUG_IDLE
		if	(lRet)
		{
			LogMessage (_DEBUG_IDLE, _T("[%p] Idle enabled [%d]"), this, mIdleLevel);
		}
#endif
		if	(
				(IsWindow ())
			&&	(!mIdleTimer)
			)
		{
#ifdef	_TRACE_RESOURCES_EX
			if	(LogIsActive (_TRACE_RESOURCES_EX))
			{
				CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::SetIdleTimer [%u]"), this, mIdleTimer);
			}
#endif

#ifdef	_DEBUG_IDLE
			mIdleTimer = SetTimer ((UINT_PTR)&mIdleTimer, 10, NULL);
#else
			mIdleTimer = SetTimer ((UINT_PTR)&mIdleTimer, 1000, NULL);
#endif

#ifdef	_TRACE_RESOURCES_EX
			if	(LogIsActive (_TRACE_RESOURCES_EX))
			{
				CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::SetIdleTimer [%u] Done"), this, mIdleTimer);
			}
#endif
		}
	}
	else
	{
		StopIdle (_T("DisableIdle"));

		if	(mIdleLevel >= 0)
		{
			lRet = true;
		}
		if	(mIdleTimer)
		{
			if	(IsWindow ())
			{
#ifdef	_TRACE_RESOURCES_EX
				if	(LogIsActive (_TRACE_RESOURCES_EX))
				{
					CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::KillIdleTimer [%u]"), this, mIdleTimer);
				}
#endif

				KillTimer (mIdleTimer);

#ifdef	_TRACE_RESOURCES_EX
				if	(LogIsActive (_TRACE_RESOURCES_EX))
				{
					CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES_EX, _T("[%p] CAgentWnd::KillIdleTimer Done"), this);
				}
#endif
			}
			lRet = true;
		}
		mIdleTimer = 0;
		mIdleLevel = -1;
		mIdleQueue.RemoveAll ();

#ifdef	_DEBUG_IDLE
		if	(lRet)
		{
			LogMessage (_DEBUG_IDLE, _T("[%p] Idle disabled"), this);
		}
#endif
	}
	return lRet;
}

bool CAgentWnd::IsIdleEnabled () const
{
	return (mIdleLevel >= 0);
}

bool CAgentWnd::StopIdle (LPCTSTR pReason)
{
	bool	lRet = false;

	if	(IsWindow ())
	{
		if	(mIdleLevel > 0)
		{
			mIdleLevel = 0;

			if	(mIdleStarted)
			{
				if	(IsPlaying ())
				{
#ifdef	_DEBUG_IDLE
					LogMessage (_DEBUG_IDLE, _T("[%p] Stop playing idle"), this);
#endif
					CDirectShowWnd::Stop ();
				}
//
//	MS Agent doesn't play idle exit animations
//				ShowGesture (NULL, NULL, false);
				if	(ClearAnimations ())
				{
					AnimationSequenceChanged ();
				}
			}

#if	FALSE
			if	(
					(lIdleStarted)
				&&	(IsWindow ())
				&&	(IsWindowVisible ())
				&&	(mQueue.IsEmpty ())
				)
			{
				QueueGesture (-1, _T("RestPose"));
			}
#endif
			lRet = true;
		}
		mIdleStarted = false;
		mIdleQueue.RemoveAll ();

#ifdef	_DEBUG_IDLE
		if	(lRet)
		{
			LogMessage (_DEBUG_IDLE, _T("[%p] Idle stopped [%s]"), this, pReason);
		}
#endif
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

int CAgentWnd::IsIdle () const
{
//
//	Return < 0 if idle disabled
//	Return 0 if not idle
//	Return > 0 if idle
//
	if	(
			(mIdleLevel >= 0)
		&&	(IsWindow ())
		&&	(IsWindowVisible ())
		&&	(mQueue.IsEmpty ())
		&&	(
				(mIdleLevel > 0)
			||	(const_cast <CAgentWnd *> (this)->IsAnimationComplete ())
			)
		)
	{
		return mIdleLevel+1;
	}
	return min (mIdleLevel, 0);
}

bool CAgentWnd::DoIdle ()
{
	bool	lRet = false;

	if	(IsIdle () > 0)
	{
#ifdef	_DEBUG_IDLE_NOT
		LogMessage (_DEBUG_IDLE|LogHighVolume, _T("[%p] Idle [%d] [%u]"), this, mIdleLevel, IsAnimationComplete ());
#endif
		if	(IsAnimationComplete ())
		{
			if	(mIdleLevel == 0)
			{
				mIdleStarted = false;
			}
			if	(mIdleQueue.GetCount() > 0)
			{
				if	(!mIdleQueue [0].IsEmpty ())
				{
					mIdleStarted = true;
					ShowGesture (mIdleQueue [0], NULL, false);
				}
				mIdleQueue.RemoveAt (0);
			}
			else
			if	(GetAgentFile())
			{
				const CAtlStringArray *	lGestures;

				if	(
						(
#ifdef	_DEBUG_IDLE
							(mIdleLevel < 2)
#else
							(mIdleLevel < 3)
#endif
						&&	(lGestures = GetFileStates() (_T("IDLINGLEVEL1")))
						)
					||	(
#ifdef	_DEBUG_IDLE
							(mIdleLevel < 4)
#else
							(mIdleLevel < 6)
#endif
						&&	(lGestures = GetFileStates() (_T("IDLINGLEVEL2")))
						)
					||	(lGestures = GetFileStates() (_T("IDLINGLEVEL3")))
					)
				{
					mIdleQueue.Add (_T(""));
					mIdleQueue.Add (_T(""));
					mIdleQueue.Add (_T(""));
					mIdleQueue.Add (_T(""));
					mIdleQueue.Append (*lGestures);
					mIdleLevel++;
				}
				else
				{
					mIdleLevel = min (max (mIdleLevel+1, 1), 10);
					if	(mIdleLevel >= 10)
					{
						mIdleStarted = true;
					}
				}
			}
		}
		lRet = true;
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentWnd::GetWindow (HWND *phwnd)
{
	HRESULT	lResult = S_OK;
	if	(!phwnd)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*phwnd) = m_hWnd;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentWnd::ContextSensitiveHelp (BOOL fEnterMode)
{
	return E_NOTIMPL;
}
