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
#include "DaServer.h"
#include "DaAgentSpeechInputProperties.h"
#include "DaGlobalConfig.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentSpeechInputProperties, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentSpeechInputProperties, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentSpeechInputProperties::CDaAgentSpeechInputProperties (LPCTSTR pClientMutexName)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentSpeechInputProperties::CDaAgentSpeechInputProperties (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	ManageObjectLifetime (this, pClientMutexName);
}

CDaAgentSpeechInputProperties::~CDaAgentSpeechInputProperties ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentSpeechInputProperties::~CDaAgentSpeechInputProperties (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	try
	{
		TheServerApp->OnDeleteAgentSpeechInputProperties (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAgentSpeechInputProperties::Terminate (bool pFinal, bool pAbandonned)
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

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void CDaAgentSpeechInputProperties::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentSpeechInputProperties::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

void CDaAgentSpeechInputProperties::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentSpeechInputProperties::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentSpeechInputProperties, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentSpeechInputProperties)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "GetInstalled", DISPID_IAgentSpeechInputProperties_GetInstalled, DspGetInstalled, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "GetEnabled", DISPID_IAgentSpeechInputProperties_GetEnabled, DspGetEnabled, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "GetHotKey", DISPID_IAgentSpeechInputProperties_GetHotKey, DspGetHotKey, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "GetLCID", DISPID_IAgentSpeechInputProperties_GetLCID, DspGetLCid, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "GetEngine", DISPID_IAgentSpeechInputProperties_GetEngine, DspGetEngine, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "SetEngine", DISPID_IAgentSpeechInputProperties_SetEngine, DspSetEngine, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentSpeechInputProperties, "GetListeningTip", DISPID_IAgentSpeechInputProperties_GetListeningTip, DspGetListeningTip, VT_EMPTY, VTS_PI4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentSpeechInputProperties, CCmdTarget)
	INTERFACE_PART(CDaAgentSpeechInputProperties, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaAgentSpeechInputProperties, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentSpeechInputProperties, __uuidof(IDaSvrSpeechInputProperties), SpeechInputProperties)
	INTERFACE_PART(CDaAgentSpeechInputProperties, __uuidof(IAgentSpeechInputProperties), SpeechInputProperties)
	INTERFACE_PART(CDaAgentSpeechInputProperties, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentSpeechInputProperties, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentSpeechInputProperties, SpeechInputProperties)
IMPLEMENT_DISPATCH_IID(CDaAgentSpeechInputProperties, __uuidof(IDaSvrSpeechInputProperties))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentSpeechInputProperties, __uuidof(IDaSvrSpeechInputProperties))

BEGIN_SUPPORTERRORINFO(CDaAgentSpeechInputProperties)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentSpeechInputProperties, __uuidof(IDaSvrSpeechInputProperties))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentSpeechInputProperties, __uuidof(IAgentSpeechInputProperties))
END_SUPPORTERRORINFO(CDaAgentSpeechInputProperties)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::GetInstalled (long *pbInstalled)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetInstalled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pbInstalled)
	{
		(*pbInstalled) = FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetInstalled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::GetEnabled (long *pbEnabled)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetEnabled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = CDaSpeechInputConfig().mEnabled ? S_OK : S_FALSE;

	if	(pbEnabled)
	{
		(*pbEnabled) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetEnabled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::GetHotKey (BSTR *pbszHotCharKey)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetHotKey"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pbszHotCharKey)
	{
//TODO
		(*pbszHotCharKey) = CString().AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetHotKey"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::GetLCID (unsigned long *plcidCurrent)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetLCID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(plcidCurrent)
	{
		(*plcidCurrent) = 0;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetLCID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::GetEngine (BSTR *pbszEngine)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetEngine"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pbszEngine)
	{
		(*pbszEngine) = CString().AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetEngine"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::SetEngine (BSTR bszEngine)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::SetEngine"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::SetEngine"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentSpeechInputProperties::XSpeechInputProperties::GetListeningTip (long *pbListeningTip)
{
	METHOD_PROLOGUE(CDaAgentSpeechInputProperties, SpeechInputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetListeningTip"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = CDaSpeechInputConfig().mListeningTip ? S_OK : S_FALSE;

	if	(pbListeningTip)
	{
		(*pbListeningTip) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentSpeechInputProperties::XSpeechInputProperties::GetListeningTip"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentSpeechInputProperties::DspGetInstalled(long * Installed)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspGetInstalled"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.GetInstalled (Installed);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentSpeechInputProperties::DspGetEnabled(long * Enabled)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspGetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.GetEnabled (Enabled);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentSpeechInputProperties::DspGetHotKey(BSTR * HotCharKey)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspGetHotKey"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.GetHotKey (HotCharKey);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentSpeechInputProperties::DspGetLCid(unsigned long * lcidCurrent)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspGetLCid"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.GetLCID (lcidCurrent);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentSpeechInputProperties::DspGetEngine(BSTR * Engine)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspGetEngine"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.GetEngine (Engine);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentSpeechInputProperties::DspSetEngine(LPCTSTR Engine)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspSetEngine"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.SetEngine (_bstr_t(Engine));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentSpeechInputProperties::DspGetListeningTip(long * ListeningTip)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentSpeechInputProperties::DspGetListeningTip"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInputProperties.GetListeningTip (ListeningTip);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
