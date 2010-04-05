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
#pragma once
#include "DirectShowPins.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CEnumPins :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IEnumPins
{
public:
	CEnumPins ();
	~CEnumPins ();

// Declarations
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CEnumPins)
		COM_INTERFACE_ENTRY(IEnumPins)
	END_COM_MAP()

// Attributes
public:
	CDirectShowPins *	mInputPins;
	CDirectShowPins *	mOutputPins;

// Operations
public:
	void Initialize (CDirectShowPins & pInputPins, CDirectShowPins & pOutputPins, LPUNKNOWN pOwnerRef = NULL);
	CEnumPins & operator= (const CEnumPins & pSource);

// Interfaces
public:
	// IEnumPins
    HRESULT STDMETHODCALLTYPE Next (ULONG cPins, IPin **ppPins, ULONG *pcFetched);
    HRESULT STDMETHODCALLTYPE Skip (ULONG cPins);
    HRESULT STDMETHODCALLTYPE Reset ();
    HRESULT STDMETHODCALLTYPE Clone (IEnumPins **ppEnum);

// Implementation
protected:
	INT_PTR		mCurrNdx;
	IUnknownPtr	mOwnerRef;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CEnumMediaTypes :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IEnumMediaTypes
{
public:
	CEnumMediaTypes ();
	~CEnumMediaTypes ();
	void Initialize (CMediaTypes & pMediaTypes, LPUNKNOWN pOwnerRef = NULL);

// Declarations
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CEnumMediaTypes)
		COM_INTERFACE_ENTRY(IEnumMediaTypes)
	END_COM_MAP()

// Attributes
public:
	CMediaTypes *	mMediaTypes;

// Operations
public:
	CEnumMediaTypes & operator= (const CEnumMediaTypes & pSource);

// Interfaces
public:
	// IEnumMediaTypes
    HRESULT STDMETHODCALLTYPE Next (ULONG cMediaTypes, AM_MEDIA_TYPE **ppMediaTypes, ULONG *pcFetched);
    HRESULT STDMETHODCALLTYPE Skip (ULONG cMediaTypes);
    HRESULT STDMETHODCALLTYPE Reset ();
    HRESULT STDMETHODCALLTYPE Clone (IEnumMediaTypes **ppEnum);

// Implementation
protected:
	INT_PTR		mCurrNdx;
	IUnknownPtr	mOwnerRef;
};

/////////////////////////////////////////////////////////////////////////////
