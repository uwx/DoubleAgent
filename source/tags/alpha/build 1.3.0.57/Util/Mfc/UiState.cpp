/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include <afxpriv.h>
#include "UiState.h"
#include "AfxTemplEx.h"
#include "Elapsed.h"
#include "DebugStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_MESSAGE_PUMP		LogDebugFast|LogTimeNs
//#define	_DEBUG_NO_MESSAGE_PUMP	LogDebugFast|LogTimeNs
//#define	_DEBUG_NOT_RESPONDING	LogNormal|LogTimeMs
#endif

//////////////////////////////////////////////////////////////////////

#ifndef	_UNICODE
UINT CUiState::mUiState = 0;
#endif

UINT CUiState::GetUiState (HWND pWindow)
{
#ifdef	_UNICODE
	return (UINT)SendMessage (pWindow, WM_QUERYUISTATE, 0, 0);
#else
	return mUiState;
#endif
}

UINT CUiState::GetUiDrawState (HWND pWindow, UINT pDrawState)
{
	UINT	lUiState = GetUiState (pWindow);

	if	(lUiState & UISF_HIDEFOCUS)
	{
		pDrawState |= ODS_NOFOCUSRECT;
	}
	else
	{
		pDrawState &= ~ODS_NOFOCUSRECT;
	}

	if	(lUiState & UISF_HIDEACCEL)
	{
		pDrawState |= ODS_NOACCEL;
	}
	else
	{
		pDrawState &= ~ODS_NOACCEL;
	}

	return pDrawState;
}

//////////////////////////////////////////////////////////////////////

void CUiState::InitUiState (HWND pWindow)
{
#ifdef	_UNICODE
	if	(pWindow)
	{
		SendMessage (pWindow, WM_CHANGEUISTATE, MAKELONG(UIS_INITIALIZE, UISF_HIDEACCEL|UISF_HIDEFOCUS), 0);
	}
#else
	mUiState = UISF_HIDEFOCUS | UISF_HIDEACCEL;
#endif
}

bool CUiState::UpdateUiState (MSG& pMsg)
{
#ifndef	_UNICODE
	if	(
			(pMsg.message == WM_SYSKEYDOWN)
		&&	(mUiState & UISF_HIDEACCEL)
		)
	{
		mUiState &= ~UISF_HIDEACCEL;
		return true;
	}
	else
	if	(
			(pMsg.message == WM_KEYDOWN)
		&&	(mUiState & UISF_HIDEFOCUS)
		&&	(
				(pMsg.wParam == VK_TAB)
			||	(pMsg.wParam == VK_UP)
			||	(pMsg.wParam == VK_DOWN)
			||	(pMsg.wParam == VK_LEFT)
			||	(pMsg.wParam == VK_RIGHT)
			)
		)
	{
		mUiState &= ~UISF_HIDEFOCUS;
		return true;
	}
#endif
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CUiState::IsScreenReaderActive ()
{
	BOOL	lScreenReader = FALSE;
	if	(
			(SystemParametersInfo (SPI_GETSCREENREADER, 0, &lScreenReader, 0))
		&&	(lScreenReader)
		)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void SetFrameStatusText (LPCTSTR pText, CWnd * pFrameWnd)
{
	try
	{
		CFrameWnd *	lFrameWnd;

		if	(!pFrameWnd)
		{
			pFrameWnd = AfxGetMainWnd ();
		}

		if	(lFrameWnd = DYNAMIC_DOWNCAST (CFrameWnd, pFrameWnd))
		{
			lFrameWnd->SetMessageText (pText);
		}
	}
	catch AnyExceptionDebug
}

void SetFrameStatusText (int pMessageId, CWnd * pFrameWnd, bool pAsync)
{
	try
	{
		CFrameWnd *	lFrameWnd;

		if	(!pFrameWnd)
		{
			pFrameWnd = AfxGetMainWnd ();
		}

		if	(pMessageId == -1)
		{
			pMessageId = AFX_IDS_IDLEMESSAGE;
		}

		if	(lFrameWnd = DYNAMIC_DOWNCAST (CFrameWnd, pFrameWnd))
		{
			if	(pAsync)
			{
				lFrameWnd->PostMessage (WM_SETMESSAGESTRING, pMessageId);
			}
			else
			if	(pMessageId == 0)
			{
				lFrameWnd->SetMessageText (_T(""));
			}
			else
			{
				lFrameWnd->SetMessageText (pMessageId);
			}
		}
	}
	catch AnyExceptionDebug
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CFocusRestore::CFocusRestore (bool pRestoreNulls)
:	mRestoreNulls (pRestoreNulls)
{
	mFocus = ::GetFocus ();
	mActive = ::GetActiveWindow ();
	mForeground = ::GetForegroundWindow ();
}

CFocusRestore::~CFocusRestore ()
{
	try
	{
		if	(
				(
					(mRestoreNulls)
				?	(
						(mFocus == NULL)
					||	(IsWindow (mFocus))
					)
				:	(
						(mFocus != NULL)
					&&	(IsWindow (mFocus))
					)
				)
			&&	(mFocus != ::GetFocus ())
			)
		{
			::SetFocus (mFocus);
		}

		if	(
				(
					(mRestoreNulls)
				?	(
						(mActive == NULL)
					||	(IsWindow (mActive))
					)
				:	(
						(mActive != NULL)
					&&	(IsWindow (mActive))
					)
				)
			&&	(mActive != ::GetActiveWindow ())
			)
		{
			::SetActiveWindow (mActive);
		}

		if	(
				(
					(mRestoreNulls)
				?	(
						(mForeground == NULL)
					||	(IsWindow (mForeground))
					)
				:	(
						(mForeground != NULL)
					&&	(IsWindow (mForeground))
					)
				)
			&&	(mForeground != ::GetForegroundWindow ())
			)
		{
			::SetForegroundWindow (mForeground);
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

void CFocusRestore::Log (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CWnd *	lFocusWnd = NULL;
			CWnd *	lActiveWnd = NULL;
			CWnd *	lForegroundWnd = NULL;
			CString	lFocusClass;
			CString	lActiveClass;
			CString	lForegroundClass;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Restore");
			}

			if	(
					(mFocus)
				&&	(IsWindow (mFocus))
				)
			{
				lFocusWnd = CWnd::FromHandlePermanent (mFocus);
				GetClassName (mFocus, lFocusClass.GetBuffer (MAX_PATH), MAX_PATH);
				lFocusClass.ReleaseBuffer ();
			}

			if	(
					(mActive)
				&&	(IsWindow (mActive))
				)
			{
				lActiveWnd = CWnd::FromHandlePermanent (mActive);
				GetClassName (mActive, lActiveClass.GetBuffer (MAX_PATH), MAX_PATH);
				lActiveClass.ReleaseBuffer ();
			}

			if	(
					(mForeground)
				&&	(IsWindow (mForeground))
				)
			{
				lForegroundWnd = CWnd::FromHandlePermanent (mForeground);
				GetClassName (mForeground, lForegroundClass.GetBuffer (MAX_PATH), MAX_PATH);
				lForegroundClass.ReleaseBuffer ();
			}

			LogMessage (pLogLevel, _T("%s Focus      [%p] [%hs] [%s]"), lTitle, mFocus, (lFocusWnd ? lFocusWnd->GetRuntimeClass ()->m_lpszClassName : ""), lFocusClass);
			lTitle = CString (_T(' '), lTitle.GetLength ());
			LogMessage (pLogLevel, _T("%s Active     [%p] [%hs] [%s]"), lTitle, mActive, (lActiveWnd ? lActiveWnd->GetRuntimeClass ()->m_lpszClassName : ""), lActiveClass);
			LogMessage (pLogLevel, _T("%s Foreground [%p] [%hs] [%s]"), lTitle, mForeground, (lForegroundWnd ? lForegroundWnd->GetRuntimeClass ()->m_lpszClassName : ""), lForegroundClass);
		}
		catch AnyExceptionSilent
	}
}

void CFocusRestore::LogState (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			HWND	lFocus = ::GetFocus ();
			HWND	lActive = ::GetActiveWindow ();
			HWND	lForeground = ::GetForegroundWindow ();
			CWnd *	lFocusWnd = NULL;
			CWnd *	lActiveWnd = NULL;
			CWnd *	lForegroundWnd = NULL;
			CString	lFocusClass;
			CString	lActiveClass;
			CString	lForegroundClass;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Current");
			}

			if	(
					(lFocus)
				&&	(IsWindow (lFocus))
				)
			{
				lFocusWnd = CWnd::FromHandlePermanent (lFocus);
				GetClassName (lFocus, lFocusClass.GetBuffer (MAX_PATH), MAX_PATH);
				lFocusClass.ReleaseBuffer ();
			}

			if	(
					(lActive)
				&&	(IsWindow (lActive))
				)
			{
				lActiveWnd = CWnd::FromHandlePermanent (lActive);
				GetClassName (lActive, lActiveClass.GetBuffer (MAX_PATH), MAX_PATH);
				lActiveClass.ReleaseBuffer ();
			}

			if	(
					(lForeground)
				&&	(IsWindow (lForeground))
				)
			{
				lForegroundWnd = CWnd::FromHandlePermanent (lForeground);
				GetClassName (lForeground, lForegroundClass.GetBuffer (MAX_PATH), MAX_PATH);
				lForegroundClass.ReleaseBuffer ();
			}

			LogMessage (pLogLevel, _T("%s Focus      [%p] [%hs] [%s]"), lTitle, lFocus, (lFocusWnd ? lFocusWnd->GetRuntimeClass ()->m_lpszClassName : ""), lFocusClass);
			lTitle = CString (_T(' '), lTitle.GetLength ());
			LogMessage (pLogLevel, _T("%s Active     [%p] [%hs] [%s]"), lTitle, lActive, (lActiveWnd ? lActiveWnd->GetRuntimeClass ()->m_lpszClassName : ""), lActiveClass);
			LogMessage (pLogLevel, _T("%s Foreground [%p] [%hs] [%s]"), lTitle, lForeground, (lForegroundWnd ? lForegroundWnd->GetRuntimeClass ()->m_lpszClassName : ""), lForegroundClass);
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool PrimeMessagePump (bool pPumpTimer)
{
	return _PrimeMessagePump (pPumpTimer);
}

//////////////////////////////////////////////////////////////////////

UINT RunMessagePump (bool pBlockInput, UINT pCycles)
{
	UINT	lRet = 0;

#ifdef	_DEBUG_NO_MESSAGE_PUMP
	LogMessage (_DEBUG_NO_MESSAGE_PUMP, _T("RunMessagePump [%u] [%u] skipped"), pBlockInput, pCycles);
	return lRet;
#endif

	try
	{
		CFrameWnd *	lFrameWnd = DYNAMIC_DOWNCAST (CFrameWnd, AfxGetMainWnd ());
		UINT		lFrameWndFlags;

		if	(IsWindow (lFrameWnd->GetSafeHwnd ()))
		{
			if	(
					(!pBlockInput)
				||	(lFrameWnd->InModalState ())
				)
			{
				lRet = RunMessagePump (lFrameWnd, pCycles);
			}
			else
			{
				lFrameWndFlags = lFrameWnd->m_nFlags;
				lFrameWnd->m_nFlags |= WF_STAYACTIVE;
				lFrameWnd->BeginModalState ();
				lFrameWnd->m_nFlags &= (lFrameWndFlags & WF_STAYACTIVE) | (~WF_STAYACTIVE);

				try
				{
					lRet = RunMessagePump (lFrameWnd, pCycles);
				}
				catch AnyExceptionSilent

				lFrameWnd->EndModalState ();
			}
		}
		else
		{
			lRet = RunMessagePump (pCycles);
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

UINT RunMessagePump (CFrameWnd * pFrameWnd, UINT pCycles)
{
	UINT	lRet = 0;

#ifdef	_DEBUG_NO_MESSAGE_PUMP
	LogMessage (_DEBUG_NO_MESSAGE_PUMP, _T("RunMessagePump [%p] [%u] skipped"), pFrameWnd->GetSafeHwnd(), pCycles);
	return lRet;
#endif

	try
	{
		CWinApp *	lApp = AfxGetApp ();
		MSG			lMsg;

		if	(pCycles == 0)
		{
			pCycles = 10;
		}

		while	(
					(pCycles > 0)
				&&	(IsWindow (pFrameWnd->GetSafeHwnd ()))
				)
		{
			pCycles--;

			if	(
					(PeekMessage (&lMsg, 0, 0, 0, PM_NOREMOVE))
				&&	(lMsg.message != WM_QUIT)
				)
			{
#ifdef	_DEBUG_MESSAGE_PUMP
				LogMessage (_DEBUG_MESSAGE_PUMP, _T("[%p] Pump [%4.4X] [%s] Class [%s] [%s]"), lMsg.hwnd, lMsg.message, CDebugTrace::GetMsgName(lMsg.hwnd, lMsg.message), WndClassName(CWnd::FromHandle(lMsg.hwnd)), WndClassName(lMsg.hwnd));
#endif
				lApp->PumpMessage ();
				lRet++;
			}
			else
			{
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

UINT RunMessagePump (UINT pCycles)
{
	UINT	lRet = 0;

#ifdef	_DEBUG_NO_MESSAGE_PUMP
	LogMessage (_DEBUG_NO_MESSAGE_PUMP, _T("RunMessagePump [%u] skipped"), pCycles);
	return lRet;
#endif

	try
	{
		CWinApp *	lApp = AfxGetApp ();
		MSG *		lAppMsg;
		MSG			lMsg;

		if	(pCycles == 0)
		{
			pCycles = 20;
		}

		while	(pCycles > 0)
		{
			pCycles--;

			if	(
					(lApp)
#if	(_MFC_VER >= 0x0800)
				&&	(lAppMsg = AfxGetCurrentMessage())
#else
				&&	(lAppMsg = &lApp->m_msgCur)
#endif
				)
			{
				if	(
						(PeekMessage (&lMsg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
					||	(PeekMessage (&lMsg, 0, WM_PAINT, WM_NCPAINT, PM_REMOVE))
					)
				{
#ifdef	_DEBUG_MESSAGE_PUMP
					LogMessage (_DEBUG_MESSAGE_PUMP, _T("[%p] Pump [%4.4X] [%s] Class [%s] [%s]"), lMsg.hwnd, lMsg.message, CDebugTrace::GetMsgName(lMsg.hwnd, lMsg.message), WndClassName(CWnd::FromHandle(lMsg.hwnd)), WndClassName(lMsg.hwnd));
#endif
					*lAppMsg = lMsg;
					if	(!lApp->PreTranslateMessage (lAppMsg))
					{
						::TranslateMessage (lAppMsg);
						::DispatchMessage (lAppMsg);
					}
					lRet++;
				}
				else
				{
					break;
				}
			}
			else
			{
				if	(PeekMessage (&lMsg, 0, 0, 0, PM_REMOVE))
				{
#ifdef	_DEBUG_MESSAGE_PUMP
					LogMessage (_DEBUG_MESSAGE_PUMP, _T("[%p] Pump [%4.4X] [%s] Class [%s] [%s]"), lMsg.hwnd, lMsg.message, CDebugTrace::GetMsgName(lMsg.hwnd, lMsg.message), WndClassName(CWnd::FromHandle(lMsg.hwnd)), WndClassName(lMsg.hwnd));
#endif
					::TranslateMessage (&lMsg);
					::DispatchMessage (&lMsg);
					lRet++;
				}
				else
				{
					break;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#if	(WINVER >= 0x0600)
//////////////////////////////////////////////////////////////////////

struct sMessagePumpMonitorWnd
{
	HWND	mWindow;
	bool	mNotResponding : 2;

	sMessagePumpMonitorWnd (HWND pWindow = NULL) : mWindow (pWindow), mNotResponding (false) {}
	bool operator == (const sMessagePumpMonitorWnd & pSource) const {return mWindow == pSource.mWindow;}
	bool operator != (const sMessagePumpMonitorWnd & pSource) const {return mWindow != pSource.mWindow;}
	bool operator > (const sMessagePumpMonitorWnd & pSource) const {return mWindow > pSource.mWindow;}
	bool operator < (const sMessagePumpMonitorWnd & pSource) const {return mWindow < pSource.mWindow;}
};

static HHOOK									sMessagePumpHook = NULL;
static CStructArray <sMessagePumpMonitorWnd>	sMessagePumpMonitor;

//////////////////////////////////////////////////////////////////////

bool MonitorMessagePumpNotResponding (HWND pWindow)
{
	bool	lRet = false;

	if	(
			(::IsWindow (pWindow))
		&&	((::GetWindowLong (pWindow, GWL_STYLE) & WS_CHILD) == 0)
		&&	(sMessagePumpMonitor.AddSortedQS (pWindow) >= 0)
		)
	{
#ifdef	_DEBUG_NOT_RESPONDING
		LogMessage (_DEBUG_NOT_RESPONDING, _T("[%p] [%s] Monitor not responding"), pWindow, WndClassName(pWindow));
#endif
		lRet = true;
	}
	return lRet;
}

bool UnmonitorMessagePumpNotResponding (HWND pWindow)
{
	bool	lRet = false;

	if	(sMessagePumpMonitor.RemoveSortedQS (pWindow) >= 0)
	{
#ifdef	_DEBUG_NOT_RESPONDING
		LogMessage (_DEBUG_NOT_RESPONDING, _T("[%p] [%s] Unmonitor not responding"), pWindow, WndClassName(pWindow));
#endif
		lRet = true;
	}
	return lRet;
}

bool CheckMessagePumpNotResponding (HWND pWindow, bool pReset)
{
	bool	lRet = false;
	INT_PTR	lNdx;

	lNdx = sMessagePumpMonitor.FindSortedQS (pWindow);
	if	(lNdx >= 0)
	{
		if	(sMessagePumpMonitor [lNdx].mNotResponding)
		{
			lRet = true;
			if	(pReset)
			{
				sMessagePumpMonitor [lNdx].mNotResponding = false;
#ifdef	_DEBUG_NOT_RESPONDING
				LogMessage (_DEBUG_NOT_RESPONDING, _T("[%p] [%s] Reset not responding"), sMessagePumpMonitor [lNdx].mWindow, WndClassName(sMessagePumpMonitor [lNdx].mWindow));
#endif
			}
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

static LRESULT CALLBACK MessagePumpMonitor (int code, WPARAM wParam, LPARAM lParam)
{
	if	(code == HSHELL_WINDOWREPLACED)
	{
		try
		{
			INT_PTR	lNdx;

#ifdef	_DEBUG_NOT_RESPONDING
			LogMessage (_DEBUG_NOT_RESPONDING, _T("HSHELL_WINDOWREPLACED [%p] [%p]"), wParam, lParam);
#endif

			if	(
					(sMessagePumpMonitor.GetSize() > 0)
				&&	((lNdx = sMessagePumpMonitor.FindSortedQS ((HWND)lParam)) >= 0)
				)
			{
#ifdef	_DEBUG_NOT_RESPONDING
				LogMessage (_DEBUG_NOT_RESPONDING, _T("[%p] [%s] Not Responding"), (HWND)lParam, WndClassName((HWND)lParam));
#endif
				sMessagePumpMonitor [lNdx].mNotResponding = true;

				try
				{
					if	(!::RedrawWindow ((HWND)lParam, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_FRAME|RDW_INTERNALPAINT))
					{
#ifdef	_DEBUG_NOT_RESPONDING
						LogWinErr (_DEBUG_NOT_RESPONDING, GetLastError(), _T("[%p] RedrawWindow"), (HWND)lParam);
#endif
					}
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
	}
	return CallNextHookEx (NULL, code, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////

HHOOK StartMessagePumpMonitor ()
{
	if	(!sMessagePumpHook)
	{
		sMessagePumpHook = ::SetWindowsHookEx (WH_SHELL, MessagePumpMonitor, NULL, GetCurrentThreadId());
#ifdef	_DEBUG_NOT_RESPONDING
		LogMessage (_DEBUG_NOT_RESPONDING, _T("StartMessagePumpMonitor [%p]"), sMessagePumpHook);
#endif
	}
	return sMessagePumpHook;
}

bool StopMessagePumpMonitor ()
{
	bool	lRet = false;

	try
	{
		HHOOK	lHandle = sMessagePumpHook;

		sMessagePumpHook = NULL;

		if	(
				(lHandle)
			&&	(::UnhookWindowsHookEx (lHandle))
			)
		{
			lRet = true;
		}
#ifdef	_DEBUG_NOT_RESPONDING
		if	(lHandle)
		{
			LogMessage (_DEBUG_NOT_RESPONDING, _T("StopMessagePumpMonitor [%p] [%u]"), lHandle, lRet);
		}
#endif
	}
	catch AnyExceptionSilent

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#endif	// WINVER
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifdef	DWMAPI
//////////////////////////////////////////////////////////////////////
#pragma comment(lib, "dwmapi.lib")

bool IsDwmCompositionEnabled ()
{
	bool	lRet = false;
#if	(WINVER >= 0x0600)
	try
	{
		BOOL	lCompositionEnabled = FALSE;

		if	(SUCCEEDED (DwmIsCompositionEnabled (&lCompositionEnabled)))
		{
			lRet = (lCompositionEnabled != FALSE);
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

bool IsDwmRenderingEnabled (HWND pWindow)
{
	bool	lRet = false;
#if	(WINVER >= 0x0600)
	try
	{
		BOOL	lCompositionEnabled = FALSE;
		BOOL	lNcRenderingEnabled = FALSE;

		if	(
				(SUCCEEDED (DwmIsCompositionEnabled (&lCompositionEnabled)))
			&&	(lCompositionEnabled)
			&&	(IsWindow (pWindow))
			&&	(SUCCEEDED (DwmGetWindowAttribute (pWindow, DWMWA_NCRENDERING_ENABLED, &lNcRenderingEnabled, sizeof(lNcRenderingEnabled))))
			&&	(lNcRenderingEnabled)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////

COLORREF GetDwmFrameColor ()
{
	COLORREF	lRet = GetSysColor (COLOR_ACTIVECAPTION)|0xFF000000;
#if	(WINVER >= 0x0600)
	try
	{
		COLORREF	lColorization = lRet;
		BOOL		lOpaqueBlend = TRUE;

		if	(SUCCEEDED (DwmGetColorizationColor (&lColorization, &lOpaqueBlend)))
		{
			lRet = RGB (GetBValue(lColorization), GetGValue(lColorization), GetRValue(lColorization));
			if	(lOpaqueBlend)
			{
				lRet |= 0xFF000000;
			}
			else
			{
				lRet |= (lColorization&0xFF000000);
			}
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

BYTE GetDwmFrameAlpha ()
{
	BYTE	lRet = 255;
#if	(WINVER >= 0x0600)
	try
	{
		COLORREF	lColorization = GetSysColor (COLOR_ACTIVECAPTION) | 0xFF000000;
		BOOL		lOpaqueBlend = TRUE;

		if	(SUCCEEDED (DwmGetColorizationColor (&lColorization, &lOpaqueBlend)))
		{
			if	(lOpaqueBlend)
			{
				lRet = 255;
			}
			else
			{
				lRet = (BYTE)(lColorization>>24);
			}
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////

void LogDwmState (UINT pLogLevel, HWND pWindow, LPCTSTR pTitle)
{
#if	(WINVER >= 0x0600)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString		lTitle (pTitle);
			BOOL		lCompositionEnabled = FALSE;
			COLORREF	lColorization = 0;
			BOOL		lOpaqueBlend = FALSE;
			BOOL		lNcRenderingEnabled = TRUE;
			RECT		lButtonBounds;
			RECT		lExtendedBounds;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("DWM State");
			}

			DwmIsCompositionEnabled (&lCompositionEnabled);
			DwmGetColorizationColor (&lColorization, &lOpaqueBlend);

			if	(pWindow)
			{
				LogMessage (pLogLevel, _T("%s for [%p] [%s]"), lTitle, pWindow, WndClassName(pWindow));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s"), lTitle);
			}
			LogMessage (pLogLevel|LogHighVolume, _T("  Composition enabled   [%u]"), lCompositionEnabled);
			LogMessage (pLogLevel|LogHighVolume, _T("  Colorization          [%2.2X%2.2X%2.2X%2.2X] opaque [%u]"), (BYTE)(lColorization>>24), GetRValue(lColorization), GetGValue(lColorization), GetBValue(lColorization), lOpaqueBlend);

			if	(IsWindow (pWindow))
			{
				tSS <WINDOWINFO, DWORD>	lWindowInfo;

				LogComErr (pLogLevel, DwmGetWindowAttribute (pWindow, DWMWA_NCRENDERING_ENABLED, &lNcRenderingEnabled, sizeof(lNcRenderingEnabled)), _T("DWMWA_NCRENDERING_ENABLED"));
				LogMessage (pLogLevel|LogHighVolume, _T("  NC Rendering          [%u]"), lNcRenderingEnabled);

				if	(GetWindowInfo (pWindow, &lWindowInfo))
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  Window                [%d %d %d %d]"), lWindowInfo.rcWindow.left, lWindowInfo.rcWindow.top, lWindowInfo.rcWindow.right, lWindowInfo.rcWindow.bottom);
					LogMessage (pLogLevel|LogHighVolume, _T("  Client                [%d %d %d %d]"), lWindowInfo.rcClient.left, lWindowInfo.rcClient.top, lWindowInfo.rcClient.right, lWindowInfo.rcClient.bottom);
				}

				if	(
						(lCompositionEnabled)
					&&	(lNcRenderingEnabled)
					&&	(SUCCEEDED (LogComErr (pLogLevel, DwmGetWindowAttribute (pWindow, DWMWA_CAPTION_BUTTON_BOUNDS,  &lButtonBounds, sizeof(lButtonBounds)), _T("DWMWA_CAPTION_BUTTON_BOUNDS"))))
					)
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  Caption button bounds [%d %d %d %d]"), lButtonBounds.left, lButtonBounds.top, lButtonBounds.right, lButtonBounds.bottom);
				}
				if	(
						(lCompositionEnabled)
					&&	(SUCCEEDED (LogComErr (pLogLevel, DwmGetWindowAttribute (pWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &lExtendedBounds, sizeof(lExtendedBounds)), _T("DWMWA_EXTENDED_FRAME_BOUNDS"))))
					)
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  Extended frame bounds [%d %d %d %d]"), lExtendedBounds.left, lExtendedBounds.top, lExtendedBounds.right, lExtendedBounds.bottom);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////
#endif	//	DWMAPI
//////////////////////////////////////////////////////////////////////
