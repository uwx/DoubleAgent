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
#ifndef VOICECOMMANDSWND_H_INCLUDED_
#define VOICECOMMANDSWND_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "DaCoreRes.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)

class _DACORE_IMPEXP CVoiceCommandsWnd : public CWnd
{
protected:
	CVoiceCommandsWnd ();
public:
	virtual ~CVoiceCommandsWnd ();
	DECLARE_DYNCREATE (CVoiceCommandsWnd)

// Attributes
public:
	long GetCharID () const {return mCharID;}
	LANGID GetLangID () const {return mLangID;}

// Operations
public:
	bool Create ();
	bool Show ();
	bool Hide ();

	bool ShowTheseCommands (long pCharID, LPCTSTR pCaption, const CArrayEx <long> & pIds, const CStringArray & pCaptions);
	void ShowOtherClients (long pCharID);
	bool ShowGlobalCommands (USHORT pHideWndCmdId = ID_COMMANDS_WINDOW_OPEN, USHORT pHideCharCmdId = ID_COMMANDS_WINDOW_CLOSE);
	bool HideGlobalCommands ();

	bool SetCharacter (long pCharID, LPCTSTR pCharName = NULL, LPCTSTR pCommandsCaption = NULL);
	bool SetCharacterClient (long pCharID, long pActiveCharID);
	bool SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption = NULL);
	bool RemoveCharacter (long pCharID);
	bool SetLangID (LANGID pLangID);

// Overrides
	//{{AFX_VIRTUAL(CVoiceCommandsWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CVoiceCommandsWnd)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void LoadConfig ();
	void SaveConfig ();
	void RecalcLayout ();

protected:
	CRect									mInitialRect;
	CTreeCtrl								mCommandTree;
	CFont									mFont;
	LANGID									mLangID;
	long									mCharID;
	CMap <long, long, long, long>			mCharClients;
	CMap <long, long, CString, LPCTSTR>		mCharNames;
	CMap <long, long, CString, LPCTSTR>		mCharCommands;
	CMap <long, long, HTREEITEM, HTREEITEM>	mCharItems;
	CMap <long, long, HTREEITEM, HTREEITEM>	mCharRoots;
	HTREEITEM								mGlobalRoot;
	HTREEITEM								mGlobalItems [2];
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // VOICECOMMANDSWND_H_INCLUDED_
