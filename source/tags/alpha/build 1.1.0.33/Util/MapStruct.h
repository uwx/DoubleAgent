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
#ifndef	_MAPSTRUCT_H
#define	_MAPSTRUCT_H
////////////////////////////////////////////////////////////////////////

#ifndef __AFXPLEX_H__
#include <afxplex_.h>
#endif

template<typename aKeyType, class aValType, typename aKeyParm = aKeyType>
class CMapStruct : public CObject
{
protected:
	struct CAssoc
	{
		CAssoc* mNext;
		UINT mHashValue;
		aKeyType mKey;
		aValType mValue;
	};

public:
	CMapStruct(int pBlockSize = 10);
	~CMapStruct();

	int GetCount() const;
	BOOL IsEmpty () const;
	BOOL HasKey (aKeyParm pKey) const;
	BOOL Lookup (aKeyParm pKey, aValType*& pValue) const;

// Operations
	aValType & operator[] (aKeyParm pKey);
	bool RemoveKey (aKeyParm pKey);
	void RemoveAll ();

	POSITION GetStartPosition () const;
	void GetNextAssoc (POSITION & pNextPosition, aKeyType& pKey, aValType*& pValue) const;

	UINT GetHashTableSize () const;
	void InitHashTable (UINT pHashSize, bool pAllocNow = true);

// Implementation
protected:
	CAssoc**		mHashTable;
	UINT			mHashTableSize;
	int				mCount;
	CAssoc*			mFreeList;
	struct CPlex*	mBlocks;
	int				mBlockSize;

	CAssoc* NewAssoc ();
	void FreeAssoc (CAssoc*);
	CAssoc* GetAssocAt (aKeyParm, UINT&) const;

public:
#ifdef _DEBUG
	void Dump (CDumpContext&) const;
	void AssertValid () const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

template<typename aKeyType, class aValType, typename aKeyParm>
AFX_INLINE int CMapStruct<aKeyType, aValType, aKeyParm>::GetCount() const
	{return mCount;}
template<typename aKeyType, class aValType, typename aKeyParm>
AFX_INLINE BOOL CMapStruct<aKeyType, aValType, aKeyParm>::IsEmpty() const
	{return mCount == 0;}
template<typename aKeyType, class aValType, typename aKeyParm>
AFX_INLINE POSITION CMapStruct<aKeyType, aValType, aKeyParm>::GetStartPosition() const
	{return (mCount == 0) ? NULL : BEFORE_START_POSITION;}
template<typename aKeyType, class aValType, typename aKeyParm>
AFX_INLINE UINT CMapStruct<aKeyType, aValType, aKeyParm>::GetHashTableSize() const
	{return mHashTableSize;}

/////////////////////////////////////////////////////////////////////////////

template<typename aKeyType, class aValType, typename aKeyParm>
CMapStruct<aKeyType, aValType, aKeyParm>::CMapStruct(int pBlockSize)
{
	ASSERT(pBlockSize > 0);

	mHashTable = NULL;
	mHashTableSize = 17;	// default size
	mCount = 0;
	mFreeList = NULL;
	mBlocks = NULL;
	mBlockSize = pBlockSize;
}

template<typename aKeyType, class aValType, typename aKeyParm>
void CMapStruct<aKeyType, aValType, aKeyParm>::InitHashTable(UINT pHashSize, bool pAllocNow)
{
	ASSERT_VALID(this);
	ASSERT(mCount == 0);
	ASSERT(pHashSize > 0);

	if (mHashTable != NULL)
	{
		delete[] mHashTable;
		mHashTable = NULL;
	}
	if (pAllocNow)
	{
		mHashTable = new CAssoc* [pHashSize];
		memset(mHashTable, 0, sizeof(CAssoc*) * pHashSize);
	}
	mHashTableSize = pHashSize;
}

template<typename aKeyType, class aValType, typename aKeyParm>
void CMapStruct<aKeyType, aValType, aKeyParm>::RemoveAll()
{
	ASSERT_VALID(this);

	if (mHashTable != NULL)
	{
		for (UINT nHash = 0; nHash < mHashTableSize; nHash++)
		{
			CAssoc* lAssoc;
			for (lAssoc = mHashTable[nHash]; lAssoc != NULL; lAssoc = lAssoc->mNext)
			{
#if	(_MFC_VER >= 0x0800)
				lAssoc->CAssoc::~CAssoc();
#else
				DestructElements<aValType>(&lAssoc->mValue, 1);
				DestructElements<aKeyType>(&lAssoc->mKey, 1);
#endif
			}
		}
	}

	delete[] mHashTable;
	mHashTable = NULL;

	mCount = 0;
	mFreeList = NULL;
	mBlocks->FreeDataChain();
	mBlocks = NULL;
}

template<typename aKeyType, class aValType, typename aKeyParm>
CMapStruct<aKeyType, aValType, aKeyParm>::~CMapStruct()
{
	RemoveAll();
	ASSERT(mCount == 0);
}

template<typename aKeyType, class aValType, typename aKeyParm>
typename CMapStruct<aKeyType, aValType, aKeyParm>::CAssoc*
CMapStruct<aKeyType, aValType, aKeyParm>::NewAssoc()
{
	if (mFreeList == NULL)
	{
		CPlex* lNewBlock = CPlex::Create(mBlocks, mBlockSize, sizeof(CMapStruct::CAssoc));
		CMapStruct::CAssoc* lAssoc = (CMapStruct::CAssoc*) lNewBlock->data();
		lAssoc += mBlockSize - 1;
		for (int lNdx = mBlockSize-1; lNdx >= 0; lNdx--, lAssoc--)
		{
			lAssoc->mNext = mFreeList;
			mFreeList = lAssoc;
		}
	}
	ASSERT(mFreeList != NULL);

	CMapStruct::CAssoc* lAssoc = mFreeList;
	mFreeList = mFreeList->mNext;
	mCount++;
	ASSERT(mCount > 0);
#if	(_MFC_VER >= 0x0800)
	lAssoc->CAssoc::CAssoc();
#else
	ConstructElements<aKeyType>(&lAssoc->mKey, 1);
	ConstructElements<aValType>(&lAssoc->mValue, 1);
#endif
	return lAssoc;
}

template<typename aKeyType, class aValType, typename aKeyParm>
void CMapStruct<aKeyType, aValType, aKeyParm>::FreeAssoc(typename CMapStruct::CAssoc* pAssoc)
{
#if	(_MFC_VER >= 0x0800)
	pAssoc->CAssoc::~CAssoc();
#else
	DestructElements<aValType>(&pAssoc->mValue, 1);
	DestructElements<aKeyType>(&pAssoc->mKey, 1);
#endif
	pAssoc->mNext = mFreeList;
	mFreeList = pAssoc;
	mCount--;
	ASSERT(mCount >= 0);

	if (mCount == 0)
	{
		RemoveAll();
	}
}

template<typename aKeyType, class aValType, typename aKeyParm>
typename CMapStruct<aKeyType, aValType, aKeyParm>::CAssoc*
CMapStruct<aKeyType, aValType, aKeyParm>::GetAssocAt(aKeyParm pKey, UINT& pHash) const
{
	pHash = HashKey<aKeyParm>(pKey) % mHashTableSize;

	if (mHashTable == NULL)
		return NULL;

	CAssoc* lAssoc;
	for (lAssoc = mHashTable[pHash]; lAssoc != NULL; lAssoc = lAssoc->mNext)
	{
		if (CompareElements(&lAssoc->mKey, &pKey))
			return lAssoc;
	}
	return NULL;
}

template<typename aKeyType, class aValType, typename aKeyParm>
BOOL CMapStruct<aKeyType, aValType, aKeyParm>::HasKey(aKeyParm pKey) const
{
	ASSERT_VALID(this);

	UINT lHash;
	CAssoc* pAssoc = GetAssocAt(pKey, lHash);
	return (pAssoc != NULL);
}

template<typename aKeyType, class aValType, typename aKeyParm>
BOOL CMapStruct<aKeyType, aValType, aKeyParm>::Lookup(aKeyParm pKey, aValType*& pValue) const
{
	ASSERT_VALID(this);

	UINT lHash;
	CAssoc* pAssoc = GetAssocAt(pKey, lHash);
	if (pAssoc == NULL)
		return FALSE;

	pValue = &pAssoc->mValue;
	return TRUE;
}

template<typename aKeyType, class aValType, typename aKeyParm>
aValType& CMapStruct<aKeyType, aValType, aKeyParm>::operator[](aKeyParm pKey)
{
	ASSERT_VALID(this);

	UINT lHash;
	CAssoc* lAssoc;
	if ((lAssoc = GetAssocAt(pKey, lHash)) == NULL)
	{
		if (mHashTable == NULL)
			InitHashTable(mHashTableSize);

		lAssoc = NewAssoc();
		lAssoc->mHashValue = lHash;
		lAssoc->mKey = pKey;
		lAssoc->mNext = mHashTable[lHash];
		mHashTable[lHash] = lAssoc;
	}
	return lAssoc->mValue;
}

template<typename aKeyType, class aValType, typename aKeyParm>
bool CMapStruct<aKeyType, aValType, aKeyParm>::RemoveKey(aKeyParm pKey)
{
	ASSERT_VALID(this);

	if (mHashTable == NULL)
		return false;

	CAssoc** lAssocPrev;
	lAssocPrev = &mHashTable[HashKey<aKeyParm>(pKey) % mHashTableSize];

	CAssoc* lAssoc;
	for (lAssoc = *lAssocPrev; lAssoc != NULL; lAssoc = lAssoc->mNext)
	{
		if (CompareElements(&lAssoc->mKey, &pKey))
		{
			*lAssocPrev = lAssoc->mNext;
			FreeAssoc(lAssoc);
			return true;
		}
		lAssocPrev = &lAssoc->mNext;
	}
	return false;
}

template<typename aKeyType, class aValType, typename aKeyParm>
void CMapStruct<aKeyType, aValType, aKeyParm>::GetNextAssoc(POSITION& pNextPosition, aKeyType& pKey, aValType*& pValue) const
{
	ASSERT_VALID(this);
	ASSERT(mHashTable != NULL);  // never call on empty map

	CAssoc* lAssocRet = (CAssoc*)pNextPosition;
	ASSERT(lAssocRet != NULL);

	if (lAssocRet == (CAssoc*) BEFORE_START_POSITION)
	{
		for (UINT lBucket = 0; lBucket < mHashTableSize; lBucket++)
			if ((lAssocRet = mHashTable[lBucket]) != NULL)
				break;
		ASSERT(lAssocRet != NULL);
	}

	ASSERT(AfxIsValidAddress(lAssocRet, sizeof(CAssoc)));
	CAssoc* lAssocNext;
	if ((lAssocNext = lAssocRet->mNext) == NULL)
	{
		for (UINT lBucket = lAssocRet->mHashValue + 1;
		  lBucket < mHashTableSize; lBucket++)
			if ((lAssocNext = mHashTable[lBucket]) != NULL)
				break;
	}

	pNextPosition = (POSITION) lAssocNext;

	pKey = lAssocRet->mKey;
	pValue = &lAssocRet->mValue;
}

#ifdef _DEBUG
template<typename aKeyType, class aValType, typename aKeyParm>
void CMapStruct<aKeyType, aValType, aKeyParm>::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << mCount << " elements";
	if (dc.GetDepth() > 0)
	{
		aKeyType lKey;
		aValType* lValue;

		POSITION lPos = GetStartPosition();
		while (lPos != NULL)
		{
			GetNextAssoc(lPos, lKey, lValue);
			dc << "\n\t[";
			DumpElements<aKeyType>(dc, &lKey, 1);
			dc << "] = ";
			DumpElements<aValType>(dc, lValue, 1);
		}
	}

	dc << "\n";
}

template<typename aKeyType, class aValType, typename aKeyParm>
void CMapStruct<aKeyType, aValType, aKeyParm>::AssertValid() const
{
	CObject::AssertValid();
	ASSERT(mHashTableSize > 0);
	ASSERT(mCount == 0 || mHashTable != NULL);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////
#endif // _MAPSTRUCT_H
