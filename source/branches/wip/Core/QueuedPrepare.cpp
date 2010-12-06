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
#include "StdAfx.h"
#include "DaCore.h"
#include "QueuedPrepare.h"
#include "FileDownload.h"
#include "AgentWnd.h"
#include "StringArrayEx.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_PREPARE	(GetProfileDebugInt(_T("DebugPrepare"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_OPS	(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CQueuedPrepare)

CQueuedPrepare::CQueuedPrepare (long pCharID, long pReqID)
:	CQueuedAction (QueueActionPrepare, pCharID, pReqID),
	mDownloadNotify (NULL),
	mDownloadIsSound (false),
	mDownloadActiveXContext (NULL),
	mMaxConcurrentDownloads (5)	// Limits the number of worker threads spawned
{
}

CQueuedPrepare::~CQueuedPrepare ()
{
}

CQueuedPrepare * CQueuedPrepare::CreateInstance (long pCharID, long pReqID)
{
	return new CQueuedPrepare (pCharID, pReqID);
}

//////////////////////////////////////////////////////////////////////

bool CQueuedPrepare::IsStarted () const
{
	return (mDownloadsRunning.GetCount() > 0);
}

bool CQueuedPrepare::IsComplete () const
{
	return (mDownloadsDone.GetCount() >= mDownloads.GetCount());
}

bool CQueuedPrepare::IsSoundDownload () const
{
	return mDownloadIsSound;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CQueuedPrepare::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool	lRet = false;

	if	(!IsStarted ())
	{
		NotifyStarted (pAgentWnd->mNotify);
	}
	if	(pQueue.GetNextAction (QueueActionPrepare) == this)
	{
#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogMessage (_DEBUG_PREPARE, _T("[%p] Advance [%ls]"), this, (BSTR)GetAnimationNames());
		}
#endif
		if	(IsSoundDownload ())
		{
			FinishDownloads ();
		}
		else
		{
			PutAnimations (pAgentWnd->GetAgentFile ());
		}

		if	(IsComplete ())
		{
#ifdef	_DEBUG_PREPARE
			if	(LogIsActive (_DEBUG_PREPARE))
			{
				LogMessage (_DEBUG_PREPARE, _T("[%p] Complete [%ls]"), this, (BSTR)GetAnimationNames());
			}
#endif
			pQueue.RemoveHead ();

			if	(pAgentWnd->mNotify.GetCount() > 0)
			{
				HRESULT			lPrepareResult = S_OK;
				CFileDownload *	lFileDownload;

				if	(lFileDownload = GetDownload ())
				{
					lPrepareResult = lFileDownload->IsDownloadComplete ();
				}
				NotifyComplete (pAgentWnd->mNotify, lPrepareResult);
			}
		}
		else
		{
			StartDownloads ();
			lRet = true;
		}
	}
	else
	{
		lRet = true; // Was deleted during NotifyStarted
	}
	return lRet;
}

bool CQueuedPrepare::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedPrepare [%p(%d)] Started [%u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted);
	}
#endif
	return false;
}

bool CQueuedPrepare::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	if	(CancelDownloads () == S_OK)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CQueuedPrepare::PutAnimationNames (CAgentFile * pAgentFile, LPCTSTR pAnimationNames, CEventNotify * pDownloadNotify, LPUNKNOWN pDownloadActiveXContext)
{
	HRESULT	lResult = E_INVALIDARG;

	if	(
			(pAgentFile)
		&&	(pAgentFile->IsAcfFile ())
		&&	(pAnimationNames)
		)
	{
		CAtlStringArray			lAnimationNames;
		INT_PTR					lNameNdx;
		INT_PTR					lAnimationNdx;
		CAtlString				lAcaPath;
		tPtr <CFileDownload>	lDownload;

#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogMessage (_DEBUG_PREPARE, _T("[%p] PutAnimationNames [%s] [%p] for [%p] [%ls]"), this, pAnimationNames, pDownloadNotify, pAgentFile, (BSTR)pAgentFile->GetPath());
		}
#endif
		lResult = S_FALSE;
		mDownloadNotify = pDownloadNotify;
		mDownloadActiveXContext = pDownloadActiveXContext;
		MakeStringArray (pAnimationNames, lAnimationNames, _T(","));

		for	(lNameNdx = 0; lNameNdx < (INT_PTR)lAnimationNames.GetCount(); lNameNdx++)
		{
			lAnimationNdx = pAgentFile->FindAnimation (lAnimationNames [lNameNdx]);
			if	(lAnimationNdx >= 0)
			{
				if	(!pAgentFile->IsAnimationLoaded (lAnimationNdx))
				{
					lAcaPath = pAgentFile->GetAnimationAcaPath (lAnimationNdx);
					if	(lAcaPath.IsEmpty ())
					{
						continue;
					}
					lDownload = CFileDownload::CreateInstance (lAcaPath);
					if	(!lDownload)
					{
						lResult = E_OUTOFMEMORY;
						break;
					}
					mDownloads.SetAt (lAnimationNames [lNameNdx], lDownload.Detach());
					lResult = S_OK;
				}
			}
			else
			{
				lResult = AGENTERR_ANIMATIONNOTFOUND;
				break;
			}
		}

#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] PutAnimationNames [%s]"), this, pAnimationNames);
		}
#endif
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CQueuedPrepare::PutStateNames (CAgentFile * pAgentFile, LPCTSTR pStateNames, CEventNotify * pDownloadNotify, LPUNKNOWN pDownloadActiveXContext)
{
	HRESULT	lResult = E_INVALIDARG;

	if	(
			(pAgentFile)
		&&	(pAgentFile->IsAcfFile ())
		&&	(pStateNames)
		)
	{
		CAtlStringArray			lStateNames;
		INT_PTR					lNameNdx;
		INT_PTR					lStateNdx;
		CAtlString				lAcaPath;
		tPtr <CFileDownload>	lDownload;

#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogMessage (_DEBUG_PREPARE, _T("[%p] PutStateNames [%s] [%p] for [%p] [%ls]"), this, pStateNames, pDownloadNotify, pAgentFile, (BSTR)pAgentFile->GetPath());
		}
#endif
		lResult = S_FALSE;
		mDownloadNotify = pDownloadNotify;
		mDownloadActiveXContext = pDownloadActiveXContext;
		MakeStringArray (pStateNames, lStateNames, _T(","));

		for	(lNameNdx = 0; lNameNdx < (INT_PTR)lStateNames.GetCount(); lNameNdx++)
		{
			lStateNdx = pAgentFile->FindState (lStateNames [lNameNdx]);
			if	(lStateNdx >= 0)
			{
				const CAtlStringArray &	lAnimations = pAgentFile->GetStates().mGestures [lStateNdx];
				INT_PTR					lAnimationNdx;

				for	(lAnimationNdx = 0; lAnimationNdx < (INT_PTR)lAnimations.GetCount(); lAnimationNdx++)
				{
					if	(
							(mDownloads.Lookup (lAnimations [lAnimationNdx]) == NULL)
						&&	(!pAgentFile->IsAnimationLoaded (lAnimations [lAnimationNdx]))
						)
					{
						lAcaPath = pAgentFile->GetAnimationAcaPath (lAnimations [lAnimationNdx]);
						if	(lAcaPath.IsEmpty ())
						{
							continue;
						}
						lDownload = CFileDownload::CreateInstance (lAcaPath);
						if	(!lDownload)
						{
							lResult = E_OUTOFMEMORY;
							break;
						}
						mDownloads.SetAt (lAnimations [lAnimationNdx], lDownload.Detach());
						lResult = S_OK;
					}
				}
			}
			else
			{
				lResult = AGENTERR_STATENOTFOUND;
				break;
			}
		}

#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] PutStateNames [%s]"), this, pStateNames);
		}
#endif
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CQueuedPrepare::PutSoundUrl (CAgentFile * pAgentFile, LPCTSTR pSoundUrl, CEventNotify * pDownloadNotify, LPUNKNOWN pDownloadActiveXContext)
{
	HRESULT	lResult = E_INVALIDARG;

	if	(
			(pAgentFile)
		&&	(pSoundUrl)
		&&	(*pSoundUrl)
		)
	{
		CAtlString				lSoundUrl;
		tPtr <CFileDownload>	lDownload;

#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogMessage (_DEBUG_PREPARE, _T("[%p] PutSoundUrl [%s] [%p] for [%p] [%ls]"), this, pSoundUrl, pDownloadNotify, pAgentFile, (BSTR)pAgentFile->GetPath());
		}
#endif
		mDownloadNotify = pDownloadNotify;
		mDownloadActiveXContext = pDownloadActiveXContext;
		mDownloadIsSound = true;

		if	(pAgentFile->IsRelativeFilePath (pSoundUrl))
		{
			lSoundUrl = pAgentFile->ParseRelativePath (pSoundUrl);
		}
		else
		{
			lSoundUrl = pAgentFile->ParseFilePath (pSoundUrl);
		}
		if	(pAgentFile->IsProperFilePath (lSoundUrl))
		{
			if	(lDownload = CFileDownload::CreateInstance (lSoundUrl))
			{
				mDownloads.SetAt (lSoundUrl, lDownload.Detach());
				lResult = S_OK;
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] PutSoundUrl [%s]"), this, pSoundUrl);
		}
#endif
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CQueuedPrepare::StartDownloads ()
{
	HRESULT		lResult = S_FALSE;
	POSITION	lPos;

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogMessage (_DEBUG_PREPARE, _T("[%p] StartDownloads [%ls] Downloads [%d] Running [%d] Done [%d]"), this, (BSTR)GetAnimationNames(), mDownloads.GetCount(), mDownloadsRunning.GetCount(), mDownloadsDone.GetCount());
	}
#endif

	if	((int)mDownloadsRunning.GetCount() < (int)mMaxConcurrentDownloads)
	{
		for	(lPos = mDownloads.GetStartPosition(); lPos;)
		{
			CAtlString		lAnimationName;
			CFileDownload *	lDownload = NULL;

			mDownloads.GetNextAssoc (lPos, lAnimationName, lDownload);

			if	(
					(mDownloadsDone.FindSortedQS (lDownload) >= 0)
				||	(mDownloadsRunning.FindSortedQS (lDownload) >= 0)
				)
			{
				continue;
			}

			lResult = lDownload->Download (mDownloadActiveXContext, mDownloadNotify);
#ifdef	_DEBUG_PREPARE
			if	(LogIsActive (_DEBUG_PREPARE))
			{
				LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] Download [%ls]"), this, (BSTR)lDownload->GetURL());
			}
#endif
			if	(SUCCEEDED (lResult))
			{
				mDownloadsRunning.AddSortedQS (lDownload);

				if	((int)mDownloadsRunning.GetCount() >= (int)mMaxConcurrentDownloads)
				{
					break;
				}
			}
			else
			{
				mDownloadsDone.AddSortedQS (lDownload);
				break;
			}
		}
	}

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] StartDownloads [%ls] Downloads [%d] Running [%d] Done [%d]"), this, (BSTR)GetAnimationNames(), mDownloads.GetCount(), mDownloadsRunning.GetCount(), mDownloadsDone.GetCount());
	}
#endif
	return lResult;
}

HRESULT CQueuedPrepare::FinishDownloads ()
{
	HRESULT			lResult = S_FALSE;
	CFileDownload *	lDownload = NULL;
	POSITION		lPos;
	INT_PTR			lNdx;

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogMessage (_DEBUG_PREPARE, _T("[%p] FinishDownloads [%d] Running [%d] Done [%d]"), this, mDownloads.GetCount(), mDownloadsRunning.GetCount(), mDownloadsDone.GetCount());
	}
#endif

	for	(lNdx = mDownloadsRunning.GetCount()-1; lNdx >= 0; lNdx--)
	{
		if	(lDownload = mDownloadsRunning.GetAt (lNdx))
		{
			try
			{
				if	(lDownload->IsDownloadComplete () != S_FALSE)
				{
					mDownloadsRunning.RemoveAt (lNdx);
					mDownloadsDone.AddSortedQS (lDownload);
					lResult = S_OK;
				}
			}
			catch AnyExceptionDebug
		}
	}

	for	(lPos = mDownloads.GetStartPosition(); lPos;)
	{
		CAtlString	lAnimationName;

		mDownloads.GetNextAssoc (lPos, lAnimationName, lDownload=NULL);

		if	(
				(lDownload)
			&&	(mDownloadsDone.FindSortedQS (lDownload) < 0)
			)
		{
			try
			{
				if	(lDownload->IsDownloadComplete () != S_FALSE)
				{
					mDownloadsDone.AddSortedQS (lDownload);
					lResult = S_OK;
				}
			}
			catch AnyExceptionDebug
		}
	}

	return lResult;
}

HRESULT CQueuedPrepare::CancelDownloads ()
{
	HRESULT			lResult = S_FALSE;
	CFileDownload *	lDownload = NULL;
	POSITION		lPos;
	INT_PTR			lNdx;

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogMessage (_DEBUG_PREPARE, _T("[%p] CancelDownloads [%ls] Downloads [%d] Running [%d] Done [%d]"), this, (BSTR)GetAnimationNames(), mDownloads.GetCount(), mDownloadsRunning.GetCount(), mDownloadsDone.GetCount());
	}
#endif

	for	(lNdx = mDownloadsRunning.GetCount()-1; lNdx >= 0; lNdx--)
	{
		if	(lDownload = mDownloadsRunning.GetAt (lNdx))
		{
			try
			{
				if	(
						(lDownload->IsDownloadComplete () != S_OK)
					&&	(lDownload->CancelDownload ())
					)
				{
					lResult = S_OK;
				}
			}
			catch AnyExceptionDebug

			mDownloadsDone.AddSortedQS (lDownload);
		}
		mDownloadsRunning.RemoveAt (lNdx);
	}

	for	(lPos = mDownloads.GetStartPosition(); lPos;)
	{
		CAtlString	lAnimationName;

		mDownloads.GetNextAssoc (lPos, lAnimationName, lDownload=NULL);

		if	(
				(lDownload)
			&&	(mDownloadsDone.FindSortedQS (lDownload) < 0)
			)
		{
			try
			{
				if	(
						(lDownload->IsDownloadComplete () != S_OK)
					&&	(lDownload->CancelDownload ())
					)
				{
					lResult = S_OK;
				}
			}
			catch AnyExceptionDebug

			mDownloadsDone.AddSortedQS (lDownload);
		}
	}

	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CQueuedPrepare::FindDownload (CFileDownload * pDownload)
{
	bool		lRet = false;
	CAtlString	lAnimationName;

	if	(
			(pDownload)
		&&	(mDownloads.FindValue (pDownload, lAnimationName))
		)
	{
		lRet = true;
	}
	return lRet;
}

CFileDownload * CQueuedPrepare::GetDownload ()
{
	CAtlString	lFirstKey;
	return mDownloads.GetFirst (lFirstKey);
}

//////////////////////////////////////////////////////////////////////

bool CQueuedPrepare::PutAnimations (CAgentFile * pAgentFile)
{
	bool		lRet = false;
	POSITION	lPos;

	if	(pAgentFile)
	{
		for	(lPos = mDownloads.GetStartPosition(); lPos;)
		{
			CAtlString		lAnimationName;
			CFileDownload *	lDownload = NULL;

			mDownloads.GetNextAssoc (lPos, lAnimationName, lDownload);

			if	(
					(mDownloadsRunning.FindSortedQS (lDownload) >= 0)
				&&	(lDownload->IsDownloadComplete() == S_OK)
				&&	(PutAnimation (pAgentFile, lDownload))
				)
			{
				lRet = true;
			}
		}
#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogMessage (_DEBUG_PREPARE, _T("[%p] PutAnimations [%ls] Downloads [%d] Running [%d] Done [%d]"), this, (BSTR)GetAnimationNames(), mDownloads.GetCount(), mDownloadsRunning.GetCount(), mDownloadsDone.GetCount());
		}
#endif
	}
	return lRet;
}

HRESULT CQueuedPrepare::PutAnimation (CAgentFile * pAgentFile, CFileDownload * pDownload)
{
	HRESULT		lResult = S_FALSE;
	CAtlString	lAnimationName;

	if	(
			(pAgentFile)
		&&	(pDownload)
		&&	(mDownloads.FindValue (pDownload, lAnimationName))
		&&	(mDownloadsDone.FindSortedQS (pDownload) < 0)
		)
	{
		lResult = pDownload->IsDownloadComplete ();
		if	(lResult == S_OK)
		{
			lResult = pAgentFile->LoadAnimationAca (lAnimationName, pDownload);
		}
#ifdef	_DEBUG_PREPARE
		if	(LogIsActive (_DEBUG_PREPARE))
		{
			LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] PutAnimation [%s] [%ls] [%ls] for [%ls]"), this, lAnimationName, (BSTR)pDownload->GetURL(), (BSTR)pDownload->GetCacheName(), (BSTR)pAgentFile->GetPath());
		}
#endif

		mDownloadsRunning.RemoveSortedQS (pDownload);
		mDownloadsDone.AddSortedQS (pDownload);

#ifdef	_DEBUG_PREPARE
		if	(
				(LogIsActive (_DEBUG_PREPARE))
			&&	(mDownloadsDone.GetCount() == mDownloads.GetCount())
			)
		{
			LogMessage (_DEBUG_PREPARE, _T("[%p] PrepareComplete [%ls]"), this, (BSTR)GetAnimationNames());
		}
#endif
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

tBstrPtr CQueuedPrepare::GetAnimationNames (LPCTSTR pDelimiter)
{
	POSITION		lPos;
	CAtlString		lAnimationName;
	CFileDownload *	lDownload;
	CAtlStringArray	lAnimationNames;

	for	(lPos = mDownloads.GetStartPosition(); lPos;)
	{
		mDownloads.GetNextAssoc (lPos, lAnimationName, lDownload);
		AddUniqueString (lAnimationNames, lAnimationName);
	}
	return JoinStringArray (lAnimationNames, pDelimiter).AllocSysString ();
}
