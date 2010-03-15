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
#include "DaSvrRecognitionEngines.h"
#include "DaSvrRecognitionEngine.h"
#include "SapiInputCache.h"
#include "Sapi5Inputs.h"
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

IMPLEMENT_DYNAMIC(CDaSvrRecognitionEngines, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaSvrRecognitionEngines, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaSvrRecognitionEngines::CDaSvrRecognitionEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrRecognitionEngines::CDaSvrRecognitionEngines (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}

CDaSvrRecognitionEngines::~CDaSvrRecognitionEngines()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrRecognitionEngines::~CDaSvrRecognitionEngines (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaSvrRecognitionEngines::Terminate (bool pFinal, bool pAbandonned)
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

void CDaSvrRecognitionEngines::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrRecognitionEngines::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

void CDaSvrRecognitionEngines::UseAllInputs ()
{
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;

	if	(
			(lInputCache = CSapiInputCache::GetStaticInstance ())
		&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs())
		)
	{
		mSapi5Inputs.Copy (*lSapi5Inputs);
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSvrRecognitionEngines, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSvrRecognitionEngines)
	DISP_PROPERTY_PARAM_ID(CDaSvrRecognitionEngines, "Item", DISPID_VALUE, DspGetItem, DspSetItem, VT_I4, VTS_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaSvrRecognitionEngines, "Count", DISPID_COLLECT, DspGetCount, DspSetCount, VT_I4)
	DISP_DEFVALUE(CDaSvrRecognitionEngines, "Item")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaSvrRecognitionEngines, CCmdTarget)
	INTERFACE_PART(CDaSvrRecognitionEngines, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines), RecognitionEngines)
	INTERFACE_PART(CDaSvrRecognitionEngines, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaSvrRecognitionEngines, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSvrRecognitionEngines, RecognitionEngines)
IMPLEMENT_DISPATCH_IID(CDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines))
IMPLEMENT_PROVIDECLASSINFO(CDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines))

BEGIN_SUPPORTERRORINFO(CDaSvrRecognitionEngines)
	IMPLEMENT_SUPPORTERRORINFO(CDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines))
END_SUPPORTERRORINFO(CDaSvrRecognitionEngines)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngines::XRecognitionEngines::get_Item (long Index, IDaSvrRecognitionEngine **RecognitionEngine)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngines, RecognitionEngines)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngines::XRecognitionEngines::get_Item"), pThis, pThis->m_dwRef);
#endif
	HRESULT						lResult = S_OK;
	CDaSvrRecognitionEngine *	lRecognitionEngine = NULL;
	IDaSvrRecognitionEnginePtr	lInterface;

	if	(!RecognitionEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngine) = NULL;

		if	(Index < 0)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(Index <= pThis->mSapi5Inputs.GetUpperBound ())
		{
			if	(lRecognitionEngine = new CDaSvrRecognitionEngine (pThis->mSapi5Inputs [Index]))
			{
				lInterface = lRecognitionEngine->GetIDispatch (FALSE);
				(*RecognitionEngine) = lInterface;
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngines::XRecognitionEngines::get_Item"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrRecognitionEngines::XRecognitionEngines::get_Count (long *Count)
{
	METHOD_PROLOGUE(CDaSvrRecognitionEngines, RecognitionEngines)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngines::XRecognitionEngines::get_Count"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)pThis->mSapi5Inputs.GetSize ();
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrRecognitionEngines::XRecognitionEngines::get_Count"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaSvrRecognitionEngines::DspGetItem(long Index)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngines::DspGetItem"), this, m_dwRef);
#endif
	IDaSvrRecognitionEngine *	lRet = NULL;
	HRESULT						lResult = m_xRecognitionEngines.get_Item (Index, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSvrRecognitionEngines::DspSetItem(long Index, LPDISPATCH RecognitionEngine)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngines::DspSetItem"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaSvrRecognitionEngines::DspGetCount()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngines::DspGetCount"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xRecognitionEngines.get_Count (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lRet);
	}
	return lRet;
}

void CDaSvrRecognitionEngines::DspSetCount(long Count)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrRecognitionEngines::DspSetCount"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}
