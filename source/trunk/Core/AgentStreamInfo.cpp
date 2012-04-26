/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include "DaCore.h"
#include "AgentStreamInfo.h"
#include "StringArrayEx.h"
#ifdef _DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
#define	_DEBUG_SEQUENCE			(GetProfileDebugInt(_T("DebugSequence"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_SEQUENCE_FRAMES	(GetProfileDebugInt(_T("DebugSequenceFrames"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_DEBUG_SEQUENCE_AUDIO	(GetProfileDebugInt(_T("DebugSequenceAudio"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
//#define	_DEBUG_MOUTH_OVERLAYS	LogNormal|LogTimeMs|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentStreamInfo::CAgentStreamInfo ()
:	mAnimationNdx (-1),
	mSpeakingDuration (0),
	mMaxLoopTime (300000),
	mMaxLoopFrames (1000)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CAgentStreamInfo::CAgentStreamInfo (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
#endif
#ifdef	_DEBUG_NOT
	const_cast <long &> (mMaxLoopTime) = 2000;
#endif
}

CAgentStreamInfo::~CAgentStreamInfo ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CAgentStreamInfo::~CAgentStreamInfo (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
#endif
	SetAgentFile (NULL, this);
}

CAgentStreamInfo& CAgentStreamInfo::Initialize (CAgentFile* pAgentFile)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p(%d)] CAgentStreamInfo::Initialize (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
#endif
	SetAgentFile (pAgentFile, this);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentStreamInfo::Lock ()
{
	if	(this)
	{
		mCritSec.Lock ();
		return true;
	}
	return false;
}

bool CAgentStreamInfo::Unlock ()
{
	if	(this)
	{
		mCritSec.Unlock ();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetMaxSequenceDuration (long *pMaxSequenceDuration)
{
	HRESULT	lResult = S_OK;

	if	(pMaxSequenceDuration)
	{
		(*pMaxSequenceDuration) = mMaxLoopTime;
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetMaxSequenceFrames (long *pMaxSequenceFrames)
{
	HRESULT	lResult = S_OK;

	if	(pMaxSequenceFrames)
	{
		(*pMaxSequenceFrames) = mMaxLoopFrames;
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetAnimationIndex (long *pAnimationNdx)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(!pAnimationNdx)
		{
			lResult = E_POINTER;
		}
		else
		{
			(*pAnimationNdx) = mAnimationNdx;
			if	(mAnimationNdx >= 0)
			{
				lResult = S_OK;
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SetAnimationIndex (long pAnimationNdx)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		long	lPrevAnimationNdx = mAnimationNdx;

		if	(pAnimationNdx < 0)
		{
			mAnimationNdx = pAnimationNdx;
		}
		else
		{
			const CAgentFileGestures&	lGestures = GetFileGestures ();

			if	(pAnimationNdx < (INT_PTR)lGestures.mAnimations.GetCount())
			{
				mAnimationNdx = pAnimationNdx;
			}
			else
			{
				lResult = E_INVALIDARG;
			}
		}
		if	(
				(lResult == S_OK)
			&&	(mAnimationNdx == lPrevAnimationNdx)
			)
		{
			lResult = S_FALSE;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetAnimationName (BSTR *pAnimationName)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(!pAnimationName)
		{
			lResult = E_POINTER;
		}
		else
		{
			const CAgentFileGestures&	lGestures = GetFileGestures ();

			(*pAnimationName) = NULL;

			if	(mAnimationNdx < 0)
			{
				lResult = S_FALSE;
			}
			else
			if	(mAnimationNdx < (INT_PTR)lGestures.mAnimations.GetCount())
			{
				(*pAnimationName) = tBstrPtr (lGestures.mAnimations [mAnimationNdx]->Name).Detach();
			}
			else
			{
				lResult = AGENTERR_ANIMATIONNOTFOUND;
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SetAnimationName (BSTR pAnimationName)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(!pAnimationName)
		{
			lResult = SetAnimationIndex (-1);
		}
		else
		{
			const CAgentFileGestures&	lGestures = GetFileGestures ();
			long						lAnimationNdx = (long)FindSortedString (lGestures.mNames, pAnimationName);

			if	(lAnimationNdx >= 0)
			{
				lResult = SetAnimationIndex (lAnimationNdx);
			}
			else
			{
				lResult = AGENTERR_ANIMATIONNOTFOUND;
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetAnimationSource (BSTR *pAnimationSource)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(!pAnimationSource)
		{
			lResult = E_POINTER;
		}
		else
		{
			(*pAnimationSource) = mAnimationSource.AllocSysString ();
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SetAnimationSource (BSTR pAnimationSource)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		mAnimationSource = pAnimationSource;
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::CalcAnimationFrameCount (long *pAnimationFrameCount, long pAnimationNdx)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*				lAgentFile;
		const CAgentFileAnimation*	lAnimation;

		if	(
				(lAgentFile = GetAgentFile())
			&&	(lAnimation = lAgentFile->GetAnimation (pAnimationNdx))
			)
		{
			(*pAnimationFrameCount) = (long)(short)lAnimation->FrameCount;
		}
		else
		{
			(*pAnimationFrameCount) = 0;
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::CalcAnimationDuration (long *pAnimationDuration, long pAnimationNdx)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*				lAgentFile;
		long						lAnimationDuration = 0;
		const CAgentFileAnimation*	lAnimation;
		long						lFrameNdx;

		if	(
				(lAgentFile = GetAgentFile())
			&&	(lAnimation = lAgentFile->GetAnimation (pAnimationNdx))
			)
		{
			for	(lFrameNdx = 0; lFrameNdx < (long)(short)lAnimation->FrameCount; lFrameNdx++)
			{
				lAnimationDuration += (long)(short)lAnimation->Frames [lFrameNdx].Duration;
			}
		}
		else
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}

		(*pAnimationDuration) = lAnimationDuration;
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::FindAnimationSpeakingFrame (long *pSpeakingFrameNdx, long pAnimationNdx, long pStartFrameNdx)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*				lAgentFile;
		const CAgentFileAnimation*	lAnimation;
		long						lFrameNdx;

		(*pSpeakingFrameNdx) = -1;

		if	(
				(lAgentFile = GetAgentFile())
			&&	(lAnimation = lAgentFile->GetAnimation (pAnimationNdx))
			)
		{
			for	(lFrameNdx = max (pStartFrameNdx, 0); lFrameNdx < (long)(short)lAnimation->FrameCount; lFrameNdx++)
			{
				if	(lAnimation->Frames [lFrameNdx].OverlayCount > 0)
				{
					(*pSpeakingFrameNdx) = lFrameNdx;
					lResult = S_OK;
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::NewAnimationSequence ()
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;

		if	(lSequence = new CAnimationSequence)
		{
			mSequences.AddTail (lSequence);
			mAnimationSource.Empty ();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::EndAnimationSequence ()
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(!mSequences.IsEmpty ())
		{
			mSequences.DeleteHead ();
			mAnimationSource.Empty ();
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::ClearAnimationSequences ()
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(!mSequences.IsEmpty ())
		{
			mSequences.DeleteAll ();
			mAnimationSource.Empty ();
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

CAnimationSequence* CAgentStreamInfo::GetAnimationSequence ()
{
	CAnimationSequence*	lRet = NULL;
	CLockCS					lLock (mCritSec);

	try
	{
		if	(!mSequences.IsEmpty ())
		{
			lRet = mSequences.GetHead ();
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetSequenceFrameCount (long *pSequenceFrameCount)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;

		if	(pSequenceFrameCount)
		{
			(*pSequenceFrameCount) = 0;
		}

		if	(lSequence = GetAnimationSequence())
		{
			if	(pSequenceFrameCount)
			{
				(*pSequenceFrameCount) = (long)lSequence->mFrames.GetCount();
			}
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetSequenceDuration (long *pSequenceDuration)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;

		if	(pSequenceDuration)
		{
			(*pSequenceDuration) = 0;
		}

		if	(lSequence = GetAnimationSequence())
		{
			if	(pSequenceDuration)
			{
				(*pSequenceDuration) = lSequence->mDuration;
			}
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::GetSequenceLoop (long *pLoopDuration)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;

		if	(pLoopDuration)
		{
			(*pLoopDuration) = 0;
		}

		if	(
				(!mSequences.IsEmpty ())
			&&	(lSequence = mSequences.GetTail ())
			)
		{
			if	(pLoopDuration)
			{
				(*pLoopDuration) = lSequence->mLoopDuration;
			}
			if	(lSequence->mLoopDuration > 0)
			{
				lResult = S_OK;
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::CalcSequenceTimeNdx (long *pTimeNdx, long pFrameNum, boolean pClampFrameNum)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;

		(*pTimeNdx) = -1;

		if	(
				(lSequence = GetAnimationSequence())
			&&	(
					(pClampFrameNum)
				||	(
						(pFrameNum >= 0)
					&&	(pFrameNum <= (long)lSequence->mFrames.GetCount())
					)
				)
			)
		{
			pFrameNum = min (max (pFrameNum, 0), (long)lSequence->mFrames.GetCount());
			if	(pFrameNum > (long)lSequence->mFrames.GetCount()-1)
			{
				(*pTimeNdx) = lSequence->mDuration;
			}
			else
			{
				(*pTimeNdx) = lSequence->mFrames [pFrameNum].mStartTime;
			}
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::CalcSequenceFrameNum (long *pFrameNum, long pTimeNdx, boolean pClampTimeNdx)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;
		long					lFrameNdx;

		(*pFrameNum) = -1;

		if	(
				(lSequence = GetAnimationSequence())
			&&	(
					(pClampTimeNdx)
				||	(
						(pTimeNdx >= 0)
					&&	(pTimeNdx < lSequence->mDuration)
					)
				)
			)
		{
			if	(pTimeNdx >= lSequence->mDuration)
			{
				(*pFrameNum) = (long)lSequence->mFrames.GetCount();
			}
			else
			{
				for	(lFrameNdx = (long)lSequence->mFrames.GetCount()-1; lFrameNdx >= 0; lFrameNdx--)
				{
					if	(pTimeNdx >= lSequence->mFrames [lFrameNdx].mStartTime)
					{
						break;
					}
				}
				(*pFrameNum) = max (lFrameNdx, 0);
			}
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::CalcSequenceAnimationFrameNdx (long* pAnimationNdx, long *pFrameNdx, long pTimeNdx, boolean pClampTimeNdx)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;
		long					lFrameNdx;

		(*pAnimationNdx) = -1;
		(*pFrameNdx) = -1;

		if	(
				(lSequence = GetAnimationSequence())
			&&	(
					(pClampTimeNdx)
				||	(
						(pTimeNdx >= 0)
					&&	(pTimeNdx < lSequence->mDuration)
					)
				)
			)
		{
			for	(lFrameNdx = (long)lSequence->mFrames.GetCount()-1; lFrameNdx >= 0; lFrameNdx--)
			{
				if	(
						(pTimeNdx >= lSequence->mFrames [lFrameNdx].mStartTime)
					||	(lFrameNdx == 0)
					)
				{
					(*pAnimationNdx) = lSequence->mFrames [lFrameNdx].mAnimationNdx;
					(*pFrameNdx) = lSequence->mFrames [lFrameNdx].mFrameNdx;
					lResult = S_OK;
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SequenceAll ()
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*				lAgentFile;
		CAnimationSequence*		lSequence;
		long						lAnimationNdx;
		const CAgentFileAnimation*	lAnimation;
		long						lFrameNdx;
		const CAgentFileFrame*		lFileFrame;

		if	(
				(lAgentFile = GetAgentFile())
			&&	(!mSequences.IsEmpty ())
			&&	(lSequence = mSequences.GetTail ())
			)
		{
			for	(lAnimationNdx = 0; lAnimation = lAgentFile->GetAnimation (lAnimationNdx); lAnimationNdx++)
			{
				for	(lFrameNdx = 0; lFrameNdx < (long)(short)lAnimation->FrameCount; lFrameNdx++)
				{
					tS <CSeqVideoFrame>	lSequenceFrame;

					lFileFrame = lAnimation->Frames + lFrameNdx;
					lSequenceFrame.mFileFrame = lFileFrame;
					lSequenceFrame.mAnimationNdx = lAnimationNdx;
					lSequenceFrame.mFrameNdx = lFrameNdx;
					lSequenceFrame.mStartTime = lSequence->mDuration;

					lSequence->mDuration += (long)(short)lFileFrame->Duration;
					lSequence->mFrames.Add (lSequenceFrame);

					lResult = S_OK;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SequenceAnimation (long pAnimationNdx, long pMaxLoopTime)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*			lAgentFile;
		CAnimationSequence*	lSequence;

		if	(
				(!mSequences.IsEmpty ())
			&&	(lSequence = mSequences.GetTail ())
			&&	(lAgentFile = GetAgentFile())
			&&	(lAgentFile->GetAnimation (pAnimationNdx) != NULL)
			)
		{
			long	lMaxLoopTime = (pMaxLoopTime > 0) ? min (pMaxLoopTime, mMaxLoopTime) : mMaxLoopTime;

#ifdef	_DEBUG_SEQUENCE
			if	(LogIsActive (_DEBUG_SEQUENCE))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation [%d] [%ls]"), pAnimationNdx, (BSTR)(lAgentFile->GetAnimation (pAnimationNdx)->Name));
			}
#endif
			if	(SequenceAnimationFrames (lSequence, pAnimationNdx, 0, LONG_MAX, -1, lMaxLoopTime) >= 0)
			{
				lResult = S_OK;
			}
		}
		else
		if	(lSequence)
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
		else
		{
			lResult = E_UNEXPECTED;
		}

#ifdef	_DEBUG_SEQUENCE
		if	(LogIsActive (_DEBUG_SEQUENCE))
		{
			if	(lResult == S_OK)
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence [%d] duration [%d] loop [%d] frames [%d]"), mSequences.GetCount(), lSequence->mDuration, lSequence->mLoopDuration, lSequence->mFrames.GetCount());
			}
			else
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation [%d] failed"), pAnimationNdx);
			}
		}
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SequenceAnimationFrame (long pAnimationNdx, long pFrameNdx)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*			lAgentFile;
		CAnimationSequence*	lSequence;

		if	(
				(!mSequences.IsEmpty ())
			&&	(lSequence = mSequences.GetTail ())
			&&	(lAgentFile = GetAgentFile())
			&&	(lAgentFile->GetAnimation (pAnimationNdx) != NULL)
			)
		{
#ifdef	_DEBUG_SEQUENCE
			if	(LogIsActive (_DEBUG_SEQUENCE))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation frame [%d] [%d] [%ls]"), pAnimationNdx, pFrameNdx, (BSTR)(lAgentFile->GetAnimation (pAnimationNdx)->Name));
			}
#endif
			if	(SequenceAnimationFrames (lSequence, pAnimationNdx, pFrameNdx, pFrameNdx, -1, mMaxLoopTime) >= 0)
			{
				lResult = S_OK;
			}
		}
		else
		if	(lSequence)
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
		else
		{
			lResult = E_UNEXPECTED;
		}

#ifdef	_DEBUG_SEQUENCE
		if	(LogIsActive (_DEBUG_SEQUENCE))
		{
			if	(lResult == S_OK)
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence [%d] duration [%d] loop [%d] frames [%d]"), mSequences.GetCount(), lSequence->mDuration, lSequence->mLoopDuration, lSequence->mFrames.GetCount());
			}
			else
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation [%d] failed"), pAnimationNdx);
			}
		}
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SequenceAnimationExit (long pAnimationNdx, long pLastFrameNdx)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*				lAgentFile;
		CAnimationSequence*		lSequence = NULL;
		const CAgentFileAnimation*	lAnimation;

		if	(!mSequences.IsEmpty ())
		{
			lSequence = mSequences.GetTail ();
		}
		if	(
				(lSequence)
			&&	(lSequence->mFrames.GetCount() > 0)
			)
		{
			if	(pAnimationNdx < 0)
			{
				pAnimationNdx = lSequence->mFrames [(INT_PTR)lSequence->mFrames.GetCount()-1].mAnimationNdx;
			}
			if	(pLastFrameNdx < 0)
			{
				pLastFrameNdx = lSequence->mFrames [(INT_PTR)lSequence->mFrames.GetCount()-1].mFrameNdx;
			}
		}
		if	(
				(lSequence)
			&&	(lAgentFile = GetAgentFile())
			&&	(lAnimation = lAgentFile->GetAnimation (pAnimationNdx))
			&&	(pLastFrameNdx >= 0)
			&&	(pLastFrameNdx < (long)(short)lAnimation->FrameCount)
			)
		{
			const CAgentFileFrame*	lFileFrame = lAnimation->Frames + pLastFrameNdx;
			long					lFrameNdx = lFileFrame->ExitFrame;

#ifdef	_DEBUG_SEQUENCE
			if	(LogIsActive (_DEBUG_SEQUENCE))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation exit [%d] [%ls] from [%d] next [%d]"), pAnimationNdx, (BSTR)lAnimation->Name, pLastFrameNdx, lFrameNdx);
			}
#endif
			if	(lFrameNdx >= 0)
			{
				SequenceAnimationFrames (lSequence, pAnimationNdx, lFrameNdx, LONG_MAX, -1, mMaxLoopTime, true);
				lResult = S_OK;
#ifdef	_DEBUG_SEQUENCE
				if	(LogIsActive (_DEBUG_SEQUENCE))
				{
					LogMessage (_DEBUG_SEQUENCE, _T("Sequence [%d] duration [%d] loop [%d] frames [%d]"), mSequences.GetCount(), lSequence->mDuration, lSequence->mLoopDuration, lSequence->mFrames.GetCount());
				}
#endif
			}
		}
		else
		if	(lSequence)
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
		else
		{
			lResult = E_UNEXPECTED;
		}

#ifdef	_DEBUG_SEQUENCE
		if	(LogIsActive (_DEBUG_SEQUENCE))
		{
			if	(lResult == S_OK)
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence [%d] duration [%d] loop [%d] frames [%d]"), mSequences.GetCount(), lSequence->mDuration, lSequence->mLoopDuration, lSequence->mFrames.GetCount());
			}
			else
			if	(FAILED (lResult))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation exit [%d] failed"), pAnimationNdx);
			}
		}
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::SequenceAnimationLoop (long pAnimationNdx, long pMaxLoopTime)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAgentFile*				lAgentFile;
		CAnimationSequence*		lPrevSequence;
		tPtr <CAnimationSequence>	lNextSequence;

		if	(
				(lAgentFile = GetAgentFile())
			&&	(mSequences.GetCount() == 1)
			&&	(lPrevSequence = mSequences.GetTail ())
			&&	(lPrevSequence->mLoopDuration > 0)
			&&	(lPrevSequence->mFrames.GetCount() > 0)
			&&	(lPrevSequence->mFrames [(INT_PTR)lPrevSequence->mFrames.GetCount()-1].mAnimationNdx == pAnimationNdx)
			&&	(lNextSequence = new CAnimationSequence)
			)
		{
			long	lMaxLoopTime = (pMaxLoopTime > 0) ? min (pMaxLoopTime, mMaxLoopTime) : mMaxLoopTime;

#ifdef	_DEBUG_SEQUENCE
			if	(LogIsActive (_DEBUG_SEQUENCE))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation loop [%d] [%ls] from [%d]"), pAnimationNdx, (BSTR)(lAgentFile->GetAnimation (pAnimationNdx)->Name), lPrevSequence->mFrames [(INT_PTR)lPrevSequence->mFrames.GetCount()-1].mFrameNdx);
			}
#endif
			lNextSequence->mLoopDuration = lPrevSequence->mLoopDuration;

			if	(SequenceAnimationFrames (lNextSequence, pAnimationNdx, 0, LONG_MAX, lPrevSequence->mFrames [lPrevSequence->mFrames.GetCount()-1].mFrameNdx, lMaxLoopTime) >= 0)
			{
				mSequences.DeleteTail ();
				mSequences.AddTail (lNextSequence.Detach());
				lResult = S_OK;
			}
		}
		else
		{
			lResult = E_UNEXPECTED;
		}

#ifdef	_DEBUG_SEQUENCE
		if	(LogIsActive (_DEBUG_SEQUENCE))
		{
			CAnimationSequence*	lSequence;

			if	(
					(lResult == S_OK)
				&&	(lSequence = mSequences.GetTail ())
				)
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence [%d] duration [%d] loop [%d] frames [%d]"), mSequences.GetCount(), lSequence->mDuration, lSequence->mLoopDuration, lSequence->mFrames.GetCount());
			}
			else
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Sequence animation loop [%d] failed"), pAnimationNdx);
			}
		}
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::TruncateAnimationLoop (long pMinDuration)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

//
//	Truncates any animation loop that might be in the sequence at a logical stopping point.
//
	try
	{
		CAgentFile*			lAgentFile;
		CAnimationSequence*	lSequence;
		long					lAnimationNdx;
		long					lFrameNdx;
		long					lMaxLoopFrame;

		if	(
				(lAgentFile = GetAgentFile())
			&&	(mSequences.GetCount() == 1)
			&&	(lSequence = mSequences.GetTail ())
			&&	(lSequence->mLoopDuration > 0)
			&&	(lSequence->mFrames.GetCount() > 0)
			)
		{
			lAnimationNdx = lSequence->mFrames [(INT_PTR)lSequence->mFrames.GetCount()-1].mAnimationNdx;
			lMaxLoopFrame = lSequence->mFrames [(INT_PTR)lSequence->mFrames.GetCount()-1].mFrameNdx;

#ifdef	_DEBUG_SEQUENCE
			if	(LogIsActive (_DEBUG_SEQUENCE))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("Truncate animation loop [%d] [%ls] after [%d] for [%d]"), lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->Name), lMaxLoopFrame, pMinDuration);
			}
#endif
			for	(lFrameNdx = 0; lFrameNdx < (long)lSequence->mFrames.GetCount()-1; lFrameNdx++)
			{
				if	(
						(lSequence->mFrames [lFrameNdx].mFrameNdx == lMaxLoopFrame)
					&&	(lSequence->mFrames [lFrameNdx+1].mStartTime > pMinDuration)
					)
				{
					lSequence->mLoopDuration = 0;
					lSequence->mDuration = lSequence->mFrames [lFrameNdx+1].mStartTime;
					lSequence->mFrames.SetCount (lFrameNdx+1);
					lResult = S_OK;
					break;
				}
			}
		}

#ifdef	_DEBUG_SEQUENCE
		if	(
				(LogIsActive (_DEBUG_SEQUENCE))
			&&	(lResult == S_OK)
			&&	(lSequence = mSequences.GetTail ())
			)
		{
			LogMessage (_DEBUG_SEQUENCE, _T("Sequence [%d] duration [%d] frames [%d]"), mSequences.GetCount(), lSequence->mDuration, lSequence->mFrames.GetCount());
		}
#endif
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

long CAgentStreamInfo::SequenceAnimationFrames (CAnimationSequence* pSequence, long pAnimationNdx, long pStartFrameNdx, long pEndFrameNdx, long pLoopFrameNdx, long pMaxLoopTime, bool pExit)
{
	long						lRet = -1;
	CAgentFile*					lAgentFile = GetAgentFile();
	const CAgentFileAnimation*	lAnimation = lAgentFile->GetAnimation (pAnimationNdx);
	long						lMaxFrameNdx = min (pEndFrameNdx, (long)(short)lAnimation->FrameCount - 1L);
	long						lFrameNdx;
	const CAgentFileFrame*		lFileFrame;
	UINT						lLoopFound = 0;
	long						lLoopDuration = 0;
	long						lMaxLoopFrame = -1;

	for	(lFrameNdx = max(max(pStartFrameNdx,pLoopFrameNdx),0); (lFrameNdx >= 0) && (lFrameNdx <= lMaxFrameNdx); lFrameNdx++)
	{
		lFileFrame = lAnimation->Frames + lFrameNdx;

		if	(
				(!pExit)
			&&	(pLoopFrameNdx >= 0)
			)
		{
			lLoopFound++;
			lMaxLoopFrame = pLoopFrameNdx;
			pLoopFrameNdx = -1;
		}
		else
		if	(lFileFrame->Duration > 0)
		{
			tS <CSeqVideoFrame>	lSequenceFrame;

			lSequenceFrame.mFileFrame = lFileFrame;
			lSequenceFrame.mAnimationNdx = pAnimationNdx;
			lSequenceFrame.mFrameNdx = lFrameNdx;
			lSequenceFrame.mStartTime = pSequence->mDuration;

			pSequence->mDuration += (long)(short)lFileFrame->Duration;
			lRet = (long)pSequence->mFrames.Add (lSequenceFrame);

			if	(lLoopFound)
			{
				lLoopDuration += (long)(short)lFileFrame->Duration;
				if	(lFrameNdx == lMaxLoopFrame)
				{
					pSequence->mLoopDuration = max (pSequence->mLoopDuration, lLoopDuration);
					lLoopDuration = 0;
				}
			}
			else
			{
				pSequence->mLoopDuration = 0;
				if	(!pExit)
				{
					lMaxLoopFrame = min (lMaxLoopFrame, lFrameNdx);
				}
			}
			if	(lFrameNdx == lMaxLoopFrame)
			{
				if	(pExit)
				{
					break;
				}
				else
				{
					lLoopFound++;
				}
			}
			lMaxLoopFrame = max (lMaxLoopFrame, lFrameNdx);

#ifdef	_DEBUG_SEQUENCE_FRAMES
			if	(
					(LogIsActive (_DEBUG_SEQUENCE_FRAMES))
				&&	(lLoopFound < 20)
				)
			{
				if	(lLoopFound >= 10)
				{
					LogMessage (_DEBUG_SEQUENCE_FRAMES, _T("  ..."));
				}
				else
				{
					LogMessage (_DEBUG_SEQUENCE_FRAMES, _T("  Frame [%d] for [%d] at [%d]"), lFrameNdx, (long)(short)lFileFrame->Duration, lSequenceFrame.mStartTime);
				}
			}
#endif
		}

		if	(
				(pSequence->mDuration >= pMaxLoopTime)
			||	((long)pSequence->mFrames.GetCount() >= mMaxLoopFrames)
			)
		{
#ifdef	_DEBUG_SEQUENCE
			if	(LogIsActive (_DEBUG_SEQUENCE))
			{
				LogMessage (_DEBUG_SEQUENCE, _T("  Break at duration [%d of %d] frames [%d of %d] last [%d %d] loops [%u of %d end %d]"), pSequence->mDuration, pMaxLoopTime, pSequence->mFrames.GetCount(), mMaxLoopFrames, pSequence->mFrames [(INT_PTR)pSequence->mFrames.GetCount()-1].mAnimationNdx, pSequence->mFrames [pSequence->mFrames.GetCount()-1].mFrameNdx, lLoopFound, pSequence->mLoopDuration, lMaxLoopFrame);
			}
#endif
			if	(lLoopFound > 2)
			{
				//
				//	Truncate sequence at end of loop
				//
				for	(lFrameNdx = (long)pSequence->mFrames.GetCount()-1; lFrameNdx > 0; lFrameNdx--)
				{
					if	(pSequence->mFrames [lFrameNdx].mFrameNdx == lMaxLoopFrame)
					{
						break;
					}
				}
				if	(lFrameNdx < (long)pSequence->mFrames.GetCount()-1)
				{
					pSequence->mDuration -= pSequence->mFrames [(INT_PTR)pSequence->mFrames.GetCount()-1].mStartTime - pSequence->mFrames [lFrameNdx].mStartTime;
					pSequence->mFrames.SetCount (lFrameNdx+1);
#ifdef	_DEBUG_SEQUENCE
					if	(LogIsActive (_DEBUG_SEQUENCE))
					{
						LogMessage (_DEBUG_SEQUENCE, _T("  End loop at duration [%d] frames [%d] last [%d %d]"), pSequence->mDuration, pSequence->mFrames.GetCount(), pSequence->mFrames [(INT_PTR)pSequence->mFrames.GetCount()-1].mAnimationNdx, pSequence->mFrames [pSequence->mFrames.GetCount()-1].mFrameNdx);
					}
#endif
				}
			}
			break;
		}

		if	(pExit)
		{
			if	(lFileFrame->ExitFrame >= -1)
			{
				lFrameNdx = lFileFrame->ExitFrame - 1;
#ifdef	_DEBUG_SEQUENCE_FRAMES
				if	(LogIsActive (_DEBUG_SEQUENCE_FRAMES))
				{
					LogMessage (_DEBUG_SEQUENCE_FRAMES, _T("  Exit to [%d]"), lFrameNdx+1);
				}
#endif
			}
			else
			if	(
					(lFileFrame->Branching [0].mProbability == 100)
				&&	((long)lFileFrame->Branching [0].mFrameNdx > lFrameNdx)
				&&	((long)lFileFrame->Branching [0].mFrameNdx < (long)(short)lAnimation->FrameCount)
				)
			{
				lFrameNdx = (long)lFileFrame->Branching [0].mFrameNdx - 1;
#ifdef	_DEBUG_SEQUENCE_FRAMES
				if	(LogIsActive (_DEBUG_SEQUENCE_FRAMES))
				{
					LogMessage (_DEBUG_SEQUENCE_FRAMES, _T("  Branch [%hu] to [%d]"), lFileFrame->Branching [0].mProbability, lFrameNdx+1);
				}
#endif
			}
		}
		else
		{
			if	(lFileFrame->Branching [0].mProbability)
			{
				int	lBranchRand = (rand() % 100) + 1;
				int	lBranchNdx;

				for	(lBranchNdx = 0; lBranchNdx < 3; lBranchNdx++)
				{
					if	(
							(lFileFrame->Branching [lBranchNdx].mProbability)
						&&	((long)lFileFrame->Branching [lBranchNdx].mFrameNdx < (long)(short)lAnimation->FrameCount)
						&&	((lBranchRand -= (int)(short)lFileFrame->Branching [lBranchNdx].mProbability) <= 0)
						)
					{
						lFrameNdx = (long)lFileFrame->Branching [lBranchNdx].mFrameNdx - 1;
#ifdef	_DEBUG_SEQUENCE_FRAMES
						if	(
								(lLoopFound < 10)
							&&	(LogIsActive (_DEBUG_SEQUENCE_FRAMES))
							)
						{
							LogMessage (_DEBUG_SEQUENCE_FRAMES, _T("  Branch [%hu] to [%d]"), lFileFrame->Branching [lBranchNdx].mProbability, lFrameNdx+1);
						}
#endif
						break;
					}
				}
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentStreamInfo::CueSequenceAudio (long pStartFrameNum)
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;
		long					lFrameNdx;

		if	(
				(!mSequences.IsEmpty ())
			&&	(lSequence = mSequences.GetTail ())
			)
		{
#ifdef	_DEBUG_SEQUENCE_AUDIO
			LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("CueSequenceAudio Frames [%d]"), lSequence->mFrames.GetCount());
#endif
			lSequence->mAudio.RemoveAll ();

			for	(lFrameNdx = max (pStartFrameNum, 0); lFrameNdx < (long)lSequence->mFrames.GetCount(); lFrameNdx++)
			{
				if	(lSequence->mFrames [lFrameNdx].mFileFrame->SoundNdx >= 0)
				{
					tS <CSeqAudioSegment>	lCue;

					lCue.mSequenceFrameNdx = lFrameNdx;
					lCue.mAnimationNdx = lSequence->mFrames [lFrameNdx].mAnimationNdx;
					lCue.mFrameNdx = lSequence->mFrames [lFrameNdx].mFrameNdx;
					lCue.mSoundNdx = (long)lSequence->mFrames [lFrameNdx].mFileFrame->SoundNdx;
					lCue.mStartTime = lSequence->mFrames [lFrameNdx].mStartTime;
					lCue.mEndTime = -1; // To be determined by the DirectShow source filter

#ifdef	_DEBUG_SEQUENCE_AUDIO
					LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("  Cue [%2d] Frame [%2d %2d] Time [%6u] Sound [%2u]"), lSequence->mAudio.GetCount(), lCue.mAnimationNdx, lCue.mFrameNdx,lCue.mStartTime, lCue.mSoundNdx);
#endif
					lSequence->mAudio.Add (lCue);
					lResult = S_OK;
				}
			}
#ifdef	_DEBUG_SEQUENCE_AUDIO
			LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("CueSequenceAudio Cues [%d]"), lSequence->mAudio.GetCount());
#endif
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentStreamInfo::ClearSequenceAudio ()
{
	HRESULT	lResult = S_FALSE;
	CLockCS	lLock (mCritSec);

	try
	{
		CAnimationSequence*	lSequence;

		if	(
				(!mSequences.IsEmpty ())
			&&	(lSequence = mSequences.GetTail ())
			)
		{
#ifdef	_DEBUG_SEQUENCE_AUDIO
			if	(lSequence->mAudio.GetCount() > 0)
			{
				LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("ClearSequenceAudio [%d]"), lSequence->mAudio.GetCount());
			}
#endif
			lSequence->mAudio.RemoveAll ();
			lResult = S_OK;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentStreamInfo::FindAudioSegment (long pFrameNum)
{
	long	lRet = -1;
	CLockCS	lLock (mCritSec);

	try
	{
		lRet = FindAudioSegment (GetAnimationSequence (), pFrameNum);
	}
	catch AnyExceptionSilent

	return lRet;
}

long CAgentStreamInfo::FindAudioSegment (CAnimationSequence* pAnimationSequence, long pFrameNum)
{
	long	lRet = -1;
	long	lAudioNdx;

	try
	{
		if	(
				(pAnimationSequence)
			&&	(pFrameNum >= 0)
			&&	(pFrameNum < (long)pAnimationSequence->mFrames.GetCount())
			)
		{
			for	(lAudioNdx = 0; lAudioNdx < (long)pAnimationSequence->mAudio.GetCount(); lAudioNdx++)
			{
				if	(pAnimationSequence->mAudio [lAudioNdx].mSequenceFrameNdx == pFrameNum)
				{
					lRet = lAudioNdx;
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentStreamInfo::ResetMouthOverlays ()
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(mMouthOverlays.GetCount() > 0)
		{
#ifdef	_DEBUG_MOUTH_OVERLAYS
			LogMessage (_DEBUG_MOUTH_OVERLAYS, _T("ResetMouthOverlays"));
#endif
			mMouthOverlays.RemoveAll();
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

bool CAgentStreamInfo::SetMouthOverlay (short pMouthOverlayNdx, long pTimeNdx)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR			lNdx;
		LARGE_INTEGER	lMouthOverlay;

		for	(lNdx = (INT_PTR)mMouthOverlays.GetCount()-1; lNdx >= 0; lNdx--)
		{
			lMouthOverlay.QuadPart = mMouthOverlays [lNdx];
			if	(lMouthOverlay.HighPart >= pTimeNdx)
			{
				if	(
						(lMouthOverlay.HighPart > pTimeNdx)
					||	(
							(lMouthOverlay.HighPart == pTimeNdx)
						&&	(lMouthOverlay.LowPart != pMouthOverlayNdx)
						)
					)
				{
					mMouthOverlays.RemoveAt (lNdx);
					lRet = true;
				}
			}
			else
			{
				break;
			}
		}

#ifdef	_DEBUG_MOUTH_OVERLAYS
		LogMessage (_DEBUG_MOUTH_OVERLAYS, _T("SetMouthOverlay [%d] at [%d]"), pMouthOverlayNdx, pTimeNdx);
#endif
		lMouthOverlay.HighPart = pTimeNdx;
		lMouthOverlay.LowPart = pMouthOverlayNdx;
		if	(mMouthOverlays.AddSortedQS (lMouthOverlay.QuadPart) >= 0)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

short CAgentStreamInfo::GetMouthOverlay (long pTimeNdx) const
{
	short	lRet = -1;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR			lNdx;
		LARGE_INTEGER	lMouthOverlay;

		for	(lNdx = (INT_PTR)mMouthOverlays.GetCount()-1; lNdx >= 0; lNdx--)
		{
			lMouthOverlay.QuadPart = mMouthOverlays [lNdx];
			if	(lMouthOverlay.HighPart <= pTimeNdx)
			{
				lRet = (short)lMouthOverlay.LowPart;
				break;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentStreamInfo::SetSpeakingDuration (long pSpeakingDuration)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(mSpeakingDuration != pSpeakingDuration)
		{
			lRet = true;
		}
		mSpeakingDuration = pSpeakingDuration;
	}
	catch AnyExceptionSilent

	return lRet;
}

long CAgentStreamInfo::GetSpeakingDuration (bool pRealiseInfinite) const
{
	long	lRet = 0;
	CLockCS	lLock (mCritSec);

	try
	{
		lRet = mSpeakingDuration;
		if	(pRealiseInfinite)
		{
			if	(lRet < 0)
			{
				lRet = 60000;
			}
			else
			{
				lRet = min (lRet, 60000);
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString MouthOverlayStr (short pMouthOverlayNdx)
{
	CAtlString	lOverlayStr;

	lOverlayStr.Format (_T("%u "), pMouthOverlayNdx);

	switch (pMouthOverlayNdx)
	{
		case MouthOverlayClosed:	lOverlayStr += _T("Closed"); break;
		case MouthOverlayWide1:		lOverlayStr += _T("Wide1"); break;
		case MouthOverlayWide2:		lOverlayStr += _T("Wide2"); break;
		case MouthOverlayWide3:		lOverlayStr += _T("Wide3"); break;
		case MouthOverlayWide4:		lOverlayStr += _T("Wide4"); break;
		case MouthOverlayMedium:	lOverlayStr += _T("Medium"); break;
		case MouthOverlayNarrow:	lOverlayStr += _T("Narrow"); break;
	}

	lOverlayStr.TrimRight();
	return lOverlayStr;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentStreamInfo::ResyncAudioVideo (CAnimationSequence* pAnimationSequence)
{
	bool	lRet = false;

	if	(
			(pAnimationSequence)
		&&	(pAnimationSequence->mAudio.GetCount() > 0)
		)
	{
		INT_PTR	lAudioNdx;
		INT_PTR	lVideoNdx;
		INT_PTR	lNdx;
		long	lTimeOffset;

#ifdef	_DEBUG_NOT
		for	(lAudioNdx = 0; lAudioNdx < (INT_PTR)pAnimationSequence->mAudio.GetCount(); lAudioNdx++)
		{
			CSeqAudioSegment &	lAudioSegment = pAnimationSequence->mAudio [lAudioNdx];

			if	(lAudioNdx < (INT_PTR)pAnimationSequence->mAudio.GetCount()-1)
			{
				if	(lAudioSegment.mEndTime >= pAnimationSequence->mAudio [lAudioNdx+1].mStartTime)
				{
					LogMessage (LogDebug, _T("  Cue [%2d] End [%6u] Next start [%6u]"), lAudioNdx, lAudioSegment.mEndTime, pAnimationSequence->mAudio [lAudioNdx+1].mStartTime);
				}
			}
			else
			{
				if	(lAudioSegment.mEndTime > pAnimationSequence->mDuration)
				{
					LogMessage (LogDebug, _T("  Cue [%2d] End [%6u] Duration [%6u]"), lAudioNdx, lAudioSegment.mEndTime, pAnimationSequence->mDuration);
				}
			}
		}
#endif
		for	(lAudioNdx = 1; lAudioNdx < (INT_PTR)pAnimationSequence->mAudio.GetCount(); lAudioNdx++)
		{
			CSeqAudioSegment &	lPrevAudioSegment = pAnimationSequence->mAudio [lAudioNdx-1];
			CSeqAudioSegment &	lNextAudioSegment = pAnimationSequence->mAudio [lAudioNdx];

			if	(lPrevAudioSegment.mEndTime > lNextAudioSegment.mStartTime)
			{
				lVideoNdx = lNextAudioSegment.mSequenceFrameNdx;

				if	(
						(lVideoNdx >= 0)
					&&	(lVideoNdx < (INT_PTR)pAnimationSequence->mFrames.GetCount())
					)
				{
					CSeqVideoFrame &	lVideoFrame = pAnimationSequence->mFrames [lVideoNdx];

					if	(lPrevAudioSegment.mEndTime > lVideoFrame.mStartTime)
					{
#ifdef	_DEBUG_SEQUENCE_AUDIO
						LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("  Shift video frame [%d] [%d %d] from [%d] to [%d]"), lVideoNdx, lVideoFrame.mAnimationNdx, lVideoFrame.mFrameNdx, lVideoFrame.mStartTime, lPrevAudioSegment.mEndTime);
						LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("  Shift audio cue   [%d] [%d] from [%d] to [%d]"), lAudioNdx, lNextAudioSegment.mSoundNdx, lNextAudioSegment.mStartTime, lPrevAudioSegment.mEndTime);
#endif
						lTimeOffset = lPrevAudioSegment.mEndTime - lVideoFrame.mStartTime;

						for	(lNdx = lVideoNdx; lNdx < (INT_PTR)pAnimationSequence->mFrames.GetCount(); lNdx++)
						{
							pAnimationSequence->mFrames [lNdx].mStartTime += lTimeOffset;
						}
						for	(lNdx = lAudioNdx; lNdx < (INT_PTR)pAnimationSequence->mAudio.GetCount(); lNdx++)
						{
							pAnimationSequence->mAudio [lNdx].mStartTime += lTimeOffset;
							pAnimationSequence->mAudio [lNdx].mEndTime += lTimeOffset;
						}
						pAnimationSequence->mDuration += lTimeOffset;
						lRet = true;
					}
				}
			}
		}

		CSeqAudioSegment &	lLastAudioSegment = pAnimationSequence->mAudio [(INT_PTR)pAnimationSequence->mAudio.GetCount()-1];

		if	(lLastAudioSegment.mEndTime > pAnimationSequence->mDuration)
		{
#ifdef	_DEBUG_SEQUENCE_AUDIO
			LogMessage (_DEBUG_SEQUENCE_AUDIO, _T("  Audio cue [%d] [%d] pushes duration [%d] to [%d]"), lAudioNdx, lLastAudioSegment.mSoundNdx, pAnimationSequence->mDuration, lLastAudioSegment.mEndTime);
#endif
			lTimeOffset = lLastAudioSegment.mEndTime - pAnimationSequence->mDuration;
			pAnimationSequence->mDuration += lTimeOffset;
			lRet = true;
		}
	}
	return lRet;
}


/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentStreamInfo::LogAnimationSequence (UINT pLogLevel, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);

				::LogAnimationSequence (pLogLevel, GetAnimationSequence(), lTitle);
			}
			else
			{
				::LogAnimationSequence (pLogLevel, GetAnimationSequence());
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentStreamInfo::LogAnimationSequenceFrames (UINT pLogLevel, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);

				::LogAnimationSequenceFrames (pLogLevel, GetAnimationSequence(), lTitle);
			}
			else
			{
				::LogAnimationSequenceFrames (pLogLevel, GetAnimationSequence());
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentStreamInfo::LogAnimationSequenceAudio (UINT pLogLevel, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);

				::LogAnimationSequenceAudio (pLogLevel, GetAnimationSequence(), lTitle);
			}
			else
			{
				::LogAnimationSequenceAudio (pLogLevel, GetAnimationSequence());
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogAnimationSequence (UINT pLogLevel, const CAnimationSequence* pSequence, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
			}
			else
			{
				lTitle = _T("Sequence");
			}

			if	(pSequence)
			{
				LogMessage (pLogLevel, _T("%s [%p] Duration [%d] LoopDuration [%d]"), lTitle, pSequence, pSequence->mDuration, pSequence->mLoopDuration);
				LogAnimationSequenceFrames (pLogLevel, pSequence, _T("  Frames"));
				LogAnimationSequenceAudio (pLogLevel, pSequence, _T("  Audio"));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s [%p]"), lTitle, pSequence);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void LogAnimationSequenceFrames (UINT pLogLevel, const CAnimationSequence* pSequence, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			CAtlString	lIndent;
			INT_PTR		lFrameNdx;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);

				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Sequence Frames");
			}

			if	(pSequence)
			{
				LogMessage (pLogLevel, _T("%s%s [%d]"), lIndent, lTitle, pSequence->mFrames.GetCount());

				for	(lFrameNdx = 0; lFrameNdx < (INT_PTR)pSequence->mFrames.GetCount(); lFrameNdx++)
				{
					const CSeqVideoFrame &	lFrame = pSequence->mFrames [lFrameNdx];
					CAtlTypeArray <DWORD>	lFrameImages;
					WORD					lFrameImageNdx;
					CAtlTypeArray <DWORD>	lFrameOverlays;
					WORD					lFrameOverlayNdx;

					for	(lFrameImageNdx = 0; lFrameImageNdx < lFrame.mFileFrame->ImageCount; lFrameImageNdx++)
					{
						lFrameImages.Add (lFrame.mFileFrame->Images [lFrameImageNdx].ImageNdx);
					}
					for	(lFrameOverlayNdx = 0; lFrameOverlayNdx < lFrame.mFileFrame->OverlayCount; lFrameOverlayNdx++)
					{
						lFrameOverlays.Add (lFrame.mFileFrame->Overlays [lFrameOverlayNdx].ImageNdx);
					}

					LogMessage (pLogLevel, _T("%s  Frame [%3.3d %3.3d] at [%6.6d] for [%3.3d] (Images [%hu] [%s] Overlays [%hu] [%s])"), lIndent, lFrame.mAnimationNdx, lFrame.mFrameNdx, lFrame.mStartTime, (long)(short)lFrame.mFileFrame->Duration, lFrame.mFileFrame->ImageCount, FormatArray(lFrameImages, _T("%3.3d")), lFrame.mFileFrame->OverlayCount, FormatArray(lFrameOverlays, _T("%3.3d")));
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pSequence);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void LogAnimationSequenceAudio (UINT pLogLevel, const CAnimationSequence* pSequence, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			CAtlString	lIndent;
			INT_PTR		lAudioNdx;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);

				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Sequence Audio");
			}

			if	(pSequence)
			{
				LogMessage (pLogLevel, _T("%s%s [%d]"), lIndent, lTitle, pSequence->mAudio.GetCount());

				for	(lAudioNdx = 0; lAudioNdx < (INT_PTR)pSequence->mAudio.GetCount(); lAudioNdx++)
				{
					const CSeqAudioSegment &	lAudio = pSequence->mAudio [lAudioNdx];

					LogMessage (pLogLevel, _T("%s  Sound [%3.3d] Frame [%3.3d] [%3.3d %3.3d] at [%6.6d - %6.6d]"), lIndent, lAudio.mSoundNdx, lAudio.mSequenceFrameNdx, lAudio.mAnimationNdx, lAudio.mFrameNdx, lAudio.mStartTime, lAudio.mEndTime);
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pSequence);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}
