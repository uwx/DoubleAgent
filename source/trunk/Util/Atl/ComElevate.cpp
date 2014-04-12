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
#include "ComElevate.h"
#include "OsVer.h"
#include "GuidStr.h"
#include "DebugWin.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_FOREGROUND	(GetProfileDebugInt(_T("LogForeground"),LogVerbose,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1400
struct BIND_OPTS3 : BIND_OPTS2
{
	HWND hwnd;
};
#endif

//////////////////////////////////////////////////////////////////////

HRESULT CoCreateInstanceElevated (REFCLSID pClassId, LPUNKNOWN* pUnknown, REFIID pInterfaceId, DWORD pClsCtx, LPUNKNOWN pOuterUnknown)
{
	HRESULT	lResult = E_NOTIMPL;

	(*pUnknown) = NULL;

#ifdef	_UNICODE
	if	(IsWindowsVista_AtLeast ())
	{
		HWND	lForegroundWnd = NULL;
		HWND	lParentWnd = NULL;
		DWORD	lProcessId = 0;

		try
		{
#ifdef	_DEBUG_FOREGROUND
			if	(LogIsActive (_DEBUG_FOREGROUND))
			{
				LogForegroundWindow	(_DEBUG_FOREGROUND, _T("CreateInstanceElevated (a)"));
			}
#endif
			lForegroundWnd = ::GetForegroundWindow ();
			lParentWnd = ::GetActiveWindow ();

			if	(
					(
						(!lParentWnd)
					||	(!IsWindowVisible (lParentWnd))
					)
				&&	(
						(lForegroundWnd)
					&&	(GetWindowThreadProcessId (lForegroundWnd, &lProcessId) == GetCurrentThreadId())
					&&	(lProcessId == GetCurrentProcessId())
					)
				)
			{
				lParentWnd = lForegroundWnd;
			}
			else
			{
				lForegroundWnd = NULL;
			}

#ifdef	_DEBUG_FOREGROUND
			if	(
					(::IsWindow (lParentWnd))
				&&	(LogIsActive (_DEBUG_FOREGROUND))
				)
			{
				LogWindowId (_DEBUG_FOREGROUND, lParentWnd, _T("CreateInstanceElevated Parent"));
			}
#endif
			::AllowSetForegroundWindow (ASFW_ANY);
		}
		catch AnyExceptionDebug

		lResult = CoCreateInstanceElevated (pClassId, pUnknown, lParentWnd, pInterfaceId, pClsCtx, pOuterUnknown);

		try
		{
#ifdef	_DEBUG_FOREGROUND
			if	(LogIsActive (_DEBUG_FOREGROUND))
			{
				LogForegroundWindow	(_DEBUG_FOREGROUND, _T("CreateInstanceElevated (c)"));
			}
#endif

			if	(
					(lForegroundWnd)
				&&	(IsWindow (lForegroundWnd))
				&&	(lForegroundWnd != ::GetForegroundWindow())
				)
			{
				::AllowSetForegroundWindow (ASFW_ANY);
				::SetForegroundWindow (lForegroundWnd);

#ifdef	_DEBUG_FOREGROUND
				if	(LogIsActive (_DEBUG_FOREGROUND))
				{
					LogForegroundWindow	(_DEBUG_FOREGROUND, _T("CreateInstanceElevated (d)"));
				}
#endif
			}
		}
		catch AnyExceptionDebug
	}
#endif
	return lResult;
}

HRESULT CoCreateInstanceElevated (REFCLSID pClassId, LPUNKNOWN* pUnknown, HWND pParentWnd, REFIID pInterfaceId, DWORD pClsCtx, LPUNKNOWN pOuterUnknown)
{
	HRESULT	lResult = E_NOTIMPL;

	(*pUnknown) = NULL;

#ifdef	_UNICODE
	if	(IsWindowsVista_AtLeast ())
	{
		try
		{
			CAtlString				lMonikerName;
			tSS <BIND_OPTS3, DWORD>	lBindOpts;
			IUnknownPtr				lUnknown;

			AllowSetForegroundWindow (ASFW_ANY);

			lMonikerName.Format (_T("Elevation:Administrator!new:%s"), (CString) CGuidStr(pClassId));
			lBindOpts.grfFlags = BIND_MAYBOTHERUSER;
			lBindOpts.hwnd = pParentWnd;
			lBindOpts.dwClassContext = (pClsCtx) ? pClsCtx : CLSCTX_LOCAL_SERVER;

			lResult = LogComErr (LogNormal|LogTime, CoGetObject (lMonikerName, &lBindOpts, __uuidof(IUnknown), (void**)&lUnknown), _T("CoGetObject [%s] [%s] [%p] [%s] [%8.8X]"), (LPCTSTR) lMonikerName, CGuidStr::GuidName(pInterfaceId), lBindOpts.hwnd, WndClassName(lBindOpts.hwnd), lBindOpts.dwClassContext);

			if	(
					(SUCCEEDED (lResult))
				&&	(lUnknown != NULL)
				&&	(!IsEqualGUID (pInterfaceId, __uuidof(IUnknown)))
				)
			{
				lResult = lUnknown->QueryInterface (pInterfaceId, (void**)pUnknown);
			}
			else
			{
				(*pUnknown) = lUnknown.Detach ();
			}
		}
		catch AnyExceptionDebug
	}
#endif
	return lResult;
}
