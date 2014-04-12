/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_THREADSECURITY_H__FC0F9248_FAD1_4C4A_A751_313DF8D71D49__INCLUDED_)
#define AFX_THREADSECURITY_H__FC0F9248_FAD1_4C4A_A751_313DF8D71D49__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CThreadSecurity
{
public:
	CThreadSecurity (HANDLE pProcessHandle = NULL);
	virtual ~CThreadSecurity ();

// Attributes

// Operations
	bool GetBackupPrivileges (bool pAllThreads = false);
	bool GetSecurityPrivileges (bool pAllThreads = true);
	bool GetDebugPrivileges (bool pAllThreads = false);

	bool GetPrivileges (bool pAllThreads, LPCTSTR pSecurityName, ...);
	void RestorePrivileges ();

	static bool AllowUiPiMessage (WORD pMessageId, bool pAllow = true);

	friend void LogProcessSecurity (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogProcessUser (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogProcessOwner (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogProcessGroup (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogProcessGroups (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogProcessPrivileges (HANDLE pProcess, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogProcessIntegrity (HANDLE pProcess, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);

	friend void LogThreadSecurity (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogThreadUser (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogThreadOwner (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogThreadGroup (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogThreadGroups (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogThreadPrivileges (HANDLE pThread, UINT pLogLevel, LPCTSTR pTitle = NULL);

	friend void LogTokenSecurity (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenUser (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenOwner (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenGroup (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenGroups (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenPrivileges (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenType (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenIntegrity (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);
	friend void LogTokenElevation (HANDLE pToken, UINT pLogLevel, LPCTSTR pTitle = NULL);

	friend void LogTokenPrivileges (PTOKEN_PRIVILEGES pTokenPrivileges, UINT pLogLevel, LPCTSTR pTitle = NULL);

// Implementation
protected:
	HANDLE			mProcessHandle;
	HANDLE			mThreadHandle;
	CGenericHandle	mTokenHandle;
	CByteArray		mOldPrivileges;
	CByteArray		mNewPrivileges;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_THREADSECURITY_H__FC0F9248_FAD1_4C4A_A751_313DF8D71D49__INCLUDED_)
