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
#ifndef FILEDOWNLOAD_H_INCLUDED_
#define FILEDOWNLOAD_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "DainternalNotify.h"
#include "InstanceGate.h"

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)

class _DACORE_IMPEXP CFileDownload : public CCmdTarget, private CInstanceGate
{
public:
	CFileDownload (LPCTSTR pURL);
	virtual ~CFileDownload ();
	DECLARE_DYNAMIC (CFileDownload)

// Attributes
public:
	ULONG_PTR	mUserData;

	tBstrPtr GetURL () const;
	tBstrPtr GetCacheName () const;

	DWORD GetBindFlags () const;
	ULONG GetDownloadSize () const;
	ULONG GetDownloadProgress () const;

	bool IsDownloadStarting () const;
	bool IsDownloadStarted () const;
	bool IsDownloadCancelling () const;
	HRESULT IsDownloadComplete () const;

// Operations
public:
	DWORD SetBindFlags (DWORD pBindFlags);
	DWORD SetReloadMode (bool pReload);
	DWORD SetResynchronizeMode (bool pResynchronize);
	DWORD SetSecurityMode (bool pEnforeSecurity);

	HRESULT Download (LPUNKNOWN pActiveXContext = NULL, IDaNotify * pNotify = NULL);
	bool CancelDownload ();

// Overrides
	//{{AFX_VIRTUAL(CFileDownload)
	protected:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(BindStatusCallback, IBindStatusCallback)
		HRESULT STDMETHODCALLTYPE OnStartBinding (DWORD dwReserved, IBinding *pib);
		HRESULT STDMETHODCALLTYPE GetPriority (LONG *pnPriority);
		HRESULT STDMETHODCALLTYPE OnLowResource (DWORD reserved);
		HRESULT STDMETHODCALLTYPE OnProgress (ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText);
		HRESULT STDMETHODCALLTYPE OnStopBinding (HRESULT hresult, LPCWSTR szError);
		HRESULT STDMETHODCALLTYPE GetBindInfo (DWORD *grfBINDF, BINDINFO *pbindinfo);
		HRESULT STDMETHODCALLTYPE OnDataAvailable (DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed);
		HRESULT STDMETHODCALLTYPE OnObjectAvailable (REFIID riid, IUnknown *punk);
	END_INTERFACE_PART(BindStatusCallback)

	DECLARE_INTERFACE_MAP()

protected:
	static DWORD WINAPI AsyncThreadProc (LPVOID lpParameter);

protected:
	mutable ::CCriticalSection	mLock;
	CString						mURL;
	CString						mCacheName;
	DWORD						mBindFlags;
	ULONG						mDownloadSize;
	ULONG						mDownloadProgress;
	bool						mDownloadStarted;
	bool						mDownloadCancelling;
	HRESULT						mDownloadComplete;
	IDaNotify *					mNotify;
	DWORD						mNotifyThreadId;
	IBindStatusCallbackPtr		mBindStatusCallback;
	IStreamPtr					mBindStatusMarshall;
	IStreamPtr					mContextMarshall;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // AGENTFILE_H_INCLUDED_
