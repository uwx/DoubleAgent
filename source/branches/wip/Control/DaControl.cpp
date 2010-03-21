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
#include "DaControlObj.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "GuidStr.h"
#include "UiState.h"
//#include "OleMessageFilterEx.h"
#include "Localize.h"
#include "UserSecurity.h"
#include "ThreadSecurity.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_SERVER_LEVEL		LogNormal
#define	_DEBUG_DLL_UNLOAD		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG				LogNormal
#endif
#define	_LOG_ROOT_PATH					_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME				_T(_CONTROL_REGNAME)
#define _LOG_DEF_LOGNAME				_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX						_T("Ctrl ")
static tPtr <CComAutoCriticalSection>	sLogCriticalSection = new CComAutoCriticalSection;
#define	_LOG_CRITICAL_SECTION			(!sLogCriticalSection?NULL:&sLogCriticalSection->m_sec)
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

class CDaControlApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CDaControlApp, CWinApp)
END_MESSAGE_MAP()

BOOL CDaControlApp::InitInstance()
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
	return CWinApp::InitInstance();
}

int CDaControlApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

CDaControlApp theApp;

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaControlModule _AtlModule;

CDaControlModule::CDaControlModule ()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
//#ifdef	_DEBUG
//	LogStart (GetProfileDebugInt(_T("LogRestart"))!=0);
//#else
//	LogStart (false);
//#endif
//	LogProcessUser (GetCurrentProcess(), LogIfActive);
//	LogProcessOwner (GetCurrentProcess(), LogIfActive);
//	LogProcessIntegrity (GetCurrentProcess(), LogIfActive);

#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceCOM.SetLevel (2);
	atlTraceCOM.SetStatus (ATLTRACESTATUS_ENABLED);
#endif	
#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceRefcount.SetLevel (0);
	atlTraceRefcount.SetStatus (ATLTRACESTATUS_ENABLED);
#endif	
#ifdef	_ATL_DEBUG_QI
	atlTraceQI.SetLevel (2);
	atlTraceQI.SetStatus (ATLTRACESTATUS_ENABLED);
#endif	
#ifdef	_DEBUG
	atlTraceGeneral.SetLevel (2);
	atlTraceGeneral.SetStatus (ATLTRACESTATUS_ENABLED);
	atlTraceWindowing.SetLevel (2);
	atlTraceWindowing.SetStatus (ATLTRACESTATUS_ENABLED);
#endif	
}

CDaControlModule::~CDaControlModule ()
{
#ifdef	_DEBUG_DLL_UNLOAD
	try
	{
		LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::~CDaControlModule ServerLevel [%d] NotifyLevel [%d] Objects [%d %d]"), mServerCallLevel, mNotifyLevel, _AtlModule.GetLockCount(), mComObjects.GetSize());
		for	(INT_PTR lNdx = 0; lNdx <= mComObjects.GetUpperBound(); lNdx++)
		{
			try
			{
				CComObjectRoot *	lObjectRoot = NULL;
				try
				{
					lObjectRoot = dynamic_cast <CComObjectRoot *> (mComObjects [lNdx]);
				}
				catch AnyExceptionSilent

				LogMessage (_DEBUG_DLL_UNLOAD, _T("  Object [%2d] [%p(%d)] [%s]"), lNdx, mComObjects [lNdx], (lObjectRoot ? lObjectRoot->m_dwRef : -1), ObjTypeName (mComObjects [lNdx]));
			}
			catch AnyExceptionSilent
		}
	}
	catch AnyExceptionSilent
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
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaControlModule::RegisterServer(BOOL bRegTypeLib, const CLSID* pCLSID)
{
	HRESULT	lResult;
	if	(SUCCEEDED (lResult = UpdateRegistry (TRUE)))
	{
		lResult = CAtlModuleT <CDaControlModule>::RegisterServer (bRegTypeLib, pCLSID);
	}
	return lResult;
}

HRESULT CDaControlModule::UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID)
{
	HRESULT	lResult;
	if	(SUCCEEDED (lResult = UpdateRegistry (FALSE)))
	{
		lResult = CAtlModuleT <CDaControlModule>::UnregisterServer (bUnRegTypeLib, pCLSID);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaControlModule::OnControlCreated (CDaControlObj * pControl)
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

void CDaControlModule::OnControlDeleted (CDaControlObj * pControl)
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

void CDaControlModule::DeleteAllControls ()
{
	try
	{
		if	(mControls.GetSize () > 0)
		{
			INT_PTR	lNdx;
#ifdef	_DEBUG_DLL_UNLOAD
			LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::DeleteAllControls [%d] ObjectCount [%d]"), mControls.GetSize(), _AtlModule.GetLockCount());
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
				tPtr <CDaControlObj>	lControl;
#ifdef	_DEBUG_DLL_UNLOAD
				LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::DeleteAllControls [%d] Control [%p(%d)] ObjectCount [%d]"), mControls.GetSize(), mControls.GetAt(0), mControls.GetAt(0)->m_dwRef, _AtlModule.GetLockCount());
#endif
				lControl = mControls.GetAt (0);
				mControls.RemoveAt (0);
				lControl->Terminate (true);
				SafeFreeSafePtr (lControl);
			}
			while	(mControls.GetSize() > 0);

#ifdef	_DEBUG_DLL_UNLOAD
			LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::DeleteAllControls Done ObjectCount [%d]"), _AtlModule.GetLockCount());
#endif
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaControlModule::PreServerCall (LPUNKNOWN pServerInterface)
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

HRESULT CDaControlModule::PostServerCall (LPUNKNOWN pServerInterface)
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

bool CDaControlModule::PreNotify ()
{
	mNotifyLevel++;
	PendingMessageFilter ();
	return true;
}

void CDaControlModule::PostNotify ()
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

void CDaControlModule::PendingMessageFilter ()
{
/*
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
*/	
}

void CDaControlModule::BusyMessageFilter ()
{
/*
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
*/	
}

void CDaControlModule::EndMessageFilter (bool pFinal)
{
/*
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
*/	
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_DLL_UNLOAD
static bool sDllCanUnload = false;
#endif

STDAPI DllCanUnloadNow(void)
{
    /**/AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT lResult = ((AfxDllCanUnloadNow()==S_OK) && (_AtlModule.GetLockCount()==0)) ? S_OK : S_FALSE;
#ifdef	_DEBUG_DLL_UNLOAD
	if	(
			(lResult == S_OK)
		&&	(!sDllCanUnload)
		)
	{
		sDllCanUnload = true;
		LogComErrAnon (MinLogLevel(_DEBUG_DLL_UNLOAD,LogAlways), lResult, _T("DllCanUnloadNow"));
	}
#endif
	return lResult;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    HRESULT lResult = _AtlModule.DllGetClassObject(rclsid, riid, ppv);
#ifdef	_DEBUG_DLL_UNLOAD    
	LogComErrAnon (MinLogLevel(_DEBUG_DLL_UNLOAD,LogAlways), lResult, _T("DllGetClassObject [%s] [%s]"), CGuidStr::GuidName(rclsid), CGuidStr::GuidName(riid));
#endif	
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllRegisterServer(void)
{
	HRESULT	lResult = S_OK;

	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		_AtlModule.DllUnregisterServer();
		lResult = _AtlModule.DllRegisterServer();
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	if	(LogIsActive ())
	{
		LogComErrAnon (LogAlways, lResult, _T("DllRegisterServer"));
	}
	return lResult;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT	lResult = S_OK;

	LogStart (false);
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		lResult = _AtlModule.DllUnregisterServer();
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
	}
	if	(LogIsActive ())
	{
		LogComErrAnon (LogAlways, lResult, _T("DllUnregisterServer"));
	}
	return lResult;
}
