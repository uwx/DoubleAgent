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
#include "DaCmnAnimationNames.h"

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnAnimationNames::Initialize (CAgentFile & pAgentFile)
{
	const CAgentFileGestures &	lGestures = pAgentFile.GetGestures ();
	INT_PTR						lNdx;

	if	(lGestures.mNames.GetCount() <= 0)
	{
		pAgentFile.ReadGestures ();
	}
	if	(mAnimationNames = new CComVariant [lGestures.mNames.GetCount()+1])
	{
		for	(lNdx = 0; lNdx < (INT_PTR)lGestures.mNames.GetCount(); lNdx++)
		{
			mAnimationNames [lNdx] = lGestures.mNames [lNdx];
		}
	}

	return mEnumVARIANT.Init (&(mAnimationNames[0]), &(mAnimationNames[(INT_PTR)lGestures.mNames.GetCount()]), NULL);
}

HRESULT CDaCmnAnimationNames::Initialize (CDaCmnAnimationNames & pSource, LPUNKNOWN pUnknown)
{
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = mEnumVARIANT.Init (pSource.mEnumVARIANT.m_begin, pSource.mEnumVARIANT.m_end, pUnknown)))
	{
		mEnumVARIANT.m_iter = pSource.mEnumVARIANT.m_iter;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnAnimationNames::get__NewEnum (IUnknown ** ppunkEnum)
{
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	IEnumVARIANTPtr			lInterface;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(lEnum = new CComObject <CEnumVARIANT>)
		{
			if	(SUCCEEDED (lResult = lEnum->Init (&(mAnimationNames[0]), &(mAnimationNames[(INT_PTR)mAnimationNames->GetSize()]), NULL)))
			{
				lInterface = lEnum.Detach ();
				(*ppunkEnum) = lInterface.Detach ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnAnimationNames::get_Item (long Index, BSTR *AnimationName)
{
	HRESULT	lResult = S_OK;

	if	(!AnimationName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AnimationName) = NULL;

		if	(
				(Index >= 0)
			&&	(Index < (long)mAnimationNames->GetSize ())
			)
		{
			(*AnimationName) = _bstr_t (V_BSTR (&mAnimationNames [Index]), true).Detach ();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	return lResult;
}

HRESULT CDaCmnAnimationNames::get_Count (long *Value)
{
	HRESULT	lResult = S_OK;

	if	(!Value)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Value) = mAnimationNames->GetSize();
	}
	return lResult;
}
