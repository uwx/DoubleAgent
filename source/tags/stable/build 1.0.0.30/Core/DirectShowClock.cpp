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
#include "DirectShowClock.h"
#include "DirectShowUtils.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_CLOCK		LogNormal|LogHighVolume|LogTimeMs
//#define	_TRACE_CLOCK		LogNormal|LogHighVolume|LogTimeMs
//#define	_TRACE_RESOURCES	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

/////////////////////////////////////////////////////////////////////////////

CDirectShowClock::CDirectShowClock()
:	mTimeOffset (0),
	mClockSemaphoreCookie (0),
	mClockEventCookie (0)
{
}

CDirectShowClock::~CDirectShowClock()
{
	EndClock ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

REFERENCE_TIME CDirectShowClock::GetStreamTime (FILTER_STATE pStreamState)
{
	REFERENCE_TIME	lRet = 0;
	CSingleLock		lLock (&mClockCS, TRUE);

	try
	{
		REFERENCE_TIME	lReferenceTime = 0;

		if	(pStreamState == State_Running)
		{
			lReferenceTime = GetReferenceTime();
		}
		if	(pStreamState != State_Stopped)
		{
			lRet += lReferenceTime - mTimeOffset;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

REFERENCE_TIME CDirectShowClock::GetReferenceTime ()
{
	REFERENCE_TIME	lRet = 0;
	CSingleLock		lLock (&mClockCS, TRUE);

	try
	{
		REFERENCE_TIME	lClockTime;

		if	(
				(mClock != NULL)
			&&	(SUCCEEDED (mClock->GetTime (&lClockTime)))
			)
		{
			lRet = lClockTime;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowClock::SetStartTime (REFERENCE_TIME pTimeOffset)
{
	CSingleLock	lLock (&mClockCS, TRUE);

	mTimeOffset = pTimeOffset;
}

REFERENCE_TIME CDirectShowClock::GetStartTime () const
{
	CSingleLock	lLock (&mClockCS, TRUE);
	return mTimeOffset;
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectShowClock::IsClockSet () const
{
	bool		lRet = false;
	CSingleLock	lLock (&mClockCS, TRUE);

	try
	{
		if	(
				(mClockEventCookie)
			&&	(mClockEventWaitHandle.SafeIsValid())
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

bool CDirectShowClock::IsClockStarted () const
{
	bool		lRet = false;
	CSingleLock	lLock (&mClockCS, TRUE);

	try
	{
		if	(
				(mClockSemaphoreCookie)
			&&	(mClockSemaphoreWaitHandle.SafeIsValid())
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowClock::SetClock (REFERENCE_TIME pReferenceTime, REFERENCE_TIME pDelay)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mClockCS, TRUE);

	try
	{
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowClock::SetClock"), this);
#endif
		if	(mClock != NULL)
		{
			if	(mClockSemaphoreCookie)
			{
				LogVfwErr (LogNormal, mClock->Unadvise (mClockSemaphoreCookie));
				mClockSemaphoreCookie = 0;
			}

			if	(mClockEventCookie)
			{
				LogVfwErr (LogNormal, mClock->Unadvise (mClockEventCookie));
				mClockEventCookie = 0;
			}

			mClockAdviseEvent.ResetEvent ();

			if	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mClock->AdviseTime (pReferenceTime, pDelay, (HEVENT)(HANDLE)mClockAdviseEvent, &mClockEventCookie))))
			{
#ifdef	_DEBUG_CLOCK
				LogMessage (_DEBUG_CLOCK, _T("[%p] SyncAdvise [%f + %f = %f] started [%u] at [%f]"), this, RefTimeSec(pReferenceTime), RefTimeSec(pDelay), RefTimeSec(pReferenceTime+pDelay), mClockEventCookie, RefTimeSec(GetReferenceTime()));
#endif
			}
		}

		if	(
				(mClockEventCookie)
			&&	(!mClockEventWaitHandle.SafeIsValid())
			)
		{
			if	(RegisterWaitForSingleObject (&mClockEventWaitHandle, mClockAdviseEvent, ClockCallback, PutGatedInstance<CDirectShowClock> (this), INFINITE, WT_EXECUTELONGFUNCTION))
			{
#ifdef	_DEBUG_CLOCK
				LogMessage (_DEBUG_CLOCK, _T("[%p] WaitForEvent [%p] started"), this, (HANDLE)mClockEventWaitHandle);
#endif
			}
			else
			{
				lResult = HRESULT_FROM_WIN32 (GetLastError());
				LogWinErr (LogAlways, GetLastError(), _T("WaitForEvent"));
			}
		}
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowClock::SetClock Done"), this);
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT CDirectShowClock::StartClock (REFERENCE_TIME pInterval)
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mClockCS, TRUE);

	try
	{
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowClock::StartClock"), this);
#endif
		REFERENCE_TIME	lClockTime;

		if	(mClock != NULL)
		{
			if	(mClockSemaphoreCookie)
			{
				LogVfwErr (LogNormal, mClock->Unadvise (mClockSemaphoreCookie));
				mClockSemaphoreCookie = 0;
			}

			if	(mClockEventCookie)
			{
				LogVfwErr (LogNormal, mClock->Unadvise (mClockEventCookie));
				mClockEventCookie = 0;
			}

			if	(
					(SUCCEEDED (lResult = LogVfwErr (LogNormal, mClock->GetTime (&lClockTime))))
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mClock->AdvisePeriodic (lClockTime, pInterval, (HSEMAPHORE)(HANDLE)mClockAdviseSemaphore, &mClockSemaphoreCookie))))
				)
			{
#ifdef	_DEBUG_CLOCK
				LogMessage (_DEBUG_CLOCK, _T("[%p] SyncAdvisePeriodic [%f] started [%u] at [%f]"), this, RefTimeSec(pInterval), mClockSemaphoreCookie, RefTimeSec(lClockTime));
#endif
			}
		}

		if	(
				(mClockSemaphoreCookie)
			&&	(!mClockSemaphoreWaitHandle.SafeIsValid())
			)
		{
			if	(RegisterWaitForSingleObject (&mClockSemaphoreWaitHandle, mClockAdviseSemaphore, ClockCallback, PutGatedInstance<CDirectShowClock> (this), INFINITE, WT_EXECUTELONGFUNCTION))
			{
#ifdef	_DEBUG_CLOCK
				LogMessage (_DEBUG_CLOCK, _T("[%p] WaitForSemaphore [%p] started"), this, (HANDLE)mClockSemaphoreWaitHandle);
#endif
			}
			else
			{
				lResult = HRESULT_FROM_WIN32 (GetLastError());
				if	(LogIsActive ())
				{
					LogWinErr (LogAlways, GetLastError(), _T("WaitForSemaphore"));
				}
			}
		}
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowClock::StartClock Done"), this);
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}


HRESULT CDirectShowClock::StopClock ()
{
	HRESULT		lResult = S_FALSE;
	CSingleLock	lLock (&mClockCS, TRUE);

	try
	{
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowClock::StopClock"), this);
#endif
		if	(mClockSemaphoreWaitHandle.SafeIsValid())
		{
#ifdef	_DEBUG_CLOCK
			LogMessage (_DEBUG_CLOCK, _T("[%p] WaitForSemaphore [%p] stopped"), this, (HANDLE)mClockSemaphoreWaitHandle);
#endif
		}
		mClockSemaphoreWaitHandle.Close ();

		if	(mClockEventWaitHandle.SafeIsValid())
		{
#ifdef	_DEBUG_CLOCK
			LogMessage (_DEBUG_CLOCK, _T("[%p] WaitForEvent [%p] stopped"), this, (HANDLE)mClockEventWaitHandle);
#endif
		}
		mClockEventWaitHandle.Close ();

		if	(mClock != NULL)
		{
			if	(
					(mClockSemaphoreCookie)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mClock->Unadvise (mClockSemaphoreCookie))))
				)
			{
#ifdef	_DEBUG_CLOCK
				LogMessage (_DEBUG_CLOCK, _T("[%p] SyncAdvisePeriodic [%u] stopped"), this, mClockSemaphoreCookie);
#endif
			}
			mClockSemaphoreCookie = 0;

			if	(
					(mClockEventCookie)
				&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, mClock->Unadvise (mClockEventCookie))))
				)
			{
#ifdef	_DEBUG_CLOCK
				LogMessage (_DEBUG_CLOCK, _T("[%p] SyncAdvise [%u] stopped"), this, mClockEventCookie);
#endif
			}
			mClockEventCookie = 0;
		}
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectShowClock::StopClock Done"), this);
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

void CDirectShowClock::EndClock ()
{
	StopClock ();
	NotGatedInstance<CDirectShowClock> (this);
	SafeFreeSafePtr (mClock);
}

/////////////////////////////////////////////////////////////////////////////

void CALLBACK CDirectShowClock::ClockCallback (PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	if	(!TimerOrWaitFired)
	{
		CDirectShowClock *	lThis = NULL;

		if	(LockGatedInstance<CDirectShowClock> (lpParameter, lThis, 20))
		{
			try
			{
#ifdef	_TRACE_CLOCK
				LogMessage (_TRACE_CLOCK, _T("[%p] [%f] ClockCallback"), lThis, RefTimeSec(lThis->GetReferenceTime()));
#endif
				lThis->OnClockPulse ();
			}
			catch AnyExceptionSilent

			FreeGatedInstance<CDirectShowClock> (lpParameter, lThis);
		}
	}
#ifdef	_TRACE_CLOCK
	else
	{
		LogMessage (_TRACE_CLOCK, _T("ClockCallback [%d] skipped"), TimerOrWaitFired);
	}
#endif
}
