/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Win32Native.h"
#include <CommCtrl.h>

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

Boolean Win32Native::IsInputPending::get ()
{
	return (GetQueueStatus (QS_INPUT) != 0);
}

Boolean Win32Native::IsMouseInputPending::get ()
{
	return (GetQueueStatus (QS_MOUSE) != 0);
}

Boolean Win32Native::IsMouseWheelPending::get ()
{
	MSG	lMsg;

	if	(::PeekMessage (&lMsg, NULL, WM_MOUSEWHEEL, WM_MOUSEWHEEL, PM_NOREMOVE))
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK PopTipsProg (HWND pHWnd, LPARAM pLparam)
{
	WCHAR	lClassName [256];

	memset (lClassName, 0, sizeof(lClassName));	
	::GetClassName (pHWnd, lClassName, sizeof(lClassName)/sizeof(WCHAR));
	if	(_tcsstr (lClassName, TOOLTIPS_CLASS))
	{
		::SendMessage (pHWnd, TTM_POP, 0, 0);
	}
	return TRUE;
}

void Win32Native::PopThreadToolTips ()
{
	EnumThreadWindows (GetCurrentThreadId(), &PopTipsProg, 0);
}

void Win32Native::PopThreadToolTips (IntPtr pWindowHandle)
{
	EnumThreadWindows (GetWindowThreadProcessId ((HWND)(INT_PTR)pWindowHandle, NULL), &PopTipsProg, 0);
}

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
