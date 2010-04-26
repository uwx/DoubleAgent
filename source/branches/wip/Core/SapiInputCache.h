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

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class CSapi5Input;
class CSapi5Inputs;

class CSapiInputClient
{
public:
	CSapiInputClient () {}
	virtual ~CSapiInputClient () {}
};

//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapiInputCache
{
	DECLARE_DLL_OBJECT(CSapiInputCache)
public:
	CSapiInputCache ();
	virtual ~CSapiInputCache ();

// Attributes
public:
	static CSapiInputCache * GetStaticInstance ();

// Operations
public:
	CSapi5Input * GetAgentInput (LANGID pLangID, bool pUseDefaults, bool pCached = true);
	CSapi5Input * GetAgentInput (LPCTSTR pEngineName, LANGID pLangID, bool pUseDefaults, bool pCached = true);

	bool CacheInput (CSapi5Input * pInput, CSapiInputClient * pClient);
	bool UncacheInput (CSapi5Input * pInput);
	bool AddInputClient (CSapi5Input * pInput, CSapiInputClient * pClient);
	bool RemoveInputClient (CSapi5Input * pInput, CSapiInputClient * pClient, bool pDeleteUnusedInput = true);

	CSapi5Input * GetCachedInput (INT_PTR pInputNdx);
	CSapi5Input * FindCachedInput (LPCTSTR pEngineId);
	bool GetInputClients (CSapi5Input * pInput, CAtlPtrTypeArray <CSapiInputClient> & pClients);

	CSapi5Inputs * GetSapi5Inputs ();

// Implementation
protected:
	CComAutoCriticalSection												mCritSec;
	tPtr <CSapi5Inputs>													mSapi5Inputs;
	CAtlOwnPtrArray <CSapi5Input>										mCachedInputs;
	CAtlOwnPtrMap <CSapi5Input *, CAtlPtrTypeArray <CSapiInputClient> >	mInputClients;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////
