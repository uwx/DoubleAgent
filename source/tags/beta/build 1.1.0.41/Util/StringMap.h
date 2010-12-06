/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef _STRINGMAP
#define _STRINGMAP
#ifndef	_DEBUG
#pragma auto_inline (on)
#endif
#include <functional>
#include <algorithm>
#ifndef	_DEBUG
#pragma auto_inline ()
#endif

//////////////////////////////////////////////////////////////////////

template <typename VALUE, typename ARG_VALUE = const VALUE &> class CStringMap : private CArray <VALUE, ARG_VALUE>
{
public:
	CStringMap (bool pCaseInsensitive = true, INT_PTR pGrowBy = 10);

// Attributes
	INT_PTR GetSize () const;
	INT_PTR GetUpperBound() const;
	bool IsEmpty () const;
	CStringArray const & OrderArray () const;

// Operations
	bool Lookup (LPCTSTR pKey, VALUE & pValue) const;
	INT_PTR SetAt (LPCTSTR pKey, ARG_VALUE pValue);
	INT_PTR AddSorted (LPCTSTR pKey, ARG_VALUE pValue);					// Allows duplicates
	void RemoveAt (INT_PTR pIndex);
	INT_PTR RemoveKey (LPCTSTR pKey);
	INT_PTR RemoveValue (ARG_VALUE pValue);
	void RemoveAll ();

	INT_PTR Add (LPCTSTR pKey, ARG_VALUE pValue);						// Use with extreme caution - breaks sorting
	INT_PTR InsertAt (INT_PTR pIndex, LPCTSTR pKey, ARG_VALUE pValue);	// Use with extreme caution - breaks sorting
	INT_PTR Append (const CStringMap<VALUE,ARG_VALUE>& pSource);		// Use with extreme caution - breaks sorting
	void Copy (const CStringMap<VALUE,ARG_VALUE>& pSource);

	INT_PTR FindValue (ARG_VALUE pValue) const;
	INT_PTR FindKey (LPCTSTR pKey) const;
	const CString & KeyAt (INT_PTR pIndex) const;

	VALUE & operator [] (LPCTSTR pKey);
	const VALUE & operator [] (INT_PTR pIndex) const;
	VALUE & operator [] (INT_PTR pIndex);

	const VALUE * operator () (LPCTSTR pKey) const;
	VALUE * operator () (LPCTSTR pKey);
	const VALUE * operator () (INT_PTR pIndex) const;
	VALUE * operator () (INT_PTR pIndex);

private:
	struct _CSortPredicate : public std::binary_function <CString, CString, bool>
	{
		const CString **	mFound;
		bool				mCaseInsensitive;

		_CSortPredicate (const CString ** pFound, bool pCaseInsensitive) : mFound (pFound), mCaseInsensitive (pCaseInsensitive) {}

		bool operator () (const CString & x, const CString & y) const
		{
			return ((mCaseInsensitive ? x.CollateNoCase (y) : x.Collate (y)) < 0);
		}

		bool operator () (const CString & x, LPCTSTR y) const
		{
			return ((mCaseInsensitive ? x.CollateNoCase (y) : x.Collate (y)) < 0);
		}

		bool operator () (LPCTSTR x, const CString & y) const
		{
			(*mFound) = &y;
			return ((mCaseInsensitive ? y.CollateNoCase (x) : y.Collate (x)) > 0);
		}
	};

	INT_PTR SearchKey (LPCTSTR pKey) const;
	INT_PTR AddKey (LPCTSTR pKey);

	bool			mCaseInsensitive;
	CStringArray	mOrder;

public:
	void* PASCAL operator new(size_t nSize) {return CObject::operator new (nSize);}
	void PASCAL operator delete(void* p) {CObject::operator delete (p);}
#ifdef	_DEBUG
	void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine) {return CObject::operator new (nSize, lpszFileName, nLine);}
	void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine) {CObject::operator delete (p, lpszFileName, nLine);}
#endif
};

//////////////////////////////////////////////////////////////////////

template <typename VALUE, typename ARG_VALUE>
CStringMap <VALUE, ARG_VALUE>::CStringMap (bool pCaseInsensitive, INT_PTR pGrowBy)
:	mCaseInsensitive (pCaseInsensitive)
{
	m_nGrowBy = max (pGrowBy, 1);
}

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE INT_PTR CStringMap <VALUE, ARG_VALUE>::GetSize () const
{
	return m_nSize;
}

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE INT_PTR CStringMap <VALUE, ARG_VALUE>::GetUpperBound() const
{
	return m_nSize-1;
}

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE bool CStringMap <VALUE, ARG_VALUE>::IsEmpty () const
{
	return (m_nSize == 0);
}

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE CStringArray const & CStringMap <VALUE, ARG_VALUE>::OrderArray () const
{
	return mOrder;
}

//////////////////////////////////////////////////////////////////////

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE bool CStringMap <VALUE, ARG_VALUE>::Lookup (LPCTSTR pKey, VALUE & pValue) const
{
	INT_PTR	lNdx = SearchKey (pKey);
	if	(lNdx >= 0)
	{
		pValue = const_cast <CStringMap <VALUE, ARG_VALUE> *> (this)->CArray <VALUE, ARG_VALUE>::ElementAt (lNdx);
		return true;
	}
	return false;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::SetAt (LPCTSTR pKey, ARG_VALUE pValue)
{
	ASSERT_VALID (this);
	INT_PTR	lRet = SearchKey (pKey);

	if	(lRet >= 0)
	{
		CArray <VALUE, ARG_VALUE>::ElementAt (lRet) = pValue;
	}
	else
	{
		lRet = AddKey (pKey);
		CArray <VALUE, ARG_VALUE>::InsertAt (lRet, pValue);
	}
	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::AddSorted (LPCTSTR pKey, ARG_VALUE pValue)
{
	ASSERT_VALID (this);
	INT_PTR	lRet = AddKey (pKey);
	CArray <VALUE, ARG_VALUE>::InsertAt (lRet, pValue);
	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::Add (LPCTSTR pKey, ARG_VALUE pValue)
{
	ASSERT_VALID (this);
	INT_PTR	lRet = mOrder.Add (pKey);
	CArray <VALUE, ARG_VALUE>::InsertAt (lRet, pValue);
	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::InsertAt (INT_PTR pIndex, LPCTSTR pKey, ARG_VALUE pValue)
{
	ASSERT_VALID (this);
	INT_PTR	lRet = min (max (pIndex, 0), mOrder.GetSize());
	mOrder.InsertAt (lRet, pKey);
	CArray <VALUE, ARG_VALUE>::InsertAt (lRet, pValue);
	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::Append (const CStringMap<VALUE,ARG_VALUE>& pSource)
{
	ASSERT_VALID (this);
	mOrder.Append (pSource.mOrder);
	return CArray <VALUE, ARG_VALUE>::Append (pSource);
}

template <typename VALUE, typename ARG_VALUE>
void CStringMap <VALUE, ARG_VALUE>::Copy (const CStringMap<VALUE,ARG_VALUE>& pSource)
{
	ASSERT_VALID (this);
	mOrder.Copy (pSource.mOrder);
	CArray <VALUE, ARG_VALUE>::Copy (pSource);
}

template <typename VALUE, typename ARG_VALUE>
void CStringMap <VALUE, ARG_VALUE>::RemoveAt (INT_PTR pIndex)
{
	ASSERT_VALID (this);
	CArray <VALUE, ARG_VALUE>::RemoveAt (pIndex);
	mOrder.RemoveAt (pIndex);
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::RemoveKey (LPCTSTR pKey)
{
	ASSERT_VALID (this);
	INT_PTR	lRet = SearchKey (pKey);

	if	(lRet >= 0)
	{
		CArray <VALUE, ARG_VALUE>::RemoveAt (lRet);
		mOrder.RemoveAt (lRet);
	}

	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE INT_PTR CStringMap <VALUE, ARG_VALUE>::RemoveValue (ARG_VALUE pValue)
{
	ASSERT_VALID (this);
	INT_PTR	lRet = 0;
	INT_PTR	lNdx;
	for	(lNdx = CArray <VALUE, ARG_VALUE>::GetUpperBound(); lNdx >= 0; lNdx--)
	{
		if	(CArray <VALUE, ARG_VALUE>::ElementAt (lNdx) == pValue)
		{
			CArray <VALUE, ARG_VALUE>::RemoveAt (lNdx);
			mOrder.RemoveAt (lNdx);
			lRet++;
		}
	}
	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
AFX_INLINE void CStringMap <VALUE, ARG_VALUE>::RemoveAll ()
{
	CArray <VALUE, ARG_VALUE>::RemoveAll ();
	mOrder.RemoveAll ();
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::FindValue (ARG_VALUE pValue) const
{
	ASSERT_VALID (this);
	INT_PTR	lNdx;
	for	(lNdx = CArray <VALUE, ARG_VALUE>::GetUpperBound(); lNdx >= 0; lNdx--)
	{
		if	(const_cast <CStringMap <VALUE, ARG_VALUE> *> (this)->CArray <VALUE, ARG_VALUE>::ElementAt (lNdx) == pValue)
		{
			return lNdx;
		}
	}
	return -1;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::FindKey (LPCTSTR pKey) const
{
	return SearchKey (pKey);
}

template <typename VALUE, typename ARG_VALUE>
const CString & CStringMap <VALUE, ARG_VALUE>::KeyAt (INT_PTR pIndex) const
{
	ASSERT_VALID (this);
	return const_cast <CStringArray &> (mOrder).ElementAt (pIndex);
}

template <typename VALUE, typename ARG_VALUE>
VALUE & CStringMap <VALUE, ARG_VALUE>::operator [] (LPCTSTR pKey)
{
	ASSERT_VALID (this);

	INT_PTR	lNdx = SearchKey (pKey);
	if	(lNdx < 0)
	{
		lNdx = AddKey (pKey);
		INT_PTR	lOldSize = m_nSize;
		SetSize (lOldSize+1);
		if	(lNdx < lOldSize)
		{
#if	(_MFC_VER >= 0x0800)
			(GetData()+lOldSize)->~VALUE();
#else
			DestructElements <VALUE> (GetData()+lOldSize, 1);
#endif
			memmove (GetData()+lNdx+1, GetData()+lNdx, (lOldSize-lNdx) * sizeof (VALUE));
			memset (GetData()+lNdx, 0, sizeof(VALUE));
#if	(_MFC_VER >= 0x0800)
#pragma push_macro("new")
#undef new
			::new ((void*)(GetData()+lNdx)) VALUE;
#pragma pop_macro("new")
#else
			ConstructElements <VALUE> (GetData()+lNdx, 1);
#endif
		}
	}
	return CArray <VALUE, ARG_VALUE>::ElementAt (lNdx);
}

template <typename VALUE, typename ARG_VALUE>
const VALUE & CStringMap <VALUE, ARG_VALUE>::operator [] (INT_PTR pIndex) const
{
	ASSERT_VALID (this);
	return const_cast <CStringMap <VALUE, ARG_VALUE> *> (this)->CArray <VALUE, ARG_VALUE>::ElementAt (pIndex);
}

template <typename VALUE, typename ARG_VALUE>
VALUE & CStringMap <VALUE, ARG_VALUE>::operator [] (INT_PTR pIndex)
{
	ASSERT_VALID (this);
	return CArray <VALUE, ARG_VALUE>::ElementAt (pIndex);
}

template <typename VALUE, typename ARG_VALUE>
const VALUE * CStringMap <VALUE, ARG_VALUE>::operator () (LPCTSTR pKey) const
{
	ASSERT_VALID (this);
	INT_PTR	lNdx = SearchKey (pKey);
	if	(lNdx >= 0)
	{
		return GetData() + lNdx;
	}
	return NULL;
}

template <typename VALUE, typename ARG_VALUE>
VALUE * CStringMap <VALUE, ARG_VALUE>::operator () (LPCTSTR pKey)
{
	ASSERT_VALID (this);
	INT_PTR	lNdx = SearchKey (pKey);
	if	(lNdx >= 0)
	{
		return GetData() + lNdx;
	}
	return NULL;
}

template <typename VALUE, typename ARG_VALUE>
const VALUE * CStringMap <VALUE, ARG_VALUE>::operator () (INT_PTR pIndex) const
{
	ASSERT_VALID (this);
	if	(
			(pIndex >= 0)
		&&	(pIndex < m_nSize)
		)
	{
		return GetData() + pIndex;
	}
	return NULL;
}

template <typename VALUE, typename ARG_VALUE>
VALUE * CStringMap <VALUE, ARG_VALUE>::operator () (INT_PTR pIndex)
{
	if	(
			(pIndex >= 0)
		&&	(pIndex < m_nSize)
		)
	{
		return GetData() + pIndex;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::SearchKey (LPCTSTR pKey) const
{
	INT_PTR			lRet = -1;
	const CString *	lFound = NULL;

	if	(
			(mOrder.GetSize () > 0)
		&&	(std::binary_search (mOrder.GetData(), mOrder.GetData()+mOrder.GetSize (), pKey, _CSortPredicate (&lFound, mCaseInsensitive)))
		&&	(lFound)
		)
	{
		lRet = lFound - mOrder.GetData();
	}
	return lRet;
}

template <typename VALUE, typename ARG_VALUE>
INT_PTR CStringMap <VALUE, ARG_VALUE>::AddKey (LPCTSTR pKey)
{
	if	(mOrder.GetSize () > 0)
	{
		CString		lKey (pKey);
		CString *	lInsert = std::upper_bound (mOrder.GetData(), mOrder.GetData()+mOrder.GetSize (), lKey, _CSortPredicate (NULL, mCaseInsensitive));
		INT_PTR		lRet = lInsert - mOrder.GetData();
		mOrder.InsertAt (lRet, lKey);
		return lRet;
	}
	else
	{
		mOrder.Add (pKey);
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////

typedef CStringMap <CString, LPCTSTR> CStringStringMap;

//////////////////////////////////////////////////////////////////////

#endif	// _STRINGMAP
