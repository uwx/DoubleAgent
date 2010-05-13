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
#include "DaCmnUserInput.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E53A-A208-11DE-ABF2-002421116FB2}")) DaCtlUserInput :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlUserInput, &__uuidof(DaCtlUserInput)>,
	public IDispatchImpl<IDaCtlUserInput, &__uuidof(IDaCtlUserInput), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlUserInput), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISupportErrorInfo
{
public:
	DaCtlUserInput();
	~DaCtlUserInput();

// Attributes
public:
	CAtlString				mCharacterID;
	long					mCommandID;
	class DaCtlCommands *	mCommands;
	IDaSvrUserInput2Ptr		mServerObject;
	tPtr <CDaCmnUserInput>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLUSERINPUT)
	DECLARE_NOT_AGGREGATABLE(DaCtlUserInput)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlUserInput)
		COM_INTERFACE_ENTRY(IDaCtlUserInput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlUserInput)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlUserInput), IDaCtlUserInput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlUserInput)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlUserInput
	STDMETHOD(get_Count)(short * Count);
	STDMETHOD(get_Name)(BSTR * Name);
	STDMETHOD(get_CharacterID)(BSTR * CharacterID);
	STDMETHOD(get_Confidence)(long * Confidence);
	STDMETHOD(get_Voice)(BSTR * Voice);
	STDMETHOD(get_Alt1Name)(BSTR * Alt1Name);
	STDMETHOD(get_Alt1Confidence)(long * Alt1Confidence);
	STDMETHOD(get_Alt1Voice)(BSTR * Alt1Voice);
	STDMETHOD(get_Alt2Name)(BSTR * Alt2Name);
	STDMETHOD(get_Alt2Confidence)(long * Alt2Confidence);
	STDMETHOD(get_Alt2Voice)(BSTR * Alt2Voice);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlUserInput), DaCtlUserInput)

/////////////////////////////////////////////////////////////////////////////
