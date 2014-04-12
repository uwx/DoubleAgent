/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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

/////////////////////////////////////////////////////////////////////////////

enum DaRequestCategory
{
	DaRequestLoad = 1,
	DaRequestShow,
	DaRequestHide,
	DaRequestPlay,
	DaRequestSpeak,
	DaRequestPrepare,
	DaRequestOther,
	DaRequestCategoryMask = 0x00000FFF,
	DaRequestNotifyEnabled = 0x00001000,
	DaRequestNotifyStart = 0x00002000,
	DaRequestNotifyComplete = 0x00004000,
	DaRequestNotifyMask = 0x00007000
};

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E536-A208-11DE-ABF2-002421116FB2}")) DaCtlRequest :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlRequest, &__uuidof(DaCtlRequest)>,
	public IDispatchImpl<IDaCtlRequest, &__uuidof(IDaCtlRequest), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlRequest), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlRequest();
	~DaCtlRequest();
// Attributes
public:
	DaRequestCategory	mCategory;
	long				mReqID;
	long				mStatus;
	HRESULT				mResult;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (class DaControl * pOwner, DaRequestCategory pCategory, long pReqID, HRESULT pResult = S_OK);
	class DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	CAtlString StatusStr () const;
	CAtlString CategoryStr () const;

	friend CAtlString RequestStatusStr (long pStatus);
	friend CAtlString RequestCategoryStr (DaRequestCategory pCategory);

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLREQUEST)
	DECLARE_NOT_AGGREGATABLE(DaCtlRequest)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlRequest)
		COM_INTERFACE_ENTRY(IDaCtlRequest)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlRequest)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlRequest), IDaCtlRequest)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlRequest)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlRequest
	HRESULT STDMETHODCALLTYPE get_ID (long* ID);
	HRESULT STDMETHODCALLTYPE get_Status (long* Status);
	HRESULT STDMETHODCALLTYPE get_Description (BSTR * Description);
	HRESULT STDMETHODCALLTYPE get_Number (long* Number);

// Implementation
private:
	class DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlRequest), DaCtlRequest)

/////////////////////////////////////////////////////////////////////////////
