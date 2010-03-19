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
#include "DaServer.h"
#include "DaAgentNotify.h"
#include "PropSheetOptions.h"
#include "PropPageOutput.h"
#include "PropPageSpeech.h"
#include "PropPageCopyright.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfilePropertySheetX		= _T("PropertySheetX");
static LPCTSTR	sProfilePropertySheetY		= _T("PropertySheetY");
static LPCTSTR	sProfilePropertySheetWidth	= _T("PropertySheetWidth");
static LPCTSTR	sProfilePropertySheetHeight = _T("PropertySheetHeight");
static LPCTSTR	sProfilePropertySheetPage	= _T("PropertySheetPage");

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPropSheetOptions, CPropSheetBase)

BEGIN_MESSAGE_MAP(CPropSheetOptions, CPropSheetBase)
	//{{AFX_MSG_MAP(CPropSheetOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropSheetOptions::CPropSheetOptions (CWnd* pParentWnd)
:	CPropSheetBase (IDS_PROPSHEET_OPTIONS, pParentWnd)
{
	CPropertyPage *	lPage;
	CRegKeyEx		lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

	if	(lPage = (CPropPageOutput *)CPropPageOutput::CreateObject())
	{
		mPages.Add (lPage);
	}
	if	(lPage = (CPropPageSpeech *)CPropPageSpeech::CreateObject())
	{
		mPages.Add (lPage);
	}
	if	(lPage = (CPropPageCopyright *)CPropPageCopyright::CreateObject())
	{
		mPages.Add (lPage);
	}
}

CPropSheetOptions::~CPropSheetOptions()
{
}

/////////////////////////////////////////////////////////////////////////////

BOOL CPropSheetOptions::OnInitDialog()
{
	CDaSpeechInputConfig::RegisterHotKey (false);
	return CPropSheetBase::OnInitDialog();
}

void CPropSheetOptions::OnDestroy()
{
	CPropSheetBase::OnDestroy();
	CDaSpeechInputConfig::RegisterHotKey (true);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CPropSheetOptions::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect		lWinRect;
	CPoint		lWinPos;

	GetWindowRect (&lWinRect);
	lWinPos.x = (long)(short)CRegDWord (lRegKey, sProfilePropertySheetX, true, lWinRect.left).Value ();
	lWinPos.y = (long)(short)CRegDWord (lRegKey, sProfilePropertySheetY, true, lWinRect.top).Value ();

	if	(lWinPos != lWinRect.TopLeft())
	{
		lWinRect.OffsetRect (lWinPos - lWinRect.TopLeft());
		MoveWindow (lWinRect);
	}
	mPsh.nStartPage = max((long)CRegDWord (lRegKey, sProfilePropertySheetPage, false, mPsh.nStartPage).Value(),0);
#ifdef	_DEBUG_NOT
	LogMessage (LogDebug, _T("CPropSheetOptions Init StartPage [%d]"), mPsh.nStartPage);
#endif

	CPropSheetBase::LoadConfig ();
}

void CPropSheetOptions::SaveConfig(int pSheetResult)
{
	if	(pSheetResult > 0)
	{
		CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
		CRect		lWinRect;

		GetWindowRect (&lWinRect);
		CRegDWord (lRegKey, sProfilePropertySheetX, true).SetValue (lWinRect.left).Update ();
		CRegDWord (lRegKey, sProfilePropertySheetY, true).SetValue (lWinRect.top).Update ();
		CRegDWord (lRegKey, sProfilePropertySheetWidth, true).SetValue (lWinRect.Width()).Update ();
		CRegDWord (lRegKey, sProfilePropertySheetHeight, true).SetValue (lWinRect.Height()).Update ();
		CRegDWord (lRegKey, sProfilePropertySheetPage, true).SetValue (mPsh.nStartPage=max(PropSheet_HwndToIndex(m_hWnd,PropSheet_GetCurrentPageHwnd(m_hWnd)),0)).Update ();
#ifdef	_DEBUG_NOT
		LogMessage (LogDebug, _T("CPropSheetOptions Save StartPage [%d]"), mPsh.nStartPage);
#endif
	}
	CPropSheetBase::SaveConfig (pSheetResult);
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetOptions::OnApplied ()
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
