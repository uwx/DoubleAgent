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
#pragma once
#include "PropertyPage.h"
#include "RegistrySearch.h"
#include "Registry.h"

/////////////////////////////////////////////////////////////////////////////

class CPropPageLogging : public CAtlPropertyPage, protected CRegistrySearch
{
public:
	CPropPageLogging();
	virtual ~CPropPageLogging();

// Dialog Data
	enum { IDD = IDD_LOGGING };
	CContainedWindow mLogServer;
	CContainedWindow mLogControl;
	CContainedWindow mLogCore;
	CContainedWindow mLogLevelNone;
	CContainedWindow mLogLevelIfActive;
	CContainedWindow mLogLevelNormal;
	CContainedWindow mLogLevelDetail;
	CContainedWindow mLogLevelVerbose;
	CContainedWindow mLogTraceActions;
	CContainedWindow mLogCrashDump;
	CContainedWindow mLogFileEdit;
	CContainedWindow mLogFileBrowse;
	CContainedWindow mLogFileReset;
	CContainedWindow mLogPathEdit;
	CContainedWindow mLogPathBrowse;
	CContainedWindow mLogPathReset;
	CContainedWindow mLogRegistry;

// Operations
	static void LogDaRegistry (UINT pLogLevel, LPCTSTR pTitle = NULL);
	static void LogMsRegistry (UINT pLogLevel, LPCTSTR pTitle = NULL);

// Overrides
protected:
	virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnApply(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnLogComponent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogLevel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogTraceActions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogCrashDump(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogFileChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogFileBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogFileReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogPathBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogPathChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogPathReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLogRegistry(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

	BEGIN_MSG_MAP(CPropPageLogging)
		NOTIFY_CODE_HANDLER(PSN_APPLY, OnApply)
		COMMAND_HANDLER(IDC_LOG_SERVER, BN_CLICKED, OnLogComponent)
		COMMAND_HANDLER(IDC_LOG_CONTROL, BN_CLICKED, OnLogComponent)
		COMMAND_HANDLER(IDC_LOG_CORE, BN_CLICKED, OnLogComponent)
		COMMAND_HANDLER(IDC_LOG_LEVEL_NONE, BN_CLICKED, OnLogLevel)
		COMMAND_HANDLER(IDC_LOG_LEVEL_IFACTIVE, BN_CLICKED, OnLogLevel)
		COMMAND_HANDLER(IDC_LOG_LEVEL_NORMAL, BN_CLICKED, OnLogLevel)
		COMMAND_HANDLER(IDC_LOG_LEVEL_DETAIL, BN_CLICKED, OnLogLevel)
		COMMAND_HANDLER(IDC_LOG_LEVEL_VERBOSE, BN_CLICKED, OnLogLevel)
		COMMAND_HANDLER(IDC_LOG_TRACE_ACTIONS, BN_CLICKED, OnLogTraceActions)
		COMMAND_HANDLER(IDC_LOG_CRASH_DUMP, BN_CLICKED, OnLogCrashDump)
		COMMAND_HANDLER(IDC_LOG_FILE_BROWSE, BN_CLICKED, OnLogFileBrowse)
		COMMAND_HANDLER(IDC_LOG_FILE_RESET, BN_CLICKED, OnLogFileReset)
		COMMAND_HANDLER(IDC_LOG_PATH_BROWSE, BN_CLICKED, OnLogPathBrowse)
		COMMAND_HANDLER(IDC_LOG_PATH_RESET, BN_CLICKED, OnLogPathReset)
		COMMAND_HANDLER(IDC_LOG_REGISTRY, BN_CLICKED, OnLogRegistry)
		COMMAND_HANDLER(IDC_LOG_FILE, EN_CHANGE, OnLogFileChange)
		COMMAND_HANDLER(IDC_LOG_PATH, EN_CHANGE, OnLogPathChange)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void ShowLogging ();
	void ShowLogging (INT_PTR pKeyNdx);
	void ShowLogLevel (const CRegDWord & pLogLevel);
	void ShowLogPath (const CRegString & pLogPath);

	CRegKeyEx * PrepUpdate (INT_PTR pKeyNdx);
	void UpdateLogPath (INT_PTR pKeyNdx);
	void UpdateLogging ();

	void SplitLogPath (LPCTSTR pLogPath, CAtlString & pFilePath, CAtlString & pFileName);
	CAtlString MakeLogPath (LPCTSTR pFilePath, LPCTSTR pFileName, bool pDefaultBlank);
	CAtlString MakeLogPath (LPCTSTR pLogPath, bool pDefaultBlank);
	CAtlString MakeLogPath (bool pDefaultBlank);

	static bool LogClassId (UINT pLogLevel, REFGUID pClassId, LPCTSTR pClassTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName);
	static bool LogAppId (UINT pLogLevel, REFGUID pAppId, LPCTSTR pAppTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName);
	static bool LogProgId (UINT pLogLevel, LPCTSTR pProgId, LPCTSTR pProgIdTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName);
	static bool LogTypeLib (UINT pLogLevel, REFGUID pTypeLibId, LPCTSTR pTypeLibTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName);
	static bool LogInterface (UINT pLogLevel, REFGUID pInterfaceId, LPCTSTR pInterfaceTitle, HKEY pGlobalRootKey, HKEY pUserRootKey, HKEY pClassesRootKey, LPCTSTR pGlobalRootName, LPCTSTR pUserRootName, LPCTSTR pClassesRootName);

	static bool LogClassId (UINT pLogLevel, REFGUID pClassId, LPCTSTR pClassTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged = NULL);
	static bool LogAppId (UINT pLogLevel, REFGUID pAppId, LPCTSTR pAppTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged = NULL);
	static bool LogProgId (UINT pLogLevel, LPCTSTR pProgId, LPCTSTR pProgIdTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged = NULL);
	static bool LogTypeLib (UINT pLogLevel, REFGUID pTypeLibId, LPCTSTR pTypeLibTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged = NULL);
	static bool LogInterface (UINT pLogLevel, REFGUID pInterfaceId, LPCTSTR pInterfaceTitle, HKEY pRootKey, LPCTSTR pRootName, bool * pTitleLogged = NULL);

	static void LogRegKey (UINT pLogLevel, CRegKeyEx & pRegKey, LPCTSTR pTitle, UINT pIndent);

protected:
	bool BrowseForFile (CAtlString & pFileName, DWORD pFlags = 0);
	bool BrowseForFolder (CAtlString & pFolderPath);
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

protected:
	const INT_PTR					mKeyNdxServer;
	const INT_PTR					mKeyNdxControl;
	const INT_PTR					mKeyNdxCore;
	INT_PTR							mKeyNdx;
	CAtlStringArray					mLogComponent;
	CAtlOwnPtrArray <CRegKeyEx>		mLogKey;
	CAtlOwnPtrArray <CRegDWord>		mLogLevel;
	CAtlOwnPtrArray <CRegString>	mLogPath;
	tPtr <CRegKeyEx>				mLogSettingsKey;
	tPtr <CRegDWord>				mLogCrashValue;
	tPtr <CRegDWord>				mLogTraceValue;
	CAtlString						mDefLogFile;
	CAtlString						mDefLogPath;
	CAtlString						mBrowseInitPath;
};

/////////////////////////////////////////////////////////////////////////////
