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
#include "AgentCharacterWnd.h"
#include "AgentNotifyIcon.h"
#include "AgentListeningWnd.h"
#include "SapiVoiceEventSink.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275 4150)
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentPopupObjectRoot : public CComObjectRootEx<CComMultiThreadModel> {};

class _DACORE_IMPEXP ATL_NO_VTABLE CAgentPopupWnd :
	public CAgentPopupObjectRoot,
	public CAgentCharacterWnd,
	public IOleWindow
{
	DECLARE_DLL_OBJECT(CAgentPopupWnd)
protected:
	CAgentPopupWnd ();
public:
	virtual ~CAgentPopupWnd ();
	static CAgentPopupWnd * CreateInstance ();

// Declarations
public:
	DECLARE_NOT_AGGREGATABLE(CAgentPopupWnd)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_PROTECT_FINAL_RELEASE(CAgentPopupObjectRoot)
	DECLARE_WND_CLASS_EX(NULL, CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_NOCLOSE, COLOR_WINDOW)

	BEGIN_COM_MAP(CAgentPopupWnd)
		COM_INTERFACE_ENTRY(IOleWindow)
	END_COM_MAP()

// Interfaces
public:
	// IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);

// Attributes
public:
	bool IsDragging () const;

// Operations
public:
	bool Create (CWindow * pParentWnd, CRect * pInitialRect = NULL, DWORD pExStyle = 0);
	virtual bool Attach (long pCharID, CEventNotify * pNotify, const CAgentIconData * pIconData, bool pSetActiveCharID);
	virtual bool Detach (long pCharID, CEventNotify * pNotify);
	void FinalRelease ();

	bool ShowPopup (long pForCharID, VisibilityCauseType pVisiblityCause, bool pAlwaysNotify = false);
	bool HidePopup (long pForCharID, VisibilityCauseType pVisiblityCause, bool pAlwaysNotify = false);
	bool MovePopup (const CPoint & pPosition, long pForCharID, MoveCauseType pMoveCause, bool pAlwaysNotify = false);
	bool SizePopup (const CSize & pSize, long pForCharID, bool pAlwaysNotify = false);

	long QueueMove (long pCharID, const CPoint & pPosition, DWORD pSpeed);

	bool IsNotifyIconValid () const;
	bool IsIconVisible () const;
	bool UpdateNotifyIcon (const CAgentIconData * pIconData = NULL);
	bool SetNotifyIconTip (const CAgentIconData * pIconData, CAgentFile * pAgentFile, LANGID pLangID = 0);

// Overrides
public:
	virtual DWORD GetAlphaSmoothing () const;
protected:
	virtual bool CanDoAnimationQueue ();
	virtual void Opened ();
	virtual void Closing ();
	virtual void IsLastActive (bool pLastActive);
	virtual bool ShowQueued (CQueuedShow * pQueuedShow);
	virtual bool HideQueued (CQueuedHide * pQueuedHide);
	virtual int _PreDoQueue ();
	virtual int _PostDoQueue ();
	virtual bool _PreNotify ();
	virtual bool _PostNotify ();
	virtual void OnFinalMessage (HWND);

// Implementation
protected:
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnEnterSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnExitSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMoving (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNotifyIcon (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnTaskbarCreated (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentPopupWnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
		MESSAGE_HANDLER(WM_NCLBUTTONUP, OnNcLButtonUp)
		MESSAGE_HANDLER(WM_NCLBUTTONDBLCLK, OnNcLButtonDblClk)
		MESSAGE_HANDLER(WM_NCRBUTTONDOWN, OnNcRButtonDown)
		MESSAGE_HANDLER(WM_NCRBUTTONUP, OnNcRButtonUp)
		MESSAGE_HANDLER(WM_NCRBUTTONDBLCLK, OnNcRButtonDblClk)
		MESSAGE_HANDLER(WM_NCMBUTTONDOWN, OnNcMButtonDown)
		MESSAGE_HANDLER(WM_NCMBUTTONUP, OnNcMButtonUp)
		MESSAGE_HANDLER(WM_NCMBUTTONDBLCLK, OnNcMButtonDblClk)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(CAgentNotifyIcon::mNotifyIconMsg, OnNotifyIcon)
		MESSAGE_HANDLER(CAgentNotifyIcon::mTaskbarCreatedMsg, OnTaskbarCreated)
		CHAIN_MSG_MAP(CAgentCharacterWnd)
	END_MSG_MAP()

protected:
	bool DoQueuedMoveCycle ();
	void OnIconDblClick (const CPoint & pPoint);

protected:
	tPtr <CPoint>		mSizeMoveStart;
	bool				mIsDragging;
	bool				mWasDragged;
	CAgentNotifyIcon	mNotifyIcon;
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
