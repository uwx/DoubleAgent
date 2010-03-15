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
#ifndef DIRECTSHOWSOURCE_H_INCLUDED_
#define DIRECTSHOWSOURCE_H_INCLUDED_
#pragma once

#include "DirectShowFilter.h"
#include "DirectShowSeeking.h"
#include "DirectSoundPinPush.h"
#include "DirectSoundConvert.h"
#include "AgentStreamUtils.h"
#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E563-A208-11DE-ABF2-002421116FB2}")) CDirectShowSource : public CDirectShowFilter, public CDirectShowSeeking, public CAgentStreamUtils, public CDirectShowUtils, protected CDirectSoundConvertCache
{
	DECLARE_DYNCREATE(CDirectShowSource)
//	DECLARE_OLECREATE_EX(CDirectShowSource) // Not registered for now
public:
	CDirectShowSource();
	virtual ~CDirectShowSource();
	void Terminate ();

// Attributes
public:
	bool SetBkColor (const COLORREF * pBkColor);
	const COLORREF * GetBkColor () const;

// Operations
public:
	HRESULT SetAgentFile (CAgentFile * pAgentFile);
	HRESULT	SegmentDurationChanged ();
	HRESULT PutVideoSample (REFERENCE_TIME & pSampleTime, REFERENCE_TIME pStopTime);

// Overrides
	//{{AFX_VIRTUAL(CDirectShowSource)
	public:
	virtual LONGLONG GetDuration ();
	virtual LONGLONG GetPreroll ();
	protected:
	virtual const GUID & GetClassID ();
	virtual HRESULT SetFilterName (LPCWSTR pFilterName);
	virtual CString GetFilterName ();
	virtual void GetSeekingTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime);
	virtual void InitializePins ();
	virtual void OnJoinedFilterGraph ();
	virtual void OnLeftFilterGraph ();
	virtual HRESULT OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState);
	virtual void OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags);
	virtual HRESULT StartOutputStreams ();
	virtual void OnClockPulse ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(FileSource, IFileSourceFilter)
		HRESULT STDMETHODCALLTYPE Load (LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt);
		HRESULT STDMETHODCALLTYPE GetCurFile (LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt);
	END_INTERFACE_PART(FileSource)

	BEGIN_INTERFACE_PART(FilterMiscFlags, IAMFilterMiscFlags)
        ULONG STDMETHODCALLTYPE GetMiscFlags (void);
	END_INTERFACE_PART(FilterMiscFlags)

	BEGIN_INTERFACE_PART(ConfigCallback, IGraphConfigCallback)
		HRESULT STDMETHODCALLTYPE Reconfigure (PVOID pvContext, DWORD dwFlags);
	END_INTERFACE_PART(ConfigCallback)

	//BEGIN_INTERFACE_PART(OpenProgress, IAMOpenProgress)
	//END_INTERFACE_PART(OpenProgress)

	DECLARE_INTERFACE_MAP()

protected:
	HRESULT OpenFile (LPCTSTR pFileName);
	void ReadFile ();

	bool PutVideoFrame ();
	bool CueAudioSegments ();
	bool CueAudioSegments (CAnimationSequence * pAnimationSequence);
	void ConnectSequenceAudio ();
	void ConnectSequenceAudio (CAnimationSequence * pAnimationSequence);
	CDirectSoundPinPush * ConnectSequenceSound (long pSoundNdx);

protected:
	tPtr <COLORREF>							mBkColor;
	tPtr <CDirectShowPinOut>				mVideoOutPin;
	COwnPtrMap <long, CDirectSoundPinPush>	mAudioOutPins;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWSOURCE_H_INCLUDED_
