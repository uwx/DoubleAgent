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
#pragma once
#include "DaCoreExp.h"

/////////////////////////////////////////////////////////////////////////////

class CAgentListeningWndBase :
	public CWindowImpl<CAgentListeningWndBase, CWindow, CWinTraits<WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE> >
{
public:
};

/////////////////////////////////////////////////////////////////////////////

class CAgentListeningWnd :
	public CAgentListeningWndBase
{
	DECLARE_DLL_OBJECT_EX(CAgentListeningWnd, _DACORE_IMPEXP)
protected:
	CAgentListeningWnd ();
public:
	_DACORE_IMPEXP virtual ~CAgentListeningWnd ();
	_DACORE_IMPEXP static CAgentListeningWnd * CreateInstance ();

// Attributes
public:
	_DACORE_IMPEXP long GetCharID () const {return mCharID;}
	_DACORE_IMPEXP long GetLangID () const {return mLangID;}

// Operations
public:
	_DACORE_IMPEXP bool Create (CWindow * pOwnerWnd, DWORD pExStyle = 0);
	_DACORE_IMPEXP bool Attach (long pCharID, LPCTSTR pCharacterName);
	_DACORE_IMPEXP bool Detach (long pCharID);
	_DACORE_IMPEXP bool SetLangID (LANGID pLangID);

	_DACORE_IMPEXP bool ShowTipWnd ();
	_DACORE_IMPEXP bool HideTipWnd ();
	_DACORE_IMPEXP void PositionTipWnd ();
	_DACORE_IMPEXP void RefreshTipWnd ();

	_DACORE_IMPEXP void ShowCharacterListening (LPCTSTR pCommandsCaption);
	_DACORE_IMPEXP void ShowCharacterNotListening (LPCTSTR pCommandsCaption, LPCTSTR pReason = NULL);
	_DACORE_IMPEXP void ShowCharacterHeard (LPCTSTR pCommand);

// Overrides

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnWindowPosChanging (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnWindowPosChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentListeningWnd)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

protected:
	CAtlString GetCharacterCaption (UINT pCaptionStrId) const;
	CAtlString GetCommandsCaption () const;
	CAtlString GetCommandCaption (LPCTSTR pCommand) const;

	void ShowToolText (LPCTSTR pCaption, LPCTSTR pDetail);
	bool CalcWinRect (CRect & pWinRect);

protected:
	long					mCharID;
	LANGID					mLangID;
	CAtlString				mCharacterName;
	CAtlString				mCommandsCaption;
	CAtlString				mCaption;
	CAtlString				mDetail;
	tSS <TOOLINFO, UINT>	mToolInfo;
	CWindow *				mOwnerWnd;
	CFontHandle				mFont;
};

/////////////////////////////////////////////////////////////////////////////
