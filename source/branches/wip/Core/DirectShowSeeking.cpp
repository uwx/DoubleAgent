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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal|LogHighVolume
//#define	_DEBUG_SEEKING		LogNormal|LogTimeMs
//#define	_DEBUG_SEEKING_EX	LogNormal|LogTimeMs
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

IMPLEMENT_IUNKNOWN_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

/////////////////////////////////////////////////////////////////////////////

CDirectShowSeeking::CDirectShowSeeking (CCmdTarget & pBase, CDirectShowClock & pClock)
:	mBase (pBase),
	mClock (pClock),
	mSeekingCaps (AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanSeekForwards | AM_SEEKING_CanSeekBackwards | AM_SEEKING_CanGetDuration),
	mCurrTime (0),
	mStopTime (0)
{
}

CDirectShowSeeking::~CDirectShowSeeking()
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSeeking::SetTimes (REFERENCE_TIME pCurrTime, REFERENCE_TIME pStopTime, REFERENCE_TIME pDuration)
{
	HRESULT		lResult = S_OK;
	CSingleLock	lLock (&mLock, TRUE);

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

void CDirectShowSeeking::GetTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime)
{
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
		pCurrTime = mCurrTime;
		pStopTime = mStopTime;
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

void CDirectShowSeeking::GetStreamTimes (REFERENCE_TIME & pStreamTime, REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime, FILTER_STATE pStreamState)
{
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
		REFERENCE_TIME	lReferenceTime = 0;
		REFERENCE_TIME	lStartTime;

		pStreamTime = 0;
		pCurrTime = mCurrTime;
		pStopTime = mStopTime;

		if	(pStreamState == State_Running)
		{
			lReferenceTime = mClock.GetReferenceTime();
		}
		if	(pStreamState != State_Stopped)
		{
			lStartTime = mClock.GetStartTime();

			pStreamTime += lReferenceTime - lStartTime;
			pCurrTime += lReferenceTime - lStartTime;
			pStopTime += lReferenceTime - lStartTime;
		}
	}
	catch AnyExceptionDebug
}

REFERENCE_TIME CDirectShowSeeking::GetCurrTime (FILTER_STATE pStreamState)
{
	REFERENCE_TIME	lRet;
	CSingleLock		lLock (&mLock, TRUE);

	try
	{
		REFERENCE_TIME	lReferenceTime = 0;

		lRet = mCurrTime;

		if	(pStreamState == State_Running)
		{
			lReferenceTime = mClock.GetReferenceTime();
		}
		if	(pStreamState != State_Stopped)
		{
			lRet += lReferenceTime - mClock.GetStartTime();
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

REFERENCE_TIME CDirectShowSeeking::GetStopTime (FILTER_STATE pStreamState)
{
	REFERENCE_TIME	lRet;
	CSingleLock		lLock (&mLock, TRUE);

	try
	{
		REFERENCE_TIME	lReferenceTime = 0;

		lRet = mStopTime;

		if	(pStreamState == State_Running)
		{
			lReferenceTime = mClock.GetReferenceTime();
		}
		if	(pStreamState != State_Stopped)
		{
			lRet += lReferenceTime - mClock.GetStartTime();
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LONGLONG CDirectShowSeeking::GetDuration ()
{
	LONGLONG			lDuration = -1;
	IMediaSeekingPtr	lMediaSeeking;

	if	(GetUpstreamSeeking (&lMediaSeeking))
	{
		lMediaSeeking->GetDuration (&lDuration);
	}
	return lDuration;
}

LONGLONG CDirectShowSeeking::GetPreroll ()
{
	return 0;
}

bool CDirectShowSeeking::GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking)
{
	(*pMediaSeeking) = NULL;
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowSeeking::SetPositions (LONGLONG * pCurrent, DWORD pCurrentFlags, LONGLONG * pStop, DWORD pStopFlags)
{
	HRESULT	lResult = S_OK;

	try
	{
		REFERENCE_TIME	lCurrTime;
		REFERENCE_TIME	lStopTime;

		GetTimes (lCurrTime, lStopTime);

		if	(pCurrent)
		{
			if	((pCurrentFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_AbsolutePositioning)
			{
				lCurrTime = *pCurrent;
			}
			else
			if	((pCurrentFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning)
			{
				lCurrTime += *pCurrent;
			}
		}
		if	(pStop)
		{
			if	((pStopFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_AbsolutePositioning)
			{
				lStopTime = *pStop;
			}
			else
			if	((pStopFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning)
			{
				lStopTime += *pStop;
			}
			else
			if	((pStopFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_IncrementalPositioning)
			{
				lStopTime = lCurrTime + *pStop;
			}
		}

		OnTimesChanging (lCurrTime, pCurrentFlags, lStopTime, pStopFlags);

		lResult = SetTimes (lCurrTime, lStopTime, GetDuration());

		GetTimes (lCurrTime, lStopTime);
		OnTimesChanged (lCurrTime, pCurrentFlags, lStopTime, pStopFlags);

		if	(
				(pCurrent)
			&&	(pCurrentFlags & AM_SEEKING_ReturnTime)
			)
		{
			*pCurrent = lCurrTime;
		}
		if	(
				(pStop)
			&&	(pStopFlags & AM_SEEKING_ReturnTime)
			)
		{
			*pStop = lStopTime;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

void CDirectShowSeeking::OnTimesChanging (REFERENCE_TIME & pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME & pStopTime, DWORD pStopFlags)
{
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%s] [%p] Curr [%f %I64d] [%s] [%f %I64d]"), ObjClassName(&mBase), &mBase, RefTimeSec(mCurrTime), mCurrTime, SeekingFlagsStr(pCurrentFlags), RefTimeSec(pCurrTime), pCurrTime);
	LogMessage (_DEBUG_SEEKING, _T("[%s] [%p] Stop [%f %I64d] [%s] [%f %I64d]"), ObjClassName(&mBase), &mBase, RefTimeSec(mStopTime), mStopTime, SeekingFlagsStr(pStopFlags), RefTimeSec(pStopTime), pStopTime);
#endif
}

void CDirectShowSeeking::OnTimesChanged (REFERENCE_TIME pCurrTime, DWORD pCurrentFlags, REFERENCE_TIME pStopTime, DWORD pStopFlags)
{
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%s] [%p] Curr [%f %I64d] Stop [%f %I64d] of [%f %I64d]"), ObjClassName(&mBase), &mBase, RefTimeSec(mCurrTime), mCurrTime, RefTimeSec(mStopTime), mStopTime, RefTimeSec(GetDuration()), GetDuration());
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetCapabilities (DWORD *pCapabilities)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::MediaSeeking::GetCapabilities"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif
	HRESULT	lResult = S_OK;

	if	(!pCapabilities)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pCapabilities) = pThis->mSeekingCaps;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetCapabilities"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::CheckCapabilities (DWORD *pCapabilities)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::CheckCapabilities"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif
	HRESULT	lResult = E_FAIL;

	if	(!pCapabilities)
	{
		lResult = E_POINTER;
	}
	else
	if	((*pCapabilities & pThis->mSeekingCaps) == pThis->mSeekingCaps)
	{
		lResult = S_OK;
	}
	else
	if	((*pCapabilities & pThis->mSeekingCaps) != 0)
	{
		lResult = S_FALSE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::CheckCapabilities"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::IsFormatSupported (const GUID *pFormat)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::IsFormatSupported"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
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
		if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->IsFormatSupported (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::IsFormatSupported"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::QueryPreferredFormat (GUID *pFormat)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::QueryPreferredFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
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
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::QueryPreferredFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetTimeFormat (GUID *pFormat)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::GetTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
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
		if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->GetTimeFormat (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::IsUsingTimeFormat (const GUID *pFormat)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::IsUsingTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
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
		if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->IsUsingTimeFormat (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::IsUsingTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::SetTimeFormat (const GUID *pFormat)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%p(%d)] %s::XMediaSeeking::SetTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif
	HRESULT				lResult = E_FAIL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pFormat)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(pThis->mClock.IsClockSet ())
		||	(pThis->mClock.IsClockStarted ())
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
		if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->SetTimeFormat (pFormat);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::SetTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::ConvertTimeFormat (LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::ConvertTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif
	HRESULT				lResult = E_NOTIMPL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
	{
		lResult = lMediaSeeking->ConvertTimeFormat (pTarget, pTargetFormat, Source, pSourceFormat);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogVfwErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] %s::XMediaSeeking::ConvertTimeFormat"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetDuration (LONGLONG *pDuration)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

	HRESULT	lResult = S_OK;

	if	(!pDuration)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pDuration = pThis->GetDuration ();
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pDuration)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetDuration [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), RefTimeSec(*pDuration));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetDuration"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), *pDuration);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetStopPosition (LONGLONG *pStop)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;
	REFERENCE_TIME		lCurrTime;

	if	(!pStop)
	{
		lResult = E_POINTER;
	}
	else
	{
		pThis->GetTimes (lCurrTime, *pStop);

		if	(
				(
					((pThis->mSeekingCaps & AM_SEEKING_CanGetStopPos) == 0)
				||	(*pStop < 0)
				)
			&&	(pThis->GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetStopPosition (pStop);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pStop)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetStopPosition [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), RefTimeSec(*pStop));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetStopPosition"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetCurrentPosition (LONGLONG *pCurrent)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;
	REFERENCE_TIME		lStopTime;

	if	(!pCurrent)
	{
		lResult = E_POINTER;
	}
	else
	{
		pThis->GetTimes (*pCurrent, lStopTime);

		if	(
				((pThis->mSeekingCaps & AM_SEEKING_CanGetCurrentPos) == 0)
			&&	(pThis->GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetCurrentPosition (pCurrent);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pCurrent)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetCurrentPosition [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), RefTimeSec(*pCurrent));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetCurrentPosition"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::SetPositions (LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING
	LogMessage (_DEBUG_SEEKING, _T("[%p(%d)] %s::XMediaSeeking::SetPositions"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif

	HRESULT	lResult = pThis->SetPositions (pCurrent, dwCurrentFlags, pStop, dwStopFlags);

#ifdef	_DEBUG_SEEKING_EX
	LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::SetPositions"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetPositions (LONGLONG *pCurrent, LONGLONG *pStop)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

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
					((pThis->mSeekingCaps & AM_SEEKING_CanGetCurrentPos) == 0)
				||	((pThis->mSeekingCaps & AM_SEEKING_CanGetStopPos) == 0)
				)
			&&	(pThis->GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetPositions (pCurrent, pStop);
		}

		pThis->GetTimes (lCurrTime, lStopTime);

		if	(
				(pCurrent)
			&&	((pThis->mSeekingCaps & AM_SEEKING_CanGetCurrentPos) != 0)
			)
		{
			*pCurrent = lCurrTime;
		}
		if	(
				(pStop)
			&&	((pThis->mSeekingCaps & AM_SEEKING_CanGetStopPos) != 0)
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
	LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetPositions {%f] [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), RefTimeSec(lCurrTime), RefTimeSec(lStopTime));
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetAvailable (LONGLONG *pEarliest, LONGLONG *pLatest)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

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

	if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
	{
		lResult = lMediaSeeking->GetAvailable (pEarliest, pLatest);
	}

#ifdef	_DEBUG_SEEKING_EX
	LONGLONG	lEarliest = pEarliest ? *pEarliest : -1;
	LONGLONG	lLatest = pLatest ? *pLatest : -1;
	LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetAvailable [%f] [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), RefTimeSec(lEarliest), RefTimeSec(lLatest));
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::SetRate (double dRate)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)
#ifdef	_DEBUG_SEEKING_EX
	LogMessage (_DEBUG_SEEKING_EX, _T("[%p(%d)] %s::XMediaSeeking::SetRate [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), dRate);
#endif
	HRESULT				lResult = E_NOTIMPL;
	IMediaSeekingPtr	lMediaSeeking;

	if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
	{
		lResult = lMediaSeeking->SetRate (dRate);
	}

#ifdef	_DEBUG_SEEKING_EX
	LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::SetRate"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetRate (double *pdRate)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pdRate)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pdRate = 1.0;

		if	(pThis->GetUpstreamSeeking (&lMediaSeeking))
		{
			lResult = lMediaSeeking->GetRate (pdRate);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pdRate)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetRate [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), *pdRate);
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetRate"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDirectShowSeeking::XMediaSeeking::GetPreroll (LONGLONG *pllPreroll)
{
	METHOD_PROLOGUE_INHERITED(CDirectShowSeeking, MediaSeeking, mBase)

	HRESULT				lResult = S_OK;
	IMediaSeekingPtr	lMediaSeeking;

	if	(!pllPreroll)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pllPreroll) = pThis->GetPreroll ();

		if	(
				(*pllPreroll == 0)
			&&	(pThis->GetUpstreamSeeking (&lMediaSeeking))
			)
		{
			lResult = lMediaSeeking->GetPreroll (pllPreroll);
		}
	}

#ifdef	_DEBUG_SEEKING_EX
	if	(pllPreroll)
	{
		LogVfwErrAnon (MinLogLevel(_DEBUG_SEEKING_EX,LogAlways), lResult, _T("[%p(%d)] %s::XMediaSeeking::GetPreroll [%f]"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase), RefTimeSec(*pllPreroll));
	}
	else
	{
		LogVfwErrAnon (_DEBUG_SEEKING_EX, lResult, _T("[%p(%d)] %s::XMediaSeeking::GetPreroll"), &pThis->mBase, pThis->mBase.m_dwRef, ObjClassName(&pThis->mBase));
	}
#endif
	return lResult;
}
