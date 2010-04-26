/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControlMod.h"
#include "DaControl.h"
#include "DaCtlCharacters.h"
#include "DaCtlCharacter.h"
#include "DaCtlCommands.h"
#include "DaCtlSettings.h"
#include "DaCtlAudioOutput.h"
#include "DaCtlSpeechInput.h"
#include "DaCtlCommandsWindow.h"
#include "DaCtlPropertySheet.h"
#include "DaCtlUserInput.h"
#include "DaCtlCharacterFiles.h"
#include "DaCtlTTSEngine.h"
#include "DaCtlTTSEngines.h"
#include "DaCtlSREngine.h"
#include "DaCtlSREngines.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "ErrorInfo.h"
#include "GuidStr.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Control"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_REQUEST			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_ATTRIBUTES		(GetProfileDebugInt(_T("DebugAttributes"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Control"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

UINT	DaControl::mCompleteRequestsMsg = RegisterWindowMessage (_T("1147E530-A208-11DE-ABF2-002421116FB2"));

/////////////////////////////////////////////////////////////////////////////

DaControl::DaControl()
:	mRaiseRequestErrors (true),
	mFinalReleased (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::DaControl"), this, m_dwRef);
	}
#endif

	_AtlModule.OnControlCreated (this);
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
	m_bWndLess = FALSE;
	m_bWindowOnly = TRUE;
	m_bRecomposeOnResize = TRUE;
	m_bResizeNatural = TRUE;
	m_bDrawFromNatural = TRUE;
	m_bDrawGetDataInHimetric = FALSE;

	SIZEL	lDefaultSize = {GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON)};
	AtlPixelToHiMetric (&lDefaultSize, &m_sizeExtent);
	m_sizeNatural = m_sizeExtent;

	m_clrBackColor = (OLE_COLOR)(0x80000000|COLOR_WINDOW);
	m_clrBorderColor = (OLE_COLOR)(0x80000000|COLOR_WINDOWTEXT);
	m_nBorderStyle = 1;
	m_bBorderVisible = TRUE;
	m_nBorderWidth = 1;
	m_nMousePointer = 0;
}

DaControl::~DaControl()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::~DaControl"), this, m_dwRef);
	}
#endif
	mFinalReleased = true;
	Terminate (true);
	mFinalReleased = false;
	_AtlModule.OnControlDeleted (this);
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DaControl::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::FinalRelease [%u]"), this, m_dwRef, mFinalReleased);
	}
#endif
	if	(!mFinalReleased)
	{
		mFinalReleased = true;
		Terminate (false);
	}
}

void DaControl::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServer.GetInterfacePtr());
		}
#endif
		try
		{
			DaCtlCharacters *		lCharacters;
			DaCtlSettings *			lSettings;
			DaCtlAudioOutput *		lAudioOutput;
			DaCtlSpeechInput *		lSpeechInput;
			DaCtlCommandsWindow *	lCommandsWindow;
			DaCtlPropertySheet *	lPropertySheet;
			DaCtlCharacterFiles *	lCharacterFiles;

			mServerNotifySink->Terminate ();
			SafeFreeSafePtr (mServerNotifySink);

			if	(
					(mCharacters != NULL)
				&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
				)
			{
				lCharacters->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mCharacters = NULL;
			}

			if	(
					(mSettings != NULL)
				&&	(lSettings = dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()))
				)
			{
				lSettings->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mSettings = NULL;
			}

			if	(
					(mAudioOutput != NULL)
				&&	(lAudioOutput = dynamic_cast <DaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()))
				)
			{
				lAudioOutput->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mAudioOutput = NULL;
			}

			if	(
					(mCharacterFiles != NULL)
				&&	(lCharacterFiles = dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()))
				)
			{
				lCharacterFiles->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mCharacterFiles = NULL;
			}

			if	(
					(mSpeechInput != NULL)
				&&	(lSpeechInput = dynamic_cast <DaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()))
				)
			{
				lSpeechInput->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mSpeechInput = NULL;
			}

			if	(
					(mCommandsWindow != NULL)
				&&	(lCommandsWindow = dynamic_cast <DaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()))
				)
			{
				lCommandsWindow->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mCommandsWindow = NULL;
			}

			if	(
					(mPropertySheet != NULL)
				&&	(lPropertySheet = dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()))
				)
			{
				lPropertySheet->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mPropertySheet = NULL;
			}

			TerminateRequests (pFinal);
			DisconnectServer (pFinal);

			if	(IsWindow ())
			{
				DestroyWindow ();
			}
		}
		catch AnyExceptionDebug
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

enum COMPAT
{
	COMPAT_AGGREGATE = 0x00000001,
	COMPAT_NO_OBJECTSAFETY = 0x00000002,
	COMPAT_NO_PROPNOTIFYSINK = 0x00000004,
	COMPAT_SEND_SHOW = 0x00000008,
	COMPAT_SEND_HIDE = 0x00000010,
	COMPAT_ALWAYS_INPLACEACTIVATE = 0x00000020,
	COMPAT_NO_SETEXTENT = 0x00000040,
	COMPAT_NO_UIACTIVATE = 0x00000080,
	COMPAT_NO_QUICKACTIVATE = 0x00000100,
	COMPAT_NO_BINDF_OFFLINEOPERATION = 0x00000200,
	COMPAT_EVIL_DONT_LOAD = 0x00000400,
	COMPAT_PROGSINK_UNTIL_ACTIVATED = 0x00000800,
	COMPAT_USE_PROPBAG_AND_STREAM = 0x00001000,
	COMPAT_DISABLEWINDOWLESS = 0x00002000,
	COMPAT_SETWINDOWRGN = 0x00004000,
	COMPAT_PRINTPLUGINSITE = 0x00008000,
	COMPAT_INPLACEACTIVATEEVENWHENINVISIBLE = 0x00010000,
	COMPAT_NEVERFOCUSSABLE = 0x00020000,
	COMPAT_ALWAYSDEFERSETWINDOWRGN = 0x00040000,
	COMPAT_INPLACEACTIVATESYNCHRONOUSLY = 0x00080000,
	COMPAT_NEEDSZEROBASEDDRAWRECT = 0x00100000,
	COMPAT_HWNDPRIVATE = 0x00200000,
	COMPAT_SECURITYCHECKONREDIRECT = 0x00400000,
	COMPAT_SAFEFOR_LOADING = 0x00800000
};

static const DWORD sControlCompat = COMPAT_NO_PROPNOTIFYSINK|COMPAT_NO_SETEXTENT|COMPAT_NO_UIACTIVATE|COMPAT_NO_QUICKACTIVATE|COMPAT_NEVERFOCUSSABLE|COMPAT_HWNDPRIVATE;

/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI DaControl::UpdateRegistryOverride (BOOL bRegister)
{
	HRESULT		lResult = UpdateRegistry (bRegister);
	CAtlString	lClassIdStr = CGuidStr (__uuidof(DaControl));

	if	(SUCCEEDED (lResult))
	{
		CRegKeyEx	lExtRoot (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Ext"), true);
		CRegKeyEx	lPreApproved (lExtRoot, _T("PreApproved"), true);
		CRegKeyEx	lCompatRoot (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility"), true);

		if	(bRegister)
		{
			CRegKeyEx (lPreApproved, lClassIdStr, false, true);
			CRegDWord (CRegKeyEx (lCompatRoot, lClassIdStr, false, true), _T("Compatibility Flags"), true).SetValue(sControlCompat).Update();
		}
		else
		{
			CRegKeyEx (lPreApproved, lClassIdStr, false).Delete ();
			CRegKeyEx (lCompatRoot, lClassIdStr, false).Delete ();
		}
	}

	if	(SUCCEEDED (lResult))
	{
		if	(bRegister)
		{
			if	(
					(IsWindows7_AtLeast())
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
				)
			{
				CRegKeyEx	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentControl)), false, true);
				CRegKeyEx	lControlProgId (HKEY_CLASSES_ROOT, _T(_AGENT_CONTROL_PROGID), false, true);
				CRegKeyEx	lControlProgId2 (HKEY_CLASSES_ROOT,_T( _AGENT_CONTROL_PROGID2), false, true);
				CAtlString	lControlPath;

				GetModuleFileName (_AtlBaseModule.GetResourceInstance(), lControlPath.GetBuffer(MAX_PATH), MAX_PATH);
				PathRemoveFileSpec (lControlPath.GetBuffer(MAX_PATH));
				PathAppend (lControlPath.GetBuffer(MAX_PATH), _T("AgentCtl.dll"));
				lControlPath.ReleaseBuffer ();

				CRegString (lMsAgentClass, (LPCTSTR)NULL, true).Update (_T("Microsoft Agent Control 2.0"));
				CRegString (lMsAgentClass, _T("Version"), true).Update (_T("2.0"));
				CRegString (CRegKeyEx (lMsAgentClass, _T("ProgID"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID2));
				CRegString (CRegKeyEx (lMsAgentClass, _T("InprocServer32"), false, true), (LPCTSTR)NULL, true).Update (lControlPath);
				CRegString (CRegKeyEx (lMsAgentClass, _T("MiscStatus"), false, true), (LPCTSTR)NULL, true).Update (_T("0"));
				CRegString (CRegKeyEx (CRegKeyEx (lMsAgentClass, _T("MiscStatus"), false, true), _T("1"), false, true), (LPCTSTR)NULL, true).Update (_T("148628"));
				AtlRegisterClassCategoriesHelper (__uuidof(AgentControl), GetCategoryMap(), TRUE);

				CRegString (lControlProgId, (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID_NAME));
				CRegString (CRegKeyEx (lControlProgId, _T("CurVer"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID2));
				CRegString (lControlProgId2, (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID_NAME));
				CRegString (CRegKeyEx (lControlProgId2, _T("CLSID"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(AgentControl)));

				CRegString (CRegKeyEx (lMsAgentClass, _T("AutoTreatAs"), false, true), (LPCTSTR)NULL, true).Update (lClassIdStr);
				CRegString (CRegKeyEx (lMsAgentClass, _T("TreatAs"), false, true), (LPCTSTR)NULL, true).Update (lClassIdStr);
			}
		}
		else
		{
			CRegKeyEx	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentControl)), false);

#if	FALSE
			if	(
					(IsWindows7_AtLeast())
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
				)
			{
				lMsAgentClass.Delete ();

				CRegKeyEx (HKEY_CLASSES_ROOT, _T(_AGENT_CONTROL_PROGID), false).Delete ();
				CRegKeyEx (HKEY_CLASSES_ROOT, _T(_AGENT_CONTROL_PROGID2), false).Delete ();
			}
			else
#endif
			{
				CRegKeyEx	lAutoTreatAs (lMsAgentClass, _T("AutoTreatAs"), false);
				CRegKeyEx	lTreatAs (lMsAgentClass, _T("TreatAs"), false);

				if	(
						(lAutoTreatAs.IsValid())
					&&	(lAutoTreatAs.Value().Value().CompareNoCase (lClassIdStr) == 0)
					)
				{
					lAutoTreatAs.Delete ();
				}
				if	(
						(lTreatAs.IsValid())
					&&	(lTreatAs.Value().Value().CompareNoCase (lClassIdStr) == 0)
					)
				{
					lTreatAs.Delete ();
				}
			}
		}
	}

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::OnDrawAdvanced(ATL_DRAWINFO& di)
{
	CRect	lBounds = *(RECT*)di.prcBounds;

	//LogMessage (LogDebugFast, _T("OnDrawAdvanced [%s] Extent [%s] Natural [%s]"), FormatRect(lBounds), FormatSize(m_sizeExtent), FormatSize(m_sizeNatural));

	if	(!mIcon)
	{
		mIcon = (HICON) LoadImage (_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDI_DOUBLEAGENT), IMAGE_ICON, lBounds.Width(), lBounds.Height(), LR_DEFAULTCOLOR);
	}
	::DrawIconEx (di.hdcDraw, lBounds.left, lBounds.top, mIcon, lBounds.Width(), lBounds.Height(), 0, (HBRUSH)GetStockObject(LTGRAY_BRUSH), DI_NORMAL);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::ConnectServer ()
{
	HRESULT	lResult = S_FALSE;

	if	(mServer == NULL)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::ConnectServer"), this, m_dwRef);
		}
#endif
		mServerNotifySink->Terminate ();
		SafeFreeSafePtr (mServerNotifySink);

		lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server"));

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = CComObject <CServerNotifySink>::CreateInstance (mServerNotifySink.Free())))
			)
		{
			lResult = mServerNotifySink->Initialize (this);
		}

//		if	(
//				(SUCCEEDED (lResult))
//			&&	(m_hWnd == NULL)
//			)
//		{
//			SubclassWindow (::CreateWindow (AfxRegisterWndClass(0), NULL, WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, NULL, _AtlBaseModule.GetResourceInstance(), NULL));
//		}
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::ConnectServer [%p] Sink [%d]"), this, m_dwRef, mServer.GetInterfacePtr(), (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
		}
#endif
	}
	if	(FAILED (lResult))
	{
		lResult = AGENTCTLERROR_SERVERINIT;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::DisconnectServer (bool pForce)
{
	HRESULT	lResult = (mServer != NULL) ? S_OK : S_FALSE;

#ifdef	_LOG_INSTANCE
	if	(
			(mServer != NULL)
		&&	(LogIsActive())
		)
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::DisconnectServer [%p(%u)] Force [%u] Sink [%u]"), this, m_dwRef, mServer.GetInterfacePtr(), CoIsHandlerConnected(mServer), pForce, (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
	}
#endif
	if	(mServerNotifySink)
	{
		if	(pForce)
		{
			mServerNotifySink->mServerNotifyId = 0;
		}
		else
		{
			mServerNotifySink->Terminate ();
		}
		SafeFreeSafePtr (mServerNotifySink);
	}
	if	(pForce)
	{
		mServer.Detach ();
	}
	else
	{
		SafeFreeSafePtr (mServer);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void DaControl::DisconnectNotify (bool pForce)
{
	if	(this)
	{
		try
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive())
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::DisconnectNotify [%u] [%p]"), this, m_dwRef, pForce, mServer.GetInterfacePtr());
			}
#endif
			if	(mServerNotifySink)
			{
				if	(pForce)
				{
					mServerNotifySink->mServerNotifyId = 0;
				}
				else
				{
					mServerNotifySink->Terminate ();
				}
				SafeFreeSafePtr (mServerNotifySink);
			}
		}
		catch AnyExceptionDebug

#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive(_LOG_INSTANCE))
		{
			int			lNdx;
			LPUNKNOWN	lConnection;

			try
			{
				if	(CProxy_DaCtlEvents<DaControl>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- _DaCtlEvents Connections [%d] ---"), CProxy_DaCtlEvents<DaControl>::m_vec.GetSize());
				}
				for	(lNdx = 0; lNdx < CProxy_DaCtlEvents<DaControl>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = CProxy_DaCtlEvents<DaControl>::m_vec.GetAt (lNdx))
					{
						LogMessage (_LOG_INSTANCE, _T("--- _DaCtlEvents Connection [%d] [%p] ---"), lNdx, lConnection);
					}
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(CProxy_AgentEvents<DaControl>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- _AgentEvents Connections [%d] ---"), CProxy_AgentEvents<DaControl>::m_vec.GetSize());
				}
				for	(lNdx = 0; lNdx < CProxy_AgentEvents<DaControl>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = CProxy_AgentEvents<DaControl>::m_vec.GetAt (lNdx))
					{
						LogMessage (_LOG_INSTANCE, _T("--- _AgentEvents Connection [%d] [%p] ---"), lNdx, lConnection);
					}
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(IPropertyNotifySinkCP<DaControl>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- IPropertyNotifySink Connections [%d] ---"), IPropertyNotifySinkCP<DaControl>::m_vec.GetSize());
				}
				for	(lNdx = 0; lNdx < IPropertyNotifySinkCP<DaControl>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = IPropertyNotifySinkCP<DaControl>::m_vec.GetAt (lNdx))
					{
						LogMessage (_LOG_INSTANCE, _T("--- IPropertyNotifySink Connection [%d] [%p] ---"), lNdx, lConnection);
					}
				}
			}
			catch AnyExceptionDebug
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IDaCtlRequest * DaControl::PutRequest (DaRequestCategory pCategory, long pReqID, HRESULT pResult)
{
	IDaCtlRequestPtr				lInterface;
	DaCtlRequest *					lRequest = NULL;
	CComObject <DaCtlRequest> *	lNewRequest = NULL;

	if	(pReqID)
	{
		if	(mActiveRequests.Lookup (pReqID, lRequest))
		{
			if	((pCategory & DaRequestCategoryMask) == 0)
			{
				lRequest->mCategory = (DaRequestCategory)(lRequest->mCategory | pCategory);
			}
			else
			{
				//
				//	This means that we have already gotten (and ignored) the RequestStart and possibly RequestComplete
				//
				lRequest->mCategory = pCategory;
			}
#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("  Reuse Request   [%p(%d)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
		}

		if	(
				(!lRequest)
			&&	(SUCCEEDED (LogComErr (LogNormal, CComObject <DaCtlRequest>::CreateInstance (&lNewRequest))))
			)
		{
			lNewRequest->SetOwner (this, pCategory, pReqID, pResult);
			lRequest = lNewRequest;
		}
		if	(lRequest)
		{
			if	(
					((pCategory & DaRequestCategoryMask) != 0)
				||	((pCategory & DaRequestNotifyComplete) != 0)
				)
			{
				lRequest->mResult = pResult;
			}
			if	(FAILED (lRequest->mResult))
			{
				lRequest->mStatus = IS_INTERRUPT_ERROR (lRequest->mResult) ? RequestStatus_Interrupted : RequestStatus_Failed;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyComplete) != 0)
			{
				lRequest->mStatus = RequestStatus_Success;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyStart) != 0)
			{
				lRequest->mStatus = RequestStatus_InProgress;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
		}

		if	(
				(lRequest)
			&&	((lRequest->mCategory & DaRequestCategoryMask) != 0)
			&&	(
					((pCategory & DaRequestCategoryMask) != 0)
				||	((lRequest->mCategory & DaRequestNotifyEnabled) != 0)
				)
			)
		{
			//
			//	We don't use any request created for RequestStart or RequestComplete immediately.
			//	Instead, we leave them to be processed by CompleteRequests
			//
			lInterface = (LPDISPATCH) lRequest;
		}
#ifdef	_DEBUG_REQUEST
		else
		if	(lRequest)
		{
			LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)] [%d] Status [%s] deferred [%p]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), m_hWnd);
		}
#endif

		if	(
				(mMsgPostingWnd)
			||	(mMsgPostingWnd = new CMsgPostingWnd <DaControl> (*this))
			)
		{
			//
			//	Trigger a call to CompleteRequests next time the message queue is cycled
			//
			mMsgPostingWnd->PostMessage (mCompleteRequestsMsg);
		}
	}
	return lInterface.Detach ();
}

/////////////////////////////////////////////////////////////////////////////

LRESULT DaControl::OnCompleteRequests (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CompleteRequests (true);
	bHandled = TRUE;
	return 0;
}

void DaControl::CompleteRequests (bool pIdleTime)
{
//
//	By the time we get here, any function that created a request has completed.  Either we're
//	in the next function that creates a request, or we're in idle time on the message queue.
//
	try
	{
		MSG	lMsg;

		while (PeekMessage (&lMsg, m_hWnd, mCompleteRequestsMsg, mCompleteRequestsMsg, PM_REMOVE))
		{;}
	}
	catch AnyExceptionSilent

	if	(!mActiveRequests.IsEmpty ())
	{
		try
		{
			POSITION							lPos;
			INT_PTR								lNdx;
			long								lReqID;
			DaCtlRequest *						lRequest;
			CAtlPtrTypeArray <DaCtlRequest>	lActiveRequests;

#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("[%p(%d)] Complete Requests [%d] Idle [%u]"), this, m_dwRef, mActiveRequests.GetCount(), pIdleTime);
#endif
			for	(lPos = mActiveRequests.GetStartPosition(); lPos;)
			{
				mActiveRequests.GetNextAssoc (lPos, lReqID, lRequest);
				lActiveRequests.Add (lRequest);
			}
			for	(lNdx = lActiveRequests.GetCount()-1; lRequest = lActiveRequests (lNdx); lNdx--)
			{
				if	(
						((lRequest->mCategory & DaRequestCategoryMask) == 0)
					||	((lRequest->mCategory & DaRequestNotifyMask) == DaRequestNotifyMask)
					||	(lRequest->m_dwRef <= 1)
					)
				{
					//
					//	If the request was created for a notification (RequestStart or RequestComplete), then it means
					//	that the function that initiated the request is done, and the request it created is already
					//	gone.
					//
					//	If the request already has DaRequestNotifyMask set, then all notifications are complete.
					//
					//	If the request's reference count is 1, then the function that initiated the request
					//	is complete, but the application did not use the returned request object.
					//
#ifdef	_DEBUG_REQUEST
					LogMessage (_DEBUG_REQUEST, _T("  Release Request [%p(%d)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
					mActiveRequests.RemoveKey (lRequest->mReqID);
					mCompletedRequests.Add (lRequest);
					lRequest->Release ();
				}
				else
				{
					lRequest->mCategory = (DaRequestCategory)(lRequest->mCategory | DaRequestNotifyEnabled);

					if	(
							(lRequest->mStatus == RequestStatus_InProgress)
						||	(lRequest->mStatus == RequestStatus_Success)
						||	(lRequest->mStatus == RequestStatus_Failed)
						||	(lRequest->mStatus == RequestStatus_Interrupted)
						)
					{
						//
						//	This means that the function that initiated the request did so after the RequestStart
						//	notification was received.
						//
						if	((lRequest->mCategory & DaRequestNotifyStart) == 0)
						{
							lRequest->mCategory = (DaRequestCategory)(lRequest->mCategory | DaRequestNotifyStart);

#ifdef	_DEBUG_REQUEST
							LogMessage (_DEBUG_REQUEST, _T("  Queued Request [%p(%d)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
#ifdef	_DEBUG_NOTIFY
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CompleteRequests::RequestStart [%d]"), this, m_dwRef, lRequest->mReqID);
#endif
							if	(_AtlModule.PreNotify ())
							{
								IDaCtlRequestPtr	lInterface ((LPDISPATCH) lRequest);
								try
								{
									FireRequestStart (lInterface);
								}
								catch AnyExceptionDebug
								_AtlModule.PostNotify ();
							}
						}
					}

					if	(
							(lRequest->mStatus == RequestStatus_Success)
						||	(lRequest->mStatus == RequestStatus_Failed)
						||	(lRequest->mStatus == RequestStatus_Interrupted)
						)
					{
						//
						//	This means that the function that initiated the request did so after the RequestComplete
						//	notification was received.
						//
						if	((lRequest->mCategory & DaRequestNotifyComplete) == 0)
						{
							lRequest->mCategory = (DaRequestCategory)(lRequest->mCategory | DaRequestNotifyComplete);

#ifdef	_DEBUG_REQUEST
							LogMessage (_DEBUG_REQUEST, _T("  Queued Request [%p(%d)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
#ifdef	_DEBUG_NOTIFY
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CompleteRequests::RequestComplete [%d]"), this, m_dwRef, lRequest->mReqID);
#endif
							if	(_AtlModule.PreNotify ())
							{
								IDaCtlRequestPtr	lInterface ((LPDISPATCH) lRequest);
								try
								{
									FireRequestComplete (lInterface);
								}
								catch AnyExceptionDebug
								_AtlModule.PostNotify ();
							}
						}
#ifdef	_DEBUG_REQUEST
						LogMessage (_DEBUG_REQUEST, _T("  Release Request [%p(%d)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
						mActiveRequests.RemoveKey (lRequest->mReqID);
						mCompletedRequests.Add (lRequest);
						lRequest->Release ();
					}
#ifdef	_DEBUG_REQUEST
					else
					if	(lRequest->mStatus == RequestStatus_Pending)
					{
						LogMessage (_DEBUG_REQUEST, _T("  Pending Request [%p(%d)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
					}
#endif
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void DaControl::TerminateRequests (bool pFinal)
{
	if	(
			(!mActiveRequests.IsEmpty ())
		||	(mCompletedRequests.GetCount() > 0)
		)
	{
		try
		{
			POSITION							lPos;
			INT_PTR								lNdx;
			long								lReqID;
			DaCtlRequest *						lRequest;
			CAtlPtrTypeArray <DaCtlRequest>	lActiveRequests;

#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("[%p(%d)] Terminate [%u] Requests [%d %d]"), this, m_dwRef, pFinal, mActiveRequests.GetCount(), mCompletedRequests.GetCount());
#endif
			for	(lPos = mActiveRequests.GetStartPosition(); lPos;)
			{
				mActiveRequests.GetNextAssoc (lPos, lReqID, lRequest);
				lActiveRequests.Add (lRequest);
			}
			mActiveRequests.RemoveAll ();
			mCompletedRequests.Append (lActiveRequests);

			for	(lNdx = lActiveRequests.GetCount()-1; lRequest = lActiveRequests (lNdx); lNdx--)
			{
				lRequest->Release ();
			}
			for	(lNdx = mCompletedRequests.GetCount()-1; lRequest = mCompletedRequests (lNdx); lNdx--)
			{
				lRequest->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mCompletedRequests.RemoveAll ();
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void DaControl::RequestCreated (DaCtlRequest * pRequest)
{
	if	(
			(pRequest)
		&&	(pRequest->mReqID > 0)
		)
	{
		pRequest->AddRef ();
		mActiveRequests.SetAt (pRequest->mReqID, pRequest);
#ifdef	_DEBUG_REQUEST
		LogMessage (_DEBUG_REQUEST, _T("  New Request     [%p(%d)] [%d] Status [%s] Category [%s] Count [%d]"), pRequest, pRequest->m_dwRef, pRequest->mReqID, pRequest->StatusStr(), pRequest->CategoryStr(), mActiveRequests.GetCount());
#endif
	}
}

void DaControl::RequestDeleted (DaCtlRequest * pRequest)
{
	if	(
			(pRequest)
		&&	(pRequest->mReqID > 0)
		)
	{
		mActiveRequests.RemoveKey (pRequest->mReqID);
		mCompletedRequests.Remove (pRequest);
#ifdef	_DEBUG_REQUEST
		LogMessage (_DEBUG_REQUEST, _T("  Deleted Request [%p(%d)] [%d] Status [%s] Category [%s] Count [%d]"), pRequest, pRequest->m_dwRef, pRequest->mReqID, pRequest->StatusStr(), pRequest->CategoryStr(), mActiveRequests.GetCount());
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAtlString DaControl::GetControlCharacterID (long pServerCharID)
{
	DaCtlCharacters *	lCharacters;
	CAtlString			lCharacterID;
	POSITION			lPos;
	DaCtlCharacter *	lCharacter;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
		)
	{
		for	(lPos = lCharacters->mCharacters.GetStartPosition(); lPos;)
		{
			if	(
					(lCharacter = dynamic_cast <DaCtlCharacter *> (lCharacters->mCharacters.GetValueAt (lPos).GetInterfacePtr()))
				&&	(lCharacter->mServerCharID == pServerCharID)
				)
			{
				lCharacterID = lCharacters->mCharacters.GetKeyAt (lPos);
				break;
			}
			lCharacters->mCharacters.GetNext (lPos);
		}
	}
	return lCharacterID;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString DaControl::GetActiveCharacterID ()
{
	DaCtlCharacters *	lCharacters;
	CAtlString			lCharacterID;
	POSITION			lPos;
	DaCtlCharacter *	lCharacter;
	ActiveStateType		lCharacterState;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
		)
	{
		for	(lPos = lCharacters->mCharacters.GetStartPosition(); lPos;)
		{
			if	(
					(lCharacter = dynamic_cast <DaCtlCharacter *> (lCharacters->mCharacters.GetValueAt (lPos).GetInterfacePtr()))
				&&	(lCharacter->mServerObject != NULL)
				&&	(SUCCEEDED (lCharacter->mServerObject->get_ActiveState (&(lCharacterState=(ActiveStateType)-1))))
				&&	(lCharacterState >= ActiveState_Active)
				)
			{
				lCharacterID = lCharacters->mCharacters.GetKeyAt (lPos);
				break;
			}
			lCharacters->mCharacters.GetNext (lPos);
		}
	}
	return lCharacterID;
}

DaCtlCharacter * DaControl::GetActiveCharacter ()
{
	DaCtlCharacters *	lCharacters;
	POSITION			lPos;
	DaCtlCharacter *	lCharacter;
	ActiveStateType		lCharacterState;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
		)
	{
		for	(lPos = lCharacters->mCharacters.GetStartPosition(); lPos;)
		{
			if	(
					(lCharacter = dynamic_cast <DaCtlCharacter *> (lCharacters->mCharacters.GetValueAt (lPos).GetInterfacePtr()))
				&&	(lCharacter->mServerObject != NULL)
				&&	(SUCCEEDED (lCharacter->mServerObject->get_ActiveState (&(lCharacterState=(ActiveStateType)-1))))
				&&	(lCharacterState >= ActiveState_Active)
				)
			{
				return lCharacter;
			}
			lCharacters->mCharacters.GetNext (lPos);
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_Characters (IDaCtlCharacters2 **Characters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Characters"), this, m_dwRef);
#endif
	HRESULT							lResult = ConnectServer ();
	CComObject <DaCtlCharacters> *	lCharacters = NULL;
	IDaCtlCharacters2Ptr			lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(Characters == NULL)
			{
				lResult = E_POINTER;
			}
			else
			{
				if	(mCharacters == NULL)
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlCharacters>::CreateInstance (&lCharacters)))
					{
						lCharacters->SetOwner (this);
						mCharacters = (LPDISPATCH)lCharacters;
						lInterface = mCharacters;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = mCharacters;
				}

				(*Characters) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Characters [%p]"), this, m_dwRef, dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Characters"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_Settings (IDaCtlSettings **Settings)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Settings"), this, m_dwRef);
#endif
	HRESULT							lResult = ConnectServer ();
	CComObject <DaCtlSettings> *	lSettings = NULL;
	IDaCtlSettingsPtr				lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(Settings == NULL)
			{
				lResult = E_POINTER;
			}
			else
			{
				if	(mSettings == NULL)
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSettings>::CreateInstance (&lSettings)))
					{
						lSettings->SetOwner (this);
						mSettings = (LPDISPATCH)lSettings;
						lInterface = mSettings;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = mSettings;
				}

				(*Settings) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Settings [%p]"), this, m_dwRef, dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Settings"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_AudioOutput (IDaCtlAudioOutput **AudioOutput)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_AudioOutput"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <DaCtlAudioOutput> *	lAudioOutput = NULL;
	IDaCtlAudioOutputPtr				lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(AudioOutput == NULL)
			{
				lResult = E_POINTER;
			}
			else
			{
				if	(mAudioOutput == NULL)
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlAudioOutput>::CreateInstance (&lAudioOutput)))
					{
						lAudioOutput->SetOwner (this);
						mAudioOutput = (LPDISPATCH)lAudioOutput;
						lInterface = mAudioOutput;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = mAudioOutput;
				}

				(*AudioOutput) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_AudioOutput [%p]"), this, m_dwRef, dynamic_cast <DaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_AudioOutput"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_SpeechInput (IDaCtlSpeechInput **SpeechInput)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_SpeechInput"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <DaCtlSpeechInput> *	lSpeechInput = NULL;
	IDaCtlSpeechInputPtr				lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(SpeechInput == NULL)
			{
				lResult = E_POINTER;
			}
			else
			{
				if	(mSpeechInput == NULL)
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSpeechInput>::CreateInstance (&lSpeechInput)))
					{
						lSpeechInput->SetOwner (this);
						mSpeechInput = (LPDISPATCH) lSpeechInput;
						lInterface = mSpeechInput;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = mSpeechInput;
				}

				(*SpeechInput) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_SpeechInput [%p]"), this, m_dwRef, dynamic_cast <DaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_SpeechInput"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_PropertySheet (IDaCtlPropertySheet2 **PropSheet)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_PropertySheet"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <DaCtlPropertySheet> *	lPropertySheet = NULL;
	IDaCtlPropertySheet2Ptr				lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(PropSheet == NULL)
			{
				lResult = E_POINTER;
			}
			else
			{
				if	(mPropertySheet == NULL)
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlPropertySheet>::CreateInstance (&lPropertySheet)))
					{
						lPropertySheet->SetOwner (this);
						mPropertySheet = (LPDISPATCH) lPropertySheet;
						lInterface = mPropertySheet;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = mPropertySheet;
				}

				(*PropSheet) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_PropertySheet [%p]"), this, m_dwRef, dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_PropertySheet"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_CommandsWindow (IDaCtlCommandsWindow **CommandsWindow)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CommandsWindow"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <DaCtlCommandsWindow> *	lCommandsWindow = NULL;
	IDaCtlCommandsWindowPtr				lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(CommandsWindow == NULL)
			{
				lResult = E_POINTER;
			}
			else
			{
				if	(mCommandsWindow == NULL)
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlCommandsWindow>::CreateInstance (&lCommandsWindow)))
					{
						lCommandsWindow->SetOwner (this);
						mCommandsWindow = (LPDISPATCH) lCommandsWindow;
						lInterface = mCommandsWindow;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = mCommandsWindow;
				}

				(*CommandsWindow) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CommandsWindow [%p]"), this, m_dwRef, dynamic_cast <DaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_CommandsWindow"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_Connected (VARIANT_BOOL *Connected)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Connected"), this, m_dwRef);
#endif
	HRESULT	lResult = (mServer == NULL) ? S_FALSE : S_OK;

	if	(Connected)
	{
		(*Connected) = (mServer == NULL) ? VARIANT_FALSE : VARIANT_TRUE;
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Connected"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_Connected (VARIANT_BOOL Connected)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::put_Connected"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(Connected)
	{
		lResult = ConnectServer ();
	}
	else
	{
		Terminate (false);
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_Connected"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_Suspended (VARIANT_BOOL *Suspended)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Suspended"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(Suspended)
	{
		(*Suspended) = VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Suspended"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_RaiseRequestErrors (VARIANT_BOOL *RaiseErrors)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::get_RaiseRequestErrors"), this, m_dwRef);
#endif
	HRESULT	lResult = mRaiseRequestErrors ? S_OK : S_FALSE;

	if	(RaiseErrors)
	{
		(*RaiseErrors) = mRaiseRequestErrors ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_RaiseRequestErrors"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_RaiseRequestErrors (VARIANT_BOOL RaiseErrors)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::put_RaiseRequestErrors"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	mRaiseRequestErrors = (RaiseErrors!=VARIANT_FALSE);

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_RaiseRequestErrors"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::ShowDefaultCharacterProperties (VARIANT x, VARIANT y)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::ShowDefaultCharacterProperties"), this, m_dwRef);
#endif
	HRESULT	lResult = ConnectServer ();

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			_variant_t	lXPos (x);
			_variant_t	lYPos (y);

			try
			{
				lXPos.ChangeType (VT_I2);
				lYPos.ChangeType (VT_I2);
			}
			catch AnyExceptionSilent

			AllowSetForegroundWindow (ASFW_ANY);
			if	(
					(lXPos.vt == VT_I2)
				&&	(lXPos.vt == VT_I2)
				)
			{
				lResult = mServer->ShowDefaultCharacterProperties ((short)lXPos, (short)lYPos, FALSE);
			}
			else
			{
				lResult = mServer->ShowDefaultCharacterProperties (0, 0, TRUE);
			}
			AllowSetForegroundWindow (GetCurrentProcessId());
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::ShowDefaultCharacterProperties"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_CharacterFiles (IDaCtlCharacterFiles **CharacterFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CharacterFiles"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <DaCtlCharacterFiles> *	lCharacterFiles = NULL;
	IDaCtlCharacterFilesPtr				lInterface;

	if	(CharacterFiles == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(mCharacterFiles == NULL)
			{
				if	(SUCCEEDED (lResult = CComObject <DaCtlCharacterFiles>::CreateInstance (&lCharacterFiles)))
				{
					lCharacterFiles->SetOwner (this);
					mCharacterFiles = (LPDISPATCH) lCharacterFiles;
					lInterface = mCharacterFiles;
				}
				else
				{
					lResult = E_OUTOFMEMORY;
				}
			}
			else
			{
				lInterface = mCharacterFiles;
			}

			(*CharacterFiles) = lInterface.Detach();
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CharacterFiles [%p]"), this, m_dwRef, dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_CharacterFiles"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_CharacterStyle (long *CharacterStyle)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::get_CharacterStyle"), this, m_dwRef);
#endif
	HRESULT	lResult = ConnectServer ();

	if	(!CharacterStyle)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			lResult = mServer->get_CharacterStyle (CharacterStyle);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_CharacterStyle"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_CharacterStyle (long CharacterStyle)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::put_CharacterStyle"), this, m_dwRef);
#endif
	HRESULT	lResult = ConnectServer ();

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
	{
		try
		{
			lResult = mServer->put_CharacterStyle (CharacterStyle);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServer);
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_CharacterStyle"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_TTSEngines (IDaCtlTTSEngines **TTSEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_TTSEngines"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	IDaSvrTTSEnginesPtr				lServerObject;
	CComObject <DaCtlTTSEngines> *	lObject = NULL;
	IDaCtlTTSEnginesPtr				lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->get_TTSEngines (&lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*TTSEngines) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_TTSEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindTTSEngines (VARIANT LanguageID, VARIANT Gender, IDaCtlTTSEngines **TTSEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindTTSEngines"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	long							lLanguageID = 0;
	short							lGender = GENDER_NEUTRAL;
	IDaSvrTTSEnginesPtr				lServerObject;
	CComObject <DaCtlTTSEngines> *	lObject = NULL;
	IDaCtlTTSEnginesPtr				lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(V_VT (&LanguageID) == VT_I4)
		{
			lLanguageID = V_I4 (&LanguageID);
		}
		else
		if	(V_VT (&LanguageID) == VT_I2)
		{
			lLanguageID = V_I2 (&LanguageID);
		}
		else
		if	(!IsEmptyParm (&LanguageID))
		{
			lResult = E_INVALIDARG;
		}

		if	(SUCCEEDED (lResult))
		{
			if	(V_VT (&Gender) == VT_I4)
			{
				lGender = (short)V_I4 (&Gender);
			}
			else
			if	(V_VT (&LanguageID) == VT_I2)
			{
				lGender = V_I2 (&Gender);
			}
			else
			if	(!IsEmptyParm (&Gender))
			{
				lResult = E_INVALIDARG;
			}
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->FindTTSEngines (lLanguageID, lGender, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*TTSEngines) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindTTSEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::GetCharacterTTSEngine (VARIANT LoadKey, IDaCtlTTSEngine **TTSEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::GetCharacterTTSEngine"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	IDaSvrTTSEnginePtr				lServerObject;
	CComObject <DaCtlTTSEngine> *	lObject = NULL;
	IDaCtlTTSEnginePtr				lInterface;

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->GetCharacterTTSEngine (LoadKey, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngine>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*TTSEngine) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::GetCharacterTTSEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindCharacterTTSEngines (VARIANT LoadKey, VARIANT LanguageID, IDaCtlTTSEngines **TTSEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindCharacterTTSEngines"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	long							lLanguageID = 0;
	IDaSvrTTSEnginesPtr				lServerObject;
	CComObject <DaCtlTTSEngines> *	lObject = NULL;
	IDaCtlTTSEnginesPtr				lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(V_VT (&LanguageID) == VT_I4)
		{
			lLanguageID = V_I4 (&LanguageID);
		}
		else
		if	(V_VT (&LanguageID) == VT_I2)
		{
			lLanguageID = V_I2 (&LanguageID);
		}
		else
		if	(!IsEmptyParm (&LanguageID))
		{
			lResult = E_INVALIDARG;
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->FindCharacterTTSEngines (LoadKey, lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*TTSEngines) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindCharacterTTSEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_SREngines (IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_SREngines"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	IDaSvrSREnginesPtr				lServerObject;
	CComObject <DaCtlSREngines> *	lObject = NULL;
	IDaCtlSREnginesPtr				lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->get_SREngines (&lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SREngines) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_SREngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindSREngines (VARIANT LanguageID, IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindSREngines"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	long							lLanguageID = 0;
	IDaSvrSREnginesPtr				lServerObject;
	CComObject <DaCtlSREngines> *	lObject = NULL;
	IDaCtlSREnginesPtr				lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(V_VT (&LanguageID) == VT_I4)
		{
			lLanguageID = V_I4 (&LanguageID);
		}
		else
		if	(V_VT (&LanguageID) == VT_I2)
		{
			lLanguageID = V_I2 (&LanguageID);
		}
		else
		if	(!IsEmptyParm (&LanguageID))
		{
			lResult = E_INVALIDARG;
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->FindSREngines (lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SREngines) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindSREngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::GetCharacterSREngine (VARIANT LoadKey, IDaCtlSREngine **SREngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::GetCharacterSREngine"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	IDaSvrSREnginePtr				lServerObject;
	CComObject <DaCtlSREngine> *	lObject = NULL;
	IDaCtlSREnginePtr				lInterface;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->GetCharacterSREngine (LoadKey, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSREngine>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SREngine) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::GetCharacterSREngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindCharacterSREngines (VARIANT LoadKey, VARIANT LanguageID, IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindCharacterSREngines"), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	long							lLanguageID = 0;
	IDaSvrSREnginesPtr				lServerObject;
	CComObject <DaCtlSREngines> *	lObject = NULL;
	IDaCtlSREnginesPtr				lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(V_VT (&LanguageID) == VT_I4)
		{
			lLanguageID = V_I4 (&LanguageID);
		}
		else
		if	(V_VT (&LanguageID) == VT_I2)
		{
			lLanguageID = V_I2 (&LanguageID);
		}
		else
		if	(!IsEmptyParm (&LanguageID))
		{
			lResult = E_INVALIDARG;
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->FindCharacterSREngines (LoadKey, lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SREngines) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindCharacterSREngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::InterfaceSupportsErrorInfo (REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaControl2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaControl), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtl), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaControl::OnAutoSizeChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnAutoSizeChanged [%d]"), this, m_dwRef, m_bAutoSize);
#endif
}

void DaControl::OnBackColorChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBackColorChanged [%8.8X]"), this, m_dwRef, m_clrBackColor);
#endif
}

void DaControl::OnBorderColorChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderColorChanged [%8.8X]"), this, m_dwRef, m_clrBorderColor);
#endif
}

void DaControl::OnBorderStyleChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderStyleChanged [%d]"), this, m_dwRef, m_nBorderStyle);
#endif
}

void DaControl::OnBorderVisibleChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderVisibleChanged [%d]"), this, m_dwRef, m_bBorderVisible);
#endif
}

void DaControl::OnBorderWidthChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderWidthChanged [%d]"), this, m_dwRef, m_nBorderWidth);
#endif
}

void DaControl::OnMouseIconChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnMouseIconChanged [%p]"), this, m_dwRef, m_pMouseIcon.p);
#endif
}

void DaControl::OnMousePointerChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnMousePointerChanged [%d]"), this, m_dwRef, m_nMousePointer);
#endif
}
