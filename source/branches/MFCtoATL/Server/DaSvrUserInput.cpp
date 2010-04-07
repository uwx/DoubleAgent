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
#include "DaSvrUserInput.h"
#include "Sapi5Input.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrUserInput::DaSvrUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrUserInput::DaSvrUserInput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrUserInput::~DaSvrUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrUserInput::~DaSvrUserInput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrUserInput * DaSvrUserInput::CreateInstance (ISpRecoResult * pRecoResult, bool pGlobalCommand)
{
	CComObject<DaSvrUserInput> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrUserInput>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize (pRecoResult, pGlobalCommand);
	}
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrUserInput::Terminate (bool pFinal, bool pAbandonned)
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
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}
	}
}

void DaSvrUserInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrUserInput::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaSvrUserInput::Initialize (ISpRecoResult * pRecoResult, bool pGlobalCommand)
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

STDMETHODIMP DaSvrUserInput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrUserInput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentUserInput), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetCount (long *pdwCount)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetCount"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		*pdwCount = (long)mItemId.GetSize();
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetCount"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemID (long dwItemIndex, long *pdwCommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetItemID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCommandID)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(dwItemIndex < 0)
		||	(dwItemIndex > mItemId.GetUpperBound())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		*pdwCommandID = mItemId [dwItemIndex];
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetItemID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemConfidence (long dwItemIndex, long *plConfidence)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetItemConfidence"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!plConfidence)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(dwItemIndex < 0)
		||	(dwItemIndex > mItemConfidence.GetUpperBound())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		*plConfidence = mItemConfidence [dwItemIndex];
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetItemConfidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemText (long dwItemIndex, BSTR *pbszText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetItemText"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszText)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(dwItemIndex < 0)
		||	(dwItemIndex > mItemText.GetUpperBound())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		*pbszText = mItemText [dwItemIndex].AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetItemText"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetAllItemData (VARIANT *pdwItemIndices, VARIANT *plConfidences, VARIANT *pbszText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetAllItemData"), this, m_dwRef);
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

		if	(V_ARRAY(pdwItemIndices) = SafeArrayCreateVector (VT_I4, 0, (long)mItemId.GetSize()))
		{
			V_VT(pdwItemIndices) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx <= mItemId.GetUpperBound(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(pdwItemIndices), &lNdx, ((long*)mItemId.GetData())+lNdx);
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

		if	(V_ARRAY(plConfidences) = SafeArrayCreateVector (VT_I4, 0, (long)mItemConfidence.GetSize()))
		{
			V_VT(plConfidences) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx <= mItemConfidence.GetUpperBound(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(plConfidences), &lNdx, ((long*)mItemConfidence.GetData())+lNdx);
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

		if	(V_ARRAY(pbszText) = SafeArrayCreateVector (VT_BSTR, 0, (long)mItemText.GetSize()))
		{
			V_VT(pbszText) = VT_BSTR|VT_ARRAY;

			for	(lNdx = 0; lNdx <= mItemText.GetUpperBound(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(pbszText), &lNdx, mItemText [lNdx].AllocSysString());
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetAllItemData"), this, m_dwRef);
	}
#endif
	return lResult;
}
