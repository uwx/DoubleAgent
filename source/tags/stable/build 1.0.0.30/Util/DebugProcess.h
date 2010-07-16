/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#if !defined(AFX_DEBUGPROCESS_H__D28EC42B_E7C3_442E_9317_F5518B24431A__INCLUDED_)
#define AFX_DEBUGPROCESS_H__D28EC42B_E7C3_442E_9317_F5518B24431A__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CDebugProcess
{
public:
	CDebugProcess ();
	CDebugProcess (HANDLE pProcessHandle);
	CDebugProcess (DWORD pProcessId);
	virtual ~CDebugProcess ();

// Attributes
public:

// Operations
public:
	void LogWorkingSet (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogWorkingSetInline (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogGuiResources (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogGuiResourcesInline (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogAddressSpace (UINT pLogLevel, bool pLogSections = false, LPCTSTR pFormat = NULL, ...);
	void LogAddressSpaceInline (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogThreads (UINT pLogLevel, bool pLogDetails = true, LPCTSTR pFormat = NULL, ...);
	void LogHeaps (UINT pLogLevel, bool pLogDetails = true, UINT pLogEntries = 0, LPCTSTR pFormat = NULL, ...);

// Implementation
protected:
	HANDLE	mProcessHandle;
	DWORD	mProcessId;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DEBUGPROCESS_H__D28EC42B_E7C3_442E_9317_F5518B24431A__INCLUDED_)
