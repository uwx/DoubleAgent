/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "DaControlMod.h"
#include "DaControl.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "GuidStr.h"
#include "ComMessageFilter.h"
#include "Localize.h"
#include "UserSecurity.h"
#include "ThreadSecurity.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_NOTIFY_PATH		(GetProfileDebugInt(_T("DebugNotifyPath"),LogVerbose,true)&0xFFFF|LogTime)
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
#define	_LOG_PREFIX						_T("Control ")
static tPtr <CComAutoCriticalSection>	sLogCriticalSection = new CComAutoCriticalSection;
#define	_LOG_CRITICAL_SECTION			(!sLogCriticalSection?NULL:&sLogCriticalSection->m_sec)
#include "LogAccess.inl"
#include "Log.inl"
#include "LogCrash.inl"
/////////////////////////////////////////////////////////////////////////////

CDaControlModule				_AtlModule;
LPCTSTR __declspec(selectany)	_AtlProfileName = _LOG_SECTION_NAME;
LPCTSTR __declspec(selectany)	_AtlProfilePath = _LOG_ROOT_PATH;
#ifdef	_DACORE_LOCAL
CDaCoreAnchor&					_CoreAnchor = _AtlModule;
#endif

/////////////////////////////////////////////////////////////////////////////

CDaControlModule::CDaControlModule ()
:	CListeningGlobal (*(CGlobalAnchor*)this),
#ifdef	_DACORE_LOCAL
	CDaCoreAnchor (*(CComModule*)this),
#endif
	mAppActive (false)
{
	mNextCharID = SHRT_MAX+1;
	LogCrash_Initialize ();
	CListeningGlobal::Startup ();

	InitModuleTheme ();
#ifdef	_DEBUG
	LogStart (GetProfileDebugInt(_T("LogRestart"))!=0);
#else
	LogStart (false);
#endif
	LogProcessUser (GetCurrentProcess(), LogIfActive);
	LogProcessOwner (GetCurrentProcess(), LogIfActive);
	LogProcessIntegrity (GetCurrentProcess(), LogIfActive);

#ifdef	_ATL_DEBUG_INTERFACES
	LogMessage (LogIfActive|LogTime, _T("_ATL_DEBUG_INTERFACES %d"), _ATL_DEBUG_INTERFACES);
	atlTraceCOM.SetLevel (_ATL_DEBUG_INTERFACES);
	atlTraceCOM.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceRefcount.SetLevel (0);
	atlTraceRefcount.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceException.SetLevel (0);
	atlTraceException.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_ATL_DEBUG_QI
	LogMessage (LogIfActive|LogTime, _T("_ATL_DEBUG_QI %d"), _ATL_DEBUG_QI);
	atlTraceQI.SetLevel (_ATL_DEBUG_QI);
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
	Terminate ();
	LogStop (LogIfActive);
	LogCrash_Terminate ();
}

/////////////////////////////////////////////////////////////////////////////

void CDaControlModule::Terminate ()
{
	__try
	{
		_Terminate ();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}
}

void CDaControlModule::_Terminate ()
{
#ifdef	_DEBUG_DLL_UNLOAD
	try
	{
#ifdef	_DACORE_LOCAL
		LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::~CDaControlModule NotifyLevel [%d] Objects [%d %d]"), mNotifyLevel, _AtlModule.GetLockCount(), mComObjects.GetCount());
#else
		LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::~CDaControlModule ServerLevel [%d] NotifyLevel [%d] Objects [%d %d]"), mServerCallLevel, mNotifyLevel, _AtlModule.GetLockCount(), mComObjects.GetCount());
#endif
		for	(INT_PTR lNdx = 0; lNdx < (INT_PTR)mComObjects.GetCount(); lNdx++)
		{
			try
			{
				CComObjectRoot *	lObjectRoot = NULL;
				try
				{
					lObjectRoot = dynamic_cast <CComObjectRoot *> (mComObjects [lNdx]);
				}
				catch AnyExceptionSilent

				LogMessage (_DEBUG_DLL_UNLOAD, _T("  Object [%2d] [%p(%d)] [%s]"), lNdx, mComObjects [lNdx], (lObjectRoot ? lObjectRoot->m_dwRef : -1), AtlTypeName (mComObjects [lNdx]));
			}
			catch AnyExceptionSilent
		}
	}
	catch AnyExceptionSilent
#endif
	EndMessageFilter (true);
	FinalMessageFilter ();
	DeleteAllControls ();
	EndMessageFilter (true);

	CListeningGlobal::Shutdown ();
	CLocalize::FreeMuiModules ();

	EndModuleTheme ();
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

void CDaControlModule::OnControlCreated (DaControl * pControl)
{
	if	(this)
	{
		try
		{
			mControls.AddUnique (pControl);
		}
		catch AnyExceptionSilent
		try
		{
			mInstanceNotify.AddUnique (&pControl->mLocalEventNotify);
		}
		catch AnyExceptionSilent
	}
}

void CDaControlModule::OnControlDeleted (DaControl * pControl)
{
	if	(this)
	{
		try
		{
			mControls.Remove (pControl);
		}
		catch AnyExceptionSilent
		try
		{
			mInstanceNotify.Remove (&pControl->mLocalEventNotify);
		}
		catch AnyExceptionSilent
	}
}

void CDaControlModule::DeleteAllControls ()
{
	try
	{
		if	(mControls.GetCount() > 0)
		{
			INT_PTR	lNdx;
#ifdef	_DEBUG_DLL_UNLOAD
			LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::DeleteAllControls [%d] ObjectCount [%d]"), mControls.GetCount(), _AtlModule.GetLockCount());
#endif
			for	(lNdx = mControls.GetCount()-1; lNdx >= 0; lNdx--)
			{
				if	(lNdx < (INT_PTR)mControls.GetCount()) // A control might be deleted during this loop
				{
					mControls.GetAt (lNdx)->DisconnectNotify (true);
				}
			}

			do
			{
				tPtr <DaControl>	lControl;
#ifdef	_DEBUG_DLL_UNLOAD
				LogMessage (_DEBUG_DLL_UNLOAD, _T("CDaControlModule::DeleteAllControls [%d] Control [%p(%d)] ObjectCount [%d]"), mControls.GetCount(), mControls.GetAt(0), mControls.GetAt(0)->m_dwRef, _AtlModule.GetLockCount());
#endif
				lControl = mControls.GetAt (0);
				mControls.RemoveAt (0);
				lControl->Terminate (true);
				SafeFreeSafePtr (lControl);
			}
			while	(mControls.GetCount() > 0);

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

#ifndef	_DACORE_LOCAL
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
	return S_OK;
}
#endif

#ifndef	_DACORE_LOCAL
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
	return S_OK;
}
#endif

/////////////////////////////////////////////////////////////////////////////

bool CDaControlModule::PreNotify ()
{
	mNotifyLevel++;
	NotifyMessageFilter ();
	return true;
}

void CDaControlModule::PostNotify ()
{
	if	(mNotifyLevel > 0)
	{
		mNotifyLevel--;
	}
	if	(mNotifyLevel <= 0)
	{
		EndMessageFilter (false);
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CDaControlModule::NotifyMessageFilter ()
{
	if	(
			(!mMessageFilter)
		&&	(mMessageFilter = new CComObjectNoLock <CComMessageFilter>)
		)
	{
		mMessageFilter->AddRef ();
	}
	if	(
			(mMessageFilter)
		&&	(!mMessageFilter->IsRegistered ())
		&&	(LogComErr (LogNormal|LogTime, mMessageFilter->Register ()) != S_OK)
		)
	{
		mMessageFilter = NULL;
	}
	if	(mMessageFilter)
	{
		mMessageFilter->DoNotDisturb (true);
		return true;
	}
	return false;
}

bool CDaControlModule::FinalMessageFilter ()
{
	if	(
			(!mMessageFilter)
		&&	(mMessageFilter = new CComObjectNoLock <CComMessageFilter>)
		)
	{
		mMessageFilter->AddRef ();
	}
	if	(
			(mMessageFilter)
		&&	(!mMessageFilter->IsRegistered ())
		&&	(LogComErr (LogNormal|LogTime, mMessageFilter->Register ()) != S_OK)
		)
	{
		mMessageFilter = NULL;
	}
	if	(mMessageFilter)
	{
		mMessageFilter->CheckOut ();
		mNotifyLevel = SHRT_MAX;
		return true;
	}
	return false;
}

void CDaControlModule::EndMessageFilter (bool pFinal)
{
	if	(
			(mMessageFilter)
		&&	(mMessageFilter->IsRegistered ())
		)
	{
		while (mMessageFilter->GetDoNotDisturb () > 0)
		{
			mMessageFilter->DoNotDisturb (false);
		}
		mMessageFilter->Revoke ();
	}
	if	(pFinal)
	{
		SafeFreeSafePtr (mMessageFilter);
		mNotifyLevel = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaControlModule::_CharacterLoaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaControlModule::_CharacterLoaded [%d]"), pCharID);
#endif
	CListeningGlobal::_CharacterLoaded (pCharID);
	CEventGlobal::_CharacterLoaded (pCharID);
}

void CDaControlModule::_CharacterUnloaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaControlModule::_CharacterUnloaded [%d]"), pCharID);
#endif
	CListeningGlobal::_CharacterUnloaded (pCharID);
	CEventGlobal::_CharacterUnloaded (pCharID);
}

void CDaControlModule::_CharacterNameChanged (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaControlModule::_CharacterNameChanged [%d]"), pCharID);
#endif
	CListeningGlobal::_CharacterNameChanged (pCharID);
	CEventGlobal::_CharacterNameChanged (pCharID);
}

void CDaControlModule::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaControlModule::_CharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
	CListeningGlobal::_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
	CEventGlobal::_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
}

void CDaControlModule::_AppActivated (bool pActive)
{
	mAppActive = pActive;
	CListeningGlobal::_AppActivated (pActive);
	CEventGlobal::_AppActivated (pActive);
}

void CDaControlModule::_OptionsChanged ()
{
	CListeningGlobal::_OptionsChanged ();
	CEventGlobal::_OptionsChanged ();
}

/////////////////////////////////////////////////////////////////////////////

bool CDaControlModule::IsAppActive () const
{
	return mAppActive;
}

bool CDaControlModule::VerifyAppActive () const
{
	bool	lRet = false;

#ifdef	_DEBUG_ACTIVE_NOT
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		try
		{
			BOOL						lIsGUIThread = IsGUIThread (FALSE);
			HWND						lForegroundWnd = GetForegroundWindow ();
			DWORD						lForegroundProcess = 0;
			tSS <GUITHREADINFO, DWORD>	lForegroundInfo;
			tSS <GUITHREADINFO, DWORD>	lThreadInfo;

			if	(lForegroundWnd)
			{
				GetWindowThreadProcessId (lForegroundWnd, &lForegroundProcess);
			}
			LogMessage (_DEBUG_ACTIVE, _T("IsAppActive IsGUIDThread [%d] ForegroundWnd [%u] [%p]"), lIsGUIThread, lForegroundProcess);
			if	(GetGUIThreadInfo (NULL, &lForegroundInfo))
			{
				lForegroundProcess = 0;
				if	(lForegroundInfo.hwndActive)
				{
					GetWindowThreadProcessId (lForegroundInfo.hwndActive, &lForegroundProcess);
				}
				LogMessage (_DEBUG_ACTIVE, _T("  Foreground Active [%u] [%p]"), lForegroundProcess, lForegroundInfo.hwndActive);
			}
			if	(
					(lIsGUIThread)
				&&	(GetGUIThreadInfo (GetCurrentThreadId(), &lThreadInfo))
				)
			{
				lForegroundProcess = 0;
				if	(lForegroundInfo.hwndActive)
				{
					GetWindowThreadProcessId (lThreadInfo.hwndActive, &lForegroundProcess);
				}
				LogMessage (_DEBUG_ACTIVE, _T("  Thread Active [%u] [%p]"), lForegroundProcess, lThreadInfo.hwndActive);
			}
		}
		catch AnyExceptionSilent
	}
#endif
	try
	{
		BOOL						lIsGUIThread = IsGUIThread (FALSE);
		HWND						lForegroundWnd = GetForegroundWindow ();
		DWORD						lForegroundProcess = 0;
		tSS <GUITHREADINFO, DWORD>	lForegroundInfo;

		if	(
				(lForegroundWnd = GetForegroundWindow ())
			&&	(GetWindowThreadProcessId (lForegroundInfo.hwndActive, &lForegroundProcess))
			&&	(GetCurrentProcessId () == lForegroundProcess)
			)
		{
			lRet = true;
		}
		else
		if	(
				(lIsGUIThread)
			&&	(GetGUIThreadInfo (NULL, &lForegroundInfo))
			&&	(lForegroundInfo.hwndActive)
			&&	(GetWindowThreadProcessId (lForegroundInfo.hwndActive, &lForegroundProcess))
			&&	(GetCurrentProcessId () == lForegroundProcess)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_DLL_UNLOAD
static bool sDllCanUnload = false;
#endif

STDAPI DllCanUnloadNow(void)
{
	HRESULT lResult = _AtlModule.DllCanUnloadNow ();
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
	if	(lResult == S_OK)
	{
		CSapiVoiceCache::TerminateStaticInstance ();
		CSapiInputCache::TerminateStaticInstance ();
	}
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
		AtlUnRegisterTypeLib (_AtlBaseModule.GetModuleInstance(), _T("\\2"));
		_AtlModule.DllUnregisterServer();

		if	(SUCCEEDED (lResult))
		{
			AtlRegisterTypeLib (_AtlBaseModule.GetModuleInstance(), _T("\\2"));
		}
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
		if	(SUCCEEDED (lResult))
		{
			AtlUnRegisterTypeLib (_AtlBaseModule.GetModuleInstance(), _T("\\2"));
		}
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
