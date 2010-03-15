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
#include "Sapi5Inputs.h"
#include "MallocPtr.h"
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

CDaSvrRecognitionEngine::CDaSvrRecognitionEngine (CSapi5InputInfo * pInputInfo)
:	mSapi5Input (pInputInfo)
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

CDaSvrRecognitionEngine::~CDaSvrRecognitionEngine ()
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
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetLanguageIDs", DISPID_IDaSvrRecognitionEngine_GetLanguageIDs, DspGetLanguageIDs, VT_EMPTY, VTS_PVARIANT)
	DISP_FUNCTION_ID(CDaSvrRecognitionEngine, "GetLanguageNames", DISPID_IDaSvrRecognitionEngine_GetLanguageNames, DspGetLanguageNames, VT_EMPTY, VTS_PVARIANT VTS_BOOL)
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
	if	(pThis->mSapi5Input)
	{
		(*SRModeID) = SysAllocString (pThis->mSapi5Input->mEngineIdShort);
	}
	else
	{
		lResult = E_FAIL;
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
	if	(pThis->mSapi5Input)
	{
		(*DisplayName) = SysAllocString (pThis->mSapi5Input->mEngineName);
	}
	else
	{
		lResult = E_FAIL;
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
	if	(pThis->mSapi5Input)
	{
		(*Manufacturer) = SysAllocString (pThis->mSapi5Input->mManufacturer);
	}
	else
	{
		lResult = E_FAIL;
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

	if	(MajorVersion)
	{
		(*MajorVersion) = 5;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = 0;
	}

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
		(*LanguageID) = 0;

		if	(pThis->mSapi5Input)
		{
			(*LanguageID) = (long)pThis->mSapi5Input->mLangId;
		}
		else
		{
			lResult = E_FAIL;
		}
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
		(*LanguageName) = NULL;

		if	(pThis->mSapi5Input)
		{
			LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
			int 	lInfoSize;
			CString lInfoValue;

			if	(lInfoSize = GetLocaleInfo (MAKELCID (pThis->mSapi5Input->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
			{
				GetLocaleInfo (MAKELCID (pThis->mSapi5Input->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
			}
			else
			{
				lResult = S_FALSE;
			}
			lInfoValue.ReleaseBuffer ();
			(*LanguageName) = lInfoValue.AllocSysString ();
		}
		else
		{
			lResult = E_FAIL;
		}
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

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageIDs (SAFEARRAY **LanguageIDs)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageIDs"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageIDs)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageIDs) = NULL;

		if	(pThis->mSapi5Input)
		{
			if	((*LanguageIDs) = SafeArrayCreateVector (VT_I4, 0, pThis->mSapi5Input->mLangIdCount))
			{
				long	lNdx;
				long	lLanguageID;

				for	(lNdx = 0; lNdx < (long)pThis->mSapi5Input->mLangIdCount; lNdx++)
				{
					lLanguageID = (long)pThis->mSapi5Input->mLangIdSupported [lNdx];
					SafeArrayPutElement (*LanguageIDs, &lNdx, &lLanguageID);
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageIDs"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}


HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageNames (SAFEARRAY **LanguageNames, boolean EnglishNames)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngine, RecognitionEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageNames"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageNames)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageNames) = NULL;

		if	(pThis->mSapi5Input)
		{
			if	((*LanguageNames) = SafeArrayCreateVector (VT_BSTR, 0, pThis->mSapi5Input->mLangIdCount))
			{
				LCTYPE	lInfoType = EnglishNames ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
				long	lNdx;

				for	(lNdx = 0; lNdx < (long)pThis->mSapi5Input->mLangIdCount; lNdx++)
				{
					int 	lInfoSize;
					CString lInfoValue;

					if	(lInfoSize = GetLocaleInfo (MAKELCID (pThis->mSapi5Input->mLangIdSupported [lNdx], SORT_DEFAULT), lInfoType, NULL, 0))
					{
						GetLocaleInfo (MAKELCID (pThis->mSapi5Input->mLangIdSupported [lNdx], SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
					}
					lInfoValue.ReleaseBuffer ();
					SafeArrayPutElement (*LanguageNames, &lNdx, tBstrPtr(lInfoValue.AllocSysString()));
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngine::XRecognitionEngine::GetLanguageNames"), pThis, pThis->m_dwRef);
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

void CDaSvrRecognitionEngine::DspGetLanguageIDs(SAFEARRAY **LanguageIDs)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetLanguageIDs"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetLanguageIDs (LanguageIDs);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrRecognitionEngine::DspGetLanguageNames(SAFEARRAY **LanguageNames, boolean EnglishNames)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngine::DspGetLanguageNames"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xRecognitionEngine.GetLanguageNames (LanguageNames, EnglishNames);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
