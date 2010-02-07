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
#if !defined(AFX_FILEVERSION_H__364A1AA7_B34C_47D2_9B43_268CDE5BD5D3__INCLUDED_)
#define AFX_FILEVERSION_H__364A1AA7_B34C_47D2_9B43_268CDE5BD5D3__INCLUDED_
#pragma once

#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////

class CFileVersion
{
public:
	CFileVersion (void * pVersionInfo);
	CFileVersion (VS_FIXEDFILEINFO & pVersionInfo);
	CFileVersion (LPCTSTR pFileName);
	CFileVersion (HMODULE pModule);
	CFileVersion (const CFileVersion & pSource);
	virtual ~CFileVersion ();

	bool IsValid () const;

	ULONGLONG FileVersion () const;
	ULONGLONG ProductVersion () const;

	CString FileVersionString () const;
	CString ProductVersionString () const;

	static CString FormatVersion (ULONGLONG pVersion, LPCTSTR pVersionString = NULL, LPCTSTR pFormatString = NULL);
	static bool ParseVersion (LPCTSTR pVersionString, ULONGLONG & pVersion);
	static bool MakeValidVersion (CString & pVersion, bool pSkipTrailingZero = false);
	static bool ExtractVersion (LPCTSTR pString, CString & pVersion, bool pLabeledOnly = true, bool pIncludeFirstWord = false);

	static bool GetModuleVersion (LPCTSTR pModuleName, ULONGLONG & pVersion, ULONGLONG & pFileVersion, bool pLoad = true);
	static bool GetShellVersion (ULONGLONG & pVersion, ULONGLONG & pFileVersion, bool pLoad = true);
	static bool GetComCtlVersion (ULONGLONG & pVersion, ULONGLONG & pFileVersion, bool pLoad = true);
	static void LogShellVersion (UINT pLogLevel, bool pLoad = true);
	static void LogComCtlVersion (UINT pLogLevel, bool pLoad = true);

protected:
#include "FileVersionStructs.h"
	virtual void GetVersionInfo (void * pVersionInfo);
	virtual void GetFileInfo (LPCTSTR pFileName);
	virtual void GetModuleInfo (HMODULE pModule);

	tS <VS_FIXEDFILEINFO>	mFileInfo;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_FILEVERSION_H__364A1AA7_B34C_47D2_9B43_268CDE5BD5D3__INCLUDED_)
