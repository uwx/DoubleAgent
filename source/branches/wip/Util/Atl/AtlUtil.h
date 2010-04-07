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
#ifndef	_ATLUTIL_H_INCLUDED_
#define	_ATLUTIL_H_INCLUDED_
#include <atlsync.h>
////////////////////////////////////////////////////////////////////////

extern LPCTSTR _AtlProfileName;
extern LPCTSTR _AtlProfilePath;

#ifndef	IDC_STATIC
#define	IDC_STATIC (-1)
#endif
#ifndef	EmptyParm
#define EmptyParm _variant_t ((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
#endif
#ifndef	IsEmptyParm
#define IsEmptyParm(v) ((V_VT (v) == VT_EMPTY) || (V_VT (v) == VT_ERROR))
#endif

////////////////////////////////////////////////////////////////////////

#define	DECLARE_DLL_OBJECT(theClass) \
public: \
static void* __stdcall operator new(size_t nSize); \
static void __stdcall operator delete(void* p); \

#define	IMPLEMENT_DLL_OBJECT(theClass) \
void* __stdcall theClass::operator new(size_t nSize) {return ::operator new(nSize);} \
void __stdcall theClass::operator delete(void* p) {::operator delete(p);} \

////////////////////////////////////////////////////////////////////////

#define	DECLARE_PROTECT_FINAL_RELEASE(baseClass) \
bool CanFinalRelease (); \
ULONG InternalRelease () \
{ \
	if	(CanFinalRelease()) \
	{ \
		if	((m_dwRef == 0) || (m_dwRef == 1)) \
		{ \
			m_dwRef = 1; \
		} \
		return baseClass::InternalRelease (); \
	} \
	else \
	if	((m_dwRef == 0) || (m_dwRef == 1)) \
	{ \
		m_dwRef = 0; \
		return 1; \
	} \
	else \
	{ \
		return baseClass::InternalRelease (); \
	} \
} \
bool HasFinalReleased () const {return (m_dwRef == 0);}

////////////////////////////////////////////////////////////////////////

class CAutoMutex : public ATL::CMutex
{
public:
	CAutoMutex (BOOL bInitialOwner = FALSE) : ATL::CMutex (bInitialOwner) {}
	CAutoMutex (LPSECURITY_ATTRIBUTES pSecurity, BOOL bInitialOwner, LPCTSTR pszName) : ATL::CMutex (pSecurity, bInitialOwner, pszName) {}

	bool Lock (DWORD pWaitTime = INFINITE)
	{
		if	(m_h)
		{
			return (::WaitForSingleObject (m_h, pWaitTime) != WAIT_TIMEOUT);
		}
		return false;
	}
	bool Unlock ()
	{
		if	(m_h)
		{
			return (Release()!=FALSE);
		}
		return false;
	}
	bool IsAbandoned ()
	{
		if	(m_h)
		{
			return (::WaitForSingleObject (m_h, 0) == WAIT_ABANDONED);
		}
		return false;
	}
};

class CAutoEvent : public ATL::CEvent
{
public:
	CAutoEvent (BOOL bManualReset = FALSE, BOOL bInitialState = FALSE) : ATL::CEvent (bManualReset, bInitialState) {}
	CAutoEvent (LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName) : ATL::CEvent (pSecurity, bManualReset, bInitialState, pszName) {}
};

class CAutoSemaphore : public ATL::CSemaphore
{
public:
	CAutoSemaphore (LONG nInitialCount = 1, LONG nMaxCount = 1) : ATL::CSemaphore (nInitialCount, nMaxCount) {}
	CAutoSemaphore (LPSECURITY_ATTRIBUTES pSecurity, LONG nInitialCount, LONG nMaxCount, LPCTSTR pszName) : ATL::CSemaphore (pSecurity, nInitialCount, nMaxCount, pszName) {}
};

////////////////////////////////////////////////////////////////////////

template <class TYPE> class CLockObject
{
public:
	CLockObject (TYPE & pSyncObject) : mSyncObject (pSyncObject) {mSyncObject.Lock();}
	~CLockObject () {mSyncObject.Unlock();}
	TYPE & mSyncObject;
};

typedef CLockObject <CComCriticalSection>	CLockCS;
typedef CLockObject <CAutoMutex>			CLockMutex;

////////////////////////////////////////////////////////////////////////
#ifdef	__ATLWIN_H__
////////////////////////////////////////////////////////////////////////

static inline void ScreenToClient (HWND pWnd, RECT * pRect)
{
	::ScreenToClient (pWnd, (LPPOINT)&pRect->left);
	::ScreenToClient (pWnd, (LPPOINT)&pRect->right);
}

static inline void ScreenToClient (HWND pWnd, RECT & pRect)
{
	::ScreenToClient (pWnd, (LPPOINT)&pRect.left);
	::ScreenToClient (pWnd, (LPPOINT)&pRect.right);
}

static inline void ClientToScreen (HWND pWnd, RECT * pRect)
{
	::ClientToScreen (pWnd, (LPPOINT)&pRect->left);
	::ClientToScreen (pWnd, (LPPOINT)&pRect->right);
}

static inline void ClientToScreen (HWND pWnd, RECT & pRect)
{
	::ClientToScreen (pWnd, (LPPOINT)&pRect.left);
	::ClientToScreen (pWnd, (LPPOINT)&pRect.right);
}

////////////////////////////////////////////////////////////////////////

static inline void FillSolidRect (HDC pDC, LPCRECT pRect, COLORREF pColor)
{
	COLORREF lBkColor = ::GetBkColor (pDC);
	::SetBkColor (pDC, pColor);
	::ExtTextOut (pDC, 0, 0, ETO_OPAQUE, pRect, NULL, 0, NULL);
	::SetBkColor (pDC, lBkColor);
}

static inline void FillSolidRect (HDC pDC, int x, int y, int cx, int cy, COLORREF pColor)
{
	FillSolidRect (pDC, &CRect (x, y, x+cx, y+cy), pColor);
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <class T>
class CMsgPostingWnd : public CWindowImpl <CMsgPostingWnd <T> >
{
public:
	CMsgPostingWnd (T & pOwner)
	:	mOwner (pOwner)
	{
		Create (HWND_MESSAGE);
	}

	~CMsgPostingWnd ()
	{
		if	(IsWindow ())
		{
			DestroyWindow ();
		}
	}

	DECLARE_WND_CLASS(NULL)

	BEGIN_MSG_MAP(CMsgPostingWnd)
		CHAIN_MSG_MAP_MEMBER(mOwner)
	END_MSG_MAP()

public:
	T &	mOwner;
};

////////////////////////////////////////////////////////////////////////
#endif	// __ATLWIN_H__
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

#include <typeinfo.h>
template <typename TYPE> static CAtlString AtlTypeName (TYPE const * pObject, bool pTrimComTemplates = true)
{
	CAtlString	lTypeName;
	if	(pObject)
	{
		try
		{
			lTypeName = CAtlString (typeid(*pObject).name());
			if	(lTypeName.Left (5) == _T("class"))
			{
				lTypeName = lTypeName.Mid (5);
			}
			lTypeName.TrimLeft ();
			lTypeName.TrimRight ();

			if	(pTrimComTemplates)
			{
				if	(
						(lTypeName.Left (16) == _T("ATL::CComObject<"))
					&&	(lTypeName.Right (1) == _T(">"))
					)
				{
					lTypeName = lTypeName.Mid (16, lTypeName.GetLength()-17);
				}
				else
				if	(
						(lTypeName.Left (22) == _T("ATL::CComObjectNoLock<"))
					&&	(lTypeName.Right (1) == _T(">"))
					)
				{
					lTypeName = lTypeName.Mid (22, lTypeName.GetLength()-23);
				}
				if	(lTypeName.Left (5) == _T("class"))
				{
					lTypeName = lTypeName.Mid (5);
				}
				lTypeName.TrimLeft ();
				lTypeName.TrimRight ();
			}
		}
		catch (...)
		{
			try
			{
				lTypeName = CString (typeid(pObject).name());
			}
			catch (...)
			{
				lTypeName = _T("<invalid>");
			}
		}
	}
	else
	{
		lTypeName = _T("<null>");
	}
	return lTypeName;
}

////////////////////////////////////////////////////////////////////////
#endif	// _ATLUTIL_H_INCLUDED_
