/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "AgentFile.h"

//////////////////////////////////////////////////////////////////////

class CAgentFiles
{
public:
	_DACORE_IMPEXP CAgentFiles();
	_DACORE_IMPEXP virtual ~CAgentFiles();

// Attributes
	_DACORE_IMPEXP const CAtlPtrTypeArray <CAgentFile>& Files () const;

// Operations
	_DACORE_IMPEXP HRESULT Load (LPCTSTR pPath = NULL, UINT pLogLevel = 15);

	_DACORE_IMPEXP INT_PTR FindDefChar ();
	_DACORE_IMPEXP CAgentFile* GetDefChar ();

	_DACORE_IMPEXP static tBstrPtr GetDefCharPath (const CAtlStringArray* pSearchPath = NULL);
	_DACORE_IMPEXP static HRESULT SetDefCharPath (LPCTSTR pCharPath);

	_DACORE_IMPEXP static tBstrPtr GetAgentPath (bool pAlternatePlatform = false);
	_DACORE_IMPEXP static tBstrPtr GetSystemCharsPath (UINT pPathNum = 0, UINT* pPathNumFound = NULL);
	_DACORE_IMPEXP static tBstrPtr GetOfficeCharsPath ();

// Implementation
protected:
	CAtlOwnPtrArray <CAgentFile>	mFiles;
};

//////////////////////////////////////////////////////////////////////
