/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "Sapi5VoicePrivate.h"
#include "Sapi5Err.h"
#include "MallocPtr.h"
#include <sphelper.h>

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR sUserVoices = SPREG_USER_ROOT L"\\PrivateVoices";
static LPCTSTR sAttributesKey = _T("Attributes");

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi5VoicePrivate)

CSapi5VoicePrivate::CSapi5VoicePrivate (LPCTSTR pVoiceId)
:	mTokenIsPrivate (true)
{
	ISpObjectTokenPtr	lToken;

	LogSapi5Err (LogIfActive|LogTime, SpCreateNewToken (sUserVoices, pVoiceId, &lToken));
	mToken = lToken;
	mVoice = NULL;
}

CSapi5VoicePrivate::CSapi5VoicePrivate (LPCTSTR pTokenPath, LPCTSTR pVoiceId)
:	mTokenIsPrivate (false)
{
	ISpObjectTokenPtr	lToken;

	LogSapi5Err (LogIfActive|LogTime, SpCreateNewToken (pTokenPath, pVoiceId, &lToken));
	mToken = lToken;
	mVoice = NULL;
}

CSapi5VoicePrivate::~CSapi5VoicePrivate ()
{
	CSapi5Voice::~CSapi5Voice ();

	if	(mTokenIsPrivate)
	{
		try
		{
			ISpObjectTokenPtr	lToken;

			if	(lToken = mToken)
			{
				LogSapi5Err (LogNormal|LogTime, lToken->Remove (NULL));
			}
		}
		catch AnyExceptionSilent
	}

	SafeFreeSafePtr (mToken);
}

CSapi5VoicePrivate* CSapi5VoicePrivate::CreateInstance (LPCTSTR pVoiceId)
{
	return new CSapi5VoicePrivate (pVoiceId);
}

CSapi5VoicePrivate* CSapi5VoicePrivate::CreateInstance (LPCTSTR pTokenPath, LPCTSTR pVoiceId)
{
	return new CSapi5VoicePrivate (pTokenPath, pVoiceId);
}

/////////////////////////////////////////////////////////////////////////////

UINT CSapi5VoicePrivate::_IsValid () const
{
	return (mToken != NULL) ? CSapi5Voice::_IsValid () : 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi5VoicePrivate::GetVoiceId (tBstrPtr& pVoiceId)
{
	HRESULT	lResult = E_FAIL;

	pVoiceId = NULL;

	if	(SafeIsValid ())
	{
		lResult = CSapi5Voice::GetVoiceId (pVoiceId);
	}
	else
	if	(mToken != NULL)
	{
		ISpObjectTokenPtr	lToken (mToken);
		tMallocPtr <WCHAR>	lVoiceId;

		if	(SUCCEEDED (lResult = lToken->GetId (lVoiceId.Free ())))
		{
			pVoiceId.Attach (SysAllocString ((LPCWSTR)lVoiceId));
		}
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::SetDisplayName (LPCTSTR pDisplayName)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pDisplayName)
			&&	(pDisplayName[0])
			)
		{
			tMallocPtr <WCHAR>	lDisplayName;

			lDisplayName = AtlAllocTaskWideString (pDisplayName);
			lResult = mToken->SetStringValue (NULL, lDisplayName);
			
			if	(SUCCEEDED (LogSapi5Err (LogNormal, lResult)))
			{
				SetAttribute (L"Name", pDisplayName);
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::GetDisplayName (tBstrPtr& pDisplayName)
{
	HRESULT	lResult = E_FAIL;

	pDisplayName = NULL;

	if	(SafeIsValid ())
	{
		lResult = CSapi5Voice::GetDisplayName (pDisplayName);
	}
	else
	if	(mToken != NULL)
	{
		lResult = GetAttribute (L"Name", pDisplayName);
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::SetLanguage (LANGID pLanguageId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(pLanguageId)
		{
			CAtlString	lLanguageId;

			lLanguageId.Format (L"%x", pLanguageId);
			lResult = SetAttribute (L"Language", lLanguageId);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::GetLanguage (LANGID& pLanguageId)
{
	HRESULT	lResult = E_FAIL;

	pLanguageId = 0;

	if	(mToken != NULL)
	{
		tBstrPtr	lLanguageId;

		if	(
				(SUCCEEDED (lResult = GetAttribute (L"Language", lLanguageId)))
			&&	((LPCWSTR)lLanguageId)
			)
		{
			pLanguageId = (LANGID) wcstoul ((LPCWSTR)lLanguageId, NULL, 16);
		}
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::SetAge (LPCTSTR pAgeString)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pAgeString)
			&&	(pAgeString[0])
			)
		{
			lResult = SetAttribute (L"Age", pAgeString);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::GetAge (tBstrPtr& pAge)
{
	HRESULT	lResult = E_FAIL;

	pAge = NULL;

	if	(mToken != NULL)
	{
		lResult = GetAttribute (L"Age", pAge);
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::SetGender (LPCTSTR pGenderString)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pGenderString)
			&&	(pGenderString[0])
			)
		{
			lResult = SetAttribute (L"Gender", pGenderString);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::GetGender (tBstrPtr& pGenderString)
{
	HRESULT	lResult = E_FAIL;

	pGenderString = NULL;

	if	(mToken != NULL)
	{
		lResult = GetAttribute (L"Gender", pGenderString);
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::SetVendor (LPCTSTR pVendorName)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pVendorName)
			&&	(pVendorName[0])
			)
		{
			lResult = SetAttribute (L"Vendor", pVendorName);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::GetVendor (tBstrPtr& pVendorName)
{
	HRESULT	lResult = E_FAIL;

	pVendorName = NULL;

	if	(mToken != NULL)
	{
		lResult = GetAttribute (L"Vendor", pVendorName);
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::SetVersion (LPCTSTR pVersionString)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pVersionString)
			&&	(pVersionString[0])
			)
		{
			lResult = SetAttribute (L"Version", pVersionString);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::GetVersion (tBstrPtr& pVersionString)
{
	HRESULT	lResult = E_FAIL;

	pVersionString = NULL;

	if	(mToken != NULL)
	{
		lResult = GetAttribute (L"Version", pVersionString);
	}
	return LogSapi5Err (LogNormal, lResult);
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5VoicePrivate::GetStringValue (LPCTSTR pValueId)
{
	tBstrPtr lStringValue;
	GetStringValue (pValueId, lStringValue);
	return lStringValue.Detach();
}

HRESULT CSapi5VoicePrivate::GetStringValue (LPCTSTR pValueId, tBstrPtr& pStringValue)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pValueId)
			&&	(pValueId[0])
			)
		{
			ISpDataKeyPtr		lKeyToken;
			CAtlString			lValueId (pValueId);
 			tMallocPtr <WCHAR>	lStringValue;

			if	(SUCCEEDED (GetKeyToken (mToken, lValueId, lKeyToken, lValueId)))
			{
				lResult = lKeyToken->GetStringValue (lValueId, lStringValue.Free());
				if	(SUCCEEDED (LogSapi5Err (LogNormal, lResult)))
				{
					pStringValue.Attach (CAtlString (lStringValue).AllocSysString ());
				}
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::SetStringValue (LPCTSTR pValueId, LPCTSTR pStringValue)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		)
	{
		if	(
				(pValueId)
			&&	(pValueId[0])
			&&	(pStringValue)
			&&	(pStringValue[0])
			)
		{
			ISpDataKeyPtr		lKeyToken;
			CAtlString			lValueId (pValueId);
			tMallocPtr <WCHAR>	lStringValue;

			if	(SUCCEEDED (GetKeyToken (mToken, lValueId, lKeyToken, lValueId, true)))
			{
				lStringValue = AtlAllocTaskWideString (pStringValue);
				lResult = lKeyToken->SetStringValue (lValueId, lStringValue);
				LogSapi5Err (LogNormal, lResult);
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5VoicePrivate::GetFilePath (LPCTSTR pFileId)
{
	return GetStringValue (pFileId);
}

HRESULT CSapi5VoicePrivate::GetFilePath (LPCTSTR pFileId, tBstrPtr& pFilePath)
{
	return GetStringValue (pFileId, pFilePath);
}

HRESULT CSapi5VoicePrivate::SetFilePath (LPCTSTR pFileId, LPCTSTR pFilePath)
{
	CAtlString	lFilePath;

	if	(!ExpandEnvironmentStrings (pFilePath, lFilePath.GetBuffer(MAX_PATH), MAX_PATH))
	{
		lFilePath = pFilePath;
	}
	return SetStringValue (pFileId, lFilePath);
}

/////////////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5VoicePrivate::GetAttribute (LPCTSTR pAttributeName)
{
	tBstrPtr lAttribute;
	GetAttribute (pAttributeName, lAttribute);
	return lAttribute.Detach();
}

HRESULT CSapi5VoicePrivate::GetAttribute (LPCTSTR pAttributeName, tBstrPtr& pAttributeValue)
{
	HRESULT			lResult = E_UNEXPECTED;
	ISpDataKeyPtr	lAttributesToken;

	if	(
			(mToken != NULL)
		&&	(!CSapi5Voice::_IsPrepared ())
		&&	(SUCCEEDED (lResult = GetAttributesToken (lAttributesToken)))
		)
	{
		if	(
				(pAttributeName)
			&&	(pAttributeName[0])
			)
		{
			tMallocPtr <WCHAR>	lAttributeName;
 			tMallocPtr <WCHAR>	lAttributeValue;

			lAttributeName = AtlAllocTaskWideString (pAttributeName);
			lResult = lAttributesToken->GetStringValue (lAttributeName, lAttributeValue.Free());
			if	(SUCCEEDED (LogSapi5Err (LogNormal, lResult)))
			{
				pAttributeValue.Attach (CAtlString (lAttributeValue).AllocSysString ());
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CSapi5VoicePrivate::SetAttribute (LPCTSTR pAttributeName, LPCTSTR pAttributeValue)
{
	HRESULT			lResult = E_UNEXPECTED;
	ISpDataKeyPtr	lAttributesToken;

	if	(
			(mToken != NULL)
		&&	(mTokenIsPrivate)
		&&	(!CSapi5Voice::_IsPrepared ())
		&&	(SUCCEEDED (lResult = GetAttributesToken (lAttributesToken, true)))
		)
	{
		if	(
				(pAttributeName)
			&&	(pAttributeName[0])
			&&	(pAttributeValue)
			&&	(pAttributeValue[0])
			)
		{
			tMallocPtr <WCHAR>	lAttributeName;
 			tMallocPtr <WCHAR>	lAttributeValue;

			lAttributeName = AtlAllocTaskWideString (pAttributeName);
			lAttributeValue = AtlAllocTaskWideString (pAttributeValue);
			lResult = lAttributesToken->SetStringValue (lAttributeName, lAttributeValue);
			LogSapi5Err (LogNormal, lResult);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi5VoicePrivate::GetKeyToken (ISpDataKey* pParentToken, LPCTSTR pValuePath, ISpDataKeyPtr& pToken, CAtlString& pValueId, bool pCreate)
{
	HRESULT	lResult = S_FALSE;
	LPCTSTR	lNextComponent;

	pToken = pParentToken;
	pValueId = pValuePath;

	while	(
				(SUCCEEDED (lResult))
			&&	(lNextComponent = PathFindNextComponent (pValuePath))
			&&	(*lNextComponent)
			)
	{
		CAtlString	lKeyName (pValueId, (int)(lNextComponent-pValuePath-1));

		if	(SUCCEEDED (lResult = pCreate ? pParentToken->CreateKey (lKeyName, &pToken) : pParentToken->OpenKey (lKeyName, &pToken)))
		{
			pValueId = pValueId.Mid (lKeyName.GetLength()+1);
			pParentToken = pToken;
			pValuePath = pValueId;
		}
	}
	return LogSapi5Err (LogNormal, lResult);
}

HRESULT CSapi5VoicePrivate::GetAttributesToken (ISpDataKeyPtr& pToken, bool pCreate, LPCTSTR pAttributesKey)
{
	HRESULT	lResult;

	if	(!pAttributesKey)
	{
		pAttributesKey = sAttributesKey;
	}
	lResult = pCreate ? mToken->CreateKey (pAttributesKey, &pToken) : mToken->OpenKey (pAttributesKey, &pToken);
	return LogSapi5Err (LogNormal, lResult);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CSapi5VoicePrivate::PrepareToSpeak (bool pHighPriority)
{
	HRESULT				lResult = E_UNEXPECTED;
	ISpObjectTokenPtr	lToken;

	if	(CSapi5Voice::_IsPrepared ())
	{
		lResult = CSapi5Voice::PrepareToSpeak (pHighPriority);
	}
	else
	if	(
			((lToken = mToken))
		&&	(SUCCEEDED (lResult = CoCreateInstance (CLSID_SpVoice, NULL, CLSCTX_SERVER, __uuidof (ISpVoice), (void**) &mVoice)))
		)
	{
		if	(SUCCEEDED (lResult = mVoice->SetVoice (lToken)))
		{
			lResult = CSapi5Voice::PrepareToSpeak (pHighPriority);
		}
		if	(FAILED (lResult))
		{
			SafeFreeSafePtr (mVoice);
		}
	}

	return LogSapi5Err (LogNormal, lResult);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSapi5VoicePrivate::Log (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle (pTitle);
			CAtlString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Sapi5Voice");
			}
			if	(mToken)
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, mToken.GetInterfacePtr());
				LogDataKey (pLogLevel, mToken, lIndent);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s <null>"), lIndent, lTitle);
			}
		}
		catch AnyExceptionDebug
	}
}
