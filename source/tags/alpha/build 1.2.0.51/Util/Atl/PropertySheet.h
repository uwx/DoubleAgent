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
#ifndef PROPERTYSHEET_H_INCLUDED_
#define PROPERTYSHEET_H_INCLUDED_
#pragma once

#include "PropertyPage.h"

/////////////////////////////////////////////////////////////////////////////

interface _AtlPropSheetOwner
{
	virtual void OnShowModelessPropertySheet (class CAtlPropertySheet* pPropertySheet) = 0;
	virtual void OnHideModelessPropertySheet (class CAtlPropertySheet* pPropertySheet) = 0;
};

/////////////////////////////////////////////////////////////////////////////

class CAtlPropertySheet : public CWindowImpl<CAtlPropertySheet>
{
public:
	CAtlPropertySheet ();
	virtual ~CAtlPropertySheet();

// Attributes
public:
	bool IsModal () const;

// Operations
public:
	bool Create (_AtlPropSheetOwner* pOwner = NULL);
	void SetModalParent (HWND pParentWnd);
	INT_PTR DoModal();

// Implementation
protected:
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	BEGIN_MSG_MAP(CAtlPropertySheet)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(PSM_CHANGED, OnChange)
		COMMAND_ID_HANDLER(IDOK, OnOk);
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel);
		COMMAND_ID_HANDLER(0x3021, OnApply);
	END_MSG_MAP()

protected:
	void Construct (UINT pCaptionId, HWND pParentWnd = NULL);
	virtual bool PreCreateSheet (bool pModal);
	virtual void PreDestroySheet ();
	virtual void PreShowSheet ();
	virtual void PreHideSheet ();

	virtual void LoadConfig ();
	virtual void SaveConfig (int pSheetResult);
	virtual void OnChanged ();
	virtual void OnApplied ();

private:
	void InitThunkForCallback ();
	static int CALLBACK PropSheetCallbackModeless (HWND hwnd, UINT message, LPARAM lParam);
	static int CALLBACK PropSheetCallbackModal (HWND hwnd, UINT message, LPARAM lParam);

protected:
	CString								mCaption;
	tSS <PROPSHEETHEADER, DWORD>		mPsh;
	tArrayPtr <PROPSHEETPAGE>			mPsp;
	CAtlOwnPtrArray <CAtlPropertyPage>	mPages;
	bool								mPageChanged;
private:
	bool								mIsModal;
	_AtlPropSheetOwner*				mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPERTYSHEET_H_INCLUDED_
