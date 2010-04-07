/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef COMMESSAGEFILTER_H_INCLUDED_
#define COMMESSAGEFILTER_H_INCLUDED_
#pragma once

#include <atlbase.h>
#include <atlcom.h>

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CComMessageFilter :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IMessageFilter
{
public:
	CComMessageFilter ();
	~CComMessageFilter ();

// Declarations
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CComMessageFilter)
		COM_INTERFACE_ENTRY(IMessageFilter)
	END_COM_MAP()

// Interfaces
public:
	// IMessageFilter
	DWORD STDMETHODCALLTYPE HandleInComingCall (DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, LPINTERFACEINFO lpInterfaceInfo);
	DWORD STDMETHODCALLTYPE RetryRejectedCall (HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType);
	DWORD STDMETHODCALLTYPE MessagePending (HTASK htaskCallee, DWORD dwTickCount, DWORD dwPendingType);

// Attributes
public:
	bool IsRegistered () const {return mRegistered;}
	UINT GetDoNotDisturb () const {return mDndLevel;}
	bool IsCheckedOut () const {return mCheckedOut;}

// Operations
public:
	HRESULT Register ();
	HRESULT Revoke ();

	UINT DoNotDisturb (bool pDoNotDisturb);
	void CheckOut ();

// Implementation
protected:
	UINT ShowBusyDlg (HTASK pBlockedTask, bool pNotResponding);

protected:
	UINT						mDndLevel;
	bool						mCheckedOut;
	DWORD						mRetryTimeout;
	DWORD						mRetryImmediate;
	DWORD						mRetryLater;
	DWORD						mMessageTimeout;
private:
	bool						mRegistered;
	CComPtr <IMessageFilter>	mPrevFilter;
	bool						mInMessagePending;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(COMMESSAGEFILTER_H_INCLUDED_)
