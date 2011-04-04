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
#ifndef	_HANDLETEMPLATES_H
#define	_HANDLETEMPLATES_H
////////////////////////////////////////////////////////////////////////

template <typename TYPE, ULONG_PTR EMPTYVALUE = 0> class tHandleType
{
public:
	inline TYPE _EmptyValue () const {return (TYPE) EMPTYVALUE;}
protected:
	inline bool _IsValid (const TYPE& pHandle) const {return (pHandle!=_EmptyValue());}
	inline void _Empty (TYPE& pHandle) {pHandle=_EmptyValue();}
	inline bool _Close (TYPE& pHandle)
	{
		bool	lRet = false;
		TYPE	lHandle = pHandle;
		try
		{
			pHandle = _EmptyValue();
			if	(lHandle != pHandle)
			{
				lRet = __Close (lHandle);
			}
		}
		catch AnyExceptionSilent
		return lRet;
	}
private:
	virtual bool __Close (TYPE& pHandle) {return ::CloseHandle (pHandle) ? true : false;}
};

////////////////////////////////////////////////////////////////////////

class tHandleTypeGlobal : public tHandleType <HGLOBAL>
{
private:
	virtual bool __Close (HGLOBAL& pHandle) {return ::GlobalFree (pHandle) ? false : true;}
};

class tHandleTypeFind : public tHandleType <HANDLE, -1>
{
private:
	virtual bool __Close (HANDLE& pHandle) {return ::FindClose (pHandle) ? true : false;}
};

class tHandleTypeMapped : public tHandleType <LPVOID, NULL>
{
private:
	virtual bool __Close (LPVOID& pHandle) {return ::UnmapViewOfFile (pHandle) ? true : false;}
};

class tHandleTypeModule : public tHandleType <HMODULE>
{
private:
	virtual bool __Close (HMODULE& pHandle) {return ::FreeLibrary (pHandle) ? true : false;}
};

#ifdef	STDMETHOD
class tHandleTypeComModule : public tHandleType <HINSTANCE>
{
private:
	virtual bool __Close (HINSTANCE& pHandle) {::CoFreeLibrary (pHandle); return true;}
};
#endif

class tHandleTypeMenu : public tHandleType <HMENU>
{
private:
	virtual bool __Close (HMENU& pHandle) {return ::DestroyMenu (pHandle) ? true : false;}
};

class tHandleTypeMemDC : public tHandleType <HDC>
{
private:
	virtual bool __Close (HDC& pHandle) {return ::DeleteDC (pHandle) ? true : false;}
};

template <typename TYPE> class tHandleTypeGdiObj : public tHandleType <TYPE>
{
private:
	virtual bool __Close (TYPE& pHandle) {return ::DeleteObject ((HGDIOBJ)pHandle) ? true : false;}
};

class tHandleTypeIcon : public tHandleType <HICON>
{
private:
	virtual bool __Close (HICON& pHandle) {return ::DestroyIcon (pHandle) ? true : false;}
};

class tHandleTypeCursor : public tHandleType <HCURSOR>
{
private:
	virtual bool __Close (HCURSOR& pHandle) {return ::DestroyCursor (pHandle) ? true : false;}
};

#ifdef	_INC_COMMCTRL
class tHandleTypeImageList : public tHandleType <HIMAGELIST>
{
private:
	virtual bool __Close (HIMAGELIST& pHandle) {return ::ImageList_Destroy (pHandle) ? true : false;}
};
#endif

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename TYPE = HANDLE, typename aHandleType = tHandleType<TYPE> > class tHandle : protected aHandleType
{
public:
	tHandle (TYPE pHandle = (TYPE)-1) {mHandle = (pHandle==(TYPE)-1) ? _EmptyValue() : pHandle;}
	virtual ~tHandle () {_Close (mHandle);}

	operator TYPE () const {return mHandle;}

	bool SafeIsValid () const {return (this==NULL)?false:_IsValid(mHandle);}
	TYPE GetSafeHandle () const {return (this==NULL)?_EmptyValue():mHandle;}

	bool Close ()
	{
		if	(SafeIsValid ())
		{
			return _Close (mHandle);
		}
		return false;
	}

	tHandle<TYPE,aHandleType>& operator= (TYPE pHandle)
	{
		if	(mHandle != pHandle)
		{
			_Close (mHandle);
			mHandle = pHandle;
		}
		return (*this);
	}

	bool operator== (TYPE pHandle) const
	{
		return (mHandle == pHandle);
	}

	bool operator!= (TYPE pHandle) const
	{
		return (mHandle != pHandle);
	}

	operator bool () const
	{
		return (_IsValid (mHandle));
	}

	bool operator! () const
	{
		return (!_IsValid (mHandle));
	}

	TYPE* operator& ()
	{
#ifdef	ASSERT
		ASSERT (!_IsValid (mHandle));
#endif
		return &mHandle;
	}

	tHandle<TYPE,aHandleType>& Attach (TYPE pHandle)
	{
#ifdef	ASSERT
		ASSERT (!_IsValid (mHandle));
#endif
		mHandle = pHandle;
		return (*this);
	}

	TYPE Detach ()
	{
		TYPE lRet = mHandle;
		_Empty (mHandle);
		return lRet;
	}

protected:
	TYPE	mHandle;
};

////////////////////////////////////////////////////////////////////////

template <typename TYPE = HANDLE, typename aHandleType = tHandleType<TYPE> > class tHandleEx : public tHandle <TYPE, aHandleType>
{
public:
	tHandleEx (TYPE pHandle = (TYPE)-1) : tHandle <TYPE,aHandleType> (pHandle) {}
	tHandleEx<TYPE,aHandleType>& operator= (TYPE pHandle) {tHandle <TYPE,aHandleType>::operator= (pHandle); return *this;}
	tHandleEx<TYPE,aHandleType>* operator & () {return this;}
	const tHandleEx<TYPE,aHandleType>* operator & () const {return this;}
	TYPE* Free () {_Close (mHandle); _Empty (mHandle); return &mHandle;}
};

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

typedef tHandle <HANDLE>									CGenericHandle;
typedef tHandleEx <HANDLE>									CGenericHandleEx;

typedef tHandle <HGLOBAL, tHandleTypeGlobal>				CGlobalHandle;
typedef tHandleEx <HGLOBAL, tHandleTypeGlobal>				CGlobalHandleEx;
typedef tHandle <HANDLE, tHandleType <HANDLE, -1> >			CFileHandle;
typedef tHandleEx <HANDLE, tHandleType <HANDLE, -1> >		CFileHandleEx;
typedef tHandle <HANDLE, tHandleTypeFind>					CFindHandle;
typedef tHandleEx <HANDLE, tHandleTypeFind>					CFindHandleEx;
typedef tHandle <LPVOID, tHandleTypeMapped>					CMappedHandle;
typedef tHandleEx <LPVOID, tHandleTypeMapped>				CMappedHandleEx;
typedef tHandle <HMODULE, tHandleTypeModule>				CModuleHandle;
typedef tHandleEx <HMODULE, tHandleTypeModule>				CModuleHandleEx;
#ifdef	STDMETHOD
typedef tHandle <HINSTANCE, tHandleTypeComModule>			CComModuleHandle;
typedef tHandleEx <HINSTANCE, tHandleTypeComModule>			CComModuleHandleEx;
#endif

typedef tHandle <HMENU, tHandleTypeMenu>					CMenuHandle;
typedef tHandleEx <HMENU, tHandleTypeMenu>					CMenuHandleEx;
typedef tHandle <HDC, tHandleTypeMemDC>						CMemDCHandle;
typedef tHandleEx <HDC, tHandleTypeMemDC>					CMemDCHandleEx;
typedef tHandle <HBRUSH, tHandleTypeGdiObj <HBRUSH> >		CBrushHandle;
typedef tHandleEx <HBRUSH, tHandleTypeGdiObj <HBRUSH> >		CBrushHandleEx;
typedef tHandle <HPEN, tHandleTypeGdiObj <HPEN> >			CPenHandle;
typedef tHandleEx <HPEN, tHandleTypeGdiObj <HPEN> >			CPenHandleEx;
typedef tHandle <HFONT, tHandleTypeGdiObj <HFONT> >			CFontHandle;
typedef tHandleEx <HFONT, tHandleTypeGdiObj <HFONT> >		CFontHandleEx;
typedef tHandle <HRGN, tHandleTypeGdiObj <HRGN> >			CRgnHandle;
typedef tHandleEx <HRGN, tHandleTypeGdiObj <HRGN> >			CRgnHandleEx;
typedef tHandle <HBITMAP, tHandleTypeGdiObj <HBITMAP> >		CBitmapHandle;
typedef tHandleEx <HBITMAP, tHandleTypeGdiObj <HBITMAP> >	CBitmapHandleEx;

typedef tHandle <HICON, tHandleTypeIcon>					CIconHandle;
typedef tHandleEx <HICON, tHandleTypeIcon>					CIconHandleEx;
typedef tHandle <HCURSOR, tHandleTypeCursor>				CCursorHandle;
typedef tHandleEx <HCURSOR, tHandleTypeCursor>				CCursorHandleEx;
#ifdef	_INC_COMMCTRL
typedef tHandle <HIMAGELIST, tHandleTypeImageList>			CImageListHandle;
typedef tHandleEx <HIMAGELIST, tHandleTypeImageList>		CImageListHandleEx;
#endif

////////////////////////////////////////////////////////////////////////
#endif	// _HANDLETEMPLATES_H
