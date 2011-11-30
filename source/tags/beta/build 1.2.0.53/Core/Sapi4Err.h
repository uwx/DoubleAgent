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
#ifndef _SAPI4ERR_H
#define _SAPI4ERR_H
////////////////////////////////////////////////////////////////////////
#include "Log.h"
#pragma warning (push)
#pragma warning (disable: 4005)
#include "speech.h"
#pragma warning (pop)
////////////////////////////////////////////////////////////////////////
EXTERN_C LPCTSTR _GetSapi4Err (HRESULT pError);
#if		defined(_DEBUG) || defined(_SAPI4_LOGGING)
#ifdef	_LOG_DISABLED
static inline LPCTSTR _GetSapi4Err (HRESULT pError) {return _T("");}
static inline HRESULT LogSapi4Err (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...) {return pError;}
#define LogSapi4ErrAnon LogSapi4Err
#else
#ifdef	_LOG_NOFILELINE
extern HRESULT LogSapi4Err (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...);
#define LogSapi4ErrAnon LogSapi4Err
#else
DEFINE_LogErrFL(Sapi4,HRESULT)
#define	LogSapi4Err _LogSapi4ErrFL(__FILE__,__LINE__).LogErr
#define	LogSapi4ErrAnon _LogSapi4ErrFL(NULL,0).LogErr
#endif
#endif
#else	// _DEBUG
#define	LogSapi4Err LogComErr
#define LogSapi4ErrAnon LogComErrAnon
#endif
////////////////////////////////////////////////////////////////////////
#endif	// _Sapi4ERR_H
