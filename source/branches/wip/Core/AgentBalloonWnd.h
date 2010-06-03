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
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "AgentText.h"
#include "AgentBalloonShape.h"
#include "SapiVoiceEventSink.h"
#include "EventNotify.h"
#include "ImageBuffer.h"
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275 4150)
/////////////////////////////////////////////////////////////////////////////

struct _DACORE_IMPEXP CAgentBalloonOptions
{
	DECLARE_DLL_OBJECT(CAgentBalloonOptions)

	DWORD		mStyle;
	USHORT		mLines;
	USHORT		mPerLine;
	COLORREF	mBkColor;
	COLORREF	mFgColor;
	COLORREF	mBrColor;
	LOGFONT		mFont;

	CAgentBalloonOptions ();
	CAgentBalloonOptions (const CAgentBalloonOptions & pSource);
	CAgentBalloonOptions & operator= (const CAgentBalloonOptions & pSource);
	bool operator== (const CAgentBalloonOptions & pSource) const;
	bool operator!= (const CAgentBalloonOptions & pSource) const;
};

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentBalloonWndObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IUnknown,
	public CWindowImpl<CAgentBalloonWndObj, CWindow, CWinTraits<WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE> >
{
// Declarations
public:
	DECLARE_WND_SUPERCLASS(NULL, TOOLTIPS_CLASS)
	DECLARE_NOT_AGGREGATABLE(CAgentBalloonWndObj)
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(CAgentBalloonWndObj)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP ATL_NO_VTABLE CAgentBalloonWnd :
	public CAgentBalloonWndObj,
	public CAgentFileClient,
	public CEventNotifiesClient<CAgentBalloonWnd>,
	public _ISapiVoiceEventSink
{
	DECLARE_DLL_OBJECT(CAgentBalloonWnd)
protected:
	CAgentBalloonWnd ();
public:
	virtual ~CAgentBalloonWnd ();
	static CAgentBalloonWnd * CreateInstance (long pCharID, CAtlPtrTypeArray <CEventNotify> & pNotify);

// Declarations
public:
	DECLARE_PROTECT_FINAL_RELEASE(CAgentBalloonWndObj)

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
	bool ClipPartialLines () const;

	static const USHORT	mMinLines;
	static const USHORT	mMaxLines;
	static const USHORT	mDefLines;
	static const USHORT	mMinPerLine;
	static const USHORT	mMaxPerLine;
	static const USHORT	mDefPerLine;
	const DWORD			mAutoPaceTime;
	DWORD				mAutoHideTime;

// Operations
public:
	bool SetOptions (DWORD pStyle, const CAgentFileBalloon & pFileBalloon, LANGID pLangID = 0);
	CAgentBalloonOptions * GetNextOptions () const;
	bool ApplyOptions (CAgentBalloonOptions * pOptions = NULL);

	bool Create (CWindow * pOwnerWnd);
	bool Attach (long pCharID, class CEventNotify * pNotify, bool pSetActiveCharID);
	bool Detach (long pCharID, class CEventNotify * pNotify);
	void FinalRelease ();

	bool ShowBalloonSpeech (LPCTSTR pText, UINT pSapiVersion = 5, bool pNoAutoPace = false);
	bool ShowBalloonSpeech (const CAgentText & pText, bool pNoAutoPace = false);
	bool ShowBalloonThought (LPCTSTR pText, UINT pSapiVersion = 5, bool pNoAutoPace = false);
	bool ShowBalloonThought (const CAgentText & pText, bool pNoAutoPace = false);
	bool ShowBalloonNow ();
	bool ShowBalloonAuto ();
	bool HideBalloon (bool pFast = false);
	bool MoveBalloon ();

	CAtlString GetDisplayText ();
	CAtlString GetSpeechText ();
	bool AbortSpeechText ();

	static bool CopyBalloonFont (const CAgentFileBalloon & pFileBalloon, LOGFONT & pFont);
	static bool CopyBalloonFont (const LOGFONT & pFont, CAgentFileBalloon & pFileBalloon);
	static bool SetFontLangID (LOGFONT & pFont, LANGID pLangID);
	static bool GetActualFont (const LOGFONT & pFont, LOGFONT & pActualFont, bool pUpdateSize = true, bool pUpdateStyle = true);

// Overrides
protected:
	virtual void OnVoiceStart (long pCharID);
	virtual void OnVoiceEnd (long pCharID);
	virtual void OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength);
	virtual void OnFinalMessage (HWND);
	virtual bool _PreNotify ();
	virtual bool _PostNotify ();

// Implementation
protected:
	LRESULT OnShow(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnPrint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnWindowPosChanging (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnWindowPosChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceWordMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentBalloonWnd)
		REFLECTED_NOTIFY_CODE_HANDLER(TTN_SHOW, OnShow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PRINT, OnPrint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPrintClient)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(mVoiceStartMsg, OnVoiceStartMsg)
		MESSAGE_HANDLER(mVoiceEndMsg, OnVoiceEndMsg)
		MESSAGE_HANDLER(mVoiceWordMsg, OnVoiceWordMsg)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

protected:
	bool ShowingBalloon ();
	bool ShowBalloonText (const CAgentText & pText, UINT pForSpeech, bool pNoAutoPace);
	bool ShowBalloon (bool pForSpeech, bool pTextChanged);
	void ShowedBalloon (bool pWasVisible);
	bool CalcLayoutRects (CRect & pTextRect, CRect & pOwnerRect, CRect & pBounds);
	bool CalcWinRect (CRect & pWinRect, bool pOnShow = false);
	void ApplyLayout (const CRect & pWinRect, bool pOnShow = false);
	bool ApplyRegion (bool pRedraw = true);

	bool StartAutoPace ();
	bool StopAutoPace ();
	bool StartAutoHide ();
	bool StopAutoHide ();
	bool StartAutoScroll ();
	bool StopAutoScroll ();

	void DrawBalloon (HDC pDC, const CRect & pDrawRect);
	void DrawBalloonText (HDC pDC, const CRect & pDrawRect);
	DWORD ApplyFontLayout (HDC pDC);
	void ShowedVoiceWord (bool pFastRefresh);

protected:
	CAgentBalloonOptions				mOptions;
	tPtr <CAgentBalloonOptions>			mPendingOptions;
	CFontHandle							mFont;
	tSS <TOOLINFO, UINT>				mToolInfo;
	CAgentTextDraw						mText;
	bool								mAutoPaceDisabled;
	UINT_PTR							mAutoPaceTimer;
	UINT_PTR							mAutoHideTimer;
	UINT_PTR							mAutoScrollTimer;
	tPtr <class CAgentBalloonShape>		mShape;
	tPtr <CSize>						mShapeSize;
	CImageBuffer						mShapeBuffer;
	CImageBuffer						mDrawBuffer;
	long								mCharID;
	LANGID								mLangID;
	static UINT							mVoiceStartMsg;
	static UINT							mVoiceEndMsg;
	static UINT							mVoiceWordMsg;
private:
	CWindow *							mOwnerWnd;
	bool								mPacingSpeech;
	bool								mPacingWord;
	bool								mApplyingLayout;
	bool								mApplyingRegion;

private:
	UINT EnterRecursion () const;
	UINT ExitRecursion () const;
	CAtlString RecursionIndent () const;
	mutable UINT mDebugRecursionLevel;
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
