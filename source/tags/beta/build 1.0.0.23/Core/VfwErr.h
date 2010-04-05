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
#ifndef _VFWERR_H
#define _VFWERR_H
////////////////////////////////////////////////////////////////////////
#include <vfwmsgs.h>
#include "Log.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#ifdef	_LOG_DISABLED
static inline LPCTSTR _GetVfwErr (HRESULT pError) {return _T("");}
static inline HRESULT LogVfwErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...) {return pError;}
#define LogVfwErrAnon LogVfwErr
#else
extern LPCTSTR _GetVfwErr (HRESULT pError);
#ifdef	_LOG_NOFILELINE
extern HRESULT LogVfwErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...);
#define LogVfwErrAnon LogVfwErr
#else
DEFINE_LogErrFL(Vfw,HRESULT)
#define	LogVfwErr _LogVfwErrFL(__FILE__,__LINE__).LogErr
#define	LogVfwErrAnon _LogVfwErrFL(NULL,0).LogErr
#endif
#endif
#else	// _DEBUG
#define	LogVfwErr LogComErr
#define LogVfwErrAnon LogComErrAnon
#endif
////////////////////////////////////////////////////////////////////////
#endif	// _VFWERR_H
