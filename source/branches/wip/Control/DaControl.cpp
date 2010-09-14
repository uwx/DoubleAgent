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
#include "DaGlobalConfig.h"
#include "LocalCharacter.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "ErrorInfo.h"
#include "GuidStr.h"
#include "ThreadSecurity.h"
#include "DebugStr.h"
#include "DebugWin.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Control"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_REQUEST			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_ATTRIBUTES		(GetProfileDebugInt(_T("DebugAttributes"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogVerbose,true)&0xFFFF|LogTimeMs)
//#define	_DEBUG_PERSIST		LogNormal
//#define	_TRACE_PERSIST		LogNormal
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Control"),LogNormal,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#define	_PROP_DATA_VER			0x0101

/////////////////////////////////////////////////////////////////////////////

const UINT	DaControl::mCompleteRequestsMsg = RegisterWindowMessage (_T("1147E530-A208-11DE-ABF2-002421116FB2"));
const UINT	DaControl::mOptionsChangedMsgId = DA_BROADCAST_OPTIONS_CHANGED;
const UINT	DaControl::mDefaultCharacterChangedMsgId = DA_BROADCAST_DEFCHAR_CHANGED;

/////////////////////////////////////////////////////////////////////////////

DaControl::DaControl()
:	CInstanceAnchor (_AtlModule),
	CListeningAnchor (_AtlModule),
	mPropDataVer (_PROP_DATA_VER),
	mRaiseRequestErrors (true),
	mAutoConnect (1),
	mLocalEventNotify (this),
	mLocalCharacterStyle (CharacterStyle_SoundEffects|CharacterStyle_IdleEnabled|CharacterStyle_AutoPopupMenu|CharacterStyle_IconShown),
	mFinalReleased (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::DaControl"), this, max(m_dwRef,-1));
	}
#endif

#ifdef	_DEBUG
	if	(GetProfileDebugInt(_T("ForceStandAlone")) > 0)
	{
		mAutoConnect = 0;
	}

	if	(GetProfileDebugInt(_T("ForceSuspendPause")) > 0)
	{
		mLocalCharacterStyle |= CharacterStyle_SuspendPause;
	}
	else
	if	(GetProfileDebugInt(_T("ForceSuspendStop")) > 0)
	{
		mLocalCharacterStyle |= CharacterStyle_SuspendStop;
	}
	if	(GetProfileDebugInt(_T("ForceSuspendHide")) > 0)
	{
		if	(mLocalCharacterStyle & (CharacterStyle_SuspendPause|CharacterStyle_SuspendStop))
		{
			mLocalCharacterStyle |= CharacterStyle_SuspendHide;
		}
	}

	if	(GetProfileDebugInt(_T("ForceSmoothed")) > 0)
	{
		mLocalCharacterStyle |= CharacterStyle_Smoothed;
	}
	else
	if	(GetProfileDebugInt(_T("ForceSmoothEdges")) > 0)
	{
		mLocalCharacterStyle |= CharacterStyle_SmoothEdges;
	}

	if	(GetProfileDebugInt(_T("ForceIconShown")) < 0)
	{
		mLocalCharacterStyle &= ~CharacterStyle_IconShown;
	}
	if	(GetProfileDebugInt(_T("ForceSoundEffects")) < 0)
	{
		mLocalCharacterStyle &= ~CharacterStyle_SoundEffects;
	}
#endif

	CInstanceAnchor::mOwnerWnd = this;
	CListeningAnchor::mOwnerWnd = this;
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

	m_bAutoSize= FALSE;
	m_clrBackColor = (OLE_COLOR)(0x80000000|COLOR_WINDOW);
	m_clrBorderColor = (OLE_COLOR)(0x80000000|COLOR_WINDOWTEXT);
	m_nBorderStyle = 0;
	m_bBorderVisible = TRUE;
	m_nBorderWidth = 1;
	m_nMousePointer = 0;

	OnAutoSizeChanged ();
	OnBackColorChanged ();
	OnMousePointerChanged ();

#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceControls.SetLevel (_ATL_DEBUG_INTERFACES);
	atlTraceControls.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
#ifdef	_ATL_DEBUG_INTERFACES
	atlTraceNotImpl.SetLevel (_ATL_DEBUG_INTERFACES);
	atlTraceNotImpl.SetStatus (ATLTRACESTATUS_ENABLED);
#endif
}

DaControl::~DaControl()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::~DaControl"), this, max(m_dwRef,-1));
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::FinalRelease [%u]"), this, max(m_dwRef,-1), mFinalReleased);
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::Terminate [%u] [%p]"), this, max(m_dwRef,-1), pFinal, mServer.GetInterfacePtr());
		}
#endif
		try
		{
			mServerNotifySink->Terminate ();
			SafeFreeSafePtr (mServerNotifySink);

			DisconnectObjects (true, pFinal);

			if	(pFinal)
			{
				SafeFreeSafePtr (mCharacters);
				SafeFreeSafePtr (mSettings);
				SafeFreeSafePtr (mAudioOutput);
				SafeFreeSafePtr (mSpeechInput);
				SafeFreeSafePtr (mCommandsWindow);
				SafeFreeSafePtr (mPropertySheet);
				SafeFreeSafePtr (mCharacterFiles);
				SafeFreeSafePtr (mTTSEngines);
				SafeFreeSafePtr (mSREngines);
			}

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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::Terminate [%u] Done [%d]"), this, max(m_dwRef,-1), pFinal, _AtlModule.GetLockCount());
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

HRESULT DaControl::ConnectServer ()
{
	HRESULT	lResult = S_FALSE;

	if	(mServer == NULL)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::ConnectServer"), this, max(m_dwRef,-1));
		}
#endif
		mServerNotifySink->Terminate ();
		SafeFreeSafePtr (mServerNotifySink);

#ifdef	_WIN64
		if	(
				(mAutoConnect != 32)
			||	(LogComErr (LogNormal, lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_ACTIVATE_32_BIT_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server (32)")) == REGDB_E_CLASSNOTREG)
			)
		{
			if	(
					(LogComErr (LogNormal, lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_ACTIVATE_64_BIT_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server (64)")) == REGDB_E_CLASSNOTREG)
				&&	(mAutoConnect != 32)
				)
			{
				LogComErr (LogNormal, lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_ACTIVATE_32_BIT_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server (32)"));
			}
		}
#else
		if	(
				(mAutoConnect != 64)
			||	(LogComErr (LogNormal, lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_ACTIVATE_64_BIT_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server (64)")) == REGDB_E_CLASSNOTREG)
			)
		{
			if	(
					(LogComErr (LogNormal, lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_ACTIVATE_32_BIT_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server (32)")) == REGDB_E_CLASSNOTREG)
				&&	(mAutoConnect != 64)
				)
			{
				LogComErr (LogNormal, lResult = CoCreateInstance (__uuidof(DaServer), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_ACTIVATE_64_BIT_SERVER, __uuidof(IDaServer2), (void**)&mServer), _T("Create Server (64)"));
			}
		}
#endif		

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = CComObject <CServerNotifySink>::CreateInstance (mServerNotifySink.Free())))
			)
		{
			lResult = mServerNotifySink->Initialize (this);
		}
		if	(mServer)
		{
			mServer->put_CharacterStyle (mLocalCharacterStyle &~LocalCharacterStyle);
		}

#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::ConnectServer [%p] Sink [%d]"), this, max(m_dwRef,-1), mServer.GetInterfacePtr(), (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::DisconnectServer [%p(%d)] Force [%u] Sink [%u]"), this, max(m_dwRef,-1), mServer.GetInterfacePtr(), CoIsHandlerConnected(mServer), pForce, (mServerNotifySink ? mServerNotifySink->mServerNotifyId : 0));
	}
#endif
	DisconnectNotify (pForce);
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
				LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::DisconnectNotify [%u] [%p]"), this, max(m_dwRef,-1), pForce, mServer.GetInterfacePtr());
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
				if	(CProxy_DaCtlEvents2<DaControl>::m_vec.GetSize() > 0)
				{
					LogMessage (_LOG_INSTANCE, _T("--- _DaCtlEvents2 Connections [%d] ---"), CProxy_DaCtlEvents2<DaControl>::m_vec.GetSize());
				}
				for	(lNdx = 0; lNdx < CProxy_DaCtlEvents2<DaControl>::m_vec.GetSize(); lNdx++)
				{
					if	(lConnection = CProxy_DaCtlEvents2<DaControl>::m_vec.GetAt (lNdx))
					{
						LogMessage (_LOG_INSTANCE, _T("--- _DaCtlEvents2 Connection [%d] [%p] ---"), lNdx, lConnection);
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

void DaControl::DisconnectObjects (bool pTerminate, bool pFinal)
{
	try
	{
		DaCtlCharacters *		lCharacters;
		DaCtlSettings *			lSettings;
		DaCtlAudioOutput *		lAudioOutput;
		DaCtlSpeechInput *		lSpeechInput;
		DaCtlCommandsWindow *	lCommandsWindow;
		DaCtlPropertySheet *	lPropertySheet;
		DaCtlCharacterFiles *	lCharacterFiles;
		DaCtlTTSEngines *		lTTSEngines;
		DaCtlSREngines *		lSREngines;

		SafeFreeSafePtr (mControlCharacter);
		CListeningAnchor::Shutdown ();

		if	(
				(mCharacters != NULL)
			&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lCharacters->Terminate (pFinal);
			}
			else
			{
				lCharacters->Disconnect (pFinal);
			}
		}

		if	(
				(mSettings != NULL)
			&&	(lSettings = dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lSettings->Terminate (pFinal);
			}
			else
			{
				lSettings->Disconnect (pFinal);
			}
		}

		if	(
				(mAudioOutput != NULL)
			&&	(lAudioOutput = dynamic_cast <DaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lAudioOutput->Terminate (pFinal);
			}
			else
			{
				lAudioOutput->Disconnect (pFinal);
			}
		}

		if	(
				(mSpeechInput != NULL)
			&&	(lSpeechInput = dynamic_cast <DaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lSpeechInput->Terminate (pFinal);
			}
			else
			{
				lSpeechInput->Terminate (pFinal);
			}
		}

		if	(
				(mCommandsWindow != NULL)
			&&	(lCommandsWindow = dynamic_cast <DaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lCommandsWindow->Terminate (pFinal);
			}
			else
			{
				lCommandsWindow->Disconnect (pFinal);
			}
		}

		if	(
				(mPropertySheet != NULL)
			&&	(lPropertySheet = dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lPropertySheet->Terminate (pFinal);
			}
			else
			{
				lPropertySheet->Disconnect (pFinal);
			}
		}

		if	(
				(mCharacterFiles != NULL)
			&&	(lCharacterFiles = dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lCharacterFiles->Terminate (pFinal);
			}
			else
			{
				lCharacterFiles->Disconnect (pFinal);
			}
		}

		if	(
				(mTTSEngines != NULL)
			&&	(lTTSEngines = dynamic_cast <DaCtlTTSEngines *> (mTTSEngines.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lTTSEngines->Terminate (pFinal);
			}
			else
			{
				lTTSEngines->Disconnect (pFinal);
			}
		}

		if	(
				(mSREngines != NULL)
			&&	(lSREngines = dynamic_cast <DaCtlSREngines *> (mSREngines.GetInterfacePtr()))
			)
		{
			if	(pTerminate)
			{
				lSREngines->Terminate (pFinal);
			}
			else
			{
				lSREngines->Disconnect (pFinal);
			}
		}

		TerminateRequests (pFinal);
	}
	catch AnyExceptionDebug
}

void DaControl::ConnectObjects ()
{
	try
	{
		DaCtlCharacters *		lCharacters;
		DaCtlSettings *			lSettings;
		DaCtlAudioOutput *		lAudioOutput;
		DaCtlSpeechInput *		lSpeechInput;
		DaCtlCommandsWindow *	lCommandsWindow;
		DaCtlPropertySheet *	lPropertySheet;
		DaCtlCharacterFiles *	lCharacterFiles;
		DaCtlTTSEngines *		lTTSEngines;
		DaCtlSREngines *		lSREngines;

		if	(
				(mCharacters != NULL)
			&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
			)
		{
			lCharacters->SetOwner (this);
		}

		if	(
				(mSettings != NULL)
			&&	(lSettings = dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()))
			)
		{
			lSettings->SetOwner (this);
		}

		if	(
				(mAudioOutput != NULL)
			&&	(lAudioOutput = dynamic_cast <DaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()))
			)
		{
			lAudioOutput->SetOwner (this);
		}

		if	(
				(mSpeechInput != NULL)
			&&	(lSpeechInput = dynamic_cast <DaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()))
			)
		{
			lSpeechInput->SetOwner (this);
		}

		if	(
				(mCommandsWindow != NULL)
			&&	(lCommandsWindow = dynamic_cast <DaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()))
			)
		{
			lCommandsWindow->SetOwner (this);
		}

		if	(
				(mPropertySheet != NULL)
			&&	(lPropertySheet = dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()))
			)
		{
			lPropertySheet->SetOwner (this);
		}

		if	(
				(mCharacterFiles != NULL)
			&&	(lCharacterFiles = dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()))
			)
		{
			lCharacterFiles->SetOwner (this);
		}

		if	(
				(mTTSEngines != NULL)
			&&	(lTTSEngines = dynamic_cast <DaCtlTTSEngines *> (mTTSEngines.GetInterfacePtr()))
			)
		{
			lTTSEngines->SetOwner (this);
		}

		if	(
				(mSREngines != NULL)
			&&	(lSREngines = dynamic_cast <DaCtlSREngines *> (mSREngines.GetInterfacePtr()))
			)
		{
			lSREngines->SetOwner (this);
		}
	}
	catch AnyExceptionDebug
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
				&&	(lCharacter->GetCharID() == pServerCharID)
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

void DaControl::CharacterLoaded (int pCharacterCount, DaCtlCharacter * pCharacter)
{
	if	(
			(pCharacterCount > 0)
		&&	(!mServer)
		&&	(!CListeningAnchor::IsStarted ())
		)
	{
		CListeningAnchor::Startup (GetMsgPostingWnd ());
	}
}

void DaControl::CharacterUnloaded (int pCharacterCount, DaCtlCharacter * pCharacter)
{
	if	(pCharacterCount <= 0)
	{
		CListeningAnchor::Shutdown ();
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IDaCtlRequest * DaControl::PutRequest (DaRequestCategory pCategory, long pReqID, HRESULT pResult)
{
	IDaCtlRequestPtr			lInterface;
	DaCtlRequest *				lRequest = NULL;
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
			LogMessage (_DEBUG_REQUEST, _T("  Reuse Request   [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
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
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)(%d)] Status [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyComplete) != 0)
			{
				lRequest->mStatus = RequestStatus_Success;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)(%d)] Status [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr());
#endif
			}
			else
			if	((pCategory & DaRequestNotifyStart) != 0)
			{
				lRequest->mStatus = RequestStatus_InProgress;
#ifdef	_DEBUG_REQUEST
				LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)(%d)] Status [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr());
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
			LogMessage (_DEBUG_REQUEST, _T("    Request       [%p(%d)(%d)] Status [%s] Category [%s] Deferred [%p]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr(), m_hWnd);
		}
#endif

		if	(GetMsgPostingWnd ())
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
			LogMessage (_DEBUG_REQUEST, _T("[%p(%d)] Complete Requests [%d] Idle [%u]"), this, max(m_dwRef,-1), mActiveRequests.GetCount(), pIdleTime);
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
					LogMessage (_DEBUG_REQUEST, _T("  Release Request [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
					mActiveRequests.RemoveKey (lRequest->mReqID);
					mCompletedRequests.Add (lRequest);
					lRequest->Release ();
				}
				else
				{
					if	((lRequest->mCategory & DaRequestNotifyEnabled) == 0)
					{
#ifdef	_DEBUG_REQUEST
						if	(lRequest->mCategory & (DaRequestNotifyStart|DaRequestNotifyComplete))
						{
							LogMessage (_DEBUG_REQUEST, _T("  Deferred Request [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
						}
#endif
						lRequest->mCategory = (DaRequestCategory)(lRequest->mCategory & ~(DaRequestNotifyStart|DaRequestNotifyComplete));
					}
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
							LogMessage (_DEBUG_REQUEST, _T("  Queued Request [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
#ifdef	_DEBUG_NOTIFY
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CompleteRequests::RequestStart [%d]"), this, max(m_dwRef,-1), lRequest->mReqID);
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
							LogMessage (_DEBUG_REQUEST, _T("  Queued Request [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
#ifdef	_DEBUG_NOTIFY
							LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CompleteRequests::RequestComplete [%d]"), this, max(m_dwRef,-1), lRequest->mReqID);
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
						LogMessage (_DEBUG_REQUEST, _T("  Release Request [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
#endif
						mActiveRequests.RemoveKey (lRequest->mReqID);
						mCompletedRequests.Add (lRequest);
						lRequest->Release ();
					}
#ifdef	_DEBUG_REQUEST
					else
					if	(lRequest->mStatus == RequestStatus_Pending)
					{
						LogMessage (_DEBUG_REQUEST, _T("  Pending Request [%p(%d)(%d)] Status [%s] Category [%s]"), lRequest, lRequest->mReqID, lRequest->m_dwRef, lRequest->StatusStr(), lRequest->CategoryStr());
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
			LogMessage (_DEBUG_REQUEST, _T("[%p(%d)] Terminate [%u] Requests [%d %d]"), this, max(m_dwRef,-1), pFinal, mActiveRequests.GetCount(), mCompletedRequests.GetCount());
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
		LogMessage (_DEBUG_REQUEST, _T("  New Request     [%p(%d)(%d)] Status [%s] Category [%s] Count [%d]"), pRequest, pRequest->mReqID, pRequest->m_dwRef, pRequest->StatusStr(), pRequest->CategoryStr(), mActiveRequests.GetCount());
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
		LogMessage (_DEBUG_REQUEST, _T("  Deleted Request [%p(%d)(%d)] Status [%s] Category [%s] Count [%d]"), pRequest, pRequest->mReqID, pRequest->m_dwRef, pRequest->StatusStr(), pRequest->CategoryStr(), mActiveRequests.GetCount());
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaControl::AddListeningTimer (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink)
{
	if	(GetMsgPostingWnd ())
	{
		return AddTimerNotify (mMsgPostingWnd->m_hWnd, pTimerId, pInterval, pNotifySink);
	}
	return false;
}

bool DaControl::DelListeningTimer (UINT_PTR pTimerId)
{
	HWND	lTimerWnd = NULL;

	if	(
			(mMsgPostingWnd)
		&&	(mMsgPostingWnd->IsWindow ())
		)
	{
		lTimerWnd = mMsgPostingWnd->m_hWnd;
	}
	return DelTimerNotify (lTimerWnd, pTimerId);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT DaControl::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(mTimerNotifies.OnTimer ((UINT_PTR)wParam))
	{
		bHandled = TRUE;
	}
	return 0;
}

LRESULT DaControl::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(_AtlModule.CListeningGlobal::OnHotKey (wParam, lParam))
	{
		bHandled = TRUE;
	}
	return 0;
}

LRESULT DaControl::OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_ACTIVE
	LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)] DaControl::OnActivateApp [%u %u]"), this, max(m_dwRef,-1), wParam, _AtlModule.VerifyAppActive());
#endif
	_AtlModule._AppActivated (wParam?true:false);
	bHandled = FALSE;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT DaControl::OnBroadcastOptionsChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::OnBroadcastOptionsChanged"), this, max(m_dwRef,-1));
#endif
	_AtlModule._OptionsChanged ();
	bHandled = TRUE;
	return 0;
}

LRESULT DaControl::OnBroadcastDefaultCharacterChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::OnBroadcastDefaultCharacterChanged"), this, max(m_dwRef,-1));
#endif
	_AtlModule.CEventGlobal::_DefaultCharacterChanged ();
	bHandled = TRUE;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HWND DaControl::CreateControlWindow(HWND hWndParent, RECT& rcPos)
{
	HWND	lWindow = NULL;
	DWORD	lStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	DWORD	lExStyle = 0;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaControl::CreateControlWindow [%p] [%p] [%s] IsDesigning [%u] IsAppActive [%u %u]"), this, max(m_dwRef,-1), m_spInPlaceSite.p, hWndParent, FormatRect(rcPos), IsDesigning(), _AtlModule.IsAppActive(), _AtlModule.VerifyAppActive());
	}
#endif
	if	(IsDesigning ())
	{
		lStyle |= WS_VISIBLE;
	}
	CalcWindowStyles (lStyle, lExStyle);
	if	(lWindow = CAgentCharacterWnd::Create (hWndParent, rcPos, NULL, lStyle, lExStyle))
	{
		GetMsgPostingWnd ();
	}
	return lWindow;
}

HRESULT DaControl::CanCreateControlWindow ()
{
	HRESULT	lResult = S_OK;

	try
	{
		CComQIPtr<IOleInPlaceSite>	lInPlaceSite (m_spInPlaceSite);
		HWND						lParentWnd = NULL;

		if	(!lInPlaceSite)
		{
			lInPlaceSite = m_spClientSite;
		}
		if	(
				(!lInPlaceSite)
			||	(FAILED (lInPlaceSite->GetWindow (&lParentWnd)))
			||	(!lParentWnd)
			)
		{
			lResult = E_UNEXPECTED;
		}
	}
	catch AnyExceptionSilent

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::OnPreVerbInPlaceActivate()
{
	HRESULT	lResult = CanCreateControlWindow ();
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive(_DEBUG_ACTIVE))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_ACTIVE), lResult, _T("[%p(%d)] DaControl::OnPreVerbInPlaceActivate NegotiatedWnd [%d] IsDesigning [%u]"), this, max(m_dwRef,-1), m_bNegotiatedWnd, IsDesigning());
	}
#endif
	return lResult;
}

HRESULT DaControl::OnPreVerbUIActivate()
{
	HRESULT	lResult = CanCreateControlWindow ();
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive(_DEBUG_ACTIVE))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_ACTIVE), lResult, _T("[%p(%d)] DaControl::OnPreVerbUIActivate NegotiatedWnd [%d] IsDesigning [%u]"), this, max(m_dwRef,-1), m_bNegotiatedWnd, IsDesigning());
	}
#endif
	return lResult;
}

HRESULT DaControl::OnPreVerbShow()
{
	HRESULT	lResult = CanCreateControlWindow ();
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive(_DEBUG_ACTIVE))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_ACTIVE), lResult, _T("[%p(%d)] DaControl::OnPreVerbShow NegotiatedWnd [%d] IsDesigning [%u]"), this, max(m_dwRef,-1), m_bNegotiatedWnd, IsDesigning());
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::OnPostVerbUIActivate ()
{
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive(_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)] DaControl::OnPostVerbUIActivate NegotiatedWnd [%d] IsDesigning [%u]"), this, max(m_dwRef,-1), m_bNegotiatedWnd, IsDesigning());
	}
#endif
	if	(mControlCharacter)
	{
		SetLastActive ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT DaControl::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(
			(!mControlCharacter)
		&&	(!IsDesigning ())
		)
	{
		LPWINDOWPOS	lWindowPos = (LPWINDOWPOS)lParam;

		if	(
				(lWindowPos->flags & SWP_SHOWWINDOW)
			||	(GetStyle () & WS_VISIBLE)
			)
		{
			lWindowPos->flags &= ~SWP_SHOWWINDOW;
			lWindowPos->flags |= SWP_HIDEWINDOW;
		}
	}
	return 0;
}

LRESULT DaControl::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if	(
			(wParam)
		&&	(!mControlCharacter)
		&&	(!IsDesigning ())
		)
	{
		if	(IsWindow ())
		{
			ShowWindow (SW_HIDE);
		}
		wParam = FALSE;
	}
	bHandled = TRUE;
	return DefWindowProc (uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
//
//	Only called when message map dynamic chainging is disabled (i.e. no ControlCharacter)
//
HRESULT DaControl::OnDraw(ATL_DRAWINFO& di)
{
	CRect			lBounds = *(RECT*)di.prcBounds;
	COLORREF		lBkColor = GetOleColor (m_clrBackColor);
	CBrushHandle	lBrush = CreateSolidBrush (lBkColor);

	if	(!mIcon)
	{
		mIcon = (HICON) LoadImage (_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDI_DOUBLEAGENT), IMAGE_ICON, lBounds.Width(), lBounds.Height(), LR_DEFAULTCOLOR);
	}
	::DrawIconEx (di.hdcDraw, lBounds.left, lBounds.top, mIcon, lBounds.Width(), lBounds.Height(), 0, lBrush, DI_NORMAL);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

bool DaControl::EraseWindow (HDC pDC, COLORREF pBkColor)
{
	if	(
			(mControlCharacter)
		&&	(IsCharShown ())
		)
	{
		return CAgentCharacterWnd::EraseWindow (pDC, pBkColor);
	}
	else
	{
		CRect	lClientRect;

		GetClientRect (&lClientRect);
		FillSolidRect (pDC, &lClientRect, pBkColor);
		return true;
	}
}

bool DaControl::PaintWindow (HDC pDC)
{
	if	(
			(mControlCharacter)
		&&	(IsCharShown ())
		)
	{
		return CAgentCharacterWnd::PaintWindow (pDC);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT DaControl::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(
			(wParam)
		&&	(GetStyle () & WS_BORDER)
		&&	(m_nBorderWidth > 1)
		&&	(
				(m_nBorderWidth != GetSystemMetrics (SM_CXBORDER))
			||	(m_nBorderWidth != GetSystemMetrics (SM_CYBORDER))
			)
		)
	{
		LRESULT				lResult;
		LPNCCALCSIZE_PARAMS	lParms = (LPNCCALCSIZE_PARAMS) lParam;

		if	(!CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult))
		{
			lResult = DefWindowProc (uMsg, wParam, lParam);
		}
		lParms->rgrc[0].left += m_nBorderWidth - GetSystemMetrics (SM_CXBORDER);
		lParms->rgrc[0].right -= m_nBorderWidth - GetSystemMetrics (SM_CXBORDER);
		lParms->rgrc[0].top += m_nBorderWidth - GetSystemMetrics (SM_CYBORDER);
		lParms->rgrc[0].bottom -= m_nBorderWidth - GetSystemMetrics (SM_CYBORDER);

		bHandled = TRUE;
		return lResult;
	}
	return 0;
}

LRESULT DaControl::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(
			(GetStyle () & WS_BORDER)
		&&	(m_nBorderWidth > 1)
		&&	(
				(m_nBorderWidth != GetSystemMetrics (SM_CXBORDER))
			||	(m_nBorderWidth != GetSystemMetrics (SM_CYBORDER))
			||	(m_clrBorderColor != (OLE_COLOR)(0x80000000|COLOR_WINDOWTEXT))
			)
		)
	{
		HDC	lDC;

		if	(lDC = GetWindowDC ())
		{
			SendMessage (WM_PRINT, (WPARAM)lDC, PRF_NONCLIENT);
			bHandled = TRUE;
		}
	}
	return 0;
}

LRESULT DaControl::OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;

	bHandled = FALSE;
	if	(
			(lParam & PRF_NONCLIENT)
		&&	(GetStyle () & WS_BORDER)
		&&	(m_nBorderWidth > 1)
		&&	(
				(m_nBorderWidth != GetSystemMetrics (SM_CXBORDER))
			||	(m_nBorderWidth != GetSystemMetrics (SM_CYBORDER))
			||	(m_clrBorderColor != (OLE_COLOR)(0x80000000|COLOR_WINDOWTEXT))
			)
		)
	{
		HDC				lDC;
		COLORREF		lBorderColor = GetOleColor (m_clrBorderColor);
		CBrushHandle	lBrush = CreateSolidBrush (lBorderColor);
		CRect			lWindowRect;
		CRect			lClientRect;

		GetWindowRect (&lWindowRect);
		GetClientRect (&lClientRect);
		ClientToScreen (&lClientRect);
		lClientRect.OffsetRect (-lWindowRect.left, -lWindowRect.top);
		lWindowRect.OffsetRect (-lWindowRect.left, -lWindowRect.top);

		if	(lDC = (HDC)wParam)
		{
			SaveDC (lDC);
			ExcludeClipRect (lDC, lClientRect.left, lClientRect.top, lClientRect.right, lClientRect.bottom);
			FillRect (lDC, &lWindowRect, lBrush);
			RestoreDC (lDC, -1);
			ReleaseDC (lDC);
		}

		lParam &= ~PRF_NONCLIENT;
		if	(
				(lParam)
			&&	(!CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult))
			)
		{
			lResult = DefWindowProc (uMsg, wParam, lParam);
		}
		bHandled = TRUE;
	}
	return lResult;
}

LRESULT DaControl::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if	(
			(mCursor.GetSafeHandle())
		&&	(!IsDesigning ())
		)
	{
		SetCursor (mCursor);
		bHandled = TRUE;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaControl::IsDesigning ()
{
	bool	lRet = false;

	try
	{
		BOOL	lUserMode = FALSE;
		if	(
				(SUCCEEDED (LogComErr (LogVerbose, GetAmbientUserMode (lUserMode))))
			&&	(!lUserMode)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool DaControl::CalcWindowStyles (DWORD & pStyle, DWORD & pExStyle)
{
	bool	lRet = false;

	if	(m_bBorderVisible)
	{
		if	(m_nBorderStyle == 0)
		{
			if	(pStyle & WS_BORDER)
			{
				pStyle &= ~WS_BORDER;
				lRet = true;
			}
			if	(!(pExStyle & WS_EX_STATICEDGE))
			{
				pExStyle |= WS_EX_STATICEDGE;
				lRet = true;
			}
		}
		else
		if	(m_nBorderWidth > 0)
		{
			if	(!(pStyle & WS_BORDER))
			{
				pStyle |= WS_BORDER;
				lRet = true;
			}
			if	(pExStyle & WS_EX_STATICEDGE)
			{
				pExStyle &= ~WS_EX_STATICEDGE;
				lRet = true;
			}
		}
		else
		{
			if	(pStyle & WS_BORDER)
			{
				pStyle &= ~WS_BORDER;
				lRet = true;
			}
			if	(pExStyle & WS_EX_STATICEDGE)
			{
				pExStyle &= ~WS_EX_STATICEDGE;
				lRet = true;
			}
		}
	}
	else
	{
		if	(pStyle & WS_BORDER)
		{
			pStyle &= ~WS_BORDER;
			lRet = true;
		}
		if	(pExStyle & WS_EX_STATICEDGE)
		{
			pExStyle &= ~WS_EX_STATICEDGE;
			lRet = true;
		}
	}
	return lRet;
}

void DaControl::UpdateWindowStyles ()
{
	if	(IsWindow ())
	{
		DWORD	lOldStyle = GetStyle ();
		DWORD	lOldExStyle = GetExStyle ();
		DWORD	lNewStyle = lOldStyle;
		DWORD	lNewExStyle = lOldExStyle;

		if	(CalcWindowStyles (lNewStyle, lNewExStyle))
		{
			ModifyStyle (lOldStyle, lNewStyle, SWP_FRAMECHANGED);
			ModifyStyleEx (lOldExStyle, lNewExStyle, SWP_FRAMECHANGED);

			if	(
					(mAutoSize)
				&&	(mControlCharacter)
				&&	(!IsDesigning ())
				)
			{
				AutoSizeWindow ();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

COLORREF DaControl::GetOleColor (OLE_COLOR pColor)
{
	COLORREF lColor = (pColor & 0x80000000) ? GetSysColor (pColor & 0x00FFFFFF) : (pColor & 0x00FFFFFF);
	OleTranslateColor (pColor, NULL, &lColor);
	return lColor;
}

/////////////////////////////////////////////////////////////////////////////

HWND DaControl::GetMsgPostingWnd ()
{
	if	(
			(mMsgPostingWnd)
		&&	(mMsgPostingWnd->IsWindow ())
		)
	{
		return mMsgPostingWnd->m_hWnd;
	}
	else
	if	(mMsgPostingWnd = new CMsgPostingWnd <DaControl> (*this))
	{
		CThreadSecurity::AllowUiPiMessage (mOptionsChangedMsgId);
		CThreadSecurity::AllowUiPiMessage (mDefaultCharacterChangedMsgId);

		// Recreate as normal window to allow it to receive broadcast messages
		mMsgPostingWnd->DestroyWindow();
		mMsgPostingWnd->Create (NULL);
		return mMsgPostingWnd->m_hWnd;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::GetMiscStatus (DWORD dwAspect, DWORD *pdwStatus)
{
	if	(pdwStatus)
	{
		(*pdwStatus) = _GetMiscStatus ();
		if	(mControlCharacter != NULL)
		{
			(*pdwStatus) &= ~(DWORD)OLEMISC_ONLYICONIC;
			(*pdwStatus) |= (DWORD)OLEMISC_STATIC;
		}
		return (dwAspect == DVASPECT_CONTENT) ? S_OK : S_FALSE;
	}
	else
	{
		return E_POINTER;
	}
}

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
#ifdef	_DEBUG
static void DumpStream (UINT pLogLevel, LPSTREAM pStm, LONGLONG pOffset = 0)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			IStreamPtr			lMemStream;
			CGlobalHandle		lBuffer;
			tS <STATSTG>		lStat;
			LARGE_INTEGER		lSeekTo = {0,0};
			LARGE_INTEGER		lSeekRestore;
			ULARGE_INTEGER		lSeekPos;

			if	(pStm)
			{
				if	(SUCCEEDED (pStm->Stat (&lStat, STATFLAG_NONAME)))
				{
					pStm->Seek (lSeekTo, STREAM_SEEK_CUR, &lSeekPos);
					lSeekRestore.QuadPart = (LONGLONG)lSeekPos.QuadPart;
					lSeekTo.QuadPart = pOffset;
					pStm->Seek (lSeekTo, STREAM_SEEK_CUR, &lSeekPos);

					LogMessage (pLogLevel, _T("  Stream [%p] Size [%I64u] at [%I64u] for [%I64d]"), pStm, lStat.cbSize.QuadPart, lSeekPos.QuadPart, lStat.cbSize.QuadPart-lSeekPos.QuadPart);
					lStat.cbSize.QuadPart -= lSeekPos.QuadPart;

					if	(
							((LONGLONG)lStat.cbSize.QuadPart > 0)
						&&	(lStat.cbSize.LowPart > 0)
						&&	(lBuffer = GlobalAlloc (GMEM_FIXED, lStat.cbSize.LowPart))
						&&	(SUCCEEDED (LogComErr (pLogLevel, CreateStreamOnHGlobal (lBuffer, FALSE, &lMemStream))))
						&&	(SUCCEEDED (LogComErr (pLogLevel, pStm->CopyTo (lMemStream, lStat.cbSize, NULL, NULL))))
						)
					{
						LogDump (pLogLevel, GlobalLock(lBuffer), lStat.cbSize.LowPart, _T("  "));
					}

					pStm->Seek (lSeekRestore, STREAM_SEEK_SET, NULL);
				}
				else
				{
					LogMessage (pLogLevel, _T("  Stream [%p] <unknown>"), pStm);
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("  Stream [%p]"), pStm);
			}
		}
		catch AnyExceptionSilent
	}
}
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::IPersistPropertyBag_Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, const ATL_PROPMAP_ENTRY* pMap)
{
#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogMessage (_DEBUG_PERSIST, _T("[%p(%d)] DaControl::IPersistPropertyBag_Load"), this, max(m_dwRef,-1));
	}
#endif

	HRESULT	lResult = IPersistPropertyBagImpl<DaControl>::IPersistPropertyBag_Load (pPropBag, pErrorLog, pMap);

#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_PERSIST), lResult, _T("[%p(%d)] DaControl::IPersistPropertyBag_Load [%4.4X]"), this, max(m_dwRef,-1), mPropDataVer);
	}
#endif
	return lResult;
}

HRESULT DaControl::IPersistPropertyBag_Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties, const ATL_PROPMAP_ENTRY* pMap)
{
#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogMessage (_DEBUG_PERSIST, _T("[%p(%d)] DaControl::IPersistPropertyBag_Save"), this, max(m_dwRef,-1));
	}
#endif

	mPropDataVer = _PROP_DATA_VER;
	HRESULT lResult = IPersistPropertyBagImpl<DaControl>::IPersistPropertyBag_Save (pPropBag, fClearDirty, fSaveAllProperties, pMap);

#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_PERSIST), lResult, _T("[%p(%d)] DaControl::IPersistPropertyBag_Save [%4.4X]"), this, max(m_dwRef,-1), mPropDataVer);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaControl::IPersistStreamInit_Load(LPSTREAM pStm, const ATL_PROPMAP_ENTRY* pMap)
{
#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogMessage (_DEBUG_PERSIST, _T("[%p(%d)] DaControl::IPersistStreamInit_Load [%p]"), this, max(m_dwRef,-1), pStm);
	}
#endif
#ifdef	_TRACE_PERSIST
	if	(LogIsActive (_TRACE_PERSIST))
	{
		DumpStream (_TRACE_PERSIST, pStm);
	}
#endif
	HRESULT			lResult = S_OK;
	tS <STATSTG>	lStat;

	if	(
			(pStm)
		&&	(
				(FAILED (pStm->Stat (&lStat, STATFLAG_NONAME)))
			||	(lStat.cbSize.QuadPart == 0)
			)
		)
	{
		lResult = S_FALSE;
	}
	else
	{
		LARGE_INTEGER			lSeekTo = {0,0};
		ULARGE_INTEGER			lSeekPos;
		IStreamPtr				lStreamCopy;
		CGlobalHandle			lStreamBuffer;
		ULARGE_INTEGER			lBufferSize = {24,0}; // Stream size for version 1.0
		const WORD *			lBufferWords;

		if	(SUCCEEDED (pStm->Seek (lSeekTo, STREAM_SEEK_CUR, &lSeekPos)))
		{
			lBufferSize.QuadPart = min (lBufferSize.QuadPart, (LONGLONG)lStat.cbSize.QuadPart - (LONGLONG)lSeekPos.QuadPart);

			if	(
					(lBufferSize.LowPart > 0)
				&&	(lStreamBuffer = GlobalAlloc (GMEM_FIXED, lBufferSize.LowPart))
				&&	(lBufferWords = (LPWORD) GlobalLock (lStreamBuffer))
				&&	(SUCCEEDED (LogComErr (LogNormal, CreateStreamOnHGlobal (lStreamBuffer, FALSE, &lStreamCopy))))
				)
			{
				lResult = LogComErr (LogNormal, pStm->CopyTo (lStreamCopy, lBufferSize, NULL, NULL));
				lSeekTo.QuadPart = (LONGLONG)lSeekPos.QuadPart;
				pStm->Seek (lSeekTo, STREAM_SEEK_SET, NULL);

				if	(SUCCEEDED (lResult))
				{
					if	(
							(((DWORD*)lBufferWords)[0] == _ATL_VER)
						&&	(lBufferWords[2] == mPropDataVer)
						)
					{
						lResult = IPersistStreamInitImpl<DaControl>::IPersistStreamInit_Load (pStm, pMap);
					}
					else
					{
						if	(lBufferSize.LowPart >= 12) // Minimum stream size for MsAgent Control
						{
							m_sizeExtent.cx = ((long*)lBufferWords)[0];
							m_sizeExtent.cy = ((long*)lBufferWords)[1];
						}
						lResult = S_FALSE;
					}
				}
				else
				{
					lResult = S_FALSE;
				}
			}
			else
			{
				lResult = S_FALSE;
			}

			if	(lResult == S_FALSE)
			{
				mPropDataVer = 0;
				lSeekTo.QuadPart = lBufferSize.QuadPart;
				pStm->Seek (lSeekTo, STREAM_SEEK_CUR, NULL);
			}
		}
	}

#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_PERSIST), lResult, _T("[%p(%d)] DaControl::IPersistStreamInit_Load [%4.4X]"), this, max(m_dwRef,-1), mPropDataVer);
	}
#endif
	if	(lResult == S_FALSE)
	{
		lResult = S_OK;
	}
	return lResult;
}

HRESULT DaControl::IPersistStreamInit_Save(LPSTREAM pStm, BOOL fClearDirty, const ATL_PROPMAP_ENTRY* pMap)
{
#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogMessage (_DEBUG_PERSIST, _T("[%p(%d)] DaControl::IPersistStreamInit_Save [%p] [%u]"), this, max(m_dwRef,-1), pStm, fClearDirty);
	}
#endif
#ifdef	_TRACE_PERSIST
	LARGE_INTEGER	lSeekTo = {0,0};
	ULARGE_INTEGER	lSeekStart;
	ULARGE_INTEGER	lSeekEnd;
	if	(LogIsActive (_TRACE_PERSIST))
	{
		pStm->Seek (lSeekTo, STREAM_SEEK_CUR, &lSeekStart);
	}
#endif

	mPropDataVer = _PROP_DATA_VER;
	HRESULT lResult = IPersistStreamInitImpl<DaControl>::IPersistStreamInit_Save (pStm, fClearDirty, pMap);

#ifdef	_TRACE_PERSIST
	if	(LogIsActive (_TRACE_PERSIST))
	{
		pStm->Seek (lSeekTo, STREAM_SEEK_CUR, &lSeekEnd);
		LogMessage (_TRACE_PERSIST, _T("  Saved [%I64d] at [%I64u]"), (LONGLONG)lSeekEnd.QuadPart-(LONGLONG)lSeekStart.QuadPart, lSeekStart.QuadPart);
		DumpStream (_TRACE_PERSIST, pStm, (LONGLONG)lSeekStart.QuadPart-(LONGLONG)lSeekEnd.QuadPart);
	}
#endif
#ifdef	_DEBUG_PERSIST
	if	(LogIsActive (_DEBUG_PERSIST))
	{
		LogComErrAnon (MinLogLevel(LogAlways,_DEBUG_PERSIST), lResult, _T("[%p(%d)] DaControl::IPersistStreamInit_Save [%4.4X]"), this, max(m_dwRef,-1), mPropDataVer);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_Connected (VARIANT_BOOL *Connected)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Connected"), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Connected"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_Connected (VARIANT_BOOL Connected)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::put_Connected [%d]"), this, max(m_dwRef,-1), Connected);
#endif
	HRESULT	lResult = S_OK;

	if	((Connected == VARIANT_FALSE) != (mServer == NULL))
	{
		DisconnectObjects (false, false);
		DisconnectObjects (false, true);
	}

	if	(Connected)
	{
		lResult = ConnectServer ();
	}
	else
	{
		lResult = DisconnectServer (false);
	}

	if	(lResult == S_OK)
	{
		ConnectObjects ();
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_Connected"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_AutoConnect (short *AutoConnect)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_AutoConnect"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = mAutoConnect ? S_OK : S_FALSE;

	if	(AutoConnect)
	{
#ifdef	_WIN64
		(*AutoConnect) = (mAutoConnect==32) ? 32 : (mAutoConnect) ? 64 : 0;
#else	
		(*AutoConnect) = (mAutoConnect==64) ? 64 : (mAutoConnect) ? 32 : 0;
#endif		
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_AutoConnect"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_AutoConnect (short AutoConnect)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::put_AutoConnect [%d]"), this, max(m_dwRef,-1), AutoConnect);
#endif
	HRESULT	lResult = S_OK;

	mAutoConnect = (AutoConnect==32) ? 32 : (AutoConnect==64) ? 64 : (AutoConnect) ? 1 : 0;

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_Connected"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_RaiseRequestErrors (VARIANT_BOOL *RaiseErrors)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES_NOT
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::get_RaiseRequestErrors"), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_RaiseRequestErrors"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_RaiseRequestErrors (VARIANT_BOOL RaiseErrors)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::put_RaiseRequestErrors [%d]"), this, max(m_dwRef,-1), RaiseErrors);
#endif
	HRESULT	lResult = S_OK;

	mRaiseRequestErrors = (RaiseErrors!=VARIANT_FALSE);

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_RaiseRequestErrors"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_Suspended (VARIANT_BOOL *Suspended)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Suspended"), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Suspended"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_Characters (IDaCtlCharacters2 **Characters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Characters"), this, max(m_dwRef,-1));
#endif
	HRESULT							lResult = S_OK;
	CComObject <DaCtlCharacters> *	lCharacters = NULL;
	IDaCtlCharacters2Ptr			lInterface;

	if	(Characters == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(mAutoConnect)
			||	(mServer != NULL)
			)
		{
			lResult = _AtlModule.PreServerCall (mServer);
		}
		if	(SUCCEEDED (lResult))
		{
			try
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
			catch AnyExceptionDebug

			if	(
					(mAutoConnect)
				||	(mServer != NULL)
				)
			{
				_AtlModule.PostServerCall (mServer);
			}

#ifdef	_DEBUG_INTERFACE
			LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Characters [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()));
#endif
		}
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Characters"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_Settings (IDaCtlSettings **Settings)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Settings"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	DaCtlSettings *						lSettings;
	tPtr <CComObject <DaCtlSettings> >	lObject;
	IDaCtlSettingsPtr					lInterface;

	if	(Settings == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(
						(mSettings != NULL)
					&&	(lSettings = dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()))
					&&	(lSettings->mServerObject != NULL)
					)
				{
					SafeFreeSafePtr (mSettings);
				}

				if	(
						(mSettings == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlSettings>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mSettings = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mSettings;
				(*Settings) = lInterface.Detach();
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(mSettings != NULL)
					&&	(lSettings = dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()))
					&&	(lSettings->mServerObject == NULL)
					)
				{
					SafeFreeSafePtr (mSettings);
				}

				if	(
						(mSettings == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlSettings>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mSettings = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mSettings;
				(*Settings) = lInterface.Detach();
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_Settings [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlSettings *> (mSettings.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_Settings"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_AudioOutput (IDaCtlAudioOutput **AudioOutput)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_AudioOutput"), this, max(m_dwRef,-1));
#endif
	HRESULT									lResult = S_OK;
	tPtr <CComObject <DaCtlAudioOutput> >	lObject;
	IDaCtlAudioOutputPtr					lInterface;

	if	(AudioOutput == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(
					(mAudioOutput == NULL)
				&&	(SUCCEEDED (lResult = CComObject <DaCtlAudioOutput>::CreateInstance (lObject.Free())))
				&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
				)
			{
				mAudioOutput = (LPDISPATCH)lObject.Detach();
			}

			lInterface = mAudioOutput;
			(*AudioOutput) = lInterface.Detach();
		}
		catch AnyExceptionDebug

		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_AudioOutput [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlAudioOutput *> (mAudioOutput.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_AudioOutput"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_SpeechInput (IDaCtlSpeechInput **SpeechInput)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_SpeechInput"), this, max(m_dwRef,-1));
#endif
	HRESULT									lResult = S_OK;
	tPtr <CComObject <DaCtlSpeechInput> >	lObject;
	IDaCtlSpeechInputPtr					lInterface;

	if	(SpeechInput == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		)
	{
		try
		{
			if	(
					(mSpeechInput == NULL)
				&&	(SUCCEEDED (lResult = CComObject <DaCtlSpeechInput>::CreateInstance (lObject.Free())))
				&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
				)
			{
				mSpeechInput = (LPDISPATCH)lObject.Detach();
			}

			lInterface = mSpeechInput;
			(*SpeechInput) = lInterface.Detach();
		}
		catch AnyExceptionDebug

		_AtlModule.PostServerCall (mServer);

#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_SpeechInput [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlSpeechInput *> (mSpeechInput.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_SpeechInput"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_PropertySheet (IDaCtlPropertySheet2 **PropSheet)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_PropertySheet"), this, max(m_dwRef,-1));
#endif
	HRESULT									lResult = S_OK;
	DaCtlPropertySheet *					lPropertySheet;
	tPtr <CComObject <DaCtlPropertySheet> >	lObject = NULL;
	IDaCtlPropertySheet2Ptr					lInterface;

	if	(PropSheet == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(
						(mPropertySheet != NULL)
					&&	(lPropertySheet = dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()))
					&&	(lPropertySheet->mServerObject != NULL)
					)
				{
					SafeFreeSafePtr (mPropertySheet);
				}

				if	(
						(mPropertySheet == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlPropertySheet>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mPropertySheet = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mPropertySheet;
				(*PropSheet) = lInterface.Detach();
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(mPropertySheet != NULL)
					&&	(lPropertySheet = dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()))
					&&	(lPropertySheet->mServerObject == NULL)
					)
				{
					SafeFreeSafePtr (mPropertySheet);
				}

				if	(
						(mPropertySheet == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlPropertySheet>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mPropertySheet = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mPropertySheet;
				(*PropSheet) = lInterface.Detach();
			}
			catch AnyExceptionDebug

			_AtlModule.PostServerCall (mServer);
		}
#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_PropertySheet [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlPropertySheet *> (mPropertySheet.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_PropertySheet"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::get_CommandsWindow (IDaCtlCommandsWindow **CommandsWindow)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CommandsWindow"), this, max(m_dwRef,-1));
#endif
	HRESULT										lResult = S_OK;
	tPtr <CComObject <DaCtlCommandsWindow> >	lObject;
	IDaCtlCommandsWindowPtr						lInterface;

	if	(CommandsWindow == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
//TODO
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(mCommandsWindow == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlCommandsWindow>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mCommandsWindow = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mCommandsWindow;
				(*CommandsWindow) = lInterface.Detach();
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CommandsWindow [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlCommandsWindow *> (mCommandsWindow.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_CommandsWindow"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::ShowDefaultCharacterProperties (VARIANT X, VARIANT Y)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::ShowDefaultCharacterProperties"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}
	if	(SUCCEEDED (lResult))
	{
		_variant_t	lXPos (X);
		_variant_t	lYPos (Y);

		try
		{
			if	(
					(V_VT (&lXPos) != VT_EMPTY)
				&&	(V_VT (&lXPos) != VT_ERROR)
				)
			{
				lXPos.ChangeType (VT_I2);
			}
			if	(
					(V_VT (&lYPos) != VT_EMPTY)
				&&	(V_VT (&lYPos) != VT_ERROR)
				)
			{
				lYPos.ChangeType (VT_I2);
			}
		}
		catch AnyExceptionSilent

		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			IDaCtlPropertySheet2Ptr	lPropertySheet;

			if	(SUCCEEDED (lResult = get_PropertySheet (&lPropertySheet)))
			{
				if	(
						(lXPos.vt == VT_I2)
					&&	((short)lXPos >= 0)
					)
				{
					lPropertySheet->put_Left ((short)lXPos);
				}
				if	(
						(lXPos.vt == VT_I2)
					&&	((short)lYPos >= 0)
					)
				{
					lPropertySheet->put_Top ((short)lYPos);
				}
				lPropertySheet->put_Page (_bstr_t(PropertySheet_PageName_Character));
				lResult = lPropertySheet->put_Visible (VARIANT_TRUE);
			}
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				AllowSetForegroundWindow (ASFW_ANY);
				if	(
						(lXPos.vt == VT_I2)
					&&	(lXPos.vt == VT_I2)
					&&	(
							((short)lXPos >= 0)
						||	((short)lYPos >= 0)
						)
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
	}

	PutControlError (lResult, __uuidof(IDaControl));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::ShowDefaultCharacterProperties"), this, max(m_dwRef,-1));
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CharacterFiles"), this, max(m_dwRef,-1));
#endif
	HRESULT										lResult = S_OK;
	DaCtlCharacterFiles *						lCharacterFiles;
	tPtr <CComObject <DaCtlCharacterFiles> >	lObject;
	IDaCtlCharacterFilesPtr						lInterface;

	if	(CharacterFiles == NULL)
	{
		lResult = E_POINTER;
	}
	else
	if	(mAutoConnect)
	{
		lResult = ConnectServer ();
	}

	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(
						(mCharacterFiles != NULL)
					&&	(lCharacterFiles = dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()))
					&&	(lCharacterFiles->mServerObject != NULL)
					)
				{
					SafeFreeSafePtr (mCharacterFiles);
				}

				if	(
						(mCharacterFiles == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlCharacterFiles>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mCharacterFiles = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mCharacterFiles;
				(*CharacterFiles) = lInterface.Detach();
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(mCharacterFiles != NULL)
					&&	(lCharacterFiles = dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()))
					&&	(lCharacterFiles->mServerObject == NULL)
					)
				{
					SafeFreeSafePtr (mCharacterFiles);
				}

				if	(
						(mCharacterFiles == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlCharacterFiles>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					mCharacterFiles = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mCharacterFiles;
				(*CharacterFiles) = lInterface.Detach();
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
#ifdef	_DEBUG_INTERFACE
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_CharacterFiles [%p]"), this, max(m_dwRef,-1), dynamic_cast <DaCtlCharacterFiles *> (mCharacterFiles.GetInterfacePtr()));
#endif
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_CharacterFiles"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_CharacterStyle (long *CharacterStyle)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES_NOT
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::get_CharacterStyle"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!CharacterStyle)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharacterStyle) = 0;
	}
	if	(SUCCEEDED (lResult))
	{
		(*CharacterStyle) = (long)mLocalCharacterStyle;

		if	(
				(mServer != NULL)
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
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_CharacterStyle"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::put_CharacterStyle (long CharacterStyle)
{
	ClearControlError ();
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::put_CharacterStyle [%8.8X]"), this, max(m_dwRef,-1), CharacterStyle);
#endif
	HRESULT	lResult = S_OK;

	if	(SUCCEEDED (lResult))
	{
		mLocalCharacterStyle = (DWORD)CharacterStyle;

		if	(
				(mServer)
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
			)
		{
			try
			{
				lResult = mServer->put_CharacterStyle (CharacterStyle &~LocalCharacterStyle);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::put_CharacterStyle"), this, max(m_dwRef,-1));
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_TTSEngines"), this, max(m_dwRef,-1));
#endif
	HRESULT									lResult = S_OK;
	DaCtlTTSEngines *						lTTSEngines;
	tPtr <CComObject <DaCtlTTSEngines> >	lObject;
	IDaCtlTTSEnginesPtr						lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(mAutoConnect)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(
						(mTTSEngines != NULL)
					&&	(lTTSEngines = dynamic_cast <DaCtlTTSEngines *> (mTTSEngines.GetInterfacePtr()))
					&&	(lTTSEngines->mServerObject != NULL)
					)
				{
					SafeFreeSafePtr (mTTSEngines);
				}

				if	(
						(mTTSEngines == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					mTTSEngines = (IDaCtlTTSEngines*)lObject.Detach();
				}

				lInterface = mTTSEngines;
				(*TTSEngines) = lInterface.Detach ();
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(mTTSEngines != NULL)
					&&	(lTTSEngines = dynamic_cast <DaCtlTTSEngines *> (mTTSEngines.GetInterfacePtr()))
					&&	(lTTSEngines->mServerObject == NULL)
					)
				{
					SafeFreeSafePtr (mTTSEngines);
				}

				if	(
						(mTTSEngines == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					mTTSEngines = (IDaCtlTTSEngines*)lObject.Detach();
				}

				lInterface = mTTSEngines;
				(*TTSEngines) = lInterface.Detach ();
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_TTSEngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindTTSEngines (VARIANT LanguageID, VARIANT Gender, IDaCtlTTSEngines **TTSEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindTTSEngines"), this, max(m_dwRef,-1));
#endif
	HRESULT									lResult = S_OK;
	long									lLanguageID = 0;
	short									lGender = GENDER_NEUTRAL;
	tPtr <CComObject <DaCtlTTSEngines> >	lObject;
	IDaCtlTTSEnginesPtr						lInterface;

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
			&&	(mAutoConnect)
			)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (lObject.Free())))
				{
					if	(lObject->mLocalObject = new CDaCmnTTSEngines)
					{
						if	(
								(SUCCEEDED (lResult = lObject->mLocalObject->UseTheseVoices (NULL, (LANGID)lLanguageID, lGender)))
							&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
							)
						{
							lObject->InitializeObjects ();
							lInterface = (IDaCtlTTSEngines*)lObject.Detach();
							(*TTSEngines) = lInterface.Detach();
						}
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = mServer->FindTTSEngines (lLanguageID, lGender, &lObject->mServerObject)))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					lInterface = (IDaCtlTTSEngines*)lObject.Detach();
					(*TTSEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindTTSEngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::GetCharacterTTSEngine (VARIANT Provider, IDaCtlTTSEngine **TTSEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::GetCharacterTTSEngine"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	tPtr <CComObject <DaCtlTTSEngine> >	lObject;
	IDaCtlTTSEnginePtr					lInterface;

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(mAutoConnect)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				tPtr <CAgentFile>	lAgentFile;

				if	(
						(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile)))
					&&	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngine>::CreateInstance (lObject.Free())))
					)
				{
					if	(lObject->mLocalObject = new CDaCmnTTSEngine)
					{
						if	(
								(SUCCEEDED (lResult = lObject->mLocalObject->Initialize (lAgentFile)))
							&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
							)
						{
							lInterface = (LPDISPATCH)lObject.Detach();
							(*TTSEngine) = lInterface.Detach();
						}
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(SUCCEEDED (lResult = CComObject <DaCtlTTSEngine>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = mServer->GetCharacterTTSEngine (Provider, &lObject->mServerObject)))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lInterface = (LPDISPATCH)lObject.Detach();
					(*TTSEngine) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::GetCharacterTTSEngine"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindCharacterTTSEngines (VARIANT Provider, VARIANT LanguageID, IDaCtlTTSEngines **TTSEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindCharacterTTSEngines"), this, max(m_dwRef,-1));
#endif
	HRESULT									lResult = S_OK;
	long									lLanguageID = 0;
	tPtr <CComObject <DaCtlTTSEngines> >	lObject;
	IDaCtlTTSEnginesPtr						lInterface;

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
			&&	(mAutoConnect)
			)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				tPtr <CAgentFile>	lAgentFile;

				if	(
						(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile)))
					&&	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (lObject.Free())))
					)
				{
					if	(lObject->mLocalObject = new CDaCmnTTSEngines)
					{
						if	(
								(SUCCEEDED (lResult = lObject->mLocalObject->UseTheseVoices (lAgentFile, (LANGID)lLanguageID, SpeechGender_Neutral)))
							&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
							)
						{
							lObject->InitializeObjects ();
							lInterface = (IDaCtlTTSEngines*)lObject.Detach();
							(*TTSEngines) = lInterface.Detach();
						}
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = mServer->FindCharacterTTSEngines (Provider, lLanguageID, &lObject->mServerObject)))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					lInterface = (IDaCtlTTSEngines*)lObject.Detach();
					(*TTSEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindCharacterTTSEngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_SREngines (IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_SREngines"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	DaCtlSREngines *					lSREngines;
	tPtr <CComObject <DaCtlSREngines> >	lObject;
	IDaCtlSREnginesPtr					lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(
				(SUCCEEDED (lResult))
			&&	(mAutoConnect)
			)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(
						(mSREngines != NULL)
					&&	(lSREngines = dynamic_cast <DaCtlSREngines *> (mSREngines.GetInterfacePtr()))
					&&	(lSREngines->mServerObject != NULL)
					)
				{
					SafeFreeSafePtr (mSREngines);
				}

				if	(
						(mSREngines == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					mSREngines = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mSREngines;
				(*SREngines) = lInterface.Detach();
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(mSREngines != NULL)
					&&	(lSREngines = dynamic_cast <DaCtlSREngines *> (mSREngines.GetInterfacePtr()))
					&&	(lSREngines->mServerObject == NULL)
					)
				{
					SafeFreeSafePtr (mSREngines);
				}

				if	(
						(mSREngines == NULL)
					&&	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					mSREngines = (LPDISPATCH)lObject.Detach();
				}

				lInterface = mSREngines;
				(*SREngines) = lInterface.Detach();
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServer);
		}
	}

	PutControlError (lResult, __uuidof(IDaControl2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::get_SREngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindSREngines (VARIANT LanguageID, IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindSREngines"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	long								lLanguageID = 0;
	tPtr <CComObject <DaCtlSREngines> >	lObject;
	IDaCtlSREnginesPtr					lInterface;

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
			&&	(mAutoConnect)
			)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (lObject.Free())))
				{
					if	(lObject->mLocalObject = new CDaCmnSREngines)
					{
						if	(
								(SUCCEEDED (lResult = lObject->mLocalObject->UseTheseInputs (NULL, (LANGID)lLanguageID)))
							&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
							)
						{
							lObject->InitializeObjects ();
							lInterface = (LPDISPATCH)lObject.Detach();
							(*SREngines) = lInterface.Detach();
						}
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = mServer->FindSREngines (lLanguageID, &lObject->mServerObject)))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					lInterface = (LPDISPATCH)lObject.Detach();
					(*SREngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindSREngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::GetCharacterSREngine (VARIANT Provider, IDaCtlSREngine **SREngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::GetCharacterSREngine"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	IDaSvrSREnginePtr					lServerObject;
	tPtr <CComObject <DaCtlSREngine> >	lObject;
	IDaCtlSREnginePtr					lInterface;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(
				(SUCCEEDED (lResult))
			&&	(mAutoConnect)
			)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				tPtr <CAgentFile>	lAgentFile;

				if	(
						(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile)))
					&&	(SUCCEEDED (lResult = CComObject <DaCtlSREngine>::CreateInstance (lObject.Free())))
					)
				{
					if	(lObject->mLocalObject = new CDaCmnSREngine)
					{
						if	(
								(SUCCEEDED (lResult = lObject->mLocalObject->Initialize (lAgentFile)))
							&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
							)
						{
							lInterface = (LPDISPATCH)lObject.Detach();
							(*SREngine) = lInterface.Detach();
						}
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(SUCCEEDED (lResult = CComObject <DaCtlSREngine>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = mServer->GetCharacterSREngine (Provider, &lObject->mServerObject)))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lInterface = (LPDISPATCH)lObject.Detach();
					(*SREngine) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::GetCharacterSREngine"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

STDMETHODIMP DaControl::FindCharacterSREngines (VARIANT Provider, VARIANT LanguageID, IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::FindCharacterSREngines"), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	long								lLanguageID = 0;
	tPtr <CComObject <DaCtlSREngines> >	lObject;
	IDaCtlSREnginesPtr					lInterface;

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
			&&	(mAutoConnect)
			)
		{
			lResult = ConnectServer ();
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(
				(!mAutoConnect)
			&&	(mServer == NULL)
			)
		{
			try
			{
				tPtr <CAgentFile>	lAgentFile;

				if	(
						(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (Provider, lAgentFile)))
					&&	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (lObject.Free())))
					)
				{
					if	(lObject->mLocalObject = new CDaCmnSREngines)
					{
						if	(
								(SUCCEEDED (lResult = lObject->mLocalObject->UseTheseInputs (lAgentFile, (LANGID)lLanguageID)))
							&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
							)
						{
							lObject->InitializeObjects ();
							lInterface = (LPDISPATCH)lObject.Detach();
							(*SREngines) = lInterface.Detach();
						}
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServer)))
		{
			try
			{
				if	(
						(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (lObject.Free())))
					&&	(SUCCEEDED (lResult = mServer->FindCharacterSREngines (Provider, lLanguageID, &lObject->mServerObject)))
					&&	(SUCCEEDED (lResult = lObject->SetOwner (this)))
					)
				{
					lObject->InitializeObjects ();
					lInterface = (LPDISPATCH)lObject.Detach();
					(*SREngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaControl::FindCharacterSREngines"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::get_ControlCharacter (IDaCtlCharacter2 **ControlCharacter)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::get_ControlCharacter"), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	IDaCtlCharacter2Ptr	lInterface;

	if	(!ControlCharacter)
	{
		lResult = E_POINTER;
	}
	else
	{
		lInterface = mControlCharacter;
		(*ControlCharacter) = lInterface.Detach ();
	}
	return lResult;
}

STDMETHODIMP DaControl::put_ControlCharacter (IDaCtlCharacter2 *ControlCharacter)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaControl::put_ControlCharacter"), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	DaCtlCharacter *	lControlCharacter = NULL;
	DaCtlCharacter *	lPrevCharacter = NULL;

	if	(ControlCharacter)
	{
		try
		{
			DaCtlCharacters *		lCharacters;

			if	(
					(mCharacters != NULL)
				&&	(lCharacters = dynamic_cast <DaCtlCharacters *> (mCharacters.GetInterfacePtr()))
				&&	(lControlCharacter = lCharacters->FindCharacter (ControlCharacter))
				&&	(!lControlCharacter->mLocalObject)
				)
			{
				lControlCharacter = NULL;
			}
		}
		catch AnyExceptionDebug

		if	(
				(!lControlCharacter)
			||	(lControlCharacter->SafeGetOwner () != this)
			)
		{
			lResult = E_INVALIDARG;
		}
	}
	if	(mControlCharacter)
	{
		try
		{
			lPrevCharacter = dynamic_cast <DaCtlCharacter *> (mControlCharacter.GetInterfacePtr());
		}
		catch AnyExceptionDebug
	}

	if	(SUCCEEDED (lResult))
	{
		if	(lPrevCharacter)
		{
			lPrevCharacter->SetContained (false, mLocalCharacterStyle);
		}
		RemoveChainEntry (1);
		SafeFreeSafePtr (mControlCharacter);
		if	(
				(lControlCharacter)
			&&	(SUCCEEDED (lResult = lControlCharacter->SetContained (true, mLocalCharacterStyle)))
			)
		{
			mControlCharacter = lControlCharacter;
			SetChainEntry (1, this, 1);
		}

		if	(mControlCharacter)
		{
			OnAutoSizeChanged ();
			CenterVideo ();
			ShowWindow (SW_SHOWNA);
			RedrawWindow ();
		}
		else
		if	(IsWindow ())
		{
			ShowWindow (SW_HIDE);
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Even though we're implementing IPropertyNotifySink to conform to ActiveX
//	standards, we're not going to rely on the client application using it.
//	Form designers will use it, but run-time changes might be missed;
//
/////////////////////////////////////////////////////////////////////////////

typedef CStockPropImpl<DaControl, IDaControl2, &__uuidof(IDaControl2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer> thisStockProp;

HRESULT STDMETHODCALLTYPE DaControl::put_AutoSize (VARIANT_BOOL AutoSize)
{
	HRESULT	lResult = thisStockProp::put_AutoSize (AutoSize);
	OnAutoSizeChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_AutoSize (VARIANT_BOOL *AutoSize)
{
	return thisStockProp::get_AutoSize (AutoSize);
}

HRESULT STDMETHODCALLTYPE DaControl::put_BackColor (OLE_COLOR BackColor)
{
	HRESULT	lResult = thisStockProp::put_BackColor (BackColor);
	OnBackColorChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_BackColor (OLE_COLOR *BackColor)
{
	return thisStockProp::get_BackColor (BackColor);
}

HRESULT STDMETHODCALLTYPE DaControl::put_BorderColor (OLE_COLOR BorderColor)
{
	HRESULT	lResult = thisStockProp::put_BorderColor (BorderColor);
	OnBorderColorChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_BorderColor (OLE_COLOR *BorderColor)
{
	return thisStockProp::get_BorderColor (BorderColor);
}

HRESULT STDMETHODCALLTYPE DaControl::put_BorderStyle (long BorderStyle)
{
	HRESULT	lResult = thisStockProp::put_BorderStyle (BorderStyle);
	OnBorderStyleChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_BorderStyle (long *BorderStyle)
{
	return thisStockProp::get_BorderStyle (BorderStyle);
}

HRESULT STDMETHODCALLTYPE DaControl::put_BorderWidth (long BorderWidth)
{
	HRESULT	lResult = thisStockProp::put_BorderWidth (BorderWidth);
	OnBorderWidthChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_BorderWidth (long *BorderWidth)
{
	return thisStockProp::get_BorderWidth (BorderWidth);
}

HRESULT STDMETHODCALLTYPE DaControl::put_BorderVisible (VARIANT_BOOL BorderVisible)
{
	HRESULT	lResult = thisStockProp::put_BorderVisible (BorderVisible);
	OnBorderVisibleChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_BorderVisible (VARIANT_BOOL *BorderVisible)
{
	return thisStockProp::get_BorderVisible (BorderVisible);
}

HRESULT STDMETHODCALLTYPE DaControl::put_MousePointer (long MousePointer)
{
	HRESULT	lResult = thisStockProp::put_MousePointer (MousePointer);
	OnMousePointerChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_MousePointer (long *MousePointer)
{
	return thisStockProp::get_MousePointer (MousePointer);
}

HRESULT STDMETHODCALLTYPE DaControl::put_MouseIcon (IPictureDisp *MouseIcon)
{
	HRESULT	lResult = thisStockProp::put_MouseIcon (MouseIcon);
	OnMouseIconChanged ();
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaControl::get_MouseIcon (IPictureDisp **MouseIcon)
{
	return thisStockProp::get_MouseIcon (MouseIcon);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaControl::OnAutoSizeChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnAutoSizeChanged [%d]"), this, max(m_dwRef,-1), m_bAutoSize);
#endif
	if	(
			(IsWindow ())
		&&	(!IsDesigning ())
		)
	{
		mAutoSize = m_bAutoSize ? true : false;

		if	(
				(mAutoSize)
			&&	(mControlCharacter)
			)
		{
			AutoSizeWindow ();
		}
	}
}

void DaControl::OnBackColorChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBackColorChanged [%8.8X]"), this, max(m_dwRef,-1), m_clrBackColor);
#endif
	SetBkColor (GetOleColor (m_clrBackColor));
	if	(IsWindow ())
	{
		RedrawWindow ();
	}
}

void DaControl::OnBorderColorChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderColorChanged [%8.8X]"), this, max(m_dwRef,-1), m_clrBorderColor);
#endif
	if	(IsWindow ())
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE|RDW_FRAME);
	}
}

void DaControl::OnBorderStyleChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderStyleChanged [%d]"), this, max(m_dwRef,-1), m_nBorderStyle);
#endif
	if	(IsWindow ())
	{
		UpdateWindowStyles ();
	}
}

void DaControl::OnBorderVisibleChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderVisibleChanged [%d]"), this, max(m_dwRef,-1), m_bBorderVisible);
#endif
	if	(IsWindow ())
	{
		UpdateWindowStyles ();
	}
}

void DaControl::OnBorderWidthChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnBorderWidthChanged [%d]"), this, max(m_dwRef,-1), m_nBorderWidth);
#endif
	if	(IsWindow ())
	{
		UpdateWindowStyles ();
	}
}

void DaControl::OnMouseIconChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnMouseIconChanged [%p]"), this, max(m_dwRef,-1), m_pMouseIcon.p);
#endif
}

void DaControl::OnMousePointerChanged()
{
#ifdef	_DEBUG_ATTRIBUTES
	LogMessage (_DEBUG_ATTRIBUTES, _T("[%p(%d)] DaControl::OnMousePointerChanged [%d]"), this, max(m_dwRef,-1), m_nMousePointer);
#endif
	switch (m_nMousePointer)
	{
		case 1:		mCursor = LoadCursor (NULL, IDC_ARROW); break;
		case 2:		mCursor = LoadCursor (NULL, IDC_CROSS); break;
		case 11:	mCursor = LoadCursor (NULL, IDC_WAIT); break;
		case 12:	mCursor = LoadCursor (NULL, IDC_NO); break;
		case 13:	mCursor = LoadCursor (NULL, IDC_APPSTARTING); break;
		case 14:	mCursor = LoadCursor (NULL, IDC_HELP); break;
		case 16:	mCursor = LoadCursor (NULL, IDC_HAND); break;
		default:	mCursor.Close ();
	}
}
