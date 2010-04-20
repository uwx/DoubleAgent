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
#include "DaGlobalConfig.h"
#include "DaCoreRes.h"
#include "PropSheetCharSel.h"
#include "PropPageCharSel.h"
#include "Registry.h"

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

CPropSheetCharSel::CPropSheetCharSel ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CPropSheetCharSel::CPropSheetCharSel (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

CPropSheetCharSel::~CPropSheetCharSel ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CPropSheetCharSel::~CPropSheetCharSel (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeletePropSheetCharSel (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

CPropSheetCharSel * CPropSheetCharSel::CreateInstance (HWND pParentWnd, LPCTSTR pClientMutexName)
{
	CComObject<CPropSheetCharSel> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<CPropSheetCharSel>::CreateInstance (&lInstance))))
	{
		lInstance->Construct (IDS_PROPSHEET_CHARSEL, pParentWnd);
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
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
					CoDisconnectObject (this, 0);
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

void CPropSheetCharSel::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CPropSheetCharSel::OnClientEnded"), this, m_dwRef);
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

bool CPropSheetCharSel::PreCreateSheet (bool pModal)
{
	CAtlPropertyPage *	lPage;

	mPages.DeleteAll ();
	if	(lPage = CPropPageCharSel::CreateInstance())
	{
		mPages.Add (lPage);
	}

	return CAtlPropertySheet::PreCreateSheet (pModal);
}

void CPropSheetCharSel::PreShowSheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CPropSheetCharSel::PreShowSheet"), this, m_dwRef);
	}
#endif
	AddRef ();
	CAtlPropertySheet::PreShowSheet ();
}

void CPropSheetCharSel::OnFinalMessage (HWND pWnd)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CPropSheetCharSel::OnFinalMessage"), this, m_dwRef);
	}
#endif
	Release ();
}

/////////////////////////////////////////////////////////////////////////////

void CPropSheetCharSel::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect		lWinRect;
	CPoint		lWinPos;

	GetWindowRect (&lWinRect);
	lWinPos.x = (long)CRegDWord (lRegKey, sProfileWinAsstPropSheetX, true, lWinRect.left).Value ();
	lWinPos.y = (long)CRegDWord (lRegKey, sProfileWinAsstPropSheetY, true, lWinRect.top).Value ();

	if	(lWinPos != lWinRect.TopLeft())
	{
		lWinRect.OffsetRect (lWinPos - lWinRect.TopLeft());
		MoveWindow (lWinRect);
	}
	CAtlPropertySheet::LoadConfig ();
}

void CPropSheetCharSel::SaveConfig (int pSheetResult)
{
	if	(pSheetResult > 0)
	{
		CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
		CRect		lWinRect;

		GetWindowRect (&lWinRect);
		CRegDWord (lRegKey, sProfileWinAsstPropSheetX, true).SetValue (lWinRect.left).Update ();
		CRegDWord (lRegKey, sProfileWinAsstPropSheetY, true).SetValue (lWinRect.top).Update ();
	}
	CAtlPropertySheet::SaveConfig (pSheetResult);
}
