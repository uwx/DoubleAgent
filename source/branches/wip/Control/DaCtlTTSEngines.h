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
#include "DaControlMod.h"
#include "DaControl.h"
#include "DaCmnTtsEngines.h"
#include "CorEnumVariant.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E554-A208-11DE-ABF2-002421116FB2}")) DaCtlTTSEngines :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlTTSEngines, &__uuidof(DaCtlTTSEngines)>,
	public IDispatchImpl<IDaCtlTTSEngines, &__uuidof(IDaCtlTTSEngines), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlTTSEngines), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISupportErrorInfo
{
public:
	DaCtlTTSEngines();
	~DaCtlTTSEngines();

// Attributes
public:
	IDaSvrTTSEnginesPtr					mServerObject;
	tPtr <CDaCmnTTSEngines>				mLocalObject;
	CInterfaceArray <IDaCtlTTSEngine>	mTTSEngines;

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
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLTTSENGINES)
	DECLARE_NOT_AGGREGATABLE(DaCtlTTSEngines)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(DaCtlTTSEngines)
		COM_INTERFACE_ENTRY(IDaCtlTTSEngines)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlTTSEngines)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(IEnumVARIANT), CCorEnumVariant<DaCtlTTSEngines>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IEnumerable), CCorEnumVariant<DaCtlTTSEngines>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IList), CCorEnumVariant<DaCtlTTSEngines>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::ICollection), CCorEnumVariant<DaCtlTTSEngines>, mCachedEnum)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlTTSEngines)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlTTSEngines
	HRESULT STDMETHODCALLTYPE get_Item (VARIANT Index,  IDaCtlTTSEngine ** TTSEngine);
	HRESULT STDMETHODCALLTYPE get_Count (long * Value);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown ** Enum);
	HRESULT STDMETHODCALLTYPE get_All (SAFEARRAY **Array);

// Implementation
public:
	HRESULT InitEnumVariant (CEnumVARIANTImpl * pEnum);
	
private:
	DaControl *	mOwner;
	IUnknownPtr	mCachedEnum;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlTTSEngines), DaCtlTTSEngines)

/////////////////////////////////////////////////////////////////////////////
