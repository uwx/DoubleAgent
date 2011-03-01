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
#include "DaCoreExp.h"
#include "InstanceGate.h"

//////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CFileDownloadObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IBindStatusCallback
{
// Declarations
public:
	DECLARE_NOT_AGGREGATABLE(CFileDownloadObj)

	BEGIN_COM_MAP(CFileDownloadObj)
		COM_INTERFACE_ENTRY(IBindStatusCallback)
	END_COM_MAP()

};

//////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CFileDownload :
	public CFileDownloadObj,
	private CInstanceGate

{
	DECLARE_DLL_OBJECT_EX(CFileDownload, _DACORE_IMPEXP)
protected:
	CFileDownload ();
public:
	_DACORE_IMPEXP virtual ~CFileDownload ();
	_DACORE_IMPEXP static CFileDownload * CreateInstance (LPCTSTR pURL);

// Attributes
public:
	ULONG_PTR	mUserData;

	_DACORE_IMPEXP tBstrPtr GetURL () const;
	_DACORE_IMPEXP tBstrPtr GetCacheName () const;

	_DACORE_IMPEXP DWORD GetBindFlags () const;
	_DACORE_IMPEXP ULONG GetDownloadSize () const;
	_DACORE_IMPEXP ULONG GetDownloadProgress () const;

	_DACORE_IMPEXP bool IsDownloadStarting () const;
	_DACORE_IMPEXP bool IsDownloadStarted () const;
	_DACORE_IMPEXP bool IsDownloadCancelling () const;
	_DACORE_IMPEXP HRESULT IsDownloadComplete () const;

// Operations
public:
	_DACORE_IMPEXP DWORD SetBindFlags (DWORD pBindFlags);
	_DACORE_IMPEXP DWORD SetReloadMode (bool pReload);
	_DACORE_IMPEXP DWORD SetResynchronizeMode (bool pResynchronize);
	_DACORE_IMPEXP DWORD SetSecurityMode (bool pEnforeSecurity);

	_DACORE_IMPEXP HRESULT Download (LPUNKNOWN pActiveXContext = NULL, class CEventNotify * pNotify = NULL);
	_DACORE_IMPEXP bool CancelDownload ();

// Overrides

// Interfaces
public:
	// IBindStatusCallback
	HRESULT STDMETHODCALLTYPE OnStartBinding (DWORD dwReserved, IBinding *pib);
	HRESULT STDMETHODCALLTYPE GetPriority (LONG *pnPriority);
	HRESULT STDMETHODCALLTYPE OnLowResource (DWORD reserved);
	HRESULT STDMETHODCALLTYPE OnProgress (ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText);
	HRESULT STDMETHODCALLTYPE OnStopBinding (HRESULT hresult, LPCWSTR szError);
	HRESULT STDMETHODCALLTYPE GetBindInfo (DWORD *grfBINDF, BINDINFO *pbindinfo);
	HRESULT STDMETHODCALLTYPE OnDataAvailable (DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed);
	HRESULT STDMETHODCALLTYPE OnObjectAvailable (REFIID riid, IUnknown *punk);

// Implementation
protected:
	static DWORD WINAPI AsyncThreadProc (LPVOID lpParameter);

protected:
	mutable CComAutoCriticalSection	mLock;
	CAtlString						mURL;
	CAtlString						mCacheName;
	DWORD							mBindFlags;
	ULONG							mDownloadSize;
	ULONG							mDownloadProgress;
	bool							mDownloadStarted;
	bool							mDownloadCancelling;
	HRESULT							mDownloadComplete;
	class CEventNotify *			mNotify;
	DWORD							mNotifyThreadId;
	IBindStatusCallbackPtr			mBindStatusCallback;
	IStreamPtr						mBindStatusMarshall;
	IStreamPtr						mContextMarshall;
};

//////////////////////////////////////////////////////////////////////
