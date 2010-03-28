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
#include "DaControl.h"
#include "DaControlObj.h"
#include "DaCtlCharacters.h"
#include "DaCtlCharacter.h"
#include "DaCtlCommands.h"
#include "DaCtlAudioOutput.h"
#include "DaCtlSpeechInput.h"
#include "DaCtlCommandsWindow.h"
#include "DaCtlPropertySheet.h"
#include "DaCtlUserInput.h"
#include "DaCtlCharacterFiles.h"
#include "DaCtlSpeechEngine.h"
#include "DaCtlSpeechEngines.h"
#include "DaCtlRecognitionEngine.h"
#include "DaCtlRecognitionEngines.h"
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

HRESULT WINAPI CDaControlObj::UpdateRegistryOverride (BOOL bRegister)
{
	HRESULT	lResult = UpdateRegistry (bRegister);
	CString	lClassIdStr = CGuidStr (__uuidof(CDaControlObj));

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
				CString		lControlPath;

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
//				RegisterDefCategory (__uuidof(AgentControl));
//				RegisterCategory (__uuidof(AgentControl), CATID_Programmable);
//				RegisterCategory (__uuidof(AgentControl), CATID_Control);

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

UINT	CDaControlObj::mCompleteRequestsMsg = RegisterWindowMessage (_T("1147E530-A208-11DE-ABF2-002421116FB2"));

CDaControlObj::CDaControlObj()
:	mRaiseRequestErrors (true),
	mFinalReleased (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::CDaControlObj"), this, m_dwRef);
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

CDaControlObj::~CDaControlObj()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::~CDaControlObj"), this, m_dwRef);
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

void CDaControlObj::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::FinalRelease [%u]"), this, m_dwRef, mFinalReleased);
	}
#endif
	if	(!mFinalReleased)
	{
		mFinalReleased = true;
		Terminate (false);
	}
}

void CDaControlObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServer.GetInterfacePtr());
		}
#endif
		try
		{
			CDaCtlCharacters *		lCharacters;
			CDaCtlAudioOutput *		lAudioOutput;
			CDaCtlSpeechInput *		lSpeechInput;
			CDaCtlCommandsWindow *	lCommandsWindow;
			CDaCtlPropertySheet *	lPropertySheet;
			CDaCtlCharacterFiles *	lCharacterFiles;

			mServerNotifySink->Terminate ();
			SafeFreeSafePtr (mServerNotifySink);

			if	(
					(mCharacters != NULL)
				&&	(lCharacters = dynamic_cast <CDaCtlCharacters *> (mCharacters.GetInterfacePtr()))
				)
			{
				lCharacters->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mCharacters = NULL;
			}

			if	(
					(mAudioOutput != NULL)
				&&	(lAudioOutput = dynamic_cast <CDaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()))
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
				&&	(lCharacterFiles = dynamic_cast <CDaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()))
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
				&&	(lSpeechInput = dynamic_cast <CDaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()))
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
				&&	(lCommandsWindow = dynamic_cast <CDaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()))
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
				&&	(lPropertySheet = dynamic_cast <CDaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()))
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaControlObj::OnDrawAdvanced(ATL_DRAWINFO& di)
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

HRESULT CDaControlObj::ConnectServer ()
{
	HRESULT	lResult = S_FALSE;

	if	(mServer == NULL)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::ConnectServer"), this, m_dwRef);
		}
#endif
		mServerNotifySink->Terminate ();
		SafeFreeSafePtr (mServerNotifySink);

		lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(CDaAgent), NULL, CLSCTX_LOCAL_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server"));

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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::ConnectServer [%p] Sink [%d]"), this, m_dwRef, mServer.GetInterfacePtr(), (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
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

HRESULT CDaControlObj::DisconnectServer (bool pForce)
{
	HRESULT	lResult = (mServer != NULL) ? S_OK : S_FALSE;

#ifdef	_LOG_INSTANCE
	if	(
			(mServer != NULL)
		&&	(LogIsActive())
		)
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::DisconnectServer [%p(%u)] Force [%u] Sink [%u]"), this, m_dwRef, mServer.GetInterfacePtr(), CoIsHandlerConnected(mServer), pForce, (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
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

void CDaControlObj::DisconnectNotify (bool pForce)
{
	if	(this)
	{
		try
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive())
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaControlObj::DisconnectNotify [%u] [%p]"), this, m_dwRef, pForce, mServer.GetInterfacePtr());
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
				if	(CProxy_DaCtlEvents<CDaControlObj>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- _DaCtlEvents Connections [%d] ---"), CProxy_DaCtlEvents<CDaControlObj>::m_vec.GetSize()); 
				}
				for	(lNdx = 0; lNdx < CProxy_DaCtlEvents<CDaControlObj>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = CProxy_DaCtlEvents<CDaControlObj>::m_vec.GetAt (lNdx))
					{
						LogMessage (_LOG_INSTANCE, _T("--- _DaCtlEvents Connection [%d] [%p] ---"), lNdx, lConnection); 
					}
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(CProxy_AgentEvents<CDaControlObj>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- _AgentEvents Connections [%d] ---"), CProxy_AgentEvents<CDaControlObj>::m_vec.GetSize()); 
				}
				for	(lNdx = 0; lNdx < CProxy_AgentEvents<CDaControlObj>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = CProxy_AgentEvents<CDaControlObj>::m_vec.GetAt (lNdx))
					{
						LogMessage (_LOG_INSTANCE, _T("--- _AgentEvents Connection [%d] [%p] ---"), lNdx, lConnection); 
					}
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(IPropertyNotifySinkCP<CDaControlObj>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- IPropertyNotifySink Connections [%d] ---"), IPropertyNotifySinkCP<CDaControlObj>::m_vec.GetSize()); 
				}
				for	(lNdx = 0; lNdx < IPropertyNotifySinkCP<CDaControlObj>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = IPropertyNotifySinkCP<CDaControlObj>::m_vec.GetAt (lNdx))
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

IDaCtlRequest * CDaControlObj::PutRequest (DaRequestCategory pCategory, long pReqID, HRESULT pResult)
{
	IDaCtlRequestPtr				lInterface;
	CDaCtlRequest *					lRequest = NULL;
	CComObject <CDaCtlRequest> *	lNewRequest = NULL;

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
			&&	(SUCCEEDED (LogComErr (LogNormal, CComObject <CDaCtlRequest>::CreateInstance (&lNewRequest))))
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
				lRequest->mStatus = IS_INTERRUPT_ERROR (lRequest->mResult) ? RequestInterrupted : RequestFailed;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyComplete) != 0)
			{
				lRequest->mStatus = RequestSuccess;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyStart) != 0)
			{
				lRequest->mStatus = RequestInProgress;
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
			||	(mMsgPostingWnd = new CMsgPostingWnd <CDaControlObj> (*this))
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

LRESULT CDaControlObj::OnCompleteRequests (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CompleteRequests (true);
	bHandled = TRUE;
	return 0;
}

void CDaControlObj::CompleteRequests (bool pIdleTime)
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
			POSITION						lPos;
			INT_PTR							lNdx;
			long							lReqID;
			CDaCtlRequest *					lRequest;
			CPtrTypeArray <CDaCtlRequest>	lActiveRequests;

#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("[%p(%d)] Complete Requests [%d] Idle [%u]"), this, m_dwRef, mActiveRequests.GetCount(), pIdleTime);
#endif
			for	(lPos = mActiveRequests.GetStartPosition(); lPos;)
			{
				mActiveRequests.GetNextAssoc (lPos, lReqID, lRequest);
				lActiveRequests.Add (lRequest);
			}
			for	(lNdx = lActiveRequests.GetUpperBound (); lRequest = lActiveRequests (lNdx); lNdx--)
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
							(lRequest->mStatus == RequestInProgress)
						||	(lRequest->mStatus == RequestSuccess)
						||	(lRequest->mStatus == RequestFailed)
						||	(lRequest->mStatus == RequestInterrupted)
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
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CompleteRequests::RequestStart [%d]"), this, m_dwRef, lRequest->mReqID);
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
							(lRequest->mStatus == RequestSuccess)
						||	(lRequest->mStatus == RequestFailed)
						||	(lRequest->mStatus == RequestInterrupted)
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
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CompleteRequests::RequestComplete [%d]"), this, m_dwRef, lRequest->mReqID);
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
					if	(lRequest->mStatus == RequestPending)
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

void CDaControlObj::TerminateRequests (bool pFinal)
{
	if	(
			(!mActiveRequests.IsEmpty ())
		||	(mCompletedRequests.GetSize () > 0)
		)
	{
		try
		{
			POSITION						lPos;
			INT_PTR							lNdx;
			long							lReqID;
			CDaCtlRequest *					lRequest;
			CPtrTypeArray <CDaCtlRequest>	lActiveRequests;

#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("[%p(%d)] Terminate [%u] Requests [%d %d]"), this, m_dwRef, pFinal, mActiveRequests.GetCount(), mCompletedRequests.GetSize());
#endif
			for	(lPos = mActiveRequests.GetStartPosition(); lPos;)
			{
				mActiveRequests.GetNextAssoc (lPos, lReqID, lRequest);
				lActiveRequests.Add (lRequest);
			}
			mActiveRequests.RemoveAll ();
			mCompletedRequests.Append (lActiveRequests);

			for	(lNdx = lActiveRequests.GetUpperBound (); lRequest = lActiveRequests (lNdx); lNdx--)
			{
				lRequest->Release ();
			}
			for	(lNdx = mCompletedRequests.GetUpperBound (); lRequest = mCompletedRequests (lNdx); lNdx--)
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

void CDaControlObj::RequestCreated (CDaCtlRequest * pRequest)
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

void CDaControlObj::RequestDeleted (CDaCtlRequest * pRequest)
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

CString CDaControlObj::GetControlCharacterID (long pServerCharID)
{
	CDaCtlCharacters *	lCharacters;
	CString				lCharacterID;
	POSITION			lPos;
	CDaCtlCharacter *	lCharacter;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = dynamic_cast <CDaCtlCharacters *> (mCharacters.GetInterfacePtr()))
		)
	{
		for	(lPos = lCharacters->mCharacters.GetStartPosition(); lPos;)
		{
			if	(
					(lCharacter = dynamic_cast <CDaCtlCharacter *> (lCharacters->mCharacters.GetValueAt (lPos).GetInterfacePtr()))
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

CString CDaControlObj::GetActiveCharacterID ()
{
	CDaCtlCharacters *	lCharacters;
	CString				lCharacterID;
	POSITION			lPos;
	CDaCtlCharacter *	lCharacter;
	short				lCharacterState;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = dynamic_cast <CDaCtlCharacters *> (mCharacters.GetInterfacePtr()))
		)
	{
		for	(lPos = lCharacters->mCharacters.GetStartPosition(); lPos;)
		{
			if	(
					(lCharacter = dynamic_cast <CDaCtlCharacter *> (lCharacters->mCharacters.GetValueAt (lPos).GetInterfacePtr()))
				&&	(lCharacter->mServerObject != NULL)
				&&	(SUCCEEDED (lCharacter->mServerObject->GetActive (&(lCharacterState=-1))))
				&&	(lCharacterState >= ActiveType_Active)
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

CDaCtlCharacter * CDaControlObj::GetActiveCharacter ()
{
	CDaCtlCharacters *	lCharacters;
	POSITION			lPos;
	CDaCtlCharacter *	lCharacter;
	short				lCharacterState;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = dynamic_cast <CDaCtlCharacters *> (mCharacters.GetInterfacePtr()))
		)
	{
		for	(lPos = lCharacters->mCharacters.GetStartPosition(); lPos;)
		{
			if	(
					(lCharacter = dynamic_cast <CDaCtlCharacter *> (lCharacters->mCharacters.GetValueAt (lPos).GetInterfacePtr()))
				&&	(lCharacter->mServerObject != NULL)
				&&	(SUCCEEDED (lCharacter->mServerObject->GetActive (&(lCharacterState=-1))))
				&&	(lCharacterState >= ActiveType_Active)
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

STDMETHODIMP CDaControlObj::get_Characters (IDaCtlCharacters **Characters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_Characters"), this, m_dwRef);
#endif
	HRESULT							lResult = ConnectServer ();
	CComObject <CDaCtlCharacters> *	lCharacters = NULL;
	IDaCtlCharactersPtr				lInterface;

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
					if	(SUCCEEDED (lResult = CComObject <CDaCtlCharacters>::CreateInstance (&lCharacters))) 
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_Characters [%p]"), this, m_dwRef, dynamic_cast <CDaCtlCharacters *> (mCharacters.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_Characters"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::get_AudioOutput (IDaCtlAudioOutput **AudioOutput)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_AudioOutput"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <CDaCtlAudioOutput> *	lAudioOutput = NULL;
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
					if	(SUCCEEDED (lResult = CComObject <CDaCtlAudioOutput>::CreateInstance (&lAudioOutput))) 
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_AudioOutput [%p]"), this, m_dwRef, dynamic_cast <CDaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_AudioOutput"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::get_SpeechInput (IDaCtlSpeechInput **SpeechInput)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_SpeechInput"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <CDaCtlSpeechInput> *	lSpeechInput = NULL;
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
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechInput>::CreateInstance (&lSpeechInput))) 
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_SpeechInput [%p]"), this, m_dwRef, dynamic_cast <CDaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_SpeechInput"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::get_PropertySheet (IDaCtlPropertySheet **PropSheet)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_PropertySheet"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <CDaCtlPropertySheet> *	lPropertySheet = NULL;
	IDaCtlPropertySheetPtr				lInterface;

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
					if	(SUCCEEDED (lResult = CComObject <CDaCtlPropertySheet>::CreateInstance (&lPropertySheet))) 
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_PropertySheet [%p]"), this, m_dwRef, dynamic_cast <CDaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_PropertySheet"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::get_CommandsWindow (IDaCtlCommandsWindow **CommandsWindow)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_CommandsWindow"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <CDaCtlCommandsWindow> *	lCommandsWindow = NULL;
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
					if	(SUCCEEDED (lResult = CComObject <CDaCtlCommandsWindow>::CreateInstance (&lCommandsWindow))) 
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_CommandsWindow [%p]"), this, m_dwRef, dynamic_cast <CDaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_CommandsWindow"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_Connected (VARIANT_BOOL *Connected)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_Connected"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_Connected"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::put_Connected (VARIANT_BOOL Connected)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::put_Connected"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::put_Connected"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_Suspended (VARIANT_BOOL *Suspended)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_Suspended"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_Suspended"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_RaiseRequestErrors (VARIANT_BOOL *RaiseErrors)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::get_RaiseRequestErrors"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_RaiseRequestErrors"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::put_RaiseRequestErrors (VARIANT_BOOL RaiseErrors)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::put_RaiseRequestErrors"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	mRaiseRequestErrors = (RaiseErrors!=VARIANT_FALSE);

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::put_RaiseRequestErrors"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::ShowDefaultCharacterProperties (VARIANT x, VARIANT y)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::ShowDefaultCharacterProperties"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::ShowDefaultCharacterProperties"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_CharacterFiles (IDaCtlCharacterFiles **CharacterFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_CharacterFiles"), this, m_dwRef);
#endif
	HRESULT								lResult = ConnectServer ();
	CComObject <CDaCtlCharacterFiles> *	lCharacterFiles = NULL;
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
				if	(SUCCEEDED (lResult = CComObject <CDaCtlCharacterFiles>::CreateInstance (&lCharacterFiles))) 
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_CharacterFiles [%p]"), this, m_dwRef, dynamic_cast <CDaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_CharacterFiles"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_CharacterStyle (long *CharacterStyle)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::get_CharacterStyle"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_CharacterStyle"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::put_CharacterStyle (long CharacterStyle)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::put_CharacterStyle"), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::put_CharacterStyle"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_SpeechEngines (IDaCtlSpeechEngines **SpeechEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_SpeechEngines"), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	IDaSvrSpeechEnginesPtr				lServerObject;
	CComObject <CDaCtlSpeechEngines> *	lObject = NULL;
	IDaCtlSpeechEnginesPtr				lInterface;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->GetSpeechEngines (&lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechEngines>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SpeechEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_SpeechEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::FindSpeechEngines (VARIANT LanguageID, VARIANT Gender, IDaCtlSpeechEngines **SpeechEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::FindSpeechEngines"), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	long								lLanguageID = 0;
	short								lGender = GENDER_NEUTRAL;
	IDaSvrSpeechEnginesPtr				lServerObject;
	CComObject <CDaCtlSpeechEngines> *	lObject = NULL;
	IDaCtlSpeechEnginesPtr				lInterface;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

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
				if	(SUCCEEDED (lResult = mServer->FindSpeechEngines (lLanguageID, lGender, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechEngines>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SpeechEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::FindSpeechEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::GetCharacterSpeechEngine (VARIANT LoadKey, IDaCtlSpeechEngine **SpeechEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::GetCharacterSpeechEngine"), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	IDaSvrSpeechEnginePtr				lServerObject;
	CComObject <CDaCtlSpeechEngine> *	lObject = NULL;
	IDaCtlSpeechEnginePtr				lInterface;

	if	(!SpeechEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngine) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->GetCharacterSpeechEngine (LoadKey, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechEngine>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SpeechEngine) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::GetCharacterSpeechEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::FindCharacterSpeechEngines (VARIANT LoadKey, VARIANT LanguageID, IDaCtlSpeechEngines **SpeechEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::FindCharacterSpeechEngines"), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	long								lLanguageID = 0;
	IDaSvrSpeechEnginesPtr				lServerObject;
	CComObject <CDaCtlSpeechEngines> *	lObject = NULL;
	IDaCtlSpeechEnginesPtr				lInterface;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

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
				if	(SUCCEEDED (lResult = mServer->FindCharacterSpeechEngines (LoadKey, lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechEngines>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*SpeechEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::FindCharacterSpeechEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::get_RecognitionEngines (IDaCtlRecognitionEngines **RecognitionEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::get_RecognitionEngines"), this, m_dwRef);
#endif
	HRESULT									lResult = S_OK;
	IDaSvrRecognitionEnginesPtr				lServerObject;
	CComObject <CDaCtlRecognitionEngines> *	lObject = NULL;
	IDaCtlRecognitionEnginesPtr				lInterface;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->GetRecognitionEngines (&lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlRecognitionEngines>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*RecognitionEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::get_RecognitionEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::FindRecognitionEngines (VARIANT LanguageID, IDaCtlRecognitionEngines **RecognitionEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::FindRecognitionEngines"), this, m_dwRef);
#endif
	HRESULT									lResult = S_OK;
	long									lLanguageID = 0;
	IDaSvrRecognitionEnginesPtr				lServerObject;
	CComObject <CDaCtlRecognitionEngines> *	lObject = NULL;
	IDaCtlRecognitionEnginesPtr				lInterface;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

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
				if	(SUCCEEDED (lResult = mServer->FindRecognitionEngines (lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlRecognitionEngines>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*RecognitionEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::FindRecognitionEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::GetCharacterRecognitionEngine (VARIANT LoadKey, IDaCtlRecognitionEngine **RecognitionEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::GetCharacterRecognitionEngine"), this, m_dwRef);
#endif
	HRESULT									lResult = S_OK;
	IDaSvrRecognitionEnginePtr				lServerObject;
	CComObject <CDaCtlRecognitionEngine> *	lObject = NULL;
	IDaCtlRecognitionEnginePtr				lInterface;

	if	(!RecognitionEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngine) = NULL;

		if	(
				(SUCCEEDED (lResult = ConnectServer ()))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServer->GetCharacterRecognitionEngine (LoadKey, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlRecognitionEngine>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*RecognitionEngine) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::GetCharacterRecognitionEngine"), this, m_dwRef);
	}
#endif
	return lResult;
}

STDMETHODIMP CDaControlObj::FindCharacterRecognitionEngines (VARIANT LoadKey, VARIANT LanguageID, IDaCtlRecognitionEngines **RecognitionEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaControlObj::FindCharacterRecognitionEngines"), this, m_dwRef);
#endif
	HRESULT									lResult = S_OK;
	long									lLanguageID = 0;
	IDaSvrRecognitionEnginesPtr				lServerObject;
	CComObject <CDaCtlRecognitionEngines> *	lObject = NULL;
	IDaCtlRecognitionEnginesPtr				lInterface;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

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
				if	(SUCCEEDED (lResult = mServer->FindCharacterRecognitionEngines (LoadKey, lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlRecognitionEngines>::CreateInstance (&lObject))) 
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (this);
						lInterface = (LPDISPATCH) lObject;
						(*RecognitionEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaControlObj::FindCharacterRecognitionEngines"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::InterfaceSupportsErrorInfo (REFIID riid)
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

void CDaControlObj::OnAutoSizeChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnAutoSizeChanged [%d]"), this, m_dwRef, m_bAutoSize);
#endif
}

void CDaControlObj::OnBackColorChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnBackColorChanged [%8.8X]"), this, m_dwRef, m_clrBackColor);
#endif
}

void CDaControlObj::OnBorderColorChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnBorderColorChanged [%8.8X]"), this, m_dwRef, m_clrBorderColor);
#endif
}

void CDaControlObj::OnBorderStyleChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnBorderStyleChanged [%d]"), this, m_dwRef, m_nBorderStyle);
#endif
}

void CDaControlObj::OnBorderVisibleChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnBorderVisibleChanged [%d]"), this, m_dwRef, m_bBorderVisible);
#endif
}

void CDaControlObj::OnBorderWidthChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnBorderWidthChanged [%d]"), this, m_dwRef, m_nBorderWidth);
#endif
}

void CDaControlObj::OnMouseIconChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnMouseIconChanged [%p]"), this, m_dwRef, m_pMouseIcon.p);
#endif
}

void CDaControlObj::OnMousePointerChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] CDaControlObj::OnMousePointerChanged [%d]"), this, m_dwRef, m_nMousePointer);
#endif
}
