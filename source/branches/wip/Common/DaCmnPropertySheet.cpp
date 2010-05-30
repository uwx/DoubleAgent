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
#include "DaCmnPropertySheet.h"
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnPropertySheet::CDaCmnPropertySheet (_AtlPropSheetOwner * pOwner, class CListeningGlobal & pListeningGlobal)
:	CPropSheetOptions (pListeningGlobal),
	mOwner (pOwner)
{
}

CDaCmnPropertySheet::~CDaCmnPropertySheet ()
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HWND CDaCmnPropertySheet::GetPropSheetWnd (bool pCreate)
{
	if	(
			(!IsWindow ())
		&&	(pCreate)
		)
	{
		try
		{
			if	(Create (mOwner))
			{
				LoadConfig ();
			}
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnPropertySheet::GetPosition (long *Left, long *Top)
{
	HRESULT	lResult = S_OK;
	CRect	lWinRect (0,0,0,0);

	if	(IsWindow ())
	{
		GetWindowRect (&lWinRect);
	}
	else
	{
		LoadLayout (lWinRect);
	}
	if	(Left)
	{
		(*Left) = lWinRect.left;
	}
	if	(Top)
	{
		(*Top) = lWinRect.top;
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::SetPosition (long Left, long Top)
{
	HRESULT						lResult = S_OK;
	CRect						lWinRect (0,0,0,0);
	HMONITOR					lMonitor = NULL;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(IsWindow ())
	{
		GetWindowRect (&lWinRect);
	}
	else
	{
		LoadLayout (lWinRect);
	}

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

	SaveLayout (lWinRect);

	if	(IsWindow ())
	{
		MoveWindow (&lWinRect);
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnPropertySheet::GetSize (long *Width, long *Height)
{
	HRESULT	lResult = S_OK;
	CRect	lWinRect (0,0,0,0);

	if	(IsWindow ())
	{
		GetWindowRect (&lWinRect);
	}
	if	(Width)
	{
		(*Width) = lWinRect.Width();
	}
	if	(Height)
	{
		(*Height) = lWinRect.Height();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnPropertySheet::put_Left (short Left)
{
	HRESULT	lResult;
	long	lTop = 0;

	if	(SUCCEEDED (lResult = GetPosition (NULL, &lTop)))
	{
		lResult = SetPosition (Left, lTop);
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Left (short *Left)
{
	long	lLeft = 0;
	HRESULT	lResult = GetPosition (&lLeft, NULL);

	if	(Left)
	{
		(*Left) = (short)lLeft;
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::put_Top (short Top)
{
	HRESULT	lResult;
	long	lLeft = 0;

	if	(SUCCEEDED (lResult = GetPosition (&lLeft, NULL)))
	{
		lResult = SetPosition (lLeft, Top);
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Top (short *Top)
{
	long	lTop = 0;
	HRESULT	lResult = GetPosition (NULL, &lTop);

	if	(Top)
	{
		(*Top) = (short)lTop;
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Height (short *Height)
{
	long	lHeight = 0;
	HRESULT	lResult = GetSize (NULL, &lHeight);

	if	(Height)
	{
		(*Height) = (short)lHeight;
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Width (short *Width)
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

HRESULT CDaCmnPropertySheet::put_Visible (VARIANT_BOOL Visible)
{
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
		if	(IsWindow ())
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
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Visible (VARIANT_BOOL *Visible)
{
	HRESULT	lResult = S_FALSE;

	if	(IsWindow ())
	{
		lResult = IsWindowVisible () ? S_OK : S_FALSE;
	}
	if	(Visible)
	{
		(*Visible) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnPropertySheet::put_Page (BSTR Page)
{
	HRESULT				lResult = S_OK;
	CString				lPageName (Page);
	int					lPageNdx = -1;
	static CAtlString	lPageNameOutput (PropertySheet_PageName_Output);
	static CAtlString	lPageNameSpeech (PropertySheet_PageName_Speech);
	static CAtlString	lPageNameCharacter (PropertySheet_PageName_Character);
	static CAtlString	lPageNameCopyright (PropertySheet_PageName_Copyright);

	if	(lPageName.CompareNoCase (lPageNameOutput) == 0)
	{
		lPageNdx = 0;
	}
	else
	if	(lPageName.CompareNoCase (lPageNameSpeech) == 0)
	{
		lPageNdx = 1;
	}
	else
	if	(lPageName.CompareNoCase (lPageNameCharacter) == 0)
	{
		lPageNdx = 2;
	}
	else
	if	(lPageName.CompareNoCase (lPageNameCopyright) == 0)
	{
		lPageNdx = 3;
	}
	else
	{
		lResult = E_INVALIDARG;
	}

	if	(SUCCEEDED (lResult))
	{
		SaveStartPage (lPageNdx);
		if	(IsWindow ())
		{
			PropSheet_SetCurSel (m_hWnd, NULL, lPageNdx);
		}
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Page (BSTR *Page)
{
	HRESULT	lResult = S_OK;
	int		lPage = -1;

	if	(!Page)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Page) = NULL;

		if	(IsWindow ())
		{
			lPage = PropSheet_HwndToIndex (m_hWnd, PropSheet_GetCurrentPageHwnd (m_hWnd));
		}
		else
		{
			lPage = LoadStartPage ();
		}

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
	return lResult;
}
