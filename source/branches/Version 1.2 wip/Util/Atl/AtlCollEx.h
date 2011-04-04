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

template <typename TYPE> class __declspec(novtable) CZeroInit
{
public:
	CZeroInit (TYPE pInit = (TYPE)0) {mValue = pInit;}
	TYPE operator= (TYPE pInit) {mValue = pInit; return mValue;}
	operator TYPE () const {return mValue;}
	operator TYPE& () {return mValue;}
	operator const TYPE& () const {return mValue;}
private:
	TYPE mValue;
};

template <typename TYPE> class CElementTraits <CZeroInit <TYPE> > : public CPrimitiveElementTraits <TYPE> {};

////////////////////////////////////////////////////////////////////////

template <typename TYPE, typename aFree> class CElementTraits <tPtr <TYPE, aFree> > : public CPrimitiveElementTraits <TYPE*>
{
public:
	typedef TYPE* INARGTYPE;
	typedef tPtr <TYPE, aFree>& OUTARGTYPE;

	static void CopyElements( tPtr <TYPE, aFree>* pDest, const tPtr <TYPE, aFree>* pSrc, size_t nElements )
	{
		for( size_t iElement = 0; iElement < nElements; iElement++ )
		{
			pDest[iElement].Attach (pSrc[iElement].Detach());
		}
	}

	static void RelocateElements( tPtr <TYPE, aFree>* pDest, tPtr <TYPE, aFree>* pSrc, size_t nElements )
	{
		Checked::memmove_s( pDest, nElements*sizeof( tPtr <TYPE, aFree> ), pSrc, nElements*sizeof( tPtr <TYPE, aFree> ));
	}
};

template <typename TYPE, typename aHandleType> class CElementTraits <tHandle <TYPE, aHandleType> > : public CPrimitiveElementTraits <TYPE>
{
public:
	typedef TYPE INARGTYPE;
	typedef tHandle <TYPE, aHandleType>& OUTARGTYPE;

	static void CopyElements( tHandle <TYPE, aHandleType>* pDest, const tHandle <TYPE, aHandleType>* pSrc, size_t nElements )
	{
		for( size_t iElement = 0; iElement < nElements; iElement++ )
		{
			pDest[iElement].Attach (pSrc[iElement].Detach());
		}
	}

	static void RelocateElements( tHandle <TYPE, aHandleType>* pDest, tHandle <TYPE, aHandleType>* pSrc, size_t nElements )
	{
		Checked::memmove_s( pDest, nElements*sizeof( tHandle <TYPE, aHandleType> ), pSrc, nElements*sizeof( tHandle <TYPE, aHandleType> ));
	}
};

////////////////////////////////////////////////////////////////////////

template<> class CElementTraits <IUnknownPtr> : public CDefaultElementTraits <IUnknownPtr>
{
public:
	typedef LPUNKNOWN INARGTYPE;
	typedef IUnknownPtr& OUTARGTYPE;
};

template<> class CElementTraits <IDispatchPtr> : public CDefaultElementTraits <IDispatchPtr>
{
public:
	typedef LPDISPATCH INARGTYPE;
	typedef IDispatchPtr& OUTARGTYPE;
};

template <typename P, typename I> class CComPtrElementTraits : public CDefaultElementTraits <P>
{
public:
	typedef I* INARGTYPE;
	typedef P& OUTARGTYPE;
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE, typename PTRTYPE> class COwnPtrTraits : public CElementTraits <PTRTYPE>
{
public:
	typedef TYPE* INARGTYPE;
	typedef TYPE*& OUTARGTYPE;
};

template <typename TYPE, typename PTRTYPE> class COwnHandleTraits : public CElementTraits <PTRTYPE>
{
public:
	typedef TYPE INARGTYPE;
	typedef TYPE& OUTARGTYPE;
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <class TYPE, class TRAITS = ATL::CElementTraits<TYPE> > class CAtlArrayEx : public ATL::CAtlArray <TYPE, TRAITS>
{
public:
	CAtlArrayEx (INT_PTR pSize = 0, INT_PTR pGrowBy = -1)
	{
		if	(pSize > 0)
		{
			SetCount (pSize, (int)pGrowBy);
		}
	}

	CAtlArrayEx (const CAtlArrayEx <TYPE, TRAITS>& pSource)
	{
		Copy (pSource);
	}

	CAtlArrayEx <TYPE, TRAITS>& operator= (const CAtlArrayEx <TYPE, TRAITS>& pSource)
	{
		Copy (pSource);
		return (*this);
	}

	TYPE* operator() (INT_PTR pNdx)
	{
		return (pNdx >= 0 && pNdx < (INT_PTR) GetCount()) ? &GetAt (pNdx) : NULL;
	}

	const TYPE* operator() (INT_PTR pNdx) const
	{
		return (pNdx >= 0 && pNdx < (INT_PTR) GetCount()) ? &GetAt (pNdx) : NULL;
	}

	INT_PTR Find (INARGTYPE pElement, INT_PTR pStartAt = 0) const
	{
		TYPE	lElement = (TYPE) pElement;
		INT_PTR lNdx;

		for	(lNdx = max (pStartAt, 0); lNdx < (INT_PTR) GetCount(); lNdx++)
		{
			if	(TRAITS::CompareElements (GetAt (lNdx), lElement))
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

	INT_PTR AppendUnique (const CAtlArrayEx <TYPE, TRAITS>& pSource)
	{
		INT_PTR	lRet = 0;
		INT_PTR	lNdx;
		for	(lNdx = 0; lNdx < (INT_PTR)pSource.GetCount(); lNdx++)
		{
			if	(AddUnique ((INARGTYPE)(pSource [lNdx])) >= 0)
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
			&&	(pNdx < (INT_PTR)GetCount())
			)
		{
			RemoveAt (pNdx);
			return true;
		}
		return false;
	}

//----------------------------------------------------------------------

	static int __cdecl DefaultCompare (const void* pElem1, const void* pElem2)
	{
		return TRAITS::CompareElementsOrdered (*(TYPE*)pElem1, *(TYPE*)pElem2);
	}

	INT_PTR AddSortedQS (INARGTYPE pElement, int (__cdecl * pCompare) (const void*, const void*) = NULL, bool pUnique = true)
	{
		TYPE	lElement = (TYPE) pElement;
		TYPE*	lArray = (TYPE*) GetData();
		INT_PTR	lCount = GetCount();

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
							&&	(TRAITS::CompareElements (lArray [lNdx], lElement))
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

	INT_PTR RemoveSortedQS (INARGTYPE pElement, int (__cdecl * pCompare) (const void*, const void*) = NULL)
	{
		INT_PTR lNdx = FindSortedQS (pElement, pCompare);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR FindSortedQS (INARGTYPE pElement, int (__cdecl * pCompare) (const void*, const void*) = NULL) const
	{
		TYPE	lElement = (TYPE) pElement;
		TYPE*	lArray = (TYPE*) GetData();
		TYPE*	lFound = (TYPE*) bsearch (&lElement, lArray, GetCount(), sizeof (TYPE), (pCompare) ? pCompare : DefaultCompare);

		if	(lFound)
		{
			return lFound - lArray;
		}
		return -1;
	}

	void SortQS (int (__cdecl * pCompare) (const void*, const void*) = NULL)
	{
		qsort (GetData(), GetCount(), sizeof (TYPE), (pCompare) ? pCompare : DefaultCompare);
	}
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CAtlTypeArray : public CAtlArrayEx <TYPE, ATL::CPrimitiveElementTraits<TYPE> >
{
public:
	CAtlTypeArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CAtlArrayEx <TYPE, ATL::CPrimitiveElementTraits<TYPE> > (pSize, pGrowBy) {}
	CAtlTypeArray (const CAtlArrayEx <TYPE, ATL::CPrimitiveElementTraits<TYPE> >& pSource) : CAtlArrayEx <TYPE, ATL::CPrimitiveElementTraits<TYPE> > (pSource) {}
	CAtlTypeArray <TYPE>& operator= (const CAtlArrayEx <TYPE, ATL::CPrimitiveElementTraits<TYPE> >& pSource) {CAtlArrayEx <TYPE, ATL::CPrimitiveElementTraits<TYPE> >::operator= (pSource); return *this;}
};

template <typename TYPE> class CAtlStructArray : public CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >
{
public:
	CAtlStructArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> > (pSize, pGrowBy) {}
	CAtlStructArray (const CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >& pSource) : CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> > (pSource) {}
	CAtlStructArray <TYPE>& operator= (const CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >& pSource) {CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >::operator= (pSource); return *this;}
};

template <class TYPE> class CAtlClassArray : public CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >
{
public:
	CAtlClassArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> > (pSize, pGrowBy) {}
	CAtlClassArray (const CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >& pSource) : CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> > (pSource) {}
	CAtlClassArray <TYPE>& operator= (const CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >& pSource) {CAtlArrayEx <TYPE, ATL::CElementTraits<TYPE> >::operator= (pSource); return *this;}
};
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CAtlPtrTypeArray : public ATL::CAtlArray <TYPE*, ATL::CPrimitiveElementTraits <TYPE*> >
{
private:
	typedef ATL::CAtlArray <TYPE*, ATL::CPrimitiveElementTraits <TYPE*> > __Base;
public:
	CAtlPtrTypeArray () {}
	CAtlPtrTypeArray (const CAtlPtrTypeArray <TYPE>& pSource)
	{
		Append (pSource);
	}

	TYPE* operator() (INT_PTR pNdx) const
	{
		return (pNdx >= 0 && pNdx < (INT_PTR) GetCount()) ? GetAt (pNdx) : NULL;
	}

	TYPE* operator% (INT_PTR pNdx) const
	{
		if	(GetCount() > 0)
		{
			while (pNdx < 0) {pNdx += (INT_PTR) GetCount();}
			return operator () (pNdx % (INT_PTR) GetCount());
		}
		return NULL;
	}

	INT_PTR Find (const TYPE* pElement) const
	{
		for (INT_PTR lNdx = 0; lNdx < (INT_PTR) GetCount(); lNdx++)
		{
			if	(GetAt (lNdx) == pElement)
			{
				return lNdx;
			}
		}
		return -1;
	}

	INT_PTR Remove (const TYPE* pElement)
	{
		INT_PTR lNdx = Find (pElement);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR AddUnique (TYPE* pElement)
	{
		if	(Find (pElement) < 0)
		{
			return Add (pElement);
		}
		return -1;
	}

	void SetAtGrow (INT_PTR pIndex, TYPE* pElement)
	{
		while (pIndex > (INT_PTR)GetCount()-2)
		{
			__Base::Add (NULL);
		}
		__Base::SetAtGrow (pIndex, pElement);
	}

//----------------------------------------------------------------------

	struct _CSortPredicate : public std::binary_function <TYPE*, TYPE*, bool>
	{
		_CSortPredicate (bool pStable = true) : mFound (NULL), mStable (pStable) {}
		TYPE*const** mFound;
		bool mStable;

		bool operator () (TYPE* const x, TYPE* const y) const
		{
			return (x < y);
		}

		bool operator () (TYPE* const x, TYPE*const* const y) const
		{
			return (x < *y);
		}

		bool operator () (TYPE*const* const x, TYPE* const & y)
		{
			if (mFound) (*mFound) = &y;
			return (*x < y);
		}
	};

	template <class aSort> INT_PTR AddSorted (TYPE* pElement, aSort& pSort, bool pUnique = true)
	{
		if	(GetCount() > 0)
		{
			INT_PTR			lRet = -1;
			TYPE*const *	lArray = (TYPE*const*) GetData();
			TYPE*const*		lArrayEnd = lArray + GetCount();
			TYPE*const*		lInsert;

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

	inline INT_PTR AddSorted (TYPE* pElement, bool pUnique = true)
	{
		return AddSorted (pElement, _CSortPredicate (), pUnique);
	}

	template <class aSort> INT_PTR RemoveSorted (const TYPE* pElement, aSort& pSort)
	{
		if	(GetCount() > 0)
		{
			INT_PTR			lRet = -1;
			TYPE*const*	lArray = (TYPE*const*) GetData();
			TYPE*const*	lArrayEnd = lArray + GetCount();
			TYPE*const*	lFound = NULL;

			pSort.mFound = &lFound;

			if	(
					(std::binary_search (lArray, lArrayEnd, (TYPE*const* const) &pElement, pSort))
				&&	(lFound)
				)
			{
				while	(
							(*lFound != pElement)
						&&	(++lFound < lArrayEnd)
						&&	(!pSort ((TYPE* const) pElement, *lFound))
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

	inline INT_PTR RemoveSorted (const TYPE* pElement)
	{
		return RemoveSorted (pElement, _CSortPredicate ());
	}

	template <class aSort> INT_PTR FindSorted (const TYPE* pElement, aSort& pSort) const
	{
		if	(GetCount() > 0)
		{
			TYPE*const*	lArray = (TYPE*const*) GetData();
			TYPE*const*	lArrayEnd = lArray + GetCount();
			TYPE*const*	lFound = NULL;

			pSort.mFound = &lFound;

			if	(
					(std::binary_search (lArray, lArrayEnd, (TYPE*const* const) &pElement, pSort))
				&&	(lFound)
				)
			{
				return lFound - lArray;
			}
		}
		return -1;
	}

	inline INT_PTR FindSorted (const TYPE* pElement) const
	{
		return FindSorted (pElement, _CSortPredicate ());
	}

	template <class aSort> void Sort (aSort& pSort)
	{
		if	(GetCount() > 0)
		{
			TYPE**	lArray = (TYPE**) GetData();
			TYPE**	lArrayEnd = lArray + GetCount();

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

	static int __cdecl DefaultCompare (const void* pElem1, const void* pElem2)
	{
		return (*(long*) pElem1) - (*(long*) pElem2);
	}

	INT_PTR AddSortedQS (TYPE* pElement, int (__cdecl * pCompare) (const void*, const void*) = NULL, bool pUnique = true)
	{
		TYPE**	lArray = (TYPE**) GetData();
		INT_PTR	lCount = (INT_PTR) GetCount();

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

	INT_PTR RemoveSortedQS (const TYPE* pElement, int (__cdecl * pCompare) (const void*, const void*) = NULL)
	{
		INT_PTR lNdx = FindSortedQS (pElement, pCompare);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR FindSortedQS (const TYPE* pElement, int (__cdecl * pCompare) (const void*, const void*) = NULL) const
	{
		TYPE**	lArray = (TYPE**) GetData();
		TYPE**	lFound = (TYPE**) bsearch (&pElement, lArray, GetCount(), sizeof (TYPE*), (pCompare) ? pCompare : DefaultCompare);

		if	(lFound)
		{
			return (lFound - lArray);
		}
		return -1;
	}

	void SortQS (int (__cdecl * pCompare) (const void*, const void*) = NULL)
	{
		qsort (GetData(), GetCount(), sizeof (TYPE*), (pCompare) ? pCompare : DefaultCompare);
	}
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CAtlPtrTypeList : public CAtlList <TYPE*, ATL::CPrimitiveElementTraits <TYPE*> >
{
private:
	typedef CAtlList <TYPE*, ATL::CPrimitiveElementTraits <TYPE*> > __Base;
public:
	CAtlPtrTypeList (UINT nBlockSize = 10) : CAtlList <TYPE*, ATL::CPrimitiveElementTraits <TYPE*> > (nBlockSize) {}
	CAtlPtrTypeList (const CAtlPtrTypeList <TYPE>& pSource)
	{
		POSITION lPos = pSource.GetHeadPosition ();
		while (lPos)
		{
			AddTail (pSource.GetNext (lPos));
		}
	}

	POSITION AddHeadUnique (TYPE* newElement)
	{
		if	(Find (newElement) == 0)
		{
			return CTypedPtrList <CPtrList, TYPE*>::AddHead (newElement);
		}
		return 0;
	}

	POSITION AddTailUnique (TYPE* newElement)
	{
		if	(Find (newElement) == 0)
		{
			return CTypedPtrList <CPtrList, TYPE*>::AddTail (newElement);
		}
		return 0;
	}

	bool Remove (TYPE* pElement)
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

template <typename BASE_CLASS, typename TYPE> class CAtlOwnArray : public BASE_CLASS
{
public:
	CAtlOwnArray () {}
	CAtlOwnArray (const CAtlOwnArray <BASE_CLASS, TYPE>& pSource) : BASE_CLASS (pSource) {}
	virtual ~CAtlOwnArray () {DeleteAll ();}

	void DeleteAll ()
	{
		for	(INT_PTR lNdx = 0; lNdx < (INT_PTR) GetCount(); lNdx++)
		{
			TYPE* lElement = GetAt (lNdx);
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
			TYPE* lElement = GetAt (lNdx);
			BASE_CLASS::SetAt (lNdx, NULL);
			if	(lElement)
			{
				delete lElement;
			}
		}
		RemoveAt (nIndex, nCount);
	}

	void SetAt (INT_PTR nIndex, TYPE* ptr)
	{
		if	(ptr != GetAt (nIndex))
		{
			TYPE* lElement = GetAt (nIndex);
			BASE_CLASS::SetAt (nIndex, NULL);
			if	(lElement)
			{
				delete lElement;
			}
			BASE_CLASS::SetAt (nIndex, ptr);
		}
	}

	void SetAtGrow (INT_PTR nIndex, TYPE* ptr)
	{
		if	(
				(nIndex < (INT_PTR) GetCount())
			&&	(ptr != GetAt (nIndex))
			)
		{
			TYPE* lElement = GetAt (nIndex);
			BASE_CLASS::SetAt (nIndex, NULL);
			if	(lElement)
			{
				delete lElement;
			}
		}
		BASE_CLASS::SetAtGrow (nIndex, ptr);
	}
};

template <typename TYPE> class CAtlOwnPtrArray : public CAtlOwnArray <CAtlPtrTypeArray <TYPE>, TYPE>
{
public:
	CAtlOwnPtrArray () {}
	CAtlOwnPtrArray (const CAtlOwnPtrArray <TYPE>& pSource) : CAtlOwnArray <CAtlPtrTypeArray <TYPE>, TYPE> (pSource) {}
};

////////////////////////////////////////////////////////////////////////

template <typename BASE_CLASS, typename TYPE> class CAtlOwnList : public BASE_CLASS
{
public:
	CAtlOwnList () {}
	CAtlOwnList (const CAtlOwnList <BASE_CLASS, TYPE>& pSource) : BASE_CLASS (pSource) {}
	virtual ~CAtlOwnList () {DeleteAll ();}

	void DeleteAll ()
	{
		POSITION	lPosition = GetHeadPosition ();
		TYPE*		lObject;

		while	(lPosition)
		{
			lObject = GetNext (lPosition);
			delete lObject;
		}

		RemoveAll ();
	}

	TYPE* DeleteHead ()
	{
		TYPE* lObject = RemoveHead ();
		delete lObject;
		return lObject;
	}

	TYPE* DeleteTail ()
	{
		TYPE* lObject = RemoveTail ();
		delete lObject;
		return lObject;
	}

	void DeleteAt (POSITION position)
	{
		TYPE* lObject = GetAt (position);
		RemoveAt (position);
		delete lObject;
	}

	bool Delete (TYPE* pElement)
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

template <typename TYPE> class CAtlOwnPtrList : public CAtlOwnList <CAtlPtrTypeList <TYPE>, TYPE>
{
public:
	CAtlOwnPtrList () {}
	CAtlOwnPtrList (const CAtlOwnPtrList <TYPE>& pSource) : CAtlOwnList <CAtlPtrTypeList <TYPE>, TYPE> (pSource) {}
};

////////////////////////////////////////////////////////////////////////

template <typename K, typename V, class KTraits = ATL::CElementTraits <K> > class CAtlPtrTypeMap : public CAtlMap <K, V*, KTraits, ATL::CPrimitiveElementTraits <V*> >
{
public:
	CAtlPtrTypeMap (UINT nBins = 17) : CAtlMap <K, V*, KTraits, ATL::CPrimitiveElementTraits <V*> > (nBins) {}

	VINARGTYPE operator() (KINARGTYPE key) const
	{
		VINARGTYPE Value = NULL;
		Lookup (key, Value);
		return Value;
	}

	bool FindValue (VINARGTYPE value, KOUTARGTYPE key) const
	{
		K			Key;
		VINARGTYPE	Value;
		POSITION	Pos;

		for	(Pos = GetStartPosition(); Pos;)
		{
			GetNextAssoc (Pos, Key, Value);
			if	(Value == value)
			{
				key = Key;
				return true;
			}
		}
		return false;
	}

	VINARGTYPE GetFirst (KOUTARGTYPE key) const
	{
		K			Key;
		VINARGTYPE	Value = NULL;
		POSITION	Pos;

		if	(Pos = GetStartPosition())
		{
			GetNextAssoc (Pos, Key, Value);
			key = Key;
		}
		return Value;
	}
};

template <typename K, typename V, class KTraits = ATL::CElementTraits <K> > class CAtlOwnPtrMap : public CAtlMap <K, tPtr <V>, KTraits, COwnPtrTraits <V, tPtr <V> > >
{
public:
	CAtlOwnPtrMap (UINT nBins = 17) : CAtlMap <K, tPtr <V>, KTraits, COwnPtrTraits <V, tPtr <V> > > (nBins) {}

	VINARGTYPE operator() (KINARGTYPE key) const
	{
		VINARGTYPE Value = NULL;
		Lookup (key, Value);
		return Value;
	}

	bool FindValue (VINARGTYPE value, KOUTARGTYPE key) const
	{
		K			Key;
		VINARGTYPE	Value;
		POSITION	Pos;

		for	(Pos = GetStartPosition(); Pos;)
		{
			GetNextAssoc (Pos, Key, Value);
			if	(Value == value)
			{
				key = Key;
				return true;
			}
		}
		return false;
	}

	VINARGTYPE GetFirst (KOUTARGTYPE key) const
	{
		K			Key;
		VINARGTYPE	Value = NULL;
		POSITION	Pos;

		if	(Pos = GetStartPosition())
		{
			GetNextAssoc (Pos, Key, Value);
			key = Key;
		}
		return Value;
	}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

typedef ATL::CComEnum <IEnumVARIANT, &__uuidof(IEnumVARIANT), VARIANT, _Copy<VARIANT> >					CEnumVARIANT;
typedef ATL::CComEnumImpl <IEnumVARIANT, &__uuidof(IEnumVARIANT), VARIANT, _Copy<VARIANT> >				CEnumVARIANTImpl;

typedef ATL::CComEnum <IEnumUnknown, &__uuidof(IEnumUnknown), LPUNKNOWN, _CopyInterface<IUnknown> >		CEnumUnknown;
typedef ATL::CComEnumImpl <IEnumUnknown, &__uuidof(IEnumUnknown), LPUNKNOWN, _CopyInterface<IUnknown> >	CEnumUnknownImpl;

typedef CAtlArrayEx <CAtlString, ATL::CStringElementTraits <CAtlString> >	CAtlStringArray;
typedef CAtlArrayEx <CAtlString, ATL::CStringElementTraitsI <CAtlString> >	CAtlStringArrayI;
typedef	CAtlArrayEx <BYTE>													CAtlByteArray;

#ifndef	__AFX_H__
#define	CStringArray	CAtlStringArray
#define	CByteArray		CAtlByteArray
#define	CTypeArray		CAtlTypeArray
#define	CStructArray	CAtlStructArray
#define	CClassArray		CAtlClassArray
#define	CPtrTypeArray	CAtlPtrTypeArray
#define	CPtrTypeList	CAtlPtrTypeList
#define	COwnPtrArray	CAtlOwnPtrArray
#define	COwnPtrList		CAtlOwnPtrList
#endif

////////////////////////////////////////////////////////////////////////
#pragma warning (pop)
////////////////////////////////////////////////////////////////////////

#ifndef	__AFXTEMPL_H__
template <typename TYPE> inline CAtlString FormatArray (const TYPE* pArray, INT_PTR pCount, LPCTSTR pFormat, LPCTSTR pDelim = _T(" "))
{
	CAtlString	lRet;
	CAtlString	lStr;
	INT_PTR		lNdx;

	for	(lNdx = 0; lNdx < pCount; lNdx++)
	{
		_stprintf (lStr.GetBuffer (100), pFormat, pArray [lNdx]);
		lStr.ReleaseBuffer ();
		lRet += lStr;
		lRet += pDelim;
	}
	lRet.TrimRight ();

	return CAtlString (lRet);
}
#endif

template <class TYPE, class TRAITS> inline CAtlString FormatArray (const CAtlArrayEx <TYPE, TRAITS>& pArray, LPCTSTR pFormat, LPCTSTR pDelim = _T(" "))
{
	return FormatArray (pArray.GetData(), pArray.GetCount(), pFormat);
}

template <class TYPE> inline CAtlString FormatArray (const CAtlPtrTypeArray <TYPE>& pArray, LPCTSTR pFormat = _T("%p"), LPCTSTR pDelim = _T(" "))
{
	return FormatArray (pArray.GetData(), pArray.GetCount(), pFormat);
}

////////////////////////////////////////////////////////////////////////
#endif	// _ATLCOLLEX_H_INCLUDED_
