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
#include "AgentCharacterWnd.h"
#include "AgentNotifyIcon.h"
#include "AgentListeningWnd.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentPopupObjectRoot : public CComObjectRootEx<CComMultiThreadModel> {};

class ATL_NO_VTABLE CAgentPopupWnd :
	public CAgentPopupObjectRoot,
	public CAgentCharacterWnd,
	public IOleWindow
{
	DECLARE_DLL_OBJECT_EX(CAgentPopupWnd, _DACORE_IMPEXP)
protected:
	CAgentPopupWnd ();
public:
	_DACORE_IMPEXP virtual ~CAgentPopupWnd ();
	_DACORE_IMPEXP static CAgentPopupWnd* CreateInstance ();

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
	_DACORE_IMPEXP bool IsDragging () const;

// Operations
public:
	_DACORE_IMPEXP bool Create (CWindow* pParentWnd, CRect* pInitialRect = NULL, DWORD pExStyle = 0);
	_DACORE_IMPEXP virtual bool Attach (long pCharID, CEventNotify* pNotify, const CAgentIconData* pIconData, bool pSetActiveCharID);
	_DACORE_IMPEXP virtual bool Detach (long pCharID, CEventNotify* pNotify);
	_DACORE_IMPEXP void FinalRelease ();

	_DACORE_IMPEXP bool ShowPopup (long pForCharID, VisibilityCauseType pVisiblityCause, bool pAlwaysNotify = false);
	_DACORE_IMPEXP bool HidePopup (long pForCharID, VisibilityCauseType pVisiblityCause, bool pAlwaysNotify = false);
	_DACORE_IMPEXP bool MovePopup (const CPoint& pPosition, long pForCharID, MoveCauseType pMoveCause, bool pAlwaysNotify = false);
	_DACORE_IMPEXP bool SizePopup (const CSize& pSize, long pForCharID, bool pAlwaysNotify = false);

	_DACORE_IMPEXP long QueueMove (long pCharID, const CPoint& pPosition, DWORD pSpeed);

	_DACORE_IMPEXP bool IsNotifyIconValid () const;
	_DACORE_IMPEXP bool IsIconVisible () const;
	_DACORE_IMPEXP bool UpdateNotifyIcon (const CAgentIconData* pIconData = NULL);
	_DACORE_IMPEXP bool SetNotifyIconTip (const CAgentIconData* pIconData, CAgentFile* pAgentFile, LANGID pLangID = 0);

// Overrides
public:
	_DACORE_IMPEXP virtual DWORD GetAlphaSmoothing () const;
protected:
	_DACORE_IMPEXP virtual bool CanDoAnimationQueue ();
	_DACORE_IMPEXP virtual void Opened ();
	_DACORE_IMPEXP virtual void Closing ();
	_DACORE_IMPEXP virtual void IsLastActive (bool pLastActive);
	_DACORE_IMPEXP virtual bool ShowQueued (CQueuedShow* pQueuedShow);
	_DACORE_IMPEXP virtual bool HideQueued (CQueuedHide* pQueuedHide);
	_DACORE_IMPEXP virtual int _PreDoQueue ();
	_DACORE_IMPEXP virtual int _PostDoQueue ();
	_DACORE_IMPEXP virtual bool _PreNotify ();
	_DACORE_IMPEXP virtual bool _PostNotify ();
	_DACORE_IMPEXP virtual void OnFinalMessage (HWND);

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnEnterSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnExitSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMoving (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNotifyIcon (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnTaskbarCreated (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

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
	void OnIconDblClick (const CPoint& pPoint);

protected:
	tPtr <CPoint>		mSizeMoveStart;
	bool				mIsDragging;
	bool				mWasDragged;
	CAgentNotifyIcon	mNotifyIcon;
};

/////////////////////////////////////////////////////////////////////////////
