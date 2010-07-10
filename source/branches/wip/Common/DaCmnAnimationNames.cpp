/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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

HRESULT CDaCmnAnimationNames::get__NewEnum (IUnknown ** EnumVariant)
{
	HRESULT								lResult = S_OK;
	tPtr <CComObject<CEnumVARIANT> >	lEnumVariant;
	IEnumVARIANTPtr						lInterface;

	if	(!EnumVariant)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*EnumVariant) = NULL;

		if	(
				(SUCCEEDED (lResult = CComObject<CEnumVARIANT>::CreateInstance (lEnumVariant.Free())))
			&&	(SUCCEEDED (lResult = lEnumVariant->Init (mEnumVARIANT.m_begin, mEnumVARIANT.m_end, mEnumVARIANT.m_spUnk)))
			)
		{
			lInterface = lEnumVariant.Detach ();
			(*EnumVariant) = lInterface.Detach ();
		}
	}
	return lResult;
}
