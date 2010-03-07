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
#include "DaRecognitionEngineObj.h"
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

IMPLEMENT_DYNAMIC(CDaRecognitionEngineObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaRecognitionEngineObj, "")
IMPLEMENT_OLETYPELIB(CDaRecognitionEngineObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaRecognitionEngineObj::CDaRecognitionEngineObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Recognition Engine")))
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

BEGIN_INTERFACE_MAP(CDaRecognitionEngineObj, CCmdTarget)
	INTERFACE_PART(CDaRecognitionEngineObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaRecognitionEngineObj, __uuidof(IDaCtlRecognitionEngine), RecognitionEngine)
	INTERFACE_PART(CDaRecognitionEngineObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaRecognitionEngineObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaRecognitionEngineObj, RecognitionEngine)
IMPLEMENT_DISPATCH_IID(CDaRecognitionEngineObj, __uuidof(IDaCtlRecognitionEngine))
IMPLEMENT_PROVIDECLASSINFO(CDaRecognitionEngineObj, __uuidof(CDaRecognitionEngineObj))

BEGIN_SUPPORTERRORINFO(CDaRecognitionEngineObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaRecognitionEngineObj, __uuidof(IDaCtlRecognitionEngine))
END_SUPPORTERRORINFO(CDaRecognitionEngineObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaRecognitionEngineObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaRecognitionEngineObj)
	DISP_PROPERTY_EX_ID(CDaRecognitionEngineObj, "SRModeID", DISPID_VALUE, DspGetSRModeID, DspSetSRModeID, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaRecognitionEngineObj, "DisplayName", DISPID_IDaCtlRecognitionEngine_DisplayName, DspGetDisplayName, DspSetDisplayName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaRecognitionEngineObj, "Manufacturer", DISPID_IDaCtlRecognitionEngine_Manufacturer, DspGetManufacturer, DspSetManufacturer, VT_BSTR)
	DISP_FUNCTION_ID(CDaRecognitionEngineObj, "GetVersion", DISPID_IDaCtlRecognitionEngine_GetVersion, DspGetVersion, VT_EMPTY, VTS_PI2 VTS_PI2)
	DISP_PROPERTY_EX_ID(CDaRecognitionEngineObj, "LanguageID", DISPID_IDaCtlRecognitionEngine_LanguageID, DspGetLanguageID, DspSetLanguageID, VT_I4)
	DISP_PROPERTY_PARAM_ID(CDaRecognitionEngineObj, "LanguageName", DISPID_IDaCtlRecognitionEngine_LanguageName, DspGetLanguageName, DspSetLanguageName, VT_BSTR, VTS_BOOL)
	DISP_DEFVALUE(CDaRecognitionEngineObj, "SRModeID")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaRecognitionEngineObj::CDaRecognitionEngineObj (IDaSvrRecognitionEngine * pServerObject)
:	mServerObject (pServerObject)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEngineObj::CDaRecognitionEngineObj (%d) [%p]"),this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaRecognitionEngineObj::~CDaRecognitionEngineObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEngineObj::~CDaRecognitionEngineObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaRecognitionEngineObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEngineObj::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
		}
#endif
#endif
		if	(pFinal)
		{
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEngineObj::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaRecognitionEngineObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEngineObj::OnFinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BSTR CDaRecognitionEngineObj::DspGetSRModeID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspGetSRModeID"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xRecognitionEngine.get_SRModeID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEngineObj::DspSetSRModeID(LPCTSTR SRModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspSetSRModeID"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaRecognitionEngineObj::DspGetDisplayName()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspGetDisplayName"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xRecognitionEngine.get_DisplayName (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEngineObj::DspSetDisplayName(LPCTSTR DisplayName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspSetDisplayName"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaRecognitionEngineObj::DspGetManufacturer()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspGetManufacturer"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xRecognitionEngine.get_Manufacturer (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEngineObj::DspSetManufacturer(LPCTSTR Manufacturer)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspSetManufacturer"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

void CDaRecognitionEngineObj::DspGetVersion(short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspGetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetVersion (MajorVersion, MinorVersion);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaRecognitionEngineObj::DspGetLanguageID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspGetLanguageID"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xRecognitionEngine.get_LanguageID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEngineObj::DspSetLanguageID(long LanguageID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspSetLanguageID"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaRecognitionEngineObj::DspGetLanguageName(BOOL EnglishName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspGetLanguageName"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xRecognitionEngine.get_LanguageName (EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEngineObj::DspSetLanguageName(BOOL EnglishName, BSTR LanguageName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::DspSetLanguageName"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_SRModeID (BSTR *SRModeID)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_SRModeID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!SRModeID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SRModeID) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetSRModeID (SRModeID);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_SRModeID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_DisplayName (BSTR *DisplayName)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_DisplayName"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DisplayName) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetDisplayName (DisplayName);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_DisplayName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_Manufacturer (BSTR *Manufacturer)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_Manufacturer"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Manufacturer) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetManufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_Manufacturer"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::GetVersion"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(MajorVersion)
	{
		(*MajorVersion) = 0;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = 0;
	}
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetVersion (MajorVersion, MinorVersion);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::GetVersion"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_LanguageID (long *LanguageID)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_LanguageID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageID) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetLanguageID (LanguageID);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_LanguageID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_LanguageName"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageName) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetLanguageName (LanguageName, (EnglishName!=VARIANT_FALSE));
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_LanguageName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_Languages (SAFEARRAY **Languages)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_Languages"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Languages)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Languages) = NULL;
		lResult = E_NOTIMPL;
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_Languages"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEngineObj::XRecognitionEngine::get_LanguageNames (VARIANT_BOOL EnglishNames, SAFEARRAY **LanguageNames)
{
	METHOD_PROLOGUE(CDaRecognitionEngineObj, RecognitionEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_LanguageNames"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!LanguageNames)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageNames) = NULL;
		lResult = E_NOTIMPL;
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEngineObj::XCommand::get_LanguageNames"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
