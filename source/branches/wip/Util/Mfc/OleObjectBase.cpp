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
#include "StdAfx.h"
#include "OleObjectBase.h"
#include "OsVer.h"
#include "GuidStr.h"
#include "DebugWin.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_DEBUG_FOREGROUND	(GetProfileDebugInt(_T("LogForeground"),LogVerbose,true)&0xFFFF|LogTime)
#endif

//////////////////////////////////////////////////////////////////////

COleWindow::COleWindow (CCmdTarget & pCmdTarget, CWnd * pWnd)
:	mCmdTarget (pCmdTarget),
	mWnd (pWnd)
{
}

//////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) COleWindow::XOleWindow::AddRef ()
{
	METHOD_PROLOGUE_(COleWindow, OleWindow)
	return pThis->mCmdTarget.ExternalAddRef ();
}

STDMETHODIMP_(ULONG) COleWindow::XOleWindow::Release ()
{
	METHOD_PROLOGUE_(COleWindow, OleWindow)
	return pThis->mCmdTarget.ExternalRelease ();
}

STDMETHODIMP COleWindow::XOleWindow::QueryInterface (REFIID pRefIId, void ** pPVoid)
{
	METHOD_PROLOGUE_(COleWindow, OleWindow)
	return pThis->mCmdTarget.ExternalQueryInterface (&pRefIId, pPVoid);
}

//////////////////////////////////////////////////////////////////////

STDMETHODIMP COleWindow::XOleWindow::GetWindow (HWND *phwnd)
{
	METHOD_PROLOGUE_(COleWindow, OleWindow)
	HRESULT lResult = E_FAIL;
	if	(phwnd)
	{
		try
		{
			(*phwnd) = pThis->mWnd->GetSafeHwnd();
			lResult = S_OK;
		}
		catch AnyExceptionSilent
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

STDMETHODIMP COleWindow::XOleWindow::ContextSensitiveHelp (BOOL fEnterMode)
{
	METHOD_PROLOGUE_(COleWindow, OleWindow)
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CRunnableObject::CRunnableObject (CCmdTarget & pCmdTarget)
:	mCmdTarget (pCmdTarget)
{
}

HRESULT CRunnableObject::GetRunningClass (CLSID & pClassId)
{
	return E_NOTIMPL;
}

HRESULT CRunnableObject::Run ()
{
	return E_NOTIMPL;
}

bool CRunnableObject::IsRunning ()
{
	return true;
}

//////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CRunnableObject::XRunnableObject::AddRef ()
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	return pThis->mCmdTarget.ExternalAddRef ();
}

STDMETHODIMP_(ULONG) CRunnableObject::XRunnableObject::Release ()
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	return pThis->mCmdTarget.ExternalRelease ();
}

STDMETHODIMP CRunnableObject::XRunnableObject::QueryInterface (REFIID pRefIId, void ** pPVoid)
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	return pThis->mCmdTarget.ExternalQueryInterface (&pRefIId, pPVoid);
}

//////////////////////////////////////////////////////////////////////

STDMETHODIMP CRunnableObject::XRunnableObject::GetRunningClass (LPCLSID lpClsid)
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	HRESULT lResult = E_FAIL;
	if	(lpClsid)
	{
		try
		{
			lResult = pThis->GetRunningClass (*lpClsid);
		}
		catch AnyExceptionSilent
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

STDMETHODIMP CRunnableObject::XRunnableObject::Run (LPBINDCTX pbc)
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	HRESULT lResult = E_FAIL;
	try
	{
		lResult = pThis->Run ();
	}
	catch AnyExceptionSilent
	return lResult;
}

STDMETHODIMP_(BOOL) CRunnableObject::XRunnableObject::IsRunning ()
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	BOOL	lRet = FALSE;
	try
	{
		if	(pThis->IsRunning())
		{
			lRet = TRUE;
		}
	}
	catch AnyExceptionSilent
	return lRet;

}

STDMETHODIMP CRunnableObject::XRunnableObject::LockRunning (BOOL fLock, BOOL fLastUnlockCloses)
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	return E_NOTIMPL;
}

STDMETHODIMP CRunnableObject::XRunnableObject::SetContainedObject (BOOL fContained)
{
	METHOD_PROLOGUE_(CRunnableObject, RunnableObject)
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT RunOleObject (LPUNKNOWN pUnknown)
{
	HRESULT	lResult = E_FAIL;

	if	(pUnknown)
	{
#ifdef	_DEBUG_FOREGROUND
		if	(LogIsActive (_DEBUG_FOREGROUND))
		{
			LogForegroundWindow	(_DEBUG_FOREGROUND, _T("RunOleObject (a)"));
		}
#endif

		try
		{
			CoAllowSetForegroundWindow (pUnknown, NULL);
			::AllowSetForegroundWindow (ASFW_ANY);
		}
		catch AnyExceptionSilent
		try
		{
			lResult = OleRun (pUnknown);
		}
		catch AnyExceptionSilent

#ifdef	_DEBUG_FOREGROUND
		if	(LogIsActive (_DEBUG_FOREGROUND))
		{
			LogForegroundWindow	(_DEBUG_FOREGROUND, _T("RunOleObject (b)"));
		}
#endif
		if	(LogIsActive())
		{
			LogComErr (LogIfActive|LogTime, lResult, _T("RunOleObject"));
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	return lResult;
}

HRESULT RunOleWindow (LPUNKNOWN pUnknown)
{
	HRESULT	lResult = E_FAIL;

	if	(pUnknown)
	{
		try
		{
#ifdef	_DEBUG_FOREGROUND
			if	(LogIsActive (_DEBUG_FOREGROUND))
			{
				LogActiveWindow		(_DEBUG_FOREGROUND, _T("RunOleWindow (a)"));
			}
#endif
			lResult = RunOleObject (pUnknown);

#ifdef	_DEBUG_FOREGROUND
			if	(LogIsActive (_DEBUG_FOREGROUND))
			{
				LogActiveWindow		(_DEBUG_FOREGROUND, _T("RunOleWindow (b)"));
			}
#endif
			if	(SUCCEEDED (lResult))
			{
				lResult = ActivateOleWindow (pUnknown);
			}
			if	(LogIsActive())
			{
				LogComErr (LogIfActive|LogTime, lResult, _T("RunOleWindow"));
			}
		}
		catch AnyExceptionSilent
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	return lResult;
}


HRESULT ActivateOleWindow (LPUNKNOWN pUnknown)
{
	HRESULT	lResult = E_FAIL;

	if	(pUnknown)
	{
		try
		{
			IOleWindowPtr	lOleWindow (pUnknown);
			HWND			lWindow = NULL;

			if	(lOleWindow == NULL)
			{
				lResult = E_NOINTERFACE;
			}
			else
			if	(
					(SUCCEEDED (lResult = lOleWindow->GetWindow (&lWindow)))
				&&	(lWindow != NULL)
				)
			{
				DWORD	lProcessId = 0;

#ifdef	_DEBUG_FOREGROUND
				if	(LogIsActive (_DEBUG_FOREGROUND))
				{
					LogWindowId			(_DEBUG_FOREGROUND, lWindow, _T("ActivateOleWindow"));
					LogForegroundWindow	(_DEBUG_FOREGROUND, _T("ActivateOleWindow (a)"));
					LogActiveWindow		(_DEBUG_FOREGROUND, _T("ActivateOleWindow (a)"));
				}
#endif
				::AllowSetForegroundWindow (ASFW_ANY);
				::SetForegroundWindow (lWindow);

#ifdef	_DEBUG_FOREGROUND
				if	(LogIsActive (_DEBUG_FOREGROUND))
				{
					LogForegroundWindow	(_DEBUG_FOREGROUND, _T("ActivateOleWindow (b)"));
				}
#endif
				if	(
						(::GetWindowThreadProcessId (lWindow, &lProcessId) == GetCurrentThreadId ())
					&&	(lProcessId == GetCurrentProcessId ())
					)
				{
					::SetActiveWindow (lWindow);

#ifdef	_DEBUG_FOREGROUND
					if	(LogIsActive (_DEBUG_FOREGROUND))
					{
						LogActiveWindow (_DEBUG_FOREGROUND, _T("ActivateOleWindow (b)"));
					}
#endif
				}
			}
			else
			{
				if	(SUCCEEDED (lResult))
				{
					lResult = E_UNEXPECTED;
				}
				if	(LogIsActive())
				{
					LogComErr (LogIfActive|LogTime, lResult, _T("ActivateOleWindow"));
				}
			}
		}
		catch AnyExceptionSilent
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	_UNICODE
#if _MSC_VER < 1400
struct BIND_OPTS3 : BIND_OPTS2
{
	HWND hwnd;
};
#endif
#endif

//////////////////////////////////////////////////////////////////////

HRESULT CoCreateInstanceElevated (REFCLSID pClassId, LPUNKNOWN* pUnknown, REFIID pInterfaceId, DWORD pClsCtx, LPUNKNOWN pOuterUnknown)
{
	HRESULT	lResult = E_NOTIMPL;

	(*pUnknown) = NULL;

#ifdef	_UNICODE
	if	(IsWindowsVista_AtLeast ())
	{
		HWND		lForegroundWnd = NULL;
		HWND		lParentWnd = NULL;
		DWORD		lProcessId = 0;
		tPtr <CWnd>	lOwnerWnd;

		try
		{
#ifdef	_DEBUG_FOREGROUND
			if	(LogIsActive (_DEBUG_FOREGROUND))
			{
				LogForegroundWindow	(_DEBUG_FOREGROUND, _T("CreateInstanceElevated (a)"));
			}
#endif
			lForegroundWnd = ::GetForegroundWindow ();

			lParentWnd = AfxGetMainWnd ()->GetSafeHwnd ();

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

			if	(
					(
						(!lParentWnd)
					||	(!IsWindowVisible (lParentWnd))
					)
				&&	(lOwnerWnd = new CWnd)
				&&	(lOwnerWnd->CreateEx (WS_EX_TOOLWINDOW, AfxRegisterWndClass(0), NULL, 0, 0, 0, 0, 0, NULL, NULL))
				)
			{
				lOwnerWnd->ModifyStyle (WS_CAPTION, 0);
				lOwnerWnd->MoveWindow (0, 0, 0, 0, TRUE);
				lOwnerWnd->CenterWindow ();
				lOwnerWnd->ShowWindow (SW_SHOWNA);
				lParentWnd = lOwnerWnd->GetSafeHwnd();
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
			CString					lMonikerName;
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
