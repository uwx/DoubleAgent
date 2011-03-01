/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "DaCmnUserInput.h"
#include "Sapi5Input.h"
#include "MallocPtr.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnUserInput::CDaCmnUserInput()
{
}

CDaCmnUserInput::~CDaCmnUserInput()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnUserInput::Initialize (ISpRecoResult * pRecoResult, bool pGlobalCommand)
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
			mItemText.Add (CAtlString ((LPCWSTR)lPhraseText));

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
						mItemText.Add (CAtlString ((LPCWSTR)lPhraseText));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnUserInput::get_Count (long *Count)
{
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mItemId.GetCount();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnUserInput::get_ItemCommandID (long ItemIndex, long *ItemCommandID)
{
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
	return lResult;
}

HRESULT CDaCmnUserInput::get_ItemConfidence (long ItemIndex, long *ItemConfidence)
{
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
	return lResult;
}

HRESULT CDaCmnUserInput::get_ItemText (long ItemIndex, BSTR *ItemText)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnUserInput::GetAllItemData (VARIANT *ItemCommandIDs, VARIANT *ItemConfidences, VARIANT *ItemTexts)
{
	HRESULT	lResult = S_OK;
	long	lNdx;

	if	(ItemCommandIDs)
	{
		try
		{
			VariantClear (ItemCommandIDs);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(ItemCommandIDs) = SafeArrayCreateVector (VT_I4, 0, (long)mItemId.GetCount()))
		{
			V_VT(ItemCommandIDs) = VT_I4|VT_ARRAY;

			for	(lNdx = 0; lNdx < (long)mItemId.GetCount(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(ItemCommandIDs), &lNdx, ((long*)mItemId.GetData())+lNdx);
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

	if	(ItemTexts)
	{
		try
		{
			VariantClear (ItemTexts);
		}
		catch AnyExceptionSilent

		if	(V_ARRAY(ItemTexts) = SafeArrayCreateVector (VT_BSTR, 0, (long)mItemText.GetCount()))
		{
			V_VT(ItemTexts) = VT_BSTR|VT_ARRAY;

			for	(lNdx = 0; lNdx < (long)mItemText.GetCount(); lNdx++)
			{
				SafeArrayPutElement (V_ARRAY(ItemTexts), &lNdx, mItemText [lNdx].AllocSysString());
			}
		}
	}
	return lResult;
}
