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
#include "StdAfx.h"
#include "DaCore.h"
#include "GuidStr.h"
#include "Localize.h"
#include "UserSecurity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG			LogNormal
#endif
#define	_LOG_ROOT_PATH				_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME			_T(_CORE_REGNAME)
#define _LOG_DEF_LOGNAME			_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX					_T("Core ")
static tPtr <::CCriticalSection>	sLogCriticalSection = new CCriticalSection;
#define	_LOG_CRITICAL_SECTION		(!sLogCriticalSection?NULL:(CRITICAL_SECTION*)(*sLogCriticalSection))
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CDaCoreApp, CWinApp)

CDaCoreApp gApp;

CDaCoreApp::CDaCoreApp()
:	CWinApp (_T(_DOUBLEAGENT_NAME))
{
	SetRegistryKeyEx (_T(_DOUBLEAGENT_NAME), _T(_CORE_REGNAME));
}

CDaCoreApp::~CDaCoreApp()
{
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDaCoreApp::InitInstance()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
#ifndef	_DEBUG
	LogStart (false);
#endif
	COleObjectFactory::RegisterAll();
	return TRUE;
}

int CDaCoreApp::ExitInstance()
{
	CLocalize::FreeMuiModules ();
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareCleanup ();
#endif
	LogStop (LogIfActive);
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////

void CDaCoreApp::InitLogging (LPCTSTR pLogFileName, UINT pLogLevel)
{
	LogStart (false, pLogFileName, pLogLevel);
#ifdef	_DEBUG
	LogDebugRuntime ();
	//LogDebugMemory (_CRTDBG_ALLOC_MEM_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_1024_DF);
	//LogDebugMemory (_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_CHECK_DEFAULT_DF);
#endif
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDaCoreApp, CWinApp)
	//{{AFX_MSG_MAP(CDaCoreApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CDaCoreModule::CDaCoreModule ()
{
}
CDaCoreModule::~CDaCoreModule ()
{
}

CDaCoreModule	_AtlModule;

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#ifdef	_DEBUG_NOT
	if	(!gLogFileName[0])
	{
		LogStart (true);
	}
#else
	LogStart (false);
#endif
	HRESULT lResult = AfxDllGetClassObject (rclsid, riid, ppv);
	if	(FAILED (lResult))
	{
		HRESULT	lAltResult = _AtlModule.DllGetClassObject (rclsid, riid, ppv);
		if	(SUCCEEDED (lAltResult))
		{
			lResult = lAltResult;
		}
	}
	return lResult;
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT lResult = AfxDllCanUnloadNow();
	if	(lResult == S_OK)
	{
		lResult = _AtlModule.DllCanUnloadNow ();
	}
	return lResult;
}

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		DllUnregisterServer ();
		if	(!COleObjectFactory::UpdateRegistryAll(TRUE))
		{
			return SELFREG_E_CLASS;
		}
		_AtlModule.DllRegisterServer(FALSE);
	}
	else
	{
		return HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		if	(!COleObjectFactory::UpdateRegistryAll(FALSE))
		{
			return SELFREG_E_CLASS;
		}
		_AtlModule.DllUnregisterServer(FALSE);
	}
	else
	{
		return HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return S_OK;
}
