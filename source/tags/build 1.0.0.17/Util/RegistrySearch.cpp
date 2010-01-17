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
#include "RegistrySearch.h"
#include "GuidStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

bool CRegistrySearch::GetGlobalClassesKey (CRegKey & pGlobalClasses, bool pAltPlatform)
{
#ifdef	_WIN64
	if	(pAltPlatform)
	{
		HKEY	lKey;

		if	(RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("Software\\Classes"), 0, KEY_READ|KEY_WOW64_32KEY, &lKey) == ERROR_SUCCESS)
		{
			pGlobalClasses.Attach (lKey);
		}
	}
	else
#endif
	{
		pGlobalClasses.Open (HKEY_LOCAL_MACHINE, _T("Software\\Classes"), true);
	}
	return pGlobalClasses.IsValid ();
}

bool CRegistrySearch::GetUserClassesKey (CRegKey & pUserClasses, bool pAltPlatform)
{
#ifdef	_WIN64
	if	(pAltPlatform)
	{
		HKEY	lKey;

		if	(RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_READ|KEY_WOW64_32KEY, &lKey) == ERROR_SUCCESS)
		{
			pUserClasses.Attach (lKey);
		}
	}
	else
#endif
	{
		pUserClasses.Open (HKEY_CURRENT_USER, _T("Software\\Classes"), true);
	}
	return pUserClasses.IsValid ();
}

bool CRegistrySearch::GetClassesRootKey (CRegKey & pClassesRoot, bool pAltPlatform)
{
#ifdef	_WIN64
	if	(pAltPlatform)
	{
		HKEY	lKey;

		if	(RegOpenKeyEx (HKEY_CLASSES_ROOT, NULL, 0, KEY_READ|KEY_WOW64_32KEY, &lKey) == ERROR_SUCCESS)
		{
			pClassesRoot.Attach (lKey);
		}
	}
	else
#endif
	{
		pClassesRoot.Open (HKEY_CLASSES_ROOT, NULL, true);
	}
	return pClassesRoot.IsValid ();
}

/////////////////////////////////////////////////////////////////////////////

void CRegistrySearch::GetRootKeys (CRegKey & pGlobalClasses, CRegKey & pUserClasses, CRegKey & pClassesRoot, bool pAltPlatform)
{
#ifdef	_WIN64
	if	(pAltPlatform)
	{
		HKEY	lKey;

		if	(RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("Software\\Classes"), 0, KEY_READ|KEY_WOW64_32KEY, &lKey) == ERROR_SUCCESS)
		{
			pGlobalClasses.Attach (lKey);
		}
		if	(RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_READ|KEY_WOW64_32KEY, &lKey) == ERROR_SUCCESS)
		{
			pUserClasses.Attach (lKey);
		}
		if	(RegOpenKeyEx (HKEY_CLASSES_ROOT, NULL, 0, KEY_READ|KEY_WOW64_32KEY, &lKey) == ERROR_SUCCESS)
		{
			pClassesRoot.Attach (lKey);
		}
	}
	else
#endif
	{
		pGlobalClasses.Open (HKEY_LOCAL_MACHINE, _T("Software\\Classes"), true);
		pUserClasses.Open (HKEY_CURRENT_USER, _T("Software\\Classes"), true);
		pClassesRoot.Open (HKEY_CLASSES_ROOT, NULL, true);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString CRegistrySearch::GetClassProgId (REFGUID pClassId, HKEY pRootKey)
{
	CString	lProgId;
	CRegKey	lClsIdKey (pRootKey, _T("CLSID"), true);
	CRegKey	lClassKey (lClsIdKey, (CString)CGuidStr(pClassId), true);
	CRegKey	lProgIdKey (lClassKey, _T("ProgID"), true);

	if	(lProgIdKey.IsValid ())
	{
		lProgId = lProgIdKey.Value().Value();
		lProgId.TrimLeft ();
		lProgId.TrimRight ();
	}
	return lProgId;
}

CString CRegistrySearch::GetClassViProgId (REFGUID pClassId, HKEY pRootKey)
{
	CString	lViProgId;
	CRegKey	lClsIdKey (pRootKey, _T("CLSID"), true);
	CRegKey	lClassKey (lClsIdKey, (CString)CGuidStr(pClassId), true);
	CRegKey	lViProgIdKey (lClassKey, _T("VersionIndependentProgID"), true);

	if	(lViProgIdKey.IsValid ())
	{
		lViProgId = lViProgIdKey.Value().Value();
		lViProgId.TrimLeft ();
		lViProgId.TrimRight ();
	}
	return lViProgId;
}

/////////////////////////////////////////////////////////////////////////////

CString CRegistrySearch::GetServerPath (REFGUID pClassId, HKEY pRootKey)
{
	CString	lServerPath;
	CRegKey	lClsIdKey (pRootKey, _T("CLSID"), true);
	CRegKey	lClassKey (lClsIdKey, (CString)CGuidStr(pClassId), true);

	if	(lClassKey.IsValid ())
	{
		CRegKey	lLocalServer (lClassKey, _T("LocalServer32"), true);
		CRegKey	lInprocServer (lClassKey, _T("InprocServer32"), true);

#ifdef	_DEBUG_NOT
		LogMessage (LogDebug, _T("--- Local [%s]"), lLocalServer.Value().Value());
		LogMessage (LogDebug, _T("--- Inproc [%s]"), lInprocServer.Value().Value());
#endif

		if	(lLocalServer.IsValid ())
		{
			lLocalServer.Value().Expand ();
			lServerPath = lLocalServer.Value().Value();
		}
		else
		if	(lInprocServer.IsValid ())
		{
			lInprocServer.Value().Expand ();
			lServerPath = lInprocServer.Value().Value();
		}
	}

	if	(!lServerPath.IsEmpty())
	{
		CString	lQualified;

		PathUnquoteSpaces (lServerPath.GetBuffer (MAX_PATH));
		PathMakePretty (lServerPath.GetBuffer (MAX_PATH));
		lServerPath.ReleaseBuffer ();
		lServerPath.SetAt (0, _totupper (lServerPath [0]));

		if	(PathSearchAndQualify (lServerPath, lQualified.GetBuffer (MAX_PATH), MAX_PATH))
		{
			lQualified.ReleaseBuffer ();
			lServerPath = lQualified;
		}
		if	(GetLongPathName (lServerPath, lQualified.GetBuffer (MAX_PATH), MAX_PATH))
		{
			lQualified.ReleaseBuffer ();
			lServerPath = lQualified;
		}
	}
	return lServerPath;
}

CString CRegistrySearch::GetServerPath (LPCTSTR pProgId, HKEY pRootKey)
{
	CString	lServerPath;
	CRegKey	lProgIdKey (pRootKey, pProgId, true);
	CRegKey	lProgClass (lProgIdKey, _T("CLSID"), true);

	if	(
			(lProgIdKey.IsValid ())
		&&	(lProgClass.IsValid ())
		)
	{
		lServerPath = GetServerPath (CGuidStr::Parse (lProgClass.Value().Value()), pRootKey);
	}
	return lServerPath;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString CRegistrySearch::GetAltTypeLibPath (UINT pTypeLibNum)
{
	CString	lModuleName;
	CString	lTypeLibNum;

	lTypeLibNum.Format (_T("%u"), pTypeLibNum);
	GetModuleFileName (AfxGetInstanceHandle(), lModuleName.GetBuffer (MAX_PATH), MAX_PATH);
	PathStripPath (lModuleName.GetBuffer (MAX_PATH));
	PathAppend (lModuleName.GetBuffer (MAX_PATH), lTypeLibNum);
	lModuleName.ReleaseBuffer ();

	return lModuleName;
}
