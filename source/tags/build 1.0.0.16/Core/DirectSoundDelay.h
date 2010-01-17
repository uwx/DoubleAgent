#ifndef DIRECTSOUNDDELAY_H_INCLUDED_
#define DIRECTSOUNDDELAY_H_INCLUDED_
#pragma once

#include "DirectShowUtils.h"
#include "DirectShowClock.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectSoundDelay : public CCmdTarget, public CDirectShowClock, public CDirectShowUtils
{
public:
	CDirectSoundDelay (LPCTSTR pFilterName = NULL);
	virtual ~CDirectSoundDelay ();
	DECLARE_DYNAMIC(CDirectSoundDelay)

// Attributes
public:
	IBaseFilter * SafeGetFilter ();

// Operations
public:
	HRESULT ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	HRESULT ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	HRESULT Disconnect ();
	HRESULT Enable (bool pEnable);
	
	HRESULT AddSoundCue (REFERENCE_TIME pStartTime);
	void ClearSoundCues ();
	INT_PTR GetSoundCueCount () const;

// Overrides
	//{{AFX_VIRTUAL(CDirectSoundDelay)
	public:
	virtual void OnFinalRelease ();
	virtual void OnClockPulse ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(Grabber, ISampleGrabberCB)
		virtual HRESULT STDMETHODCALLTYPE SampleCB (double SampleTime, IMediaSample *pSample);
		virtual HRESULT STDMETHODCALLTYPE BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen);
	END_INTERFACE_PART(Grabber)

	DECLARE_INTERFACE_MAP()

protected:
	HRESULT DelaySample (double pSampleTime, IMediaSample * pMediaSample);
	HRESULT CacheSample (IMediaSample * pMediaSample);
	HRESULT RepeatSamples ();

protected:
	CString										mFilterName;
	IBaseFilterPtr								mGrabberFilter;
	ISampleGrabberPtr							mGrabber;
	CArrayEx <REFERENCE_TIME>					mCueTimes;
	INT_PTR										mCueNdx;
	REFERENCE_TIME								mCueDuration;
	CArrayEx <IMediaSamplePtr, IMediaSample *>	mSampleCache;
	IMemAllocatorPtr							mSampleCacheAllocator;
	mutable CCriticalSection					mStateLock;
	mutable CCriticalSection					mDataLock;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSOUNDDELAY_H_INCLUDED_
