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
#include "AgentIconMaker.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_DEBUG_CALLS	(GetProfileDebugInt(_T("DebugIconCalls"),LogDetails,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_STATE	(GetProfileDebugInt(_T("DebugIconState"),LogVerbose,true)&0xFFFF|LogTimeMs)
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

bool CAgentNotifyIcon::ShowState (HWND pOwnerWnd, class CAgentFile * pAgentFile)
{
	bool	lRet = false;

#ifdef	_DEBUG_STATE
	if	(LogIsActive (_DEBUG_STATE))
	{
		LogMessage (_DEBUG_STATE, _T("[%d] ShowState IsWindowVisible [%u] ShowIcon [%u] GenerateIcon [%u]"), mCharID, IsWindowVisible (pOwnerWnd), mAgentIconData.mShowIcon, mAgentIconData.mGenerateIcon);
	}
#endif
	if	(mAgentIconData.mShowIcon)
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

			if	(!mAgentIconData.mTip.IsEmpty ())
			{
				_tcsncpy (mNotifyIconData.szTip, mAgentIconData.mTip, sizeof(mNotifyIconData.szTip)/sizeof(TCHAR));
				mNotifyIconData.uFlags |= NIF_TIP;
			}

			if	(pAgentFile)
			{
				if	(mAgentIconData.mGenerateIcon)
				{
					if	(!mGeneratedIcon)
					{
						CAgentIconMaker	lIconMaker;
						const CRect *	lIconClip = mAgentIconData.mGenerateIconClip.IsRectEmpty() ? NULL : &mAgentIconData.mGenerateIconClip;

						mGeneratedIcon.Attach (lIconMaker.MakeIcon (pAgentFile, CSize (GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON)), lIconClip));
					}
					mNotifyIconData.hIcon = mGeneratedIcon;
				}
				if	(!mNotifyIconData.hIcon)
				{
					mNotifyIconData.hIcon = pAgentFile->GetIcon();
				}
			}

#ifdef	_DEBUG_CALLS
			if	(LogIsActive (_DEBUG_CALLS))
			{
				LogMessage (_DEBUG_CALLS, _T("[%d] AddIcon [%s] [%s] [%p]"), mCharID, (CString)CGuidStr(mNotifyIconData.guidItem), mAgentIconData.mTip, mNotifyIconData.hIcon);
			}
#endif
			if	(
					(mNotifyIconData.hIcon)
				&&	(Shell_NotifyIcon (NIM_ADD, &mNotifyIconData))
				)
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

bool CAgentNotifyIcon::SetIconTip (const CAgentIconData * pIconData, CAgentFile * pAgentFile, LANGID pLangID)
{
	bool				lRet = false;
	CAgentFileName *	lAgentFileName = NULL;

	if	(
			(pIconData)
		&&	(!pIconData->mTip.IsEmpty ())
		)
	{
		lRet = SetIconTip (pIconData->mTip);
	}
	else
	if	(pAgentFile)
	{
		lAgentFileName = pAgentFile->FindName (pLangID);
		if	(lAgentFileName)
		{
			lRet = SetIconTip (CString ((BSTR)lAgentFileName->mName));
		}
	}
	return lRet;
}

bool CAgentNotifyIcon::SetIconTip (LPCTSTR pIconTip)
{
	bool	lRet = false;

	mAgentIconData.mTip = pIconTip;
#ifdef	_DEBUG
	mAgentIconData.mTip.Format (_T("%s [%d]"), CString((LPCTSTR)mAgentIconData.mTip), mCharID);
#endif
	if	(SafeIsValid ())
	{
		_tcsncpy (mNotifyIconData.szTip, mAgentIconData.mTip, sizeof(mNotifyIconData.szTip)/sizeof(TCHAR));
		mNotifyIconData.uFlags = NIF_TIP;
		if	(!IsEqualGUID (mAgentIconData.mIdentity, GUID_NULL))
		{
			mNotifyIconData.uFlags |= NIF_GUID;
		}
#ifdef	_DEBUG_CALLS
		if	(LogIsActive (_DEBUG_CALLS))
		{
			LogMessage (_DEBUG_CALLS, _T("[%d] SetIcon [%s] [%s]"), mCharID, (CString)CGuidStr(mNotifyIconData.guidItem), mAgentIconData.mTip);
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