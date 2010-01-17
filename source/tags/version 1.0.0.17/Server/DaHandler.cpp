/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "SecurityDescriptor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE	(GetProfileDebugInt(_T("LogInstance_Handler"),LogVerbose,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG		LogNormal
#endif
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T(_SERVER_REGNAME)
#define _LOG_DEF_LOGNAME		_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX				_T("Hnd  ")
static tPtr <CCriticalSection>	sLogCriticalSection = new CCriticalSection;
#define	_LOG_CRITICAL_SECTION	(!sLogCriticalSection?NULL:(CRITICAL_SECTION*)(*sLogCriticalSection))
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDaHandlerApp, CWinApp)
	//{{AFX_MSG_MAP(CDaHandlerApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC (CDaHandlerApp, CWinApp)

CDaHandlerApp gApp;

/////////////////////////////////////////////////////////////////////////////

CDaHandlerApp::CDaHandlerApp()
:	CWinApp (_T(_DOUBLEAGENT_NAME))
{
	SetRegistryKeyEx (_T(_DOUBLEAGENT_NAME), _T(_SERVER_REGNAME));
}

CDaHandlerApp::~CDaHandlerApp()
{
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDaHandlerApp::InitInstance()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
	LogStart (false);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogProcessUser (GetCurrentProcess(), _LOG_INSTANCE);
		LogProcessOwner (GetCurrentProcess(), _LOG_INSTANCE);
		LogProcessIntegrity (GetCurrentProcess(), _LOG_INSTANCE);
	}
#endif
	COleObjectFactory::RegisterAll();
	return TRUE;
}

int CDaHandlerApp::ExitInstance()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareCleanup ();
#endif
	EndAllLifetimes ();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("CDaHandlerApp::ExitInstance [%d]"), AfxGetModuleState()->m_nObjectCount);
	}
#endif
	LogStop (LogIfActive);
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////

void CDaHandlerApp::StartThreadLifetime ()
{
	GUID	lThreadId = GUID_NULL;
	CString	lMutexName;

	if	(SUCCEEDED (CoGetCurrentLogicalThreadId (&lThreadId)))
	{
		lMutexName.Format (_T("Local\\%s"), (CString)CGuidStr (lThreadId));

		if	(mLifetimeMutex [lMutexName] == NULL)
		{
			try
			{
				tPtr <CMutex>						lLifetimeMutex;
				tSS <SECURITY_ATTRIBUTES, DWORD>	lMutexSecurity;
				CSecurityDescriptor					lAccessDescriptor (_T("O:BAG:BAD:(A;;0x0001f0001;;;WD)"));

				lMutexSecurity.bInheritHandle = FALSE;
				lMutexSecurity.lpSecurityDescriptor = lAccessDescriptor.mDescriptor;

				try
				{
					if	(CUserSecurity::IsUserAdministrator())
					{
						lLifetimeMutex = new CMutex (TRUE, lMutexName, &lMutexSecurity);
					}
					else
					{
						lLifetimeMutex = new CMutex (TRUE, lMutexName);
					}
				}
				catch AnyExceptionDebug

				if	(
						(!lLifetimeMutex)
					||	(!lLifetimeMutex->m_hObject)
					)
				{
					LogWinErr (LogAlways, GetLastError());
				}

#ifdef	_LOG_INSTANCE
				if	(LogIsActive())
				{
					LogMessage (_LOG_INSTANCE, _T("CDaHandlerApp::StartThreadLifetime Thread [%s] Mutex [%p] [%s]"), (CString)CGuidStr(lThreadId), (lLifetimeMutex.Ptr() ? lLifetimeMutex->m_hObject : NULL), lMutexName);
				}
#endif
				mLifetimeMutex.SetAt (lMutexName, lLifetimeMutex.Detach());
			}
			catch AnyExceptionDebug
		}
	}
}

void CDaHandlerApp::EndAllLifetimes ()
{
	mLifetimeMutex.RemoveAll ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	((CDaHandlerApp*) AfxGetApp())->StartThreadLifetime ();
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
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
	}
	else
	{
		return HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return S_OK;
}
