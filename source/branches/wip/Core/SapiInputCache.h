/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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

class CSapiInputCache
{
	DECLARE_DLL_OBJECT_EX(CSapiInputCache, _DACORE_IMPEXP)
public:
	_DACORE_IMPEXP CSapiInputCache ();
	_DACORE_IMPEXP virtual ~CSapiInputCache ();

// Attributes
public:
	_DACORE_IMPEXP static CSapiInputCache * GetStaticInstance ();

// Operations
public:
	_DACORE_IMPEXP CSapi5Input * GetAgentInput (LANGID pLangID, bool pUseDefaults, bool pCached = true);
	_DACORE_IMPEXP CSapi5Input * GetAgentInput (LPCTSTR pEngineName, LANGID pLangID, bool pUseDefaults, bool pCached = true);

	_DACORE_IMPEXP bool CacheInput (CSapi5Input * pInput, CSapiInputClient * pClient);
	_DACORE_IMPEXP bool UncacheInput (CSapi5Input * pInput);
	_DACORE_IMPEXP bool AddInputClient (CSapi5Input * pInput, CSapiInputClient * pClient);
	_DACORE_IMPEXP bool RemoveInputClient (CSapi5Input * pInput, CSapiInputClient * pClient, bool pDeleteUnusedInput = false);

	_DACORE_IMPEXP CSapi5Input * GetCachedInput (INT_PTR pInputNdx);
	_DACORE_IMPEXP CSapi5Input * FindCachedInput (LPCTSTR pEngineId);
	_DACORE_IMPEXP bool GetInputClients (CSapi5Input * pInput, CAtlPtrTypeArray <CSapiInputClient> & pClients);

	_DACORE_IMPEXP CSapi5Inputs * GetSapi5Inputs ();

// Implementation
protected:
	CComAutoCriticalSection												mCritSec;
	tPtr <CSapi5Inputs>													mSapi5Inputs;
	CAtlOwnPtrArray <CSapi5Input>										mCachedInputs;
	CAtlOwnPtrMap <CSapi5Input *, CAtlPtrTypeArray <CSapiInputClient> >	mInputClients;
};

//////////////////////////////////////////////////////////////////////
