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

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetVisible (long *pbVisible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(GetPropSheetWnd (false))
	{
		if	(pbVisible)
		{
			(*pbVisible) = IsWindowVisible ();
		}
		lResult = IsWindowVisible () ? S_OK : S_FALSE;
	}
	else
	{
		if	(pbVisible)
		{
			(*pbVisible) = FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetVisible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetVisible (long bVisible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::SetVisible [%d]"), this, m_dwRef, bVisible);
#endif
	HRESULT	lResult = S_OK;

	if	(bVisible)
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

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::SetVisible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetPosition (long *plLeft, long *plTop)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetPosition"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);

	if	(GetPropSheetWnd (true))
	{
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		if	(plLeft)
		{
			(*plLeft) = lWinRect.left;
		}
		if	(plTop)
		{
			(*plTop) = lWinRect.top;
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

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetPosition"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetSize (long *plWidth, long *plHeight)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetSize"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);

	if	(GetPropSheetWnd (true))
	{
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		if	(plWidth)
		{
			(*plWidth) = lWinRect.Width();
		}
		if	(plHeight)
		{
			(*plHeight) = lWinRect.Height();
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

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetSize"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sPageNameOutput = _T("Output");
static LPCTSTR	sPageNameSpeech = _T("Speech");
static LPCTSTR	sPageNameCopyright = _T("Copyright");

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetPage (BSTR *pbszPage)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetPage"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);

	if	(GetPropSheetWnd (true))
	{
		if	(!pbszPage)
		{
			lResult = E_POINTER;
		}
		else
		{
			int	lPage = PropSheet_HwndToIndex (m_hWnd, PropSheet_GetCurrentPageHwnd (m_hWnd));

			(*pbszPage) = NULL;

			if	(lPage == 0)
			{
				(*pbszPage) = _bstr_t (sPageNameOutput).Detach();
			}
			else
			if	(lPage == 1)
			{
				(*pbszPage) = _bstr_t (sPageNameSpeech).Detach();
			}
			else
			if	(lPage == 2)
			{
				(*pbszPage) = _bstr_t (sPageNameCopyright).Detach();
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

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetPage"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetPage (BSTR bszPage)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::SetPage [%ls]"), this, m_dwRef, bszPage);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (m_hWnd == NULL);

	if	(GetPropSheetWnd (true))
	{
		CString	lPageName (bszPage);

		if	(lPageName.CompareNoCase (sPageNameOutput) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 0);
		}
		else
		if	(lPageName.CompareNoCase (sPageNameSpeech) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 1);
		}
		else
		if	(lPageName.CompareNoCase (sPageNameCopyright) == 0)
		{
			PropSheet_SetCurSel (m_hWnd, NULL, 2);
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

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::SetPage"), this, m_dwRef);
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
