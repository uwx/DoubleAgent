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
#include "AgentFile.h"
#include "AtlCollEx.h"

//////////////////////////////////////////////////////////////////////

class CAgentFileClient
{
public:
	CAgentFileClient () {}
	virtual ~CAgentFileClient () {}
};

//////////////////////////////////////////////////////////////////////

class CAgentFileCache
{
public:
	CAgentFileCache ();
	virtual ~CAgentFileCache ();

// Attributes
public:
	INT_PTR CachedFileCount () const;

// Operations
public:
	bool CacheFile (CAgentFile * pFile, CAgentFileClient * pClient);
	bool UncacheFile (CAgentFile * pFile);
	bool AddFileClient (CAgentFile * pFile, CAgentFileClient * pClient);
	bool RemoveFileClient (CAgentFile * pFile, CAgentFileClient * pClient, bool pDeleteUnusedFile = true);

	CAgentFile * GetCachedFile (INT_PTR pFileNdx);
	CAgentFile * FindCachedFile (LPCTSTR pFileName);
	CAgentFile * FindCachedFile (const GUID & pFileGuid);
	bool GetFileClients (CAgentFile * pFile, CAtlPtrTypeArray <CAgentFileClient> & pClients);

// Implementation
protected:
	mutable CComAutoCriticalSection										mCritSec;
	CAtlOwnPtrArray <CAgentFile>										mCachedFiles;
	CAtlOwnPtrMap <CAgentFile *, CAtlPtrTypeArray <CAgentFileClient> >	mFileClients;
};

//////////////////////////////////////////////////////////////////////
