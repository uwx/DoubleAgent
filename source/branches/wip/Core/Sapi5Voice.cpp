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
#include <sphelper.h>
#include "Sapi5Voice.h"
#include "Sapi5Err.h"
#include "MallocPtr.h"
#include "StringArrayEx.h"
#include "AgentFile.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_EVENTS	(GetProfileDebugInt(_T("DebugSapiEvents"),LogVerbose,true)&0xFFFF|LogHighVolume|LogTimeMs)
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ISpAudio, __uuidof(ISpAudio));
_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));
_COM_SMARTPTR_TYPEDEF (ISpObjectTokenCategory, __uuidof(ISpObjectTokenCategory));
_COM_SMARTPTR_TYPEDEF (ISpDataKey, __uuidof(ISpDataKey));

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi5Voice)

CSapi5Voice::CSapi5Voice ()
:	mVoiceStreamNum (0),
	mPrepared (false),
	mLastVoiceEvent (SPEI_UNDEFINED)
{
	LogComErr (LogIfActive, CoCreateInstance (CLSID_SpVoice, NULL, CLSCTX_SERVER, __uuidof (ISpVoice), (void **) &mVoice));
}

CSapi5Voice::~CSapi5Voice ()
{
	if	(mVoice != NULL)
	{
		try
		{
			mVoice->SetNotifySink (NULL);
		}
		catch AnyExceptionSilent
	}
	SafeFreeSafePtr (mVoice);
}

CSapi5Voice * CSapi5Voice::CreateInstance ()
{
	return new CSapi5Voice;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

UINT CSapi5Voice::_IsValid () const
{
	return (mVoice != NULL) ? 5 : 0;
}

bool CSapi5Voice::_IsPrepared () const
{
	return ((mVoice != NULL) && mPrepared);
}

bool CSapi5Voice::_IsSpeaking () const
{
	tS <SPVOICESTATUS>	lStatus;

	if	(
			(_IsValid ())
		&&	(
				(mLastVoiceEvent == SPEI_START_INPUT_STREAM)
			||	(
					(SUCCEEDED (mVoice->GetStatus (&lStatus, NULL)))
				&&	(
						(lStatus.dwRunningState == SPRS_IS_SPEAKING)
					||	(lStatus.dwRunningState == 0)
					)
				)
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

void __stdcall CSapi5Voice::VoiceNotifyCallback(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CSapi5Voice *	lThis = (CSapi5Voice *) lParam;
		CSpEvent		lEvent;

#ifdef	_DEBUG_EVENTS
		tS <SPEVENTSOURCEINFO>	lEventInfo;
		lEventInfo.ullEventInterest = SPFEI_ALL_TTS_EVENTS;
		lEventInfo.ullQueuedInterest = SPFEI_ALL_TTS_EVENTS;
		LogSapi5Err (LogIfActive|LogHighVolume, lThis->mVoice->GetInfo (&lEventInfo));
		LogMessage (_DEBUG_EVENTS, _T("[%p] EventCount [%u]"), lThis, lEventInfo.ulCount);
#endif

		while (lEvent.GetFrom (lThis->mVoice) == S_OK)
		{
#ifdef	_DEBUG_EVENTS
			CAtlString	lEventStr;

			switch (lEvent.eEventId)
			{
				case SPEI_START_INPUT_STREAM:	lEventStr = _T("START_INPUT_STREAM"); break;
				case SPEI_END_INPUT_STREAM:		lEventStr = _T("END_INPUT_STREAM"); break;
				case SPEI_VOICE_CHANGE:			lEventStr = _T("VOICE_CHANGE"); break;
				case SPEI_TTS_BOOKMARK:			lEventStr.Format (_T("TTS_BOOKMARK %d (%s)"), lEvent.wParam, lEvent.BookmarkName()); break;
				case SPEI_WORD_BOUNDARY:		lEventStr.Format (_T("WORD_BOUNDARY %u at %u"), lEvent.InputWordLen(), lEvent.InputWordPos()); break;
				case SPEI_PHONEME:				lEventStr.Format (_T("PHONEME %u"), lEvent.Phoneme()); break;
				case SPEI_SENTENCE_BOUNDARY:	lEventStr.Format (_T("SENTENCE_BOUNDARY %u at %u"), lEvent.InputSentLen(), lEvent.InputSentPos()); break;
				case SPEI_VISEME:				lEventStr.Format (_T("VISEME %s (%2u) for %3u"),  VoiceVisemeStr(lEvent.Viseme()), HIWORD(lEvent.lParam), HIWORD(lEvent.wParam)); break;
				case SPEI_TTS_AUDIO_LEVEL:		lEventStr.Format (_T("TTS_AUDIO_LEVEL %d"), lEvent.wParam); break;
				case SPEI_TTS_PRIVATE:			lEventStr = _T("TTS_PRIVATE"); break;
				case SPEI_UNDEFINED:			lEventStr = _T("UNDEFINED"); break;
				default:						lEventStr.Format (_T("%u"), lEvent.eEventId);
			}
			LogMessage (_DEBUG_EVENTS, _T("  Stream [%u] Event [%u] [%s]"), lEvent.ulStreamNum, lEvent.eEventId, lEventStr);
#endif
			if	(
					(lEvent.eEventId == SPEI_START_INPUT_STREAM)
				||	(lEvent.eEventId == SPEI_END_INPUT_STREAM)
				)
			{
				lThis->mLastVoiceEvent = lEvent.eEventId;
			}

			if	(
					(
						(lEvent.eEventId == SPEI_START_INPUT_STREAM)
					||	(lEvent.eEventId == SPEI_END_INPUT_STREAM)
					||	(lEvent.eEventId == SPEI_TTS_BOOKMARK)
					||	(lEvent.eEventId == SPEI_WORD_BOUNDARY)
					||	(lEvent.eEventId == SPEI_VISEME)
					)
				&&	(lThis->mEventSinks.GetSize() > 0)
				)
			{
				int						lNdx;
				_ISapiVoiceEventSink *	lEventSink;

				for	(lNdx = 0; lNdx <= lThis->mEventSinks.GetUpperBound(); lNdx++)
				{
					if	(lEventSink = lThis->mEventSinks [lNdx])
					{
						try
						{
							if	(lEvent.eEventId == SPEI_START_INPUT_STREAM)
							{
								lEventSink->OnVoiceStart (lThis->mEventCharID);
							}
							else
							if	(lEvent.eEventId == SPEI_END_INPUT_STREAM)
							{
								lEventSink->OnVoiceEnd (lThis->mEventCharID);
							}
							else
							if	(lEvent.eEventId == SPEI_TTS_BOOKMARK)
							{
								lEventSink->OnVoiceBookMark (lThis->mEventCharID, (long)lEvent.wParam);
							}
							else
							if	(lEvent.eEventId == SPEI_WORD_BOUNDARY)
							{
								lEventSink->OnVoiceWord (lThis->mEventCharID, lEvent.InputWordPos(), lEvent.InputWordLen());
							}
							else
							if	(lEvent.eEventId == SPEI_VISEME)
							{
								lEventSink->OnVoiceVisual (lThis->mEventCharID, VoiceVisemeOverlay (lEvent.Viseme()));
							}
						}
						catch AnyExceptionSilent
					}
				}
			}
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi5Voice::PrepareToSpeak (bool pHighPriority)
{
	HRESULT	lResult = E_UNEXPECTED;

	mVoiceStreamNum = 0;
	mPrepared = false;
	mLastVoiceEvent = SPEI_UNDEFINED;

	if	(
			(_IsValid ())
		&&	(SUCCEEDED (LogSapi5Err (LogNormal, lResult = mVoice->SetPriority (pHighPriority?SPVPRI_ALERT:SPVPRI_NORMAL))))
		)
	{
		LogSapi5Err (LogNormal, mVoice->SetInterest (SPFEI_ALL_TTS_EVENTS, SPFEI_ALL_TTS_EVENTS));
		LogSapi5Err (LogNormal, mVoice->SetNotifyCallbackFunction (&VoiceNotifyCallback, 0, (LPARAM)this));
		mPrepared = true;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi5Voice::Speak (LPCTSTR pMessage, bool pAsync)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(_IsValid ())
	{
		DWORD				lFlags = SPF_PURGEBEFORESPEAK|SPF_IS_XML;
		tMallocPtr <WCHAR>	lMessage = AtlAllocTaskWideString (pMessage);

		if	(pAsync)
		{
			lFlags |= SPF_ASYNC;
		}
		mLastVoiceEvent = SPEI_UNDEFINED;
		lResult = mVoice->Speak (lMessage, lFlags, &mVoiceStreamNum);
	}
	if	(LogIsActive ())
	{
		LogSapi5Err (LogNormal, lResult);
	}
	return lResult;
}

HRESULT CSapi5Voice::Stop ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(_IsValid ())
	{
		LPWSTR	lSkipType = _T("SENTENCE");
		ULONG	lSkipped = 0;
		ULONG	lTotalSkipped = 0;

		while	(
					((lResult = mVoice->Skip (lSkipType, 1, &(lSkipped=0))) == S_OK)
				&&	(lSkipped > 0)
				)
		{
			lTotalSkipped += lSkipped;
		}
		lResult = MAKE_HRESULT (SEVERITY_SUCCESS, FACILITY_WIN32, lTotalSkipped);
	}
	if	(LogIsActive ())
	{
		LogSapi5Err (LogNormal, lResult);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::GetVoiceId ()
{
	tBstrPtr lVoiceId;
	GetVoiceId (lVoiceId);
	return lVoiceId.Detach();
}

HRESULT CSapi5Voice::GetVoiceId (tBstrPtr & pVoiceId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lTokenId;

		pVoiceId.Free();

		if	(
				(SUCCEEDED (lResult = mVoice->GetVoice (&lToken)))
			&&	(SUCCEEDED (lResult = lToken->GetId (lTokenId.Free ())))
			)
		{
			pVoiceId.Attach (CAtlString (lTokenId).AllocSysString ());
		}
	}
	return lResult;
}

HRESULT CSapi5Voice::SetVoiceId (LPCTSTR pVoiceId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		CAtlString			lNewVoiceId (pVoiceId);
		ISpObjectTokenPtr	lOldToken;
		ISpObjectTokenPtr	lNewToken;
		tMallocPtr <WCHAR>	lOldTokenId;
		tMallocPtr <WCHAR>	lNewTokenId;

		if	(lNewVoiceId.IsEmpty ())
		{
			SpGetDefaultTokenIdFromCategoryId (SPCAT_VOICES, lNewTokenId.Free ());
		}
		else
		{
			lNewTokenId = AtlAllocTaskWideString (LongVoiceId (lNewVoiceId));
		}

		if	(
				((LPCWSTR) lNewTokenId != NULL)
			&&	(SUCCEEDED (lResult = mVoice->GetVoice (&lOldToken)))
			&&	(SUCCEEDED (lResult = lOldToken->GetId (lOldTokenId.Free ())))
			)
		{
			if	(wcscmp (lOldTokenId, lNewTokenId) == 0)
			{
				lResult = S_FALSE;
			}
			else
			if	(
					(SUCCEEDED (lResult = SpGetTokenFromId (lNewTokenId, &lNewToken, NULL)))
				&&	(SUCCEEDED (lResult = mVoice->SetVoice (lNewToken)))
				)
			{
				lResult = S_OK;
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::GetOutputId ()
{
	tBstrPtr lOutputId;
	GetOutputId (lOutputId);
	return lOutputId.Detach();
}

HRESULT CSapi5Voice::GetOutputId (tBstrPtr & pOutputId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lTokenId;

		pOutputId.Free();

		if	(
				(SUCCEEDED (lResult = mVoice->GetOutputObjectToken (&lToken)))
			&&	(SUCCEEDED (lResult = lToken->GetId (lTokenId.Free ())))
			)
		{
			pOutputId.Attach (CAtlString (lTokenId).AllocSysString ());
		}
	}
	return lResult;
}

HRESULT CSapi5Voice::SetOutputId (LPCTSTR pOutputId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		CAtlString			lNewOutputId (pOutputId);
		ISpObjectTokenPtr	lOldToken;
		ISpObjectTokenPtr	lNewToken;
		tMallocPtr <WCHAR>	lOldTokenId;
		tMallocPtr <WCHAR>	lNewTokenId;

		if	(lNewOutputId.IsEmpty ())
		{
			SpGetDefaultTokenIdFromCategoryId (SPCAT_AUDIOOUT, lNewTokenId.Free ());
		}
		else
		{
			lNewTokenId = AtlAllocTaskWideString (CSapi5Voice::LongOutputId (lNewOutputId));
		}

		if	(
				(SUCCEEDED (lResult = mVoice->GetOutputObjectToken (&lOldToken)))
			&&	(SUCCEEDED (lResult = lOldToken->GetId (lOldTokenId.Free ())))
			)
		{
			if	(
					(!lNewTokenId)
				||	(wcscmp (lOldTokenId, lNewTokenId) == 0)
				)
			{
				lResult = S_FALSE;
			}
			else
			if	(
					(SUCCEEDED (lResult = SpGetTokenFromId (lNewTokenId, &lNewToken, NULL)))
				&&	(SUCCEEDED (lResult = mVoice->SetOutput (lNewToken, TRUE)))
				)
			{
				lResult = S_OK;
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::GetVoiceName ()
{
	tBstrPtr lVoiceName;
	GetVoiceName (lVoiceName);
	return lVoiceName.Detach ();
}

HRESULT CSapi5Voice::GetVoiceName (tBstrPtr & pVoiceName)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lDescription;

		pVoiceName.Free();

		if	(
				(SUCCEEDED (lResult = mVoice->GetVoice (&lToken)))
			&&	(SUCCEEDED (lResult = SpGetDescription (lToken, lDescription.Free (), NULL)))
			)
		{
			pVoiceName.Attach (CAtlString (lDescription).AllocSysString ());
		}
	}
	return lResult;
}

HRESULT CSapi5Voice::GetVoiceLanguages (CAtlTypeArray <LANGID> & pLanguages)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		ISpObjectTokenPtr	lToken;
		ISpDataKeyPtr		lAttributes;
		tMallocPtr <WCHAR>	lLanguages;
		CAtlStringArray		lLanguageArray;

		pLanguages.RemoveAll ();

		if	(
				(SUCCEEDED (lResult = mVoice->GetVoice (&lToken)))
			&&	(SUCCEEDED (lResult = lToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
			&&	(SUCCEEDED (lResult = lAttributes->GetStringValue (L"Language", lLanguages.Free ())))
			)
		{
			int		lNdx;
			DWORD	lValue;
			LPTSTR	lValueEnd;

			lResult = S_FALSE;
			MakeStringArray (CAtlString ((BSTR)lLanguages), lLanguageArray, _T(";"));

			for	(lNdx = 0; lNdx <= lLanguageArray.GetUpperBound(); lNdx++)
			{
				if	(
						(lValue = _tcstoul (lLanguageArray [lNdx], &lValueEnd, 16))
					&&	(*lValueEnd == 0)
					)
				{
					pLanguages.Add ((LANGID) lValue);
					lResult = S_OK;
				}
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::GetDisplayName ()
{
	return CSapiVoice::GetDisplayName ();
}

HRESULT CSapi5Voice::GetDisplayName (tBstrPtr & pDisplayName)
{
	return GetVoiceName (pDisplayName);
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::GetUniqueId ()
{
	return CSapiVoice::GetUniqueId ();
}

HRESULT CSapi5Voice::GetUniqueId (tBstrPtr & pUniqueId)
{
	HRESULT	lResult = GetVoiceId (pUniqueId);
	if	(!pUniqueId.IsEmpty())
	{
		pUniqueId = ShortVoiceId (CAtlString (pUniqueId));
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

ULONG CSapi5Voice::GetRate ()
{
	return CSapiVoice::GetRate();
}

HRESULT CSapi5Voice::GetRate (ULONG & pRate)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		long	lRate = 0;

		if	(SUCCEEDED (lResult = mVoice->GetRate (&lRate)))
		{
			lRate = min (max (lRate, -10), 10);
			lRate += 11;
		}
		pRate = (ULONG) lRate;
	}
	return lResult;
}

HRESULT CSapi5Voice::SetRate (ULONG pRate)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		long				lNewRate = (long) pRate;
		long				lOldRate = 0;
		ISpObjectTokenPtr	lToken;

		if	(
				(lNewRate <= 0)
			&&	(SUCCEEDED (SpGetTokenFromId (SPREG_USER_ROOT L"\\Voices", &lToken, FALSE)))
			&&	(SUCCEEDED (lToken->GetDWORD (SPVOICECATEGORY_TTSRATE, (DWORD *) &lNewRate)))
			)
		{
			lNewRate = min (max (lNewRate, -10), 10);
			lNewRate += 11;
		}

		if	(lNewRate > 0)
		{
			lNewRate -= 11;
			lNewRate = min (max (lNewRate, -10), 10);
		}

		if	(SUCCEEDED (lResult = mVoice->GetRate (&lOldRate)))
		{
			if	(lNewRate == lOldRate)
			{
				lResult = S_FALSE;
			}
			else
			if	(SUCCEEDED (lResult = mVoice->SetRate (lNewRate)))
			{
				lResult = S_OK;
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

USHORT CSapi5Voice::GetVolume ()
{
	return CSapiVoice::GetVolume();
}

HRESULT CSapi5Voice::GetVolume (USHORT & pVolume)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		pVolume = 0;
		lResult = mVoice->GetVolume (&pVolume);
	}
	return lResult;
}

HRESULT CSapi5Voice::SetVolume (USHORT pVolume)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid ())
	{
		USHORT	lOldVolume = 0;

		pVolume = min (pVolume, 100);

		if	(SUCCEEDED (lResult = mVoice->GetVolume (&lOldVolume)))
		{
			if	(pVolume == lOldVolume)
			{
				lResult = S_FALSE;
			}
			else
			if	(SUCCEEDED (lResult = mVoice->SetVolume (pVolume)))
			{
				lResult = S_OK;
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::ShortVoiceId (LPCTSTR pLongVoiceId)
{
	CAtlString	lVoiceId (pLongVoiceId);

	PathStripPath (lVoiceId.GetBuffer (lVoiceId.GetLength ()));
	lVoiceId.ReleaseBuffer ();
	return lVoiceId.AllocSysString();
}

tBstrPtr CSapi5Voice::LongVoiceId (LPCTSTR pShortVoiceId)
{
	CAtlString	lVoiceId;

	PathCombine (lVoiceId.GetBuffer (MAX_PATH), SPCAT_VOICES, _T("Tokens"));
	PathAppend (lVoiceId.GetBuffer (MAX_PATH), PathFindFileName (pShortVoiceId));
	lVoiceId.ReleaseBuffer ();
	return lVoiceId.AllocSysString();
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Voice::ShortOutputId (LPCTSTR pLongOutputId)
{
	CAtlString	lOutputId (pLongOutputId);

	PathStripPath (lOutputId.GetBuffer (lOutputId.GetLength ()));
	lOutputId.ReleaseBuffer ();
	return lOutputId.AllocSysString();
}

tBstrPtr CSapi5Voice::LongOutputId (LPCTSTR pShortOutputId)
{
	CAtlString	lOutputId;

	PathCombine (lOutputId.GetBuffer (MAX_PATH), SPMMSYS_AUDIO_OUT_TOKEN_ID, PathFindFileName (pShortOutputId));
	lOutputId.ReleaseBuffer ();
	return lOutputId.AllocSysString();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int VoiceVisemeOverlay (int pViseme)
{
	int	lMouthOverlayNdx = -1;

	switch (pViseme)
	{
		case SP_VISEME_0:
		case SP_VISEME_21:
		{
			lMouthOverlayNdx = MouthOverlayClosed;
		}	break;
		case SP_VISEME_12:
		case SP_VISEME_13:
		case SP_VISEME_14:
		case SP_VISEME_15:
		case SP_VISEME_16:
		case SP_VISEME_17:
		case SP_VISEME_18:
		case SP_VISEME_19:
		case SP_VISEME_20:
		{
			lMouthOverlayNdx = MouthOverlayWide1;
		}	break;
		case SP_VISEME_1:
		case SP_VISEME_5:
		{
			lMouthOverlayNdx = MouthOverlayWide2;
		}	break;
		case SP_VISEME_11:
		case SP_VISEME_4:
		{
			lMouthOverlayNdx = MouthOverlayWide3;
		}	break;
		case SP_VISEME_2:
		{
			lMouthOverlayNdx = MouthOverlayWide4;
		}	break;
		case SP_VISEME_3:
		case SP_VISEME_6:
		case SP_VISEME_9:
		{
			lMouthOverlayNdx = MouthOverlayMedium;
		}	break;
		case SP_VISEME_7:
		case SP_VISEME_8:
		case SP_VISEME_10:
		{
			lMouthOverlayNdx = MouthOverlayNarrow;
		}	break;
	}

	return lMouthOverlayNdx;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString VoiceVisemeStr (int pViseme)
{
	CAtlString	lVoiceVisemeStr;

	lVoiceVisemeStr.Format (_T("%2u"), pViseme);

	switch (pViseme)
	{
		case SP_VISEME_0:	lVoiceVisemeStr += _T(" silence                                     "); break;
		case SP_VISEME_1:	lVoiceVisemeStr += _T(" ae (cAt), ax (Ago), ah (cUt)                "); break;
		case SP_VISEME_2:	lVoiceVisemeStr += _T(" aa (fAther)                                 "); break;
		case SP_VISEME_3:	lVoiceVisemeStr += _T(" ao (dOg)                                    "); break;
		case SP_VISEME_4:	lVoiceVisemeStr += _T(" ey (Ate), eh (pEt), uh (bOOk)               "); break;
		case SP_VISEME_5:	lVoiceVisemeStr += _T(" er (fUR)                                    "); break;
		case SP_VISEME_6:	lVoiceVisemeStr += _T(" y (Yard), iy (fEEl), ih (fIll)              "); break;
		case SP_VISEME_7:	lVoiceVisemeStr += _T(" w (With), uw (tOO)                          "); break;
		case SP_VISEME_8:	lVoiceVisemeStr += _T(" ow (gO)                                     "); break;
		case SP_VISEME_9:	lVoiceVisemeStr += _T(" aw (fOUl)                                   "); break;
		case SP_VISEME_10:	lVoiceVisemeStr += _T(" oy (tOY)                                    "); break;
		case SP_VISEME_11:	lVoiceVisemeStr += _T(" ay (bIte)                                   "); break;
		case SP_VISEME_12:	lVoiceVisemeStr += _T(" h (Help)                                    "); break;
		case SP_VISEME_13:	lVoiceVisemeStr += _T(" r (Red)                                     "); break;
		case SP_VISEME_14:	lVoiceVisemeStr += _T(" l (Lid)                                     "); break;
		case SP_VISEME_15:	lVoiceVisemeStr += _T(" s (Sit), z (Zap)                            "); break;
		case SP_VISEME_16:	lVoiceVisemeStr += _T(" sh (SHe), ch (CHin), jh (Joy), zh (pleaSure)"); break;
		case SP_VISEME_17:	lVoiceVisemeStr += _T(" th (THin), dh (THen)                        "); break;
		case SP_VISEME_18:	lVoiceVisemeStr += _T(" f (Fork), v (Vat)                           "); break;
		case SP_VISEME_19:	lVoiceVisemeStr += _T(" d (Dig), t (talk), n (No)                   "); break;
		case SP_VISEME_20:	lVoiceVisemeStr += _T(" k (Cut), g (Gut), ng (siNG)                 "); break;
		case SP_VISEME_21:	lVoiceVisemeStr += _T(" p (Put), b (Big), m (Mat)                   "); break;
	}

	return lVoiceVisemeStr;
}
