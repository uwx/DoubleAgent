/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCmnTTSEngine.h"
#include "AgentFile.h"
#include "SapiVoiceCache.h"
#include "Sapi5Voices.h"
#include "Sapi5Voice.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#include "Sapi4Voice.h"
#endif
#include "GuidStr.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnTTSEngine::CDaCmnTTSEngine ()
:	mSapi5Voice (NULL)
{
#ifndef	_WIN64
	mSapi4Voice = NULL;
#endif
}

CDaCmnTTSEngine::~CDaCmnTTSEngine()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnTTSEngine::Initialize (CSapi5VoiceInfo * pVoiceInfo)
{
	mSapi5Voice = pVoiceInfo;
}

#ifndef	_WIN64
void CDaCmnTTSEngine::Initialize (CSapi4VoiceInfo * pVoiceInfo)
{
	mSapi4Voice = pVoiceInfo;
}
#endif

bool CDaCmnTTSEngine::Initialize (CSapi5Voice * pVoice)
{
	bool				lRet = false;
	CSapiVoiceCache *	lVoiceCache;
	CSapi5Voices *		lSapi5Voices;

	if	(
			(pVoice)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		&&	(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
		&&	(mSapi5Voice = lSapi5Voices->GetVoiceId (CString (pVoice->GetUniqueId ())))
		)
	{
		lRet = true;
	}
	return lRet;
}

#ifndef	_WIN64
bool CDaCmnTTSEngine::Initialize (CSapi4Voice * pVoice)
{
	bool				lRet = false;
	CSapiVoiceCache *	lVoiceCache;
	CSapi4Voices *		lSapi4Voices;

	if	(
			(pVoice)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		&&	(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
		&&	(mSapi4Voice = lSapi4Voices->GetModeId (CGuidStr::Parse (pVoice->GetUniqueId ())))
		)
	{
		lRet = true;
	}
	return lRet;
}
#endif

bool CDaCmnTTSEngine::Initialize (CSapiVoice * pVoice)
{
	bool				lRet = false;
	CSapiVoiceCache *	lVoiceCache;
	CSapi5Voices *		lSapi5Voices;
#ifndef	_WIN64
	CSapi4Voices *		lSapi4Voices;
#endif

	if	(
			(pVoice)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		&&	(
				(
					(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
				&&	(mSapi5Voice = lSapi5Voices->GetVoiceId (CString (pVoice->GetUniqueId ())))
				)
#ifndef	_WIN64
			||	(
					(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
				&&	(mSapi4Voice = lSapi4Voices->GetModeId (CGuidStr::Parse (pVoice->GetUniqueId ())))
				)
#endif
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaCmnTTSEngine::Initialize (CAgentFile * pFile)
{
	bool				lRet = false;
	CSapiVoiceCache *	lVoiceCache;
	tPtr <CSapiVoice>	lSapiVoice;

	if	(
			(pFile)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		&&	(lSapiVoice = lVoiceCache->GetAgentVoice (pFile->GetTts(), true, false))
		)
	{
		lRet = Initialize (lSapiVoice);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnTTSEngine::get_TTSModeID (BSTR *TTSModeID)
{
	HRESULT	lResult = S_OK;

	if	(!TTSModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*TTSModeID) = SysAllocString (mSapi5Voice->mVoiceIdShort);
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		CString	lTTSModeId = (CString) CGuidStr (mSapi4Voice->mModeId);
		(*TTSModeID) = lTTSModeId.AllocSysString ();
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnTTSEngine::get_DisplayName (BSTR *DisplayName)
{
	HRESULT	lResult = S_OK;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*DisplayName) = SysAllocString (mSapi5Voice->mVoiceName);
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*DisplayName) = SysAllocString (mSapi4Voice->mVoiceName);
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnTTSEngine::get_Manufacturer (BSTR *Manufacturer)
{
	HRESULT	lResult = S_OK;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*Manufacturer) = SysAllocString (mSapi5Voice->mManufacturer);
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*Manufacturer) = SysAllocString (mSapi4Voice->mManufacturer);
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnTTSEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
	HRESULT	lResult = S_OK;

	if	(mSapi5Voice)
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = 5;
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = 0;
		}
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = 4;
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = 0;
		}
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnTTSEngine::get_Gender (SpeechGenderType *Gender)
{
	HRESULT	lResult = S_OK;

	if	(!Gender)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*Gender) = (SpeechGenderType)mSapi5Voice->mSpeakerGender;
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*Gender) = (SpeechGenderType)mSapi4Voice->mSpeakerGender;
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnTTSEngine::get_LanguageID (long *LanguageID)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*LanguageID) = (long)mSapi5Voice->mLangId;
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*LanguageID) = (long)mSapi4Voice->mLangId;
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnTTSEngine::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
		int 	lInfoSize;
		CString lInfoValue;

		if	(lInfoSize = GetLocaleInfo (MAKELCID (mSapi5Voice->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
		{
			GetLocaleInfo (MAKELCID (mSapi5Voice->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
		}
		else
		{
			lResult = S_FALSE;
		}
		lInfoValue.ReleaseBuffer ();
		(*LanguageName) = lInfoValue.AllocSysString ();
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
		int 	lInfoSize;
		CString lInfoValue;

		if	(lInfoSize = GetLocaleInfo (MAKELCID (mSapi4Voice->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
		{
			GetLocaleInfo (MAKELCID (mSapi4Voice->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
		}
		else
		{
			lResult = S_FALSE;
		}
		lInfoValue.ReleaseBuffer ();
		(*LanguageName) = lInfoValue.AllocSysString ();
	}
#endif
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}
