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
#include "DaSvrRecognitionEngine.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaSvrRecognitionEngine, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaSvrRecognitionEngine, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaSvrRecognitionEngine::CDaSvrRecognitionEngine()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrRecognitionEngine::CDaSvrRecognitionEngine (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}

CDaSvrRecognitionEngine::~CDaSvrRecognitionEngine()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrRecognitionEngine::~CDaSvrRecognitionEngine (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaSvrRecognitionEngine::Terminate (bool pFinal, bool pAbandonned)
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
	}
}

void CDaSvrRecognitionEngine::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrRecognitionEngine::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSvrRecognitionEngine, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSvrRecognitionEngine)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetSRModeID", DISPID_IDaSvrRecognitionEngine_GetSRModeID, DspGetSRModeID, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetDisplayName", DISPID_IDaSvrRecognitionEngine_GetDisplayName, DspGetDisplayName, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetManufacturer", DISPID_IDaSvrRecognitionEngine_GetManufacturer, DspGetManufacturer, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetVersion", DISPID_IDaSvrRecognitionEngine_GetVersion, DspGetVersion, VT_EMPTY, VTS_PI2 VTS_PI2)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetLanguageID", DISPID_IDaSvrRecognitionEngine_GetLanguageID, DspGetLanguageID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetLanguageName", DISPID_IDaSvrRecognitionEngine_GetLanguageName, DspGetLanguageName, VT_EMPTY, VTS_PBSTR VTS_BOOL)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetLanguages", DISPID_IDaSvrRecognitionEngine_GetLanguages, DspGetLanguages, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PBSTR VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaSvrRecognitionEngine, CCmdTarget)
	INTERFACE_PART(CDaSvrRecognitionEngine, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine), RecognitionEngine)
	INTERFACE_PART(CDaSvrRecognitionEngine, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaSvrRecognitionEngine, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSvrRecognitionEngine, RecognitionEngine)
IMPLEMENT_DISPATCH_IID(CDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine))
IMPLEMENT_PROVIDECLASSINFO(CDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine))

BEGIN_SUPPORTERRORINFO(CDaSvrRecognitionEngine)
	IMPLEMENT_SUPPORTERRORINFO(CDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine))
END_SUPPORTERRORINFO(CDaSvrRecognitionEngine)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetSRModeID (BSTR *SRModeID)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetSRModeID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!SRModeID)
	{
		lResult = E_POINTER;
	}
	else
	{
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetSRModeID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetDisplayName (BSTR *DisplayName)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetDisplayName"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	{
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetDisplayName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetManufacturer (BSTR *Manufacturer)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetManufacturer"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	{
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetManufacturer"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetVersion"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;


	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetVersion"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageID (long *LanguageID)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	{
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageName (BSTR *LanguageName, boolean EnglishName)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageName"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	{
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguages (long *Count, long **Languages, BSTR **LanguageNames, boolean EnglishNames)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguages"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguages"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaSvrRecognitionEngine::DspGetSRModeID(BSTR * SRModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetSRModeID"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetSRModeID (SRModeID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetDisplayName(BSTR * DisplayName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetDisplayName"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetDisplayName (DisplayName);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetManufacturer(BSTR * Manufacturer)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetManufacturer"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetManufacturer (Manufacturer);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetVersion(short * MajorVersion, short * MinorVersion)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetVersion (MajorVersion, MinorVersion);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetLanguageID(long * LanguageID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetLanguageID"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetLanguageID (LanguageID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetLanguageName(BSTR * LanguageName, boolean EnglishName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetLanguageName"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetLanguageName (LanguageName, EnglishName);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetLanguages(long *Count, long **Languages, BSTR **LanguageNames, boolean EnglishNames)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetLanguages"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetLanguages (Count, Languages, LanguageNames, EnglishNames);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
