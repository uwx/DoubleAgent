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
#include <ObjSafe.h>
#include "DaControl.h"
#include "DaAgentCtl.h"
#include "DaCharactersObj.h"
#include "DaCharacterObj.h"
#include "DaCommandsObj.h"
#include "DaAudioOutputObj.h"
#include "DaSpeechInputObj.h"
#include "DaCommandsWindowObj.h"
#include "DaPropertySheetObj.h"
#include "DaUserInputObj.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "ErrorInfo.h"
#include "GuidStr.h"
#include "UiState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Control"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_REQUEST			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_SAFETY			(GetProfileDebugInt(_T("DebugSafety"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Control"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

#ifdef	_DEBUG_REQUEST
#ifdef  _DEBUG_NOTIFY
#define	_DEBUG_REQUEST_NOTIFY	MinLogLevel(_DEBUG_REQUEST,_DEBUG_NOTIFY)
#else
#define	_DEBUG_REQUEST_NOTIFY	_DEBUG_REQUEST
#endif
#else
#ifdef  _DEBUG_NOTIFY
#define	_DEBUG_REQUEST_NOTIFY	_DEBUG_NOTIFY
#endif
#endif

/////////////////////////////////////////////////////////////////////////////

typedef enum
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
}	COMPAT;

/////////////////////////////////////////////////////////////////////////////

static const DWORD BASED_CODE sControlOleMisc = OLEMISC_INVISIBLEATRUNTIME|OLEMISC_SETCLIENTSITEFIRST|OLEMISC_NOUIACTIVATE|OLEMISC_INSIDEOUT|OLEMISC_CANTLINKINSIDE|OLEMISC_INSERTNOTREPLACE|OLEMISC_ONLYICONIC|OLEMISC_STATIC;
static const DWORD BASED_CODE sControlCompat = COMPAT_NO_PROPNOTIFYSINK|COMPAT_NO_SETEXTENT|COMPAT_NO_UIACTIVATE|COMPAT_NO_QUICKACTIVATE|COMPAT_NEVERFOCUSSABLE|COMPAT_HWNDPRIVATE;

IMPLEMENT_DYNCREATE(CDaAgentCtl, COleControl)
IMPLEMENT_OLECREATE_UUID(CDaAgentCtl, _CONTROL_PROGID)
IMPLEMENT_OLETYPELIB(CDaAgentCtl, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)
IMPLEMENT_OLECTLTYPE(CDaAgentCtl, IDS_AGENT, sControlOleMisc)

/////////////////////////////////////////////////////////////////////////////

BOOL CDaAgentCtl::CDaAgentCtlFactory::UpdateRegistry(BOOL bRegister)
{
	BOOL	lRet = FALSE;
	CString	lClassIdStr = CGuidStr (m_clsid);

	if	(bRegister)
	{
		if	(LogIsActive ())
		{
			LogMessage (LogIfActive, _T("==> Register %hs %s"), m_pRuntimeClass->m_lpszClassName, lClassIdStr);
		}
		if	(AfxOleRegisterControlClass (AfxGetInstanceHandle(), m_clsid, m_lpszProgID, IDS_AGENT, IDB_AGENT, afxRegApartmentThreading, sControlOleMisc, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor))
		{
			RegisterServer ();
			RegisterApartmentThreaded ();
			RegisterDefCategory ();
			RegisterCategory (m_clsid, CATID_Programmable);
			RegisterCategory (m_clsid, CATID_Control);
			RegisterCategory (m_clsid, CATID_SafeForScripting);
			RegisterCategory (m_clsid, CATID_SafeForInitializing);
			lRet = TRUE;
		}
	}
	else
	{
		if	(LogIsActive ())
		{
			LogMessage (LogIfActive, _T("==> Unregister %hs %s"), m_pRuntimeClass->m_lpszClassName, lClassIdStr);
		}
		if	(AfxOleUnregisterClass (m_clsid, m_lpszProgID))
		{
			COleObjectFactoryExEx::UpdateRegistry (bRegister, (LPCTSTR)NULL);
			lRet = TRUE;
		}
	}

	if	(lRet)
	{
		CRegKey	lExtRoot (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Ext"), true);
		CRegKey	lPreApproved (lExtRoot, _T("PreApproved"), true);
		CRegKey	lCompatRoot (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility"), true);

		if	(bRegister)
		{
			CRegKey (lPreApproved, lClassIdStr, false, true);
			CRegDWord (CRegKey (lCompatRoot, lClassIdStr, false, true), _T("Compatibility Flags"), true).SetValue(sControlCompat).Update();
		}
		else
		{
			CRegKey (lPreApproved, lClassIdStr, false).Delete ();
			CRegKey (lCompatRoot, lClassIdStr, false).Delete ();
		}
	}

	if	(lRet)
	{
		if	(bRegister)
		{
			if	(
					(IsWindows7_AtLeast())
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
				)
			{
				CRegKey		lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentControl)), false, true);
				CRegKey		lControlProgId (HKEY_CLASSES_ROOT, _T(_AGENT_CONTROL_PROGID), false, true);
				CRegKey		lControlProgId2 (HKEY_CLASSES_ROOT,_T( _AGENT_CONTROL_PROGID2), false, true);
				CString		lControlPath;

				GetModuleFileName (AfxGetInstanceHandle(), lControlPath.GetBuffer(MAX_PATH), MAX_PATH);
				PathRemoveFileSpec (lControlPath.GetBuffer(MAX_PATH));
				PathAppend (lControlPath.GetBuffer(MAX_PATH), _T("AgentCtl.dll"));
				lControlPath.ReleaseBuffer ();

				CRegString (lMsAgentClass, (LPCTSTR)NULL, true).Update (_T("Microsoft Agent Control 2.0"));
				CRegString (lMsAgentClass, _T("Version"), true).Update (_T("2.0"));
				CRegString (CRegKey (lMsAgentClass, _T("ProgID"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID2));
				CRegString (CRegKey (lMsAgentClass, _T("InprocServer32"), false, true), (LPCTSTR)NULL, true).Update (lControlPath);
				CRegString (CRegKey (lMsAgentClass, _T("MiscStatus"), false, true), (LPCTSTR)NULL, true).Update (_T("0"));
				CRegString (CRegKey (CRegKey (lMsAgentClass, _T("MiscStatus"), false, true), _T("1"), false, true), (LPCTSTR)NULL, true).Update (_T("148628"));
				RegisterDefCategory (__uuidof(AgentControl));
				RegisterCategory (__uuidof(AgentControl), CATID_Programmable);
				RegisterCategory (__uuidof(AgentControl), CATID_Control);

				CRegString (lControlProgId, (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID_NAME));
				CRegString (CRegKey (lControlProgId, _T("CurVer"), false, true), (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID2));
				CRegString (lControlProgId2, (LPCTSTR)NULL, true).Update (_T(_AGENT_CONTROL_PROGID_NAME));
				CRegString (CRegKey (lControlProgId2, _T("CLSID"), false, true), (LPCTSTR)NULL, true).Update ((CString)CGuidStr(__uuidof(AgentControl)));

				CRegString (CRegKey (lMsAgentClass, _T("AutoTreatAs"), false, true), (LPCTSTR)NULL, true).Update (lClassIdStr);
				CRegString (CRegKey (lMsAgentClass, _T("TreatAs"), false, true), (LPCTSTR)NULL, true).Update (lClassIdStr);
			}
		}
		else
		{
			CRegKey	lMsAgentClass (HKEY_CLASSES_ROOT, _T("CLSID\\")+(CString)CGuidStr(__uuidof(AgentControl)), false);

			if	(
					(IsWindows7_AtLeast())
				&&	(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
				)
			{
				lMsAgentClass.Delete ();

				CRegKey (HKEY_CLASSES_ROOT, _T(_AGENT_CONTROL_PROGID), false).Delete ();
				CRegKey (HKEY_CLASSES_ROOT, _T(_AGENT_CONTROL_PROGID2), false).Delete ();
			}
			else
			{
				CRegKey	lAutoTreatAs (lMsAgentClass, _T("AutoTreatAs"), false);
				CRegKey	lTreatAs (lMsAgentClass, _T("TreatAs"), false);

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

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

UINT	CDaAgentCtl::mCompleteRequestsMsg = RegisterWindowMessage (_T("1147E530-A208-11DE-ABF2-002421116FB2"));

BEGIN_MESSAGE_MAP(CDaAgentCtl, COleControl)
	//{{AFX_MSG_MAP(CDaAgentCtl)
	ON_REGISTERED_MESSAGE (mCompleteRequestsMsg, OnCompleteRequests)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaAgentCtl, COleControl)
	INTERFACE_PART(CDaAgentCtl, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentCtl, __uuidof(IDaControl), AgentCtl)
	INTERFACE_PART(CDaAgentCtl, __uuidof(IDaControl), AgentCtl)
	INTERFACE_PART(CDaAgentCtl, __uuidof(IAgentCtl), AgentCtl)
	INTERFACE_PART(CDaAgentCtl, __uuidof(IAgentCtlEx), AgentCtl)
	INTERFACE_PART(CDaAgentCtl, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAgentCtl, __uuidof(IObjectSafety), ObjectSafety)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentCtl, AgentCtl)
IMPLEMENT_IUNKNOWN(CDaAgentCtl, ObjectSafety)

BEGIN_SUPPORTERRORINFO(CDaAgentCtl)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCtl, __uuidof(IDaControl))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCtl, __uuidof(IDaControl))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCtl, __uuidof(IDaControl))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCtl, __uuidof(IDaControl))
END_SUPPORTERRORINFO(CDaAgentCtl)

BEGIN_CONNECTION_MAP(CDaAgentCtl, COleControl)
	CONNECTION_PART(CDaAgentCtl, __uuidof(_DaCtlEvents), EventConnPt)
	CONNECTION_PART(CDaAgentCtl, __uuidof(_AgentEvents), EventConnPt)
END_CONNECTION_MAP()

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentCtl, COleControl)
	//{{AFX_DISPATCH_MAP(CDaAgentCtl)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "Characters", DISPID_IAgentCtl_Characters, DspGetCharacters, DspSetCharacters, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "AudioOutput", DISPID_IAgentCtl_AudioOutput, DspGetAudioOutput, DspSetAudioOutput, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "SpeechInput", DISPID_IAgentCtl_SpeechInput, DspGetSpeechInput, DspSetSpeechInput, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "PropertySheet", DISPID_IAgentCtl_PropertySheet, DspGetPropertySheet, DspSetPropertySheet, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "CommandsWindow", DISPID_IAgentCtl_CommandsWindow, DspGetCommandsWindow, DspSetCommandsWindow, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "Connected", DISPID_IAgentCtl_Connected, DspGetConnected, DspSetConnected, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "Suspended", DISPID_IAgentCtl_Suspended, DspGetSuspended, DspSetSuspended, VT_BOOL)
	DISP_FUNCTION_ID(CDaAgentCtl, "ShowDefaultCharacterProperties", DISPID_IAgentCtlEx_ShowDefaultCharacterProperties, DspShowDefaultCharacterProperties, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_PROPERTY_EX_ID(CDaAgentCtl, "RaiseRequestErrors", DISPID_IAgentCtlEx_RaiseRequestErrors, DspGetRaiseRequestErrors, DspSetRaiseRequestErrors, VT_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_MAP(CDaAgentCtl, COleControl)
	//{{AFX_EVENT_MAP(CDaAgentCtl)
	EVENT_CUSTOM("ActiveClientChange", FireActiveClientChange, VTS_BSTR  VTS_BOOL)
	EVENT_CUSTOM("HelpComplete", FireHelpComplete, VTS_BSTR  VTS_BSTR  VTS_I2)
	EVENT_CUSTOM("ListenStart", FireListenStart, VTS_BSTR)
	EVENT_CUSTOM("ListenComplete", FireListenComplete, VTS_BSTR  VTS_I2)
	EVENT_CUSTOM("DefaultCharacterChange", FireDefaultCharacterChange, VTS_BSTR)
	EVENT_CUSTOM("AgentPropertyChange", FireAgentPropertyChange, VTS_NONE)
	EVENT_CUSTOM_ID("ActivateInput", DISPID_AgentEvents_ActivateInput, FireActivateInput, VTS_BSTR)
	EVENT_CUSTOM_ID("DeactivateInput", DISPID_AgentEvents_DeactivateInput, FireDeactivateInput, VTS_BSTR)
	EVENT_CUSTOM_ID("Click", DISPID_AgentEvents_Click, FireClick, VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM_ID("DblClick", DISPID_AgentEvents_DblClick, FireDblClick, VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM_ID("DragStart", DISPID_AgentEvents_DragStart, FireDragStart, VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM_ID("DragComplete", DISPID_AgentEvents_DragComplete, FireDragComplete, VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM_ID("Show", DISPID_AgentEvents_Show, FireShow, VTS_BSTR  VTS_I2)
	EVENT_CUSTOM_ID("Hide", DISPID_AgentEvents_Hide, FireHide, VTS_BSTR  VTS_I2)
	EVENT_CUSTOM_ID("RequestStart", DISPID_AgentEvents_RequestStart, FireRequestStart, VTS_DISPATCH)
	EVENT_CUSTOM_ID("RequestComplete", DISPID_AgentEvents_RequestComplete, FireRequestComplete, VTS_DISPATCH)
	EVENT_CUSTOM_ID("Restart", DISPID_AgentEvents_Restart, FireRestart, VTS_NONE)
	EVENT_CUSTOM_ID("Shutdown", DISPID_AgentEvents_Shutdown, FireShutdown, VTS_NONE)
	EVENT_CUSTOM_ID("Bookmark", DISPID_AgentEvents_Bookmark, FireBookmark, VTS_I4)
	EVENT_CUSTOM_ID("Command", DISPID_AgentEvents_Command, FireCommand, VTS_DISPATCH)
	EVENT_CUSTOM_ID("IdleStart", DISPID_AgentEvents_IdleStart, FireIdleStart, VTS_BSTR)
	EVENT_CUSTOM_ID("IdleComplete", DISPID_AgentEvents_IdleComplete, FireIdleComplete, VTS_BSTR)
	EVENT_CUSTOM_ID("Move", DISPID_AgentEvents_Move, FireMove, VTS_BSTR  VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM_ID("Size", DISPID_AgentEvents_Size, FireSize, VTS_BSTR  VTS_I2  VTS_I2)
	EVENT_CUSTOM_ID("BalloonShow", DISPID_AgentEvents_BalloonShow, FireBalloonShow, VTS_BSTR)
	EVENT_CUSTOM_ID("BalloonHide", DISPID_AgentEvents_BalloonHide, FireBalloonHide, VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl::CDaAgentCtl()
:	mRaiseRequestErrors (true),
	mFinalReleased (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::CDaAgentCtl"), this, m_dwRef);
	}
#endif
	InitializeIIDs (&__uuidof(IDaControl), &__uuidof(_DaCtlEvents));
	SetInitialSize (GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
	TheControlApp->OnControlCreated (this);
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaAgentCtl::~CDaAgentCtl()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::~CDaAgentCtl"), this, m_dwRef);
	}
#endif
	mFinalReleased = true;
	Terminate (true);
	mFinalReleased = false;
	TheControlApp->OnControlDeleted (this);
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

void CDaAgentCtl::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::Terminate [%u] [%p] [%p]"), this, m_dwRef, pFinal, mServer.GetInterfacePtr(), m_hWnd);
		}
#endif
		try
		{
			CDaCharactersObj *		lCharacters;
			CDaAudioOutputObj *		lAudioOutput;
			CDaSpeechInputObj *		lSpeechInput;
			CDaCommandsWindowObj *	lCommandsWindow;
			CDaPropertySheetObj *	lPropertySheet;

			SafeFreeSafePtr (mServerNotifySink);

			if	(
					(mCharacters != NULL)
				&&	(lCharacters = (CDaCharactersObj *) CCmdTarget::FromIDispatch (mCharacters))
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
				&&	(lAudioOutput = (CDaAudioOutputObj *) CCmdTarget::FromIDispatch (mAudioOutput))
				)
			{
				lAudioOutput->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mAudioOutput = NULL;
			}

			if	(
					(mSpeechInput != NULL)
				&&	(lSpeechInput = (CDaSpeechInputObj *) CCmdTarget::FromIDispatch (mSpeechInput))
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
				&&	(lCommandsWindow = (CDaCommandsWindowObj *) CCmdTarget::FromIDispatch (mCommandsWindow))
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
				&&	(lPropertySheet = (CDaPropertySheetObj *) CCmdTarget::FromIDispatch (mPropertySheet))
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

			if	(IsWindow (m_hWnd))
			{
				DestroyWindow ();
			}
		}
		catch AnyExceptionDebug

#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaAgentCtl::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::OnFinalRelease [%u]"), this, m_dwRef, mFinalReleased);
	}
#endif
	if	(!mFinalReleased)
	{
		mFinalReleased = true;
		Terminate (false);
		COleControl::OnFinalRelease();
	}
}

LPUNKNOWN CDaAgentCtl::GetInterfaceHook(const void* iid)
{
	LPUNKNOWN	lRet = NULL;
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CDaAgentCtl::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgentCtl::ConnectServer ()
{
	HRESULT	lResult = S_FALSE;

	if	(mServer == NULL)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::ConnectServer"), this, m_dwRef);
		}
#endif
		SafeFreeSafePtr (mServerNotifySink);

		lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(CDaAgent), NULL, CLSCTX_LOCAL_SERVER, __uuidof(IDaServer), (void**)&mServer), _T("Create Server"));

		if	(SUCCEEDED (lResult))
		{
			mServerNotifySink = new CServerNotifySink (*this);
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(m_hWnd == NULL)
			)
		{
			SubclassWindow (::CreateWindow (AfxRegisterWndClass(0), NULL, WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, NULL, AfxGetInstanceHandle(), NULL));
		}
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::ConnectServer [%p] [%d]"), this, m_dwRef, mServer.GetInterfacePtr(), (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
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

HRESULT CDaAgentCtl::DisconnectServer (bool pForce)
{
	HRESULT	lResult = (mServer != NULL) ? S_OK : S_FALSE;

#ifdef	_LOG_INSTANCE
	if	(
			(mServer != NULL)
		&&	(LogIsActive())
		)
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::DisconnectServer [%u] [%p] [%u]"), this, m_dwRef, pForce, mServer.GetInterfacePtr(), (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
	}
#endif
	if	(mServerNotifySink)
	{
		if	(pForce)
		{
			mServerNotifySink->mServerNotifyId = 0;
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

void CDaAgentCtl::DisconnectNotify (bool pForce)
{
	if	(this)
	{
		try
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive())
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::DisconnectNotify [%u] [%p] [%d]"), this, m_dwRef, pForce, mServer.GetInterfacePtr(), m_xEventConnPt.GetConnectionCount());
			}
#endif
			if	(mServerNotifySink)
			{
				if	(pForce)
				{
					mServerNotifySink->mServerNotifyId = 0;
				}
				SafeFreeSafePtr (mServerNotifySink);
			}

#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
			POSITION	lPos;
			LPUNKNOWN	lConnection;

			for	(lPos = m_xEventConnPt.GetStartPosition(); lPos;)
			{
				lConnection = m_xEventConnPt.GetNextConnection (lPos);
				LogMessage (_LOG_INSTANCE, _T("--- Event Connection [%p] ---"), lConnection);
			}
#endif
#endif
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IDaCtlRequest * CDaAgentCtl::PutRequest (DaRequestCategory pCategory, long pReqID, HRESULT pResult)
{
	IDaCtlRequestPtr	lInterface;
	CDaRequestObj *		lRequest = NULL;

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
			LogMessage (_DEBUG_REQUEST, _T("  Reuse Request   [%p(%u)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
		}

		if	(!lRequest)
		{
			lRequest = new CDaRequestObj (*this, pCategory, pReqID, pResult);
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
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%u)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyComplete) != 0)
			{
				lRequest->mStatus = RequestSuccess;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%u)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyStart) != 0)
			{
				lRequest->mStatus = RequestInProgress;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%u)] [%d] Status [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
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
			lInterface = lRequest->GetIDispatch (FALSE);
		}
#ifdef	_DEBUG_REQUEST
		else
		if	(lRequest)
		{
			LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%u)] [%d] Status [%s] deferred"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr());
		}
#endif

		if	(m_hWnd)
		{
			//
			//	Trigger a call to CompleteRequests next time the message queue is cycled
			//
			PostMessage (mCompleteRequestsMsg);
		}
	}
	return lInterface.Detach ();
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CDaAgentCtl::OnCompleteRequests (WPARAM wParam, LPARAM lParam)
{
	CompleteRequests (true);
	return Default ();
}

void CDaAgentCtl::CompleteRequests (bool pIdleTime)
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
			CDaRequestObj *					lRequest;
			CPtrTypeArray <CDaRequestObj>	lActiveRequests;

#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("[%p(%u)] Complete Requests [%d] Idle [%u]"), this, m_dwRef, mActiveRequests.GetCount(), pIdleTime);
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
					LogMessage (_DEBUG_REQUEST, _T("  Release Request [%p(%u)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
					mActiveRequests.RemoveKey (lRequest->mReqID);
					mCompletedRequests.Add (lRequest);
					lRequest->ExternalRelease ();
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
							LogMessage (_DEBUG_REQUEST, _T("  Queued Request [%p(%u)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
#ifdef	_DEBUG_NOTIFY
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::CompleteRequests::RequestStart [%d]"), this, m_dwRef, lRequest->mReqID);
#endif
							if	(TheControlApp->PreNotify ())
							{
								IDaCtlRequestPtr	lInterface (lRequest->GetIDispatch (FALSE));
								try
								{
									FireRequestStart (lInterface);
								}
								catch AnyExceptionDebug
								TheControlApp->PostNotify ();
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
							LogMessage (_DEBUG_REQUEST, _T("  Queued Request [%p(%u)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
#ifdef	_DEBUG_NOTIFY
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::CompleteRequests::RequestComplete [%d]"), this, m_dwRef, lRequest->mReqID);
#endif
							if	(TheControlApp->PreNotify ())
							{
								IDaCtlRequestPtr	lInterface (lRequest->GetIDispatch (FALSE));
								try
								{
									FireRequestComplete (lInterface);
								}
								catch AnyExceptionDebug
								TheControlApp->PostNotify ();
							}
						}
#ifdef	_DEBUG_REQUEST
						LogMessage (_DEBUG_REQUEST, _T("  Release Request [%p(%u)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
						mActiveRequests.RemoveKey (lRequest->mReqID);
						mCompletedRequests.Add (lRequest);
						lRequest->ExternalRelease ();
					}
#ifdef	_DEBUG_REQUEST
					else
					if	(lRequest->mStatus == RequestPending)
					{
						LogMessage (_DEBUG_REQUEST, _T("  Pending Request [%p(%u)] [%d] Status [%s] Category [%s]"), lRequest, lRequest->m_dwRef, lRequest->mReqID, lRequest->StatusStr(), lRequest->CategoryStr());
					}
#endif
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void CDaAgentCtl::TerminateRequests (bool pFinal)
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
			CDaRequestObj *					lRequest;
			CPtrTypeArray <CDaRequestObj>	lActiveRequests;

#ifdef	_DEBUG_REQUEST
			LogMessage (_DEBUG_REQUEST, _T("[%p(%u)] Terminate [%u] Requests [%d %d]"), this, m_dwRef, pFinal, mActiveRequests.GetCount(), mCompletedRequests.GetSize());
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
				lRequest->ExternalRelease ();
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

void CDaAgentCtl::RequestCreated (CDaRequestObj * pRequest)
{
	if	(
			(pRequest)
		&&	(pRequest->mReqID > 0)
		)
	{
		mActiveRequests.SetAt (pRequest->mReqID, pRequest);
#ifdef	_DEBUG_REQUEST
		LogMessage (_DEBUG_REQUEST, _T("  New Request     [%p(%u)] [%d] Status [%s] Category [%s] Count [%d]"), pRequest, pRequest->m_dwRef, pRequest->mReqID, pRequest->StatusStr(), pRequest->CategoryStr(), mActiveRequests.GetCount());
#endif
	}
}

void CDaAgentCtl::RequestDeleted (CDaRequestObj * pRequest)
{
	if	(
			(pRequest)
		&&	(pRequest->mReqID > 0)
		)
	{
		mActiveRequests.RemoveKey (pRequest->mReqID);
		mCompletedRequests.Remove (pRequest);
#ifdef	_DEBUG_REQUEST
		LogMessage (_DEBUG_REQUEST, _T("  Deleted Request [%p(%u)] [%d] Status [%s] Category [%s] Count [%d]"), pRequest, pRequest->m_dwRef, pRequest->mReqID, pRequest->StatusStr(), pRequest->CategoryStr(), mActiveRequests.GetCount());
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString CDaAgentCtl::GetControlCharacterID (long pServerCharID)
{
	CDaCharactersObj *	lCharacters;
	CString				lCharacterID;
	int					lNdx;
	CDaCharacterObj *	lCharacter;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = (CDaCharactersObj *) CCmdTarget::FromIDispatch (mCharacters))
		)
	{
		for	(lNdx = 0; lNdx <= lCharacters->mCharacters.GetUpperBound(); lNdx++)
		{
			if	(
					(lCharacter = (CDaCharacterObj *)CCmdTarget::FromIDispatch (lCharacters->mCharacters [lNdx]))
				&&	(lCharacter->mServerCharID == pServerCharID)
				)
			{
				lCharacterID = lCharacters->mCharacters.KeyAt (lNdx);
				break;
			}
		}
	}
	return lCharacterID;
}

/////////////////////////////////////////////////////////////////////////////

CString CDaAgentCtl::GetActiveCharacterID ()
{
	CDaCharactersObj *	lCharacters;
	CString				lCharacterID;
	int					lNdx;
	CDaCharacterObj *	lCharacter;
	short				lCharacterState;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = (CDaCharactersObj *) CCmdTarget::FromIDispatch (mCharacters))
		)
	{
		for	(lNdx = 0; lNdx <= lCharacters->mCharacters.GetUpperBound(); lNdx++)
		{
			if	(
					(lCharacter = (CDaCharacterObj *)CCmdTarget::FromIDispatch (lCharacters->mCharacters [lNdx]))
				&&	(lCharacter->mServerObject != NULL)
				&&	(SUCCEEDED (lCharacter->mServerObject->GetActive (&(lCharacterState=-1))))
				&&	(lCharacterState >= ACTIVATE_ACTIVE)
				)
			{
				lCharacterID = lCharacters->mCharacters.KeyAt (lNdx);
				break;
			}
		}
	}
	return lCharacterID;
}

CDaCharacterObj * CDaAgentCtl::GetActiveCharacter ()
{
	CDaCharactersObj *	lCharacters;
	int					lNdx;
	CDaCharacterObj *	lCharacter;
	short				lCharacterState;

	if	(
			(mCharacters != NULL)
		&&	(lCharacters = (CDaCharactersObj *) CCmdTarget::FromIDispatch (mCharacters))
		)
	{
		for	(lNdx = 0; lNdx <= lCharacters->mCharacters.GetUpperBound(); lNdx++)
		{
			if	(
					(lCharacter = (CDaCharacterObj *)CCmdTarget::FromIDispatch (lCharacters->mCharacters [lNdx]))
				&&	(lCharacter->mServerObject != NULL)
				&&	(SUCCEEDED (lCharacter->mServerObject->GetActive (&(lCharacterState=-1))))
				&&	(lCharacterState >= ACTIVATE_ACTIVE)
				)
			{
				return lCharacter;
			}
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCtl::OnSetClientSite()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCtl::OnSetClientSite [%p] Connected [%p] Window [%p]"), this, m_dwRef, m_pClientSite, mServer.GetInterfacePtr(), m_hWnd);
	}
#endif
	COleControl::OnSetClientSite();
	if	(!m_pClientSite)
	{
		Terminate (false);
	}
}

BOOL CDaAgentCtl::OnSetExtent(LPSIZEL lpSizeL)
{
	return FALSE;
}

void CDaAgentCtl::OnDraw (CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	HICON	lIcon;

	lIcon = (HICON) LoadImage (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_DOUBLEAGENT), IMAGE_ICON, rcBounds.Width(), rcBounds.Height(), LR_DEFAULTCOLOR);
	::DrawIconEx (pdc->GetSafeHdc(), rcBounds.left, rcBounds.top, lIcon, rcBounds.Width(), rcBounds.Height(), 0, (HBRUSH)GetStockObject(WHITE_BRUSH), DI_NORMAL);
	DestroyIcon (lIcon);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XObjectSafety::GetInterfaceSafetyOptions (REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions)
{
	METHOD_PROLOGUE(CDaAgentCtl, ObjectSafety)
#ifdef	_DEBUG_SAFETY
	LogMessage (_DEBUG_SAFETY, _T("[%p(%u)] CDaAgentCtl::XObjectSafety::GetInterfaceSafetyOptions [%s]"), pThis, pThis->m_dwRef, (CString)CGuidStr(riid));
#endif
	if	(pdwSupportedOptions)
	{
		*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA;
	}
	if	(pdwEnabledOptions)
	{
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XObjectSafety::SetInterfaceSafetyOptions (REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions)
{
	METHOD_PROLOGUE(CDaAgentCtl, ObjectSafety)
#ifdef	_DEBUG_SAFETY
	LogMessage (_DEBUG_SAFETY, _T("[%p(%u)] CDaAgentCtl::XObjectSafety::SetInterfaceSafetyOptions [%s] [%8.8X] [%8.8X]"), pThis, pThis->m_dwRef, (CString)CGuidStr(riid), dwOptionSetMask, dwEnabledOptions);
#endif
//
//	MS Agent shows a message for "unsafe" web sites.  We'll just skip that annoyance.
//
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaAgentCtl::DspGetCharacters()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetCharacters"), this, m_dwRef);
#endif
	IDaCtlCharacters *	lRet = NULL;
	HRESULT					lResult = m_xAgentCtl.get_Characters (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaAgentCtl::DspSetCharacters(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetCharacters"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaAgentCtl::DspGetAudioOutput()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetAudioOutput"), this, m_dwRef);
#endif
	IDaCtlAudioObject *	lRet = NULL;
	HRESULT				lResult = m_xAgentCtl.get_AudioOutput (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaAgentCtl::DspSetAudioOutput(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetAudioOutput"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaAgentCtl::DspGetSpeechInput()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetSpeechInput"), this, m_dwRef);
#endif
	IDaCtlSpeechInput *	lRet = NULL;
	HRESULT				lResult = m_xAgentCtl.get_SpeechInput (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaAgentCtl::DspSetSpeechInput(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetSpeechInput"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaAgentCtl::DspGetPropertySheet()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetPropertySheet"), this, m_dwRef);
#endif
	IDaCtlPropertySheet *	lRet = NULL;
	HRESULT					lResult = m_xAgentCtl.get_PropertySheet (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaAgentCtl::DspSetPropertySheet(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetPropertySheet"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaAgentCtl::DspGetCommandsWindow()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetCommandsWindow"), this, m_dwRef);
#endif
	IDaCtlCommandsWindow *	lRet = NULL;
	HRESULT					lResult = m_xAgentCtl.get_CommandsWindow (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaAgentCtl::DspSetCommandsWindow(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetCommandsWindow"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaAgentCtl::DspGetConnected()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetConnected"), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xAgentCtl.get_Connected (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaAgentCtl::DspSetConnected(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetConnected"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgentCtl.put_Connected (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaAgentCtl::DspGetSuspended()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetSuspended"), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xAgentCtl.get_Suspended (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaAgentCtl::DspSetSuspended(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetSuspended"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

void CDaAgentCtl::DspShowDefaultCharacterProperties(const VARIANT & x, const VARIANT & y)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspShowDefaultCharacterProperties"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgentCtl.ShowDefaultCharacterProperties (x, y);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaAgentCtl::DspGetRaiseRequestErrors()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspGetRaiseRequestErrors"), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xAgentCtl.get_RaiseRequestErrors (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaAgentCtl::DspSetRaiseRequestErrors(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCtl::DspSetRaiseRequestErrors"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAgentCtl.put_RaiseRequestErrors (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_Characters (IDaCtlCharacters **Characters)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Characters"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = pThis->ConnectServer ();
	CDaCharactersObj *	lCharacters;
	IDaCtlCharactersPtr	lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServer)))
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
				if	(pThis->mCharacters == NULL)
				{
					if	(lCharacters = new CDaCharactersObj (*pThis))
					{
						pThis->mCharacters.Attach (lCharacters->GetIDispatch (FALSE));
						lInterface = pThis->mCharacters;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = pThis->mCharacters;
				}

				(*Characters) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Characters [%p]"), pThis, pThis->m_dwRef, CCmdTarget::FromIDispatch(pThis->mCharacters));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Characters"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_AudioOutput (IDaCtlAudioObject **AudioOutput)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_AudioOutput"), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = pThis->ConnectServer ();
	CDaAudioOutputObj *		lAudioOutput;
	IDaCtlAudioObjectPtr	lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServer)))
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
				if	(pThis->mAudioOutput == NULL)
				{
					if	(lAudioOutput = new CDaAudioOutputObj (*pThis))
					{
						pThis->mAudioOutput.Attach (lAudioOutput->GetIDispatch (FALSE));
						lInterface = pThis->mAudioOutput;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = pThis->mAudioOutput;
				}

				(*AudioOutput) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_AudioOutput [%p]"), pThis, pThis->m_dwRef, CCmdTarget::FromIDispatch(pThis->mAudioOutput));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_AudioOutput"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_SpeechInput (IDaCtlSpeechInput **SpeechInput)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_SpeechInput"), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = pThis->ConnectServer ();
	CDaSpeechInputObj *		lSpeechInput;
	IDaCtlSpeechInputPtr	lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServer)))
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
				if	(pThis->mSpeechInput == NULL)
				{
					if	(lSpeechInput = new CDaSpeechInputObj (*pThis))
					{
						pThis->mSpeechInput.Attach (lSpeechInput->GetIDispatch (FALSE));
						lInterface = pThis->mSpeechInput;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = pThis->mSpeechInput;
				}

				(*SpeechInput) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_SpeechInput [%p]"), pThis, pThis->m_dwRef, CCmdTarget::FromIDispatch(pThis->mSpeechInput));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_SpeechInput"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_PropertySheet (IDaCtlPropertySheet **PropSheet)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_PropertySheet"), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = pThis->ConnectServer ();
	CDaPropertySheetObj *	lPropertySheet;
	IDaCtlPropertySheetPtr	lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServer)))
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
				if	(pThis->mPropertySheet == NULL)
				{
					if	(lPropertySheet = new CDaPropertySheetObj (*pThis))
					{
						pThis->mPropertySheet.Attach (lPropertySheet->GetIDispatch (FALSE));
						lInterface = pThis->mPropertySheet;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = pThis->mPropertySheet;
				}

				(*PropSheet) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_PropertySheet [%p]"), pThis, pThis->m_dwRef, CCmdTarget::FromIDispatch(pThis->mPropertySheet));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_PropertySheet"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_CommandsWindow (IDaCtlCommandsWindow **CommandsWindow)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_CommandsWindow"), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = pThis->ConnectServer ();
	CDaCommandsWindowObj *	lCommandsWindow;
	IDaCtlCommandsWindowPtr	lInterface;

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServer)))
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
				if	(pThis->mCommandsWindow == NULL)
				{
					if	(lCommandsWindow = new CDaCommandsWindowObj (*pThis))
					{
						pThis->mCommandsWindow.Attach (lCommandsWindow->GetIDispatch (FALSE));
						lInterface = pThis->mCommandsWindow;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
				else
				{
					lInterface = pThis->mCommandsWindow;
				}

				(*CommandsWindow) = lInterface.Detach();
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_CommandsWindow [%p]"), pThis, pThis->m_dwRef, CCmdTarget::FromIDispatch(pThis->mCommandsWindow));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_CommandsWindow"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_Connected (VARIANT_BOOL *Connected)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Connected"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = (pThis->mServer == NULL) ? S_FALSE : S_OK;

	if	(Connected)
	{
		(*Connected) = (pThis->mServer == NULL) ? VARIANT_FALSE : VARIANT_TRUE;
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Connected"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::put_Connected (VARIANT_BOOL Connected)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::put_Connected"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(Connected)
	{
		lResult = pThis->ConnectServer ();
	}
	else
	{
		pThis->Terminate (false);
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::put_Connected"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_Suspended (VARIANT_BOOL *Suspended)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Suspended"), pThis, pThis->m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_Suspended"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::get_RaiseRequestErrors (VARIANT_BOOL *RaiseErrors)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_RaiseRequestErrors"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = pThis->mRaiseRequestErrors ? S_OK : S_FALSE;

	if	(RaiseErrors)
	{
		(*RaiseErrors) = pThis->mRaiseRequestErrors ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::get_RaiseRequestErrors"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::put_RaiseRequestErrors (VARIANT_BOOL RaiseErrors)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::put_RaiseRequestErrors"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	pThis->mRaiseRequestErrors = (RaiseErrors!=VARIANT_FALSE);

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::put_RaiseRequestErrors"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::XAgentCtl::ShowDefaultCharacterProperties (VARIANT x, VARIANT y)
{
	METHOD_PROLOGUE(CDaAgentCtl, AgentCtl)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::ShowDefaultCharacterProperties"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = pThis->ConnectServer ();

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServer)))
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
				lResult = pThis->mServer->ShowDefaultCharacterProperties ((short)lXPos, (short)lYPos, FALSE);
			}
			else
			{
				lResult = pThis->mServer->ShowDefaultCharacterProperties (0, 0, TRUE);
			}
			AllowSetForegroundWindow (GetCurrentProcessId());
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServer);
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCtl::XAgentCtl::ShowDefaultCharacterProperties"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaAgentCtl::CServerNotifySink, CCmdTarget)
	INTERFACE_PART(CDaAgentCtl::CServerNotifySink, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentCtl::CServerNotifySink, __uuidof(IDaSvrNotifySink15), NotifySink)
	INTERFACE_PART(CDaAgentCtl::CServerNotifySink, __uuidof(IDaSvrNotifySink), NotifySink)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentCtl::CServerNotifySink, NotifySink)

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl::CServerNotifySink::CServerNotifySink (CDaAgentCtl & pOwner)
:	mOwner (pOwner),
	mServerNotifyId (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CServerNotifySink::CServerNotifySink"), this, m_dwRef);
	}
#endif
	EnableAutomation ();

	if	(SUCCEEDED (TheControlApp->PreServerCall (mOwner.mServer)))
	{
		try
		{
			if	(
					(SUCCEEDED (LogComErr (LogNormal, mOwner.mServer->Register (GetControllingUnknown(), &mServerNotifyId), _T("Register Server [%p]"), mOwner.mServer.GetInterfacePtr())))
				&&	(mServerNotifyId)
				)
			{
				mOwner.mServer->AddRef ();
			}
		}
		catch AnyExceptionSilent
		TheControlApp->PostServerCall (mOwner.mServer);
	}
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaAgentCtl::CServerNotifySink::~CServerNotifySink ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CServerNotifySink::~CServerNotifySink"), this, m_dwRef);
	}
#endif
	if	(
			(mOwner.mServer != NULL)
		&&	(mServerNotifyId)
		)
	{
		try
		{
			long	lServerNotifyId = mServerNotifyId;

			mServerNotifyId = 0;

			TheControlApp->PreServerCall (mOwner.mServer);
			try
			{
				LogComErr (LogNormal, mOwner.mServer->Unregister (lServerNotifyId), _T("Unregister Server [%p] [%u]"), mOwner.mServer.GetInterfacePtr(), lServerNotifyId);
			}
			catch AnyExceptionSilent
			TheControlApp->PostServerCall (mOwner.mServer);
		}
		catch AnyExceptionSilent

		try
		{
			mOwner.mServer->Release ();
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#define KeyButtonBits(KeyMask)	((KeyMask & 0x1003) | ((KeyMask & 0x0010) >> 2))
#define KeyShiftBits(KeyMask)	(((KeyMask & 0x000C) >> 2) | ((KeyMask & 0x0020) >> 3))

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Command (long dwCommandID, IUnknown *punkUserInput)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[[%p(%u)] CDaAgentCtl::XNotifySink::Command"), &pThis->mOwner, pThis->mOwner.m_dwRef);
#endif
	CString				lActiveCharacterID;
	CDaCharacterObj *	lActiveCharacter;
	CDaUserInputObj *	lUserInput;
	IDaCtlUserInputPtr	lUserInputDispatch;

	if	(lUserInput = new CDaUserInputObj)
	{
		try
		{
			CDaCommandsObj *	lCommands;
			IDaCtlCommandsPtr	lInterface;

			lActiveCharacterID = pThis->mOwner.GetActiveCharacterID ();

			if	(
					(lActiveCharacter = pThis->mOwner.GetActiveCharacter ())
				&&	(lCommands = lActiveCharacter->GetCommands (lInterface))
				)
			{
				lUserInput->mServerCommands = lCommands->mServerObject;
			}
		}
		catch AnyExceptionSilent

		lUserInput->m_dwRef = 0;
		lUserInput->mCharacterID = lActiveCharacterID;
		lUserInput->mServerObject = punkUserInput;
		lUserInputDispatch = lUserInput->GetIDispatch(FALSE);
	}

	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireCommand (lUserInputDispatch);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::ActivateInputState (long dwCharID, long bActivated)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::ActivateInputState [%d] [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, bActivated);
#endif
	if	(bActivated == ACTIVATE_INPUTACTIVE)
	{
		if	(TheControlApp->PreNotify ())
		{
			try
			{
				pThis->mOwner.FireActivateInput (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
			catch AnyExceptionDebug
			TheControlApp->PostNotify ();
		}
	}
	else
	if	(bActivated == ACTIVATE_NOTACTIVE)
	{
		if	(TheControlApp->PreNotify ())
		{
			try
			{
				pThis->mOwner.FireDeactivateInput (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
			catch AnyExceptionDebug
			TheControlApp->PostNotify ();
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Restart (void)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::Restart"), &pThis->mOwner, pThis->mOwner.m_dwRef);
#endif
	// Unused
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Shutdown (void)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::Shutdown"), &pThis->mOwner, pThis->mOwner.m_dwRef);
#endif
	// Unused
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::VisibleState (long dwCharID, long bVisible, long dwCause)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::VisibleState [%d] [%d] cause [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, bVisible, dwCause);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			if	(bVisible)
			{
				pThis->mOwner.FireShow (pThis->mOwner.GetControlCharacterID (dwCharID), (short)dwCause);
			}
			else
			{
				pThis->mOwner.FireHide (pThis->mOwner.GetControlCharacterID (dwCharID), (short)dwCause);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Click (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[[%p(%u)] CDaAgentCtl::XNotifySink::Click [%d] [%4.4X] [%d %d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireClick (pThis->mOwner.GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::DblClick (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::DblClick [%d] [%4.4X] [%d %d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireDblClick (pThis->mOwner.GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::DragStart (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::DragStart [%d] [%4.4X] [%d %d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireDragStart (pThis->mOwner.GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::DragComplete (long dwCharID, short fwKeys, long x, long y)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireDragComplete (pThis->mOwner.GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::RequestStart (long dwRequestID)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::RequestStart [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwRequestID);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (pThis->mOwner.PutRequest (DaRequestNotifyStart, dwRequestID, S_OK));

	if	(
			(lInterface != NULL)
		&&	(TheControlApp->PreNotify ())
		)
	{
		try
		{
			pThis->mOwner.FireRequestStart (lInterface);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
#ifdef	_DEBUG_REQUEST_NOTIFY
	else
	{
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::RequestStart [%d] IGNORED"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwRequestID);
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::RequestComplete (long dwRequestID, long hrStatus)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::RequestComplete [%d] [%8.8X]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwRequestID, hrStatus);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (pThis->mOwner.PutRequest (DaRequestNotifyComplete, dwRequestID, (HRESULT)hrStatus));

	if	(
			(lInterface != NULL)
		&&	(TheControlApp->PreNotify ())
		)
	{
		try
		{
			pThis->mOwner.FireRequestComplete (lInterface);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
#ifdef	_DEBUG_REQUEST_NOTIFY
	else
	{
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::RequestComplete [%d] [%8.8X] IGNORED"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwRequestID, hrStatus);
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::BookMark (long dwBookMarkID)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::BookMark [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwBookMarkID);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireBookmark (dwBookMarkID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Idle (long dwCharID, long bStart)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::Idle [%d] [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, bStart);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			if	(bStart)
			{
				pThis->mOwner.FireIdleStart (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
			else
			{
				pThis->mOwner.FireIdleComplete (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Move (long dwCharID, long x, long y, long dwCause)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::Move [%d] [%d %d] cause [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, x, y, dwCause);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireMove (pThis->mOwner.GetControlCharacterID (dwCharID), (short)x, (short)y, (short)dwCause);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::Size (long dwCharID, long lWidth, long lHeight)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::Size [%d] [%d %d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, lWidth, lHeight);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireSize (pThis->mOwner.GetControlCharacterID (dwCharID), (short)lWidth, (short)lHeight);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::BalloonVisibleState (long dwCharID, long bVisible)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::BalloonVisibleState [%d] [%d]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, bVisible);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			if	(bVisible)
			{
				pThis->mOwner.FireBalloonShow (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
			else
			{
				pThis->mOwner.FireBalloonHide (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::HelpComplete (long dwCharID, long dwCommandID, long dwCause)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::HelpComplete"), &pThis->mOwner, pThis->mOwner.m_dwRef);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireHelpComplete (pThis->mOwner.GetControlCharacterID (dwCharID), _T(""), (short)dwCause);
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::ListeningState (long dwCharID, long bListening, long dwCause)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::ListeningState"), &pThis->mOwner, pThis->mOwner.m_dwRef);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			if	(bListening)
			{
				pThis->mOwner.FireListenStart (pThis->mOwner.GetControlCharacterID (dwCharID));
			}
			else
			{
				pThis->mOwner.FireListenComplete (pThis->mOwner.GetControlCharacterID (dwCharID), (short)dwCause);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::DefaultCharacterChange (BSTR bszGUID)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::DefaultCharacterChange [%ls]"), &pThis->mOwner, pThis->mOwner.m_dwRef, bszGUID);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireDefaultCharacterChange (CString (bszGUID));
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::AgentPropertyChange(void)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::AgentPropertyChange"), &pThis->mOwner, pThis->mOwner.m_dwRef);
#endif
	if	(TheControlApp->PreNotify ())
	{
		try
		{
			pThis->mOwner.FireAgentPropertyChange ();
		}
		catch AnyExceptionDebug
		TheControlApp->PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCtl::CServerNotifySink::XNotifySink::ActiveClientChange (long dwCharID, long lStatus)
{
	METHOD_PROLOGUE(CDaAgentCtl::CServerNotifySink, NotifySink)
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%u)] CDaAgentCtl::XNotifySink::ActiveClientChange [%d] [%8.8X]"), &pThis->mOwner, pThis->mOwner.m_dwRef, dwCharID, lStatus);
#endif
	if	(lStatus == ACTIVATE_INPUTACTIVE)
	{
		if	(TheControlApp->PreNotify ())
		{
			try
			{
				pThis->mOwner.FireActiveClientChange (pThis->mOwner.GetControlCharacterID (dwCharID), VARIANT_TRUE);
			}
			catch AnyExceptionDebug
			TheControlApp->PostNotify ();
		}
	}
	else
	if	(lStatus == ACTIVATE_NOTACTIVE)
	{
		if	(TheControlApp->PreNotify ())
		{
			try
			{
				pThis->mOwner.FireActiveClientChange (pThis->mOwner.GetControlCharacterID (dwCharID), VARIANT_FALSE);
			}
			catch AnyExceptionDebug
			TheControlApp->PostNotify ();
		}
	}
	return S_OK;
}
