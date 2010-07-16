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

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class CAgentListeningWndBase :
	public CWindowImpl<CAgentListeningWndBase, CWindow, CWinTraits<WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE> >
{
public:
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CAgentListeningWnd :
	public CAgentListeningWndBase
{
	DECLARE_DLL_OBJECT(CAgentListeningWnd)
protected:
	CAgentListeningWnd ();
public:
	virtual ~CAgentListeningWnd ();
	static CAgentListeningWnd * CreateInstance ();

// Attributes
public:
	long GetCharID () const {return mCharID;}
	long GetLangID () const {return mLangID;}

// Operations
public:
	bool Create (HWND pParentWnd);
	bool Attach (long pCharID, LPCTSTR pCharacterName);
	bool Detach (long pCharID);
	bool SetLangID (LANGID pLangID);

	bool ShowTipWnd ();
	bool HideTipWnd ();
	void PositionTipWnd ();
	void RefreshTipWnd ();

	void ShowCharacterListening (LPCTSTR pCommandsCaption);
	void ShowCharacterNotListening (LPCTSTR pCommandsCaption, LPCTSTR pReason = NULL);
	void ShowCharacterHeard (LPCTSTR pCommand);

// Overrides

// Implementation
protected:
	LRESULT OnWindowPosChanging (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnWindowPosChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentBalloonWnd)
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
	CFontHandle				mFont;
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
