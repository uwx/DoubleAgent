/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "AtlCollEx.h"
#include "DirectShowUtils.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CDirectShowPin :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IPin
{
public:
	CDirectShowPin (PIN_DIRECTION pDirection = (PIN_DIRECTION)-1);
	virtual ~CDirectShowPin ();

// Attributes
public:
	CAtlString			mUniqueId;
	CAtlString			mName;
	const PIN_DIRECTION	mDirection;
	tMediaTypePtr		mMediaType;
	CMediaTypes			mMediaTypes;

	IPin * SafeGetConnection () const;
	IMemAllocator * SafeGetAllocator () const;
	bool SafeIsFlushing () const;
	bool SafeIsEndOfStream () const;

// Operations
public:
	CDirectShowPin & Initialize (class CDirectShowFilter & pFilter, PIN_DIRECTION pDirection, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL);
	void FinalRelease ();

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

// Interfaces
public:
	BEGIN_COM_MAP(CDirectShowPin)
		COM_INTERFACE_ENTRY(IPin)
		COM_INTERFACE_ENTRY_FUNC(__uuidof(IMediaSeeking), 0, &DelegateIMediaSeeking)
	END_COM_MAP()

public:
	// IPin
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

// Implementation
protected:
	static HRESULT WINAPI DelegateIMediaSeeking (void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw);

	virtual HRESULT CanConnect (IPin *pReceivePin);
	virtual HRESULT InternalConnect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT InternalReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT InternalDisconnect ();

protected:
	class CDirectShowFilter *	mFilter;
	mutable CAutoMutex			mStateLock;
	mutable CAutoMutex			mDataLock;
	IPinPtr						mConnection;
	IMemAllocatorPtr			mAllocator;
	bool						mIsFlushing;
	bool						mIsEndOfStream;
};

/////////////////////////////////////////////////////////////////////////////

typedef CAtlPtrTypeArray <CDirectShowPin> CDirectShowPins;

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CDirectShowPinIn : public CDirectShowPin, public IMemInputPin, public	IPinConnection
{
public:
	CDirectShowPinIn ();
	virtual ~CDirectShowPinIn ();

// Attributes
public:
	const UINT	mDesiredSampleCount;
	const UINT	mMaxSampleCount;

	bool IsInputReadOnly () const {return mReadOnlySamples;}
	INT_PTR GetCachedSampleCount () const;

// Operations
public:
	CDirectShowPinIn & Initialize (class CDirectShowFilter & pFilter, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL, UINT pDesiredSampleCount = 0, UINT pMaxSampleCount = 0);

// Overrides
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
	virtual HRESULT InternalReceiveConnection (IPin *pConnector, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT InternalDisconnect ();

// Interfaces
public:
	BEGIN_COM_MAP(CDirectShowPinIn)
		COM_INTERFACE_ENTRY(IMemInputPin)
		COM_INTERFACE_ENTRY(IPinConnection)
		COM_INTERFACE_ENTRY_CHAIN(CDirectShowPin)
	END_COM_MAP()

public:
	// IMemInputPin
	HRESULT STDMETHODCALLTYPE GetAllocator (IMemAllocator **ppAllocator);
	HRESULT STDMETHODCALLTYPE NotifyAllocator (IMemAllocator *pAllocator, BOOL bReadOnly);
	HRESULT STDMETHODCALLTYPE GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps);
	HRESULT STDMETHODCALLTYPE Receive (IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE ReceiveMultiple (IMediaSample **pSamples, long nSamples, long *nSamplesProcessed);
	HRESULT STDMETHODCALLTYPE ReceiveCanBlock (void);

	// IPinConnection
	HRESULT STDMETHODCALLTYPE DynamicQueryAccept (const AM_MEDIA_TYPE *pmt);
	HRESULT STDMETHODCALLTYPE NotifyEndOfStream (HANDLE hNotifyEvent);
	HRESULT STDMETHODCALLTYPE IsEndPin (void);
	HRESULT STDMETHODCALLTYPE DynamicDisconnect (void);

// Implementation
protected:
	HRESULT ProvideAllocator ();
	HRESULT ReceiveAllocator (IMemAllocator * pAllocator, bool pReadOnly);
	HRESULT PutInputSample (IMediaSample * pSample, DWORD pLockTime = INFINITE);

	void EmptyCache ();

protected:
	bool							mReadOnlySamples;
	CInterfaceArray <IMediaSample>	mSamples;
	IMemAllocatorPtr				mCacheAllocator;
	tPtr <CAutoSemaphore>			mMaxSampleSemaphore;
	HANDLE							mEosNotifyEvent;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CDirectShowPinOut : public CDirectShowPin
{
public:
	CDirectShowPinOut ();
	virtual ~CDirectShowPinOut ();

// Attributes
public:
	const UINT	mDesiredSampleCount;
	ULONG		mSampleSize;
	bool		mReadOnlySamples;

// Operations
public:
	CDirectShowPinOut & Initialize (class CDirectShowFilter & pFilter, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL, UINT pDesiredSampleCount = 0);

// Overrides
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
	virtual HRESULT InternalConnect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT InternalDisconnect ();

// Implementation
protected:
	HRESULT NegotiateAllocator (const AM_MEDIA_TYPE * pMediaType);
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CDirectShowPinPull : public CDirectShowPin, public IAsyncReader, public IPinFlowControl
{
public:
	CDirectShowPinPull ();
	virtual ~CDirectShowPinPull ();

// Attributes
public:

// Operations
public:
	CDirectShowPinPull & Initialize (class CDirectShowFilter & pFilter, LPCTSTR pName = NULL, LPCTSTR pUniqueId = NULL);
	HRESULT SetInputBuffer (LPCVOID pBuffer, ULONG pBufferSize, bool pCopyBuffer = false);

// Overrides
public:
	virtual HRESULT EndOutputStream ();
	virtual HRESULT BeginOutputFlush ();
	virtual HRESULT EndOutputFlush ();
protected:
	virtual HRESULT CanConnect (IPin *pReceivePin);
	virtual HRESULT InternalConnect (IPin *pReceivePin, const AM_MEDIA_TYPE * pMediaType);
	virtual HRESULT InternalDisconnect ();

// Interfaces
public:
	BEGIN_COM_MAP(CDirectShowPinPull)
		COM_INTERFACE_ENTRY(IAsyncReader)
		COM_INTERFACE_ENTRY(IPinFlowControl)
		COM_INTERFACE_ENTRY_CHAIN(CDirectShowPin)
	END_COM_MAP()

public:
	// IAsyncReader
	HRESULT STDMETHODCALLTYPE RequestAllocator (IMemAllocator *pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator **ppActual);
	HRESULT STDMETHODCALLTYPE Request (IMediaSample *pSample, DWORD_PTR dwUser);
	HRESULT STDMETHODCALLTYPE WaitForNext (DWORD dwTimeout, IMediaSample **ppSample,DWORD_PTR *pdwUser);
	HRESULT STDMETHODCALLTYPE SyncReadAligned (IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE SyncRead (LONGLONG llPosition, LONG lLength, BYTE *pBuffer);
	HRESULT STDMETHODCALLTYPE Length (LONGLONG *pTotal, LONGLONG *pAvailable);
	HRESULT STDMETHODCALLTYPE BeginFlush (void);
	HRESULT STDMETHODCALLTYPE EndFlush (void);

	// IPinFlowControl
	HRESULT STDMETHODCALLTYPE Block (DWORD dwBlockFlags, HANDLE hEvent);

// Implementation
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
	CAtlStructArray <PullRequest>	mRequests;
	CAutoEvent						mRequestEvent;
	LPCVOID							mInputBuffer;
	ULONG							mInputBufferSize;
	tArrayPtr <BYTE>				mInputBufferCopy;
	CComAutoCriticalSection			mBlockingLock;
	CAutoMutex						mBlockingMutex;
	HANDLE							mBlockingEvent;
};

/////////////////////////////////////////////////////////////////////////////
