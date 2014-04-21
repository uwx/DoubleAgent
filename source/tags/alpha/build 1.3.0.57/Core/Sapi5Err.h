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
#ifndef _SAPI5ERR_H
#define _SAPI5ERR_H
////////////////////////////////////////////////////////////////////////
#include "Log.h"
#include <sperror.h>
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#ifdef	_LOG_DISABLED
static inline LPCTSTR _GetSapi5Err (HRESULT pError) {return _T("");}
static inline HRESULT LogSapi5Err (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...) {return pError;}
#define LogSapi5ErrAnon LogSapi5Err
#else
extern LPCTSTR _GetSapi5Err (HRESULT pError);
#ifdef	_LOG_NOFILELINE
extern HRESULT LogSapi5Err (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...);
#define LogSapi5ErrAnon LogSapi5Err
#else
DEFINE_LogErrFL(Sapi5,HRESULT)
#define	LogSapi5Err _LogSapi5ErrFL(__FILE__,__LINE__).LogErr
#define	LogSapi5ErrAnon _LogSapi5ErrFL(NULL,0).LogErr
#endif
#endif
#else	// _DEBUG
#define	LogSapi5Err LogComErr
#define LogSapi5ErrAnon LogComErrAnon
#endif
////////////////////////////////////////////////////////////////////////
#endif	// _SAPI5ERR_H
