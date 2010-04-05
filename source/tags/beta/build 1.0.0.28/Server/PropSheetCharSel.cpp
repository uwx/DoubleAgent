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
#include "DaGlobalConfig.h"
#include "PropSheetCharSel.h"
#include "PropPageCharSel.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileWinAsstPropSheetX	= _T("WinAsstPropSheetX");
static LPCTSTR	sProfileWinAsstPropSheetY	= _T("WinAsstPropSheetY");

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPropSheetCharSel, CPropSheetBase)

BEGIN_INTERFACE_MAP(CPropSheetCharSel, CPropSheetBase)
	INTERFACE_PART(CPropSheetCharSel, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CPropSheetCharSel, __uuidof(IDispatch), Dispatch)
END_INTERFACE_MAP()

BEGIN_MESSAGE_MAP(CPropSheetCharSel, CPropSheetBase)
	//{{AFX_MSG_MAP(CPropSheetCharSel)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CPropSheetCharSel::CPropSheetCharSel (CWnd* pParentWnd, LPCTSTR pClientMutexName)
:	CPropSheetBase(IDS_PROPSHEET_CHARSEL, pParentWnd)
{
	CPropertyPage *	lPage;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCharSel::CPropSheetCharSel (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	if	(lPage = (CPropPageCharSel *)CPropPageCharSel::CreateObject())
	{
		mPages.Add (lPage);
	}

	EnableAutomation();
	EnableAggregation();
	ManageObjectLifetime (this, pClientMutexName);
}

CPropSheetCharSel::~CPropSheetCharSel ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCharSel::~CPropSheetCharSel (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	try
	{
		TheServerApp->OnDeletePropSheetCharSel (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
	AfxOleUnlockApp();
}

void CPropSheetCharSel::Terminate (bool pFinal, bool pAbandonned)
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
					ExternalDisconnect ();
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

void CPropSheetCharSel::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCharSel::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

void CPropSheetCharSel::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCharSel::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CPropSheetCharSel::OnInitDialog()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCharSel::OnInitDialog"), this, m_dwRef);
	}
#endif
	ExternalAddRef ();
	return CPropSheetBase::OnInitDialog();
}

void CPropSheetCharSel::OnDestroy()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CPropSheetCharSel::OnDestroy"), this, m_dwRef);
	}
#endif
	CPropSheetBase::OnDestroy();
	ExternalRelease ();
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetCharSel::LoadConfig ()
{
	CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect	lWinRect;
	CPoint	lWinPos;

	GetWindowRect (&lWinRect);
	lWinPos.x = (long)CRegDWord (lRegKey, sProfileWinAsstPropSheetX, true, lWinRect.left).Value ();
	lWinPos.y = (long)CRegDWord (lRegKey, sProfileWinAsstPropSheetY, true, lWinRect.top).Value ();

	if	(lWinPos != lWinRect.TopLeft())
	{
		lWinRect.OffsetRect (lWinPos - lWinRect.TopLeft());
		MoveWindow (lWinRect);
	}
	CPropSheetBase::LoadConfig ();
}

void CPropSheetCharSel::SaveConfig (int pSheetResult)
{
	if	(pSheetResult > 0)
	{
		CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		CRegDWord (lRegKey, sProfileWinAsstPropSheetX, true).SetValue (lWinRect.left).Update ();
		CRegDWord (lRegKey, sProfileWinAsstPropSheetY, true).SetValue (lWinRect.top).Update ();
	}
	CPropSheetBase::SaveConfig (pSheetResult);
}
