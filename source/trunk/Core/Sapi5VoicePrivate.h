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
#pragma once
#include "Sapi5Voice.h"

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));
_COM_SMARTPTR_TYPEDEF (ISpDataKey, __uuidof(ISpDataKey));

//////////////////////////////////////////////////////////////////////

class CSapi5VoicePrivate : public CSapi5Voice
{
	DECLARE_DLL_OBJECT_EX(CSapi5VoicePrivate, _DACORE_IMPEXP)
protected:
	CSapi5VoicePrivate (LPCTSTR pVoiceId = NULL);
	CSapi5VoicePrivate (LPCTSTR pTokenPath, LPCTSTR pVoiceId);
public:
	_DACORE_IMPEXP virtual ~CSapi5VoicePrivate ();
	_DACORE_IMPEXP static CSapi5VoicePrivate* CreateInstance (LPCTSTR pVoiceId = NULL);
	_DACORE_IMPEXP static CSapi5VoicePrivate* CreateInstance (LPCTSTR pTokenPath, LPCTSTR pVoiceId);

// Attributes
	_DACORE_IMPEXP virtual HRESULT GetVoiceId (tBstrPtr& pVoiceId);
	_DACORE_IMPEXP HRESULT SetDisplayName (LPCTSTR pDisplayName);
	_DACORE_IMPEXP virtual HRESULT GetDisplayName (tBstrPtr& pDisplayName);
	_DACORE_IMPEXP HRESULT SetLanguage (LANGID pLanguageId);
	_DACORE_IMPEXP HRESULT GetLanguage (LANGID& pLanguageId);
	_DACORE_IMPEXP HRESULT SetAge (LPCTSTR pAgeString); // "Child", "Teen", "Adult", or "Senior"
	_DACORE_IMPEXP HRESULT GetAge (tBstrPtr& pAge);
	_DACORE_IMPEXP HRESULT SetGender (LPCTSTR pGenderString); // "Male" or "Female"
	_DACORE_IMPEXP HRESULT GetGender (tBstrPtr& pGenderString);
	_DACORE_IMPEXP HRESULT SetVendor (LPCTSTR pVendorName);
	_DACORE_IMPEXP HRESULT GetVendor (tBstrPtr& pVendorName);
	_DACORE_IMPEXP HRESULT SetVersion (LPCTSTR pVersionString);
	_DACORE_IMPEXP HRESULT GetVersion (tBstrPtr& pVersionString);

	_DACORE_IMPEXP HRESULT GetStringValue (LPCTSTR pValueId, tBstrPtr& pStringValue);
	_DACORE_IMPEXP tBstrPtr GetStringValue (LPCTSTR pValueId);
	_DACORE_IMPEXP HRESULT SetStringValue (LPCTSTR pValueId, LPCTSTR pStringValue);
	_DACORE_IMPEXP HRESULT GetFilePath (LPCTSTR pFileId, tBstrPtr& pFilePath);
	_DACORE_IMPEXP tBstrPtr GetFilePath (LPCTSTR pFileId);
	_DACORE_IMPEXP HRESULT SetFilePath (LPCTSTR pFileId, LPCTSTR pFilePath);
	_DACORE_IMPEXP HRESULT GetAttribute (LPCTSTR pAttributeName, tBstrPtr& pAttributeValue);
	_DACORE_IMPEXP tBstrPtr GetAttribute (LPCTSTR pAttributeName);
	_DACORE_IMPEXP HRESULT SetAttribute (LPCTSTR pAttributeName, LPCTSTR pAttributeValue);

// Operations
	_DACORE_IMPEXP HRESULT virtual PrepareToSpeak (bool pHighPriority = false);
	_DACORE_IMPEXP virtual void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);

// Overrides
protected:
	_DACORE_IMPEXP virtual UINT _IsValid () const;

// Implementation
public:
	__declspec(property(get=_get_InitToken))		ISpDataKey* InitToken;
	__declspec(property(get=_get_TokenIsPrivate))	bool		TokenIsPrivate;

	_DACORE_IMPEXP ISpDataKey* _get_InitToken () const {return mToken.GetInterfacePtr();}
	_DACORE_IMPEXP bool _get_TokenIsPrivate () const {return mTokenIsPrivate;}
protected:
	_DACORE_IMPEXP HRESULT GetKeyToken (ISpDataKey* pParentToken, LPCTSTR pValuePath, ISpDataKeyPtr& pToken, CAtlString& pValueId, bool pCreate = false);
	_DACORE_IMPEXP HRESULT GetAttributesToken (ISpDataKeyPtr& pToken, bool pCreate = false, LPCTSTR pAttributesKey = NULL);

protected:
	ISpDataKeyPtr	mToken;
	bool			mTokenIsPrivate;
};

//////////////////////////////////////////////////////////////////////
