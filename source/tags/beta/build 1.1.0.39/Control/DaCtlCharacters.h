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
#include "CorEnumVariant.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E531-A208-11DE-ABF2-002421116FB2}")) DaCtlCharacters :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCharacters, &__uuidof(DaCtlCharacters)>,
	public IDispatchImpl<IDaCtlCharacters2, &__uuidof(IDaCtlCharacters2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCharacters), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlCharacters ();
	~DaCtlCharacters ();

// Attributes
public:
	CAtlMap <CAtlString, IDispatchPtr, CStringElementTraitsI <CAtlString> > mCharacters;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);
	void Disconnect (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	class DaCtlCharacter * FindCharacter (IDaCtlCharacter2 * pInterface);

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCHARACTERS)
	DECLARE_NOT_AGGREGATABLE(DaCtlCharacters)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(DaCtlCharacters)
		COM_INTERFACE_ENTRY(IDaCtlCharacters2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCharacters2)
		COM_INTERFACE_ENTRY2(IDaCtlCharacters, IDaCtlCharacters2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCharacters), IDaCtlCharacters2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(IEnumVARIANT), CCorEnumVariant<DaCtlCharacters>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IEnumerable), CCorEnumVariant<DaCtlCharacters>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IList), CCorEnumVariant<DaCtlCharacters>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::ICollection), CCorEnumVariant<DaCtlCharacters>, mCachedEnum)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCharacters)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlCharacters2
	HRESULT STDMETHODCALLTYPE get_Item (BSTR CharacterID, IDaCtlCharacter2 ** Item);
	HRESULT STDMETHODCALLTYPE Character (BSTR CharacterID, IDaCtlCharacter2 ** Character);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown ** EnumVariant);
	HRESULT STDMETHODCALLTYPE Unload (BSTR CharacterID);
	HRESULT STDMETHODCALLTYPE Load (BSTR CharacterID, VARIANT Provider, IDaCtlRequest ** ppidRequest);
	HRESULT STDMETHODCALLTYPE get_Count (long * Count);
	HRESULT STDMETHODCALLTYPE get_Index (long Index, IDaCtlCharacter2 ** Character);

// Implementation
public:
	HRESULT InitEnumVariant (CEnumVARIANTImpl * pEnum);

private:
	DaControl *	mOwner;
	IUnknownPtr	mCachedEnum;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCharacters), DaCtlCharacters)

/////////////////////////////////////////////////////////////////////////////
