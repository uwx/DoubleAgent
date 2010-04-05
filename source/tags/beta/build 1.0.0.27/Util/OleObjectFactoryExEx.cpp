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
#include <comdef.h>
#include <shlwapi.h>
#include "OleObjectFactoryExEx.h"
#include "Registry.h"
#include "GuidStr.h"

#ifdef	_UNICODE
#undef	HKEY_CLASSES_ROOT
#define	HKEY_CLASSES_ROOT CRegKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Classes"))
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (COleObjectFactoryExEx, COleObjectFactory)

COleObjectFactoryExEx::COleObjectFactoryExEx (REFCLSID clsid, CRuntimeClass* pRuntimeClass, BOOL bMultiInstance, LPCTSTR lpszProgID)
:	COleObjectFactory (clsid, pRuntimeClass, bMultiInstance, lpszProgID)
{
}

COleObjectFactoryExEx::~COleObjectFactoryExEx ()
{
}

//////////////////////////////////////////////////////////////////////

BOOL COleObjectFactoryExEx::DoUpdateRegistry (BOOL pRegister, UINT pClassNameId, bool pShellExt)
{
	BOOL	lRet;
	CString	lClassName;

	lClassName.LoadString (pClassNameId);
	lRet = DoUpdateRegistry (pRegister, (LPCTSTR) lClassName, pShellExt);

	if	(
			(lRet)
		&&	(pRegister)
		)
	{
		RegisterLocalizedString (pClassNameId);
	}
	return lRet;
}

BOOL COleObjectFactoryExEx::DoUpdateRegistry (BOOL pRegister, LPCTSTR pClassName, bool pShellExt)
{
	BOOL	lRet = FALSE;

	if	(pRegister)
	{
		CString	lProgId (m_lpszProgID);

		if	(LogIsActive ())
		{
			LogMessage (LogIfActive, _T("==> Register %hs %s"), m_pRuntimeClass->m_lpszClassName, (LPCTSTR) CGuidStr (m_clsid));
		}

		lProgId.TrimRight ();
		if	(lProgId.IsEmpty())
		{
			CRegKey	lClassKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid), false, true);

			if	(
					(pClassName)
				&&	(pClassName[0] != 0)
				)
			{
				CRegString (lClassKey, (LPCTSTR)NULL, true).Update (pClassName);
			}
			if	(afxContextIsDLL)
			{
				RegisterServer (true);
			}
			else
			{
				RegisterServer (false);
			}
			lRet = TRUE;
		}
		else
		{
			lRet = AfxOleRegisterServerClass (m_clsid, m_lpszProgID, m_lpszProgID, pClassName, OAT_DISPATCH_OBJECT);
		}

		if	(
				(lRet)
			&&	(pShellExt)
			)
		{
			CRegString (CRegKey (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"), false, false), CGuidStr (m_clsid), true).Update (pClassName);
			CRegString (CRegKey (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Blocked"), false, false), CGuidStr (m_clsid)).Delete ();
		}
	}
	else
	{
		if	(LogIsActive ())
		{
			LogMessage (LogIfActive, _T("==> Unregister %hs %s"), m_pRuntimeClass->m_lpszClassName, (LPCTSTR) CGuidStr (m_clsid));
		}

		lRet = AfxOleUnregisterServerClass (m_clsid, m_lpszProgID, m_lpszProgID, pClassName, OAT_DISPATCH_OBJECT);

		if	(lRet)
		{
			CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)).Delete ();
			CRegKey (HKEY_CLASSES_ROOT, m_lpszProgID).Delete ();
			if	(pShellExt)
			{
				CRegString (CRegKey (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved")), CGuidStr (m_clsid)).Delete ();
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterDefCategory ()
{
	RegisterDefCategory (m_clsid);
}

void COleObjectFactoryExEx::UnregisterDefCategory ()
{
	UnregisterDefCategory (m_clsid);
}

void COleObjectFactoryExEx::RegisterCategory (const GUID & pCatId)
{
	RegisterCategory (m_clsid, pCatId);
}

void COleObjectFactoryExEx::UnregisterCategory (const GUID & pCatId)
{
	UnregisterCategory (m_clsid, pCatId);
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterDefCategory (const GUID & pClsid)
{
	GUID	lDefCatid;

	CreateDefCategory (lDefCatid);
	RegisterCategory (pClsid, lDefCatid);
}

void COleObjectFactoryExEx::UnregisterDefCategory (const GUID & pClsid)
{
	GUID	lDefCatid;

	CreateDefCategory (lDefCatid);
	UnregisterCategory (pClsid, lDefCatid);
}

void COleObjectFactoryExEx::RegisterCategory (const GUID & pClsid, const GUID & pCatId)
{
	ICatRegisterPtr	lCatRegister;

	if	(
			(SUCCEEDED (LogComErr (LogIfActive, CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, __uuidof (ICatRegister), (void **) &lCatRegister))))
		&&	(SUCCEEDED (LogComErr (LogIfActive, lCatRegister->RegisterClassImplCategories (pClsid, 1, const_cast <GUID *> (&pCatId)))))
		)
	{
		if	(IsEqualGUID (pCatId, CATID_Programmable))
		{
			CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (pClsid)), _T("Programmable"), false, true);
		}
	}
}

void COleObjectFactoryExEx::UnregisterCategory (const GUID & pClsid, const GUID & pCatId)
{
	ICatRegisterPtr	lCatRegister;

	if	(
			(SUCCEEDED (LogComErr (LogIfActive, CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, __uuidof (ICatRegister), (void **) &lCatRegister))))
		&&	(SUCCEEDED (LogComErr (LogIfActive, lCatRegister->UnRegisterClassImplCategories (pClsid, 1, const_cast <GUID *> (&pCatId)))))
		)
	{
		CRegKey lClassKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (pClsid));
		CRegKey	lCategoriesKey (lClassKey, _T("Implemented Categories"), false);

		if	(IsEqualGUID (pCatId, CATID_Programmable))
		{
			CRegKey (lClassKey, _T("Programmable"), false).Delete ();
		}
		if	(
				(lCategoriesKey.IsValid ())
			&&	(lCategoriesKey.IsEmpty ())
			)
		{
			lCategoriesKey.Delete ();
		}
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterAppId (const GUID & pAppId, UINT pAppNameId, LPCTSTR pRunAs, LPCTSTR pLocalService)
{
	CString	lAppName;
	lAppName.LoadString (pAppNameId);
	RegisterAppId (pAppId, (LPCTSTR)lAppName, pRunAs, pLocalService);
}

void COleObjectFactoryExEx::RegisterAppId (const GUID & pAppId, LPCTSTR pAppName, LPCTSTR pRunAs, LPCTSTR pLocalService)
{
	CRegString (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), _T("AppID"), true).Update (CGuidStr (pAppId));
	if	(pAppName)
	{
		CRegString (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), CGuidStr (pAppId), false, true), (LPCTSTR)NULL, true).Update (pAppName);
	}
	if	(pRunAs)
	{
		CRegString (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), CGuidStr (pAppId), false, true), _T("RunAs"), true).Update (pRunAs);
	}
	if	(pLocalService)
	{
		CRegString (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), CGuidStr (pAppId), false, true), _T("LocalService"), true).Update (pLocalService);
	}
}

void COleObjectFactoryExEx::UnregisterAppId (const GUID & pAppId)
{
	CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), CGuidStr (pAppId), false).Delete ();
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterExeAppId (const GUID & pAppId, UINT pAppNameId, LPCTSTR pExeName)
{
	CString	lAppName;
	lAppName.LoadString (pAppNameId);
	RegisterExeAppId (pAppId, (LPCTSTR) lAppName, pExeName);
}

void COleObjectFactoryExEx::RegisterExeAppId (const GUID & pAppId, LPCTSTR pAppName, LPCTSTR pExeName)
{
	CString	lExeName (pExeName);

	if	(lExeName.IsEmpty ())
	{
		if	(GetModuleFileName (NULL, lExeName.GetBuffer (MAX_PATH), MAX_PATH))
		{
			PathStripPath (lExeName.GetBuffer (MAX_PATH));
		}
		lExeName.ReleaseBuffer ();
	}
	if	(!lExeName.IsEmpty ())
	{
		CRegKey	lExeKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), lExeName, false, true);

		CRegString (lExeKey, _T("AppID"), true).Update (CGuidStr (pAppId));
		if	(pAppName)
		{
			CRegString (lExeKey, NULL, true).Update (pAppName);
		}
	}
}

void COleObjectFactoryExEx::UnregisterExeAppId (LPCTSTR pExeName)
{
	CString	lExeName (pExeName);

	if	(lExeName.IsEmpty ())
	{
		if	(GetModuleFileName (NULL, lExeName.GetBuffer (MAX_PATH), MAX_PATH))
		{
			PathStripPath (lExeName.GetBuffer (MAX_PATH));
		}
		lExeName.ReleaseBuffer ();
	}
	if	(!lExeName.IsEmpty ())
	{
		CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), lExeName, false).Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterDllSurrogate (const GUID & pAppId, LPCTSTR pSurrogatePath)
{
	CRegKey		lRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), CGuidStr (pAppId), false);
	CRegString	lSurrogate (lRegKey, _T("DllSurrogate"), true);

	if	(lSurrogate.IsValid ())
	{
		lSurrogate.Update (pSurrogatePath);
	}
}

void COleObjectFactoryExEx::UnegisterDllSurrogate (const GUID & pAppId)
{
	CRegKey		lRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID")), CGuidStr (pAppId), false);
	CRegString	lSurrogate (lRegKey, _T("DllSurrogate"));

	lSurrogate.Delete ();
}

void COleObjectFactoryExEx::RegisterElevated (UINT pClassNameId, LPCTSTR pClassNamePath)
{
	RegisterElevated ((LPCTSTR) NULL);
	RegisterLocalizedString (pClassNameId, pClassNamePath);
}

void COleObjectFactoryExEx::RegisterElevated (LPCTSTR pClassNamePath)
{
	CRegKey		lRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), false);

	CRegDWord (CRegKey (lRegKey, _T("Elevation"), false, true), _T("Enabled"), true, 1).Update ();
	if	(pClassNamePath)
	{
		RegisterLocalizedString (pClassNamePath);
	}
}

void COleObjectFactoryExEx::RegisterTypeLib (const GUID & pTypeLib)
{
	CRegString (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), _T("TypeLib"), false, true), NULL, true).Update (CGuidStr (pTypeLib));
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterServer (bool pInproc, LPCTSTR pInprocHandler)
{
	CString	lFileName;
	CString	lLongName;

	GetModuleFileName (AfxGetInstanceHandle (), lFileName.GetBuffer (MAX_PATH), MAX_PATH);
	lFileName.ReleaseBuffer ();
#ifdef	_UNICODE
	if	(GetLongPathName (lFileName, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#else
	if	(GetShortPathName (lFileName, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#endif
	{
		lFileName = lLongName;
	}

	RegisterServer (lFileName, pInproc, pInprocHandler);
}

void COleObjectFactoryExEx::RegisterServer (LPCTSTR pServerPath, bool pInproc, LPCTSTR pInprocHandler)
{
	CString	lServerPath (pServerPath);
	LPCTSTR	lServerKeyName;

	if	(pInproc)
	{
		lServerKeyName = _T("InProcServer32");
	}
	else
	{
		lServerKeyName = _T("LocalServer32");
	}

	CRegKey	lClassKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid), false, true);
	CRegKey	lServerKey (lClassKey, lServerKeyName, false, true);

	PathQuoteSpaces (lServerPath.GetBuffer (MAX_PATH));
	lServerPath.ReleaseBuffer ();
	CRegString (lServerKey, NULL, true).Update (lServerPath);

	if	(
			(!pInproc)
		||	(pInprocHandler)
		)
	{
		if	(!pInprocHandler)
		{
			pInprocHandler = _T("ole32.dll");
		}
		CRegString (CRegKey (lClassKey, _T("InprocHandler32"), false, true), NULL, true).Update (pInprocHandler);
	}
}

void COleObjectFactoryExEx::UnregisterServer (bool pInproc)
{
	LPCTSTR	lServerKeyName;

	if	(pInproc)
	{
		lServerKeyName = _T("InProcServer32");
	}
	else
	{
		lServerKeyName = _T("LocalServer32");
	}

	CRegKey	(CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), lServerKeyName, false).Delete ();
	CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), _T("InprocHandler32"), false).Delete ();
}

void COleObjectFactoryExEx::RegisterThreadingModel (LPCTSTR pThreadingModel, bool pInproc, LPCTSTR pInprocHandler)
{
	LPCTSTR	lServerKeyName;

	if	(pInproc)
	{
		lServerKeyName = _T("InProcServer32");
	}
	else
	{
		lServerKeyName = _T("LocalServer32");
	}

	CRegKey	lServerKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), lServerKeyName, false, true);
	CString	lFileName;
	CString	lLongName;

	GetModuleFileName (AfxGetInstanceHandle (), lFileName.GetBuffer (MAX_PATH), MAX_PATH);
	lFileName.ReleaseBuffer ();
#ifdef	_UNICODE
	if	(GetLongPathName (lFileName, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#else
	if	(GetShortPathName (lFileName, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#endif
	{
		PathQuoteSpaces (lLongName.GetBuffer (MAX_PATH));
		lLongName.ReleaseBuffer ();
		lFileName = lLongName;
	}

	CRegString (lServerKey, NULL, true).Update (lFileName);
	CRegString (lServerKey, _T("ThreadingModel"), true).Update (pThreadingModel);

	if	(
			(!pInproc)
		||	(pInprocHandler)
		)
	{
		if	(!pInprocHandler)
		{
			pInprocHandler = _T("ole32.dll");
		}
		CRegString (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), _T("InprocHandler32"), false, true), NULL, true).Update (pInprocHandler);
	}
}

void COleObjectFactoryExEx::RegisterLocalizedString (UINT pClassNameId, LPCTSTR pClassNamePath)
{
	CString	lFileName (pClassNamePath);
	CString	lLongName;

	if	(lFileName.IsEmpty ())
	{
		GetModuleFileName (AfxGetInstanceHandle (), lFileName.GetBuffer(MAX_PATH), MAX_PATH);
		lFileName.ReleaseBuffer ();
	}
#ifdef	_UNICODE
	if	(GetLongPathName (lFileName, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#else
	if	(GetShortPathName (lFileName, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#endif
	{
		lLongName.ReleaseBuffer ();
		lFileName = lLongName;
	}

	lLongName.Format (_T("@%s,-%u"), lFileName, pClassNameId);
	RegisterLocalizedString (lLongName);
}

void COleObjectFactoryExEx::RegisterLocalizedString (LPCTSTR pClassNamePath)
{
	if	(pClassNamePath)
	{
		CRegKey	lRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (m_clsid)), false);
		CRegString (lRegKey, _T("LocalizedString"), true).Update (pClassNamePath);
	}
}

void COleObjectFactoryExEx::RegisterDefaultIcon (int pIconId, int pOpenIconId, LPCTSTR pProgId, const GUID * pClassId)
{
	CString	lModuleName;

	GetModuleFileName (AfxGetInstanceHandle (), lModuleName.GetBuffer (MAX_PATH), MAX_PATH);
	lModuleName.ReleaseBuffer ();
	RegisterDefaultIcon (lModuleName, pIconId, pOpenIconId, pProgId, pClassId);
}

void COleObjectFactoryExEx::RegisterDefaultIcon (LPCTSTR pIconFile, int pIconId, int pOpenIconId, LPCTSTR pProgId, const GUID * pClassId)
{
	CString	lIconFile (pIconFile);
	CString	lLongName;
	CString	lIconLocation;
	CString	lIconLocationOpen;

#ifdef	_UNICODE
	if	(GetLongPathName (pIconFile, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#else
	if	(GetShortPathName (pIconFile, lLongName.GetBuffer (MAX_PATH), MAX_PATH))
#endif
	{
		lLongName.ReleaseBuffer ();
		lIconFile = lLongName;
	}

	lIconLocation.Format (_T("%s,%-d"), lIconFile, -pIconId);
	if	(pOpenIconId)
	{
		lIconLocationOpen.Format (_T("%s,%-d"), lIconFile, -pOpenIconId);
	}

	if	(
			(
				(pProgId != NULL)
			||	(pProgId = m_lpszProgID)
			)
		&&	(pProgId [0])
		)
	{
		CRegString (CRegKey (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("DefaultIcon"), false, true), NULL, true).Update (lIconLocation);
		if	(pOpenIconId)
		{
			CRegString (CRegKey (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("DefaultIcon"), false, true), _T("OpenIcon"), true).Update (lIconLocationOpen);
		}
	}
	if	(
			(
				(pClassId != NULL)
			||	(pClassId = &m_clsid)
			)
		&&	(!IsEqualGUID (*pClassId, GUID_NULL))
		)
	{
		CRegString (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (*pClassId)), _T("DefaultIcon"), false, true), NULL, true).Update (lIconLocation);
		if	(pOpenIconId)
		{
			CRegString (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (*pClassId)), _T("DefaultIcon"), false, true), _T("OpenIcon"), true).Update (lIconLocationOpen);
		}
	}
}

void COleObjectFactoryExEx::RegisterInfoTip (LPCTSTR pInfoTip)
{
	CRegString (CRegKey (HKEY_CLASSES_ROOT, m_lpszProgID), _T("InfoTip"), true).Update (pInfoTip);
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterPropSheetHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName)
{
	if	(!pHandlerName)
	{
		pHandlerName = _T("Default");
	}
	CRegString (CRegKey (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, m_lpszProgID), _T("shellex"), false, true), _T("PropertySheetHandlers"), false, true), pHandlerName, false, true), NULL, true).Update ((LPCTSTR) CGuidStr (pHandlerClsid));
}

void COleObjectFactoryExEx::RegisterPropSheetHandler (LPCTSTR pProgId, LPCTSTR pHandlerName)
{
	CRegString (CRegKey (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false, true), _T("PropertySheetHandlers"), false, true), pHandlerName, false, true), NULL, true).Update ((LPCTSTR) CGuidStr (m_clsid));
}

void COleObjectFactoryExEx::UnregisterPropSheetHandler (LPCTSTR pProgId, LPCTSTR pHandlerName)
{
	CRegKey	lKey1 (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false);
	CRegKey	lKey2 (lKey1, _T("PropertySheetHandlers"));

	CRegKey (lKey2, pHandlerName, false).Delete ();
	if	(lKey2.IsEmpty ())
	{
		lKey2.Delete ();
	}
	if	(lKey1.IsEmpty ())
	{
		lKey1.Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterContextMenuHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName)
{
	RegisterContextMenuHandler (m_lpszProgID, pHandlerClsid, pHandlerName);
}

void COleObjectFactoryExEx::UnregisterContextMenuHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName)
{
	UnregisterContextMenuHandler (m_lpszProgID, pHandlerClsid, pHandlerName);
}

void COleObjectFactoryExEx::RegisterContextMenuHandler (LPCTSTR pProgId, LPCTSTR pHandlerName)
{
	RegisterContextMenuHandler (pProgId, m_clsid, pHandlerName);
}

void COleObjectFactoryExEx::UnregisterContextMenuHandler (LPCTSTR pProgId, LPCTSTR pHandlerName)
{
	UnregisterContextMenuHandler (pProgId, m_clsid, pHandlerName);
}

void COleObjectFactoryExEx::RegisterContextMenuHandler (LPCTSTR pProgId, const GUID & pHandlerClsid, LPCTSTR pHandlerName)
{
	CString	lHandlerName (pHandlerName);

	if	(lHandlerName.IsEmpty ())
	{
		if	(IsEqualGUID (pHandlerClsid, m_clsid))
		{
			lHandlerName = (CString) CGuidStr(m_clsid);
		}
		else
		{
			lHandlerName = _T("Default");
		}
	}

	CRegString (CRegKey (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false, true), _T("ContextMenuHandlers"), false, true), lHandlerName, false, true), NULL, true).Update ((LPCTSTR) CGuidStr (pHandlerClsid));
}

void COleObjectFactoryExEx::UnregisterContextMenuHandler (LPCTSTR pProgId, const GUID & pHandlerClsid, LPCTSTR pHandlerName)
{
	CString	lHandlerName (pHandlerName);

	if	(lHandlerName.IsEmpty ())
	{
		if	(IsEqualGUID (pHandlerClsid, m_clsid))
		{
			lHandlerName = (CString) CGuidStr(m_clsid);
		}
		else
		{
			lHandlerName = _T("Default");
		}
	}

	CRegKey	lKey1 (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false);
	CRegKey	lKey2 (lKey1, _T("ContextMenuHandlers"), false);

	CRegKey (lKey2, lHandlerName, false).Delete ();

	if	(lKey2.IsEmpty ())
	{
		lKey2.Delete ();
	}
	if	(lKey1.IsEmpty ())
	{
		lKey1.Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterDragDropHandler (const GUID & pHandlerClsid, LPCTSTR pHandlerName)
{
	if	(!pHandlerName)
	{
		pHandlerName = _T("Default");
	}
	CRegString (CRegKey (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, m_lpszProgID), _T("shellex"), false, true), _T("DragDropHandlers"), false, true), pHandlerName, false, true), NULL, true).Update ((LPCTSTR) CGuidStr (pHandlerClsid));
}

void COleObjectFactoryExEx::RegisterDragDropHandler (LPCTSTR pProgId, LPCTSTR pHandlerName)
{
	CRegString (CRegKey (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false, true), _T("DragDropHandlers"), false, true), pHandlerName, false, true), NULL, true).Update ((LPCTSTR) CGuidStr (m_clsid));
}

void COleObjectFactoryExEx::UnregisterDragDropHandler (LPCTSTR pProgId, LPCTSTR pHandlerName)
{
	CRegKey	lKey1 (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false);
	CRegKey	lKey2 (lKey1, _T("DragDropHandlers"));

	CRegKey (lKey2, pHandlerName, false).Delete ();
	if	(lKey2.IsEmpty ())
	{
		lKey2.Delete ();
	}
	if	(lKey1.IsEmpty ())
	{
		lKey1.Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterIconHandler (const GUID & pHandlerClsid)
{
	RegisterGenericHandler (m_lpszProgID, pHandlerClsid, GUID_NULL, _T("IconHandler"));
}

void COleObjectFactoryExEx::RegisterIconHandler (LPCTSTR pProgId)
{
	RegisterGenericHandler (pProgId, m_clsid, GUID_NULL, _T("IconHandler"));
}

void COleObjectFactoryExEx::UnregisterIconHandler (LPCTSTR pProgId)
{
	UnregisterGenericHandler (pProgId, GUID_NULL, _T("IconHandler"));
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterInfoTipHandler (const GUID & pHandlerClsid)
{
	RegisterGenericHandler (m_lpszProgID, pHandlerClsid, GUID_NULL, _T("{00021500-0000-0000-C000-000000000046}"));
}

void COleObjectFactoryExEx::RegisterInfoTipHandler (LPCTSTR pProgId)
{
	RegisterGenericHandler (pProgId, m_clsid, GUID_NULL, _T("{00021500-0000-0000-C000-000000000046}"));
}

void COleObjectFactoryExEx::UnregisterInfoTipHandler (LPCTSTR pProgId)
{
	UnregisterGenericHandler (pProgId, GUID_NULL, _T("{00021500-0000-0000-C000-000000000046}"));
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterGenericHandler (LPCTSTR pProgId, const GUID & pHandlerClsid, const GUID & pHandlerId, LPCTSTR pHandlerTypeName)
{
	CString	lHandlerTypeName (pHandlerTypeName);
	if	(lHandlerTypeName.IsEmpty())
	{
		lHandlerTypeName = (CString) CGuidStr (pHandlerId);
	}
	CRegString (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false, true), lHandlerTypeName, false, true), NULL, true).Update ((LPCTSTR) CGuidStr (pHandlerClsid));
}

void COleObjectFactoryExEx::UnregisterGenericHandler (LPCTSTR pProgId, const GUID & pHandlerId, LPCTSTR pHandlerTypeName)
{
	CString	lHandlerTypeName (pHandlerTypeName);
	if	(lHandlerTypeName.IsEmpty())
	{
		lHandlerTypeName = (CString) CGuidStr (pHandlerId);
	}
	CRegKey	lKey1 (CRegKey (HKEY_CLASSES_ROOT, pProgId), _T("shellex"), false);
	CRegKey	lKey2 (lKey1, lHandlerTypeName);

	lKey2.Delete ();
	if	(lKey1.IsEmpty ())
	{
		lKey1.Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterNamespace (LPCTSTR pNamespace, const GUID & pHandlerClsid, UINT pHandlerNameId, HKEY pRootKey)
{
	CString	lHandlerName;

	lHandlerName.LoadString (pHandlerNameId);
	RegisterNamespace (pNamespace, pHandlerClsid, lHandlerName, pRootKey);
}

void COleObjectFactoryExEx::RegisterNamespace (LPCTSTR pNamespace, const GUID & pHandlerClsid, LPCTSTR pHandlerName, HKEY pRootKey)
{
	if	(
			(pNamespace)
		&&	(
				(pRootKey == HKEY_LOCAL_MACHINE)
			||	(pRootKey == HKEY_CURRENT_USER)
			)
		)
	{
		CRegString (CRegKey (CRegKey (CRegKey (CRegKey (pRootKey, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer"), false, false), pNamespace, false, true), _T("NameSpace"), false, true), CGuidStr (pHandlerClsid), false, true), (LPCTSTR)NULL, true).Update (pHandlerName);
	}
}

void COleObjectFactoryExEx::RegisterNamespaceOption (const GUID & pHandlerClsid, LPCTSTR pOption, HKEY pRootKey)
{
	CRegString (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (pHandlerClsid)), CString (_T("ShellFolder")), false, true), pOption, true).Update (_T(""));
}

void COleObjectFactoryExEx::RegisterNamespaceAttrs (const GUID & pHandlerClsid, DWORD pShellAttrs, HKEY pRootKey)
{
	CRegDWord (CRegKey (CRegKey (CRegKey (HKEY_CLASSES_ROOT, _T("CLSID")), CGuidStr (pHandlerClsid)), CString (_T("ShellFolder")), false, true), _T("Attributes"), true).SetValue (pShellAttrs).Update ();
}

void COleObjectFactoryExEx::UnregisterNamespace (LPCTSTR pNamespace, const GUID & pHandlerClsid, HKEY pRootKey)
{
	if	(
			(pNamespace)
		&&	(
				(pRootKey == HKEY_LOCAL_MACHINE)
			||	(pRootKey == HKEY_CURRENT_USER)
			)
		)
	{
		CRegKey (CRegKey (CRegKey (CRegKey (pRootKey, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer")), pNamespace), _T("NameSpace"), false), CGuidStr (pHandlerClsid), false).Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

void COleObjectFactoryExEx::RegisterBrowserHelperObject (const GUID & pClassId, LPCTSTR pClassName)
{
	CRegKey	lRegKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects"), false);
	CRegKey	lHelperObject (lRegKey, (LPCTSTR) CGuidStr (pClassId), false, true);

	if	(pClassName)
	{
		CRegString (lHelperObject, (LPCTSTR) NULL, true).Update (pClassName);
	}
}

void COleObjectFactoryExEx::UnegisterBrowserHelperObject (const GUID & pClassId)
{
	CRegKey	lRegKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects"), false);
	CRegKey	lHelperObject (lRegKey, (LPCTSTR) CGuidStr (pClassId), false, false);

	if	(lHelperObject.IsValid ())
	{
		lHelperObject.Delete ();
	}
}

//////////////////////////////////////////////////////////////////////

bool COleObjectFactoryExEx::FindExtProgId (LPCTSTR pExt, CString & pProgId, CString * pProgIdName)
{
	bool	lRet = false;
	CRegKey	lExtKey (HKEY_CLASSES_ROOT, pExt, true);

	if	(lExtKey.IsValid ())
	{
		pProgId = lExtKey.Value ().Value ();
		pProgId.TrimLeft ();
		pProgId.TrimRight ();

		if	(!pProgId.IsEmpty ())
		{
			lRet = true;

			if	(pProgIdName)
			{
				(*pProgIdName) = CRegKey (HKEY_CLASSES_ROOT, pProgId, true).Value ().Value ();
				pProgIdName->TrimLeft ();
				pProgIdName->TrimRight ();
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString COleObjectFactoryExEx::MakeRelatedPath (LPCTSTR pFileName)
{
	CString	lPath;

	GetModuleFileName (AfxGetInstanceHandle (), lPath.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec (lPath.GetBuffer (MAX_PATH));
	PathAppend (lPath.GetBuffer (MAX_PATH), pFileName);
	lPath.ReleaseBuffer ();

	return lPath;
}

bool COleObjectFactoryExEx::RegisterComProxy (LPCTSTR pProxyFileName, bool pRegister)
{
	bool	lRet = false;
	UINT	lErrMode = SetErrorMode (SEM_NOOPENFILEERRORBOX);

	try
	{
		CString				lProxyPath = MakeRelatedPath (pProxyFileName);
		CModuleHandle		lModuleHandle = ::LoadLibrary (lProxyPath);
		LPCSTR				lProcName;
		LPFNCANUNLOADNOW	lProc;
		HRESULT				lProcResult;

		if	(lModuleHandle.SafeIsValid ())
		{
			if	(pRegister)
			{
				lProcName = "DllRegisterServer";
			}
			else
			{
				lProcName = "DllUnregisterServer";
			}

			if	(lProc = (LPFNCANUNLOADNOW) ::GetProcAddress (lModuleHandle, lProcName))
			{
				lProcResult = E_FAIL;
				try
				{
					lProcResult = (*lProc) ();
				}
				catch AnyExceptionSilent

				if	(LogIsActive())
				{
					LogComErrAnon (LogAlways, lProcResult, _T("%hs in %s"), lProcName, lProxyPath);
				}
			}
			else
			if	(LogIsActive())
			{
				LogMessage (LogIfActive, _T("[%hs] not found in [%s]"), lProcName, lProxyPath);
			}
		}
		else
		if	(LogIsActive())
		{
			LogWinErr (LogIfActive, GetLastError(), _T("Load [%s"), lProxyPath);
		}
	}
	catch AnyExceptionDebug

	SetErrorMode (lErrMode);
	return lRet;
}
