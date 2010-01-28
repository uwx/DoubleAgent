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
#ifndef QUEUEDPREPARE_H_INCLUDED_
#define QUEUEDPREPARE_H_INCLUDED_
#pragma once

#include "QueuedAction.h"
#include "DaInternalNotify.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)

class _DACORE_IMPEXP CQueuedPrepare : public CQueuedAction
{
protected:
	CQueuedPrepare (long pCharID = 0, long pReqID = -1);
public:
	virtual ~CQueuedPrepare ();
	DECLARE_DYNCREATE (CQueuedPrepare)

// Attributes
public:
	const UINT	mMaxConcurrentDownloads;

	bool IsStarted () const;
	bool IsComplete () const;
	bool IsSoundDownload () const;

// Operations
public:
	static CQueuedPrepare * CreateObject (long pCharID, long pReqID = -1);

	HRESULT PutAnimationNames (CAgentFile * pAgentFile, LPCTSTR pAnimationNames, IDaNotify * pDownloadNotify = NULL, LPUNKNOWN pDownloadActiveXContext = NULL);
	HRESULT PutStateNames (CAgentFile * pAgentFile, LPCTSTR pStateNames, IDaNotify * pDownloadNotify = NULL, LPUNKNOWN pDownloadActiveXContext = NULL);
	HRESULT PutSoundUrl (CAgentFile * pAgentFile, LPCTSTR pSoundUrl, IDaNotify * pDownloadNotify = NULL, LPUNKNOWN pDownloadActiveXContext = NULL);

	HRESULT StartDownloads ();
	HRESULT CancelDownloads ();
	bool FindDownload (CFileDownload * pDownload);
	CFileDownload * GetDownload ();

	bool PutAnimations (CAgentFile * pAgentFile);
	HRESULT PutAnimation (CAgentFile * pAgentFile, CFileDownload * pDownload);
	tBstrPtr GetAnimationNames (LPCTSTR pDelimiter = _T(","));

// Overrides
	//{{AFX_VIRTUAL(CQueuedAction)
	//}}AFX_VIRTUAL

// Implementation
protected:
	COwnPtrMap <CString, CFileDownload, LPCTSTR>	mDownloads;
	CPtrTypeArray <CFileDownload>					mDownloadsRunning;
	CPtrTypeArray <CFileDownload>					mDownloadsDone;
	IDaNotify *										mDownloadNotify;
	bool											mDownloadIsSound;
	LPUNKNOWN										mDownloadActiveXContext;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

#endif // QUEUEDPREPARE_H_INCLUDED_
