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
#include <initguid.h>
#include <speech.h>
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "AgentFile.h"
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef	_DEBUG
//#define	_DEBUG_EVENTS	LogNormal|LogHighVolume|LogTimeMs
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ITTSEnum, IID_ITTSEnum);
_COM_SMARTPTR_TYPEDEF (ITTSAttributes, IID_ITTSAttributes);

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CSapi4Voice, CSapiVoice)

CSapi4Voice::CSapi4Voice ()
:	mDefaultRate (0),
	mDefaultVolume (0),
	mDefaultPitch (0),
	mIsQueueing (false),
	mIsSpeaking (false)
{
}

CSapi4Voice::~CSapi4Voice ()
{
	try
	{
		Stop ();
	}
	catch AnyExceptionDebug

	SafeFreeSafePtr (mNotifySink);
	SafeFreeSafePtr (mEngine);
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
				(mIsQueueing)
			||	(mIsSpeaking)
			)
		)
	{
		return true;
	}
	return false;
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
			LogSapi4Err (LogNormal, mEngine->AudioReset ());
		}
		if	(SUCCEEDED (lResult))
		{
			lResult = S_OK;
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f     CSapi4Voice::PrepareToSpeak [%8.8X]"), DebugTimeElapsed, lResult);
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
		tS <SDATA>	lSpeechData;
		bool		lIsQueueing = mIsQueueing;

		mIsQueueing = true;

		lSpeechData.pData = (PVOID)pMessage;
		lSpeechData.dwSize = (_tcslen (pMessage)+1) * sizeof(TCHAR);
		lResult = mEngine->TextData (CHARSET_TEXT, TTSDATAFLAG_TAGGED, lSpeechData, &mNotifySink->m_xBufNotifySink, IID_ITTSBufNotifySink);

		if	(FAILED (lResult))
		{
			mIsQueueing = lIsQueueing;
		}
	}
	if	(LogIsActive ())
	{
		LogSapi4Err (LogNormal, lResult);
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f     CSapi4Voice::Speak [%8.8X] [%s]"), DebugTimeElapsed, lResult, DebugStr(pMessage));
#endif
	return lResult;
}

HRESULT CSapi4Voice::Stop ()
{
	HRESULT	lResult = E_UNEXPECTED;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(_IsValid ())
	{
		lResult = mEngine->AudioReset ();
		mIsQueueing = false;
		mIsSpeaking = false;
	}
	if	(LogIsActive ())
	{
		LogSapi4Err (LogNormal, lResult);
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f     CSapi4Voice::Stop [%8.8X]"), DebugTimeElapsed, lResult);
#endif
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
		&&	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_SERVER, IID_ITTSEnum, (void **) &lEnum))))
		&&	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_MMAudioDest, NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void **) &lAudioDest))))
		)
	{
		lResult = LogSapi4Err (LogNormal, lEnum->Select (pModeId, &mEngine, lAudioDest));
		if	(SUCCEEDED (lResult))
		{
			mDefaultRate = GetRate ();
			mDefaultVolume = GetVolume ();
			mDefaultPitch = GetPitch ();
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
		pDisplayName = CString (lModeInfo.szSpeaker).AllocSysString ();
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
	HRESULT	lResult;
	GUID	lModeId;
	CString	lTTSModeStr;

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
		if	(lMouth->bMouthWidth <= 20)
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
				(lMouth->bMouthWidth >= 120)
			&&	(lMouth->bMouthHeight >= 90)
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

CString VoiceMouthStr (LPVOID pMouth)
{
	CString		lMouthStr;
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CSapi4Voice::CTTSNotifySink, CCmdTarget)
	INTERFACE_PART(CSapi4Voice::CTTSNotifySink, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CSapi4Voice::CTTSNotifySink, IID_ITTSNotifySink, NotifySink)
	INTERFACE_PART(CSapi4Voice::CTTSNotifySink, IID_ITTSNotifySink2, NotifySink)
	INTERFACE_PART(CSapi4Voice::CTTSNotifySink, IID_ITTSBufNotifySink, BufNotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CSapi4Voice::CTTSNotifySink, NotifySink)
IMPLEMENT_IUNKNOWN(CSapi4Voice::CTTSNotifySink, BufNotifySink)

/////////////////////////////////////////////////////////////////////////////

CSapi4Voice::CTTSNotifySink::CTTSNotifySink (CSapi4Voice & pOwner)
:	mOwner (pOwner),
	mRegisteredKey (0)
{
	EnableAggregation ();
}

CSapi4Voice::CTTSNotifySink::~CTTSNotifySink ()
{
	try
	{
		Disconnect ();
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG
	if	(m_dwRef > 1)
	{
		LogMessage (LogNormal, _T("CTTSNotifySink Destructor [%u]"), m_dwRef);
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
		lResult = LogSapi4Err (LogNormal, mOwner.mEngine->Register (&m_xNotifySink, IID_ITTSNotifySink, &mRegisteredKey));
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
		lResult = LogSapi4Err (LogNormal, mOwner.mEngine->UnRegister (mRegisteredKey));
	}
	mRegisteredKey = 0;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::AttribChanged (DWORD dwAttribute)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::AttribChanged [%u]"), &pThis->mOwner, dwAttribute);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::AudioStart (QWORD qTimeStamp)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::AudioStart [%I64u]"), &pThis->mOwner, qTimeStamp);
#endif
	try
	{
		pThis->mOwner.mIsSpeaking = true;
	}
	catch AnyExceptionSilent

	if	(pThis->mOwner.mEventSinks.GetSize() > 0)
	{
		int						lNdx;
		ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx <= pThis->mOwner.mEventSinks.GetUpperBound(); lNdx++)
		{
			if	(lEventSink = pThis->mOwner.mEventSinks [lNdx])
			{
				try
				{
					lEventSink->OnVoiceStart (pThis->mOwner.mEventCharID);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::AudioStop (QWORD qTimeStamp)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::AudioStop [%I64u]"), &pThis->mOwner, qTimeStamp);
#endif
	try
	{
		pThis->mOwner.mIsSpeaking = false;
	}
	catch AnyExceptionSilent

	if	(pThis->mOwner.mEventSinks.GetSize() > 0)
	{
		int						lNdx;
		ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx <= pThis->mOwner.mEventSinks.GetUpperBound(); lNdx++)
		{
			if	(lEventSink = pThis->mOwner.mEventSinks [lNdx])
			{
				try
				{
					lEventSink->OnVoiceEnd (pThis->mOwner.mEventCharID);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::Visual (QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::Visual [%I64u] [%s %s] [%s]"), &pThis->mOwner, qTimeStamp, DebugStr(CString(cIPAPhoneme,1)), DebugStr(CString(cEnginePhoneme,1)), VoiceMouthStr(pTTSMouth));
#endif

	if	(pThis->mOwner.mEventSinks.GetSize() > 0)
	{
		int						lNdx;
		ISapiVoiceEventSink *	lEventSink;
		int						lMouthOverlay = VoiceMouthOverlay (pTTSMouth);

		for	(lNdx = 0; lNdx <= pThis->mOwner.mEventSinks.GetUpperBound(); lNdx++)
		{
			if	(lEventSink = pThis->mOwner.mEventSinks [lNdx])
			{
				try
				{
					lEventSink->OnVoiceVisual (pThis->mOwner.mEventCharID, lMouthOverlay);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::Error (LPUNKNOWN pError)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::Error [%p]"), &pThis->mOwner, pError);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::Warning (LPUNKNOWN pWarning)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::Warning [%p]"), &pThis->mOwner, pWarning);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XNotifySink::VisualFuture (DWORD dwMilliseconds, QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, NotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XNotifySink::VisualFuture [%u] [%I64u]"), &pThis->mOwner, dwMilliseconds, qTimeStamp);
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XBufNotifySink::TextDataDone (QWORD qTimeStamp, DWORD dwFlags)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, BufNotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XBufNotifySink::TextDataDone [%I64u] [%8.8X]"), &pThis->mOwner, qTimeStamp, dwFlags);
#endif
	try
	{
		pThis->mOwner.mIsQueueing = false;
	}
	catch AnyExceptionSilent
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XBufNotifySink::TextDataStarted (QWORD qTimeStamp)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, BufNotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XBufNotifySink::TextDataStarted [%I64u]"), &pThis->mOwner, qTimeStamp);
#endif
	try
	{
		pThis->mOwner.mIsQueueing = true;
	}
	catch AnyExceptionSilent
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XBufNotifySink::BookMark (QWORD qTimeStamp, DWORD dwMarkNum)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, BufNotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XBufNotifySink::BookMark [%I64u] [%u]"), &pThis->mOwner, qTimeStamp, dwMarkNum);
#endif

	if	(pThis->mOwner.mEventSinks.GetSize() > 0)
	{
		int						lNdx;
		ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx <= pThis->mOwner.mEventSinks.GetUpperBound(); lNdx++)
		{
			if	(lEventSink = pThis->mOwner.mEventSinks [lNdx])
			{
				try
				{
					lEventSink->OnVoiceBookMark (pThis->mOwner.mEventCharID, (long)dwMarkNum);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSapi4Voice::CTTSNotifySink::XBufNotifySink::WordPosition (QWORD qTimeStamp, DWORD dwByteOffset)
{
	METHOD_PROLOGUE(CSapi4Voice::CTTSNotifySink, BufNotifySink)
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("[%p] CSapi4Voice::XBufNotifySink::WordPosition [%I64u] [%u]"), &pThis->mOwner, qTimeStamp, dwByteOffset);
#endif

	if	(pThis->mOwner.mEventSinks.GetSize() > 0)
	{
		int						lNdx;
		ISapiVoiceEventSink *	lEventSink;

		for	(lNdx = 0; lNdx <= pThis->mOwner.mEventSinks.GetUpperBound(); lNdx++)
		{
			if	(lEventSink = pThis->mOwner.mEventSinks [lNdx])
			{
				try
				{
					lEventSink->OnVoiceWord (pThis->mOwner.mEventCharID, dwByteOffset/sizeof(TCHAR), 1);
				}
				catch AnyExceptionSilent
			}
		}
	}
	return S_OK;
}
