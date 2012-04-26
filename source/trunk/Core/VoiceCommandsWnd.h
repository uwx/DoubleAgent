/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#pragma once
#include "DaCoreExp.h"
#include "DaCoreRes.h"

/////////////////////////////////////////////////////////////////////////////

class CVoiceCommandsWndBase :
	public CWindowImpl<CVoiceCommandsWndBase>
{
public:
};

/////////////////////////////////////////////////////////////////////////////

class CVoiceCommandsWnd :
	public CVoiceCommandsWndBase
{
	DECLARE_DLL_OBJECT_EX(CVoiceCommandsWnd, _DACORE_IMPEXP)
protected:
	CVoiceCommandsWnd ();
public:
	_DACORE_IMPEXP virtual ~CVoiceCommandsWnd ();
	_DACORE_IMPEXP static CVoiceCommandsWnd* CreateInstance ();

// Attributes
public:
	_DACORE_IMPEXP long GetCharID () const {return mCharID;}
	_DACORE_IMPEXP LANGID GetLangID () const {return mLangID;}

// Operations
public:
	_DACORE_IMPEXP bool Create (CWindow* pOwnerWnd);
	_DACORE_IMPEXP bool Show (bool pActivate);
	_DACORE_IMPEXP bool Hide ();

	_DACORE_IMPEXP bool ShowTheseCommands (long pCharID, LPCTSTR pCaption, const CAtlTypeArray <long>& pIds, const CAtlStringArray& pCaptions);
	_DACORE_IMPEXP void ShowOtherClients (long pCharID);
	_DACORE_IMPEXP bool ShowGlobalCommands (USHORT pHideWndCmdId = ID_COMMANDS_WINDOW_OPEN, USHORT pHideCharCmdId = ID_COMMANDS_WINDOW_CLOSE);
	_DACORE_IMPEXP bool HideGlobalCommands ();

	_DACORE_IMPEXP bool SetCharacter (long pCharID, LPCTSTR pCharName = NULL, LPCTSTR pCommandsCaption = NULL);
	_DACORE_IMPEXP bool SetCharacterClient (long pCharID, long pActiveCharID);
	_DACORE_IMPEXP bool SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption = NULL);
	_DACORE_IMPEXP bool RemoveCharacter (long pCharID);
	_DACORE_IMPEXP bool SetLangID (LANGID pLangID);

// Overrides

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnItemExpanding(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BEGIN_MSG_MAP(CVoiceCommandsWnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		NOTIFY_HANDLER(IDS_FOR_COMMANDS, TVN_ITEMEXPANDING, OnItemExpanding)
	END_MSG_MAP()

protected:
	void LoadConfig ();
	void SaveConfig ();
	void RecalcLayout ();

	HTREEITEM InsertTreeItem (HWND pTree, LPCTSTR pItemText, HTREEITEM pParentItem = TVI_ROOT, HTREEITEM pPrevItem = TVI_LAST);
	CAtlString GetTreeItemText (HWND pTree, HTREEITEM pTreeItem);
	void SetTreeItemText (HWND pTree, HTREEITEM pTreeItem, LPCTSTR pItemText);
	void SetTreeItemData (HWND pTree, HTREEITEM pTreeItem, LPARAM pItemData);

protected:
	CRect																				mInitialRect;
	CContainedWindow																	mCommandTree;
	CFontHandle																			mFont;
	LANGID																				mLangID;
	long																				mCharID;
	CAtlMap <long, CZeroInit<long> >													mCharClients;
	CAtlMap <long, CAtlString, CElementTraits<long>, CStringElementTraits<CAtlString> >	mCharNames;
	CAtlMap <long, CAtlString, CElementTraits<long>, CStringElementTraits<CAtlString> >	mCharCommands;
	CAtlMap <long, CZeroInit<HTREEITEM> >												mCharItems;
	CAtlMap <long, CZeroInit<HTREEITEM> >												mCharRoots;
	HTREEITEM																			mGlobalRoot;
	HTREEITEM																			mGlobalItems [2];
};

/////////////////////////////////////////////////////////////////////////////
