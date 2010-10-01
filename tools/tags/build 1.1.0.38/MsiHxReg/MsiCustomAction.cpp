#include "StdAfx.h"
#include "MsiCustomAction.h"

#pragma comment(lib, "msi.lib")

#ifdef	_DEBUG
//#define	_DEBUG_PROGRESS	LogNormal|LogHighVolume|LogTimeMs
#endif

//////////////////////////////////////////////////////////////////////

CMsiCustomAction::CMsiCustomAction (MSIHANDLE pInstall)
:	mInstall (pInstall),
	mTickIncrement (1)
{
}

CMsiCustomAction::~CMsiCustomAction ()
{
}

//////////////////////////////////////////////////////////////////////

bool CMsiCustomAction::IsInstallMode () const
{
	if	(
			(!MsiGetMode (mInstall, MSIRUNMODE_ROLLBACK))
		&&	(!MsiGetMode (mInstall, MSIRUNMODE_COMMIT))
		)
	{
		return true;
	}
	return false;
}

bool CMsiCustomAction::IsCommitMode () const
{
	if	(MsiGetMode (mInstall, MSIRUNMODE_COMMIT))
	{
		return true;
	}
	return false;
}

bool CMsiCustomAction::IsRollbackMode () const
{
	if	(MsiGetMode (mInstall, MSIRUNMODE_ROLLBACK))
	{
		return true;
	}
	return false;
}

bool CMsiCustomAction::IsScheduledMode () const
{
	if	(MsiGetMode (mInstall, MSIRUNMODE_SCHEDULED))
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

LPTSTR CMsiCustomAction::GetMsiProperty (LPCTSTR pName) const
{
	DWORD				lError;
	DWORD				lBufferSize = 1000;
	tArrayPtr <TCHAR>	lBuffer = new TCHAR [lBufferSize];

	if	((lError = MsiGetProperty (mInstall, pName, lBuffer, &lBufferSize)) == ERROR_SUCCESS)
	{
		return lBuffer.Detach ();
	}
	else
	if	(
			((lError = MsiGetProperty (mInstall, pName, lBuffer, &lBufferSize)) == ERROR_MORE_DATA)
		&&	(lBufferSize)
		&&	(lBuffer = new TCHAR [lBufferSize])
		&&	((lError = MsiGetProperty (mInstall, pName, lBuffer, &lBufferSize)) == ERROR_SUCCESS)
		)
	{
		return lBuffer.Detach ();
	}
	else
	if	(LogIsActive())
	{
		LogWinErr (LogIfActive, lError, _T("MsiGetProperty"));
	}
	return NULL;
}

bool CMsiCustomAction::PutMsiProperty (LPCTSTR pName, LPCTSTR pProperty) const
{
	DWORD	lError;

	if	(!pProperty)
	{
		pProperty = _T("");
	}
	if	((lError = MsiSetProperty (mInstall, pName, pProperty)) == ERROR_SUCCESS)
	{
		return true;
	}
	else
	if	(LogIsActive())
	{
		LogWinErr (LogIfActive, lError, _T("MsiSetProperty"));
	}
	return false;
}

LPTSTR CMsiCustomAction::GetActionData () const
{
	return GetMsiProperty (_T("CustomActionData"));
}

LPTSTR CMsiCustomAction::GetActionDataPart (LPCTSTR pActionData, int pPart, TCHAR pDelim) const
{
	if	(pActionData)
	{
		tArrayPtr <TCHAR>	lRet;
		int					lPart = pPart;
		LPCTSTR				lStart = pActionData;
		LPCTSTR				lEnd;
		int					lSize;

		while	(
					(lEnd = _tcschr (lStart, pDelim))
				&&	(lPart-- > 0)
				)
		{
			lStart = lEnd+1;
		}
		if	(
				(!lEnd)
			&&	(lPart == 0)
			)
		{
			lEnd = lStart + _tcslen (lStart);
		}

		if	(lEnd > lStart)
		{
			lSize = lEnd - lStart + 1;
			lRet = new TCHAR [lSize];
			memset ((void *) (LPCTSTR) lRet, 0, lSize*sizeof(TCHAR));
			if	(lSize > 1)
			{
				_tcsncpy (lRet, lStart, lSize-1);
			}
		}

		if	(lRet)
		{
			return lRet.Detach ();
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////

void CMsiCustomAction::ActionStart (LPCTSTR pActionName, LPCTSTR pActionDescription, LPCTSTR pActionText, UINT pTotalTicks)
{
	try
	{
		PMSIHANDLE	lRecord;
		TCHAR		lActionDescription [2048] = {0};

		if	(pActionDescription)
		{
			_stprintf (lActionDescription, pActionDescription, pActionText);
		}

		if	(LogIsActive ())
		{
			LogMessage (LogNormal, _T("ActionStart [%s] [%s] [%s] Ticks [%u]"), pActionName, lActionDescription, pActionText, pTotalTicks);
		}

		if	(
				(pTotalTicks > 0)
			&&	(!IsScheduledMode ())
			)
		{
			AddToProgress (pTotalTicks);
		}

		lRecord = MsiCreateRecord (3);
		if	(lRecord != NULL)
		{
			MsiRecordSetString (lRecord, 1, pActionName);
			MsiRecordSetString (lRecord, 2, lActionDescription);
			MsiRecordSetString (lRecord, 3, _T("[1]"));
			MsiProcessMessage (mInstall, INSTALLMESSAGE_ACTIONSTART, lRecord);
		}
	}
	catch AnyExceptionSilent
}

void CMsiCustomAction::ActionProgress (LPCTSTR pActionText, UINT pTicks)
{
	try
	{
		PMSIHANDLE	lRecord;
		int			lResult;

		if	(pActionText)
		{
			lRecord = MsiCreateRecord (1);
			if	(lRecord != NULL)
			{
				MsiRecordSetString (lRecord, 1, pActionText);
				lResult = MsiProcessMessage (mInstall, INSTALLMESSAGE_ACTIONDATA, lRecord);
#ifdef	_DEBUG_PROGRESS
				LogMessage (_DEBUG_PROGRESS, _T("ActionProgress [%s] [%d]"), pActionText, lResult);
#endif
			}
		}

		if	(
				(pTicks)
			&&	(!IsScheduledMode ())
			)
		{
			AdvanceProgress (pTicks);
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

void CMsiCustomAction::ResetProgress (UINT pTotalTicks)
{
	try
	{
		PMSIHANDLE	lRecord;
		int			lResult;

		lRecord = MsiCreateRecord (4);
		if	(lRecord != NULL)
		{
			MsiRecordSetInteger (lRecord, 1, 0);
			MsiRecordSetInteger (lRecord, 2, pTotalTicks * mTickIncrement);
			MsiRecordSetInteger (lRecord, 3, 0);
			MsiRecordSetInteger (lRecord, 4, 0);
			lResult = MsiProcessMessage (mInstall, INSTALLMESSAGE_PROGRESS, lRecord);
#ifdef	_DEBUG_PROGRESS
			LogMessage (_DEBUG_PROGRESS, _T("ResetProgress [%u] [%d]"), pTotalTicks * mTickIncrement, lResult);
#endif
		}
	}
	catch AnyExceptionSilent
}

void CMsiCustomAction::AddToProgress (UINT pTotalTicks)
{
	try
	{
		PMSIHANDLE	lRecord;
		int			lResult;

		lRecord = MsiCreateRecord (4);
		if	(lRecord != NULL)
		{
			MsiRecordSetInteger (lRecord, 1, 3);
			MsiRecordSetInteger (lRecord, 2, pTotalTicks * mTickIncrement);
			MsiRecordSetInteger (lRecord, 3, 0);
			MsiRecordSetInteger (lRecord, 4, 0);
			lResult = MsiProcessMessage (mInstall, INSTALLMESSAGE_PROGRESS, lRecord);
#ifdef	_DEBUG_PROGRESS
			LogMessage (_DEBUG_PROGRESS, _T("AddToProgress [%u] [%d]"), pTotalTicks * mTickIncrement, lResult);
#endif
		}
	}
	catch AnyExceptionSilent
}

void CMsiCustomAction::AdvanceProgress (UINT pTicks)
{
	try
	{
		PMSIHANDLE	lRecord;
		int			lResult;

		lRecord = MsiCreateRecord (4);
		if	(lRecord != NULL)
		{
			MsiRecordSetInteger (lRecord, 1, 2);
			MsiRecordSetInteger (lRecord, 2, pTicks * mTickIncrement);
			MsiRecordSetInteger (lRecord, 3, 0);
			MsiRecordSetInteger (lRecord, 4, 0);
			lResult = MsiProcessMessage (mInstall, INSTALLMESSAGE_PROGRESS, lRecord);
#ifdef	_DEBUG_PROGRESS
			LogMessage (_DEBUG_PROGRESS, _T("AdvanceProgress [%u] [%d]"), pTicks * mTickIncrement, lResult);
#endif
		}
	}
	catch AnyExceptionSilent
}

void CMsiCustomAction::SetAutoProgress (UINT pTicks)
{
	try
	{
		PMSIHANDLE	lRecord;
		int			lResult;

		lRecord = MsiCreateRecord (4);
		if	(lRecord != NULL)
		{
			MsiRecordSetInteger (lRecord, 1, 1);
			MsiRecordSetInteger (lRecord, 2, pTicks * mTickIncrement);
			MsiRecordSetInteger (lRecord, 3, pTicks ? 1 : 0);
			MsiRecordSetInteger (lRecord, 4, 0);
			lResult = MsiProcessMessage (mInstall, INSTALLMESSAGE_PROGRESS, lRecord);
#ifdef	_DEBUG_PROGRESS
			LogMessage (_DEBUG_PROGRESS, _T("SetAutoProgress [%u] [%d]"), pTicks * mTickIncrement, lResult);
#endif
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

void CMsiCustomAction::EnableCancel (bool pEnable)
{
	try
	{
		PMSIHANDLE	lRecord;

		lRecord = MsiCreateRecord (2);

		if	(lRecord != NULL)
		{
			MsiRecordSetInteger (lRecord, 1, 2);
			MsiRecordSetInteger (lRecord, 2, pEnable ? 1 : 0);
			MsiProcessMessage (mInstall, INSTALLMESSAGE_COMMONDATA, lRecord);
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

bool CMsiCustomAction::IsLogEnabled () const
{
	if	(IsScheduledMode ())
	{
		return true;
	}
	else
	{
		tArrayPtr <TCHAR>	lLogFile = GetMsiProperty (_T("MsiLogFileLocation"));

		if	(
				(lLogFile)
			&&	(*lLogFile)
			)
		{
			return true;
		}
	}
	return false;
}

void CMsiCustomAction::LogInfoMessage (LPCTSTR pMessage)
{
	try
	{
		PMSIHANDLE	lRecord;

		lRecord = MsiCreateRecord (1);
		if	(lRecord != NULL)
		{
			MsiRecordSetString (lRecord, 0, pMessage);
			MsiProcessMessage (mInstall, INSTALLMESSAGE_INFO, lRecord);
		}
	}
	catch AnyExceptionSilent
}

int CMsiCustomAction::ShowWarningMessage (LPCTSTR pMessage, UINT pErrorNum)
{
	int	lResult = 0;

	try
	{
		PMSIHANDLE	lRecord;

		lRecord = MsiCreateRecord (1);

		if	(lRecord != NULL)
		{
			MsiRecordSetString (lRecord, 0, pMessage);
			if	(pErrorNum)
			{
				MsiRecordSetInteger (lRecord, 1, pErrorNum);
			}
			lResult = MsiProcessMessage (mInstall, (INSTALLMESSAGE)(INSTALLMESSAGE_WARNING|MB_ICONWARNING), lRecord);
		}
		else
		{
			lResult = -1;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

int CMsiCustomAction::ShowErrorMessage (LPCTSTR pMessage, UINT pErrorNum)
{
	int	lResult = 0;

	try
	{
		PMSIHANDLE	lRecord;

		lRecord = MsiCreateRecord (1);

		if	(lRecord != NULL)
		{
			MsiRecordSetString (lRecord, 0, pMessage);
			if	(pErrorNum)
			{
				MsiRecordSetInteger (lRecord, 1, pErrorNum);
			}
			lResult = MsiProcessMessage (mInstall, (INSTALLMESSAGE)(INSTALLMESSAGE_ERROR|MB_ICONERROR), lRecord);
		}
		else
		{
			lResult = -1;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

int CMsiCustomAction::ShowUserMessage (LPCTSTR pMessage, UINT pMessageType, UINT pErrorNum)
{
	int	lResult = 0;

	try
	{
		PMSIHANDLE	lRecord;

		lRecord = MsiCreateRecord (1);

		if	(lRecord != NULL)
		{
			MsiRecordSetString (lRecord, 0, pMessage);
			if	(pErrorNum)
			{
				MsiRecordSetInteger (lRecord, 1, pErrorNum);
			}
			if	((pMessageType & MB_ICONMASK) == 0)
			{
				pMessageType |= MB_ICONEXCLAMATION;
			}
			lResult = MsiProcessMessage (mInstall, (INSTALLMESSAGE)(INSTALLMESSAGE_USER|pMessageType), lRecord);
		}
		else
		{
			lResult = -1;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString FormatString (LPCTSTR pFormat, ...)
{
	CAtlString	lRet;
    LPTSTR		lMessage = NULL;
	va_list		lArgPtr;

	va_start (lArgPtr, pFormat);
    FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING, (LPVOID) pFormat, 0, 0, (LPTSTR) &lMessage, 0, &lArgPtr);
    if	(lMessage)
    {
		lRet = lMessage;
	    LocalFree (lMessage);
	}
	return lRet;
}
