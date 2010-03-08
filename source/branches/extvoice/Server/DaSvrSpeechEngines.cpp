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
#include "DaSvrSpeechEngines.h"
#include "DaSvrSpeechEngine.h"
#include "SapiVoiceCache.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif
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

IMPLEMENT_DYNAMIC(CDaSvrSpeechEngines, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaSvrSpeechEngines, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaSvrSpeechEngines::CDaSvrSpeechEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngines::CDaSvrSpeechEngines (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}

CDaSvrSpeechEngines::~CDaSvrSpeechEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngines::~CDaSvrSpeechEngines (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaSvrSpeechEngines::Terminate (bool pFinal, bool pAbandonned)
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

void CDaSvrSpeechEngines::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrSpeechEngines::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

void CDaSvrSpeechEngines::UseAllVoices ()
{
	CSapiVoiceCache *	lVoiceCache;
	
	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		CSapi5Voices *	lSapi5Voices;
		
		if	(lSapi5Voices = lVoiceCache->GetSapi5Voices())
		{
			mSapi5Voices.Copy (*lSapi5Voices);
		}
#ifndef	_WIN64
		CSapi4Voices *	lSapi4Voices;

		if	(lSapi4Voices = lVoiceCache->GetSapi4Voices())
		{
			mSapi4Voices.Copy (*lSapi4Voices);
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSvrSpeechEngines, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSvrSpeechEngines)
	DISP_PROPERTY_PARAM_ID(CDaSvrSpeechEngines, "Item", DISPID_VALUE, DspGetItem, DspSetItem, VT_I4, VTS_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaSvrSpeechEngines, "Count", DISPID_COLLECT, DspGetCount, DspSetCount, VT_I4)
	DISP_DEFVALUE(CDaSvrSpeechEngines, "Item")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaSvrSpeechEngines, CCmdTarget)
	INTERFACE_PART(CDaSvrSpeechEngines, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines), SpeechEngines)
	INTERFACE_PART(CDaSvrSpeechEngines, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaSvrSpeechEngines, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSvrSpeechEngines, SpeechEngines)
IMPLEMENT_DISPATCH_IID(CDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines))
IMPLEMENT_PROVIDECLASSINFO(CDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines))

BEGIN_SUPPORTERRORINFO(CDaSvrSpeechEngines)
	IMPLEMENT_SUPPORTERRORINFO(CDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines))
END_SUPPORTERRORINFO(CDaSvrSpeechEngines)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngines::XSpeechEngines::get_Item (long Index, IDaSvrSpeechEngine **SpeechEngine)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngines, SpeechEngines)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngines::XSpeechEngines::get_Item"), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	CDaSvrSpeechEngine *	lSpeechEngine = NULL;
	IDaSvrSpeechEnginePtr	lInterface;

	if	(!SpeechEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngine) = NULL;

		if	(Index < 0)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(Index <= pThis->mSapi5Voices.GetUpperBound ())
		{
			if	(lSpeechEngine = new CDaSvrSpeechEngine (pThis->mSapi5Voices [Index]))
			{
				lInterface = lSpeechEngine->GetIDispatch (FALSE);
				(*SpeechEngine) = lInterface;
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
#ifndef	_WIN64
		else
		if	(Index - pThis->mSapi5Voices.GetSize() <= pThis->mSapi4Voices.GetUpperBound ())
		{
			Index -= pThis->mSapi5Voices.GetSize();
			if	(lSpeechEngine = new CDaSvrSpeechEngine (pThis->mSapi4Voices [Index]))
			{
				lInterface = lSpeechEngine->GetIDispatch (FALSE);
				(*SpeechEngine) = lInterface;
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
#endif
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngines::XSpeechEngines::get_Item"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrSpeechEngines::XSpeechEngines::get_Count (long *Count)
{
	METHOD_PROLOGUE(CDaSvrSpeechEngines, SpeechEngines)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrSpeechEngines::XSpeechEngines::get_Count"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)pThis->mSapi5Voices.GetSize ();
#ifndef	_WIN64
		(*Count) += (long)pThis->mSapi4Voices.GetSize ();
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrSpeechEngines::XSpeechEngines::get_Count"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaSvrSpeechEngines::DspGetItem(long Index)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngines::DspGetItem"), this, m_dwRef);
#endif
	IDaSvrSpeechEngine *	lRet = NULL;
	HRESULT					lResult = m_xSpeechEngines.get_Item (Index, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSvrSpeechEngines::DspSetItem(long Index, LPDISPATCH SpeechEngine)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngines::DspSetItem"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaSvrSpeechEngines::DspGetCount()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngines::DspGetCount"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xSpeechEngines.get_Count (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lRet);
	}
	return lRet;
}

void CDaSvrSpeechEngines::DspSetCount(long Count)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrSpeechEngines::DspSetCount"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}
