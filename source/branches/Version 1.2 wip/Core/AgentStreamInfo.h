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
#include "DaCoreOdl.h"
#include "AgentStreamUtils.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentStreamInfo :
	public CComObjectRootEx<CComMultiThreadModel>,
	public _IAgentStreamInfo,
	public CAgentStreamUtils,
	public CAgentFileClient
{
public:
	CAgentStreamInfo ();
	virtual ~CAgentStreamInfo ();

// Attributes
public:

// Operations
public:
	CAgentStreamInfo& Initialize (CAgentFile* pAgentFile);

	void LogAnimationSequence (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogAnimationSequenceFrames (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogAnimationSequenceAudio (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	friend void LogAnimationSequence (UINT pLogLevel, const CAnimationSequence* pSequence, LPCTSTR pFormat = NULL, ...);
	friend void LogAnimationSequenceFrames (UINT pLogLevel, const CAnimationSequence* pSequence, LPCTSTR pFormat = NULL, ...);
	friend void LogAnimationSequenceAudio (UINT pLogLevel, const CAnimationSequence* pSequence, LPCTSTR pFormat = NULL, ...);

// Overrides

// Interfaces
public:
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CAgentStreamInfo)
		COM_INTERFACE_ENTRY(_IAgentStreamInfo)
	END_COM_MAP()

public:
	// _IAgentStreamInfo
	virtual HRESULT STDMETHODCALLTYPE GetMaxSequenceDuration (long *pMaxSequenceDuration);
	virtual HRESULT STDMETHODCALLTYPE GetMaxSequenceFrames (long *pMaxSequenceFrames);

	virtual HRESULT STDMETHODCALLTYPE GetAnimationIndex (long *pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE SetAnimationIndex (long pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE GetAnimationName (BSTR *pAnimationName);
	virtual HRESULT STDMETHODCALLTYPE SetAnimationName (BSTR pAnimationName);
	virtual HRESULT STDMETHODCALLTYPE GetAnimationSource (BSTR *pAnimationSource);
	virtual HRESULT STDMETHODCALLTYPE SetAnimationSource (BSTR pAnimationSource);

	virtual HRESULT STDMETHODCALLTYPE CalcAnimationFrameCount (long *pAnimationFrameCount, long pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE CalcAnimationDuration (long *pAnimationDuration, long pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE FindAnimationSpeakingFrame (long *pSpeakingFrameNdx, long pAnimationNdx, long pStartFrameNdx = 0);

	virtual HRESULT STDMETHODCALLTYPE NewAnimationSequence ();
	virtual HRESULT STDMETHODCALLTYPE EndAnimationSequence ();
	virtual HRESULT STDMETHODCALLTYPE ClearAnimationSequences ();

	virtual HRESULT STDMETHODCALLTYPE GetSequenceFrameCount (long *pSequenceFrameCount);
	virtual HRESULT STDMETHODCALLTYPE GetSequenceDuration (long *pSequenceDuration);
	virtual HRESULT STDMETHODCALLTYPE GetSequenceLoop (long *pLoopDuration);
	virtual HRESULT STDMETHODCALLTYPE CalcSequenceTimeNdx (long *pTimeNdx, long pFrameNum, boolean pClampFrameNum = false);
	virtual HRESULT STDMETHODCALLTYPE CalcSequenceFrameNum (long *pFrameNum, long pTimeNdx, boolean pClampTimeNdx = false);
	virtual HRESULT STDMETHODCALLTYPE CalcSequenceAnimationFrameNdx (long* pAnimationNdx, long *pFrameNdx, long pTimeNdx, boolean pClampTimeNdx = false);

	virtual HRESULT STDMETHODCALLTYPE SequenceAll ();
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimation (long pAnimationNdx, long pMaxLoopTime = 0);
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimationFrame (long pAnimationNdx, long pFrameNdx);
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimationExit (long pAnimationNdx, long pLastFrameNdx);
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimationLoop (long pAnimationNdx, long pMaxLoopTime = 0);
	virtual HRESULT STDMETHODCALLTYPE TruncateAnimationLoop (long pMinDuration = 0);

	virtual HRESULT STDMETHODCALLTYPE CueSequenceAudio (long pStartFrameNum = 0);
	virtual HRESULT STDMETHODCALLTYPE ClearSequenceAudio ();

// Implementation
public:
	bool Lock ();	// Use carefully!
	bool Unlock ();	// Use carefully!

	CAnimationSequence* GetAnimationSequence ();

	long FindAudioSegment (long pFrameNum);
	static long FindAudioSegment (CAnimationSequence* pAnimationSequence, long pFrameNum) ;
	static bool ResyncAudioVideo (CAnimationSequence* pAnimationSequence);

	bool ResetMouthOverlays ();
	bool SetMouthOverlay (short pMouthOverlayNdx, long pTimeNdx = 0);
	short GetMouthOverlay (long pTimeNdx = 0) const;
	bool SetSpeakingDuration (long pSpeakingDuration);					// Duration of -1 means "infinite"
	long GetSpeakingDuration (bool pRealiseInfinite = false) const;		// Realising an "infinite" duration turns it into an arbitrarily long positive duration

	friend CAtlString MouthOverlayStr (short pMouthOverlayNdx);

protected:
	long SequenceAnimationFrames (CAnimationSequence* pSequence, long pAnimationNdx, long pStartFrameNdx, long pEndFrameNdx, long pLoopFrameNdx, long pMaxLoopTime, bool pExit = false);

protected:
	long								mAnimationNdx;
	CAtlString							mAnimationSource;
	CAtlOwnPtrList <CAnimationSequence>	mSequences;
	CAtlTypeArray <LONGLONG>			mMouthOverlays;
	long								mSpeakingDuration;
	const long							mMaxLoopTime;
	const long							mMaxLoopFrames;
	mutable CComAutoCriticalSection		mCritSec;
};

/////////////////////////////////////////////////////////////////////////////
