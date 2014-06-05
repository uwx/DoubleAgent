/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "DaCmnTTSPrivate.h"
#include "SapiVoiceCache.h"
#include "Sapi5Voices.h"
#include "Sapi5VoicePrivate.h"
#include "StringArrayEx.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnTTSPrivate::CDaCmnTTSPrivate ()
{
	mSapi5VoicePrivate = CSapi5VoicePrivate::CreateInstance ();
}

CDaCmnTTSPrivate::~CDaCmnTTSPrivate()
{
	SafeFreeSafePtr (mSapi5VoicePrivate);
	SafeFreeSafePtr (mPrivateVoiceInfo);
}

/////////////////////////////////////////////////////////////////////////////

CSapi5Voice* CDaCmnTTSPrivate::GetCachedVoice (CSapiVoiceClient * pClient)
{
	CSapi5Voice*		lVoice = NULL;
	CSapiVoiceCache*	lVoiceCache;

	if	(
			(mSapi5Voice)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		)
	{
		lVoice = dynamic_cast<CSapi5Voice*> (lVoiceCache->FindCachedVoice (mSapi5Voice->mVoiceIdShort));

		if	(
				(lVoice)
			&&	(pClient)
			)
		{
			lVoiceCache->AddVoiceClient (lVoice, pClient);
		}
	}
	return lVoice;
}

CSapi5Voice* CDaCmnTTSPrivate::PrepareAndCacheVoice (CSapiVoiceClient * pClient)
{
	CSapi5Voice*		lVoice = NULL;
	ISpObjectTokenPtr	lVoiceToken;
	CSapiVoiceCache*	lVoiceCache;
	CSapiVoice*			lCachedVoice = NULL;
		
	if	(	
			(mSapi5VoicePrivate)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		)
	{
		if	((lCachedVoice = lVoiceCache->FindCachedVoice (mSapi5VoicePrivate->GetUniqueId())))
		{
			lVoiceCache->UncacheVoice (lCachedVoice);
		}
		if	(!mSapi5VoicePrivate->SafeIsPrepared ())
		{
			mSapi5VoicePrivate->PrepareToSpeak ();
		}
		if	(
				(mSapi5VoicePrivate->SafeIsPrepared ())
			&&	((lVoiceToken = mSapi5VoicePrivate->InitToken))
			&&	((mPrivateVoiceInfo = CSapi5Voices::VoiceFromToken (lVoiceToken)))
			&&	(lVoiceCache->CacheVoice (mSapi5VoicePrivate, pClient))
			)
		{
			mSapi5Voice = mPrivateVoiceInfo;
			lVoice = mSapi5VoicePrivate.Detach ();
		}
	}
	return lVoice;
}
 
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnTTSPrivate::get_TTSModeID (BSTR *TTSModeID)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(TTSModeID)
		&&	(mSapi5VoicePrivate)
		)
	{
		tBstrPtr	lVoiceId;

		lResult = mSapi5VoicePrivate->GetVoiceId (lVoiceId);
		(*TTSModeID) = lVoiceId.Detach ();
	}
	else
	{
		lResult = CDaCmnTTSEngine::get_TTSModeID (TTSModeID);
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_DisplayName (BSTR *DisplayName)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(DisplayName)
		&&	(mSapi5VoicePrivate)
		)
	{
		tBstrPtr	lDisplayName;

		lResult = mSapi5VoicePrivate->GetDisplayName (lDisplayName);
		(*DisplayName) = lDisplayName.Detach ();
	}
	else
	{
		lResult = CDaCmnTTSEngine::get_DisplayName (DisplayName);
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_Manufacturer (BSTR *Manufacturer)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(Manufacturer)
		&&	(mSapi5VoicePrivate)
		)
	{
		tBstrPtr	lVendor;

		lResult = mSapi5VoicePrivate->GetVendor (lVendor);
		(*Manufacturer) = lVendor.Detach ();
	}
	else
	{
		lResult = CDaCmnTTSEngine::get_Manufacturer (Manufacturer);
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::GetVersion (short *MajorVersion, short *MinorVersion)
{
	HRESULT	lResult = S_FALSE;

	if	(MajorVersion)
	{
		(*MajorVersion) = 0;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = 0;
	}

	if	(mSapi5VoicePrivate)
	{
		tBstrPtr		lVersion;
		CAtlStringArray	lVersionParts;

		if (SUCCEEDED (lResult = mSapi5VoicePrivate->GetVersion (lVersion)))
		{
			MakeStringArray (lVersion, lVersionParts, _T("."), true);
			if	(
					(MajorVersion)
				&&	(lVersionParts.GetCount() > 0)
				)
			{
				(*MajorVersion) = (short)_tstoi (lVersionParts [0]);
			}
			if	(
					(MinorVersion)
				&&	(lVersionParts.GetCount() > 1)
				)
			{
				(*MinorVersion) = (short)_tstoi (lVersionParts [1]);
			}
		}
	}
	else
	{
		lResult = CDaCmnTTSEngine::GetVersion (MajorVersion, MinorVersion);
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_Gender (SpeechGenderType *Gender)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(Gender)
		&&	(mSapi5VoicePrivate)
		)
	{
		tBstrPtr	lGender;

		if	(SUCCEEDED (lResult = mSapi5VoicePrivate->GetGender (lGender)))
		{
			(*Gender) = (_wcsicmp (lGender, L"Male") == 0) ? SpeechGender_Male : (_wcsicmp (lGender, L"Female") == 0) ? SpeechGender_Female : SpeechGender_Neutral;
		}
	}
	else
	{
		lResult = CDaCmnTTSEngine::get_Gender (Gender);
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_LanguageID (long *LanguageID)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(LanguageID)
		&&	(mSapi5VoicePrivate)
		)
	{
		LANGID	lLanguageID = 0;

		lResult = mSapi5VoicePrivate->GetLanguage (lLanguageID);
		(*LanguageID) = lLanguageID;
	}
	else
	{
		lResult = CDaCmnTTSEngine::get_LanguageID (LanguageID);
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
	return CDaCmnTTSEngine::get_LanguageName (EnglishName, LanguageName);
}
 
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnTTSPrivate::get_InitDisplayName (BSTR *DisplayName)
{
	HRESULT	lResult = S_FALSE;

	if	(DisplayName)
	{
		(*DisplayName) = NULL;

		if	(mSapi5VoicePrivate)
		{
			tBstrPtr	lDisplayName;

			lResult = mSapi5VoicePrivate->GetDisplayName (lDisplayName);
			(*DisplayName) = lDisplayName.Detach ();
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitDisplayName (BSTR DisplayName)
{
	HRESULT	lResult = E_FAIL;

	if	(mSapi5Voice)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mSapi5VoicePrivate)
	{
		lResult = mSapi5VoicePrivate->SetDisplayName (DisplayName);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_InitManufacturer (BSTR *Manufacturer)
{
	HRESULT	lResult = S_FALSE;

	if	(Manufacturer)
	{
		(*Manufacturer)  = NULL;

		if	(mSapi5VoicePrivate)
		{
			tBstrPtr	lVendor;

			lResult = mSapi5VoicePrivate->GetVendor (lVendor);
			(*Manufacturer) = lVendor.Detach ();
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitManufacturer (BSTR Manufacturer)
{
	HRESULT	lResult = E_FAIL;

	if	(mSapi5Voice)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mSapi5VoicePrivate)
	{
		lResult = mSapi5VoicePrivate->SetVendor (Manufacturer);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_InitVersion (BSTR *Version)
{
	HRESULT	lResult = S_FALSE;

	if	(Version)
	{
		(*Version) = NULL;

		if	(mSapi5VoicePrivate)
		{
			tBstrPtr	lVersion;

			lResult = mSapi5VoicePrivate->GetVersion (lVersion);
			(*Version) = lVersion.Detach ();
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitVersion (BSTR Version)
{
	HRESULT	lResult = E_FAIL;

	if	(mSapi5Voice)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mSapi5VoicePrivate)
	{
		lResult = mSapi5VoicePrivate->SetVersion (Version);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_InitGender (SpeechGenderType *Gender)
{
	HRESULT	lResult = S_FALSE;

	if	(Gender)
	{
		(*Gender) = SpeechGender_Neutral;
		 
		if	(mSapi5VoicePrivate)
		{
			tBstrPtr	lGender;

			if	(SUCCEEDED (lResult = mSapi5VoicePrivate->GetGender (lGender)))
			{
				(*Gender) = (_wcsicmp (lGender, L"Male") == 0) ? SpeechGender_Male : (_wcsicmp (lGender, L"Female") == 0) ? SpeechGender_Female : SpeechGender_Neutral;
			}
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}


HRESULT CDaCmnTTSPrivate::put_InitGender (SpeechGenderType Gender)
{
	HRESULT	lResult = E_FAIL;

	if	(mSapi5Voice)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mSapi5VoicePrivate)
	{
		lResult = mSapi5VoicePrivate->SetGender ((Gender == SpeechGender_Male) ? _bstr_t("Male") : (Gender == SpeechGender_Female) ? _bstr_t("Female") : _bstr_t("Neutral"));
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_InitLanguageID (long *LanguageID)
{
	HRESULT	lResult = S_FALSE;

	if	(LanguageID)
	{
		(*LanguageID) = 0;

		if	(mSapi5VoicePrivate)
		{
			LANGID	lLanguageID = 0;

			lResult = mSapi5VoicePrivate->GetLanguage (lLanguageID);
			(*LanguageID) = lLanguageID;
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitLanguageID (long LanguageID)
{
	HRESULT	lResult = E_FAIL;

	if	(mSapi5Voice)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mSapi5VoicePrivate)
	{
		lResult = mSapi5VoicePrivate->SetLanguage ((LANGID)LanguageID);
	}
	else
	{
		lResult = E_UNEXPECTED;
	}
	return lResult;
}
 
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnTTSPrivate::get_InitString (BSTR ValuePath, BSTR *StringValue)
{
	HRESULT		lResult = E_FAIL;
	tBstrPtr	lStringValue;

	if	(!ValuePath)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!StringValue)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*StringValue) = NULL;

		if	(mSapi5VoicePrivate)
		{
			lResult = mSapi5VoicePrivate->GetStringValue (ValuePath, lStringValue);
			(*StringValue) = lStringValue.Detach();
		}
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitString (BSTR ValuePath, BSTR StringValue)
{
	HRESULT	lResult = E_FAIL;

	if	(	
			(!ValuePath)
		||	(!StringValue)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mSapi5VoicePrivate)
		{
			lResult = mSapi5VoicePrivate->SetStringValue (ValuePath, StringValue);
		}
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_InitAttribute (BSTR AttributeName, BSTR *AttributeValue)
{
	HRESULT		lResult = E_FAIL;
	tBstrPtr	lAttributeValue;

	if	(!AttributeName)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!AttributeValue)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AttributeValue) = NULL;

		if	(mSapi5VoicePrivate)
		{
			lResult = mSapi5VoicePrivate->GetAttribute (AttributeName, lAttributeValue);
			(*AttributeValue) = lAttributeValue.Detach();
		}
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitAttribute (BSTR AttributeName, BSTR AttributeValue)
{
	HRESULT	lResult = E_FAIL;

	if	(	
			(!AttributeName)
		||	(!AttributeValue)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mSapi5VoicePrivate)
		{
			lResult = mSapi5VoicePrivate->SetAttribute (AttributeName, AttributeValue);
		}
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::get_InitFilePath (BSTR FileId, BSTR *FilePath)
{
	HRESULT		lResult = E_FAIL;
	tBstrPtr	lFilePath;

	if	(!FileId)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!FilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePath) = NULL;

		if	(mSapi5VoicePrivate)
		{
			lResult = mSapi5VoicePrivate->GetStringValue (FileId, lFilePath);
			(*FilePath) = lFilePath.Detach();
		}
	}
	return lResult;
}

HRESULT CDaCmnTTSPrivate::put_InitFilePath (BSTR FileId, BSTR FilePath)
{
	HRESULT	lResult = E_FAIL;

	if	(	
			(!FileId)
		||	(!FilePath)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mSapi5VoicePrivate)
		{
			lResult = mSapi5VoicePrivate->SetStringValue (FileId, FilePath);
		}
	}
	return lResult;
}
 
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnTTSPrivate::InitFromRegistry (BSTR RegistryPath, BSTR VoiceId, VARIANT_BOOL *Success)
{
	HRESULT	lResult = E_FAIL;

	if	(Success)
	{
		(*Success) = VARIANT_FALSE;
	}

	if	(	
			(!RegistryPath)
		||	(!VoiceId)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(mSapi5Voice)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		SafeFreeSafePtr (mSapi5VoicePrivate);
		SafeFreeSafePtr (mPrivateVoiceInfo);
		mSapi5VoicePrivate = CSapi5VoicePrivate::CreateInstance (RegistryPath, VoiceId);

		if	(mSapi5VoicePrivate->InitToken)
		{
			if	(Success)
			{
				(*Success) = VARIANT_TRUE;
			}
			lResult = S_OK;
		}
	}
	return lResult;
}
