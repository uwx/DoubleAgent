/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControlMod.h"
#include "DaControl.h"
#include "DaCmnPropertySheet.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E539-A208-11DE-ABF2-002421116FB2}")) DaCtlPropertySheet :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlPropertySheet, &__uuidof(DaCtlPropertySheet)>,
	public IDispatchImpl<IDaCtlPropertySheet2, &__uuidof(IDaCtlPropertySheet2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlPropertySheet), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlPropertySheet();
	~DaCtlPropertySheet();

// Attributes
public:
	IDaSvrPropertySheet2Ptr		mServerObject;
	tPtr <CDaCmnPropertySheet>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);
	void Disconnect (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLPROPERTYSHEET)
	DECLARE_NOT_AGGREGATABLE(DaCtlPropertySheet)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlPropertySheet)
		COM_INTERFACE_ENTRY(IDaCtlPropertySheet2)
		COM_INTERFACE_ENTRY2(IDaCtlPropertySheet, IDaCtlPropertySheet2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlPropertySheet)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlPropertySheet), IDaCtlPropertySheet)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlPropertySheet)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlPropertySheet
	HRESULT STDMETHODCALLTYPE get__Left (short * Left);
	HRESULT STDMETHODCALLTYPE get__Top (short * Top);
	HRESULT STDMETHODCALLTYPE get_Height (short * Height);
	HRESULT STDMETHODCALLTYPE get_Width (short * Width);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL * Visible);
	HRESULT STDMETHODCALLTYPE put_Page (BSTR Page);
	HRESULT STDMETHODCALLTYPE get_Page (BSTR * Page);
	HRESULT STDMETHODCALLTYPE put_Left (short Left);
	HRESULT STDMETHODCALLTYPE get_Left (short * Left);
	HRESULT STDMETHODCALLTYPE put_Top (short Top);
	HRESULT STDMETHODCALLTYPE get_Top (short * Top);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlPropertySheet), DaCtlPropertySheet)

/////////////////////////////////////////////////////////////////////////////
