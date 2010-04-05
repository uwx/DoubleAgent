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
#ifndef _MMSYSERROR_H
#define _MMSYSERROR_H
////////////////////////////////////////////////////////////////////////
#include <MmSystem.h>
#include "Log.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#ifdef	_LOG_DISABLED
static inline LPCTSTR _GetMmSysErr (MMRESULT pError) {return _T("");}
static inline MMRESULT LogMmSysErr (unsigned int pLogLevel, MMRESULT pError, LPCTSTR pFormat = NULL, ...) {return pError;}
#define LogMmSysErrAnon LogMmSysErr
#else
extern LPCTSTR _GetMmSysErr (MMRESULT pError);
#ifdef	_LOG_NOFILELINE
extern MMRESULT LogMmSysErr (unsigned int pLogLevel, MMRESULT pError, LPCTSTR pFormat = NULL, ...);
#define LogMmSysErrAnon LogMmSysErr
#else
DEFINE_LogErrFL(MmSys,MMRESULT)
#define	LogMmSysErr _LogMmSysErrFL(__FILE__,__LINE__).LogErr
#define	LogMmSysErrAnon _LogMmSysErrFL(NULL,0).LogErr
#endif
#endif
#else	// _DEBUG
#define	LogMmSysErr LogWinErr
#define LogMmSysErrAnon LogWinErrAnon
#endif
////////////////////////////////////////////////////////////////////////
#endif	// _MMSYSERROR_H
