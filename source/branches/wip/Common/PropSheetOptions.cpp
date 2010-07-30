/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "PropSheetOptions.h"
#include "PropPageOutput.h"
#include "PropPageSpeech.h"
#include "PropPageCopyright.h"
#include "PropPageCharSel.h"
#include "ListeningState.h"
#include "Registry.h"

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfilePropertySheetX		= _T("PropertySheetX");
static LPCTSTR	sProfilePropertySheetY		= _T("PropertySheetY");
static LPCTSTR	sProfilePropertySheetWidth	= _T("PropertySheetWidth");
static LPCTSTR	sProfilePropertySheetHeight = _T("PropertySheetHeight");
static LPCTSTR	sProfilePropertySheetPage	= _T("PropertySheetPage");

/////////////////////////////////////////////////////////////////////////////

CPropSheetOptions::CPropSheetOptions (CListeningGlobal & pListeningGlobal, HWND pParentWnd)
:	mListeningGlobal (pListeningGlobal)
{
	Construct (IDS_PROPSHEET_OPTIONS, pParentWnd);
}

CPropSheetOptions::~CPropSheetOptions()
{
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CPropSheetOptions::DoModal ()
{
	INT_PTR	lRet = -1;

	mListeningGlobal.Suspend ();
	try
	{
		lRet = CAtlPropertySheet::DoModal ();
	}
	catch AnyExceptionDebug
	mListeningGlobal.Resume ();

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CPropSheetOptions::PreCreateSheet (bool pModal)
{
	CAtlPropertyPage *	lPage;

	mPages.DeleteAll ();

	if	(lPage = CPropPageOutput::CreateInstance())
	{
		mPages.Add (lPage);
	}
	if	(lPage = CPropPageSpeech::CreateInstance())
	{
		mPages.Add (lPage);
	}
	if	(lPage = CPropPageCharSel::CreateInstance())
	{
		mPages.Add (lPage);
	}
	if	(lPage = CPropPageCopyright::CreateInstance())
	{
		mPages.Add (lPage);
	}

	return CAtlPropertySheet::PreCreateSheet (pModal);
}

void CPropSheetOptions::PreShowSheet ()
{
	mListeningGlobal.Suspend ();
	CAtlPropertySheet::PreShowSheet ();
}

void CPropSheetOptions::OnFinalMessage (HWND)
{
	mListeningGlobal.Resume ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropSheetOptions::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect		lWinRect;

	GetWindowRect (&lWinRect);
	if	(LoadLayout (lWinRect))
	{
		MoveWindow (lWinRect);
	}
	LoadStartPage ();
#ifdef	_DEBUG_NOT
	LogMessage (LogDebug, _T("CPropSheetOptions Init StartPage [%d]"), mPsh.nStartPage);
#endif

	CAtlPropertySheet::LoadConfig ();
}

bool CPropSheetOptions::LoadLayout (CRect & pWinRect)
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CPoint		lWinPos;

	lWinPos.x = (long)(short)CRegDWord (lRegKey, sProfilePropertySheetX, true, pWinRect.left).Value ();
	lWinPos.y = (long)(short)CRegDWord (lRegKey, sProfilePropertySheetY, true, pWinRect.top).Value ();

	if	(lWinPos != pWinRect.TopLeft())
	{
		pWinRect.OffsetRect (lWinPos - pWinRect.TopLeft());
		return true;
	}
	return false;
}

int CPropSheetOptions::LoadStartPage ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

	mPsh.nStartPage = max((long)CRegDWord (lRegKey, sProfilePropertySheetPage, false, mPsh.nStartPage).Value(),0);
	return (int) mPsh.nStartPage;
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetOptions::SaveConfig (int pSheetResult)
{
	if	(pSheetResult > 0)
	{
		CRect		lWinRect;

		GetWindowRect (&lWinRect);
		SaveLayout (lWinRect);
		SaveStartPage ((int)(mPsh.nStartPage=max(PropSheet_HwndToIndex(m_hWnd,PropSheet_GetCurrentPageHwnd(m_hWnd)),0)));
#ifdef	_DEBUG_NOT
		LogMessage (LogDebug, _T("CPropSheetOptions Save StartPage [%d]"), mPsh.nStartPage);
#endif
	}
	CAtlPropertySheet::SaveConfig (pSheetResult);
}

void CPropSheetOptions::SaveLayout (const CRect & pWinRect)
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

	CRegDWord (lRegKey, sProfilePropertySheetX, true).SetValue (pWinRect.left).Update ();
	CRegDWord (lRegKey, sProfilePropertySheetY, true).SetValue (pWinRect.top).Update ();
	CRegDWord (lRegKey, sProfilePropertySheetWidth, true).SetValue (pWinRect.Width()).Update ();
	CRegDWord (lRegKey, sProfilePropertySheetHeight, true).SetValue (pWinRect.Height()).Update ();
}

void CPropSheetOptions::SaveStartPage (int pStartPageNdx)
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

	CRegDWord (lRegKey, sProfilePropertySheetPage, true).SetValue (max(pStartPageNdx,0)).Update ();
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetOptions::OnApplied ()
{
	if	(mPageChanged)
	{
		try
		{
			DWORD	lTargets = BSM_APPLICATIONS;
			long	lResult;

			lResult = BroadcastSystemMessage (BSF_FORCEIFHUNG|/*BSF_POSTMESSAGE*/BSF_SENDNOTIFYMESSAGE, &lTargets, DA_BROADCAST_OPTIONS_CHANGED, 0, 0);
#ifdef	_DEBUG
			LogMessage (LogNormal, _T("DA_BROADCAST_OPTIONS_CHANGED [%d]"), lResult);
#endif
		}
		catch AnyExceptionDebug
	}
	CAtlPropertySheet::OnApplied ();
}
