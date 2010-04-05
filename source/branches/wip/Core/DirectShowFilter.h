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
#pragma once
#include "DirectShowUtils.h"
#include "DirectShowPins.h"
#include "DirectShowClock.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CDirectShowFilter :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IBaseFilter,
	public CDirectShowClock
{
public:
	CDirectShowFilter();
	virtual ~CDirectShowFilter();

// Attributes
public:
	FILTER_STATE GetState () const {return mState;}
	IFilterGraph * GetFilterGraph () const {return mFilterGraph;}

// Operations
public:
	void Terminate ();
	void FinalRelease ();

	virtual HRESULT StartFilter (REFERENCE_TIME pStartOffset);
	virtual HRESULT StopFilter ();
	virtual HRESULT PauseFilter ();

	void Flush ();
	virtual void BeginFlush ();
	virtual void EndFlush ();

// Overrides

// Interfaces
public:
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(CDirectShowFilter)
		COM_INTERFACE_ENTRY(IBaseFilter)
	END_COM_MAP()

public:
	// IBaseFilter
	HRESULT STDMETHODCALLTYPE GetClassID (CLSID *pClassID);
    HRESULT STDMETHODCALLTYPE Stop (void);
    HRESULT STDMETHODCALLTYPE Pause (void);
    HRESULT STDMETHODCALLTYPE Run (REFERENCE_TIME tStart);
    HRESULT STDMETHODCALLTYPE GetState (DWORD dwMilliSecsTimeout, FILTER_STATE *State);
    HRESULT STDMETHODCALLTYPE SetSyncSource (IReferenceClock *pClock);
    HRESULT STDMETHODCALLTYPE GetSyncSource (IReferenceClock **pClock);
    HRESULT STDMETHODCALLTYPE EnumPins (IEnumPins **ppEnum);
    HRESULT STDMETHODCALLTYPE FindPin (LPCWSTR Id, IPin **ppPin);
    HRESULT STDMETHODCALLTYPE QueryFilterInfo (FILTER_INFO *pInfo);
    HRESULT STDMETHODCALLTYPE JoinFilterGraph (IFilterGraph *pGraph, LPCWSTR pName);
    HRESULT STDMETHODCALLTYPE QueryVendorInfo (LPWSTR *pVendorInfo);

// Implementation
protected:
	virtual const GUID & GetClassID () = 0;
	virtual HRESULT SetFilterName (LPCWSTR pFilterName) = 0;
	virtual CAtlString GetFilterName () = 0;
	virtual void InitializePins () = 0;

	virtual HRESULT StartPins ();
	virtual HRESULT StopPins ();
	virtual HRESULT StartInputPin (CDirectShowPin * pPin);
	virtual HRESULT StopInputPin (CDirectShowPin * pPin);
	virtual HRESULT StartOutputPin (CDirectShowPin * pPin);
	virtual HRESULT StopOutputPin (CDirectShowPin * pPin);

	virtual HRESULT StartOutputStreams ();
	virtual HRESULT StopOutputStreams ();
	virtual HRESULT StartOutputStream (CDirectShowPin * pPin);
	virtual HRESULT StopOutputStream (CDirectShowPin * pPin);

	virtual void GetSeekingTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime);
	virtual bool GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking);

public:
	virtual void OnPinConnected (class CDirectShowPin * pPin);
	virtual void OnPinDisconnected (class CDirectShowPin * pPin);
	virtual void OnStartInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate);
	virtual void OnEndInputStream (INT_PTR pPendingSamples);
	virtual void OnGotInputSample (class CDirectShowPin * pPin);
protected:
	virtual HRESULT OnStateChanging (FILTER_STATE pOldState, FILTER_STATE pNewState);
	virtual HRESULT OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState);
	virtual void OnJoinedFilterGraph ();
	virtual void OnLeftFilterGraph ();

public:
	CDirectShowPins		mInputPins;
	CDirectShowPins		mOutputPins;
protected:
	mutable CAutoMutex	mStateLock;
	mutable CAutoMutex	mDataLock;
	IFilterGraph *		mFilterGraph;
	FILTER_STATE		mState;
};

/////////////////////////////////////////////////////////////////////////////
