/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <shlwapi.h>
#include "AgentFiles.h"
#include "Registry.h"

#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////////////////////////////

CAgentFiles::CAgentFiles()
{
}

CAgentFiles::~CAgentFiles()
{
}

const CAtlPtrTypeArray <CAgentFile> & CAgentFiles::Files () const
{
	return mFiles;
}

//////////////////////////////////////////////////////////////////////

HRESULT CAgentFiles::Load (LPCTSTR pPath, UINT pLogLevel)
{
	HRESULT			lResult = S_FALSE;
	CAtlString		lFindPath (pPath);
	CAtlString		lFoundPath;
	CFindHandle		lFindHandle;
	WIN32_FIND_DATA	lFindData;

	if	(lFindPath.IsEmpty ())
	{
		lFindPath = (BSTR) GetSystemCharsPath ();
	}
	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Load from [%s]"), lFindPath);
	}

	PathAppend (lFindPath.GetBuffer (MAX_PATH), _T("*.acs"));
	lFindPath.ReleaseBuffer ();

	lFindHandle = FindFirstFile (lFindPath, &lFindData);
	if	(lFindHandle.SafeIsValid ())
	{
		do
		{
			lFoundPath = lFindPath;
			PathRemoveFileSpec (lFoundPath.GetBuffer (MAX_PATH));
			PathAppend (lFoundPath.GetBuffer (MAX_PATH), lFindData.cFileName);
			lFoundPath.ReleaseBuffer ();

			try
			{
				tPtr <CAgentFile>	lFile = CAgentFile::CreateInstance();

#if	FALSE
				CAtlString lLogPath;
				SHGetSpecialFolderPath (NULL, lLogPath.GetBuffer (MAX_PATH), CSIDL_DESKTOPDIRECTORY, FALSE);
				PathAppend (lLogPath.GetBuffer (MAX_PATH), _T("TestDump"));
				PathAppend (lLogPath.GetBuffer (MAX_PATH), lFindData.cFileName);
				PathRenameExtension (lLogPath.GetBuffer (MAX_PATH), _T(".Dump.txt"));
				lLogPath.ReleaseBuffer ();
				LogStop (pLogLevel);
				LogStart (true, lLogPath);
#endif

				if	(SUCCEEDED (lFile->Open (lFoundPath, pLogLevel)))
				{
					mFiles.Add (lFile.Detach ());
					lResult = S_OK;
				}
			}
			catch AnyExceptionDebug
		}
		while (FindNextFile (lFindHandle, &lFindData));
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (GetLastError ());
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

INT_PTR CAgentFiles::FindDefChar ()
{
	CAtlString		lDefaultChar;
	CAgentFile *	lFile;
	INT_PTR			lNdx;

	lDefaultChar = (BSTR)GetDefCharPath ();
	if	(!lDefaultChar.IsEmpty ())
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mFiles.GetCount(); lNdx++)
		{
			if	(
					(lFile = mFiles (lNdx))
				&&	(lDefaultChar.CompareNoCase ((BSTR)lFile->GetPath ()) == 0)
				)
			{
				return lNdx;
			}
		}
	}
	return -1;
}

CAgentFile * CAgentFiles::GetDefChar ()
{
	CAtlString		lDefaultChar;
	CAgentFile *	lFile;
	INT_PTR			lNdx;

	lDefaultChar = (BSTR)GetDefCharPath ();
	if	(!lDefaultChar.IsEmpty ())
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mFiles.GetCount(); lNdx++)
		{
			if	(
					(lFile = mFiles (lNdx))
				&&	(lDefaultChar.CompareNoCase ((BSTR)lFile->GetPath ()) == 0)
				)
			{
				return lFile;
			}
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

tBstrPtr CAgentFiles::GetAgentPath (bool pAlternatePlatform)
{
	CAtlString	lPath;
	CAtlString	lLongPath;

	GetWindowsDirectory (lPath.GetBuffer (MAX_PATH), MAX_PATH);
#ifdef	_WIN64
	if	(pAlternatePlatform)
	{
		PathAppend (lPath.GetBuffer (MAX_PATH), _T("MsAgent"));
	}
	else
	{
		PathAppend (lPath.GetBuffer (MAX_PATH), _T("MsAgent64"));
	}
#else
	if	(pAlternatePlatform)
	{
		PathAppend (lPath.GetBuffer (MAX_PATH), _T("MsAgent64"));
	}
	else
	{
		PathAppend (lPath.GetBuffer (MAX_PATH), _T("MsAgent"));
	}
#endif
	lPath.ReleaseBuffer ();

	if	(PathIsDirectory (lPath))
	{
		if	(GetLongPathName (lPath, lLongPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			lLongPath.ReleaseBuffer ();
			lPath = lLongPath;
		}
	}
	else
	{
		lPath.Empty();
	}
	return lPath.AllocSysString();
}

tBstrPtr CAgentFiles::GetSystemCharsPath (UINT pPathNum, UINT * pPathNumFound)
{
	UINT		lPathNum = 0;
	CAtlString	lPath;
	CAtlString	lLongPath;

	for	(lPathNum = pPathNum; lPathNum <= pPathNum; lPathNum++)
	{
		if	(lPathNum == 0)
		{
			GetWindowsDirectory (lPath.GetBuffer (MAX_PATH), MAX_PATH);
			PathAppend (lPath.GetBuffer (MAX_PATH), _T("MsAgent"));
		}
		else
		if	(lPathNum == 1)
		{
			GetModuleFileName (_AtlBaseModule.GetModuleInstance(), lPath.GetBuffer(MAX_PATH), MAX_PATH);
			PathRemoveFileSpec (lPath.GetBuffer (MAX_PATH));
		}
		else
#ifdef	_WIN64
		if	(lPathNum == 2)
		{
			CAtlString	lPrograms;
			CAtlString	lProgramsAlt;

			GetModuleFileName (NULL, lPath.GetBuffer(MAX_PATH), MAX_PATH);
			PathRemoveFileSpec (lPath.GetBuffer (MAX_PATH));
			if	(GetLongPathName (lPath, lLongPath.GetBuffer(MAX_PATH), MAX_PATH))
			{
				lLongPath.ReleaseBuffer ();
				lPath = lLongPath;
			}

			SHGetSpecialFolderPath (NULL, lPrograms.GetBuffer(MAX_PATH), CSIDL_PROGRAM_FILES, false);
			lPrograms.ReleaseBuffer ();
			SHGetSpecialFolderPath (NULL, lProgramsAlt.GetBuffer(MAX_PATH), CSIDL_PROGRAM_FILESX86, false);
			lProgramsAlt.ReleaseBuffer ();

			if	(
					(!lPrograms.IsEmpty ())
				&&	(!lProgramsAlt.IsEmpty ())
				&&	(lPrograms.CompareNoCase (lProgramsAlt) != 0)
				&&	(PathRelativePathTo (lLongPath.GetBuffer(MAX_PATH), lPrograms, FILE_ATTRIBUTE_DIRECTORY, lPath, FILE_ATTRIBUTE_DIRECTORY))
				&&	(PathAppend (lProgramsAlt.GetBuffer(MAX_PATH), lLongPath))
				)
			{
				lProgramsAlt.ReleaseBuffer ();
				if	(PathFileExists (lProgramsAlt))
				{
					lPath = lProgramsAlt;
				}
				else
				{
					lPath.Empty ();
					break;
				}
			}
		}
		else
#endif
		{
			lPath.Empty ();
			lPathNum++;
			break;
		}

		PathAppend (lPath.GetBuffer (MAX_PATH), _T("Chars"));
		lPath.ReleaseBuffer ();

		if	(!PathIsDirectory (lPath))
		{
			lPath.Empty ();
			pPathNum++;
		}
	}
	lPathNum--;

	if	(
			(!lPath.IsEmpty ())
		&&	(GetLongPathName (lPath, lLongPath.GetBuffer(MAX_PATH), MAX_PATH))
		)
	{
		lLongPath.ReleaseBuffer ();
		lPath = lLongPath;
	}
	if	(pPathNumFound)
	{
		(*pPathNumFound) = lPathNum;
	}
	return lPath.AllocSysString();
}

tBstrPtr CAgentFiles::GetOfficeCharsPath ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, _T("Software\\Microsoft\\Office\\Common\\Assistant"), true);
	CRegString	lRegString (lRegKey, _T("AsstFile"));
	CAtlString	lPath;
	CAtlString	lLongPath;

	if	(
			(lRegString.IsValid ())
		&&	(!(lPath = lRegString.Value ()).IsEmpty ())
		)
	{
		if	(!PathIsDirectory (lPath))
		{
			PathRemoveFileSpec (lPath.GetBuffer (MAX_PATH));
			lPath.ReleaseBuffer ();
		}
		if	(PathIsDirectory (lPath))
		{
			if	(GetLongPathName (lPath, lLongPath.GetBuffer(MAX_PATH), MAX_PATH))
			{
				lLongPath.ReleaseBuffer ();
				lPath = lLongPath;
			}
		}
		else
		{
			lPath.Empty ();
		}
	}

	return lPath.AllocSysString();
}

//////////////////////////////////////////////////////////////////////

tBstrPtr CAgentFiles::GetDefCharPath (const CAtlStringArray * pSearchPath)
{
	CAtlString	lFileName = CRegString (CRegKeyEx (HKEY_CURRENT_USER, _T("Software\\Microsoft\\Microsoft Agent"), true), _T("SystemCharacter")).Value ();

	if	(PathIsFileSpec (lFileName))
	{
		CAtlString	lPathName;
		INT_PTR		lNdx;
		UINT		lPathNum;

		if	(pSearchPath)
		{
			for	(lNdx = 0; lNdx < (INT_PTR)pSearchPath->GetCount(); lNdx++)
			{
				PathCombine (lPathName.GetBuffer (MAX_PATH), (*pSearchPath) [lNdx], lFileName);
				lPathName.ReleaseBuffer ();
				if	(PathFileExists (lPathName))
				{
					return lPathName.AllocSysString();
				}
			}
		}

		for	(lPathNum = 0; true; lPathNum++)
		{
			lPathName = CAgentFiles::GetSystemCharsPath (lPathNum);
			if	(lPathName.IsEmpty ())
			{
				break;
			}
			PathAppend (lPathName.GetBuffer (MAX_PATH), lFileName);
			lPathName.ReleaseBuffer ();
			if	(PathFileExists (lPathName))
			{
				return lPathName.AllocSysString();
			}
		}
	}
	return lFileName.AllocSysString();
}

HRESULT CAgentFiles::SetDefCharPath (LPCTSTR pCharPath)
{
	CAtlString			lPath;
	tPtr <CAgentFile>	lAgentFile;
	HRESULT				lResult = S_OK;

	if	(LogIsActive ())
	{
		LogMessage (LogNormal|LogTime, _T("SetDefCharPath [%s]"), pCharPath);
	}

	if	(PathCanonicalize (lPath.GetBuffer (MAX_PATH), pCharPath))
	{
		if	(PathFileExists (lPath))
		{
			if	(
					(lAgentFile = CAgentFile::CreateInstance())
				&&	(SUCCEEDED (lResult = lAgentFile->Open (lPath)))
				)
			{
				CAtlString	lDefPath = (BSTR) GetSystemCharsPath ();
				CAtlString	lPathName (lPath);
				CAtlString	lFileName (lPath);
				CRegKeyEx	lRegKey (CRegKeyEx (HKEY_CURRENT_USER, _T("Software\\Microsoft"), true), _T("Microsoft Agent"), false, true);
				CRegString	lRegString (lRegKey, _T("SystemCharacter"), true);

				PathRemoveFileSpec (lPathName.GetBuffer (MAX_PATH));
				lPathName.ReleaseBuffer ();
				PathStripPath (lFileName.GetBuffer (MAX_PATH));
				lFileName.ReleaseBuffer ();

				if	(lPathName.CompareNoCase (lDefPath) == 0)
				{
					lRegString.Update (lFileName);
				}
				else
				{
					lRegString.Update (lPath);
				}

				if	(LogIsActive ())
				{
					LogMessage (LogNormal|LogTime, _T("[%s\\%s] = [%s]"), lRegKey.Name (), lRegString.Name (), lRegString.Value ());
				}
			}
		}
		else
		{
			lResult = HRESULT_FROM_WIN32 (ERROR_FILE_NOT_FOUND);
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}

	if	(LogIsActive ())
	{
		LogComErr (LogNormal|LogTime, lResult, _T("SetDefCharPath [%s]"), pCharPath);
	}
	return lResult;
}
