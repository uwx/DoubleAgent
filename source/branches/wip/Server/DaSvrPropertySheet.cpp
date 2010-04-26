/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaServerApp.h"
#include "DaSvrPropertySheet.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrPropertySheet::DaSvrPropertySheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::DaSvrPropertySheet (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrPropertySheet::~DaSvrPropertySheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::~DaSvrPropertySheet (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrPropertySheet (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrPropertySheet * DaSvrPropertySheet::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrPropertySheet> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrPropertySheet>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrPropertySheet::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void DaSvrPropertySheet::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::FinalRelease [%p]"), this, m_dwRef, m_hWnd);
	}
#endif
	Terminate (false);
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrPropertySheet::PreShowSheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::PreShowSheet [%p]"), this, m_dwRef, m_hWnd);
	}
#endif
	AddRef ();
	CPropSheetOptions::PreShowSheet ();
}

void DaSvrPropertySheet::OnFinalMessage (HWND pWnd)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::OnFinalMessage [%p]"), this, m_dwRef, m_hWnd);
	}
#endif
	CPropSheetOptions::OnFinalMessage (pWnd);
	Release ();
}

void DaSvrPropertySheet::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::OnClientEnded [%p]"), this, m_dwRef, m_hWnd);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HWND DaSvrPropertySheet::GetPropSheetWnd (bool pCreate)
{
	if	(
			(!IsWindow ())
		&&	(pCreate)
		)
	{
		try
		{
			Create (&_AtlModule);
		}
		catch AnyExceptionDebug
	}
	if	(IsWindow ())
	{
		return m_hWnd;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrPropertySheet::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrPropertySheet), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrPropertySheet2), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentPropertySheet), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;
	HRESULT			lResult = get_Visible (&lVisible);
	
	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetPosition (long *Left, long *Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetPosition"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);
	CRect	lWinRect (0,0,0,0);

	if	(GetPropSheetWnd (true))
	{
		LoadConfig ();
		GetWindowRect (&lWinRect);
		if	(lCreated)
		{
			DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	if	(Left)
	{
		(*Left) = lWinRect.left;
	}
	if	(Top)
	{
		(*Top) = lWinRect.top;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetPosition"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetPosition (long Left, long Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::SetPosition"), this, m_dwRef);
#endif
	HRESULT						lResult = S_OK;
	CRect						lWinRect (0,0,0,0);
	HMONITOR					lMonitor = NULL;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(GetPropSheetWnd (true))
	{
		LoadConfig ();
		GetWindowRect (&lWinRect);
		lWinRect.OffsetRect (Left-lWinRect.left, Top-lWinRect.top);
		
		if	(
				(lMonitor = ::MonitorFromRect (&lWinRect, MONITOR_DEFAULTTONEAREST))
			&&	(::GetMonitorInfo (lMonitor, &lMonitorInfo))
			)
		{
			if	(lWinRect.right > lMonitorInfo.rcWork.right)
			{
				lWinRect.OffsetRect (lMonitorInfo.rcWork.right - lWinRect.right, 0);
			}
			if	(lWinRect.bottom > lMonitorInfo.rcWork.bottom)
			{
				lWinRect.OffsetRect (0, lMonitorInfo.rcWork.bottom - lWinRect.bottom);
			}
			if	(lWinRect.left < lMonitorInfo.rcWork.left)
			{
				lWinRect.OffsetRect (lMonitorInfo.rcWork.left - lWinRect.left, 0);
			}
			if	(lWinRect.top < lMonitorInfo.rcWork.top)
			{
				lWinRect.OffsetRect (0, lMonitorInfo.rcWork.top - lWinRect.top);
			}
		}
		MoveWindow (&lWinRect);
		SaveConfig (IDOK);
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::SetPosition"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetSize"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);
	CRect	lWinRect (0,0,0,0);

	if	(GetPropSheetWnd (true))
	{
		GetWindowRect (&lWinRect);
		if	(lCreated)
		{
			DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	if	(Width)
	{
		(*Width) = lWinRect.Width();
	}
	if	(Height)
	{
		(*Height) = lWinRect.Height();
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetSize"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetPage (BSTR *Page)
{
	return get_Page (Page);
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetPage (BSTR Page)
{
	return put_Page (Page);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Left (short Left)
{
	HRESULT	lResult;
	long	lTop = 0;

	if	(SUCCEEDED (lResult = GetPosition (NULL, &lTop)))
	{
		lResult = SetPosition (Left, lTop);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Left (short *Left)
{
	long	lLeft = 0;
	HRESULT	lResult = GetPosition (&lLeft, NULL);
	
	if	(Left)
	{
		(*Left) = (short)lLeft;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Top (short Top)
{
	HRESULT	lResult;
	long	lLeft = 0;

	if	(SUCCEEDED (lResult = GetPosition (&lLeft, NULL)))
	{
		lResult = SetPosition (lLeft, Top);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Top (short *Top)
{
	long	lTop = 0;
	HRESULT	lResult = GetPosition (NULL, &lTop);
	
	if	(Top)
	{
		(*Top) = (short)lTop;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Height (short *Height)
{
	long	lHeight = 0;
	HRESULT	lResult = GetSize (NULL, &lHeight);
	
	if	(Height)
	{
		(*Height) = (short)lHeight;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Width (short *Width)
{
	long	lWidth = 0;
	HRESULT	lResult = GetSize (&lWidth, NULL);
	
	if	(Width)
	{
		(*Width) = (short)lWidth;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::put_Visible [%d]"), this, m_dwRef, Visible);
#endif
	HRESULT	lResult = S_OK;

	if	(Visible)
	{
		if	(GetPropSheetWnd (true))
		{
			if	(IsWindowVisible ())
			{
				lResult = S_FALSE;
			}
			else
			{
				ShowWindow (SW_SHOW);
			}
			BringWindowToTop ();
			::SetForegroundWindow (m_hWnd);
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	else
	{
		if	(GetPropSheetWnd (false))
		{
			if	(IsWindowVisible ())
			{
				ShowWindow (SW_HIDE);
			}
			else
			{
				lResult = S_FALSE;
			}
			DestroyWindow ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::put_Visible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Visible"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(GetPropSheetWnd (false))
	{
		lResult = IsWindowVisible () ? S_OK : S_FALSE;
	}
	if	(Visible)
	{
		(*Visible) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Visible"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Page (BSTR Page)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::put_Page [%ls]"), this, m_dwRef, Page);
#endif
	HRESULT				lResult = S_OK;
	bool				lCreated = (m_hWnd == NULL);
	static CAtlString	lPageNameOutput (PropertySheet_PageName_Output);
	static CAtlString	lPageNameSpeech (PropertySheet_PageName_Speech);
	static CAtlString	lPageNameCharacter (PropertySheet_PageName_Character);
	static CAtlString	lPageNameCopyright (PropertySheet_PageName_Copyright);

	if	(GetPropSheetWnd (true))
	{
		CString	lPageName (Page);

		if	(lPageName.CompareNoCase (lPageNameOutput) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 0);
		}
		else
		if	(lPageName.CompareNoCase (lPageNameSpeech) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 1);
		}
		else
		if	(lPageName.CompareNoCase (lPageNameCharacter) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 2);
		}
		else
		if	(lPageName.CompareNoCase (lPageNameCopyright) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 3);
		}
		else
		{
			lResult = E_INVALIDARG;
		}

		if	(lCreated)
		{
			DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::put_Page"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Page (BSTR *Page)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Page"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);

	if	(GetPropSheetWnd (true))
	{
		if	(!Page)
		{
			lResult = E_POINTER;
		}
		else
		{
			int	lPage = PropSheet_HwndToIndex (m_hWnd, PropSheet_GetCurrentPageHwnd (m_hWnd));

			(*Page) = NULL;

			if	(lPage == 0)
			{
				(*Page) = _bstr_t (PropertySheet_PageName_Output).Detach();
			}
			else
			if	(lPage == 1)
			{
				(*Page) = _bstr_t (PropertySheet_PageName_Speech).Detach();
			}
			else
			if	(lPage == 2)
			{
				(*Page) = _bstr_t (PropertySheet_PageName_Character).Detach();
			}
			else
			if	(lPage == 3)
			{
				(*Page) = _bstr_t (PropertySheet_PageName_Copyright).Detach();
			}
		}

		if	(lCreated)
		{
			DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Page"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetWindow (HWND *phwnd)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetWindow"), this, m_dwRef);
#endif
	if	(phwnd)
	{
		(*phwnd) = m_hWnd;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::ContextSensitiveHelp (BOOL fEnterMode)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::ContextSensitiveHelp"), this, m_dwRef);
#endif
	return E_NOTIMPL;
}
