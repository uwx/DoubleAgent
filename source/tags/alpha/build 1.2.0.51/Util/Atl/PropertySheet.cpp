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
#include "PropertySheet.h"
#include "Localize.h"
#include "DebugStr.h"

#pragma comment(lib, "comctl32.lib")

#ifdef	_DEBUG
//#define	_DEBUG_PROPSHEET	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

CAtlPropertySheet::CAtlPropertySheet()
:	mPageChanged (false),
	mIsModal (false),
	mOwner (NULL)
{
}

CAtlPropertySheet::~CAtlPropertySheet()
{
	if	(IsWindow ())
	{
		DestroyWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAtlPropertySheet::Construct (UINT pCaptionId, HWND pParentWnd)
{
	mPsh.hwndParent = pParentWnd;
	if	(pCaptionId)
	{
		mCaption = CLocalize::LoadString (pCaptionId);
		if	(!mCaption.IsEmpty ())
		{
			mPsh.pszCaption = (LPCTSTR) mCaption;
		}
	}
}

bool CAtlPropertySheet::IsModal () const
{
	return mIsModal;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAtlPropertySheet::Create (_AtlPropSheetOwner* pOwner)
{
	bool	lRet = false;

	mIsModal = false;
	mOwner = pOwner;

	if	(PreCreateSheet (false))
	{
		INT_PTR	lNdx;
		HWND	lWnd;

		mPsp = new PROPSHEETPAGE [mPages.GetCount()];

		for	(lNdx = 0; lNdx < (INT_PTR)mPages.GetCount(); lNdx++)
		{
			mPsp [lNdx] = mPages [lNdx]->mPsp;
		}
		mPsh.nPages = (UINT)mPages.GetCount();
		mPsh.ppsp = (LPCPROPSHEETPAGE) mPsp.Ptr();
		mPsh.pfnCallback = PropSheetCallbackModeless;
		mPsh.dwFlags |= PSH_MODELESS|PSH_USECALLBACK|PSH_PROPSHEETPAGE|PSH_USEPSTARTPAGE;

		InitThunkForCallback ();
		if	(lWnd = (HWND)PropertySheet (&mPsh))
		{
			ATLASSERT(lWnd == m_hWnd);
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAtlPropertySheet::SetModalParent (HWND pParentWnd)
{
	mPsh.hwndParent = pParentWnd;
}

INT_PTR CAtlPropertySheet::DoModal ()
{
	INT_PTR	lRet = -1;

	if	(PreCreateSheet (true))
	{
		INT_PTR	lNdx;
		bool	lParentDisabled = false;

		mPsp = new PROPSHEETPAGE [mPages.GetCount()];

		for	(lNdx = 0; lNdx < (INT_PTR)mPages.GetCount(); lNdx++)
		{
			mPsp [lNdx] = mPages [lNdx]->mPsp;
		}
		mPsh.nPages = (UINT)mPages.GetCount();
		mPsh.ppsp = (LPCPROPSHEETPAGE) mPsp.Ptr();
		mPsh.pfnCallback = PropSheetCallbackModal;
		mPsh.dwFlags |= PSH_USECALLBACK|PSH_PROPSHEETPAGE|PSH_USEPSTARTPAGE|PSH_USEPAGELANG;

		if	(
				(::IsWindow (mPsh.hwndParent))
			&&	(::IsWindowEnabled (mPsh.hwndParent))
			)
		{
			::EnableWindow (mPsh.hwndParent, FALSE);
			lParentDisabled = true;
		}

		mIsModal = true;
		try
		{
			InitThunkForCallback ();
			lRet = PropertySheet (&mPsh);
		}
		catch AnyExceptionDebug
		mIsModal = false;

		if	(lParentDisabled)
		{
			::EnableWindow (mPsh.hwndParent, TRUE);
		}
	}
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s Modal Result [%d]"), this, AtlTypeName(this), lRet);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAtlPropertySheet::PreCreateSheet (bool pModal)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s PreCreateSheet"), this, AtlTypeName(this));
#endif
	return true;
}

void CAtlPropertySheet::PreDestroySheet ()
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s PreDestroySheet"), this, AtlTypeName(this));
#endif
	SaveConfig (0);
}

void CAtlPropertySheet::PreShowSheet ()
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s PreShowSheet"), this, AtlTypeName(this));
#endif
	HFONT	lFont;

	if	(lFont = (HFONT)::SendMessage (PropSheet_GetCurrentPageHwnd (m_hWnd), WM_GETFONT, 0, 0))
	{
		::SendMessage (PropSheet_GetTabControl(m_hWnd), WM_SETFONT, (WPARAM)lFont, TRUE);
	}

	LoadConfig ();
	PropSheet_RecalcPageSizes (m_hWnd);
	PropSheet_SetCurSel (m_hWnd, NULL, mPsh.nStartPage);

	if	(mPsh.dwFlags & PSH_HASHELP)
	{
		ModifyStyleEx (0, WS_EX_CONTEXTHELP);
	}
	else
	{
		ModifyStyleEx (WS_EX_CONTEXTHELP, 0);
	}
	if	(mOwner)
	{
		try
		{
			mOwner->OnShowModelessPropertySheet (this);
		}
		catch AnyExceptionSilent
	}
}

void CAtlPropertySheet::PreHideSheet ()
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s PreHideSheet"), this, AtlTypeName(this));
#endif
	if	(mOwner)
	{
		try
		{
			mOwner->OnHideModelessPropertySheet (this);
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////

void CAtlPropertySheet::InitThunkForCallback ()
{
	m_thunk.Init (NULL, NULL);
	_AtlWinModule.AddCreateWndData (&m_thunk.cd, this);
}

int CALLBACK CAtlPropertySheet::PropSheetCallbackModeless (HWND hwnd, UINT message, LPARAM lParam)
{
	switch (message)
	{
		case PSCB_PRECREATE:
		{
			LPDLGTEMPLATE		lTemplate = (LPDLGTEMPLATE)lParam;

			lTemplate->style &= ~WS_VISIBLE;
			lTemplate->style |= DS_SETFONT;
			lTemplate->dwExtendedStyle &= ~WS_EX_APPWINDOW;
		}	break;
		case PSCB_INITIALIZED:
		{
			CAtlPropertySheet* lThis = (CAtlPropertySheet*) _AtlWinModule.ExtractCreateWndData();

			lThis->SubclassWindow (hwnd);
#ifdef	_DEBUG_PROPSHEET
			LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s PropSheetCallbackModeless"), lThis, AtlTypeName(lThis));
#endif
		}	break;
	}
	return 0;
}

int CALLBACK CAtlPropertySheet::PropSheetCallbackModal (HWND hwnd, UINT message, LPARAM lParam)
{
	switch (message)
	{
		case PSCB_PRECREATE:
		{
			LPDLGTEMPLATE	lTemplate = (LPDLGTEMPLATE)lParam;

			lTemplate->style |= DS_SETFONT;
		}	break;
		case PSCB_INITIALIZED:
		{
			CAtlPropertySheet* lThis = (CAtlPropertySheet*) _AtlWinModule.ExtractCreateWndData();

			lThis->SubclassWindow (hwnd);
#ifdef	_DEBUG_PROPSHEET
			LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s PropSheetCallbackModal"), lThis, AtlTypeName(lThis));
#endif
		}	break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAtlPropertySheet::OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s OnShowWindow [%u]"), this, AtlTypeName(this), wParam);
#endif
	if	(wParam)
	{
		PreShowSheet ();
	}
	else
	{
		PreHideSheet ();
	}
	return DefWindowProc ();
}

LRESULT CAtlPropertySheet::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s OnDestroy"), this, AtlTypeName(this));
#endif
	PreHideSheet ();
	PreDestroySheet ();
	return DefWindowProc ();
}

LRESULT CAtlPropertySheet::OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s OnClose [%u]"), this, AtlTypeName(this), mIsModal);
#endif
	if	(mIsModal)
	{
		return DefWindowProc ();
	}
	else
	{
		DestroyWindow ();
	}
	return 0;
}

LRESULT CAtlPropertySheet::OnSysCommand (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if	((wParam & 0xFFF0) == SC_CLOSE)
	{
		SendMessage (WM_CLOSE);
	}
	else
	{
		SaveConfig (IDCANCEL);
		return DefWindowProc ();
	}
	return 0;
}

LRESULT CAtlPropertySheet::OnChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnChanged ();
	return DefWindowProc ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAtlPropertySheet::OnOk (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s OnOk [%u]"), this, AtlTypeName(this), mPageChanged);
#endif
	PropSheet_Apply (m_hWnd);
	OnApplied ();
	SaveConfig (IDOK);
	if	(mIsModal)
	{
		return DefWindowProc ();
	}
	else
	{
		DestroyWindow ();
	}
	return 0;
}

LRESULT CAtlPropertySheet::OnCancel (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s OnCancel [%u]"), this, AtlTypeName(this), mPageChanged);
#endif
	SaveConfig (IDCANCEL);
	if	(mIsModal)
	{
		return DefWindowProc ();
	}
	else
	{
		DestroyWindow ();
	}
	return 0;
}

LRESULT CAtlPropertySheet::OnApply (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
#ifdef	_DEBUG_PROPSHEET
	LogMessage (_DEBUG_PROPSHEET, _T("[%p] %s OnApply [%u]"), this, AtlTypeName(this), mPageChanged);
#endif
	LRESULT lResult = DefWindowProc ();
	OnApplied ();
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CAtlPropertySheet::LoadConfig ()
{
	CRect						lWinRect;
	HMONITOR					lMonitor;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	GetWindowRect (&lWinRect);
	if	(
			(
				(lMonitor = MonitorFromPoint (lWinRect.TopLeft(), MONITOR_DEFAULTTONEAREST))
			||	(lMonitor = MonitorFromPoint (lWinRect.TopLeft(), MONITOR_DEFAULTTOPRIMARY))
			)
		&&	(GetMonitorInfo (lMonitor, &lMonitorInfo))
		)
	{
		if	(lWinRect.right > lMonitorInfo.rcWork.right)
		{
			lWinRect.OffsetRect (lMonitorInfo.rcWork.right - lWinRect.right, 0);
		}
		if	(lWinRect.left < lMonitorInfo.rcWork.left)
		{
			lWinRect.OffsetRect (lMonitorInfo.rcWork.left - lWinRect.left, 0);
		}
		if	(lWinRect.bottom > lMonitorInfo.rcWork.bottom)
		{
			lWinRect.OffsetRect (0, lMonitorInfo.rcWork.bottom - lWinRect.bottom);
		}
		if	(lWinRect.top < lMonitorInfo.rcWork.top)
		{
			lWinRect.OffsetRect (0, lMonitorInfo.rcWork.top - lWinRect.top);
		}
	}
	MoveWindow (&lWinRect);
}

void CAtlPropertySheet::SaveConfig (int pSheetResult)
{
}

void CAtlPropertySheet::OnChanged ()
{
	mPageChanged = true;
}

void CAtlPropertySheet::OnApplied ()
{
	mPageChanged = false;
}
