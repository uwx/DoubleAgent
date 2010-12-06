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

#ifdef	_DEBUG
//#define	_DEBUG_FILTER		LogNormal
//#define	_DEBUG_FILTER_EX	LogNormal|LogHighVolume
//#define	_DEBUG_SEEKING		LogNormal|LogTimeMs
//#define	_DEBUG_SEEKING_EX	LogNormal|LogTimeMs
//#define	_DEBUG_STATE		LogNormal|LogTimeMs|LogHighVolume
//#define	_DEBUG_STREAM_EX	LogNormal|LogTimeMs|LogHighVolume
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_DirectShowFilter"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

CDirectShowFilter::CDirectShowFilter()
:	mFilterGraph (NULL),
	mState (State_Stopped)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowFilter::CDirectShowFilter (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CDirectShowFilter::~CDirectShowFilter()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowFilter::~CDirectShowFilter (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Terminate ();
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowFilter::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDirectShowFilter::FinalRelease (%d) [%8.8X %8.8X]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	Terminate ();
}

void CDirectShowFilter::Terminate ()
{
	if	(this)
	{
		try
		{
			EndClock ();

			CMutexLock	lLock (mStateLock);
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowFilter::StartFilter (REFERENCE_TIME pStartOffset)
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CMutexLock	lLock (mStateLock);

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

HRESULT CDirectShowFilter::StopFilter ()
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CMutexLock	lLock (mStateLock);

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

HRESULT CDirectShowFilter::PauseFilter ()
{
	HRESULT		lResult = VFW_E_NOT_IN_GRAPH;
	CMutexLock	lLock (mStateLock);

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
	CMutexLock	lLock (mStateLock);

	try
	{
		HRESULT				lPinResult;
		INT_PTR				lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mOutputPins.GetCount(); lPinNdx++)
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

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mInputPins.GetCount(); lPinNdx++)
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
	CMutexLock	lLock (mStateLock);

	try
	{
		HRESULT				lPinResult;
		INT_PTR				lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mOutputPins.GetCount(); lPinNdx++)
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

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mInputPins.GetCount(); lPinNdx++)
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
		CMutexLock	lLock (mStateLock);
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
		CMutexLock	lLock (mStateLock);
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
		CMutexLock	lLock (mStateLock);
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
		CMutexLock	lLock (mStateLock);
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
	CMutexLock	lLock (mStateLock);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] StartOutputStreams [%f - %f] of [%f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(GetDuration()));
#endif
		HRESULT				lPinResult;
		INT_PTR				lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mOutputPins.GetCount(); lPinNdx++)
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
	CMutexLock	lLock (mStateLock);

	try
	{
#ifdef	_DEBUG_STATE
		LogMessage (_DEBUG_STATE, _T("[%s] [%p] [%f] StopOutputStreams [%f - %f] of [%f]"), ObjClassName(this), this, RefTimeSec(GetStreamTime(mState)), RefTimeSec(mCurrTime), RefTimeSec(mStopTime), RefTimeSec(GetDuration()));
#endif
		HRESULT				lPinResult;
		INT_PTR				lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mOutputPins.GetCount(); lPinNdx++)
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

		CMutexLock	lLock (mStateLock);
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
		CMutexLock	lLock (mStateLock);
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
		CMutexLock	lLock (mStateLock);

		try
		{
			CDirectShowPin *	lInputPin;

			if	(
					(mInputPins.GetCount() > 0)
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
	CMutexLock	lLock (mStateLock);

	try
	{
		INT_PTR				lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mOutputPins.GetCount(); lPinNdx++)
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
	CMutexLock	lLock (mStateLock);

	try
	{
		INT_PTR				lPinNdx;
		CDirectShowPin *	lPin;

		for	(lPinNdx = 0; lPinNdx < (INT_PTR)mOutputPins.GetCount(); lPinNdx++)
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
	if	(mOutputPins.GetCount() == 1)
	{
		StartOutputStreams ();
	}
}

void CDirectShowFilter::OnEndInputStream (INT_PTR pPendingSamples)
{
	if	(
			(pPendingSamples <= 0)
		&&	(mOutputPins.GetCount() == 1)
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

HRESULT STDMETHODCALLTYPE CDirectShowFilter::GetClassID (CLSID *pClassID)
{
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::GetClassID"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	if	(pClassID)
	{
		(*pClassID) = GetClassID ();
		return S_OK;
	}
	else
	{
		return E_POINTER;
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::Stop (void)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::Stop"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = StopFilter ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Stop"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::Pause (void)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::Pause"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = PauseFilter ();

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Pause"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::Run (REFERENCE_TIME tStart)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::Run"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = StartFilter (tStart);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::Run"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::GetState (DWORD dwMilliSecsTimeout, FILTER_STATE *State)
{
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::GetState"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = S_OK;

	if	(!State)
	{
		lResult = E_POINTER;
	}
	else
	if	(mStateLock.Lock (dwMilliSecsTimeout))
	{
		try
		{
			(*State) = mState;
		}
		catch AnyExceptionSilent

		mStateLock.Unlock ();
	}
	else
	{
		lResult = VFW_S_STATE_INTERMEDIATE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetState"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::SetSyncSource (IReferenceClock *pClock)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::SetSyncSource [%p]"), this, max(m_dwRef,-1), AtlTypeName(this), pClock);
#endif
	HRESULT		lResult = S_OK;
	CMutexLock	lLock (mStateLock);

	try
	{
		StopClock ();
		mClock = pClock;

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SetSyncSource"), this, max(m_dwRef,-1), AtlTypeName(this));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::GetSyncSource (IReferenceClock **pClock)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::GetSyncSource"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT		lResult = S_OK;
	CMutexLock	lLock (mStateLock);

	try
	{
		if	(!pClock)
		{
			lResult = E_POINTER;
		}
		else
		if	((*pClock) = mClock)
		{
			(*pClock)->AddRef ();
		}

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetSyncSource"), this, max(m_dwRef,-1), AtlTypeName(this));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::EnumPins (IEnumPins **ppEnum)
{
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::EnumPins"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT						lResult = S_OK;
	CComObject <CEnumPins> *	lEnum = NULL;
	IEnumPinsPtr				lInterface;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (CComObject <CEnumPins>::CreateInstance (&lEnum)))
	{
		lEnum->Initialize (mInputPins, mOutputPins, GetControllingUnknown());
		lInterface = lEnum;
		(*ppEnum) = lInterface.Detach ();
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::EnumPins"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::FindPin (LPCWSTR Id, IPin **ppPin)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::FindPin"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = E_NOTIMPL;

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::FindPin"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowFilter::QueryFilterInfo (FILTER_INFO *pInfo)
{
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::QueryFilterInfo"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT		lResult = S_OK;
#ifndef	_DEBUG	// Skip for debugging - allows logging to be reentrant
	CMutexLock	lLock (mStateLock);
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
			lFilterName = AtlAllocTaskWideString (GetFilterName ());
			memset (pInfo->achName, 0, sizeof(pInfo->achName));
			wcsncpy (pInfo->achName, (LPCWSTR)lFilterName, sizeof(pInfo->achName)/sizeof(WCHAR));

			if	(pInfo->pGraph = mFilterGraph)
			{
				pInfo->pGraph->AddRef ();
			}
		}

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::QueryFilterInfo"), this, max(m_dwRef,-1), AtlTypeName(this));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::JoinFilterGraph (IFilterGraph *pGraph, LPCWSTR pName)
{
#ifdef	_DEBUG_FILTER
	LogMessage (_DEBUG_FILTER, _T("[%p(%d)] %s::JoinFilterGraph [%p] [%ls]"), this, max(m_dwRef,-1), AtlTypeName(this), pGraph, pName);
#endif
	HRESULT		lResult = S_OK;
	CMutexLock	lLock (mStateLock);

	try
	{
		if	(
				(pGraph)
			&&	(pName)
			)
		{
			if	(SUCCEEDED (LogVfwErr (LogNormal|LogTime, lResult = SetFilterName (pName))))
			{
				mFilterGraph = pGraph;
				OnJoinedFilterGraph ();
				lResult = S_OK;
			}
		}
		else
		{
			OnLeftFilterGraph ();
			mFilterGraph = NULL;
		}

#ifdef	_LOG_RESULTS
		if	(LogIsActive (_LOG_RESULTS))
		{
			LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::JoinFilterGraph"), this, max(m_dwRef,-1), AtlTypeName(this));
		}
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowFilter::QueryVendorInfo (LPWSTR *pVendorInfo)
{
#ifdef	_DEBUG_FILTER_EX
	LogMessage (_DEBUG_FILTER_EX, _T("[%p(%d)] %s::QueryVendorInfo"), this, max(m_dwRef,-1), AtlTypeName(this));
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pVendorInfo)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pVendorInfo) = AtlAllocTaskWideString (_T("Internal"));
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::QueryVendorInfo"), this, max(m_dwRef,-1), AtlTypeName(this));
	}
#endif
	return lResult;
}
