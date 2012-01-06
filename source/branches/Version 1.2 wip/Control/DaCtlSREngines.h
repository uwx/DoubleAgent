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
#include "DaCmnSrEngines.h"
#include "CorEnumVariant.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E556-A208-11DE-ABF2-002421116FB2}")) DaCtlSREngines :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlSREngines, &__uuidof(DaCtlSREngines)>,
	public IDispatchImpl<IDaCtlSREngines, &__uuidof(IDaCtlSREngines), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSREngines), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlSREngines();
	~DaCtlSREngines();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrSREnginesPtr					mServerObject;
#endif
	tPtr <CDaCmnSREngines>				mLocalObject;
	CInterfaceArray <IDaCtlSREngine>	mSREngines;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);
	void Disconnect (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	void InitializeObjects ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSRENGINES)
	DECLARE_NOT_AGGREGATABLE(DaCtlSREngines)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(DaCtlSREngines)
		COM_INTERFACE_ENTRY(IDaCtlSREngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSREngines)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(IEnumVARIANT), CCorEnumVariant<DaCtlSREngines>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IEnumerable), CCorEnumVariant<DaCtlSREngines>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IList), CCorEnumVariant<DaCtlSREngines>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::ICollection), CCorEnumVariant<DaCtlSREngines>, mCachedEnum)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlSREngines)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlSREngines
	HRESULT STDMETHODCALLTYPE get_Item (VARIANT Index, IDaCtlSREngine ** SREngine);
	HRESULT STDMETHODCALLTYPE get_Count (long* Value);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown ** Enum);

// Implementation
public:
	HRESULT InitEnumVariant (CEnumVARIANTImpl * pEnum);

private:
	DaControl *	mOwner;
	IUnknownPtr	mCachedEnum;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSREngines), DaCtlSREngines)

/////////////////////////////////////////////////////////////////////////////
