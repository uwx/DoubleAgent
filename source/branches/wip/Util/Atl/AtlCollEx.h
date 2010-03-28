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
#ifndef	_ATLCOLLEX_H_INCLUDED_
#define	_ATLCOLLEX_H_INCLUDED_
////////////////////////////////////////////////////////////////////////
#include <atlcoll.h>
////////////////////////////////////////////////////////////////////////
#ifndef	_DEBUG
#pragma auto_inline (on)
#endif
#include <functional>
#include <algorithm>
#ifndef	_DEBUG
#pragma auto_inline ()
#endif
////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4150)
////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CElementTraits <tPtr <TYPE> > : public CPrimitiveElementTraits <TYPE *>
{
public:
	typedef TYPE * INARGTYPE;
	typedef tPtr <TYPE> & OUTARGTYPE;
};

template<> class CElementTraits <IUnknownPtr> : public CDefaultElementTraits <IUnknownPtr>
{
public:
	typedef LPUNKNOWN INARGTYPE;
	typedef IUnknownPtr & OUTARGTYPE;
};

template<> class CElementTraits <IDispatchPtr> : public CDefaultElementTraits <IDispatchPtr>
{
public:
	typedef LPDISPATCH INARGTYPE;
	typedef IDispatchPtr & OUTARGTYPE;
};

template <typename P, typename I> class CComPtrElementTraits : public CDefaultElementTraits <P>
{
public:
	typedef I* INARGTYPE;
	typedef P& OUTARGTYPE;
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE, class TRAITS = CElementTraits<TYPE> > class CAtlMfcArray : public ATL::CAtlArray <TYPE, TRAITS> 
{
public:
	INT_PTR GetSize() const {return GetCount();}
	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1) {SetCount((size_t)nNewSize, (int)nGrowBy);}
	INT_PTR GetUpperBound() const {return GetCount()-1;}
	TYPE& ElementAt(INT_PTR nIndex) {return GetAt(nIndex);}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <class TYPE, class TRAITS = CElementTraits<TYPE> > class CAtlArrayEx : public CAtlMfcArray <TYPE, TRAITS>
{
public:
	CAtlArrayEx (INT_PTR pSize = 0, INT_PTR pGrowBy = -1)
	{
		if	(pSize > 0)
		{
			SetCount (pSize, (int)pGrowBy);
		}
	}

	CAtlArrayEx (const CAtlMfcArray <TYPE, TRAITS> & pSource)
	{
		Copy (pSource);
	}

	CAtlArrayEx <TYPE, TRAITS> & operator= (const CAtlMfcArray <TYPE, TRAITS> & pSource)
	{
		Copy (pSource);
		return (*this);
	}

	INT_PTR Find (INARGTYPE pElement, INT_PTR pStartAt = 0) const
	{
		TYPE	lElement = (TYPE) pElement;
		INT_PTR lNdx;

		for	(lNdx = max (pStartAt, 0); lNdx < (INT_PTR) GetCount (); lNdx++)
		{
			if	(CompareElements (GetAt (lNdx), lElement))
			{
				return lNdx;
			}
		}
		return -1;
	}

	INT_PTR AddUnique (INARGTYPE pElement)
	{
		if	(Find (pElement) < 0)
		{
			return Add (pElement);
		}
		return -1;
	}

	INT_PTR AppendUnique (const CAtlMfcArray <TYPE, TRAITS> & pSource)
	{
		INT_PTR	lRet = 0;
		INT_PTR	lNdx;
		for	(lNdx = 0; lNdx < pSource.GetCount(); lNdx++)
		{
			if	(AddUnique (pSource [lNdx]) >= 0)
			{
				lRet++;
			}
		}
		return lRet;
	}

	INT_PTR Remove (INARGTYPE pElement)
	{
		INT_PTR lRet = Find (pElement);
		if	(lRet >= 0)
		{
			RemoveAt (lRet);
		}
		return lRet;
	}

	bool RemoveNdx (INT_PTR pNdx)
	{
		if	(
				(pNdx >= 0)
			&&	(pNdx < GetCount ())
			)
		{
			RemoveAt (pNdx);
			return true;
		}
		return false;
	}

//----------------------------------------------------------------------

	static int __cdecl DefaultCompare (const void * pElem1, const void * pElem2)
	{
		return CompareElementsOrdered (*(TYPE *)pElem1, *(TYPE*)pElem2);
	}

	INT_PTR AddSortedQS (INARGTYPE pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL, bool pUnique = true)
	{
		TYPE	lElement = (TYPE) pElement;
		TYPE *	lArray = (TYPE *) GetData ();
		INT_PTR	lCount = GetSize ();

		if	(
				(lArray)
			&&	(lCount)
			)
		{
			INT_PTR lBase = 0;
			INT_PTR lOffset = lCount/2;
			INT_PTR lNdx;
			int		lCompare;

			for (lNdx = lOffset; (lOffset >= 0) && (lNdx < lCount); lNdx = lBase + lOffset)
			{
				if	(pCompare)
				{
					lCompare = (*pCompare) (&lArray [lNdx], &lElement);
				}
				else
				{
					lCompare = DefaultCompare (&lArray [lNdx], &lElement);
				}
				if	(lCompare < 0)
				{
					if	(lOffset > 0)
					{
						lOffset = (lCount-lNdx)/2;
					}
					else
					{
						lNdx++;
						break;
					}
					lBase = lNdx;
				}
				else
				{
					if	(lCompare == 0)
					{
						if	(
								(pUnique)
							&&	(CompareElements (lArray [lNdx], lElement))
							)
						{
							return -1;
						}
						break;
					}

					if	(lOffset > 0)
					{
						lOffset = min ((lOffset+1)/2, lOffset-1);
					}
					else
					{
						break;
					}
				}
			}

			InsertAt (lNdx, pElement);
			return lNdx;
		}
		else
		{
			return Add (pElement);
		}
		return -1;
	}

	INT_PTR RemoveSortedQS (INARGTYPE pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		INT_PTR lNdx = FindSortedQS (pElement, pCompare);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR FindSortedQS (INARGTYPE pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL) const
	{
		TYPE *	lArray = (TYPE *) GetData ();
		TYPE *	lFound = (TYPE *) bsearch (&pElement, lArray, GetSize (), sizeof (TYPE), (pCompare) ? pCompare : DefaultCompare);

		if	(lFound)
		{
			return lFound - lArray;
		}
		return -1;
	}

	void SortQS (int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		qsort (GetData (), GetSize (), sizeof (TYPE), (pCompare) ? pCompare : DefaultCompare);
	}
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CTypeArray : public CAtlArrayEx <TYPE, CPrimitiveElementTraits<TYPE> >
{
public:
	CTypeArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CAtlArrayEx <TYPE, CPrimitiveElementTraits<TYPE> > (pSize, pGrowBy) {}
	CTypeArray (const CAtlArrayEx <TYPE, CPrimitiveElementTraits<TYPE> > & pSource) : CAtlArrayEx <TYPE, CPrimitiveElementTraits<TYPE> > (pSource) {}
	CTypeArray <TYPE> & operator= (const CAtlArrayEx <TYPE, CPrimitiveElementTraits<TYPE> > & pSource) {CAtlArrayEx <TYPE, CPrimitiveElementTraits<TYPE> >::operator= (pSource); return *this;}
};

template <typename TYPE> class CStructArray : public CAtlArrayEx <TYPE, CElementTraits<TYPE> >
{
public:
	CStructArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CAtlArrayEx <TYPE, CElementTraits<TYPE> > (pSize, pGrowBy) {}
	CStructArray (const CAtlArrayEx <TYPE, CElementTraits<TYPE> > & pSource) : CAtlArrayEx <TYPE, CElementTraits<TYPE> > (pSource) {}
	CStructArray <TYPE> & operator= (const CAtlArrayEx <TYPE, CElementTraits<TYPE> > & pSource) {CAtlArrayEx <TYPE, CElementTraits<TYPE> >::operator= (pSource); return *this;}
};

template <class TYPE> class CClassArray : public CAtlArrayEx <TYPE, CElementTraits<TYPE> >
{
public:
	CClassArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CAtlArrayEx <TYPE, CElementTraits<TYPE> > (pSize, pGrowBy) {}
	CClassArray (const CAtlArrayEx <TYPE, CElementTraits<TYPE> > & pSource) : CAtlArrayEx <TYPE, CElementTraits<TYPE> > (pSource) {}
	CClassArray <TYPE> & operator= (const CAtlArrayEx <TYPE, CElementTraits<TYPE> > & pSource) {CAtlArrayEx <TYPE, CElementTraits<TYPE> >::operator= (pSource); return *this;}
};
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CPtrTypeArray : public CAtlMfcArray <TYPE *, CPrimitiveElementTraits <TYPE *> >
{
public:
	CPtrTypeArray () {}
	CPtrTypeArray (const CPtrTypeArray <TYPE> & pSource)
	{
		Append (pSource);
	}

	TYPE * operator() (INT_PTR pNdx) const
	{
		return (pNdx >= 0 && pNdx < (INT_PTR) GetCount ()) ? GetAt (pNdx) : NULL;
	}

	TYPE * operator% (INT_PTR pNdx) const
	{
		if	(GetCount () > 0)
		{
			while (pNdx < 0) {pNdx += (INT_PTR) GetCount ();}
			return operator () (pNdx % (INT_PTR) GetCount ());
		}
		return NULL;
	}

	INT_PTR Find (const TYPE * pElement) const
	{
		for (INT_PTR lNdx = 0; lNdx < (INT_PTR) GetCount (); lNdx++)
		{
			if	(GetAt (lNdx) == pElement)
			{
				return lNdx;
			}
		}
		return -1;
	}

	INT_PTR Remove (const TYPE * pElement)
	{
		INT_PTR lNdx = Find (pElement);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR AddUnique (TYPE * pElement)
	{
		if	(Find (pElement) < 0)
		{
			return Add (pElement);
		}
		return -1;
	}

//----------------------------------------------------------------------

	struct _CSortPredicate : public std::binary_function <TYPE *, TYPE *, bool>
	{
		_CSortPredicate (bool pStable = true) : mFound (NULL), mStable (pStable) {}
		TYPE * const ** mFound;
		bool mStable;

		bool operator () (TYPE * const x, TYPE * const y) const
		{
			return (x < y);
		}

		bool operator () (TYPE * const x, TYPE * const * const y) const
		{
			return (x < *y);
		}

		bool operator () (TYPE * const * const x, TYPE * const & y)
		{
			if (mFound) (*mFound) = &y;
			return (*x < y);
		}
	};

	template <class aSort> INT_PTR AddSorted (TYPE * pElement, aSort & pSort, bool pUnique = true)
	{
		if	(GetCount () > 0)
		{
			INT_PTR			lRet = -1;
			TYPE * const *	lArray = (TYPE * const *) GetData ();
			TYPE * const *	lArrayEnd = lArray + GetCount ();
			TYPE * const *	lInsert;

			pSort.mFound = NULL;
			lInsert = std::upper_bound (lArray, lArrayEnd, pElement, pSort);
			if	(
					(pUnique)
				&&	(lInsert < lArrayEnd)
				&&	(
						(*lInsert == pElement)
					||	(
							(lInsert > lArray)
						&&	(*(lInsert-1) == pElement)
						)
					)
				)
			{
				return lRet;
			}

			lRet = lInsert - lArray;
			InsertAt (lRet, pElement);
			return lRet;
		}
		else
		{
			return Add (pElement);
		}
	}

	inline INT_PTR AddSorted (TYPE * pElement, bool pUnique = true)
	{
		return AddSorted (pElement, _CSortPredicate (), pUnique);
	}

	template <class aSort> INT_PTR RemoveSorted (const TYPE * pElement, aSort & pSort)
	{
		if	(GetCount () > 0)
		{
			INT_PTR			lRet = -1;
			TYPE * const *	lArray = (TYPE * const *) GetData ();
			TYPE * const *	lArrayEnd = lArray + GetCount ();
			TYPE * const *	lFound = NULL;

			pSort.mFound = &lFound;

			if	(
					(std::binary_search (lArray, lArrayEnd, (TYPE * const * const) &pElement, pSort))
				&&	(lFound)
				)
			{
				while	(
							(*lFound != pElement)
						&&	(++lFound < lArrayEnd)
						&&	(!pSort ((TYPE * const) pElement, *lFound))
						)
				{}

				if	(*lFound == pElement)
				{
					lRet = lFound - lArray;
					RemoveAt (lRet);
				}
			}
			return lRet;
		}
		return -1;
	}

	inline INT_PTR RemoveSorted (const TYPE * pElement)
	{
		return RemoveSorted (pElement, _CSortPredicate ());
	}

	template <class aSort> INT_PTR FindSorted (const TYPE * pElement, aSort & pSort) const
	{
		if	(GetCount () > 0)
		{
			TYPE * const *	lArray = (TYPE * const *) GetData ();
			TYPE * const *	lArrayEnd = lArray + GetCount ();
			TYPE * const *	lFound = NULL;

			pSort.mFound = &lFound;

			if	(
					(std::binary_search (lArray, lArrayEnd, (TYPE * const * const) &pElement, pSort))
				&&	(lFound)
				)
			{
				return lFound - lArray;
			}
		}
		return -1;
	}

	inline INT_PTR FindSorted (const TYPE * pElement) const
	{
		return FindSorted (pElement, _CSortPredicate ());
	}

	template <class aSort> void Sort (aSort & pSort)
	{
		if	(GetCount () > 0)
		{
			TYPE * *	lArray = (TYPE * *) GetData ();
			TYPE * *	lArrayEnd = lArray + GetCount ();

			pSort.mFound = NULL;
			if	(pSort.mStable)
			{
				std::stable_sort (lArray, lArrayEnd, pSort);
			}
			else
			{
				std::sort (lArray, lArrayEnd, pSort);
			}
		}
	}

	inline void Sort ()
	{
		Sort (_CSortPredicate ());
	}

//----------------------------------------------------------------------

	static int __cdecl DefaultCompare (const void * pElem1, const void * pElem2)
	{
		return (* (long *) pElem1) - (* (long *) pElem2);
	}

	INT_PTR AddSortedQS (TYPE * pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL, bool pUnique = true)
	{
		TYPE * *	lArray = (TYPE * *) GetData ();
		INT_PTR		lCount = (INT_PTR) GetCount ();

		if	(
				(lArray)
			&&	(lCount)
			)
		{
			INT_PTR lBase = 0;
			INT_PTR lOffset = lCount/2;
			INT_PTR lNdx;
			int		lCompare;

			for (lNdx = lOffset; (lOffset >= 0) && (lNdx < lCount); lNdx = lBase + lOffset)
			{
				if	(pCompare)
				{
					lCompare = (*pCompare) (&lArray [lNdx], &pElement);
				}
				else
				{
					lCompare = DefaultCompare (&lArray [lNdx], &pElement);
				}
				if	(lCompare < 0)
				{
					if	(lOffset > 0)
					{
						lOffset = (lCount-lNdx)/2;
					}
					else
					{
						lNdx++;
						break;
					}
					lBase = lNdx;
				}
				else
				{
					if	(lCompare == 0)
					{
						if	(
								(pUnique)
							&&	(lArray [lNdx] == pElement)
							)
						{
							return -1;
						}
						break;
					}

					if	(lOffset > 0)
					{
						lOffset = min ((lOffset+1)/2, lOffset-1);
					}
					else
					{
						break;
					}
				}
			}

			InsertAt (lNdx, pElement);
			return lNdx;
		}
		else
		{
			return Add (pElement);
		}
		return -1;
	}

	INT_PTR RemoveSortedQS (const TYPE * pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		INT_PTR lNdx = FindSortedQS (pElement, pCompare);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR FindSortedQS (const TYPE * pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL) const
	{
		TYPE * *	lArray = (TYPE * *) GetData ();
		TYPE * *	lFound = (TYPE * *) bsearch (&pElement, lArray, GetCount (), sizeof (TYPE *), (pCompare) ? pCompare : DefaultCompare);

		if	(lFound)
		{
			return (lFound - lArray);
		}
		return -1;
	}

	void SortQS (int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		qsort (GetData (), GetCount (), sizeof (TYPE *), (pCompare) ? pCompare : DefaultCompare);
	}
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CPtrTypeList : public CAtlList <TYPE *, CPrimitiveElementTraits <TYPE *> >
{
public:
	CPtrTypeList (INT_PTR nBlockSize = 10) : CAtlList <TYPE *, CPrimitiveElementTraits <TYPE *> > (nBlockSize) {}
	CPtrTypeList (const CPtrTypeList <TYPE> & pSource)
	{
		POSITION lPos = pSource.GetHeadPosition ();
		while (lPos)
		{
			AddTail (pSource.GetNext (lPos));
		}
	}

	POSITION AddHeadUnique (TYPE * newElement)
	{
		if	(Find (newElement) == 0)
		{
			return CTypedPtrList <CPtrList, TYPE *>::AddHead (newElement);
		}
		return 0;
	}

	POSITION AddTailUnique (TYPE * newElement)
	{
		if	(Find (newElement) == 0)
		{
			return CTypedPtrList <CPtrList, TYPE *>::AddTail (newElement);
		}
		return 0;
	}

	bool Remove (TYPE * pElement)
	{
		POSITION lPos;
		if	(lPos = Find (pElement))
		{
			RemoveAt (lPos);
			return true;
		}
		return false;
	}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename BASE_CLASS, typename TYPE> class COwnArray : public BASE_CLASS
{
public:
	COwnArray () {}
	COwnArray (const COwnArray <BASE_CLASS, TYPE> & pSource) : BASE_CLASS (pSource) {}
	virtual ~COwnArray () {DeleteAll ();}

	void DeleteAll ()
	{
		for	(INT_PTR lNdx = 0; lNdx < (INT_PTR) GetCount (); lNdx++)
		{
			TYPE * lElement = GetAt (lNdx);
			BASE_CLASS::SetAt (lNdx, NULL);
			if	(lElement)
			{
				delete lElement;
			}
		}
		RemoveAll ();
	}

	void DeleteAt (INT_PTR nIndex, INT_PTR nCount = 1)
	{
		for	(INT_PTR lNdx = nIndex + nCount - 1; lNdx >= nIndex; lNdx--)
		{
			TYPE * lElement = GetAt (lNdx);
			BASE_CLASS::SetAt (lNdx, NULL);
			if	(lElement)
			{
				delete lElement;
			}
		}
		RemoveAt (nIndex, nCount);
	}

	void SetAt (INT_PTR nIndex, TYPE * ptr)
	{
		if	(ptr != GetAt (nIndex))
		{
			TYPE * lElement = GetAt (nIndex);
			BASE_CLASS::SetAt (nIndex, NULL);
			if	(lElement)
			{
				delete lElement;
			}
			BASE_CLASS::SetAt (nIndex, ptr);
		}
	}

	void SetAtGrow (INT_PTR nIndex, TYPE * ptr)
	{
		if	(
				(nIndex < (INT_PTR) GetCount ())
			&&	(ptr != GetAt (nIndex))
			)
		{
			TYPE * lElement = GetAt (nIndex);
			BASE_CLASS::SetAt (nIndex, NULL);
			if	(lElement)
			{
				delete lElement;
			}
		}
		BASE_CLASS::SetAtGrow (nIndex, ptr);
	}
};

template <typename TYPE> class COwnPtrArray : public COwnArray <CPtrTypeArray <TYPE>, TYPE>
{
public:
	COwnPtrArray () {}
	COwnPtrArray (const COwnPtrArray <TYPE> & pSource) : COwnArray <CPtrTypeArray <TYPE>, TYPE> (pSource) {}
};

////////////////////////////////////////////////////////////////////////

template <typename BASE_CLASS, typename TYPE> class COwnList : public BASE_CLASS
{
public:
	COwnList () {}
	COwnList (const COwnList <BASE_CLASS, TYPE> & pSource) : BASE_CLASS (pSource) {}
	virtual ~COwnList () {DeleteAll ();}

	void DeleteAll ()
	{
		POSITION	lPosition = GetHeadPosition ();
		TYPE *	lObject;

		while	(lPosition)
		{
			lObject = GetNext (lPosition);
			delete lObject;
		}

		RemoveAll ();
	}

	TYPE * DeleteHead ()
	{
		TYPE * lObject = RemoveHead ();
		delete lObject;
		return lObject;
	}

	TYPE * DeleteTail ()
	{
		TYPE * lObject = RemoveTail ();
		delete lObject;
		return lObject;
	}

	void DeleteAt (POSITION position)
	{
		TYPE * lObject = GetAt (position);
		RemoveAt (position);
		delete lObject;
	}

	bool Delete (TYPE * pElement)
	{
		POSITION lPos;
		if	(lPos = Find (pElement))
		{
			DeleteAt (lPos);
			return true;
		}
		return false;
	}
};

template <typename TYPE> class COwnPtrList : public COwnList <CPtrTypeList <TYPE>, TYPE>
{
public:
	COwnPtrList () {}
	COwnPtrList (const COwnPtrList <TYPE> & pSource) : COwnList <CPtrTypeList <TYPE>, TYPE> (pSource) {}
};

////////////////////////////////////////////////////////////////////////

template <typename K, typename V, class KTraits = CElementTraits <K> > class CPtrTypeMap : public CAtlMap <K, V*, KTraits, CPrimitiveElementTraits <V*> >
{
public:
	CPtrTypeMap (UINT nBins = 17)
	:	CAtlMap <K, V*, KTraits, CPrimitiveElementTraits <V*> > (nBins)
	{
	}
};

template <typename K, typename V, class KTraits = CElementTraits <K> > class COwnPtrMap : public CAtlMap <K, tPtr <V>, KTraits, CElementTraits <tPtr <V> > >
{
public:
	COwnPtrMap (UINT nBins = 17)
	:	CAtlMap <K, tPtr <V>, KTraits, CElementTraits <tPtr <V> > > (nBins)
	{
	}
};

////////////////////////////////////////////////////////////////////////
#pragma warning (pop)
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

#ifndef	__AFX_H__
typedef CAtlMfcArray <CString, CStringElementTraits <CString> >		CStringArray;
typedef CAtlMfcArray <CString, CStringElementTraitsI <CString>>		CStringArrayI;
typedef CAtlMfcArray <BYTE>											CByteArray;
#endif

typedef ATL::CComEnum <IEnumVARIANT, &__uuidof(IEnumVARIANT), VARIANT, _Copy<VARIANT> >				CEnumVARIANT;
typedef ATL::CComEnum <IEnumUnknown, &__uuidof(IEnumUnknown), LPUNKNOWN, _CopyInterface<IUnknown> >	CEnumUnknown;

////////////////////////////////////////////////////////////////////////

template <typename TYPE> inline CString FormatArray (const TYPE * pArray, INT_PTR pCount, LPCTSTR pFormat, LPCTSTR pDelim = _T(" "))
{
	CString	lRet;
	CString	lStr;
	INT_PTR	lNdx;

	for	(lNdx = 0; lNdx < pCount; lNdx++)
	{
		_stprintf (lStr.GetBuffer (100), pFormat, pArray [lNdx]);
		lStr.ReleaseBuffer ();
		lRet += lStr;
		lRet += pDelim;
	}
	lRet.TrimRight ();

	return CString (lRet);
}

template <typename TYPE> inline CString FormatArray (const CAtlArrayEx <TYPE> & pArray, LPCTSTR pFormat, LPCTSTR pDelim = _T(" "))
{
	return FormatArray (pArray.GetData (), pArray.GetSize (), pFormat);
}

////////////////////////////////////////////////////////////////////////
#endif	// _ATLCOLLEX_H_INCLUDED_
