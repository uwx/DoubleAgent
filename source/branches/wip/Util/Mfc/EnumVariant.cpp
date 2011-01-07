/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "EnumVariant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEnumVariant, CCmdTarget)

CEnumVariant::CEnumVariant (const CEnumVariant & pSource)
:	mCurrNdx (pSource.mCurrNdx)
{
	EnableAggregation();
}

CEnumVariant::CEnumVariant ()
:	mCurrNdx (-1)
{
	EnableAggregation();
}

CEnumVariant::~CEnumVariant ()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CEnumVariant, CCmdTarget)
	INTERFACE_PART(CEnumVariant, __uuidof(IEnumVARIANT), Enum)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE CEnumVariant::XEnum::AddRef ()
{
	METHOD_PROLOGUE_EX_(CEnumVariant, Enum)
	return pThis->ExternalAddRef ();
}

ULONG STDMETHODCALLTYPE CEnumVariant::XEnum::Release ()
{
	METHOD_PROLOGUE_EX_(CEnumVariant, Enum)
	return pThis->ExternalRelease ();
}

HRESULT STDMETHODCALLTYPE CEnumVariant::XEnum::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX(CEnumVariant, Enum)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumVariant::XEnum::Next (ULONG celt, VARIANT *rgVar, ULONG *pCeltFetched)
{
	METHOD_PROLOGUE(CEnumVariant, Enum)

	HRESULT	lResult = S_FALSE;
	long	lNdx = 0;
	ULONG	lCount = celt;
	ULONG	lFetched = 0;

	if	(!rgVar)
	{
		lResult = E_POINTER;
	}
	else
	{
		while (lCount > 0)
		{
			VariantClear (rgVar+lNdx);
			if	(pThis->mCurrNdx < pThis->GetItemCount()-1)
			{
				pThis->PutItem (++pThis->mCurrNdx, rgVar [lNdx]);
				lFetched++;
				lResult = S_OK;
			}
			else
			{
				lResult = S_FALSE;
			}
			lNdx++;
			lCount--;
		}
	}
	if	(pCeltFetched)
	{
		(*pCeltFetched) = lFetched;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumVariant::XEnum::Skip (ULONG celt)
{
	METHOD_PROLOGUE(CEnumVariant, Enum)

	HRESULT	lResult = S_OK;

	pThis->mCurrNdx += (long)celt;
	if	(pThis->mCurrNdx > pThis->GetItemCount())
	{
		pThis->mCurrNdx = pThis->GetItemCount();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumVariant::XEnum::Reset ()
{
	METHOD_PROLOGUE(CEnumVariant, Enum)

	HRESULT	lResult = S_OK;

	if	(pThis->mCurrNdx >= 0)
	{
		pThis->mCurrNdx = -1;
	}
	else
	{
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumVariant::XEnum::Clone (IEnumVARIANT **ppEnum)
{
	METHOD_PROLOGUE(CEnumVariant, Enum)

	HRESULT			lResult = S_OK;
	CEnumVariant *	lClone;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(lClone = pThis->Clone())
	{
		(*ppEnum) = &lClone->m_xEnum;
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}
	return lResult;
}
