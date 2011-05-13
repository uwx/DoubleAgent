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
#include "StdAfx.h"
#include <initguid.h>
#include <speech.h>
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "AgentFile.h"
#include "Elapsed.h"
#include "DebugStr.h"
#include "LocalizeEx.h"
#include "DaVersion.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#define	_SAPI4_LOGGING

/////////////////////////////////////////////////////////////////////////////
#ifdef	_SAPI4_LOGGING
/////////////////////////////////////////////////////////////////////////////
#define	_SAPI4_LOG	Sapi4Log
namespace _SAPI4_LOG
{
#define	_LOG_ROOT_PATH		_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME	_T("Sapi4")
#define _LOG_DEF_LOGNAME	_LOG_SECTION_NAME _T(".log")
#define	_LOG_PREFIX			_T("Sapi4   ")

#define	_LOG_NO_EXTERN_C
#undef	_LOG_H
#include "Log.h"
#include "Log.inl"
#undef	_SAPI4ERR_H
#include "Sapi4Err.h"
#undef	_LOG_NO_EXTERN_C
}
#else
#define	_SAPI4_LOG
/////////////////////////////////////////////////////////////////////////////
#endif	// _SAPI4_LOGGING
/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
//#define	_DEBUG_MOUTH	LogNormal|LogTimeMs|LogHighVolume
//#define	_LOG_NOTIFY		LogNormal|LogTimeMs
#define	_TRACE_EVENTS		(GetProfileDebugInt(_T("TraceSapi4Events"),LogDetails,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_TRACE_EVENTS_EX	(GetProfileDebugInt(_T("TraceSapi4Events"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_TRACE_STATE		(GetProfileDebugInt(_T("TraceSapi4State"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_TRACE_STOP			(GetProfileDebugInt(_T("TraceSapi4Stop"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#endif

#ifdef	_SAPI4_LOGGING
#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogIfActive|LogTimeMs
#endif
#ifndef	_LOG_NOTIFY
#define	_LOG_NOTIFY			LogVerbose|LogTimeMs
#endif
#ifndef	_LOG_SPEECH
#define	_LOG_SPEECH			LogNormal|LogTimeMs|LogHighVolume
#endif
#ifndef	_TRACE_EVENTS
#define	_TRACE_EVENTS		LogDetails|LogTimeMs|LogHighVolume
#endif
#ifndef	_TRACE_EVENTS_EX
#define	_TRACE_EVENTS_EX	LogVerbose|LogTimeMs|LogHighVolume
#endif
#ifndef	_TRACE_STOP
#define	_TRACE_STOP			LogVerbose|LogTimeMs|LogHighVolume
#endif
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ITTSEnum, IID_ITTSEnum);
_COM_SMARTPTR_TYPEDEF (ITTSAttributes, IID_ITTSAttributes);

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi4Voice)

CSapi4Voice::CSapi4Voice ()
:	mDefaultRate (0),
	mDefaultVolume (0),
	mDefaultPitch (0),
	mPaused (false)
{
#ifdef	_SAPI4_LOGGING
	_SAPI4_LOG::LogStart ();
#endif
#ifdef	_LOG_INSTANCE
	if	(_SAPI4_LOG::LogIsActive (_LOG_INSTANCE))
	{
		_SAPI4_LOG::LogMessage (_LOG_INSTANCE, _T("[%p] CSapi4Voice"), this);
	}
#endif
}

CSapi4Voice::~CSapi4Voice ()
{
#ifdef	_TRACE_STOP
	if	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
	{
		_SAPI4_LOG::LogMessage (_TRACE_STOP, _T("[%p] Destructor [%ls] IsResetting [%u] IsQueuing [%u] IsSpeaking [%u]"), this, (BSTR)GetDisplayName(), CheckIsResetting(), CheckIsQueueing(), CheckIsSpeaking());
	}
#endif

	Stop ();
	SafeFreeSafePtr (mAudioDest);
	SafeFreeSafePtr (mEngine);
	SafeFreeSafePtr (mNotifySink);

#ifdef	_LOG_INSTANCE
	if	(_SAPI4_LOG::LogIsActive (_LOG_INSTANCE))
	{
		_SAPI4_LOG::LogMessage (_LOG_INSTANCE, _T("[%p] ~CSapi4Voice"), this);
	}
#endif
}

CSapi4Voice * CSapi4Voice::CreateInstance ()
{
	return new CSapi4Voice;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

UINT CSapi4Voice::_IsValid () const
{
	return (mEngine != NULL) ? 4 : 0;
}

bool CSapi4Voice::_IsPrepared () const
{
	return ((mEngine != NULL) && (mNotifySink->SafeIsConnected ()));
}

bool CSapi4Voice::_IsSpeaking () const
{
	if	(
			(_IsValid ())
		&&	(
				(mIsQueueing.Ptr())
			||	(mIsSpeaking.Ptr())
			||	(
					(mResetPending.Ptr())
				&&	(!mIsParsing.Ptr())
				)
			)
		)
	{
#ifdef	_TRACE_STATE
		if	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
		{
			long	lQueueingElapsed = 0;
			long	lParsingElapsed = 0;
			long	lSpeakingElapsed = 0;
			long	lResettingElapsed = 0;
			long	lElapsed = 0;

			if	(mIsQueueing)
			{
				lQueueingElapsed = ElapsedTicks (*mIsQueueing);
				lElapsed = max (lElapsed, lQueueingElapsed);
			}
			if	(mIsParsing)
			{
				lParsingElapsed = ElapsedTicks (*mIsParsing);
				lElapsed = max (lElapsed, lParsingElapsed);
			}
			if	(mIsSpeaking)
			{
				lSpeakingElapsed = ElapsedTicks (*mIsSpeaking);
				lElapsed = max (lElapsed, lSpeakingElapsed);
			}
			if	(mResetPending)
			{
				lResettingElapsed = ElapsedTicks (*mResetPending);
				lElapsed = max (lElapsed, lResettingElapsed);
			}
			if	(lElapsed >= 300000)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] *** BusyTime %f [Queueing %f Parsing %f Speaking %f Resetting %f] ***"), this, ((double)lElapsed)/1000.0, ((double)lQueueingElapsed)/1000.0, ((double)lParsingElapsed)/1000.0, ((double)lSpeakingElapsed)/1000.0, ((double)lResettingElapsed)/1000.0);
				_SAPI4_LOG::LogWriteCache ();
				const_cast <CSapi4Voice *> (this)->mIsQueueing = NULL;
				const_cast <CSapi4Voice *> (this)->mIsParsing = NULL;
				const_cast <CSapi4Voice *> (this)->mIsSpeaking = NULL;
				const_cast <CSapi4Voice *> (this)->mResetPending = NULL;
			}
		}
#endif
		return true;
	}
	return false;
}

bool CSapi4Voice::_IsPaused () const
{
	return ((mEngine != NULL) && mPaused);
}

/////////////////////////////////////////////////////////////////////////////

bool CSapi4Voice::CheckIsQueueing () const
{
	if	(this)
	{
		return (mIsQueueing.Ptr() != NULL);
	}
	return false;
}

bool CSapi4Voice::CheckIsParsing () const
{
	if	(this)
	{
		return (mIsParsing.Ptr() != NULL);
	}
	return false;
}

bool CSapi4Voice::CheckIsSpeaking () const
{
	if	(this)
	{
		return (mIsSpeaking.Ptr() != NULL);
	}
	return false;
}

bool CSapi4Voice::CheckIsResetting () const
{
	if	(this)
	{
		return (mResetPending.Ptr() != NULL);
	}
	return false;
}

void CSapi4Voice::SetIsQueueing (bool pIsQueueing)
{
	if	(this)
	{
		if	(pIsQueueing)
		{
#ifdef	_TRACE_STATE
			if	(
					(!mIsQueueing)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Start Queueing"), this);
			}
#endif
			mIsQueueing = new DWORD;
			(*mIsQueueing) = GetTickCount ();
		}
		else
		{
#ifdef	_TRACE_STATE
			if	(
					(mIsQueueing)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Stop  Queueing"), this);
			}
#endif
			mIsQueueing = NULL;
		}
	}
}

void CSapi4Voice::SetIsParsing (bool pIsParsing)
{
	if	(this)
	{
		if	(pIsParsing)
		{
#ifdef	_TRACE_STATE
			if	(
					(!mIsParsing)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Start Parsing"), this);
			}
#endif
			mIsParsing = new DWORD;
			(*mIsParsing) = GetTickCount ();
		}
		else
		{
#ifdef	_TRACE_STATE
			if	(
					(mIsParsing)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Stop  Parsing"), this);
			}
#endif
			mIsParsing = NULL;
		}
	}
}

void CSapi4Voice::SetIsSpeaking (bool pIsSpeaking)
{
	if	(this)
	{
		if	(pIsSpeaking)
		{
#ifdef	_TRACE_STATE
			if	(
					(!mIsSpeaking)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Start Speaking"), this);
			}
#endif
			mIsSpeaking = new DWORD;
			(*mIsSpeaking) = GetTickCount ();
		}
		else
		{
#ifdef	_TRACE_STATE
			if	(
					(mIsSpeaking)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Stop  Speaking"), this);
			}
#endif
			mIsSpeaking = NULL;
		}
	}
}

void CSapi4Voice::SetIsResetting (bool pIsResetting)
{
	if	(this)
	{
		if	(pIsResetting)
		{
#ifdef	_TRACE_STATE
			if	(
					(!mResetPending)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Start Resetting"), this);
			}
#endif
			mResetPending = new DWORD;
			(*mResetPending) = GetTickCount ();
		}
		else
		{
#ifdef	_TRACE_STATE
			if	(
					(mResetPending)
				&&	(_SAPI4_LOG::LogIsActive (_TRACE_STATE))
				)
			{
				_SAPI4_LOG::LogMessage (_TRACE_STATE, _T("[%p] Stop  Resetting"), this);
			}
#endif
			mResetPending = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi4Voice::PrepareToSpeak (bool pHighPriority)
{
	HRESULT	lResult = E_UNEXPECTED;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(_IsValid ())
	{
		lResult = S_OK;

		if	(
				(!mNotifySink->SafeIsConnected ())
			&&	(mNotifySink = new CTTSNotifySink (*this))
			)
		{
			lResult = mNotifySink->Connect ();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
		if	(
				(SUCCEEDED (lResult))
			&&	(pHighPriority)
			)
		{
			Stop ();
		}
		if	(SUCCEEDED (lResult))
		{
			lResult = S_OK;
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f     CSapi4Voice::PrepareToSpeak [%8.8X]"), DebugTimeElapsed, lResult);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi4Voice::Speak (LPCTSTR pMessage, bool pAsync)
{
	HRESULT	lResult = E_UNEXPECTED;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif
	if	(_IsValid ())
	{
		if	(_IsPaused ())
		{
			lResult = TTSERR_ALREADYPAUSED;
		}
		else
		{
			try
			{
				tS <SDATA>	lSpeechData;
				CAtlString	lMessage (pMessage);

#ifdef	_TRACE_STOP
				if	(
						(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
					&&	(CheckIsQueueing ())
					&&	(CheckIsResetting ())
					)
				{
					_SAPI4_LOG::LogMessage (_TRACE_STOP, _T("[%p] ResetPending at Speak"), this);
				}
#endif
#ifdef	_LOG_SPEECH
				if	(_SAPI4_LOG::LogIsActive (_LOG_SPEECH))
				{
					_SAPI4_LOG::LogMessage (_LOG_SPEECH, _T("[%p] TextData [%s]"), this, DebugStr(lMessage));
				}
#endif
				SetIsQueueing (true);
				SetIsParsing (false);

				mLastText = AtlAllocTaskWideString (lMessage);
				lSpeechData.pData = (PVOID)mLastText.Ptr();
				lSpeechData.dwSize = (lMessage.GetLength() + 1) * sizeof(WCHAR);
				lResult = mEngine->TextData (CHARSET_TEXT, TTSDATAFLAG_TAGGED, lSpeechData, (ITTSBufNotifySink *)mNotifySink.Ptr(), IID_ITTSBufNotifySink);
#ifdef	_TRACE_STOP
				if	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
				{
					_SAPI4_LOG::LogSapi4ErrAnon (MinLogLevel(_TRACE_STOP,LogAlways), lResult, _T("[%p] TextData"), this);
				}
				else
#endif
				{
					_SAPI4_LOG::LogSapi4Err (LogIfActive|LogTime, lResult, _T("[%p] TextData [%s]"), this, DebugStr(lMessage));
				}
			}
			catch AnyExceptionDebug

			if	(FAILED (lResult))
			{
				SetIsQueueing (false);
			}
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f     CSapi4Voice::Speak [%8.8X] [%s]"), DebugTimeElapsed, lResult, DebugStr(pMessage));
#endif
	return lResult;
}

HRESULT CSapi4Voice::Stop ()
{
	HRESULT	lResult = E_FAIL;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(_IsValid ())
	{
		try
		{
			if	(
					(
						(CheckIsQueueing())
					||	(CheckIsSpeaking())
					||	(CheckIsParsing())
					)
				&&	(!CheckIsResetting())
				)
			{
				SetIsResetting (CheckIsParsing());
#ifdef	_TRACE_STOP
				if	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
				{
					_SAPI4_LOG::LogMessage (_TRACE_STOP, _T("[%p] AudioReset IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), this, CheckIsQueueing(), CheckIsParsing(), CheckIsSpeaking(), CheckIsResetting());
				}
#endif
				lResult = mEngine->AudioReset ();

#ifdef	_TRACE_STOP
				if	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
				{
					_SAPI4_LOG::LogSapi4ErrAnon (MinLogLevel(_TRACE_STOP,LogAlways), lResult, _T("[%p] AudioReset IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), this, CheckIsQueueing(), CheckIsParsing(), CheckIsSpeaking(), CheckIsResetting());
				}
#endif
			}
			else
			{
#ifdef	_TRACE_STOP
				if	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
				{
					if	(CheckIsResetting ())
					{
						_SAPI4_LOG::LogMessage (_TRACE_STOP, _T("[%p] AudioReset already resetting"), this);
					}
					else
					{
						_SAPI4_LOG::LogMessage (_TRACE_STOP, _T("[%p] NOT AudioReset IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), this, CheckIsQueueing(), CheckIsParsing(), CheckIsSpeaking(), CheckIsResetting());
					}
				}
#endif
				lResult = S_FALSE;
			}
		}
		catch AnyExceptionDebug
	}
	else
	{
		lResult = E_UNEXPECTED;
	}

	if	(_SAPI4_LOG::LogIsActive ())
	{
		_SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, lResult);
	}

	if	(FAILED (lResult))
	{
		SetIsQueueing (false);
		SetIsParsing (false);
		SetIsSpeaking (false);
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f     CSapi4Voice::Stop [%8.8X]"), DebugTimeElapsed, lResult);
#endif
	return lResult;
}

HRESULT CSapi4Voice::Pause ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(_IsValid ())
	{
		try
		{
			_SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, lResult = mEngine->AudioPause());
		}
		catch AnyExceptionDebug

		mPaused = true;
	}
	return lResult;
}

HRESULT CSapi4Voice::Resume ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(_IsValid ())
	{
		try
		{
			_SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, lResult = mEngine->AudioResume());
			_SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, lResult = mEngine->AudioResume()); // SAPI4 seems to sometime lose a call?
		}
		catch AnyExceptionDebug

		mPaused = false;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi4Voice::GetEngineId (GUID & pEngineId)
{
	HRESULT				lResult = E_UNEXPECTED;
	tS <TTSMODEINFO>	lModeInfo;

	if	(
			(mEngine != NULL)
		&&	(SUCCEEDED (lResult = mEngine->ModeGet (&lModeInfo)))
		)
	{
		pEngineId = lModeInfo.gEngineID;
	}
	return lResult;
}

HRESULT CSapi4Voice::GetModeId (GUID & pModeId)
{
	HRESULT				lResult = E_UNEXPECTED;
	tS <TTSMODEINFO>	lModeInfo;

	if	(
			(mEngine != NULL)
		&&	(SUCCEEDED (lResult = mEngine->ModeGet (&lModeInfo)))
		)
	{
		pModeId = lModeInfo.gModeID;
	}
	return lResult;
}

HRESULT CSapi4Voice::SetModeId (const GUID & pModeId)
{
	HRESULT		lResult = E_UNEXPECTED;
	ITTSEnumPtr	lEnum;
	IUnknownPtr	lAudioDest;

	if	(
			(mEngine == NULL)
		&&	(SUCCEEDED (lResult = _SAPI4_LOG::LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_SERVER, IID_ITTSEnum, (void **) &lEnum))))
		&&	(SUCCEEDED (lResult = _SAPI4_LOG::LogComErr (LogNormal|LogTime, CoCreateInstance (CLSID_MMAudioDest, NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void **) &lAudioDest))))
		)
	{
		mAudioDest = NULL;
		lResult = _SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, lEnum->Select (pModeId, &mEngine, lAudioDest));
		if	(SUCCEEDED (lResult))
		{
			mAudioDest = lAudioDest;
			mDefaultRate = GetRate ();
			mDefaultVolume = GetVolume ();
			mDefaultPitch = GetPitch ();
#ifdef	_LOG_INSTANCE
			LogTtsEngine (_LOG_INSTANCE, mEngine);
			LogTtsAudio (_LOG_INSTANCE, mAudioDest);
#endif
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi4Voice::GetDisplayName ()
{
	return CSapiVoice::GetDisplayName ();
}

HRESULT CSapi4Voice::GetDisplayName (tBstrPtr & pDisplayName)
{
	HRESULT				lResult = E_UNEXPECTED;
	tS <TTSMODEINFO>	lModeInfo;

	if	(
			(mEngine != NULL)
		&&	(SUCCEEDED (lResult = mEngine->ModeGet (&lModeInfo)))
		)
	{
		pDisplayName = CAtlString (lModeInfo.szSpeaker).AllocSysString ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi4Voice::GetUniqueId ()
{
	return CSapiVoice::GetUniqueId ();
}

HRESULT CSapi4Voice::GetUniqueId (tBstrPtr & pUniqueId)
{
	HRESULT		lResult;
	GUID		lModeId;
	CAtlString	lTTSModeStr;

	if	(SUCCEEDED (lResult = GetModeId (lModeId)))
	{
		lTTSModeStr = (CString) CGuidStr (lModeId);
		pUniqueId = lTTSModeStr.AllocSysString ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

ULONG CSapi4Voice::GetDefaultRate () const
{
	return mDefaultRate;
}

ULONG CSapi4Voice::GetRate ()
{
	return CSapiVoice::GetRate();
}

HRESULT CSapi4Voice::GetRate (ULONG & pRate)
{
	HRESULT				lResult = E_UNEXPECTED;
	ITTSAttributesPtr	lAttributes (mEngine);

	if	(lAttributes != NULL)
	{
		lResult = lAttributes->SpeedGet (&pRate);
	}
	return lResult;
}

HRESULT CSapi4Voice::SetRate (ULONG pRate)
{
	HRESULT				lResult = E_UNEXPECTED;
	ITTSAttributesPtr	lAttributes (mEngine);

	if	(lAttributes != NULL)
	{
		lResult = lAttributes->SpeedSet (pRate);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

USHORT CSapi4Voice::GetDefaultVolume () const
{
	return mDefaultVolume;
}

USHORT CSapi4Voice::GetVolume ()
{
	return CSapiVoice::GetVolume();
}

HRESULT CSapi4Voice::GetVolume (USHORT & pVolume)
{
	HRESULT				lResult = E_UNEXPECTED;
	ITTSAttributesPtr	lAttributes (mEngine);
	DWORD				lVolume = 0;

	if	(lAttributes != NULL)
	{
		lResult = lAttributes->VolumeGet (&lVolume);
		pVolume = (USHORT) (((DWORD)LOWORD(lVolume) + (DWORD)HIWORD(lVolume)) / 2);
	}
	return lResult;
}

HRESULT CSapi4Voice::SetVolume (USHORT pVolume)
{
	HRESULT				lResult = E_UNEXPECTED;
	ITTSAttributesPtr	lAttributes (mEngine);

	if	(lAttributes != NULL)
	{
		lResult = lAttributes->VolumeSet (MAKELONG (pVolume, pVolume));
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

USHORT CSapi4Voice::GetDefaultPitch () const
{
	return mDefaultPitch;
}

USHORT CSapi4Voice::GetPitch ()
{
	return CSapiVoice::GetPitch();
}

HRESULT CSapi4Voice::GetPitch (USHORT & pPitch)
{
	HRESULT				lResult = E_UNEXPECTED;
	ITTSAttributesPtr	lAttributes (mEngine);

	if	(lAttributes != NULL)
	{
		lResult = lAttributes->PitchGet (&pPitch);
	}
	return lResult;
}

HRESULT CSapi4Voice::SetPitch (USHORT pPitch)
{
	HRESULT				lResult = E_UNEXPECTED;
	ITTSAttributesPtr	lAttributes (mEngine);

	if	(lAttributes != NULL)
	{
		lResult = lAttributes->PitchSet (pPitch);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static CAtlString FeatureFlagsString (DWORD pFlags)
{
	CAtlString	lFeatures;

	lFeatures.Format (_T("%8.8X"), pFlags);

	if	(pFlags & TTSFEATURE_ANYWORD)
	{
		lFeatures += _T(" ANYWORD");
	}
	if	(pFlags & TTSFEATURE_VOLUME)
	{
		lFeatures += _T(" VOLUME");
	}
	if	(pFlags & TTSFEATURE_SPEED)
	{
		lFeatures += _T(" SPEED");
	}
	if	(pFlags & TTSFEATURE_PITCH)
	{
		lFeatures += _T(" PITCH");
	}
	if	(pFlags & TTSFEATURE_VISUAL)
	{
		lFeatures += _T(" VISUAL");
	}
	if	(pFlags & TTSFEATURE_WORDPOSITION)
	{
		lFeatures += _T(" WORDPOSITION");
	}
	if	(pFlags & TTSFEATURE_FIXEDAUDIO)
	{
		lFeatures += _T(" FIXEDAUDIO");
	}
	if	(pFlags & TTSFEATURE_PCOPTIMIZED)
	{
		lFeatures += _T(" PCOPTIMIZED");
	}
	if	(pFlags & TTSFEATURE_PHONEOPTIMIZED)
	{
		lFeatures += _T(" PHONEOPTIMIZED");
	}
	if	(pFlags & TTSFEATURE_IPATEXTDATA)
	{
		lFeatures += _T(" IPATEXTDATA");
	}
	if	(pFlags & TTSFEATURE_IPAUNICODE)
	{
		lFeatures += _T(" IPAUNICODE");
	}
	if	(pFlags & TTSFEATURE_PREFERRED)
	{
		lFeatures += _T(" PREFERRED");
	}
	if	(pFlags & TTSFEATURE_TRANSPLANTED)
	{
		lFeatures += _T(" TRANSPLANTED");
	}
	if	(pFlags & TTSFEATURE_SINGLEINSTANCE)
	{
		lFeatures += _T(" SINGLEINSTANCE");
	}
	if	(pFlags & TTSFEATURE_THREADSAFE)
	{
		lFeatures += _T(" THREADSAFE");
	}
	if	(pFlags & TTSFEATURE_SAPI4)
	{
		lFeatures += _T(" SAPI4");
	}
	return lFeatures;
}

static CAtlString InterfaceFlagsString (DWORD pFlags)
{
	CAtlString	lInterfaces;

	lInterfaces.Format (_T("%8.8X"), pFlags);

	if	(pFlags & TTSI_ITTSCENTRAL)
	{
		lInterfaces += _T(" TTSI_ITTSCENTRAL");
	}
	if	(pFlags & TTSI_ITTSATTRIBUTES)
	{
		lInterfaces += _T(" TTSI_ITTSATTRIBUTES");
	}
	if	(pFlags & TTSI_ITTSDIALOGS)
	{
		lInterfaces += _T(" TTSI_ITTSDIALOGS");
	}
	if	(pFlags & TTSI_ILEXPRONOUNCE)
	{
		lInterfaces += _T(" TTSI_ILEXPRONOUNCE");
	}
	if	(pFlags & TTSI_ILEXPRONOUNCE2)
	{
		lInterfaces += _T(" TTSI_ILEXPRONOUNCE2");
	}
	return lInterfaces;
}

/////////////////////////////////////////////////////////////////////////////

static void SubtractCurrentTime (FILETIME & pFileTime)
{
	FILETIME		lCurrFileTime;
	ULARGE_INTEGER	lLargeFileTime [2];

	GetSystemTimeAsFileTime (&lCurrFileTime);
	lLargeFileTime[0].HighPart = pFileTime.dwHighDateTime;
	lLargeFileTime[0].LowPart = pFileTime.dwLowDateTime;
	lLargeFileTime[1].HighPart = lCurrFileTime.dwHighDateTime;
	lLargeFileTime[1].LowPart = lCurrFileTime.dwLowDateTime;
	//lLargeFileTime[1].QuadPart = min (lLargeFileTime[1].QuadPart, lLargeFileTime[0].QuadPart);
	lLargeFileTime[0].QuadPart -= lLargeFileTime[1].QuadPart;
	pFileTime.dwHighDateTime = lLargeFileTime[0].HighPart;
	pFileTime.dwLowDateTime = lLargeFileTime[0].LowPart;
}

/////////////////////////////////////////////////////////////////////////////

void CSapi4Voice::LogTtsEngine (UINT pLogLevel, ITTSCentral * pTtsEngine, LPCTSTR pFormat, ...)
{
	if	(_SAPI4_LOG::LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString			lTitle;
			CAtlString			lIndent;
			tS <TTSMODEINFO>	lModeInfo;
			QWORD				lTimeStamp = 0;
			FILETIME			lFileTime;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			if	(lTitle.IsEmpty())
			{
				lTitle = _T("Sapi4Engine");
			}

			_SAPI4_LOG::LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pTtsEngine);
			if	(pTtsEngine)
			{
				if	(SUCCEEDED (_SAPI4_LOG::LogSapi4Err (pLogLevel, pTtsEngine->ModeGet (&lModeInfo))))
				{
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Engine     [%s] [%s] [%s]"), (CAtlString)CGuidStr(lModeInfo.gEngineID), lModeInfo.szProductName, lModeInfo.szMfgName);
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Mode       [%s] [%s]"), (CAtlString)CGuidStr(lModeInfo.gModeID), lModeInfo.szModeName);
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Speaker    [%s] Gender [%hu] Age [%hu] Style [%s]"), lModeInfo.szSpeaker, lModeInfo.wGender, lModeInfo.wAge, lModeInfo.szStyle);
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Features   [%s]"), FeatureFlagsString(lModeInfo.dwFeatures));
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Interfaces [%s]"), InterfaceFlagsString(lModeInfo.dwInterfaces));
				}
				if	(SUCCEEDED (_SAPI4_LOG::LogSapi4Err (pLogLevel, pTtsEngine->PosnGet (&lTimeStamp))))
				{
					_SAPI4_LOG::LogSapi4Err (pLogLevel, pTtsEngine->ToFileTime (&lTimeStamp, &lFileTime));
					SubtractCurrentTime (lFileTime);
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Position   [%I64u] [%s]"), lTimeStamp, LclFormatDuration (lFileTime));
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void CSapi4Voice::LogTtsAudio (UINT pLogLevel, IAudioDest * pTtsAudio, LPCTSTR pFormat, ...)
{
	if	(_SAPI4_LOG::LogIsActive (pLogLevel))
	{
		try
		{
			IAudioPtr	lTtsAudio (pTtsAudio);
			CAtlString	lTitle;
			CAtlString	lIndent;
			DWORD		lLevel = 0;
			QWORD		lTimeStamp = 0;
			FILETIME	lFileTime;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			if	(lTitle.IsEmpty())
			{
				lTitle = _T("Sapi4Audio");
			}

			_SAPI4_LOG::LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pTtsAudio);

			if	(lTtsAudio)
			{
				if	(SUCCEEDED (_SAPI4_LOG::LogSapi4Err (pLogLevel, lTtsAudio->LevelGet (&lLevel))))
				{
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Level      [%d]"), lLevel);
				}
				if	(SUCCEEDED (_SAPI4_LOG::LogSapi4Err (pLogLevel, lTtsAudio->PosnGet (&lTimeStamp))))
				{
					_SAPI4_LOG::LogSapi4Err (pLogLevel, lTtsAudio->ToFileTime (&lTimeStamp, &lFileTime));
					SubtractCurrentTime (lFileTime);
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Position   [%I64u] [%s]"), lTimeStamp, LclFormatDuration (lFileTime));
				}
				if	(SUCCEEDED (_SAPI4_LOG::LogSapi4Err (pLogLevel, lTtsAudio->TotalGet (&lTimeStamp))))
				{
					_SAPI4_LOG::LogSapi4Err (pLogLevel, lTtsAudio->ToFileTime (&lTimeStamp, &lFileTime));
					SubtractCurrentTime (lFileTime);
					_SAPI4_LOG::LogMessage (pLogLevel|LogHighVolume, _T("  Total      [%I64u] [%s]"), lTimeStamp, LclFormatDuration (lFileTime));
				}
			}
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	_SAPI4_LOGGING
extern "C" void _LogPrepFileLine (LPTSTR pString, LPCSTR pFile, UINT pLine);
namespace _SAPI4_LOG
{
/////////////////////////////////////////////////////////////////////////////

HRESULT _LogSapi4ErrFL::LogErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
{
	if	(
			(
				(FAILED (pError))
			||	((pLogLevel & 0x000000FF) == LogAlways)
			)
		&&	(LogIsActive (pLogLevel))
		)
	{
		CAtlString	lFormat;
		LPCTSTR		lError;

		if	(lError = ::_GetSapi4Err (pError))
		{
			lFormat.Format (_T("Sapi4Error [%8.8X] [%s]"), pError, lError);
		}
		if  (pFormat)
		{
			va_list lArgPtr;

			if	(!lFormat.IsEmpty ())
			{
				lFormat += _T(" => ");
			}
			va_start (lArgPtr, pFormat);
			_vsntprintf (lFormat.GetBuffer(lFormat.GetLength()+4096)+lFormat.GetLength(), 4096, pFormat, lArgPtr);
			lFormat.ReleaseBuffer ();
		}
		if	(SUCCEEDED (pError))
		{
			_LogComErrFL(mFile,mLine).LogErr (MinLogLevel(pLogLevel,LogAlways), pError, lFormat);
		}
		else
		{
			_LogPrepFileLine (lFormat.GetBuffer (lFormat.GetLength()+(MAX_PATH*2)), mFile, mLine);
			lFormat.ReleaseBuffer ();
			LogMessage (MinLogLevel(pLogLevel,LogAlways), lFormat);
		}
	}
	return pError;
}

/////////////////////////////////////////////////////////////////////////////
}
#endif	// _SAPI4_LOGGING
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int VoiceMouthOverlay (LPVOID pMouth)
{
	int			lMouthOverlayNdx = -1;
	PTTSMOUTH	lMouth;

	if	(lMouth = (PTTSMOUTH)pMouth)
	{
		if	(lMouth->bMouthHeight <= 20)
		{
			lMouthOverlayNdx = MouthOverlayClosed;
		}
		else
		if	(lMouth->bMouthWidth <= 40)
		{
			lMouthOverlayNdx = MouthOverlayNarrow;
		}
		else
		if	(
				(lMouth->bMouthWidth >= 60)
			&&	(lMouth->bMouthHeight >= 120)
			)
		{
			lMouthOverlayNdx = MouthOverlayWide4;
		}
		else
		if	(
				(lMouth->bMouthWidth >= 160)
			&&	(lMouth->bMouthHeight >= 60)
			)
		{
			lMouthOverlayNdx = MouthOverlayWide3;
		}
		else
		if	(
				(lMouth->bMouthWidth >= 130)
			&&	(lMouth->bMouthHeight >= 40)
			)
		{
			lMouthOverlayNdx = MouthOverlayWide2;
		}
		else
		if	(lMouth->bMouthHeight <= 90)
		{
			lMouthOverlayNdx = MouthOverlayWide1;
		}
		else
		{
			lMouthOverlayNdx = MouthOverlayMedium;
		}
	}
	return lMouthOverlayNdx;
}

CAtlString VoiceMouthStr (LPVOID pMouth)
{
	CAtlString	lMouthStr;
	PTTSMOUTH	lMouth;

	if	(lMouth = (PTTSMOUTH)pMouth)
	{
		lMouthStr.Format (_T("[%3u x %3u] up [%3u] open [%3u] teeth [%3u %3u] tounge [%3u] tension [%3u]"), lMouth->bMouthWidth, lMouth->bMouthHeight, lMouth->bMouthUpturn, lMouth->bJawOpen, lMouth->bTeethUpperVisible, lMouth->bTeethLowerVisible, lMouth->bTonguePosn, lMouth->bLipTension);
	}
	else
	{
		lMouthStr = _T("<null>");
	}

	return lMouthStr;
}

static CAtlString MouthOverlayStr (short pMouthOverlayNdx)
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

CSapi4Voice::CTTSNotifySink::CTTSNotifySink (CSapi4Voice & pOwner)
:	mOwner (pOwner),
	mRegisteredKey (0)
{
	m_dwRef = 1;
#ifdef	_LOG_NOTIFY
	if	(_SAPI4_LOG::LogIsActive (_LOG_NOTIFY))
	{
		_SAPI4_LOG::LogMessage (_LOG_NOTIFY, _T("[%p] [%p(%d)] CTTSNotifySink"), &mOwner, this, max(m_dwRef,-1));
	}
#endif
}

CSapi4Voice::CTTSNotifySink::~CTTSNotifySink ()
{
#ifdef	_LOG_NOTIFY
	if	(_SAPI4_LOG::LogIsActive (_LOG_NOTIFY))
	{
		_SAPI4_LOG::LogMessage (_LOG_NOTIFY, _T("[%p] [%p(%d)] ~CTTSNotifySink"), &mOwner, this, max(m_dwRef,-1));
	}
#endif
	try
	{
		Disconnect ();
	}
	catch AnyExceptionDebug

#ifdef	_TRACE_STOP
	if	(
			(m_dwRef > 1)
		&&	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
		)
	{
		_SAPI4_LOG::LogMessage (_TRACE_STOP&~LogHighVolume, _T("[%p(%d)] CTTSNotifySink Destructor"), this, max(m_dwRef,-1));
	}
#endif
	m_dwRef = 0;
}

/////////////////////////////////////////////////////////////////////////////

bool CSapi4Voice::CTTSNotifySink::SafeIsConnected () const
{
	if	(
			(this)
		&&	(mRegisteredKey != 0)
		)
	{
		return true;
	}
	return false;
}

HRESULT CSapi4Voice::CTTSNotifySink::Connect ()
{
	HRESULT	lResult = S_FALSE;

	if	(mOwner.mEngine == NULL)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mRegisteredKey == 0)
	{
		lResult = _SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, mOwner.mEngine->Register ((ITTSNotifySink2 *)this, IID_ITTSNotifySink, &mRegisteredKey));
#ifdef	_LOG_NOTIFY
		if	(_SAPI4_LOG::LogIsActive (_LOG_NOTIFY))
		{
			_SAPI4_LOG::LogComErrAnon (MinLogLevel(_LOG_NOTIFY,LogAlways), lResult, _T("[%p] [%p(%d)] Connected  [%u]"), &mOwner, this, max(m_dwRef,-1), mRegisteredKey);
		}
#endif
	}
	return lResult;
}

HRESULT CSapi4Voice::CTTSNotifySink::Disconnect ()
{
	HRESULT	lResult = S_FALSE;

	if	(
			(mOwner.mEngine != NULL)
		&&	(mRegisteredKey != 0)
		)
	{
		lResult = mOwner.mEngine->UnRegister (mRegisteredKey);
#ifdef	_LOG_NOTIFY
		if	(_SAPI4_LOG::LogIsActive (_LOG_NOTIFY))
		{
			_SAPI4_LOG::LogSapi4ErrAnon (MinLogLevel(_LOG_NOTIFY,LogAlways), lResult, _T("[%p] [%p(%d)] Disconnect [%u]"), &mOwner, this, max(m_dwRef,-1), mRegisteredKey);
		}
		else
#endif
		{
			_SAPI4_LOG::LogSapi4Err (LogNormal|LogTime, lResult);
		}
	}
	mRegisteredKey = 0;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::AttribChanged (DWORD dwAttribute)
{
#ifdef	_TRACE_EVENTS_EX_NOT
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS_EX))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS_EX, _T("[%p] [%p(%d)] CSapi4Voice::AttribChanged [%u]"), &mOwner, this, max(m_dwRef,-1), dwAttribute);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::AudioStart (QWORD qTimeStamp)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::AudioStart [%I64u] IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, mOwner.CheckIsQueueing(), mOwner.CheckIsParsing(), mOwner.CheckIsSpeaking(), mOwner.CheckIsResetting());
	}
#endif
	try
	{
		mOwner.SetIsSpeaking (true);
		mOwner.SetIsQueueing (false);
	}
	catch AnyExceptionSilent

	if	(mOwner.GetNotifySinkCount() > 0)
	{
		INT_PTR					lNdx;
		_ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx < mOwner.GetNotifySinkCount(); lNdx++)
		{
			if	(lEventSink = mOwner.GetNotifySink (lNdx))
			{
				try
				{
					lEventSink->OnVoiceStart (mOwner.mEventCharID);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::AudioStop (QWORD qTimeStamp)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::AudioStop [%I64u] IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, mOwner.CheckIsQueueing(), mOwner.CheckIsParsing(), mOwner.CheckIsSpeaking(), mOwner.CheckIsResetting());
	}
#endif
	try
	{
		mOwner.SetIsSpeaking (false);
		mOwner.SetIsQueueing (false);
	}
	catch AnyExceptionSilent

	if	(mOwner.GetNotifySinkCount() > 0)
	{
		INT_PTR					lNdx;
		_ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx < mOwner.GetNotifySinkCount(); lNdx++)
		{
			if	(lEventSink = mOwner.GetNotifySink (lNdx))
			{
				try
				{
					lEventSink->OnVoiceEnd (mOwner.mEventCharID);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::Visual (QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
#ifdef	_TRACE_EVENTS_EX
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS_EX))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS_EX, _T("[%p] [%p(%d)] CSapi4Voice::Visual [%I64u] [%s %s] [%s]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, DebugStr(CAtlString(cIPAPhoneme,1)), DebugStr(CAtlString(cEnginePhoneme,1)), VoiceMouthStr(pTTSMouth));
	}
#endif
	if	(mOwner.GetNotifySinkCount() > 0)
	{
		INT_PTR					lNdx;
		_ISapiVoiceEventSink *	lEventSink;
		int						lMouthOverlay = VoiceMouthOverlay (pTTSMouth);

#ifdef	_DEBUG_MOUTH
		LogMessage (_DEBUG_MOUTH, _T("[%p] Visual [%s %s] [%s] [%s]"), mOwner, DebugStr(CAtlString(cIPAPhoneme,1)), DebugStr(CAtlString(cEnginePhoneme,1)), VoiceMouthStr(pTTSMouth), MouthOverlayStr(lMouthOverlay));
#endif
		for	(lNdx = 0; lNdx < mOwner.GetNotifySinkCount(); lNdx++)
		{
			if	(lEventSink = mOwner.GetNotifySink (lNdx))
			{
				try
				{
					lEventSink->OnVoiceVisual (mOwner.mEventCharID, lMouthOverlay);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::Error (LPUNKNOWN pError)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::Error [%p]"), &mOwner, this, max(m_dwRef,-1), pError);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::Warning (LPUNKNOWN pWarning)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::Warning [%p]"), &mOwner, this, max(m_dwRef,-1), pWarning);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::VisualFuture (DWORD dwMilliseconds, QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::VisualFuture [%u] [%I64u]"), &mOwner, this, max(m_dwRef,-1), dwMilliseconds, qTimeStamp);
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::TextDataStarted (QWORD qTimeStamp)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::TextDataStarted [%I64u] IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, mOwner.CheckIsQueueing(), mOwner.CheckIsParsing(), mOwner.CheckIsSpeaking(), mOwner.CheckIsResetting());
	}
#endif
	try
	{
		mOwner.SetIsParsing (!mOwner.CheckIsResetting());
	}
	catch AnyExceptionSilent
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::TextDataDone (QWORD qTimeStamp, DWORD dwFlags)
{
#ifdef	_TRACE_EVENTS
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS, _T("[%p] [%p(%d)] CSapi4Voice::TextDataDone [%I64u] [%8.8X] IsQueuing [%u] IsParsing [%u] IsSpeaking [%u] IsResetting [%u]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, dwFlags, mOwner.CheckIsQueueing(), mOwner.CheckIsParsing(), mOwner.CheckIsSpeaking(), mOwner.CheckIsResetting());
	}
#ifdef	_TRACE_STOP
	else
#endif
#endif
#ifdef	_TRACE_STOP
	if	(
			(dwFlags & TTSBNS_ABORTED)
		&&	(_SAPI4_LOG::LogIsActive (_TRACE_STOP))
		)
	{
		_SAPI4_LOG::LogMessage (_TRACE_STOP, _T("[%p] [%p(%d)] TextDataDone [%I64u] [%8.8X]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, dwFlags);
	}
#endif
	try
	{
		mOwner.SetIsParsing (false);
		mOwner.SetIsResetting (false);
	}
	catch AnyExceptionSilent
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::BookMark (QWORD qTimeStamp, DWORD dwMarkNum)
{
#ifdef	_TRACE_EVENTS_EX
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS_EX))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS_EX, _T("[%p] [%p(%d)] CSapi4Voice::BookMark [%I64u] [%u]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, dwMarkNum);
	}
#endif
	if	(mOwner.GetNotifySinkCount() > 0)
	{
		INT_PTR					lNdx;
		_ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx < mOwner.GetNotifySinkCount(); lNdx++)
		{
			if	(lEventSink = mOwner.GetNotifySink (lNdx))
			{
				try
				{
					lEventSink->OnVoiceBookMark (mOwner.mEventCharID, (long)dwMarkNum);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::WordPosition (QWORD qTimeStamp, DWORD dwByteOffset)
{
#ifdef	_TRACE_EVENTS_EX
	if	(_SAPI4_LOG::LogIsActive (_TRACE_EVENTS_EX))
	{
		_SAPI4_LOG::LogMessage (_TRACE_EVENTS_EX, _T("[%p] [%p(%d)] CSapi4Voice::WordPosition [%I64u] [%u]"), &mOwner, this, max(m_dwRef,-1), qTimeStamp, dwByteOffset);
	}
#endif
	if	(mOwner.GetNotifySinkCount() > 0)
	{
		INT_PTR					lNdx;
		_ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx < mOwner.GetNotifySinkCount(); lNdx++)
		{
			if	(lEventSink = mOwner.GetNotifySink (lNdx))
			{
				try
				{
					lEventSink->OnVoiceWord (mOwner.mEventCharID, dwByteOffset/sizeof(TCHAR), 1);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}
