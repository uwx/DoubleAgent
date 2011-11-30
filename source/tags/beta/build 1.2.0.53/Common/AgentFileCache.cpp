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
#include "StdAfx.h"
#include <shlwapi.h>
#include "AgentFileCache.h"
#include "Registry.h"
#include "GuidStr.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_CACHE	(GetProfileDebugInt(_T("LogFileCache"),LogVerbose,true)&0xFFFF|LogTime)
#endif

//////////////////////////////////////////////////////////////////////

CAgentFileCache::CAgentFileCache ()
:	mIdCode (0)
{
}

CAgentFileCache::~CAgentFileCache ()
{
#ifdef	_DEBUG_CACHE
	try
	{
		LogMessage (_DEBUG_CACHE, _T("[%p(%u)] CAgentFileCache::~CAgentFileCache Files [%d] Clients [%d]"), this, mIdCode, mCachedFiles.GetCount(), mFileClients.GetCount());
	}
	catch AnyExceptionSilent
#endif
	try
	{
		mFileClients.RemoveAll ();
	}
	catch AnyExceptionSilent
	try
	{
		mCachedFiles.DeleteAll ();
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CAgentFileCache::CachedFileCount () const
{
	INT_PTR	lRet = 0;
	CLockCS	lLock (mCritSec);

	try
	{
		lRet = mCachedFiles.GetCount();
	}
	catch AnyExceptionSilent

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CAgentFileCache::CacheFile (CAgentFile* pFile, CAgentFileClient* pClient)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(
				(pFile)
			&&	(pClient)
			)
		{
			INT_PTR											lFileNdx;
			tPtr <CAtlPtrTypeArray <CAgentFileClient> >&	lClients = mFileClients [pFile];

			lFileNdx = mCachedFiles.Find (pFile);
			if	(lFileNdx < 0)
			{
				mCachedFiles.Add (pFile);
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] Cache file [%p] [%s] for Client [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient));
#endif
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] Duplicate file [%p] [%s] for Client [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient));
			}
#endif

			if	(!lClients)
			{
				lClients = new CAtlPtrTypeArray <CAgentFileClient>;
			}
			if	(
					(lClients)
				&&	(lClients->Find (pClient) < 0)
				)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] File [%p] [%s] Client [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient));
#endif
				lClients->Add (pClient);
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] File [%p] [%s] Duplicate Client [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient));
			}
#endif
			lRet = true;
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("[%p(%u)] CacheFile failed for File [%p] Client [%p] [%s]"), this, mIdCode, pFile, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CAgentFileCache::UncacheFile (CAgentFile* pFile)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR	lFileNdx;

		if	(pFile)
		{
			lFileNdx = mCachedFiles.Find (pFile);

			if	(lFileNdx >= 0)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] Uncache file [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path);
#endif
				mCachedFiles.RemoveAt (lFileNdx);
				mFileClients.RemoveKey (pFile);
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CAgentFile* CAgentFileCache::GetCachedFile (INT_PTR pFileNdx)
{
	CAgentFile*	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		lRet = mCachedFiles (pFileNdx);
	}
	catch AnyExceptionDebug

	return lRet;
}

CAgentFile* CAgentFileCache::FindCachedFile (LPCTSTR pFileName)
{
	CAgentFile*	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		CAgentFile*	lFile;
		CAtlString		lFileName (pFileName);
		INT_PTR			lNdx;

		PathSearchAndQualify (pFileName, lFileName.GetBuffer(MAX_PATH), MAX_PATH);
		lFileName.ReleaseBuffer ();

		for	(lNdx = 0; lFile = mCachedFiles (lNdx); lNdx++)
		{
			if	(lFileName.CompareNoCase ((BSTR)lFile->Path) == 0)
			{
				lRet = lFile;
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

CAgentFile* CAgentFileCache::FindCachedFile (const GUID& pFileGuid)
{
	CAgentFile*	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		CAgentFile*	lFile;
		INT_PTR			lNdx;

		for	(lNdx = 0; lFile = mCachedFiles (lNdx); lNdx++)
		{
			if	(IsEqualGUID (lFile->Header.Guid, pFileGuid))
			{
				lRet = lFile;
				break;
			}
		}
	}
	catch AnyExceptionDebug

#ifdef	_DEBUG_CACHE
	LogMessage (_DEBUG_CACHE, _T("[%p(%u)] FindCachedFile [%p] [%s]"), this, mIdCode, lRet, CGuidStr::GuidName (pFileGuid));
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CAgentFileCache::AddFileClient (CAgentFile* pFile, CAgentFileClient* pClient)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(
				(pFile)
			&&	(pClient)
			&&	(mCachedFiles.Find (pFile) >= 0)
			)
		{
			tPtr <CAtlPtrTypeArray <CAgentFileClient> >&	lClients = mFileClients [pFile];

			if	(
					(lClients)
				&&	(lClients->Find (pClient) < 0)
				)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] File [%p] [%s] Client [%p] [%s] Clients [%d]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient), lClients->GetCount());
#endif
				lClients->Add (pClient);
				lRet = true;
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] File [%p] [%s] Duplicate Client [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient));
			}
#endif
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("[%p(%u)] AddClient failed for File [%p] Client [%p] [%s]"), this, mIdCode, pFile, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CAgentFileCache::RemoveFileClient (CAgentFile* pFile, CAgentFileClient* pClient, bool pDeleteUnusedFile)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR	lFileNdx;

		if	(
				(pFile)
			&&	(pClient)
			&&	((lFileNdx = mCachedFiles.Find (pFile)) >= 0)
			)
		{
			tPtr <CAtlPtrTypeArray <CAgentFileClient> >&	lClients = mFileClients [pFile];

			if	(
					(lClients)
				&&	(lClients->Find (pClient) >= 0)
				)
			{
				lClients->Remove (pClient);
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] File [%p] [%s] Remove client [%p] [%s] Clients [%d]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient), lClients->GetCount());
#endif
				lRet = true;

				if	(lClients->GetCount() <= 0)
				{
					mFileClients.RemoveKey (pFile);
					if	(pDeleteUnusedFile)
					{
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("[%p(%u)] Delete cached File [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path);
#endif
						mCachedFiles.DeleteAt (lFileNdx);
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("[%p(%u)] Deleted cached File [%p]"), this, mIdCode, pFile);
#endif
					}
					else
					{
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("[%p(%u)] Unused cached File [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path);
#endif
						mCachedFiles.RemoveAt (lFileNdx);
					}
				}
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("[%p(%u)] File [%p] [%s] No Client [%p] [%s]"), this, mIdCode, pFile, (BSTR)(BSTR)(BSTR)(BSTR)pFile->Path, pClient, AtlTypeName(pClient));
			}
#endif
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("[%p(%u)] RemoveClient failed for [%p] Client [%p] [%s]"), this, mIdCode, pFile, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CAgentFileCache::GetFileClients (CAgentFile* pFile, CAtlPtrTypeArray <CAgentFileClient>& pClients)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		CAtlPtrTypeArray <CAgentFileClient>*	lClients = NULL;

		if	(
				(pFile)
			&&	(mFileClients.Lookup (pFile, lClients))
			&&	(lClients)
			)
		{
			pClients.Copy (*lClients);
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
