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
#include "Localize.h"
#include "ResName.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

HINSTANCE CLocalize::FindResource (const CResName & pResName, WORD pLangId, HINSTANCE pResModule)
{
	HINSTANCE	lRet = NULL;

	try
	{
		LPCTSTR	lResName = pResName.Name();
		HRSRC	lResource;

		if	(!pResModule)
		{
			pResModule = AfxGetResourceHandle ();
		}

#ifdef	_UNICODE
		LANGID	lLangId = pLangId;

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

		if	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, lLangId))
		{
			lRet = pResModule;
		}
		else
		if	(
				(SUBLANGID (lLangId) != SUBLANG_NEUTRAL)
			&&	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL)))
			)
		{
			lRet = pResModule;
		}
		else
		if	(
				(SUBLANGID (lLangId) != SUBLANG_DEFAULT)
			&&	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_DEFAULT)))
			)
		{
			lRet = pResModule;
		}
		else
		if	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, LANG_USER_DEFAULT))
		{
			lRet = pResModule;
		}
		else
		if	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, LANG_SYSTEM_DEFAULT))
		{
			lRet = pResModule;
		}
		else
		if	(
				(lLangId != LANG_NEUTRAL)
			&&	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, LANG_NEUTRAL))
			)
		{
			lRet = pResModule;
		}
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

LPCVOID CLocalize::LoadResource (const CResName & pResName, ULONG & pResSize, WORD pLangId, HINSTANCE pResModule)
{
	LPCVOID	lRet = NULL;

	try
	{
		LPCTSTR	lResName = pResName.Name();
		HGLOBAL	lResHandle = NULL;
		HRSRC	lResource;

		if	(!pResModule)
		{
			pResModule = AfxGetResourceHandle ();
		}

#ifdef	_UNICODE
		LANGID	lLangId = pLangId;

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

		if	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, lLangId))
		{
			lResHandle = ::LoadResource (pResModule, lResource);
		}
		else
		if	(
				(SUBLANGID (lLangId) != SUBLANG_NEUTRAL)
			&&	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_NEUTRAL)))
			)
		{
			lResHandle = ::LoadResource (pResModule, lResource);
		}
		else
		if	(
				(SUBLANGID (lLangId) != SUBLANG_DEFAULT)
			&&	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, MAKELANGID (PRIMARYLANGID (lLangId), SUBLANG_DEFAULT)))
			)
		{
			lResHandle = ::LoadResource (pResModule, lResource);
		}
		else
		if	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, LANG_USER_DEFAULT))
		{
			lResHandle = ::LoadResource (pResModule, lResource);
		}
		else
		if	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, LANG_SYSTEM_DEFAULT))
		{
			lResHandle = ::LoadResource (pResModule, lResource);
		}
 		else
		if	(
				(lLangId != LANG_NEUTRAL)
			&&	(lResource = ::FindResourceEx (pResModule, pResName.Type(), lResName, LANG_NEUTRAL))
			)
		{
			lResHandle = ::LoadResource (pResModule, lResource);
		}

		if	(
				(lResHandle)
			&&	(pResSize = (ULONG) ::SizeofResource (pResModule, lResource))
			&&	(lResHandle = ::LoadResource (pResModule, lResource))
			)
		{
			lRet = ::LockResource (lResHandle);
		}
#endif
		if	(
				(!lRet)
			&&	(lResource = ::FindResource (pResModule, lResName, pResName.Type()))
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

bool CLocalize::LoadMuiString (UINT pId, WORD pLangId, LPCWSTR & pString, ULONG & pStringSize)
{
	bool	lRet = false;
#ifdef	_UNICODE
	try
	{
		LANGID		lLangId = pLangId;
		HINSTANCE	lResModule = AfxGetResourceHandle ();
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
				(SUBLANGID (lLangId) != SUBLANG_NEUTRAL)
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
				(SUBLANGID (lLangId) != SUBLANG_DEFAULT)
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
				(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), LANG_USER_DEFAULT))
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
				(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), LANG_SYSTEM_DEFAULT))
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
				(lResource = ::FindResourceEx (lResModule, lResName.Type(), lResName.Name(), LANG_NEUTRAL))
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
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CLocalize::LoadMuiMenu (UINT pMenuId, WORD pLangId, LPCVOID & pMenuTemplate, ULONG & pTemplateSize)
{
	if	(pMenuTemplate = LoadResource (CResName (pMenuId, RT_MENU), pTemplateSize, pLangId))
	{
		return true;
	}
	return false;
}

bool CLocalize::LoadMuiDialog (UINT pDialogId, WORD pLangId, LPCVOID & pDialogTemplate, ULONG & pTemplateSize)
{
	if	(pDialogTemplate = LoadResource (CResName (pDialogId, RT_DIALOG), pTemplateSize, pLangId))
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

HINSTANCE CLocalize::GetMuiModule (LANGID pLangId)
{
	return NULL;
}

HINSTANCE CLocalize::GetMuiModule (LANGID pLangId, ULONG & pModuleNum)
{
	return NULL;
}

void CLocalize::FreeMuiModules ()
{
}
