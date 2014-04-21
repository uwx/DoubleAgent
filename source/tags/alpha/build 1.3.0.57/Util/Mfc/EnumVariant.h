/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#if !defined(AFX_ENUMVARIANT_H__53D83B40_700F_45C1_BC45_8097216767B8__INCLUDED_)
#define AFX_ENUMVARIANT_H__53D83B40_700F_45C1_BC45_8097216767B8__INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CEnumVariant : public CCmdTarget
{
protected:
	CEnumVariant (const CEnumVariant & pSource);
public:
	CEnumVariant ();
	virtual ~CEnumVariant ();
	DECLARE_DYNAMIC(CEnumVariant)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CEnumVariant)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(Enum, IEnumVARIANT)
        HRESULT STDMETHODCALLTYPE Next (ULONG celt, VARIANT *rgVar, ULONG *pCeltFetched);
        HRESULT STDMETHODCALLTYPE Skip (ULONG celt);
        HRESULT STDMETHODCALLTYPE Reset ();
        HRESULT STDMETHODCALLTYPE Clone (IEnumVARIANT **ppEnum);
	END_INTERFACE_PART(Enum)

	DECLARE_INTERFACE_MAP()

protected:
	virtual long GetItemCount () = 0;
	virtual void PutItem (long pItemNdx, VARIANT& pItem) = 0;
	virtual CEnumVariant * Clone () = 0;
protected:
	long	mCurrNdx;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMVARIANT_H__53D83B40_700F_45C1_BC45_8097216767B8__INCLUDED_)
