/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaGlobalConfig.h"
#include "VoiceCommandsWnd.h"
#include "Localize.h"
#include "Registry.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
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

IMPLEMENT_DLL_OBJECT(CVoiceCommandsWnd)

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
	if	(IsWindow ())
	{
		DestroyWindow ();
	}
}

CVoiceCommandsWnd* CVoiceCommandsWnd::CreateInstance ()
{
	return new CVoiceCommandsWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CVoiceCommandsWnd::Create (CWindow* pOwnerWnd)
{
	bool			lRet = false;
	HWND			lOwnerWnd = (pOwnerWnd) ? pOwnerWnd->m_hWnd : NULL;
	DWORD			lStyle = WS_POPUP|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU;
	DWORD			lExStyle = WS_EX_TOOLWINDOW | ((pOwnerWnd) ? 0 : WS_EX_TOPMOST);
	CRect			lInitialRect (0,0,200,200);
	tS <LOGFONT>	lLogFont;

	if	(
			(!IsWindow ())
		&&	(CVoiceCommandsWndBase::Create (lOwnerWnd, lInitialRect, CLocalize::LoadString (IDS_COMMANDS_VOICE_TITLE, mLangID), lStyle, lExStyle))
		)
	{
		mCommandTree.Attach (::CreateWindowEx (0, WC_TREEVIEW, NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|TVS_HASLINES|TVS_DISABLEDRAGDROP|TVS_SHOWSELALWAYS, lInitialRect.left, lInitialRect.top, lInitialRect.Width(), lInitialRect.Height(), m_hWnd, (HMENU)IDS_FOR_COMMANDS, _AtlBaseModule.GetModuleInstance(), NULL));

		if	(IsWindowsVista_AtLeast())
		{
			TreeView_SetExtendedStyle (mCommandTree.m_hWnd, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
		}
		if	(
				(GetObject (GetStockObject (DEFAULT_GUI_FONT), sizeof(lLogFont), &lLogFont) > 0)
			&&	(mFont = CreateFontIndirect (&lLogFont))
			)
		{
			mCommandTree.SetFont (mFont);
		}
		LoadConfig ();
		RecalcLayout ();
		lRet = true;
	}
	return lRet;
}

bool CVoiceCommandsWnd::Show (bool pActivate)
{
	bool	lRet = false;

	if	(
			(IsWindow ())
		&&	(!IsWindowVisible ())
		)
	{
		ShowWindow (pActivate ? SW_SHOW : SW_SHOWNA);
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
	CRegKeyEx					lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
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
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
	CRect		lWinRect;

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
		if	(TreeView_GetItemState (mCommandTree, mGlobalRoot, TVIS_EXPANDED) & TVIS_EXPANDED)
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

HTREEITEM CVoiceCommandsWnd::InsertTreeItem (HWND pTree, LPCTSTR pItemText, HTREEITEM pParentItem, HTREEITEM pPrevItem)
{
	tS <TVINSERTSTRUCT>	lTreeInsert;

	lTreeInsert.hParent = pParentItem;
	lTreeInsert.hInsertAfter = pPrevItem;
	lTreeInsert.item.mask = TVIF_TEXT;
	lTreeInsert.item.pszText = (LPTSTR)pItemText;
	return TreeView_InsertItem (pTree, &lTreeInsert);
}

CAtlString CVoiceCommandsWnd::GetTreeItemText (HWND pTree, HTREEITEM pTreeItem)
{
	CAtlString	lText;
	tS <TVITEM>	lTreeItem;

	lTreeItem.hItem = pTreeItem;
	lTreeItem.mask = TVIF_TEXT;
	lTreeItem.cchTextMax = 2048;
	lTreeItem.pszText = lText.GetBuffer (lTreeItem.cchTextMax);
	TreeView_GetItem (pTree, &lTreeItem);

	lText.ReleaseBuffer ();
	return lText;
}

void CVoiceCommandsWnd::SetTreeItemText (HWND pTree, HTREEITEM pTreeItem, LPCTSTR pItemText)
{
	tS <TVITEM>	lTreeItem;

	lTreeItem.hItem = pTreeItem;
	lTreeItem.mask = TVIF_TEXT;
	lTreeItem.pszText = (LPTSTR)pItemText;
	TreeView_SetItem (pTree, &lTreeItem);
}

void CVoiceCommandsWnd::SetTreeItemData (HWND pTree, HTREEITEM pTreeItem, LPARAM pItemData)
{
	tS <TVITEM>	lTreeItem;

	lTreeItem.hItem = pTreeItem;
	lTreeItem.mask = TVIF_PARAM;
	lTreeItem.lParam = pItemData;
	TreeView_SetItem (pTree, &lTreeItem);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CVoiceCommandsWnd::ShowTheseCommands (long pCharID, LPCTSTR pCaption, const CAtlTypeArray <long>& pIds, const CAtlStringArray& pCaptions)
{
	bool		lRet = false;
	bool		lSpeechEnabled = CDaSettingsConfig().LoadConfig().mSrEnabled;
	CAtlString	lCaption (pCaption);
	HTREEITEM	lCmdItem;
	HTREEITEM	lPrevItem = TVI_FIRST;
	INT_PTR		lNdx;

	mCharCommands.SetAt (pCharID, pCaption);

	if	(mCommandTree.m_hWnd)
	{
		HTREEITEM&	lCharRoot = mCharRoots [pCharID];

		if	(
				(pIds.GetCount() > 0)
			&&	(pCaptions.GetCount() == pIds.GetCount())
			&&	(lSpeechEnabled)
			)
		{
			if	(lCaption.IsEmpty ())
			{
				lCaption = CLocalize::LoadString (IDS_COMMAND_UNDEFINED, mLangID);
			}
#ifdef	_DEBUG_NOT
			lCaption.Format (_T("%s [%d]"), CAtlString((LPCTSTR)lCaption), mCharID);
#endif

			if	(lCharRoot)
			{
				if	(GetTreeItemText (mCommandTree, lCharRoot) != lCaption)
				{
					SetTreeItemText (mCommandTree, lCharRoot, lCaption);
				}
			}
			else
			if	(lCharRoot = InsertTreeItem (mCommandTree, lCaption, TVI_ROOT, TVI_FIRST))
			{
				TreeView_SetItemState (mCommandTree, lCharRoot, TVIS_EXPANDED, TVIS_EXPANDED);
			}

			for	(lNdx = 0, lCmdItem = TreeView_GetChild (mCommandTree, lCharRoot); (lNdx < (INT_PTR)pIds.GetCount()) || (lCmdItem != NULL); lNdx++, lCmdItem = lCmdItem ? TreeView_GetNextSibling (mCommandTree, lCmdItem) : NULL)
			{
				if	(lNdx < (INT_PTR)pIds.GetCount())
				{
#ifdef	_DEBUG_NOT
					const_cast <CAtlStringArray&> (pCaptions) [lNdx].Format (_T("%s [%d] [%d]"), CAtlString((LPCTSTR)pCaptions [lNdx]), pIds[lNdx], mCharID);
#endif
					if	(lCmdItem)
					{
						if	(pCaptions [lNdx].IsEmpty ())
						{
							lCmdItem = (lPrevItem == TVI_FIRST) ? TreeView_GetChild (mCommandTree, lCharRoot) : (lPrevItem == TVI_LAST) ? NULL : lPrevItem;
							continue;
						}
						else
						if	(GetTreeItemText (mCommandTree, lCmdItem) != pCaptions [lNdx])
						{
							SetTreeItemText (mCommandTree, lCmdItem, pCaptions [lNdx]);
						}
					}
					else
					if	(!pCaptions [lNdx].IsEmpty ())
					{
						lCmdItem = InsertTreeItem (mCommandTree, pCaptions [lNdx], lCharRoot, lPrevItem);
					}
					if	(lCmdItem)
					{
						SetTreeItemData (mCommandTree, lCmdItem, pIds [lNdx]);
					}
				}
				else
				if	(
						(lNdx > (INT_PTR)pIds.GetCount())
					&&	(lPrevItem != TVI_FIRST)
					&&	(lPrevItem != TVI_LAST)
					)
				{
					TreeView_DeleteItem (mCommandTree, lPrevItem);
				}

				lPrevItem = lCmdItem ? lCmdItem : TVI_LAST;
			}

			if	(
					(lNdx > (INT_PTR)pIds.GetCount())
				&&	(lPrevItem != TVI_FIRST)
				&&	(lPrevItem != TVI_LAST)
				)
			{
				TreeView_DeleteItem (mCommandTree, lPrevItem);
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
				if	(GetTreeItemText (mCommandTree, lCharRoot) != lCaption)
				{
					SetTreeItemText (mCommandTree, lCharRoot, lCaption);
				}
				while	(lCmdItem = TreeView_GetChild (mCommandTree, lCharRoot))
				{
					TreeView_DeleteItem (mCommandTree, lCmdItem);
				}
			}
			else
			if	(lCharRoot = InsertTreeItem (mCommandTree, lCaption, TVI_ROOT, TVI_FIRST))
			{
				TreeView_SetItemState (mCommandTree, lCharRoot, TVIS_EXPANDED, TVIS_EXPANDED);
			}
		}
		else
		{
			if	(lCharRoot)
			{
				TreeView_DeleteItem (mCommandTree, lCharRoot);
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
		CAtlString	lCaption;
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
					while (lCmdItem = TreeView_GetChild (mCommandTree, lCharRoot))
					{
						TreeView_DeleteItem (mCommandTree, lCmdItem);
					}
				}
				else
				{
					TreeView_DeleteItem (mCommandTree, lCharRoot);
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
				if	(mCharRoots [pCharID])
				{
					lPrevItem = mCharRoots [pCharID];
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
					if	(GetTreeItemText (mCommandTree, lCharRoot) != lCaption)
					{
						SetTreeItemText (mCommandTree, lCharRoot, lCaption);
					}
					lPrevItem = lCharRoot;
				}
				else
				if	(lCharRoot = InsertTreeItem (mCommandTree, lCaption, TVI_ROOT, lPrevItem))
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
	bool																		lRet = false;
	HTREEITEM																	lPrevItem = TVI_FIRST;
	POSITION																	lPos;
	long																		lCharID;
	CAtlString																	lCharName;
	CAtlMap <CAtlString, CZeroInit<long>, CStringElementTraits<CAtlString> >	lCharNames;

	if	(!CDaSettingsConfig().LoadConfig().mSrEnabled)
	{
		HideGlobalCommands ();
	}
	else
	if	(mCommandTree.m_hWnd)
	{
		if	(!mGlobalRoot)
		{
			CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

			mGlobalRoot = InsertTreeItem (mCommandTree, CLocalize::LoadString (IDS_COMMANDS_GLOBAL_TITLE, mLangID), TVI_ROOT, TVI_LAST);

			if	(CRegDWord (lRegKey, sProfileGlobalExpanded, true).Value ())
			{
				TreeView_SetItemState (mCommandTree, mGlobalRoot, TVIS_EXPANDED, TVIS_EXPANDED);
			}
		}

		if	(pHideWndCmdId)
		{
			if	(
					(mGlobalItems[sGlobalItemHideWnd])
				||	(mGlobalItems[sGlobalItemHideWnd] = InsertTreeItem (mCommandTree, CLocalize::LoadString (IDS_COMMANDS_WINDOW_CLOSE_PROMPT, mLangID), mGlobalRoot, lPrevItem))
				)
			{
				SetTreeItemData (mCommandTree, mGlobalItems[sGlobalItemHideWnd], pHideWndCmdId);
				lPrevItem = mGlobalItems[sGlobalItemHideWnd];
			}
		}
		else
		{
			if	(mGlobalItems[sGlobalItemHideWnd])
			{
				TreeView_DeleteItem (mCommandTree, mGlobalItems[sGlobalItemHideWnd]);
				mGlobalItems[sGlobalItemHideWnd] = NULL;
			}
		}

		//	Sort the character names alphabetically
		for	(lPos = mCharNames.GetStartPosition(); lPos;)
		{
			mCharNames.GetNextAssoc (lPos, lCharID, lCharName);
			lCharNames.SetAt (lCharName, lCharID);
		}

		for	(lPos = lCharNames.GetStartPosition(); lPos;)
		{
			lCharNames.GetNextAssoc (lPos, lCharName, lCharID);

			if	(
					(!lCharName.IsEmpty ())
				&&	(mCharClients [lCharID] == lCharID)
				)
			{
				if	(
						(mCharItems[lCharID])
					&&	(TreeView_GetPrevSibling (mCommandTree, mCharItems[lCharID]) != lPrevItem)
					)
				{
					TreeView_DeleteItem (mCommandTree, mCharItems[lCharID]);
					mCharItems[lCharID] = NULL;
				}
				if	(
						(mCharItems[lCharID])
					||	(mCharItems[lCharID] = InsertTreeItem (mCommandTree, lCharName, mGlobalRoot, lPrevItem))
					)
				{
					lPrevItem = mCharItems[lCharID];
				}
			}
			else
			{
				if	(mCharItems[lCharID])
				{
					TreeView_DeleteItem (mCommandTree, mCharItems[lCharID]);
					mCharItems[lCharID] = NULL;
				}
			}
		}

		if	(pHideCharCmdId)
		{
			if	(
					(mGlobalItems[sGlobalItemHideChar])
				||	(mGlobalItems[sGlobalItemHideChar] = InsertTreeItem (mCommandTree, CLocalize::LoadString (IDS_COMMANDS_HIDE_PROMPT, mLangID), mGlobalRoot, lPrevItem))
				)
			{
				SetTreeItemData (mCommandTree, mGlobalItems[sGlobalItemHideChar], pHideCharCmdId);
				lPrevItem = mGlobalItems[sGlobalItemHideChar];
			}
		}
		else
		{
			if	(mGlobalItems[sGlobalItemHideChar])
			{
				TreeView_DeleteItem (mCommandTree, mGlobalItems[sGlobalItemHideChar]);
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
		TreeView_DeleteItem (mCommandTree, mGlobalRoot);
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
		CAtlString	lCaption;
		CAtlString	lCharName;

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
			lCaption.Format (_T("%s [%d]"), CAtlString((LPCTSTR)lCaption), mCharID);
#endif
		}
		SetWindowText (lCaption);

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
				SetTreeItemText (mCommandTree, mCharItems [pActiveCharID], mCharNames [pActiveCharID]);
			}
		}

		ShowOtherClients (pCharID);
		return true;
	}
	return false;
}

bool CVoiceCommandsWnd::SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption)
{
	bool		lRet = false;
	CAtlString	lCharName (pCharName);
	CAtlString	lCommandsCaption (pCommandsCaption);

#ifdef	_DEBUG_NOT
	if	(!lCharName.IsEmpty ())
	{
		lCharName.Format (_T("%s [%d]"), CAtlString((LPCTSTR)lCharName), pCharID);
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
				SetTreeItemText (mCommandTree, mCharItems [pCharID], mCharNames [pCharID]);
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
		TreeView_DeleteItem (mCommandTree, mCharRoots [pCharID]);
	}
	mCharRoots.RemoveKey (pCharID);
	if	(
			(mCommandTree.m_hWnd)
		&&	(mCharItems [pCharID])
		)
	{
		TreeView_DeleteItem (mCommandTree, mCharItems [pCharID]);
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

LRESULT CVoiceCommandsWnd::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SaveConfig ();
	bHandled = FALSE;
	return 0;
}

LRESULT CVoiceCommandsWnd::OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = DefWindowProc ();
	RecalcLayout ();
	return lResult;
}

LRESULT CVoiceCommandsWnd::OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Hide ();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CVoiceCommandsWnd::OnItemExpanding(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LRESULT			lResult = FALSE;
	LPNMTREEVIEW	lNotify = (LPNMTREEVIEW) pnmh;

	if	(
			(lNotify->hdr.hwndFrom == mCommandTree.m_hWnd)
		&&	(lNotify->itemNew.hItem != mGlobalRoot)
		&&	(
				(lNotify->action == TVE_COLLAPSE)
			||	(lNotify->action == TVE_COLLAPSERESET)
			)
		)
	{
		lResult = TRUE;
	}
	return lResult;
}
