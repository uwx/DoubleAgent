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
#pragma once
#include "DaCoreExp.h"
#include "AtlCollEx.h"

class CSapiVoice;
class CSapi5Voice;
class CSapi5Voices;
#ifndef	_WIN64
class CSapi4Voice;
class CSapi4Voices;
#endif

//////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)

class CSapiVoiceClient
{
public:
	CSapiVoiceClient () {}
	virtual ~CSapiVoiceClient () {}
};

//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapiVoiceCache
{
	DECLARE_DLL_OBJECT(CSapiVoiceCache)
public:
	CSapiVoiceCache ();
	virtual ~CSapiVoiceCache ();

// Attributes
public:
	static CSapiVoiceCache * GetStaticInstance ();

	UINT GetSapiVersionRestriction () const;
	UINT GetSapiVersionPreference () const;
	bool RestrictSapiVersion (UINT pSapiVersion);
	bool PreferSapiVersion (UINT pSapiVersion);

#ifndef	_WIN64
	CSapi4Voices * GetSapi4Voices ();
#endif
	CSapi5Voices * GetSapi5Voices ();

// Operations
public:
	CSapiVoice * GetAgentVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapiVoice * GetAgentVoice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);

	bool CacheVoice (CSapiVoice * pVoice, CSapiVoiceClient * pClient);
	bool UncacheVoice (CSapiVoice * pVoice);
	bool AddVoiceClient (CSapiVoice * pVoice, CSapiVoiceClient * pClient);
	bool RemoveVoiceClient (CSapiVoice * pVoice, CSapiVoiceClient * pClient, bool pDeleteUnusedVoice = false);

	CSapiVoice * GetCachedVoice (INT_PTR pVoiceNdx);
	CSapiVoice * FindCachedVoice (LPCTSTR pVoiceId);
	bool GetVoiceClients (CSapiVoice * pVoice, CAtlPtrTypeArray <CSapiVoiceClient> & pClients);

	void Terminate (DWORD pWaitForCompletion = 0);
	bool DeleteUnusedVoices ();
	void DeleteAllVoices ();
	static void TerminateStaticInstance (DWORD pWaitForCompletion = 5000);
	static void CleanupStaticInstance ();

// Implementation
protected:
	CSapi5Voice * GetAgentSapi5Voice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapi5Voice * GetAgentSapi5Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);
#ifndef	_WIN64
	CSapi4Voice * GetAgentSapi4Voice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapi4Voice * GetAgentSapi4Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);
#endif

protected:
	CComAutoCriticalSection												mCritSec;
#ifndef	_WIN64
	UINT																mSapiVersionRestriction;
	UINT																mSapiVersionPreference;
	tPtr <CSapi4Voices>													mSapi4Voices;
#endif
	tPtr <CSapi5Voices>													mSapi5Voices;
	CAtlOwnPtrArray <CSapiVoice>										mCachedVoices;
	CAtlOwnPtrMap <CSapiVoice *, CAtlPtrTypeArray <CSapiVoiceClient> >	mVoiceClients;
	bool																mVoiceDeletePending;
};

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////
