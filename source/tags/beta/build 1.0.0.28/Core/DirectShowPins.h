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
#ifndef DIRECTSHOWPINS_H_INCLUDED_
#define DIRECTSHOWPINS_H_INCLUDED_
#pragma once

#include "AfxTemplEx.h"
#include "DirectShowUtils.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowPin : public CCmdTarget
{
public:
	CDirectShowPin (class CDirectShowFilter & pFilter, PIN_DIRECTION pDirection, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL);
	virtual ~CDirectShowPin ();
	DECLARE_DYNAMIC(CDirectShowPin)

// Attributes
public:
	CString				mUniqueId;
	CString				mName;
	const PIN_DIRECTION	mDirection;
	tMediaTypePtr		mMediaType;
	CMediaTypes			mMediaTypes;

	IPin * GetInterface () {return &m_xPin;}
	IPin * SafeGetConnection () const;
	IMemAllocator * SafeGetAllocator () const;
	bool SafeIsFlushing () const;
	bool SafeIsEndOfStream () const;

// Operations
public:
	virtual HRESULT BeginOutput ();
	virtual HRESULT EndOutput ();
	virtual HRESULT GetOutputSample (IMediaSample ** pSample, const REFERENCE_TIME * pStartTime = NULL, const REFERENCE_TIME * pEndTime = NULL, DWORD pFlags = AM_GBF_NOWAIT);
	virtual HRESULT PutOutputSample (IMediaSample * pSample, ULONG pSampleSize = 0);
	virtual HRESULT BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);
	virtual HRESULT EndOutputStream ();
	virtual HRESULT BeginOutputFlush ();
	virtual HRESULT EndOutputFlush ();

	virtual HRESULT BeginInput ();
	virtual HRESULT EndInput ();
	virtual HRESULT PeekInputSample (IMediaSample ** pSample, DWORD pLockTime = 100);
	virtual HRESULT GetInputSample (IMediaSample ** pSample, DWORD pLockTime = 100);
	virtual HRESULT BeginInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);
	virtual HRESULT EndInputStream ();
	virtual HRESULT BeginInputFlush ();
	virtual HRESULT EndInputFlush ();

// Overrides
	//{{AFX_VIRTUAL(CDirectShowPin)
	public:
	virtual void OnFinalRelease ();
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(Pin, IPin)
		HRESULT STDMETHODCALLTYPE Connect (IPin *pReceivePin, const AM_MEDIA_TYPE *pmt);
		HRESULT STDMETHODCALLTYPE ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE *pmt);
		HRESULT STDMETHODCALLTYPE Disconnect (void);
		HRESULT STDMETHODCALLTYPE ConnectedTo (IPin **pPin);
		HRESULT STDMETHODCALLTYPE ConnectionMediaType (AM_MEDIA_TYPE *pmt);
		HRESULT STDMETHODCALLTYPE QueryPinInfo (PIN_INFO *pInfo);
		HRESULT STDMETHODCALLTYPE QueryDirection (PIN_DIRECTION *pPinDir);
		HRESULT STDMETHODCALLTYPE QueryId (LPWSTR *Id);
		HRESULT STDMETHODCALLTYPE QueryAccept (const AM_MEDIA_TYPE *pmt);
		HRESULT STDMETHODCALLTYPE EnumMediaTypes (IEnumMediaTypes **ppEnum);
		HRESULT STDMETHODCALLTYPE QueryInternalConnections (IPin **apPin, ULONG *nPin);
		HRESULT STDMETHODCALLTYPE EndOfStream (void);
		HRESULT STDMETHODCALLTYPE BeginFlush (void);
		HRESULT STDMETHODCALLTYPE EndFlush (void);
		HRESULT STDMETHODCALLTYPE NewSegment (REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
	END_INTERFACE_PART(Pin)

	DECLARE_INTERFACE_MAP()

protected:
	virtual HRESULT CanConnect (IPin *pReceivePin);
	virtual HRESULT Connect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT Disconnect ();

protected:
	class CDirectShowFilter &	mFilter;
	mutable CMutex				mStateLock;
	mutable CMutex				mDataLock;
	IPinPtr						mConnection;
	IMemAllocatorPtr			mAllocator;
	bool						mIsFlushing;
	bool						mIsEndOfStream;
};

/////////////////////////////////////////////////////////////////////////////

typedef CPtrTypeArray <CDirectShowPin> CDirectShowPins;

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CDirectShowPinIn : public CDirectShowPin
{
public:
	CDirectShowPinIn (class CDirectShowFilter & pFilter, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL, UINT pDesiredSampleCount = 0, UINT pMaxSampleCount = 0);
	virtual ~CDirectShowPinIn ();
	DECLARE_DYNAMIC(CDirectShowPinIn)

// Attributes
public:
	const UINT	mDesiredSampleCount;
	const UINT	mMaxSampleCount;

	bool IsInputReadOnly () const {return mReadOnlySamples;}
	INT_PTR GetCachedSampleCount () const;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDirectShowPinIn)
	public:
	virtual HRESULT BeginInput ();
	virtual HRESULT EndInput ();
	virtual HRESULT PeekInputSample (IMediaSample ** pSample, DWORD pLockTime = 100);
	virtual HRESULT GetInputSample (IMediaSample ** pSample, DWORD pLockTime = 100);
	virtual HRESULT BeginInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);
	virtual HRESULT EndInputStream ();
	virtual HRESULT BeginInputFlush ();
	virtual HRESULT EndInputFlush ();
	protected:
	virtual HRESULT ReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT Disconnect ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(MemInput, IMemInputPin)
		HRESULT STDMETHODCALLTYPE GetAllocator (IMemAllocator **ppAllocator);
		HRESULT STDMETHODCALLTYPE NotifyAllocator (IMemAllocator *pAllocator, BOOL bReadOnly);
		HRESULT STDMETHODCALLTYPE GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps);
		HRESULT STDMETHODCALLTYPE Receive (IMediaSample *pSample);
		HRESULT STDMETHODCALLTYPE ReceiveMultiple (IMediaSample **pSamples, long nSamples, long *nSamplesProcessed);
		HRESULT STDMETHODCALLTYPE ReceiveCanBlock (void);
	END_INTERFACE_PART(MemInput)

	BEGIN_INTERFACE_PART(PinConnection, IPinConnection)
		HRESULT STDMETHODCALLTYPE DynamicQueryAccept (const AM_MEDIA_TYPE *pmt);
		HRESULT STDMETHODCALLTYPE NotifyEndOfStream (HANDLE hNotifyEvent);
		HRESULT STDMETHODCALLTYPE IsEndPin (void);
		HRESULT STDMETHODCALLTYPE DynamicDisconnect (void);
	END_INTERFACE_PART(PinConnection)

	DECLARE_INTERFACE_MAP()

protected:
	HRESULT ProvideAllocator ();
	HRESULT ReceiveAllocator (IMemAllocator * pAllocator, bool pReadOnly);
	HRESULT PutInputSample (IMediaSample * pSample, DWORD pLockTime = INFINITE);

	void EmptyCache ();

protected:
	bool										mReadOnlySamples;
	CArrayEx <IMediaSamplePtr, IMediaSample *>	mSamples;
	IMemAllocatorPtr							mCacheAllocator;
	tPtr <CSemaphore>							mMaxSampleSemaphore;
	HANDLE										mEosNotifyEvent;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CDirectShowPinOut : public CDirectShowPin
{
public:
	CDirectShowPinOut (class CDirectShowFilter & pFilter, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL, UINT pDesiredSampleCount = 0);
	virtual ~CDirectShowPinOut ();
	DECLARE_DYNAMIC(CDirectShowPinOut)

// Attributes
public:
	const UINT	mDesiredSampleCount;
	ULONG		mSampleSize;
	bool		mReadOnlySamples;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDirectShowPinOut)
	public:
	virtual HRESULT BeginOutput ();
	virtual HRESULT EndOutput ();
	virtual HRESULT GetOutputSample (IMediaSample ** pSample, const REFERENCE_TIME * pStartTime = NULL, const REFERENCE_TIME * pEndTime = NULL, DWORD pFlags = AM_GBF_NOWAIT);
	virtual HRESULT PutOutputSample (IMediaSample * pSample, ULONG pSampleSize = 0);
	virtual HRESULT BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);
	virtual HRESULT EndOutputStream ();
	virtual HRESULT BeginOutputFlush ();
	virtual HRESULT EndOutputFlush ();
	protected:
	virtual HRESULT CanConnect (IPin *pReceivePin);
	virtual HRESULT Connect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT Disconnect ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HRESULT NegotiateAllocator (const AM_MEDIA_TYPE * pMediaType);
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CDirectShowPinPull : public CDirectShowPin
{
public:
	CDirectShowPinPull (class CDirectShowFilter & pFilter, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL);
	virtual ~CDirectShowPinPull ();
	DECLARE_DYNAMIC(CDirectShowPinPull)

// Attributes
public:

// Operations
public:
	HRESULT SetInputBuffer (LPCVOID pBuffer, ULONG pBufferSize, bool pCopyBuffer = false);

// Overrides
	//{{AFX_VIRTUAL(CDirectShowPinPull)
	public:
	virtual HRESULT EndOutputStream ();
	virtual HRESULT BeginOutputFlush ();
	virtual HRESULT EndOutputFlush ();
	protected:
	virtual HRESULT CanConnect (IPin *pReceivePin);
	virtual HRESULT Connect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT Disconnect ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(AsyncReader, IAsyncReader)
		HRESULT STDMETHODCALLTYPE RequestAllocator (IMemAllocator *pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator **ppActual);
		HRESULT STDMETHODCALLTYPE Request (IMediaSample *pSample, DWORD_PTR dwUser);
		HRESULT STDMETHODCALLTYPE WaitForNext (DWORD dwTimeout, IMediaSample **ppSample,DWORD_PTR *pdwUser);
		HRESULT STDMETHODCALLTYPE SyncReadAligned (IMediaSample *pSample);
		HRESULT STDMETHODCALLTYPE SyncRead (LONGLONG llPosition, LONG lLength, BYTE *pBuffer);
		HRESULT STDMETHODCALLTYPE Length (LONGLONG *pTotal, LONGLONG *pAvailable);
		HRESULT STDMETHODCALLTYPE BeginFlush (void);
		HRESULT STDMETHODCALLTYPE EndFlush (void);
	END_INTERFACE_PART(AsyncReader)

	BEGIN_INTERFACE_PART(FlowControl, IPinFlowControl)
		HRESULT STDMETHODCALLTYPE Block (DWORD dwBlockFlags, HANDLE hEvent);
	END_INTERFACE_PART(FlowControl)

	DECLARE_INTERFACE_MAP()

protected:
	HRESULT UseAllocator (IMemAllocator * pAllocator);
	HRESULT GetAllocator (const ALLOCATOR_PROPERTIES * pRequest = NULL);
	HRESULT FillSample (IMediaSample * pSample);
	HRESULT FillBuffer (LPBYTE pBuffer, LONGLONG pStart, long & pLength);
	HRESULT ValidateSampleAlign (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime);
	HRESULT ValidateSampleAlign (IMediaSample *pSample);

	virtual HRESULT BeginPullFlush ();
	virtual HRESULT EndPullFlush ();
	virtual HRESULT EndPullStream ();

	HRESULT BlockStream (bool pBlock, HANDLE pBlockingEvent = NULL);
	bool BlockStreamAsync ();
	bool IsStreamBlocked ();

protected:
	struct PullRequest
	{
		IMediaSample *	mSample;
		DWORD_PTR		mUserVal;
	};

protected:
	CArrayEx <PullRequest, PullRequest &>	mRequests;
	CEvent									mRequestEvent;
	LPCVOID									mInputBuffer;
	ULONG									mInputBufferSize;
	tArrayPtr <BYTE>						mInputBufferCopy;
	CCriticalSection						mBlockingLock;
	CMutex									mBlockingMutex;
	HANDLE									mBlockingEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWPINS_H_INCLUDED_
