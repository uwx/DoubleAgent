/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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

/////////////////////////////////////////////////////////////////////////////
//
//	Note - all public methods are virtual so that this file can be in multiple
//	modules and still work properly.
//
/////////////////////////////////////////////////////////////////////////////

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
	virtual INT_PTR CachedFileCount () const;

// Operations
public:
	virtual bool CacheFile (CAgentFile* pFile, CAgentFileClient* pClient);
	virtual bool UncacheFile (CAgentFile* pFile);
	virtual bool AddFileClient (CAgentFile* pFile, CAgentFileClient* pClient);
	virtual bool RemoveFileClient (CAgentFile* pFile, CAgentFileClient* pClient, bool pDeleteUnusedFile = true);

	virtual CAgentFile* GetCachedFile (INT_PTR pFileNdx);
	virtual CAgentFile* FindCachedFile (LPCTSTR pFileName);
	virtual CAgentFile* FindCachedFile (const GUID& pFileGuid);
	virtual bool GetFileClients (CAgentFile* pFile, CAtlPtrTypeArray <CAgentFileClient>& pClients);

// Implementation
public:
	UINT																mIdCode;
protected:
	mutable CComAutoCriticalSection										mCritSec;
	CAtlOwnPtrArray <CAgentFile>										mCachedFiles;
	CAtlOwnPtrMap <CAgentFile*, CAtlPtrTypeArray <CAgentFileClient> >	mFileClients;
};

//////////////////////////////////////////////////////////////////////
