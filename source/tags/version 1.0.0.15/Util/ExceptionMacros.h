/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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

#ifdef	__AFX_H__

#if defined (_LOG_H) && !defined (_LOG_DISABLED)
#define AnyException \
(CException * pException) \
{ \
	try \
	{ \
		LogException (LogNormal, pException, __FILE__, __LINE__); \
		pException->ReportError (); \
		pException->Delete (); \
	} \
	catch (...) \
	{} \
} \
catch (...) \
{ \
	try \
	{ \
		LogMessage (LogIfActive, _T("*** Exception *** at %hs %d"), __FILE__, __LINE__); \
	} \
	catch (...) \
	{} \
}
#else
#define AnyException \
(CException * pException) \
{ \
	try \
	{ \
		LogException (LogNormal, pException, __FILE__, __LINE__); \
		pException->ReportError (); \
	} \
	catch (...) \
	{} \
	try \
	{ \
		pException->Delete (); \
	} \
	catch (...) \
	{} \
} \
catch (...) \
{}
#endif

#ifdef	_DEBUG
#define AnyExceptionSilent \
(CException * pException) \
{ \
	try \
	{ \
		LogException (LogDetails, pException, __FILE__, __LINE__); \
		pException->Delete (); \
	} \
	catch (...) \
	{} \
} \
catch (...) \
{}
#else
#define AnyExceptionSilent \
(CException * pException) \
{ \
	try \
	{ \
		pException->Delete (); \
	} \
	catch (...) \
	{} \
} \
catch (...) \
{}
#endif

#ifdef	_DEBUG
#define AnyExceptionDebug \
(CException * pException) \
{ \
	try \
	{ \
		LogException (LogNormal, pException, __FILE__, __LINE__); \
	} \
	catch (...) \
	{} \
	try \
	{ \
		pException->Delete (); \
	} \
	catch (...) \
	{} \
} \
catch (...) \
{ \
	try \
	{ \
		LogMessage (LogIfActive, _T("*** Exception *** at %hs %d"), __FILE__, __LINE__); \
	} \
	catch (...) \
	{} \
}
#else
#if defined (_LOG_H) && !defined (_LOG_DISABLED)
#define AnyExceptionDebug \
(CException * pException) \
{ \
	try \
	{ \
		LogException (LogIfActive, pException, __FILE__, __LINE__); \
	} \
	catch (...) \
	{} \
	try \
	{ \
		pException->Delete (); \
	} \
	catch (...) \
	{} \
} \
catch (...) \
{ \
	try \
	{ \
		LogMessage (LogIfActive, _T("*** Exception *** at %hs %d"), __FILE__, __LINE__); \
	} \
	catch (...) \
	{} \
}
#else
#define AnyExceptionDebug AnyExceptionSilent
#endif
#endif

#else // __AFX_H__

#define AnyException (...) {}

#define AnyExceptionSilent (...) {}

#define AnyExceptionDebug (...) {}

#endif // __AFX_H__

////////////////////////////////////////////////////////////////////////
#endif // _EXCEPTIONMACROS_H
