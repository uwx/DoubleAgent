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
#include "DaCore.h"
#include "SapiVoiceCache.h"
#ifndef	_WIN64
#include "Sapi4Voice.h"
#include "Sapi4Voices.h"
#include "Sapi4Err.h"
#include "GuidStr.h"
#endif
#include "Sapi5Voice.h"
#include "Sapi5Voices.h"
#include "Sapi5Err.h"
#include "Registry.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_CACHE	(GetProfileDebugInt(_T("LogVoiceCache"),LogVerbose,true)&0xFFFF)
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapiVoiceCache)

CSapiVoiceCache::CSapiVoiceCache ()
{
#ifndef	_WIN64
	mSapiVersionRestriction = 0;
	mSapiVersionPreference = 4;
#endif
}

CSapiVoiceCache::~CSapiVoiceCache ()
{
#ifdef	_DEBUG_CACHE
	try
	{
		LogMessage (_DEBUG_CACHE, _T("CSapiVoiceCache::~CSapiVoiceCache Voices [%d] Clients [%d]"), mCachedVoices.GetSize(), mVoiceClients.GetCount());
	}
	catch AnyExceptionSilent
#endif
	try
	{
		mVoiceClients.RemoveAll ();
	}
	catch AnyExceptionSilent
	try
	{
		mCachedVoices.DeleteAll ();
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSapiVoiceCache * CSapiVoiceCache::GetStaticInstance ()
{
	return &_AtlModule;
}

CSapi5Voices * CSapiVoiceCache::GetSapi5Voices ()
{
	if	(!mSapi5Voices)
	{
		mSapi5Voices = CSapi5Voices::CreateInstance();
	}
	return mSapi5Voices;
}

#ifndef	_WIN64
CSapi4Voices * CSapiVoiceCache::GetSapi4Voices ()
{
	if	(!mSapi4Voices)
	{
		mSapi4Voices = CSapi4Voices::CreateInstance();
	}
	return mSapi4Voices;
}
#endif

//////////////////////////////////////////////////////////////////////

UINT CSapiVoiceCache::GetSapiVersionRestriction () const
{
#ifdef	_WIN64
	return 5;
#else
	return mSapiVersionRestriction;
#endif
}

UINT CSapiVoiceCache::GetSapiVersionPreference () const
{
#ifdef	_WIN64
	return 5;
#else
	return mSapiVersionPreference;
#endif
}

//////////////////////////////////////////////////////////////////////

bool CSapiVoiceCache::RestrictSapiVersion (UINT pSapiVersion)
{
#ifndef	_WIN64
	if	(
			(pSapiVersion == 0)
		||	(pSapiVersion == 4)
		||	(pSapiVersion == 5)
		)
	{
		mSapiVersionRestriction = pSapiVersion;
		return true;
	}
#endif
	return false;
}

bool CSapiVoiceCache::PreferSapiVersion (UINT pSapiVersion)
{
#ifndef	_WIN64
	if	(
			(pSapiVersion == 4)
		||	(pSapiVersion == 5)
		)
	{
		mSapiVersionPreference = pSapiVersion;
		return true;
	}
#endif
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSapiVoice * CSapiVoiceCache::GetAgentVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached)
{
	CSapiVoice *	lRet = NULL;

#ifdef	_WIN64
	lRet = GetAgentSapi5Voice (pAgentFileTts, pUseDefaults, pCached);
#else
#ifdef	_DEBUG
	mSapiVersionRestriction = (UINT)GetProfileDebugInt(_T("DebugSapiVersion"),mSapiVersionRestriction,true);
	mSapiVersionPreference = (UINT)GetProfileDebugInt(_T("DebugSapiPreference"),mSapiVersionPreference,true);
#endif
#ifdef	_DEBUG_CACHE
	LogMessage (_DEBUG_CACHE, _T("SapiVersionRestriction [%u] SapiVersionPreference [%u]"), mSapiVersionRestriction, mSapiVersionPreference);
#endif
	if	(mSapiVersionRestriction == 5)
	{
		lRet = GetAgentSapi5Voice (pAgentFileTts, pUseDefaults, pCached);
	}
	else
	if	(mSapiVersionRestriction == 4)
	{
		lRet = GetAgentSapi4Voice (pAgentFileTts, pCached);
	}
	else
	if	(mSapiVersionPreference == 5)
	{
		lRet = GetAgentSapi5Voice (pAgentFileTts, pUseDefaults, pCached);
		if	(!lRet)
		{
			lRet = GetAgentSapi4Voice (pAgentFileTts, pCached);
		}
	}
	else
	{
		lRet = GetAgentSapi4Voice (pAgentFileTts, pCached);
		if	(!lRet)
		{
			lRet = GetAgentSapi5Voice (pAgentFileTts, pUseDefaults, pCached);
		}
	}
#endif
	return lRet;
}

CSapiVoice * CSapiVoiceCache::GetAgentVoice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached)
{
	CSapiVoice *	lRet = NULL;

#ifdef	_WIN64
	lRet = GetAgentSapi5Voice (pVoiceName, pLangID, pUseDefaults, pCached);
#else
#ifdef	_DEBUG
	mSapiVersionRestriction = (UINT)GetProfileDebugInt(_T("DebugSapiVersion"),mSapiVersionRestriction,true);
	mSapiVersionPreference = (UINT)GetProfileDebugInt(_T("DebugSapiPreference"),mSapiVersionPreference,true);
#endif
#ifdef	_DEBUG_CACHE
	LogMessage (_DEBUG_CACHE, _T("SapiVersionRestriction [%u] SapiVersionPreference [%u]"), mSapiVersionRestriction, mSapiVersionPreference);
#endif
	if	(mSapiVersionRestriction == 5)
	{
		lRet = GetAgentSapi5Voice (pVoiceName, pLangID, pUseDefaults, pCached);
	}
	else
	if	(mSapiVersionRestriction == 4)
	{
		lRet = GetAgentSapi4Voice (pVoiceName, pLangID, pCached);
	}
	else
	if	(mSapiVersionPreference == 5)
	{
		lRet = GetAgentSapi5Voice (pVoiceName, pLangID, pUseDefaults, pCached);
		if	(!lRet)
		{
			lRet = GetAgentSapi4Voice (pVoiceName, pLangID, pCached);
		}
	}
	else
	{
		lRet = GetAgentSapi4Voice (pVoiceName, pLangID, pCached);
		if	(!lRet)
		{
			lRet = GetAgentSapi5Voice (pVoiceName, pLangID, pUseDefaults, pCached);
		}
	}
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////

CSapi5Voice * CSapiVoiceCache::GetAgentSapi5Voice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached)
{
	CSapi5Voice *		lRet = NULL;
	CSapi5VoiceInfo *	lSapi5VoiceInfo;
	tPtr <CSapi5Voice>	lSapi5Voice;

	if	(GetSapi5Voices())
	{
		while	(lSapi5VoiceInfo = mSapi5Voices->GetVoice (pAgentFileTts, pUseDefaults))
		{
			if	(pCached)
			{
				lRet = dynamic_cast <CSapi5Voice *> (FindCachedVoice (lSapi5VoiceInfo->mVoiceIdShort));
			}
			if	(
					(!lRet)
				&&	(lSapi5Voice = CSapi5Voice::CreateInstance())
				)
			{
				lSapi5Voice->SetVoiceId (lSapi5VoiceInfo->mVoiceIdLong);
				if	(lSapi5Voice->SafeIsValid())
				{
					if	(
							(FAILED (LogSapi5ErrAnon (LogNormal, lSapi5Voice->Speak (_T("")), _T("Test [%ls]"), (BSTR)lSapi5VoiceInfo->mVoiceIdShort)))
						&&	(mSapi5Voices->RemoveVoice (lSapi5VoiceInfo))
						)
					{
						continue;
					}
					lRet = lSapi5Voice.Detach();
				}
			}
			break;
		}
#ifndef	_STRICT_COMPATIBILITY
		if	(
				(!lRet)
			&&	(lSapi5VoiceInfo = mSapi5Voices->operator () (mSapi5Voices->DefaultVoiceNdx()))
			)
		{
			if	(pCached)
			{
				lRet = FindCachedVoice (lSapi5VoiceInfo->mVoiceIdShort);
			}
			if	(
					(!lRet)
				&&	(lSapi5Voice = (CSapi5Voice *)CSapi5Voice::CreateObject())
				)
			{
				lSapi5Voice->SetVoiceId (lSapi5VoiceInfo->mVoiceIdLong);
				if	(lSapi5Voice->SafeIsValid())
				{
					lRet = lSapi5Voice.Detach ();
				}
			}
		}
#endif
	}
	return lRet;
}

CSapi5Voice * CSapiVoiceCache::GetAgentSapi5Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached)
{
	CSapi5Voice *		lRet = NULL;
	CSapi5VoiceInfo *	lSapi5VoiceInfo;
	tPtr <CSapi5Voice>	lSapi5Voice;

	if	(GetSapi5Voices())
	{
		if	(
				(
					(lSapi5VoiceInfo = mSapi5Voices->GetVoiceId (pVoiceName))
				||	(lSapi5VoiceInfo = mSapi5Voices->GetVoiceName (pVoiceName))
				)
			&&	(mSapi5Voices->VoiceSupportsLanguage (lSapi5VoiceInfo, pLangID, pUseDefaults))
			)
		{
			if	(pCached)
			{
				lRet = dynamic_cast <CSapi5Voice *> (FindCachedVoice (lSapi5VoiceInfo->mVoiceIdShort));
			}
			if	(
					(!lRet)
				&&	(lSapi5Voice = CSapi5Voice::CreateInstance())
				)
			{
				lSapi5Voice->SetVoiceId (lSapi5VoiceInfo->mVoiceIdLong);
				if	(lSapi5Voice->SafeIsValid())
				{
					lRet = lSapi5Voice.Detach();
				}
			}
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#ifndef	_WIN64
//////////////////////////////////////////////////////////////////////

CSapi4Voice * CSapiVoiceCache::GetAgentSapi4Voice (const struct CAgentFileTts & pAgentFileTts, bool pCached)
{
	CSapi4Voice *		lRet = NULL;
	CSapi4VoiceInfo *	lSapi4VoiceInfo;
	tPtr <CSapi4Voice>	lSapi4Voice;

	if	(GetSapi4Voices())
	{
		while	(lSapi4VoiceInfo = mSapi4Voices->GetVoice (pAgentFileTts, true))
		{
			if	(pCached)
			{
				lRet = dynamic_cast <CSapi4Voice *> (FindCachedVoice ((CString)CGuidStr(lSapi4VoiceInfo->mModeId)));
			}
			if	(
					(!lRet)
				&&	(lSapi4Voice = CSapi4Voice::CreateInstance())
				)
			{
				if	(
						(SUCCEEDED (lSapi4Voice->SetModeId (lSapi4VoiceInfo->mModeId)))
					&&	(lSapi4Voice->SafeIsValid())
					)
				{
//					if	(
//							(FAILED (LogSapi4ErrAnon (LogNormal, lSapi4Voice->Speak (_T("")), _T("Test [%ls]"), (BSTR)lSapi4VoiceInfo->mVoiceName)))
//						&&	(mSapi4Voices->RemoveVoice (lSapi4VoiceInfo))
//						)
//					{
//						continue;
//					}
					lRet = lSapi4Voice.Detach();
				}
			}
			break;
		}
	}
	return lRet;
}

CSapi4Voice * CSapiVoiceCache::GetAgentSapi4Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pCached)
{
	CSapi4Voice *		lRet = NULL;
	CSapi4VoiceInfo *	lSapi4VoiceInfo;
	tPtr <CSapi4Voice>	lSapi4Voice;
	GUID				lModeId = CGuidStr::Parse (pVoiceName);

	if	(GetSapi4Voices())
	{
		if	(
				(
					(IsEqualGUID (lModeId, GUID_NULL))
				?	(lSapi4VoiceInfo = mSapi4Voices->GetVoiceName (pVoiceName))
				:	(lSapi4VoiceInfo = mSapi4Voices->GetModeId (lModeId))
				)
			&&	(mSapi4Voices->VoiceSupportsLanguage (lSapi4VoiceInfo, pLangID, true))
			)
		{
			if	(pCached)
			{
				lRet = dynamic_cast <CSapi4Voice *> (FindCachedVoice ((CString)CGuidStr(lSapi4VoiceInfo->mModeId)));
			}
			if	(
					(!lRet)
				&&	(lSapi4Voice = CSapi4Voice::CreateInstance())
				)
			{
				if	(
						(SUCCEEDED (lSapi4Voice->SetModeId (lSapi4VoiceInfo->mModeId)))
					&&	(lSapi4Voice->SafeIsValid())
					)
				{
					lRet = lSapi4Voice.Detach();
				}
			}
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#endif	// _WIN64
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CSapiVoiceCache::CacheVoice (CSapiVoice * pVoice, CSapiVoiceClient * pClient)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(
				(pVoice)
			&&	(pClient)
			)
		{
			INT_PTR										lVoiceNdx;
			tPtr <CAtlPtrTypeArray <CSapiVoiceClient> > &	lClients = mVoiceClients [pVoice];

			lVoiceNdx = mCachedVoices.Find (pVoice);
			if	(lVoiceNdx < 0)
			{
				mCachedVoices.Add (pVoice);
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Cache Voice [%p] [%s] for Client [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient));
#endif
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Duplicate Voice [%p] [%s] for Client [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient));
			}
#endif

			if	(!lClients)
			{
				lClients = new CAtlPtrTypeArray <CSapiVoiceClient>;
			}
			if	(
					(lClients)
				&&	(lClients->Find (pClient) < 0)
				)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Voice [%p] [%s] Client [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient));
#endif
				lClients->Add (pClient);
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Voice [%p] [%s] Duplicate Client [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient));
			}
#endif
			lRet = true;
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("CacheVoice failed for Voice [%p] Client [%p] [%s]"), pVoice, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CSapiVoiceCache::UncacheVoice (CSapiVoice * pVoice)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR	lVoiceNdx;

		if	(pVoice)
		{
			lVoiceNdx = mCachedVoices.Find (pVoice);

			if	(lVoiceNdx >= 0)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Uncache Voice [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId());
#endif
				mCachedVoices.RemoveAt (lVoiceNdx);
				mVoiceClients.RemoveKey (pVoice);
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CSapiVoice * CSapiVoiceCache::GetCachedVoice (int pVoiceNdx)
{
	CSapiVoice *	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		lRet = mCachedVoices (pVoiceNdx);
	}
	catch AnyExceptionDebug

	return lRet;
}

CSapiVoice * CSapiVoiceCache::FindCachedVoice (LPCTSTR pVoiceId)
{
	CSapiVoice *	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		CSapiVoice *	lSapiVoice;
		CAtlString		lVoiceName (pVoiceId);
		int				lNdx;

		for	(lNdx = 0; lSapiVoice = mCachedVoices (lNdx); lNdx++)
		{
			if	(lVoiceName.CompareNoCase ((BSTR)lSapiVoice->GetUniqueId ()) == 0)
			{
				lRet = lSapiVoice;
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CSapiVoiceCache::AddVoiceClient (CSapiVoice * pVoice, CSapiVoiceClient * pClient)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(
				(pVoice)
			&&	(pClient)
			&&	(mCachedVoices.Find (pVoice) >= 0)
			)
		{
			tPtr <CAtlPtrTypeArray <CSapiVoiceClient> > &	lClients = mVoiceClients [pVoice];

			if	(
					(lClients)
				&&	(lClients->Find (pClient) < 0)
				)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Voice [%p] [%s] Client [%p] [%s] Clients [%d]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient), lClients->GetSize());
#endif
				lClients->Add (pClient);
				lRet = true;
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Voice [%p] [%s] Duplicate Client [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient));
			}
#endif
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("AddClient failed for Voice [%p] Client [%p] [%s]"), pVoice, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CSapiVoiceCache::RemoveVoiceClient (CSapiVoice * pVoice, CSapiVoiceClient * pClient, bool pDeleteUnusedVoice)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR	lVoiceNdx;

		if	(
				(pVoice)
			&&	(pClient)
			&&	((lVoiceNdx = mCachedVoices.Find (pVoice)) >= 0)
			)
		{
			tPtr <CAtlPtrTypeArray <CSapiVoiceClient> > &	lClients = mVoiceClients [pVoice];

			if	(
					(lClients)
				&&	(lClients->Find (pClient) >= 0)
				)
			{
				lClients->Remove (pClient);
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Voice [%p] [%s] Remove client [%p] [%s] Clients [%d]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient), lClients->GetSize());
#endif
				lRet = true;

				if	(lClients->GetSize() <= 0)
				{
					mVoiceClients.RemoveKey (pVoice);
					if	(pDeleteUnusedVoice)
					{
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("Delete cached Voice [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId());
#endif
						mCachedVoices.DeleteAt (lVoiceNdx);
					}
					else
					{
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("Unused cached Voice [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId());
#endif
						mCachedVoices.RemoveAt (lVoiceNdx);
					}
				}
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Voice [%p] [%s] No Client [%p] [%s]"), pVoice, (BSTR)pVoice->GetUniqueId(), pClient, AtlTypeName(pClient));
			}
#endif
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("RemoveClient failed for [%p] Client [%p] [%s]"), pVoice, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CSapiVoiceCache::GetVoiceClients (CSapiVoice * pVoice, CAtlPtrTypeArray <CSapiVoiceClient> & pClients)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		CAtlPtrTypeArray <CSapiVoiceClient> *	lClients = NULL;

		if	(
				(pVoice)
			&&	(mVoiceClients.Lookup (pVoice, lClients))
			&&	(lClients)
			)
		{
			pClients.Copy (*lClients);
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
