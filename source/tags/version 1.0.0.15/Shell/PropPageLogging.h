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
#ifndef PROPPAGELOGGING_H_INCLUDED_
#define PROPPAGELOGGING_H_INCLUDED_
#pragma once

#include "PropPageFix.h"
#include "RegistrySearch.h"
#include "Registry.h"

/////////////////////////////////////////////////////////////////////////////

class CPropPageLogging : public CPropertyPage, protected CRegistrySearch
{
public:
	CPropPageLogging();
	virtual ~CPropPageLogging();
	DECLARE_DYNAMIC(CPropPageLogging)

// Dialog Data
	//{{AFX_DATA(CPropPageLogging)
	enum { IDD = IDD_LOGGING };
	CButton mLogServer;
	CButton mLogControl;
	CButton mLogCore;
	CButton mLogLevelNone;
	CButton mLogLevelIfActive;
	CButton mLogLevelNormal;
	CButton mLogLevelDetail;
	CButton mLogLevelVerbose;
	CEdit mLogFileEdit;
	CButton mLogFileBrowse;
	CButton mLogFileReset;
	CEdit mLogPathEdit;
	CButton mLogPathBrowse;
	CButton mLogPathReset;
	CButton	mLogRegistry;
	//}}AFX_DATA

// Operations
	static void LogDaRegistry (UINT pLogLevel, LPCTSTR pTitle = NULL);
	static void LogMsRegistry (UINT pLogLevel, LPCTSTR pTitle = NULL);

// Overrides
	//{{AFX_VIRTUAL(CPropPageLogging)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPageLogging)
	afx_msg void OnLogComponent();
	afx_msg void OnLogLevel();
	afx_msg void OnLogFileChange();
	afx_msg void OnLogFileBrowse();
	afx_msg void OnLogFileReset();
	afx_msg void OnLogPathChange();
	afx_msg void OnLogPathBrowse();
	afx_msg void OnLogPathReset();
	afx_msg void OnLogRegistry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ShowLogging ();
	void ShowLogging (INT_PTR pKeyNdx);
	void ShowLogLevel (const CRegDWord & pLogLevel);
	void ShowLogPath (const CRegString & pLogPath);

	CRegKey * PrepUpdate (INT_PTR pKeyNdx);
	void UpdateLogPath (INT_PTR pKeyNdx);
	void UpdateLogging ();

	void SplitLogPath (LPCTSTR pLogPath, CString & pFilePath, CString & pFileName);
	CString MakeLogPath (LPCTSTR pFilePath, LPCTSTR pFileName, bool pDefaultBlank);
	CString MakeLogPath (LPCTSTR pLogPath, bool pDefaultBlank);
	CString MakeLogPath (bool pDefaultBlank);

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

	static void LogRegKey (UINT pLogLevel, CRegKey & pRegKey, LPCTSTR pTitle, UINT pIndent);

protected:
	CPropPageFix				mPropPageFix;
	const INT_PTR				mKeyNdxServer;
	const INT_PTR				mKeyNdxControl;
	const INT_PTR				mKeyNdxCore;
	INT_PTR						mKeyNdx;
	CStringArray				mLogComponent;
	COwnPtrArray <CRegKey>		mLogKey;
	COwnPtrArray <CRegDWord>	mLogLevel;
	COwnPtrArray <CRegString>	mLogPath;
	CString						mDefLogFile;
	CString						mDefLogPath;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPPAGELOGGING_H_INCLUDED_
