#include "StdAfx.h"
#include "ReplayActions.h"
#include "ActionFile.h"
#include "StringArrayEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CActionFile::CActionFile()
{
	m_bCloseOnDelete = TRUE;
}

CActionFile::~CActionFile()
{
	mThreadEndEvent.SetEvent ();
	NotGatedInstance (this);
}

/////////////////////////////////////////////////////////////////////////////

bool CActionFile::Open (LPCTSTR pPath)
{
	bool			lRet = false;
	CFileException	lException;

	if	(CStdioFile::Open (pPath, modeRead|shareDenyWrite, &lException))
	{
		ReadLine (NULL, NULL);
		if	(!QueueUserWorkItem (AsyncThreadProc, PutGatedInstance (this), WT_EXECUTEDEFAULT))
		{
			LogWinErr (LogNormal, GetLastError());
		}
		lRet = true;
	}
	else
	{
		lException.ReportError ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CActionFile::GetLineCount () const
{
	INT_PTR		lRet = 0;
	CSingleLock	lLock (&mCriticalSection, TRUE);

	try
	{
		lRet = mLines.GetSize();
	}
	catch AnyExceptionSilent

	return lRet;
}

CActionLine * CActionFile::GetNextLine ()
{
	CActionLine *	lRet = NULL;
	CSingleLock		lLock (&mCriticalSection, TRUE);

	try
	{
		if	(!mLines.IsEmpty ())
		{
			lRet = mLines.RemoveHead ();
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

CActionLine * CActionFile::PeekNextLine ()
{
	CActionLine *	lRet = NULL;
	CSingleLock		lLock (&mCriticalSection, TRUE);

	try
	{
		if	(!mLines.IsEmpty ())
		{
			lRet = mLines.GetHead ();
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DWORD WINAPI CActionFile::AsyncThreadProc (PVOID lpParameter)
{
	CActionFile *	lThis = NULL;

	if	(LockGatedInstance (lpParameter, lThis, 100))
	{
		try
		{
			for (;;)
			{
				INT_PTR		lLineCount = 0;
				CFileTime	lLineTime;
				CFileTime	lFirstLineTime;

				if	(lThis->mThreadEndEvent.Lock (0))
				{
					lThis->mThreadEndEvent.Unlock ();
					break;
				}

				lLineCount = lThis->ReadLine (&lLineTime, &lFirstLineTime);

				if	(lLineCount <= 0)
				{
					break;
				}
				else
				if	(
						(lLineCount > 2000)
					||	(
							(lLineCount > 1)
						&&	((lLineTime-lFirstLineTime).GetTimeSpan()/CFileTime::Minute > 2)
						)
					)
				{
#ifdef	_DEBUG_NOT
					LogMessage (LogDebugFast, _T("Pausing at [%d] lines [%d] minutes"), lLineCount, (lLineTime-lFirstLineTime).GetTimeSpan()/CFileTime::Minute);
#endif
					SleepEx (1000, TRUE);
				}
			}
		}
		catch AnyExceptionDebug

		FreeGatedInstance (lpParameter, lThis);
	}
	return ERROR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CActionFile::ReadLine (CFileTime * pLineTime, CFileTime * pFirstLineTime)
{
	INT_PTR	lLineCount = -1;
	CString	lFileLine;

	while	(ReadString (lFileLine))
	{
		CStringArray		lLineParts;
		tPtr <CActionLine>	lLine;

		if	(MakeStringArray (lFileLine, lLineParts, _T("\t"), true) >= 3)
		{
			lLine = new CActionLine;
			ParseLineTime (lLineParts[0], lLineParts[1], lLine->mTime);
			lLine->mAction = lLineParts[2];
			lLineParts.RemoveAt (0, 3);
			lLine->mValues.Copy (lLineParts);

			if	(!lLine->mAction.IsEmpty ())
			{
				CSingleLock	lLock (&mCriticalSection, TRUE);

				try
				{
					if	(pLineTime)
					{
						(*pLineTime) = lLine->mTime;
					}
					if	(
							(pFirstLineTime)
						&&	(!mLines.IsEmpty ())
						)
					{
						(*pFirstLineTime) = mLines.GetHead()->mTime;
					}
					mLines.AddTail (lLine.Detach ());
					lLineCount = mLines.GetSize();
				}
				catch AnyExceptionSilent
				break;
			}
			else
			{
				LogMessage (LogNormal, _T("Invalid [%s]"), lFileLine);
			}
		}
		else
		{
			LogMessage (LogNormal, _T("Invalid [%s]"), lFileLine);
		}
	}
	return lLineCount;
}

void CActionFile::ParseLineTime (LPCTSTR pDateStr, LPCTSTR pTimeStr, CFileTime & pLineTime)
{
	tS <SYSTEMTIME>	lLineTime;
	LPCTSTR			lStrPtr = pDateStr;
	LPTSTR			lEndPtr;

	if	(*lStrPtr)
	{
		lLineTime.wYear = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}
	if	(*lStrPtr)
	{
		lLineTime.wMonth = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}
	if	(*lStrPtr)
	{
		lLineTime.wDay = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}

	lStrPtr = pTimeStr;
	if	(*lStrPtr)
	{
		lLineTime.wHour = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}
	if	(*lStrPtr)
	{
		lLineTime.wMinute = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}
	if	(*lStrPtr)
	{
		lLineTime.wSecond = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}
	if	(*lStrPtr)
	{
		lLineTime.wMilliseconds = (WORD)_tcstoul (lStrPtr, &lEndPtr, 10);
		lStrPtr = lEndPtr+1;
	}

	SystemTimeToFileTime (&lLineTime, &pLineTime);
}
