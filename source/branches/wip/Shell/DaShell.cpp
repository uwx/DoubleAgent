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
#include <cpl.h>
#include <shlwapi.h>
#include "DaShell.h"
#include "DaCore.h"
#include "PropSheetCpl.h"
#include "Registry.h"
#include "GuidStr.h"
#include "Localize.h"
#include "StringArrayEx.h"
#include "ThreadSecurity.h"
#include "UserSecurity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_APPLET	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG			LogNormal
#endif
#define	_LOG_ROOT_PATH				_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME			_T(_SHELL_REGNAME)
#define _LOG_DEF_LOGNAME			_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX					_T("Shel ")
static tPtr <::CCriticalSection>	sLogCriticalSection = new ::CCriticalSection;
#define	_LOG_CRITICAL_SECTION		(!sLogCriticalSection?NULL:(CRITICAL_SECTION*)(*sLogCriticalSection))
#include "LogAccess.inl"
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CDaShellApp, CWinApp)

CDaShellApp gApp;

CDaShellApp::CDaShellApp()
:	CWinApp (_T(_DOUBLEAGENT_NAME))
{
	SetRegistryKeyEx (_T(_DOUBLEAGENT_NAME), _T(_SHELL_REGNAME));
}

CDaShellApp::~CDaShellApp()
{
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDaShellApp::InitInstance()
{
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareInit ();
#endif
#ifdef	_DEBUG
	LogStart (GetProfileDebugInt(_T("LogRestart"))!=0);
	LogDebugRuntime ();
#else
	LogStart (false);
#endif
	LogProcessUser (GetCurrentProcess(), LogIfActive);
	LogProcessOwner (GetCurrentProcess(), LogIfActive);
	LogProcessIntegrity (GetCurrentProcess(), LogIfActive);

#ifdef	_DEBUG
	CDaCoreApp::InitLogging (gLogFileName, gLogLevel);
#endif
	COleObjectFactory::RegisterAll();
	return TRUE;
}

int CDaShellApp::ExitInstance()
{
	SafeFreeSafePtr (mCplPropSheet);
	CLocalize::FreeMuiModules ();
#if	ISOLATION_AWARE_ENABLED
	IsolationAwareCleanup ();
#endif
	LogStop (LogIfActive);
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDaShellApp, CWinApp)
	//{{AFX_MSG_MAP(CDaShellApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LONG APIENTRY CPlApplet (HWND hwndCPl, UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
	LONG	lRet = 0;

#ifdef	_DEBUG_APPLET
	CString	lMsgName;
	switch (uMsg)
	{
		case CPL_INIT:			lMsgName = _T("CL_INIT"); break;
		case CPL_EXIT:			lMsgName = _T("CPL_EXIT"); break;
		case CPL_GETCOUNT:		lMsgName = _T("CPL_GETCOUNT"); break;
		case CPL_INQUIRE:		lMsgName = _T("CPL_INQUIRE"); break;
		case CPL_NEWINQUIRE:	lMsgName = _T("CPL_NEWINQUIRE"); break;
		case CPL_STARTWPARMS:	lMsgName = _T("CPL_STARTWPARMS"); break;
		case CPL_DBLCLK:		lMsgName = _T("CPL_DBLCLK"); break;
		case CPL_STOP:			lMsgName = _T("CPL_STOP"); break;
		case CPL_SETUP:			lMsgName = _T("CPL_SETUP"); break;
	}
	LogMessage (_DEBUG_APPLET, _T("CPlApplet [%p] [%u] [%s]"), hwndCPl, uMsg, lMsgName);
#endif

	switch (uMsg)
	{
		case CPL_INIT:
		{
			CDaShellApp *	lApp = TheShellApp;

			if	(!lApp->mCplPropSheet)
			{
				lApp->mCplPropSheet = new CPropSheetCpl;
			}
			lRet = TRUE;
		}	break;
		case CPL_EXIT:
		{
			CDaShellApp *	lApp = TheShellApp;

			SafeFreeSafePtr (lApp->mCplPropSheet);
		}	break;
		case CPL_GETCOUNT:
		{
			lRet = 1;
		}	break;
		case CPL_INQUIRE:
		{
			CDaShellApp *	lApp = TheShellApp;
			UINT			lAppNum = (UINT)lParam1;
			LPCPLINFO		lInfo = (LPCPLINFO) lParam2;

			lInfo->idIcon = IDI_CPL;
			lInfo->idName = IDS_CPL_NAME;
			lInfo->idInfo = IDS_CPL_DESC;
			lInfo->lData = (LONG_PTR)lApp->mCplPropSheet.Ptr();
		}	break;
		case CPL_STARTWPARMS:
		{
			CDaShellApp *	lApp = TheShellApp;
			UINT			lAppNum = (UINT)lParam1;

			lApp->mCplStartPage = (LPCTSTR)lParam2;
#ifdef	_DEBUG_APPLET
			LogMessage (_DEBUG_APPLET, _T("  [%u] [%s]"), lParam1, (LPCTSTR)lParam2);
#endif
		}	break;
		case CPL_DBLCLK:
		{
			CDaShellApp *	lApp = TheShellApp;
			UINT			lAppNum = (UINT)lParam1;
			bool			lElevatedPages = (lApp->mCplStartPage.CompareNoCase (CPropSheetCpl::mPageNameRegistry) == 0);

			if	(lApp->mCplPropSheet)
			{

				lApp->mCplPropSheet->SetModalParent (CWnd::FromHandle (hwndCPl));
				if	(lApp->mCplPropSheet->InitPages (lElevatedPages))
				{
					lApp->mCplPropSheet->SetStartPage (lApp->mCplStartPage);
					lApp->mCplPropSheet->DoModal ();
				}
			}
		}	break;
		case CPL_STOP:
		{
			CDaShellApp *	lApp = TheShellApp;
			UINT			lAppNum = (UINT)lParam1;

			if	(lApp->mCplPropSheet->GetSafeHwnd())
			{
			}
		}	break;
	}

#ifdef	_DEBUG_APPLET
	LogMessage (_DEBUG_APPLET, _T("Return [%d]"), lRet);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaShellApp::RegisterCpl ()
{
	CString	lModuleName;

	GetModuleFileName (AfxGetInstanceHandle(), lModuleName.GetBuffer(MAX_PATH), MAX_PATH);
	lModuleName.ReleaseBuffer ();

	if	(LogIsActive ())
	{
		LogMessage (LogIfActive, _T("==> RegisterCpl [%s]"), lModuleName);
	}

	if	(IsWindowsVista_AtLeast())
	{
		CRegKeyEx	lCplRoot (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace"), false);
		CRegKeyEx	lCplIdKey (lCplRoot, (CString)CGuidStr(__uuidof(CDaShellApp)), false, true);
		CRegString	lCplName (lCplIdKey, (LPCTSTR)NULL, true);
		CRegKeyEx	lClassIdRoot (HKEY_CLASSES_ROOT, _T("CLSID"), false);
		CRegKeyEx	lClassIdKey (lClassIdRoot, (CString)CGuidStr(__uuidof(CDaShellApp)), false, true);
		CRegKeyEx	lClassIconKey (lClassIdKey, _T("DefaultIcon"), false, true);
		CRegString	lClassIcon (lClassIconKey, (LPCTSTR)NULL, true);
		CRegString	lClassName (lClassIdKey, (LPCTSTR)NULL, true);
		CRegString	lClassNameLocal (lClassIdKey, _T("LocalizedString"), true);
		CRegString	lClassTip (lClassIdKey, _T("InfoTip"), true);
		CRegString	lClassApp (lClassIdKey, _T("System.ApplicationName"), true);
		CRegString	lClassCat (lClassIdKey, _T("System.ControlPanel.Category"), true);
		CRegString	lClassTasks (lClassIdKey, _T("System.Software.TasksFileUrl"), true);
		CRegKeyEx	lShellKey (lClassIdKey, _T("Shell"), false, true);
		CRegKeyEx	lOpenKey (lShellKey, _T("Open"), false, true);
		CRegKeyEx	lCommandKey (lOpenKey, _T("Command"), false, true);
		CRegString	lCommand (lCommandKey, (LPCTSTR)NULL, true);

		lCplName.Value().LoadString (IDS_CPL_NAME);
		lCplName.Update();
		lClassName.Value().LoadString (IDS_CPL_NAME);
		lClassName.Update();
		lClassNameLocal.Value().Format (_T("@%s,-%d"), lModuleName, IDS_CPL_NAME);
		lClassNameLocal.Update();
		lClassTip.Value().Format (_T("@%s,-%d"), lModuleName, IDS_CPL_DESC);
		lClassTip.Update();
		lClassIcon.Value().Format (_T("%s,-%d"), lModuleName, IDI_CPL);
		lClassIcon.Update();
		lClassApp.Update (_T(_CPL_CANONICAL_NAME));
		lClassCat.Update (_T("1,7"));
//	Save for later with a full namespace extension
//		lClassTasks.Value().Format (_T("%s,-%d"), lModuleName, IDR_CPL_TASKS);
//		lClassTasks.Update();

		PathRemoveFileSpec (lModuleName.GetBuffer (MAX_PATH));
		PathAppend (lModuleName.GetBuffer (MAX_PATH), _T(_SERVER_FILENAME) _T(".exe"));
		lModuleName.ReleaseBuffer ();
		lCommand.Value().Format (_T("%s /settings"), lModuleName);
		lCommand.Update ();
	}
	else
	{
		CRegKeyEx	lRootKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Control Panel"), true);
		CRegKeyEx	lCplsKey (lRootKey, _T("Cpls"), false, true);
		CRegKeyEx	lExtendedKey (lRootKey, _T("Extended Properties"), false, true);
		CRegString	lCplPath (lCplsKey, _T(_CPL_NAME), true);
		CRegKeyEx	lAppCatKey (lExtendedKey, _T("{305CA226-D286-468e-B848-2B2E8E697B74} 2"), false, true);
		CRegDWord	lCplAppCat (lAppCatKey, lModuleName, true);

		lCplPath.Update (lModuleName);
		lCplAppCat.SetValue(1).Update();
	}

	if	(IsWindowsXp_AtMost())
	{
		CRegKeyEx	lEnvironment (HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\Session Manager\\Environment"), false);
		CRegString	lPath (lEnvironment, _T("Path"));
		CString		lPathLower;
		CString		lModuleLower;

		PathRemoveFileSpec (lModuleName.GetBuffer(MAX_PATH));
		lModuleName.ReleaseBuffer ();

		if	(lPath.IsValid ())
		{
			lPathLower = lPath.Value();
			lModuleLower = lModuleName;
			lPathLower.MakeLower ();
			lModuleLower.MakeLower ();

			if	(_tcsstr (lPathLower, lModuleLower) == NULL)
			{
				if	(LogIsActive ())
				{
					LogMessage (LogIfActive, _T("Environment [%s]"), lPath.Value());
				}

				lPath.Value().TrimRight ();
				lPath.Value().TrimRight (_T(';'));
				lPath.Value() += _T(';');
				lPath.Value() += lModuleName;
				lPath.Update ();

				if	(LogIsActive ())
				{
					CRegString	lPathVerify (lEnvironment, _T("Path"));
					LogMessage (LogIfActive, _T("  Updated   [%s]"), lPath.Value());
					LogMessage (LogIfActive, _T("  Verify    [%s]"), lPathVerify.Value());
				}

				try
				{
					DWORD	lRecipients = BSM_ALLCOMPONENTS;
					if	(BroadcastSystemMessage (BSF_FORCEIFHUNG, &lRecipients, WM_SETTINGCHANGE, 0, (LPARAM)_T("Environment")) < 0)
					{
						LogWinErr (LogIfActive, GetLastError());
					}
				}
				catch AnyExceptionSilent
			}
		}
	}
}

void CDaShellApp::UnregisterCpl ()
{
	CString	lModuleName;

	GetModuleFileName (AfxGetInstanceHandle(), lModuleName.GetBuffer(MAX_PATH), MAX_PATH);
	lModuleName.ReleaseBuffer ();

	if	(LogIsActive ())
	{
		LogMessage (LogIfActive, _T("==> UnregisterCpl [%s]"), lModuleName);
	}

	if	(IsWindowsVista_AtLeast())
	{
		CRegKeyEx	lCplRoot (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace"), false);
		CRegKeyEx	lCplIdKey (lCplRoot, (CString)CGuidStr(__uuidof(CDaShellApp)), false, true);
		CRegKeyEx	lClassIdRoot (HKEY_CLASSES_ROOT, _T("CLSID"), false);
		CRegKeyEx	lClassIdKey (lClassIdRoot, (CString)CGuidStr(__uuidof(CDaShellApp)), false, true);

		lCplIdKey.Delete ();
		lClassIdKey.Delete ();
	}
	else
	{
		CRegKeyEx	lRootKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Control Panel"), true);
		CRegKeyEx	lCplsKey (lRootKey, _T("Cpls"), false);
		CRegKeyEx	lExtendedKey (lRootKey, _T("Extended Properties"), false);
		CRegString	lCplPath (lCplsKey, _T(_CPL_NAME));
		CRegKeyEx	lAppCatKey (lExtendedKey, _T("{305CA226-D286-468e-B848-2B2E8E697B74} 2"), false);
		CRegDWord	lCplAppCat (lAppCatKey, lModuleName);

		lCplPath.Delete ();
		lCplAppCat.Delete ();
	}

	if	(IsWindowsXp_AtMost())
	{
		CRegKeyEx	lEnvironment (HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\Session Manager\\Environment"), false);
		CRegString	lPath (lEnvironment, _T("Path"));
		CString		lPathLower;
		CString		lModuleLower;
		LPCTSTR		lModuleFound;
		int			lModulePos;

		PathRemoveFileSpec (lModuleName.GetBuffer(MAX_PATH));
		lModuleName.ReleaseBuffer ();

		if	(lPath.IsValid ())
		{
			lPathLower = lPath.Value();
			lModuleLower = lModuleName;
			lPathLower.MakeLower ();
			lModuleLower.MakeLower ();

			if	(lModuleFound = _tcsstr (lPathLower, lModuleLower))
			{
				if	(LogIsActive())
				{
					LogMessage (LogIfActive, _T("Environment [%s]"), lPath.Value());
				}

				lModulePos = (int)(lModuleFound - (LPCTSTR)lPathLower);
				lPath.Value().Delete (lModulePos, lModuleLower.GetLength());
				if	(
						(lModulePos > 0)
					&&	(lModulePos < lPath.Value().GetLength())
					&&	(lPath.Value() [lModulePos] == _T(';'))
					&&	(lPath.Value() [lModulePos-1] == _T(';'))
					)
				{
					lPath.Value().Delete (lModulePos);
				}
				lPath.Value().TrimRight (_T(';'));
				lPath.Update ();

				if	(LogIsActive())
				{
					CRegString	lPathVerify (lEnvironment, _T("Path"));
					LogMessage (LogIfActive, _T("  Updated   [%s]"), lPath.Value());
					LogMessage (LogIfActive, _T("  Verify    [%s]"), lPathVerify.Value());
				}

				try
				{
					DWORD	lRecipients = BSM_ALLCOMPONENTS;
					if	(BroadcastSystemMessage (BSF_FORCEIFHUNG, &lRecipients, WM_SETTINGCHANGE, 0, (LPARAM)_T("Environment")) < 0)
					{
						LogWinErr (LogIfActive, GetLastError());
					}
				}
				catch AnyExceptionSilent
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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

		if	(COleObjectFactory::UpdateRegistryAll(TRUE))
		{
			try
			{
				CDaShellApp::RegisterCpl ();
			}
			catch AnyExceptionSilent
		}
		else
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
		try
		{
			CDaShellApp::UnregisterCpl ();
		}
		catch AnyExceptionSilent

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
