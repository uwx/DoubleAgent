/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include <Cpl.h>
#include <winsafer.h>
#include <psapi.h>
#include "DaServerApp.h"
#include "DaServer.h"
#include "DaSvrCharacter.h"
#include "DaSvrCommands.h"
#include "DaElevatedSettings.h"
#include "DaGlobalConfig.h"
#include "AgentFiles.h"
#include "DaSvrPropertySheet.h"
#include "DaSvrSettings.h"
#include "DaSvrAudioOutput.h"
#include "DaSvrSpeechInput.h"
#include "DaSvrCommandsWindow.h"
#include "DaSvrCharacterFiles.h"
#include "VoiceCommandsWnd.h"
#include "SapiVoiceCache.h"
#include "ComMessageFilter.h"
#include "Localize.h"
#include "ThreadSecurity.h"
#include "UserSecurity.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "MallocPtr.h"
#include "WerOpt.h"
#include "GuidStr.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#pragma comment (lib,"comctl32.lib")
#pragma warning (disable : 4722)

#ifdef	_DEBUG
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_CROATIAN, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_CZECH, SUBLANG_CZECH_CZECH_REPUBLIC)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_DANISH, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_GERMAN, SUBLANG_GERMAN)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_GREEK, SUBLANG_GREEK_GREECE)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_SPANISH, SUBLANG_SPANISH_MODERN)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_BASQUE, SUBLANG_BASQUE_BASQUE)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_FINNISH, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_FRENCH, SUBLANG_FRENCH)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_HEBREW, SUBLANG_HEBREW_ISRAEL)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_HUNGARIAN, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_ITALIAN, SUBLANG_ITALIAN)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_KOREAN, SUBLANG_KOREAN)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_DUTCH, SUBLANG_DUTCH)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_POLISH, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_PORTUGUESE, SUBLANG_PORTUGUESE)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_ROMANIAN, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_RUSSIAN, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_SLOVAK, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_SLOVENIAN, SUBLANG_DEFAULT)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_SWEDISH, SUBLANG_SWEDISH)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_THAI, SUBLANG_THAI_THAILAND)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_TURKISH, SUBLANG_TURKISH_TURKEY)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
//#define	_DEBUG_LANGUAGE		MAKELANGID (LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#endif

#ifdef	_DEBUG
#define	_DEBUG_NOTIFY_PATH			(GetProfileDebugInt(_T("DebugNotifyPath"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_MODELESS_PROPSHEET	(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF|LogTime)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG				LogNormal
#endif
#define	_LOG_ROOT_PATH					_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME				_T(_SERVER_REGNAME)
#define _LOG_DEF_LOGNAME				_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX						_T("Server  ")
static tPtr <CComAutoCriticalSection>	sLogCriticalSection = new CComAutoCriticalSection;
#define	_LOG_CRITICAL_SECTION			(!sLogCriticalSection?NULL:&sLogCriticalSection->m_sec)
#include "LogAccess.inl"
#include "Log.inl"
#include "LogCrash.inl"
/////////////////////////////////////////////////////////////////////////////

extern "C" int WINAPI _tWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

////////////////////////////////////////////////////////////////////////////

CDaServerModule					_AtlModule;
LPCTSTR __declspec(selectany)	_AtlProfileName = _LOG_SECTION_NAME;
LPCTSTR __declspec(selectany)	_AtlProfilePath = _LOG_ROOT_PATH;

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sCmdLineSettings = _T("Settings");
static LPCTSTR	sCmdLinePage = _T("Page");

const UINT	CDaServerModule::mOptionsChangedMsgId = DA_BROADCAST_OPTIONS_CHANGED;
const UINT	CDaServerModule::mDefaultCharacterChangedMsgId = DA_BROADCAST_DEFCHAR_CHANGED;

////////////////////////////////////////////////////////////////////////////

CDaServerModule::CDaServerModule ()
:	CListeningGlobal (*(CGlobalAnchor*)this),
	CListeningAnchor (*(CListeningGlobal*)this),
	mClientLifetimeTimer (0)
{
	LogCrash_Initialize ();
#ifdef	_DEBUG_LANGUAGE
	SetThreadLocale (MAKELCID (_DEBUG_LANGUAGE, SORT_DEFAULT));
#endif

#ifdef	_DEBUG
	LogStart (GetProfileDebugInt(_T("LogRestart"))!=0);
	LogDebugRuntime ();
#else
	LogStart (false);
#endif
#ifdef	_DEBUG_NOT
	LogProcessSecurity (GetCurrentProcess(), LogIfActive);
#else
	LogProcessUser (GetCurrentProcess(), LogIfActive);
	LogProcessOwner (GetCurrentProcess(), LogIfActive);
	LogProcessIntegrity (GetCurrentProcess(), LogIfActive);
#endif
#ifdef	_DEBUG
	if	(GetProfileDebugInt(_T("LogRestart"))!=0)
	{
		m_dwTimeOut /= 10;
		m_dwPause /= 10;
		LogMessage (LogIfActive, _T("Shutdown Timeout [%u] Pause [%u]"), m_dwTimeOut, m_dwPause);
	}
#endif

#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceCOM.SetLevel (_ATL_DEBUG_INTERFACES);
	atlTraceCOM.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceRefcount.SetLevel (0);
	atlTraceRefcount.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_ATL_DEBUG_QI
	atlTraceQI.SetLevel (_ATL_DEBUG_QI);
	atlTraceQI.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_DEBUG
	atlTraceGeneral.SetLevel (2);
	atlTraceGeneral.SetStatus (ATLTRACESTATUS_ENABLED);
	atlTraceWindowing.SetLevel (2);
	atlTraceWindowing.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
	WerOptOut (false);
}

CDaServerModule::~CDaServerModule ()
{
	CLocalize::FreeMuiModules ();
#if	TRUE // Do it now rather than in superclass destructor so we can log any crashes
	if	(m_bComInitialized)
	{
		UninitializeCom ();
	}
	m_bComInitialized = FALSE;
#endif
	LogStop (LogIfActive);
	LogCrash_Terminate ();
}

////////////////////////////////////////////////////////////////////////////

bool CDaServerModule::ParseCommandLine (LPCTSTR lpCmdLine, HRESULT* pnRetCode)
{
	bool		lRet;
	const TCHAR	lTokens[] = _T("-/");
	LPCTSTR		lToken;
	LPCTSTR		lPageName = NULL;

#ifdef	_DEBUG
	LogMessage (LogNormal|LogTime, _T("Command line [%s]"), lpCmdLine);
#endif
	lRet = __super::ParseCommandLine (lpCmdLine, pnRetCode);

	if	(lRet)
	{
		for	(lToken = FindOneOf (lpCmdLine, lTokens); lToken; lToken = FindOneOf (lToken, lTokens))
		{
			if	(WordCmpI (lToken, sCmdLineSettings) == 0)
			{
				if	(
						(lToken = FindOneOf (lToken, lTokens))
					&&	(WordCmpI (lToken, sCmdLinePage) == 0)
					)
				{
					lPageName = FindOneOf (lToken, _T(" "));
				}

				__try
				{
					_PreMessageLoop (true);
					ShowSettings (lPageName);
					_PostMessageLoop (true);
				}
				__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
				{}

				lRet = false;
				break;
			}
		}
	}
	return lRet;
}

////////////////////////////////////////////////////////////////////////////

HRESULT CDaServerModule::PreMessageLoop (int nShowCmd)
{
	HRESULT	lResult = S_FALSE;
	__try
	{
		_PreMessageLoop (false);
		lResult = __super::PreMessageLoop (nShowCmd);
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}
	return lResult;
}

HRESULT CDaServerModule::PostMessageLoop ()
{
	HRESULT	lResult = S_FALSE;
	__try
	{
		_PostMessageLoop (false);
		lResult = __super::PostMessageLoop ();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}
	return lResult;
}

////////////////////////////////////////////////////////////////////////////

void CDaServerModule::_PreMessageLoop (bool pForModal)
{
	tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
	lInitControls.dwICC = ICC_WIN95_CLASSES|ICC_STANDARD_CLASSES|ICC_LINK_CLASS;
	InitCommonControlsEx (&lInitControls);

	_AtlBaseModule.SetResourceInstance (GetModuleHandle (_T("DaCore")));

	CThreadSecurity::AllowUiPiMessage (mOptionsChangedMsgId);
	CThreadSecurity::AllowUiPiMessage (mDefaultCharacterChangedMsgId);

	if	(!pForModal)
	{
		CListeningGlobal::Startup ();
		CListeningAnchor::Startup (NULL);
		CListeningGlobal::Resume ();
		AddListeningTimer (mClientLifetimeTimer=(UINT)&mClientLifetimeTimer, 5000, this); // Move to monitor thread?

		if	(mMessageFilter = new CComObjectNoLock <CComMessageFilter>)
		{
#if	TRUE // Disable the "Client busy" dialog
			mMessageFilter->SetRetryTimeout ((DWORD)-1);
#endif
#if	TRUE // Disable the "Not Responding" dialog
			mMessageFilter->SetMessageTimeout ((DWORD)-1);
#endif
			LogComErr (LogNormal|LogTime, mMessageFilter->Register ());
		}
	}
}

void CDaServerModule::_PostMessageLoop (bool pForModal)
{
	CoSuspendClassObjects ();

	CListeningAnchor::Shutdown ();
	CListeningGlobal::Shutdown ();
	SafeFreeSafePtr (mSvrPropertySheet);
	SafeFreeSafePtr (mSvrSettings);
	SafeFreeSafePtr (mSvrAudioOutput);
	SafeFreeSafePtr (mSvrSpeechInput);
	SafeFreeSafePtr (mSvrCommandsWindow);
	SafeFreeSafePtr (mSvrCharacterFiles);
	SafeFreeSafePtr (mMessageFilter);
	CSapiVoiceCache::TerminateStaticInstance ();
	CSapiInputCache::TerminateStaticInstance ();
}

////////////////////////////////////////////////////////////////////////////

void CDaServerModule::RunMessageLoop ()
{
	__try
	{
		MSG	lMsg;

		while (GetMessage (&lMsg, 0, 0, 0) > 0)
		{
			if	(
					(lMsg.message == WM_TIMER)
				&&	(lMsg.hwnd == NULL)
				)
			{
				if	(mTimerNotifies.OnTimer ((UINT_PTR)lMsg.wParam))
				{
					continue;
				}
			}
			else
			if	(lMsg.message == WM_HOTKEY)
			{
				LRESULT	lResult;
				if	(ProcessWindowMessage (lMsg.hwnd, lMsg.message, lMsg.wParam, lMsg.lParam, lResult))
				{
					continue;
				}
			}
			else
			if	(
					(lMsg.message == mOptionsChangedMsgId)
				||	(lMsg.message == mDefaultCharacterChangedMsgId)
				)
			{
				MSG		lRemoveMsg;
				LRESULT	lResult;

				while (PeekMessage (&lRemoveMsg, 0, lMsg.message, lMsg.message, PM_REMOVE))
				{}
				if	(ProcessWindowMessage (lMsg.hwnd, lMsg.message, lMsg.wParam, lMsg.lParam, lResult))
				{
					continue;
				}
			}

			if	(mModelessPropSheets.GetCount() > 0)
			{
				INT_PTR				lNdx;
				CAtlPropertySheet*	lPropertySheet;

				for	(lNdx = (INT_PTR)mModelessPropSheets.GetCount()-1; lNdx >= 0; lNdx--)
				{
					if	(
							(lPropertySheet = mModelessPropSheets [lNdx])
						&&	(IsWindow (lPropertySheet->m_hWnd))
						&&	(PropSheet_IsDialogMessage (lPropertySheet->m_hWnd, &lMsg))
						)
					{
#ifdef	_DEBUG_MODELESS_PROPSHEET_NOT
						LogMessage (_DEBUG_MODELESS_PROPSHEET, _T("PropSheet_IsDialogMessage [%p]"), lPropertySheet->m_hWnd);
#endif
						break;
					}
				}
				if	(lNdx >= 0)
				{
					continue;
				}
			}

			if	(lMsg.message == WM_USER)
			{
				_try
				{
					TranslateMessage (&lMsg);
					DispatchMessage (&lMsg);
				}
				__except (LogCrashStack (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
				{
					LogMessage (LogIfActive, _T("  Message [%p] [%8.8X] [(%I64d) (%I64u) (%p)] [(%I64d) (%I64u) (%p)]"), lMsg.hwnd, lMsg.message, lMsg.wParam, lMsg.wParam, lMsg.wParam, lMsg.lParam, lMsg.lParam, lMsg.lParam);
				}
			}
			else
			{
				TranslateMessage (&lMsg);
				DispatchMessage (&lMsg);
			}

			if	(mObjectWasAbandoned)
			{
				mObjectWasAbandoned = false;
				VerifyObjectLifetimes (true);
			}
		}

		_PostMessageLoop (false); // Cleanup before delayed shutdown
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}
}

void CDaServerModule::UninitializeCom ()
{
	__try
	{
		CoFreeAllLibraries ();
	}
	__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
	{}
	__try
	{
		CoUninitialize ();
	}
	__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
	{}
}

CComMessageFilter* CDaServerModule::GetMessageFilter ()
{
	return mMessageFilter;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaServerModule::RegisterServer(BOOL bRegTypeLib, const CLSID* pCLSID)
{
#if	FALSE
	if	(bRegTypeLib)
	{
		_AtlComModule.RegisterTypeLib (_T("\\3"));
	}
#endif
	return __super::RegisterServer (bRegTypeLib, pCLSID);
}

HRESULT CDaServerModule::UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID)
{
#if	FALSE
	if	(bUnRegTypeLib)
	{
		_AtlComModule.UnRegisterTypeLib (_T("\\3"));
	}
#endif
	return __super::UnregisterServer (bUnRegTypeLib, pCLSID);
}

HRESULT CDaServerModule::RegisterAppId ()
{
	HRESULT	lResult = __super::RegisterAppId ();

//
//	Set the launch permissions for the server object allow a low-integrity process to access the server at medium integrity.
//
	if	(
			(SUCCEEDED (lResult))
		&&	(IsWindowsVista_AtLeast ())
		)
	{
		CRegKeyEx		lAppIdKey (CRegKeyEx (HKEY_CLASSES_ROOT, _T("AppID"), true), CGuidStr(__uuidof(DaServer)), false);
		CRegDWord		lSRPTrustLevel (lAppIdKey, _T("SRPTrustLevel"), true);
		CRegBinary		lLaunchPermission (lAppIdKey, _T("LaunchPermission"), true);
		CRegBinary		lAccessPermission (lAppIdKey, _T("AccessPermission"), true);
		CSecurityDesc	lLaunchDescriptor (_T("O:BAG:BAD:(A;;CCDCSW;;;WD)"));
		CSecurityDesc	lAccessDescriptor (_T("O:BAG:BAD:(A;;CCDCSW;;;WD)S:(ML;;NX;;;LW)"));
		DWORD			lDescriptorSize;

		lSRPTrustLevel.SetValue (SAFER_LEVELID_FULLYTRUSTED).Update ();

		lLaunchDescriptor.MakeAbsolute ();
		lDescriptorSize = lLaunchDescriptor.MakeSelfRelative ();
		if	(lDescriptorSize > 0)
		{
			lLaunchPermission.Value().SetCount (lDescriptorSize);
			memcpy (lLaunchPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lLaunchDescriptor.mDescriptor, lDescriptorSize);
			lLaunchPermission.Update ();

			if	(LogIsActive (LogDetails))
			{
				lLaunchDescriptor.DumpAccess (LogDetails|LogTime, true, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
			}
			else
			if	(LogIsActive ())
			{
				LogMessage (LogIfActive|LogTime, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
			}
#ifdef	_DEBUG
			if	(LogIsActive (LogNormal))
			{
				CAtlString	lAccessStr;
				CAtlString	lByteStr;
				INT_PTR		lNdx;

				for	(lNdx = 0; lNdx < (INT_PTR)lLaunchPermission.Value().GetCount(); lNdx++)
				{
					lByteStr.Format (_T("%2.2X"), lLaunchPermission.Value().GetAt(lNdx));
					lAccessStr += lByteStr;
				}
				LogMessage (LogNormal|LogTime, _T("  LaunchPermission [%s]"), lAccessStr);
			}
#endif
		}
		else
		{
			lLaunchPermission.Delete ();
		}

		lAccessDescriptor.MakeAbsolute ();
		lDescriptorSize = lAccessDescriptor.MakeSelfRelative ();
		if	(lDescriptorSize > 0)
		{
			lAccessPermission.Value().SetCount (lDescriptorSize);
			memcpy (lAccessPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lAccessDescriptor.mDescriptor, lDescriptorSize);
			lAccessPermission.Update ();

			if	(LogIsActive (LogDetails))
			{
				lAccessDescriptor.DumpAccess (LogDetails|LogTime, true, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
			}
			else
			if	(LogIsActive ())
			{
				LogMessage (LogIfActive|LogTime, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
			}
#ifdef	_DEBUG
			if	(LogIsActive (LogNormal))
			{
				CAtlString	lAccessStr;
				CAtlString	lByteStr;
				INT_PTR		lNdx;

				for	(lNdx = 0; lNdx < (INT_PTR)lAccessPermission.Value().GetCount(); lNdx++)
				{
					lByteStr.Format (_T("%2.2X"), lAccessPermission.Value().GetAt(lNdx));
					lAccessStr += lByteStr;
				}
				LogMessage (LogNormal|LogTime, _T("  AccessPermission [%s]"), lAccessStr);
			}
#endif
		}
		else
		{
			lAccessPermission.Delete ();
		}
	}

//
//	Set the elevation policy for protected-mode Internet Explorer
//
	if	(SUCCEEDED (lResult))
	{
		CRegKeyEx	lPolicyKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights\\ElevationPolicy"), true);
		CRegKeyEx	lAppKey (lPolicyKey, CGuidStr(__uuidof(DaServer)), false, true);
		CAtlString	lModuleName;
		CAtlString	lModulePath;

		GetModuleFileName (_AtlBaseModule.GetModuleInstance(), lModulePath.GetBuffer(MAX_PATH), MAX_PATH);
		lModuleName = PathFindFileName (lModulePath);
		PathRemoveFileSpec (lModulePath.GetBuffer (lModulePath.GetLength()));
		lModulePath.ReleaseBuffer ();

		CRegString (lAppKey, _T("AppName"), true).Update (lModuleName);
		CRegString (lAppKey, _T("AppPath"), true).Update (lModulePath);
		CRegDWord (lAppKey, _T("Policy"), true).SetValue (3).Update ();
	}

	return lResult;
}

HRESULT CDaServerModule::UnregisterAppId ()
{
//
//	Remove the elevation policy (see above)
//
	{
		CRegKeyEx	lPolicyKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights\\ElevationPolicy"), true);
		CRegKeyEx	lAppKey (lPolicyKey, CGuidStr(__uuidof(DaServer)), false);

		lAppKey.Delete ();
	}

	return __super::UnregisterAppId ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaSvrPropertySheet* CDaServerModule::GetSvrPropertySheet (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrPropertySheet)
		&&	(pCreate)
		)
	{
		mSvrPropertySheet = DaSvrPropertySheet::CreateInstance (pClientMutexName);
	}
	return mSvrPropertySheet;
}

void CDaServerModule::OnDeleteSvrPropertySheet (DaSvrPropertySheet* pSvrPropertySheet)
{
	if	(pSvrPropertySheet == mSvrPropertySheet)
	{
		mSvrPropertySheet.Detach ();
	}
	mModelessPropSheets.Remove (pSvrPropertySheet);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSettings* CDaServerModule::GetSvrSettings (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrSettings)
		&&	(pCreate)
		)
	{
		mSvrSettings = DaSvrSettings::CreateInstance (pClientMutexName);
	}
	return mSvrSettings;
}

void CDaServerModule::OnDeleteSvrSettings (DaSvrSettings* pSvrSettings)
{
	if	(pSvrSettings == mSvrSettings)
	{
		mSvrSettings.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

DaSvrAudioOutput* CDaServerModule::GetSvrAudioOutput (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrAudioOutput)
		&&	(pCreate)
		)
	{
		mSvrAudioOutput = DaSvrAudioOutput::CreateInstance (pClientMutexName);
	}
	return mSvrAudioOutput;
}

void CDaServerModule::OnDeleteSvrAudioOutput (DaSvrAudioOutput* pSvrAudioOutput)
{
	if	(pSvrAudioOutput == mSvrAudioOutput)
	{
		mSvrAudioOutput.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechInput* CDaServerModule::GetSvrSpeechInput (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrSpeechInput)
		&&	(pCreate)
		)
	{
		mSvrSpeechInput = DaSvrSpeechInput::CreateInstance (pClientMutexName);
	}
	return mSvrSpeechInput;
}

void CDaServerModule::OnDeleteSvrSpeechInput (DaSvrSpeechInput* pSvrSpeechInput)
{
	if	(pSvrSpeechInput == mSvrSpeechInput)
	{
		mSvrSpeechInput.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommandsWindow* CDaServerModule::GetSvrCommandsWindow (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrCommandsWindow)
		&&	(pCreate)
		)
	{
		mSvrCommandsWindow = DaSvrCommandsWindow::CreateInstance (CListeningGlobal::GetVoiceCommandsWnd (true), pClientMutexName);
	}
	return mSvrCommandsWindow;
}

void CDaServerModule::OnDeleteSvrCommandsWindow (DaSvrCommandsWindow* pSvrCommandsWindow)
{
	if	(pSvrCommandsWindow == mSvrCommandsWindow)
	{
		mSvrCommandsWindow.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacterFiles* CDaServerModule::GetSvrCharacterFiles (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrCharacterFiles)
		&&	(pCreate)
		)
	{
		mSvrCharacterFiles = DaSvrCharacterFiles::CreateInstance (pClientMutexName);
	}
	return mSvrCharacterFiles;
}

void CDaServerModule::OnDeleteSvrCharacterFiles (DaSvrCharacterFiles* pSvrCharacterFiles)
{
	if	(pSvrCharacterFiles == mSvrCharacterFiles)
	{
		mSvrCharacterFiles.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::OnTimerNotify (CTimerNotify* pTimerNotify, UINT_PTR pTimerId)
{
	if	(pTimerId == mClientLifetimeTimer)
	{
		CSapiVoiceCache::CleanupStaticInstance ();
		VerifyObjectLifetimes ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::OnShowModelessPropertySheet (CAtlPropertySheet* pPropertySheet)
{
#ifdef	_DEBUG_MODELESS_PROPSHEET
	LogMessage (_DEBUG_MODELESS_PROPSHEET, _T("CDaServerModule::OnShowModelessPropertySheet [%p] [%s]"), pPropertySheet, AtlTypeName(pPropertySheet));
#endif
	mModelessPropSheets.AddUnique (pPropertySheet);
}

void CDaServerModule::OnHideModelessPropertySheet (CAtlPropertySheet* pPropertySheet)
{
#ifdef	_DEBUG_MODELESS_PROPSHEET
	LogMessage (_DEBUG_MODELESS_PROPSHEET, _T("CDaServerModule::OnHideModelessPropertySheet [%p] [%s]"), pPropertySheet, AtlTypeName(pPropertySheet));
#endif
	mModelessPropSheets.Remove (pPropertySheet);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CDaServerModule::OnThreadHotKey (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(CListeningGlobal::OnHotKey (wParam, lParam))
	{
		bHandled = TRUE;
	}
	return 0;
}

LRESULT CDaServerModule::OnBroadcastOptionsChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_OptionsChanged ();
	return 0;
}

LRESULT CDaServerModule::OnBroadcastDefaultCharacterChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CEventGlobal::_DefaultCharacterChanged ();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::_CharacterLoaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaServerModule::_CharacterLoaded [%d]"), pCharID);
#endif
	StartActionTrace (pCharID);
	CListeningGlobal::_CharacterLoaded (pCharID);
	CEventGlobal::_CharacterLoaded (pCharID);
}

void CDaServerModule::_CharacterUnloaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaServerModule::_CharacterUnloaded [%d]"), pCharID);
#endif
	CListeningGlobal::_CharacterUnloaded (pCharID);
	CEventGlobal::_CharacterUnloaded (pCharID);
	StopActionTrace (pCharID);

	try
	{
		EmptyWorkingSet (GetCurrentProcess ());
	}
	catch AnyExceptionSilent
}

void CDaServerModule::_CharacterNameChanged (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaServerModule::_CharacterNameChanged [%d]"), pCharID);
#endif
	CListeningGlobal::_CharacterNameChanged (pCharID);
	CEventGlobal::_CharacterNameChanged (pCharID);
}

void CDaServerModule::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaServerModule::_CharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
	CListeningGlobal::_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
	CEventGlobal::_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
}

void CDaServerModule::_AppActivated (bool pActive)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaServerModule::_AppActivated [%u]"), pActive);
#endif
	// Intentionally ignored - the server is always active
}

void CDaServerModule::_OptionsChanged ()
{
	CListeningGlobal::_OptionsChanged ();
	CEventGlobal::_OptionsChanged ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaServerModule::ShowSettings (LPCTSTR pStartPage)
{
	bool			lRet = false;
	UINT			lErrorMode = SetErrorMode (SEM_NOOPENFILEERRORBOX);
	CModuleHandle	lSettingsModule;
	APPLET_PROC		lCplProc = NULL;
	CAtlString		lStartPage (pStartPage);

	try
	{
		SetLastError (0);
		if	(lSettingsModule = LoadLibrary (_T(_SHELL_FILENAME)))
		{
			lCplProc = (APPLET_PROC) GetProcAddress (lSettingsModule, "CPlApplet");
		}
		else
		{
			LogWinErr (LogNormal|LogTime, GetLastError (), _T("Load ") _T(_SHELL_FILENAME));
		}
	}
	catch AnyExceptionSilent

	SetErrorMode (lErrorMode);

	if	(lCplProc)
	{
		try
		{
			if	((*lCplProc) (NULL, CPL_INIT, 0, 0))
			{
				if	(!lStartPage.IsEmpty ())
				{
					(*lCplProc) (NULL, CPL_STARTWPARMS, 0, (LPARAM)(LPCTSTR)lStartPage);
				}
				(*lCplProc) (NULL, CPL_DBLCLK, 0, 0);
				(*lCplProc) (NULL, CPL_EXIT, 0, 0);
			}
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaServerModule::TraceCharacterAction (long pCharID, LPCTSTR pAction, LPCTSTR pFormat, ...)
{
	bool	lRet = false;
#ifdef	_TRACE_CHARACTER_ACTIONS
	if	(
			(pCharID > 0)
		?	(mActionTraceMap.Lookup (pCharID) != NULL)
		:	(mActionTraceMap.GetCount() > 0)
		)
	{
		try
		{
			tS <SYSTEMTIME>	lTraceTime;
			CAtlString		lTraceDetail;
			CAtlString		lTraceMsg;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTraceDetail.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTraceDetail.ReleaseBuffer ();
			}
			if	(!lTraceDetail.IsEmpty ())
			{
				lTraceDetail.Insert (0, _T('\t'));
			}
			GetLocalTime (&lTraceTime);
			lTraceMsg.Format (_T("%4.4u-%2.2u-%2.2u\t%2.2u:%2.2u:%2.2u:%3.3u\t%s%s\r\n"), lTraceTime.wYear, lTraceTime.wMonth, lTraceTime.wDay, lTraceTime.wHour, lTraceTime.wMinute, lTraceTime.wSecond, lTraceTime.wMilliseconds, pAction, lTraceDetail);

			if	(pCharID > 0)
			{
				LogWrite (lTraceMsg, mActionTraceMap [pCharID]);
			}
			else
			{
				POSITION	lPos;

				for	(lPos = mActionTraceMap.GetStartPosition(); lPos;)
				{
					LogWrite (lTraceMsg, mActionTraceMap.GetNextValue (lPos));
				}
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaServerModule::StartActionTrace (long pCharID)
{
	bool	lRet = false;
#ifdef	_TRACE_CHARACTER_ACTIONS
	try
	{
		CDaCmnCharacter*	lCharacter;

		if	(
				(CRegDWord (CRegKeyEx (CRegKeyEx (HKEY_CURRENT_USER, gProfileKeyDa, true), _T(_SERVER_REGNAME), true), _T("ActionTrace")).Value() != 0)
			&&	(lCharacter = _GetCharacter (pCharID, *this))
			)
		{
			CAtlString		lTraceFilePath;
			CAtlString		lTraceFileName;
			tS <SYSTEMTIME>	lTraceTime;

			if	(gLogFileName[0] == 0)
			{
				UINT lLogLevel = LogIfActive;
				LogControl (gLogFileName, lLogLevel);
			}

			GetLocalTime (&lTraceTime);
			lTraceFilePath = lCharacter->GetFile()->FileName;
			PathStripPath (lTraceFilePath.GetBuffer (lTraceFilePath.GetLength ()));
			PathRemoveExtension (lTraceFilePath.GetBuffer (lTraceFilePath.GetLength ()));
#ifdef	_DEBUG
			lTraceFileName.Format (_T("%s(%d) %4.4u-%2.2u-%2.2u"), lTraceFilePath, pCharID, lTraceTime.wYear, lTraceTime.wMonth, lTraceTime.wDay);
#else
			lTraceFileName.Format (_T("%s(%d) %4.4u-%2.2u-%2.2u %2.2u-%2.2u-%2.2u"), lTraceFilePath, pCharID, lTraceTime.wYear, lTraceTime.wMonth, lTraceTime.wDay, lTraceTime.wHour, lTraceTime.wMinute, lTraceTime.wSecond);
#endif
			lTraceFilePath = gLogFileName;
			PathRemoveFileSpec (lTraceFilePath.GetBuffer (MAX_PATH));
			PathAppend (lTraceFilePath.GetBuffer (MAX_PATH), lTraceFileName);
			PathAddExtension (lTraceFilePath.GetBuffer (MAX_PATH), _T(".txt"));
			lTraceFilePath.ReleaseBuffer ();

			mActionTraceMap.SetAt (pCharID, lTraceFilePath);
			try
			{
				UINT lErrorMode = SetErrorMode (SEM_NOOPENFILEERRORBOX);
				DeleteFile (lTraceFilePath);
				SetErrorMode (lErrorMode);
			}
			catch AnyExceptionSilent

			lRet = true;
		}
	}
	catch AnyExceptionDebug
#endif
	return lRet;
}

bool CDaServerModule::StopActionTrace (long pCharID)
{
	bool	lRet = false;
#ifdef	_TRACE_CHARACTER_ACTIONS
	try
	{
		if	(mActionTraceMap.Lookup (pCharID) != NULL)
		{
			mActionTraceMap.RemoveKey (pCharID);
			lRet = true;
		}
	}
	catch AnyExceptionDebug
#endif
	return lRet;
}
