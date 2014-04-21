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
#pragma once
#include "DaCmnTTSEngine.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnTTSPrivate : public CDaCmnTTSEngine 
{
public:
	CDaCmnTTSPrivate ();
	virtual ~CDaCmnTTSPrivate ();

// Operations
public:
	class CSapi5Voice* GetCachedVoice (class CSapiVoiceClient * pClient);
	class CSapi5Voice* PrepareAndCacheVoice (class CSapiVoiceClient * pClient);
private:
	virtual void Initialize (class CSapi5VoiceInfo* pVoiceInfo) {}
	virtual bool Initialize (class CSapi5Voice* pVoice) {return false;}
	virtual bool Initialize (class CSapiVoice* pVoice) {return false;}
	virtual bool Initialize (class CAgentFile* pFile) {return false;}

// Interfaces
public:
	// IDaXxxTTSEngine
	HRESULT get_TTSModeID (BSTR *TTSModeID);
	HRESULT get_DisplayName (BSTR *DisplayName);
	HRESULT get_Manufacturer (BSTR *Manufacturer);
	HRESULT GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT get_Gender (SpeechGenderType *Gender);
	HRESULT get_LanguageID (long *LanguageID);
	HRESULT get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName);

	// IDaXxxTTSPrivate
	HRESULT get_InitDisplayName (BSTR *DisplayName);
    HRESULT put_InitDisplayName (BSTR DisplayName);
	HRESULT get_InitManufacturer (BSTR *Manufacturer);
    HRESULT put_InitManufacturer (BSTR Manufacturer);
	HRESULT get_InitVersion (BSTR *Version);
    HRESULT put_InitVersion (BSTR Version);
	HRESULT get_InitGender (SpeechGenderType *Gender);
    HRESULT put_InitGender (SpeechGenderType Gender);
	HRESULT get_InitLanguageID (long *LanguageID);
    HRESULT put_InitLanguageID (long LanguageID);
    HRESULT get_InitString (BSTR ValuePath, BSTR *StringValue);
    HRESULT put_InitString (BSTR ValuePath, BSTR StringValue);
    HRESULT get_InitAttribute (BSTR AttributeName, BSTR *AttributeValue);
    HRESULT put_InitAttribute (BSTR AttributeName, BSTR AttributeValue);
	HRESULT get_InitFilePath (BSTR FileId, BSTR *FilePath);
    HRESULT put_InitFilePath (BSTR FileId, BSTR FilePath);

// Implementation
protected:
	tPtr<class CSapi5VoicePrivate>	mSapi5VoicePrivate;
	tPtr<class CSapi5VoiceInfo>		mPrivateVoiceInfo;
};

/////////////////////////////////////////////////////////////////////////////
