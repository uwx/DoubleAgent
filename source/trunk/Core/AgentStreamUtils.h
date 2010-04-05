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
#ifndef AGENTSTREAMUTILS_H_INCLUDED_
#define AGENTSTREAMUTILS_H_INCLUDED_
#pragma once

#include "AgentFile.h"
#include "DaCoreOdl.h"

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(_IAgentStreamInfo, __uuidof(_IAgentStreamInfo));

/////////////////////////////////////////////////////////////////////////////

class CAgentStreamInfo;

class CAgentStreamUtils
{
public:
	CAgentStreamUtils ();
	virtual ~CAgentStreamUtils ();

// Attributes
public:
	CAgentFile * GetAgentFile () const;
	CAgentStreamInfo * GetAgentStreamInfo () const;

// Operations
public:
	long CalcFileFrameCount () const;
	long CalcFileDuration () const;

// Implementation
protected:
	void SetAgentFile (CAgentFile * pAgentFile, CObject * pClient);

	const CStringMap <CStringArray> & GetFileStates (UINT pLogLevel = 15) const;
	const CAgentFileGestures & GetFileGestures (UINT pLogLevel = 15) const;
	bool GetFileImages (UINT pLogLevel = 15) const;
	bool GetFileSounds (UINT pLogLevel = 15) const;

	void SetAgentStreamInfo (_IAgentStreamInfo * pStreamInfo);
	void SetAgentStreamInfo (CAgentStreamInfo * pStreamInfo);

	static void SetPaletteBkColor (LPBITMAPINFO pBitmapInfo, BYTE pTransparentNdx, COLORREF pBkColor);

private:
	mutable CAgentFile *		mAgentFile;
	_IAgentStreamInfoPtr		mStreamInfo;
	mutable CCriticalSection	mUtilCritSec;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

struct CSeqVideoFrame
{
	const CAgentFileFrame *		mFileFrame;
	long						mAnimationNdx;
	long						mFrameNdx;
	long						mStartTime;
};

struct CSeqAudioSegment
{
	long						mSequenceFrameNdx;
	long						mAnimationNdx;
	long						mFrameNdx;
	long						mSoundNdx;
	long						mStartTime;
	long						mEndTime;
};

struct CAnimationSequence
{
	long						mDuration;
	long						mLoopDuration;
	CArrayEx <CSeqVideoFrame>	mFrames;
	CArrayEx <CSeqAudioSegment>	mAudio;

	CAnimationSequence () : mDuration (0), mLoopDuration (0) {}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTSTREAMUTILS_H_INCLUDED_
