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
#include "DaSpeechEngineObj.h"
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

IMPLEMENT_DYNAMIC(CDaSpeechEngineObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaSpeechEngineObj, "")
IMPLEMENT_OLETYPELIB(CDaSpeechEngineObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaSpeechEngineObj::CDaSpeechEngineObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Speech Engine")))
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

BEGIN_INTERFACE_MAP(CDaSpeechEngineObj, CCmdTarget)
	INTERFACE_PART(CDaSpeechEngineObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSpeechEngineObj, __uuidof(IDaCtlSpeechEngine), SpeechEngine)
	INTERFACE_PART(CDaSpeechEngineObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaSpeechEngineObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSpeechEngineObj, SpeechEngine)
IMPLEMENT_DISPATCH_IID(CDaSpeechEngineObj, __uuidof(IDaCtlSpeechEngine))
IMPLEMENT_PROVIDECLASSINFO(CDaSpeechEngineObj, __uuidof(CDaSpeechEngineObj))

BEGIN_SUPPORTERRORINFO(CDaSpeechEngineObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaSpeechEngineObj, __uuidof(IDaCtlSpeechEngine))
END_SUPPORTERRORINFO(CDaSpeechEngineObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSpeechEngineObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSpeechEngineObj)
	DISP_PROPERTY_EX_ID(CDaSpeechEngineObj, "TTSModeID", DISPID_VALUE, DspGetTTSModeID, DspSetTTSModeID, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSpeechEngineObj, "DisplayName", DISPID_IDaCtlSpeechEngine_DisplayName, DspGetDisplayName, DspSetDisplayName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSpeechEngineObj, "Manufacturer", DISPID_IDaCtlSpeechEngine_Manufacturer, DspGetManufacturer, DspSetManufacturer, VT_BSTR)
	DISP_FUNCTION_ID(CDaSpeechEngineObj, "GetVersion", DISPID_IDaCtlSpeechEngine_GetVersion, DspGetVersion, VT_EMPTY, VTS_PI2 VTS_PI2)
	DISP_PROPERTY_EX_ID(CDaSpeechEngineObj, "Gender", DISPID_IDaCtlSpeechEngine_Gender, DspGetGender, DspSetGender, VT_I2)
	DISP_PROPERTY_EX_ID(CDaSpeechEngineObj, "LanguageID", DISPID_IDaCtlSpeechEngine_LanguageID, DspGetLanguageID, DspSetLanguageID, VT_I4)
	DISP_PROPERTY_PARAM_ID(CDaSpeechEngineObj, "LanguageName", DISPID_IDaCtlSpeechEngine_LanguageName, DspGetLanguageName, DspSetLanguageName, VT_BSTR, VTS_BOOL)
	DISP_DEFVALUE(CDaSpeechEngineObj, "TTSModeID")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaSpeechEngineObj::CDaSpeechEngineObj (IDaSvrSpeechEngine * pServerObject)
:	mServerObject (pServerObject)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEngineObj::CDaSpeechEngineObj (%d) [%p]"),this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaSpeechEngineObj::~CDaSpeechEngineObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEngineObj::~CDaSpeechEngineObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaSpeechEngineObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEngineObj::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEngineObj::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaSpeechEngineObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEngineObj::OnFinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BSTR CDaSpeechEngineObj::DspGetTTSModeID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetTTSModeID"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechEngine.get_TTSModeID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEngineObj::DspSetTTSModeID(LPCTSTR TTSModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspSetTTSModeID"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSpeechEngineObj::DspGetDisplayName()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetDisplayName"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechEngine.get_DisplayName (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEngineObj::DspSetDisplayName(LPCTSTR DisplayName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspSetDisplayName"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSpeechEngineObj::DspGetManufacturer()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetManufacturer"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechEngine.get_Manufacturer (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEngineObj::DspSetManufacturer(LPCTSTR Manufacturer)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspSetManufacturer"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

void CDaSpeechEngineObj::DspGetVersion(short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetVersion (MajorVersion, MinorVersion);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

short CDaSpeechEngineObj::DspGetGender()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetGender"), this, m_dwRef);
#endif
	short	lRet = GENDER_NEUTRAL;
	HRESULT	lResult = m_xSpeechEngine.get_Gender (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEngineObj::DspSetGender(short Gender)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspSetGender"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaSpeechEngineObj::DspGetLanguageID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetLanguageID"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xSpeechEngine.get_LanguageID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEngineObj::DspSetLanguageID(long LanguageID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspSetLanguageID"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSpeechEngineObj::DspGetLanguageName(BOOL EnglishName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspGetLanguageName"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xSpeechEngine.get_LanguageName (EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEngineObj::DspSetLanguageName(BOOL EnglishName, BSTR LanguageName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::DspSetLanguageName"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::get_TTSModeID (BSTR *TTSModeID)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_TTSModeID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!TTSModeID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSModeID) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetTTSModeID (TTSModeID);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_TTSModeID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::get_DisplayName (BSTR *DisplayName)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_DisplayName"), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_DisplayName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::get_Manufacturer (BSTR *Manufacturer)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_Manufacturer"), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_Manufacturer"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::GetVersion"), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::GetVersion"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::get_Gender (short *Gender)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_Gender"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Gender)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Gender) = GENDER_NEUTRAL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetGender (Gender);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_Gender"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::get_LanguageID (long *LanguageID)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_LanguageID"), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_LanguageID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEngineObj::XSpeechEngine::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
	METHOD_PROLOGUE(CDaSpeechEngineObj, SpeechEngine)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_LanguageName"), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEngineObj::XSpeechEngine::get_LanguageName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
