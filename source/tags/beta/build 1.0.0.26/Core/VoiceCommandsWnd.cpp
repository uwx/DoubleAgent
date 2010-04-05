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
#include <AfxPriv.h>
#include "DaCore.h"
#include "DaGlobalConfig.h"
#include "VoiceCommandsWnd.h"
#include "Localize.h"
#include "Registry.h"
#include "StringMap.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sProfileWinPosSet		= _T("CommandsWindowLocationSet");
static LPCTSTR	sProfileWinTop			= _T("CommandsWindowTop");
static LPCTSTR	sProfileWinLeft			= _T("CommandsWindowLeft");
static LPCTSTR	sProfileWinWidth		= _T("CommandsWindowWidth");
static LPCTSTR	sProfileWinHeight		= _T("CommandsWindowHeight");
static LPCTSTR	sProfileGlobalExpanded	= _T("CommandsWindowExpandGlobal");

static const int	sGlobalItemHideWnd = 0;
static const int	sGlobalItemHideChar = 1;

#ifndef	TVS_EX_DOUBLEBUFFER
#define	TVS_EX_DOUBLEBUFFER	0x0004
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CVoiceCommandsWnd, CWnd)

CVoiceCommandsWnd::CVoiceCommandsWnd ()
:	mCharID (0),
	mLangID (LANG_USER_DEFAULT),
	mInitialRect (0,0,0,0),
	mGlobalRoot (NULL)
{
	memset (mGlobalItems, 0, sizeof (mGlobalItems));
}

CVoiceCommandsWnd::~CVoiceCommandsWnd ()
{
	if	(m_hWnd)
	{
		DestroyWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CVoiceCommandsWnd, CWnd)
	//{{AFX_MSG_MAP(CVoiceCommandsWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_ITEMEXPANDING, AFX_IDC_LISTBOX, OnItemExpanding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

bool CVoiceCommandsWnd::Create ()
{
	bool			lRet = false;
	CRect			lInitialRect (0,0,200,200);
	tS <LOGFONT>	lLogFont;

	if	(
			(!m_hWnd)
		&&	(CreateEx (WS_EX_TOOLWINDOW|WS_EX_TOPMOST, AfxRegisterWndClass(0), CLocalize::LoadString (IDS_COMMANDS_VOICE_TITLE, mLangID), WS_POPUP|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU, lInitialRect, NULL, NULL))
		)
	{
		mCommandTree.Create (WS_CHILD|WS_VISIBLE|WS_TABSTOP|TVS_HASLINES|TVS_DISABLEDRAGDROP|TVS_SHOWSELALWAYS, lInitialRect, this, AFX_IDC_LISTBOX);
		if	(IsWindowsVista_AtLeast())
		{
			TreeView_SetExtendedStyle (mCommandTree.m_hWnd, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
		}
		if	(
				(GetObject (GetStockObject (DEFAULT_GUI_FONT), sizeof(lLogFont), &lLogFont) > 0)
			&&	(mFont.CreateFontIndirect (&lLogFont))
			)
		{
			mCommandTree.SetFont (&mFont);
		}
		LoadConfig ();
		RecalcLayout ();
		lRet = true;
	}
	return lRet;
}

bool CVoiceCommandsWnd::Show ()
{
	bool	lRet = false;

	if	(
			(m_hWnd)
		&&	(!IsWindowVisible ())
		)
	{
		ShowWindow (SW_SHOW);
		lRet = true;
	}
	return lRet;
}

bool CVoiceCommandsWnd::Hide ()
{
	bool	lRet = false;

	if	(m_hWnd)
	{
		if	(IsWindowVisible ())
		{
			lRet = true;
		}
		ShowWindow (SW_HIDE);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CVoiceCommandsWnd::LoadConfig ()
{
	CRegKey						lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRect						lWinRect;
	CPoint						lWinPos;
	CSize						lWinSize;
	HMONITOR					lMonitor;
	tSS <MONITORINFO, DWORD>	lMonitorInfo;

	GetWindowRect (&lWinRect);

	lWinSize.cx = (long)CRegDWord (lRegKey, sProfileWinWidth, true, lWinRect.Width()).Value ();
	lWinSize.cy = (long)CRegDWord (lRegKey, sProfileWinHeight, true, lWinRect.Height()).Value ();
	lWinRect.right = lWinRect.left + max (lWinSize.cx, 200);
	lWinRect.bottom = lWinRect.top + max (lWinSize.cy, 200);

	if	(CRegDWord (lRegKey, sProfileWinPosSet, true).Value ())
	{
		lWinPos.x = (long)CRegDWord (lRegKey, sProfileWinLeft, true, lWinRect.left).Value ();
		lWinPos.y = (long)CRegDWord (lRegKey, sProfileWinTop, true, lWinRect.top).Value ();
		lWinRect.OffsetRect (lWinPos.x - lWinRect.left, lWinPos.y - lWinRect.top);
	}
	else
	if	(GetMonitorInfo (MonitorFromWindow (NULL, MONITOR_DEFAULTTOPRIMARY), &lMonitorInfo))
	{
		lWinRect.OffsetRect (lMonitorInfo.rcWork.right - lWinRect.right, lMonitorInfo.rcWork.bottom - lWinRect.bottom);
	}

	if	(
			(
				(lMonitor = MonitorFromPoint (lWinRect.TopLeft(), MONITOR_DEFAULTTONEAREST))
			||	(lMonitor = MonitorFromPoint (lWinRect.TopLeft(), MONITOR_DEFAULTTOPRIMARY))
			)
		&&	(GetMonitorInfo (lMonitor, &lMonitorInfo))
		)
	{
		if	(lWinRect.right > lMonitorInfo.rcWork.right)
		{
			lWinRect.OffsetRect (lMonitorInfo.rcWork.right - lWinRect.right, 0);
		}
		if	(lWinRect.left < lMonitorInfo.rcWork.left)
		{
			lWinRect.OffsetRect (lMonitorInfo.rcWork.left - lWinRect.left, 0);
		}
		if	(lWinRect.bottom > lMonitorInfo.rcWork.bottom)
		{
			lWinRect.OffsetRect (0, lMonitorInfo.rcWork.bottom - lWinRect.bottom);
		}
		if	(lWinRect.top < lMonitorInfo.rcWork.top)
		{
			lWinRect.OffsetRect (0, lMonitorInfo.rcWork.top - lWinRect.top);
		}
	}
	MoveWindow (&lWinRect);
	mInitialRect = lWinRect;
}

void CVoiceCommandsWnd::SaveConfig ()
{
	CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
	CRect	lWinRect;

	GetWindowRect (&lWinRect);
	if	(!lWinRect.EqualRect (&mInitialRect))
	{
		CRegDWord (lRegKey, sProfileWinPosSet, true).SetValue (TRUE).Update ();
		CRegDWord (lRegKey, sProfileWinLeft, true).SetValue (lWinRect.left).Update ();
		CRegDWord (lRegKey, sProfileWinTop, true).SetValue (lWinRect.top).Update ();
		CRegDWord (lRegKey, sProfileWinWidth, true).SetValue (lWinRect.Width()).Update ();
		CRegDWord (lRegKey, sProfileWinHeight, true).SetValue (lWinRect.Height()).Update ();
	}

	if	(
			(mCommandTree.m_hWnd)
		&&	(mGlobalRoot)
		)
	{
		if	(mCommandTree.GetItemState (mGlobalRoot, TVIS_EXPANDED) & TVIS_EXPANDED)
		{
			CRegDWord (lRegKey, sProfileGlobalExpanded, true).SetValue (TRUE).Update ();
		}
		else
		{
			CRegDWord (lRegKey, sProfileGlobalExpanded, true).SetValue (FALSE).Update ();
		}
	}
}

void CVoiceCommandsWnd::RecalcLayout ()
{
	if	(mCommandTree.m_hWnd)
	{
		CRect	lRect;

		GetClientRect (&lRect);
		mCommandTree.MoveWindow (&lRect);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CVoiceCommandsWnd::ShowTheseCommands (long pCharID, LPCTSTR pCaption, const CArrayEx <long> & pIds, const CStringArray & pCaptions)
{
	bool		lRet = false;
	bool		lSpeechEnabled = CDaSpeechInputConfig().LoadConfig().mEnabled;
	CString		lCaption (pCaption);
	HTREEITEM	lCmdItem;
	HTREEITEM	lPrevItem = TVI_FIRST;
	INT_PTR		lNdx;

	mCharCommands.SetAt (pCharID, pCaption);

	if	(mCommandTree.m_hWnd)
	{
		HTREEITEM &	lCharRoot = mCharRoots[pCharID];

		if	(
				(pIds.GetSize() > 0)
			&&	(pCaptions.GetSize() == pIds.GetSize())
			&&	(lSpeechEnabled)
			)
		{
			if	(lCaption.IsEmpty ())
			{
				lCaption = CLocalize::LoadString (IDS_COMMAND_UNDEFINED, mLangID);
			}
#ifdef	_DEBUG_NOT
			lCaption.Format (_T("%s [%d]"), CString((LPCTSTR)lCaption), mCharID);
#endif

			if	(lCharRoot)
			{
				if	(mCommandTree.GetItemText (lCharRoot) != lCaption)
				{
					mCommandTree.SetItemText (lCharRoot, lCaption);
				}
			}
			else
			if	(lCharRoot = mCommandTree.InsertItem (lCaption, TVI_ROOT, TVI_FIRST))
			{
				mCommandTree.SetItemState (lCharRoot, TVIS_EXPANDED, TVIS_EXPANDED);
			}

			for	(lNdx = 0, lCmdItem = mCommandTree.GetChildItem (lCharRoot); (lNdx <= pIds.GetUpperBound()) || (lCmdItem != NULL); lNdx++, lCmdItem = lCmdItem ? mCommandTree.GetNextSiblingItem (lCmdItem) : NULL)
			{
				if	(lNdx <= pIds.GetUpperBound ())
				{
#ifdef	_DEBUG_NOT
					const_cast <CStringArray &> (pCaptions) [lNdx].Format (_T("%s [%d] [%d]"), CString((LPCTSTR)pCaptions [lNdx]), pIds[lNdx], mCharID);
#endif
					if	(lCmdItem)
					{
						if	(mCommandTree.GetItemText (lCmdItem) != pCaptions [lNdx])
						{
							mCommandTree.SetItemText (lCmdItem, pCaptions [lNdx]);
						}
					}
					else
					{
						lCmdItem = mCommandTree.InsertItem (pCaptions [lNdx], lCharRoot, lPrevItem);
					}
					if	(lCmdItem)
					{
						mCommandTree.SetItemData (lCmdItem, pIds [lNdx]);
					}
				}
				else
				if	(
						(lNdx > pIds.GetSize ())
					&&	(lPrevItem != TVI_FIRST)
					&&	(lPrevItem != TVI_LAST)
					)
				{
					mCommandTree.DeleteItem (lPrevItem);
				}

				lPrevItem = lCmdItem ? lCmdItem : TVI_LAST;
			}

			if	(
					(lNdx > pIds.GetSize ())
				&&	(lPrevItem != TVI_FIRST)
				&&	(lPrevItem != TVI_LAST)
				)
			{
				mCommandTree.DeleteItem (lPrevItem);
			}
			lRet = true;
		}
		else
		if	(
				(!mGlobalRoot)
			||	(!lSpeechEnabled)
			)
		{
			if	(lSpeechEnabled)
			{
				lCaption = CLocalize::LoadString (IDS_COMMANDS_VOICE_TITLE_NONE, mLangID);
			}
			else
			{
				lCaption = CLocalize::LoadString (IDS_COMMANDS_NO_SPEECH_DISABLED, mLangID);
			}

			if	(lCharRoot)
			{
				if	(mCommandTree.GetItemText (lCharRoot) != lCaption)
				{
					mCommandTree.SetItemText (lCharRoot, lCaption);
				}
				while	(lCmdItem = mCommandTree.GetChildItem (lCharRoot))
				{
					mCommandTree.DeleteItem (lCmdItem);
				}
			}
			else
			if	(lCharRoot = mCommandTree.InsertItem (lCaption, TVI_ROOT, TVI_FIRST))
			{
				mCommandTree.SetItemState (lCharRoot, TVIS_EXPANDED, TVIS_EXPANDED);
			}
		}
		else
		{
			if	(lCharRoot)
			{
				mCommandTree.DeleteItem (lCharRoot);
			}
			lCharRoot = NULL;
		}
	}
	return lRet;
}

void CVoiceCommandsWnd::ShowOtherClients (long pCharID)
{
	if	(
			(pCharID > 0)
		&&	(mCommandTree.m_hWnd)
		)
	{
		POSITION	lPos;
		CString		lCaption;
		long		lCharID;
		HTREEITEM	lCharRoot;
		HTREEITEM	lCmdItem;
		HTREEITEM	lPrevItem = TVI_FIRST;

		for	(lPos = mCharRoots.GetStartPosition(); lPos;)
		{
			lCharRoot = NULL;
			mCharRoots.GetNextAssoc (lPos, lCharID, lCharRoot);

			if	(
					(lCharRoot)
				&&	(lCharID != pCharID)
				)
			{
				if	(mCharClients [lCharID] == pCharID)
				{
					while (lCmdItem = mCommandTree.GetChildItem (lCharRoot))
					{
						mCommandTree.DeleteItem (lCmdItem);
					}
				}
				else
				{
					mCommandTree.DeleteItem (lCharRoot);
					mCharRoots [lCharID] = NULL;
				}
			}
		}

		for	(lPos = mCharCommands.GetStartPosition(); lPos;)
		{
			lCaption.Empty ();
			lCharID = 0;
			mCharCommands.GetNextAssoc (lPos, lCharID, lCaption);

			if	(lCharID == pCharID)
			{
				if	(mCharRoots[pCharID])
				{
					lPrevItem = mCharRoots[pCharID];
				}
			}
			else
			if	(
					(lCharID > 0)
				&&	(!lCaption.IsEmpty ())
				&&	(mCharClients [lCharID] == pCharID)
				)
			{
				if	(lCharRoot = mCharRoots [lCharID])
				{
					if	(mCommandTree.GetItemText (lCharRoot) != lCaption)
					{
						mCommandTree.SetItemText (lCharRoot, lCaption);
					}
					lPrevItem = lCharRoot;
				}
				else
				if	(lCharRoot = mCommandTree.InsertItem (lCaption, TVI_ROOT, lPrevItem))
				{
					mCharRoots [lCharID] = lCharRoot;
					lPrevItem = lCharRoot;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CVoiceCommandsWnd::ShowGlobalCommands (USHORT pHideWndCmdId, USHORT pHideCharCmdId)
{
	bool				lRet = false;
	HTREEITEM			lPrevItem = TVI_FIRST;
	POSITION			lPos;
	long				lCharID;
	CString				lCharName;
	CStringMap <long>	lCharNames;
	INT_PTR				lCharNdx;

	if	(!CDaSpeechInputConfig().LoadConfig().mEnabled)
	{
		HideGlobalCommands ();
	}
	else
	if	(mCommandTree.m_hWnd)
	{
		if	(!mGlobalRoot)
		{
			CRegKey	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

			mGlobalRoot = mCommandTree.InsertItem (CLocalize::LoadString (IDS_COMMANDS_GLOBAL_TITLE, mLangID), TVI_ROOT, TVI_LAST);

			if	(CRegDWord (lRegKey, sProfileGlobalExpanded, true).Value ())
			{
				mCommandTree.SetItemState (mGlobalRoot, TVIS_EXPANDED, TVIS_EXPANDED);
			}
		}

		if	(pHideWndCmdId)
		{
			if	(
					(mGlobalItems[sGlobalItemHideWnd])
				||	(mGlobalItems[sGlobalItemHideWnd] = mCommandTree.InsertItem (CLocalize::LoadString (IDS_COMMANDS_WINDOW_CLOSE_PROMPT, mLangID), mGlobalRoot, lPrevItem))
				)
			{
				mCommandTree.SetItemData (mGlobalItems[sGlobalItemHideWnd], pHideWndCmdId);
				lPrevItem = mGlobalItems[sGlobalItemHideWnd];
			}
		}
		else
		{
			if	(mGlobalItems[sGlobalItemHideWnd])
			{
				mCommandTree.DeleteItem (mGlobalItems[sGlobalItemHideWnd]);
				mGlobalItems[sGlobalItemHideWnd] = NULL;
			}
		}

		//	Sort the character names alphabetically
		for	(lPos = mCharNames.GetStartPosition(); lPos;)
		{
			mCharNames.GetNextAssoc (lPos, lCharID, lCharName);
			lCharNames.SetAt (lCharName, lCharID);
		}

		for	(lCharNdx = 0; lCharNdx <= lCharNames.GetUpperBound(); lCharNdx++)
		{
			lCharID = lCharNames [lCharNdx];
			lCharName = lCharNames.KeyAt (lCharNdx);

			if	(
					(!lCharName.IsEmpty ())
				&&	(mCharClients [lCharID] == lCharID)
				)
			{
				if	(
						(mCharItems[lCharID])
					&&	(mCommandTree.GetPrevSiblingItem (mCharItems[lCharID]) != lPrevItem)
					)
				{
					mCommandTree.DeleteItem (mCharItems[lCharID]);
					mCharItems[lCharID] = NULL;
				}
				if	(
						(mCharItems[lCharID])
					||	(mCharItems[lCharID] = mCommandTree.InsertItem (lCharName, mGlobalRoot, lPrevItem))
					)
				{
					lPrevItem = mCharItems[lCharID];
				}
			}
			else
			{
				if	(mCharItems[lCharID])
				{
					mCommandTree.DeleteItem (mCharItems[lCharID]);
					mCharItems[lCharID] = NULL;
				}
			}
		}

		if	(pHideCharCmdId)
		{
			if	(
					(mGlobalItems[sGlobalItemHideChar])
				||	(mGlobalItems[sGlobalItemHideChar] = mCommandTree.InsertItem (CLocalize::LoadString (IDS_COMMANDS_HIDE_PROMPT, mLangID), mGlobalRoot, lPrevItem))
				)
			{
				mCommandTree.SetItemData (mGlobalItems[sGlobalItemHideChar], pHideCharCmdId);
				lPrevItem = mGlobalItems[sGlobalItemHideChar];
			}
		}
		else
		{
			if	(mGlobalItems[sGlobalItemHideChar])
			{
				mCommandTree.DeleteItem (mGlobalItems[sGlobalItemHideChar]);
				mGlobalItems[sGlobalItemHideChar] = NULL;
			}
		}
	}

	return lRet;
}

bool CVoiceCommandsWnd::HideGlobalCommands ()
{
	bool	lRet = false;

	if	(mGlobalRoot)
	{
		SaveConfig ();
		mCommandTree.DeleteItem (mGlobalRoot);
		mGlobalRoot = NULL;
		memset (mGlobalItems, 0, sizeof (mGlobalItems));
		mCharItems.RemoveAll ();
		lRet = true;
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CVoiceCommandsWnd::SetCharacter (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption)
{
	bool	lRet = false;

	if	(m_hWnd)
	{
		CString	lCaption;
		CString	lCharName;

		if	(pCharID != mCharID)
		{
			mCharID = pCharID;
			lRet = true;
		}
		if	(pCharID)
		{
			lCharName = pCharName;
			if	(lCharName.IsEmpty ())
			{
				lCharName = mCharNames [pCharID];
			}
		}
		else
		{
			lCharName.Empty ();
		}
		if	(!lCharName.IsEmpty ())
		{
			lCaption = CLocalize::LoadString (IDS_COMMANDS_VOICE_TITLE_EX, mLangID);
			lCaption.Replace (_T("%1s"), _T("%1"));
			lCaption = FormatString (lCaption, lCharName);
		}
		else
		{
			lCaption = CLocalize::LoadString (IDS_COMMANDS_VOICE_TITLE, mLangID);
#ifdef	_DEBUG_NOT
			lCaption.Format (_T("%s [%d]"), CString((LPCTSTR)lCaption), mCharID);
#endif
		}
		AfxSetWindowText (m_hWnd, lCaption);

		if	(
				(pCharID)
			&&	(pCharName)
			)
		{
			SetCharacterName (pCharID, pCharName, pCommandsCaption);
		}
	}
	return lRet;
}

bool CVoiceCommandsWnd::SetCharacterClient (long pCharID, long pActiveCharID)
{
	if	(
			(pCharID > 0)
		&&	(pActiveCharID > 0)
		&&	(mCharClients [pCharID] != pActiveCharID)
		)
	{
		mCharClients [pCharID] = pActiveCharID;

		if	(
				(pActiveCharID != pCharID)
			&&	(mCharItems [pCharID])
			&&	(!mCharItems [pActiveCharID])
			)
		{
			mCharItems [pActiveCharID] = mCharItems [pCharID];
			mCharItems [pCharID] = NULL;

			if	(
					(mCommandTree.m_hWnd)
				&&	(!mCharNames [pActiveCharID].IsEmpty ())
				&&	(mCharNames [pActiveCharID] != mCharNames [pCharID])
				)
			{
				mCommandTree.SetItemText (mCharItems [pActiveCharID], mCharNames [pActiveCharID]);
			}
		}

		ShowOtherClients (pCharID);
		return true;
	}
	return false;
}

bool CVoiceCommandsWnd::SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption)
{
	bool	lRet = false;
	CString	lCharName (pCharName);
	CString	lCommandsCaption (pCommandsCaption);

#ifdef	_DEBUG_NOT
	if	(!lCharName.IsEmpty ())
	{
		lCharName.Format (_T("%s [%d]"), CString((LPCTSTR)lCharName), pCharID);
	}
#endif
	if	(pCharID)
	{
		if	(
				(!lCharName.IsEmpty ())
			&&	(mCharNames [pCharID] != lCharName)
			)
		{
			mCharNames [pCharID] = lCharName;

			if	(
					(m_hWnd)
				&&	(mCharID == pCharID)
				)
			{
				SetCharacter (mCharID);
			}
			if	(
					(mCommandTree.m_hWnd)
				&&	(mCharItems [pCharID])
				)
			{
				mCommandTree.SetItemText (mCharItems [pCharID], mCharNames [pCharID]);
			}
			lRet = true;
		}
		if	(
				(pCommandsCaption)
			&&	(mCharCommands [pCharID] != lCommandsCaption)
			)
		{
			mCharCommands [pCharID] = lCommandsCaption;
			lRet = true;
		}
	}
	return lRet;
}

bool CVoiceCommandsWnd::RemoveCharacter (long pCharID)
{
	bool	lRet = false;

	mCharClients.RemoveKey (pCharID);
	mCharCommands.RemoveKey (pCharID);
	if	(mCharNames.RemoveKey (pCharID))
	{
		lRet = true;
	}
	if	(
			(mCommandTree.m_hWnd)
		&&	(mCharRoots [pCharID])
		)
	{
		mCommandTree.DeleteItem (mCharRoots [pCharID]);
	}
	mCharRoots.RemoveKey (pCharID);
	if	(
			(mCommandTree.m_hWnd)
		&&	(mCharItems [pCharID])
		)
	{
		mCommandTree.DeleteItem (mCharItems [pCharID]);
	}
	mCharItems.RemoveKey (pCharID);

	return lRet;
}

bool CVoiceCommandsWnd::SetLangID (LANGID pLangID)
{
	if	(mLangID != pLangID)
	{
		mLangID = pLangID;

		if	(m_hWnd)
		{
			SetCharacter (mCharID);
		}
		if	(mCommandTree.m_hWnd)
		{
			HideGlobalCommands ();
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CVoiceCommandsWnd::OnDestroy()
{
	SaveConfig ();
	CWnd::OnDestroy ();
}

void CVoiceCommandsWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize (nType, cx, cy);
	RecalcLayout ();
}

void CVoiceCommandsWnd::OnClose()
{
	Hide ();
}

/////////////////////////////////////////////////////////////////////////////

void CVoiceCommandsWnd::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW	lNotify = (LPNMTREEVIEW) pNMHDR;

	if	(
			(lNotify->hdr.hwndFrom == mCommandTree.m_hWnd)
		&&	(lNotify->itemNew.hItem != mGlobalRoot)
		&&	(
				(lNotify->action == TVE_COLLAPSE)
			||	(lNotify->action == TVE_COLLAPSERESET)
			)
		)
	{
		*pResult = TRUE;
	}
}
