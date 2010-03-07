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
#include "DaAgentUserInput.h"
#include "Sapi5Input.h"
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

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentUserInput, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentUserInput, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentUserInput::CDaAgentUserInput(ISpRecoResult * pRecoResult, bool pGlobalCommand)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentUserInput::CDaAgentUserInput (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	Initialize (pRecoResult, pGlobalCommand);
}

CDaAgentUserInput::~CDaAgentUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentUserInput::~CDaAgentUserInput (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAgentUserInput::Terminate (bool pFinal, bool pAbandonned)
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

void CDaAgentUserInput::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentUserInput::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentUserInput, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentUserInput)
	DISP_FUNCTION_ID(CDaAgentUserInput, "GetCount", DISPID_IAgentUserInput_GetCount, DspGetCount, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentUserInput, "GetItemID", DISPID_IAgentUserInput_GetItemID, DspGetItemid, VT_EMPTY, VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentUserInput, "GetItemConfidence", DISPID_IAgentUserInput_GetItemConfidence, DspGetItemConfidence, VT_EMPTY, VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentUserInput, "GetItemText", DISPID_IAgentUserInput_GetItemText, DspGetItemText, VT_EMPTY, VTS_I4 VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentUserInput, "GetAllItemData", DISPID_IAgentUserInput_GetAllItemData, DspGetAllItemData, VT_EMPTY, VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentUserInput, CCmdTarget)
	INTERFACE_PART(CDaAgentUserInput, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentUserInput, __uuidof(IDaSvrUserInput), UserInput)
	INTERFACE_PART(CDaAgentUserInput, __uuidof(IAgentUserInput), UserInput)
	INTERFACE_PART(CDaAgentUserInput, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentUserInput, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentUserInput, UserInput)
IMPLEMENT_DISPATCH_IID(CDaAgentUserInput, __uuidof(IDaSvrUserInput))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentUserInput, __uuidof(IDaSvrUserInput))

BEGIN_SUPPORTERRORINFO(CDaAgentUserInput)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentUserInput, __uuidof(IDaSvrUserInput))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentUserInput, __uuidof(IAgentUserInput))
END_SUPPORTERRORINFO(CDaAgentUserInput)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentUserInput::Initialize (ISpRecoResult * pRecoResult, bool pGlobalCommand)
{
	if	(pRecoResult)
	{
		tMallocPtr <SPPHRASE>	lPhrase;
		tMallocPtr <WCHAR>		lPhraseText;
		BYTE					lPhraseDisplay;
		ULONG					lAlternateCount;
		ULONG					lAlternateNdx;
		ISpPhraseAltPtr			lAlternates [100];

		if	(
				(SUCCEEDED (pRecoResult->GetPhrase (lPhrase.Free())))
			&&	(SUCCEEDED (pRecoResult->GetText (SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, lPhraseText.Free(), &lPhraseDisplay)))
			)
		{
			mItemId.Add (pGlobalCommand ? 0 : lPhrase->Rule.ulId);
			mItemConfidence.Add ((long)lPhrase->Rule.Confidence*100);
			mItemText.Add (CString ((LPCWSTR)lPhraseText));

			if	(SUCCEEDED (pRecoResult->GetAlternates (0, SPPR_ALL_ELEMENTS, 100, (ISpPhraseAlt**)lAlternates, &lAlternateCount)))
			{
				for	(lAlternateNdx = 1; lAlternateNdx < lAlternateCount; lAlternateNdx++)
				{
#ifdef	_STRICT_COMPATIBILITY
					if	(lAlternateNdx > 2)
					{
						break;
					}
#endif
					if	(
							(SUCCEEDED (lAlternates [lAlternateNdx]->GetPhrase (lPhrase.Free())))
						&&	(SUCCEEDED (lAlternates [lAlternateNdx]->GetText (SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, lPhraseText.Free(), &lPhraseDisplay)))
						)
					{
						mItemId.Add (pGlobalCommand ? 0 : lPhrase->Rule.ulId);
						mItemConfidence.Add ((long)lPhrase->Rule.Confidence*100);
						mItemText.Add (CString ((LPCWSTR)lPhraseText));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentUserInput::XUserInput::GetCount (long *pdwCount)
{
	METHOD_PROLOGUE(CDaAgentUserInput, UserInput)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetCount"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pdwCount = (long)pThis->mItemId.GetSize();
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetCount"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentUserInput::XUserInput::GetItemID (long dwItemIndex, long *pdwCommandID)
{
	METHOD_PROLOGUE(CDaAgentUserInput, UserInput)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetItemID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCommandID)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(dwItemIndex < 0)
		||	(dwItemIndex > pThis->mItemId.GetUpperBound())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		*pdwCommandID = pThis->mItemId [dwItemIndex];
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetItemID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentUserInput::XUserInput::GetItemConfidence (long dwItemIndex, long *plConfidence)
{
	METHOD_PROLOGUE(CDaAgentUserInput, UserInput)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetItemConfidence"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!plConfidence)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(dwItemIndex < 0)
		||	(dwItemIndex > pThis->mItemConfidence.GetUpperBound())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		*plConfidence = pThis->mItemConfidence [dwItemIndex];
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetItemConfidence"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentUserInput::XUserInput::GetItemText (long dwItemIndex, BSTR *pbszText)
{
	METHOD_PROLOGUE(CDaAgentUserInput, UserInput)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetItemText"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszText)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(dwItemIndex < 0)
		||	(dwItemIndex > pThis->mItemText.GetUpperBound())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		*pbszText = pThis->mItemText [dwItemIndex].AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetItemText"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentUserInput::XUserInput::GetAllItemData (VARIANT *pdwItemIndices, VARIANT *plConfidences, VARIANT *pbszText)
{
	METHOD_PROLOGUE(CDaAgentUserInput, UserInput)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetAllItemData"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lNdx;

	if	(pdwItemIndices)
	{
		try
		{
			VariantClear (pdwItemIndices);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(pdwItemIndices) = SafeArrayCreateVector (VT_I4, 0, (long)pThis->mItemId.GetSize()))
		{
			V_VT(pdwItemIndices) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx <= pThis->mItemId.GetUpperBound(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(pdwItemIndices), &lNdx, ((long*)pThis->mItemId.GetData())+lNdx);
			}
		}
	}

	if	(plConfidences)
	{
		try
		{
			VariantClear (plConfidences);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(plConfidences) = SafeArrayCreateVector (VT_I4, 0, (long)pThis->mItemConfidence.GetSize()))
		{
			V_VT(plConfidences) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx <= pThis->mItemConfidence.GetUpperBound(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(plConfidences), &lNdx, ((long*)pThis->mItemConfidence.GetData())+lNdx);
			}
		}
	}

	if	(pbszText)
	{
		try
		{
			VariantClear (pbszText);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(pbszText) = SafeArrayCreateVector (VT_BSTR, 0, (long)pThis->mItemText.GetSize()))
		{
			V_VT(pbszText) = VT_BSTR|VT_ARRAY;

			for	(lNdx = 0; lNdx <= pThis->mItemText.GetUpperBound(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(pbszText), &lNdx, pThis->mItemText [lNdx].AllocSysString());
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentUserInput::XUserInput::GetAllItemData"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentUserInput::DspGetCount(long * Count)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentUserInput::DspGetCount"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xUserInput.GetCount (Count);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentUserInput::DspGetItemid(long ItemIndex, long * CommandID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentUserInput::DspGetItemid"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xUserInput.GetItemID (ItemIndex, CommandID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentUserInput::DspGetItemConfidence(long ItemIndex, long * Confidence)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentUserInput::DspGetItemConfidence"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xUserInput.GetItemConfidence (ItemIndex, Confidence);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentUserInput::DspGetItemText(long ItemIndex, BSTR * Text)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentUserInput::DspGetItemText"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xUserInput.GetItemText (ItemIndex, Text);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentUserInput::DspGetAllItemData(VARIANT * ItemIndices, VARIANT * Confidences, VARIANT * Text)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentUserInput::DspGetAllItemData"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xUserInput.GetAllItemData (ItemIndices, Confidences, Text);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
