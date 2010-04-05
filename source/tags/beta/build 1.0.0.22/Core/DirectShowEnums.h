/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef DIRECTSHOWENUMS_H_INCLUDED_
#define DIRECTSHOWENUMS_H_INCLUDED_
#pragma once

#include "DirectShowPins.h"

/////////////////////////////////////////////////////////////////////////////

class CEnumPins : public CCmdTarget
{
protected:
	CEnumPins (const CEnumPins & pSource);
public:
	CEnumPins (CDirectShowPins & pInputPins, CDirectShowPins & pOutputPins, LPUNKNOWN pOwnerRef = NULL);
	virtual ~CEnumPins ();
	DECLARE_DYNAMIC(CEnumPins)

// Attributes
public:
	CDirectShowPins &	mInputPins;
	CDirectShowPins &	mOutputPins;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CEnumPins)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(Enum, IEnumPins)
        HRESULT STDMETHODCALLTYPE Next (ULONG cPins, IPin **ppPins, ULONG *pcFetched);
        HRESULT STDMETHODCALLTYPE Skip (ULONG cPins);
        HRESULT STDMETHODCALLTYPE Reset ();
        HRESULT STDMETHODCALLTYPE Clone (IEnumPins **ppEnum);
	END_INTERFACE_PART(Enum)

	DECLARE_INTERFACE_MAP()

protected:
	INT_PTR		mCurrNdx;
	IUnknownPtr	mOwnerRef;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CEnumMediaTypes : public CCmdTarget
{
protected:
	CEnumMediaTypes (const CEnumMediaTypes & pSource);
public:
	CEnumMediaTypes (CMediaTypes & pMediaTypes, LPUNKNOWN pOwnerRef = NULL);
	virtual ~CEnumMediaTypes ();
	DECLARE_DYNAMIC(CEnumMediaTypes)

// Attributes
public:
	CMediaTypes &	mMediaTypes;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CEnumMediaTypes)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(Enum, IEnumMediaTypes)
        HRESULT STDMETHODCALLTYPE Next (ULONG cMediaTypes, AM_MEDIA_TYPE **ppMediaTypes, ULONG *pcFetched);
        HRESULT STDMETHODCALLTYPE Skip (ULONG cMediaTypes);
        HRESULT STDMETHODCALLTYPE Reset ();
        HRESULT STDMETHODCALLTYPE Clone (IEnumMediaTypes **ppEnum);
	END_INTERFACE_PART(Enum)

	DECLARE_INTERFACE_MAP()

protected:
	INT_PTR		mCurrNdx;
	IUnknownPtr	mOwnerRef;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWENUMS_H_INCLUDED_
