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
#include "DaSvrSpeechEngine.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif
#include "GuidStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaSvrSpeechEngine, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaSvrSpeechEngine, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaSvrSpeechEngine::CDaSvrSpeechEngine (CSapi5VoiceInfo * pVoiceInfo)
:	mSapi5Voice (pVoiceInfo)
{
#ifndef	_WIN64
	mSapi4Voice = NULL;
#endif	
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngine::CDaSvrSpeechEngine (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}

#ifndef	_WIN64
CDaSvrSpeechEngine::CDaSvrSpeechEngine (CSapi4VoiceInfo * pVoiceInfo)
:	mSapi4Voice (pVoiceInfo),
	mSapi5Voice (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngine::CDaSvrSpeechEngine (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}
#endif

CDaSvrSpeechEngine::~CDaSvrSpeechEngine()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngine::~CDaSvrSpeechEngine (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaSvrSpeechEngine::Terminate (bool pFinal, bool pAbandonned)
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

void CDaSvrSpeechEngine::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngine::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSvrSpeechEngine, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSvrSpeechEngine)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetTTSModeID", DISPID_IDaSvrSpeechEngine_GetTTSModeID, DspGetTTSModeID, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetDisplayName", DISPID_IDaSvrSpeechEngine_GetDisplayName, DspGetDisplayName, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetManufacturer", DISPID_IDaSvrSpeechEngine_GetManufacturer, DspGetManufacturer, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetVersion", DISPID_IDaSvrSpeechEngine_GetVersion, DspGetVersion, VT_EMPTY, VTS_PI2 VTS_PI2)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetGender", DISPID_IDaSvrSpeechEngine_GetGender, DspGetGender, VT_EMPTY, VTS_PI2)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetLanguageID", DISPID_IDaSvrSpeechEngine_GetLanguageID, DspGetLanguageID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaSvrSpeechEngine, "GetLanguageName", DISPID_IDaSvrSpeechEngine_GetLanguageName, DspGetLanguageName, VT_EMPTY, VTS_PBSTR VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaSvrSpeechEngine, CCmdTarget)
	INTERFACE_PART(CDaSvrSpeechEngine, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine), SpeechEngine)
	INTERFACE_PART(CDaSvrSpeechEngine, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaSvrSpeechEngine, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSvrSpeechEngine, SpeechEngine)
IMPLEMENT_DISPATCH_IID(CDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine))
IMPLEMENT_PROVIDECLASSINFO(CDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine))

BEGIN_SUPPORTERRORINFO(CDaSvrSpeechEngine)
	IMPLEMENT_SUPPORTERRORINFO(CDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine))
END_SUPPORTERRORINFO(CDaSvrSpeechEngine)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetTTSModeID (BSTR *TTSModeID)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetTTSModeID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!TTSModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSapi5Voice)
	{
		(*TTSModeID) = SysAllocString (pThis->mSapi5Voice->mVoiceIdShort);
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		CString	lTTSModeId = (CString) CGuidStr (pThis->mSapi4Voice->mModeId);
		(*TTSModeID) = lTTSModeId.AllocSysString ();
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetTTSModeID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetDisplayName (BSTR *DisplayName)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetDisplayName"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSapi5Voice)
	{
		(*DisplayName) = SysAllocString (pThis->mSapi5Voice->mVoiceName);
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		(*DisplayName) = SysAllocString (pThis->mSapi4Voice->mVoiceName);
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetDisplayName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetManufacturer (BSTR *Manufacturer)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetManufacturer"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSapi5Voice)
	{
		(*Manufacturer) = SysAllocString (pThis->mSapi5Voice->mManufacturer);
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		(*Manufacturer) = SysAllocString (pThis->mSapi4Voice->mManufacturer);
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetManufacturer"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetVersion"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mSapi5Voice)
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = 5;
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = 0;
		}
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = 4;
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = 0;
		}
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetVersion"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetGender (short *Gender)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetGender"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Gender)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSapi5Voice)
	{
		(*Gender) = (short)pThis->mSapi5Voice->mSpeakerGender;
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		(*Gender) = (short)pThis->mSapi4Voice->mSpeakerGender;
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetGender"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetLanguageID (long *LanguageID)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetLanguageID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSapi5Voice)
	{
		(*LanguageID) = (long)pThis->mSapi5Voice->mLangId;
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		(*LanguageID) = (long)pThis->mSapi4Voice->mLangId;
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetLanguageID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngine::XSpeechEngine::GetLanguageName (BSTR *LanguageName, boolean EnglishName)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngine, SpeechEngine)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetLanguageName"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mSapi5Voice)
	{
		LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
		int 	lInfoSize;
		CString lInfoValue;

		if	(lInfoSize = GetLocaleInfo (MAKELCID (pThis->mSapi5Voice->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
		{
			GetLocaleInfo (MAKELCID (pThis->mSapi5Voice->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
		}
		else
		{
			lResult = S_FALSE;
		}
		lInfoValue.ReleaseBuffer ();
		(*LanguageName) = lInfoValue.AllocSysString ();
	}
#ifndef	_WIN64
	else
	if	(pThis->mSapi4Voice)
	{
		LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
		int 	lInfoSize;
		CString lInfoValue;

		if	(lInfoSize = GetLocaleInfo (MAKELCID (pThis->mSapi4Voice->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
		{
			GetLocaleInfo (MAKELCID (pThis->mSapi4Voice->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
		}
		else
		{
			lResult = S_FALSE;
		}
		lInfoValue.ReleaseBuffer ();
		(*LanguageName) = lInfoValue.AllocSysString ();
	}
#endif	
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngine::XSpeechEngine::GetLanguageName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaSvrSpeechEngine::DspGetTTSModeID(BSTR * TTSModeID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetTTSModeID"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetTTSModeID (TTSModeID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrSpeechEngine::DspGetDisplayName(BSTR * DisplayName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetDisplayName"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetDisplayName (DisplayName);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrSpeechEngine::DspGetManufacturer(BSTR * Manufacturer)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetManufacturer"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetManufacturer (Manufacturer);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrSpeechEngine::DspGetVersion(short * MajorVersion, short * MinorVersion)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetVersion (MajorVersion, MinorVersion);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrSpeechEngine::DspGetGender(short * Gender)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetGender"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetGender (Gender);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrSpeechEngine::DspGetLanguageID(long * LanguageID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetLanguageID"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetLanguageID (LanguageID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaSvrSpeechEngine::DspGetLanguageName(BSTR * LanguageName, boolean EnglishName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngine::DspGetLanguageName"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xSpeechEngine.GetLanguageName (LanguageName, EnglishName);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
