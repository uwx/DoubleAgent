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
#include "DaCommandObj.h"
#include "DaCharacterObj.h"
#include "DaAgentCtl.h"
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

IMPLEMENT_DYNAMIC(CDaCommandObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaCommandObj, "")
IMPLEMENT_OLETYPELIB(CDaCommandObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaCommandObj::CDaCommandObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Command")))
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

BEGIN_INTERFACE_MAP(CDaCommandObj, CCmdTarget)
	INTERFACE_PART(CDaCommandObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaCommandObj, __uuidof(IDaCtlCommand), Command)
	INTERFACE_PART(CDaCommandObj, __uuidof(IAgentCtlCommand), Command)
	INTERFACE_PART(CDaCommandObj, __uuidof(IAgentCtlCommandEx), Command)
	INTERFACE_PART(CDaCommandObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaCommandObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaCommandObj, Command)
IMPLEMENT_DISPATCH_IID(CDaCommandObj, __uuidof(IDaCtlCommand))
IMPLEMENT_PROVIDECLASSINFO(CDaCommandObj, __uuidof(CDaCommandObj))

BEGIN_SUPPORTERRORINFO(CDaCommandObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandObj, __uuidof(IDaCtlCommand))
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandObj, __uuidof(IAgentCtlCommand))
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandObj, __uuidof(IAgentCtlCommandEx))
END_SUPPORTERRORINFO(CDaCommandObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaCommandObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaCommandObj)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "Voice", DISPID_IAgentCtlCommand_Voice, DspGetVoice, DspSetVoice, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "Caption", DISPID_IAgentCtlCommand_Caption, DspGetCaption, DspSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "Enabled", DISPID_IAgentCtlCommand_Enabled, DspGetEnabled, DspSetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "Visible", DISPID_IAgentCtlCommand_Visible, DspGetVisible, DspSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "Confidence", DISPID_IAgentCtlCommand_Confidence, DspGetConfidence, DspSetConfidence, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "ConfidenceText", DISPID_IAgentCtlCommand_ConfidenceText, DspGetConfidenceText, DspSetConfidenceText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "HelpContextID", DISPID_IAgentCtlCommandEx_HelpContextID, DspGetHelpContextID, DspSetHelpContextID, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCommandObj, "VoiceCaption", DISPID_IAgentCtlCommandEx_VoiceCaption, DspGetVoiceCaption, DspSetVoiceCaption, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCommandObj::CDaCommandObj (class CDaCharacterObj & pOwner)
:	mOwner (&pOwner),
	mServerId (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::CDaCommandObj (%d)"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCommandObj::~CDaCommandObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::~CDaCommandObj (%d) [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCommandObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::Terminate [%u] [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::Terminate [%u] Done [%d]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaCommandObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::OnFinalRelease [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaCharacterObj * CDaCommandObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCommandObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BSTR CDaCommandObj::DspGetVoice()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommand.get_Voice (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandObj::DspSetVoice(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_Voice (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandObj::DspGetCaption()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommand.get_Caption (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandObj::DspSetCaption(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_Caption (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCommandObj::DspGetEnabled()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetEnabled"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCommand.get_Enabled (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandObj::DspSetEnabled(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_Enabled (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCommandObj::DspGetVisible()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetVisible"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCommand.get_Visible (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandObj::DspSetVisible(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_Visible (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaCommandObj::DspGetConfidence()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetConfidence"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCommand.get_Confidence (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandObj::DspSetConfidence(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_Confidence (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandObj::DspGetConfidenceText()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetConfidenceText"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommand.get_ConfidenceText (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandObj::DspSetConfidenceText(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_ConfidenceText (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaCommandObj::DspGetHelpContextID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetHelpContextID"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCommand.get_HelpContextID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandObj::DspSetHelpContextID(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetHelpContextID"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_HelpContextID (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandObj::DspGetVoiceCaption()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspGetVoiceCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommand.get_VoiceCaption (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandObj::DspSetVoiceCaption(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::DspSetVoiceCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.put_VoiceCaption (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_Voice (BSTR *Voice)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Voice) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVoice (Voice);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_Voice (BSTR Voice)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVoice (Voice);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_Caption (BSTR *Caption)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetCaption (Caption);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_Caption (BSTR Caption)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetCaption (Caption);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_Enabled (VARIANT_BOOL *Enabled)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Enabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Enabled)
	{
		lResult = E_POINTER;
	}
	else
	{
		long	lEnabled = TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetEnabled (&lEnabled);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}

		(*Enabled) = lEnabled ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Enabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_Enabled (VARIANT_BOOL Enabled)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Enabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetEnabled (Enabled!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Enabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_Visible (VARIANT_BOOL *Visible)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Visible)
	{
		lResult = E_POINTER;
	}
	else
	{
		long	lVisible = TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVisible (&lVisible);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}

		(*Visible) = lVisible ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_Visible (VARIANT_BOOL Visible)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVisible (Visible!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_Confidence (long *Confidence)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Confidence"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Confidence) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetConfidenceThreshold (Confidence);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_Confidence"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_Confidence (long Confidence)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Confidence"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetConfidenceThreshold (Confidence);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_Confidence"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_ConfidenceText (BSTR *Text)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_ConfidenceText"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Text)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Text) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetConfidenceText (Text);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_ConfidenceText"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_ConfidenceText (BSTR Text)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_ConfidenceText"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetConfidenceText (Text);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_ConfidenceText"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_HelpContextID (long *ID)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!ID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ID) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetHelpContextID (ID);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_HelpContextID (long ID)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetHelpContextID (ID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::get_VoiceCaption (BSTR *VoiceCaption)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVoiceCaption (VoiceCaption);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::get_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandObj::XCommand::put_VoiceCaption (BSTR VoiceCaption)
{
	METHOD_PROLOGUE(CDaCommandObj, Command)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVoiceCaption (VoiceCaption);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandObj::XCommand::put_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
