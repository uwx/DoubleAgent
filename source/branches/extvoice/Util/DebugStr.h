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
#ifndef	_DEBUGSTR_H_
#define _DEBUGSTR_H_
//////////////////////////////////////////////////////////////////////

static inline CString WinStyleStr (DWORD pStyle)
{
	CString	lStyleStr;

	if	(pStyle & WS_VISIBLE)
	{
		lStyleStr += _T("VISIBLE ");
	}
	if	(pStyle & WS_POPUP)
	{
		lStyleStr += _T("POPUP ");
	}
	if	(pStyle & WS_CHILD)
	{
		lStyleStr += _T("CHILD ");
	}
	if	(pStyle & WS_DISABLED)
	{
		lStyleStr += _T("DISABLED ");
	}
	if	(pStyle & WS_MINIMIZE)
	{
		lStyleStr += _T("MINIMIZE ");
	}
	if	(pStyle & WS_MAXIMIZE)
	{
		lStyleStr += _T("MAXIMIZE ");
	}
	if	(pStyle & WS_CLIPSIBLINGS)
	{
		lStyleStr += _T("CLIPSIBLINGS ");
	}
	if	(pStyle & WS_CLIPCHILDREN)
	{
		lStyleStr += _T("CLIPCHILDREN ");
	}
	if	(pStyle & WS_THICKFRAME)
	{
		lStyleStr += _T("THICKFRAME ");
	}
	else
	if	(
			((pStyle & WS_CAPTION) != WS_CAPTION)
		&&	(pStyle & WS_BORDER)
		)
	{
		lStyleStr += _T("BORDER ");
	}

	if	((pStyle & WS_CAPTION) == WS_CAPTION)
	{
		lStyleStr += _T("CAPTION ");
	}
	else
	if	(pStyle & WS_DLGFRAME)
	{
		lStyleStr += _T("DLGFRAME ");
	}

	if	(pStyle & WS_SYSMENU)
	{
		lStyleStr += _T("SYSMENU ");
	}
	if	(pStyle & WS_VSCROLL)
	{
		lStyleStr += _T("VSCROLL ");
	}
	if	(pStyle & WS_HSCROLL)
	{
		lStyleStr += _T("HSCROLL ");
	}
	if	(pStyle & WS_CHILD)
	{
		if	(pStyle & WS_TABSTOP)
		{
			lStyleStr += _T("TABSTOP ");
		}
		if	(pStyle & WS_GROUP)
		{
			lStyleStr += _T("GROUP ");
		}
	}
	else
	{
		if	(pStyle & WS_MINIMIZEBOX)
		{
			lStyleStr += _T("MINIMIZEBOX ");
		}
		if	(pStyle & WS_MAXIMIZEBOX)
		{
			lStyleStr += _T("MAXIMIZEBOX ");
		}
	}

	lStyleStr.TrimRight ();
	return lStyleStr;
}

static inline CString WinExStyleStr (DWORD pExStyle)
{
	CString	lExStyleStr;

	if	(pExStyle & WS_EX_DLGMODALFRAME)
	{
		lExStyleStr += _T("DLGMODALFRAME ");
	}
	if	(pExStyle & WS_EX_NOPARENTNOTIFY)
	{
		lExStyleStr += _T("NOPARENTNOTIFY ");
	}
	if	(pExStyle & WS_EX_TOPMOST)
	{
		lExStyleStr += _T("TOPMOST ");
	}
	if	(pExStyle & WS_EX_ACCEPTFILES)
	{
		lExStyleStr += _T("ACCEPTFILES ");
	}
	if	(pExStyle & WS_EX_TRANSPARENT)
	{
		lExStyleStr += _T("TRANSPARENT ");
	}
	if	(pExStyle & WS_EX_MDICHILD)
	{
		lExStyleStr += _T("MDICHILD ");
	}
	if	(pExStyle & WS_EX_APPWINDOW)
	{
		lExStyleStr += _T("APPWINDOW ");
	}
	if	(pExStyle & WS_EX_TOOLWINDOW)
	{
		lExStyleStr += _T("TOOLWINDOW ");
	}
#ifdef	WS_EX_LAYERED
	if	(pExStyle & WS_EX_LAYERED)
	{
		lExStyleStr += _T("LAYERED ");
	}
#endif
#ifdef	WS_EX_COMPOSITED
	if	(pExStyle & WS_EX_COMPOSITED)
	{
		lExStyleStr += _T("COMPOSITED ");
	}
#endif
	if	(pExStyle & WS_EX_NOACTIVATE)
	{
		lExStyleStr += _T("NOACTIVATE ");
	}
	if	(pExStyle & WS_EX_WINDOWEDGE)
	{
		lExStyleStr += _T("WINDOWEDGE ");
	}
	if	(pExStyle & WS_EX_CLIENTEDGE)
	{
		lExStyleStr += _T("CLIENTEDGE ");
	}
	if	(pExStyle & WS_EX_STATICEDGE)
	{
		lExStyleStr += _T("STATICEDGE ");
	}
	if	(pExStyle & WS_EX_CONTEXTHELP)
	{
		lExStyleStr += _T("CONTEXTHELP ");
	}
	if	(pExStyle & WS_EX_CONTROLPARENT)
	{
		lExStyleStr += _T("CONTROLPARENT ");
	}
	if	(pExStyle & WS_EX_RIGHT)
	{
		lExStyleStr += _T("RIGHT ");
	}
	if	(pExStyle & WS_EX_RTLREADING)
	{
		lExStyleStr += _T("RTLREADING ");
	}
	if	(pExStyle & WS_EX_LEFTSCROLLBAR)
	{
		lExStyleStr += _T("LEFTSCROLLBAR ");
	}
	if	(pExStyle & WS_EX_LAYOUTRTL)
	{
		lExStyleStr += _T("LAYOUTRTL ");
	}
	if	(pExStyle & WS_EX_NOINHERITLAYOUT)
	{
		lExStyleStr += _T("NOINHERITLAYOUT ");
	}

	lExStyleStr.TrimRight ();
	return lExStyleStr;
}

//////////////////////////////////////////////////////////////////////

static inline CString WinPosFlagsStr (UINT pFlags)
{
	CString	lFlagsStr;

	if	(pFlags & SWP_NOMOVE)
	{
		lFlagsStr += _T("NOMOVE ");
	}
	if	(pFlags & SWP_NOSIZE)
	{
		lFlagsStr += _T("NOSIZE ");
	}
	if	(pFlags & SWP_NOZORDER)
	{
		lFlagsStr += _T("NOZORDER ");
	}
	if	(pFlags & SWP_SHOWWINDOW)
	{
		lFlagsStr += _T("SHOWWINDOW ");
	}
	if	(pFlags & SWP_HIDEWINDOW)
	{
		lFlagsStr += _T("HIDEWINDOW ");
	}
	if	(pFlags & SWP_FRAMECHANGED)
	{
		lFlagsStr += _T("FRAMECHANGED ");
	}
	if	(pFlags & SWP_NOACTIVATE)
	{
		lFlagsStr += _T("NOACTIVATE ");
	}
	if	(pFlags & SWP_NOOWNERZORDER)
	{
		lFlagsStr += _T("NOOWNERZORDER ");
	}
	if	(pFlags & SWP_NOSENDCHANGING)
	{
		lFlagsStr += _T("NOSENDCHANGING ");
	}
	if	(pFlags & SWP_DEFERERASE)
	{
		lFlagsStr += _T("DEFERERASE ");
	}
	if	(pFlags & SWP_ASYNCWINDOWPOS)
	{
		lFlagsStr += _T("ASYNCWINDOWPOS ");
	}
	if	(pFlags & SWP_NOREDRAW)
	{
		lFlagsStr += _T("NOREDRAW ");
	}
	if	(pFlags & SWP_NOCOPYBITS)
	{
		lFlagsStr += _T("NOCOPYBITS ");
	}

	lFlagsStr.TrimRight ();
	return lFlagsStr;
}

static inline CString WinPosAfterStr (HWND pAfter)
{
	CString	lAfterStr;

	if	(pAfter == HWND_TOP)
	{
		lAfterStr = _T("HWND_TOP");
	}
	else
	if	(pAfter == HWND_BOTTOM)
	{
		lAfterStr = _T("HWND_BOTTOM");
	}
	else
	if	(pAfter == HWND_TOPMOST)
	{
		lAfterStr = _T("HWND_TOPMOST");
	}
	else
	if	(pAfter == HWND_NOTOPMOST)
	{
		lAfterStr = _T("HWND_NOTOPMOST");
	}
	else
	{
		lAfterStr.Format (_T("%8.8X"), pAfter);
	}

	return lAfterStr;
}

static inline CString WindowPosStr (WINDOWPOS & pWindowPos)
{
	CString	lStr = WinPosFlagsStr (pWindowPos.flags);

	if	((pWindowPos.flags & (SWP_NOMOVE|SWP_NOSIZE)) == 0)
	{
		lStr.Format (_T("%s Move/Size [%d %d %d %d (%d %d)]"), CString((LPCTSTR)lStr), pWindowPos.x, pWindowPos.y, pWindowPos.x+pWindowPos.cx, pWindowPos.y+pWindowPos.cy, pWindowPos.cx, pWindowPos.cy);
	}
	else
	if	((pWindowPos.flags & SWP_NOMOVE) == 0)
	{
		lStr.Format (_T("%s Move [%d %d]"), CString((LPCTSTR)lStr), pWindowPos.x, pWindowPos.y);
	}
	else
	if	((pWindowPos.flags & SWP_NOSIZE) == 0)
	{
		lStr.Format (_T("%s Size [%d %d]"), CString((LPCTSTR)lStr), pWindowPos.cx, pWindowPos.cy);
	}

	if	((pWindowPos.flags & SWP_NOACTIVATE) == 0)
	{
		lStr.Format (_T("%s After [%s]"), CString((LPCTSTR)lStr), WinPosAfterStr (pWindowPos.hwndInsertAfter));
	}

	return lStr;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifndef	_DEBUG_FORMAT_ALIGNED
#define	_DEBUG_FORMAT_ALIGNED	0
#endif
#ifndef	_DEBUG_FORMAT_SIGNED
#define	_DEBUG_FORMAT_SIGNED	false
#endif

//////////////////////////////////////////////////////////////////////

static inline CString FormatPoint (const POINT & pPoint, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d"), pAligned, pPoint.x, pAligned, pPoint.y);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d"), pAligned, pPoint.x, pAligned, pPoint.y);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d"), pPoint.x, pPoint.y);
		}
		else
		{
			lRet.Format (_T("%d %d"), pPoint.x, pPoint.y);
		}
	}
	return CString ((LPCTSTR)lRet);
}

inline CString FormatSize (const SIZE & pSize, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d"), pAligned, pSize.cx, pAligned, pSize.cy);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d"), pAligned, pSize.cx, pAligned, pSize.cy);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d"), pSize.cx, pSize.cy);
		}
		else
		{
			lRet.Format (_T("%d %d"), pSize.cx, pSize.cy);
		}
	}
	return CString ((LPCTSTR)lRet);
}

static inline CString FormatRect (const RECT & pRect, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d %+*.1d %+*.1d (%+*.1d %+*.1d)"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom, pAligned, pRect.right-pRect.left, pAligned, pRect.bottom-pRect.top);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d %*.1d %*.1d (%*.1d %*.1d)"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom, pAligned, pRect.right-pRect.left, pAligned, pRect.bottom-pRect.top);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d %+d %+d (%+d %+d)"), pRect.left, pRect.top, pRect.right, pRect.bottom, pRect.right-pRect.left, pRect.bottom-pRect.top);
		}
		else
		{
			lRet.Format (_T("%d %d %d %d (%d %d)"), pRect.left, pRect.top, pRect.right, pRect.bottom, pRect.right-pRect.left, pRect.bottom-pRect.top);
		}
	}
	return CString ((LPCTSTR)lRet);
}

static inline CString FormatMargin (const RECT & pRect, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d %+*.1d %+*.1d"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d %*.1d %*.1d"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d %+d %+d"), pRect.left, pRect.top, pRect.right, pRect.bottom);
		}
		else
		{
			lRet.Format (_T("%d %d %d %d"), pRect.left, pRect.top, pRect.right, pRect.bottom);
		}
	}
	return CString ((LPCTSTR)lRet);
}

//////////////////////////////////////////////////////////////////////
#endif // _DEBUGSTR_H_
