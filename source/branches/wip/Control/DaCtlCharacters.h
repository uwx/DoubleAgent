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
//#include "StringMap.h"

class ATL_NO_VTABLE __declspec(uuid("{1147E531-A208-11DE-ABF2-002421116FB2}")) CDaCtlCharacters :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlCharacters, &__uuidof(DaCtlCharacters)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCharacters), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCharacters, &__uuidof(IDaCtlCharacters), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlCharacters ();
	~CDaCtlCharacters ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCHARACTERS)
	DECLARE_NOT_AGGREGATABLE(CDaCtlCharacters)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlCharacters)
		COM_INTERFACE_ENTRY(IDaCtlCharacters)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCharacters)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCharacters), IDaCtlCharacters)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlCharacters)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCharacters
	STDMETHOD(get_Item)(BSTR CharacterID,  IDaCtlCharacter2 * * ppidItem);
	STDMETHOD(Character)(BSTR CharacterID,  IDaCtlCharacter2 * * ppidItem);
	STDMETHOD(get__NewEnum)(IUnknown * * ppunkEnum);
	STDMETHOD(Unload)(BSTR CharacterID);
	STDMETHOD(Load)(BSTR CharacterID,  VARIANT LoadKey,  IDaCtlRequest * * ppidRequest);

// Attributes
public:
	CAtlMap <CString, IDispatchPtr, CStringElementTraitsI <CString> > mCharacters;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (CDaControlObj * pOwner);
	CDaControlObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Implementation
private:
	CDaControlObj *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCharacters), CDaCtlCharacters)

/////////////////////////////////////////////////////////////////////////////
