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
#include "DaAudioOutputObj.h"
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

IMPLEMENT_DYNAMIC(CDaAudioOutputObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaAudioOutputObj, "")
IMPLEMENT_OLETYPELIB(CDaAudioOutputObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaAudioOutputObj::CDaAudioOutputObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Audio Output")))
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

BEGIN_INTERFACE_MAP(CDaAudioOutputObj, CCmdTarget)
	INTERFACE_PART(CDaAudioOutputObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAudioOutputObj, __uuidof(IDaCtlAudioOutput), AudioOutput)
	INTERFACE_PART(CDaAudioOutputObj, __uuidof(IAgentCtlAudioObject), AudioOutput)
	INTERFACE_PART(CDaAudioOutputObj, __uuidof(IAgentCtlAudioObjectEx), AudioOutput)
	INTERFACE_PART(CDaAudioOutputObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAudioOutputObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAudioOutputObj, AudioOutput)
IMPLEMENT_DISPATCH_IID(CDaAudioOutputObj, __uuidof(IDaCtlAudioOutput))
IMPLEMENT_PROVIDECLASSINFO(CDaAudioOutputObj, __uuidof(CDaAudioOutputObj))

BEGIN_SUPPORTERRORINFO(CDaAudioOutputObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaAudioOutputObj, __uuidof(IDaCtlAudioOutput))
	IMPLEMENT_SUPPORTERRORINFO(CDaAudioOutputObj, __uuidof(IAgentCtlAudioObject))
	IMPLEMENT_SUPPORTERRORINFO(CDaAudioOutputObj, __uuidof(IAgentCtlAudioObjectEx))
END_SUPPORTERRORINFO(CDaAudioOutputObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAudioOutputObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAudioOutputObj)
	DISP_PROPERTY_EX_ID(CDaAudioOutputObj, "Enabled", DISPID_IAgentCtlAudioObject_Enabled, DspGetEnabled, DspSetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaAudioOutputObj, "SoundEffects", DISPID_IAgentCtlAudioObject_SoundEffects, DspGetSoundEffects, DspSetSoundEffects, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaAudioOutputObj, "Status", DISPID_IAgentCtlAudioObjectEx_Status, DspGetStatus, DspSetStatus, VT_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAudioOutputObj::CDaAudioOutputObj (CDaAgentCtl & pOwner)
:	mOwner (&pOwner),
	mServerObject (pOwner.mServer)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::CDaAudioOutputObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaAudioOutputObj::~CDaAudioOutputObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::~CDaAudioOutputObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mAudioOutput != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj Attached [%p] Owner [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mOwner->mAudioOutput.GetInterfacePtr(), mOwner);
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

void CDaAudioOutputObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::Terminate [%u] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaAudioOutputObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::OnFinalRelease [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaAudioOutputObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaAudioOutputObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CDaAudioOutputObj::DspGetEnabled()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::DspGetEnabled"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xAudioOutput.get_Enabled (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaAudioOutputObj::DspSetEnabled(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::DspSetEnabled"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaAudioOutputObj::DspGetSoundEffects()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::DspGetSoundEffects"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xAudioOutput.get_SoundEffects (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaAudioOutputObj::DspSetSoundEffects(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::DspSetSoundEffects"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaAudioOutputObj::DspGetStatus()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::DspGetStatus"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xAudioOutput.get_Status (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaAudioOutputObj::DspSetStatus(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::DspSetStatus"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAudioOutputObj::XAudioOutput::get_Enabled (VARIANT_BOOL *AudioEnabled)
{
	METHOD_PROLOGUE(CDaAudioOutputObj, AudioOutput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::XAudioOutput::get_Enabled"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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
	if	(AudioEnabled)
	{
		(*AudioEnabled) = lEnabled ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::XAudioOutput::get_Enabled"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAudioOutputObj::XAudioOutput::get_SoundEffects (VARIANT_BOOL *SoundEffects)
{
	METHOD_PROLOGUE(CDaAudioOutputObj, AudioOutput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::XAudioOutput::get_SoundEffects"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lSoundEffects = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetUsingSoundEffects (&lSoundEffects);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(SoundEffects)
	{
		(*SoundEffects) = lSoundEffects ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::XAudioOutput::get_SoundEffects"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAudioOutputObj::XAudioOutput::get_Status (short *Available)
{
	METHOD_PROLOGUE(CDaAudioOutputObj, AudioOutput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::XAudioOutput::get_Status"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lStatus = 0;

	if	(!Available)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetStatus (&lStatus);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}

		(*Available) = (short)lStatus;
	}

	PutControlError (lResult, __uuidof(IDaCtlAudioOutput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaAudioOutputObj::XAudioOutput::get_Status"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
