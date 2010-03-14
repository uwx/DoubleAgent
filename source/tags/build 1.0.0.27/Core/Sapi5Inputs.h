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
#ifndef _SAPI5INPUTS_H_INCLUDED
#define _SAPI5INPUTS_H_INCLUDED
#pragma once

#include "DaCoreExp.h"

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CSapi5InputInfo
{
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

//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi5Inputs : public COwnPtrArray <CSapi5InputInfo>
{
protected:
	CSapi5Inputs();
public:
	virtual ~CSapi5Inputs();
	DECLARE_DYNCREATE(CSapi5Inputs)

// Attributes
	const UINT	mLogLevelDebug;

// Operations
	void Enumerate ();

	INT_PTR FindEngineId (LPCTSTR pEngineId);
	CSapi5InputInfo * GetEngineId (LPCTSTR pEngineId);

	INT_PTR FindEngineName (LPCTSTR pEngineName);
	CSapi5InputInfo * GetEngineName (LPCTSTR pEngineName);

	INT_PTR FindInput (LANGID pLangId, bool pUseDefaults);
	CSapi5InputInfo * GetInput (LANGID pLangId, bool pUseDefaults);

	bool RemoveInput (INT_PTR pInputNdx);
	bool RemoveInput (const CSapi5InputInfo * pInputInfo);

	INT_PTR DefaultInputNdx ();
	bool InputSupportsLanguage (CSapi5InputInfo * pInputInfo, LANGID pLangId, bool pUseDefaults);

	void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	static void LogInputInfo (UINT pLogLevel, CSapi5InputInfo & pInputInfo, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);

// Implementation
protected:
	void MakeLanguageMatchList (LANGID pLanguageId, CArrayEx <LANGID, LANGID> & pLanguageIds, bool pUseDefaults);
	static void LogInputToken (UINT pLogLevel, void * pInputToken, LPCTSTR pTitle = NULL);
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // _SAPI5INPUTS_H_INCLUDED
