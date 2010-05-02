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

CDaCmnPropertySheet::CDaCmnPropertySheet (_AtlPropSheetOwner * pOwner)
:	mOwner (pOwner),
	mLoadingTemp (false)
{
}

CDaCmnPropertySheet::~CDaCmnPropertySheet ()
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HWND CDaCmnPropertySheet::GetPropSheetWnd (bool pCreate, bool pCreateTemp)
{
	if	(
			(!IsWindow ())
		&&	(pCreate)
		)
	{
		try
		{
			mLoadingTemp = pCreateTemp;
			if	(Create (mOwner))
			{
				LoadConfig ();
			}
			else
			{
				mLoadingTemp = false;
			}
		}
		catch AnyExceptionDebug
	}
	else
	{
		mLoadingTemp = false;
	}
	if	(IsWindow ())
	{
		mLoadedRect = new CRect;
		GetWindowRect (mLoadedRect);
		return m_hWnd;
	}
	return NULL;
}

void CDaCmnPropertySheet::DestroyTempWnd ()
{
	if	(
			(mLoadingTemp)
		&&	(IsWindow ())
		)
	{
		DestroyWindow ();
	}
	mLoadingTemp = false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnPropertySheet::GetPosition (long *Left, long *Top)
{
	HRESULT	lResult = S_OK;

	if	(GetPropSheetWnd (true, true))
	{
		DestroyTempWnd ();
	}
	else
	{
		lResult = E_FAIL;
	}
	if	(Left)
	{
		(*Left) = mLoadedRect->left;
	}
	if	(Top)
	{
		(*Top) = mLoadedRect->top;
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::SetPosition (long Left, long Top)
{
	HRESULT						lResult = S_OK;
	CRect						lWinRect (0,0,0,0);
	HMONITOR					lMonitor = NULL;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	if	(GetPropSheetWnd (true, false))
	{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnPropertySheet::GetSize (long *Width, long *Height)
{
	HRESULT	lResult = S_OK;

	if	(GetPropSheetWnd (true, true))
	{
		DestroyTempWnd ();
	}
	else
	{
		lResult = E_FAIL;
	}
	if	(Width)
	{
		(*Width) = mLoadedRect->Width();
	}
	if	(Height)
	{
		(*Height) = mLoadedRect->Height();
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
		if	(GetPropSheetWnd (true, false))
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
		if	(GetPropSheetWnd (false, false))
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

	if	(GetPropSheetWnd (false, false))
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
	static CAtlString	lPageNameOutput (PropertySheet_PageName_Output);
	static CAtlString	lPageNameSpeech (PropertySheet_PageName_Speech);
	static CAtlString	lPageNameCharacter (PropertySheet_PageName_Character);
	static CAtlString	lPageNameCopyright (PropertySheet_PageName_Copyright);

	if	(GetPropSheetWnd (true, true))
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

		DestroyTempWnd ();
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnPropertySheet::get_Page (BSTR *Page)
{
	HRESULT	lResult = S_OK;

	if	(GetPropSheetWnd (true, true))
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

		DestroyTempWnd ();
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}
