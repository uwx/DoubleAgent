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
#include "DaServer.h"
#include "DaCore.h"
#include "DaAgentNotify.h"
#include "DaAgent.h"
#include "DaAgentCharacter.h"
#include "DaAgentCommands.h"
#include "DaElevatedSettings.h"
#include "DaGlobalConfig.h"
#include "AgentFiles.h"
#include "DaAgentPropertySheet.h"
#include "DaAgentAudioOutputProperties.h"
#include "DaAgentSpeechInputProperties.h"
#include "DaAgentCommandWindow.h"
#include "PropSheetCharSel.h"
#include "DaSvrCharacterFiles.h"
#include "VoiceCommandsWnd.h"
#include "CmdLineInfoEx.h"
#include "Localize.h"
#include "ThreadSecurity.h"
#include "UserSecurity.h"
#include "OleMessageFilterEx.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "MallocPtr.h"
#include "WerOpt.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#pragma warning (disable : 4722)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
//#define	_TRACE_RESOURCES	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG		LogNormal
#endif
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T(_SERVER_REGNAME)
#define _LOG_DEF_LOGNAME		_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX				_T("Srvr ")
static tPtr <CCriticalSection>	sLogCriticalSection = new CCriticalSection;
#define	_LOG_CRITICAL_SECTION	(!sLogCriticalSection?NULL:(CRITICAL_SECTION*)(*sLogCriticalSection))
#include "LogAccess.inl"
#include "Log.inl"
#include "LogCrash.inl"
/////////////////////////////////////////////////////////////////////////////

const GUID gDaTypeLibId = __uuidof(DaServerTypeLib);
const WORD gDaTypeLibVerMajor = _SERVER_VER_MAJOR;
const WORD gDaTypeLibVerMinor = _SERVER_VER_MINOR;

const GUID gDaMsTypeLibId = __uuidof(AgentServerTypeLib);
const WORD gDaMsTypeLibVerMajor = 2;
const WORD gDaMsTypeLibVerMinor = 0;

////////////////////////////////////////////////////////////////////////////

class CServerCmdLine : public CCmdLineInfoEx
{
public:
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
};

static LPCTSTR	sCmdLineSettings = _T("Settings");
static LPCTSTR	sCmdLinePage = _T("Page");

const UINT	CDaServerApp::mOptionsChangedMsgId = DA_BROADCAST_OPTIONS_CHANGED;
const UINT	CDaServerApp::mDefaultCharacterChangedMsgId = DA_BROADCAST_DEFCHAR_CHANGED;

////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CDaServerApp, CWinApp)

BEGIN_MESSAGE_MAP(CDaServerApp, CWinApp)
	//{{AFX_MSG_MAP(CDaServerApp)
	ON_THREAD_MESSAGE(WM_HOTKEY, OnThreadHotKey)
	ON_REGISTERED_THREAD_MESSAGE(mOptionsChangedMsgId, OnBroadcastOptionsChanged)
	ON_REGISTERED_THREAD_MESSAGE(mDefaultCharacterChangedMsgId, OnBroadcastDefaultCharacterChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDaServerApp gApp;

////////////////////////////////////////////////////////////////////////////

CDaServerApp::CDaServerApp()
:	CWinApp (_T(_DOUBLEAGENT_NAME)),
	mNextCharID (256),
	mLastHotKey (0),
	mClientLifetimeTimer (0)
{
	SetRegistryKeyEx (_T(_DOUBLEAGENT_NAME), _T(_SERVER_REGNAME));
#ifdef	_DEBUG
	LogStart (GetProfileDebugInt(_T("LogRestart"))!=0);
	LogDebugRuntime ();
#else
	LogStart (false);
#endif
#ifdef	_DEBUG_LANGUAGE
	SetThreadLocale (MAKELCID (_DEBUG_LANGUAGE, SORT_DEFAULT));
#endif
#ifdef	_DEBUG
	CDaCoreApp::InitLogging (gLogFileName, gLogLevel);
#endif
}

CDaServerApp::~CDaServerApp()
{
	LogStop (LogIfActive);
#ifndef	_DEBUG
	ExitProcess (0);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaServerApp::_InitInstance()
{
	CoInitialize (NULL);
	AfxSetResourceHandle (GetModuleHandle (_T("DaCore")));
	tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
	lInitControls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx (&lInitControls);
	AfxEnableControlContainer ();

#ifdef	_DEBUG_NOT
	LogProcessSecurity (GetCurrentProcess(), LogIfActive);
#else
	LogProcessUser (GetCurrentProcess(), LogIfActive);
	LogProcessOwner (GetCurrentProcess(), LogIfActive);
	LogProcessIntegrity (GetCurrentProcess(), LogIfActive);
#endif
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("Command line [%s]"), GetCommandLine());
#endif
	ParseCommandLine (*(m_pCmdInfo = new CServerCmdLine));
}

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

void CDaServerApp::_ExitInstance()
{
	try
	{
		mTimerNotifies.DeleteAll ();
	}
	catch AnyExceptionSilent

	CDaSpeechInputConfig::RegisterHotKey (false);
	SafeFreeSafePtr (mAgentPropertySheet);
	SafeFreeSafePtr (mAgentAudioOutputProperties);
	SafeFreeSafePtr (mAgentSpeechInputProperties);
	SafeFreeSafePtr (mAgentCommandWindow);
	SafeFreeSafePtr (mPropSheetCharSel);
	SafeFreeSafePtr (mSvrCharacterFiles);
	SafeFreeSafePtr (mVoiceCommandsWnd);
	CLocalize::FreeMuiModules ();
	COleMessageFilterEx::ResetAppFilter (false);

	try
	{
		COleObjectFactory::RevokeAll ();
	}
	catch AnyExceptionSilent
	try
	{
		COleObjectFactory::UnregisterAll ();
	}
	catch AnyExceptionSilent
}

int CDaServerApp::ExitInstance()
{
	int	lRet = 4;

	__try
	{
#ifdef	_DEBUG
		if	(GetProfileDebugInt(_T("LogRestart"))==0)
		{
			LogMessage (LogNormal, _T("ExitInstance"));
		}
#endif
		_ExitInstance ();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__, EXCEPTION_CONTINUE_EXECUTION))
	{}

	__try
	{
		CoUninitialize ();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__, EXCEPTION_CONTINUE_EXECUTION))
	{}

	__try
	{
#ifdef	_DEBUG
		if	(GetProfileDebugInt(_T("LogRestart"))==0)
		{
			LogMessage (LogNormal, _T("ExitInstance Done"));
		}
#endif
		LogStop (LogIfActive);
		lRet = CWinApp::ExitInstance();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__, EXCEPTION_CONTINUE_EXECUTION))
	{}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CDaServerApp::Run ()
{
	int	lRet = 4;

	__try
	{
		AddTimerNotify (mClientLifetimeTimer=(UINT)&mClientLifetimeTimer, 5000, this);
		lRet = CWinApp::Run();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
	{}

	return lRet;
}

//BOOL CDaServerApp::PumpMessage()
//{
//	BOOL	lRet = FALSE;
//
//	__try
//	{
//		lRet = CWinApp::PumpMessage();
//	}
//	__except (LogCrash (GetExceptionCode(), GetExceptionInformation(), __FILE__, __LINE__))
//	{}
//
//	return lRet;
//}

/////////////////////////////////////////////////////////////////////////////

BOOL CDaServerApp::PreTranslateMessage(MSG *pMsg)
{
	if	(
			(pMsg->message == WM_TIMER)
		&&	(pMsg->hwnd == NULL)
		&&	(!mTimerNotifies.OnTimer ((UINT_PTR)pMsg->wParam))
		)
	{
		return TRUE;
	}

	if	(
			(pMsg->message == mOptionsChangedMsgId)
		||	(pMsg->message == mDefaultCharacterChangedMsgId)
		)
	{
		MSG	lMsg;
		while (PeekMessage (&lMsg, 0, pMsg->message, pMsg->message, PM_REMOVE))
		{}
		return DispatchThreadMessageEx(pMsg);
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

void CDaServerApp::OnTimerNotify (CTimerNotify * pTimerNotify, UINT_PTR pTimerId)
{
	if	(pTimerId == mClientLifetimeTimer)
	{
		VerifyObjectLifetimes ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CServerCmdLine::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	if	(
			(bFlag)
		&&	(_tcsicmp (pszParam, sCmdLineSettings) == 0)
		)
	{
		m_nShellCommand = FileNothing;
		m_strFileName = sCmdLineSettings;
	}
	else
	if	(
			(bFlag)
		&&	(!bLast)
		&&	(m_nShellCommand == FileNothing)
		&&	(m_strFileName.CompareNoCase (sCmdLineSettings) == 0)
		&&	(_tcsicmp (pszParam, sCmdLinePage) == 0)
		)
	{
		m_strDriverName = sCmdLinePage;
	}
	else
	if	(
			(!bFlag)
		&&	(bLast)
		&&	(m_nShellCommand == FileNothing)
		&&	(m_strFileName.CompareNoCase (sCmdLineSettings) == 0)
		&&	(m_strDriverName.CompareNoCase (sCmdLinePage) == 0)
		)
	{
		m_strPortName = pszParam;
	}
	else
	{
		CCmdLineInfoEx::ParseParam (pszParam, bFlag, bLast);
	}
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaServerApp::RegisterServer ()
{
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
		//AfxOleUnregisterTypeLib (gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor);
		COleObjectFactory::UpdateRegistryAll (FALSE);
		COleObjectFactory::UpdateRegistryAll (TRUE);
		AfxOleRegisterTypeLib (AfxGetInstanceHandle(), gDaTypeLibId);
//
//	If MS Agent is not installed, we have to register its type library as well.
//
		if	(
				(IsWindows7_AtLeast())
			&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer), HKEY_CLASSES_ROOT).IsEmpty())
#ifdef	_WIN64
			&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer64), HKEY_CLASSES_ROOT).IsEmpty())
#endif
			)
		{
			AfxOleRegisterTypeLib(AfxGetInstanceHandle(), gDaMsTypeLibId, CRegistrySearch::GetAltTypeLibPath (2));
		}
	}
	else
	if	(IsWindowsVista_AtLeast ())
	{
		AfxMessageBox (_T("Registration requires administrative privileges"), MB_OK|MB_ICONASTERISK);
	}
}

void CDaServerApp::UnregisterServer ()
{
	if	(
			(IsWindowsXp_AtMost ())
		||	(CUserSecurity::IsUserAdministrator ())
		)
	{
//
//	If we installed the MS Agent type library, we should uninstall it too.
//
		if	(
				(IsWindows7_AtLeast())
			&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer), HKEY_CLASSES_ROOT).IsEmpty())
#ifdef	_WIN64
			&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer64), HKEY_CLASSES_ROOT).IsEmpty())
#endif
			)
		{
			AfxOleUnregisterTypeLib(gDaMsTypeLibId, gDaMsTypeLibVerMajor, gDaMsTypeLibVerMinor);
		}
		AfxOleUnregisterTypeLib (gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor);
		COleObjectFactory::UpdateRegistryAll (FALSE);
	}
	else
	if	(IsWindowsVista_AtLeast ())
	{
		AfxMessageBox (_T("Registration requires administrative privileges"), MB_OK|MB_ICONASTERISK);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAgentPropertySheet * CDaServerApp::GetAgentPropertySheet (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mAgentPropertySheet)
		&&	(pCreate)
		&&	(mAgentPropertySheet = new CDaAgentPropertySheet (pClientMutexName))
		)
	{
		mAgentPropertySheet->m_dwRef = 0;
	}
	return mAgentPropertySheet;
}

void CDaServerApp::OnDeleteAgentPropertySheet (CDaAgentPropertySheet * pAgentPropertySheet)
{
	if	(pAgentPropertySheet == mAgentPropertySheet)
	{
		mAgentPropertySheet.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentAudioOutputProperties * CDaServerApp::GetAgentAudioOutputProperties (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mAgentAudioOutputProperties)
		&&	(pCreate)
		&&	(mAgentAudioOutputProperties = new CDaAgentAudioOutputProperties (pClientMutexName))
		)
	{
		mAgentAudioOutputProperties->m_dwRef = 0;
	}
	return mAgentAudioOutputProperties;
}

void CDaServerApp::OnDeleteAgentAudioOutputProperties (CDaAgentAudioOutputProperties * pAgentAudioOutputProperties)
{
	if	(pAgentAudioOutputProperties == mAgentAudioOutputProperties)
	{
		mAgentAudioOutputProperties.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentSpeechInputProperties * CDaServerApp::GetAgentSpeechInputProperties (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mAgentSpeechInputProperties)
		&&	(pCreate)
		&&	(mAgentSpeechInputProperties = new CDaAgentSpeechInputProperties (pClientMutexName))
		)
	{
		mAgentSpeechInputProperties->m_dwRef = 0;
	}
	return mAgentSpeechInputProperties;
}

void CDaServerApp::OnDeleteAgentSpeechInputProperties (CDaAgentSpeechInputProperties * pAgentSpeechInputProperties)
{
	if	(pAgentSpeechInputProperties == mAgentSpeechInputProperties)
	{
		mAgentSpeechInputProperties.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCommandWindow * CDaServerApp::GetAgentCommandWindow (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mAgentCommandWindow)
		&&	(pCreate)
		&&	(mAgentCommandWindow = new CDaAgentCommandWindow (GetVoiceCommandsWnd (true), pClientMutexName))
		)
	{
		mAgentCommandWindow->m_dwRef = 0;
	}
	return mAgentCommandWindow;
}

void CDaServerApp::OnDeleteAgentCommandWindow (CDaAgentCommandWindow * pAgentCommandWindow)
{
	if	(pAgentCommandWindow == mAgentCommandWindow)
	{
		mAgentCommandWindow.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CPropSheetCharSel * CDaServerApp::GetPropSheetCharSel (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mPropSheetCharSel)
		&&	(pCreate)
		&&	(mPropSheetCharSel = new CPropSheetCharSel (NULL, pClientMutexName))
		)
	{
		mPropSheetCharSel->m_dwRef = 0;
	}
	return mPropSheetCharSel;
}

void CDaServerApp::OnDeletePropSheetCharSel (CPropSheetCharSel * pPropSheetCharSel)
{
	if	(pPropSheetCharSel == mPropSheetCharSel)
	{
		mPropSheetCharSel.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CDaSvrCharacterFiles * CDaServerApp::GetSvrCharacterFiles (bool pCreate, LPCTSTR pClientMutexName)
{
	if	(
			(!mSvrCharacterFiles)
		&&	(pCreate)
		&&	(mSvrCharacterFiles = new CDaSvrCharacterFiles (pClientMutexName))
		)
	{
		mSvrCharacterFiles->m_dwRef = 0;
	}
	return mSvrCharacterFiles;
}

void CDaServerApp::OnDeleteSvrCharacterFiles (CDaSvrCharacterFiles * pSvrCharacterFiles)
{
	if	(pSvrCharacterFiles == mSvrCharacterFiles)
	{
		mSvrCharacterFiles.Detach ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAgentCharacter * CDaServerApp::GetAppCharacter (long pCharID)
{
	CDaAgentCharacter *	lRet = NULL;

	if	(pCharID > 0)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
			{
				CObTypeArray <CObject>	lFileClients;
				INT_PTR					lClientNdx;
				CDaAgentCharacter *		lCharacter;

				if	(GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
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

CDaAgentCharacter * CDaServerApp::GetListenCharacter ()
{
	CDaAgentCharacter *	lListenCharacter = NULL;

	try
	{
		CDaAgentCharacter *	lActiveCharacter = NULL;
		CDaAgentCharacter *	lClientActiveCharacter = NULL;
		INT_PTR				lFileNdx;
		CAgentFile *		lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CDaAgentCharacter *		lCharacter;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
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

CVoiceCommandsWnd * CDaServerApp::GetVoiceCommandsWnd (bool pCreate, long pCharID)
{
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDaServerApp::GetVoiceCommandsWnd"));
#endif
	if	(
			(!mVoiceCommandsWnd)
		&&	(pCreate)
		&&	(mVoiceCommandsWnd = (CVoiceCommandsWnd *) CVoiceCommandsWnd::CreateObject ())
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
		CDaAgentCharacter *	lCharacter;
		CDaAgentCommands *	lCommands = NULL;
		BSTR				lName = NULL;

		if	(lCharacter = GetAppCharacter (pCharID))
		{
			lName = lCharacter->GetName ();
			lCommands = lCharacter->GetCommandsObj (true);
		}
		mVoiceCommandsWnd->SetCharacter (pCharID, CString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
	}
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDaServerApp::GetVoiceCommandsWnd Done"));
#endif
	return mVoiceCommandsWnd;
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerApp::SetVoiceCommandClients (long pCharID)
{
	if	(mVoiceCommandsWnd)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
			{
				CObTypeArray <CObject>	lFileClients;
				INT_PTR					lClientNdx;
				CDaAgentCharacter *		lCharacter;

				if	(GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
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

void CDaServerApp::SetVoiceCommandNames (long pCharID)
{
	if	(mVoiceCommandsWnd)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
			{
				CObTypeArray <CObject>	lFileClients;
				INT_PTR					lClientNdx;
				CDaAgentCharacter *		lCharacter;
				CDaAgentCommands *		lCommands;
				BSTR					lName;

				if	(GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
					{
						if	(
								(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
							&&	(
									(pCharID <= 0)
								||	(lCharacter->GetCharID() == pCharID)
								)
							&&	(lName = lCharacter->GetName())
							)
						{
							lCommands = lCharacter->GetCommandsObj (true);
							mVoiceCommandsWnd->SetCharacterName (lCharacter->GetCharID(), CString (lName), (lCommands ? (LPCTSTR)lCommands->GetVoiceCommandsCaption() : NULL));
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

bool CDaServerApp::AddTimerNotify (UINT_PTR pTimerId, DWORD pInterval, ITimerNotifySink * pNotifySink)
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

bool CDaServerApp::DelTimerNotify (UINT_PTR pTimerId)
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

bool CDaServerApp::HasTimerNotify (UINT_PTR pTimerId)
{
	return (mTimerNotifies.FindTimer (pTimerId) >= 0);
}

CTimerNotify * CDaServerApp::GetTimerNotify (UINT_PTR pTimerId)
{
	return mTimerNotifies.GetTimer (pTimerId);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaServerApp::TraceCharacterAction (long pCharID, LPCTSTR pAction, LPCTSTR pFormat, ...)
{
	bool	lRet = false;
#ifdef	_TRACE_CHARACTER_ACTIONS
	if	(
			(pCharID > 0)
		?	(mActionTraceLog.FindValue (pCharID) >= 0)
		:	(mActionTraceLog.GetSize() > 0)
		)
	{
		try
		{
			tS <SYSTEMTIME>	lTraceTime;
			CString			lTraceDetail;
			CString			lTraceMsg;

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
				LogWrite (lTraceMsg, mActionTraceLog.KeyAt (mActionTraceLog.FindValue (pCharID)));
			}
			else
			{
				INT_PTR	lNdx;

				for	(lNdx = 0; lNdx <= mActionTraceLog.GetUpperBound(); lNdx++)
				{
					LogWrite (lTraceMsg, mActionTraceLog.KeyAt (lNdx));
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

bool CDaServerApp::StartActionTrace (long pCharID)
{
	bool	lRet = false;
#ifdef	_TRACE_CHARACTER_ACTIONS
	try
	{
		CDaAgentCharacter *	lCharacter;

		if	(
				(CRegDWord (CRegKey (CRegKey (HKEY_CURRENT_USER, gProfileKeyDa, true), _T(_SERVER_REGNAME), true), _T("ActionTrace")).Value() != 0)
			&&	(lCharacter = GetAppCharacter (pCharID))
			)
		{
			CString			lTraceFilePath;
			CString			lTraceFileName;
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

			mActionTraceLog.SetAt (lTraceFilePath, pCharID);
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

bool CDaServerApp::StopActionTrace (long pCharID)
{
	bool	lRet = false;
#ifdef	_TRACE_CHARACTER_ACTIONS
	try
	{
		INT_PTR	lTraceNdx;

		lTraceNdx = mActionTraceLog.FindValue (pCharID);
		if	(lTraceNdx >= 0)
		{
			mActionTraceLog.RemoveAt (lTraceNdx);
			lRet = true;
		}
	}
	catch AnyExceptionDebug
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaServerApp::ShowSettings (LPCTSTR pStartPage)
{
	bool			lRet = false;
	UINT			lErrorMode = SetErrorMode (SEM_NOOPENFILEERRORBOX);
	CModuleHandle	lSettingsModule;
	APPLET_PROC		lCplProc = NULL;
	CString			lStartPage (pStartPage);

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

void CDaServerApp::OnThreadHotKey(WPARAM wParam, LPARAM lParam)
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
			CDaAgentCharacter *	lCharacter = GetListenCharacter ();

			if	(lCharacter)
			{
				LogComErr (LogDetails, lCharacter->StartListening (false));
			}
		}
		catch AnyExceptionDebug
	}
}

bool CDaServerApp::IsHotKeyStillPressed () const
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

void CDaServerApp::OnBroadcastOptionsChanged(WPARAM wParam, LPARAM lParam)
{
	_OnOptionsChanged ();
}

void CDaServerApp::OnBroadcastDefaultCharacterChanged(WPARAM wParam, LPARAM lParam)
{
	_OnDefaultCharacterChanged ();
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerApp::_OnOptionsChanged ()
{
	try
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

		CDaSpeechInputConfig::RegisterHotKey (true);

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_OptionsChanged ();
		}
	}
	catch AnyExceptionDebug
}

void CDaServerApp::_OnDefaultCharacterChanged ()
{
	try
	{
		int					lNotifyNdx;
		IDaNotify *			lNotify;
		tPtr <CAgentFile>	lFile;
		CString				lDefCharPath ((BSTR)CAgentFiles::GetDefCharPath());

		if	(
				(!lDefCharPath.IsEmpty ())
			&&	(lFile = (CAgentFile *) CAgentFile::CreateObject())
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

void CDaServerApp::_OnCharacterLoaded (long pCharID)
{
	try
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

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

void CDaServerApp::_OnCharacterUnloaded (long pCharID)
{
	try
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

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

void CDaServerApp::_OnCharacterNameChanged (long pCharID)
{
	try
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

		SetVoiceCommandNames (pCharID);

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterNameChanged (pCharID);
		}
	}
	catch AnyExceptionSilent
}

void CDaServerApp::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

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

void CDaServerApp::OnCharacterListening (long pCharID, bool pListening, long pCause)
{
//
//	This notification is slightly different in that it's sent to all of a character's
//	clients rather than just the active client.
//
	try
	{
		int			lNotifyNdx;
		IDaNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterListening (pCharID, pListening, pCause);
		}
	}
	catch AnyExceptionSilent
}
