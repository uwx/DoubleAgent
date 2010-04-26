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
			(InlineIsEqualGUID (__uuidof(IDaSvrUserInput2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrUserInput), riid))
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

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetCount (long *Count)
{
	return get_Count (Count);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_Count"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mItemId.GetCount();
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemID (long ItemIndex, long *CommandID)
{
	return get_ItemCommandID (ItemIndex, CommandID);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_ItemCommandID (long ItemIndex, long *ItemCommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_ItemCommandID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ItemCommandID)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(ItemIndex < 0)
		||	(ItemIndex >= (long)mItemId.GetCount())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		(*ItemCommandID) = mItemId [ItemIndex];
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_ItemCommandID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemConfidence (long ItemIndex, long *Confidence)
{
	return get_ItemConfidence (ItemIndex, Confidence);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_ItemConfidence (long ItemIndex, long *ItemConfidence)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_ItemConfidence"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ItemConfidence)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(ItemIndex < 0)
		||	(ItemIndex >= (long)mItemConfidence.GetCount())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		(*ItemConfidence) = mItemConfidence [ItemIndex];
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_ItemConfidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemText (long ItemIndex, BSTR *Text)
{
	return get_ItemText (ItemIndex, Text);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_ItemText (long ItemIndex, BSTR *ItemText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_ItemText"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ItemText)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(ItemIndex < 0)
		||	(ItemIndex >= (long)mItemText.GetCount())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		(*ItemText) = mItemText [ItemIndex].AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_ItemText"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetAllItemData (VARIANT *ItemIndices, VARIANT *ItemConfidences, VARIANT *ItemText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetAllItemData"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lNdx;

	if	(ItemIndices)
	{
		try
		{
			VariantClear (ItemIndices);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(ItemIndices) = SafeArrayCreateVector (VT_I4, 0, (long)mItemId.GetCount()))
		{
			V_VT(ItemIndices) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx < (long)mItemId.GetCount(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(ItemIndices), &lNdx, ((long*)mItemId.GetData())+lNdx);
			}
		}
	}

	if	(ItemConfidences)
	{
		try
		{
			VariantClear (ItemConfidences);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(ItemConfidences) = SafeArrayCreateVector (VT_I4, 0, (long)mItemConfidence.GetCount()))
		{
			V_VT(ItemConfidences) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx < (long)mItemConfidence.GetCount(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(ItemConfidences), &lNdx, ((long*)mItemConfidence.GetData())+lNdx);
			}
		}
	}

	if	(ItemText)
	{
		try
		{
			VariantClear (ItemText);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(ItemText) = SafeArrayCreateVector (VT_BSTR, 0, (long)mItemText.GetCount()))
		{
			V_VT(ItemText) = VT_BSTR|VT_ARRAY;

			for	(lNdx = 0; lNdx < (long)mItemText.GetCount(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(ItemText), &lNdx, mItemText [lNdx].AllocSysString());
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetAllItemData"), this, m_dwRef);
	}
#endif
	return lResult;
}
