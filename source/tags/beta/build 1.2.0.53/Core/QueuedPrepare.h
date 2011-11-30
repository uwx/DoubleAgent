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
#include "QueuedAction.h"
#include "FileDownload.h"

/////////////////////////////////////////////////////////////////////////////

class CQueuedPrepare : public CQueuedAction
{
	DECLARE_DLL_OBJECT_EX(CQueuedPrepare, _DACORE_IMPEXP)
protected:
	CQueuedPrepare (long pCharID = 0, long pReqID = -1);
public:
	_DACORE_IMPEXP virtual ~CQueuedPrepare ();

// Attributes
public:
	const UINT	mMaxConcurrentDownloads;

	_DACORE_IMPEXP bool IsStarted () const;
	_DACORE_IMPEXP bool IsComplete () const;
	_DACORE_IMPEXP bool IsSoundDownload () const;

// Operations
public:
	_DACORE_IMPEXP static CQueuedPrepare * CreateInstance (long pCharID, long pReqID = -1);

	_DACORE_IMPEXP HRESULT PutAnimationNames (CAgentFile* pAgentFile, LPCTSTR pAnimationNames, class CEventNotify* pDownloadNotify = NULL, LPUNKNOWN pDownloadActiveXContext = NULL);
	_DACORE_IMPEXP HRESULT PutStateNames (CAgentFile* pAgentFile, LPCTSTR pStateNames, class CEventNotify* pDownloadNotify = NULL, LPUNKNOWN pDownloadActiveXContext = NULL);
	_DACORE_IMPEXP HRESULT PutSoundUrl (CAgentFile* pAgentFile, LPCTSTR pSoundUrl, class CEventNotify* pDownloadNotify = NULL, LPUNKNOWN pDownloadActiveXContext = NULL);

	_DACORE_IMPEXP HRESULT StartDownloads ();
	_DACORE_IMPEXP HRESULT FinishDownloads ();
	_DACORE_IMPEXP HRESULT CancelDownloads ();
	_DACORE_IMPEXP bool FindDownload (CFileDownload* pDownload);
	_DACORE_IMPEXP CFileDownload* GetDownload ();

	_DACORE_IMPEXP bool PutAnimations (CAgentFile* pAgentFile);
	_DACORE_IMPEXP HRESULT PutAnimation (CAgentFile* pAgentFile, CFileDownload* pDownload);
	_DACORE_IMPEXP tBstrPtr GetAnimationNames (LPCTSTR pDelimiter = _T(","));

// Overrides
	_DACORE_IMPEXP virtual bool Advance (class CQueuedActions& pQueue, class CAgentWnd* pAgentWnd);
	_DACORE_IMPEXP virtual bool Pause (class CQueuedActions& pQueue, class CAgentWnd* pAgentWnd, bool pPause);
	_DACORE_IMPEXP virtual bool Abort (class CQueuedActions& pQueue, class CAgentWnd* pAgentWnd, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

// Implementation
protected:
	CAtlOwnPtrMap <CAtlString, CFileDownload, CStringElementTraitsI<CAtlString> >	mDownloads;
	CAtlPtrTypeArray <CFileDownload>												mDownloadsRunning;
	CAtlPtrTypeArray <CFileDownload>												mDownloadsDone;
	class CEventNotify*															mDownloadNotify;
	bool																			mDownloadIsSound;
	LPUNKNOWN																		mDownloadActiveXContext;
};

/////////////////////////////////////////////////////////////////////////////
