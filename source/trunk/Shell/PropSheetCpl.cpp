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
#include "PropSheetCpl.h"
#include "PropPageCharSel.h"
#include "PropPageOutput.h"
#include "PropPageSpeech.h"
#include "PropPageCopyright.h"
#include "PropPageRegistry.h"
#include "PropPageLogging.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance"),LogDetails,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPropSheetCpl, CPropSheetBase)

BEGIN_MESSAGE_MAP(CPropSheetCpl, CPropSheetBase)
	//{{AFX_MSG_MAP(CPropSheetCpl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

LPCTSTR	CPropSheetCpl::mPageNameCharSel		= _T("pageCharacter");
LPCTSTR	CPropSheetCpl::mPageNameOutput		= _T("pageOutput");
LPCTSTR	CPropSheetCpl::mPageNameSpeech		= _T("pageSpeech");
LPCTSTR	CPropSheetCpl::mPageNameRegistry	= _T("pageRegistry");

static LPCTSTR	sProfilePropertyPage		= _T("ControlPanelPage");
static LPCTSTR	sProfilePropertySheetX		= _T("ControlPanelX");
static LPCTSTR	sProfilePropertySheetY		= _T("ControlPanelY");
static LPCTSTR	sProfileLogControl			= _T("LogControl");

/////////////////////////////////////////////////////////////////////////////

CPropSheetCpl::CPropSheetCpl(CWnd* pParentWnd)
:	CPropSheetBase(IDS_CPL_NAME, pParentWnd)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCpl::CPropSheetCpl"), this, m_dwRef);
	}
#endif
}

CPropSheetCpl::~CPropSheetCpl()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCpl::~CPropSheetCpl"), this, m_dwRef);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetCpl::SetModalParent (CWnd * pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

bool CPropSheetCpl::InitPages (bool pElevatedOnly)
{
	bool						lRet = false;
	CPropertyPage *				lPage;
	tPtr <CPropPageRegistry>	lRegistryPage;

	if	(!pElevatedOnly)
	{
		if	(lPage = (CPropPageCharSel *)CPropPageCharSel::CreateObject())
		{
			mPages.Add (lPage);
			lRet = true;
		}
		if	(lPage = (CPropPageOutput *)CPropPageOutput::CreateObject())
		{
			mPages.Add (lPage);
			lRet = true;
		}
		if	(lPage = (CPropPageSpeech *)CPropPageSpeech::CreateObject())
		{
			mPages.Add (lPage);
			lRet = true;
		}
		if	(lPage = (CPropPageCopyright *)CPropPageCopyright::CreateObject())
		{
			mPages.Add (lPage);
			lRet = true;
		}
	}

	if	(
			(lRegistryPage = new CPropPageRegistry)
		&&	(
				(!pElevatedOnly)
			||	(lRegistryPage->PrepareElevated (m_pParentWnd->GetSafeHwnd()))
			)
		)
	{
		mPages.Add (lRegistryPage.Detach());
		lRet = true;
	}

	if	(
			(!pElevatedOnly)
#ifndef	_DEBUG
		&&	(CRegDWord (CRegKey (HKEY_LOCAL_MACHINE, gProfileKeyDa, true), sProfileLogControl).Value() != 0)
#endif
		&&	(lPage = new CPropPageLogging)
		)
	{
		mPages.Add (lPage);
		lRet = true;
	}

	if	(mPages.GetSize () > 1)
	{
		CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

		mPsh.nStartPage = max ((long)CRegDWord (lRegKey, sProfilePropertyPage, false, mPsh.nStartPage).Value(), 0);
#ifdef	_DEBUG_NOT
		LogMessage (LogDebug, _T("CPropSheetCpl Init StartPage [%d]"), mPsh.nStartPage);
#endif
	}
	return lRet;
}

void CPropSheetCpl::SetStartPage (LPCTSTR pPageName)
{
	CString			lPageName (pPageName);
	CPropertyPage *	lPage;
	int				lNdx;

	if	(!lPageName.IsEmpty ())
	{
		for	(lNdx = 0; lPage = mPages (lNdx); lNdx++)
		{
			if	(
					(lPageName.CompareNoCase (mPageNameCharSel) == 0)
				&&	(lPage->IsKindOf (RUNTIME_CLASS (CPropPageCharSel)))
				)
			{
				mPsh.nStartPage = lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameOutput) == 0)
				&&	(lPage->IsKindOf (RUNTIME_CLASS (CPropPageOutput)))
				)
			{
				mPsh.nStartPage = lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameSpeech) == 0)
				&&	(lPage->IsKindOf (RUNTIME_CLASS (CPropPageSpeech)))
				)
			{
				mPsh.nStartPage = lNdx;
				break;
			}
			else
			if	(
					(lPageName.CompareNoCase (mPageNameRegistry) == 0)
				&&	(lPage->IsKindOf (RUNTIME_CLASS (CPropPageRegistry)))
				)
			{
				mPsh.nStartPage = lNdx;
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
	CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect	lWinRect;
	CPoint	lWinPos;

	GetWindowRect (&lWinRect);
	lWinPos.x = (long)CRegDWord (lRegKey, sProfilePropertySheetX, true, lWinRect.left).Value ();
	lWinPos.y = (long)CRegDWord (lRegKey, sProfilePropertySheetY, true, lWinRect.top).Value ();

	if	(lWinPos != lWinRect.TopLeft())
	{
		lWinRect.OffsetRect (lWinPos - lWinRect.TopLeft());
		MoveWindow (lWinRect);
	}
	CPropSheetBase::LoadConfig ();
}

void CPropSheetCpl::SaveConfig (int pSheetResult)
{
	if	(pSheetResult > 0)
	{
		CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		CRegDWord (lRegKey, sProfilePropertySheetX, true).SetValue (lWinRect.left).Update ();
		CRegDWord (lRegKey, sProfilePropertySheetY, true).SetValue (lWinRect.top).Update ();
		if	(mPages.GetSize() > 1)
		{
			CRegDWord (lRegKey, sProfilePropertyPage, true).SetValue (mPsh.nStartPage=max(PropSheet_HwndToIndex(m_hWnd,PropSheet_GetCurrentPageHwnd(m_hWnd)),0)).Update ();
#ifdef	_DEBUG_NOT
			LogMessage (LogDebug, _T("CPropSheetCpl Save StartPage [%d]"), mPsh.nStartPage);
#endif
		}
	}
	CPropSheetBase::SaveConfig (pSheetResult);
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetCpl::OnApplied ()
{
	if	(mPageChanged)
	{
		try
		{
			DWORD	lTargets = BSM_APPLICATIONS;
			BroadcastSystemMessage (BSF_FORCEIFHUNG|BSF_POSTMESSAGE, &lTargets, DA_BROADCAST_OPTIONS_CHANGED, 0, 0);
		}
		catch AnyExceptionDebug
	}
	CPropSheetBase::OnApplied ();
}
