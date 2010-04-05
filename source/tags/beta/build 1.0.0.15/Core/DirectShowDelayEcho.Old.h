#ifndef DIRECTSHOWDELAYECHO_H_INCLUDED_
#define DIRECTSHOWDELAYECHO_H_INCLUDED_
#pragma once

#include "DirectShowFilter.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{E5337AED-33E6-47F7-853F-FA820F891F06}")) CDirectShowDelayEcho : public CDirectShowFilter, public CDirectShowUtils
{
public:
	CDirectShowDelayEcho (LPCTSTR pFilterName, IPin * pDurationSourcePin);
	virtual ~CDirectShowDelayEcho ();
	DECLARE_DYNAMIC(CDirectShowDelayEcho)

// Attributes
public:
	long GetCueCount () const {return mStartTimes.GetSize();}

// Operations
public:
	HRESULT ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	HRESULT ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	HRESULT Disconnect ();

	void AddCue (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime);

// Overrides
	//{{AFX_VIRTUAL(CDirectShowDelayEcho)
	public:
	virtual void OnFinalRelease ();
	virtual LONGLONG GetDuration ();
	protected:
	virtual const GUID & GetClassID ();
	virtual LRESULT SetFilterName (LPCWSTR pFilterName);
	virtual CString GetFilterName ();
	virtual void InitializePins ();
	virtual void BeginFlush ();
	virtual void OnStartInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate);
	virtual void OnEndInputStream (long pPendingSamples);
	virtual void OnGotInputSample (CDirectShowPin * pPin);
	virtual HRESULT SetClock (REFERENCE_TIME pReferenceTime, REFERENCE_TIME pDelay);
	virtual HRESULT StartClock (REFERENCE_TIME pInterval);
	virtual HRESULT StopClock ();
	virtual void OnClockPulse ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HRESULT CacheSample (IMediaSample * pSample);
	HRESULT DuplicateSample (IMediaSample * pSample, IMediaSample ** pDuplicate);
	HRESULT CopySample (IMediaSample * pSample, IMediaSample * pCopy);

protected:
	CString										mFilterName;
	IPinPtr										mDurationSourcePin;
	tPtr <CDirectShowPinIn>						mInputPin;
	tPtr <CDirectShowPinOut>					mOutputPin;
	CArrayEx <REFERENCE_TIME>					mStartTimes;
	CArrayEx <REFERENCE_TIME>					mEndTimes;
	CArrayEx <IMediaSamplePtr, IMediaSample *>	mSampleCache;
	IMemAllocatorPtr							mCacheAllocator;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWDELAYECHO_H_INCLUDED_
