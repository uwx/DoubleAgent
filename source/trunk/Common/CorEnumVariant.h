/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#pragma once

#ifdef	_DEBUG
#include "DebugStr.h"
#include "Registry.h"
#define	_DEBUG_ENUMVARIANT	(GetProfileDebugInt(_T("DebugEnumVariant"),LogVerbose,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

template <class Owner> class CCorEnumVariant :
	public CComTearOffObjectBase <Owner>,
	public CEnumVARIANTImpl,
	public IDispatchImpl<mscorlib::IEnumerable, &__uuidof(mscorlib::IEnumerable), &GUID_NULL>,
	public IDispatchImpl<mscorlib::IList, &__uuidof(mscorlib::IList), &GUID_NULL>,
	public IDispatchImpl<mscorlib::ICollection, &__uuidof(mscorlib::ICollection), &GUID_NULL>
{
public:
	CCorEnumVariant () {}
	virtual ~CCorEnumVariant () {}

// Attributes
	Owner * SafeGetOwner () const {return (this==NULL)?NULL:m_pOwner;}
	int SafeGetOwnerUsed () const {return (this==NULL)?-1:(m_pOwner==NULL)?-1:(int)max(m_pOwner->m_dwRef,-1);}

// Operations
public:
	HRESULT FinalConstruct ()
	{
#ifdef	_DEBUG_ENUMVARIANT
		if	(LogIsActive (_DEBUG_ENUMVARIANT))
		{
			LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::FinalConstruct"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
		}
#endif

		HRESULT	lResult = m_pOwner->InitEnumVariant (this);

#ifdef	_DEBUG_ENUMVARIANT
		if	(LogIsActive (_DEBUG_ENUMVARIANT))
		{
			LogComErrAnon (MinLogLevel(_DEBUG_ENUMVARIANT,LogAlways), lResult, _T("[%p(%d)] [%p] %s::FinalConstruct [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this), (long)(m_end-m_begin));
		}
#endif
		return lResult;
	}

// Declarations
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(CCorEnumVariant<Owner>)
		COM_INTERFACE_ENTRY(IEnumVARIANT)
		COM_INTERFACE_ENTRY(mscorlib::IEnumerable)
		COM_INTERFACE_ENTRY(mscorlib::IList)
		COM_INTERFACE_ENTRY(mscorlib::ICollection)
	END_COM_MAP()

// Interfaces
public:
	// IEnumerable
	HRESULT STDMETHODCALLTYPE cor_GetEnumerator (IEnumVARIANT ** pRetVal);

	// IList
	HRESULT STDMETHODCALLTYPE corget_Item (long index, VARIANT* pRetVal);
	HRESULT STDMETHODCALLTYPE corputref_Item (long index, VARIANT pRetVal);
	HRESULT STDMETHODCALLTYPE cor_Add (VARIANT value, long* pRetVal);
	HRESULT STDMETHODCALLTYPE cor_Contains (VARIANT value, VARIANT_BOOL* pRetVal);
	HRESULT STDMETHODCALLTYPE cor_Clear ();
	HRESULT STDMETHODCALLTYPE corget_IsReadOnly (VARIANT_BOOL* pRetVal);
	HRESULT STDMETHODCALLTYPE corget_IsFixedSize (VARIANT_BOOL* pRetVal);
	HRESULT STDMETHODCALLTYPE cor_IndexOf (VARIANT value, long* pRetVal);
	HRESULT STDMETHODCALLTYPE cor_Insert (long index, VARIANT value);
	HRESULT STDMETHODCALLTYPE cor_Remove (VARIANT value);
	HRESULT STDMETHODCALLTYPE cor_RemoveAt (long index);

	// ICollection
	HRESULT STDMETHODCALLTYPE cor_CopyTo (struct mscorlib::_Array * Array, long index);
	HRESULT STDMETHODCALLTYPE corget_Count (long* pRetVal);
	HRESULT STDMETHODCALLTYPE corget_SyncRoot (VARIANT * pRetVal);
	HRESULT STDMETHODCALLTYPE corget_IsSynchronized (VARIANT_BOOL * pRetVal);
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_GetEnumerator (IEnumVARIANT ** pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_GetEnumerator"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	HRESULT	lResult = S_OK;

	if	(pRetVal)
	{
		IEnumVARIANTPtr	lEnumVariant (this);
		lEnumVariant->Reset ();
		(*pRetVal) = lEnumVariant.Detach();
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corget_Item (long index, VARIANT* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corget_Item [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this), index);
#endif
	HRESULT	lResult = S_OK;
	if	(pRetVal)
	{
		VariantInit (pRetVal);

		if	(
				(index >= 0)
			&&	(index < (long)(m_end-m_begin))
			)
		{
			lResult = _Copy<VARIANT>::copy (pRetVal, m_begin+index);
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corputref_Item (long index, VARIANT pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corputref_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	return E_NOTIMPL;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_Add (VARIANT value, long* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_Add"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	return E_NOTIMPL;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_Contains (VARIANT value, VARIANT_BOOL* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_Contains"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	HRESULT		lResult = S_FALSE;
	VARIANT *	lIter;

	if	(pRetVal)
	{
		(*pRetVal) = VARIANT_FALSE;

		for	(lIter = m_begin; lIter < m_end; lIter++)
		{
			if	(
					(V_VT (&value) == V_VT (lIter))
				&&	(V_DISPATCH (&value) == V_DISPATCH (lIter))
				)
			{
				(*pRetVal) = VARIANT_TRUE;
				lResult = S_OK;
				break;
			}
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_Clear ()
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_Clear"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	return E_NOTIMPL;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corget_IsReadOnly (VARIANT_BOOL* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corget_IsReadOnly"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	if	(pRetVal)
	{
		(*pRetVal) = VARIANT_TRUE;
	}
	return S_OK;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corget_IsFixedSize (VARIANT_BOOL* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corget_IsFixedSize"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	if	(pRetVal)
	{
		(*pRetVal) = VARIANT_TRUE;
	}
	return S_OK;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_IndexOf (VARIANT value, long* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_IndexOf"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	HRESULT		lResult = S_FALSE;
	VARIANT *	lIter;

	if	(pRetVal)
	{
		(*pRetVal) = -1;

		for	(lIter = m_begin; lIter < m_end; lIter++)
		{
			(*pRetVal)++;

			if	(
					(V_VT (&value) == V_VT (lIter))
				&&	(V_DISPATCH (&value) == V_DISPATCH (lIter))
				)
			{
				break;
			}
		}
		if	(lIter < m_end)
		{
			lResult = S_OK;
		}
		else
		{
			(*pRetVal) = -1;
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_Insert (long index, VARIANT value)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_Insert"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	return E_NOTIMPL;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_Remove (VARIANT value)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_Remove"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	return E_NOTIMPL;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_RemoveAt (long index)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_RemoveAt"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::cor_CopyTo (struct mscorlib::_Array * Array, long index)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::cor_CopyTo [%p] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this), Array, index);
#endif
	return E_NOTIMPL;
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corget_Count (long* pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corget_Count [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this), (long)(m_end-m_begin));
#endif
	if	(pRetVal)
	{
		(*pRetVal) = (long)(m_end-m_begin);
		return S_OK;
	}
	else
	{
		return E_POINTER;
	}
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corget_SyncRoot (VARIANT * pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corget_SyncRoot"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	if	(pRetVal)
	{
		VariantInit (pRetVal);
		V_VT (pRetVal) = VT_ERROR;
		return S_OK;
	}
	else
	{
		return E_POINTER;
	}
}

template <class Owner>
HRESULT STDMETHODCALLTYPE CCorEnumVariant<Owner>::corget_IsSynchronized (VARIANT_BOOL * pRetVal)
{
#ifdef	_DEBUG_ENUMVARIANT
	LogMessage (_DEBUG_ENUMVARIANT, _T("[%p(%d)] [%p] %s::corget_IsSynchronized"), SafeGetOwner(), SafeGetOwnerUsed(), this, ObjTypeName(this));
#endif
	if	(pRetVal)
	{
		(*pRetVal) = VARIANT_FALSE;
		return S_OK;
	}
	else
	{
		return E_POINTER;
	}
}

/////////////////////////////////////////////////////////////////////////////
