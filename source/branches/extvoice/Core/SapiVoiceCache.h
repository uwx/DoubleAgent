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
#ifndef _SAPIVOICECACHE_H_INCLUDED
#define _SAPIVOICECACHE_H_INCLUDED
#pragma once

#include "DaCoreExp.h"
#include "AfxTemplEx.h"

class CSapiVoice;
class CSapi5Voice;
class CSapi5Voices;
#ifndef	_WIN64
class CSapi4Voice;
class CSapi4Voices;
#endif

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CSapiVoiceCache
{
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

// Operations
public:
	CSapiVoice * GetAgentVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapiVoice * GetAgentVoice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);

	bool CacheVoice (CSapiVoice * pVoice, CObject * pClient);
	bool UncacheVoice (CSapiVoice * pVoice);
	bool AddVoiceClient (CSapiVoice * pVoice, CObject * pClient);
	bool RemoveVoiceClient (CSapiVoice * pVoice, CObject * pClient, bool pDeleteUnusedVoice = true);

	CSapiVoice * GetCachedVoice (int pVoiceNdx);
	CSapiVoice * FindCachedVoice (LPCTSTR pVoiceId);
	bool GetVoiceClients (CSapiVoice * pVoice, CObTypeArray <CObject> & pClients);

#ifndef	_WIN64
	CSapi4Voices * GetSapi4Voices ();
#endif
	CSapi5Voices * GetSapi5Voices ();

// Implementation
protected:
	CSapi5Voice * GetAgentSapi5Voice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapi5Voice * GetAgentSapi5Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);
#ifndef	_WIN64
	CSapi4Voice * GetAgentSapi4Voice (const struct CAgentFileTts & pAgentFileTts, bool pCached = true);
	CSapi4Voice * GetAgentSapi4Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pCached = true);
#endif

protected:
	CCriticalSection									mCritSec;
#ifndef	_WIN64
	UINT												mSapiVersionRestriction;
	UINT												mSapiVersionPreference;
	tPtr <CSapi4Voices>									mSapi4Voices;
#endif
	tPtr <CSapi5Voices>									mSapi5Voices;
	COwnPtrArray <CSapiVoice>							mCachedVoices;
	COwnPtrMap <CSapiVoice *, CObTypeArray <CObject> >	mVoiceClients;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // _SAPIVOICECACHE_H_INCLUDED
