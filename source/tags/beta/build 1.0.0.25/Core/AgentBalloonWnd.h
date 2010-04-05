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
#ifndef AGENTBALLOONWND_H_INCLUDED_
#define AGENTBALLOONWND_H_INCLUDED_
#pragma once

#include "AgentFile.h"
#include "AgentText.h"
#include "BitmapBuffer.h"
#include "SapiVoiceEventSink.h"
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

interface IDaNotify;

class _DACORE_IMPEXP CAgentBalloonWnd : public CToolTipCtrl, public ISapiVoiceEventSink
{
protected:
	CAgentBalloonWnd ();
public:
	virtual ~CAgentBalloonWnd ();
	DECLARE_DYNCREATE (CAgentBalloonWnd)

// Attributes
public:
	long GetCharID () const;
	LANGID GetLangID () const;

	bool IsSpeechShape () const;
	bool IsThoughtShape () const;
	bool IsBusy (bool pForIdle) const;
	bool IsDrawingLayered () const;

	bool IsAutoSize () const;
	bool IsAutoPace () const;
	bool IsAutoHide () const;

	static const USHORT	mMinLines;
	static const USHORT	mMaxLines;
	static const USHORT	mDefLines;
	static const USHORT	mMinPerLine;
	static const USHORT	mMaxPerLine;
	static const USHORT	mDefPerLine;
	const DWORD			mAutoPaceTime;
	DWORD				mAutoHideTime;
	const bool			mClipPartialLines;

// Operations
public:
	bool SetOptions (const CAgentFileBalloon & pFileBalloon, IDaSvrBalloon * pCharBalloon, LANGID pLangID = 0);
	bool CommitOptions ();

	bool Create (CWnd * pParentWnd);
	bool Attach (long pCharID, IDaNotify * pNotify, bool pSetActiveCharID);
	bool Detach (long pCharID, IDaNotify * pNotify);

	bool ShowBalloonSpeech (LPCTSTR pText, UINT pSapiVersion = 5, bool pNoAutoPace = false);
	bool ShowBalloonSpeech (const CAgentText & pText, bool pNoAutoPace = false);
	bool ShowBalloonThought (LPCTSTR pText, UINT pSapiVersion = 5, bool pNoAutoPace = false);
	bool ShowBalloonThought (const CAgentText & pText, bool pNoAutoPace = false);
	bool ShowBalloonNow ();
	bool ShowBalloonAuto ();
	bool HideBalloon (bool pFast = false);
	bool MoveBalloon ();

	CString GetDisplayText ();
	CString GetSpeechText ();
	bool AbortSpeechText ();

	static bool CopyBalloonFont (const CAgentFileBalloon & pFileBalloon, LOGFONT & pFont);
	static bool CopyBalloonFont (const LOGFONT & pFont, CAgentFileBalloon & pFileBalloon);
	static bool CopyBalloonFont (IDaSvrBalloon * pCharBalloon, LOGFONT & pFont);
	static bool SetFontLangID (LOGFONT & pFont, LANGID pLangID);
	static bool GetActualFont (const LOGFONT & pFont, LOGFONT & pActualFont, bool pUpdateSize = true, bool pUpdateStyle = true);

// Overrides
	//{{AFX_VIRTUAL(CAgentBalloonWnd)
	public:
	virtual void OnFinalRelease ();
	protected:
	virtual void OnVoiceStart (long pCharID);
	virtual void OnVoiceEnd (long pCharID);
	virtual void OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAgentBalloonWnd)
	afx_msg BOOL OnShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	afx_msg LRESULT OnPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanging (WINDOWPOS *lpwndpos);
	afx_msg void OnWindowPosChanged (WINDOWPOS *lpwndpos);
	afx_msg void OnSize (UINT nType, int cx, int cy);
	afx_msg _MFC_NCHITTEST_RESULT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	bool ShowingBalloon ();
	bool ShowBalloonText (const CAgentText & pText, UINT pForSpeech, bool pNoAutoPace);
	bool ShowBalloon (bool pForSpeech, bool pTextChanged);
	void ShowedBalloon (bool pWasVisible);
	bool CalcLayoutRects (CRect & pTextRect, CRect & pOwnerRect, CRect & pBounds);
	bool CalcWinRect (CRect & pWinRect, bool pOnShow = false);
	void ApplyLayout (const CRect & pWinRect, bool pOnShow = false);
	bool ApplyRegion (bool pRedraw = true);
	bool ApplyOptions ();

	bool StartAutoPace ();
	bool StopAutoPace ();
	bool StartAutoHide ();
	bool StopAutoHide ();
	bool StartAutoScroll ();
	bool StopAutoScroll ();

	void DrawBalloon (HDC pDC, const CRect & pDrawRect);
	void DrawBalloonText (HDC pDC, const CRect & pDrawRect);
	DWORD ApplyFontLayout (HDC pDC);
	void ShowedVoiceWord ();

public:
	bool PreNotify ();
	bool PostNotify ();
	UINT IsInNotify () const;

protected:
	struct CBalloonOptions
	{
		DWORD						mStyle;
		USHORT						mLines;
		USHORT						mPerLine;
		COLORREF					mBkColor;
		COLORREF					mFgColor;
		COLORREF					mBrColor;
		LOGFONT						mFont;
		
		CBalloonOptions ();
		CBalloonOptions (const CBalloonOptions & pSource);
		CBalloonOptions & operator= (const CBalloonOptions & pSource);
		bool operator== (const CBalloonOptions & pSource) const;
		bool operator!= (const CBalloonOptions & pSource) const;
	};
	
protected:
	CBalloonOptions					mOptions;
	tPtr <CBalloonOptions>			mNextOptions;
	CFont							mFont;
	tSS <TOOLINFO, UINT>			mToolInfo;
	CAgentText						mText;
	bool							mAutoPaceDisabled;
	UINT_PTR						mAutoPaceTimer;
	UINT_PTR						mAutoHideTimer;
	UINT_PTR						mAutoScrollTimer;
	tPtr <class CAgentBalloonShape>	mShape;
	tPtr <CSize>					mShapeSize;
	CBitmapBuffer					mShapeBuffer;
	CBitmapBuffer					mDrawBuffer;
	long							mCharID;
	LANGID							mLangID;
	CPtrTypeArray <IDaNotify>		mNotify;
private:
	UINT							mInNotify;
	bool							mPacingSpeech;
	bool							mPacingWord;
	bool							mApplyingLayout;
	bool							mApplyingRegion;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTBALLOONWND_H_INCLUDED_
