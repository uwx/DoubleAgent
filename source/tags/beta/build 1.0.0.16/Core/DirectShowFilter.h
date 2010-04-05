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
#ifndef DIRECTSHOWFILTER_H_INCLUDED_
#define DIRECTSHOWFILTER_H_INCLUDED_
#pragma once

#include "DirectShowUtils.h"
#include "DirectShowPins.h"
#include "DirectShowClock.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowFilter : public CCmdTarget, public CDirectShowClock
{
public:
	CDirectShowFilter();
	virtual ~CDirectShowFilter();
	void Terminate ();
	DECLARE_DYNAMIC(CDirectShowFilter)

// Attributes
public:
	FILTER_STATE GetState () const {return mState;}
	IBaseFilter * GetFilter () const {return &const_cast <CDirectShowFilter *> (this)->m_xBaseFilter;}
	IFilterGraph * GetFilterGraph () const {return mFilterGraph;}

// Operations
public:
	virtual HRESULT Start (REFERENCE_TIME pStartOffset);
	virtual HRESULT Stop ();
	virtual HRESULT Pause ();

	void Flush ();
	virtual void BeginFlush ();
	virtual void EndFlush ();

// Overrides
	//{{AFX_VIRTUAL(CDirectShowFilter)
	public:
	virtual void OnFinalRelease ();
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(BaseFilter, IBaseFilter)
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
	END_INTERFACE_PART(BaseFilter)

	DECLARE_INTERFACE_MAP()

protected:
	virtual const GUID & GetClassID () = 0;
	virtual HRESULT SetFilterName (LPCWSTR pFilterName) = 0;
	virtual CString GetFilterName () = 0;
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
	mutable CMutex		mStateLock;
	mutable CMutex		mDataLock;
	IFilterGraph *		mFilterGraph;
	FILTER_STATE		mState;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWFILTER_H_INCLUDED_
