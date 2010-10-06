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
static int __cdecl SortRankDescending (const void * pElem1, const void * pElem2)
{
	return -ATL::CElementTraits<int>::CompareElementsOrdered (*(int *)pElem1, *(int*)pElem2);
}
#endif
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_LANGUAGE			(GetProfileDebugInt(_T("DebugLanguage"),LogVerbose,true)&0xFFFF)
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
	mVoiceNdxMap.RemoveAll ();
#endif

	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		if	(lVoiceCache->GetSapiVersionRestriction () != 4)
		{
			CSapi5Voices *	lSapi5Voices;

			if	(lSapi5Voices = lVoiceCache->GetSapi5Voices())
			{
				mSapi5Voices.Copy (*lSapi5Voices);
				lResult = S_OK;
			}
		}
#ifndef	_WIN64
		if	(lVoiceCache->GetSapiVersionRestriction () != 5)
		{
			CSapi4Voices *	lSapi4Voices;

			if	(lSapi4Voices = lVoiceCache->GetSapi4Voices())
			{
				mSapi4Voices.Copy (*lSapi4Voices);
				lResult = S_OK;
			}
		}
#endif
	}
	return lResult;
}

HRESULT CDaCmnTTSEngines::UseTheseVoices (CAgentFile * pFile, LANGID pLangId, short pGender)
{
	HRESULT								lResult = S_FALSE;
	CSapiVoiceCache *					lVoiceCache;
	CSapi5Voices *						lSapi5Voices;
	tPtr <CSapi5VoiceInfoArray const>	lSapi5InfoArray;
#ifndef	_WIN64
	CSapi4Voices *						lSapi4Voices;
	tPtr <CSapi4VoiceInfoArray const>	lSapi4InfoArray;
	tPtr <CSapi5VoiceMatchRanks const>	lSapi5Ranks;
	tPtr <CSapi4VoiceMatchRanks const>	lSapi4Ranks;
	CAtlTypeArray <int>					lRankMap;
	INT_PTR								lRankNdx;
	int									lRankOffset;
#endif

	mSapi5Voices.RemoveAll ();
#ifndef	_WIN64
	mSapi4Voices.RemoveAll ();
	mVoiceNdxMap.RemoveAll ();
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

		if	(lVoiceCache->GetSapiVersionRestriction () != 4)
		{
			if	(
					(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
#ifdef	_WIN64
				&&	(lSapi5InfoArray = lSapi5Voices->GetVoices (lFileTts, true))
#else
				&&	(lSapi5InfoArray = lSapi5Voices->GetVoices (lFileTts, true, lSapi5Ranks.Free()))
#endif				
				)
			{
				mSapi5Voices.Copy (*lSapi5InfoArray);
				lResult = S_OK;
			}
		}
#ifndef	_WIN64
		if	(lVoiceCache->GetSapiVersionRestriction () != 5)
		{
			if	(
					(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
				&&	(lSapi4InfoArray = lSapi4Voices->GetVoices (lFileTts, true, lSapi4Ranks.Free()))
				)
			{
				mSapi4Voices.Copy (*lSapi4InfoArray);
				lResult = S_OK;
			}
		}
		
		if	(lSapi5Ranks)
		{
			for	(lRankNdx = 0; lRankNdx < (INT_PTR)lSapi5Ranks->GetCount(); lRankNdx++)
			{
				lRankOffset = 0;
				if	(
						(lSapi4Ranks)
					&&	(lVoiceCache->GetSapiVersionPreference () == 5)
					)
				{
					while (lSapi4Ranks->Find ((*lSapi5Ranks) [lRankNdx] + lRankOffset) >= 0)
					{
						lRankOffset++;
					}
				}
				mVoiceNdxMap.InsertAt (lRankMap.AddSortedQS ((*lSapi5Ranks) [lRankNdx] + lRankOffset, CSapi4VoiceMatchRanks::SortDescending, false), lRankNdx);
			}
		}
		if	(lSapi4Ranks)
		{
			for	(lRankNdx = 0; lRankNdx < (INT_PTR)lSapi4Ranks->GetCount(); lRankNdx++)
			{
				lRankOffset = 0;
				if	(
						(lSapi5Ranks)
					&&	(lVoiceCache->GetSapiVersionPreference () == 4)
					)
				{
					while (lSapi5Ranks->Find ((*lSapi4Ranks) [lRankNdx] + lRankOffset) >= 0)
					{
						lRankOffset++;
					}
				}
				mVoiceNdxMap.InsertAt (lRankMap.AddSortedQS ((*lSapi4Ranks) [lRankNdx] + lRankOffset, SortRankDescending, false), lRankNdx + (INT_PTR)mSapi5Voices.GetCount());
			}
		}
		
#ifdef	_DEBUG_LANGUAGE
		if	(LogIsActive (_DEBUG_LANGUAGE))
		{
			if	(lSapi5Ranks)
			{
				LogMessage (_DEBUG_LANGUAGE, _T("Sapi5Ranks  [%2u] [%s]"), lSapi5Ranks->GetCount(), FormatArray (*lSapi5Ranks, _T("%6.6d")));
			}
			if	(lSapi4Ranks)
			{
				LogMessage (_DEBUG_LANGUAGE, _T("Sapi4Ranks  [%2u] [%s]"), lSapi4Ranks->GetCount(), FormatArray (*lSapi4Ranks, _T("%6.6d")));
			}
			LogMessage (_DEBUG_LANGUAGE, _T("MappedRanks [%2u] [%s]"), lRankMap.GetCount(), FormatArray (lRankMap, _T("%6.6d")));
			LogMessage (_DEBUG_LANGUAGE, _T("IndexMap    [%2u] [%s]"), mVoiceNdxMap.GetCount(), FormatArray (mVoiceNdxMap, _T("%6d")));
		}
#endif
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	_WIN64
/////////////////////////////////////////////////////////////////////////////

CSapi5VoiceInfo * CDaCmnTTSEngines::GetSapi5VoiceAt (INT_PTR pIndex) const
{
	return mSapi5Voices (pIndex);
}

/////////////////////////////////////////////////////////////////////////////
#else	// _WIN64
/////////////////////////////////////////////////////////////////////////////

CSapi5VoiceInfo * CDaCmnTTSEngines::GetSapi5VoiceAt (INT_PTR pIndex) const
{
	CSapi5VoiceInfo *	lRet = NULL;

	if	(mVoiceNdxMap.GetCount() > 0)
	{	
		if	(
				(pIndex >= 0)
			&&	(pIndex < (INT_PTR)mVoiceNdxMap.GetCount ())
			)
		{
			lRet = mSapi5Voices (mVoiceNdxMap [pIndex]);
		}
	}
	else
	{
		lRet = mSapi5Voices (pIndex);
	}
	return lRet;
}

CSapi4VoiceInfo * CDaCmnTTSEngines::GetSapi4VoiceAt (INT_PTR pIndex) const
{
	CSapi4VoiceInfo *	lRet = NULL;

	if	(mVoiceNdxMap.GetCount() > 0)
	{	
		if	(
				(pIndex >= 0)
			&&	(pIndex < (INT_PTR)mVoiceNdxMap.GetCount ())
			)
		{
			lRet = mSapi4Voices (mVoiceNdxMap [pIndex] - (INT_PTR)mSapi5Voices.GetCount());
		}
	}
	else
	{
		lRet = mSapi4Voices (pIndex - (INT_PTR)mSapi5Voices.GetCount());
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _WIN64
/////////////////////////////////////////////////////////////////////////////
