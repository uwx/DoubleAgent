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

class ATL_NO_VTABLE __declspec(uuid("{1147E558-A208-11DE-ABF2-002421116FB2}")) CDaCtlCharacterFiles :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlCharacterFiles, &__uuidof(DaCtlCharacterFiles)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCharacterFiles), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCharacterFiles, &__uuidof(IDaCtlCharacterFiles), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlCharacterFiles ();
	~CDaCtlCharacterFiles ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCHARACTERFILES)
	DECLARE_NOT_AGGREGATABLE(CDaCtlCharacterFiles)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlCharacterFiles)
		COM_INTERFACE_ENTRY(IDaCtlCharacterFiles)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCharacterFiles)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlCharacterFiles)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCharacterFiles
	STDMETHOD(get_FilePaths)(SAFEARRAY * * FilePaths);
	STDMETHOD(get_SearchPath)(BSTR * SearchPath);
	STDMETHOD(put_SearchPath)(BSTR SearchPath);
	STDMETHOD(get_DefaultSearchPath)(BSTR * DefaultSearchPath);
	STDMETHOD(get_DoubleAgentFiles)(VARIANT_BOOL * DoubleAgentFiles);
	STDMETHOD(put_DoubleAgentFiles)(VARIANT_BOOL DoubleAgentFiles);
	STDMETHOD(get_MsAgentFiles)(VARIANT_BOOL * MsAgentFiles);
	STDMETHOD(put_MsAgentFiles)(VARIANT_BOOL MsAgentFiles);
	STDMETHOD(get_MsOfficeFiles)(VARIANT_BOOL * MsOfficeFiles);
	STDMETHOD(put_MsOfficeFiles)(VARIANT_BOOL MsOfficeFiles);
	STDMETHOD(get_SpeakingCharacters)(VARIANT_BOOL * SpeakingCharacters);
	STDMETHOD(put_SpeakingCharacters)(VARIANT_BOOL SpeakingCharacters);
	STDMETHOD(get_NonSpeakingCharacters)(VARIANT_BOOL * NonSpeakingCharacters);
	STDMETHOD(put_NonSpeakingCharacters)(VARIANT_BOOL NonSpeakingCharacters);
	STDMETHOD(get_VerifyVersion)(VARIANT_BOOL * VerifyVersion);
	STDMETHOD(put_VerifyVersion)(VARIANT_BOOL VerifyVersion);

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
	IDaSvrCharacterFilesPtr		mServerObject;
private:
	CDaControlObj *				mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCharacterFiles), CDaCtlCharacterFiles)

/////////////////////////////////////////////////////////////////////////////
