/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "DaServerApp.h"
#include "DaCmnPropertySheet.h"
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E513-A208-11DE-ABF2-002421116FB2}")) DaSvrPropertySheet :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrPropertySheet, &__uuidof(DaSvrPropertySheet)>,
	public IDispatchImpl<IDaSvrPropertySheet2, &__uuidof(IDaSvrPropertySheet2), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrPropertySheet), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public IOleWindow,
	public CDaCmnPropertySheet,
	public CSvrObjLifetime
{
public:
	DaSvrPropertySheet ();
	virtual ~DaSvrPropertySheet ();

// Attributes
public:

// Operations
public:
	static DaSvrPropertySheet* CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void PreShowSheet ();
	virtual void OnFinalMessage (HWND);
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRPROPERTYSHEET)
	DECLARE_NOT_AGGREGATABLE(DaSvrPropertySheet)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrPropertySheet)
		COM_INTERFACE_ENTRY(IDaSvrPropertySheet2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrPropertySheet2)
		COM_INTERFACE_ENTRY2(IDaSvrPropertySheet, IDaSvrPropertySheet2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentPropertySheet), IDaSvrPropertySheet2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IOleWindow)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrPropertySheet)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrPropertySheet2
	HRESULT STDMETHODCALLTYPE GetVisible (long *Visible);
	HRESULT STDMETHODCALLTYPE SetVisible (long Visible);
	HRESULT STDMETHODCALLTYPE GetPosition (long *Left, long *Top);
	HRESULT STDMETHODCALLTYPE GetSize (long *Width, long *Height);
	HRESULT STDMETHODCALLTYPE GetPage (BSTR *Page);
	HRESULT STDMETHODCALLTYPE SetPage (BSTR Page);

	HRESULT STDMETHODCALLTYPE put_Left (short Left);
	HRESULT STDMETHODCALLTYPE get_Left (short *Left);
	HRESULT STDMETHODCALLTYPE put_Top (short Top);
	HRESULT STDMETHODCALLTYPE get_Top (short *Top);
	HRESULT STDMETHODCALLTYPE get_Height (short *Height);
	HRESULT STDMETHODCALLTYPE get_Width (short *Width);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
	HRESULT STDMETHODCALLTYPE put_Page (BSTR Page);
	HRESULT STDMETHODCALLTYPE get_Page (BSTR *Page);
	HRESULT STDMETHODCALLTYPE SetPosition (long Left, long Top);

	// IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);

// Implementation
protected:
	void Disconnect (bool pAbandonned);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrPropertySheet), DaSvrPropertySheet)

/////////////////////////////////////////////////////////////////////////////
