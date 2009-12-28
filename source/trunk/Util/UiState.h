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
#if !defined(AFX_UISTATE_H__14B88E60_074B_11D7_AB89_0010B577C50C__INCLUDED_)
#define AFX_UISTATE_H__14B88E60_074B_11D7_AB89_0010B577C50C__INCLUDED_
#pragma once

#if	(_MSC_VER >= 1400)
#ifdef	_UNICODE
#include <dwmapi.h>
#endif
#endif
#ifndef	DWMAPI
#define IsDwmCompositionEnabled()	false
#define IsDwmRenderingEnabled(w)	false
#endif

//////////////////////////////////////////////////////////////////////

#ifndef	WM_QUERYUISTATE
#define WM_CHANGEUISTATE	0x0127
#define WM_UPDATEUISTATE	0x0128
#define WM_QUERYUISTATE		0x0129
#endif

#ifndef	UISF_HIDEFOCUS
#define UISF_HIDEFOCUS		0x1
#define UISF_HIDEACCEL		0x2
#endif

#ifndef	ODS_NOACCEL
#define	ODS_NOACCEL			0x0100
#endif

#ifndef	ODS_NOFOCUSRECT
#define	ODS_NOFOCUSRECT		0x0200
#endif

#ifndef	ODS_HOTLIGHT
#define ODS_HOTLIGHT		0x0040
#endif

//////////////////////////////////////////////////////////////////////

class CUiState
{
public:
	static UINT GetUiState (HWND pWindow);
	static UINT GetUiDrawState (HWND pWindow, UINT pDrawState);

	static void InitUiState (HWND pWindow = NULL);
	static bool UpdateUiState (MSG & pMsg);

	static bool IsScreenReaderActive ();

private:
#ifndef	_UNICODE
	static UINT	mUiState;
#endif

public:
	friend void SetFrameStatusText (LPCTSTR pText, CWnd * pFrameWnd = NULL);
	friend void SetFrameStatusText (int pMessageId, CWnd * pFrameWnd = NULL, bool pAsync = false);

	friend bool PrimeMessagePump (bool pPumpTimer = false);
	friend UINT RunMessagePump (bool pBlockInput, UINT pCycles = 0);
	friend UINT RunMessagePump (CFrameWnd * pFrameWnd, UINT pCycles = 0);
	friend UINT RunMessagePump (UINT pCycles = 0);

#ifdef	_VISTA
	friend bool MonitorMessagePumpNotResponding (HWND pWindow);
	friend bool UnmonitorMessagePumpNotResponding (HWND pWindow);
	friend bool CheckMessagePumpNotResponding (HWND pWindow, bool pReset = true);
	friend bool StopMessagePumpMonitor ();
	friend HHOOK StartMessagePumpMonitor ();
#endif

#ifdef	DWMAPI
	friend bool IsDwmCompositionEnabled ();
	friend bool IsDwmRenderingEnabled (HWND pWindow);
	friend COLORREF GetDwmFrameColor ();
	friend BYTE GetDwmFrameAlpha ();
	friend void LogDwmState (UINT pLogLevel, HWND pWindow, LPCTSTR pTitle = NULL);
#endif
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class CFocusRestore
{
public:
	CFocusRestore (bool pRestoreNulls = false);
	~CFocusRestore ();

	void Log (UINT pLogLevel, LPCTSTR pTitle = NULL);
	static void LogState (UINT pLogLevel, LPCTSTR pTitle = NULL);

private:
	bool	mRestoreNulls;
	HWND	mFocus;
	HWND	mActive;
	HWND	mForeground;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline bool _PrimeMessagePump (bool pPumpTimer = false)
{
	bool	lRet = false;
	try
	{
		MSG				lMsg;
		CWinThread *	lThread;
		MSG *			lThreadMsg;

		if	(
				(pPumpTimer)
			&&	(lThread = AfxGetThread())
#if	(_MFC_VER >= 0x0800)
			&&	(lThreadMsg = AfxGetCurrentMessage())
#else
			&&	(lThreadMsg = &lThread->m_msgCur)
#endif
			)
		{
			if	(PeekMessage (&lMsg, 0, WM_TIMER, WM_TIMER, PM_REMOVE))
			{
				try
				{
					*lThreadMsg = lMsg;
					if	(!lThread->PreTranslateMessage (lThreadMsg))
					{
						::TranslateMessage (lThreadMsg);
						::DispatchMessage (lThreadMsg);
					}
				}
				catch AnyExceptionSilent

				lRet = true;
			}
		}
		else
#if	(_WIN32_WINNT >= 0x0501)
		if	(IsGUIThread (FALSE))
#endif
		{
			if	(pPumpTimer)
			{
				if	(PeekMessage (&lMsg, 0, WM_TIMER, WM_TIMER, PM_REMOVE))
				{
					try
					{
						::TranslateMessage (&lMsg);
						::DispatchMessage (&lMsg);
					}
					catch AnyExceptionSilent
				}
			}
			else
			{
				if	(PeekMessage (&lMsg, 0, 0, 0, PM_NOREMOVE))
				{
					lRet = true;
				}
			}
		}
	}
	catch AnyExceptionSilent
	return lRet;
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_UISTATE_H__14B88E60_074B_11D7_AB89_0010B577C50C__INCLUDED_)
