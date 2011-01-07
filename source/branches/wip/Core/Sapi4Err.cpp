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
#include "Sapi4Err.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
////////////////////////////////////////////////////////////////////////

LPCTSTR _GetSapi4Err (HRESULT pError)
{
	LPCTSTR	lError = NULL;

	switch (pError)
	{
		case AUDERR_BADDEVICEID:				lError = _T("AUDERR_BADDEVICEID"); break;
		case AUDERR_NEEDWAVEFORMAT:				lError = _T("AUDERR_NEEDWAVEFORMAT"); break;
		case AUDERR_NOTENOUGHDATA:				lError = _T("AUD/SR/VTXTERR_NOTENOUGHDATA"); break;
		case AUDERR_NOTPLAYING:					lError = _T("AUDERR_NOTPLAYING"); break;
		case AUDERR_WAVEFORMATNOTSUPPORTED:		lError = _T("AUD/TTS/SR/VTXTERR_WAVEFORMATNOTSUPPORTED"); break;
		case AUDERR_WAVEDEVICEBUSY:				lError = _T("AUD/TTS/SR/VTXTERR_WAVEDEVICEBUSY"); break;
		case AUDERR_WAVEDEVNOTSUPPORTED:		lError = _T("AUDERR_WAVEDEVNOTSUPPORTED"); break;
		case AUDERR_NOTRECORDING:				lError = _T("AUDERR_NOTRECORDING"); break;
		case AUDERR_INVALIDFLAG:				lError = _T("AUDERR_SRERR_INVALIDFLAG"); break;
		case AUDERR_NODRIVER:					lError = _T("AUDERR_NODRIVER"); break;
		case AUDERR_HANDLEBUSY:					lError = _T("AUDERR_HANDLEBUSY"); break;
		case AUDERR_INVALIDNOTIFYSINK:			lError = _T("AUDERR_INVALIDNOTIFYSINK"); break;
		case AUDERR_WAVENOTENABLED:				lError = _T("AUDERR_WAVENOTENABLED"); break;
		case AUDERR_ALREADYCLAIMED:				lError = _T("AUDERR_ALREADYCLAIMED"); break;
		case AUDERR_NOTCLAIMED:					lError = _T("AUDERR_NOTCLAIMED"); break;
		case AUDERR_STILLPLAYING:				lError = _T("AUDERR_STILLPLAYING"); break;
		case AUDERR_ALREADYSTARTED:				lError = _T("AUDERR_ALREADYSTARTED"); break;
		case AUDERR_SYNCNOTALLOWED:				lError = _T("AUDERR_SYNCNOTALLOWED"); break;
		case SRWARN_BAD_LIST_PRONUNCIATION:		lError = _T("SRWARN_BAD_LIST_PRONUNCIATION"); break;
		case TTSERR_OUTOFDISK:					lError = _T("TTS/SR/VCMD/VTXTERR_OUTOFDISK"); break;
		case TTSERR_INVALIDWINDOW:				lError = _T("TTS/SR/VCMD/VTXTERR_INVALIDWINDOW"); break;
		case TTSERR_INVALIDMODE:				lError = _T("TTS/SR/VCMDERR_INVALIDMODE"); break;
		case TTSERR_INVALIDKEY:					lError = _T("TTS/SRERR_INVALIDKEY"); break;
		case TTSERR_INVALIDCHAR:				lError = _T("TTS/SR/VTXTERR_INVALIDCHAR"); break;
		case TTSERR_QUEUEFULL:					lError = _T("TTS/VTXTERR_QUEUEFULL"); break;
		case TTSERR_NOTPAUSED:					lError = _T("TTSERR_NOTPAUSED"); break;
		case TTSERR_ALREADYPAUSED:				lError = _T("TTSERR_ALREADYPAUSED"); break;
		case SRERR_GRAMMARTOOCOMPLEX:			lError = _T("SRERR_GRAMMARTOOCOMPLEX"); break;
		case SRERR_GRAMMARWRONGTYPE:			lError = _T("SRERR_GRAMMARWRONGTYPE"); break;
		case SRERR_TOOMANYGRAMMARS:				lError = _T("SRERR_TOOMANYGRAMMARS"); break;
		case SRERR_INVALIDLIST:					lError = _T("SR/VCMDERR_INVALIDLIST"); break;
		case SRERR_GRAMTOOLARGE:				lError = _T("SRERR_GRAMTOOLARGE"); break;
		case SRERR_GRAMMARERROR:				lError = _T("SRERR_GRAMMARERROR"); break;
		case SRERR_INVALIDRULE:					lError = _T("SRERR_INVALIDRULE"); break;
		case SRERR_RULEALREADYACTIVE:			lError = _T("SRERR_RULEALREADYACTIVE"); break;
		case SRERR_RULENOTACTIVE:				lError = _T("SRERR_RULENOTACTIVE"); break;
		case SRERR_NOUSERSELECTED:				lError = _T("SRERR_NOUSERSELECTED"); break;
		case SRERR_BAD_PRONUNCIATION:			lError = _T("SRERR_BAD_PRONUNCIATION"); break;
		case SRERR_DATAFILEERROR:				lError = _T("SRERR_DATAFILEERROR"); break;
		case SRERR_GRAMMARALREADYACTIVE:		lError = _T("SRERR_GRAMMARALREADYACTIVE"); break;
		case SRERR_GRAMMARNOTACTIVE:			lError = _T("SRERR_GRAMMARNOTACTIVE"); break;
		case SRERR_GLOBALGRAMMARALREADYACTIVE:	lError = _T("SRERR_GLOBALGRAMMARALREADYACTIVE"); break;
		case SRERR_LANGUAGEMISMATCH:			lError = _T("SRERR_LANGUAGEMISMATCH"); break;
		case SRERR_MULTIPLELANG:				lError = _T("SRERR_MULTIPLELANG"); break;
		case SRERR_LDGRAMMARNOWORDS:			lError = _T("SRERR_LDGRAMMARNOWORDS"); break;
		case SRERR_NOLEXICON:					lError = _T("SRERR_NOLEXICON"); break;
		case SRERR_SPEAKEREXISTS:				lError = _T("SRERR_SPEAKEREXISTS"); break;
		case SRERR_GRAMMARENGINEMISMATCH:		lError = _T("SRERR_GRAMMARENGINEMISMATCH"); break;
		case SRERR_BOOKMARKEXISTS:				lError = _T("SRERR_BOOKMARKEXISTS"); break;
		case SRERR_BOOKMARKDOESNOTEXIST:		lError = _T("SRERR_BOOKMARKDOESNOTEXIST"); break;
		case SRERR_MICWIZARDCANCELED:			lError = _T("SRERR_MICWIZARDCANCELED"); break;
		case SRERR_WORDTOOLONG:					lError = _T("SRERR_WORDTOOLONG"); break;
		case SRERR_BAD_WORD:					lError = _T("SRERR_BAD_WORD"); break;
		case VCMDERR_MENUTOOCOMPLEX:			lError = _T("VCMDERR_MENUTOOCOMPLEX"); break;
		case VCMDERR_MENUWRONGLANGUAGE:			lError = _T("VCMDERR_MENUWRONGLANGUAGE"); break;
		case VCMDERR_TOOMANYMENUS:				lError = _T("VCMDERR_TOOMANYMENUS"); break;
		case VCMDERR_MENUDOESNOTEXIST:			lError = _T("VCMDERR_MENUDOESNOTEXIST"); break;
		case VCMDERR_MENUACTIVE:				lError = _T("VCMDERR_MENUACTIVE"); break;
		case VCMDERR_NOENGINE:					lError = _T("VCMDERR_NOENGINE"); break;
		case VCMDERR_NOGRAMMARINTERFACE:		lError = _T("VCMDERR_NOGRAMMARINTERFACE"); break;
		case VCMDERR_NOFINDINTERFACE:			lError = _T("VCMDERR_NOFINDINTERFACE"); break;
		case VCMDERR_CANTCREATESRENUM:			lError = _T("VCMDERR_CANTCREATESRENUM"); break;
		case VCMDERR_NOSITEINFO:				lError = _T("VCMDERR_NOSITEINFO"); break;
		case VCMDERR_SRFINDFAILED:				lError = _T("VCMDERR_SRFINDFAILED"); break;
		case VCMDERR_CANTCREATEAUDIODEVICE:		lError = _T("VCMDERR_CANTCREATEAUDIODEVICE"); break;
		case VCMDERR_CANTSETDEVICE:				lError = _T("VCMDERR_CANTSETDEVICE"); break;
		case VCMDERR_CANTSELECTENGINE:			lError = _T("VCMDERR_CANTSELECTENGINE"); break;
		case VCMDERR_CANTCREATENOTIFY:			lError = _T("VCMDERR_CANTCREATENOTIFY"); break;
		case VCMDERR_CANTCREATEDATASTRUCTURES:	lError = _T("VCMDERR_CANTCREATEDATASTRUCTURES"); break;
		case VCMDERR_CANTINITDATASTRUCTURES:	lError = _T("VCMDERR_CANTINITDATASTRUCTURES"); break;
		case VCMDERR_NOCACHEDATA:				lError = _T("VCMDERR_NOCACHEDATA"); break;
		case VCMDERR_NOCOMMANDS:				lError = _T("VCMDERR_NOCOMMANDS"); break;
		case VCMDERR_CANTXTRACTWORDS:			lError = _T("VCMDERR_CANTXTRACTWORDS"); break;
		case VCMDERR_CANTGETDBNAME:				lError = _T("VCMDERR_CANTGETDBNAME"); break;
		case VCMDERR_CANTCREATEDBNAME:			lError = _T("VCMDERR_CANTCREATEDBNAME"); break;
		case VCMDERR_CANTUPDATEREGISTRY:		lError = _T("VCMDERR_CANTUPDATEREGISTRY"); break;
		case VCMDERR_CANTOPENREGISTRY:			lError = _T("VCMDERR_CANTOPENREGISTRY"); break;
		case VCMDERR_CANTOPENDATABASE:			lError = _T("VCMDERR_CANTOPENDATABASE"); break;
		case VCMDERR_CANTCREATESTORAGE:			lError = _T("VCMDERR_CANTCREATESTORAGE"); break;
		case VCMDERR_CANNOTMIMIC:				lError = _T("VCMDERR_CANNOTMIMIC"); break;
		case VCMDERR_MENUEXIST:					lError = _T("VCMDERR_MENUEXIST"); break;
		case VCMDERR_MENUOPEN:					lError = _T("VCMDERR_MENUOPEN"); break;
		case VTXTERR_EMPTYSPEAKSTRING:			lError = _T("VTXTERR_EMPTYSPEAKSTRING"); break;
		case VTXTERR_NOFINDINTERFACE:			lError = _T("VTXTERR_NOFINDINTERFACE"); break;
		case VTXTERR_CANTCREATETTSENUM:			lError = _T("VTXTERR_CANTCREATETTSENUM"); break;
		case VTXTERR_NOSITEINFO:				lError = _T("VTXTERR_NOSITEINFO"); break;
		case VTXTERR_TTSFINDFAILED:				lError = _T("VTXTERR_TTSFINDFAILED"); break;
		case VTXTERR_CANTSETDEVICE:				lError = _T("VTXTERR_CANTSETDEVICE"); break;
		case VTXTERR_CANTSELECTENGINE:			lError = _T("VTXTERR_CANTSELECTENGINE"); break;
		case VTXTERR_CANTCREATENOTIFY:			lError = _T("VTXTERR_CANTCREATENOTIFY"); break;
		case VTXTERR_NOTENABLED:				lError = _T("VTXTERR_NOTENABLED"); break;
		case LEXERR_INVALIDTEXTCHAR:			lError = _T("LEXERR_INVALIDTEXTCHAR"); break;
		case LEXERR_INVALIDSENSE:				lError = _T("LEXERR_INVALIDSENSE"); break;
		case LEXERR_NOTINLEX:					lError = _T("LEXERR_NOTINLEX"); break;
		case LEXERR_OUTOFDISK:					lError = _T("LEXERR_OUTOFDISK"); break;
		case LEXERR_INVALIDPRONCHAR:			lError = _T("LEXERR_INVALIDPRONCHAR"); break;
		case LEXERR_ALREADYINLEX:				lError = _T("LEXERR_ALREADYINLEX"); break;
		case LEXERR_PRNBUFTOOSMALL:				lError = _T("LEXERR_PRNBUFTOOSMALL"); break;
		case LEXERR_ENGBUFTOOSMALL:				lError = _T("LEXERR_ENGBUFTOOSMALL"); break;
		case LEXERR_INVALIDLEX:					lError = _T("LEXERR_INVALIDLEX"); break;
	}

	return lError;
}

////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
HRESULT LogSapi4Err (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
#else
extern "C" void _LogPrepFileLine (LPTSTR pString, LPCSTR pFile, UINT pLine);
HRESULT _LogSapi4ErrFL::LogErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
#endif
{
	if	(
			(
				(FAILED (pError))
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

			if	(pError == S_OK)
			{
				lError = _T("S_OK");
			}
			else
			if	(pError == S_FALSE)
			{
				lError = _T("S_FALSE");
			}
			else
			{
				lError = _GetSapi4Err (pError);
			}

			if	(lError)
			{
				_tcscpy (lFormat, _T("Sapi4Error [%8.8X] [%s]"));
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
					LogComErr (MinLogLevel (pLogLevel, LogAlways), pError, lFormat, lExtra);
#else
					_LogComErrFL(mFile,mLine).LogErr (MinLogLevel (pLogLevel, LogAlways), pError, lFormat, lExtra);
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
