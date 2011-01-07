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
#define PSAPI_VERSION 1
#include <shlwapi.h>
#include <wininet.h>
#include <psapi.h>
#include "DaServer.h"
#include "DaSvrCharacter.h"
#include "DaSvrPropertySheet.h"
#include "DaSvrSettings.h"
#include "DaSvrAudioOutput.h"
#include "DaSvrSpeechInput.h"
#include "DaSvrCommandsWindow.h"
#include "DaSvrTTSEngines.h"
#include "DaSvrTTSEngine.h"
#include "DaSvrSREngines.h"
#include "DaSvrSREngine.h"
#include "DaSvrCharacterFiles.h"
#include "AgentFiles.h"
#include "FileDownload.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "GuidStr.h"
#include "ThreadSecurity.h"
#include "UserSecurity.h"
#include "MallocPtr.h"
#include "DebugProcess.h"
#include "DebugStr.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma warning (disable: 4355)

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Server"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_NOTIFY_LEVEL		(GetProfileDebugInt(_T("DebugNotifyLevel"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_STYLE			(GetProfileDebugInt(_T("DebugStyle"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_HANDLER			(GetProfileDebugInt(_T("DebugInterface_Handler"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_CHARACTER			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Server"),LogNormal,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED			MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
//#define	_TRACE_RESOURCES	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#endif

//#define	__RUNNING_STRESS_TEST__	1
//#define	__EMPTY_WORKING_SET__	1

/////////////////////////////////////////////////////////////////////////////

DaServer::DaServer()
:	CInstanceAnchor (_AtlModule),
	mUsingHandler (0),
	mNotifyPtr ((CEventNotify*)&mNotify),
	mCharacterStyle (CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_AutoPopupMenu|CharacterStyle_IconShown)
{
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResources (_TRACE_RESOURCES&~LogHighVolume, _T("[%p] DaServer"), this);
	}
#endif
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::DaServer (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif

	try
	{
		mNotify.mOwner = this;
		mNotify.mAnchor = this;
		mNotify.mGlobal = (CEventGlobal*)&_AtlModule;
		mNotify.ReflectSinks::AddNotifySink (this);
		mNotify.LockSinks::AddNotifySink (this);
	}
	catch AnyExceptionDebug
	try
	{
		_AtlModule.mInstanceNotify.Add (&mNotify);
	}
	catch AnyExceptionSilent

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::DaServer (%d) Done"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaServer::~DaServer()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::~DaServer (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::~DaServer (%d) Done [%d]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), (_AtlModule.GetLockCount()==0));
	}
#endif
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResources (_TRACE_RESOURCES&~LogHighVolume, _T("[%p] ~DaServer"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DaServer::Terminate (bool pFinal, bool pAbandonned)
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
					CoDisconnectObject (GetUnknown(), 0);
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
			mNotify.ReflectSinks::RemoveNotifySink (this);

			UnloadAllCharacters (pAbandonned);

			if	(pFinal)
			{
				mNotify.LockSinks::RemoveNotifySink (this);
			}
		}
		catch AnyExceptionDebug

		try
		{
			_AtlModule.mInstanceNotify.Remove (&mNotify);
		}
		catch AnyExceptionSilent
	}
}

void DaServer::Abandon ()
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::Abandon"), this, max(m_dwRef,-1));
		}
#endif
		if	(m_dwRef > 0)
		{
			try
			{
				CoDisconnectObject (GetUnknown(), 0);
			}
			catch AnyExceptionDebug
			m_dwRef = 0;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void DaServer::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::FinalRelease [%u]"), this, max(m_dwRef,-1), IsInNotify());
	}
#endif
	Terminate (false);
}

bool DaServer::VerifyClientLifetime ()
{
	if	(HasFinalReleased())
	{
#ifdef	_LOG_ABANDONED
		if	(LogIsActive (_LOG_ABANDONED))
		{
			LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaServer::VerifyClientLifetime [%u] HasFinalReleased"), this, max(m_dwRef,-1), IsInNotify());
		}
#endif
		try
		{
			if	(CanFinalRelease())
			{
#ifdef	_LOG_INSTANCE
				if	(LogIsActive (_LOG_INSTANCE))
				{
					LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer release abandoned"), this, max(m_dwRef,-1));
				}
#ifdef	_LOG_ABANDONED
				else
				if	(LogIsActive (_LOG_ABANDONED))
				{
					LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaServer release abandoned"), this, max(m_dwRef,-1));
				}
#endif
#endif
				m_dwRef = 1;
				Release ();
			}
		}
		catch AnyExceptionDebug

		return true;
	}
	return CSvrObjLifetime::VerifyClientLifetime ();
}

void DaServer::OnClientEnded ()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaServer::OnClientEnded [%u]"), this, max(m_dwRef,-1), IsInNotify());
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

bool DaServer::CanFinalRelease ()
{
	if	(IsInNotify() > 0)
	{
		return false;
	}
	else
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = CachedFileCount()-1; lFile = GetCachedFile (lFileNdx); lFileNdx--)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					try
					{
						DaSvrCharacter *	lCharacter = NULL;

						if	(
								(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
							&&	(!lCharacter->CanFinalRelease ())
							)
						{
							return false;
						}
					}
					catch AnyExceptionSilent
				}
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

bool DaServer::_PreNotify ()
{
#ifdef	_DEBUG_NOTIFY_LEVEL
	LogMessage (_DEBUG_NOTIFY_LEVEL, _T("[%p(%d)] DaServer::_PreNotify [%u]"), this, max(m_dwRef,-1), IsInNotify());
#endif
	if	(
			(m_dwRef > 0)
		&&	(CSvrObjLifetime::VerifyClientLifetime ())
		)
	{
		return CEventNotifyHolder<DaServer>::_PreNotify ();
	}
#ifdef	_DEBUG_NOTIFY_LEVEL
	LogMessage (_DEBUG_NOTIFY_LEVEL, _T("[%p(%d)] DaServer::_PreNotify failed"), this, max(m_dwRef,-1));
#endif
	return false;
}

bool DaServer::_PostNotify ()
{
	CEventNotifyHolder<DaServer>::_PostNotify ();
#ifdef	_DEBUG_NOTIFY_LEVEL
	LogMessage (_DEBUG_NOTIFY_LEVEL, _T("[%p(%d)] DaServer::_PostNotify [%u] HasFinalRelased [%u] CanFinalRelease [%u]"), this, max(m_dwRef,-1), IsInNotify(), HasFinalReleased(), CanFinalRelease());
#endif

	if	(IsInNotify() == 0)
	{
		mNotify.UnregisterDelayed ();
	}

	if	(HasFinalReleased ())
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer PostNotify -> HasFinalReleased"), this, max(m_dwRef,-1));
		}
#endif
#if	FALSE // For safety, just let it be abandoned
		if	(CanFinalRelease ())
		{
			m_dwRef = 1;
			Release ();
		}
		else
#endif
		{
			Abandon ();
		}
		return false;
	}
	else
	if	(
			(!IsInNotify ())
		&&	(!CSvrObjLifetime::VerifyClientLifetime ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer PostNotify -> !VerifyClientLifetime"), this, max(m_dwRef,-1));
		}
#endif
		Abandon ();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI DaServer::UpdateRegistryOverride (BOOL bRegister)
{
	HRESULT		lResult = UpdateRegistry (bRegister);

	if	(SUCCEEDED (lResult))
	{
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
				CRegKeyEx	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentServer)), false, true);
				CRegKeyEx	lServerProgId (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID), false, true);
				CRegKeyEx	lServerProgId2 (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID2), false, true);

				CRegString (lMsAgentClass, (LPCTSTR)NULL, true).Update (_T("Microsoft Agent Server 2.0"));
				CRegString (CRegKeyEx (lMsAgentClass, _T("ProgID"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID2));
				AtlRegisterClassCategoriesHelper (__uuidof(AgentServer), GetCategoryMap(), TRUE);

				CRegString (lServerProgId, (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID_NAME));
				CRegString (CRegKeyEx (lServerProgId, _T("CurVer"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID2));
				CRegString (lServerProgId2, (LPCTSTR)NULL, true).Update (_T(_AGENT_SERVER_PROGID_NAME));
				CRegString (CRegKeyEx (lServerProgId2, _T("CLSID"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(AgentServer)));

				CRegString (CRegKeyEx (lMsAgentClass, _T("AutoTreatAs"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(DaServer)));
				CRegString (CRegKeyEx (lMsAgentClass, _T("TreatAs"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(DaServer)));
			}
		}
		else
		{
			CRegKeyEx	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentServer)), false);
#ifdef	_WIN64
			CRegKeyEx	lMsAgentClass64 (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentServer64)), false);
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

				CRegKeyEx (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID), false).Delete ();
				CRegKeyEx (HKEY_CLASSES_ROOT, _T(_AGENT_SERVER_PROGID2), false).Delete ();
			}
			else
			{
				CRegKeyEx	lAutoTreatAs (lMsAgentClass, _T("AutoTreatAs"), false);
				CRegKeyEx	lTreatAs (lMsAgentClass, _T("TreatAs"), false);

				if	(
						(lAutoTreatAs.IsValid())
					&&	(lAutoTreatAs.Value().Value().CompareNoCase ((CString)CGuidStr(__uuidof(DaServer))) == 0)
					)
				{
					lAutoTreatAs.Delete ();
				}
				if	(
						(lTreatAs.IsValid())
					&&	(lTreatAs.Value().Value().CompareNoCase ((CString)CGuidStr(__uuidof(DaServer))) == 0)
					)
				{
					lTreatAs.Delete ();
				}
#ifdef	_WIN64
				if	(lMsAgentClass64.IsValid ())
				{
					CRegKeyEx	lAutoTreatAs64 (lMsAgentClass, _T("AutoTreatAs"), false);
					CRegKeyEx	lTreatAs64 (lMsAgentClass, _T("TreatAs"), false);

					if	(
							(lAutoTreatAs64.IsValid())
						&&	(lAutoTreatAs64.Value().Value().CompareNoCase ((CString)CGuidStr(__uuidof(DaServer))) == 0)
						)
					{
						lAutoTreatAs64.Delete ();
					}
					if	(
							(lTreatAs64.IsValid())
						&&	(lTreatAs64.Value().Value().CompareNoCase ((CString)CGuidStr(__uuidof(DaServer))) == 0)
						)
					{
						lTreatAs64.Delete ();
					}
				}
#endif
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI DaServer::CatchFirstQueryInterface (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	DaServer *	lThis = (DaServer *) pv;

	if	(
			(lThis->mUsingHandler > 0)
		&&	(lThis->mUsingHandler < 10)
		)
	{
		lThis->UnmanageObjectLifetime (lThis);
		if	(lThis->ManageObjectLifetime (lThis))
		{
			lThis->mUsingHandler = 10;
		}
		else
		{
			lThis->mUsingHandler++;
		}
	}
	return S_FALSE;
}

HRESULT WINAPI DaServer::DelegateIDaSvrPropertySheet (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT					lResult = E_NOINTERFACE;
	DaServer *				lThis = (DaServer *) pv;
	DaSvrPropertySheet *	lPropSheet;

	if	(lPropSheet = _AtlModule.GetSvrPropertySheet (true, lThis->mClientMutexName))
	{
		lResult = lPropSheet->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaServer::DelegateIDaSvrSettings (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT			lResult = E_NOINTERFACE;
	DaServer *		lThis = (DaServer *) pv;
	DaSvrSettings *	lSettings;

	if	(lSettings = _AtlModule.GetSvrSettings (true, lThis->mClientMutexName))
	{
		lResult = lSettings->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaServer::DelegateIDaSvrAudioOutput (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT				lResult = E_NOINTERFACE;
	DaServer *			lThis = (DaServer *) pv;
	DaSvrAudioOutput *	lAudioOutput;

	if	(lAudioOutput = _AtlModule.GetSvrAudioOutput (true, lThis->mClientMutexName))
	{
		lResult = lAudioOutput->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaServer::DelegateIDaSvrSpeechInput (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT				lResult = E_NOINTERFACE;
	DaServer *			lThis = (DaServer *) pv;
	DaSvrSpeechInput *	lSpeechInput;

	if	(lSpeechInput = _AtlModule.GetSvrSpeechInput (true, lThis->mClientMutexName))
	{
		lResult = lSpeechInput->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaServer::DelegateIDaSvrCommandsWindow (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT					lResult = E_NOINTERFACE;
	DaServer *				lThis = (DaServer *) pv;
	DaSvrCommandsWindow *	lCommandsWindow;

	if	(lCommandsWindow = _AtlModule.GetSvrCommandsWindow (true, lThis->mClientMutexName))
	{
		lResult = lCommandsWindow->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaServer::DelegateIDaSvrCharacterFiles (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT					lResult = E_NOINTERFACE;
	DaServer *				lThis = (DaServer *) pv;
	DaSvrCharacterFiles *	lCharacterFiles;

	if	(lCharacterFiles = _AtlModule.GetSvrCharacterFiles (true, lThis->mClientMutexName))
	{
		lResult = lCharacterFiles->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaServer::UnloadAllCharacters (bool pAbandonned)
{
	INT_PTR			lFileNdx;
	CAgentFile *	lFile;

#ifdef	_LOG_INSTANCE
	if	(
			(CachedFileCount() > 0)
		&&	(LogIsActive (_LOG_INSTANCE))
		)
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::UnloadAllCharacters [%d] Abandonned [%u]"), this, max(m_dwRef,-1), CachedFileCount(), pAbandonned);
	}
#endif
	for	(lFileNdx = CachedFileCount()-1; lFile = GetCachedFile (lFileNdx); lFileNdx--)
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;

		if	(GetFileClients (lFile, lFileClients))
		{
			try
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					DaSvrCharacter *	lCharacter = NULL;

					try
					{
						lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]);
					}
					catch AnyExceptionDebug

					if	(lCharacter)
					{
						try
						{
							if	(pAbandonned)
							{
#ifdef	_LOG_ABANDONED
								if	(LogIsActive (_LOG_ABANDONED))
								{
									LogMessage (_LOG_ABANDONED, _T("[%p(%d)] TerminateCharacter [%p(%d)(%d)]"), this, max(m_dwRef,-1), lCharacter, lCharacter->GetCharID(), max(lCharacter->m_dwRef,-1));
								}
#endif
								try
								{
									lCharacter->Terminate (false, pAbandonned);
								}
								catch AnyExceptionDebug
								try
								{
									lCharacter->Terminate (true, pAbandonned);
								}
								catch AnyExceptionDebug
								try
								{
									delete lCharacter;
								}
								catch AnyExceptionSilent

#ifdef	_LOG_ABANDONED
								if	(LogIsActive (_LOG_ABANDONED))
								{
									LogMessage (_LOG_ABANDONED, _T("[%p(%d)] TerminateCharacter [%p] Done"), this, max(m_dwRef,-1), lCharacter);
								}
#endif
							}
							else
							if	(lCharacter->CanFinalRelease ())
							{
								lCharacter->Terminate (false);
#ifdef	_STRICT_COMPATIBILITY
								lCharacter->Terminate (true);
								try
								{
									delete lCharacter;
								}
								catch AnyExceptionSilent
#else
								lCharacter->Release ();
#endif
							}
							else
							{
								lCharacter->Abandon ();
							}
						}
						catch AnyExceptionDebug

						lFileNdx--;
					}
				}
			}
			catch AnyExceptionDebug
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAtlString DaServer::GetSearchPath ()
{
	CAtlString				lSearchPath;
	DaSvrCharacterFiles *	lCharacterFiles;
	tBstrPtr				lCharacterSearchPath;

	if	(
			(lCharacterFiles = _AtlModule.GetSvrCharacterFiles (false, mClientMutexName))
		&&	(SUCCEEDED (lCharacterFiles->get_SearchPath (lCharacterSearchPath.Free())))
		)
	{
		lSearchPath = lCharacterSearchPath;
	}
	return lSearchPath;
}

HRESULT DaServer::LoadCharacter (LPCTSTR pFilePath, bool pIsDefault, long & pCharID, long & pReqID)
{
	HRESULT		lResult = S_OK;
	CAtlString	lFilePath (pFilePath);

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] DaServer::LoadCharacter [%s]"), this, pFilePath);
	}
#endif
#ifdef	_LOG_CHARACTER
	if	(LogIsActive (_LOG_CHARACTER))
	{
		LogMessage (_LOG_CHARACTER, _T("[%p(%d)] LoadCharacter [%s]"), this, max(m_dwRef,-1), pFilePath);
	}
#endif

	if	(lFilePath.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!CAgentFile::IsProperFilePath (lFilePath))
	{
		lResult = AGENTPROVERROR_PROTOCOL;
	}
	else
	if	(
			(GetDefaultCharacter ())
		||	(
				(pIsDefault)
			&&	(GetInstanceCharacter (-1))
			)
		)
	{
		lResult = AGENTERR_DEFAULTCHARACTER;
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

		if	(lDownload = CFileDownload::CreateInstance (lFilePath))
		{
			lDownload->mUserData = pCharID = _AtlModule.NextCharID();
			mCharactersLoading.SetAt (pReqID, lDownload);
#ifdef	_LOG_CHARACTER
			if	(LogIsActive (_LOG_CHARACTER))
			{
				LogMessage (_LOG_CHARACTER, _T("[%p(%d)] Character [%d] Loading [%d]"), this, max(m_dwRef,-1), pCharID, pReqID);
			}
#endif
			lResult = lDownload.Detach()->Download (GetControllingUnknown(), &mNotify);

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
		DaSvrCharacter *	lSvrCharacter = NULL;

		if	(pReqID <= 0)
		{
			pReqID = mNotify.NextReqID ();
		}
#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("RequestStart    [%d]"), pReqID);
#endif
		mNotify.RequestStart (pReqID);

		if	(lLoadFile = CAgentFile::CreateInstance())
		{
			if	(SUCCEEDED (lResult = lLoadFile->Open (lFilePath)))
			{
				lAgentFile = _AtlModule.FindCachedFile (lLoadFile->GetGuid());
				if	(!lAgentFile)
				{
					lAgentFile = lLoadFile;
				}

				if	(FindCachedFile (lLoadFile->GetGuid()))
				{
					lResult = AGENTERR_CHARACTERALREADYLOADED;
				}
				else
				if	(lSvrCharacter = DaSvrCharacter::CreateInstance (_AtlModule.NextCharID(), &mNotify, &_AtlModule, mClientMutexName))
				{
					lSvrCharacter->AddRef ();
					if	(
							(SUCCEEDED (lResult = lSvrCharacter->OpenFile (lAgentFile, pIsDefault)))
						&&	(SUCCEEDED (lResult = lSvrCharacter->RealizePopup (NULL, mCharacterStyle, WS_EX_TOPMOST)))
						)
					{
						if	(lLoadFile == lAgentFile)
						{
							lLoadFile.Detach ();
						}
						pCharID = lSvrCharacter->GetCharID();
						_AtlModule._CharacterLoaded (lSvrCharacter->GetCharID());
#ifdef	_LOG_CHARACTER
						if	(LogIsActive (_LOG_CHARACTER))
						{
							LogMessage (_LOG_CHARACTER, _T("[%p(%d)] Character [%d] Loaded [%p(%d)]"), this, max(m_dwRef,-1), pCharID, lSvrCharacter, lSvrCharacter->m_dwRef);
						}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
						_AtlModule.TraceCharacterAction (lSvrCharacter->GetCharID(), _T("Load"), _T("%s\t%ls\t%d"), pFilePath, lAgentFile->GetPath(), pReqID);
#endif
					}
					else
					{
						try
						{
							lSvrCharacter->Release ();
						}
						catch AnyExceptionSilent
					}
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

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] DaServer::LoadCharacter [%s] Done"), this, pFilePath);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool DaServer::_OnDownloadComplete (CFileDownload * pDownload)
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
			DaSvrCharacter *	lSvrCharacter = NULL;

			if	(lLoadFile = CAgentFile::CreateInstance())
			{
				if	(SUCCEEDED (lResult = lLoadFile->LoadAcf (pDownload)))
				{
					lAgentFile = _AtlModule.FindCachedFile (lLoadFile->GetGuid());
					if	(!lAgentFile)
					{
						lAgentFile = lLoadFile;
					}

					if	(FindCachedFile (lLoadFile->GetGuid()))
					{
						lResult = AGENTERR_CHARACTERALREADYLOADED;
					}
					else
					if	(lSvrCharacter = DaSvrCharacter::CreateInstance ((long)pDownload->mUserData, &mNotify, &_AtlModule, mClientMutexName))
					{
						lSvrCharacter->AddRef ();
						if	(
								(SUCCEEDED (lResult = lSvrCharacter->OpenFile (lAgentFile)))
							&&	(SUCCEEDED (lResult = lSvrCharacter->RealizePopup (NULL, mCharacterStyle, WS_EX_TOPMOST)))
							)
						{
							if	(lAgentFile == lLoadFile)
							{
								lLoadFile.Detach ();
							}
							_AtlModule._CharacterLoaded (lSvrCharacter->GetCharID());
#ifdef	_LOG_CHARACTER
							if	(LogIsActive (_LOG_CHARACTER))
							{
								LogMessage (_LOG_CHARACTER, _T("[%p(%d)] Character [%d] Loaded [%p(%d)]"), this, max(m_dwRef,-1), lSvrCharacter->GetCharID(), lSvrCharacter, lSvrCharacter->m_dwRef);
							}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
							_AtlModule.TraceCharacterAction (lSvrCharacter->GetCharID(), _T("Load"), _T("%ls\t%ls\t%d"), pDownload->GetURL(), lAgentFile->GetPath(), lReqID);
#endif
						}
						else
						{
							try
							{
								lSvrCharacter->Release ();
							}
							catch AnyExceptionSilent
						}
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

HRESULT DaServer::UnloadCharacter (long pCharID)
{
	HRESULT	lResult = S_OK;

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] DaServer::UnloadCharacter [%d]"), this, pCharID);
	}
#endif
#ifdef	_LOG_CHARACTER
	if	(LogIsActive (_LOG_CHARACTER))
	{
		LogMessage (_LOG_CHARACTER, _T("[%p(%d)] UnloadCharacter [%d]"), this, max(m_dwRef,-1), pCharID);
	}
#endif
	try
	{
		DaSvrCharacter *	lCharacter;

		if	(lCharacter = dynamic_cast <DaSvrCharacter *> (GetInstanceCharacter (pCharID)))
		{
#ifdef	_LOG_CHARACTER
			if	(LogIsActive (_LOG_CHARACTER))
			{
				LogMessage (_LOG_CHARACTER, _T("[%p(%d)] Character [%d] IsClientActive [%d] Clients [%d]"), this, max(m_dwRef,-1), pCharID, lCharacter->IsClientActive (), lCharacter->GetClientCount (lCharacter->GetCharID()));
			}
#endif
			try
			{
				if	(lCharacter->IsClientActive ())
				{
					lCharacter->CDaCmnCharacter::StopAll (StopAll_Everything, AGENTREQERR_INTERRUPTEDUSER);
				}
				if	(lCharacter->GetClientCount (lCharacter->GetCharID()) <= 0)
				{
					lCharacter->CDaCmnCharacter::Hide (true, true);
				}
			}
			catch AnyExceptionDebug
			try
			{
#ifdef	_TRACE_CHARACTER_ACTIONS
				_AtlModule.TraceCharacterAction (lCharacter->GetCharID(), _T("Unload"));
#endif
				if	(lCharacter->CanFinalRelease())
				{
					lCharacter->Terminate (false);
#ifdef	_STRICT_COMPATIBILITY
					lCharacter->Terminate (true);
					try
					{
						delete lCharacter;
					}
					catch AnyExceptionSilent
#else
					lCharacter->Release ();
#endif
				}
				else
				{
					lCharacter->Abandon ();
				}
			}
			catch AnyExceptionDebug

#ifdef	_LOG_CHARACTER
			if	(LogIsActive (_LOG_CHARACTER))
			{
				LogMessage (_LOG_CHARACTER, _T("[%p(%d)] Character [%d] Unloaded [%p]"), this, max(m_dwRef,-1), pCharID, lCharacter);
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
		_AtlModule._CharacterUnloaded (pCharID);
	}
	catch AnyExceptionDebug
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] DaServer::UnloadCharacter [%d] Done"), this, pCharID);
	}
#endif

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaServer::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaServer2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaServer), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgent), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid)
{
	if	(!pClsid)
	{
		return E_POINTER;
	}
	*pClsid = __uuidof(DaServerHandler);

	try
	{
		mUsingHandler = 1;
		UnmanageObjectLifetime (this);
	}
	catch AnyExceptionSilent

#ifdef	_DEBUG_HANDLER
	if	(LogIsActive (_DEBUG_HANDLER))
	{
		try
		{
			GUID lThreadId = GUID_NULL;
			LogComErr (LogNormal|LogTime, CoGetCurrentLogicalThreadId (&lThreadId));
			LogMessage (_DEBUG_HANDLER, _T("[%p(%d)] DaServer::GetClassForHandler [%8.8X] [%p] [%s] Thread [%s]"), this, max(m_dwRef,-1), dwDestContext, pvDestContext, CGuidStr::GuidName(*pClsid), (CString)CGuidStr(lThreadId));
		}
		catch AnyExceptionSilent
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::Load (VARIANT Provider, long * pdwCharID, long * RequestID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Load [%s]"), this, max(m_dwRef,-1), DebugVariant(Provider));
#endif
	HRESULT		lResult = S_OK;
	CAtlString	lFilePath;
	bool		lFilePathIsDefault = false;
	long		lReqID = 0;

	if	(pdwCharID == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = CDaCmnCharacter::GetLoadPath (Provider, lFilePath, GetSearchPath(), &lFilePathIsDefault)))
	{
		if	(RequestID)
		{
			lReqID = mNotify.NextReqID ();
		}
		lResult = LoadCharacter (lFilePath, lFilePathIsDefault, *pdwCharID, lReqID);
	}

	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::Load [%d] [%s] Default [%u]"), this, max(m_dwRef,-1), (pdwCharID?*pdwCharID:-1), lFilePath, lFilePathIsDefault);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::Unload (long CharacterID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Unload [%d]"), this, max(m_dwRef,-1), CharacterID);
#endif
	HRESULT	lResult = UnloadCharacter (CharacterID);

#if	__RUNNING_STRESS_TEST__
	CDebugProcess().LogWorkingSetInline (LogIfActive|LogHighVolume);
	CDebugProcess().LogAddressSpaceInline (LogIfActive|LogHighVolume);
	CDebugProcess().LogGuiResourcesInline (LogIfActive|LogHighVolume);
	LogMessage (LogIfActive|LogTime, _T(""));
#endif
#if	__EMPTY_WORKING_SET__
	if	(SUCCEEDED (lResult))
	{
		try
		{
			::EmptyWorkingSet (GetCurrentProcess ());
		}
		catch AnyExceptionSilent
	}
#endif

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::Unload [%d]"), this, max(m_dwRef,-1), CharacterID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::Register (IUnknown * punkNotifySink, long * pdwSinkID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Register"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = mNotify.Register (punkNotifySink, pdwSinkID);

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::Register [%d]"), this, max(m_dwRef,-1), *pdwSinkID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::Unregister (long dwSinkID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Unregister [%d]"), this, max(m_dwRef,-1), dwSinkID);
#endif
	HRESULT	lResult;

	lResult = mNotify.Unregister (dwSinkID, (IsInNotify()>0));

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::Unregister [%d]"), this, max(m_dwRef,-1), dwSinkID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetCharacter (long CharacterID, IDispatch ** Character)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacter"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Character)
	{
		lResult = E_POINTER;
	}
	else
	{
		IDaSvrCharacterPtr	lCharacter;

		lResult = GetCharacterEx (CharacterID, &lCharacter);
		(*Character) = lCharacter.Detach();
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacter"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterEx (long CharacterID, IDaSvrCharacter **Character)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterEx"), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	DaSvrCharacter *	lCharacter;
	IDaSvrCharacterPtr	lSvrCharacter;

	if	(!Character)
	{
		lResult = E_POINTER;
	}
	else
	if	(lCharacter = dynamic_cast <DaSvrCharacter *> (GetInstanceCharacter (CharacterID)))
	{
		lSvrCharacter = lCharacter->GetControllingUnknown ();
		(*Character) = lSvrCharacter.Detach ();
	}
	else
	{
		POSITION		lPos;
		long			lReqID;
		CFileDownload *	lDownload = NULL;

		for	(lPos = mCharactersLoading.GetStartPosition(); lPos;)
		{
			lDownload = NULL;
			mCharactersLoading.GetNextAssoc (lPos, lReqID, lDownload);
			if	(
					(lDownload)
				&&	(lDownload->mUserData == CharacterID)
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
#ifdef	_STRICT_COMPATIBILITY_NOT
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterEx"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::get_Character (long CharacterID, IDaSvrCharacter2 **Character2)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_Character"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Character2)
	{
		lResult = E_POINTER;
	}
	else
	{
		IDaSvrCharacterPtr	lCharacter;
		IDaSvrCharacter2Ptr	lCharacter2;

		lResult = GetCharacterEx (CharacterID, &lCharacter);
		lCharacter2 = lCharacter;
		(*Character2) = lCharacter2.Detach();
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_Character"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::ShowDefaultCharacterProperties (short X, short Y, long UseDefaultPosition)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::ShowDefaultCharacterProperties"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	DaSvrPropertySheet *	lPropertySheet;

	if	(lPropertySheet = _AtlModule.GetSvrPropertySheet (true, mClientMutexName))
	{
		lPropertySheet->put_Page (_bstr_t (PropertySheet_PageName_Character));
		if	(!UseDefaultPosition)
		{
			if	(
					(X >= 0)
				&&	(Y >= 0)
				)
			{
				lPropertySheet->SetPosition (X, Y);
			}
			else
			{
				if	(X >= 0)
				{
					lPropertySheet->put_Left (X);
				}
				if	(Y >= 0)
				{
					lPropertySheet->put_Top (Y);
				}
			}
		}
		lPropertySheet->put_Visible (VARIANT_TRUE);
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::ShowDefaultCharacterProperties"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetVersion (short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetVersion"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(MajorVersion)
	{
		(*MajorVersion) = (short)DoubleAgentSvr_MajorVer;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = (short)DoubleAgentSvr_MinorVer;
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetVersion"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetSuspended (long * Suspended)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetSuspended"), this, max(m_dwRef,-1));
#endif
	if	(Suspended)
	{
		*Suspended = 0;
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

HRESULT STDMETHODCALLTYPE DaServer::get_CharacterFiles (IDaSvrCharacterFiles **CharacterFiles)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_CharacterFiles"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	DaSvrCharacterFiles *	lCharacterFiles;
	IDaSvrCharacterFilesPtr	lInterface;

	if	(!CharacterFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharacterFiles) = NULL;

		if	(lCharacterFiles = _AtlModule.GetSvrCharacterFiles (true, mClientMutexName))
		{
			lInterface = lCharacterFiles->GetControllingUnknown ();
			(*CharacterFiles) = lInterface.Detach();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_CharacterFiles"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::get_PropertySheet (IDaSvrPropertySheet2 **PropertySheet)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_PropertySheet"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	DaSvrPropertySheet *	lPropertySheet;
	IDaSvrPropertySheet2Ptr	lInterface;

	if	(!PropertySheet)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*PropertySheet) = NULL;

		if	(lPropertySheet = _AtlModule.GetSvrPropertySheet (true, mClientMutexName))
		{
			lInterface = lPropertySheet->GetControllingUnknown ();
			(*PropertySheet) = lInterface.Detach();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_PropertySheet"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::get_CommandsWindow (IDaSvrCommandsWindow2 **CommandsWindow)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_CommandsWindow"), this, max(m_dwRef,-1));
#endif
	HRESULT						lResult = S_OK;
	DaSvrCommandsWindow *		lCommandsWindow;
	IDaSvrCommandsWindow2Ptr	lInterface;

	if	(!CommandsWindow)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CommandsWindow) = NULL;

		if	(lCommandsWindow = _AtlModule.GetSvrCommandsWindow (true, mClientMutexName))
		{
			lInterface = lCommandsWindow->GetControllingUnknown ();
			(*CommandsWindow) = lInterface.Detach();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_CommandsWindow"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::get_Settings (IDaSvrSettings **Settings)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_Settings"), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	DaSvrSettings *		lSettings;
	IDaSvrSettingsPtr	lInterface;

	if	(!Settings)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Settings) = NULL;

		if	(lSettings = _AtlModule.GetSvrSettings (true, mClientMutexName))
		{
			lInterface = lSettings->GetControllingUnknown ();
			(*Settings) = lInterface.Detach();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_Settings"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::put_CharacterStyle (long CharacterStyle)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::put_CharacterStyle [%d]"), this, max(m_dwRef,-1), CharacterStyle);
	}
#endif
	HRESULT	lResult = S_OK;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (0, _T("put_CharacterStyle"), _T("%u"), CharacterStyle);
#endif
#ifdef	_DEBUG_STYLE
	if	(
			((DWORD)CharacterStyle != mCharacterStyle)
		&&	(LogIsActive (_DEBUG_STYLE))
		)
	{
		LogMessage (_DEBUG_STYLE, _T("[%p(%d)] DaServer CharacterStyle [%8.8X]"), this, max(m_dwRef,-1), CharacterStyle);
	}
#endif

	mCharacterStyle = (DWORD)CharacterStyle;

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::put_CharacterStyle"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::get_CharacterStyle (long *CharacterStyle)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_CharacterStyle"), this, max(m_dwRef,-1));
	}
#endif
	HRESULT	lResult = S_OK;

	if	(CharacterStyle)
	{
		(*CharacterStyle) = (long)mCharacterStyle;
	}
	else
	{
		lResult = E_POINTER;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_CharacterStyle"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::get_TTSEngines (IDaSvrTTSEngines **TTSEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_TTSEngines"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	tPtr <DaSvrTTSEngines>	lTTSEngines;
	IDaSvrTTSEnginesPtr		lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(lTTSEngines = DaSvrTTSEngines::CreateInstance (mClientMutexName))
		{
			if	(SUCCEEDED (lResult = lTTSEngines->UseAllVoices ()))
			{
				lInterface = lTTSEngines.Detach()->GetControllingUnknown();
				(*TTSEngines) = lInterface.Detach();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_TTSEngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindTTSEngines (long LanguageID, short Gender, IDaSvrTTSEngines **TTSEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindTTSEngines"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <DaSvrTTSEngines>	lTTSEngines;
	IDaSvrTTSEnginesPtr		lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(lTTSEngines = DaSvrTTSEngines::CreateInstance (mClientMutexName))
		{
			if	(SUCCEEDED (lResult = lTTSEngines->UseTheseVoices (NULL, (LANGID)LanguageID, Gender)))
			{
				lInterface = lTTSEngines.Detach()->GetControllingUnknown();
				(*TTSEngines) = lInterface.Detach();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindTTSEngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterTTSEngine (VARIANT Provider, IDaSvrTTSEngine **TTSEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterTTSEngine"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	tPtr <CAgentFile>		lAgentFile;
	tPtr <DaSvrTTSEngine>	lTTSEngine;
	IDaSvrTTSEnginePtr		lInterface;

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile, GetSearchPath())))
		{
			if	(lTTSEngine = DaSvrTTSEngine::CreateInstance ((CSapi5VoiceInfo*)NULL, mClientMutexName))
			{
				if	(lTTSEngine->Initialize (lAgentFile))
				{
					lInterface = lTTSEngine.Detach()->GetControllingUnknown();
					(*TTSEngine) = lInterface.Detach();
					lResult = S_OK;
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterTTSEngine"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindCharacterTTSEngines (VARIANT Provider, long LanguageID, IDaSvrTTSEngines **TTSEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindCharacterTTSEngines"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <CAgentFile>		lAgentFile;
	tPtr <DaSvrTTSEngines>	lTTSEngines;
	IDaSvrTTSEnginesPtr		lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile, GetSearchPath())))
		{
			if	(lTTSEngines = DaSvrTTSEngines::CreateInstance (mClientMutexName))
			{
				if	(SUCCEEDED (lResult = lTTSEngines->UseTheseVoices (lAgentFile, (LANGID)LanguageID, SpeechGender_Neutral)))
				{
					lInterface = lTTSEngines.Detach()->GetControllingUnknown();
					(*TTSEngines) = lInterface.Detach();
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindCharacterTTSEngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::get_SREngines (IDaSvrSREngines **SREngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_SREngines"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	tPtr <DaSvrSREngines>	lSREngines;
	IDaSvrSREnginesPtr		lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(lSREngines = DaSvrSREngines::CreateInstance (mClientMutexName))
		{
			if	(SUCCEEDED (lResult = lSREngines->UseAllInputs ()))
			{
				lInterface = lSREngines.Detach()->GetControllingUnknown();
				(*SREngines) = lInterface.Detach();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_SREngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindSREngines (long LanguageID, IDaSvrSREngines **SREngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindSREngines"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <DaSvrSREngines>	lSREngines;
	IDaSvrSREnginesPtr		lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(lSREngines = DaSvrSREngines::CreateInstance (mClientMutexName))
		{
			if	(SUCCEEDED (lResult = lSREngines->UseTheseInputs (NULL, (LANGID)LanguageID)))
			{
				lInterface = lSREngines.Detach()->GetControllingUnknown();
				(*SREngines) = lInterface.Detach();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindSREngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterSREngine (VARIANT Provider, IDaSvrSREngine **SREngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterSREngine"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_OK;
	tPtr <CAgentFile>		lAgentFile;
	tPtr <DaSvrSREngine>	lSREngine;
	IDaSvrSREnginePtr		lInterface;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile, GetSearchPath())))
		{
			if	(lSREngine = DaSvrSREngine::CreateInstance (NULL, mClientMutexName))
			{
				if	(lSREngine->Initialize (lAgentFile))
				{
					lInterface = lSREngine.Detach()->GetControllingUnknown();
					(*SREngine) = lInterface.Detach();
					lResult = S_OK;
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterSREngine"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindCharacterSREngines (VARIANT Provider, long LanguageID, IDaSvrSREngines **SREngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindCharacterSREngines"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = S_FALSE;
	tPtr <CAgentFile>		lAgentFile;
	tPtr <DaSvrSREngines>	lSREngines;
	IDaSvrSREnginesPtr		lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile, GetSearchPath())))
		{
			if	(lSREngines = DaSvrSREngines::CreateInstance (mClientMutexName))
			{
				if	(SUCCEEDED (lResult = lSREngines->UseTheseInputs (lAgentFile, (LANGID)LanguageID)))
				{
					lInterface = lSREngines.Detach()->GetControllingUnknown();
					(*SREngines) = lInterface.Detach();
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindCharacterSREngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
