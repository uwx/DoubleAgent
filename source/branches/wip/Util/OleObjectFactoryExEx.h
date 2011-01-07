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
#if !defined(AFX_OLEOBJECTFACTORYEXEX_H__4B2D998E_5DAF_4A64_8064_9945C72ADC5F__INCLUDED_)
#define AFX_OLEOBJECTFACTORYEXEX_H__4B2D998E_5DAF_4A64_8064_9945C72ADC5F__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class COleObjectFactoryExEx : public COleObjectFactory
{
public:
	COleObjectFactoryExEx (REFCLSID clsid, CRuntimeClass* pRuntimeClass, BOOL bMultiInstance, LPCTSTR lpszProgID);
	virtual ~COleObjectFactoryExEx ();
	DECLARE_DYNAMIC (COleObjectFactoryExEx)

// Attributes
public:
	const GUID & ClassId () const {return m_clsid;}
	LPCTSTR ProgId () const {return m_lpszProgID;}
	BOOL WasRegistered () const {return m_bRegistered;}

// Operations
public:
	BOOL UpdateRegistryNow (BOOL pRegister) {return UpdateRegistry (pRegister);}

// Implementation
protected:
	BOOL DoUpdateRegistry (BOOL pRegister, UINT pClassNameId, bool pShellExt = false);
	BOOL DoUpdateRegistry (BOOL pRegister, LPCTSTR pClassName, bool pShellExt = false);

public:
	void RegisterDefCategory ();
	void UnregisterDefCategory ();
	void RegisterCategory (const GUID & pCatId);
	void UnregisterCategory (const GUID & pCatId);

	static void RegisterDefCategory (const GUID & pClsid);
	static void UnregisterDefCategory (const GUID & pClsid);
	static void RegisterCategory (const GUID & pClsid, const GUID & pCatId);
	static void UnregisterCategory (const GUID & pClsid, const GUID & pCatId);

	bool RegisterProgIdVer (LPCTSTR pProgId, int pProgIdVerMajor, int pProgIdVerMinor, LPCTSTR pProgIdName = NULL);
	bool RegisterProgIdVer (LPCTSTR pProgId, LPCTSTR pProgIdVersionned, LPCTSTR pProgIdName = NULL);
	static bool RegisterProgIdVer (const GUID & pClsid, LPCTSTR pProgId, LPCTSTR pProgIdVersionned, LPCTSTR pProgIdName);
	void UnregisterProgIdVer (LPCTSTR pProgId, int pProgIdVerMajor, int pProgIdVerMinor);
	static void UnregisterProgIdVer (LPCTSTR pProgId, LPCTSTR pProgIdVersionned);

	void RegisterAppId (const GUID & pAppId, UINT pAppNameId, LPCTSTR pRunAs = NULL, LPCTSTR pLocalService = NULL);
	void RegisterAppId (const GUID & pAppId, LPCTSTR pAppName = NULL, LPCTSTR pRunAs = NULL, LPCTSTR pLocalService = NULL);
	void RegisterAppIdSelf (UINT pAppNameId, LPCTSTR pRunAs = NULL, LPCTSTR pLocalService = NULL) {RegisterAppId (m_clsid, pAppNameId, pRunAs, pLocalService);}
	void RegisterAppIdSelf (LPCTSTR pAppName = NULL, LPCTSTR pRunAs = NULL, LPCTSTR pLocalService = NULL) {RegisterAppId (m_clsid, pAppName, pRunAs, pLocalService);}
	void UnregisterAppId (const GUID & pAppId);
	void UnregisterAppIdSelf () {UnregisterAppId (m_clsid);}

	void RegisterExeAppId (const GUID & pAppId, UINT pAppNameId, LPCTSTR pExeName = NULL);
	void RegisterExeAppId (const GUID & pAppId, LPCTSTR pAppName = NULL, LPCTSTR pExeName = NULL);
	void RegisterExeAppIdSelf (UINT pAppNameId, LPCTSTR pExeName = NULL) {RegisterExeAppId (m_clsid, pAppNameId, pExeName);}
	void RegisterExeAppIdSelf (LPCTSTR pAppName = NULL, LPCTSTR pExeName = NULL) {RegisterExeAppId (m_clsid, pAppName, pExeName);}
	void UnregisterExeAppId (LPCTSTR pExeName = NULL);

	void RegisterDllSurrogate (const GUID & pAppId, LPCTSTR pSurrogatePath = NULL);
	void RegisterDllSurrogateSelf (LPCTSTR pSurrogatePath = NULL) {RegisterDllSurrogate (m_clsid, pSurrogatePath);}
	void UnegisterDllSurrogate (const GUID & pAppId);
	void UnregisterDllSurrogateSelf () {UnegisterDllSurrogate (m_clsid);}

	void RegisterServer (bool pInproc = true, LPCTSTR pInprocHandler = NULL);
	void RegisterServer (LPCTSTR pServerPath, bool pInproc = true, LPCTSTR pInprocHandler = NULL);
	void UnregisterServer (bool pInproc = true);

	void RegisterElevated (UINT pClassNameId, LPCTSTR pClassNamePath = NULL);
	void RegisterElevated (LPCTSTR pClassNamePath = NULL);
	void RegisterTypeLib (const GUID & pTypeLib);

	void RegisterThreadingModel (LPCTSTR pThreadingModel, bool pInproc = true, LPCTSTR pInprocHandler = NULL);
	void RegisterApartmentThreaded (bool pInproc = true, LPCTSTR pInprocHandler = NULL) {RegisterThreadingModel (_T("Apartment"), pInproc, pInprocHandler);}
	void RegisterFreeThreaded (bool pInproc = true, LPCTSTR pInprocHandler = NULL) {RegisterThreadingModel (_T("Free"), pInproc, pInprocHandler);}
	void RegisterBothThreaded (bool pInproc = true, LPCTSTR pInprocHandler = NULL) {RegisterThreadingModel (_T("Both"), pInproc, pInprocHandler);}

	void RegisterLocalizedString (UINT pClassNameId, LPCTSTR pClassNamePath = NULL);
	void RegisterLocalizedString (LPCTSTR pClassNamePath);
	void RegisterDefaultIcon (int pIconId, int pOpenIconId = 0, LPCTSTR pProgId = NULL, const GUID * pClassId = NULL);
	void RegisterDefaultIcon (LPCTSTR pIconFile, int pIconId, int pOpenIconId = 0, LPCTSTR pProgId = NULL, const GUID * pClassId = NULL);
	void RegisterInfoTip (LPCTSTR pInfoTip);

	void RegisterPropSheetHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL);
	void RegisterPropSheetHandler (LPCTSTR pProgId, LPCTSTR pHandlerName);
	void UnregisterPropSheetHandler (LPCTSTR pProgId, LPCTSTR pHandlerName);

	void RegisterContextMenuHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL);
	void UnregisterContextMenuHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL);
	void RegisterContextMenuHandler (LPCTSTR pProgId, LPCTSTR pHandlerName);
	void UnregisterContextMenuHandler (LPCTSTR pProgId, LPCTSTR pHandlerName);
	void RegisterContextMenuHandler (LPCTSTR pProgId, const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL);
	void UnregisterContextMenuHandler (LPCTSTR pProgId, const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL);

	void RegisterDragDropHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL);
	void RegisterDragDropHandler (LPCTSTR pProgId, LPCTSTR pHandlerName);
	void UnregisterDragDropHandler (LPCTSTR pProgId, LPCTSTR pHandlerName);

	void RegisterIconHandler (const GUID & pHandlerClsid);
	void RegisterIconHandler (LPCTSTR pProgId);
	void UnregisterIconHandler (LPCTSTR pProgId);

	void RegisterInfoTipHandler (const GUID & pHandlerClsid);
	void RegisterInfoTipHandler (LPCTSTR pProgId);
	void UnregisterInfoTipHandler (LPCTSTR pProgId);

	void RegisterGenericHandler (LPCTSTR pProgId, const GUID & pHandlerClsid, const GUID & pHandlerId, LPCTSTR pHandlerTypeName = NULL);
	void UnregisterGenericHandler (LPCTSTR pProgId, const GUID & pHandlerId, LPCTSTR pHandlerTypeName = NULL);

	static void RegisterNamespace (LPCTSTR pNamespace, const GUID & pHandlerClsid, UINT pHandlerNameId, HKEY pRootKey = HKEY_LOCAL_MACHINE);
	static void RegisterNamespace (LPCTSTR pNamespace, const GUID & pHandlerClsid, LPCTSTR pHandlerName = NULL, HKEY pRootKey = HKEY_LOCAL_MACHINE);
	static void RegisterNamespaceOption (const GUID & pHandlerClsid, LPCTSTR pOption, HKEY pRootKey = HKEY_LOCAL_MACHINE);
	static void RegisterNamespaceAttrs (const GUID & pHandlerClsid, DWORD pShellAttrs, HKEY pRootKey = HKEY_LOCAL_MACHINE);
	static void UnregisterNamespace (LPCTSTR pNamespace, const GUID & pHandlerClsid, HKEY pRootKey = HKEY_LOCAL_MACHINE);

	static void RegisterBrowserHelperObject (const GUID & pClassId, LPCTSTR pClassName = NULL);
	static void UnegisterBrowserHelperObject (const GUID & pClassId);

	static void CreateDefCategory (GUID & pCatId);
	static bool FindExtProgId (LPCTSTR pExt, CString & pProgId, CString * pProgIdName = NULL);
	static CString MakeRelatedPath (LPCTSTR pFileName);
	static bool RegisterComProxy (LPCTSTR pProxyFileName, bool pRegister);
};

//////////////////////////////////////////////////////////////////////

#ifdef	COleObjectFactoryEx
#undef	COleObjectFactoryEx
#define	COleObjectFactoryEx COleObjectFactoryExEx
#endif

//////////////////////////////////////////////////////////////////////
#include "Registry.h"
#include "GuidStr.h"

#ifndef	_DEF_CATEGORY_GUID
#define	_DEF_CATEGORY_GUID	"{21556C40-465F-11D2-B3A2-00600831FF5A}"
#endif
#ifndef	_DEF_CATEGORY_NAME
#define	_DEF_CATEGORY_NAME	"Cinnamon"
#endif

inline void COleObjectFactoryExEx::CreateDefCategory (GUID & pCatId)
{
	::CRegKeyEx	lRootKey (HKEY_CLASSES_ROOT, _T("Component Categories"));
	::CRegKeyEx	lCatKey;
	LPCTSTR		lCatGuid = _T(_DEF_CATEGORY_GUID);

	pCatId = CGuidStr::Parse (lCatGuid);

	if	(lCatKey.Open (lRootKey, lCatGuid, false, true) == ERROR_SUCCESS)
	{
		::CRegString (lCatKey, _T("409"), true).Update (_T(_DEF_CATEGORY_NAME));
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifndef	DECLARE_ISALLUSERS
#define	DECLARE_ISALLUSERS(c) \
	static bool IsInstalledAllUsers () \
	{ \
		return CRegKeyEx (HKEY_LOCAL_MACHINE, CString (_T("Software\\Classes\\CLSID\\")) + (CString) CGuidStr (__uuidof (c)), true).IsValid (); \
	}
#endif

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_OLEOBJECTFACTORYEXEX_H__4B2D998E_5DAF_4A64_8064_9945C72ADC5F__INCLUDED_)
