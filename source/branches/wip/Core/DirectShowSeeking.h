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
#ifndef DIRECTSHOWSEEKING_H_INCLUDED_
#define DIRECTSHOWSEEKING_H_INCLUDED_
#pragma once

#include "DirectShowClock.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowSeeking
{
public:
	CDirectShowSeeking (CCmdTarget & pBase, CDirectShowClock & pClock);
	virtual ~CDirectShowSeeking ();

// Attributes
public:
	virtual LONGLONG GetDuration ();
	virtual LONGLONG GetPreroll ();

// Operations
public:
	HRESULT SetTimes (REFERENCE_TIME pCurrTime, REFERENCE_TIME pStopTime, REFERENCE_TIME pDuration = 0);
	void GetTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime);
	void GetStreamTimes (REFERENCE_TIME & pStreamTime, REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime, FILTER_STATE pStreamState = State_Stopped);

	REFERENCE_TIME GetCurrTime (FILTER_STATE pStreamState = State_Stopped);
	REFERENCE_TIME GetStopTime (FILTER_STATE pStreamState = State_Stopped);

// Implementation
protected:
	virtual HRESULT SetPositions (LONGLONG * pCurrent, DWORD pCurrentFlags, LONGLONG * pStop, DWORD pStopFlags);
	virtual bool GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking);
	virtual void OnTimesChanging (REFERENCE_TIME & pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME & pStopTime, DWORD pStopFlags);
	virtual void OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags);

	BEGIN_INTERFACE_PART(MediaSeeking, IMediaSeeking)
		HRESULT STDMETHODCALLTYPE GetCapabilities (DWORD *pCapabilities);
		HRESULT STDMETHODCALLTYPE CheckCapabilities (DWORD *pCapabilities);
		HRESULT STDMETHODCALLTYPE IsFormatSupported (const GUID *pFormat);
		HRESULT STDMETHODCALLTYPE QueryPreferredFormat (GUID *pFormat);
		HRESULT STDMETHODCALLTYPE GetTimeFormat (GUID *pFormat);
		HRESULT STDMETHODCALLTYPE IsUsingTimeFormat (const GUID *pFormat);
		HRESULT STDMETHODCALLTYPE SetTimeFormat (const GUID *pFormat);
		HRESULT STDMETHODCALLTYPE GetDuration (LONGLONG *pDuration);
		HRESULT STDMETHODCALLTYPE GetStopPosition (LONGLONG *pStop);
		HRESULT STDMETHODCALLTYPE GetCurrentPosition (LONGLONG *pCurrent);
		HRESULT STDMETHODCALLTYPE ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat);
		HRESULT STDMETHODCALLTYPE SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags);
		HRESULT STDMETHODCALLTYPE GetPositions (LONGLONG *pCurrent, LONGLONG *pStop);
		HRESULT STDMETHODCALLTYPE GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest);
		HRESULT STDMETHODCALLTYPE SetRate (double dRate);
		HRESULT STDMETHODCALLTYPE GetRate (double *pdRate);
		HRESULT STDMETHODCALLTYPE GetPreroll (LONGLONG *pllPreroll);
	END_INTERFACE_PART(MediaSeeking)

protected:
	DWORD						mSeekingCaps;
	REFERENCE_TIME				mCurrTime;
	REFERENCE_TIME				mStopTime;
	CDirectShowClock &			mClock;
private:
	mutable CCriticalSection	mLock;
	CCmdTarget &				mBase;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWSEEKING_H_INCLUDED_
