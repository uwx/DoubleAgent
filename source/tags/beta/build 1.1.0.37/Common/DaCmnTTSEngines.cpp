/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaCmnTTSEngines.h"
#include "AgentFile.h"
#include "SapiVoiceCache.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif

/////////////////////////////////////////////////////////////////////////////

CDaCmnTTSEngines::CDaCmnTTSEngines()
{
}

CDaCmnTTSEngines::~CDaCmnTTSEngines()
{
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnTTSEngines::UseAllVoices ()
{
	HRESULT				lResult = S_FALSE;
	CSapiVoiceCache *	lVoiceCache;

	mSapi5Voices.RemoveAll ();
#ifndef	_WIN64
	mSapi4Voices.RemoveAll ();
#endif

	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		CSapi5Voices *	lSapi5Voices;

		if	(lSapi5Voices = lVoiceCache->GetSapi5Voices())
		{
			mSapi5Voices.Copy (*lSapi5Voices);
			lResult = S_OK;
		}
#ifndef	_WIN64
		CSapi4Voices *	lSapi4Voices;

		if	(lSapi4Voices = lVoiceCache->GetSapi4Voices())
		{
			mSapi4Voices.Copy (*lSapi4Voices);
			lResult = S_OK;
		}
#endif
	}
	return lResult;
}

HRESULT CDaCmnTTSEngines::UseTheseVoices (CAgentFile * pFile, LANGID pLangId, short pGender)
{
	HRESULT				lResult = S_FALSE;
	CSapiVoiceCache *	lVoiceCache;
	CSapi5Voices *		lSapi5Voices;
	INT_PTR				lSapi5VoiceNdx = -1;
#ifndef	_WIN64
	CSapi4Voices *		lSapi4Voices;
	INT_PTR				lSapi4VoiceNdx = -1;
#endif

	mSapi5Voices.RemoveAll ();
#ifndef	_WIN64
	mSapi4Voices.RemoveAll ();
#endif

	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		tS <CAgentFileTts>	lFileTts;

		if	(pFile)
		{
			lFileTts = pFile->GetTts ();
			lFileTts.mMode = GUID_NULL;
			lFileTts.mModeId = NULL;
		}
		else
		{
			lFileTts.mGender = pGender;
		}

		lFileTts.mLanguage = pLangId;

		if	(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
		{
			while ((lSapi5VoiceNdx = lSapi5Voices->FindVoice (lFileTts, (pLangId==0), lSapi5VoiceNdx)) >= 0)
			{
				mSapi5Voices.Add (lSapi5Voices->GetAt (lSapi5VoiceNdx));
			}
			lResult = S_OK;
		}
#ifndef	_WIN64
		if	(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
		{
			while ((lSapi4VoiceNdx = lSapi4Voices->FindVoice (lFileTts, (pLangId==0), lSapi4VoiceNdx)) >= 0)
			{
				mSapi4Voices.Add (lSapi4Voices->GetAt (lSapi4VoiceNdx));
			}
			lResult = S_OK;
		}
#endif
	}
	return lResult;
}
