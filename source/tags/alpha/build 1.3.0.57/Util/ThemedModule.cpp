/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include <uxtheme.h>
#include "ThemedModule.h"
#include "Log.h"

#pragma comment(lib, "uxtheme.lib")

#ifdef	_DEBUG
//#define	_DEBUG_CONTEXT	LogNormal
#endif

//////////////////////////////////////////////////////////////////////

CThemedModule::CThemedModule ()
:	mActCtxHandle (0)
{
}

CThemedModule::~CThemedModule ()
{
	EndModuleTheme ();
}

//////////////////////////////////////////////////////////////////////

bool CThemedModule::InitModuleTheme ()
{
	bool	lRet = false;
#if	ISOLATION_AWARE_ENABLED
	try
	{
		IsolationAwareInit ();
	}
	catch AnyExceptionSilent

	try
	{
		tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
		lInitControls.dwICC = ICC_WIN95_CLASSES|ICC_STANDARD_CLASSES|ICC_LINK_CLASS|ICC_INTERNET_CLASSES;
		InitCommonControlsEx (&lInitControls);
	}
	catch AnyExceptionSilent

	try
	{
		if	(GetCurrentActCtx (&mActCtxHandle))
		{
#ifdef	_DEBUG_CONTEXT
			LogMessage (_DEBUG_CONTEXT, _T("Initial    ActCtx [%p]"), mActCtxHandle);
#endif
			lRet = true;
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

void CThemedModule::EndModuleTheme ()
{
#if	ISOLATION_AWARE_ENABLED
	try
	{
		if	(mActCtxHandle)
		{
#ifdef	_DEBUG_CONTEXT
			LogMessage (_DEBUG_CONTEXT, _T("Release    ActCtx [%p]"), mActCtxHandle);
#endif
			ReleaseActCtx (mActCtxHandle);
			mActCtxHandle = NULL;
		}
	}
	catch AnyExceptionSilent
	try
	{
		IsolationAwareCleanup ();
	}
	catch AnyExceptionSilent
#endif
}


//////////////////////////////////////////////////////////////////////

ULONG_PTR CThemedModule::ActivateModuleTheme ()
{
	ULONG_PTR	lCookie = 0;

#if	ISOLATION_AWARE_ENABLED
	HANDLE	lCtxHandle = NULL;

	if	(GetCurrentActCtx (&lCtxHandle))
	{
		if	(lCtxHandle == mActCtxHandle)
		{
			return 0;
		}
	}
	else
	{
		LogWinErr (LogNormal|LogTime, GetLastError (), _T("GetCurrentActCtx"));
	}

	if	(ActivateActCtx (mActCtxHandle, &lCookie))
	{
#ifdef	_DEBUG_CONTEXT
		LogMessage (_DEBUG_CONTEXT, _T("Activate   ActCtx [%p] [%u]"), mActCtxHandle, lCookie);
#endif
	}
	else
	{
		LogWinErr (LogNormal|LogTime, GetLastError (), _T("ActivateActCtx [%p]"), mActCtxHandle);
	}
#endif
	return lCookie;
}

void CThemedModule::DeactivateModuleTheme (ULONG_PTR pCookie)
{
#if	ISOLATION_AWARE_ENABLED
	if	(pCookie)
	{
		if	(DeactivateActCtx (0, pCookie))
		{
#ifdef	_DEBUG_CONTEXT
			LogMessage (_DEBUG_CONTEXT, _T("Deactivate ActCtx [%p] [%u]"), mActCtxHandle, pCookie);
#endif
		}
		else
		{
			LogWinErr (LogNormal|LogTime, GetLastError (), _T("DeactivateActCtx [%p] [%u]"), mActCtxHandle, pCookie);
		}
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void CThemedModule::LogModuleTheme (UINT pLogLevel, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			typedef HRESULT (CALLBACK * tDllGetVersion) (DLLVERSIONINFO *pdvi);

			CAtlString							lTitle;
			HINSTANCE							lComCtlModule;
			CAtlString							lComCtlDll;
			static tSS <DLLVERSIONINFO, DWORD>	lComCtlVersion;
			tDllGetVersion						lDllGetVersion;

			if	(pFormat)
			{
				va_list	lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
			}
			else
			{
				lTitle = _T("Module");
			}

			if	(lComCtlModule = GetModuleHandle (_T("COMCTL32")))
			{
				if	(lDllGetVersion = (tDllGetVersion) GetProcAddress (lComCtlModule, "DllGetVersion"))
				{
					try
					{
						(*lDllGetVersion) (&lComCtlVersion);
					}
					catch AnyExceptionSilent
				}

				GetModuleFileName (lComCtlModule, lComCtlDll.GetBuffer (MAX_PATH), MAX_PATH);
				lComCtlDll.ReleaseBuffer ();

				LogMessage (LogNormal|LogTime, _T("%s  Themed [%8.8X] ComCtlVersion [%u.%u.%u] ComCtlModule [%s]"), lTitle, GetThemeAppProperties(), lComCtlVersion.dwMajorVersion, lComCtlVersion.dwMinorVersion, lComCtlVersion.dwBuildNumber, lComCtlDll);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

