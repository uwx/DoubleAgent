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
#ifndef	_AFXTEMPLEX_H
#define	_AFXTEMPLEX_H
////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4150)
#ifndef ALL_WARNINGS
#pragma warning (disable: 4114)
#endif
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

template <typename TYPE, typename ARG_TYPE = TYPE> class CTypeArray : public CArray <TYPE, ARG_TYPE>
{
public:
	CTypeArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1)
	{
		if	(pSize > 0)
		{
			SetSize (pSize, pGrowBy);
		}
	}

	CTypeArray (const CArray <TYPE, ARG_TYPE> & pSource)
	{
		Copy (pSource);
	}

	CTypeArray <TYPE, ARG_TYPE> & operator= (const CArray <TYPE, ARG_TYPE> & pSource)
	{
		Copy (pSource);
		return (*this);
	}

	INT_PTR Find (ARG_TYPE pElement, INT_PTR pStartAt = 0) const
	{
		for	(INT_PTR lNdx = max (pStartAt, 0); lNdx < GetCount(); lNdx++)
		{
			if	(ElementAt (lNdx) == pElement)
			{
				return lNdx;
			}
		}
		return -1;
	}

	INT_PTR AddUnique (ARG_TYPE pElement)
	{
		if	(Find (pElement) < 0)
		{
			return Add (pElement);
		}
		return -1;
	}

	INT_PTR AppendUnique (const CArray <TYPE, ARG_TYPE> & pSource)
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

	INT_PTR Remove (ARG_TYPE pElement)
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
			&&	(pNdx < GetCount())
			)
		{
			RemoveAt (pNdx);
			return true;
		}
		return false;
	}

//----------------------------------------------------------------------

	TYPE & ElementAt (INT_PTR nIndex)
	{
		ASSERT (nIndex >= 0 && nIndex < m_nSize);
		return m_pData [nIndex];
	}

	const TYPE & ElementAt (INT_PTR nIndex) const
	{
		ASSERT (nIndex >= 0 && nIndex < m_nSize);
		return m_pData [nIndex];
	}

	TYPE & operator [] (INT_PTR nIndex)
	{
		return ElementAt (nIndex);
	}

	const TYPE & operator [] (INT_PTR nIndex) const
	{
		return ElementAt (nIndex);
	}

//----------------------------------------------------------------------

	static int __cdecl DefaultCompare (const void * pElem1, const void * pElem2)
	{
		return (* (TYPE *) pElem1) > (* (TYPE *) pElem2) ? 1 : (* (TYPE *) pElem1) < (* (TYPE *) pElem2) ? -1 : 0;
	}

	INT_PTR AddSortedQS (ARG_TYPE pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL, bool pUnique = true)
	{
		TYPE	lElement = (TYPE) pElement;
		TYPE *	lArray = (TYPE *) GetData();
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
							&&	(lArray [lNdx] == lElement)
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

	INT_PTR RemoveSortedQS (ARG_TYPE pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		INT_PTR lNdx = FindSortedQS (pElement, pCompare);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}

	INT_PTR FindSortedQS (ARG_TYPE pElement, int (__cdecl * pCompare) (const void *, const void *) = NULL) const
	{
		TYPE *	lArray = (TYPE *) GetData();
		TYPE *	lFound = (TYPE *) bsearch (&pElement, lArray, GetSize (), sizeof (TYPE), (pCompare) ? pCompare : DefaultCompare);

		if	(lFound)
		{
			return lFound - lArray;
		}
		return -1;
	}

	void SortQS (int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		qsort (GetData(), GetSize (), sizeof (TYPE), (pCompare) ? pCompare : DefaultCompare);
	}
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CStructArray : public CTypeArray <TYPE, const TYPE &>
{
public:
	CStructArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CTypeArray <TYPE, const TYPE &> (pSize, pGrowBy) {}
	CStructArray (const CArray <TYPE, const TYPE &> & pSource) : CTypeArray <TYPE, const TYPE &> (pSource) {}
	CStructArray <TYPE> & operator= (const CArray <TYPE, const TYPE &> & pSource) {CTypeArray <TYPE, const TYPE &>::operator= (pSource); return *this;}
};

template <class TYPE> class CClassArray : public CTypeArray <TYPE, const TYPE &>
{
public:
	CClassArray (INT_PTR pSize = 0, INT_PTR pGrowBy = -1) : CTypeArray <TYPE, const TYPE &> (pSize, pGrowBy) {}
	CClassArray (const CArray <TYPE, const TYPE &> & pSource) : CTypeArray <TYPE, const TYPE &> (pSource) {}
	CClassArray <TYPE> & operator= (const CArray <TYPE, const TYPE &> & pSource) {CTypeArray <TYPE, const TYPE &>::operator= (pSource); return *this;}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CPtrTypeArray : public CTypedPtrArray <CPtrArray, TYPE *>
{
public:
	CPtrTypeArray () {}
	CPtrTypeArray (const CPtrTypeArray <TYPE> & pSource)
	{
		Append (pSource);
	}

	TYPE * operator() (INT_PTR pNdx) const
	{
		return (pNdx >= 0 && pNdx < GetCount()) ? GetAt (pNdx) : NULL;
	}

	TYPE * operator% (INT_PTR pNdx) const
	{
		if	(GetSize () > 0)
		{
			while (pNdx < 0) {pNdx += GetSize ();}
			return operator () (pNdx % GetSize ());
		}
		return NULL;
	}

	INT_PTR Find (const TYPE * pElement) const
	{
		for (INT_PTR lNdx = 0; lNdx < GetCount(); lNdx++)
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
		if	(GetSize () > 0)
		{
			INT_PTR			lRet = -1;
			TYPE * const *	lArray = (TYPE * const *) GetData();
			TYPE * const *	lArrayEnd = lArray + GetSize ();
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
		if	(GetSize () > 0)
		{
			INT_PTR			lRet = -1;
			TYPE * const *	lArray = (TYPE * const *) GetData();
			TYPE * const *	lArrayEnd = lArray + GetSize ();
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
		if	(GetSize () > 0)
		{
			TYPE * const *	lArray = (TYPE * const *) GetData();
			TYPE * const *	lArrayEnd = lArray + GetSize ();
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
		if	(GetSize () > 0)
		{
			TYPE * *	lArray = (TYPE * *) GetData();
			TYPE * *	lArrayEnd = lArray + GetSize ();

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
		TYPE * *	lArray = (TYPE * *) GetData();
		INT_PTR		lCount = GetSize ();

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
		TYPE * *	lArray = (TYPE * *) GetData();
		TYPE ** lFound = (TYPE * *) bsearch (&pElement, lArray, GetSize (), sizeof (TYPE *), (pCompare) ? pCompare : DefaultCompare);

		if	(lFound)
		{
			return (lFound - lArray);
		}
		return -1;
	}

	void SortQS (int (__cdecl * pCompare) (const void *, const void *) = NULL)
	{
		qsort (GetData(), GetSize (), sizeof (TYPE *), (pCompare) ? pCompare : DefaultCompare);
	}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CObTypeArray : public CTypedPtrArray <CObArray, TYPE *>
{
public:
	CObTypeArray () {}
	CObTypeArray (const CObTypeArray <TYPE> & pSource)
	{
		Append (pSource);
	}

	TYPE * operator() (INT_PTR pNdx) const
	{
		return (pNdx >= 0 && pNdx < GetCount()) ? GetAt (pNdx) : NULL;
	}

	TYPE * operator% (INT_PTR pNdx) const
	{
		if	(GetSize () > 0)
		{
			while (pNdx < 0) {pNdx += GetSize ();}
			return operator () (pNdx % GetSize ());
		}
		return NULL;
	}

	INT_PTR Find (const TYPE * pElement) const
	{
		for (INT_PTR lNdx = 0; lNdx < GetCount(); lNdx++)
		{
			if	(GetAt (lNdx) == pElement)
			{
				return lNdx;
			}
		}
		return -1;
	}

	INT_PTR Remove (TYPE * pElement)
	{
		INT_PTR lNdx = Find (pElement);
		if	(lNdx >= 0)
		{
			RemoveAt (lNdx);
		}
		return lNdx;
	}
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE> class CPtrTypeList : public CTypedPtrList <CPtrList, TYPE *>
{
public:
	CPtrTypeList (INT_PTR nBlockSize = 10) : CTypedPtrList <CPtrList, TYPE *> (nBlockSize) {}
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

template <typename TYPE> class CObTypeList : public CTypedPtrList <CObList, TYPE *>
{
public:
	CObTypeList (INT_PTR nBlockSize = 10) : CTypedPtrList <CObList, TYPE *> (nBlockSize) {}
	CObTypeList (const CObTypeList <TYPE *> & pSource)
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
			return CTypedPtrList <CObList, TYPE *>::AddHead (newElement);
		}
		return 0;
	}

	POSITION AddTailUnique (TYPE * newElement)
	{
		if	(Find (newElement) == 0)
		{
			return CTypedPtrList <CObList, TYPE *>::AddTail (newElement);
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
		for	(INT_PTR lNdx = 0; lNdx < GetCount(); lNdx++)
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
		if	(ptr != ElementAt (nIndex))
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
				(nIndex < GetCount())
			&&	(ptr != ElementAt (nIndex))
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

template <typename TYPE> class COwnObArray : public COwnArray <CObTypeArray <TYPE>, TYPE>
{
public:
	COwnObArray () {}
	COwnObArray (const COwnObArray <TYPE> & pSource) : COwnArray <CObTypeArray <TYPE>, TYPE> (pSource) {}
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

template <typename TYPE> class COwnObList : public COwnList <CObTypeList <TYPE>, TYPE>
{
public:
	COwnObList () {}
	COwnObList (const COwnObList <TYPE> & pSource) : COwnList <CObTypeList <TYPE>, TYPE> (pSource) {}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <class KEY, class ARG_KEY=KEY> class CMapString : public CMap <KEY, ARG_KEY, CString, LPCTSTR>
{
public:
	CMapString (INT_PTR nBlockSize = 10) : CMap <KEY, ARG_KEY, CString, LPCTSTR> (nBlockSize) {}

	CString operator() (ARG_KEY key) const
	{
		CString Value;
		Lookup (key, Value);
		return Value;
	}

	bool HasKey (ARG_KEY key) const
	{
		CString Value;
		return Lookup (key, Value) ? true : false;
	}
};

////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class ARG_KEY=KEY> class COwnPtrMap : public CMap <KEY, ARG_KEY, VALUE*, VALUE*>
{
public:
	COwnPtrMap (INT_PTR nBlockSize = 10) : CMap <KEY, ARG_KEY, VALUE*, VALUE*> (nBlockSize) {}
	~COwnPtrMap () {RemoveAll ();}

	VALUE* operator() (ARG_KEY key) const
	{
		VALUE* Value = NULL;
		Lookup (key, Value);
		return Value;
	}

	bool HasKey (ARG_KEY key) const
	{
		VALUE* Value = NULL;
		return Lookup (key, Value) ? true : false;
	}

	bool FindValue (VALUE* value, KEY & key) const
	{
		KEY					Key;
		VALUE*	Value;
		POSITION			Pos;

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

	VALUE* GetFirst (KEY * key = NULL) const
	{
		KEY					Key;
		VALUE*	Value = NULL;
		POSITION			Pos;

		if	(Pos = GetStartPosition())
		{
			GetNextAssoc (Pos, Key, Value);
			if	(key)
			{
				*key = Key;
			}
		}
		return Value;
	}

public:
	void SetAt (ARG_KEY key, VALUE* newValue)
	{
		if	(HasKey (key))
		{
#ifdef	_CPPUNWIND
			try
			{
#endif
				delete operator[] (key);
#ifdef	_CPPUNWIND
			}
			catch (...)
			{}
#endif
		}
		return CMap <KEY, ARG_KEY, VALUE*, VALUE*>::SetAt (key, newValue);
	}

	BOOL RemoveKey (ARG_KEY key)
	{
		VALUE* value = operator() (key);
		BOOL lRet = CMap <KEY, ARG_KEY, VALUE*, VALUE*>::RemoveKey (key);
		if (value != NULL)
		{
#ifdef	_CPPUNWIND
			try
			{
#endif
				delete value;
#ifdef	_CPPUNWIND
			}
			catch (...)
			{}
#endif
		}
		return lRet;
	}

	void RemoveAll ()
	{
#if	(_MFC_VER >= 0x0800)
		CPair *	lPair;

		for	(lPair = PGetFirstAssoc (); lPair; lPair = PGetNextAssoc (lPair))
		{
			if	(lPair->value)
			{
#ifdef	_CPPUNWIND
				try
				{
#endif
					delete lPair->value;
#ifdef	_CPPUNWIND
				}
				catch (...)
				{}
#endif
				lPair->value = NULL;
			}
		}

		CMap <KEY, ARG_KEY, VALUE*, VALUE*>::RemoveAll ();
#else	// _MFC_VER
		VALUE** Values = NULL;
		INT_PTR ValueCount = 0;

		if	(
				(m_pHashTable != NULL)
			&&	(m_nCount > 0)
			&&	(Values = new VALUE* [m_nCount])
			)
		{
			for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
			{
				CAssoc* pAssoc;
				for	(pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
				{
					if	(pAssoc->value)
					{
						Values [ValueCount++] = pAssoc->value;
						pAssoc->value = NULL;
					}
				}
			}
		}

		CMap <KEY, ARG_KEY, VALUE*, VALUE*>::RemoveAll ();

		if	(ValueCount > 0)
		{
#ifdef	_CPPUNWIND
			try
			{
#endif
				while (--ValueCount >= 0)
				{
#ifdef	_CPPUNWIND
					try
					{
#endif
						delete Values [ValueCount];
#ifdef	_CPPUNWIND
					}
					catch (...)
					{}
#endif
				}
#ifdef	_CPPUNWIND
			}
			catch (...)
			{}
#endif
		}
#endif	// _MFC_VER
	}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename aType> inline CString FormatArray (const aType * pArray, INT_PTR pCount, LPCTSTR pFormat, LPCTSTR pDelim = _T(" "))
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

template <typename aType> inline CString FormatArray (const CArray <aType, aType> & pArray, LPCTSTR pFormat, LPCTSTR pDelim = _T(" "))
{
	return FormatArray (pArray.GetData(), pArray.GetSize (), pFormat);
}

////////////////////////////////////////////////////////////////////////
#pragma warning (pop)
////////////////////////////////////////////////////////////////////////
#endif // _AFXTEMPLEX_H
