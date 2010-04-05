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
#include <Cpl.h>
#include <winsafer.h>
#include "DaServerApp.h"
#include "ServerNotify.h"
#include "DaServer.h"
#include "DaSvrCharacter.h"
#include "DaSvrCommands.h"
#include "DaElevatedSettings.h"
#include "DaGlobalConfig.h"
#include "AgentFiles.h"
#include "DaSvrPropertySheet.h"
#include "DaSvrAudioOutput.h"
#include "DaSvrSpeechInput.h"
#include "DaSvrCommandsWindow.h"
#include "PropSheetCharSel.h"
#include "DaSvrCharacterFiles.h"
#include "VoiceCommandsWnd.h"
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
#define	_DEBUG_MODELESS_PROPSHEET	(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG				LogNormal
#endif
#define	_LOG_ROOT_PATH					_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME				_T(_SERVER_REGNAME)
#define _LOG_DEF_LOGNAME				_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX						_T("Srvr ")
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
:	mNextCharID (256),
	mLastHotKey (0),
	mClientLifetimeTimer (0)
{
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
	m_dwPause = 100;
#endif

	WerOptOut (false);
}

CDaServerModule::~CDaServerModule ()
{
	CLocalize::FreeMuiModules ();
	LogStop (LogIfActive);
}

////////////////////////////////////////////////////////////////////////////

bool CDaServerModule::ParseCommandLine (LPCTSTR lpCmdLine, HRESULT* pnRetCode)
{
	bool		lRet;
	const TCHAR	lTokens[] = _T("-/");
	LPCTSTR		lToken;
	LPCTSTR		lPageName = NULL;

#ifdef	_DEBUG
	LogMessage (LogNormal, _T("Command line [%s]"), lpCmdLine);
#endif
	lRet = CAtlExeModuleT<CDaServerModule>::ParseCommandLine (lpCmdLine, pnRetCode);

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
		lResult = CAtlExeModuleT<CDaServerModule>::PreMessageLoop (nShowCmd);
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
		lResult = CAtlExeModuleT<CDaServerModule>::PostMessageLoop ();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}
	return lResult;
}

////////////////////////////////////////////////////////////////////////////

void CDaServerModule::_PreMessageLoop (bool pForModal)
{
	tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
	lInitControls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx (&lInitControls);

	_AtlBaseModule.SetResourceInstance (GetModuleHandle (_T("DaCore")));

	CThreadSecurity::AllowUiPiMessage (mOptionsChangedMsgId);
	CThreadSecurity::AllowUiPiMessage (mDefaultCharacterChangedMsgId);
	
	if	(!pForModal)
	{
		CDaSpeechInputConfig::RegisterHotKey (true);
		AddTimerNotify (mClientLifetimeTimer=(UINT)&mClientLifetimeTimer, 5000, this); // Move to monitor thread
		
		if	(mMessageFilter = new CComObjectNoLock <CComMessageFilter>)
		{
		}
	}
}

void CDaServerModule::_PostMessageLoop (bool pForModal)
{
	try
	{
		mTimerNotifies.DeleteAll ();
	}
	catch AnyExceptionSilent

	CDaSpeechInputConfig::RegisterHotKey (false);
	SafeFreeSafePtr (mSvrPropertySheet);
	SafeFreeSafePtr (mSvrAudioOutput);
	SafeFreeSafePtr (mSvrSpeechInput);
	SafeFreeSafePtr (mSvrCommandsWindow);
	SafeFreeSafePtr (mPropSheetCharSel);
	SafeFreeSafePtr (mSvrCharacterFiles);
	SafeFreeSafePtr (mVoiceCommandsWnd);
	SafeFreeSafePtr (mMessageFilter);
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
				CAtlPropertySheet *	lPropertySheet;
				
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
			
			TranslateMessage (&lMsg);
			DispatchMessage (&lMsg);
		}

		_PostMessageLoop (false); // Cleanup before delayed shutdown
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#if	 FALSE

BOOL CDaServerApp::InitInstance()
{
	BOOL	lRet = FALSE;

	__try
	{
		_InitInstance ();

		if	(m_pCmdInfo->m_nShellCommand == m_pCmdInfo->AppRegister)
		{
			RegisterServer ();
		}
		else
#if	(_MFC_VER >= 0x0800)
		if	(m_pCmdInfo->m_nShellCommand == m_pCmdInfo->AppUnregister)
#else
		if	(
				(m_pCmdInfo->m_nShellCommand == m_pCmdInfo->FileNothing)
			&&	(m_pCmdInfo->m_strFileName.IsEmpty ())
			)
#endif
		{
			UnregisterServer ();
		}
		else
		if	(
				(m_pCmdInfo->m_nShellCommand == m_pCmdInfo->FileNothing)
			&&	(m_pCmdInfo->m_strFileName.CompareNoCase (sCmdLineSettings) == 0)
			)
		{
			ShowSettings (m_pCmdInfo->m_strPortName);
		}
		else
		if	(
				(m_pCmdInfo->m_bRunEmbedded)
			||	(m_pCmdInfo->m_bRunAutomated)
			)
		{
			COleMessageFilterEx *	lMessageFilter;

			COleObjectFactory::RegisterAll ();
			CThreadSecurity::AllowUiPiMessage (mOptionsChangedMsgId);
			CThreadSecurity::AllowUiPiMessage (mDefaultCharacterChangedMsgId);
			CDaSpeechInputConfig::RegisterHotKey (true);
			WerOptOut (false);

			if	(lMessageFilter = COleMessageFilterEx::SetAppFilter ())
			{
				lMessageFilter->SafeEnableNotResponding (0);
				lMessageFilter->SafeEnableBusy (0);
#ifdef	_DEBUG_NOT
				lMessageFilter->mLogLevelDlg = LogNormal|LogHighVolume;
#endif
			}
			lRet = TRUE;
		}
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#endif

////////////////////////////////////////////////////////////////////////////

HRESULT CDaServerModule::RegisterAppId ()
{
	HRESULT	lResult = CAtlExeModuleT<CDaServerModule>::RegisterAppId ();

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
			lLaunchPermission.Value().SetSize (lDescriptorSize);
			memcpy (lLaunchPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lLaunchDescriptor.mDescriptor, lDescriptorSize);
			lLaunchPermission.Update ();

			if	(LogIsActive (LogDetails))
			{
				lLaunchDescriptor.DumpAccess (LogDetails, true, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
			}
			else
			if	(LogIsActive ())
			{
				LogMessage (LogIfActive, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
			}
#ifdef	_DEBUG
			if	(LogIsActive (LogNormal))
			{
				CString	lAccessStr;
				CString	lByteStr;
				INT_PTR	lNdx;

				for	(lNdx = 0; lNdx <= lLaunchPermission.Value().GetUpperBound(); lNdx++)
				{
					lByteStr.Format (_T("%2.2X"), lLaunchPermission.Value().GetAt(lNdx));
					lAccessStr += lByteStr;
				}
				LogMessage (LogNormal, _T("  LaunchPermission [%s]"), lAccessStr);
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
			lAccessPermission.Value().SetSize (lDescriptorSize);
			memcpy (lAccessPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lAccessDescriptor.mDescriptor, lDescriptorSize);
			lAccessPermission.Update ();

			if	(LogIsActive (LogDetails))
			{
				lAccessDescriptor.DumpAccess (LogDetails, true, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
			}
			else
			if	(LogIsActive ())
			{
				LogMessage (LogIfActive, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
			}
#ifdef	_DEBUG
			if	(LogIsActive (LogNormal))
			{
				CString	lAccessStr;
				CString	lByteStr;
				INT_PTR	lNdx;

				for	(lNdx = 0; lNdx <= lAccessPermission.Value().GetUpperBound(); lNdx++)
				{
					lByteStr.Format (_T("%2.2X"), lAccessPermission.Value().GetAt(lNdx));
					lAccessStr += lByteStr;
				}
				LogMessage (LogNormal, _T("  AccessPermission [%s]"), lAccessStr);
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
		CString		lModuleName;
		CString		lModulePath;

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
	
	return CAtlExeModuleT<CDaServerModule>::UnregisterAppId ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaSvrPropertySheet * CDaServerModule::GetSvrPropertySheet (bool pCreate, LPCTSTR pClientMutexName)
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

void CDaServerModule::OnDeleteSvrPropertySheet (DaSvrPropertySheet * pSvrPropertySheet)
{
	if	(pSvrPropertySheet == mSvrPropertySheet)
	{
		mSvrPropertySheet.Detach ();
	}
	mModelessPropSheets.Remove (pSvrPropertySheet);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrAudioOutput * CDaServerModule::GetSvrAudioOutput (bool pCreate, LPCTSTR pClientMutexName)
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

void CDaServerModule::OnDeleteSvrAudioOutput (DaSvrAudioOutput * pSvrAudioOutput)
{
	if	(pSvrAudioOutput == mSvrAudioOutput)
	{
		mSvrAudioOutput.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechInput * CDaServerModule::GetSvrSpeechInput (bool pCreate, LPCTSTR pClientMutexName)
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

void CDaServerModule::OnDeleteSvrSpeechInput (DaSvrSpeechInput * pSvrSpeechInput)
{
	if	(pSvrSpeechInput == mSvrSpeechInput)
	{
		mSvrSpeechInput.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommandsWindow * CDaServerModule::GetSvrCommandsWindow (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrCommandsWindow)
		&&	(pCreate)
		)
	{
		mSvrCommandsWindow = DaSvrCommandsWindow::CreateInstance (GetVoiceCommandsWnd (true), pClientMutexName);
	}
	return mSvrCommandsWindow;
}

void CDaServerModule::OnDeleteSvrCommandsWindow (DaSvrCommandsWindow * pSvrCommandsWindow)
{
	if	(pSvrCommandsWindow == mSvrCommandsWindow)
	{
		mSvrCommandsWindow.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CPropSheetCharSel * CDaServerModule::GetPropSheetCharSel (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mPropSheetCharSel)
		&&	(pCreate)
		)
	{
		mPropSheetCharSel = CPropSheetCharSel::CreateInstance (NULL, pClientMutexName);
	}
	return mPropSheetCharSel;
}

void CDaServerModule::OnDeletePropSheetCharSel (CPropSheetCharSel * pPropSheetCharSel)
{
	if	(pPropSheetCharSel == mPropSheetCharSel)
	{
		mPropSheetCharSel.Detach ();
	}
	mModelessPropSheets.Remove (pPropSheetCharSel);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacterFiles * CDaServerModule::GetSvrCharacterFiles (bool pCreate, LPCTSTR pClientMutexName)
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

void CDaServerModule::OnDeleteSvrCharacterFiles (DaSvrCharacterFiles * pSvrCharacterFiles)
{
	if	(pSvrCharacterFiles == mSvrCharacterFiles)
	{
		mSvrCharacterFiles.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaSvrCharacter * CDaServerModule::GetAppCharacter (long pCharID)
{
	DaSvrCharacter *	lRet = NULL;

	if	(pCharID > 0)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				DaSvrCharacter *					lCharacter;

				if	(GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
							&&	(lCharacter->GetCharID() == pCharID)
							)
						{
							lRet = lCharacter;
						}
					}
				}
				if	(lRet)
				{
					break;
				}
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

DaSvrCharacter * CDaServerModule::GetListenCharacter ()
{
	DaSvrCharacter *	lListenCharacter = NULL;

	try
	{
		DaSvrCharacter *	lActiveCharacter = NULL;
		DaSvrCharacter *	lClientActiveCharacter = NULL;
		INT_PTR				lFileNdx;
		CAgentFile *		lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			DaSvrCharacter *					lCharacter;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
					{
						if	(lCharacter->IsInputActive ())
						{
							lActiveCharacter = lCharacter;
							break;
						}
						else
						if	(
								(!lClientActiveCharacter)
							&&	(lCharacter->IsClientActive ())
							)
						{
							lClientActiveCharacter = lCharacter;
						}
					}
				}
			}
			if	(lActiveCharacter)
			{
				break;
			}
		}

		if	(lActiveCharacter)
		{
			lListenCharacter = lActiveCharacter;
		}
		else
		{
			lListenCharacter = lClientActiveCharacter;
		}
	}
	catch AnyExceptionDebug

	return lListenCharacter;
}

/////////////////////////////////////////////////////////////////////////////

CVoiceCommandsWnd * CDaServerModule::GetVoiceCommandsWnd (bool pCreate, long pCharID)
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDaServerApp::GetVoiceCommandsWnd"));
	}
#endif
	if	(
			(!mVoiceCommandsWnd)
		&&	(pCreate)
		&&	(mVoiceCommandsWnd = CVoiceCommandsWnd::CreateInstance ())
		)
	{
		if	(mVoiceCommandsWnd->Create ())
		{
			SetVoiceCommandClients (-1);
			SetVoiceCommandNames (-1);
		}
		else
		{
			SafeFreeSafePtr (mVoiceCommandsWnd);
		}
	}
	if	(
			(mVoiceCommandsWnd)
		&&	(pCharID > 0)
		&&	(mVoiceCommandsWnd->GetCharID() != pCharID)
		)
	{
		DaSvrCharacter *	lCharacter;
		DaSvrCommands *	lCommands = NULL;
		BSTR				lName = NULL;

		if	(lCharacter = GetAppCharacter (pCharID))
		{
			lName = lCharacter->GetName ();
			lCommands = lCharacter->GetCommandsObj (true);
		}
		mVoiceCommandsWnd->SetCharacter (pCharID, CAtlString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
	}
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDaServerApp::GetVoiceCommandsWnd Done"));
	}
#endif
	return mVoiceCommandsWnd;
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::SetVoiceCommandClients (long pCharID)
{
	if	(mVoiceCommandsWnd)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				DaSvrCharacter *					lCharacter;

				if	(GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							)
						{
							mVoiceCommandsWnd->SetCharacterClient (lCharacter->GetCharID(), lCharacter->GetActiveClient());
							if	(pCharID > 0)
							{
								break;
							}
						}
					}
					if	(lClientNdx >= 0)
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void CDaServerModule::SetVoiceCommandNames (long pCharID)
{
	if	(mVoiceCommandsWnd)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				DaSvrCharacter *					lCharacter;
				DaSvrCommands *					lCommands;
				BSTR								lName;

				if	(GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							&&	(lName = lCharacter->GetName())
							)
						{
							lCommands = lCharacter->GetCommandsObj (true);
							mVoiceCommandsWnd->SetCharacterName (lCharacter->GetCharID(), CAtlString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
							if	(pCharID > 0)
							{
								break;
							}
						}
					}
					if	(lClientNdx >= 0)
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaServerModule::AddTimerNotify (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink)
{
	bool			lRet = false;
	CTimerNotify *	lTimer;

	if	(
			(pTimerId != 0)
		&&	(pNotifySink != NULL)
		&&	(pInterval >= USER_TIMER_MINIMUM)
		&&	(mTimerNotifies.FindTimer (pTimerId) < 0)
		&&	(lTimer = new CTimerNotify (pTimerId, pNotifySink))
		)
	{
		mTimerNotifies.Add (lTimer);

		if	(lTimer->StartTimer (NULL, pInterval))
		{
			lRet = true;
		}
		else
		{
			mTimerNotifies.Remove (lTimer);
		}
	}
	return lRet;
}

bool CDaServerModule::DelTimerNotify (UINT_PTR pTimerId)
{
	bool				lRet = false;
	tPtr <CTimerNotify>	lTimer;

	if	(lTimer = mTimerNotifies.GetTimer (pTimerId))
	{
		mTimerNotifies.Remove (lTimer);
		lTimer->StopTimer (NULL);
		lRet = true;
	}
	return lRet;
}

bool CDaServerModule::HasTimerNotify (UINT_PTR pTimerId)
{
	return (mTimerNotifies.FindTimer (pTimerId) >= 0);
}

CTimerNotify * CDaServerModule::GetTimerNotify (UINT_PTR pTimerId)
{
	return mTimerNotifies.GetTimer (pTimerId);
}

void CDaServerModule::OnTimerNotify (CTimerNotify * pTimerNotify, UINT_PTR pTimerId)
{
//TODO Move to monitor thread?
	if	(pTimerId == mClientLifetimeTimer)
	{
		VerifyObjectLifetimes ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::OnShowModelessPropertySheet (CAtlPropertySheet * pPropertySheet)
{
#ifdef	_DEBUG_MODELESS_PROPSHEET	
	LogMessage (_DEBUG_MODELESS_PROPSHEET, _T("CDaServerModule::OnShowModelessPropertySheet [%p] [%s]"), pPropertySheet, ObjTypeName(pPropertySheet));
#endif	
	mModelessPropSheets.AddUnique (pPropertySheet);
}

void CDaServerModule::OnHideModelessPropertySheet (CAtlPropertySheet * pPropertySheet)
{
#ifdef	_DEBUG_MODELESS_PROPSHEET	
	LogMessage (_DEBUG_MODELESS_PROPSHEET, _T("CDaServerModule::OnHideModelessPropertySheet [%p] [%s]"), pPropertySheet, ObjTypeName(pPropertySheet));
#endif	
	mModelessPropSheets.Remove (pPropertySheet);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CDaServerModule::OnThreadHotKey (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if	(wParam == (WPARAM) CDaSpeechInputConfig::mHotKeyRegisterId)
	{
		mLastHotKey = HIWORD (lParam);
		if	(LOWORD (lParam) & MOD_ALT)
		{
			mLastHotKey |= VK_MENU << 8;
		}
		if	(LOWORD (lParam) & MOD_CONTROL)
		{
			mLastHotKey |= VK_CONTROL << 16;
		}
		if	(LOWORD (lParam) & MOD_SHIFT)
		{
			mLastHotKey |= VK_SHIFT << 24;
		}

		try
		{
			DaSvrCharacter *	lCharacter = GetListenCharacter ();

			if	(lCharacter)
			{
				LogComErr (LogDetails, lCharacter->StartListening (false));
			}
		}
		catch AnyExceptionDebug
	}
	return 0;
}

bool CDaServerModule::IsHotKeyStillPressed () const
{
	if	(
			(LOBYTE (LOWORD (mLastHotKey)))
		&&	(GetAsyncKeyState (LOBYTE (LOWORD (mLastHotKey))) < 0)
		&&	(
				(!HIBYTE (LOWORD (mLastHotKey)))
			||	(GetAsyncKeyState (HIBYTE (LOWORD (mLastHotKey))) < 0)
			)
		&&	(
				(!LOBYTE (HIWORD (mLastHotKey)))
			||	(GetAsyncKeyState (LOBYTE (HIWORD (mLastHotKey))) < 0)
			)
		&&	(
				(!HIBYTE (HIWORD (mLastHotKey)))
			||	(GetAsyncKeyState (HIBYTE (HIWORD (mLastHotKey))) < 0)
			)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CDaServerModule::OnBroadcastOptionsChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	_OnOptionsChanged ();
	return 0;
}

LRESULT CDaServerModule::OnBroadcastDefaultCharacterChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	_OnDefaultCharacterChanged ();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::_OnOptionsChanged ()
{
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;

		CDaSpeechInputConfig::RegisterHotKey (true);

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_OptionsChanged ();
		}
	}
	catch AnyExceptionDebug
}

void CDaServerModule::_OnDefaultCharacterChanged ()
{
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;
		tPtr <CAgentFile>	lFile;
		CAtlString			lDefCharPath ((BSTR)CAgentFiles::GetDefCharPath());

		if	(
				(!lDefCharPath.IsEmpty ())
			&&	(lFile = CAgentFile::CreateInstance())
			&&	(SUCCEEDED (lFile->Open (lDefCharPath)))
			)
		{
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->_DefaultCharacterChanged (lFile->GetGuid());
			}
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaServerModule::_OnCharacterLoaded (long pCharID)
{
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;

		StartActionTrace (pCharID);
		SetVoiceCommandClients (pCharID);
		SetVoiceCommandNames (pCharID);

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterLoaded (pCharID);
		}
	}
	catch AnyExceptionSilent
}

void CDaServerModule::_OnCharacterUnloaded (long pCharID)
{
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;

		if	(mVoiceCommandsWnd)
		{
			mVoiceCommandsWnd->RemoveCharacter (pCharID);
		}

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterUnloaded (pCharID);
		}
		StopActionTrace (pCharID);
	}
	catch AnyExceptionSilent
}

void CDaServerModule::_OnCharacterNameChanged (long pCharID)
{
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;

		SetVoiceCommandNames (pCharID);

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterNameChanged (pCharID);
		}
	}
	catch AnyExceptionSilent
}

void CDaServerModule::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;

		if	(pActiveCharID > 0)
		{
			SetVoiceCommandClients (pActiveCharID);
		}
		if	(pInactiveCharID > 0)
		{
			SetVoiceCommandClients (pInactiveCharID);
		}

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
		}
	}
	catch AnyExceptionSilent
}

void CDaServerModule::OnCharacterListening (long pCharID, bool pListening, long pCause)
{
//
//	This notification is slightly different in that it's sent to all of a character's
//	clients rather than just the active client.
//
	try
	{
		int					lNotifyNdx;
		_IServerNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterListening (pCharID, pListening, pCause);
		}
	}
	catch AnyExceptionSilent
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
			LogWinErr (LogNormal, GetLastError (), _T("Load ") _T(_SHELL_FILENAME));
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
		DaSvrCharacter *	lCharacter;

		if	(
				(CRegDWord (CRegKeyEx (CRegKeyEx (HKEY_CURRENT_USER, gProfileKeyDa, true), _T(_SERVER_REGNAME), true), _T("ActionTrace")).Value() != 0)
			&&	(lCharacter = GetAppCharacter (pCharID))
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
			lTraceFilePath = lCharacter->GetFile()->GetFileName();
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
