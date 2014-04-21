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
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnTTSEngine
{
public:
	CDaCmnTTSEngine ();
	virtual ~CDaCmnTTSEngine ();

// Operations
public:
	virtual void Initialize (class CSapi5VoiceInfo * pVoiceInfo);
	virtual bool Initialize (class CSapi5Voice* pVoice);
#ifndef	_WIN64
	void Initialize (class CSapi4VoiceInfo* pVoiceInfo);
	bool Initialize (class CSapi4Voice* pVoice);
#endif
	virtual bool Initialize (class CSapiVoice* pVoice);
	virtual bool Initialize (class CAgentFile* pFile);

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

// Implementation
protected:
	class CSapi5VoiceInfo * mSapi5Voice;
#ifndef	_WIN64
	class CSapi4VoiceInfo*	mSapi4Voice;
#endif
};

/////////////////////////////////////////////////////////////////////////////
