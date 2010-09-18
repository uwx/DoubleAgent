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
#ifndef _MALLOCPTR_H
#define _MALLOCPTR_H
#include <comdef.h>
////////////////////////////////////////////////////////////////////////

template <typename aType> class tMallocPtrFree
{
protected:
	void _Init (aType *& pPtr) {}
	void _Free (aType * pPtr)
	{
		if	(pPtr)
		{
			if	(mMalloc)
			{
#ifdef	_DEBUG
				if	(mMalloc->DidAlloc (pPtr) <= 0)
				{
					LogMessage (LogIfActive, _T("*** DidAlloc [%d]"), mMalloc->DidAlloc (pPtr));
				}
#endif
				mMalloc->Free (pPtr);
			}
			else
			{
#ifdef	_DEBUG
				IMallocPtr	lMalloc;
				if	(
						(CoGetMalloc (1, &lMalloc) == S_OK)
					&&	(lMalloc->DidAlloc (pPtr) <= 0)
					)
				{
					LogMessage (LogIfActive, _T("*** DidAlloc [%d]"), lMalloc->DidAlloc (pPtr));
				}
#endif
				CoTaskMemFree (pPtr);
			}
		}
	}

	IMallocPtr	mMalloc;
};

////////////////////////////////////////////////////////////////////////

template <typename aType> class tMallocPtr : public tArrayPtr <aType, tMallocPtrFree<aType> >
{
public:
	tMallocPtr (IMalloc * pMalloc = NULL) {mMalloc = pMalloc;}
	tMallocPtr (aType * pPtr, IMalloc * pMalloc = NULL) : tArrayPtr <aType, tMallocPtrFree<aType> > (pPtr) {mMalloc = pMalloc;}
	~tMallocPtr () {_Free (mPtr); mPtr = NULL;}

	aType * operator= (aType * pPtr) {return tArrayPtr <aType, tMallocPtrFree<aType> >::operator= (pPtr);}
	aType * operator= (tMallocPtr <aType> & pPtr) {return tArrayPtr <aType, tMallocPtrFree<aType> >::operator= (pPtr);}

	aType * Alloc (UINT pCount = 1)
	{
#ifdef	ASSERT
		ASSERT (mPtr == NULL);
#endif
		Free ();
		if	(mMalloc)
		{
			mPtr = (aType *) mMalloc->Alloc (sizeof (aType) * pCount);
		}
		else
		{
			mPtr = (aType *) CoTaskMemAlloc (sizeof (aType) * pCount);
		}
		if	(mPtr)
		{
			memset (mPtr, 0, sizeof (aType));
		}
		return mPtr;
	}

	void * AllocBulk (UINT pSize)
	{
		Free ();
#ifdef	ASSERT
		ASSERT (mPtr == NULL);
#endif
		if	(mMalloc)
		{
			mPtr = (aType *) mMalloc->Alloc (pSize);
		}
		else
		{
			mPtr = (aType *) CoTaskMemAlloc (pSize);
		}
		if	(mPtr)
		{
			memset (mPtr, 0, pSize);
		}
		return (void *) mPtr;
	}

	aType ** Free ()
	{
		_Free (mPtr);
		mPtr = NULL;
		return &mPtr;
	}

	IMalloc * GetMalloc () const
	{
		return mMalloc;
	}

	void SetMalloc (IMalloc * pMalloc)
	{
		mMalloc = pMalloc;
	}
};
////////////////////////////////////////////////////////////////////////
#endif
