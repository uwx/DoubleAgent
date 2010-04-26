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
#include "AgentFile.h"

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)
//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CAgentFiles
{
public:
	CAgentFiles();
	virtual ~CAgentFiles();

// Attributes
	const CAtlPtrTypeArray <CAgentFile> & Files () const;

// Operations
	HRESULT Load (LPCTSTR pPath = NULL, UINT pLogLevel = 15);

	INT_PTR FindDefChar ();
	CAgentFile * GetDefChar ();

	static tBstrPtr GetDefCharPath (const CAtlStringArray * pSearchPath = NULL);
	static HRESULT SetDefCharPath (LPCTSTR pCharPath);

	static tBstrPtr GetAgentPath (bool pAlternatePlatform = false);
	static tBstrPtr GetSystemCharsPath (UINT pPathNum = 0, UINT * pPathNumFound = NULL);
	static tBstrPtr GetOfficeCharsPath ();

// Implementation
protected:
	CAtlOwnPtrArray <CAgentFile>	mFiles;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////
