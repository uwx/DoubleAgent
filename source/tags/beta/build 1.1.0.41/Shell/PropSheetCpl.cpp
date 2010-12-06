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
#include "DaShell.h"
#include "DaGlobalConfig.h"
#include "PropSheetCpl.h"
#include "PropPageCharSel.h"
#include "PropPageOutput.h"
#include "PropPageSpeech.h"
#include "PropPageCopyright.h"
#include "PropPageRegistry.h"
#include "PropPageLogging.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance"),LogDetails,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

LPCTSTR	CPropSheetCpl::mPageNameCharSel		= _T("pageCharacter");
LPCTSTR	CPropSheetCpl::mPageNameOutput		= _T("pageOutput");
LPCTSTR	CPropSheetCpl::mPageNameSpeech		= _T("pageSpeech");
LPCTSTR	CPropSheetCpl::mPageNameCopyright	= _T("pageCopyright");
LPCTSTR	CPropSheetCpl::mPageNameRegistry	= _T("pageRegistry");

static LPCTSTR	sProfilePropertyPage		= _T("ControlPanelPage");
static LPCTSTR	sProfilePropertySheetX		= _T("ControlPanelX");
static LPCTSTR	sProfilePropertySheetY		= _T("ControlPanelY");
static LPCTSTR	sProfileLogControl			= _T("LogControl");

/////////////////////////////////////////////////////////////////////////////

CPropSheetCpl::CPropSheetCpl (HWND pParentWnd)
{
	Construct (IDS_CPL_NAME, pParentWnd);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CPropSheetCpl::CPropSheetCpl"), this);
	}
#endif
}

CPropSheetCpl::~CPropSheetCpl()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CPropSheetCpl::~CPropSheetCpl"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CPropSheetCpl::InitPages (bool pElevatedOnly)
{
	bool						lRet = false;
	CAtlPropertyPage *			lPage;
	tPtr <CPropPageRegistry>	lRegistryPage;

	mPages.DeleteAll ();

	if	(!pElevatedOnly)
	{
		if	(lPage = CPropPageOutput::CreateInstance())
		{
			mPages.Add (lPage);
			lRet = true;
		}
		if	(lPage = CPropPageSpeech::CreateInstance())
		{
			mPages.Add (lPage);
			lRet = true;
		}
		if	(lPage = CPropPageCharSel::CreateInstance())
		{
			mPages.Add (lPage);
			lRet = true;
		}
		if	(lPage = CPropPageCopyright::CreateInstance())
		{
			mPages.Add (lPage);
			lRet = true;
		}
	}

	if	(
			(lRegistryPage = new CPropPageRegistry)
		&&	(
				(!pElevatedOnly)
			||	(lRegistryPage->PrepareElevated (mPsh.hwndParent))
			)
		)
	{
		mPages.Add (lRegistryPage.Detach());
		lRet = true;
	}

	if	(
			(!pElevatedOnly)
#ifndef	_DEBUG
		&&	(CRegDWord (CRegKeyEx (HKEY_LOCAL_MACHINE, gProfileKeyDa, true), sProfileLogControl).Value() != 0)
#endif
		&&	(lPage = new CPropPageLogging)
		)
	{
		mPages.Add (lPage);
		lRet = true;
	}

	if	(mPages.GetCount() > 1)
	{
		CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

		mPsh.nStartPage = max ((long)CRegDWord (lRegKey, sProfilePropertyPage, false, mPsh.nStartPage).Value(), 0);
#ifdef	_DEBUG_NOT
		LogMessage (LogDebug, _T("CPropSheetCpl Init StartPage [%d]"), mPsh.nStartPage);
#endif
	}
	return lRet;
}

void CPropSheetCpl::SetStartPage (LPCTSTR pPageName)
{
	CAtlString			lPageName (pPageName);
	CAtlPropertyPage *	lPage;
	INT_PTR				lNdx;

	if	(!lPageName.IsEmpty ())
	{
		for	(lNdx = 0; lPage = mPages (lNdx); lNdx++)
		{
			if	(
					(lPageName.CompareNoCase (mPageNameOutput) == 0)
				&&	(dynamic_cast <CPropPageOutput *> (lPage))
				)
			{
				mPsh.nStartPage = (UINT)lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameSpeech) == 0)
				&&	(dynamic_cast <CPropPageSpeech *> (lPage))
				)
			{
				mPsh.nStartPage = (UINT)lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameCharSel) == 0)
				&&	(dynamic_cast <CPropPageCharSel *> (lPage))
				)
			{
				mPsh.nStartPage = (UINT)lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameCopyright) == 0)
				&&	(dynamic_cast <CPropPageCopyright *> (lPage))
				)
			{
				mPsh.nStartPage = (UINT)lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameRegistry) == 0)
				&&	(dynamic_cast <CPropPageRegistry *> (lPage))
				)
			{
				mPsh.nStartPage = (UINT)lNdx;
				break;
			}
		}
	}
#ifdef	_DEBUG_NOT
	LogMessage (LogDebug, _T("CPropSheetCpl Set StartPage [%d]"), mPsh.nStartPage);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropSheetCpl::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect		lWinRect;
	CPoint		lWinPos;

	GetWindowRect (&lWinRect);
	lWinPos.x = (long)CRegDWord (lRegKey, sProfilePropertySheetX, true, lWinRect.left).Value ();
	lWinPos.y = (long)CRegDWord (lRegKey, sProfilePropertySheetY, true, lWinRect.top).Value ();

	if	(lWinPos != lWinRect.TopLeft())
	{
		lWinRect.OffsetRect (lWinPos - lWinRect.TopLeft());
		MoveWindow (lWinRect);
	}
	CAtlPropertySheet::LoadConfig ();
}

void CPropSheetCpl::SaveConfig (int pSheetResult)
{
	if	(pSheetResult > 0)
	{
		CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
		CRect		lWinRect;

		GetWindowRect (&lWinRect);
		CRegDWord (lRegKey, sProfilePropertySheetX, true).SetValue (lWinRect.left).Update ();
		CRegDWord (lRegKey, sProfilePropertySheetY, true).SetValue (lWinRect.top).Update ();
		if	(mPages.GetCount() > 1)
		{
			CRegDWord (lRegKey, sProfilePropertyPage, true).SetValue (mPsh.nStartPage=max(PropSheet_HwndToIndex(m_hWnd,PropSheet_GetCurrentPageHwnd(m_hWnd)),0)).Update ();
#ifdef	_DEBUG_NOT
			LogMessage (LogDebug, _T("CPropSheetCpl Save StartPage [%d]"), mPsh.nStartPage);
#endif
		}
	}
	CAtlPropertySheet::SaveConfig (pSheetResult);
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetCpl::OnApplied ()
{
	if	(mPageChanged)
	{
		try
		{
			DWORD	lTargets = BSM_ALLCOMPONENTS;
			long	lResult;

			lResult = BroadcastSystemMessage (BSF_FORCEIFHUNG|BSF_POSTMESSAGE, &lTargets, DA_BROADCAST_OPTIONS_CHANGED, 0, 0);
#ifdef	_DEBUG
			LogMessage (LogNormal|LogTime, _T("DA_BROADCAST_OPTIONS_CHANGED [%d]"), lResult);
#endif
		}
		catch AnyExceptionDebug
	}
	CAtlPropertySheet::OnApplied ();
}
