/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaControl.h"
#include "DaControlObj.h"

class ATL_NO_VTABLE __declspec(uuid("{1147E539-A208-11DE-ABF2-002421116FB2}")) CDaCtlPropertySheet :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlPropertySheet, &__uuidof(DaCtlPropertySheet)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlPropertySheet), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlPropertySheet, &__uuidof(IDaCtlPropertySheet), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlPropertySheet();
	~CDaCtlPropertySheet();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLPROPERTYSHEET)
	DECLARE_NOT_AGGREGATABLE(CDaCtlPropertySheet)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlPropertySheet)
		COM_INTERFACE_ENTRY(IDaCtlPropertySheet)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlPropertySheet)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlPropertySheet), IDaCtlPropertySheet)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlPropertySheet)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlPropertySheet
	STDMETHOD(get_Left)(short * Left);
	STDMETHOD(get_Top)(short * Top);
	STDMETHOD(get_Height)(short * Height);
	STDMETHOD(get_Width)(short * Width);
	STDMETHOD(put_Visible)(VARIANT_BOOL Visible);
	STDMETHOD(get_Visible)(VARIANT_BOOL * Visible);
	STDMETHOD(put_Page)(BSTR Page);
	STDMETHOD(get_Page)(BSTR * Page);

// Attributes
public:

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (CDaControlObj * pOwner);
	CDaControlObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Implementation
public:
	IDaSvrPropertySheetPtr	mServerObject;
private:
	CDaControlObj *			mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlPropertySheet), CDaCtlPropertySheet)

/////////////////////////////////////////////////////////////////////////////
