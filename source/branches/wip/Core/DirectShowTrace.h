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

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CTraceSamples :
	public CComObjectRootEx<CComMultiThreadModel>,
	public ISampleGrabberCB,
	public CDirectShowUtils
{
public:
	CTraceSamples ();
	virtual ~CTraceSamples ();

// Attributes
public:
	IBaseFilter * GetFilter () {return mGrabberFilter;}

// Operations
public:
	CTraceSamples & Initialize (LPCTSTR pFilterName);
	void FinalRelease ();

	CTraceSamples & ConnectBefore (IFilterGraph * pFilterGraph, IBaseFilter * pDownstreamFilter, IPin * pDownstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	CTraceSamples & ConnectAfter (IFilterGraph * pFilterGraph, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	CTraceSamples & Disconnect ();

	CTraceSamples & LogSamples (UINT pLogLevel = LogIfActive|LogTimeMs|LogHighVolume, bool pOneShot = false);
	CTraceSamples & DumpSamples (UINT pByteCount = 64, UINT pLogLevel = LogIfActive|LogTimeMs|LogHighVolume, bool pOneShot = false);

// Overrides

// Interfaces
public:
	DECLARE_GET_CONTROLLING_UNKNOWN()
	BEGIN_COM_MAP(CTraceSamples)
		COM_INTERFACE_ENTRY(ISampleGrabberCB)
	END_COM_MAP()

public:
	// ISampleGrabberCB
	virtual HRESULT STDMETHODCALLTYPE SampleCB (double SampleTime, IMediaSample *pSample);
	virtual HRESULT STDMETHODCALLTYPE BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen);

// Implementation
protected:
	CAtlString			mFilterName;
	IBaseFilterPtr		mGrabberFilter;
	ISampleGrabberPtr	mGrabber;
	UINT				mLogLevelSample;
	UINT				mLogLevelDump;
	UINT				mDumpSize;
};

/////////////////////////////////////////////////////////////////////////////

interface __declspec(uuid("{1147E5C0-A208-11DE-ABF2-002421116FB2}")) _ITraceFilter : public IUnknown {};
interface __declspec(uuid("{1147E5C1-A208-11DE-ABF2-002421116FB2}")) _ITracePins : public IUnknown {};
interface __declspec(uuid("{1147E5C2-A208-11DE-ABF2-002421116FB2}")) _ITracePin : public IUnknown {};

_COM_SMARTPTR_TYPEDEF (_ITraceFilter, __uuidof(_ITraceFilter));
_COM_SMARTPTR_TYPEDEF (_ITracePins, __uuidof(_ITracePins));
_COM_SMARTPTR_TYPEDEF (_ITracePin, __uuidof(_ITracePin));

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CTraceFilter :
	public CComObjectRootEx<CComMultiThreadModel>,
	public _ITraceFilter,
	public IBaseFilter,
	public IMediaSeeking,
	public IFileSourceFilter,
	public IAMFilterMiscFlags,
	public ISampleGrabber
{
public:
	CTraceFilter ();
	virtual ~CTraceFilter ();

// Attributes
public:
	UINT	mLogLevelPins;

// Operations
public:
	CTraceFilter & Initialize (LPUNKNOWN pUnknown, LPCTSTR pFilterName = NULL, UINT pLogLevelPins = LogVerbose|LogTime);
	void FinalRelease ();
	void Aggregate (LPUNKNOWN pUnknown);

// Overrides

// Interfaces
public:
	DECLARE_GET_CONTROLLING_UNKNOWN()
	BEGIN_COM_MAP(CTraceFilter)
		COM_INTERFACE_ENTRY(_ITraceFilter)
		COM_INTERFACE_ENTRY_FUNC_BLIND(0, CheckInternalInterfaces)
		COM_INTERFACE_ENTRY(IBaseFilter)
		COM_INTERFACE_ENTRY(IMediaSeeking)
		COM_INTERFACE_ENTRY(IFileSourceFilter)
		COM_INTERFACE_ENTRY(IAMFilterMiscFlags)
		COM_INTERFACE_ENTRY(ISampleGrabber)
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

	// IFileSourceFilter
	HRESULT STDMETHODCALLTYPE Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt);
	HRESULT STDMETHODCALLTYPE GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt);

	// IAMFilterMiscFlags
    ULONG STDMETHODCALLTYPE GetMiscFlags (void);

	// ISampleGrabber
	HRESULT STDMETHODCALLTYPE SetOneShot (BOOL OneShot);
	HRESULT STDMETHODCALLTYPE SetMediaType (const AM_MEDIA_TYPE *pType);
	HRESULT STDMETHODCALLTYPE GetConnectedMediaType ( AM_MEDIA_TYPE *pType);
	HRESULT STDMETHODCALLTYPE SetBufferSamples (BOOL BufferThem);
	HRESULT STDMETHODCALLTYPE GetCurrentBuffer (long *pBufferSize, long *pBuffer);
	HRESULT STDMETHODCALLTYPE GetCurrentSample (IMediaSample **ppSample);
	HRESULT STDMETHODCALLTYPE SetCallback (ISampleGrabberCB *pCallback, long WhichMethodToCallback);

// Implementation
protected:
	static HRESULT WINAPI CheckInternalInterfaces (void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw);

protected:
	CAtlString				mFilterName;
	IFilterGraph *			mFilterGraph;
	LPUNKNOWN				mUnknown;
	IBaseFilterPtr			mInnerBaseFilter;
	IMediaSeekingPtr		mInnerMediaSeeking;
	ISampleGrabberPtr		mInnerSampleGrabber;
	IFileSourceFilterPtr	mInnerFileSourceFilter;
	IAMFilterMiscFlagsPtr	mInnerAMFilterMiscFlags;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CTracePins :
	public CComObjectRootEx<CComMultiThreadModel>,
	public _ITracePins,
	public IEnumPins
{
public:
	CTracePins ();
	virtual ~CTracePins ();

// Attributes
public:
	UINT	mLogLevelPins;

// Operations
public:
	CTracePins & Initialize (LPUNKNOWN pUnknown, IBaseFilter * pFilter, UINT pLogLevelPins = LogVerbose|LogTime);
	void FinalRelease ();

// Overrides

// Interfaces
public:
	DECLARE_GET_CONTROLLING_UNKNOWN()
	BEGIN_COM_MAP(CTracePins)
		COM_INTERFACE_ENTRY(_ITracePins)
		COM_INTERFACE_ENTRY(IEnumPins)
	END_COM_MAP()

public:
	// IEnumPins
    HRESULT STDMETHODCALLTYPE Next (ULONG cPins, IPin **ppPins, ULONG *pcFetched);
    HRESULT STDMETHODCALLTYPE Skip (ULONG cPins);
    HRESULT STDMETHODCALLTYPE Reset ();
    HRESULT STDMETHODCALLTYPE Clone (IEnumPins **ppEnum);

// Implementation
protected:
	IBaseFilter *	mFilter;
	LPUNKNOWN		mUnknown;
	IEnumPinsPtr	mInnerEnum;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CTracePin :
	public CComObjectRootEx<CComMultiThreadModel>,
	public _ITracePin,
	public IPin,
	public IMemInputPin,
	public IAsyncReader,
	public IMediaSeeking,
	public IPinConnection,
	public IPinFlowControl
{
public:
	CTracePin ();
	virtual ~CTracePin ();

// Attributes
public:
	UINT	mLogLevel;

// Operations
public:
	CTracePin & Initialize (LPUNKNOWN pUnknown, IBaseFilter * pFilter, UINT pLogLevel = LogIfActive);
	void FinalRelease ();

// Overrides

// Interfaces
public:
	DECLARE_GET_CONTROLLING_UNKNOWN()
	BEGIN_COM_MAP(CTracePin)
		COM_INTERFACE_ENTRY(_ITracePin)
		COM_INTERFACE_ENTRY_FUNC_BLIND(0, CheckInternalInterfaces)
		COM_INTERFACE_ENTRY(IMemInputPin)
		COM_INTERFACE_ENTRY(IAsyncReader)
		COM_INTERFACE_ENTRY(IMediaSeeking)
		COM_INTERFACE_ENTRY(IPinConnection)
		COM_INTERFACE_ENTRY(IPinFlowControl)
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

	// IMemInputPin
	HRESULT STDMETHODCALLTYPE GetAllocator (IMemAllocator **ppAllocator);
	HRESULT STDMETHODCALLTYPE NotifyAllocator (IMemAllocator *pAllocator, BOOL bReadOnly);
	HRESULT STDMETHODCALLTYPE GetAllocatorRequirements (ALLOCATOR_PROPERTIES *pProps);
	HRESULT STDMETHODCALLTYPE Receive (IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE ReceiveMultiple (IMediaSample **pSamples, long nSamples, long *nSamplesProcessed);
	HRESULT STDMETHODCALLTYPE ReceiveCanBlock (void);

	// IAsyncReader
	HRESULT STDMETHODCALLTYPE RequestAllocator (IMemAllocator *pPreferred, ALLOCATOR_PROPERTIES *pProps, IMemAllocator **ppActual);
	HRESULT STDMETHODCALLTYPE Request (IMediaSample *pSample, DWORD_PTR dwUser);
	HRESULT STDMETHODCALLTYPE WaitForNext (DWORD dwTimeout, IMediaSample **ppSample,DWORD_PTR *pdwUser);
	HRESULT STDMETHODCALLTYPE SyncReadAligned (IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE SyncRead (LONGLONG llPosition, LONG lLength, BYTE *pBuffer);
	HRESULT STDMETHODCALLTYPE Length (LONGLONG *pTotal, LONGLONG *pAvailable);

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

	// IPinConnection
	HRESULT STDMETHODCALLTYPE DynamicQueryAccept (const AM_MEDIA_TYPE *pmt);
	HRESULT STDMETHODCALLTYPE NotifyEndOfStream (HANDLE hNotifyEvent);
	HRESULT STDMETHODCALLTYPE IsEndPin (void);
	HRESULT STDMETHODCALLTYPE DynamicDisconnect (void);

	// IPinFlowControl
	HRESULT STDMETHODCALLTYPE Block (DWORD dwBlockFlags, HANDLE hEvent);

// Implementation
protected:
	static HRESULT WINAPI CheckInternalInterfaces (void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw);

protected:
	CAtlString			mName;
	IBaseFilter *		mFilter;
	LPUNKNOWN			mUnknown;
	IPinPtr				mInnerPin;
	IMemInputPinPtr		mInnerMemInputPin;
	IAsyncReaderPtr		mInnerAsyncReader;
	IMediaSeekingPtr	mInnerMediaSeeking;
	IPinConnectionPtr	mInnerPinConnection;
	IPinFlowControlPtr	mInnerPinFlowControl;
};

/////////////////////////////////////////////////////////////////////////////
