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
#include <commdlg.h>
#include <shlobj.h>
#include "DaShell.h"
#include "DaGlobalConfig.h"
#include "PropPageLogging.h"
#include "PropPageRegistry.h"
#include "Registry.h"
#include "GuidStr.h"
#include "DaServerOdl.h"
#include "DaControlOdl.h"

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

CPropPageLogging::CPropPageLogging()
:	CAtlPropertyPage (IDD),
	mKeyNdxServer (0),
	mKeyNdxControl (1),
	mKeyNdxCore (2),
	mKeyNdx (-1),
	mDefLogFile (_T(_DOUBLEAGENT_NAME) _T(".log"))
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageLogging::CPropPageLogging"), this);
#endif
	SHGetSpecialFolderPath (NULL, mDefLogPath.GetBuffer (MAX_PATH), CSIDL_DESKTOPDIRECTORY, FALSE);
	PathRemoveBackslash (mDefLogPath.GetBuffer (MAX_PATH));
	mDefLogPath.ReleaseBuffer ();

	mLogComponent.SetAtGrow (mKeyNdxServer, _T(_SERVER_REGNAME));
	mLogComponent.SetAtGrow (mKeyNdxControl, _T(_CONTROL_REGNAME));
	mLogComponent.SetAtGrow (mKeyNdxCore, _T(_CORE_REGNAME));
}

CPropPageLogging::~CPropPageLogging()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageLogging::~CPropPageLogging"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageLogging::OnInitDialog ()
{
	mLogServer.Attach			(GetDlgItem (IDC_LOG_SERVER));
	mLogControl.Attach			(GetDlgItem (IDC_LOG_CONTROL));
	mLogCore.Attach				(GetDlgItem (IDC_LOG_CORE));
	mLogLevelNone.Attach		(GetDlgItem (IDC_LOG_LEVEL_NONE));
	mLogLevelIfActive.Attach	(GetDlgItem (IDC_LOG_LEVEL_IFACTIVE));
	mLogLevelNormal.Attach		(GetDlgItem (IDC_LOG_LEVEL_NORMAL));
	mLogLevelDetail.Attach		(GetDlgItem (IDC_LOG_LEVEL_DETAIL));
	mLogLevelVerbose.Attach		(GetDlgItem (IDC_LOG_LEVEL_VERBOSE));
	mLogTraceActions.Attach		(GetDlgItem (IDC_LOG_TRACE_ACTIONS));
	mLogCrashDump.Attach		(GetDlgItem (IDC_LOG_CRASH_DUMP));
	mLogFileEdit.Attach			(GetDlgItem (IDC_LOG_FILE));
	mLogFileBrowse.Attach		(GetDlgItem (IDC_LOG_FILE_BROWSE));
	mLogFileReset.Attach		(GetDlgItem (IDC_LOG_FILE_RESET));
	mLogPathEdit.Attach			(GetDlgItem (IDC_LOG_PATH));
	mLogPathBrowse.Attach		(GetDlgItem (IDC_LOG_PATH_BROWSE));
	mLogPathReset.Attach		(GetDlgItem (IDC_LOG_PATH_RESET));
	mLogRegistry.Attach			(GetDlgItem (IDC_LOG_REGISTRY));

	ShowLogging ();
	return TRUE;
}

LRESULT CPropPageLogging::OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	UpdateLogging ();
	return PSNRET_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::ShowLogging ()
{
	if	(
			(!Button_GetCheck (mLogServer))
		&&	(!Button_GetCheck (mLogControl))
		&&	(!Button_GetCheck (mLogCore))
		)
	{
		Button_SetCheck (mLogServer, TRUE);
	}

	if	(Button_GetCheck (mLogServer))
	{
		ShowLogging (mKeyNdx = mKeyNdxServer);
	}
	else
	if	(Button_GetCheck (mLogControl))
	{
		ShowLogging (mKeyNdx = mKeyNdxControl);
	}
	else
	if	(Button_GetCheck (mLogCore))
	{
		ShowLogging (mKeyNdx = mKeyNdxCore);
	}

	if	(!mLogCrashValue)
	{
		mLogSettingsKey = new CRegKeyEx (CRegKeyEx (HKEY_CURRENT_USER, gProfileKeyDa, false), _T(_SERVER_REGNAME), false, true);
		mLogCrashValue = new CRegDWord (*mLogSettingsKey, _T("CrashDump"), true);
		mLogTraceValue = new CRegDWord (*mLogSettingsKey, _T("ActionTrace"), true);
	}
	Button_SetCheck (mLogCrashDump, mLogCrashValue->Value() != 0);
	mLogCrashDump.ShowWindow (Button_GetCheck (mLogServer) ? SW_SHOWNA : SW_HIDE);
	Button_SetCheck (mLogTraceActions, mLogTraceValue->Value() != 0);
	mLogTraceActions.ShowWindow (Button_GetCheck (mLogServer) ? SW_SHOWNA : SW_HIDE);
}

void CPropPageLogging::ShowLogging (INT_PTR pKeyNdx)
{
	CRegKeyEx		lRootKey (HKEY_CURRENT_USER, gProfileKeyDa, true);
	CRegKeyEx		lItemKey (lRootKey, mLogComponent [pKeyNdx], true);
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

	Button_SetCheck (mLogLevelNone, lLogLevel == 0);
	Button_SetCheck (mLogLevelIfActive, lLogLevel == LogIfActive);
	Button_SetCheck (mLogLevelNormal, lLogLevel == LogNormal);
	Button_SetCheck (mLogLevelDetail, lLogLevel == LogDetail);
	Button_SetCheck (mLogLevelVerbose, lLogLevel == LogVerbose);
}

void CPropPageLogging::ShowLogPath (const CRegString & pLogPath)
{
	CAtlString	lFilePath;
	CAtlString	lFileName;

	SplitLogPath (CRegString (pLogPath).Expand().Value(), lFilePath, lFileName);

	mLogFileEdit.SetWindowText (lFileName);
	mLogPathEdit.SetWindowText (lFilePath);
	mLogFileReset.EnableWindow (lFileName.CompareNoCase (mDefLogFile) != 0);
	mLogPathReset.EnableWindow (lFilePath.CompareNoCase (mDefLogPath) != 0);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CRegKeyEx * CPropPageLogging::PrepUpdate (INT_PTR pKeyNdx)
{
	if	(
			(pKeyNdx >= 0)
		&&	(pKeyNdx <= mLogComponent.GetUpperBound ())
		&&	(!mLogKey (pKeyNdx))
		)
	{
		CRegKeyEx	lRootKey (HKEY_CURRENT_USER, gProfileKeyDa, false, true);

		mLogKey.SetAtGrow (pKeyNdx, new CRegKeyEx (lRootKey, mLogComponent [pKeyNdx], false, true));
	}
	return mLogKey (pKeyNdx);
}

void CPropPageLogging::UpdateLogPath (INT_PTR pKeyNdx)
{
	CAtlString	lLogPath = MakeLogPath (true);
	CRegKeyEx *	lLogKey;

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

void CPropPageLogging::SplitLogPath (LPCTSTR pLogPath, CAtlString & pFilePath, CAtlString & pFileName)
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

CAtlString CPropPageLogging::MakeLogPath (LPCTSTR pFilePath, LPCTSTR pFileName, bool pDefaultBlank)
{
	CAtlString	lFilePath (pFilePath);
	CAtlString	lFileName (pFileName);

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

	SplitLogPath (CAtlString ((LPCTSTR)lFilePath), lFilePath, lFileName);

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

CAtlString CPropPageLogging::MakeLogPath (LPCTSTR pLogPath, bool pDefaultBlank)
{
	CAtlString	lFileName;
	CAtlString	lFilePath;

	SplitLogPath (pLogPath, lFilePath, lFileName);
	return MakeLogPath (lFilePath, lFileName, pDefaultBlank);
}

CAtlString CPropPageLogging::MakeLogPath (bool pDefaultBlank)
{
	CAtlString	lFileName;
	CAtlString	lFilePath;

	mLogFileEdit.GetWindowText (lFileName);
	mLogPathEdit.GetWindowText (lFilePath);

	return MakeLogPath (lFilePath, lFileName, pDefaultBlank);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageLogging::OnLogComponent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	ShowLogging ();
	return 0;
}

LRESULT CPropPageLogging::OnLogLevel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CRegKeyEx *	lLogKey;
	DWORD		lLogLevel = Button_GetCheck (mLogLevelIfActive) ? LogIfActive : Button_GetCheck (mLogLevelNormal) ? LogNormal : Button_GetCheck (mLogLevelDetail) ? LogDetail : Button_GetCheck (mLogLevelVerbose) ? LogVerbose : 0;

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
	return 0;
}

LRESULT CPropPageLogging::OnLogTraceActions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if	(mLogTraceValue)
	{
		mLogTraceValue->SetValue (Button_GetCheck (mLogTraceActions) ? 2 : 0);
		SetModified (TRUE);
	}
	return 0;
}

LRESULT CPropPageLogging::OnLogCrashDump(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if	(mLogCrashValue)
	{
		mLogCrashValue->SetValue (Button_GetCheck (mLogCrashDump) ? 2 : 0);
		SetModified (TRUE);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageLogging::OnLogFileChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	UpdateLogPath (mKeyNdx);
	bHandled = FALSE;
	return 0;
}

LRESULT CPropPageLogging::OnLogFileBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CAtlString	lFilePath = MakeLogPath (false);
	CAtlString	lFileName;

	if	(BrowseForFile (lFilePath))
	{
		SplitLogPath (MakeLogPath (lFilePath, false), lFilePath, lFileName);
		mLogFileEdit.SetWindowText (lFileName);
		mLogPathEdit.SetWindowText (lFilePath);
		UpdateLogPath (mKeyNdx);
		ShowLogging (mKeyNdx);
	}
	return 0;
}

LRESULT CPropPageLogging::OnLogFileReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CAtlString	lFileName;
	CAtlString	lFilePath;

	SplitLogPath (MakeLogPath (NULL, NULL, false), lFilePath, lFileName);
	mLogFileEdit.SetWindowText (lFileName);
	UpdateLogPath (mKeyNdx);
	ShowLogging (mKeyNdx);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageLogging::OnLogPathChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	UpdateLogPath (mKeyNdx);
	bHandled = FALSE;
	return 0;
}

LRESULT CPropPageLogging::OnLogPathBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CAtlString	lFilePath;
	CAtlString	lFileName;

	SplitLogPath (MakeLogPath (NULL, NULL, false), lFilePath, lFileName);

	if	(BrowseForFolder (lFilePath))
	{
		SplitLogPath (MakeLogPath (lFilePath, NULL, false), lFilePath, lFileName);
		mLogPathEdit.SetWindowText (lFilePath);
		UpdateLogPath (mKeyNdx);
		ShowLogging (mKeyNdx);
	}
	return 0;
}

LRESULT CPropPageLogging::OnLogPathReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CAtlString	lFileName;
	CAtlString	lFilePath;

	SplitLogPath (MakeLogPath (NULL, NULL, false), lFilePath, lFileName);
	mLogPathEdit.SetWindowText (lFilePath);
	UpdateLogPath (mKeyNdx);
	ShowLogging (mKeyNdx);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CPropPageLogging::BrowseForFile (CAtlString & pFileName, DWORD pFlags)
{
	bool						lRet = false;
	tSS <OPENFILENAME, DWORD>	lOpenFile;
	CAtlString					lFileName (pFileName);

	lOpenFile.lpstrTitle = _T("Log File");
	lOpenFile.lpstrFilter = _T("All Files\0*.*\0Log Files\0*.log\0");
	lOpenFile.lpstrDefExt = _T(".log");
	lOpenFile.Flags = OFN_EXPLORER|OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_DONTADDTORECENT;
	lOpenFile.lpstrFile = lFileName.GetBuffer (MAX_PATH);
	lOpenFile.nMaxFile = MAX_PATH;

	if	(GetSaveFileName (&lOpenFile))
	{
		lFileName.ReleaseBuffer ();
		pFileName = lFileName;
		lRet = true;
	}
	return lRet;
}

bool CPropPageLogging::BrowseForFolder (CAtlString & pFolderPath)
{
	bool			lRet = false;
	tS <BROWSEINFO>	lBrowseInfo;
	LPITEMIDLIST	lBrowseResult;
	CAtlString		lBrowseFolder;

	lBrowseInfo.hwndOwner = m_hWnd;
	lBrowseInfo.lpszTitle = _T("Specify the log file's location");
	lBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_VALIDATE|BIF_USENEWUI;
	lBrowseInfo.lpfn = &BrowseCallbackProc;
	lBrowseInfo.lParam = (LPARAM)this;

	mBrowseInitPath = pFolderPath;
	if	(lBrowseResult = SHBrowseForFolder (&lBrowseInfo))
	{
		if	(SHGetPathFromIDList (lBrowseResult, lBrowseFolder.GetBuffer(MAX_PATH)))
		{
			lBrowseFolder.ReleaseBuffer ();
			pFolderPath = lBrowseFolder;
			lRet = true;
		}
		ILFree (lBrowseResult);
	}
	return lRet;
}

int CALLBACK CPropPageLogging::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CPropPageLogging * lThis;

	if	(
			(uMsg == BFFM_INITIALIZED)
		&&	(lThis = (CPropPageLogging*) lpData)
		&&	(!lThis->mBrowseInitPath.IsEmpty ())
		)
	{
		SendMessage (hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)(LPCTSTR)lThis->mBrowseInitPath);
	}
	return 0;
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

LRESULT CPropPageLogging::OnLogRegistry(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CAtlString	lLogPath;

	lLogPath = (mLogPath (mKeyNdxServer)) ? mLogPath (mKeyNdxServer)->Value() : CRegString (CRegKeyEx (HKEY_CURRENT_USER, gProfileKeyDa, true), mLogComponent [mKeyNdxServer], true).Value();
	lLogPath = MakeLogPath (lLogPath, false);

	if	(BrowseForFile (lLogPath, OFN_PATHMUSTEXIST))
	{
		HCURSOR		lPrevCursor = SetCursor (::LoadCursor (NULL, IDC_WAIT));
		UINT		lLogLevel = gLogLevel;
		CAtlString	lLogFileName = gLogFileName;

		LogStop ();
		LogStart (false, lLogPath, LogIfActive);

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
		SetCursor (lPrevCursor);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::LogDaRegistry (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle (pTitle);
			CRegKeyEx	lGlobalClasses;
			CRegKeyEx	lUserClasses;
			CRegKeyEx	lClassesRoot;

			if	(lTitle.IsEmpty())
			{
				lTitle = "Double Agent Registry";
			}

			GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot);
#ifdef	_WIN64
			CRegKeyEx	lGlobalClasses32;
			CRegKeyEx	lUserClasses32;
			CRegKeyEx	lClassesRoot32;
			GetRootKeys (lGlobalClasses32, lUserClasses32, lClassesRoot32, true);
#endif
			LogMessage (pLogLevel, lTitle);

			_LogClassIdPlatform (pLogLevel, __uuidof(DaServer), _T(_SERVER_PROGID_NAME))
			_LogClassIdPlatform (pLogLevel, __uuidof(DaServerHandler), _T(_SERVER_HANDLER_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_SERVER_PROGID), _T(_SERVER_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_SERVER_PROGID_VER), _T(_SERVER_PROGID_VER));
			_LogAppIdPlatform (pLogLevel, __uuidof(DaServer), _T(_SERVER_PROGID_NAME))

			_LogClassIdPlatform (pLogLevel, __uuidof(CDaAgentCtl), _T(_CONTROL_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_CONTROL_PROGID), _T(_CONTROL_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_CONTROL_PROGID_VER), _T(_CONTROL_PROGID_VER));

			_LogClassIdAny (pLogLevel, __uuidof(CDaCharacterProps), _T(_PROPERTIES_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_PROPERTIES_PROGID), _T(_PROPERTIES_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_PROPERTIES_PROGID_VER), _T(_PROPERTIES_PROGID_VER));

			_LogClassIdPlatform (pLogLevel, __uuidof(CAgentPreviewWnd), _T(_PREVIEW_PROGID_NAME))
			_LogProgIdPlatform (pLogLevel, _T(_PREVIEW_PROGID), _T(_PREVIEW_PROGID));
			_LogProgIdPlatform (pLogLevel, _T(_PREVIEW_PROGID_VER), _T(_PREVIEW_PROGID_VER));

			if	(IsWindowsVista_AtLeast())
			{
				_LogClassIdAny (pLogLevel, __uuidof(CDaElevatedSettings), _T("Double Agent Emulation Management"))
				_LogProgIdAny (pLogLevel, _T(_ELEVATED_SETTINGS_PROGID), _T(_ELEVATED_SETTINGS_PROGID));
				_LogProgIdAny (pLogLevel, _T(_ELEVATED_SETTINGS_PROGID_VER), _T(_ELEVATED_SETTINGS_PROGID_VER));
				_LogAppIdAny (pLogLevel, __uuidof(CDaElevatedSettings), _T("Double Agent Emulation Management"))
			}

			_LogTypeLibAny (pLogLevel, __uuidof(DaServerTypeLib), _T(_SERVER_PROGID_NAME));
#ifdef	_DEBUG
			_LogInterfaceAny (pLogLevel, __uuidof(IDaServer), _T("IDaServer"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaServer2), _T("IDaServer2"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCharacter), _T("IDaSvrCharacter"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCharacter2), _T("IDaSvrCharacter2"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrBalloon), _T("IDaSvrBalloon"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCommands), _T("IDaSvrCommands"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCommand), _T("IDaSvrCommand"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCommandsWindow), _T("IDaSvrCommandsWindow"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrUserInput), _T("IDaSvrUserInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrPropertySheet), _T("IDaSvrPropertySheet"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrSpeechInput), _T("IDaSvrSpeechInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrAudioOutput), _T("IDaSvrAudioOutput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrCharacterFiles), _T("IDaSvrCharacterFiles"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrSpeechEngine), _T("IDaSvrSpeechEngine"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrSpeechEngines), _T("IDaSvrSpeechEngines"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrRecognitionEngine), _T("IDaSvrRecognitionEngine"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrRecognitionEngines), _T("IDaSvrRecognitionEngines"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrNotifySink), _T("IDaSvrNotifySink"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaSvrNotifySink15), _T("IDaSvrNotifySink15"));
#endif
			_LogTypeLibAny (pLogLevel, __uuidof(DaControlTypeLib), _T(_CONTROL_PROGID_NAME));
#ifdef	_DEBUG
			_LogInterfaceAny (pLogLevel, __uuidof(IDaControl), _T("IDaControl"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaControl2), _T("IDaControl2"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCharacters), _T("IDaCtlCharacters"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCharacter), _T("IDaCtlCharacter"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCharacter2), _T("IDaCtlCharacter2"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlBalloon), _T("IDaCtlBalloon"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCommands), _T("IDaCtlCommands"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCommand), _T("IDaCtlCommand"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlRequest), _T("IDaCtlRequest"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlAudioOutput), _T("IDaCtlAudioOutput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlSpeechInput), _T("IDaCtlSpeechInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlPropertySheet), _T("IDaCtlPropertySheet"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlUserInput), _T("IDaCtlUserInput"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCommandsWindow), _T("IDaCtlCommandsWindow"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlAnimationNames), _T("IDaCtlAnimationNames"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlCharacterFiles), _T("IDaCtlCharacterFiles"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlSpeechEngine), _T("IDaCtlSpeechEngine"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlSpeechEngines), _T("IDaCtlSpeechEngines"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlRecognitionEngine), _T("IDaCtlRecognitionEngine"));
			_LogInterfaceAny (pLogLevel, __uuidof(IDaCtlRecognitionEngines), _T("IDaCtlRecognitionEngines"));
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
			CAtlString	lTitle (pTitle);
			CRegKeyEx	lGlobalClasses;
			CRegKeyEx	lUserClasses;
			CRegKeyEx	lClassesRoot;

			if	(lTitle.IsEmpty())
			{
				lTitle = "Microsoft Agent Registry";
			}

			GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot);
#ifdef	_WIN64
			CRegKeyEx	lGlobalClasses32;
			CRegKeyEx	lUserClasses32;
			CRegKeyEx	lClassesRoot32;
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
	CRegKeyEx	lClassIdKey (pRootKey, _T("CLSID\\")+(CString)CGuidStr(pClassId), true);

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
		LogRegKey (pLogLevel, lClassIdKey, CAtlString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogAppId (UINT pLogLevel, REFGUID pAppId, LPCTSTR pAppTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKeyEx	lAppIdKey (pRootKey, _T("APPID\\")+(CString)CGuidStr(pAppId), true);

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
		LogRegKey (pLogLevel, lAppIdKey, CAtlString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogProgId (UINT pLogLevel, LPCTSTR pProgId, LPCTSTR pProgIdTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKeyEx	lProgIdKey (pRootKey, pProgId, true);

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
		LogRegKey (pLogLevel, lProgIdKey, CAtlString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogTypeLib (UINT pLogLevel, REFGUID pTypeLibId, LPCTSTR pTypeLibTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKeyEx	lTypeLibKey (pRootKey, _T("TYPELIB\\")+(CString)CGuidStr(pTypeLibId), true);

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
		LogRegKey (pLogLevel, lTypeLibKey, CAtlString (pRootName) + _T("\\"), 4);
		return true;
	}
	return false;
}

bool CPropPageLogging::LogInterface (UINT pLogLevel, REFGUID pInterfaceId, LPCTSTR pInterfaceTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged)
{
	CRegKeyEx	lInterfaceKey (pRootKey, _T("INTERFACE\\")+(CString)CGuidStr(pInterfaceId), true);

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
		LogRegKey (pLogLevel, lInterfaceKey, CAtlString (pRootName) + _T("\\"), 6);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageLogging::LogRegKey (UINT pLogLevel, CRegKeyEx & pRegKey, LPCTSTR pTitle, UINT pIndent)
{
	CAtlString			lTitle (pTitle);
	CAtlString			lIndent (_T(' '), (int)pIndent);
	long				lKeyNdx;
	tPtr <CRegKeyEx>	lKey;
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
