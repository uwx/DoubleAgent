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
#include "DaCtlCharacter.h"
#include "DaCmnAnimationNames.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E54B-A208-11DE-ABF2-002421116FB2}")) DaCtlAnimationNames :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlAnimationNames, &__uuidof(DaCtlAnimationNames)>,
	public IDispatchImpl<IDaCtlAnimationNames, &__uuidof(IDaCtlAnimationNames), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlAnimationNames), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISupportErrorInfo
{
public:
	DaCtlAnimationNames ();
	~DaCtlAnimationNames ();

// Attributes
public:
	IDaSvrAnimationNamesPtr		mServerObject;
	tPtr <CDaCmnAnimationNames>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaCtlCharacter * pOwner);
	DaCtlCharacter * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLANIMATIONNAMES)
	DECLARE_NOT_AGGREGATABLE(DaCtlAnimationNames)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlAnimationNames)
		COM_INTERFACE_ENTRY(IDaCtlAnimationNames)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlAnimationNames)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlAnimationNames)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlAnimationNames
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown ** EnumVariant);
	HRESULT STDMETHODCALLTYPE get_Item (VARIANT Index, BSTR *AnimationName);
	HRESULT STDMETHODCALLTYPE get_Count (long *Value);
	HRESULT STDMETHODCALLTYPE get_All (SAFEARRAY **Array);

// Implementation
private:
	DaCtlCharacter *			mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlAnimationNames), DaCtlAnimationNames)

/////////////////////////////////////////////////////////////////////////////
