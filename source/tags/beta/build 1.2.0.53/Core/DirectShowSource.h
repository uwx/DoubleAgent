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
#include "DirectShowFilter.h"
#include "DirectShowSeeking.h"
#include "DirectSoundPinPush.h"
#include "DirectSoundConvert.h"
#include "AgentStreamUtils.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E563-A208-11DE-ABF2-002421116FB2}")) CDirectShowSource : public CDirectShowFilter, public _IAgentStreamSource, public IFileSourceFilter, public IAMFilterMiscFlags, public IGraphConfigCallback, public CDirectShowSeeking<CDirectShowFilter>, public CAgentFileClient, public CAgentStreamUtils, public CDirectShowUtils, protected CDirectSoundConvertCache
{
public:
	CDirectShowSource();
	virtual ~CDirectShowSource();

// Attributes
public:

// Operations
public:
	HRESULT FinalConstruct ();
	void Terminate ();
	void FinalRelease ();

// Overrides
public:
	virtual LONGLONG GetDuration ();
	virtual LONGLONG GetPreroll ();
protected:
	virtual const GUID& GetClassID ();
	virtual HRESULT SetFilterName (LPCWSTR pFilterName);
	virtual CAtlString GetFilterName ();
	virtual void GetSeekingTimes (REFERENCE_TIME& pCurrTime, REFERENCE_TIME& pStopTime);
	virtual void InitializePins ();
	virtual void OnJoinedFilterGraph ();
	virtual void OnLeftFilterGraph ();
	virtual HRESULT OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState);
	virtual void OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags);
	virtual HRESULT StartOutputStreams ();
	virtual void OnClockPulse ();

// Interfaces
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDirectShowSource)
		COM_INTERFACE_ENTRY(_IAgentStreamSource)
		COM_INTERFACE_ENTRY(IMediaSeeking)
		COM_INTERFACE_ENTRY(IFileSourceFilter)
		COM_INTERFACE_ENTRY(IAMFilterMiscFlags)
		COM_INTERFACE_ENTRY(IGraphConfigCallback)
		COM_INTERFACE_ENTRY_CHAIN(CDirectShowFilter)
	END_COM_MAP()

public:
	// _IAgentStreamSource
	HRESULT STDMETHODCALLTYPE GetAgentFile (ULONG_PTR *pAgentFile);
	HRESULT STDMETHODCALLTYPE SetAgentFile (ULONG_PTR pAgentFile);
	HRESULT STDMETHODCALLTYPE GetAgentStreamInfo (ULONG_PTR *pAgentStreamInfo);
	HRESULT STDMETHODCALLTYPE SetAgentStreamInfo (ULONG_PTR pAgentStreamInfo);
	HRESULT STDMETHODCALLTYPE GetBkColor (COLORREF*pBkColor);
	HRESULT STDMETHODCALLTYPE SetBkColor (const COLORREF*pBkColor);
	HRESULT STDMETHODCALLTYPE SegmentDurationChanged (void);

	// IFileSourceFilter
	HRESULT STDMETHODCALLTYPE Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt);
	HRESULT STDMETHODCALLTYPE GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt);

	// IAMFilterMiscFlags
    ULONG STDMETHODCALLTYPE GetMiscFlags (void);

	// IGraphConfigCallback
	HRESULT STDMETHODCALLTYPE Reconfigure (PVOID pvContext, DWORD dwFlags);

	// IAMOpenProgress //

// Implementation
protected:
	HRESULT OpenFile (LPCTSTR pFileName);
	void ReadFile ();

	HRESULT PutVideoSample (REFERENCE_TIME& pSampleTime, REFERENCE_TIME pStopTime);
	bool PutVideoFrame ();
	bool CueAudioSegments ();
	bool CueAudioSegments (CAnimationSequence* pAnimationSequence);
	void ConnectSequenceAudio ();
	void ConnectSequenceAudio (CAnimationSequence* pAnimationSequence);
	CDirectSoundPinPush * ConnectSequenceSound (long pSoundNdx);

protected:
	tPtr <COLORREF>								mBkColor;
	tPtr <CDirectShowPinOut>					mVideoOutPin;
	CAtlOwnPtrMap <long, CDirectSoundPinPush>	mAudioOutPins;
};

/////////////////////////////////////////////////////////////////////////////
