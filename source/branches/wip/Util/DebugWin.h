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
#ifndef	_DEBUGWIN_H_
#define _DEBUGWIN_H_
#include <atltypes.h>
#include "DebugStr.h"

//////////////////////////////////////////////////////////////////////

inline void LogWindowId (UINT pLogLevel, HWND pWindow, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Window");
			}
			if	(IsWindow (pWindow))
			{
				HWND	lParent = ::GetParent (pWindow);
				HWND	lOwner = ::GetWindow (pWindow, GW_OWNER);
				DWORD	lProcessId = 0;
				DWORD	lThreadId = ::GetWindowThreadProcessId (pWindow, &lProcessId);

				LogMessage (pLogLevel, _T("%s[%8.8X] %s [%u (0x%4.4X)] [%s] Parent [%8.8X] [%s] Owner [%8.8X] [%s] Process/Thread [%4.4u %4.4u]"), lIndent, pWindow, lTitle, ::GetWindowLong(pWindow,GWL_ID), ::GetWindowLong(pWindow,GWL_ID), WndClassName(pWindow), lParent, WndClassName(lParent), lOwner, WndClassName(lOwner), lProcessId, lThreadId);
			}
			else
			if	(pWindow == HWND_TOP)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_TOP"), lIndent, pWindow, lTitle);
			}
			else
			if	(pWindow == HWND_BOTTOM)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_BOTTOM"), lIndent, pWindow, lTitle);
			}
			else
			if	(pWindow == HWND_TOPMOST)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_TOPMOST"), lIndent, pWindow, lTitle);
			}
			else
			if	(pWindow == HWND_NOTOPMOST)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_NOTOPMOST"), lIndent, pWindow, lTitle);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s <invalid window handle>"), lIndent, pWindow, lTitle);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

inline void LogWindowStyle (UINT pLogLevel, HWND pWindow, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL, LPCTSTR pSuffix = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Style ");
			}
			if	(IsWindow (pWindow))
			{
				DWORD	lStyle = GetWindowLong (pWindow, GWL_STYLE);
				DWORD	lExStyle = GetWindowLong (pWindow, GWL_EXSTYLE);
				CString	lClassName;

#ifdef	__AFXWIN_H__
				CWnd *	lWnd = CWnd::FromHandlePermanent (pWindow);
				if	(lWnd)
				{
					lClassName.Format (_T(" [%s]"), WndClassName(lWnd));
				}
#endif
				lClassName.Format (_T(" [%s]%s"), WndClassName(pWindow), CString((LPCTSTR) lClassName));

				if	(pSuffix)
				{
					LogMessage (pLogLevel, _T("%s[%8.8X] %s%s %s [%8.8X %s] [%8.8X %s]"), lIndent, pWindow, lTitle, lClassName, pSuffix, lStyle, WinStyleStr(lStyle), lExStyle, WinExStyleStr(lExStyle));
				}
				else
				{
					LogMessage (pLogLevel, _T("%s[%8.8X] %s [%8.8X %s] [%8.8X %s]%s"), lIndent, pWindow, lTitle, lStyle, WinStyleStr(lStyle), lExStyle, WinExStyleStr(lExStyle), lClassName);
				}
			}
			else
			if	(pWindow == HWND_TOP)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_TOP"), lIndent, pWindow, lTitle);
			}
			else
			if	(pWindow == HWND_BOTTOM)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_BOTTOM"), lIndent, pWindow, lTitle);
			}
			else
			if	(pWindow == HWND_TOPMOST)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_TOPMOST"), lIndent, pWindow, lTitle);
			}
			else
			if	(pWindow == HWND_NOTOPMOST)
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s HWND_NOTOPMOST"), lIndent, pWindow, lTitle);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s <invalid window handle>"), lIndent, pWindow, lTitle);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

inline void LogWindowPos (UINT pLogLevel, HWND pWindow, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("At    ");
			}
			if	(IsWindow (pWindow))
			{
				CRect	lWindowRect;
				CRect	lClientRect;
				CRect	lParentRect;
				CRect	lFrameRect;

				GetWindowRect (pWindow, &lWindowRect);
				GetClientRect (pWindow, &lClientRect);

				lFrameRect = lWindowRect;
				ScreenToClient (pWindow, (LPPOINT)&lFrameRect.left);
				ScreenToClient (pWindow, (LPPOINT)&lFrameRect.right);
				lFrameRect.SetRect (lClientRect.left-lFrameRect.left, lClientRect.top-lFrameRect.top, lFrameRect.right-lClientRect.right, lFrameRect.bottom-lClientRect.bottom);

				if	(::GetWindowLong (pWindow, GWL_STYLE) & WS_CHILD)
				{
					lParentRect = lWindowRect;
					ScreenToClient (::GetParent(pWindow), (LPPOINT)&lParentRect.left);
					ScreenToClient (::GetParent(pWindow), (LPPOINT)&lParentRect.right);
					LogMessage (pLogLevel, _T("%s[%8.8X] %s Window [%s] Client [%s] Frame [%s] Screen [%s]"), lIndent, pWindow, lTitle, FormatRect(lParentRect), FormatRect(lClientRect), FormatMargin(lFrameRect), FormatRect(lWindowRect));
				}
				else
				{
					LogMessage (pLogLevel, _T("%s[%8.8X] %s Window [%s] Client [%s] Frame [%s]"), lIndent, pWindow, lTitle, FormatRect(lWindowRect), FormatRect(lClientRect), FormatMargin(lFrameRect));
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s <invalid window handle>"), lIndent, pWindow, lTitle);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

inline void LogWindowPos (UINT pLogLevel, LPWINDOWPOS pWindowPos, UINT pMsgId = 0, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				if	(pMsgId == WM_WINDOWPOSCHANGING)
				{
					lTitle = _T("WM_WINDOWPOSCHANGING");
					pMsgId = 0;
				}
				else
				if	(pMsgId == WM_WINDOWPOSCHANGED)
				{
					lTitle = _T("WM_WINDOWPOSCHANGED");
					pMsgId = 0;
				}
				else
				{
					lTitle = _T("WinPos");
				}
			}

			if	(IsWindow (pWindowPos->hwnd))
			{
				CString	lFlagsStr;
				CString	lMsgStr;
				CString	lPosStr;
				CString	lPrevStr;
				CString	lClassName;
				CRect	lWindowRect;

				GetWindowRect (pWindowPos->hwnd, &lWindowRect);
				if	(::GetWindowLong (pWindowPos->hwnd, GWL_STYLE) & WS_CHILD)
				{
					ScreenToClient (::GetParent(pWindowPos->hwnd), (LPPOINT)&lWindowRect.left);
					ScreenToClient (::GetParent(pWindowPos->hwnd), (LPPOINT)&lWindowRect.right);
				}

				if	((pWindowPos->flags & (SWP_NOMOVE|SWP_NOSIZE)) == 0)
				{
					lPosStr.Format (_T(" MoveSize [%s] to [%d %d %d %d (%d %d)]"), FormatRect(lWindowRect), pWindowPos->x, pWindowPos->y, pWindowPos->x+pWindowPos->cx, pWindowPos->y+pWindowPos->cy, pWindowPos->cx, pWindowPos->cy);
				}
				else
				{
					if	(pWindowPos->flags & SWP_NOMOVE)
					{
						lFlagsStr += _T("NOMOVE ");
					}
					else
					{
						lPosStr.Format (_T(" Move [%s] to [%d %d %d %d (%d %d)]"), FormatMargin(lWindowRect), pWindowPos->x, pWindowPos->y, pWindowPos->x+pWindowPos->cx, pWindowPos->y+pWindowPos->cy, pWindowPos->cx, pWindowPos->cy);
					}
					if	(pWindowPos->flags & SWP_NOSIZE)
					{
						lFlagsStr += _T("NOSIZE ");
					}
					else
					{
						lPosStr.Format (_T(" Size [%s] to [%d %d (%d %d %d %d)"), FormatSize(lWindowRect.Size ()), pWindowPos->cx, pWindowPos->cy, pWindowPos->x, pWindowPos->y, pWindowPos->x+pWindowPos->cx, pWindowPos->y+pWindowPos->cy);
					}
				}
				if	(pWindowPos->flags & SWP_NOZORDER)
				{
					lFlagsStr += _T("NOZORDER ");
				}
				else
				{
#ifdef	__AFXWIN_H__
					CWnd *	lPrevWnd = CWnd::FromHandlePermanent (pWindowPos->hwndInsertAfter);

					if	(lPrevWnd)
					{
						lPrevStr.Format (_T(" After [%8.8X] [%s] [%s]"),  pWindowPos->hwndInsertAfter, WndClassName(lPrevWnd), WndClassName(pWindowPos->hwndInsertAfter));
					}
					else
#endif
					{
						lPrevStr.Format (_T(" After [%8.8X] [%s]"),  pWindowPos->hwndInsertAfter, WndClassName(pWindowPos->hwndInsertAfter));
					}

				}
				if	(pWindowPos->flags & SWP_SHOWWINDOW)
				{
					lFlagsStr += _T("SHOWWINDOW ");
				}
				if	(pWindowPos->flags & SWP_HIDEWINDOW)
				{
					lFlagsStr += _T("HIDEWINDOW ");
				}
				if	(pWindowPos->flags & SWP_FRAMECHANGED)
				{
					lFlagsStr += _T("FRAMECHANGED ");
				}
				if	(pWindowPos->flags & SWP_NOACTIVATE)
				{
					lFlagsStr += _T("NOACTIVATE ");
				}
				if	(pWindowPos->flags & SWP_NOOWNERZORDER)
				{
					lFlagsStr += _T("NOOWNERZORDER ");
				}
				if	(pWindowPos->flags & SWP_NOSENDCHANGING)
				{
					lFlagsStr += _T("NOSENDCHANGING ");
				}
				if	(pWindowPos->flags & SWP_DEFERERASE)
				{
					lFlagsStr += _T("DEFERERASE ");
				}
				if	(pWindowPos->flags & SWP_ASYNCWINDOWPOS)
				{
					lFlagsStr += _T("ASYNCWINDOWPOS ");
				}
				if	(pWindowPos->flags & SWP_NOREDRAW)
				{
					lFlagsStr += _T("NOREDRAW ");
				}
				if	(pWindowPos->flags & SWP_NOCOPYBITS)
				{
					lFlagsStr += _T("NOCOPYBITS ");
				}
				lFlagsStr.TrimRight ();

				if	(pMsgId)
				{
					lMsgStr.Format (_T(" [%4.4X]"), pMsgId);
				}
#ifdef	__AFXWIN_H__
				CWnd *	lWnd = CWnd::FromHandlePermanent (pWindowPos->hwnd);
				if	(lWnd)
				{
					lClassName.Format (_T(" [%s]"), WndClassName(lWnd));
				}
#endif
				lClassName.Format (_T(" [%s]%s"), WndClassName(pWindowPos->hwnd), CString((LPCTSTR) lClassName));

				LogMessage (pLogLevel, _T("%s[%8.8X] %s [%s]%s%s%s%s"), lIndent, pWindowPos->hwnd, lTitle, lFlagsStr, lPosStr, lMsgStr, lClassName, lPrevStr);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s[%8.8X] %s <invalid window handle>"), lIndent, pWindowPos->hwnd, lTitle);
			}
		}
		catch AnyExceptionSilent
	}
}
//////////////////////////////////////////////////////////////////////

inline void LogWindow (UINT pLogLevel, HWND pWindow, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			if	(IsWindow (pWindow))
			{
				CString	lTitle (pTitle);
				CString	lStyleTitle;
				CString	lPosTitle;

				if	(lTitle.IsEmpty ())
				{
					lTitle = _T("Window");
				}
				lTitle = (lTitle+_T("     ")).Left (max (lTitle.GetLength(),7));
				lStyleTitle.Format (_T("  %-*s"), lTitle.GetLength()-2, _T("Style"));
				lPosTitle.Format (_T("  %-*s"), lTitle.GetLength()-2, _T("At"));

				LogWindowId (pLogLevel, pWindow, lTitle, pIndent);
				LogWindowStyle (pLogLevel, pWindow, lStyleTitle, pIndent);
				LogWindowPos (pLogLevel, pWindow, lPosTitle, pIndent);
			}
			else
			{
				LogWindowId (pLogLevel, pWindow, pTitle, pIndent);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline void LogForegroundWindow (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);
			CString	lClassName;
			CString	lWindowText;
			DWORD	lProcessId;
			DWORD	lThreadId;
			HWND	lWindow;

			if	(lTitle.IsEmpty())
			{
				lTitle = _T("Window");
			}
			if	(lWindow = ::GetForegroundWindow())
			{
				::GetClassName (lWindow, lClassName.GetBuffer(MAX_PATH), MAX_PATH);
				lClassName.ReleaseBuffer();
				::SendMessage (lWindow, WM_GETTEXT, ::GetWindowTextLength(lWindow)+1, (LPARAM)lWindowText.GetBuffer(::GetWindowTextLength(lWindow)));
				lWindowText.ReleaseBuffer();
				lThreadId = GetWindowThreadProcessId (lWindow, &(lProcessId=0));

				LogMessage (pLogLevel, _T("%sForeground %s [%8.8X] [%4.4u %4.4u] [%s] [%s]"), lIndent, lTitle, lWindow, lProcessId, lThreadId, lClassName, lWindowText);
			}
			else
			{
				LogMessage (pLogLevel, _T("%sForeground %s [%8.8X]"), lIndent, lTitle, lWindow);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

inline void LogActiveWindow (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);
			CString	lClassName;
			CString	lWindowText;
			DWORD	lProcessId;
			DWORD	lThreadId;
			HWND	lWindow;

			if	(lTitle.IsEmpty())
			{
				lTitle = _T("Window");
			}
			if	(lWindow = ::GetActiveWindow())
			{
				::GetClassName (lWindow, lClassName.GetBuffer(MAX_PATH), MAX_PATH);
				lClassName.ReleaseBuffer();
				::GetWindowText (lWindow, lWindowText.GetBuffer(::GetWindowTextLength(lWindow)), ::GetWindowTextLength(lWindow)+1);
				lWindowText.ReleaseBuffer();
				lThreadId = GetWindowThreadProcessId (lWindow, &(lProcessId=0));

				LogMessage (pLogLevel, _T("%sActive     %s [%8.8X] [%4.4u %4.4u] [%s] [%s]"), lIndent, lTitle, lWindow, lProcessId, lThreadId, lClassName, lWindowText);
			}
			else
			{
				LogMessage (pLogLevel, _T("%sActive     %s [%8.8X]"), lIndent, lTitle, lWindow);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

inline void LogFocusWindow (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);
			CString	lClassName;
			CString	lWindowText;
			DWORD	lProcessId;
			DWORD	lThreadId;
			HWND	lWindow;

			if	(lTitle.IsEmpty())
			{
				lTitle = _T("Window");
			}
			if	(lWindow = ::GetFocus())
			{
				::GetClassName (lWindow, lClassName.GetBuffer(MAX_PATH), MAX_PATH);
				lClassName.ReleaseBuffer();
				::GetWindowText (lWindow, lWindowText.GetBuffer(::GetWindowTextLength(lWindow)), ::GetWindowTextLength(lWindow)+1);
				lWindowText.ReleaseBuffer();
				lThreadId = GetWindowThreadProcessId (lWindow, &(lProcessId=0));

				LogMessage (pLogLevel, _T("%sFocus      %s [%8.8X] [%4.4u %4.4u] [%s] [%s]"), lIndent, lTitle, lWindow, lProcessId, lThreadId, lClassName, lWindowText);
			}
			else
			{
				LogMessage (pLogLevel, _T("%sFocus      %s [%8.8X]"), lIndent, lTitle, lWindow);
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline void LogCustomDraw (UINT pLogLevel, NMCUSTOMDRAW * pNotify, LPCTSTR pTitle = NULL)
{
	if	(
			(pNotify)
		||	(LogIsActive (pLogLevel))
		)
	{
		CString	lTitle (pTitle);

		if	(!lTitle.IsEmpty ())
		{
			lTitle += _T(" ");
		}

		if	(pNotify->dwDrawStage == CDDS_PREERASE)
		{
			LogMessage (pLogLevel, _T("%sCDDS_PREERASE [%8.8X] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_POSTERASE)
		{
			LogMessage (pLogLevel, _T("%sCDDS_POSTERASE [%8.8X] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_PREPAINT)
		{
			CRect	lClientRect;
			CRect	lClipRect;
			::GetClientRect (pNotify->hdr.hwndFrom, &lClientRect);
			GetClipBox (pNotify->hdc, &lClipRect);

			LogMessage (pLogLevel, _T("%sCDDS_PREPAINT [%8.8X] [%d %d %d %d (%d %d)] Clip [%d %d %d %d] Client [%d %d %d %d]"), lTitle, pNotify->hdc, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top, lClipRect.left, lClipRect.top, lClipRect.right, lClipRect.bottom, lClientRect.left, lClientRect.top, lClientRect.right, lClientRect.bottom);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_POSTPAINT)
		{
			LogMessage (pLogLevel, _T("%sCDDS_POSTPAINT [%8.8X] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_ITEMPREERASE)
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s  CDDS_ITEMPREERASE [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_ITEMPOSTERASE)
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s CDDS_ITEMPOSTERASE [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_ITEMPREPAINT)
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s  CDDS_ITEMPREPAINT [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == CDDS_ITEMPOSTPAINT)
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s  CDDS_ITEMPOSTPAINT [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == (CDDS_ITEMPREPAINT|CDDS_SUBITEM))
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s  CDDS_SUBITEMPREPAINT [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		if	(pNotify->dwDrawStage == (CDDS_ITEMPOSTPAINT|CDDS_SUBITEM))
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s  CDDS_SUBITEMPOSTPAINT [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
		else
		{
			LogMessage (pLogLevel|LogHighVolume, _T("%s  CDDS_??? [%8.8X] [%8.8X] [%d] [%d %d %d %d (%d %d)]"), lTitle, pNotify->dwDrawStage, pNotify->hdc, pNotify->dwItemSpec, pNotify->rc.left, pNotify->rc.top, pNotify->rc.right, pNotify->rc.bottom, pNotify->rc.right-pNotify->rc.left, pNotify->rc.bottom-pNotify->rc.top);
		}
	}
}

//////////////////////////////////////////////////////////////////////
#endif // _DEBUGWIN_H_
