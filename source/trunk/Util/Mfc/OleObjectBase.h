/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_OLEOBJECTBASE_H__3A8C7BDC_3802_47dc_B4B4_A33F00F16038__INCLUDED_)
#define AFX_OLEOBJECTBASE_H__3A8C7BDC_3802_47dc_B4B4_A33F00F16038__INCLUDED_
#pragma once

#include <comdef.h>

//////////////////////////////////////////////////////////////////////

class COleWindow
{
protected:
	COleWindow (CCmdTarget& pCmdTarget, CWnd * pWnd);

	BEGIN_INTERFACE_PART (OleWindow, IOleWindow)
        STDMETHOD (GetWindow) (HWND *phwnd);
        STDMETHOD (ContextSensitiveHelp) (BOOL fEnterMode);
	END_INTERFACE_PART (OleWindow)

protected:
	CCmdTarget &	mCmdTarget;
	CWnd *			mWnd;
};

//////////////////////////////////////////////////////////////////////

class CRunnableObject
{
protected:
	CRunnableObject (CCmdTarget& pCmdTarget);

	BEGIN_INTERFACE_PART (RunnableObject, IRunnableObject)
        STDMETHOD (GetRunningClass) (LPCLSID lpClsid);
        STDMETHOD (Run) (LPBINDCTX pbc);
        STDMETHOD_ (BOOL,IsRunning)();
        STDMETHOD (LockRunning) (BOOL fLock, BOOL fLastUnlockCloses);
        STDMETHOD (SetContainedObject) (BOOL fContained);
	END_INTERFACE_PART (RunnableObject)

protected:
	virtual HRESULT GetRunningClass (CLSID& pClassId);
	virtual HRESULT Run ();
	virtual bool IsRunning ();

protected:
	CCmdTarget &	mCmdTarget;
};

//////////////////////////////////////////////////////////////////////

class CRunnableSingleton : public CRunnableObject
{
protected:
	CRunnableSingleton (CCmdTarget& pCmdTarget, class CRotSingleton& pSingleton);

protected:
	virtual HRESULT GetRunningClass (CLSID& pClassId);
	virtual HRESULT Run ();

protected:
	class CRotSingleton&	mSingleton;
};

//////////////////////////////////////////////////////////////////////

extern HRESULT RunOleObject (LPUNKNOWN pUnknown);
extern HRESULT RunOleWindow (LPUNKNOWN pUnknown);
extern HRESULT ActivateOleWindow (LPUNKNOWN pUnknown);

extern HRESULT CoCreateInstanceElevated (REFCLSID pClassId, LPUNKNOWN* pUnknown, REFIID pInterfaceId = __uuidof(IUnknown), DWORD pClsCtx = CLSCTX_LOCAL_SERVER, LPUNKNOWN pOuterUnknown = NULL);
extern HRESULT CoCreateInstanceElevated (REFCLSID pClassId, LPUNKNOWN* pUnknown, HWND pParentWnd, REFIID pInterfaceId = __uuidof(IUnknown), DWORD pClsCtx = CLSCTX_LOCAL_SERVER, LPUNKNOWN pOuterUnknown = NULL);

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ROTSINGLETON_H__1482F2C5_6977_4CAF_8195_8D83F029CE76__INCLUDED_)
