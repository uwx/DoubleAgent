/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControl.h"
#include "DaAgentCtl.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "GuidStr.h"
#include "UiState.h"
#include "OleMessageFilterEx.h"
#include "Localize.h"
#include "UserSecurity.h"
#include "ThreadSecurity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_DEBUG_SERVER_LEVEL		LogNormal
#define	_DEBUG_DLL_UNLOAD		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG		LogNormal
#endif
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T(_CONTROL_REGNAME)
#define _LOG_DEF_LOGNAME		_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX				_T("Ctrl ")
static tPtr <CCriticalSection>	sLogCriticalSection = new CCriticalSection;
#define	_LOG_CRITICAL_SECTION	(!sLogCriticalSection?NULL:(CRITICAL_SECTION*)(*sLogCriticalSection))
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG_DLL_UNLOAD
/////////////////////////////////////////////////////////////////////////////
static bool sDllCanUnload = false;

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT lResult = AfxDllGetClassObject(rclsid, riid, ppv);
	LogComErrAnon (MinLogLevel(_DEBUG_DLL_UNLOAD,LogAlways), lResult, _T("DllGetClassObject [%s] [%s]"), CGuidStr::GuidName(rclsid), CGuidStr::GuidName(riid));
	return lResult;
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT lResult = AfxDllCanUnloadNow();
	if	(
			(lResult == S_OK)
		&&	(!sDllCanUnload)
		)
	{
		sDllCanUnload = true;
		LogComErrAnon (MinLogLevel(_DEBUG_DLL_UNLOAD,LogAlways), lResult, _T("DllCanUnloadNow"));
	}

	LogComErrAnon (LogAlways|LogHighVolume, lResult, _T("DllCanUnloadNow"));
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG_DLL_UNLOAD
/////////////////////////////////////////////////////////////////////////////

const GUID gDaTypeLibId = __uuidof(DaControlTypeLib);
const WORD gDaTypeLibVerMajor = _CONTROL_VER_MAJOR;
const WORD gDaTypeLibVerMinor = _CONTROL_VER_MINOR;

////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaControlApp, COleControlModule);

CDaControlApp gApp;

////////////////////////////////////////////////////////////////////////////

CDaControlApp::CDaControlApp ()
:	mServerCallLevel (0),
	mNotifyLevel (0)
{
}

CDaControlApp::~CDaControlApp ()
{
}

////////////////////////////////////////////////////////////////////////////

BOOL CDaControlApp::InitInstance()
{
	BOOL	lRet;
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
	lRet = COleControlModule::InitInstance();
	if	(lRet)
	{
		SetRegistryKeyEx (_T(_DOUBLEAGENT_NAME), _T(_CONTROL_REGNAME));
#ifdef	_DEBUG
		LogStart (GetProfileDebugInt(_T("LogRestart"))!=0);
#else
		LogStart (false);
#endif
		LogProcessUser (GetCurrentProcess(), LogIfActive);
		LogProcessOwner (GetCurrentProcess(), LogIfActive);
		LogProcessIntegrity (GetCurrentProcess(), LogIfActive);
	}
	return lRet;
}

int CDaControlApp::ExitInstance()
{
#ifdef	_DEBUG_DLL_UNLOAD
	LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlApp::ExitInstance ServerLevel [%d] NotifyLevel [%d] Objects [%d %d]"), mServerCallLevel, mNotifyLevel, AfxGetModuleState()->m_nObjectCount, mComObjects.GetSize());
	for	(INT_PTR lNdx = 0; lNdx <= mComObjects.GetUpperBound(); lNdx++)
	{
		try
		{
			LogMessage (LogDebug, _T("Object [%2d] [%p(%d)] [%s]"), lNdx, mComObjects [lNdx], mComObjects [lNdx]->m_dwRef, ObjClassName (mComObjects [lNdx]));
		}
		catch AnyExceptionSilent
	}
#endif
	EndMessageFilter (true);
	BusyMessageFilter ();
	DeleteAllControls ();
	EndMessageFilter (true);

	CLocalize::FreeMuiModules ();
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareCleanup ();
#endif
	LogStop (LogIfActive);
	return COleControlModule::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaControlApp::OnControlCreated (CDaAgentCtl * pControl)
{
	if	(this)
	{
		try
		{
			mControls.AddUnique (pControl);
		}
		catch AnyExceptionSilent
	}
}

void CDaControlApp::OnControlDeleted (CDaAgentCtl * pControl)
{
	if	(this)
	{
		try
		{
			mControls.Remove (pControl);
		}
		catch AnyExceptionSilent
	}
}

void CDaControlApp::DeleteAllControls ()
{
	try
	{
		if	(mControls.GetSize () > 0)
		{
			INT_PTR	lNdx;
#ifdef	_DEBUG_DLL_UNLOAD
			LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlApp::DeleteAllControls [%d] ObjectCount [%d]"), mControls.GetSize(), AfxGetModuleState()->m_nObjectCount);
#endif
			for	(lNdx = mControls.GetUpperBound(); lNdx >= 0; lNdx--)
			{
				if	(lNdx <= mControls.GetUpperBound()) // A control might be deleted during this loop
				{
					mControls.GetAt (lNdx)->DisconnectNotify (true);
				}
			}

			do
			{
				tPtr <CDaAgentCtl>	lControl;
#ifdef	_DEBUG_DLL_UNLOAD
				LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlApp::DeleteAllControls [%d] Control [%p(%d)] ObjectCount [%d]"), mControls.GetSize(), mControls.GetAt(0), mControls.GetAt(0)->m_dwRef, AfxGetModuleState()->m_nObjectCount);
#endif
				lControl = mControls.GetAt (0);
				mControls.RemoveAt (0);
				lControl->Terminate (true);
				SafeFreeSafePtr (lControl);
			}
			while	(mControls.GetSize() > 0);

#ifdef	_DEBUG_DLL_UNLOAD
			LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlApp::DeleteAllControls Done ObjectCount [%d]"), AfxGetModuleState()->m_nObjectCount);
#endif
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaControlApp::PreServerCall (LPUNKNOWN pServerInterface)
{
	if	(!pServerInterface)
	{
#ifdef	_DEBUG_SERVER_LEVEL
		LogMessage (_DEBUG_SERVER_LEVEL, _T("PreServerCall NULL"));
#endif
		return AGENTCTLERROR_SERVERINIT;
	}

	mServerCallLevel++;
	PendingMessageFilter ();
	return S_OK;
}

HRESULT CDaControlApp::PostServerCall (LPUNKNOWN pServerInterface)
{
	if	(mServerCallLevel > 0)
	{
		mServerCallLevel--;
	}
#ifdef	_DEBUG_SERVER_LEVEL
	else
	{
		LogMessage (_DEBUG_SERVER_LEVEL, _T("Imbalanced Pre/Post ServerCall"));
	}
#endif
	if	(
			(mServerCallLevel <= 0)
		&&	(mNotifyLevel <= 0)
		)
	{
		EndMessageFilter (false);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaControlApp::PreNotify ()
{
	mNotifyLevel++;
	PendingMessageFilter ();
	return true;
}

void CDaControlApp::PostNotify ()
{
	if	(mNotifyLevel > 0)
	{
		mNotifyLevel--;
	}
	if	(
			(mServerCallLevel <= 0)
		&&	(mNotifyLevel <= 0)
		)
	{
		EndMessageFilter (false);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaControlApp::PendingMessageFilter ()
{
	if	(
			(!mMessageFilter)
		&&	(mMessageFilter = new COleMessageFilterEx (this))
		)
	{
		mMessageFilter->SafeEnableNotResponding (0);
		mMessageFilter->SafeEnableBusy (0);
#ifdef	_DEBUG
		mMessageFilter->mLogLevelDlg = LogNormal|LogTimeMs;
//		mMessageFilter->mLogLevelMsg = LogNormal|LogTimeMs;
//		mMessageFilter->SetMessagePendingDelay (0);
#endif
	}

	if	(
			(mMessageFilter)
		&&	(m_pMessageFilter != mMessageFilter)
		&&	(mMessageFilter->Register ())
		)
	{
		m_pMessageFilter = mMessageFilter;
	}
}

void CDaControlApp::BusyMessageFilter ()
{
	if	(
			(mMessageFilter)
		||	(mMessageFilter = new COleMessageFilterEx (this))
		)
	{
		mMessageFilter->SafeEnableNotResponding (INFINITE);
		mMessageFilter->SafeEnableBusy (0);
		mMessageFilter->SetMessagePendingDelay (0);
#ifdef	_DEBUG
		mMessageFilter->mLogLevelDlg = LogNormal|LogTimeMs;
		mMessageFilter->mLogLevelMsg = LogNormal|LogTimeMs;
#endif
	}

	if	(
			(mMessageFilter)
		&&	(m_pMessageFilter != mMessageFilter)
		&&	(mMessageFilter->Register ())
		)
	{
//		mMessageFilter->BeginBusyState ();
		m_pMessageFilter = mMessageFilter;
		mServerCallLevel = SHRT_MAX;
	}
}

void CDaControlApp::EndMessageFilter (bool pFinal)
{
	if	(
			(m_pMessageFilter)
		&&	(m_pMessageFilter == mMessageFilter)
		)
	{
		mMessageFilter->Revoke ();
		m_pMessageFilter = NULL;
	}
	if	(pFinal)
	{
		SafeFreeSafePtr (mMessageFilter);
		mServerCallLevel = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllRegisterServer(void)
{
	HRESULT	lResult = S_OK;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		//AfxOleUnregisterTypeLib(gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor);
		COleObjectFactoryEx::UpdateRegistryAll(FALSE);

		if	(!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), gDaTypeLibId))
		{
			lResult = SELFREG_E_TYPELIB;
		}
		if	(!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		{
			lResult = SELFREG_E_CLASS;
		}
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return lResult;
}


/////////////////////////////////////////////////////////////////////////////

STDAPI DllUnregisterServer(void)
{
	HRESULT	lResult = S_OK;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		if	(!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		{
			lResult = SELFREG_E_CLASS;
		}
		if	(!AfxOleUnregisterTypeLib(gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor))
		{
			lResult = SELFREG_E_TYPELIB;
		}
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	return lResult;
}
