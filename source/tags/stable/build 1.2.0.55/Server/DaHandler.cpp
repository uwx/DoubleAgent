/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaHandler.h"
#include "Registry.h"
#include "GuidStr.h"
#include "ThreadSecurity.h"
#include "UserSecurity.h"
#include "SecurityDesc.h"

#ifdef	_DEBUG
#define	_LOG_INSTANCE	(GetProfileDebugInt(_T("LogInstance_Handler"),LogVerbose,true)&0xFFFF|LogTime)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG				LogNormal
#endif
#define	_LOG_ROOT_PATH					_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME				_T(_SERVER_REGNAME)
#define _LOG_DEF_LOGNAME				_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX						_T("Handler ")
static tPtr <CComAutoCriticalSection>	sLogCriticalSection = new CComAutoCriticalSection;
#define	_LOG_CRITICAL_SECTION			(!sLogCriticalSection?NULL:&sLogCriticalSection->m_sec)
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

CDaHandlerModule				_AtlModule;
LPCTSTR __declspec(selectany)	_AtlProfileName = _LOG_SECTION_NAME;
LPCTSTR __declspec(selectany)	_AtlProfilePath = _LOG_ROOT_PATH;

/////////////////////////////////////////////////////////////////////////////

CDaHandlerModule::CDaHandlerModule()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
	LogStart (false);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("CDaHandlerModule::CDaHandlerModule"));
		LogProcessUser (GetCurrentProcess(), _LOG_INSTANCE);
		LogProcessOwner (GetCurrentProcess(), _LOG_INSTANCE);
		LogProcessIntegrity (GetCurrentProcess(), _LOG_INSTANCE);
	}
#endif
}

CDaHandlerModule::~CDaHandlerModule()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareCleanup ();
#endif
	EndAllLifetimes ();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("CDaHandlerModule::~CDaHandlerModule [%d]"), GetLockCount());
	}
#endif
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////

void CDaHandlerModule::StartThreadLifetime ()
{
	GUID	lThreadId = GUID_NULL;
	CString	lMutexName;

	if	(SUCCEEDED (CoGetCurrentLogicalThreadId (&lThreadId)))
	{
		lMutexName.Format (_T("Local\\%s"), (CString)CGuidStr (lThreadId));

		if	(!mLifetimeMutex [lMutexName])
		{
			try
			{
				tPtr <CAutoMutex>					lLifetimeMutex;
				tSS <SECURITY_ATTRIBUTES, DWORD>	lMutexSecurity;
				CSecurityDesc						lAccessDescriptor (_T("O:BAG:BAD:(A;;0x0001f0001;;;WD)"));

				lMutexSecurity.bInheritHandle = FALSE;
				lMutexSecurity.lpSecurityDescriptor = lAccessDescriptor.mDescriptor;

				try
				{
					if	(CUserSecurity::IsUserAdministrator())
					{
						lLifetimeMutex = new CAutoMutex (&lMutexSecurity, TRUE, lMutexName);
					}
					else
					{
						lLifetimeMutex = new CAutoMutex (NULL, TRUE, lMutexName);
					}
				}
				catch AnyExceptionDebug

				if	(
						(!lLifetimeMutex)
					||	(!lLifetimeMutex->m_h)
					)
				{
					LogWinErr (LogAlways, GetLastError());
				}

#ifdef	_LOG_INSTANCE
				if	(LogIsActive())
				{
					LogMessage (_LOG_INSTANCE, _T("CDaHandlerModule::StartThreadLifetime Thread [%s] Mutex [%p] [%s]"), (CString)CGuidStr(lThreadId), (lLifetimeMutex.Ptr() ? lLifetimeMutex->m_h : NULL), lMutexName);
				}
#endif
				mLifetimeMutex.SetAt (lMutexName, lLifetimeMutex.Detach());
			}
			catch AnyExceptionDebug
		}
	}
}

void CDaHandlerModule::EndAllLifetimes ()
{
	mLifetimeMutex.RemoveAll ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	_AtlModule.StartThreadLifetime ();
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	return (_AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer(void)
{
	HRESULT	lResult = S_FALSE;

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
	LogComErrAnon (LogNormal|LogTime, lResult, _T("DllRegisterServer"));
	return lResult;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT	lResult = S_FALSE;

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
	LogComErrAnon (LogNormal|LogTime, lResult, _T("DllUnregisterServer"));
	return lResult;
}
