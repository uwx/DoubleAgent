/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaShell.h"
#include "PropPageRegistry.h"
#include "Registry.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "FileVersion.h"
#include "UserSecurity.h"
#include "ComElevate.h"
#ifdef	_DEBUG
#include "PropPageLogging.h"
#endif

#pragma comment (lib,"comctl32.lib")

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
#define	_DEBUG_EMULATION	LogDebugFast
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef	BCM_SETSHIELD
#define BCM_SETSHIELD	(BCM_FIRST + 0x000C)
#endif

#ifdef	_WIN64
static LPCTSTR	sNameFormat64 = _T("%s (64-bit)");
#endif
static LPCTSTR	sNameFormat32 = _T("%s (32-bit)");

/////////////////////////////////////////////////////////////////////////////

CPropPageRegistry::CPropPageRegistry()
:	CAtlPropertyPage(IDD)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageRegistry::CPropPageRegistry"), this);
#endif
	tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
	lInitControls.dwICC = ICC_WIN95_CLASSES|ICC_LINK_CLASS;
	InitCommonControlsEx (&lInitControls);
}

CPropPageRegistry::~CPropPageRegistry()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CPropPageRegistry::~CPropPageRegistry"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CPropPageRegistry::OnInitDialog ()
{
	bool	lMsAgentInstalled;
	bool	lMsAgentEmulated;

	mMaStatus.Attach		(GetDlgItem (IDC_MA_STATUS));
	mDaStatus.Attach		(GetDlgItem (IDC_DA_STATUS));
	mUseMaButton.Attach		(GetDlgItem (IDC_USE_MA));
	mUseDaButton.Attach		(GetDlgItem (IDC_USE_DA));
	mDaTree.Attach			(GetDlgItem (IDC_DA_REGISTRY));
	mMaTree.Attach			(GetDlgItem (IDC_MA_REGISTRY));

	InitFonts ();
	ShowDaRegistry ();
	lMsAgentInstalled = ShowMsRegistry ();
	lMsAgentEmulated = ShowInstallStatus ();
	InitLayout (lMsAgentInstalled, lMsAgentEmulated);
	ShowElevated ();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CPropPageRegistry::InitFonts ()
{
	HFONT			lFont;
	tS <LOGFONT>	lLogFont;

	if	(
			(!mTitleFont.GetSafeHandle())
		&&	(lFont = GetFont ())
		&&	(::GetObject (lFont, sizeof(lLogFont), &lLogFont))
		)
	{
		lLogFont.lfWidth = 0;
		lLogFont.lfHeight = MulDiv (lLogFont.lfHeight, 5, 4);
		mTitleFont = ::CreateFontIndirect (&lLogFont);
	}
	if	(mTitleFont.GetSafeHandle())
	{
		mDaStatus.SetFont (mTitleFont);
		mMaStatus.SetFont (mTitleFont);
	}

	if	(
			(!mBoldFont.GetSafeHandle())
		&&	(lFont = GetFont ())
		&&	(::GetObject (lFont, sizeof(lLogFont), &lLogFont))
		)
	{
		lLogFont.lfWidth = 0;
		lLogFont.lfWeight = FW_BOLD;
		mBoldFont = CreateFontIndirect (&lLogFont);
	}
}

void CPropPageRegistry::InitLayout (bool pMsAgentInstalled, bool pMsAgentEmulated)
{
	if	(!pMsAgentInstalled)
	{
		mMaTree.ShowWindow (SW_HIDE);
		mUseMaButton.ShowWindow (SW_HIDE);

		AlignBottom (mDaTree, mMaTree, true, ChildWndRect(mDaTree).bottom - ChildWndRect(mMaStatus).bottom);
		AlignBottom (mMaStatus, mMaTree);

		if	(pMsAgentEmulated)
		{
			mUseDaButton.ShowWindow (SW_HIDE);
			AlignRight (mDaStatus, mUseDaButton, true);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropPageRegistry::ShowDaRegistry ()
{
	CAtlString	lProgName;

	mDaTree.SendMessage (CCM_SETVERSION, max (mDaTree.SendMessage (CCM_GETVERSION), 5));
	TreeView_DeleteAllItems (mDaTree);
	mDaInstalled.Clear ();
#ifdef	_WIN64
	mDaInstalled32.Clear ();
#endif

	mDaInstalled.mServerName = _T(_SERVER_PROGID_NAME);
#ifdef	_WIN64
	if	(mDaInstalled.mServerItem = ShowClassId (mDaInstalled.mServerName, sNameFormat64, __uuidof(DaServer), mDaTree))
#else
	if	(mDaInstalled.mServerItem = ShowClassId (mDaInstalled.mServerName, (IsWow64()?sNameFormat32:NULL), __uuidof(DaServer), mDaTree))
#endif
	{
		mDaInstalled.mItemCount++;
	}
#ifdef	_WIN64
	mDaInstalled32.mServerName = _T(_SERVER_PROGID_NAME);
	if	(mDaInstalled32.mServerItem = ShowClassId (mDaInstalled32.mServerName, sNameFormat32, __uuidof(DaServer), mDaTree, true, true))
	{
		mDaInstalled32.mItemCount++;
	}
#endif

	mDaInstalled.mControlName = _T(_CONTROL_PROGID_NAME);
#ifdef	_WIN64
	if	(mDaInstalled.mControlItem = ShowClassId (mDaInstalled.mControlName, sNameFormat64, __uuidof(DaControl), mDaTree))
#else
	if	(mDaInstalled.mControlItem = ShowClassId (mDaInstalled.mControlName, (IsWow64()?sNameFormat32:NULL), __uuidof(DaControl), mDaTree))
#endif
	{
		mDaInstalled.mItemCount++;
	}
#ifdef	_WIN64
	mDaInstalled32.mControlName = _T(_CONTROL_PROGID_NAME);
	if	(mDaInstalled32.mControlItem = ShowClassId (mDaInstalled32.mControlName, sNameFormat32, __uuidof(DaControl), mDaTree, true, true))
	{
		mDaInstalled32.mItemCount++;
	}
#endif

	lProgName = _T(_PREVIEW_PROGID_NAME);
#ifdef	_WIN64
	if	(ShowClassId (lProgName, sNameFormat64, __uuidof(CAgentPreviewWnd), mDaTree))
#else
	if	(ShowClassId (lProgName, (IsWow64()?sNameFormat32:NULL), __uuidof(CAgentPreviewWnd), mDaTree))
#endif
	{
		mDaInstalled.mItemCount++;
	}
#ifdef	_WIN64
	if	(ShowClassId (lProgName, sNameFormat32, __uuidof(CAgentPreviewWnd), mDaTree, true, true))
	{
		mDaInstalled32.mItemCount++;
	}
#endif

	mDaInstalled.mCharPropsName = _T(_PROPERTIES_PROGID_NAME);
	if	(mDaInstalled.mCharPropsItem = ShowClassId (mDaInstalled.mCharPropsName, (IsWow64()?sNameFormat32:NULL), __uuidof(CDaCharacterProps), mDaTree))
	{
		mDaInstalled.mItemCount++;
	}

#if	FALSE
	lProgName = _T(_DSFILTER_PROGID_NAME);
	if	(ShowClassId (lProgName, NULL, __uuidof(CDirectShowSource), mDaTree))
	{
		mDaInstalled.mItemCount++;
	}
#endif

	if	(
			(IsWindows7_AtLeast ())
		&&	(mDaInstalled.mCharPropsItem)
		)
	{
		ShowProgId (_T(_AGENT_CHAR_PROGID_NAME), NULL, _T(_AGENT_CHAR_PROGID), mDaTree);
		ShowProgId (_T(_AGENT_CHAR_WEB_PROGID_NAME), NULL, _T(_AGENT_CHAR_WEB_PROGID), mDaTree);
		ShowProgId (_T(_AGENT_CHAR_PVW_PROGID_NAME), NULL, _T(_AGENT_CHAR_PVW_PROGID), mDaTree, false);
	}
}

bool CPropPageRegistry::ShowMsRegistry ()
{
	mMaTree.SendMessage (CCM_SETVERSION, max (mMaTree.SendMessage (CCM_GETVERSION), 5));
	TreeView_DeleteAllItems (mMaTree);
	mMaInstalled.Clear ();
#ifdef	_WIN64
	mMaInstalled32.Clear ();
#endif

	mMaInstalled.mServerName = _T(_MS_AGENT_SERVER_TITLE);
#ifdef	_WIN64
	if	(mMaInstalled.mServerItem = ShowClassId (mMaInstalled.mServerName, sNameFormat64, __uuidof(AgentServer), mMaTree, false))
	{
		mMaInstalled.mItemCount++;
	}

	mMaInstalled.mServerNameAlt = _T(_MS_AGENT_SERVER_TITLE);
	if	(mMaInstalled.mServerItemAlt = ShowClassId (mMaInstalled.mServerNameAlt, sNameFormat64, __uuidof(AgentServer64), mMaTree, false))
	{
		mMaInstalled.mItemCount++;
	}

	if	(
			(!mMaInstalled.mServerItem)
		&&	(!mMaInstalled.mServerItemAlt)
		)
	{
		mMaInstalled.mServerName = _T(_MS_AGENT_SERVER_TITLE);
		mMaInstalled.mServerItem = ShowClassId (mMaInstalled.mServerName, sNameFormat64, __uuidof(AgentServer64), mMaTree);
	}

	if	(
			(mMaInstalled.mServerItem)
		&&	(!mMaInstalled.mServerItemAlt)
		)
	{
		mMaInstalled.mServerNameAlt.Empty ();
	}
	else
	if	(
			(!mMaInstalled.mServerItem)
		&&	(mMaInstalled.mServerItemAlt)
		)
	{
		mMaInstalled.mServerName.Empty ();
	}
	else
	if	(
			(mMaInstalled.mServerItem)
		&&	(mMaInstalled.mServerItemAlt)
		)
	{
		mMaInstalled.mServerNameAlt += _T(" (alternate)");
	}

	mMaInstalled32.mServerName = _T(_MS_AGENT_SERVER_TITLE);
	if	(mMaInstalled32.mServerItem = ShowClassId (mMaInstalled32.mServerName, sNameFormat32, __uuidof(AgentServer), mMaTree, true, true))
	{
		mMaInstalled32.mItemCount++;
	}
#else
	if	(mMaInstalled.mServerItem = ShowClassId (mMaInstalled.mServerName, NULL, __uuidof(AgentServer), mMaTree))
	{
		mMaInstalled.mItemCount++;
	}
#endif

	mMaInstalled.mControlName = _T(_MS_AGENT_CONTROL_TITLE);
#ifdef	_WIN64
	if	(mMaInstalled.mControlItem = ShowClassId (mMaInstalled.mControlName, sNameFormat64, __uuidof(AgentControl), mMaTree))
#else
	if	(mMaInstalled.mControlItem = ShowClassId (mMaInstalled.mControlName, NULL, __uuidof(AgentControl), mMaTree))
#endif
	{
		mMaInstalled.mItemCount++;
	}
#ifdef	_WIN64
	mMaInstalled32.mControlName = _T(_MS_AGENT_CONTROL_TITLE);
	if	(mMaInstalled32.mControlItem = ShowClassId (mMaInstalled32.mControlName, sNameFormat32, __uuidof(AgentControl), mMaTree, true, true))
	{
		mMaInstalled32.mItemCount++;
	}
#endif

	mMaInstalled.mCharPropsName = _T(_MS_AGENT_PROPERTIES_TITLE);
#ifdef	_WIN64
	if	(mMaInstalled.mCharPropsItem = ShowClassId (mMaInstalled.mCharPropsName, sNameFormat64, __uuidof(AgentCharacterProps), mMaTree))
#else
	if	(mMaInstalled.mCharPropsItem = ShowClassId (mMaInstalled.mCharPropsName, NULL, __uuidof(AgentCharacterProps), mMaTree))
#endif
	{
		mMaInstalled.mItemCount++;
	}
#ifdef	_WIN64
	else
	{
		mMaInstalled32.mCharPropsName = _T(_MS_AGENT_PROPERTIES_TITLE);
		mMaInstalled32.mCharPropsItem = ShowClassId (mMaInstalled32.mCharPropsName, sNameFormat32, __uuidof(AgentCharacterProps), mMaTree, false, true);
	}
#endif

	if	(ShowProgId (_T(_MS_AGENT_CHARACTER_TITLE), NULL, _T(_AGENT_CHAR_PROGID), mMaTree))
	{
		mMaInstalled.mItemCount++;
	}
	if	(ShowProgId (_T(_MS_AGENT_CHARACTER_WEB_TITLE), NULL, _T(_AGENT_CHAR_WEB_PROGID), mMaTree))
	{
		mMaInstalled.mItemCount++;
	}
	if	(ShowProgId (_T(_MS_AGENT_CHARACTER_PVW_TITLE), NULL, _T(_AGENT_CHAR_PVW_PROGID), mMaTree, false))
	{
		mMaInstalled.mItemCount++;
	}

	if	(
			(!mMaInstalled.mServerItem)
		&&	(!mMaInstalled.mServerItemAlt)
		&&	(!mMaInstalled.mControlItem)
		&&	(!mMaInstalled.mItemCount)
		)
	{
		mMaInstalled.Clear ();
	}
#ifdef	_WIN64
	if	(
			(!mMaInstalled32.mServerItem)
		&&	(!mMaInstalled32.mControlItem)
		&&	(!mMaInstalled32.mItemCount)
		)
	{
		mMaInstalled32.Clear ();
	}
#endif

	if	(
			(!mMaInstalled.mServerItem)
		&&	(!mMaInstalled.mServerItemAlt)
		&&	(!mMaInstalled.mControlItem)
		&&	(!mMaInstalled.mItemCount)
#ifdef	_WIN64
		&&	(!mMaInstalled32.mServerItem)
		&&	(!mMaInstalled32.mControlItem)
		&&	(!mMaInstalled32.mItemCount)
#endif
		)
	{
		TreeView_DeleteAllItems (mMaTree);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HTREEITEM CPropPageRegistry::ShowClassId (CAtlString & pProgName, LPCTSTR pNameFormat, REFGUID pClassId, CWindow & pTree, bool pShowMissing, bool pAltPlatform, HTREEITEM pParentItem)
{
	CRegKeyEx	lGlobalClasses;
	CRegKeyEx	lUserClasses;
	CRegKeyEx	lClassesRoot;

	GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot, pAltPlatform);
	return ShowClassId (pProgName, pNameFormat, pClassId, pTree, lGlobalClasses, lUserClasses, lClassesRoot, pShowMissing, pParentItem);
}

HTREEITEM CPropPageRegistry::ShowClassId (CAtlString & pProgName, LPCTSTR pNameFormat, REFGUID pClassId, CWindow & pTree, HKEY pGlobalClasses, HKEY pUserClasses, HKEY pClassesRoot, bool pShowMissing, HTREEITEM pParentItem)
{
	CRegKeyEx	lClassIdKey (pClassesRoot, _T("CLSID\\")+(CString)CGuidStr(pClassId), true);
	HTREEITEM	lProgItem;
	HTREEITEM	lProgPathItem;
	CAtlString	lProgPath;
	CAtlString	lProgVersion;

	if	(lClassIdKey.IsValid ())
	{
		CRegKeyEx	lClassProgKey (lClassIdKey, _T("ProgID"), true);
		CRegKeyEx	lProgIdKey (pClassesRoot, lClassProgKey.Value().Value(), true);

		if	(
				(lProgIdKey.IsValid())
			&&	(!lProgIdKey.Value().Value().IsEmpty())
			)
		{
			pProgName = lProgIdKey.Value().Value();
		}
		else
		if	(!lClassIdKey.Value().Value().IsEmpty())
		{
			pProgName = lClassIdKey.Value().Value();
		}
	}
	else
	if	(!pShowMissing)
	{
		return NULL;
	}
	if	(pNameFormat)
	{
		pProgName.Format (pNameFormat, CAtlString ((LPCTSTR)pProgName));
	}

	lProgPath = GetServerPath (pClassId, pClassesRoot);

	if	(
			(lProgPath.IsEmpty ())
		||	(
				(IsWindows7_AtLeast ())
			&&	(pTree.m_hWnd == mMaTree.m_hWnd)
			&&	(!IsEqualGUID (pClassId, __uuidof(AgentCharacterProps)))
			&&	(CRegKeyEx (lClassIdKey, _T("VersionIndependentProgID"), true).Value().Value().IsEmpty ())
			)
		)
	{
		if	(pShowMissing)
		{
			lProgItem = InsertTreeItem (pTree, pProgName, pParentItem, true);
			InsertTreeItem (pTree, _T("Not registered"), lProgItem);
		}
	}
	else
	{
		lProgItem = InsertTreeItem (pTree, pProgName, pParentItem, true);
		lProgPathItem = InsertTreeItem (pTree, lProgPath, lProgItem, true);

		if	(PathFileExists (lProgPath))
		{
			lProgVersion = CFileVersion (lProgPath).FileVersionString();
			if	(!lProgVersion.IsEmpty ())
			{
				InsertTreeItem (pTree, _T("Version: ") + lProgVersion, lProgPathItem);
			}
		}
		else
		{
			InsertTreeItem (pTree, _T("File not found"), lProgPathItem);
		}

		if	(CRegKeyEx (lClassIdKey, _T("Programmable"), true).IsValid ())
		{
			CRegKeyEx	lClassTypelibKey (lClassIdKey, _T("TypeLib"), true);
			CRegKeyEx	lClassVersionKey (lClassIdKey, _T("Version"), true);

			if	(
					(lClassTypelibKey.IsValid ())
				&&	(!IsEqualGUID (CGuidStr::Parse (lClassTypelibKey.Value().Value()), GUID_NULL))
				)
			{
				CRegKeyEx			lTypeLibKey (pClassesRoot, _T("TYPELIB\\") + lClassTypelibKey.Value().Value());
				tPtr <CRegKeyEx>	lTypeVersionKey;

				if	(
						(
							(lClassVersionKey.IsValid ())
						&&	(lTypeVersionKey = new CRegKeyEx (lTypeLibKey, lClassVersionKey.Value().Value()))
						&&	(lTypeVersionKey->IsValid())
						)
					||	(
							(lTypeLibKey.IsValid ())
						&&	(lTypeLibKey.KeyCount () == 1)
						&&	(lTypeVersionKey = lTypeLibKey [1L])
						)
					)
				{
					InsertTreeItem (pTree, _T("Programming interface: \"") + lTypeVersionKey->Value().Value() + _T("\""), lProgItem);
				}
			}
			else
			{
//TODO - Look for typelib in program?
				//LogMessage (LogDebug, _T("%s Programmable [%u] [%u]"), lClassIdKey.Name(), lClassTypelibKey.IsValid(), lClassVersionKey.IsValid());
			}
		}

		if	(!GetServerPath (pClassId, pUserClasses).IsEmpty ())
		{
			InsertTreeItem (pTree, _T("Registered for this user"), lProgItem);
		}
		if	(!GetServerPath (pClassId, pGlobalClasses).IsEmpty ())
		{
			InsertTreeItem (pTree, _T("Registered for all users"), lProgItem);
		}
		return lProgItem;
	}
	return NULL;
}

HTREEITEM CPropPageRegistry::ShowProgId (LPCTSTR pProgName, LPCTSTR pNameFormat, LPCTSTR pProgId, CWindow & pTree, bool pShowMissing, bool pAltPlatform, HTREEITEM pParentItem)
{
	CRegKeyEx	lGlobalClasses;
	CRegKeyEx	lUserClasses;
	CRegKeyEx	lClassesRoot;

	GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot, pAltPlatform);
	return ShowProgId (pProgName, pNameFormat, pProgId, pTree, lGlobalClasses, lUserClasses, lClassesRoot, pShowMissing, pParentItem);
}

HTREEITEM CPropPageRegistry::ShowProgId (LPCTSTR pProgName, LPCTSTR pNameFormat, LPCTSTR pProgId, CWindow & pTree, HKEY pGlobalClasses, HKEY pUserClasses, HKEY pClassesRoot, bool pShowMissing, HTREEITEM pParentItem)
{
	CRegKeyEx	lProgIdKey (pClassesRoot, pProgId, true);
	HTREEITEM	lProgItem;
	HTREEITEM	lProgPathItem;
	CAtlString	lProgName (pProgName);
	CAtlString	lProgPath;
	CAtlString	lProgVersion;
	CAtlString	lProgPropExt;

	if	(&pTree == &mMaTree)
	{
		CRegKeyEx	lClassIdKey (lProgIdKey, _T("CLSID"), true);

		if	(
				(lClassIdKey.IsValid ())
			&&	(lClassIdKey.Value().Value().CollateNoCase ((CAtlString)CGuidStr(__uuidof(CDaCharacterProps))) == 0)
			)
		{
			return NULL;
		}
	}
	if	(lProgIdKey.IsValid())
	{
		if	(!lProgIdKey.Value().Value().IsEmpty())
		{
			lProgName = lProgIdKey.Value().Value();
		}
	}
	else
	if	(!pShowMissing)
	{
		return NULL;
	}
	if	(pNameFormat)
	{
		lProgName.Format (pNameFormat, CAtlString ((LPCTSTR)lProgName));
	}

	lProgPropExt = GetShellPropertiesExt (pProgId, pClassesRoot);
	lProgPath = GetServerPath (pProgId, pClassesRoot);
	lProgItem = InsertTreeItem (pTree, lProgName, pParentItem, true);

	if	(
			(lProgPath.IsEmpty ())
		&&	(lProgPropExt.IsEmpty ())
		)
	{
		InsertTreeItem (pTree, _T("Not registered"), lProgItem);
	}
	else
	if	(lProgPath.IsEmpty ())
	{
		if	(!lProgPropExt.IsEmpty ())
		{
			InsertTreeItem (pTree, _T("Properties: \"") + lProgPropExt + _T("\""), lProgItem);
		}
	}
	else
	{
		lProgPathItem = InsertTreeItem (pTree, lProgPath, lProgItem, true);

		if	(PathFileExists (lProgPath))
		{
			lProgVersion = CFileVersion (lProgPath).FileVersionString();
			if	(!lProgVersion.IsEmpty ())
			{
				InsertTreeItem (pTree, _T("Version: ") + lProgVersion, lProgPathItem);
			}
		}
		else
		{
			InsertTreeItem (pTree, _T("File not found"), lProgPathItem);
		}

		if	(!lProgPropExt.IsEmpty ())
		{
			InsertTreeItem (pTree, _T("Properties: \"") + lProgPropExt + _T("\""), lProgItem);
		}

		if	(!GetServerPath (pProgId, pUserClasses).IsEmpty ())
		{
			InsertTreeItem (pTree, _T("Registered for this user"), lProgItem);
		}
		if	(!GetServerPath (pProgId, pGlobalClasses).IsEmpty ())
		{
			InsertTreeItem (pTree, _T("Registered for all users"), lProgItem);
		}
		return lProgItem;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString CPropPageRegistry::GetShellPropertiesExt (LPCTSTR pProgId, HKEY pClassesRoot)
{
	CAtlString			lPropExtName;
	CRegKeyEx			lProgIdKey (pClassesRoot, pProgId, true);
	CRegKeyEx			lShellExtKey (lProgIdKey, _T("shellex\\PropertySheetHandlers"), true);
	tPtr <CRegKeyEx>	lSubKey;
	long				lNdx;
	GUID				lPropExtClass;

	for	(lNdx = 0; lNdx < lShellExtKey.KeyCount(); lNdx++)
	{
		if	(lSubKey = lShellExtKey [lNdx])
		{
			lPropExtClass = CGuidStr::Parse (lSubKey->Value().Value());
			if	(IsEqualGUID (lPropExtClass, GUID_NULL))
			{
				lPropExtClass = CGuidStr::Parse (lSubKey->Name());
			}
			if	(!IsEqualGUID (lPropExtClass, GUID_NULL))
			{
				CRegKeyEx	lClsIdKey (pClassesRoot, _T("CLSID"), true);
				CRegKeyEx	lClassKey (lClsIdKey, (CString)CGuidStr(lPropExtClass), true);

				if	(lClassKey.IsValid())
				{
					lPropExtName = lClassKey.Value().Value();
				}
			}
		}
		if	(!lPropExtName.IsEmpty())
		{
			break;
		}
	}
	return lPropExtName;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CPropPageRegistry::ShowInstallStatus ()
{
	bool	lRet = false;

	mEmulationStatus.mServerStatus = ShowTreatAs (mDaInstalled.mServerItem, mDaInstalled.mServerName, mDaInstalled.mServerTreatAs, mMaInstalled.mServerItem, mMaInstalled.mServerName, mMaInstalled.mServerTreatAs, __uuidof(AgentServer), __uuidof(DaServer));
	mEmulationStatus.mControlStatus = ShowTreatAs (mDaInstalled.mControlItem, mDaInstalled.mControlName, mDaInstalled.mControlTreatAs, mMaInstalled.mControlItem, mMaInstalled.mControlName, mMaInstalled.mControlTreatAs, __uuidof(AgentControl), __uuidof(DaControl));
	mEmulationStatus.mCharPropsStatus = ShowTreatAs (mDaInstalled.mCharPropsItem, mDaInstalled.mCharPropsName, mDaInstalled.mCharPropsTreatAs, mMaInstalled.mCharPropsItem, mMaInstalled.mCharPropsName, mMaInstalled.mCharPropsTreatAs, __uuidof(AgentCharacterProps), __uuidof(CDaCharacterProps));
#ifdef	_WIN64
	if	(mMaInstalled.mServerItemAlt)
	{
		mEmulationStatus.mServerStatusAlt = ShowTreatAs (mDaInstalled.mServerItem, mDaInstalled.mServerName, mDaInstalled.mServerTreatAsAlt, mMaInstalled.mServerItemAlt, mMaInstalled.mServerNameAlt, mMaInstalled.mServerTreatAsAlt, __uuidof(AgentServer64), __uuidof(DaServer));
	}
	mEmulationStatus32.mServerStatus = ShowTreatAs (mDaInstalled32.mServerItem, mDaInstalled32.mServerName, mDaInstalled32.mServerTreatAs, mMaInstalled32.mServerItem, mMaInstalled32.mServerName, mMaInstalled32.mServerTreatAs, __uuidof(AgentServer), __uuidof(DaServer), true);
	mEmulationStatus32.mControlStatus = ShowTreatAs (mDaInstalled32.mControlItem, mDaInstalled32.mControlName, mDaInstalled32.mControlTreatAs, mMaInstalled32.mControlItem, mMaInstalled32.mControlName, mMaInstalled32.mControlTreatAs, __uuidof(AgentControl), __uuidof(DaControl), true);
#endif

	if	(ShowDaInstallStatus ())
	{
		lRet = true;
	}
	if	(ShowMsInstallStatus ())
	{
		lRet = true;
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CPropPageRegistry::ShowDaInstallStatus ()
{
	bool			lRet = false;
	CAtlStringArray	lStatusText;

	if	(
			(mDaInstalled.mServerItem)
#ifdef	_WIN64
		||	(mDaInstalled32.mServerItem)
#endif
		)
	{
#ifdef	_WIN64
		if	(
				(mDaInstalled.mServerItem)
			&&	(!mDaInstalled32.mServerItem)
			)
		{
			lStatusText.Add (_T("Double Agent (64-bit) is installed."));
		}
		else
		if	(
				(!mDaInstalled.mServerItem)
			&&	(mDaInstalled32.mServerItem)
			)
		{
			lStatusText.Add (_T("Double Agent (32-bit) is installed."));
		}
		else
#else
		if	(IsWow64 ())
		{
			lStatusText.Add (_T("Double Agent (32-bit) is installed."));
		}
		else
#endif
		{
			lStatusText.Add (_T("Double Agent is installed."));
		}

		if	(
				(mEmulationStatus.mServerStatus)
			||	(mEmulationStatus.mControlStatus)
			||	(mEmulationStatus.mCharPropsStatus)
#ifdef	_WIN64
			||	(mEmulationStatus.mServerStatusAlt)
			||	(mEmulationStatus32.mServerStatus)
			||	(mEmulationStatus32.mControlStatus)
#endif
			)
		{
			if	(
					(
						(!mDaInstalled.mServerItem)
					||	(!mMaInstalled.mServerItem)
					||	(mEmulationStatus.mServerStatus)
					)
				&&	(
						(!mDaInstalled.mControlItem)
					||	(!mMaInstalled.mControlItem)
					||	(mEmulationStatus.mControlStatus)
					)
				&&	(
						(!mDaInstalled.mCharPropsItem)
					||	(!mMaInstalled.mCharPropsItem)
					||	(mEmulationStatus.mCharPropsStatus)
					)
#ifdef	_WIN64
				&&	(
						(!mDaInstalled.mServerItem)
					||	(!mMaInstalled.mServerItemAlt)
					||	(mEmulationStatus.mServerStatusAlt)
					)
				&&	(
						(!mDaInstalled32.mServerItem)
					||	(!mMaInstalled32.mServerItem)
					||	(mEmulationStatus32.mServerStatus)
					)
				&&	(
						(!mDaInstalled32.mControlItem)
					||	(!mMaInstalled32.mControlItem)
					||	(mEmulationStatus32.mControlStatus)
					)
#endif
				)
			{
				lStatusText.Add (_T("It's currently emulating Microsoft® Agent."));
#ifdef	_DEBUG_EMULATION
				mUseDaButton.EnableWindow (TRUE);
#else
				mUseDaButton.EnableWindow (FALSE);
#endif
				lRet = true;
			}
			else
			{
				lStatusText.Add (_T("It's currently emulating some components of Microsoft® Agent."));
				mUseDaButton.EnableWindow (TRUE);
			}
		}
		else
		{
			mUseDaButton.EnableWindow (TRUE);
		}
	}
	else
	{
		lStatusText.Add (_T("Double Agent is not installed."));

		if	(mDaInstalled.mItemCount)
		{
			lStatusText.Add (_T("However, some of its components are installed."));
		}
		mUseDaButton.EnableWindow (FALSE);
	}

	if	(
			(mEmulationStatus.mServerStatus & TreatAsThisUser)
		||	(mEmulationStatus.mControlStatus & TreatAsThisUser)
#ifdef	_WIN64
		||	(mEmulationStatus.mServerStatusAlt & TreatAsThisUser)
		||	(mEmulationStatus32.mServerStatus & TreatAsThisUser)
		||	(mEmulationStatus32.mControlStatus & TreatAsThisUser)
#endif
		)
	{
		lStatusText.Add (_T("(<A ID=\"EMULATIONDETAILS.HTM\">click for more details</A>)"));
		mDaStatus.ModifyStyle (0, WS_TABSTOP);
	}
	else
	{
		mDaStatus.ModifyStyle (0, WS_TABSTOP);
	}

	mDaStatus.SetWindowText (JoinStringArray (lStatusText, _T("  ")));
	mDaTree.RedrawWindow ();

	return lRet;
}

bool CPropPageRegistry::ShowMsInstallStatus ()
{
	bool			lRet = false;
	CAtlStringArray	lStatusText;

	if	(
			(mMaInstalled.mServerItem)
#ifdef	_WIN64
		||	(mMaInstalled.mServerItemAlt)
		||	(mMaInstalled32.mServerItem)
#endif
		)
	{
#ifdef	_WIN64
		if	(
				(
					(mMaInstalled.mServerItem)
				||	(mMaInstalled.mServerItemAlt)
				)
			&&	(!mMaInstalled32.mServerItem)
			)
		{
			lStatusText.Add (_T("Microsoft® Agent (64-bit) is installed."));
		}
		else
		if	(
				(!mMaInstalled.mServerItem)
			&&	(!mMaInstalled.mServerItemAlt)
			&&	(mMaInstalled32.mServerItem)
			)
		{
			lStatusText.Add (_T("Microsoft® Agent (32-bit) is installed."));
		}
		else
#endif
		{
			lStatusText.Add (_T("Microsoft® Agent is installed."));
		}

		if	(
				(mEmulationStatus.mServerStatus)
			||	(mEmulationStatus.mControlStatus)
			||	(mEmulationStatus.mCharPropsStatus)
#ifdef	_WIN64
			||	(mEmulationStatus.mServerStatusAlt)
			||	(mEmulationStatus32.mServerStatus)
			||	(mEmulationStatus32.mControlStatus)
#endif
			)
		{
			if	(
					(
						(!mMaInstalled.mServerItem)
					||	(mEmulationStatus.mServerStatus)
					)
				&&	(
						(!mMaInstalled.mControlItem)
					||	(mEmulationStatus.mControlStatus)
					)
				&&	(
						(!mMaInstalled.mCharPropsItem)
					||	(mEmulationStatus.mCharPropsStatus)
					)
#ifdef	_WIN64
				&&	(
						(!mMaInstalled.mServerItemAlt)
					||	(mEmulationStatus.mServerStatusAlt)
					)
				&&	(
						(!mMaInstalled32.mServerItem)
					||	(mEmulationStatus32.mServerStatus)
					)
				&&	(
						(!mMaInstalled32.mControlItem)
					||	(mEmulationStatus32.mControlStatus)
					)
#endif
				)
			{
				lStatusText.Add (_T("However, it's currently being emulated by Double Agent."));
				mUseMaButton.EnableWindow (TRUE);
			}
			else
			{
				lStatusText.Add (_T("However, some of its components are currently being emulated by Double Agent."));
				mUseMaButton.EnableWindow (TRUE);
			}
		}
		else
		{
#ifdef	_DEBUG_EMULATION
			mUseMaButton.EnableWindow (TRUE);
#else
			mUseMaButton.EnableWindow (FALSE);
#endif
		}
	}
	else
	{
		lStatusText.Add (_T("Microsoft® Agent is not installed."));

		if	(mMaInstalled.mItemCount)
		{
			lStatusText.Add (_T("However, some of its components are installed."));
		}
		mUseMaButton.EnableWindow (FALSE);
	}

	mMaStatus.SetWindowText (JoinStringArray (lStatusText, _T("  ")));
	mMaTree.RedrawWindow ();

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

UINT CPropPageRegistry::ShowTreatAs (HTREEITEM pDaItem, LPCTSTR pDaItemName, HTREEITEM & pDaTreatAs, HTREEITEM pMaItem, LPCTSTR pMaItemName, HTREEITEM & pMaTreatAs, REFGUID pClsId, REFGUID pTreatAsClsId, bool pAltPlatform)
{
	UINT		lTreatAsStatus = 0;
	CRegKeyEx	lGlobalClasses;
	CRegKeyEx	lUserClasses;
	CRegKeyEx	lClassesRoot;

	GetRootKeys (lGlobalClasses, lUserClasses, lClassesRoot, pAltPlatform);

	if	(GetTreatAs (lClassesRoot, pClsId, pTreatAsClsId))
	{
		lTreatAsStatus |= TreatAsEnabled;
	}
	if	(GetTreatAs (lGlobalClasses, pClsId, pTreatAsClsId))
	{
		lTreatAsStatus |= TreatAsAllUsers;
	}
	if	(GetTreatAs (lUserClasses, pClsId, pTreatAsClsId))
	{
		lTreatAsStatus |= TreatAsThisUser;
	}

	ShowTreatAs (pDaItem, pDaItemName, pDaTreatAs, pMaItem, pMaItemName, pMaTreatAs, lTreatAsStatus);

	return lTreatAsStatus;
}

void CPropPageRegistry::ShowTreatAs (HTREEITEM pDaItem, LPCTSTR pDaItemName, HTREEITEM & pDaTreatAs, HTREEITEM pMaItem, LPCTSTR pMaItemName, HTREEITEM & pMaTreatAs, UINT pTreatAsStatus)
{
	CAtlString	lTreatAsText;

	if	(pDaItem)
	{
		if	(
				(pTreatAsStatus)
			&&	(!pDaTreatAs)
			&&	(pMaItemName)
			&&	(*pMaItemName)
			)
		{
			if	(pTreatAsStatus & TreatAsAllUsers)
			{
				lTreatAsText.Format (_T("Emulating %s for all users"), pMaItemName);
			}
			else
			if	(pTreatAsStatus & TreatAsThisUser)
			{
				lTreatAsText.Format (_T("Emulating %s for this user"), pMaItemName);
			}
			else
			{
				lTreatAsText.Format (_T("Emulating %s"), pMaItemName);
			}
			pDaTreatAs = InsertTreeItem (mDaTree, lTreatAsText, pDaItem);
		}
		else
		if	(
				(!pTreatAsStatus)
			&&	(pDaTreatAs)
			)
		{
			TreeView_DeleteItem (mDaTree, pDaTreatAs);
			pDaTreatAs = NULL;
		}
	}

	if	(pMaItem)
	{
		if	(
				(pTreatAsStatus)
			&&	(!pMaTreatAs)
			&&	(pDaItemName)
			&&	(*pDaItemName)
			)
		{
			if	(pTreatAsStatus & TreatAsAllUsers)
			{
				lTreatAsText.Format (_T("Emulated by %s for all users"), pDaItemName);
			}
			else
			if	(pTreatAsStatus & TreatAsThisUser)
			{
				lTreatAsText.Format (_T("Emulated by %s for this user"), pDaItemName);
			}
			else
			{
				lTreatAsText.Format (_T("Emulated by %s"), pDaItemName);
			}
			pMaTreatAs = InsertTreeItem (mMaTree, lTreatAsText, pMaItem);
		}
		else
		if	(
				(!pTreatAsStatus)
			&&	(pMaTreatAs)
			)
		{
			TreeView_DeleteItem (mMaTree, pMaTreatAs);
			pMaTreatAs = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CPropPageRegistry::PrepareElevated (HWND pOwnerWnd)
{
	if	(
			(IsWindowsVista_AtLeast())
		&&	(mElevatedSettings == NULL)
		)
	{
		try
		{
			IUnknownPtr	lUnknown;

			if	(
					(IsWindowsVista_AtLeast ())
				&&	(!CUserSecurity::IsUserAdministrator())
				)
			{
				if	(SUCCEEDED (LogComErr (LogNormal, CoCreateInstanceElevated (__uuidof(CDaElevatedSettings), &lUnknown, pOwnerWnd))))
				{
					mElevatedSettings = lUnknown;
				}
			}
			else
			{
				if	(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (__uuidof(CDaElevatedSettings), NULL, CLSCTX_SERVER, __uuidof(IUnknown), (void**)&lUnknown))))
				{
					mElevatedSettings = lUnknown;
				}
			}
		}
		catch AnyExceptionSilent

		ShowElevated ();
	}
	if	(IsWindowsVista_AtLeast())
	{
		if	(mElevatedSettings != NULL)
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;
}

void CPropPageRegistry::ShowElevated ()
{
	if	(
			(IsWindow ())
		&&	(IsWindowsVista_AtLeast())
		)
	{
		if	(
				(!CUserSecurity::IsUserAdministrator())
			&&	(mElevatedSettings == NULL)
			)
		{
			mUseDaButton.SendMessage (BCM_SETSHIELD, 0, TRUE);
			mUseMaButton.SendMessage (BCM_SETSHIELD, 0, TRUE);
		}
		else
		{
			mUseDaButton.SendMessage (BCM_SETSHIELD, 0, FALSE);
			mUseMaButton.SendMessage (BCM_SETSHIELD, 0, FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CPropPageRegistry::GetTreatAs (REFGUID pClsId, REFGUID pTreatAsClsId, bool pAltPlatform)
{
	bool	lRet = false;
	GUID	lTreatAsClass = GUID_NULL;

#ifdef	_WIN64
	if	(pAltPlatform)
	{
		CRegKeyEx	lClassesRoot;

		if	(GetClassesRootKey (lClassesRoot, true))
		{
			CRegKeyEx	lClassIds (lClassesRoot, _T("CLSID"), true);
			CRegKeyEx	lClassId (lClassIds, (CString)CGuidStr (pClsId), true);
			CRegKeyEx	lTreatAs (lClassId, _T("TreatAs"), true);

			if	(lTreatAs.IsValid ())
			{
				lTreatAsClass = CGuidStr::Parse (lTreatAs.Value().Value());
				if	(IsEqualGUID (lTreatAsClass, pTreatAsClsId))
				{
					lRet = true;
				}
			}
		}
	}
	else
#endif
	if	(
			(CoGetTreatAsClass (pClsId, &lTreatAsClass) == S_OK)
		&&	(IsEqualGUID (lTreatAsClass, pTreatAsClsId))
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CPropPageRegistry::GetTreatAs (HKEY pClassesRoot, REFGUID pClsId, REFGUID pTreatAsClsId)
{
	bool	lRet = false;

	if	(pClassesRoot)
	{
		CRegKeyEx	lClassIds (pClassesRoot, _T("CLSID"), true);
		CRegKeyEx	lClassId (lClassIds, (CString)CGuidStr (pClsId), true);
		CRegKeyEx	lTreatAs (lClassId, _T("TreatAs"), true);
		GUID		lTreatAsClass = GUID_NULL;

		if	(lTreatAs.IsValid ())
		{
			lTreatAsClass = CGuidStr::Parse (lTreatAs.Value().Value());
			if	(IsEqualGUID (lTreatAsClass, pTreatAsClsId))
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CPropPageRegistry::UpdateTreatAs (REFGUID pClsId, REFGUID pTreatAsClsId)
{
	bool	lRet = false;

	if	(IsWindowsVista_AtLeast ())
	{
		lRet = UpdateTreatAs ((CString)CGuidStr(pClsId), (CString)CGuidStr(pTreatAsClsId));
	}
	else
	if	(SUCCEEDED (LogComErr (LogNormal, CoTreatAsClass (pClsId, pTreatAsClsId))))
	{
		lRet = true;
	}
	return lRet;
}

bool CPropPageRegistry::UpdateTreatAs (LPCTSTR pClsId, LPCTSTR pTreatAsClsId)
{
	bool			lRet = false;
	tS <VARIANT>	lParm;
	long			lParmNdx;
	_bstr_t			lClsId (pClsId);
	_bstr_t			lTreatAsClsId (pTreatAsClsId);
	HRESULT			lResult;

	if	(mElevatedSettings != NULL)
	{
		lParm.vt = VT_SAFEARRAY|VT_ARRAY;
		lParm.parray = SafeArrayCreateVector (VT_BSTR, 0, 2);
		SafeArrayPutElement (lParm.parray, &(lParmNdx=0), (BSTR)lClsId);
		SafeArrayPutElement (lParm.parray, &(lParmNdx=1), (BSTR)lTreatAsClsId);

		try
		{
			lResult = LogComErr (LogNormal, mElevatedSettings->Exec (NULL, OLECMDID_OPEN, OLECMDEXECOPT_DODEFAULT, &lParm, NULL));
			if	(lResult == S_OK)
			{
				lRet = true;
			}
		}
		catch AnyExceptionSilent

		SafeArrayDestroy (lParm.parray);
		lParm.parray = NULL;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HTREEITEM CPropPageRegistry::InsertTreeItem (CWindow & pTree, LPCTSTR pItemText, HTREEITEM pParentItem, bool pExpanded)
{
	HTREEITEM				lRet;
	tS <TV_INSERTSTRUCT>	lTreeInsert;

	lTreeInsert.hParent = pParentItem;
	lTreeInsert.item.mask = TVIF_TEXT;
	lTreeInsert.item.pszText = (LPTSTR)pItemText;
	if	(
			(lRet = TreeView_InsertItem (pTree, &lTreeInsert))
		&&	(pExpanded)
		)
	{
		TreeView_SetItemState (pTree, lRet, TVIS_EXPANDED, TVIS_EXPANDED);
	}
	return lRet;
}

bool CPropPageRegistry::IsHighlightedItem (CWindow & pTree, HTREEITEM pTreeItem, HTREEITEM pTreatAsItem, HTREEITEM pRootItem)
{
	if	(
			(pTreeItem)
		&&	(
				(pTreeItem == pTreatAsItem)
			||	(
					(pTreatAsItem)
				&&	(pTreeItem == pRootItem)
				&&	((TreeView_GetItemState (pTree, pTreeItem, TVIS_EXPANDED) & TVIS_EXPANDED) == 0)
				)
			)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageRegistry::OnCustomDrawTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LRESULT				lResult = CDRF_DODEFAULT;
	LPNMTVCUSTOMDRAW	lCustomDraw = (LPNMTVCUSTOMDRAW) pnmh;

	switch (lCustomDraw->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			lResult |= CDRF_NOTIFYITEMDRAW;
		}	break;
		case CDDS_ITEMPREPAINT:
		{
			if	(
					((HTREEITEM) lCustomDraw->nmcd.dwItemSpec)
//				&&	((lCustomDraw->nmcd.uItemState & CDIS_SELECTED) == 0)
				&&	(
						(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled.mServerTreatAs, mMaInstalled.mServerItem))
					||	(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled.mControlTreatAs, mMaInstalled.mControlItem))
					||	(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled.mCharPropsTreatAs, mMaInstalled.mCharPropsItem))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled.mServerTreatAs, mDaInstalled.mServerItem))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled.mControlTreatAs, mDaInstalled.mControlItem))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled.mCharPropsTreatAs, mDaInstalled.mCharPropsItem))
#ifdef	_WIN64
					||	(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled.mServerTreatAsAlt, mMaInstalled.mServerItemAlt))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled.mServerTreatAsAlt, mDaInstalled.mServerItemAlt))
					||	(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled32.mServerTreatAs, mMaInstalled32.mServerItem))
					||	(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled32.mControlTreatAs, mMaInstalled32.mControlItem))
					||	(IsHighlightedItem (mMaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mMaInstalled32.mCharPropsTreatAs, mMaInstalled32.mCharPropsItem))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled32.mServerTreatAs, mDaInstalled32.mServerItem))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled32.mControlTreatAs, mDaInstalled32.mControlItem))
					||	(IsHighlightedItem (mDaTree, (HTREEITEM) lCustomDraw->nmcd.dwItemSpec, mDaInstalled32.mCharPropsTreatAs, mDaInstalled32.mCharPropsItem))
#endif
					)
				)
			{
				if	((lCustomDraw->nmcd.uItemState & CDIS_SELECTED) == 0)
				{
					lCustomDraw->clrText = GetSysColor (COLOR_HOTLIGHT);
				}
				else
				{
					lCustomDraw->clrTextBk = GetSysColor (COLOR_HOTLIGHT);
				}
//
//	Bold font doesn't work because the tree clips the text
//
//				if	(
//						(IsWindowsVista_AtLeast ())
//					&&	(mBoldFont.GetSafeHandle())
//					)
//				{
//					::SelectObject (lCustomDraw->nmcd.hdc, mBoldFont.GetSafeHandle ());
//					lResult |= CDRF_NEWFONT;
//				}
			}
		}	break;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageRegistry::OnUseDa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	bool	lChanged = false;

	if	(PrepareElevated (m_hWnd))
	{
#ifdef	_WIN64
		if	(UpdateTreatAs (__uuidof(AgentServer64), __uuidof(DaServer)))
		{
			lChanged = true;
		}
#endif
		if	(UpdateTreatAs (__uuidof(AgentServer), __uuidof(DaServer)))
		{
			lChanged = true;
		}
		if	(UpdateTreatAs (__uuidof(AgentControl), __uuidof(DaControl)))
		{
			lChanged = true;
		}
		if	(UpdateTreatAs (__uuidof(AgentCharacterProps), __uuidof(CDaCharacterProps)))
		{
			lChanged = true;
		}
	}
#ifdef	_DEBUG_EMULATION
	CPropPageLogging::LogMsRegistry (_DEBUG_EMULATION);
#endif

	if	(lChanged)
	{
		ShowInstallStatus ();
	}
	else
	{
		MessageBeep (MB_ICONWARNING);
	}
	return 0;
}

LRESULT CPropPageRegistry::OnUseMa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	bool	lChanged = false;

	if	(PrepareElevated (m_hWnd))
	{
#ifdef	_WIN64
		if	(UpdateTreatAs (__uuidof(AgentServer64), GUID_NULL))
		{
			lChanged = true;
		}
#endif
		if	(UpdateTreatAs (__uuidof(AgentServer), GUID_NULL))
		{
			lChanged = true;
		}
		if	(UpdateTreatAs (__uuidof(AgentControl), GUID_NULL))
		{
			lChanged = true;
		}
		if	(UpdateTreatAs (__uuidof(AgentCharacterProps), GUID_NULL))
		{
			lChanged = true;
		}
	}
#ifdef	_DEBUG_EMULATION
	CPropPageLogging::LogMsRegistry (_DEBUG_EMULATION);
#endif

	if	(lChanged)
	{
		ShowInstallStatus ();
	}
	else
	{
		MessageBeep (MB_ICONWARNING);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageRegistry::OnLinkClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	PNMLINK	lNotify = (PNMLINK)pnmh;

	if	(lNotify->item.szID[0])
	{
		try
		{
			CDetailsDlg	lDetailsDlg;
			lDetailsDlg.mDetailsURL = lNotify->item.szID;
			lDetailsDlg.DoModal (m_hWnd);
		}
		catch AnyExceptionSilent
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CPropPageRegistry::CDetailsDlg::OnInitDialog (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	HWND		lTextWnd;
	CRect		lTextRect;
	CAtlString	lDetailsURL;

	CenterWindow ();

	lTextWnd = GetDlgItem (IDC_REGISTRY_DETAIL_TEXT);
	::GetWindowRect (lTextWnd, &lTextRect);
	ScreenToClient (&lTextRect);
	::DestroyWindow (lTextWnd);

	if	(mDetailsText.Create (m_hWnd, lTextRect, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_TABSTOP, WS_EX_STATICEDGE, IDC_REGISTRY_DETAIL_TEXT))
	{
		GetModuleFileName (_AtlBaseModule.GetModuleInstance(), lDetailsURL.GetBuffer(MAX_PATH), MAX_PATH);
		lDetailsURL.ReleaseBuffer ();
		lDetailsURL.Format (_T("res://%s/%s"), CAtlString((LPCTSTR)lDetailsURL), mDetailsURL);
		mDetailsText.CreateControl (lDetailsURL);
	}
	return TRUE;
}

LRESULT CPropPageRegistry::CDetailsDlg::OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	EndDialog (IDCANCEL);
	return 0;
}

LRESULT CPropPageRegistry::CDetailsDlg::OnOk (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	EndDialog (IDOK);
	return 0;
}
