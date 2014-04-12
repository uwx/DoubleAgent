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
#include "StdAfx.h"
#include "DaCore.h"
#include "GuidStr.h"
#include "Localize.h"
#include "UserSecurity.h"

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG				LogNormal
#endif
#define	_LOG_ROOT_PATH					_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME				_T(_CORE_REGNAME)
#define _LOG_DEF_LOGNAME				_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX						_T("Core    ")
static tPtr <CComAutoCriticalSection>	sLogCriticalSection = new CComAutoCriticalSection;
#define	_LOG_CRITICAL_SECTION			(!sLogCriticalSection?NULL:&sLogCriticalSection->m_sec)
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CDaCoreModule)

CDaCoreModule::CDaCoreModule ()
#ifndef	_DACORE_LOCAL
:	CDaCoreAnchor (*(CComModule*)this)
#endif
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
#ifdef	_DEBUG
	LogStart (false);
	LogDebugRuntime ();
#else
	LogStart (false);
#endif
}

CDaCoreModule::~CDaCoreModule ()
{
	CLocalize::FreeMuiModules ();
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareCleanup ();
#endif
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////

CDaCoreModule					_AtlModule;
LPCTSTR __declspec(selectany)	_AtlProfileName = _LOG_SECTION_NAME;
LPCTSTR __declspec(selectany)	_AtlProfilePath = _LOG_ROOT_PATH;
#ifndef	_DACORE_LOCAL
CDaCoreAnchor&					_CoreAnchor = _AtlModule;
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	LogStart (false);
	return _AtlModule.DllGetClassObject (rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow ();
}

STDAPI DllRegisterServer(void)
{
	HRESULT	lResult;

	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		DllUnregisterServer ();
		lResult = _AtlModule.DllRegisterServer (FALSE);
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return lResult;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT	lResult;

	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		lResult = _AtlModule.DllUnregisterServer (FALSE);
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return lResult;
}
