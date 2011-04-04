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
#ifndef _ITEMIDLIST_H
#define _ITEMIDLIST_H
#include <comdef.h>
#ifdef	_UNICODE
#include <shlobj.h>
#ifndef	ILCloneFull
#define	ILCloneFull			ILClone
#endif
#ifndef	PUIDLIST_RELATIVE
#define	PUIDLIST_RELATIVE	LPITEMIDLIST
#endif
#ifndef	PUIDLIST_ABSOLUTE
#define	PUIDLIST_ABSOLUTE	LPITEMIDLIST
#endif
#ifndef	PCIDLIST_ABSOLUTE
#define	PCIDLIST_ABSOLUTE	LPCITEMIDLIST
#endif
#ifndef	PCUIDLIST_RELATIVE
#define	PCUIDLIST_RELATIVE	LPCITEMIDLIST
#endif
#ifndef	PCUIDLIST_ABSOLUTE
#define	PCUIDLIST_ABSOLUTE	LPCITEMIDLIST
#endif
#endif
////////////////////////////////////////////////////////////////////////
class CItemIdList
{
public:
	CItemIdList (LPCITEMIDLIST pIdl = NULL, IMalloc * pMalloc = NULL) : mMalloc (pMalloc), mIdl (NULL) {operator= (pIdl);}
	CItemIdList (const CItemIdList& pIdList) : mMalloc (pIdList.mMalloc), mIdl (NULL) {operator= (pIdList.mIdl);}
	~CItemIdList () {Free ();}

	LPITEMIDLIST Ptr() {return mIdl;}
	LPCITEMIDLIST Ptr() const {return mIdl;}
	operator LPITEMIDLIST () {return mIdl;}
	operator LPCITEMIDLIST () const {return mIdl;}
    LPITEMIDLIST operator-> () {return (mIdl);}
    const LPCITEMIDLIST operator-> () const {return (mIdl);}
	LPCITEMIDLIST * operator& () const {return (LPCITEMIDLIST *) &mIdl;}

    operator bool () const {return (mIdl != NULL);}
    bool operator! () const {return (mIdl == NULL);}

	CItemIdList& operator= (LPCITEMIDLIST pIdl)
	{
		Free ();
		if	(pIdl)
		{
			UINT lIdlSize = IdlSize (pIdl);
			Alloc (lIdlSize);
			memcpy (mIdl, pIdl, lIdlSize);
		}
		return *this;
	}

	CItemIdList& operator= (const CItemIdList& pIdList)
	{
		return operator= (pIdList.mIdl);
	}

	CItemIdList operator+ (LPCITEMIDLIST pIdl)
	{
		if	(pIdl)
		{
			if	(mIdl)
			{
				UINT		lSize1 = IdlSize (mIdl) - sizeof (mIdl->mkid.cb);
				UINT		lSize2 = IdlSize (pIdl);
				CItemIdList	lRet (NULL, mMalloc);

				lRet.Alloc (lSize1 + lSize2 + sizeof (mIdl->mkid.cb));
				memcpy (lRet.mIdl, mIdl, lSize1);
				memcpy (((LPBYTE) lRet.mIdl) + lSize1, pIdl, lSize2);
				memset (((LPBYTE) lRet.mIdl) + lSize1 + lSize2, 0, sizeof (mIdl->mkid.cb));

				return CItemIdList (NULL, mMalloc).Attach (lRet.Detach());
			}
			else
			{
				return CItemIdList (pIdl, mMalloc);
			}
		}
		else
		{
			return CItemIdList (*this);
		}
	}

	CItemIdList& operator+= (LPCITEMIDLIST pIdl)
	{
		if	(
				(pIdl)
			&&	(mIdl)
			)
		{
			UINT		lSize1 = IdlSize (mIdl) - sizeof (mIdl->mkid.cb);
			UINT		lSize2 = IdlSize (pIdl);
			CItemIdList	lRet (NULL, mMalloc);

			lRet.Alloc (lSize1 + lSize2 + sizeof (mIdl->mkid.cb));
			memcpy (lRet.mIdl, mIdl, lSize1);
			memcpy (((LPBYTE) lRet.mIdl) + lSize1, pIdl, lSize2);
			memset (((LPBYTE) lRet.mIdl) + lSize1 + lSize2, 0, sizeof (mIdl->mkid.cb));

			Attach (lRet.Detach ());
		}
		else
		if	(pIdl)
		{
			operator= (pIdl);
		}
		return (*this);
	}

	CItemIdList operator+ (const ITEMIDLIST& pIdl)
	{
		return operator+ (&pIdl);
	}

	CItemIdList& operator+= (const ITEMIDLIST& pIdl)
	{
		return operator+= (&pIdl);
	}

	CItemIdList operator+ (const CItemIdList& pIdList)
	{
		return operator+ (pIdList.mIdl);
	}

	CItemIdList& operator+= (const CItemIdList& pIdList)
	{
		return operator+= (pIdList.mIdl);
	}

	bool operator==	(LPCITEMIDLIST pIdl) const
	{
		UINT	lIdlSize;

		if	(
				(pIdl)
			&&	(mIdl)
			&&	((lIdlSize = IdlSize (mIdl)) == IdlSize (pIdl))
			&&	(memcmp (mIdl, pIdl, lIdlSize) == 0)
			)
		{
			return true;
		}
		return false;
	}

	bool operator==	(const ITEMIDLIST& pIdl) const
	{
		return operator== ((LPCITEMIDLIST) &pIdl);
	}

	bool operator==	(const CItemIdList& pIdList) const
	{
		return operator== ((LPCITEMIDLIST) pIdList);
	}

	bool operator!=	(LPCITEMIDLIST pIdl) const
	{
		return !operator== (pIdl);
	}

	bool operator!=	(const ITEMIDLIST& pIdl) const
	{
		return !operator== (pIdl);
	}

	bool operator!=	(const CItemIdList& pIdList) const
	{
		return !operator== (pIdList);
	}

	CItemIdList& Alloc (UINT pSize)
	{
		ASSERT (mIdl == NULL);
		Free ();
		if	(mMalloc)
		{
			mIdl = (LPITEMIDLIST) mMalloc->Alloc (pSize);
		}
		else
		{
			mIdl = (LPITEMIDLIST) CoTaskMemAlloc (pSize);
		}
		memset (mIdl, 0, pSize);
		return *this;
	}

	LPITEMIDLIST * Free ()
	{
		if	(mIdl)
		{
			if	(mMalloc)
			{
				mMalloc->Free (mIdl);
			}
			else
			{
				CoTaskMemFree (mIdl);
			}
			mIdl = NULL;
		}
		return &mIdl;
	}

	CItemIdList& Attach (LPITEMIDLIST pIdl)
	{
		Free ();
		mIdl = pIdl;
		return *this;
	}

	LPITEMIDLIST Detach ()
	{
		LPITEMIDLIST lRet = mIdl;
		mIdl = NULL;
		return lRet;
	}

	CItemIdList& Empty ()
	{
		tS <ITEMIDLIST> lEmpty;
		return operator= (&lEmpty);
	}

	UINT Size () const {return IdlSize (*this);}

	CString Path () const {return GetPath (*this);}

	CItemIdList Parent () const
	{
		LPITEMIDLIST	lIdl;
#ifdef	_UNICODE
		if	(
				(mIdl)
			&&	(mIdl->mkid.cb > 0)
			&&	(lIdl = ILCloneFull (mIdl))
			)
		{
			if	(ILRemoveLastID (lIdl))
			{
				return CItemIdList (NULL, mMalloc).Attach (lIdl);
			}
			ILFree (lIdl);
		}
		return CItemIdList (NULL, mMalloc);
#else
		if	(
				(lIdl = mIdl)
			&&	(lIdl->mkid.cb > 0)
			)
		{
			UINT			lIdlSize = 0;
			LPITEMIDLIST	lLastIdl = lIdl;

			do
			{
				lLastIdl = (LPITEMIDLIST) (((LPBYTE) lIdl) + lIdl->mkid.cb);
				if	(lLastIdl->mkid.cb > 0)
				{
					lIdlSize += lIdl->mkid.cb;
					lIdl = lLastIdl;
				}
			}
			while (lLastIdl->mkid.cb > 0);
			lIdlSize += sizeof (lIdl->mkid.cb);

			if	(
					(lIdlSize > 0)
				&&	(lIdl = (LPITEMIDLIST) mMalloc->Alloc (lIdlSize))
				)
			{
				memcpy (lIdl, mIdl, lIdlSize);
				lLastIdl = (LPITEMIDLIST) (((LPBYTE) lIdl) + lIdlSize - sizeof (lIdl->mkid.cb));
				lLastIdl->mkid.cb = 0;
			}
		}
		else
		{
			lIdl = NULL;
		}
		return CItemIdList (NULL, mMalloc).Attach (lIdl);
#endif
	}

public:
	IMallocPtr		mMalloc;
protected:
	LPITEMIDLIST	mIdl;

public:
	static UINT IdlSize (LPCITEMIDLIST pIdl)
	{
		UINT	lIdlSize = 0;
#ifdef	_UNICODE
		if	(pIdl)
		{
			lIdlSize = ILGetSize (pIdl);
		}
#else
		LPCITEMIDLIST	lIdl;

		if	(lIdl = pIdl)
		{
			while (lIdl->mkid.cb)
			{
				lIdlSize += lIdl->mkid.cb;
				lIdl = (LPCITEMIDLIST) (((LPBYTE) lIdl) + lIdl->mkid.cb);
			}
			lIdlSize += sizeof (lIdl->mkid.cb);
		}
#endif
		return lIdlSize;
	}

	static CString GetPath (LPCITEMIDLIST pIdl)
	{
		if	(pIdl)
		{
			CString	lPath;
			if	(SHGetPathFromIDList (pIdl, lPath.GetBuffer (MAX_PATH)))
			{
				lPath.ReleaseBuffer ();
				return CString (lPath);
			}
		}
		return CString();
	}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

#include "Log.h"
#include "ExceptionMacros.h"

////////////////////////////////////////////////////////////////////////

static CString FormatIdList (LPCITEMIDLIST pIdList)
{
	CString		lRet;
	LPSHITEMID	lItemId;

	if	(pIdList)
	{
		try
		{
			lItemId = &const_cast<LPITEMIDLIST>(pIdList)->mkid;
			while (lItemId->cb)
			{
				LPCSTR	lIdVal = (LPCSTR)lItemId->abID;
				CString	lIdStr;
				CString	lIdChar;
				int		lNdx;

				for	(lNdx = 0; lNdx < (int)(lItemId->cb - sizeof (USHORT)); lNdx++)
				{
					if	(
							(_istalnum (lIdVal [lNdx]))
						||	(_istprint (lIdVal [lNdx]))
						)
					{
						lIdStr += _T('.');
						lIdStr += lIdVal [lNdx];
					}
					else
					{
						lIdChar.Format (_T("%2.2X"), (UINT)(USHORT)lIdVal [lNdx]);
						lIdStr += lIdChar;
					}
				}
				if	(!lRet.IsEmpty ())
				{
					lRet += "/";
				}
				lRet += lIdStr;

				lItemId = (LPSHITEMID) (((LPBYTE) lItemId) + lItemId->cb);
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

static CString IdListStr (LPCITEMIDLIST pIdList)
{
	CString		lRet;
	LPSHITEMID	lItemId;

	if	(pIdList)
	{
		try
		{
			lItemId = &const_cast<LPITEMIDLIST>(pIdList)->mkid;
			if	(lItemId->cb == 0)
			{
				lRet = _T("[<empty>]");
			}
			else
			{
				while (lItemId->cb)
				{
					LPCSTR	lIdVal = (LPCSTR)lItemId->abID;
					CString	lIdStr;
					CString	lIdChar;
					int		lNdx;

					lIdStr.Format (_T("(%hu)"), lItemId->cb);
					for	(lNdx = 0; lNdx < (int)(lItemId->cb - sizeof (USHORT)); lNdx++)
					{
						if	(_istalnum (lIdVal [lNdx]))
						{
							lIdStr += lIdVal [lNdx];
						}
						else
						{
							lIdStr += _T('.');
						}
					}
					if	(!lRet.IsEmpty ())
					{
						lRet += "•••";
					}
					lRet += _T('[');
					lRet += lIdStr;
					lRet += _T(']');

					lItemId = (LPSHITEMID) (((LPBYTE) lItemId) + lItemId->cb);
				}
			}
		}
		catch AnyExceptionSilent
	}

	return lRet;
}

static void LogIdList (UINT pLogLevel, LPCITEMIDLIST pIdl, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle (pTitle);
			CString			lIndent (pIndent);
			LPCITEMIDLIST	lIdl = pIdl;
			UINT			lIdlSize = 0;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("ItemIdList");
			}

			try
			{
				if	(pIdl)
				{
					lIdlSize = CItemIdList::IdlSize (pIdl);
				}
			}
			catch AnyExceptionSilent

			LogMessage (pLogLevel, _T("%s%s [%8.8X] [%u]"), lIndent, lTitle, pIdl, lIdlSize);

			if	(pIdl)
			{
				while (lIdl->mkid.cb)
				{
					LogMessage (pLogLevel|LogHighVolume, _T("%s  Item [%u]"), lIndent, lIdl->mkid.cb);
					LogDump (pLogLevel|LogHighVolume, (LPBYTE) (&lIdl->mkid), lIdl->mkid.cb, lIndent + _T("    "));
					lIdl = (LPCITEMIDLIST) (((LPBYTE) lIdl) + lIdl->mkid.cb);
				}
				LogMessage (pLogLevel|LogHighVolume, _T("%s  End [%8.8X] [%u]"), lIndent, lIdl, lIdl->mkid.cb);
			}

		}
		catch AnyExceptionSilent
	}
}
////////////////////////////////////////////////////////////////////////
#endif	//  _ITEMIDLIST_H
////////////////////////////////////////////////////////////////////////
#ifdef	_AFXTEMPLEX_H
#ifndef _ITEMIDLISTEX_H
#define	_ITEMIDLISTEX_H
////////////////////////////////////////////////////////////////////////

class CIdListArray : public CPtrTypeArray <ITEMIDLIST __unaligned>
{
public:
	CIdListArray () {}
	virtual ~CIdListArray () {DeleteAll ();}

	void DeleteAll ()
	{
		for	(int lNdx = 0; lNdx < GetCount(); lNdx++)
		{
			LPITEMIDLIST	lElement = ElementAt (lNdx);
			if	(lElement)
			{
				CoTaskMemFree (lElement);
			}
		}
		RemoveAll ();
	}

	void DeleteAt (int nIndex, int nCount = 1)
	{
		for	(int lNdx = nIndex + nCount - 1; lNdx >= nIndex; lNdx--)
		{
			LPITEMIDLIST	lElement = ElementAt (lNdx);
			if	(lElement)
			{
				CoTaskMemFree (lElement);
			}
		}
		RemoveAt (nIndex, nCount);
	}

	void FreeAt (int nIndex, int nCount = 1)
	{
		for	(int lNdx = nIndex + nCount - 1; lNdx >= nIndex; lNdx--)
		{
			LPITEMIDLIST	lElement = ElementAt (lNdx);
			if	(lElement)
			{
				ElementAt (lNdx) = NULL;
				CoTaskMemFree (lElement);
			}
		}
	}

	void SetAt (int nIndex, LPITEMIDLIST ptr)
	{
		if	(ptr != ElementAt (nIndex))
		{
			FreeAt (nIndex);
			CPtrTypeArray <ITEMIDLIST __unaligned>::SetAt (nIndex, ptr);
		}
	}

	void SetAtGrow (int nIndex, LPITEMIDLIST ptr)
	{
		if	(
				(nIndex < GetCount())
			&&	(ptr != ElementAt (nIndex))
			)
		{
			FreeAt (nIndex);
		}
		CPtrTypeArray <ITEMIDLIST __unaligned>::SetAtGrow (nIndex, ptr);
	}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
#ifdef	_UNICODE
////////////////////////////////////////////////////////////////////////

//	ILFindChild	on Windows XP can be unreliable
inline PUIDLIST_RELATIVE __ILFindChild (PCIDLIST_ABSOLUTE pidlParent, PCIDLIST_ABSOLUTE pidlChild)
{
	PCUIDLIST_RELATIVE	lParentNext;
	PCUIDLIST_RELATIVE	lChildNext = NULL;

	if	(
			(pidlParent)
		&&	(pidlParent->mkid.cb > 0)
		&&	(pidlChild)
		&&	(pidlChild->mkid.cb > 0)
		)
	{
		for	(lParentNext = pidlParent, lChildNext = pidlChild; lParentNext->mkid.cb > 0; lParentNext = (PCUIDLIST_RELATIVE)(((LPBYTE)lParentNext)+lParentNext->mkid.cb), lChildNext = (PCUIDLIST_RELATIVE)(((LPBYTE)lChildNext)+lChildNext->mkid.cb))
		{
			if	(
					(lChildNext->mkid.cb == 0)
				||	(lParentNext->mkid.cb != lChildNext->mkid.cb)
				||	(memcmp (&lParentNext->mkid, &lChildNext->mkid, lChildNext->mkid.cb) != 0)
				)
			{
				lChildNext = NULL;
				break;
			}
		}
	}
	return (PUIDLIST_RELATIVE)lChildNext;
}

//	ILIsParent on Windows XP can be unreliable
inline BOOL __ILIsParent (PCIDLIST_ABSOLUTE pidlParent, PCIDLIST_ABSOLUTE pidlChild, BOOL fImmediate)
{
	PUIDLIST_RELATIVE lChildNext;
	return	(
				(lChildNext = __ILFindChild (pidlParent, pidlChild))
			&&	(
					(!fImmediate)
				||	(((PCUIDLIST_RELATIVE)(((LPBYTE)lChildNext)+lChildNext->mkid.cb))->mkid.cb == 0)
				)
			);
}

////////////////////////////////////////////////////////////////////////
#endif	// _UNICODE
////////////////////////////////////////////////////////////////////////
#endif	// _ITEMIDLISTEX_H
#endif	// _AFXTEMPLEX_H
////////////////////////////////////////////////////////////////////////
