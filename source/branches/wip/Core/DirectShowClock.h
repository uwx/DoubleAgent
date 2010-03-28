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
#ifndef DIRECTSHOWCLOCK_H_INCLUDED_
#define DIRECTSHOWCLOCK_H_INCLUDED_
#pragma once

#include <strmif.h>
#include "InstanceGate.h"

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IReferenceClock, __uuidof(IReferenceClock));
static const REFERENCE_TIME sDefaultClockInterval = 50000;

class tHandleTypeRegisteredWait : public tHandleType <HANDLE>
{
private:
	virtual bool __Close (HANDLE & pHandle) {return UnregisterWait (pHandle) ? false : true;}
};
typedef tHandle <HANDLE, tHandleTypeRegisteredWait>	CRegisteredWaitHandle;

/////////////////////////////////////////////////////////////////////////////

class CDirectShowClock : private CInstanceGate
{
public:
	CDirectShowClock();
	virtual ~CDirectShowClock();

// Attributes
public:
	bool IsClockSet () const;
	bool IsClockStarted () const;

// Operations
public:
	REFERENCE_TIME GetStreamTime (FILTER_STATE pStreamState = State_Stopped);
	REFERENCE_TIME GetReferenceTime ();
	REFERENCE_TIME GetStartTime () const;

// Implementation
protected:
	void EndClock (); // Blocks until worker thread is done
	void SetStartTime (REFERENCE_TIME pTimeOffset);

	virtual HRESULT SetClock (REFERENCE_TIME pReferenceTime, REFERENCE_TIME pDelay);
	virtual HRESULT StartClock (REFERENCE_TIME pInterval);
	virtual HRESULT StopClock ();

	virtual void OnClockPulse () = 0;
private:
	static void CALLBACK ClockCallback (PVOID lpParameter, BOOLEAN TimerOrWaitFired);

protected:
	IReferenceClockPtr			mClock;
	REFERENCE_TIME				mTimeOffset;
private:
	mutable ::CCriticalSection	mClockCS;
	CSemaphore					mClockAdviseSemaphore;
	CEvent						mClockAdviseEvent;
	DWORD_PTR					mClockSemaphoreCookie;
	DWORD_PTR					mClockEventCookie;
	CRegisteredWaitHandle		mClockSemaphoreWaitHandle;
	CRegisteredWaitHandle		mClockEventWaitHandle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWCLOCK_H_INCLUDED_
