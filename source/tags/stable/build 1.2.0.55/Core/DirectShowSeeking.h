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
#pragma once
#include "DirectShowClock.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowSeekingImpl : public IMediaSeeking
{
protected:
	CDirectShowSeekingImpl ();
	CDirectShowSeekingImpl& Initialize (CComObjectRootBase& pBase, CDirectShowClock& pClock, LPCTSTR pBaseClassName = NULL);
public:
	virtual ~CDirectShowSeekingImpl ();

// Attributes
public:
	virtual LONGLONG GetDuration ();
	virtual LONGLONG GetPreroll ();

// Operations
public:
	HRESULT SetTimes (REFERENCE_TIME pCurrTime, REFERENCE_TIME pStopTime, REFERENCE_TIME pDuration = 0);
	void GetTimes (REFERENCE_TIME& pCurrTime, REFERENCE_TIME& pStopTime);
	void GetStreamTimes (REFERENCE_TIME& pStreamTime, REFERENCE_TIME& pCurrTime, REFERENCE_TIME& pStopTime, FILTER_STATE pStreamState = State_Stopped);

	REFERENCE_TIME GetCurrTime (FILTER_STATE pStreamState = State_Stopped);
	REFERENCE_TIME GetStopTime (FILTER_STATE pStreamState = State_Stopped);

// Interfaces
public:
	// IMediaSeeking
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

// Implementation
protected:
	virtual bool GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking);
	virtual void OnTimesChanging (REFERENCE_TIME& pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME& pStopTime, DWORD pStopFlags);
	virtual void OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags);

protected:
	DWORD							mSeekingCaps;
	REFERENCE_TIME					mCurrTime;
	REFERENCE_TIME					mStopTime;
	CDirectShowClock *				mClock;
private:
	mutable CComAutoCriticalSection	mLock;
	CComObjectRootBase *			mBase;
	CAtlString						mBaseClassName;
};

/////////////////////////////////////////////////////////////////////////////

template <class BASE> class CDirectShowSeeking : public CDirectShowSeekingImpl
{
public:
	CDirectShowSeeking () {}
	~CDirectShowSeeking () {}

protected:
	void InitMediaSeeking (BASE& pBase, CDirectShowClock& pClock, DWORD pRemoveSeekingCaps = 0, DWORD pAddSeekingCaps = 0)
	{
		CDirectShowSeekingImpl::Initialize (pBase, pClock, CAtlString(typeid(BASE).name()).Mid(6));
		mSeekingCaps &= ~pRemoveSeekingCaps;
		mSeekingCaps |= pAddSeekingCaps;
	}
};

/////////////////////////////////////////////////////////////////////////////
