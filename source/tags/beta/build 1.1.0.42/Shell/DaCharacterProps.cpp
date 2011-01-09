/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include <shellapi.h>
#include <AgtErr.h>
#include "DaShell.h"
#include "DaCharacterProps.h"
#include "PropPageCharacter.h"
#include "PropPageRegistry.h"
#include "Registry.h"
#include "GuidStr.h"
#include "StringArrayEx.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

CDaCharacterProps::CDaCharacterProps ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCharacterProps::CDaCharacterProps (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	try
	{
		SHGetInstanceExplorer (&mExplorerInstance);
	}
	catch AnyExceptionDebug
}

CDaCharacterProps::~CDaCharacterProps ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCharacterProps::~CDaCharacterProps (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

void CDaCharacterProps::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCharacterProps::OnFinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	SafeFreeSafePtr (mExplorerInstance);
	SafeFreeSafePtr (mSite);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI CDaCharacterProps::UpdateRegistryOverride (BOOL bRegister)
{
	HRESULT		lResult = UpdateRegistry (bRegister);

	if	(SUCCEEDED (lResult))
	{
		if	(bRegister)
		{
			if	(IsWindows7_AtLeast ())
			{
				CRegKeyEx	lProgId1Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PROGID), false, true);
				CRegKeyEx	lProgId2Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_WEB_PROGID), false, true);
				CRegKeyEx	lProgId3Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PVW_PROGID), false, true);
				CRegKeyEx	lClassId1Key (lProgId1Key, _T("CLSID"), false, true);
				CRegKeyEx	lClassId2Key (lProgId2Key, _T("CLSID"), false, true);
				CRegKeyEx	lClassId3Key (lProgId3Key, _T("CLSID"), false, true);
				CRegKeyEx	lFileExt1Key (HKEY_CLASSES_ROOT, _T(".acs"), false, true);
				CRegKeyEx	lFileExt2Key (HKEY_CLASSES_ROOT, _T(".acf"), false, true);
				CRegKeyEx	lFileExt3Key (HKEY_CLASSES_ROOT, _T(".acg"), false, true);

				CRegString (lFileExt1Key, (LPCTSTR)NULL, true).Update (lProgId1Key.Name());
				CRegString (lFileExt2Key, (LPCTSTR)NULL, true).Update (lProgId2Key.Name());
				CRegString (lFileExt3Key, (LPCTSTR)NULL, true).Update (lProgId3Key.Name());

				if	(lClassId1Key.Value().Value().IsEmpty())
				{
					CRegString (lClassId1Key, (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(CDaCharacterProps)));
				}
				if	(lClassId2Key.Value().Value().IsEmpty())
				{
					CRegString (lClassId2Key, (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(CDaCharacterProps)));
				}
				if	(lClassId3Key.Value().Value().IsEmpty())
				{
					CRegString (lClassId3Key, (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(CDaCharacterProps)));
				}
			}
		}
		else
		{
			CRegKeyEx	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentCharacterProps)), false);

			if	(
					(IsWindows7_AtLeast ())
				&&	(CRegistrySearch::GetServerPath (__uuidof(AgentCharacterProps), HKEY_CLASSES_ROOT).IsEmpty())
				)
			{
				lMsAgentClass.Delete ();
			}
			else
			{
				CRegKeyEx	lAutoTreatAs (lMsAgentClass, _T("AutoTreatAs"), false);
				CRegKeyEx	lTreatAs (lMsAgentClass, _T("TreatAs"), false);

				if	(
						(lAutoTreatAs.IsValid())
					&&	(lAutoTreatAs.Value().Value().CompareNoCase ((CString)CGuidStr(__uuidof(CDaCharacterProps))) == 0)
					)
				{
					lAutoTreatAs.Delete ();
				}
				if	(
						(lTreatAs.IsValid())
					&&	(lTreatAs.Value().Value().CompareNoCase ((CString)CGuidStr(__uuidof(CDaCharacterProps))) == 0)
					)
				{
					lTreatAs.Delete ();
				}
			}

			if	(IsWindows7_AtLeast ())
			{
				CRegKeyEx	lProgId1Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PROGID), false);
				CRegKeyEx	lProgId2Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_WEB_PROGID), false);
				CRegKeyEx	lProgId3Key (HKEY_CLASSES_ROOT, _T(_AGENT_CHAR_PVW_PROGID), false);
				CRegKeyEx	lClassId1Key (lProgId1Key, _T("CLSID"), false);
				CRegKeyEx	lClassId2Key (lProgId2Key, _T("CLSID"), false);
				CRegKeyEx	lClassId3Key (lProgId3Key, _T("CLSID"), false);
				CRegKeyEx	lFileExt1Key (HKEY_CLASSES_ROOT, _T(".acs"), false);
				CRegKeyEx	lFileExt2Key (HKEY_CLASSES_ROOT, _T(".acf"), false);
				CRegKeyEx	lFileExt3Key (HKEY_CLASSES_ROOT, _T(".acg"), false);

				if	(lClassId1Key.Value().Value() == (CString)CGuidStr(__uuidof(CDaCharacterProps)))
				{
					lClassId1Key.Delete ();
					lProgId1Key.Delete ();
					lFileExt1Key.Delete ();
				}
				if	(lClassId2Key.Value().Value() == (CString)CGuidStr(__uuidof(CDaCharacterProps)))
				{
					lClassId2Key.Delete ();
					lProgId2Key.Delete ();
					lFileExt2Key.Delete ();
				}
				if	(lClassId3Key.Value().Value() == (CString)CGuidStr(__uuidof(CDaCharacterProps)))
				{
					lClassId3Key.Delete ();
					lProgId3Key.Delete ();
					lFileExt3Key.Delete ();
				}
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterProps::SetSite (IUnknown *pUnkSite)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::SetSite [%p]"), this, max(m_dwRef,-1), pUnkSite);
#endif
	mSite = pUnkSite;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaCharacterProps::GetSite (REFIID riid, void **ppvSite)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::GetSite [%s]"), this, max(m_dwRef,-1), CGuidStr::GuidName(riid));
#endif
	HRESULT	lResult = S_OK;

	if	(!ppvSite)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSite == NULL)
	{
		(*ppvSite) = NULL;
		lResult = E_UNEXPECTED;
	}
	else
	{
		lResult = mSite->QueryInterface (riid, ppvSite);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::GetSite"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterProps::Initialize (LPCITEMIDLIST pFolderId, IDataObject * pDataObject, HKEY pProgIdKey)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::Initialize [%p] [%p] [%p]"), this, max(m_dwRef,-1), pFolderId, pDataObject, pProgIdKey);
#endif
	HRESULT			lResult = S_FALSE;
	CAtlStringArray	lFileNames;

	if	(pDataObject)
	{
		try
		{
			tS <FORMATETC>	lFormat;
			tS <STGMEDIUM>	lStgMedium;
			HDROP			lDropData;

			lFormat.cfFormat = CF_HDROP;
			lFormat.tymed = TYMED_HGLOBAL;

			if	(
					(SUCCEEDED (pDataObject->GetData (&lFormat, &lStgMedium)))
				&&	(lStgMedium.hGlobal)
				&&	(lDropData = (HDROP)GlobalLock (lStgMedium.hGlobal))
				)
			{
				try
				{
					UINT		lFileCount = DragQueryFile (lDropData, -1, NULL, 0);
					UINT		lFileNdx;
					CAtlString	lFileName;

					for	(lFileNdx = 0; lFileNdx < lFileCount; lFileNdx++)
					{
						if	(DragQueryFile (lDropData, lFileNdx, lFileName.GetBuffer (MAX_PATH), MAX_PATH))
						{
							lFileName.ReleaseBuffer ();
							lFileNames.Add (lFileName);
						}
						else
						{
							lFileName.ReleaseBuffer ();
						}
					}
					GlobalUnlock (lStgMedium.hGlobal);
				}
				catch AnyExceptionDebug
			}

			ReleaseStgMedium (&lStgMedium);
		}
		catch AnyExceptionDebug
	}

#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::Initialize [%s]"), this, max(m_dwRef,-1), JoinStringArray(lFileNames, _T("][")));
#endif

	if	(
			(lFileNames.GetCount() == 1)
		&&	(mAgentFile = CAgentFile::CreateInstance())
		)
	{
		if	(SUCCEEDED (lResult = mAgentFile->Open (lFileNames [0])))
		{
#ifdef	_DEBUG_INTERFACE
			LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)]   Opened [%ls]"), this, max(m_dwRef,-1), (BSTR)mAgentFile->GetPath());
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
			SafeFreeSafePtr (mAgentFile);
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::Initialize"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterProps::AddPages (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::AddPages [%p]"), this, max(m_dwRef,-1), pAddPageFunc);
#endif
	HRESULT						lResult = S_OK;
	tPtr <CPropPageCharacter>	lPropPage;

	if	(
			(lPropPage = CPropPageCharacter::CreateInstance ())
		&&	(SUCCEEDED (lResult = lPropPage->AddSheetPage (pAddPageFunc, pLparam)))
		)
	{
		lPropPage->mAgentFile = mAgentFile.Detach ();
		lPropPage.Detach ();
	}
	else
	{
		lResult = S_FALSE;
	}
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::AddPages"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterProps::ReplacePage (EXPPS pPageID, LPFNSVADDPROPSHEETPAGE pReplaceProc, LPARAM pLparam)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCharacterProps::ReplacePage"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = E_NOTIMPL;

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCharacterProps::ReplacePage"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
