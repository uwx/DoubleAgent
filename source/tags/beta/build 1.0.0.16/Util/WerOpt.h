/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009 Cinnamon Software Inc.
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
#ifndef _WEROPT_H
#define _WEROPT_H
////////////////////////////////////////////////////////////////////////
#include "ExceptionMacros.h"
#include "HelperTemplates.h"
////////////////////////////////////////////////////////////////////////
typedef HRESULT (WINAPI * tWerAddExcludedApplication) (PCWSTR, BOOL);
typedef HRESULT (WINAPI * tWerRemoveExcludedApplication) (PCWSTR, BOOL);
////////////////////////////////////////////////////////////////////////
static inline bool WerOptOut (LPCTSTR pFileName, bool pAllUsers)
{
	bool						lRet = false;
	CModuleHandle				lWerModule;
	tWerAddExcludedApplication	lAddExcluded;
	CString						lModuleName;
	UINT						lErrMode = SetErrorMode (SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX);

	try
	{	
		lWerModule = ::LoadLibrary (_T("WER.DLL"));
	}
	catch AnyExceptionSilent

	SetErrorMode (lErrMode);
	
	if	(
			(lWerModule.SafeIsValid ())
		&&	(lAddExcluded = (tWerAddExcludedApplication) ::GetProcAddress (lWerModule, "WerAddExcludedApplication"))
		)
	{
		try
		{
			if	(SUCCEEDED ((*lAddExcluded) (pFileName, (pAllUsers!=false))))
			{
				lRet = true;
			} 
		}
		catch AnyExceptionSilent
	}
	return lRet;
}
////////////////////////////////////////////////////////////////////////
static inline bool WerOptIn (LPCTSTR pFileName, bool pAllUsers)
{
	bool							lRet = false;
	CModuleHandle					lWerModule;
	tWerRemoveExcludedApplication	lRemoveExcluded;
	CString							lModuleName;
	UINT							lErrMode = SetErrorMode (SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX);

	try
	{	
		lWerModule = ::LoadLibrary (_T("WER.DLL"));
	}
	catch AnyExceptionSilent

	SetErrorMode (lErrMode);
	
	if	(
			(lWerModule.SafeIsValid ())
		&&	(lRemoveExcluded = (tWerRemoveExcludedApplication) ::GetProcAddress (lWerModule, "WerRemoveExcludedApplication"))
		)
	{
		try
		{
			if	(SUCCEEDED ((*lRemoveExcluded) (pFileName, (pAllUsers!=false))))
			{
				lRet = true;
			} 
		}
		catch AnyExceptionSilent
	}
	return lRet;
}
////////////////////////////////////////////////////////////////////////
static inline bool WerOptOut (bool pAllUsers)
{
	CString	lFileName;
	
	GetModuleFileName (NULL, lFileName.GetBuffer (MAX_PATH), MAX_PATH);
	lFileName.ReleaseBuffer ();
	
	return WerOptOut (lFileName, pAllUsers);
}
////////////////////////////////////////////////////////////////////////
static inline bool WerOptIn (bool pAllUsers)
{
	CString	lFileName;
	
	GetModuleFileName (NULL, lFileName.GetBuffer (MAX_PATH), MAX_PATH);
	lFileName.ReleaseBuffer ();
	
	return WerOptIn (lFileName, pAllUsers);
}
////////////////////////////////////////////////////////////////////////
#endif
