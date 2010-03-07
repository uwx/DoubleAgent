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
#include <AgtErr.h>
#include "DaShell.h"
#include "DaCharacterProps.h"
#include "PropPageCharacter.h"
#include "PropPageRegistry.h"
#include "Registry.h"
#include "GuidStr.h"
#include "Clipboard.h"
#include "StringArrayEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

// {1147E565-A208-11DE-ABF2-002421116FB2}
IMPLEMENT_DYNCREATE(CDaCharacterProps, CCmdTarget)
IMPLEMENT_OLECREATE_EX(CDaCharacterProps, _PROPERTIES_PROGID, 0x1147E565, 0xA208, 0x11DE, 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2)

BOOL CDaCharacterProps::CDaCharacterPropsFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_PROPERTIES_PROGID_NAME), true))
	{
		if	(bRegister)
		{
			RegisterApartmentThreaded ();
			RegisterDefCategory ();
		}
		else
		{
			UnregisterPropSheetHandler (_T(_AGENT_CHAR_PROGID), _T("DaPage"));
			UnregisterPropSheetHandler (_T(_AGENT_CHAR_WEB_PROGID), _T("DaPage"));
			UnregisterPropSheetHandler (_T(_AGENT_CHAR_PVW_PROGID), _T("DaPage"));
		}

		if	(bRegister)
		{
			if	(IsWindows7_AtLeast ())
			{
				CRegKey	lProgId1Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PROGID), false, true);
				CRegKey	lProgId2Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_WEB_PROGID), false, true);
				CRegKey	lProgId3Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PVW_PROGID), false, true);
				CRegKey	lClassId1Key (lProgId1Key, _T("CLSID"), false, true);
				CRegKey	lClassId2Key (lProgId2Key, _T("CLSID"), false, true);
				CRegKey	lClassId3Key (lProgId3Key, _T("CLSID"), false, true);
				CRegKey	lFileExt1Key (HKEY_CLASSES_ROOT, _T(".acs"), false, true);
				CRegKey	lFileExt2Key (HKEY_CLASSES_ROOT, _T(".acf"), false, true);
				CRegKey	lFileExt3Key (HKEY_CLASSES_ROOT, _T(".acg"), false, true);

				CRegString (lFileExt1Key, (LPCTSTR)NULL, true).Update (lProgId1Key.Name());
				CRegString (lFileExt2Key, (LPCTSTR)NULL, true).Update (lProgId2Key.Name());
				CRegString (lFileExt3Key, (LPCTSTR)NULL, true).Update (lProgId3Key.Name());

				if	(lClassId1Key.Value().Value().IsEmpty())
				{
					CRegString (lClassId1Key, (LPCTSTR)NULL, true).Update ((CString)CGuidStr(m_clsid));
				}
				if	(lClassId2Key.Value().Value().IsEmpty())
				{
					CRegString (lClassId2Key, (LPCTSTR)NULL, true).Update ((CString)CGuidStr(m_clsid));
				}
				if	(lClassId3Key.Value().Value().IsEmpty())
				{
					CRegString (lClassId3Key, (LPCTSTR)NULL, true).Update ((CString)CGuidStr(m_clsid));
				}

				RegisterPropSheetHandler (_T(_AGENT_CHAR_PROGID), _T("DaPage"));
				RegisterPropSheetHandler (_T(_AGENT_CHAR_WEB_PROGID), _T("DaPage"));
				RegisterPropSheetHandler (_T(_AGENT_CHAR_PVW_PROGID), _T("DaPage"));
			}
		}
		else
		{
			CRegKey	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentCharacterProps)), false);

			if	(
					(IsWindows7_AtLeast ())
				&&	(CRegistrySearch::GetServerPath (__uuidof(AgentCharacterProps), HKEY_CLASSES_ROOT).IsEmpty())
				)
			{
				lMsAgentClass.Delete ();
			}
			else
			{
				CRegKey	lAutoTreatAs (lMsAgentClass, _T("AutoTreatAs"), false);
				CRegKey	lTreatAs (lMsAgentClass, _T("TreatAs"), false);

				if	(
						(lAutoTreatAs.IsValid())
					&&	(lAutoTreatAs.Value().Value().CompareNoCase ((CString)CGuidStr(m_clsid)) == 0)
					)
				{
					lAutoTreatAs.Delete ();
				}
				if	(
						(lTreatAs.IsValid())
					&&	(lTreatAs.Value().Value().CompareNoCase ((CString)CGuidStr(m_clsid)) == 0)
					)
				{
					lTreatAs.Delete ();
				}
			}

			if	(IsWindows7_AtLeast ())
			{
				CRegKey	lProgId1Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PROGID), false);
				CRegKey	lProgId2Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_WEB_PROGID), false);
				CRegKey	lProgId3Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PVW_PROGID), false);
				CRegKey	lClassId1Key (lProgId1Key, _T("CLSID"), false);
				CRegKey	lClassId2Key (lProgId2Key, _T("CLSID"), false);
				CRegKey	lClassId3Key (lProgId3Key, _T("CLSID"), false);
				CRegKey	lFileExt1Key (HKEY_CLASSES_ROOT, _T(".acs"), false);
				CRegKey	lFileExt2Key (HKEY_CLASSES_ROOT, _T(".acf"), false);
				CRegKey	lFileExt3Key (HKEY_CLASSES_ROOT, _T(".acg"), false);

				if	(lClassId1Key.Value().Value() == (CString)CGuidStr(m_clsid))
				{
					lClassId1Key.Delete ();
					lProgId1Key.Delete ();
					lFileExt1Key.Delete ();
				}
				if	(lClassId2Key.Value().Value() == (CString)CGuidStr(m_clsid))
				{
					lClassId2Key.Delete ();
					lProgId2Key.Delete ();
					lFileExt2Key.Delete ();
				}
				if	(lClassId3Key.Value().Value() == (CString)CGuidStr(m_clsid))
				{
					lClassId3Key.Delete ();
					lProgId3Key.Delete ();
					lFileExt3Key.Delete ();
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaCharacterProps, CCmdTarget)
	INTERFACE_PART(CDaCharacterProps, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaCharacterProps, __uuidof(IObjectWithSite), ObjectWithSite)
	INTERFACE_PART(CDaCharacterProps, __uuidof(IShellExtInit), ShellExtInit)
	INTERFACE_PART(CDaCharacterProps, __uuidof(IShellPropSheetExt), ShellPropSheetExt)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CDaCharacterProps, ObjectWithSite)
IMPLEMENT_IUNKNOWN(CDaCharacterProps, ShellExtInit)
IMPLEMENT_IUNKNOWN(CDaCharacterProps, ShellPropSheetExt)

/////////////////////////////////////////////////////////////////////////////

CDaCharacterProps::CDaCharacterProps ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCharacterProps::CDaCharacterProps (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	try
	{
		SHGetInstanceExplorer (&mExplorerInstance);
	}
	catch AnyExceptionDebug

	EnableAggregation ();
}

CDaCharacterProps::~CDaCharacterProps ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCharacterProps::~CDaCharacterProps (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	SafeFreeSafePtr (mExplorerInstance);
	SafeFreeSafePtr (mSite);
	AfxOleUnlockApp();
}

void CDaCharacterProps::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCharacterProps::OnFinalRelease"), this, m_dwRef);
	}
#endif
	CCmdTarget::OnFinalRelease();
}

LPUNKNOWN CDaCharacterProps::GetInterfaceHook(const void* iid)
{
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] CDaCharacterProps::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterProps::XObjectWithSite::SetSite (IUnknown *pUnkSite)
{
	METHOD_PROLOGUE(CDaCharacterProps, ObjectWithSite)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::XObjectWithSite::SetSite [%p]"), pThis, pThis->m_dwRef, pUnkSite);
#endif
	HRESULT	lResult = S_OK;

	pThis->mSite = pUnkSite;

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::XObjectWithSite::SetSite"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterProps::XObjectWithSite::GetSite (REFIID riid, void **ppvSite)
{
	METHOD_PROLOGUE(CDaCharacterProps, ObjectWithSite)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::XObjectWithSite::GetSite [%s]"), pThis, pThis->m_dwRef, CGuidStr::GuidName(riid));
#endif
	HRESULT	lResult = S_OK;

	if	(!ppvSite)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSite == NULL)
	{
		(*ppvSite) = NULL;
		lResult = E_UNEXPECTED;
	}
	else
	{
		lResult = pThis->mSite->QueryInterface (riid, ppvSite);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::XObjectWithSite::GetSite"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterProps::XShellExtInit::Initialize (LPCITEMIDLIST pFolderId, IDataObject * pDataObject, HKEY pProgIdKey)
{
	METHOD_PROLOGUE(CDaCharacterProps, ShellExtInit)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::XShellExtInit::Initialize [%p] [%p] [%p]"), pThis, pThis->m_dwRef, pFolderId, pDataObject, pProgIdKey);
#endif
	HRESULT			lResult = S_FALSE;
	COleDataObject	lDataObject;
	CStringArray	lFileNames;

	if	(pDataObject)
	{
		lDataObject.Attach (pDataObject, FALSE);

		try
		{
			CClipboard::GetFiles (lFileNames, lDataObject);
		}
		catch AnyException

		lDataObject.Detach ();
	}

	if	(
			(lFileNames.GetSize() == 1)
		&&	(pThis->mAgentFile = (CAgentFile *)CAgentFile::CreateObject())
		)
	{
		if	(SUCCEEDED (lResult = pThis->mAgentFile->Open (lFileNames [0])))
		{
#ifdef	_DEBUG_INTERFACE
			LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)]   Opened [%ls]"), pThis, pThis->m_dwRef, (BSTR)pThis->mAgentFile->GetPath());
#endif
			lResult = S_OK;
		}
		else
		{
			if	(
					(lResult == AGENTPROVERROR_MAGIC)
				||	(lResult == AGENTPROVERROR_CHARACTERVERSION)
				||	(lResult == AGENTPROVERROR_CHARACTERINVALID)
				)
			{
				lResult = S_OK;
			}
			SafeFreeSafePtr (pThis->mAgentFile);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::XShellExtInit::Initialize"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterProps::XShellPropSheetExt::AddPages (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam)
{
	METHOD_PROLOGUE(CDaCharacterProps, ShellPropSheetExt)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::XShellPropSheetExt::AddPages [%p]"), pThis, pThis->m_dwRef, pAddPageFunc);
#endif
	HRESULT						lResult = S_OK;
	tPtr <CPropPageCharacter>	lPropPage;

	if	(
			(lPropPage = (CPropPageCharacter *)CPropPageCharacter::CreateObject ())
		&&	(SUCCEEDED (lResult = lPropPage->AddSheetPage (pAddPageFunc, pLparam)))
		)
	{
		lPropPage->mAgentFile = pThis->mAgentFile.Detach ();
		lPropPage.Detach ();
	}
	else
	{
		lResult = S_FALSE;
	}
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::XShellPropSheetExt::AddPages"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterProps::XShellPropSheetExt::ReplacePage (EXPPS pPageID, LPFNSVADDPROPSHEETPAGE pReplaceProc, LPARAM pLparam)
{
	METHOD_PROLOGUE(CDaCharacterProps, ShellPropSheetExt)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::XShellPropSheetExt::ReplacePage"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::XShellPropSheetExt::ReplacePage"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
