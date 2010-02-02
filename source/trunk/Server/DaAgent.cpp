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
#include <shlwapi.h>
#include <wininet.h>
#include <psapi.h>
#include <winsafer.h>
#include "DaServer.h"
#include "DaAgent.h"
#include "DaAgentCharacter.h"
#include "DaAgentPropertySheet.h"
#include "DaAgentAudioOutputProperties.h"
#include "DaAgentSpeechInputProperties.h"
#include "DaAgentCommandWindow.h"
#include "PropSheetCharSel.h"
#include "AgentFiles.h"
#include "FileDownload.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "UiState.h"
#include "OleVariantEx.h"
#include "GuidStr.h"
#include "ThreadSecurity.h"
#include "UserSecurity.h"
#include "MallocPtr.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma warning (disable: 4355)

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Server"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_CHARACTER			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_FILE_LOAD			(GetProfileDebugInt(_T("LogFileLoad"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Server"),LogNormal,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_FILE_LOAD
#define	_LOG_FILE_LOAD			LogDetails
#endif

#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDaAgent, CCmdTarget)
IMPLEMENT_OLECREATE_EX(CDaAgent, _SERVER_PROGID, 0x1147E500, 0xA208, 0x11DE, 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2)
IMPLEMENT_OLETYPELIB(CDaAgent, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaAgent::CDaAgentFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_SERVER_PROGID_NAME)))
	{
		if	(bRegister)
		{
			RegisterServer (false);
			RegisterAppIdSelf (_T(_SERVER_PROGID_NAME), _T("Interactive User"));
			RegisterExeAppIdSelf ();
			RegisterTypeLib (gDaTypeLibId);
			RegisterApartmentThreaded (false);
			RegisterDefCategory ();
			RegisterCategory (m_clsid, CATID_Programmable);
		}
		else
		{
			UnregisterServer (false);
			UnregisterAppIdSelf ();
			UnregisterExeAppId ();
		}

		if	(bRegister)
		{
			if	(
					(IsWindows7_AtLeast())
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer), HKEY_CLASSES_ROOT).IsEmpty())
#ifdef	_WIN64
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer64), HKEY_CLASSES_ROOT).IsEmpty())
#endif
				)
			{
				CRegKey		lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentServer)), false, true);
				CRegKey		lServerProgId (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID), false, true);
				CRegKey		lServerProgId2 (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID2), false, true);

				CRegString (lMsAgentClass, (LPCTSTR)NULL, true).Update (_T("Microsoft Agent Server 2.0"));
				CRegString (CRegKey (lMsAgentClass, _T("ProgID"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID2));
				RegisterDefCategory (__uuidof(AgentServer));
				RegisterCategory (__uuidof(AgentServer), CATID_Programmable);

				CRegString (lServerProgId, (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID_NAME));
				CRegString (CRegKey (lServerProgId, _T("CurVer"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID2));
				CRegString (lServerProgId2, (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID_NAME));
				CRegString (CRegKey (lServerProgId2, _T("CLSID"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(AgentServer)));

				CRegString (CRegKey (lMsAgentClass, _T("AutoTreatAs"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(m_clsid));
				CRegString (CRegKey (lMsAgentClass, _T("TreatAs"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(m_clsid));
			}
		}
		else
		{
			CRegKey	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentServer)), false);
#ifdef	_WIN64
			CRegKey	lMsAgentClass64 (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentServer64)), false);
#endif

			if	(
					(IsWindows7_AtLeast())
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer), HKEY_CLASSES_ROOT).IsEmpty())
#ifdef	_WIN64
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentServer64), HKEY_CLASSES_ROOT).IsEmpty())
#endif
				)
			{
				lMsAgentClass.Delete ();

				CRegKey (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID), false).Delete ();
				CRegKey (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID2), false).Delete ();
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
#ifdef	_WIN64
				if	(lMsAgentClass64.IsValid ())
				{
					CRegKey	lAutoTreatAs64 (lMsAgentClass, _T("AutoTreatAs"), false);
					CRegKey	lTreatAs64 (lMsAgentClass, _T("TreatAs"), false);

					if	(
							(lAutoTreatAs64.IsValid())
						&&	(lAutoTreatAs64.Value().Value().CompareNoCase ((CString)CGuidStr(m_clsid)) == 0)
						)
					{
						lAutoTreatAs64.Delete ();
					}
					if	(
							(lTreatAs64.IsValid())
						&&	(lTreatAs64.Value().Value().CompareNoCase ((CString)CGuidStr(m_clsid)) == 0)
						)
					{
						lTreatAs64.Delete ();
					}
				}
#endif
			}
		}
//
//	Set the launch permissions for the server object allow a low-integrity process to access the server at medium integrity.
//
		if	(
				(bRegister)
			&&	(IsWindowsVista_AtLeast ())
			)
		{
			CRegKey				lAppIdKey (CRegKey (HKEY_CLASSES_ROOT, _T("AppID"), true), CGuidStr(m_clsid), false);
			CRegDWord			lSRPTrustLevel (lAppIdKey, _T("SRPTrustLevel"), true);
			CRegBinary			lLaunchPermission (lAppIdKey, _T("LaunchPermission"), true);
			CRegBinary			lAccessPermission (lAppIdKey, _T("AccessPermission"), true);
			CSecurityDescriptor	lLaunchDescriptor (_T("O:BAG:BAD:(A;;CCDCSW;;;WD)"));
			CSecurityDescriptor	lAccessDescriptor (_T("O:BAG:BAD:(A;;CCDCSW;;;WD)S:(ML;;NX;;;LW)"));
			DWORD				lDescriptorSize;

			lSRPTrustLevel.SetValue (SAFER_LEVELID_FULLYTRUSTED).Update ();

			lLaunchDescriptor.MakeAbsolute ();
			lDescriptorSize = lLaunchDescriptor.MakeSelfRelative ();
			if	(lDescriptorSize > 0)
			{
				lLaunchPermission.Value().SetSize (lDescriptorSize);
				memcpy (lLaunchPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lLaunchDescriptor.mDescriptor, lDescriptorSize);
				lLaunchPermission.Update ();

				if	(LogIsActive (LogDetails))
				{
					lLaunchDescriptor.DumpAccess (LogDetails, true, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
				}
				else
				if	(LogIsActive ())
				{
					LogMessage (LogIfActive, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
				}
#ifdef	_DEBUG
				if	(LogIsActive (LogNormal))
				{
					CString	lAccessStr;
					CString	lByteStr;
					INT_PTR	lNdx;

					for	(lNdx = 0; lNdx <= lLaunchPermission.Value().GetUpperBound(); lNdx++)
					{
						lByteStr.Format (_T("%2.2X"), lLaunchPermission.Value().GetAt(lNdx));
						lAccessStr += lByteStr;
					}
					LogMessage (LogNormal, _T("  LaunchPermission [%s]"), lAccessStr);
				}
#endif
			}
			else
			{
				lLaunchPermission.Delete ();
			}

			lAccessDescriptor.MakeAbsolute ();
			lDescriptorSize = lAccessDescriptor.MakeSelfRelative ();
			if	(lDescriptorSize > 0)
			{
				lAccessPermission.Value().SetSize (lDescriptorSize);
				memcpy (lAccessPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lAccessDescriptor.mDescriptor, lDescriptorSize);
				lAccessPermission.Update ();

				if	(LogIsActive (LogDetails))
				{
					lAccessDescriptor.DumpAccess (LogDetails, true, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
				}
				else
				if	(LogIsActive ())
				{
					LogMessage (LogIfActive, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
				}
#ifdef	_DEBUG
				if	(LogIsActive (LogNormal))
				{
					CString	lAccessStr;
					CString	lByteStr;
					INT_PTR	lNdx;

					for	(lNdx = 0; lNdx <= lAccessPermission.Value().GetUpperBound(); lNdx++)
					{
						lByteStr.Format (_T("%2.2X"), lAccessPermission.Value().GetAt(lNdx));
						lAccessStr += lByteStr;
					}
					LogMessage (LogNormal, _T("  AccessPermission [%s]"), lAccessStr);
				}
#endif
			}
			else
			{
				lAccessPermission.Delete ();
			}
		}

// Set the elevation policy for protected-mode Internet Explorer

		if	(bRegister)
		{
			CRegKey	lPolicyKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights\\ElevationPolicy"), true);
			CRegKey	lAppKey (lPolicyKey, CGuidStr(m_clsid), false, true);
			CString	lModuleName;
			CString	lModulePath;

			GetModuleFileName (AfxGetInstanceHandle(), lModulePath.GetBuffer(MAX_PATH), MAX_PATH);
			lModuleName = PathFindFileName (lModulePath);
			PathRemoveFileSpec (lModulePath.GetBuffer (lModulePath.GetLength()));
			lModulePath.ReleaseBuffer ();

			CRegString (lAppKey, _T("AppName"), true).Update (lModuleName);
			CRegString (lAppKey, _T("AppPath"), true).Update (lModulePath);
			CRegDWord (lAppKey, _T("Policy"), true).SetValue (3).Update ();
		}
		else
		{
			CRegKey	lPolicyKey (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights\\ElevationPolicy"), true);
			CRegKey	lAppKey (lPolicyKey, CGuidStr(m_clsid), false);

			lPolicyKey.Delete ();
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAgent::CDaAgent()
:	mUsingHandler (0),
	mNotify (*this),
	mInNotify (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::CDaAgent (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	try
	{
		TheServerApp->mNotify.Add (&mNotify);
	}
	catch AnyExceptionSilent

	EnableAutomation();
	EnableTypeLib();

	mNotify._RegisterInternalNotify (this, true);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::CDaAgent (%d) Done"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

CDaAgent::~CDaAgent()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::~CDaAgent (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::~CDaAgent (%d) Done [%d]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, AfxOleCanExitApp());
	}
#endif
}

void CDaAgent::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					ExternalDisconnect ();
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		try
		{
			if	(pFinal)
			{
				UnmanageObjectLifetime (this);
			}
			if	(pAbandonned)
			{
				mNotify.AbandonAll ();
			}
			else
			{
				mNotify.UnregisterAll ();
			}
			mNotify._RegisterInternalNotify (this, false);
			UnloadAllCharacters (pAbandonned);
		}
		catch AnyExceptionDebug

		try
		{
			TheServerApp->mNotify.Remove (&mNotify);
		}
		catch AnyExceptionSilent
	}
}

void CDaAgent::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::OnFinalRelease [%u]"), this, m_dwRef, IsInNotify());
	}
#endif
	if	(IsInNotify() == 0)
	{
		Terminate (false);
		CCmdTarget::OnFinalRelease();
	}
}

void CDaAgent::OnClientEnded ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease ();
}

/////////////////////////////////////////////////////////////////////////////

LPUNKNOWN CDaAgent::GetInterfaceHook(const void* iid)
{
	LPUNKNOWN	lRet = NULL;
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CDaAgent::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	if	(mUsingHandler == 1)
	{
		UnmanageObjectLifetime (this);
		if	(ManageObjectLifetime (this))
		{
			mUsingHandler = 2;
		}
	}

	if	(
			(IsEqualIID (*(const IID *)iid, __uuidof (IDaSvrPropertySheet)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentPropertySheet)))
		)
	{
		CDaAgentPropertySheet *	lPropSheet;
		IDaSvrPropertySheetPtr	lInterface;

		if	(
				(lPropSheet = TheServerApp->GetAgentPropertySheet (true, mClientMutexName))
			&&	(lInterface = lPropSheet->GetIDispatch (FALSE))
			)
		{
			lRet = lInterface.Detach ();
			InterlockedDecrement (&m_dwRef);
		}
	}
	else
	if	(
			(IsEqualIID (*(const IID *)iid, __uuidof (IDaSvrAudioOutputProperties)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentAudioOutputProperties)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentAudioOutputPropertiesEx)))
		)
	{
		CDaAgentAudioOutputProperties *		lProperties;
		IDaSvrAudioOutputPropertiesPtr		lInterface;

		if	(
				(lProperties = TheServerApp->GetAgentAudioOutputProperties (true, mClientMutexName))
			&&	(lInterface = lProperties->GetIDispatch (FALSE))
			)
		{
			lRet = lInterface.Detach ();
			InterlockedDecrement (&m_dwRef);
		}
	}
	else
	if	(
			(IsEqualIID (*(const IID *)iid, __uuidof (IDaSvrSpeechInputProperties)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentSpeechInputProperties)))
		)
	{
		CDaAgentSpeechInputProperties *	lProperties;
		IDaSvrSpeechInputPropertiesPtr	lInterface;

		if	(
				(lProperties = TheServerApp->GetAgentSpeechInputProperties (true, mClientMutexName))
			&&	(lInterface = lProperties->GetIDispatch (FALSE))
			)
		{
			lRet = lInterface.Detach ();
			InterlockedDecrement (&m_dwRef);
		}
	}
	else
	if	(
			(IsEqualIID (*(const IID *)iid, __uuidof (IDaSvrCommandWindow)))
		||	(IsEqualIID (*(const IID *)iid, __uuidof (IAgentCommandWindow)))
		)
	{
		CDaAgentCommandWindow *	lCommandWindow;
		IDaSvrCommandWindowPtr	lInterface;

		if	(
				(lCommandWindow = TheServerApp->GetAgentCommandWindow (true, mClientMutexName))
			&&	(lInterface = lCommandWindow->GetIDispatch (FALSE))
			)
		{
			lRet = lInterface.Detach ();
			InterlockedDecrement (&m_dwRef);
		}
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgent::PreNotify ()
{
	if	(
			(this)
		&&	(m_dwRef > 0)
		)
	{
		mInNotify++;
		return true;
	}
	return false;
}

bool CDaAgent::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		if	(
				(mInNotify == 0)
			&&	(mInNotifyUnregister.GetSize () > 0)
			)
		{
			while (mInNotifyUnregister.GetSize () > 0)
			{
				long	lSinkId = mInNotifyUnregister [0];

				mInNotifyUnregister.RemoveAt (0);
				try
				{
					mNotify.Unregister (lSinkId);
				}
				catch AnyExceptionSilent
			}
		}
		if	(
				(mInNotify == 0)
			&&	(m_dwRef == 0)
			)
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive (_LOG_INSTANCE))
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent PostNotify -> OnFinalRelease"), this, m_dwRef);
			}
#endif
			OnFinalRelease ();
			return false;
		}
		return true;
	}
	return false;
}

UINT CDaAgent::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CDaAgent::UnloadAllCharacters (bool pAbandonned)
{
	INT_PTR			lFileNdx;
	CAgentFile *	lFile;

#ifdef	_LOG_INSTANCE
	if	(
			(mNotify.GetCachedFile (0))
		&&	(LogIsActive())
		)
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgent::UnloadAllCharacters [%d] Abandonned [%u]"), this, m_dwRef, mNotify.CachedFileCount(), pAbandonned);
	}
#endif
	for	(lFileNdx = 0; lFile = mNotify.GetCachedFile (lFileNdx); lFileNdx++)
	{
		CObTypeArray <CObject>	lFileClients;
		INT_PTR					lClientNdx;
		CDaAgentCharacter *		lCharacter;

		if	(mNotify.GetFileClients (lFile, lFileClients))
		{
			for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
			{
				if	(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
				{
					try
					{
						if	(pAbandonned)
						{
							lCharacter->Terminate (true, pAbandonned);
							lCharacter->OnFinalRelease ();
						}
						else
						{
							lCharacter->Terminate (false);
#ifdef	_STRICT_COMPATIBILITY
							lCharacter->Terminate (true);
							lCharacter->OnFinalRelease ();
#else
							lCharacter->ExternalRelease ();
#endif
						}
					}
					catch AnyExceptionDebug

					lFileNdx--;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgent::LoadCharacter (LPCTSTR pFilePath, long & pCharID, long & pReqID)
{
	HRESULT	lResult = S_OK;
	CString	lFilePath (pFilePath);

#ifdef	_LOG_CHARACTER
	if	(LogIsActive (_LOG_CHARACTER))
	{
		LogMessage (_LOG_CHARACTER, _T("LoadCharacter [%s]"), pFilePath);
	}
#endif

	if	(lFilePath.IsEmpty ())
	{
		lFilePath = CAgentFiles::GetDefCharPath ();
#ifdef	_LOG_FILE_LOAD
		if	(LogIsActive (_LOG_FILE_LOAD))
		{
			LogMessage (_LOG_FILE_LOAD, _T("File [%s] default Path [%s]"), pFilePath, lFilePath);
		}
#endif
	}
	else
	if	(CAgentFile::IsRelativeFilePath (pFilePath))
	{
		UINT	lPathNum;

		lFilePath = CAgentFile::ParseFilePath (pFilePath);

		if	(
				(!lFilePath.IsEmpty ())
			&&	(!PathFileExists (lFilePath))
			)
		{
			for	(lPathNum = 0; true; lPathNum++)
			{
				lFilePath = CAgentFiles::GetSystemCharsPath (lPathNum);
				if	(lFilePath.IsEmpty ())
				{
					break;
				}
				PathAppend (lFilePath.GetBuffer (MAX_PATH), pFilePath);
				lFilePath.ReleaseBuffer ();
#ifdef	_LOG_FILE_LOAD
				if	(LogIsActive (_LOG_FILE_LOAD))
				{
					LogMessage (_LOG_FILE_LOAD, _T("File [%s] try Path [%s]"), pFilePath, lFilePath);
				}
#endif
				if	(PathFileExists (lFilePath))
				{
					break;
				}
			}
			if	(lFilePath.IsEmpty ())
			{
				lFilePath = pFilePath;
			}
		}
#ifdef	_LOG_FILE_LOAD
		else
		if	(LogIsActive (_LOG_FILE_LOAD))
		{
			LogMessage (_LOG_FILE_LOAD, _T("File [%s] try Path [%s]"), pFilePath, lFilePath);
		}
#endif
	}

	if	(lFilePath.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		lFilePath = CAgentFile::ParseFilePath (lFilePath);

		if	(!CAgentFile::IsProperFilePath (lFilePath))
		{
			lResult = AGENTPROVERROR_PROTOCOL;
		}
		else
		if	(
				(PathIsURL (lFilePath))
			&&	(pReqID)
			)
		{
			tPtr <CFileDownload>	lDownload;

#ifdef	_DEBUG_REQUESTS
			LogMessage (_DEBUG_REQUESTS, _T("RequestStart    [%d]"), pReqID);
#endif
			mNotify.RequestStart (pReqID);

			if	(lDownload = new CFileDownload (lFilePath))
			{
				lDownload->mUserData = pCharID = TheServerApp->mNextCharID++;
				mCharactersLoading.SetAt (pReqID, lDownload);
#ifdef	_LOG_CHARACTER
				if	(LogIsActive (_LOG_CHARACTER))
				{
					LogMessage (_LOG_CHARACTER, _T("Character [%d] Loading [%d]"), pCharID, pReqID);
				}
#endif
				lResult = lDownload.Detach()->Download (m_pOuterUnknown, &mNotify);

				if	(SUCCEEDED (lResult))
				{
					lResult = S_OK;
				}
				else
				{
					mCharactersLoading.RemoveKey (pReqID);
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
			if	(FAILED (lResult))
			{
#ifdef	_DEBUG_REQUESTS
				LogMessage (_DEBUG_REQUESTS, _T("RequestComplete [%d] [%8.8X]"), pReqID, lResult);
#endif
				pCharID = 0;
				mNotify.RequestComplete (pReqID, lResult);
			}
		}
		else
		{
			tPtr <CAgentFile>	lLoadFile;
			CAgentFile *		lAgentFile = NULL;
			CDaAgentCharacter *	lAgentCharacter = NULL;

			if	(pReqID <= 0)
			{
				pReqID = mNotify.NextReqID ();
			}
#ifdef	_DEBUG_REQUESTS
			LogMessage (_DEBUG_REQUESTS, _T("RequestStart    [%d]"), pReqID);
#endif
			mNotify.RequestStart (pReqID);

			if	(lLoadFile = (CAgentFile *)CAgentFile::CreateObject())
			{
				if	(SUCCEEDED (lResult = lLoadFile->Open (lFilePath)))
				{
					lAgentFile = TheServerApp->FindCachedFile (lLoadFile->GetGuid());
					if	(!lAgentFile)
					{
						lAgentFile = lLoadFile;
					}

					if	(mNotify.FindCachedFile (lLoadFile->GetGuid()))
					{
						lResult = AGENTERR_CHARACTERALREADYLOADED;
					}
					else
					if	(lAgentCharacter = new CDaAgentCharacter (TheServerApp->mNextCharID, lAgentFile, mNotify, mNotify))
					{
						if	(lLoadFile == lAgentFile)
						{
							lLoadFile.Detach ();
						}
						TheServerApp->mNextCharID++;
						TheServerApp->_OnCharacterLoaded (lAgentCharacter->GetCharID());
						pCharID = lAgentCharacter->GetCharID();
#ifdef	_LOG_CHARACTER
						if	(LogIsActive (_LOG_CHARACTER))
						{
							LogMessage (_LOG_CHARACTER, _T("Character [%d] Loaded [%p]"), pCharID, lAgentCharacter);
						}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
						TheServerApp->TraceCharacterAction (lAgentCharacter->GetCharID(), _T("Load"), _T("%s\t%ls"), pFilePath, lAgentFile->GetPath());
#endif						
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}

#ifdef	_DEBUG_REQUESTS
			LogMessage (_DEBUG_REQUESTS, _T("RequestComplete [%d] [%8.8X]"), pReqID, lResult);
#endif
			mNotify.RequestComplete (pReqID, lResult);
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgent::_OnDownloadComplete (CFileDownload * pDownload)
{
	bool	lRet = false;
	long	lReqID;

	if	(
			(pDownload)
		&&	(mCharactersLoading.FindValue (pDownload, lReqID))
		)
	{
		try
		{
			HRESULT				lResult;
			tPtr <CAgentFile>	lLoadFile;
			CAgentFile *		lAgentFile = NULL;
			CDaAgentCharacter *	lAgentCharacter = NULL;

			if	(lLoadFile = (CAgentFile *)CAgentFile::CreateObject())
			{
				if	(SUCCEEDED (lResult = lLoadFile->LoadAcf (pDownload)))
				{
					lAgentFile = TheServerApp->FindCachedFile (lLoadFile->GetGuid());
					if	(!lAgentFile)
					{
						lAgentFile = lLoadFile;
					}

					if	(mNotify.FindCachedFile (lLoadFile->GetGuid()))
					{
						lResult = AGENTERR_CHARACTERALREADYLOADED;
					}
					else
					if	(lAgentCharacter = new CDaAgentCharacter ((long)pDownload->mUserData, lAgentFile, mNotify, mNotify))
					{
						if	(lAgentFile == lLoadFile)
						{
							lLoadFile.Detach ();
						}
						TheServerApp->_OnCharacterLoaded (lAgentCharacter->GetCharID());
#ifdef	_LOG_CHARACTER
						if	(LogIsActive (_LOG_CHARACTER))
						{
							LogMessage (_LOG_CHARACTER, _T("Character [%d] Loaded [%p]"), lAgentCharacter->GetCharID(), lAgentCharacter);
						}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
						TheServerApp->TraceCharacterAction (lAgentCharacter->GetCharID(), _T("Load"), _T("%ls\t%ls"), pDownload->GetURL(), lAgentFile->GetPath());
#endif						
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}

#ifdef	_DEBUG_REQUESTS
			LogMessage (_DEBUG_REQUESTS, _T("RequestComplete [%d] [%8.8X]"), lReqID, lResult);
#endif
			mNotify.RequestComplete (lReqID, lResult);
		}
		catch AnyExceptionSilent

		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgent::UnloadCharacter (long pCharID)
{
	HRESULT	lResult = S_OK;

#ifdef	_LOG_CHARACTER
	if	(LogIsActive (_LOG_CHARACTER))
	{
		LogMessage (_LOG_CHARACTER, _T("UnloadCharacter [%d]"), pCharID);
	}
#endif
	try
	{
		CDaAgentCharacter *	lCharacter;

		if	(lCharacter = mNotify._GetCharacter (pCharID))
		{
#ifdef	_LOG_CHARACTER
			if	(LogIsActive (_LOG_CHARACTER))
			{
				LogMessage (_LOG_CHARACTER, _T("Character [%d] IsClientActive [%d] Clients [%d]"), pCharID, lCharacter->IsClientActive (), lCharacter->GetClientCount (lCharacter->GetCharID()));
			}
#endif
			try
			{
				if	(lCharacter->IsClientActive ())
				{
					lCharacter->StopAll (STOP_TYPE_ALL, AGENTREQERR_INTERRUPTEDUSER);
				}
				if	(lCharacter->GetClientCount (lCharacter->GetCharID()) <= 0)
				{
					lCharacter->Hide (true, true);
				}
			}
			catch AnyExceptionDebug
			try
			{
#ifdef	_TRACE_CHARACTER_ACTIONS
				TheServerApp->TraceCharacterAction (lCharacter->GetCharID(), _T("Unload"));
#endif				
				lCharacter->Terminate (false);
#ifdef	_STRICT_COMPATIBILITY
				lCharacter->Terminate (true);
				lCharacter->OnFinalRelease ();
#else
				lCharacter->ExternalRelease ();
#endif
			}
			catch AnyExceptionDebug

#ifdef	_LOG_CHARACTER
			if	(LogIsActive (_LOG_CHARACTER))
			{
				LogMessage (_LOG_CHARACTER, _T("Character [%d] Unloaded [%p]"), pCharID, lCharacter);
			}
#endif
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	catch AnyExceptionDebug
	try
	{
		TheServerApp->_OnCharacterUnloaded (pCharID);
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgent::LogMemory (UINT pLogLevel, LPCTSTR pTitle)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);

			if	(lTitle.IsEmpty())
			{
				lTitle = _T("Memory");
			}

			LogMessage (pLogLevel & ~LogHighVolume, _T("%s"), lTitle);

			try
			{
				tSS <PROCESS_MEMORY_COUNTERS, DWORD>	lCounters;

				if	(GetProcessMemoryInfo (GetCurrentProcess (), &lCounters, sizeof (lCounters)))
				{
					LogMessage (pLogLevel, _T("  WorkingSet [%u] Peak [%u]"), lCounters.WorkingSetSize, lCounters.PeakWorkingSetSize);
					LogMessage (pLogLevel, _T("  Paged      [%u] Peak [%u]"), lCounters.QuotaPagedPoolUsage, lCounters.QuotaPeakPagedPoolUsage);
					LogMessage (pLogLevel, _T("  NonPaged   [%u] Peak [%u]"), lCounters.QuotaNonPagedPoolUsage, lCounters.QuotaPeakNonPagedPoolUsage);
					LogMessage (pLogLevel, _T("  PageFile   [%u] Peak [%u]"), lCounters.PagefileUsage, lCounters.PeakPagefileUsage);
					LogMessage (pLogLevel, _T("    Faults   [%u]"), lCounters.PageFaultCount);
				}
				else
				{
					LogWinErr (pLogLevel, GetLastError (), lTitle);
				}
			}
			catch AnyExceptionSilent
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDaAgent, CCmdTarget)
	//{{AFX_MSG_MAP(CDaAgent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgent, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgent)
	DISP_FUNCTION_ID(CDaAgent, "Load", DISPID_IAgent_Load, DspLoad, VT_EMPTY, VTS_VARIANT VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgent, "Unload", DISPID_IAgent_Unload, DspUnload, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgent, "Register", DISPID_IAgent_Register, DspRegister, VT_EMPTY, VTS_UNKNOWN VTS_PI4)
	DISP_FUNCTION_ID(CDaAgent, "Unregister", DISPID_IAgent_Unregister, DspUnregister, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgent, "GetCharacter", DISPID_IAgent_GetCharacter, DspGetCharacter, VT_EMPTY, VTS_I4 VTS_PDISPATCH)
	DISP_FUNCTION_ID(CDaAgent, "GetSuspended", DISPID_IAgent_GetSuspended, DspGetSuspended, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgent, "GetCharacterEx", DISPID_IAgentEx_GetCharacterEx, DspGetCharacter, VT_EMPTY, VTS_I4 VTS_DISPATCH)
	DISP_FUNCTION_ID(CDaAgent, "GetVersion", DISPID_IAgentEx_GetVersion, DspGetVersion, VT_EMPTY, VTS_PI2 VTS_PI2)
	DISP_FUNCTION_ID(CDaAgent, "ShowDefaultCharacterProperties", DISPID_IAgentEx_ShowDefaultCharacterProperties, DspShowDefaultCharacterProperties, VT_EMPTY, VTS_I2 VTS_I2 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaAgent, CCmdTarget)
	INTERFACE_PART(CDaAgent, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgent, __uuidof(IDaServer), Agent)
	INTERFACE_PART(CDaAgent, __uuidof(IAgentEx), Agent)
	INTERFACE_PART(CDaAgent, __uuidof(IAgent), Agent)
	INTERFACE_PART(CDaAgent, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgent, __uuidof(IProvideClassInfo2), ProvideClassInfo)
	INTERFACE_PART(CDaAgent, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAgent, __uuidof(IStdMarshalInfo), StdMarshalInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgent, Agent)
IMPLEMENT_DISPATCH_IID(CDaAgent, __uuidof(IDaServer))
IMPLEMENT_IUNKNOWN(CDaAgent, StdMarshalInfo)

BEGIN_PROVIDECLASSINFO2(CDaAgent, __uuidof(CDaAgent))
	IMPLEMENT_PROVIDECLASSINFO2(CDaAgent, __uuidof(IDaSvrNotifySink15))
	IMPLEMENT_PROVIDECLASSINFO2(CDaAgent, __uuidof(IDaSvrNotifySink))
	IMPLEMENT_PROVIDECLASSINFO2(CDaAgent, __uuidof(IAgentNotifySinkEx))
END_PROVIDECLASSINFO2(CDaAgent)

BEGIN_SUPPORTERRORINFO(CDaAgent)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgent, __uuidof(IDaServer))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgent, __uuidof(IAgentEx))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgent, __uuidof(IAgent))
END_SUPPORTERRORINFO(CDaAgent)

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgent::XStdMarshalInfo::GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid)
{
	METHOD_PROLOGUE(CDaAgent, StdMarshalInfo)
	if	(!pClsid)
	{
		return E_POINTER;
	}
	*pClsid = __uuidof(DaServerHandler);

	try
	{
		pThis->mUsingHandler = 1;
		pThis->UnmanageObjectLifetime (pThis);
	}
	catch AnyExceptionSilent
#ifdef	_DEBUG_COM
	try
	{
		GUID lThreadId = GUID_NULL;
		LogComErr (LogNormal, CoGetCurrentLogicalThreadId (&lThreadId));
		LogMessage (_DEBUG_COM, _T("[%p(%u)] CDaAgent::XStdMarshalInfo::GetClassForHandler [%8.8X] [%s] Thread [%s]"), pThis, pThis->m_dwRef, dwDestContext, CGuidStr::GuidName(*pClsid), (CString)CGuidStr(lThreadId));
	}
	catch AnyExceptionSilent
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::Load (VARIANT vLoadKey, long * pdwCharID, long * pdwReqID)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::Load [%s]"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lFilePath;
	long	lReqID = 0;

	if	(pdwCharID == NULL)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!IsEmptyParm (&vLoadKey))
		{
			try
			{
				lFilePath = (BSTR)(_bstr_t)_variant_t(vLoadKey);
			}
			catch AnyExceptionSilent
		}

		lFilePath.TrimLeft ();
		lFilePath.TrimRight ();
		PathUnquoteSpaces (lFilePath.GetBuffer (lFilePath.GetLength ()));
		lFilePath.ReleaseBuffer ();
		lFilePath.TrimLeft ();
		lFilePath.TrimRight ();

		if	(pdwReqID)
		{
			lReqID = pThis->mNotify.NextReqID ();
		}
		lResult = pThis->LoadCharacter (lFilePath, *pdwCharID, lReqID);
	}

	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%u)] CDaAgent::XAgent::Load [%d] [%s]"), pThis, pThis->m_dwRef, (pdwCharID?*pdwCharID:-1), lFilePath);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::Unload (long dwCharID)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::Unload [%d]"), pThis, pThis->m_dwRef, dwCharID);
#endif
	HRESULT	lResult = pThis->UnloadCharacter (dwCharID);

	if	(SUCCEEDED (lResult))
	{
#ifdef	_DEBUG_NOT
		pThis->LogMemory (LogNormal|LogHighVolume, _T("Empty WorkingSet"));
#endif
		try
		{
			::EmptyWorkingSet (GetCurrentProcess ());
		}
		catch AnyExceptionSilent

#ifdef	_DEBUG_NOT
		pThis->LogMemory (LogNormal|LogHighVolume, _T("Emptied WorkingSet"));
#endif
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%u)] CDaAgent::XAgent::Unload [%d]"), pThis, pThis->m_dwRef, dwCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::Register (IUnknown * punkNotifySink, long * pdwSinkID)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::Register"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = pThis->mNotify.Register (punkNotifySink, pdwSinkID);

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgent::XAgent::Register [%d]"), pThis, pThis->m_dwRef, *pdwSinkID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::Unregister (long dwSinkID)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::Unregister [%d]"), pThis, pThis->m_dwRef, dwSinkID);
#endif
	HRESULT	lResult;

	if	(pThis->mInNotify > 0)
	{
		pThis->mInNotifyUnregister.Add (dwSinkID);
		lResult = S_FALSE;
	}
	else
	{
		lResult = pThis->mNotify.Unregister (dwSinkID);
	}
	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgent::XAgent::Unregister [%d]"), pThis, pThis->m_dwRef, dwSinkID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::GetCharacter (long dwCharID, IDispatch ** ppunkCharacter)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::GetCharacter"), pThis, pThis->m_dwRef);
#endif
	IDaSvrCharacterPtr	lCharacter;
	HRESULT				lResult = GetCharacterEx (dwCharID, &lCharacter);

	(*ppunkCharacter) = lCharacter.Detach();

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgent::XAgent::GetCharacter"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::GetCharacterEx (long dwCharID, IDaSvrCharacter **ppCharacterEx)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::GetCharacterEx"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCharacter *	lCharacter;
	IDaSvrCharacterPtr	lAgentCharacter;

	if	(!ppCharacterEx)
	{
		lResult = E_POINTER;
	}
	else
	if	(lCharacter = pThis->mNotify._GetCharacter (dwCharID))
	{
		lAgentCharacter = lCharacter->GetIDispatch (FALSE);
		(*ppCharacterEx) = lAgentCharacter.Detach ();
	}
	else
	{
		POSITION		lPos;
		long			lReqID;
		CFileDownload *	lDownload = NULL;

		for	(lPos = pThis->mCharactersLoading.GetStartPosition(); lPos;)
		{
			lDownload = NULL;
			pThis->mCharactersLoading.GetNextAssoc (lPos, lReqID, lDownload);
			if	(
					(lDownload)
				&&	(lDownload->mUserData == dwCharID)
				)
			{
				break;
			}
		}
		if	(lDownload)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
#ifdef	_STRICT_COMPATIBILITY
			lResult = E_OUTOFMEMORY;
#else
			lResult = AGENTERR_CHARACTERINVALID;
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgent::XAgent::GetCharacterEx"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::ShowDefaultCharacterProperties (short x, short y, long bUseDefaultPosition)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::ShowDefaultCharacterProperties"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CPropSheetCharSel *	lPropSheet = NULL;

	if	(lPropSheet = TheServerApp->GetPropSheetCharSel (true, pThis->mClientMutexName))
	{
		if	(!lPropSheet->GetSafeHwnd())
		{
			try
			{
				lPropSheet->Create ();
			}
			catch AnyExceptionDebug
		}
		if	(lPropSheet->GetSafeHwnd())
		{
			if	(!bUseDefaultPosition)
			{
				CRect	lWinRect;

				lPropSheet->GetWindowRect (&lWinRect);
				lWinRect.OffsetRect (x - lWinRect.left, y - lWinRect.top);
				lPropSheet->MoveWindow (&lWinRect);
			}

			lPropSheet->ShowWindow (SW_SHOW);
			lPropSheet->BringWindowToTop ();
			lPropSheet->SetForegroundWindow ();
		}
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgent::XAgent::ShowDefaultCharacterProperties"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::GetVersion (short *psMajor, short *psMinor)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::GetVersion"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(psMajor)
	{
		(*psMajor) = (short)gDaTypeLibVerMajor;
	}
	if	(psMinor)
	{
		(*psMinor) = (short)gDaTypeLibVerMinor;
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgent::XAgent::GetVersion"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgent::XAgent::GetSuspended (long * pbSuspended)
{
	METHOD_PROLOGUE(CDaAgent, Agent)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgent::XAgent::GetSuspended"), pThis, pThis->m_dwRef);
#endif
	if	(pbSuspended)
	{
		*pbSuspended = 0;
	}
#ifdef	_STRICT_COMPATIBILITY
	return S_OK;
#else
	return S_FALSE;
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgent::DspLoad(const VARIANT & LoadKey, long * CharID, long * ReqID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspLoad"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.Load (LoadKey, CharID, ReqID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspUnload(long CharID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspUnload"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.Unload (CharID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspRegister(LPUNKNOWN NotifySink, long * SinkID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspRegister"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.Register (NotifySink, SinkID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspUnregister(long SinkID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspUnregister"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.Unregister (SinkID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspGetCharacter(long CharID, LPDISPATCH * Character)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspGetCharacter"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.GetCharacter (CharID, Character);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspGetSuspended(long * Suspended)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspGetSuspended"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.GetSuspended (Suspended);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspGetVersion(short * Major, short * Minor)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspGetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.GetVersion (Major, Minor);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgent::DspShowDefaultCharacterProperties(short x, short y, long UseDefaultPosition)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgent::DspShowDefaultCharacterProperties"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgent.ShowDefaultCharacterProperties (x, y, UseDefaultPosition);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
