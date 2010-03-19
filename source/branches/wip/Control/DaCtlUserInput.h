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

class ATL_NO_VTABLE CDaCtlUserInput :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlUserInput, &__uuidof(DaCtlUserInput)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlUserInput), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlUserInput, &__uuidof(IDaCtlUserInput), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlUserInput() {}
	~CDaCtlUserInput() {}

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLUSERINPUT)
	DECLARE_NOT_AGGREGATABLE(CDaCtlUserInput)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlUserInput)
		COM_INTERFACE_ENTRY(IDaCtlUserInput)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlUserInput)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlUserInput)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlUserInput
	STDMETHOD(get_Count)(short * pCount)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Name)(BSTR * pName)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_CharacterID)(BSTR * pCharacterID)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Confidence)(long * pConfidence)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Voice)(BSTR * pVoice)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Alt1Name)(BSTR * pAlt1Name)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Alt1Confidence)(long * pAlt1Confidence)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Alt1Voice)(BSTR * pAlt1Voice)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Alt2Name)(BSTR * pAlt2Name)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Alt2Confidence)(long * pAlt2Confidence)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(get_Alt2Voice)(BSTR * pAlt2Voice)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlUserInput), CDaCtlUserInput)

/////////////////////////////////////////////////////////////////////////////
