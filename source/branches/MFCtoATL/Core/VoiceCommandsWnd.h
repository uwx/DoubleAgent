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
#pragma once
#include "DaCoreExp.h"
#include "DaCoreRes.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)
/////////////////////////////////////////////////////////////////////////////

class CVoiceCommandsWndBase :
	public CWindowImpl<CVoiceCommandsWndBase>
{
public:
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CVoiceCommandsWnd :
	public CVoiceCommandsWndBase
{
	DECLARE_DLL_OBJECT(CVoiceCommandsWnd)
protected:
	CVoiceCommandsWnd ();
public:
	virtual ~CVoiceCommandsWnd ();
	static CVoiceCommandsWnd * CreateInstance ();

// Attributes
public:
	long GetCharID () const {return mCharID;}
	LANGID GetLangID () const {return mLangID;}

// Operations
public:
	bool Create ();
	bool Show ();
	bool Hide ();

	bool ShowTheseCommands (long pCharID, LPCTSTR pCaption, const CAtlTypeArray <long> & pIds, const CAtlStringArray & pCaptions);
	void ShowOtherClients (long pCharID);
	bool ShowGlobalCommands (USHORT pHideWndCmdId = ID_COMMANDS_WINDOW_OPEN, USHORT pHideCharCmdId = ID_COMMANDS_WINDOW_CLOSE);
	bool HideGlobalCommands ();

	bool SetCharacter (long pCharID, LPCTSTR pCharName = NULL, LPCTSTR pCommandsCaption = NULL);
	bool SetCharacterClient (long pCharID, long pActiveCharID);
	bool SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption = NULL);
	bool RemoveCharacter (long pCharID);
	bool SetLangID (LANGID pLangID);

// Overrides

// Implementation
protected:
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnItemExpanding(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

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

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
