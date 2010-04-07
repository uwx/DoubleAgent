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
#define PSAPI_VERSION 1
#include <shlwapi.h>
#include <wininet.h>
#include <psapi.h>
#include "DaServer.h"
#include "DaSvrCharacter.h"
#include "DaSvrPropertySheet.h"
#include "DaSvrAudioOutput.h"
#include "DaSvrSpeechInput.h"
#include "DaSvrCommandsWindow.h"
#include "DaSvrSpeechEngines.h"
#include "DaSvrRecognitionEngines.h"
#include "PropSheetCharSel.h"
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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Server"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_HANDLER			(GetProfileDebugInt(_T("DebugInterface_Handler"),LogVerbose,true)&0xFFFF)
#define	_LOG_CHARACTER			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_FILE_LOAD			(GetProfileDebugInt(_T("LogFileLoad"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Server"),LogNormal,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_FILE_LOAD
#define	_LOG_FILE_LOAD			LogDetails
#endif
//#define	__RUNNING_STRESS_TEST__	1
//#define	__EMPTY_WORKING_SET__	1

/////////////////////////////////////////////////////////////////////////////

DaServer::DaServer()
:	mUsingHandler (0),
	mCharacterStyle (CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_AutoPopupMenu|CharacterStyle_IconShown),
	mNotify (*this),
	mInNotify (0)
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::DaServer (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif

	try
	{
		_AtlModule.mNotify.Add (&mNotify);
	}
	catch AnyExceptionSilent

	mNotify._RegisterInternalNotify (this, true);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::DaServer (%d) Done"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaServer::~DaServer()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::~DaServer (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::~DaServer (%d) Done [%d]"), this, m_dwRef, _AtlModule.GetLockCount(), (_AtlModule.GetLockCount()==0));
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
			mNotify._RegisterInternalNotify (this, false);
			UnloadAllCharacters (pAbandonned);
		}
		catch AnyExceptionDebug

		try
		{
			_AtlModule.mNotify.Remove (&mNotify);
		}
		catch AnyExceptionSilent
	}
}

void DaServer::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::FinalRelease [%u]"), this, m_dwRef, IsInNotify());
	}
#endif
	Terminate (false);
}

bool DaServer::CanFinalRelease ()
{
	return (IsInNotify() == 0);
}

void DaServer::OnClientEnded ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::OnClientEnded"), this, m_dwRef);
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

	if	(lThis->mUsingHandler == 1)
	{
		lThis->UnmanageObjectLifetime (lThis);
		if	(lThis->ManageObjectLifetime (lThis))
		{
			lThis->mUsingHandler = 2;
		}
		else
		{
			lThis->mUsingHandler = 0;
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

bool DaServer::PreNotify ()
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

bool DaServer::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		if	(
				(CanFinalRelease ())
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
				(CanFinalRelease ())
			&&	(HasFinalReleased ())
			)
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive (_LOG_INSTANCE))
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer PostNotify -> Release"), this, m_dwRef);
			}
#endif
			m_dwRef = 1;
			Release ();
			return false;
		}
		return true;
	}
	return false;
}

UINT DaServer::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void DaServer::UnloadAllCharacters (bool pAbandonned)
{
	INT_PTR			lFileNdx;
	CAgentFile *	lFile;

#ifdef	_LOG_INSTANCE
	if	(
			(mNotify.GetCachedFile (0))
		&&	(LogIsActive())
		)
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaServer::UnloadAllCharacters [%d] Abandonned [%u]"), this, m_dwRef, mNotify.CachedFileCount(), pAbandonned);
	}
#endif
	for	(lFileNdx = 0; lFile = mNotify.GetCachedFile (lFileNdx); lFileNdx++)
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		DaSvrCharacter *					lCharacter;

		if	(mNotify.GetFileClients (lFile, lFileClients))
		{
			for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
			{
				if	(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
				{
					try
					{
						if	(pAbandonned)
						{
							lCharacter->Terminate (true, pAbandonned);
							try
							{
								delete lCharacter;
							}
							catch AnyExceptionSilent
						}
						else
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

HRESULT DaServer::GetLoadPath (VARIANT pLoadKey, CString & pFilePath)
{
	HRESULT	lResult = S_OK;

	pFilePath.Empty ();

	if	(!IsEmptyParm (&pLoadKey))
	{
		try
		{
			pFilePath = (BSTR)(_bstr_t)_variant_t(pLoadKey);
	}
		catch AnyExceptionSilent
	}

	pFilePath.TrimLeft ();
	pFilePath.TrimRight ();
	PathUnquoteSpaces (pFilePath.GetBuffer (pFilePath.GetLength ()));
	pFilePath.ReleaseBuffer ();
	pFilePath.TrimLeft ();
	pFilePath.TrimRight ();

	if	(pFilePath.IsEmpty ())
	{
		pFilePath = CAgentFiles::GetDefCharPath ();
#ifdef	_LOG_FILE_LOAD
		if	(LogIsActive (_LOG_FILE_LOAD))
		{
			LogMessage (_LOG_FILE_LOAD, _T("Load default Path [%s]"), pFilePath);
		}
#endif
	}
	else
	if	(CAgentFile::IsRelativeFilePath (pFilePath))
	{
		UINT	lPathNum;
		CString	lFilePath;

		lFilePath = CAgentFile::ParseFilePath (pFilePath);

		if	(!lFilePath.IsEmpty ())
		{
			if	(PathFileExists (lFilePath))
			{
#ifdef	_LOG_FILE_LOAD
				if	(LogIsActive (_LOG_FILE_LOAD))
				{
					LogMessage (_LOG_FILE_LOAD, _T("File [%s] try Path [%s]"), pFilePath, lFilePath);
				}
#endif
				pFilePath = lFilePath;
			}
			else
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
				if	(!lFilePath.IsEmpty ())
			{
					pFilePath = lFilePath;
			}
		}
		}
	}

	if	(pFilePath.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
		else
		{
		pFilePath = CAgentFile::ParseFilePath (pFilePath);

		if	(!CAgentFile::IsProperFilePath (pFilePath))
		{
			lResult = AGENTPROVERROR_PROTOCOL;
		}
	}
	return lResult;
}

HRESULT DaServer::LoadCharacter (LPCTSTR pFilePath, long & pCharID, long & pReqID)
{
	HRESULT	lResult = S_OK;
	CString	lFilePath (pFilePath);

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] DaServer::LoadCharacter [%s]"), this, pFilePath);
	}
#endif
#ifdef	_LOG_CHARACTER
	if	(LogIsActive (_LOG_CHARACTER))
	{
		LogMessage (_LOG_CHARACTER, _T("LoadCharacter [%s]"), pFilePath);
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
				lDownload->mUserData = pCharID = _AtlModule.mNextCharID++;
				mCharactersLoading.SetAt (pReqID, lDownload);
#ifdef	_LOG_CHARACTER
				if	(LogIsActive (_LOG_CHARACTER))
				{
					LogMessage (_LOG_CHARACTER, _T("Character [%d] Loading [%d]"), pCharID, pReqID);
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

					if	(mNotify.FindCachedFile (lLoadFile->GetGuid()))
					{
						lResult = AGENTERR_CHARACTERALREADYLOADED;
					}
					else
					if	(lSvrCharacter = DaSvrCharacter::CreateInstance (_AtlModule.mNextCharID, lAgentFile, &mNotify, &mNotify, mClientMutexName))
					{
						if	(lLoadFile == lAgentFile)
						{
							lLoadFile.Detach ();
						}
						lSvrCharacter->AddRef ();
						lSvrCharacter->SetStyle (~mCharacterStyle, mCharacterStyle);
						pCharID = lSvrCharacter->GetCharID();
						_AtlModule.mNextCharID++;
						_AtlModule._OnCharacterLoaded (lSvrCharacter->GetCharID());
#ifdef	_LOG_CHARACTER
						if	(LogIsActive (_LOG_CHARACTER))
						{
							LogMessage (_LOG_CHARACTER, _T("Character [%d] Loaded [%p(%d)]"), pCharID, lSvrCharacter, lSvrCharacter->m_dwRef);
						}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
						_AtlModule.TraceCharacterAction (lSvrCharacter->GetCharID(), _T("Load"), _T("%s\t%ls\t%d"), pFilePath, lAgentFile->GetPath(), pReqID);
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

					if	(mNotify.FindCachedFile (lLoadFile->GetGuid()))
					{
						lResult = AGENTERR_CHARACTERALREADYLOADED;
					}
					else
					if	(lSvrCharacter = DaSvrCharacter::CreateInstance ((long)pDownload->mUserData, lAgentFile, &mNotify, &mNotify, mClientMutexName))
					{
						if	(lAgentFile == lLoadFile)
						{
							lLoadFile.Detach ();
						}
						lSvrCharacter->AddRef ();
						lSvrCharacter->SetStyle (~mCharacterStyle, mCharacterStyle);
						_AtlModule._OnCharacterLoaded (lSvrCharacter->GetCharID());
#ifdef	_LOG_CHARACTER
						if	(LogIsActive (_LOG_CHARACTER))
						{
							LogMessage (_LOG_CHARACTER, _T("Character [%d] Loaded [%p(%d)]"), lSvrCharacter->GetCharID(), lSvrCharacter, lSvrCharacter->m_dwRef);
						}
#endif
#ifdef	_TRACE_CHARACTER_ACTIONS
						_AtlModule.TraceCharacterAction (lSvrCharacter->GetCharID(), _T("Load"), _T("%ls\t%ls\t%d"), pDownload->GetURL(), lAgentFile->GetPath(), lReqID);
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
		LogMessage (_LOG_CHARACTER, _T("UnloadCharacter [%d]"), pCharID);
	}
#endif
	try
	{
		DaSvrCharacter *	lCharacter;

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
					lCharacter->StopAll (StopType_All, AGENTREQERR_INTERRUPTEDUSER);
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
				_AtlModule.TraceCharacterAction (lCharacter->GetCharID(), _T("Unload"));
#endif
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
		_AtlModule._OnCharacterUnloaded (pCharID);
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
			LogComErr (LogNormal, CoGetCurrentLogicalThreadId (&lThreadId));
			LogMessage (_DEBUG_HANDLER, _T("[%p(%d)] DaServer::GetClassForHandler [%8.8X] [%s] Thread [%s]"), this, m_dwRef, dwDestContext, CGuidStr::GuidName(*pClsid), (CString)CGuidStr(lThreadId));
		}
		catch AnyExceptionSilent
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::Load (VARIANT vLoadKey, long * pdwCharID, long * pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Load [%s]"), this, m_dwRef, DebugVariant(vLoadKey));
#endif
	HRESULT	lResult = S_OK;
	CString	lFilePath;
	long	lReqID = 0;

	if	(pdwCharID == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = GetLoadPath (vLoadKey, lFilePath)))
	{
		if	(pdwReqID)
		{
			lReqID = mNotify.NextReqID ();
		}
		lResult = LoadCharacter (lFilePath, *pdwCharID, lReqID);
	}

	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%d)] DaServer::Load [%d] [%s]"), this, m_dwRef, (pdwCharID?*pdwCharID:-1), lFilePath);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::Unload (long dwCharID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Unload [%d]"), this, m_dwRef, dwCharID);
#endif
	HRESULT	lResult = UnloadCharacter (dwCharID);

#if	__RUNNING_STRESS_TEST__
	CDebugProcess().LogWorkingSetInline (LogIfActive|LogHighVolume);
	CDebugProcess().LogAddressSpaceInline (LogIfActive|LogHighVolume);
	CDebugProcess().LogGuiResourcesInline (LogIfActive|LogHighVolume);
	LogMessage (LogIfActive, _T(""));
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
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%d)] DaServer::Unload [%d]"), this, m_dwRef, dwCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::Register (IUnknown * punkNotifySink, long * pdwSinkID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Register"), this, m_dwRef);
#endif
	HRESULT	lResult = mNotify.Register (punkNotifySink, pdwSinkID);

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::Register [%d]"), this, m_dwRef, *pdwSinkID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::Unregister (long dwSinkID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::Unregister [%d]"), this, m_dwRef, dwSinkID);
#endif
	HRESULT	lResult;

	if	(mInNotify > 0)
	{
		mInNotifyUnregister.Add (dwSinkID);
		lResult = S_FALSE;
	}
	else
	{
		lResult = mNotify.Unregister (dwSinkID);
	}
	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::Unregister [%d]"), this, m_dwRef, dwSinkID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetCharacter (long dwCharID, IDispatch ** ppunkCharacter)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacter"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!ppunkCharacter)
	{
		lResult = E_POINTER;
	}
	else
	{
		IDaSvrCharacterPtr	lCharacter;

		lResult = GetCharacterEx (dwCharID, &lCharacter);
		(*ppunkCharacter) = lCharacter.Detach();
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacter"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterEx (long dwCharID, IDaSvrCharacter **ppCharacterEx)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterEx"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCharacter *	lCharacter;
	IDaSvrCharacterPtr	lSvrCharacter;

	if	(!ppCharacterEx)
	{
		lResult = E_POINTER;
	}
	else
	if	(lCharacter = mNotify._GetCharacter (dwCharID))
	{
		lSvrCharacter = lCharacter->GetControllingUnknown ();
		(*ppCharacterEx) = lSvrCharacter.Detach ();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterEx"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacter2 (long CharacterID, IDaSvrCharacter2 **Character2)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacter2"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacter2"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::ShowDefaultCharacterProperties (short x, short y, long bUseDefaultPosition)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::ShowDefaultCharacterProperties"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CPropSheetCharSel *	lPropSheet = NULL;

	if	(lPropSheet = _AtlModule.GetPropSheetCharSel (true, mClientMutexName))
	{
		if	(!lPropSheet->IsWindow ())
		{
			try
			{
				lPropSheet->Create (&_AtlModule);
			}
			catch AnyExceptionDebug
		}
		if	(lPropSheet->IsWindow ())
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
			::SetForegroundWindow (lPropSheet->m_hWnd);
		}
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::ShowDefaultCharacterProperties"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetVersion (short *psMajor, short *psMinor)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(psMajor)
	{
		(*psMajor) = (short)_SERVER_VER_MAJOR;
	}
	if	(psMinor)
	{
		(*psMinor) = (short)_SERVER_VER_MINOR;
	}

	PutServerError (lResult, __uuidof(IDaServer));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetVersion"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetSuspended (long * pbSuspended)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetSuspended"), this, m_dwRef);
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

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterFiles (IDaSvrCharacterFiles **CharacterFiles)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterFiles"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterFiles"), this, m_dwRef);
	}
#endif
	return lResult;
}
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::put_CharacterStyle (long CharacterStyle)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::put_CharacterStyle [%d]"), this, m_dwRef, CharacterStyle);
	}
#endif
	HRESULT	lResult = S_OK;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (0, _T("put_CharacterStyle"), _T("%u"), CharacterStyle);
#endif
	mCharacterStyle = CharacterStyle;

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::put_CharacterStyle"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::get_CharacterStyle (long *CharacterStyle)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::get_CharacterStyle"), this, m_dwRef);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(CharacterStyle)
	{
		(*CharacterStyle) = mCharacterStyle;
	}
	else
	{
		lResult = E_POINTER;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::get_CharacterStyle"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetSpeechEngines (IDaSvrSpeechEngines **SpeechEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetSpeechEngines"), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	DaSvrSpeechEngines *	lSpeechEngines;
	IDaSvrSpeechEnginesPtr	lInterface;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

		if	(lSpeechEngines = DaSvrSpeechEngines::CreateInstance (mClientMutexName))
		{
			lSpeechEngines->UseAllVoices ();
			lInterface = lSpeechEngines->GetControllingUnknown();
			(*SpeechEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetSpeechEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindSpeechEngines (long LanguageID, short Gender, IDaSvrSpeechEngines **SpeechEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindSpeechEngines"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

		lResult = DaSvrCharacter::FindSpeechEngines (NULL, (LANGID)LanguageID, Gender, SpeechEngines, mClientMutexName);
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindSpeechEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterSpeechEngine (VARIANT LoadKey, IDaSvrSpeechEngine **SpeechEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterSpeechEngine"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lFilePath;
	tPtr <CAgentFile>	lAgentFile;

	if	(!SpeechEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngine) = NULL;

		if	(SUCCEEDED (lResult = GetLoadPath (LoadKey, lFilePath)))
		{
			if	(lAgentFile = CAgentFile::CreateInstance())
			{
				if	(SUCCEEDED (lResult = lAgentFile->Open (lFilePath)))
				{
					lResult = DaSvrCharacter::GetDefaultSpeechEngine (lAgentFile, SpeechEngine, mClientMutexName);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterSpeechEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindCharacterSpeechEngines (VARIANT LoadKey, long LanguageID, IDaSvrSpeechEngines **SpeechEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindCharacterSpeechEngines"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lFilePath;
	tPtr <CAgentFile>	lAgentFile;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

		if	(SUCCEEDED (lResult = GetLoadPath (LoadKey, lFilePath)))
		{
			if	(lAgentFile = CAgentFile::CreateInstance())
			{
				if	(SUCCEEDED (lResult = lAgentFile->Open (lFilePath)))
				{
					lResult = DaSvrCharacter::FindSpeechEngines (lAgentFile, (LANGID)LanguageID, SpeechGender_Neutral, SpeechEngines, mClientMutexName);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindCharacterSpeechEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaServer::GetRecognitionEngines (IDaSvrRecognitionEngines **RecognitionEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetRecognitionEngines"), this, m_dwRef);
#endif
	HRESULT						lResult = S_OK;
	DaSvrRecognitionEngines *	lRecognitionEngines;
	IDaSvrRecognitionEnginesPtr	lInterface;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

		if	(lRecognitionEngines = DaSvrRecognitionEngines::CreateInstance (mClientMutexName))
		{
			lRecognitionEngines->UseAllInputs ();
			lInterface = lRecognitionEngines->GetControllingUnknown();
			(*RecognitionEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetRecognitionEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindRecognitionEngines (long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindRecognitionEngines"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

		lResult = DaSvrCharacter::FindRecognitionEngines (NULL, (LANGID)LanguageID, RecognitionEngines, mClientMutexName);
	}

	PutServerError (lResult, __uuidof(IDaServer2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindRecognitionEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::GetCharacterRecognitionEngine (VARIANT LoadKey, IDaSvrRecognitionEngine **RecognitionEngine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::GetCharacterRecognitionEngine"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lFilePath;
	tPtr <CAgentFile>	lAgentFile;

	if	(!RecognitionEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngine) = NULL;

		if	(SUCCEEDED (lResult = GetLoadPath (LoadKey, lFilePath)))
		{
			if	(lAgentFile = CAgentFile::CreateInstance())
			{
				if	(SUCCEEDED (lResult = lAgentFile->Open (lFilePath)))
				{
					lResult = DaSvrCharacter::GetDefaultRecognitionEngine (lAgentFile, RecognitionEngine, mClientMutexName);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::GetCharacterRecognitionEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaServer::FindCharacterRecognitionEngines (VARIANT LoadKey, long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaServer::FindCharacterRecognitionEngines"), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lFilePath;
	tPtr <CAgentFile>	lAgentFile;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

		if	(SUCCEEDED (lResult = GetLoadPath (LoadKey, lFilePath)))
		{
			if	(lAgentFile = CAgentFile::CreateInstance())
			{
				if	(SUCCEEDED (lResult = lAgentFile->Open (lFilePath)))
				{
					lResult = DaSvrCharacter::FindRecognitionEngines (lAgentFile, (LANGID)LanguageID, RecognitionEngines, mClientMutexName);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaServer::FindCharacterRecognitionEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}
