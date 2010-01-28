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
#ifndef DIRECTSHOWUTILS_H_INCLUDED_
#define DIRECTSHOWUTILS_H_INCLUDED_
#pragma once

#include <dshow.h>
#include <dsound.h>
#include <amaudio.h>
#include <strmif.h>
#include <mmsystem.h>
#include <control.h>
#include <amvideo.h>
#include <mediaobj.h>
#include <evcode.h>
#include <dmo.h>
#include <d3d9.h>
#include <vmr9.h>
#include <uuids.h>
#include "VfwErr.h"
#include "DaCoreExp.h"

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IGraphBuilder, __uuidof(IGraphBuilder));
_COM_SMARTPTR_TYPEDEF (IGraphConfig, __uuidof(IGraphConfig));
_COM_SMARTPTR_TYPEDEF (IFilterGraph, __uuidof(IFilterGraph));
_COM_SMARTPTR_TYPEDEF (IFilterGraph2, __uuidof(IFilterGraph2));
_COM_SMARTPTR_TYPEDEF (IFilterChain, __uuidof(IFilterChain));
_COM_SMARTPTR_TYPEDEF (IBaseFilter, __uuidof(IBaseFilter));
_COM_SMARTPTR_TYPEDEF (IMediaFilter, __uuidof(IMediaFilter));
_COM_SMARTPTR_TYPEDEF (IFileSourceFilter, __uuidof(IFileSourceFilter));
_COM_SMARTPTR_TYPEDEF (IPin, __uuidof(IPin));
_COM_SMARTPTR_TYPEDEF (IPinConnection, __uuidof(IPinConnection));
_COM_SMARTPTR_TYPEDEF (IPinFlowControl, __uuidof(IPinFlowControl));
_COM_SMARTPTR_TYPEDEF (IReferenceClock, __uuidof(IReferenceClock));
_COM_SMARTPTR_TYPEDEF (IReferenceClockTimerControl, __uuidof(IReferenceClockTimerControl));
_COM_SMARTPTR_TYPEDEF (IMemInputPin, __uuidof(IMemInputPin));
_COM_SMARTPTR_TYPEDEF (IMemAllocator, __uuidof(IMemAllocator));
_COM_SMARTPTR_TYPEDEF (IAsyncReader, __uuidof(IAsyncReader));
_COM_SMARTPTR_TYPEDEF (IMediaSample, __uuidof(IMediaSample));
_COM_SMARTPTR_TYPEDEF (IMediaSample2, __uuidof(IMediaSample2));
_COM_SMARTPTR_TYPEDEF (IMediaControl, __uuidof(IMediaControl));
_COM_SMARTPTR_TYPEDEF (IMediaEventEx, __uuidof(IMediaEventEx));
_COM_SMARTPTR_TYPEDEF (IMediaEventSink, __uuidof(IMediaEventSink));
_COM_SMARTPTR_TYPEDEF (IVideoWindow, __uuidof(IVideoWindow));
_COM_SMARTPTR_TYPEDEF (IBasicAudio, __uuidof(IBasicAudio));
_COM_SMARTPTR_TYPEDEF (IBasicVideo, __uuidof(IBasicVideo));
_COM_SMARTPTR_TYPEDEF (IMediaPosition, __uuidof(IMediaPosition));
_COM_SMARTPTR_TYPEDEF (IMediaSeeking, __uuidof(IMediaSeeking));
_COM_SMARTPTR_TYPEDEF (IQualityControl, __uuidof(IQualityControl));
_COM_SMARTPTR_TYPEDEF (IAMFilterMiscFlags, __uuidof(IAMFilterMiscFlags));
_COM_SMARTPTR_TYPEDEF (IAMStreamControl, __uuidof(IAMStreamControl));
_COM_SMARTPTR_TYPEDEF (IVMRFilterConfig, __uuidof(IVMRFilterConfig));
_COM_SMARTPTR_TYPEDEF (IVMRWindowlessControl, __uuidof(IVMRWindowlessControl));
_COM_SMARTPTR_TYPEDEF (IVMRFilterConfig9, __uuidof(IVMRFilterConfig9));
_COM_SMARTPTR_TYPEDEF (IVMRWindowlessControl9, __uuidof(IVMRWindowlessControl9));
_COM_SMARTPTR_TYPEDEF (IEnumFilters, __uuidof(IEnumFilters));
_COM_SMARTPTR_TYPEDEF (IEnumPins, __uuidof(IEnumPins));
_COM_SMARTPTR_TYPEDEF (IEnumMediaTypes, __uuidof(IEnumMediaTypes));

class __declspec(uuid("{D51BD5A1-7548-11CF-A520-0080C77EF58A}")) WaveParser;

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static const LONGLONG MsPer100Ns = 100000;
static const double Ns100PerSec = 10000000.0;
static inline double RefTimeSec (REFERENCE_TIME pRefTime) {return (double)pRefTime / Ns100PerSec;}

/////////////////////////////////////////////////////////////////////////////

class tMediaTypePtrFree {protected: static inline void _Init (AM_MEDIA_TYPE *& pPtr) {}; static inline void _Free (AM_MEDIA_TYPE * pPtr) {if (pPtr) MoDeleteMediaType ((DMO_MEDIA_TYPE*)pPtr);}};
typedef tPtr <AM_MEDIA_TYPE, tMediaTypePtrFree> tMediaTypePtr;
typedef CArrayEx <tMediaTypePtr, AM_MEDIA_TYPE*> CMediaTypes;

/////////////////////////////////////////////////////////////////////////////

struct __declspec(novtable) AM_MEDIA_TYPE_Safe : public AM_MEDIA_TYPE
{
    AM_MEDIA_TYPE_Safe () {memset(this, 0, sizeof(*this)); MoInitMediaType ((DMO_MEDIA_TYPE*) this, 0);}
    AM_MEDIA_TYPE_Safe (const AM_MEDIA_TYPE & pSource) {MoCopyMediaType ((DMO_MEDIA_TYPE*) this, (DMO_MEDIA_TYPE*) &pSource);}
	AM_MEDIA_TYPE_Safe & operator= (const AM_MEDIA_TYPE & pSource) {MoFreeMediaType ((DMO_MEDIA_TYPE*) this); MoCopyMediaType ((DMO_MEDIA_TYPE*) this, (DMO_MEDIA_TYPE*) &pSource); return *this;}
    AM_MEDIA_TYPE_Safe * Clear () {MoFreeMediaType ((DMO_MEDIA_TYPE*) this); memset(this, 0, sizeof(*this)); return this;}
    ~AM_MEDIA_TYPE_Safe () {MoFreeMediaType ((DMO_MEDIA_TYPE*) this);}

    operator DMO_MEDIA_TYPE* () {return (DMO_MEDIA_TYPE*)this;}
    operator const DMO_MEDIA_TYPE* () const {return (DMO_MEDIA_TYPE*)this;}
};

struct __declspec(novtable) PIN_INFO_Safe : public PIN_INFO
{
    PIN_INFO_Safe () {memset(this, 0, sizeof(*this));}
    PIN_INFO_Safe (const PIN_INFO & pSource) {memcpy(this, &pSource, sizeof(*this)); if (pFilter) pFilter->AddRef();}
	PIN_INFO_Safe & operator= (const PIN_INFO & pSource) {if (pFilter) pFilter->Release(); memcpy(this, &pSource, sizeof(*this)); if (pFilter) pFilter->AddRef(); return *this;};
    PIN_INFO_Safe * Clear () {if (pFilter) pFilter->Release(); memset(this, 0, sizeof(*this)); return this;}
    ~PIN_INFO_Safe () {if (pFilter) pFilter->Release();}
};

struct __declspec(novtable) FILTER_INFO_Safe : public FILTER_INFO
{
    FILTER_INFO_Safe () {memset(this, 0, sizeof(*this));}
    FILTER_INFO_Safe (const FILTER_INFO & pSource) {memcpy(this, &pSource, sizeof(*this)); if (pGraph) pGraph->AddRef();}
	FILTER_INFO_Safe & operator= (const FILTER_INFO & pSource) {if (pGraph) pGraph->Release(); memcpy(this, &pSource, sizeof(*this)); if (pGraph) pGraph->AddRef(); return *this;};
    FILTER_INFO_Safe * Clear () {if (pGraph) pGraph->Release(); memset(this, 0, sizeof(*this)); return this;}
    ~FILTER_INFO_Safe () {if (pGraph) pGraph->Release();}
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CDirectShowUtils
{
public:
	static HRESULT GetMonikerFilter (LPCTSTR pMonikerName, IBaseFilter ** pFilter, CString & pFilterName);
	static HRESULT GetDefaultFilter (const GUID & pCategory, IBaseFilter ** pFilter, CString & pFilterName);

	static HRESULT ConnectFilters (IGraphBuilder * pGraphBuilder, IBaseFilter * pDownstreamFilter, IBaseFilter * pUpstreamFilter, bool pDirect = false, const AM_MEDIA_TYPE * pDirectMediaType = NULL, IPin ** pDownstreamPin = NULL, IPin ** pUpstreamPin = NULL);
	static HRESULT GetFilterPins (IBaseFilter * pFilter, IPin ** pInputPin, IPin ** pOutputPin);
	static HRESULT GetFilterPin (IBaseFilter * pFilter, PIN_DIRECTION pPinDir, REFGUID pMajorMediaType, IPin ** pPin, AM_MEDIA_TYPE ** pPinMediaType = NULL);
	static HRESULT GetRenderType (IPin * pPin, GUID & pRenderType);

	static HRESULT MoveFilterToCache (IBaseFilter * pFilter, IFilterGraph * pFilterGraph = NULL);
	static HRESULT AddFilterToCache (IBaseFilter * pFilter, IFilterGraph * pFilterGraph);
	static HRESULT AddFilterToCache (IBaseFilter * pFilter, IGraphConfig * pGraphConfig);
	static HRESULT MoveFiltersToCache (IBaseFilter * pStartFilter, IBaseFilter * pEndFilter, IFilterGraph * pFilterGraph = NULL);
	static HRESULT MoveFiltersToCache (IBaseFilter * pStartFilter, IBaseFilter * pEndFilter, IFilterChain * pFilterChain, IBaseFilter * pExcludeFilter = NULL, ...);

	static HRESULT FindFilterInCache (REFGUID pFilterClass, IFilterGraph * pFilterGraph, IBaseFilter ** pFilter);
	static HRESULT FindFilterInCache (REFGUID pFilterClass, IGraphConfig * pGraphConfig, IBaseFilter ** pFilter);
	static HRESULT GetFilterFromCache (REFGUID pFilterClass, IFilterGraph * pFilterGraph, IBaseFilter ** pFilter);
	static HRESULT GetFilterFromCache (REFGUID pFilterClass, IGraphConfig * pGraphConfig, IBaseFilter ** pFilter);

	static HRESULT ConnectNullRenderer (IGraphBuilder * pGraphBuilder, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin = NULL, AM_MEDIA_TYPE * pMediaType = NULL);
	static HRESULT InitPinAllocator (IPin * pInputPin, AM_MEDIA_TYPE* pPinMediaType = NULL);

	static HRESULT CopySample (IMediaSample * pSource, IMediaSample * pTarget);
	static HRESULT DuplicateSample (IMediaSample * pSource, IMemAllocator * pAllocator, IMediaSample ** pDuplicate, DWORD pFlags = AM_GBF_NOWAIT);

	static LPWAVEFORMATEX GetSoundFormat (LPCVOID pSoundData, long pSoundSize, long * pFormatSize = NULL, UINT pLogLevel = 15);
	static LPWAVEFORMATEX GetSoundFormat (LPCTSTR pSoundFileName, long * pFormatSize = NULL, UINT pLogLevel = 15);
	static LPWAVEFORMATEX GetSoundFormat (HMMIO pSound, long * pFormatSize = NULL, UINT pLogLevel = 15);
	static AM_MEDIA_TYPE * GetSoundMediaType (LPCVOID pSoundData, long pSoundSize, UINT pLogLevel = 15);
	static AM_MEDIA_TYPE * GetSoundMediaType (LPCTSTR pSoundFileName, UINT pLogLevel = 15);

public:
	friend CString _DACORE_IMPEXP MediaTypeStr (const AM_MEDIA_TYPE & pMediaType);
	friend CString _DACORE_IMPEXP WaveFormatStr (WORD pFormatTag);
	friend CString _DACORE_IMPEXP FilterStateStr (OAFilterState pFilterState, bool pStatePending = false);
	friend CString _DACORE_IMPEXP SeekingFlagsStr (DWORD pSeekingFlags);
	friend CString _DACORE_IMPEXP SeekingCapsStr (DWORD pSeekingCaps);
	friend CString _DACORE_IMPEXP SampleIdStr (DWORD pSampleId);
	friend CString _DACORE_IMPEXP SampleFlagsStr (DWORD pSampleFlags);
	friend CString _DACORE_IMPEXP PinIdStr (IPin * pPin, bool pIncludeFilter = true);

public:
	friend void _DACORE_IMPEXP LogFilters (UINT pLogLevel, IFilterGraph * pFilterGraph, bool pEnumPinTypes = false, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogFilter (UINT pLogLevel, IBaseFilter * pFilter, bool pEnumPinTypes = false, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogFilterPins (UINT pLogLevel, IBaseFilter * pFilter, bool pEnumTypes = false, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogFilterPin (UINT pLogLevel, IPin * pPin, bool pEnumTypes = false, LPCTSTR pFormat = NULL, ...);

	friend void _DACORE_IMPEXP LogFilterStates (UINT pLogLevel, IFilterGraph * pFilterGraph, bool pEnumPins = false, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogFilterState (UINT pLogLevel, IBaseFilter * pFilter, bool pEnumPins = false, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogFilterPinState (UINT pLogLevel, IPin * pPin, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogFilterCache (UINT pLogLevel, IFilterGraph * pFilterGraph, LPCTSTR pFormat = NULL, ...);

	friend void _DACORE_IMPEXP LogPinAllocator (UINT pLogLevel, IPin * pPin, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogMemAllocator (UINT pLogLevel, IMemInputPin * pInputPin, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogMemAllocator (UINT pLogLevel, IMemAllocator * pAllocator, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogAllocatorProps (UINT pLogLevel, const ALLOCATOR_PROPERTIES & pProperties, LPCTSTR pFormat = NULL, ...);

	friend void _DACORE_IMPEXP LogMediaSeeking (UINT pLogLevel, IMediaSeeking * pMediaSeeking, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogMediaSeekingPos (UINT pLogLevel, IMediaSeeking * pMediaSeeking, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogMediaSample (UINT pLogLevel, IMediaSample * pMediaSample, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogMediaSampleId (UINT pLogLevel, IMediaSample * pMediaSample, LPCTSTR pFormat = NULL, ...);

	friend void _DACORE_IMPEXP LogMediaType (UINT pLogLevel, const AM_MEDIA_TYPE & pMediaType, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogWaveFormat (UINT pLogLevel, const WAVEFORMAT & pWaveFormat, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogWaveFormat (UINT pLogLevel, const WAVEFORMATEX & pWaveFormat, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogVideoFormat (UINT pLogLevel, const VIDEOINFOHEADER & pVideoFormat, LPCTSTR pFormat = NULL, ...);

	friend void _DACORE_IMPEXP LogWaveSound (UINT pLogLevel, LPCVOID pSoundData, long pSoundSize, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogWaveSound (UINT pLogLevel, LPCTSTR pSoundFileName, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogWaveSound (UINT pLogLevel, HMMIO pSound, LPCTSTR pFormat = NULL, ...);

	friend void _DACORE_IMPEXP LogMediaEvent (UINT pLogLevel, IMediaEvent * pMediaEvent, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogMediaEvent (UINT pLogLevel, long pEventCode, LONG_PTR pEventParam1, LONG_PTR pEventParam2, LPCTSTR pFormat = NULL, ...);
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{C1F400A0-3F08-11d3-9F0B-006008039E37}")) SampleGrabber;
class __declspec(uuid("{C1F400A4-3F08-11d3-9F0B-006008039E37}")) NullRenderer;

interface __declspec(uuid("{0579154A-2B53-4994-B0D0-E773148EFF85}")) ISampleGrabberCB : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SampleCB (double SampleTime, IMediaSample *pSample) = 0;
    virtual HRESULT STDMETHODCALLTYPE BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen) = 0;
};

interface __declspec(uuid("{6B652FFF-11FE-4fce-92AD-0266B5D7C78F}")) ISampleGrabber : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetOneShot (BOOL OneShot) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType (const AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType ( AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples (BOOL BufferThem) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer (long *pBufferSize, long *pBuffer) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample (IMediaSample **ppSample) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback (ISampleGrabberCB *pCallback, long WhichMethodToCallback) = 0;
};

_COM_SMARTPTR_TYPEDEF (ISampleGrabber, __uuidof(ISampleGrabber));

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG

#include "Registry.h"
#define	_LOG_DIRECT_SHOW	(GetProfileDebugInt(_T("LogDirectShow"),LogVerbose,true)&0xFFFF|LogHighVolume|LogTimeMs)

#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWUTILS_H_INCLUDED_
