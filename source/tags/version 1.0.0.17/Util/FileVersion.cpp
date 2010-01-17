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
#include "StdAfx.h"
#include <shlwapi.h>
#include "FileVersion.h"
#include "StringArrayEx.h"
#include "Log.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CFileVersion::CFileVersion (void * pVersionInfo)
{
	GetVersionInfo (pVersionInfo);
}

CFileVersion::CFileVersion (VS_FIXEDFILEINFO & pVersionInfo)
{
	memcpy (&mFileInfo, &pVersionInfo, sizeof(VS_FIXEDFILEINFO));
}

CFileVersion::CFileVersion (LPCTSTR pFileName)
{
	GetFileInfo (pFileName);
}

CFileVersion::CFileVersion (HMODULE pModule)
{
	GetModuleInfo (pModule);
}

CFileVersion::CFileVersion (const CFileVersion & pSource)
{
	mFileInfo = pSource.mFileInfo;
}

CFileVersion::~CFileVersion ()
{
}

//////////////////////////////////////////////////////////////////////

bool CFileVersion::IsValid () const
{
	if	(mFileInfo.dwSignature == 0xFEEF04BD)
	{
		return true;
	}
	return false;
}

void CFileVersion::GetVersionInfo (void * pVersionInfo)
{
	if	(pVersionInfo)
	{
		VS_VERSION_INFO_W * lVersionInfoW = (VS_VERSION_INFO_W *) pVersionInfo;
		VS_VERSION_INFO_A * lVersionInfoA = (VS_VERSION_INFO_A *) pVersionInfo;

		if	(
				(lVersionInfoW->Value.dwSignature == 0xFEEF04BD)
			&&	(lVersionInfoW->szKey [sizeof (lVersionInfoW->szKey) / sizeof (WCHAR)] == 0)
			&&	(wcscmp (lVersionInfoW->szKey, L"VS_VERSION_INFO") == 0)
			)
		{
			memcpy (&mFileInfo, &lVersionInfoW->Value, sizeof (VS_FIXEDFILEINFO));
		}
		else
		if	(
				(lVersionInfoA->Value.dwSignature == 0xFEEF04BD)
			&&	(lVersionInfoA->szKey [sizeof (lVersionInfoA->szKey) - 1] == 0)
			&&	(strcmp (lVersionInfoA->szKey, "VS_VERSION_INFO") == 0)
			)
		{
			memcpy (&mFileInfo, &lVersionInfoA->Value, sizeof (VS_FIXEDFILEINFO));
		}
	}
}

void CFileVersion::GetFileInfo (LPCTSTR pFileName)
{
	if	(pFileName)
	{
		try
		{
			DWORD				lInfoSize;
			DWORD				lDummy;
			tArrayPtr <BYTE>	lBuffer;

			if	(
					(lInfoSize = GetFileVersionInfoSize (pFileName, &lDummy))
				&&	(lBuffer = new BYTE [lInfoSize])
				&&	(GetFileVersionInfo (pFileName, 0, lInfoSize, lBuffer))
				)
			{
				GetVersionInfo ((LPBYTE) lBuffer);
			}
		}
		catch AnyExceptionSilent
	}
}

void CFileVersion::GetModuleInfo (HMODULE pModule)
{
	try
	{
		TCHAR				lModuleName [MAX_PATH];
		DWORD				lInfoSize;
		DWORD				lDummy;
		tArrayPtr <BYTE>	lBuffer;

		if	(
				(GetModuleFileName (pModule, lModuleName, MAX_PATH))
			&&	(lInfoSize = GetFileVersionInfoSize (lModuleName, &lDummy))
			&&	(lBuffer = new BYTE [lInfoSize])
			&&	(GetFileVersionInfo (lModuleName, 0, lInfoSize, lBuffer))
			)
		{
			GetVersionInfo ((LPBYTE) lBuffer);
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

ULONGLONG CFileVersion::FileVersion () const
{
	ULARGE_INTEGER	lFileVersion;

	lFileVersion.HighPart = mFileInfo.dwFileVersionMS;
	lFileVersion.LowPart = mFileInfo.dwFileVersionLS;

	return lFileVersion.QuadPart;
}

ULONGLONG CFileVersion::ProductVersion () const
{
	ULARGE_INTEGER	lProductVersion;

	lProductVersion.HighPart = mFileInfo.dwProductVersionMS;
	lProductVersion.LowPart = mFileInfo.dwProductVersionLS;

	return lProductVersion.QuadPart;
}

CString CFileVersion::FileVersionString () const
{
	return FormatVersion (FileVersion ());
}

CString CFileVersion::ProductVersionString () const
{
	return FormatVersion (ProductVersion ());
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CFileVersion::FormatVersion (ULONGLONG pVersion, LPCTSTR pVersionString, LPCTSTR pFormatString)
{
	CString			lRet;
	ULARGE_INTEGER	lVersion;
	CString			lVersionString (pVersionString);
	bool			lStringSame;

	if	(pVersion)
	{
		lVersion.QuadPart = pVersion;
		lRet.Format (_T("%u.%u.%u.%u"), HIWORD (lVersion.HighPart), LOWORD (lVersion.HighPart), HIWORD (lVersion.LowPart), LOWORD (lVersion.LowPart));
	}

	if	(!lVersionString.IsEmpty ())
	{
		MakeValidVersion (lVersionString);

		lStringSame = (lVersionString == lRet);

		lVersionString = pVersionString;
		lVersionString.TrimLeft ();
		lVersionString.TrimRight ();
		lVersionString.Replace (_T(", "), _T("."));
		lVersionString.Replace (',', '.');

		if	(
				(lRet.IsEmpty ())
			||	(lStringSame)
			)
		{
			lRet = lVersionString;
		}
		else
		{
			if	(pFormatString)
			{
				CString	lFormatted;
				LPCTSTR lInserts [2];
				lInserts [0] = lRet;
				lInserts [1] = lVersionString;
				AfxFormatStrings (lFormatted, pFormatString, lInserts, 2);
				lRet = lFormatted;
			}
			else
			{
				lRet += " \"";
				lRet += lVersionString;
				lRet += "\"";
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

static inline bool GetPartVal (LPCTSTR pPart, ULONG & pPartVal)
{
	LPTSTR	lPartEnd;
	pPartVal = _tcstoul (pPart, &lPartEnd, 10);
	pPartVal = min (pPartVal, (ULONG) USHRT_MAX);
	return (lPartEnd [0] == 0);
}

bool CFileVersion::ParseVersion (LPCTSTR pVersionString, ULONGLONG & pVersion)
{
	bool			lRet = false;
	CString			lVersionString (pVersionString);
	ULARGE_INTEGER	lVersion = {0, 0};
	CStringArray	lParts;
	ULONG			lPartVal;
	int				lPartNdx = 0;
	int				lChar;

	lVersionString.TrimLeft ();
	lVersionString.TrimRight ();
	lVersionString.Replace (_T(", "), _T("."));
	lVersionString.Replace (',', '.');

	lParts.SetSize (4);

	while ((lChar = lVersionString.Find ('.')) >= 0)
	{
		if	(lPartNdx <= lParts.GetUpperBound ())
		{
			lParts [lPartNdx] = lVersionString.Left (lChar);
		}
		lVersionString.Delete (0, lChar+1);
		lPartNdx++;
	}

	if	(lPartNdx <= lParts.GetUpperBound ())
	{
		lVersionString.FreeExtra ();
		lParts [lPartNdx] = lVersionString;
	}

	if	(lParts.GetSize () > 0)
	{
		if	(GetPartVal (lParts [0], lPartVal))
		{
			lVersion.HighPart = MAKELONG (0, lPartVal);
			lRet = true;
		}

		if	(lParts.GetSize () > 1)
		{
			if	(GetPartVal (lParts [1], lPartVal))
			{
				lVersion.HighPart = MAKELONG (lPartVal, HIWORD (lVersion.HighPart));
			}
			else
			{
				lRet = false;
			}
		}

		if	(lParts.GetSize () > 2)
		{
			if	(GetPartVal (lParts [2], lPartVal))
			{
				lVersion.LowPart = MAKELONG (0, lPartVal);
			}
			else
			{
				lRet = false;
			}
		}

		if	(lParts.GetSize () > 3)
		{
			if	(GetPartVal (lParts [3], lPartVal))
			{
				lVersion.LowPart = MAKELONG (lPartVal, HIWORD (lVersion.LowPart));
			}
			else
			{
				lRet = false;
			}
		}

		pVersion = lVersion.QuadPart;
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CFileVersion::MakeValidVersion (CString & pVersion, bool pSkipTrailingZero)
{
	CString			lVersion (pVersion);
	CStringArray	lParts;
	CString			lPart;
	unsigned long	lPartVal;
	INT_PTR			lPartNdx = 0;
	int				lChar;

	lVersion.TrimLeft ();
	lVersion.TrimRight ();
	lVersion.Replace (_T(", "), _T("."));
	lVersion.Replace (',', '.');

	while ((lChar = lVersion.Find ('.')) >= 0)
	{
		lParts.Add (lVersion.Left (lChar));
		lVersion.Delete (0, lChar+1);
	}

	lVersion.FreeExtra ();
	if	(!lVersion.IsEmpty ())
	{
		lParts.Add (lVersion);
	}

	lVersion.Empty ();

	for	(lPartNdx = min (lParts.GetUpperBound (), 3); lPartNdx >= 0; lPartNdx--)
	{
		lPart = lParts [lPartNdx];
		if	(lPart.IsEmpty ())
		{
			lPartVal = 0;
		}
		else
		{
			lPartVal = _tcstoul (lPart, NULL, 10);
			lPartVal = min (lPartVal, (unsigned long) USHRT_MAX);
		}
		lPart.Format (_T("%lu"), lPartVal);

		if	(
				(!lVersion.IsEmpty ())
			||	(lPartNdx == 0)
			||	(lPartVal)
			||	(!pSkipTrailingZero)
			)
		{
			if	(!lVersion.IsEmpty ())
			{
				lPart += _T(".");
			}
			lVersion = lPart + lVersion;
		}
	}

	if	(lVersion != pVersion)
	{
		pVersion = lVersion;
		return true;
	}

	return false;
}

bool CFileVersion::ExtractVersion (LPCTSTR pString, CString & pVersion, bool pLabeledOnly, bool pIncludeFirstWord)
{
	bool			lRet = false;
	CStringArray	lWords;
	int				lNdx;
	ULONGLONG		lVersion;

	MakeStringArray (pString, lWords, _T(" "));

	for	(lNdx = (pIncludeFirstWord ? 0 : 1); lNdx <= lWords.GetUpperBound (); lNdx++)
	{
		const CString &	lWord = lWords [lNdx];

		if	(_istdigit (lWord [0]))
		{
			if	(
					(
						(!pLabeledOnly)
					||	(
							(lNdx > 0)
						&&	(
								(lWords [lNdx-1].CompareNoCase (_T("version")) == 0)
							||	(lWords [lNdx-1].CompareNoCase (_T("ver")) == 0)
							||	(lWords [lNdx-1].CompareNoCase (_T("ver.")) == 0)
							)
						)
					)
				&&	(ParseVersion (lWord, lVersion))
				)
			{
				pVersion = lWord;
				MakeValidVersion (pVersion);
				lRet = true;
				break;
			}
		}
		else
		if	(
				(lWord.GetLength () > 1)
			&&	(
					(lWord [0] == _T('v'))
				||	(lWord [0] == _T('V'))
				)
			&&	(_istdigit (lWord [1]))
			&&	(ParseVersion (((LPCTSTR) lWord)+1, lVersion))
			)
		{
			pVersion = lWord.Mid (1);
			MakeValidVersion (pVersion);
			lRet = true;
			break;
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

typedef HRESULT (CALLBACK * tDllGetVersion) (DLLVERSIONINFO *pdvi);

bool CFileVersion::GetModuleVersion (LPCTSTR pModuleName, ULONGLONG & pVersion, ULONGLONG & pFileVersion, bool pLoad)
{
	bool	lRet = false;

	try
	{
		HINSTANCE	lModuleHandle;
		bool		lModuleLoaded = false;

		if	(
				(lModuleHandle = GetModuleHandle (pModuleName))
			||	(
					(pLoad)
				&&	(lModuleHandle = LoadLibraryEx (pModuleName, NULL, 0))
				&&	(lModuleLoaded = true)
				)
			)
		{
			try
			{
				tSS <DLLVERSIONINFO, DWORD>	lVersionInfo;
				tDllGetVersion				lDllGetVersion;
				ULARGE_INTEGER				lVersion;

				if	(lDllGetVersion = (tDllGetVersion) GetProcAddress (lModuleHandle, "DllGetVersion"))
				{
					try
					{
						if	(SUCCEEDED ((*lDllGetVersion) (&lVersionInfo)))
						{
							lRet = true;
						}
					}
					catch AnyExceptionSilent
				}

				lVersion.HighPart = MAKELONG (lVersionInfo.dwMinorVersion, lVersionInfo.dwMajorVersion);
				lVersion.LowPart = MAKELONG (0, lVersionInfo.dwBuildNumber);
				pVersion = lVersion.QuadPart;
			}
			catch AnyExceptionSilent

			try
			{
				pFileVersion = CFileVersion (lModuleHandle).FileVersion ();
				lRet = true;
			}
			catch AnyExceptionSilent
		}

		if	(lModuleLoaded)
		{
			FreeLibrary (lModuleHandle);
		}
	}
	catch AnyExceptionSilent

	return lRet;
}


bool CFileVersion::GetShellVersion (ULONGLONG & pVersion, ULONGLONG & pFileVersion, bool pLoad)
{
	return GetModuleVersion (_T("SHELL32.DLL"), pVersion, pFileVersion, pLoad);
}

bool CFileVersion::GetComCtlVersion (ULONGLONG & pVersion, ULONGLONG & pFileVersion, bool pLoad)
{
	return GetModuleVersion (_T("COMCTL32.DLL"), pVersion, pFileVersion, pLoad);
}

void CFileVersion::LogShellVersion (UINT pLogLevel, bool pLoad)
{
	ULONGLONG	lVersion;
	ULONGLONG	lFileVersion;

	if	(
			(LogIsActive (pLogLevel))
		&&	(GetShellVersion (lVersion, lFileVersion, pLoad))
		)
	{
		LogMessage (pLogLevel, _T("Shell Version %s (%s)"), FormatVersion (lVersion), FormatVersion (lFileVersion));
	}
}

void CFileVersion::LogComCtlVersion (UINT pLogLevel, bool pLoad)
{
	ULONGLONG	lVersion;
	ULONGLONG	lFileVersion;

	if	(
			(LogIsActive (pLogLevel))
		&&	(GetComCtlVersion (lVersion, lFileVersion, pLoad))
		)
	{
		LogMessage (pLogLevel, _T("ComCtl Version %s (%s)"), FormatVersion (lVersion), FormatVersion (lFileVersion));
	}
}


