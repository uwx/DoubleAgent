#ifndef DIRECTSHOWDELAYECHO_H_INCLUDED_
#define DIRECTSHOWDELAYECHO_H_INCLUDED_
#pragma once

#include "DirectShowUtils.h"
#include "DirectShowClock.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowDelayEcho : public CCmdTarget, public CDirectShowUtils
{
public:
	CDirectShowDelayEcho (LPCTSTR pFilterName = NULL);
	virtual ~CDirectShowDelayEcho ();
	DECLARE_DYNAMIC(CDirectShowDelayEcho)

// Attributes
public:
	IBaseFilter * SafeGetFilter ();

// Operations
public:
	HRESULT ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	HRESULT ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	HRESULT Disconnect ();
	HRESULT Enable ();
	
	void AddCue (REFERENCE_TIME pStartTime);
	void ClearCues ();
	HRESULT CueDelay ();
	HRESULT CueEcho ();

// Overrides
	//{{AFX_VIRTUAL(CDirectShowDelayEcho)
	public:
	virtual void OnFinalRelease ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(Grabber, ISampleGrabberCB)
		virtual HRESULT STDMETHODCALLTYPE SampleCB (double SampleTime, IMediaSample *pSample);
		virtual HRESULT STDMETHODCALLTYPE BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen);
	END_INTERFACE_PART(Grabber)

	DECLARE_INTERFACE_MAP()

protected:
	HRESULT GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking);	
	bool IsEndOfStream (double pSampleTime, IMediaSample * pMediaSample);
	HRESULT DelaySample (double pSampleTime, IMediaSample * pMediaSample);

private:
	static void CALLBACK EchoCallback (PVOID lpParameter, BOOLEAN TimerOrWaitFired);

protected:
	CString						mFilterName;
	IBaseFilterPtr				mGrabberFilter;
	ISampleGrabberPtr			mGrabber;
	CArrayEx <REFERENCE_TIME>	mStartTimes;
	INT_PTR						mCueNdx;
	REFERENCE_TIME				mCueDuration;
	CCriticalSection			mStateLock;
	CCriticalSection			mDataLock;
private:
	CEvent						mEchoEvent;
	CRegisteredWaitHandle		mEchoWaitHandle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWDELAYECHO_H_INCLUDED_
