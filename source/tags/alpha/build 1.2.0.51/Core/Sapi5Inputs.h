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
#include "SapiLanguageMatch.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi5InputInfo
{
	DECLARE_DLL_OBJECT(CSapi5InputInfo)
public:
	CSapi5InputInfo ();
	virtual ~CSapi5InputInfo ();

// Attributes
	tBstrPtr			mEngineName;
	tBstrPtr			mEngineIdShort;
	tBstrPtr			mEngineIdLong;
	LANGID				mLangId;
	UINT				mLangIdCount;
	tArrayPtr <LANGID>	mLangIdSupported;
	tBstrPtr			mClassId;
	tBstrPtr			mRecoExtension;
	tBstrPtr			mManufacturer;
};

/////////////////////////////////////////////////////////////////////////////

class CSapi5InputIndexArray : public CAtlTypeArray <INT_PTR>
{
	DECLARE_DLL_OBJECT(CSapi5InputIndexArray)
public:
	CSapi5InputIndexArray() {}
	virtual ~CSapi5InputIndexArray() {}
};

class CSapi5InputInfoArray : public CAtlPtrTypeArray <CSapi5InputInfo>
{
	DECLARE_DLL_OBJECT(CSapi5InputInfoArray)
public:
	CSapi5InputInfoArray() {}
	virtual ~CSapi5InputInfoArray() {}
};

class CSapi5InputMatchRanks : public CAtlTypeArray <int>
{
	DECLARE_DLL_OBJECT(CSapi5InputMatchRanks)
public:
	CSapi5InputMatchRanks() {}
	virtual ~CSapi5InputMatchRanks() {}

	static int __cdecl SortDescending (const void* pElem1, const void* pElem2)
	{
		return -ATL::CElementTraits<int>::CompareElementsOrdered (*(int *)pElem1, *(int*)pElem2);
	}
};

/////////////////////////////////////////////////////////////////////////////

class CSapi5Inputs : public CAtlOwnPtrArray <CSapi5InputInfo>, protected CSapiLanguageMatch
{
	DECLARE_DLL_OBJECT_EX(CSapi5Inputs, _DACORE_IMPEXP)
protected:
	CSapi5Inputs();
public:
	_DACORE_IMPEXP virtual ~CSapi5Inputs();
	_DACORE_IMPEXP static CSapi5Inputs * CreateInstance ();

// Attributes
	const UINT	mLogLevelDebug;

// Operations
	_DACORE_IMPEXP void Enumerate ();

	_DACORE_IMPEXP INT_PTR FindEngineId (LPCTSTR pEngineId);
	_DACORE_IMPEXP CSapi5InputInfo* GetEngineId (LPCTSTR pEngineId);

	_DACORE_IMPEXP INT_PTR FindEngineName (LPCTSTR pEngineName);
	_DACORE_IMPEXP CSapi5InputInfo* GetEngineName (LPCTSTR pEngineName);

	_DACORE_IMPEXP INT_PTR FindInput (LANGID pLangId, bool pUseDefaults, int * pMatchRank = NULL);
	_DACORE_IMPEXP CSapi5InputInfo* GetInput (LANGID pLangId, bool pUseDefaults, int * pMatchRank = NULL);

	_DACORE_IMPEXP CSapi5InputIndexArray const * FindInputs (LANGID pLangId, bool pUseDefaults, CSapi5InputMatchRanks const ** pMatchRanks = NULL);
	_DACORE_IMPEXP CSapi5InputInfoArray const * GetInputs (LANGID pLangId, bool pUseDefaults, CSapi5InputMatchRanks const ** pMatchRanks = NULL);

	_DACORE_IMPEXP bool RemoveInput (INT_PTR pInputNdx);
	_DACORE_IMPEXP bool RemoveInput (const CSapi5InputInfo* pInputInfo);

	_DACORE_IMPEXP INT_PTR DefaultInputNdx ();
	_DACORE_IMPEXP bool InputSupportsLanguage (CSapi5InputInfo* pInputInfo, LANGID pLangId, bool pUseDefaults);

	_DACORE_IMPEXP void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	_DACORE_IMPEXP static void LogInputInfo (UINT pLogLevel, CSapi5InputInfo& pInputInfo, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);

// Implementation
protected:
	_DACORE_IMPEXP static void LogInputToken (UINT pLogLevel, void* pInputToken, LPCTSTR pTitle = NULL);
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
