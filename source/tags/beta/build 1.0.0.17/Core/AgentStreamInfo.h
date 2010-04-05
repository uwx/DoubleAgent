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
#ifndef AGENTSTREAMINFO_H_INCLUDED_
#define AGENTSTREAMINFO_H_INCLUDED_
#pragma once

#include "DaCoreOdl.h"
#include "AgentStreamUtils.h"

/////////////////////////////////////////////////////////////////////////////

class CAgentStreamInfo : public CCmdTarget, public _IAgentStreamInfo, public CAgentStreamUtils
{
public:
	CAgentStreamInfo (CAgentFile * pAgentFile, LPUNKNOWN pOuterUnknown);
	virtual ~CAgentStreamInfo ();
	DECLARE_DYNAMIC (CAgentStreamInfo)

// Attributes
public:
	long MaxSequenceDuration () const;
	long MaxSequenceFrames () const;

// Operations
public:
	virtual HRESULT STDMETHODCALLTYPE GetAnimationIndex (long *pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE SetAnimationIndex (long pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE GetAnimationName (BSTR *pAnimationName);
	virtual HRESULT STDMETHODCALLTYPE SetAnimationName (BSTR pAnimationName);
	virtual HRESULT STDMETHODCALLTYPE GetAnimationSource (BSTR *pAnimationSource);
	virtual HRESULT STDMETHODCALLTYPE SetAnimationSource (BSTR pAnimationSource);

	virtual HRESULT STDMETHODCALLTYPE CalcAnimationFrameCount (long *pAnimationFrameCount, long pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE CalcAnimationDuration (long *pAnimationDuration, long pAnimationNdx);
	virtual HRESULT STDMETHODCALLTYPE FindAnimationSpeakingFrame (long *pSpeakingFrameNdx, long pAnimationNdx, long pStartFrameNdx = 0);

	HRESULT CalcAnimationFrameCount (long *pAnimationFrameCount, LPCTSTR pAnimationName) {return CalcAnimationFrameCount (pAnimationFrameCount, (long)GetAgentFile()->FindGesture (pAnimationName));}
	HRESULT CalcAnimationDuration (long *pAnimationDuration, LPCTSTR pAnimationName) {return CalcAnimationDuration (pAnimationDuration, (long)GetAgentFile()->FindGesture (pAnimationName));}
	HRESULT FindAnimationSpeakingFrame (long *pSpeakingFrameNdx, LPCTSTR pAnimationName, long pStartFrameNdx = 0) {return FindAnimationSpeakingFrame (pSpeakingFrameNdx, (long)GetAgentFile()->FindGesture (pAnimationName), pStartFrameNdx);}

	virtual HRESULT STDMETHODCALLTYPE NewAnimationSequence ();
	virtual HRESULT STDMETHODCALLTYPE EndAnimationSequence ();
	virtual HRESULT STDMETHODCALLTYPE ClearAnimationSequences ();

	virtual HRESULT STDMETHODCALLTYPE GetSequenceFrameCount (long *pSequenceFrameCount);
	virtual HRESULT STDMETHODCALLTYPE GetSequenceDuration (long *pSequenceDuration);
	virtual HRESULT STDMETHODCALLTYPE GetSequenceLoop (long *pLoopDuration);
	virtual HRESULT STDMETHODCALLTYPE CalcSequenceTimeNdx (long *pTimeNdx, long pFrameNum, boolean pClampFrameNum = false);
	virtual HRESULT STDMETHODCALLTYPE CalcSequenceFrameNum (long *pFrameNum, long pTimeNdx, boolean pClampTimeNdx = false);
	virtual HRESULT STDMETHODCALLTYPE CalcSequenceAnimationFrameNdx (long * pAnimationNdx, long *pFrameNdx, long pTimeNdx, boolean pClampTimeNdx = false);

	virtual HRESULT STDMETHODCALLTYPE SequenceAll ();
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimation (long pAnimationNdx, long pMaxLoopTime = 0);
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimationFrame (long pAnimationNdx, long pFrameNdx);
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimationExit (long pAnimationNdx, long pLastFrameNdx);
	virtual HRESULT STDMETHODCALLTYPE SequenceAnimationLoop (long pAnimationNdx, long pMaxLoopTime = 0);
	virtual HRESULT STDMETHODCALLTYPE TruncateAnimationLoop (long pMinDuration = 0);

	virtual HRESULT STDMETHODCALLTYPE CueSequenceAudio (long pStartFrameNum = 0);
	virtual HRESULT STDMETHODCALLTYPE ClearSequenceAudio ();

	void LogAnimationSequence (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogAnimationSequenceFrames (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogAnimationSequenceAudio (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	friend void LogAnimationSequence (UINT pLogLevel, const CAnimationSequence * pSequence, LPCTSTR pFormat = NULL, ...);
	friend void LogAnimationSequenceFrames (UINT pLogLevel, const CAnimationSequence * pSequence, LPCTSTR pFormat = NULL, ...);
	friend void LogAnimationSequenceAudio (UINT pLogLevel, const CAnimationSequence * pSequence, LPCTSTR pFormat = NULL, ...);

// Overrides
	//{{AFX_VIRTUAL(CAgentStreamInfo)
	public:
	virtual void OnFinalRelease ();
	virtual STDMETHODIMP_(ULONG) AddRef ();
	virtual STDMETHODIMP_(ULONG) Release ();
	virtual STDMETHODIMP QueryInterface (REFIID iid, LPVOID* ppvObj);
	//}}AFX_VIRTUAL

// Implementation
protected:
	DECLARE_INTERFACE_MAP()

public:
	bool Lock ();	// Use carefully!
	bool Unlock ();	// Use carefully!

	CAnimationSequence * GetAnimationSequence ();

	long FindAudioSegment (long pFrameNum);
	static long FindAudioSegment (CAnimationSequence * pAnimationSequence, long pFrameNum) ;
	static bool ResyncAudioVideo (CAnimationSequence * pAnimationSequence);

	bool ResetMouthOverlays ();
	bool SetMouthOverlay (short pMouthOverlayNdx, long pTimeNdx = 0);
	short GetMouthOverlay (long pTimeNdx = 0) const;
	bool SetSpeakingDuration (long pSpeakingDuration);
	long GetSpeakingDuration () const;

	friend CString MouthOverlayStr (short pMouthOverlayNdx);

protected:
	long SequenceAnimationFrames (CAnimationSequence * pSequence, long pAnimationNdx, long pStartFrameNdx, long pEndFrameNdx, long pLoopFrameNdx, long pMaxLoopTime, bool pExit = false);

protected:
	long								mAnimationNdx;
	CString								mAnimationSource;
	COwnPtrList <CAnimationSequence>	mSequences;
	CArrayEx <LONGLONG>					mMouthOverlays;
	long								mSpeakingDuration;
	const long							mMaxLoopTime;
	const long							mMaxLoopFrames;
	mutable CCriticalSection			mCritSec;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTSTREAMINFO_H_INCLUDED_
