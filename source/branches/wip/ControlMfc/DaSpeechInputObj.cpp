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
#include "DaSpeechInputObj.h"
#include "ErrorInfo.h"
#ifdef	_DEBUG
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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaSpeechInputObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaSpeechInputObj, "")
IMPLEMENT_OLETYPELIB(CDaSpeechInputObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaSpeechInputObj::CDaSpeechInputObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Speech Input")))
	{
		if	(bRegister)
		{
			RegisterApartmentThreaded ();
			RegisterTypeLib (gDaTypeLibId);
			RegisterDefCategory ();
			RegisterCategory (m_clsid, CATID_Programmable);
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaSpeechInputObj, CCmdTarget)
	INTERFACE_PART(CDaSpeechInputObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSpeechInputObj, __uuidof(IDaCtlSpeechInput), SpeechInput)
	INTERFACE_PART(CDaSpeechInputObj, __uuidof(IAgentCtlSpeechInput), SpeechInput)
	INTERFACE_PART(CDaSpeechInputObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaSpeechInputObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSpeechInputObj, SpeechInput)
IMPLEMENT_DISPATCH_IID(CDaSpeechInputObj, __uuidof(IDaCtlSpeechInput))
IMPLEMENT_PROVIDECLASSINFO(CDaSpeechInputObj, __uuidof(CDaSpeechInputObj))

BEGIN_SUPPORTERRORINFO(CDaSpeechInputObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaSpeechInputObj, __uuidof(IDaCtlSpeechInput))
	IMPLEMENT_SUPPORTERRORINFO(CDaSpeechInputObj, __uuidof(IAgentCtlSpeechInput))
END_SUPPORTERRORINFO(CDaSpeechInputObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSpeechInputObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSpeechInputObj)
	DISP_PROPERTY_EX_ID(CDaSpeechInputObj, "Enabled", DISPID_IAgentCtlSpeechInput_Enabled, DspGetEnabled, DspSetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaSpeechInputObj, "Language", DISPID_IAgentCtlSpeechInput_Language, DspGetLanguage, DspSetLanguage, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSpeechInputObj, "HotKey", DISPID_IAgentCtlSpeechInput_HotKey, DspGetHotKey, DspSetHotKey, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSpeechInputObj, "VoiceInstalled", DISPID_IAgentCtlSpeechInput_Installed, DspGetVoiceInstalled, DspSetVoiceInstalled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaSpeechInputObj, "Engine", DISPID_IAgentCtlSpeechInput_Engine, DspGetEngine, DspSetEngine, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSpeechInputObj, "ListeningTip", DISPID_IAgentCtlSpeechInput_ListeningTip, DspGetListeningTip, DspSetListeningTip, VT_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaSpeechInputObj::CDaSpeechInputObj (CDaAgentCtl & pOwner)
:	mOwner (&pOwner),
	mServerObject (pOwner.mServer)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::CDaSpeechInputObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaSpeechInputObj::~CDaSpeechInputObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::~CDaSpeechInputObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mSpeechInput != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj Attached [%p] Owner [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mOwner->mSpeechInput.GetInterfacePtr(), mOwner);
		}
	}
	catch AnyExceptionSilent
#endif

	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaSpeechInputObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::Terminate [%u] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
		}
#endif
#endif
		if	(pFinal)
		{
			mOwner = NULL;
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaSpeechInputObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::OnFinalRelease [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaSpeechInputObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaSpeechInputObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CDaSpeechInputObj::DspGetEnabled()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspGetEnabled"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xSpeechInput.get_Enabled (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaSpeechInputObj::DspSetEnabled(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspSetEnabled"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSpeechInputObj::DspGetLanguage()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspGetLanguage"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechInput.get_Language (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechInputObj::DspSetLanguage(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspSetLanguage"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSpeechInputObj::DspGetHotKey()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspGetHotKey"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechInput.get_HotKey (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechInputObj::DspSetHotKey(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspSetHotKey"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaSpeechInputObj::DspGetVoiceInstalled()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspGetVoiceInstalled"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xSpeechInput.get_Installed (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaSpeechInputObj::DspSetVoiceInstalled(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspSetVoiceInstalled"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSpeechInputObj::DspGetEngine()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspGetEngine"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechInput.get_Engine (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechInputObj::DspSetEngine(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspSetEngine"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechInput.put_Engine (_bstr_t (lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaSpeechInputObj::DspGetListeningTip()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspGetListeningTip"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xSpeechInput.get_ListeningTip (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaSpeechInputObj::DspSetListeningTip(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::DspSetListeningTip"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::get_Enabled (VARIANT_BOOL *VoiceEnabled)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Enabled"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lEnabled = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetEnabled (&lEnabled);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(VoiceEnabled)
	{
		(*VoiceEnabled) = lEnabled ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Enabled"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::get_Language (BSTR *Language)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Language"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;
	if	(Language)
	{
		(*Language) = NULL;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Language"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::get_HotKey (BSTR *HotKey)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_HotKey"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!HotKey)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*HotKey) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetHotKey (HotKey);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_HotKey"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::get_Installed (VARIANT_BOOL *VoiceInstalled)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Installed"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;
	if	(VoiceInstalled)
	{
		(*VoiceInstalled) = VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Installed"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::get_Engine (BSTR *Engine)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Engine"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;
	if	(Engine)
	{
		(*Engine) = NULL;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_Engine"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::put_Engine (BSTR Engine)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::put_Engine"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetEngine (Engine);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::put_Engine"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechInputObj::XSpeechInput::get_ListeningTip (VARIANT_BOOL *ListeningTip)
{
	METHOD_PROLOGUE(CDaSpeechInputObj, SpeechInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_ListeningTip"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lListeningTip = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetListeningTip (&lListeningTip);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(ListeningTip)
	{
		(*ListeningTip) = lListeningTip ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaSpeechInputObj::XSpeechInput::get_ListeningTip"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
