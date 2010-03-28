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
////////////////////////////////////////////////////////////////////////

extern LPCTSTR _AtlProfileName;
extern LPCTSTR _AtlProfilePath;

#define EmptyParm _variant_t ((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
#define IsEmptyParm(v) ((V_VT (v) == VT_EMPTY) || (V_VT (v) == VT_ERROR))

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
#ifndef	__AFXMT_H__
////////////////////////////////////////////////////////////////////////

typedef	ATL::CComAutoCriticalSection	CCriticalSection;

////////////////////////////////////////////////////////////////////////
#endif	// __AFXMT_H__
////////////////////////////////////////////////////////////////////////
#ifndef	__AFXWIN_H__
////////////////////////////////////////////////////////////////////////

class CWnd : public CWindow
{
public:
	HWND GetSafeHwnd() const {return (this == NULL) ? NULL : m_hWnd;}
};

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
#endif	// __AFXWIN_H__
////////////////////////////////////////////////////////////////////////
#endif	// _ATLUTIL_H_INCLUDED_
