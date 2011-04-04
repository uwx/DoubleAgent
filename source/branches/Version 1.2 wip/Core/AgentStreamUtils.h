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
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "DaCoreOdl.h"

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(_IAgentStreamInfo, __uuidof(_IAgentStreamInfo));
_COM_SMARTPTR_TYPEDEF(_IAgentStreamRender, __uuidof(_IAgentStreamRender));
_COM_SMARTPTR_TYPEDEF(_IAgentStreamSource, __uuidof(_IAgentStreamSource));

/////////////////////////////////////////////////////////////////////////////

class CAgentStreamInfo;

class CAgentStreamUtils
{
public:
	CAgentStreamUtils ();
	virtual ~CAgentStreamUtils ();

// Attributes
public:
	CAgentFile* GetAgentFile () const;
	CAgentStreamInfo* GetAgentStreamInfo () const;

// Operations
public:
	long CalcFileFrameCount () const;
	long CalcFileDuration () const;

	const CAgentFileStates& GetFileStates () const;
	const CAgentFileGestures& GetFileGestures () const;
	bool GetFileImages () const;
	bool GetFileSounds () const;

// Implementation
protected:
	void SetAgentFile (CAgentFile* pAgentFile, CAgentFileClient* pClient);
	void SetAgentStreamInfo (_IAgentStreamInfo* pStreamInfo);
	void SetAgentStreamInfo (CAgentStreamInfo* pStreamInfo);

	static void SetPaletteBkColor (LPBITMAPINFO pBitmapInfo, BYTE pTransparentNdx, COLORREF pBkColor);

private:
	mutable CAgentFile*				mAgentFile;
	_IAgentStreamInfoPtr				mStreamInfo;
	mutable CComAutoCriticalSection		mUtilCritSec;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

struct CSeqVideoFrame
{
	const CAgentFileFrame*				mFileFrame;
	long								mAnimationNdx;
	long								mFrameNdx;
	long								mStartTime;
};

struct CSeqAudioSegment
{
	long								mSequenceFrameNdx;
	long								mAnimationNdx;
	long								mFrameNdx;
	long								mSoundNdx;
	long								mStartTime;
	long								mEndTime;
};

struct CAnimationSequence
{
	long								mDuration;
	long								mLoopDuration;
	CAtlStructArray <CSeqVideoFrame>	mFrames;
	CAtlStructArray <CSeqAudioSegment>	mAudio;

	CAnimationSequence () : mDuration (0), mLoopDuration (0) {}
};

/////////////////////////////////////////////////////////////////////////////
