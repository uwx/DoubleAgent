/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
#include "Localize.h"
#include "ResName.h"
#include "Log.h"

#ifdef	__AFX_H__
#include "StringMap.h"
#define	_GetResourceHandle AfxGetResourceHandle
#else
#include <AtlColl.h>
#define	_GetResourceHandle _AtlBaseModule.GetResourceInstance
#endif

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#ifdef	_DEBUG
//#define	_DEBUG_MUI_LOAD	LogDebug
//#define	_DEBUG_STR_FIND	1594
#endif

//////////////////////////////////////////////////////////////////////
#ifndef	MakeMuiPath
#define	MakeMuiPath _MakeMuiPath

static CString _MakeMuiPath (LPCTSTR pResModulePath, LPCTSTR pMuiPath, LANGID pLangId, UINT pPathNum)
{
	CString	lMuiPath;

	if	(pPathNum == 0)
	{
		lMuiPath = pResModulePath;
		PathRemoveFileSpec (lMuiPath.GetBuffer (MAX_PATH));
		PathAppend (lMuiPath.GetBuffer (MAX_PATH), pMuiPath);
		PathAppend (lMuiPath.GetBuffer (MAX_PATH), PathFindFileName (pResModulePath));
		lMuiPath.ReleaseBuffer ();
		lMuiPath += _T(".mui");
	}

	return lMuiPath;
}

#endif
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
static CStringMap <CModuleHandle, HINSTANCE>	sMuiResources;
#else
static CAtlMap <CAtlString, CModuleHandle, CStringElementTraitsI<CAtlString>, CPrimitiveElementTraits<HINSTANCE> >	sMuiResources;
#endif

void CLocalize::FreeMuiModules ()
{
	try
	{
		sMuiResources.RemoveAll ();
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

HINSTANCE CLocalize::GetMuiModule (LANGID pLangId)
{
	CString		lResourcePath;
	CString		lModulePath;
	TCHAR		lMuiPath [85];
	UINT		lPathNum;

	if	(
			(pLangId == MAKELANGID (LANG_ENGLISH, SUBLANG_NEUTRAL))
		||	(pLangId == MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT))
		||	(pLangId == MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_CAN))
		)
	{
		return NULL;
	}

#if	(WINVER >= 0x0600)
	TCHAR	lParentPath [85];

	GetLocaleInfo (MAKELCID (pLangId, SORT_DEFAULT), LOCALE_SNAME, lMuiPath, sizeof(lMuiPath)/sizeof(TCHAR));

	if	(
			(SUBLANGID (pLangId) == SUBLANG_NEUTRAL)
		&&	(GetLocaleInfo (MAKELCID (pLangId, SORT_DEFAULT), LOCALE_SPARENT, lParentPath, sizeof(lParentPath)/sizeof(TCHAR)))
		)
	{
		_tcscpy (lMuiPath, lParentPath);
	}
#else
	_stprintf (lMuiPath, _T("MUI\\%4.4X"), pLangId);
#endif

#ifdef	__AFX_H__
	if	(sMuiResources.FindKey (lMuiPath) < 0)
#else
	if	(sMuiResources.Lookup (lMuiPath) == NULL)
#endif
	{
		::GetModuleFileName (_GetResourceHandle(), lResourcePath.GetBuffer(MAX_PATH), MAX_PATH);
		lResourcePath.ReleaseBuffer ();

		for	(lPathNum = 0; !(lModulePath = MakeMuiPath (lResourcePath, lMuiPath, pLangId, lPathNum)).IsEmpty(); lPathNum++)
		{
			HINSTANCE	lModule = NULL;
			UINT		lErrMode = SetErrorMode (SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX);
			try
			{
				SetLastError (0);
				lModule = ::LoadLibraryEx (lModulePath, NULL, LOAD_LIBRARY_AS_DATAFILE);
#ifdef	_DEBUG_MUI_LOAD
				if	(LogIsActive (_DEBUG_MUI_LOAD))
				{
					LogWinErrAnon (LogAlways, GetLastError(), _T("Load [%s]"), lModulePath);
				}
#endif
			}
			catch AnyExceptionSilent
			SetErrorMode (lErrMode);

			if	(lModule)
			{
				sMuiResources [lMuiPath] = lModule;
				break;
			}
		}
	}

	if	(sMuiResources [lMuiPath].SafeIsValid ())
	{
		return sMuiResources [lMuiPath];
	}
	return NULL;
}

HINSTANCE CLocalize::GetMuiModule (LANGID pLangId, ULONG& pModuleNum)
{
	HINSTANCE	lModule = NULL;

	if	(pModuleNum == 0)
	{
		pModuleNum++;
		lModule = GetMuiModule (pLangId);
	}

	if	(
			(!lModule)
		&&	(pModuleNum == 1)
		)
	{
		pModuleNum++;
		if	(SUBLANGID (pLangId) != SUBLANG_NEUTRAL)
		{
			lModule = GetMuiModule (MAKELANGID (PRIMARYLANGID (pLangId), SUBLANG_NEUTRAL));
		}
	}

	if	(
			(!lModule)
		&&	(pModuleNum == 2)
		)
	{
		pModuleNum++;
		lModule = _GetResourceHandle ();
	}
	return lModule;
}

//////////////////////////////////////////////////////////////////////

HINSTANCE CLocalize::FindResource (const CResName& pResName, WORD pLangId, HINSTANCE pResModule)
{
	HINSTANCE	lRet = NULL;

	try
	{
		HINSTANCE	lAfxModule = _GetResourceHandle();
		LPCTSTR		lResName = pResName.Name();
		HRSRC		lResource;

		if	(!pResModule)
		{
			pResModule = lAfxModule;
		}

#ifdef	_UNICODE
		HINSTANCE	lResModule = pResModule;
		LANGID		lLangId = pLangId;
		ULONG		lModuleNum = 0;

		if	(pLangId == LANG_USER_DEFAULT)
		{
#ifdef	_LOCALIZE_BY_THREAD
			lLangId = LANGIDFROMLCID (GetThreadLocale ());
#else
			lLangId = GetUserDefaultUILanguage();
#endif
		}
		else
		if	(pLangId == LANG_SYSTEM_DEFAULT)
		{
			lLangId = GetSystemDefaultUILanguage();
		}

		do
		{
			if	(pResModule == lAfxModule)
			{
				lResModule = GetMuiModule (lLangId, lModuleNum);
			}
			if	(lResModule)
			{
				if	(
						(lResource = ::FindResourceEx (lResModule, pResName.Type(), lResName, lLangId))
					||	(
							(SUBLANGID (lLangId) != SUBLANG_NEUTRAL)
						&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), lResName, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL)))
						)
					||	(
							(lLangId != LANG_NEUTRAL)
						&&	(lResModule == pResModule)
						&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), lResName, LANG_NEUTRAL))
						)
					||	(
							(lResModule == pResModule)
						&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), lResName, LANG_USER_DEFAULT))
						)
					||	(
							(lResModule == pResModule)
						&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), lResName, LANG_SYSTEM_DEFAULT))
						)
					)
				{
					lRet = lResModule;
					break;
				}
			}
			if	(pResModule != lAfxModule)
			{
				break;
			}
		} while (lResModule != NULL);
#endif
		if	(
				(!lRet)
			&&	(lResource = ::FindResource (pResModule, lResName, pResName.Type()))
			)
		{
			lRet = pResModule;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

LPCVOID CLocalize::LoadResource (const CResName& pResName, ULONG& pResSize, WORD pLangId, HINSTANCE pResModule)
{
	LPCVOID	lRet = NULL;

	try
	{
		HINSTANCE	lAfxModule = _GetResourceHandle();
		HGLOBAL		lResHandle;
		HRSRC		lResource;

		if	(!pResModule)
		{
			pResModule = lAfxModule;
		}

#ifdef	_UNICODE
		HINSTANCE	lResModule = pResModule;
		LANGID		lLangId = pLangId;
		ULONG		lModuleNum = 0;

		if	(pLangId == LANG_USER_DEFAULT)
		{
#ifdef	_LOCALIZE_BY_THREAD
			lLangId = LANGIDFROMLCID (GetThreadLocale ());
#else
			lLangId = GetUserDefaultUILanguage();
#endif
		}
		else
		if	(pLangId == LANG_SYSTEM_DEFAULT)
		{
			lLangId = GetSystemDefaultUILanguage();
		}

		do
		{
			if	(pResModule == lAfxModule)
			{
				lResModule = GetMuiModule (lLangId, lModuleNum);
			}
			if	(lResModule)
			{
				if	(
						(
							(lResource = ::FindResourceEx (lResModule, pResName.Type(), pResName.Name(), lLangId))
						||	(
								(PRIMARYLANGID (lLangId) != LANG_NEUTRAL)
							&&	(SUBLANGID (lLangId) != SUBLANG_NEUTRAL)
							&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), pResName.Name(), MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL)))
							)
						||	(
								(PRIMARYLANGID (lLangId) != LANG_NEUTRAL)
							&&	(SUBLANGID (lLangId) != SUBLANG_DEFAULT)
							&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), pResName.Name(), MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_DEFAULT)))
							)
						||	(
								(lResModule == pResModule)
							&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), pResName.Name(), LANG_USER_DEFAULT))
							)
						||	(
								(lResModule == pResModule)
							&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), pResName.Name(), LANG_SYSTEM_DEFAULT))
							)
						||	(
								(lLangId != LANG_NEUTRAL)
							&&	(lResModule == pResModule)
							&&	(lResource = ::FindResourceEx (lResModule, pResName.Type(), pResName.Name(), LANG_NEUTRAL))
							)
						)
					&&	(pResSize = (ULONG) ::SizeofResource (lResModule, lResource))
					&&	(lResHandle = ::LoadResource (lResModule, lResource))
					)
				{
					lRet = ::LockResource (lResHandle);
					break;
				}
			}
			if	(pResModule != lAfxModule)
			{
				break;
			}
		} while (lResModule != NULL);
#endif
		if	(
				(!lRet)
			&&	(lResource = ::FindResource (pResModule, pResName.Name(), pResName.Type()))
			&&	(pResSize = (ULONG) ::SizeofResource (pResModule, lResource))
			&&	(lResHandle = ::LoadResource (pResModule, lResource))
			)
		{
			lRet = ::LockResource (lResHandle);
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

static LPCWSTR FindResString (UINT pId, WORD & pStrSize, LPCVOID pResData, ULONG pResSize)
{
	int		lStrNum = (int) pId % 16;
	LPWORD	lStrSize;
	LPCWSTR	lStrVal;

#ifdef	_DEBUG_STR_FIND
	UINT lStrId = (pId / 16) + 1;
	if	(pId == _DEBUG_STR_FIND)
	{
		LogMessage (LogDebugFast, _T("  Strings [%4d] find [%4d]"), lStrId, pId);
	}
	lStrId = (lStrId - 1) * 16;
#endif

	for	(lStrSize = (LPWORD)pResData; (pResSize > 0) && (lStrNum >= 0); lStrSize += (*lStrSize)+1)
	{
		lStrVal = (LPCWSTR)(lStrSize+1);

#ifdef	_DEBUG_STR_FIND
		if	(pId == _DEBUG_STR_FIND)
		{
			LogMessage (LogDebugFast, _T("    String [%4d] of [%3d] at [%p] [%s]"), lStrId, (*lStrSize), (LPBYTE)lStrVal-(LPBYTE)pResData, DebugStr(CString(lStrVal,*lStrSize)));
			lStrId++;
		}
#endif
		if	(lStrNum == 0)
		{
#ifdef	_DEBUG_STR_FIND
			if	(pId == _DEBUG_STR_FIND)
			{
				LogMessage (LogDebugFast, _T("    Found  [%4d] of [%3d] at [%p] [%s]"), lStrId, (*lStrSize), (LPBYTE)lStrVal-(LPBYTE)pResData, DebugStr(CString(lStrVal,*lStrSize)));
			}
#endif
			if	(pStrSize = (*lStrSize))
			{
				return lStrVal;
			}
			break;
		}
		pResSize -= ((*lStrSize)+1)*2;
		lStrNum--;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////

bool CLocalize::LoadMuiString (UINT pId, WORD pLangId, LPCWSTR& pString, ULONG& pStringSize)
{
	bool	lRet = false;
#ifdef	_UNICODE
	try
	{
		HINSTANCE	lAfxModule = _GetResourceHandle();
		LANGID		lLangId = pLangId;
		ULONG		lModuleNum = 0;
		HINSTANCE	lResModule;
		CResName	lResName ((pId / 16) + 1, RT_STRING);
		HGLOBAL		lResHandle;
		HRSRC		lResource;
		LPCVOID		lResData;
		ULONG		lResSize;
		LPCWSTR		lStrVal;
		WORD		lStrSize;

		if	(pLangId == LANG_USER_DEFAULT)
		{
#ifdef	_LOCALIZE_BY_THREAD
			lLangId = LANGIDFROMLCID (GetThreadLocale ());
#else
			lLangId = GetUserDefaultUILanguage();
#endif
		}
		else
		if	(pLangId == LANG_SYSTEM_DEFAULT)
		{
			lLangId = GetSystemDefaultUILanguage();
		}

#ifdef	_DEBUG_STR_FIND
		if	(pId == _DEBUG_STR_FIND)
		{
			LogMessage (LogDebugFast, _T("LoadMuiString [%4d] LangId [%4d (0x%4.4X)]"), pId, lLangId, lLangId);
		}
#endif
		while (lResModule = GetMuiModule (lLangId, lModuleNum))
		{
			if	(
					(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), lLangId))
				&&	(lResSize = (ULONG) ::SizeofResource (lResModule, lResource))
				&&	(lResHandle = ::LoadResource (lResModule, lResource))
				&&	(lResData = ::LockResource (lResHandle))
				&&	(lStrVal = FindResString (pId, lStrSize, lResData, lResSize))
				)
			{
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] LangId [%4d (0x%4.4X)]"), pId, lLangId, lLangId);
				}
#endif
				lRet = true;
			}
			else
			if	(
					(PRIMARYLANGID (lLangId) != LANG_NEUTRAL)
				&&	(SUBLANGID (lLangId) != SUBLANG_NEUTRAL)
				&&	(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL)))
				&&	(lResSize = (ULONG) ::SizeofResource (lResModule, lResource))
				&&	(lResHandle = ::LoadResource (lResModule, lResource))
				&&	(lResData = ::LockResource (lResHandle))
				&&	(lStrVal = FindResString (pId, lStrSize, lResData, lResSize))
				)
			{
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] LangId [%4d (0x%4.4X)]"), pId, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL), MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL));
				}
#endif
				lRet = true;
			}
			else
			if	(
					(PRIMARYLANGID (lLangId) != LANG_NEUTRAL)
				&&	(SUBLANGID (lLangId) != SUBLANG_DEFAULT)
				&&	(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_DEFAULT)))
				&&	(lResSize = (ULONG) ::SizeofResource (lResModule, lResource))
				&&	(lResHandle = ::LoadResource (lResModule, lResource))
				&&	(lResData = ::LockResource (lResHandle))
				&&	(lStrVal = FindResString (pId, lStrSize, lResData, lResSize))
				)
			{
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] LangId [%4d (0x%4.4X)]"), pId, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_DEFAULT), MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_DEFAULT));
				}
#endif
				lRet = true;
			}
			else
			if	(
					(lResModule == lAfxModule)
				&&	(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), LANG_USER_DEFAULT))
				&&	(lResSize = (ULONG) ::SizeofResource (lResModule, lResource))
				&&	(lResHandle = ::LoadResource (lResModule, lResource))
				&&	(lResData = ::LockResource (lResHandle))
				&&	(lStrVal = FindResString (pId, lStrSize, lResData, lResSize))
				)
			{
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] LangId [%4d (0x%4.4X)]"), pId, LANG_USER_DEFAULT, LANG_USER_DEFAULT);
				}
#endif
				lRet = true;
			}
			else
			if	(
					(lResModule == lAfxModule)
				&&	(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), LANG_SYSTEM_DEFAULT))
				&&	(lResSize = (ULONG) ::SizeofResource (lResModule, lResource))
				&&	(lResHandle = ::LoadResource (lResModule, lResource))
				&&	(lResData = ::LockResource (lResHandle))
				&&	(lStrVal = FindResString (pId, lStrSize, lResData, lResSize))
				)
			{
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] LangId [%4d (0x%4.4X)]"), pId, LANG_SYSTEM_DEFAULT, LANG_SYSTEM_DEFAULT);
				}
#endif
				lRet = true;
			}
			else
			if	(
					(lLangId != LANG_NEUTRAL)
				&&	(lResModule == lAfxModule)
				&&	(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), LANG_NEUTRAL))
				&&	(lResSize = (ULONG) ::SizeofResource (lResModule, lResource))
				&&	(lResHandle = ::LoadResource (lResModule, lResource))
				&&	(lResData = ::LockResource (lResHandle))
				&&	(lStrVal = FindResString (pId, lStrSize, lResData, lResSize))
				)
			{
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] LangId [%4d (0x%4.4X)]"), pId, LANG_NEUTRAL, LANG_NEUTRAL);
				}
#endif
				lRet = true;
			}

			if	(lRet)
			{
				pString = lStrVal;
				pStringSize = lStrSize;
#ifdef	_DEBUG_STR_FIND
				if	(pId == _DEBUG_STR_FIND)
				{
					LogMessage (LogDebugFast, _T("  Found [%4d] [%s]"), pId, DebugStr(CString(pString,pStringSize)));
				}
#endif
				break;
			}
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CLocalize::LoadMuiMenu (UINT pMenuId, WORD pLangId, LPCVOID& pMenuTemplate, ULONG& pTemplateSize)
{
	if	(pMenuTemplate = LoadResource (CResName (pMenuId, RT_MENU), pTemplateSize, pLangId))
	{
		return true;
	}
	return false;
}

bool CLocalize::LoadMuiDialog (UINT pDialogId, WORD pLangId, LPCVOID& pDialogTemplate, ULONG& pTemplateSize)
{
	if	(pDialogTemplate = LoadResource (CResName (pDialogId, RT_DIALOG), pTemplateSize, pLangId))
	{
		return true;
	}
	return false;
}
