/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef	_EXCEPTIONMACROS_H
#define	_EXCEPTIONMACROS_H
////////////////////////////////////////////////////////////////////////

#if defined (_LOG_H) && !defined (_LOG_DISABLED)
#define	__LogAnyException(lvl) try {LogMessage (lvl, _T("*** Exception *** at %hs %d"), __FILE__, __LINE__);} catch (...) {}
#else
#define	__LogAnyException(lvl)
#endif

#ifdef	__AFX_H__
#if defined (_LOG_H) && !defined (_LOG_DISABLED)
#define	__LogAfxException(lvl) LogMfcException (lvl, pException, __FILE__, __LINE__);
#else
#define	__LogAfxException(lvl)
#endif

#define	__CatchAfxExceptionL(lvl) \
(CException * pException) \
{ \
	__LogAfxException (lvl) \
	try {pException->Delete ();} catch (...) {} \
} \
catch

#define	__CatchAfxExceptionR(lvl) \
(CException * pException) \
{ \
	__LogAfxException (lvl) \
	try {pException->ReportError ();} catch (...) {} \
	try {pException->Delete ();} catch (...) {} \
} \
catch

#define	__CatchAfxExceptionN \
(CException * pException) \
{ \
	try {pException->Delete ();} catch (...) {} \
} \
catch

#else
#define	__CatchAfxExceptionL(lvl)
#define	__CatchAfxExceptionR(lvl)
#define	__CatchAfxExceptionN
#endif	// __AFX_H__

#ifdef	__ATLEXCEPT_H__
#if defined (_LOG_H) && !defined (_LOG_DISABLED)
#define	__LogAtlException(lvl) LogAtlException (lvl, pException, __FILE__, __LINE__);
#else
#define	__LogAtlException(lvl) (pException);
#endif

#define	__CatchAtlException(lvl) \
(CAtlException pException) \
{ \
	__LogAtlException (lvl) \
} \
catch

#else
#define	__CatchAtlException(lvl)
#endif	// __ATLEXCEPT_H__

#ifdef	__cplusplus_cli
#if defined (_LOG_H) && !defined (_LOG_DISABLED)
#define	__LogCliException(lvl) LogCliException (lvl, pException, __FILE__, __LINE__);
#else
#define	__LogCliException(lvl) pException;
#endif

#define	__CatchCliException(lvl) \
(System::Exception^ pException) \
{ \
	__LogCliException (lvl) \
} \
catch

#else
#define	__CatchCliException(lvl)
#endif	// __cplusplus_cli

#define AnyException \
__CatchAfxExceptionR (LogNormal|LogTime) \
__CatchAtlException (LogNormal|LogTime) \
__CatchCliException (LogNormal|LogTime) \
(...) {__LogAnyException (LogIfActive|LogTime)}

#ifdef	_DEBUG
#define AnyExceptionSilent \
__CatchAfxExceptionL (LogDetails|LogTime) \
__CatchAtlException (LogDetails|LogTime) \
__CatchCliException (LogDetails|LogTime) \
(...) {}
#else
#define AnyExceptionSilent \
(...) {}
#endif

#ifdef	_DEBUG
#define AnyExceptionDebug \
__CatchAfxExceptionL (LogNormal) \
__CatchAtlException (LogNormal) \
__CatchCliException (LogNormal) \
(...) {__LogAnyException (LogIfActive)}
#else
#define AnyExceptionDebug \
__CatchAfxExceptionL (LogIfActive) \
__CatchAtlException (LogIfActive) \
__CatchCliException (LogIfActive) \
(...) {__LogAnyException (LogIfActive)}
#endif

////////////////////////////////////////////////////////////////////////
#endif // _EXCEPTIONMACROS_H
