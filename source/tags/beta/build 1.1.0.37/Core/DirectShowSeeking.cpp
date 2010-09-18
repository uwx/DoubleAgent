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
#include "StdAfx.h"
#include "DaCore.h"
#include "DirectShowSeeking.h"
#include "DirectShowUtils.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_SEEKING		LogNormal|LogTimeMs
//#define	_DEBUG_SEEKING_EX	LogNormal|LogTimeMs
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectShowSeekingImpl::CDirectShowSeekingImpl ()
:	mBase (NULL),
	mClock (NULL),
	mSeekingCaps (AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanSeekForwards | AM_SEEKING_CanSeekBackwards | AM_SEEKING_CanGetDuration),
	mCurrTime (0),
	mStopTime (0)
{
}

CDirectShowSeekingImpl::~CDirectShowSeekingImpl()
{
}

CDirectShowSeekingImpl & CDirectShowSeekingImpl::Initialize (CComObjectRootBase & pBase, CDirectShowClock & pClock, LPCTSTR pBaseClassName)
{
	mBase = &pBase;
	mClock = &pClock;

	mBaseClassName = pBaseClassName;
	if	(mBaseClassName.IsEmpty ())
	{
		mBaseClassName = AtlTypeName(mBase);
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSeekingImpl::SetTimes (REFERENCE_TIME pCurrTime, REFERENCE_TIME pStopTime, REFERENCE_TIME pDuration)
{
	HRESULT	lResult = S_OK;
	CLockCS	lLock (mLock);

	try
	{
		if	(
				(pCurrTime < 0)
			||	(pStopTime < pCurrTime)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		{
			mCurrTime = pCurrTime;
			mStopTime = pStopTime;

			if	(pDuration > 0)
			{
				mCurrTime = min (mCurrTime, pDuration);
				mStopTime = min (mStopTime, pDuration);
			}

			mStopTime = max (mStopTime, mCurrTime);
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

void CDirectShowSeekingImpl::GetTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime)
{
	CLockCS	lLock (mLock);

	try
	{
		pCurrTime = mCurrTime;
		pStopTime = mStopTime;
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSeekingImpl::GetStreamTimes (REFERENCE_TIME & pStreamTime, REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime, FILTER_STATE pStreamState)
{
	CLockCS	lLock (mLock);

	try
	{
		pStreamTime = 0;
		pCurrTime = mCurrTime;
		pStopTime = mStopTime;

		if	(pStreamState != State_Stopped)
		{
			REFERENCE_TIME	lReferenceTime = 0;
			REFERENCE_TIME	lStartTime;

			if	(pStreamState == State_Running)
			{
				lReferenceTime = mClock->GetReferenceTime();
			}
			lStartTime = mClock->GetStartTime();

			pStreamTime += lReferenceTime - lStartTime;
			pCurrTime += lReferenceTime - lStartTime;
			pStopTime += lReferenceTime - lStartTime;
		}
	}
	catch AnyExceptionDebug
}

REFERENCE_TIME CDirectShowSeekingImpl::GetCurrTime (FILTER_STATE pStreamState)
{
	REFERENCE_TIME	lRet;
	CLockCS			lLock (mLock);

	try
	{
		lRet = mCurrTime;

		if	(pStreamState != State_Stopped)
		{
			REFERENCE_TIME	lReferenceTime = 0;
			if	(pStreamState == State_Running)
			{
				lReferenceTime = mClock->GetReferenceTime();
			}
			lRet += lReferenceTime - mClock->GetStartTime();
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

REFERENCE_TIME CDirectShowSeekingImpl::GetStopTime (FILTER_STATE pStreamState)
{
	REFERENCE_TIME	lRet;
	CLockCS			lLock (mLock);

	try
	{
		lRet = mStopTime;

		if	(pStreamState != State_Stopped)
		{
			REFERENCE_TIME	lReferenceTime = 0;

			if	(pStreamState == State_Running)
			{
				lReferenceTime = mClock->GetReferenceTime();
			}
			lRet += lReferenceTime - mClock->GetStartTime();
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LONGLONG CDirectShowSeekingImpl::GetDuration ()
{
	LONGLONG			lDuration = -1;
	IMediaSeekingPtr	lMediaSeeking;

	if	(GetUpstreamSeeking (&lMediaSeeking))
	{
		lMediaSeeking->GetDuration (&lDuration);
	}
	return lDuration;
}

LONGLONG CDirectShowSeekingImpl::GetPreroll ()
{
	return 0;
}

bool CDirectShowSeekingImpl::GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking)
{
	(*pMediaSeeking) = NULL;
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDirectShowSeekingImpl::OnTimesChanging (REFERENCE_TIME & pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME & pStopTime, DWORD pStopFlags)
{
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%s] [%p] Curr [%f %I64d] [%s] [%f %I64d]"), mBaseClassName, mBase, RefTimeSec(mCurrTime), mCurrTime, SeekingFlagsStr(pCurrentFlags), RefTimeSec(pCurrTime), pCurrTime);
	LogMessage (_DEBUG_SEEKING, _T("[%s] [%p] Stop [%f %I64d] [%s] [%f %I64d]"), mBaseClassName, mBase, RefTimeSec(mStopTime), mStopTime, SeekingFlagsStr(pStopFlags), RefTimeSec(pStopTime), pStopTime);
#endif
}

void CDirectShowSeekingImpl::OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags)
{
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%s] [%p] Curr [%f %I64d] Stop [%f %I64d] of [%f %I64d]"), mBaseClassName, mBase, RefTimeSec(mCurrTime), mCurrTime, RefTimeSec(mStopTime), mStopTime, RefTimeSec(GetDuration()), GetDuration());
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetCapabilities (DWORD *pCapabilities)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::MediaSeeking::GetCapabilities"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pCapabilities)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pCapabilities) = mSeekingCaps;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetCapabilities"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::CheckCapabilities (DWORD *pCapabilities)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::CheckCapabilities"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT	lResult = E_FAIL;

	if	(!pCapabilities)
	{
		lResult = E_POINTER;
	}
	else
	if	((*pCapabilities & mSeekingCaps) == mSeekingCaps)
	{
		lResult = S_OK;
	}
	else
	if	((*pCapabilities & mSeekingCaps) != 0)
	{
		lResult = S_FALSE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::CheckCapabilities"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::IsFormatSupported (const GUID *pFormat)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::IsFormatSupported"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT				lResult = S_FALSE;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pFormat)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(IsEqualGUID (*pFormat, TIME_FORMAT_MEDIA_TIME))
		{
			lResult = S_OK;
		}
		if	(GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->IsFormatSupported (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::IsFormatSupported"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::QueryPreferredFormat (GUID *pFormat)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::QueryPreferredFormat"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pFormat)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pFormat = TIME_FORMAT_MEDIA_TIME;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::QueryPreferredFormat"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetTimeFormat (GUID *pFormat)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::GetTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pFormat)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pFormat = TIME_FORMAT_MEDIA_TIME;
		if	(GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->GetTimeFormat (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::GetTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::IsUsingTimeFormat (const GUID *pFormat)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::IsUsingTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT				lResult = S_FALSE;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pFormat)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(IsEqualGUID (*pFormat, TIME_FORMAT_MEDIA_TIME))
		{
			lResult = S_OK;
		}
		if	(GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->IsUsingTimeFormat (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::IsUsingTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::SetTimeFormat (const GUID *pFormat)
{
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%p(%d)] %s::SetTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT				lResult = E_FAIL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pFormat)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(mClock->IsClockSet ())
		||	(mClock->IsClockStarted ())
		)
	{
		lResult = VFW_E_WRONG_STATE;
	}
	else
	{
		if	(IsEqualGUID (*pFormat, TIME_FORMAT_MEDIA_TIME))
		{
			lResult = S_OK;
		}
		if	(GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->SetTimeFormat (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::SetTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::ConvertTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT				lResult = E_NOTIMPL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(GetUpstreamSeeking (&lMediaSeeking))
	{
		lResult = lMediaSeeking->ConvertTimeFormat (pTarget, pTargetFormat, Source, pSourceFormat);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::ConvertTimeFormat"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetDuration (LONGLONG *pDuration)
{
	HRESULT	lResult = S_OK;

	if	(!pDuration)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDuration = GetDuration ();
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pDuration)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetDuration [%f]"), mBase, mBase->m_dwRef, mBaseClassName, RefTimeSec(*pDuration));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::GetDuration"), mBase, mBase->m_dwRef, mBaseClassName, *pDuration);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetStopPosition (LONGLONG *pStop)
{
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;
	REFERENCE_TIME		lCurrTime;

	if	(!pStop)
	{
		lResult = E_POINTER;
	}
	else
	{
		GetTimes (lCurrTime, *pStop);

		if	(
				(
					((mSeekingCaps & AM_SEEKING_CanGetStopPos) == 0)
				||	(*pStop < 0)
				)
			&&	(GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetStopPosition (pStop);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pStop)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetStopPosition [%f]"), mBase, mBase->m_dwRef, mBaseClassName, RefTimeSec(*pStop));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::GetStopPosition"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetCurrentPosition (LONGLONG *pCurrent)
{
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;
	REFERENCE_TIME		lStopTime;

	if	(!pCurrent)
	{
		lResult = E_POINTER;
	}
	else
	{
		GetTimes (*pCurrent, lStopTime);

		if	(
				((mSeekingCaps & AM_SEEKING_CanGetCurrentPos) == 0)
			&&	(GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetCurrentPosition (pCurrent);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pCurrent)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetCurrentPosition [%f]"), mBase, mBase->m_dwRef, mBaseClassName, RefTimeSec(*pCurrent));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::GetCurrentPosition"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags)
{
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%p(%d)] %s::SetPositions"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	HRESULT	lResult = S_OK;

	try
	{
		REFERENCE_TIME	lCurrTime;
		REFERENCE_TIME	lStopTime;

		GetTimes (lCurrTime, lStopTime);

		if	(pCurrent)
		{
			if	((dwCurrentFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_AbsolutePositioning)
			{
				lCurrTime = *pCurrent;
			}
			else
			if	((dwCurrentFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning)
			{
				lCurrTime += *pCurrent;
			}
		}
		if	(pStop)
		{
			if	((dwStopFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_AbsolutePositioning)
			{
				lStopTime = *pStop;
			}
			else
			if	((dwStopFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning)
			{
				lStopTime += *pStop;
			}
			else
			if	((dwStopFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_IncrementalPositioning)
			{
				lStopTime = lCurrTime + *pStop;
			}
		}

		OnTimesChanging (lCurrTime, dwCurrentFlags, lStopTime, dwStopFlags);

		lResult = SetTimes (lCurrTime, lStopTime, GetDuration());

		GetTimes (lCurrTime, lStopTime);
		OnTimesChanged (lCurrTime, dwCurrentFlags, lStopTime, dwStopFlags);

		if	(
				(pCurrent)
			&&	(dwCurrentFlags & AM_SEEKING_ReturnTime)
			)
		{
			*pCurrent = lCurrTime;
		}
		if	(
				(pStop)
			&&	(dwStopFlags & AM_SEEKING_ReturnTime)
			)
		{
			*pStop = lStopTime;
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_SEEKING_EX
	LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::SetPositions"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetPositions (LONGLONG *pCurrent, LONGLONG *pStop)
{
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;
	REFERENCE_TIME		lCurrTime;
	REFERENCE_TIME		lStopTime;

	try
	{
		if	(pCurrent)
		{
			*pCurrent = 0;
		}
		if	(pStop)
		{
			*pStop = 0;
		}

		if	(
				(
					((mSeekingCaps & AM_SEEKING_CanGetCurrentPos) == 0)
				||	((mSeekingCaps & AM_SEEKING_CanGetStopPos) == 0)
				)
			&&	(GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetPositions (pCurrent, pStop);
		}

		GetTimes (lCurrTime, lStopTime);

		if	(
				(pCurrent)
			&&	((mSeekingCaps & AM_SEEKING_CanGetCurrentPos) != 0)
			)
		{
			*pCurrent = lCurrTime;
		}
		if	(
				(pStop)
			&&	((mSeekingCaps & AM_SEEKING_CanGetStopPos) != 0)
			&&	(lStopTime >= 0)
			)
		{
			*pStop = lStopTime;
		}
		lResult = S_OK;
	}
	catch AnyExceptionSilent

#ifdef	_DEBUG_SEEKING_EX
	lCurrTime = pCurrent ? *pCurrent : -1;
	lStopTime = pStop ? *pStop : -1;
	LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetPositions {%f] [%f]"), mBase, mBase->m_dwRef, mBaseClassName, RefTimeSec(lCurrTime), RefTimeSec(lStopTime));
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest)
{
	HRESULT				lResult = E_NOTIMPL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(pEarliest)
	{
		*pEarliest = 0;
	}
	if	(pLatest)
	{
		*pLatest = 0;
	}

	if	(GetUpstreamSeeking (&lMediaSeeking))
	{
		lResult = lMediaSeeking->GetAvailable (pEarliest, pLatest);
	}

#ifdef	_DEBUG_SEEKING_EX
	LONGLONG	lEarliest = pEarliest ? *pEarliest : -1;
	LONGLONG	lLatest = pLatest ? *pLatest : -1;
	LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetAvailable [%f] [%f]"), mBase, mBase->m_dwRef, mBaseClassName, RefTimeSec(lEarliest), RefTimeSec(lLatest));
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::SetRate (double dRate)
{
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::SetRate [%f]"), mBase, mBase->m_dwRef, mBaseClassName, dRate);
#endif
	HRESULT				lResult = E_NOTIMPL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(GetUpstreamSeeking (&lMediaSeeking))
	{
		lResult = lMediaSeeking->SetRate (dRate);
	}

#ifdef	_DEBUG_SEEKING_EX
	LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::SetRate"), mBase, mBase->m_dwRef, mBaseClassName);
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetRate (double *pdRate)
{
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pdRate)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pdRate = 1.0;

		if	(GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->GetRate (pdRate);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pdRate)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetRate [%f]"), mBase, mBase->m_dwRef, mBaseClassName, *pdRate);
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::GetRate"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeekingImpl::GetPreroll (LONGLONG *pllPreroll)
{
	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pllPreroll)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pllPreroll) = GetPreroll ();

		if	(
				(*pllPreroll == 0)
			&&	(GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetPreroll (pllPreroll);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pllPreroll)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::GetPreroll [%f]"), mBase, mBase->m_dwRef, mBaseClassName, RefTimeSec(*pllPreroll));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::GetPreroll"), mBase, mBase->m_dwRef, mBaseClassName);
	}
#endif
	return lResult;
}
