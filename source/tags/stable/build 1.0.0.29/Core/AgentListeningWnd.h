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
#ifndef AGENTLISTENINGWND_H_INCLUDED_
#define AGENTLISTENINGWND_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CAgentListeningWnd : public CToolTipCtrl
{
protected:
	CAgentListeningWnd ();
public:
	virtual ~CAgentListeningWnd ();
	DECLARE_DYNCREATE (CAgentListeningWnd)

// Attributes
public:
	long GetCharID () const {return mCharID;}
	long GetLangID () const {return mLangID;}

// Operations
public:
	bool Create (CWnd * pParentWnd);
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
	//{{AFX_VIRTUAL(CAgentListeningWnd)
	afx_msg void OnWindowPosChanging (WINDOWPOS *lpwndpos);
	afx_msg void OnWindowPosChanged (WINDOWPOS *lpwndpos);
	afx_msg _MFC_NCHITTEST_RESULT OnNcHitTest(CPoint point);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAgentListeningWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString GetCharacterCaption (UINT pCaptionStrId) const;
	CString GetCommandsCaption () const;
	CString GetCommandCaption (LPCTSTR pCommand) const;

	void ShowToolText (LPCTSTR pCaption, LPCTSTR pDetail);
	bool CalcWinRect (CRect & pWinRect);

protected:
	long					mCharID;
	LANGID					mLangID;
	CString					mCharacterName;
	CString					mCommandsCaption;
	CString					mCaption;
	CString					mDetail;
	tSS <TOOLINFO, UINT>	mToolInfo;
	CFont					mFont;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTLISTENINGWND_H_INCLUDED_
