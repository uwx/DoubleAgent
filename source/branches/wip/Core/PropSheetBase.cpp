/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include <AfxPriv.h>
#include "PropSheetBase.h"
#include "Localize.h"
#include "DebugStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPropSheetBase, CDialog)

BEGIN_MESSAGE_MAP(CPropSheetBase, CDialog)
	//{{AFX_MSG_MAP(CPropSheetBase)
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_COMMAND(IDOK,OnOK)
	ON_COMMAND(IDCANCEL,OnCancel)
	ON_MESSAGE(PSM_CHANGED, OnChange)
	ON_COMMAND(ID_APPLY_NOW, OnApply)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropSheetBase::CPropSheetBase(UINT pCaptionId, CWnd* pParentWnd)
:	CDialog ((UINT)0, pParentWnd),
	mPageChanged (false)
{
	mPsh.dwSize = sizeof(mPsh);
	mPsh.hwndParent = m_pParentWnd->GetSafeHwnd();
	mPsh.pszCaption = (LPCTSTR) (mCaption = CLocalize::LoadString (pCaptionId));
}

CPropSheetBase::~CPropSheetBase()
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CPropSheetBase::Create ()
{
	bool	lRet = false;
	long	lNdx;
	HWND	lWnd;

	mPsp = new PROPSHEETPAGE_MFC_VER [mPages.GetSize()];

	for	(lNdx = 0; lNdx <= mPages.GetUpperBound(); lNdx++)
	{
		mPsp [lNdx] = mPages [lNdx]->m_psp;
	}
	mPsh.nPages = (UINT)mPages.GetSize();
	mPsh.ppsp = (LPCPROPSHEETPAGE) mPsp.Ptr();
	mPsh.pfnCallback = PropSheetCallbackModeless;
	mPsh.dwFlags |= PSH_MODELESS | PSH_USECALLBACK | PSH_PROPSHEETPAGE | PSH_USEPSTARTPAGE;

	AfxHookWindowCreate (this);
	lWnd = (HWND)PropertySheet ((LPPROPSHEETHEADER)&mPsh);
	if	(AfxUnhookWindowCreate ())
	{
		ASSERT(lWnd == m_hWnd);
		lRet = true;
	}
	else
	{
		PostNcDestroy();
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CPropSheetBase::DoModal ()
{
	long	lNdx;
	HWND	lWnd;

	mPsp = new PROPSHEETPAGE_MFC_VER [mPages.GetSize()];

	for	(lNdx = 0; lNdx <= mPages.GetUpperBound(); lNdx++)
	{
		mPsp [lNdx] = mPages [lNdx]->m_psp;
	}
	mPsh.nPages = (UINT)mPages.GetSize();
	mPsh.ppsp = (LPCPROPSHEETPAGE) mPsp.Ptr();
	mPsh.pfnCallback = PropSheetCallbackModal;
	mPsh.dwFlags |= PSH_USECALLBACK | PSH_PROPSHEETPAGE | PSH_USEPSTARTPAGE;
	mPsh.hwndParent = m_pParentWnd->GetSafeHwnd();

	if	(AfxGetApp() != NULL)
	{
		AfxGetApp()->EnableModeless(FALSE);
	}
	m_nModalResult = 0;
	m_nFlags |= WF_MODALLOOP|WF_CONTINUEMODAL;

	AfxHookWindowCreate (this);
	lWnd = (HWND)PropertySheet ((LPPROPSHEETHEADER)&mPsh);
	AfxUnhookWindowCreate ();

	m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);
	if	(AfxGetApp() != NULL)
	{
		AfxGetApp()->EnableModeless(TRUE);
	}
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("%s Modal Result [%d]"), ObjClassName(this), m_nModalResult);
#endif
	return m_nModalResult;
}

/////////////////////////////////////////////////////////////////////////////

int CALLBACK CPropSheetBase::PropSheetCallbackModeless (HWND hwnd, UINT message, LPARAM lParam)
{
	switch (message)
	{
		case PSCB_PRECREATE:
		{
			LPDLGTEMPLATE lTemplate = (LPDLGTEMPLATE)lParam;
			lTemplate->style &= ~WS_VISIBLE;
			lTemplate->style |= DS_SETFONT;
			lTemplate->dwExtendedStyle &= ~WS_EX_APPWINDOW;
		}	break;
	}
	return 0;
}

int CALLBACK CPropSheetBase::PropSheetCallbackModal (HWND hwnd, UINT message, LPARAM lParam)
{
	switch (message)
	{
		case PSCB_PRECREATE:
		{
			LPDLGTEMPLATE lTemplate = (LPDLGTEMPLATE)lParam;
			lTemplate->style |= WS_VISIBLE;
			lTemplate->style |= DS_SETFONT;
		}	break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropSheetBase::PreTranslateMessage(MSG* pMsg)
{
	if	(CWnd::PreTranslateMessage(pMsg))
	{
		return TRUE;
	}
	if	(PropSheet_IsDialogMessage (m_hWnd, pMsg))
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CPropSheetBase::HandleInitDialog (WPARAM, LPARAM)
{
	LRESULT lResult = Default ();
	HFONT	lFont;

	OnInitDialog ();
	LoadConfig ();

	PropSheet_RecalcPageSizes (m_hWnd);
	PropSheet_SetCurSel (m_hWnd, NULL, mPsh.nStartPage);

	if	(lFont = (HFONT)::SendMessage (PropSheet_GetCurrentPageHwnd (m_hWnd), WM_GETFONT, 0, 0))
	{
		::SendMessage (PropSheet_GetTabControl(m_hWnd), WM_SETFONT, (WPARAM)lFont, TRUE);
	}
	if	(mPsh.dwFlags & PSH_HASHELP)
	{
		ModifyStyleEx (0, WS_EX_CONTEXTHELP);
	}
	else
	{
		ModifyStyleEx (WS_EX_CONTEXTHELP, 0);
	}
	return lResult;
}

BOOL CPropSheetBase::OnInitDialog()
{
	return TRUE; // Don't do CDialog default - it was handled above
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetBase::OnSysCommand(UINT nID, LPARAM)
{
	if	((nID & 0xFFF0) == SC_CLOSE)
	{
		SendMessage (WM_CLOSE);
	}
	else
	{
		SaveConfig (IDCANCEL);
		Default ();
	}
}

void CPropSheetBase::OnClose()
{
	if	(m_nFlags & WF_MODALLOOP)
	{
		Default ();
	}
	else
	{
		DestroyWindow ();
	}
}

void CPropSheetBase::OnDestroy()
{
	SaveConfig (0);
	CWnd::OnDestroy ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropSheetBase::OnOK()
{
	PropSheet_Apply (m_hWnd);
	OnApplied ();
	SaveConfig (IDOK);
	if	(m_nFlags & WF_MODALLOOP)
	{
		Default ();
	}
	else
	{
		DestroyWindow ();
	}
}

void CPropSheetBase::OnCancel()
{
	SaveConfig (IDCANCEL);
	if	(m_nFlags & WF_MODALLOOP)
	{
		Default ();
	}
	else
	{
		DestroyWindow ();
	}
}

void CPropSheetBase::OnApply()
{
	Default ();
	OnApplied ();
}

LRESULT CPropSheetBase::OnChange(WPARAM wParam, LPARAM lParam)
{
	OnChanged ();
	return Default ();
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetBase::LoadConfig ()
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

void CPropSheetBase::SaveConfig (int pSheetResult)
{
}

void CPropSheetBase::OnChanged ()
{
	mPageChanged = true;
}

void CPropSheetBase::OnApplied ()
{
	mPageChanged = false;
}
