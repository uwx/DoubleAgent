/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "DaCmnCommandsWindow.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E53B-A208-11DE-ABF2-002421116FB2}")) DaCtlCommandsWindow :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCommandsWindow, &__uuidof(DaCtlCommandsWindow)>,
	public IDispatchImpl<IDaCtlCommandsWindow, &__uuidof(IDaCtlCommandsWindow), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommandsWindow), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlCommandsWindow();
	~DaCtlCommandsWindow();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrCommandsWindow2Ptr	mServerObject;
#endif
	tPtr <CDaCmnCommandsWindow>	mLocalObject;

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
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCOMMANDSWINDOW)
	DECLARE_NOT_AGGREGATABLE(DaCtlCommandsWindow)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlCommandsWindow)
		COM_INTERFACE_ENTRY(IDaCtlCommandsWindow)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCommandsWindow)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommandsWindow), IDaCtlCommandsWindow)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCommandsWindow)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlCommandsWindow
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL * Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_Left (short * Left);
	HRESULT STDMETHODCALLTYPE get_Top (short * Top);
	HRESULT STDMETHODCALLTYPE get_Height (short * Height);
	HRESULT STDMETHODCALLTYPE get_Width (short * Width);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCommandsWindow), DaCtlCommandsWindow)

/////////////////////////////////////////////////////////////////////////////
