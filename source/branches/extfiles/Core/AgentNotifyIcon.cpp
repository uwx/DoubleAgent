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
#include "DaCore.h"
#include "AgentNotifyIcon.h"
#include "AgentFile.h"
#ifdef	_DEBUG
#include "BitmapDebugger.h"
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_DEBUG_CALLS	LogNormal
#define	_DEBUG_STATE	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT	CAgentNotifyIcon::mNotifyIconMsg = RegisterWindowMessage (_T("1147E500-A208-11DE-ABF2-002421116FB2"));
const UINT	CAgentNotifyIcon::mTaskbarCreatedMsg = RegisterWindowMessage (_T("TaskbarCreated"));

/////////////////////////////////////////////////////////////////////////////

CAgentNotifyIcon::CAgentNotifyIcon ()
:	mCharID (0)
{
}

CAgentNotifyIcon::~CAgentNotifyIcon ()
{
	Remove ();
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentNotifyIcon::SafeIsValid () const
{
	if	(
			(this)
		&&	(IsWindow (mNotifyIconData.hWnd))
		&&	(mNotifyIconData.hIcon)
		)
	{
		return true;
	}
	return false;
}

bool CAgentNotifyIcon::SafeIsVisible () const
{
	if	(
			(this)
		&&	(IsWindow (mNotifyIconData.hWnd))
		&&	((mNotifyIconData.dwState & NIS_HIDDEN) == 0)
		)
	{
		return true;
	}
	return false;
}

long CAgentNotifyIcon::GetCharID () const
{
	return mCharID;
}

const CAgentIconData & CAgentNotifyIcon::GetSettings () const
{
	return mAgentIconData;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentNotifyIcon::Attach (long pCharID, const CAgentIconData * pIconData)
{
	bool	lRet = false;
	
	if	(mCharID != pCharID)
	{
		mCharID = pCharID;
		lRet = true;
	}
	if	(pIconData)
	{
		mAgentIconData = *pIconData;
	}
	return lRet;	
}

bool CAgentNotifyIcon::Detach (long pCharID)
{
	bool	lRet = false;
	
	if	(mCharID == pCharID)
	{
		mCharID = 0;
		lRet = true;
	}
	return lRet;	
}

bool CAgentNotifyIcon::Remove ()
{
	bool	lRet = false;
	
	if	(this)
	{
		try
		{
#ifdef	_DEBUG_CALLS
			if	(
					(mNotifyIconData.hWnd)
				&&	(LogIsActive (_DEBUG_CALLS))	
				)
			{
				LogMessage (_DEBUG_CALLS, _T("[%d] DelIcon [%s]"), mCharID, (CString)CGuidStr(mNotifyIconData.guidItem));
			}
#endif
			if	(!IsEqualGUID (mAgentIconData.mIdentity, GUID_NULL))
			{
				mNotifyIconData.uFlags = NIF_GUID;
			}
			if	(
					(mNotifyIconData.hWnd)
				&&	(Shell_NotifyIcon (NIM_DELETE, &mNotifyIconData))
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionSilent
	
		mNotifyIconData.Clear ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentNotifyIcon::ShowState (HWND pOwnerWnd, class CAgentFile * pAgentFile, bool pActive)
{
	bool	lRet = false;
	
#ifdef	_DEBUG_STATE
	if	(LogIsActive (_DEBUG_STATE))	
	{
		LogMessage (_DEBUG_STATE, _T("[%d] ShowState Active [%u] IsWindowVisible [%u]"), mCharID, pActive, IsWindowVisible (pOwnerWnd));
		LogMessage (_DEBUG_STATE, _T("[%d]           ShowAlways [%u] ShowWhenActive [%u] ShowWhenVisible [%u] mShowWhenHidden [%u]"), mCharID, mAgentIconData.mShowAlways, mAgentIconData.mShowWhenActive, mAgentIconData.mShowWhenVisible, mAgentIconData.mShowWhenHidden);
	}
#endif		
	if	(
			(mAgentIconData.mShowAlways)
		||	(
				(mAgentIconData.mShowWhenActive)
			&&	(pActive)
			)
		||	(
				(mAgentIconData.mShowWhenVisible)
			&&	(::IsWindowVisible (pOwnerWnd))
			)
		||	(
				(mAgentIconData.mShowWhenHidden)
			&&	(!::IsWindowVisible (pOwnerWnd))
			)
		)
	{
		if	(
				(!SafeIsValid ())
			&&	(IsWindow (pOwnerWnd))
			)
		{
			mNotifyIconData.uVersion = NOTIFYICON_VERSION;
			Shell_NotifyIcon (NIM_SETVERSION, &mNotifyIconData);

			mNotifyIconData.Clear();
			mNotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_STATE;
			mNotifyIconData.hWnd = pOwnerWnd;
			mNotifyIconData.uCallbackMessage = mNotifyIconMsg;

			mNotifyIconData.guidItem = mAgentIconData.mIdentity;
			if	(!IsEqualGUID (mAgentIconData.mIdentity, GUID_NULL))
			{
				mNotifyIconData.uFlags |= NIF_GUID;
			}

			if	(!mAgentIconData.mName.IsEmpty ())
			{
				_tcsncpy (mNotifyIconData.szTip, mAgentIconData.mName, sizeof(mNotifyIconData.szTip)/sizeof(TCHAR));
				mNotifyIconData.uFlags |= NIF_TIP;
			}

			if	(pAgentFile)
			{
				mNotifyIconData.hIcon = pAgentFile->GetIcon();
			}
#ifdef	_DEBUG_CALLS
			if	(LogIsActive (_DEBUG_CALLS))	
			{
				LogMessage (_DEBUG_CALLS, _T("[%d] AddIcon [%s] [%s]"), mCharID, (CString)CGuidStr(mNotifyIconData.guidItem), mAgentIconData.mName);
			}
#endif					
			if	(Shell_NotifyIcon (NIM_ADD, &mNotifyIconData))
			{
				lRet = true;
			}
			else
			{
				mNotifyIconData.Clear ();
#ifdef	_DEBUG_CALLS
				if	(LogIsActive (_DEBUG_CALLS))	
				{
					LogMessage (_DEBUG_CALLS, _T("[%d] AddIcon failed"), mCharID);
				}
#endif					
			}
		}
	}
	else
	{
		lRet = Remove ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentNotifyIcon::SetIconName (const CAgentIconData * pIconData, CAgentFile * pAgentFile, LANGID pLangID)
{
	bool				lRet = false;
	CAgentFileName *	lAgentFileName = NULL;

	if	(
			(pIconData)
		&&	(!pIconData->mName.IsEmpty ())
		)
	{
		lRet = SetIconName (pIconData->mName);
	}
	else
	if	(pAgentFile)
	{
		lAgentFileName = pAgentFile->FindName (pLangID);
		if	(lAgentFileName)
		{
			lRet = SetIconName (CString ((BSTR)lAgentFileName->mName));
		}
	}
	return lRet;
}

bool CAgentNotifyIcon::SetIconName (LPCTSTR pName)
{
	bool	lRet = false;

	mAgentIconData.mName = pName;
#ifdef	_DEBUG
	mAgentIconData.mName.Format (_T("%s [%d]"), CString((LPCTSTR)mAgentIconData.mName), mCharID);
#endif
	if	(SafeIsValid ())
	{
		_tcsncpy (mNotifyIconData.szTip, mAgentIconData.mName, sizeof(mNotifyIconData.szTip)/sizeof(TCHAR));
		mNotifyIconData.uFlags = NIF_TIP;
		if	(!IsEqualGUID (mAgentIconData.mIdentity, GUID_NULL))
		{
			mNotifyIconData.uFlags |= NIF_GUID;
		}
#ifdef	_DEBUG_CALLS
		if	(LogIsActive (_DEBUG_CALLS))	
		{
			LogMessage (_DEBUG_CALLS, _T("[%d] SetIcon [%s] [%s]"), mCharID, (CString)CGuidStr(mNotifyIconData.guidItem), mAgentIconData.mName);
		}
#endif					
		Shell_NotifyIcon (NIM_MODIFY, &mNotifyIconData);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

UINT CAgentNotifyIcon::OnNotifyIcon(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UINT	lRet = 0;
	
	if	(wParam == mNotifyIconData.uID)
	{
		switch (lParam)
		{
			case NIN_KEYSELECT:
			case NIN_SELECT:
			{
				CPoint	lCursorPos;

				GetCursorPos (&lCursorPos);
				::PostMessage (hWnd, WM_CONTEXTMENU, (WPARAM)hWnd, MAKELPARAM(lCursorPos.x, lCursorPos.y));
			}	break;
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONUP:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
			{
				lRet = (UINT)lParam;
			}	break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentNotifyIcon::OnTaskbarCreated(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
//
//	In case the desktop is recreated for some reason we want to recreate the notification icon.
//
	Remove ();
}