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
#include "DirectShowFilter.h"
#include "DirectShowEnums.h"
#include "MallocPtr.h"
#include "GuidStr.h"
#include "VfwErr.h"
#include "DebugStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal|LogHighVolume
//#define	_DEBUG_FILTER		LogNormal
//#define	_DEBUG_FILTER_EX	LogNormal|LogHighVolume
//#define	_DEBUG_SEEKING		LogNormal|LogTimeMs
//#define	_DEBUG_SEEKING_EX	LogNormal|LogTimeMs
//#define	_DEBUG_STATE		LogNormal|LogHighVolume|LogTimeMs
//#define	_DEBUG_STREAM_EX	LogNormal|LogHighVolume|LogTimeMs
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_DirectShow"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CDirectShowFilter, CCmdTarget)
	INTERFACE_PART(CDirectShowFilter, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDirectShowFilter, __uuidof(IPersist), BaseFilter)
	INTERFACE_PART(CDirectShowFilter, __uuidof(IBaseFilter), BaseFilter)
	INTERFACE_PART(CDirectShowFilter, __uuidof(IMediaFilter), BaseFilter)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CDirectShowFilter, BaseFilter)

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CDirectShowFilter, CCmdTarget)

CDirectShowFilter::CDirectShowFilter()
:	mFilterGraph (NULL),
	mState (State_Stopped)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowFilter::CDirectShowFilter (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation ();
	EnableAutomation (); // Only required so CCmdTarget::FromIDispatch will work;
}

CDirectShowFilter::~CDirectShowFilter()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowFilter::~CDirectShowFilter (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Terminate ();
	AfxOleUnlockApp ();
}

void CDirectShowFilter::Terminate ()
{
	if	(this)
	{
		try
		{
			EndClock ();

			CSingleLock	lLock (&mStateLock, TRUE);
			try
			{
				mFilterGraph = NULL;
				SafeFreeSafePtr (mClock);
				mInputPins.RemoveAll ();
				mOutputPins.RemoveAll ();
			}
			catch AnyExceptionSilent
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowFilter::OnFinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CDirectShowFilter::OnFinalRelease"), this);
	}
#endif
	CCmdTarget::OnFinalRelease ();
}

LPUNKNOWN CDirectShowFilter::GetInterfaceHook(const void* iid)
{
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] %s::QueryInterface [%s]"), this, m_dwRef, ObjClassName(this), CGuidStr::GuidName(*(GUID*)iid));
#endif
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::Start (REFERENCE_TIME pStartOffset)
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(mFilterGraph)
		{
			FILTER_STATE	lOldState = mState;
#ifdef	_DEBUG_STATE
			if	(lOldState != State_Running)
			{
				LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] State [%s (%u %u)] <Start> at [%f] for [%f - %f] Offset [%f = %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), IsClockStarted(), IsClockSet(), RefTimeSec(GetReferenceTime()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(pStartOffset), RefTimeSec(GetReferenceTime()-pStartOffset));
			}
#endif
			if	(SUCCEEDED (lResult = OnStateChanging (lOldState, State_Running)))
			{
				SetStartTime (pStartOffset);
				mState = State_Running;
				lResult = OnStateChanged (lOldState, State_Running);
			}
#ifdef	_DEBUG_STATE
			LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] State [%s (%u %u)] [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), IsClockStarted(), IsClockSet(), RefTimeSec(GetStreamTime(mState)), RefTimeSec(GetStopTime(mState)));
#endif
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT CDirectShowFilter::Stop ()
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(mFilterGraph)
		{
			FILTER_STATE	lOldState = mState;
#ifdef	_DEBUG_STATE
			if	(lOldState != State_Stopped)
			{
				LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] State [%s (%u %u)] <Stop> at [%f] for [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), IsClockStarted(), IsClockSet(), RefTimeSec(GetReferenceTime()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime));
			}
#endif
			OnStateChanging (lOldState, State_Stopped);
			mState = State_Stopped;
			lResult = OnStateChanged (lOldState, State_Stopped);
#ifdef	_DEBUG_STATE
			LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] State [%s (%u %u)] [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), IsClockStarted(), IsClockSet(), RefTimeSec(GetStreamTime(mState)), RefTimeSec(GetStopTime(mState)));
#endif
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT CDirectShowFilter::Pause ()
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		if	(mFilterGraph)
		{
			FILTER_STATE	lOldState = mState;
			REFERENCE_TIME	lStreamTime = GetStreamTime(mState);
#ifdef	_DEBUG_STATE
			if	(lOldState != State_Paused)
			{
				LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] State [%s (%u %u)] <Pause> at [%f] for [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), IsClockStarted(), IsClockSet(), RefTimeSec(GetReferenceTime()), RefTimeSec(mCurrTime), RefTimeSec(mStopTime));
			}
#endif
			if	(SUCCEEDED (lResult = OnStateChanging (lOldState, State_Paused)))
			{
				if	(lOldState == State_Running)
				{
					SetStartTime (-lStreamTime);
				}
				mState = State_Paused;
				lResult = OnStateChanged (lOldState, State_Paused);
			}
#ifdef	_DEBUG_STATE
			LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] State [%s (%u %u)] [%f - %f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), FilterStateStr(mState), IsClockStarted(), IsClockSet(), RefTimeSec(GetStreamTime(mState)), RefTimeSec(mStopTime));
#endif
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::OnStateChanging (FILTER_STATE pOldState, FILTER_STATE pNewState)
{
	HRESULT	lResult = S_OK;

	switch (pNewState)
	{
		case State_Stopped:
		{
			StopClock ();
		}	break;
		case State_Paused:
		{
			if	(pOldState == State_Stopped)
			{
				if	(SUCCEEDED (lResult = StartPins ()))
				{
					StartOutputStreams ();
				}
			}
		}	break;
		case State_Running:
		{
			if	(pOldState == State_Stopped)
			{
				lResult = Pause ();
			}
		}	break;
	}
	return lResult;
}

HRESULT CDirectShowFilter::OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState)
{
	HRESULT	lResult = S_OK;

	switch (pNewState)
	{
		case State_Stopped:
		{
			StopPins (); // Result intentionally ignored
			SetStartTime (0);
		}	break;
		case State_Paused:
		{
			if	(pOldState == State_Stopped)
			{
				StartClock (sDefaultClockInterval);
			}
		}	break;
		case State_Running:
		{
			if	(!IsClockStarted ())
			{
				StartClock (sDefaultClockInterval);
			}
		}	break;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::StartPins ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		HRESULT				lPinResult;
		int					lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx <= mOutputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mOutputPins [lPinNdx])
			{
				if	(SUCCEEDED (lPinResult = StartOutputPin (lPin)))
				{
					if	(SUCCEEDED (lResult))
					{
						lResult = S_OK;
					}
				}
				else
				{
					lResult = lPinResult;
				}
			}
		}

		for	(lPinNdx = 0; lPinNdx <= mInputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mInputPins [lPinNdx])
			{
				if	(SUCCEEDED (lPinResult = StartInputPin (lPin)))
				{
					if	(SUCCEEDED (lResult))
					{
						lResult = S_OK;
					}
				}
				else
				{
					lResult = lPinResult;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowFilter::StopPins ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		HRESULT				lPinResult;
		int					lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx <= mOutputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mOutputPins [lPinNdx])
			{
				if	(SUCCEEDED (lPinResult = StopOutputPin (lPin)))
				{
					if	(SUCCEEDED (lResult))
					{
						lResult = S_OK;
					}
				}
				else
				{
					lResult = lPinResult;
				}
			}
		}

		for	(lPinNdx = 0; lPinNdx <= mInputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mInputPins [lPinNdx])
			{
				if	(SUCCEEDED (lPinResult = StopInputPin (lPin)))
				{
					if	(SUCCEEDED (lResult))
					{
						lResult = S_OK;
					}
				}
				else
				{
					lResult = lPinResult;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::StartInputPin (CDirectShowPin * pPin)
{
	HRESULT	lResult = S_FALSE;

	if	(pPin)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			lResult = pPin->BeginInput ();
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowFilter::StopInputPin (CDirectShowPin * pPin)
{
	HRESULT	lResult = S_FALSE;

	if	(pPin)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			lResult = pPin->EndInput ();
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::StartOutputPin (CDirectShowPin * pPin)
{
	HRESULT	lResult = S_FALSE;

	if	(pPin)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			lResult = pPin->BeginOutput ();
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowFilter::StopOutputPin (CDirectShowPin * pPin)
{
	HRESULT	lResult = S_FALSE;

	if	(pPin)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			lResult = pPin->EndOutput ();
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::StartOutputStreams ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] StartOutputStreams [%f - %f] of [%f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(GetDuration()));
#endif
		HRESULT				lPinResult;
		int					lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx <= mOutputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mOutputPins [lPinNdx])
			{
				if	(SUCCEEDED (lPinResult = StartOutputStream (lPin)))
				{
					if	(SUCCEEDED (lResult))
					{
						lResult = S_OK;
					}
				}
				else
				{
					lResult = lPinResult;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowFilter::StopOutputStreams ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] StopOutputStreams [%f - %f] of [%f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(GetDuration()));
#endif
		HRESULT				lPinResult;
		int					lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx <= mOutputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mOutputPins [lPinNdx])
			{
				if	(SUCCEEDED (lPinResult = StopOutputStream (lPin)))
				{
					if	(SUCCEEDED (lResult))
					{
						lResult = S_OK;
					}
				}
				else
				{
					lResult = lPinResult;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::StartOutputStream (CDirectShowPin * pPin)
{
	HRESULT	lResult = S_FALSE;

	if	(pPin)
	{
		REFERENCE_TIME	lCurrTime = 0;
		REFERENCE_TIME	lStopTime = 0;

		GetSeekingTimes (lCurrTime, lStopTime);

		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			lResult = pPin->BeginOutputStream (lCurrTime, lStopTime);
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowFilter::StopOutputStream (CDirectShowPin * pPin)
{
	HRESULT	lResult = S_FALSE;

	if	(pPin)
	{
		CSingleLock	lLock (&mStateLock, TRUE);
		try
		{
			lResult = pPin->EndOutputStream ();
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowFilter::GetSeekingTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime)
{
	pCurrTime = 0;
	pStopTime = 0;
}

bool CDirectShowFilter::GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking)
{
	bool				lRet = false;
	IPinPtr				lUpstreamPin;
	PIN_INFO_Safe		lPinInfo;
	IMediaSeekingPtr	lMediaSeeking;

	(*pMediaSeeking) = NULL;

	{
		CSingleLock	lLock (&mStateLock, TRUE);

		try
		{
			CDirectShowPin *	lInputPin;

			if	(
					(mInputPins.GetSize () > 0)
				&&	(lInputPin = mInputPins (0))
				)
			{
				lUpstreamPin = lInputPin->SafeGetConnection ();
			}
		}
		catch AnyExceptionDebug
	}

	lMediaSeeking = lUpstreamPin;

	if	(
			(lMediaSeeking == NULL)
		&&	(lUpstreamPin != NULL)
		&&	(SUCCEEDED (lUpstreamPin->QueryPinInfo (&lPinInfo)))
		)
	{
		lMediaSeeking = lPinInfo.pFilter;
	}

	if	(lMediaSeeking != NULL)
	{
		(*pMediaSeeking) = lMediaSeeking.Detach ();
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowFilter::Flush ()
{
	BeginFlush ();
	EndFlush ();
}

void CDirectShowFilter::BeginFlush ()
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		int					lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx <= mOutputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mOutputPins [lPinNdx])
			{
				lPin->BeginOutputFlush ();
			}
		}
	}
	catch AnyExceptionDebug
}

void CDirectShowFilter::EndFlush ()
{
	CSingleLock	lLock (&mStateLock, TRUE);

	try
	{
		int					lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx <= mOutputPins.GetUpperBound (); lPinNdx++)
		{
			if	(lPin = mOutputPins [lPinNdx])
			{
				lPin->EndOutputFlush ();
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowFilter::OnJoinedFilterGraph ()
{
	InitializePins ();
}

void CDirectShowFilter::OnLeftFilterGraph ()
{
	Terminate ();
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowFilter::OnPinConnected (CDirectShowPin * pPin)
{
}

void CDirectShowFilter::OnPinDisconnected (CDirectShowPin * pPin)
{
}

void CDirectShowFilter::OnStartInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate)
{
	if	(mOutputPins.GetSize() == 1)
	{
		StartOutputStreams ();
	}
}

void CDirectShowFilter::OnEndInputStream (INT_PTR pPendingSamples)
{
	if	(
			(pPendingSamples <= 0)
		&&	(mOutputPins.GetSize() == 1)
		)
	{
		StopOutputStreams ();
	}
}

void CDirectShowFilter::OnGotInputSample (CDirectShowPin * pPin)
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::GetClassID (CLSID *pClassID)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::XBaseFilter::GetClassID"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	if	(pClassID)
	{
		(*pClassID) = pThis->GetClassID ();
		return S_OK;
	}
	else
	{
		return E_POINTER;
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::Stop (void)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::Stop"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = pThis->Stop ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::Stop"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::Pause (void)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::Pause"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = pThis->Pause ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::Pause"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::Run (REFERENCE_TIME tStart)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::Run"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = pThis->Start (tStart);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::Run"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::GetState (DWORD dwMilliSecsTimeout, FILTER_STATE *State)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::XBaseFilter::GetState"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = S_OK;

	if	(!State)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mStateLock.Lock (dwMilliSecsTimeout))
	{
		try
		{
			(*State) = pThis->mState;
		}
		catch AnyExceptionSilent

		pThis->mStateLock.Unlock ();
	}
	else
	{
		lResult = VFW_S_STATE_INTERMEDIATE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::GetState"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::SetSyncSource (IReferenceClock *pClock)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::SetSyncSource [%p]"), pThis, pThis->m_dwRef, ObjClassName(pThis), pClock);
#endif
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&pThis->mStateLock, TRUE);

	try
	{
		pThis->StopClock ();
		pThis->mClock = pClock;

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::SetSyncSource"), pThis, pThis->m_dwRef, ObjClassName(pThis));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::GetSyncSource (IReferenceClock **pClock)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::GetSyncSource"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&pThis->mStateLock, TRUE);

	try
	{
		if	(!pClock)
		{
			lResult = E_POINTER;
		}
		else
		if	((*pClock) = pThis->mClock)
		{
			(*pClock)->AddRef ();
		}

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::GetSyncSource"), pThis, pThis->m_dwRef, ObjClassName(pThis));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::EnumPins (IEnumPins **ppEnum)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::XBaseFilter::EnumPins"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT			lResult = S_OK;
	CEnumPins *		lEnum;
	IEnumPinsPtr	lInterface;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(lEnum = new CEnumPins (pThis->mInputPins, pThis->mOutputPins, pThis->GetControllingUnknown()))
	{
		lInterface = lEnum->GetControllingUnknown ();
		lEnum->ExternalRelease ();
		(*ppEnum) = lInterface.Detach ();
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::EnumPins"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::FindPin (LPCWSTR Id, IPin **ppPin)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::FindPin"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = E_NOTIMPL;

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::FindPin"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::QueryFilterInfo (FILTER_INFO *pInfo)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::XBaseFilter::QueryFilterInfo"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT		lResult = S_OK;
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
	CSingleLock	lLock (&pThis->mStateLock, TRUE);
#endif

	try
	{
		tMallocPtr <WCHAR>	lFilterName;

		if	(!pInfo)
		{
			lResult = E_POINTER;
		}
		else
		{
			lFilterName = AfxAllocTaskWideString (pThis->GetFilterName ());
			memset (pInfo->achName, 0, sizeof(pInfo->achName));
			wcsncpy (pInfo->achName, (LPCWSTR)lFilterName, sizeof(pInfo->achName)/sizeof(WCHAR));

			if	(pInfo->pGraph = pThis->mFilterGraph)
			{
				pInfo->pGraph->AddRef ();
			}
		}

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::QueryFilterInfo"), pThis, pThis->m_dwRef, ObjClassName(pThis));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::JoinFilterGraph (IFilterGraph *pGraph, LPCWSTR pName)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::XBaseFilter::JoinFilterGraph [%p] [%ls]"), pThis, pThis->m_dwRef, ObjClassName(pThis), pGraph, pName);
#endif
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&pThis->mStateLock, TRUE);

	try
	{
		if	(
				(pGraph)
			&&	(pName)
			)
		{
			if	(SUCCEEDED (LogVfwErr (LogNormal, lResult = pThis->SetFilterName (pName))))
			{
				pThis->mFilterGraph = pGraph;
				pThis->OnJoinedFilterGraph ();
				lResult = S_OK;
			}
		}
		else
		{
			pThis->OnLeftFilterGraph ();
			pThis->mFilterGraph = NULL;
		}

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::JoinFilterGraph"), pThis, pThis->m_dwRef, ObjClassName(pThis));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::XBaseFilter::QueryVendorInfo (LPWSTR *pVendorInfo)
{
	METHOD_PROLOGUE(CDirectShowFilter, BaseFilter)
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::XBaseFilter::QueryVendorInfo"), pThis, pThis->m_dwRef, ObjClassName(pThis));
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pVendorInfo)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pVendorInfo) = AfxAllocTaskWideString (_T("Internal"));
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XBaseFilter::QueryVendorInfo"), pThis, pThis->m_dwRef, ObjClassName(pThis));
	}
#endif
	return lResult;
}
