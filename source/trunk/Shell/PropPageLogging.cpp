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
#include <AfxPriv.h>
#include <AfxShellManager.h>
#include "DaShell.h"
#include "DaGlobalConfig.h"
#include "PropPageLogging.h"
#include "PropPageRegistry.h"
#include "Registry.h"
#include "GuidStr.h"
#include "NotifyLock.h"
#include "DaServerOdl.h"
#include "DaControlOdl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPropPageLogging, CPropertyPage)

BEGIN_MESSAGE_MAP(CPropPageLogging, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLogging)
	ON_BN_CLICKED(IDC_LOG_SERVER, OnLogComponent)
	ON_BN_CLICKED(IDC_LOG_CONTROL, OnLogComponent)
	ON_BN_CLICKED(IDC_LOG_CORE, OnLogComponent)
	ON_BN_CLICKED(IDC_LOG_LEVEL_NONE, OnLogLevel)
	ON_BN_CLICKED(IDC_LOG_LEVEL_IFACTIVE, OnLogLevel)
	ON_BN_CLICKED(IDC_LOG_LEVEL_NORMAL, OnLogLevel)
	ON_BN_CLICKED(IDC_LOG_LEVEL_DETAIL, OnLogLevel)
	ON_BN_CLICKED(IDC_LOG_LEVEL_VERBOSE, OnLogLevel)
	ON_BN_CLICKED(IDC_LOG_TRACE_ACTIONS, OnLogTraceActions)
	ON_BN_CLICKED(IDC_LOG_CRASH_DUMP, OnLogCrashDump)
	ON_BN_CLICKED(IDC_LOG_FILE_BROWSE, OnLogFileBrowse)
	ON_BN_CLICKED(IDC_LOG_FILE_RESET, OnLogFileReset)
	ON_BN_CLICKED(IDC_LOG_PATH_BROWSE, OnLogPathBrowse)
	ON_BN_CLICKED(IDC_LOG_PATH_RESET, OnLogPathReset)
	ON_BN_CLICKED(IDC_LOG_REGISTRY, OnLogRegistry)
	ON_EN_CHANGE(IDC_LOG_FILE, OnLogFileChange)
	ON_EN_CHANGE(IDC_LOG_PATH, OnLogPathChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropPageLogging::CPropPageLogging()
:	CPropertyPage(IDD),
	mKeyNdxServer (0),
	mKeyNdxControl (1),
	mKeyNdxCore (2),
	mKeyNdx (-1),
	mDefLogFile (_T(_DOUBLEAGENT_NAME) _T(".log"))
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageLogging::CPropPageLogging"), this);
#endif
	//{{AFX_DATA_INIT(CPropPageLogging)
	//}}AFX_DATA_INIT

	SHGetSpecialFolderPath (NULL, mDefLogPath.GetBuffer (MAX_PATH), CSIDL_DESKTOPDIRECTORY, FALSE);
	PathRemoveBackslash (mDefLogPath.GetBuffer (MAX_PATH));
	mDefLogPath.ReleaseBuffer ();

	mLogComponent.SetAtGrow (mKeyNdxServer, _T(_SERVER_REGNAME));
	mLogComponent.SetAtGrow (mKeyNdxControl, _T(_CONTROL_REGNAME));
	mLogComponent.SetAtGrow (mKeyNdxCore, _T(_CORE_REGNAME));

	if	(m_psp.pResource = mPropPageFix.GetWritableTemplate (IDD))
	{
		m_psp.dwFlags |= PSP_DLGINDIRECT;
		m_psp.pszTitle = (LPCTSTR) (m_strCaption = mPropPageFix.GetTemplateCaption (m_psp.pResource));
	}
}

CPropPageLogging::~CPropPageLogging()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageLogging::~CPropPageLogging"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLogging)
	DDX_Control(pDX, IDC_LOG_SERVER, mLogServer);
	DDX_Control(pDX, IDC_LOG_CONTROL, mLogControl);
	DDX_Control(pDX, IDC_LOG_CORE, mLogCore);
	DDX_Control(pDX, IDC_LOG_LEVEL_NONE, mLogLevelNone);
	DDX_Control(pDX, IDC_LOG_LEVEL_IFACTIVE, mLogLevelIfActive);
	DDX_Control(pDX, IDC_LOG_LEVEL_NORMAL, mLogLevelNormal);
	DDX_Control(pDX, IDC_LOG_LEVEL_DETAIL, mLogLevelDetail);
	DDX_Control(pDX, IDC_LOG_LEVEL_VERBOSE, mLogLevelVerbose);
	DDX_Control(pDX, IDC_LOG_TRACE_ACTIONS, mLogTraceActions);
	DDX_Control(pDX, IDC_LOG_CRASH_DUMP, mLogCrashDump);
	DDX_Control(pDX, IDC_LOG_FILE, mLogFileEdit);
	DDX_Control(pDX, IDC_LOG_FILE_BROWSE, mLogFileBrowse);
	DDX_Control(pDX, IDC_LOG_FILE_RESET, mLogFileReset);
	DDX_Control(pDX, IDC_LOG_PATH, mLogPathEdit);
	DDX_Control(pDX, IDC_LOG_PATH_BROWSE, mLogPathBrowse);
	DDX_Control(pDX, IDC_LOG_PATH_RESET, mLogPathReset);
	DDX_Control(pDX, IDC_LOG_REGISTRY, mLogRegistry);
	//}}AFX_DATA_MAP

	if	(pDX->m_bSaveAndValidate)
	{
		UpdateLogging ();
	}
	else
	{
		ShowLogging ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::ShowLogging ()
{
	CNotifyLock	lLock (m_hWnd);

	if	(
			(!mLogServer.GetCheck())
		&&	(!mLogControl.GetCheck())
		&&	(!mLogCore.GetCheck())
		)
	{
		mLogServer.SetCheck (TRUE);
	}

	if	(mLogServer.GetCheck())
	{
		ShowLogging (mKeyNdx = mKeyNdxServer);
	}
	else
	if	(mLogControl.GetCheck())
	{
		ShowLogging (mKeyNdx = mKeyNdxControl);
	}
	else
	if	(mLogCore.GetCheck())
	{
		ShowLogging (mKeyNdx = mKeyNdxCore);
	}

	if	(!mLogCrashValue)
	{
		mLogSettingsKey = new CRegKey (CRegKey (HKEY_CURRENT_USER, gProfileKeyDa, false), _T(_SERVER_REGNAME), false, true);
		mLogCrashValue = new CRegDWord (*mLogSettingsKey, _T("CrashDump"), true);
		mLogTraceValue = new CRegDWord (*mLogSettingsKey, _T("ActionTrace"), true);
	}
	mLogCrashDump.SetCheck (mLogCrashValue->Value() != 0);
	mLogCrashDump.ShowWindow (mLogServer.GetCheck() ? SW_SHOWNA : SW_HIDE);
	mLogTraceActions.SetCheck (mLogTraceValue->Value() != 0);
	mLogTraceActions.ShowWindow (mLogServer.GetCheck() ? SW_SHOWNA : SW_HIDE);
}

void CPropPageLogging::ShowLogging (INT_PTR pKeyNdx)
{
	CNotifyLock		lLock (m_hWnd);
	CRegKey			lRootKey (HKEY_CURRENT_USER, gProfileKeyDa, true);
	CRegKey			lItemKey (lRootKey, mLogComponent [pKeyNdx], true);
	CRegDWord		lItemLogLevel (lItemKey, _T("LogLevel"));
	CRegString		lItemLogPath (lItemKey, _T("LogFile"));
	CRegDWord *		lLogLevel;
	CRegString *	lLogPath;

	lLogLevel = mLogLevel (pKeyNdx);
	if	(!lLogLevel)
	{
		lLogLevel = &lItemLogLevel;
	}
	ShowLogLevel (*lLogLevel);

	lLogPath = mLogPath (pKeyNdx);
	if	(!lLogPath)
	{
		lLogPath = &lItemLogPath;
	}
	ShowLogPath (*lLogPath);
}

void CPropPageLogging::ShowLogLevel (const CRegDWord & pLogLevel)
{
	DWORD	lLogLevel = pLogLevel.Value() & LogLevelMask;

	mLogLevelNone.SetCheck (lLogLevel == 0);
	mLogLevelIfActive.SetCheck (lLogLevel == LogIfActive);
	mLogLevelNormal.SetCheck (lLogLevel == LogNormal);
	mLogLevelDetail.SetCheck (lLogLevel == LogDetail);
	mLogLevelVerbose.SetCheck (lLogLevel == LogVerbose);
}

void CPropPageLogging::ShowLogPath (const CRegString & pLogPath)
{
	CString	lFilePath;
	CString	lFileName;

	SplitLogPath (CRegString (pLogPath).Expand().Value(), lFilePath, lFileName);

	mLogFileEdit.SetWindowText (lFileName);
	mLogPathEdit.SetWindowText (lFilePath);
	mLogFileReset.EnableWindow (lFileName.CompareNoCase (mDefLogFile) != 0);
	mLogPathReset.EnableWindow (lFilePath.CompareNoCase (mDefLogPath) != 0);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CRegKey * CPropPageLogging::PrepUpdate (INT_PTR pKeyNdx)
{
	if	(
			(pKeyNdx >= 0)
		&&	(pKeyNdx <= mLogComponent.GetUpperBound ())
		&&	(!mLogKey (pKeyNdx))
		)
	{
		CRegKey	lRootKey (HKEY_CURRENT_USER, gProfileKeyDa, false, true);

		mLogKey.SetAtGrow (pKeyNdx, new CRegKey (lRootKey, mLogComponent [pKeyNdx], false, true));
	}
	return mLogKey (pKeyNdx);
}

void CPropPageLogging::UpdateLogPath (INT_PTR pKeyNdx)
{
	CString		lLogPath = MakeLogPath (true);
	CRegKey *	lLogKey;

	if	(
			(!mLogPath (pKeyNdx))
		&&	(lLogKey = PrepUpdate (pKeyNdx))
		)
	{
		mLogPath.SetAtGrow (pKeyNdx, new CRegString (*lLogKey, _T("LogFile"), true));
	}
	if	(
			(mLogPath (pKeyNdx))
		&&	(mLogPath (pKeyNdx)->Value() != lLogPath)
		)
	{
		mLogPath (pKeyNdx)->Value () = lLogPath;
		SetModified (TRUE);
	}
}

void CPropPageLogging::UpdateLogging ()
{
	INT_PTR	lKeyNdx;

	for	(lKeyNdx = 0; lKeyNdx <= mLogLevel.GetUpperBound(); lKeyNdx++)
	{
		if	(mLogLevel (lKeyNdx))
		{
			mLogLevel (lKeyNdx)->Update ();
		}
	}
	for	(lKeyNdx = 0; lKeyNdx <= mLogPath.GetUpperBound(); lKeyNdx++)
	{
		if	(mLogPath (lKeyNdx))
		{
			if	(mLogPath (lKeyNdx)->Value().IsEmpty ())
			{
				mLogPath (lKeyNdx)->Delete ();
			}
			else
			{
				mLogPath (lKeyNdx)->Update ();
			}
		}
	}

	mLogLevel.DeleteAll ();
	mLogPath.DeleteAll ();
	mLogKey.DeleteAll ();

	if	(mLogCrashValue)
	{
		mLogCrashValue->Update ();
	}
	if	(mLogTraceValue)
	{
		mLogTraceValue->Update ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::SplitLogPath (LPCTSTR pLogPath, CString & pFilePath, CString & pFileName)
{
	pFilePath = pLogPath;
	pFilePath.TrimLeft ();
	pFilePath.TrimRight ();
	PathUnquoteSpaces (pFilePath.GetBuffer (MAX_PATH));
	pFilePath.ReleaseBuffer ();
	pFilePath.TrimLeft ();
	pFilePath.TrimRight ();

	if	(pFilePath.IsEmpty ())
	{
		pFilePath = mDefLogPath;
	}
	else
	if	(PathIsFileSpec (pFilePath))
	{
		pFileName = pFilePath;
		PathCombine (pFilePath.GetBuffer (MAX_PATH), mDefLogPath, pFileName);
		pFilePath.ReleaseBuffer ();
	}
	if	(
			(!pFilePath.IsEmpty ())
		&&	(GetLongPathName (pFilePath, pFileName.GetBuffer (MAX_PATH), MAX_PATH))
		)
	{
		pFileName.ReleaseBuffer ();
		pFilePath = pFileName;
	}
	pFileName = pFilePath;

	if	(PathIsDirectory (pFilePath))
	{
		pFileName.Empty ();
	}
	else
	{
		PathRemoveFileSpec (pFilePath.GetBuffer (MAX_PATH));
		pFilePath.ReleaseBuffer ();
		PathStripPath (pFileName.GetBuffer (MAX_PATH));
		pFileName.ReleaseBuffer ();
	}

	PathRemoveBackslash (pFilePath.GetBuffer (MAX_PATH));
	pFilePath.ReleaseBuffer ();
	if	(pFileName.IsEmpty ())
	{
		pFileName = mDefLogFile;
	}
}

CString CPropPageLogging::MakeLogPath (LPCTSTR pFilePath, LPCTSTR pFileName, bool pDefaultBlank)
{
	CString	lFilePath (pFilePath);
	CString	lFileName (pFileName);

	lFilePath.TrimLeft ();
	lFilePath.TrimRight ();
	PathUnquoteSpaces (lFilePath.GetBuffer (MAX_PATH));
	lFilePath.ReleaseBuffer ();
	lFilePath.TrimLeft ();
	lFilePath.TrimRight ();

	lFileName.TrimLeft ();
	lFileName.TrimRight ();
	PathUnquoteSpaces (lFileName.GetBuffer (MAX_PATH));
	lFileName.ReleaseBuffer ();
	lFileName.TrimLeft ();
	lFileName.TrimRight ();

	if	(lFilePath.IsEmpty ())
	{
		lFilePath = mDefLogPath;
	}
	if	(lFileName.IsEmpty ())
	{
		lFileName = mDefLogFile;
	}
	PathAppend (lFilePath.GetBuffer (MAX_PATH), lFileName);
	lFilePath.ReleaseBuffer ();

	SplitLogPath (CString ((LPCTSTR)lFilePath), lFilePath, lFileName);

	if	(
			(pDefaultBlank)
		&&	(lFilePath.CompareNoCase (mDefLogPath) == 0)
		)
	{
		if	(lFileName.CompareNoCase (mDefLogFile) == 0)
		{
			lFilePath.Empty ();
		}
		else
		{
			lFilePath = lFileName;
		}
	}
	else
	{
		PathAppend (lFilePath.GetBuffer (MAX_PATH), lFileName);
		lFilePath.ReleaseBuffer ();
	}
	return lFilePath;
}

CString CPropPageLogging::MakeLogPath (LPCTSTR pLogPath, bool pDefaultBlank)
{
	CString	lFileName;
	CString	lFilePath;

	SplitLogPath (pLogPath, lFilePath, lFileName);
	return MakeLogPath (lFilePath, lFileName, pDefaultBlank);
}

CString CPropPageLogging::MakeLogPath (bool pDefaultBlank)
{
	CString	lFileName;
	CString	lFilePath;

	mLogFileEdit.GetWindowText (lFileName);
	mLogPathEdit.GetWindowText (lFilePath);

	return MakeLogPath (lFilePath, lFileName, pDefaultBlank);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::OnLogComponent()
{
	ShowLogging ();
}

void CPropPageLogging::OnLogLevel()
{
	CRegKey *	lLogKey;
	DWORD		lLogLevel = mLogLevelIfActive.GetCheck() ? LogIfActive : mLogLevelNormal.GetCheck() ? LogNormal : mLogLevelDetail.GetCheck() ? LogDetail : mLogLevelVerbose.GetCheck() ? LogVerbose : 0;

	if	(
			(!mLogLevel (mKeyNdx))
		&&	(lLogKey = PrepUpdate (mKeyNdx))
		)
	{
		mLogLevel.SetAtGrow (mKeyNdx, new CRegDWord (*lLogKey, _T("LogLevel"), true));
	}
	if	(mLogLevel (mKeyNdx))
	{
		if	(mLogLevel (mKeyNdx)->Value () != lLogLevel)
		{
			mLogLevel (mKeyNdx)->SetValue (lLogLevel);
			SetModified (TRUE);
		}
		ShowLogLevel (*mLogLevel (mKeyNdx));
	}
}

void CPropPageLogging::OnLogTraceActions()
{
	if	(mLogTraceValue)
	{
		mLogTraceValue->SetValue (mLogTraceActions.GetCheck() ? 2 : 0);
		SetModified (TRUE);
	}
}

void CPropPageLogging::OnLogCrashDump()
{
	if	(mLogCrashValue)
	{
		mLogCrashValue->SetValue (mLogCrashDump.GetCheck() ? 2 : 0);
		SetModified (TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::OnLogFileChange()
{
	UpdateLogPath (mKeyNdx);
}

void CPropPageLogging::OnLogFileBrowse()
{
	CFileDialog	lFileDlg (FALSE, _T(".log"), MakeLogPath (false), OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_DONTADDTORECENT, _T("All Files|*.*|Log Files|*.log||"), this);
	CString		lFileName;
	CString		lFilePath;

	lFileDlg.GetOFN().lpstrTitle = _T("Log File");

	if	(lFileDlg.DoModal () == IDOK)
	{
		SplitLogPath (MakeLogPath (lFileDlg.GetPathName(), false), lFilePath, lFileName);
		mLogFileEdit.SetWindowText (lFileName);
		mLogPathEdit.SetWindowText (lFilePath);
		UpdateLogPath (mKeyNdx);
		ShowLogging (mKeyNdx);
	}
}

void CPropPageLogging::OnLogFileReset()
{
	CString	lFileName;
	CString	lFilePath;

	SplitLogPath (MakeLogPath (NULL, NULL, false), lFilePath, lFileName);
	mLogFileEdit.SetWindowText (lFileName);
	UpdateLogPath (mKeyNdx);
	ShowLogging (mKeyNdx);
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::OnLogPathChange()
{
	UpdateLogPath (mKeyNdx);
}

void CPropPageLogging::OnLogPathBrowse()
{
	CShellManager	lShellManager;
	CString			lFileName;
	CString			lFilePath;

	SplitLogPath (MakeLogPath (NULL, NULL, false), lFilePath, lFileName);

	if	(lShellManager.BrowseForFolder (lFilePath, this, lFilePath, _T("Specify the log file's location"), BIF_RETURNONLYFSDIRS|BIF_VALIDATE|BIF_USENEWUI))
	{
		SplitLogPath (MakeLogPath (lFilePath, NULL, false), lFilePath, lFileName);
		mLogPathEdit.SetWindowText (lFilePath);
		UpdateLogPath (mKeyNdx);
		ShowLogging (mKeyNdx);
	}
}

void CPropPageLogging::OnLogPathReset()
{
	CString	lFileName;
	CString	lFilePath;

	SplitLogPath (MakeLogPath (NULL, NULL, false), lFilePath, lFileName);
	mLogPathEdit.SetWindowText (lFilePath);
	UpdateLogPath (mKeyNdx);
	ShowLogging (mKeyNdx);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#define	_GLOBAL_ROOT_NAME	_T("HKEY_LOCAL_MACHINE\\Classes")
#define _USER_ROOT_NAME		_T("HKEY_CURRENT_USER\\Classes")
#define	_CLASSES_ROOT_NAME	_T("HKEY_CLASSES_ROOT")

#ifdef	_WIN64
#define	_SUFFIX_64			_T(" (64-bit)")
#define	_SUFFIX_32			_T(" (32-bit)")
#define	_ALT_ROOT_NAME		_T("\\Wow6432Node")
#endif

/////////////////////////////////////////////////////////////////////////////

#define	_LogClassIdAny(loglevel,clsid,name) LogClassId(loglevel,clsid,name,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME);
#define	_LogAppIdAny(loglevel,appid,name) LogAppId(loglevel,appid,name,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME);
#define	_LogProgIdAny(loglevel,progid,name) LogProgId(loglevel,progid,name,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME);
#define	_LogTypeLibAny(loglevel,typelibid,name) LogTypeLib(loglevel,typelibid,name,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME);
#define	_LogInterfaceAny(loglevel,interfaceid,name) LogInterface(loglevel,interfaceid,name,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME);

#ifdef	_WIN64
#define	_LogClassIdPlatform(loglevel,clsid,name) \
	LogClassId(loglevel,clsid,name _SUFFIX_64,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME); \
	LogClassId(loglevel,clsid,name _SUFFIX_32,lGlobalClasses32,lUserClasses32,lClassesRoot32,_GLOBAL_ROOT_NAME _ALT_ROOT_NAME,_USER_ROOT_NAME _ALT_ROOT_NAME,_CLASSES_ROOT_NAME _ALT_ROOT_NAME);
#define	_LogAppIdPlatform(loglevel,appid,name) \
	LogAppId(loglevel,appid,name _SUFFIX_64,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME); \
	LogAppId(loglevel,appid,name _SUFFIX_32,lGlobalClasses32,lUserClasses32,lClassesRoot32,_GLOBAL_ROOT_NAME _ALT_ROOT_NAME,_USER_ROOT_NAME _ALT_ROOT_NAME,_CLASSES_ROOT_NAME _ALT_ROOT_NAME);
#define	_LogProgIdPlatform(loglevel,progid,name) \
	LogProgId(loglevel,progid,name _SUFFIX_64,lGlobalClasses,lUserClasses,lClassesRoot,_GLOBAL_ROOT_NAME,_USER_ROOT_NAME,_CLASSES_ROOT_NAME); \
	LogProgId(loglevel,progid,name _SUFFIX_32,lGlobalClasses32,lUserClasses32,lClassesRoot32,_GLOBAL_ROOT_NAME _ALT_ROOT_NAME,_USER_ROOT_NAME _ALT_ROOT_NAME,_CLASSES_ROOT_NAME _ALT_ROOT_NAME);
#else
#define	_LogClassIdPlatform(loglevel,clsid,name)	_LogClassIdAny(loglevel,clsid,name)
#define	_LogAppIdPlatform(loglevel,appid,name)		_LogAppIdAny(loglevel,appid,name)
#define	_LogProgIdPlatform(loglevel,progid,name)	_LogProgIdAny(loglevel,progid,name)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::OnLogRegistry()
{
	CString		lLogPath = (mLogPath (mKeyNdxServer)) ? mLogPath (mKeyNdxServer)->Value() : CRegString (CRegKey (HKEY_CURRENT_USER, gProfileKeyDa, true), mLogComponent [mKeyNdxServer], true).Value();
	CFileDialog	lFileDlg (FALSE, _T(".log"), MakeLogPath (lLogPath, false), OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_DONTADDTORECENT, _T("All Files|*.*|Log Files|*.log||"), this);

	lFileDlg.GetOFN().lpstrTitle = _T("Log File");

	if	(lFileDlg.DoModal () == IDOK)
	{
		UINT	lLogLevel = gLogLevel;
		CString	lLogFileName = gLogFileName;

		LogStop ();
		LogStart (false, lFileDlg.GetPathName(), LogIfActive);

		try
		{
			LogMessage (LogIfActive|LogNoPrefix, _T(""));
			LogDaRegistry (LogIfActive|LogNoPrefix);
			LogMessage (LogIfActive|LogNoPrefix, _T(""));
			LogMsRegistry (LogIfActive|LogNoPrefix);
			LogMessage (LogIfActive|LogNoPrefix, _T(""));
		}
		catch AnyExceptionSilent

		LogStop (LogIfActive);
		LogStart (false, lLogFileName, lLogLevel);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::LogDaRegistry (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CRegKey	lGlobalClasses;
			CRegKey	lUserClasses;
			CRegKey	lClassesRoot;

			if	(lTitle.IsEmpty())
			{
				lTitle = "Double Agent Registry";
			}

			GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot);
#ifdef	_WIN64
			CRegKey	lGlobalClasses32;
			CRegKey	lUserClasses32;
			CRegKey	lClassesRoot32;
			GetRootKeys (lGlobalClasses32, lUserClasses32, lClassesRoot32, true);
#endif
			LogMessage (pLogLevel, lTitle);

			_LogClassIdPlatform (pLogLevel, __uuidof(CDaAgent), _T(_SERVER_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_SERVER_PROGID), _T(_SERVER_PROGID));
			_LogAppIdPlatform (pLogLevel, __uuidof(CDaAgent), _T(_SERVER_PROGID_NAME))

			_LogClassIdPlatform (pLogLevel, __uuidof(CDaAgentCtl), _T(_CONTROL_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_CONTROL_PROGID), _T(_CONTROL_PROGID));

			_LogClassIdAny (pLogLevel, __uuidof(CDaCharacterProps), _T(_PROPERTIES_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_PROPERTIES_PROGID), _T(_PROPERTIES_PROGID));

			_LogClassIdPlatform (pLogLevel, __uuidof(CAgentPreviewWnd), _T(_PREVIEW_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_PREVIEW_PROGID), _T(_PREVIEW_PROGID));

			if	(IsWindowsVista_AtLeast())
			{
				_LogClassIdAny (pLogLevel, __uuidof(CDaElevatedSettings), _T("Double Agent Emulation Management"))
				_LogProgIdAny (pLogLevel, _T(_ELEVATED_SETTINGS_PROGID), _T(_ELEVATED_SETTINGS_PROGID));
				_LogAppIdAny (pLogLevel, __uuidof(CDaElevatedSettings), _T("Double Agent Emulation Management"))
			}

			_LogTypeLibAny (pLogLevel, __uuidof(DaServerTypeLib), _T(_SERVER_PROGID_NAME));
#ifdef	_DEBUG
			_LogInterfaceAny (pLogLevel, __uuidof(IDaServer), _T("IDaServer"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCharacter), _T("IDaSvrCharacter"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrBalloon), _T("IDaSvrBalloon"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCommands), _T("IDaSvrCommands"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCommand), _T("IDaSvrCommand"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCommandWindow), _T("IDaSvrCommandWindow"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrUserInput), _T("IDaSvrUserInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrPropertySheet), _T("IDaSvrPropertySheet"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrSpeechInputProperties), _T("IDaSvrSpeechInputProperties"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrAudioOutputProperties), _T("IDaSvrAudioOutputProperties"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrNotifySink), _T("IDaSvrNotifySink"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrNotifySink15), _T("IDaSvrNotifySink15"));
#endif
			_LogTypeLibAny (pLogLevel, __uuidof(DaControlTypeLib), _T(_CONTROL_PROGID_NAME));
#ifdef	_DEBUG
			_LogInterfaceAny (pLogLevel, __uuidof(IDaControl), _T("IDaControl"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCharacters), _T("IDaCtlCharacters"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCharacter), _T("IDaCtlCharacter"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlBalloon), _T("IDaCtlBalloon"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCommands), _T("IDaCtlCommands"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCommand), _T("IDaCtlCommand"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlRequest), _T("IDaCtlRequest"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlAudioObject), _T("IDaCtlAudioObject"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlSpeechInput), _T("IDaCtlSpeechInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlPropertySheet), _T("IDaCtlPropertySheet"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlUserInput), _T("IDaCtlUserInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCommandsWindow), _T("IDaCtlCommandsWindow"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlAnimationNames), _T("IDaCtlAnimationNames"));
			_LogInterfaceAny (pLogLevel, __uuidof(_DaCtlEvents), _T("_DaCtlEvents"));
#endif
		}
		catch AnyExceptionSilent
	}
}

void CPropPageLogging::LogMsRegistry (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CRegKey	lGlobalClasses;
			CRegKey	lUserClasses;
			CRegKey	lClassesRoot;

			if	(lTitle.IsEmpty())
			{
				lTitle = "Microsoft Agent Registry";
			}

			GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot);
#ifdef	_WIN64
			CRegKey	lGlobalClasses32;
			CRegKey	lUserClasses32;
			CRegKey	lClassesRoot32;
			GetRootKeys (lGlobalClasses32, lUserClasses32, lClassesRoot32, true);
#endif
			LogMessage (pLogLevel, lTitle);

#ifdef	_WIN64
			_LogClassIdAny (pLogLevel, __uuidof(AgentServer64), _T(_MS_AGENT_SERVER_TITLE) _SUFFIX_64)
#endif
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentServer), _T(_MS_AGENT_SERVER_TITLE))
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_SERVER_PROGID), _T(_AGENT_SERVER_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_SERVER_PROGID2), _T(_AGENT_SERVER_PROGID2));

			_LogClassIdPlatform (pLogLevel, __uuidof(AgentControl), _T(_MS_AGENT_CONTROL_TITLE))
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_CONTROL_PROGID), _T(_AGENT_CONTROL_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_CONTROL_PROGID2), _T(_AGENT_CONTROL_PROGID2));

			_LogClassIdPlatform (pLogLevel, __uuidof(AgentControl15), _T(_MS_AGENT_CONTROL15_TITLE))
			_LogProgIdPlatform (pLogLevel, _T(_MS_AGENT_CONTROL15), _T(_MS_AGENT_CONTROL15));

			_LogClassIdPlatform (pLogLevel, __uuidof(AgentCharacterProps), _T(_MS_AGENT_PROPERTIES_TITLE))
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_CHAR_WEB_PROGID), _T(_AGENT_CHAR_WEB_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_CHAR_PROGID), _T(_AGENT_CHAR_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_AGENT_CHAR_PVW_PROGID), _T(_AGENT_CHAR_PVW_PROGID));

#ifdef	_DEBUG
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentServerHandler), _T(_MS_AGENT_SERVER_PROXY))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentNotifyProxy), _T(_MS_AGENT_NOTIFY_PROXY))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentCharacterProxy), _T(_MS_AGENT_CHARACTER_PROXY))
#endif
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentFileProvider), _T(_MS_AGENT_FILE_PROVIDER))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentDocFileProvider), _T(_MS_AGENT_DOCFILE_PROVIDER))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentFlatFileProvider), _T(_MS_AGENT_FLATFILE_PROVIDER))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentTtsEngine), _T(_MS_AGENT_TTS_ENGINE))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentTtsEngine2), _T(_MS_AGENT_TTS_ENGINE))
			_LogClassIdPlatform (pLogLevel, __uuidof(AgentVoiceCommands), _T(_MS_AGENT_VOICE_COMMANDS))

			_LogTypeLibAny (pLogLevel, __uuidof(AgentServerTypeLib), _T(_MS_AGENT_SERVER_TITLE));
#ifdef	_DEBUG
			_LogInterfaceAny (pLogLevel, __uuidof(IAgent), _T("IAgent"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentEx), _T("IAgentEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentNotifySink), _T("IAgentNotifySink"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentNotifySinkEx), _T("IAgentNotifySinkEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCharacter), _T("IAgentCharacter"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCharacterEx), _T("IAgentCharacterEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCommands), _T("IAgentCommands"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCommandsEx), _T("IAgentCommandsEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCommand), _T("IAgentCommand"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCommandEx), _T("IAgentCommandEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCommandWindow), _T("IAgentCommandWindow"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentUserInput), _T("IAgentUserInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentBalloon), _T("IAgentBalloon"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentBalloonEx), _T("IAgentBalloonEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentPropertySheet), _T("IAgentPropertySheet"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentAudioOutputProperties), _T("IAgentAudioOutputProperties"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentAudioOutputPropertiesEx), _T("IAgentAudioOutputPropertiesEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentSpeechInputProperties), _T("IAgentSpeechInputProperties"));
// 			_LogInterfaceAny (pLogLevel, __uuidof(IAgentPrivateNotifySink), _T("IAgentPrivateNotifySink"));
//			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCustomMarshalMaker), _T("IAgentCustomMarshalMaker"));
//			_LogInterfaceAny (pLogLevel, __uuidof(IAgentClientStatus), _T("IAgentClientStatus"));

			_LogTypeLibAny (pLogLevel, __uuidof(AgentServerTypeLib2), _T(_MS_AGENT_SERVER_TYPELIB2_TITLE));
#endif
			_LogTypeLibAny (pLogLevel, __uuidof(AgentControlTypeLib), _T(_MS_AGENT_CONTROL_TITLE));
#ifdef	_DEBUG
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtl), _T("IAgentCtl"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlEx), _T("IAgentCtlEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCharacters), _T("IAgentCtlCharacters"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCharacter), _T("IAgentCtlCharacter"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCharacterEx), _T("IAgentCtlCharacterEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCommands), _T("IAgentCtlCommands"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCommandsEx), _T("IAgentCtlCommandsEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCommand), _T("IAgentCtlCommand"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCommandEx), _T("IAgentCtlCommandEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlAnimationNames), _T("IAgentCtlAnimationNames"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlBalloon), _T("IAgentCtlBalloon"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlBalloonEx), _T("IAgentCtlBalloonEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlRequest), _T("IAgentCtlRequest"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlUserInput), _T("IAgentCtlUserInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlAudioObject), _T("IAgentCtlAudioObject"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlAudioObjectEx), _T("IAgentCtlAudioObjectEx"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlSpeechInput), _T("IAgentCtlSpeechInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlPropertySheet), _T("IAgentCtlPropertySheet"));
			_LogInterfaceAny (pLogLevel, __uuidof(IAgentCtlCommandsWindow), _T("IAgentCtlCommandsWindow"));
			_LogInterfaceAny (pLogLevel, __uuidof(_AgentEvents), _T("_AgentEvents"));
#endif
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CPropPageLogging::LogClassId (UINT pLogLevel, REFGUID pClassId, LPCTSTR pClassTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName)
{
	bool lTitleLogged = false;

	if	(
			(LogClassId (pLogLevel, pClassId, pClassTitle, pGlobalRootKey, pGlobalRootName, &lTitleLogged))
		|	(LogClassId (pLogLevel, pClassId, pClassTitle, pUserRootKey, pUserRootName, &lTitleLogged))
		|	(LogClassId (pLogLevel, pClassId, pClassTitle, pClassesRootKey, pClassesRootName, &lTitleLogged))
		)
	{
		return true;
	}
	else
	{
		LogMessage (pLogLevel, _T(""));
		LogMessage (pLogLevel, _T("- %s \"CLSID\\%s\" is not registered"), pClassTitle, (CString)CGuidStr(pClassId));
	}
	return false;
}

bool CPropPageLogging::LogAppId (UINT pLogLevel, REFGUID pAppId, LPCTSTR pAppTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName)
{
	bool lTitleLogged = false;

	if	(
			(LogAppId (pLogLevel, pAppId, pAppTitle, pGlobalRootKey, pGlobalRootName, &lTitleLogged))
		|	(LogAppId (pLogLevel, pAppId, pAppTitle, pUserRootKey, pUserRootName, &lTitleLogged))
		|	(LogAppId (pLogLevel, pAppId, pAppTitle, pClassesRootKey, pClassesRootName, &lTitleLogged))
		)
	{
		return true;
	}
	else
	{
		LogMessage (pLogLevel, _T(""));
		LogMessage (pLogLevel, _T("- %s \"APPID\\%s\" is not registered"), pAppTitle, (CString)CGuidStr(pAppId));
	}
	return false;
}

bool CPropPageLogging::LogProgId (UINT pLogLevel, LPCTSTR pProgId, LPCTSTR pProgIdTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName)
{
	bool lTitleLogged = false;

	if	(
			(LogProgId (pLogLevel, pProgId, pProgIdTitle, pGlobalRootKey, pGlobalRootName, &lTitleLogged))
		|	(LogProgId (pLogLevel, pProgId, pProgIdTitle, pUserRootKey, pUserRootName, &lTitleLogged))
		|	(LogProgId (pLogLevel, pProgId, pProgIdTitle, pClassesRootKey, pClassesRootName, &lTitleLogged))
		)
	{
		return true;
	}
	else
	{
		LogMessage (pLogLevel, _T(""));
		LogMessage (pLogLevel, _T("- %s \"%s\" is not registered"), pProgIdTitle, pProgId);
	}
	return false;
}

bool CPropPageLogging::LogTypeLib (UINT pLogLevel, REFGUID pTypeLibId, LPCTSTR pTypeLibTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName)
{
	bool lTitleLogged = false;

	if	(
			(
				(LogTypeLib (pLogLevel, pTypeLibId, pTypeLibTitle, pGlobalRootKey, pGlobalRootName, &lTitleLogged))
			|	(LogTypeLib (pLogLevel, pTypeLibId, pTypeLibTitle, pUserRootKey, pUserRootName, &lTitleLogged))
			)
		||	(LogTypeLib (pLogLevel, pTypeLibId, pTypeLibTitle, pClassesRootKey, pClassesRootName, &lTitleLogged))
		)
	{
		return true;
	}
	else
	{
		LogMessage (pLogLevel, _T(""));
		LogMessage (pLogLevel, _T("- %s \"TYPELIB\\%s\" is not registered"), pTypeLibTitle, (CString)CGuidStr(pTypeLibId));
	}
	return false;
}

bool CPropPageLogging::LogInterface (UINT pLogLevel, REFGUID pInterfaceId, LPCTSTR pInterfaceTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName)
{
	bool lTitleLogged = false;

	if	(
			(
				(LogInterface (pLogLevel, pInterfaceId, pInterfaceTitle, pGlobalRootKey, pGlobalRootName, &lTitleLogged))
			|	(LogInterface (pLogLevel, pInterfaceId, pInterfaceTitle, pUserRootKey, pUserRootName, &lTitleLogged))
			)
		||	(LogInterface (pLogLevel, pInterfaceId, pInterfaceTitle, pClassesRootKey, pClassesRootName, &lTitleLogged))
		)
	{
		return true;
	}
	else
	{
		LogMessage (pLogLevel, _T(""));
		LogMessage (pLogLevel, _T("  - %s \"INTERFACE\\%s\" is not registered"), pInterfaceTitle, (CString)CGuidStr(pInterfaceId));
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CPropPageLogging::LogClassId (UINT pLogLevel, REFGUID pClassId, LPCTSTR pClassTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKey	lClassIdKey (pRootKey, _T("CLSID\\")+(CString)CGuidStr(pClassId), true);

	if	(lClassIdKey.IsValid ())
	{
		LogMessage (pLogLevel, _T("")); \
		if	(
				(!pTitleLogged)
			||	(!*pTitleLogged)
			)
		{
			LogMessage (pLogLevel, _T("- %s"), pClassTitle);
			if	(pTitleLogged)
			{
				*pTitleLogged = true;
			}
		}
		LogRegKey (pLogLevel, lClassIdKey, CString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogAppId (UINT pLogLevel, REFGUID pAppId, LPCTSTR pAppTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKey	lAppIdKey (pRootKey, _T("APPID\\")+(CString)CGuidStr(pAppId), true);

	if	(lAppIdKey.IsValid ())
	{
		LogMessage (pLogLevel, _T("")); \
		if	(
				(!pTitleLogged)
			||	(!*pTitleLogged)
			)
		{
			LogMessage (pLogLevel, _T("- %s"), pAppTitle);
			if	(pTitleLogged)
			{
				*pTitleLogged = true;
			}
		}
		LogRegKey (pLogLevel, lAppIdKey, CString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogProgId (UINT pLogLevel, LPCTSTR pProgId, LPCTSTR pProgIdTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKey	lProgIdKey (pRootKey, pProgId, true);

	if	(lProgIdKey.IsValid ())
	{
		LogMessage (pLogLevel, _T("")); \
		if	(
				(!pTitleLogged)
			||	(!*pTitleLogged)
			)
		{
			LogMessage (pLogLevel, _T("- %s"), pProgIdTitle);
			if	(pTitleLogged)
			{
				*pTitleLogged = true;
			}
		}
		LogRegKey (pLogLevel, lProgIdKey, CString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogTypeLib (UINT pLogLevel, REFGUID pTypeLibId, LPCTSTR pTypeLibTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKey	lTypeLibKey (pRootKey, _T("TYPELIB\\")+(CString)CGuidStr(pTypeLibId), true);

	if	(lTypeLibKey.IsValid ())
	{
		LogMessage (pLogLevel, _T("")); \
		if	(
				(!pTitleLogged)
			||	(!*pTitleLogged)
			)
		{
			LogMessage (pLogLevel, _T("- %s"), pTypeLibTitle);
			if	(pTitleLogged)
			{
				*pTitleLogged = true;
			}
		}
		LogRegKey (pLogLevel, lTypeLibKey, CString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogInterface (UINT pLogLevel, REFGUID pInterfaceId, LPCTSTR pInterfaceTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKey	lInterfaceKey (pRootKey, _T("INTERFACE\\")+(CString)CGuidStr(pInterfaceId), true);

	if	(lInterfaceKey.IsValid ())
	{
		LogMessage (pLogLevel, _T("")); \
		if	(
				(!pTitleLogged)
			||	(!*pTitleLogged)
			)
		{
			LogMessage (pLogLevel, _T("  - %s"), pInterfaceTitle);
			if	(pTitleLogged)
			{
				*pTitleLogged = true;
			}
		}
		LogRegKey (pLogLevel, lInterfaceKey, CString (pRootName) + _T("\\"), 6);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::LogRegKey (UINT pLogLevel, CRegKey & pRegKey, LPCTSTR pTitle, UINT pIndent)
{
	CString				lTitle (pTitle);
	CString				lIndent (_T(' '), (int)pIndent);
	long				lKeyNdx;
	tPtr <CRegKey>		lKey;
	long				lValNdx;
	tPtr <CRegValue>	lVal;

	LogMessage (pLogLevel, _T("%s%s%s"), lIndent, lTitle, pRegKey.Name());

	for	(lValNdx = 0; lVal = pRegKey (lValNdx); lValNdx++)
	{
		lVal->Dump (pLogLevel, NULL, pIndent+2);
	}
	for	(lKeyNdx = 0; lKey = pRegKey [lKeyNdx]; lKeyNdx++)
	{
		LogRegKey (pLogLevel, *lKey, _T("REG_KEY "), pIndent+2);
	}
}
