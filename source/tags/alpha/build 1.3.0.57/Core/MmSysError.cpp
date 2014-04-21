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
#include "StdAfx.h"
#include <vfw.h>
#include <vfwmsgs.h>
#include "MmSysError.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
////////////////////////////////////////////////////////////////////////

LPCTSTR _GetMmSysErr (MMRESULT pError)
{
	LPCTSTR	lError = NULL;

	switch (pError)
	{
		case MMSYSERR_ERROR:							lError = _T("MMSYSERR_ERROR"); break;
		case MMSYSERR_BADDEVICEID:						lError = _T("MMSYSERR_BADDEVICEID"); break;
		case MMSYSERR_NOTENABLED:						lError = _T("MMSYSERR_NOTENABLED"); break;
		case MMSYSERR_ALLOCATED:						lError = _T("MMSYSERR_ALLOCATED"); break;
		case MMSYSERR_INVALHANDLE:						lError = _T("MMSYSERR_INVALHANDLE"); break;
		case MMSYSERR_NODRIVER:							lError = _T("MMSYSERR_NODRIVER"); break;
		case MMSYSERR_NOMEM:							lError = _T("MMSYSERR_NOMEM"); break;
		case MMSYSERR_NOTSUPPORTED:						lError = _T("MMSYSERR_NOTSUPPORTED"); break;
		case MMSYSERR_BADERRNUM:						lError = _T("MMSYSERR_BADERRNUM"); break;
		case MMSYSERR_INVALFLAG:						lError = _T("MMSYSERR_INVALFLAG"); break;
		case MMSYSERR_INVALPARAM:						lError = _T("MMSYSERR_INVALPARAM"); break;
		case MMSYSERR_HANDLEBUSY:						lError = _T("MMSYSERR_HANDLEBUSY"); break;
		case MMSYSERR_INVALIDALIAS:						lError = _T("MMSYSERR_INVALIDALIAS"); break;
		case MMSYSERR_BADDB:							lError = _T("MMSYSERR_BADDB"); break;
		case MMSYSERR_KEYNOTFOUND:						lError = _T("MMSYSERR_KEYNOTFOUND"); break;
		case MMSYSERR_READERROR:						lError = _T("MMSYSERR_READERROR"); break;
		case MMSYSERR_WRITEERROR:						lError = _T("MMSYSERR_WRITEERROR"); break;
		case MMSYSERR_DELETEERROR:						lError = _T("MMSYSERR_DELETEERROR"); break;
		case MMSYSERR_VALNOTFOUND:						lError = _T("MMSYSERR_VALNOTFOUND"); break;
		case MMSYSERR_NODRIVERCB:						lError = _T("MMSYSERR_NODRIVERCB"); break;
		case MMSYSERR_MOREDATA:							lError = _T("MMSYSERR_MOREDATA"); break;

		case WAVERR_BADFORMAT:							lError = _T("WAVERR_BADFORMAT"); break;
		case WAVERR_STILLPLAYING:						lError = _T("WAVERR_STILLPLAYING"); break;
		case WAVERR_UNPREPARED:							lError = _T("WAVERR_UNPREPARED"); break;
		case WAVERR_SYNC:								lError = _T("WAVERR_SYNC"); break;

		case MIDIERR_UNPREPARED:						lError = _T("MIDIERR_UNPREPARED"); break;
		case MIDIERR_STILLPLAYING:						lError = _T("MIDIERR_STILLPLAYING"); break;
		case MIDIERR_NOMAP:								lError = _T("MIDIERR_NOMAP"); break;
		case MIDIERR_NOTREADY:							lError = _T("MIDIERR_NOTREADY"); break;
		case MIDIERR_NODEVICE:							lError = _T("MIDIERR_NODEVICE"); break;
		case MIDIERR_INVALIDSETUP:						lError = _T("MIDIERR_INVALIDSETUP"); break;
		case MIDIERR_BADOPENMODE:						lError = _T("MIDIERR_BADOPENMODE"); break;
		case MIDIERR_DONT_CONTINUE:						lError = _T("MIDIERR_DONT_CONTINUE"); break;

		case TIMERR_NOCANDO:							lError = _T("TIMERR_NOCANDO"); break;
		case TIMERR_STRUCT:								lError = _T("TIMERR_STRUCT"); break;

		case MMIOERR_FILENOTFOUND:						lError = _T("MMIOERR_FILENOTFOUND"); break;
		case MMIOERR_OUTOFMEMORY:						lError = _T("MMIOERR_OUTOFMEMORY"); break;
		case MMIOERR_CANNOTOPEN:						lError = _T("MMIOERR_CANNOTOPEN"); break;
		case MMIOERR_CANNOTCLOSE:						lError = _T("MMIOERR_CANNOTCLOSE"); break;
		case MMIOERR_CANNOTREAD:						lError = _T("MMIOERR_CANNOTREAD"); break;
		case MMIOERR_CANNOTWRITE:						lError = _T("MMIOERR_CANNOTWRITE"); break;
		case MMIOERR_CANNOTSEEK:						lError = _T("MMIOERR_CANNOTSEEK"); break;
		case MMIOERR_CANNOTEXPAND:						lError = _T("MMIOERR_CANNOTEXPAND"); break;
		case MMIOERR_CHUNKNOTFOUND:						lError = _T("MMIOERR_CHUNKNOTFOUND"); break;
		case MMIOERR_UNBUFFERED:						lError = _T("MMIOERR_UNBUFFERED"); break;
		case MMIOERR_PATHNOTFOUND:						lError = _T("MMIOERR_PATHNOTFOUND"); break;
		case MMIOERR_ACCESSDENIED:						lError = _T("MMIOERR_ACCESSDENIED"); break;
		case MMIOERR_SHARINGVIOLATION:					lError = _T("MMIOERR_SHARINGVIOLATION"); break;
		case MMIOERR_NETWORKERROR:						lError = _T("MMIOERR_NETWORKERROR"); break;
		case MMIOERR_TOOMANYOPENFILES:					lError = _T("MMIOERR_TOOMANYOPENFILES"); break;
		case MMIOERR_INVALIDFILE:						lError = _T("MMIOERR_INVALIDFILE"); break;

		case ACMERR_NOTPOSSIBLE:						lError = _T("ACMERR_NOTPOSSIBLE"); break;
		case ACMERR_BUSY:								lError = _T("ACMERR_BUSY"); break;
		case ACMERR_UNPREPARED:							lError = _T("ACMERR_UNPREPARED"); break;
		case ACMERR_CANCELED:							lError = _T("ACMERR_CANCELED"); break;

		case MIXERR_INVALLINE:							lError = _T("xxx"); break;
		case MIXERR_INVALCONTROL:						lError = _T("xxx"); break;
		case MIXERR_INVALVALUE:							lError = _T("xxx"); break;
	}

	return lError;
}

////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
MMRESULT LogMmSysErr (unsigned int pLogLevel, MMRESULT pError, LPCTSTR pFormat, ...)
#else
extern "C" void _LogPrepFileLine (LPTSTR pString, LPCSTR pFile, UINT pLine);
MMRESULT _LogMmSysErrFL::LogErr (unsigned int pLogLevel, MMRESULT pError, LPCTSTR pFormat, ...)
#endif
{
	if	(
			(
				(pError != MMSYSERR_NOERROR)
			||	((pLogLevel & 0x000000FF) == LogAlways)
			)
		&&	(LogIsActive (pLogLevel))
		)
	{
		try
		{
			LPTSTR	lExtra = NULL;
			TCHAR	lFormat [1024];
			LPCTSTR	lError;

			if	(pError == MMSYSERR_NOERROR)
			{
				lError = _T("NoError");
			}
			else
			{
				lError = _GetMmSysErr (pError);
			}

			if	(lError)
			{
				_tcscpy (lFormat, _T("MmSysError [%8.8X] [%s]"));
			}
			else
			{
				_tcscpy (lFormat, _T(""));
			}

			if  (pFormat)
			{
				const size_t    lExtraSize = 4096;
				va_list 		lArgPtr;

				lExtra = new TCHAR [lExtraSize];
				if	(lError)
				{
					_tcscpy (lExtra, _T(" => "));
				}
				else
				{
					_tcscpy (lExtra, _T(""));
				}
				va_start (lArgPtr, pFormat);
				_vsntprintf (lExtra+_tcslen(lExtra), lExtraSize-_tcslen(lExtra), pFormat, lArgPtr);
				_tcscat (lFormat, _T("%s"));
			}

			try
			{
				if	(lError)
				{
#ifndef	_LOG_NOFILELINE
					_LogPrepFileLine (lFormat, mFile, mLine);
#endif
					LogMessage (pLogLevel, lFormat, pError, lError, lExtra);
				}
				else
				{
#ifdef	_LOG_NOFILELINE
					LogWinErr (MinLogLevel (pLogLevel, LogAlways), pError, lFormat, lExtra);
#else
					_LogWinErrFL(mFile,mLine).LogErr (MinLogLevel (pLogLevel, LogAlways), pError, lFormat, lExtra);
#endif
				}
			}
			catch AnyExceptionSilent

			if	(lExtra)
			{
				delete [] lExtra;
			}
		}
		catch AnyExceptionSilent
	}

	return pError;
}

////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG
////////////////////////////////////////////////////////////////////////
