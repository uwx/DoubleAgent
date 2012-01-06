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
#include "StdAfx.h"
#include "Sapi5Err.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
////////////////////////////////////////////////////////////////////////

LPCTSTR _GetSapi5Err (HRESULT pError)
{
	LPCTSTR	lError = NULL;

	switch (pError)
	{
		case SPERR_UNINITIALIZED:						lError = _T("SPERR_UNINITIALIZED"); break;
		case SPERR_ALREADY_INITIALIZED:					lError = _T("SPERR_ALREADY_INITIALIZED"); break;
		case SPERR_UNSUPPORTED_FORMAT:					lError = _T("SPERR_UNSUPPORTED_FORMAT"); break;
		case SPERR_INVALID_FLAGS:						lError = _T("SPERR_INVALID_FLAGS"); break;
		case SPERR_DEVICE_BUSY:							lError = _T("SPERR_DEVICE_BUSY"); break;
		case SPERR_DEVICE_NOT_SUPPORTED:				lError = _T("SPERR_DEVICE_NOT_SUPPORTED"); break;
		case SPERR_DEVICE_NOT_ENABLED:					lError = _T("SPERR_DEVICE_NOT_ENABLED"); break;
		case SPERR_NO_DRIVER:							lError = _T("SPERR_NO_DRIVER"); break;
		case SPERR_FILE_MUST_BE_UNICODE:				lError = _T("SPERR_FILE_MUST_BE_UNICODE"); break;
		case SPERR_INVALID_PHRASE_ID:					lError = _T("SPERR_INVALID_PHRASE_ID"); break;
		case SPERR_BUFFER_TOO_SMALL:					lError = _T("SPERR_BUFFER_TOO_SMALL"); break;
		case SPERR_FORMAT_NOT_SPECIFIED:				lError = _T("SPERR_FORMAT_NOT_SPECIFIED"); break;
		case SPERR_AUDIO_STOPPED:						lError = _T("SPERR_AUDIO_STOPPED"); break;
		case SPERR_RULE_NOT_FOUND:						lError = _T("SPERR_RULE_NOT_FOUND"); break;
		case SPERR_TTS_ENGINE_EXCEPTION:				lError = _T("SPERR_TTS_ENGINE_EXCEPTION"); break;
		case SPERR_TTS_NLP_EXCEPTION:					lError = _T("SPERR_TTS_NLP_EXCEPTION"); break;
		case SPERR_ENGINE_BUSY:							lError = _T("SPERR_ENGINE_BUSY"); break;
		case SPERR_CANT_CREATE:							lError = _T("SPERR_CANT_CREATE"); break;
		case SPERR_NOT_IN_LEX:							lError = _T("SPERR_NOT_IN_LEX"); break;
		case SPERR_LEX_VERY_OUT_OF_SYNC:				lError = _T("SPERR_LEX_VERY_OUT_OF_SYNC"); break;
		case SPERR_UNDEFINED_FORWARD_RULE_REF:			lError = _T("SPERR_UNDEFINED_FORWARD_RULE_REF"); break;
		case SPERR_EMPTY_RULE:							lError = _T("SPERR_EMPTY_RULE"); break;
		case SPERR_GRAMMAR_COMPILER_INTERNAL_ERROR:		lError = _T("SPERR_GRAMMAR_COMPILER_INTERNAL_ERROR"); break;
		case SPERR_RULE_NOT_DYNAMIC:					lError = _T("SPERR_RULE_NOT_DYNAMIC"); break;
		case SPERR_DUPLICATE_RULE_NAME:					lError = _T("SPERR_DUPLICATE_RULE_NAME"); break;
		case SPERR_DUPLICATE_RESOURCE_NAME:				lError = _T("SPERR_DUPLICATE_RESOURCE_NAME"); break;
		case SPERR_TOO_MANY_GRAMMARS:					lError = _T("SPERR_TOO_MANY_GRAMMARS"); break;
		case SPERR_CIRCULAR_REFERENCE:					lError = _T("SPERR_CIRCULAR_REFERENCE"); break;
		case SPERR_INVALID_IMPORT:						lError = _T("SPERR_INVALID_IMPORT"); break;
		case SPERR_INVALID_WAV_FILE:					lError = _T("SPERR_INVALID_WAV_FILE"); break;
		case SPERR_ALL_WORDS_OPTIONAL:					lError = _T("SPERR_ALL_WORDS_OPTIONAL"); break;
		case SPERR_INSTANCE_CHANGE_INVALID:				lError = _T("SPERR_INSTANCE_CHANGE_INVALID"); break;
		case SPERR_RULE_NAME_ID_CONFLICT:				lError = _T("SPERR_RULE_NAME_ID_CONFLICT"); break;
		case SPERR_NO_RULES:							lError = _T("SPERR_NO_RULES"); break;
		case SPERR_CIRCULAR_RULE_REF:					lError = _T("SPERR_CIRCULAR_RULE_REF"); break;
		case SPERR_INVALID_HANDLE:						lError = _T("SPERR_INVALID_HANDLE"); break;
		case SPERR_REMOTE_CALL_TIMED_OUT:				lError = _T("SPERR_REMOTE_CALL_TIMED_OUT"); break;
		case SPERR_AUDIO_BUFFER_OVERFLOW:				lError = _T("SPERR_AUDIO_BUFFER_OVERFLOW"); break;
		case SPERR_NO_AUDIO_DATA:						lError = _T("SPERR_NO_AUDIO_DATA"); break;
		case SPERR_DEAD_ALTERNATE:						lError = _T("SPERR_DEAD_ALTERNATE"); break;
		case SPERR_HIGH_LOW_CONFIDENCE:					lError = _T("SPERR_HIGH_LOW_CONFIDENCE"); break;
		case SPERR_INVALID_FORMAT_STRING:				lError = _T("SPERR_INVALID_FORMAT_STRING"); break;
		case SPERR_APPLEX_READ_ONLY:					lError = _T("SPERR_APPLEX_READ_ONLY"); break;
		case SPERR_NO_TERMINATING_RULE_PATH:			lError = _T("SPERR_NO_TERMINATING_RULE_PATH"); break;
		case SPERR_STREAM_CLOSED:						lError = _T("SPERR_STREAM_CLOSED"); break;
		case SPERR_NO_MORE_ITEMS:						lError = _T("SPERR_NO_MORE_ITEMS"); break;
		case SPERR_NOT_FOUND:							lError = _T("SPERR_NOT_FOUND"); break;
		case SPERR_INVALID_AUDIO_STATE:					lError = _T("SPERR_INVALID_AUDIO_STATE"); break;
		case SPERR_GENERIC_MMSYS_ERROR:					lError = _T("SPERR_GENERIC_MMSYS_ERROR"); break;
		case SPERR_MARSHALER_EXCEPTION:					lError = _T("SPERR_MARSHALER_EXCEPTION"); break;
		case SPERR_NOT_DYNAMIC_GRAMMAR:					lError = _T("SPERR_NOT_DYNAMIC_GRAMMAR"); break;
		case SPERR_AMBIGUOUS_PROPERTY:					lError = _T("SPERR_AMBIGUOUS_PROPERTY"); break;
		case SPERR_INVALID_REGISTRY_KEY:				lError = _T("SPERR_INVALID_REGISTRY_KEY"); break;
		case SPERR_INVALID_TOKEN_ID:					lError = _T("SPERR_INVALID_TOKEN_ID"); break;
		case SPERR_XML_BAD_SYNTAX:						lError = _T("SPERR_XML_BAD_SYNTAX"); break;
		case SPERR_XML_RESOURCE_NOT_FOUND:				lError = _T("SPERR_XML_RESOURCE_NOT_FOUND"); break;
		case SPERR_TOKEN_IN_USE:						lError = _T("SPERR_TOKEN_IN_USE"); break;
		case SPERR_TOKEN_DELETED:						lError = _T("SPERR_TOKEN_DELETED"); break;
		case SPERR_MULTI_LINGUAL_NOT_SUPPORTED:			lError = _T("SPERR_MULTI_LINGUAL_NOT_SUPPORTED"); break;
		case SPERR_EXPORT_DYNAMIC_RULE:					lError = _T("SPERR_EXPORT_DYNAMIC_RULE"); break;
		case SPERR_STGF_ERROR:							lError = _T("SPERR_STGF_ERROR"); break;
		case SPERR_WORDFORMAT_ERROR:					lError = _T("SPERR_WORDFORMAT_ERROR"); break;
		case SPERR_STREAM_NOT_ACTIVE:					lError = _T("SPERR_STREAM_NOT_ACTIVE"); break;
		case SPERR_ENGINE_RESPONSE_INVALID:				lError = _T("SPERR_ENGINE_RESPONSE_INVALID"); break;
		case SPERR_SR_ENGINE_EXCEPTION:					lError = _T("SPERR_SR_ENGINE_EXCEPTION"); break;
		case SPERR_STREAM_POS_INVALID:					lError = _T("SPERR_STREAM_POS_INVALID"); break;
		case SPERR_REMOTE_CALL_ON_WRONG_THREAD:			lError = _T("SPERR_REMOTE_CALL_ON_WRONG_THREAD"); break;
		case SPERR_REMOTE_PROCESS_TERMINATED:			lError = _T("SPERR_REMOTE_PROCESS_TERMINATED"); break;
		case SPERR_REMOTE_PROCESS_ALREADY_RUNNING:		lError = _T("SPERR_REMOTE_PROCESS_ALREADY_RUNNING"); break;
		case SPERR_LANGID_MISMATCH:						lError = _T("SPERR_LANGID_MISMATCH"); break;
		case SPERR_NOT_TOPLEVEL_RULE:					lError = _T("SPERR_NOT_TOPLEVEL_RULE"); break;
		case SPERR_LEX_REQUIRES_COOKIE:					lError = _T("SPERR_LEX_REQUIRES_COOKIE"); break;
		case SPERR_UNSUPPORTED_LANG:					lError = _T("SPERR_UNSUPPORTED_LANG"); break;
		case SPERR_VOICE_PAUSED:						lError = _T("SPERR_VOICE_PAUSED"); break;
		case SPERR_AUDIO_BUFFER_UNDERFLOW:				lError = _T("SPERR_AUDIO_BUFFER_UNDERFLOW"); break;
		case SPERR_AUDIO_STOPPED_UNEXPECTEDLY:			lError = _T("SPERR_AUDIO_STOPPED_UNEXPECTEDLY"); break;
		case SPERR_NO_WORD_PRONUNCIATION:				lError = _T("SPERR_NO_WORD_PRONUNCIATION"); break;
		case SPERR_ALTERNATES_WOULD_BE_INCONSISTENT:	lError = _T("SPERR_ALTERNATES_WOULD_BE_INCONSISTENT"); break;
		case SPERR_NOT_SUPPORTED_FOR_SHARED_RECOGNIZER:	lError = _T("SPERR_NOT_SUPPORTED_FOR_SHARED_RECOGNIZER"); break;
		case SPERR_TIMEOUT:								lError = _T("SPERR_TIMEOUT"); break;
		case SPERR_REENTER_SYNCHRONIZE:					lError = _T("SPERR_REENTER_SYNCHRONIZE"); break;
		case SPERR_STATE_WITH_NO_ARCS:					lError = _T("SPERR_STATE_WITH_NO_ARCS"); break;
		case SPERR_NOT_ACTIVE_SESSION:					lError = _T("SPERR_NOT_ACTIVE_SESSION"); break;
		case SPERR_ALREADY_DELETED:						lError = _T("SPERR_ALREADY_DELETED"); break;
		case SP_INSUFFICIENT_DATA:						lError = _T("SP_INSUFFICIENT_DATA"); break;
		case SP_END_OF_STREAM:							lError = _T("SP_END_OF_STREAM"); break;
		case SP_AUDIO_PAUSED:							lError = _T("SP_AUDIO_PAUSED"); break;
		case SP_AUDIO_CONVERSION_ENABLED:				lError = _T("SP_AUDIO_CONVERSION_ENABLED"); break;
		case SP_NO_HYPOTHESIS_AVAILABLE:				lError = _T("SP_NO_HYPOTHESIS_AVAILABLE"); break;
		case SP_ALREADY_IN_LEX:							lError = _T("SP_ALREADY_IN_LEX"); break;
		case SP_LEX_NOTHING_TO_SYNC:					lError = _T("SP_LEX_NOTHING_TO_SYNC"); break;
		case SP_REQUEST_PENDING:						lError = _T("SP_REQUEST_PENDING"); break;
		case SP_NO_PARSE_FOUND:							lError = _T("SP_NO_PARSE_FOUND"); break;
		case SP_UNSUPPORTED_ON_STREAM_INPUT:			lError = _T("SP_UNSUPPORTED_ON_STREAM_INPUT"); break;
		case SP_WORD_EXISTS_WITHOUT_PRONUNCIATION:		lError = _T("SP_WORD_EXISTS_WITHOUT_PRONUNCIATION"); break;
		case SP_RECOGNIZER_INACTIVE:					lError = _T("SP_RECOGNIZER_INACTIVE"); break;
		case SP_PARTIAL_PARSE_FOUND:					lError = _T("SP_PARTIAL_PARSE_FOUND"); break;
		case SP_NO_RULE_ACTIVE:							lError = _T("SP_NO_RULE_ACTIVE"); break;
		case SP_STREAM_UNINITIALIZED:					lError = _T("SP_STREAM_UNINITIALIZED"); break;
		case SP_AUDIO_STOPPED:							lError = _T("SP_AUDIO_STOPPED"); break;
	}

	return lError;
}

////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
HRESULT LogSapi5Err (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
#else
extern "C" void _LogPrepFileLine (LPTSTR pString, LPCSTR pFile, UINT pLine);
HRESULT _LogSapi5ErrFL::LogErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
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
				lError = _GetSapi5Err (pError);
			}

			if	(lError)
			{
				_tcscpy (lFormat, _T("Sapi5Error [%8.8X] [%s]"));
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
