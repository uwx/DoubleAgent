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
#include "DaControl.h"
#include "DaCmnCharacterFiles.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E558-A208-11DE-ABF2-002421116FB2}")) DaCtlCharacterFiles :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCharacterFiles, &__uuidof(DaCtlCharacterFiles)>,
	public IDispatchImpl<IDaCtlCharacterFiles, &__uuidof(IDaCtlCharacterFiles), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCharacterFiles), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlCharacterFiles ();
	~DaCtlCharacterFiles ();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrCharacterFilesPtr		mServerObject;
#endif
	tPtr <CDaCmnCharacterFiles>	mLocalObject;

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
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCHARACTERFILES)
	DECLARE_NOT_AGGREGATABLE(DaCtlCharacterFiles)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlCharacterFiles)
		COM_INTERFACE_ENTRY(IDaCtlCharacterFiles)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCharacterFiles)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCharacterFiles)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlCharacterFiles
	HRESULT STDMETHODCALLTYPE get_FilePaths (SAFEARRAY** FilePaths);
	HRESULT STDMETHODCALLTYPE get_SearchPath (BSTR * SearchPath);
	HRESULT STDMETHODCALLTYPE put_SearchPath (BSTR SearchPath);
	HRESULT STDMETHODCALLTYPE get_DefaultSearchPath (BSTR * DefaultSearchPath);
	HRESULT STDMETHODCALLTYPE get_DoubleAgentFiles (VARIANT_BOOL * DoubleAgentFiles);
	HRESULT STDMETHODCALLTYPE put_DoubleAgentFiles (VARIANT_BOOL DoubleAgentFiles);
	HRESULT STDMETHODCALLTYPE get_MsAgentFiles (VARIANT_BOOL * MsAgentFiles);
	HRESULT STDMETHODCALLTYPE put_MsAgentFiles (VARIANT_BOOL MsAgentFiles);
	HRESULT STDMETHODCALLTYPE get_MsOfficeFiles (VARIANT_BOOL * MsOfficeFiles);
	HRESULT STDMETHODCALLTYPE put_MsOfficeFiles (VARIANT_BOOL MsOfficeFiles);
	HRESULT STDMETHODCALLTYPE get_CompliantCharacters (VARIANT_BOOL * CompliantCharacters);
	HRESULT STDMETHODCALLTYPE put_CompliantCharacters (VARIANT_BOOL CompliantCharacters);
	HRESULT STDMETHODCALLTYPE get_NonCompliantCharacters (VARIANT_BOOL * NonCompliantCharacters);
	HRESULT STDMETHODCALLTYPE put_NonCompliantCharacters (VARIANT_BOOL NonCompliantCharacters);
	HRESULT STDMETHODCALLTYPE get_SpeakingCharacters (VARIANT_BOOL * SpeakingCharacters);
	HRESULT STDMETHODCALLTYPE put_SpeakingCharacters (VARIANT_BOOL SpeakingCharacters);
	HRESULT STDMETHODCALLTYPE get_NonSpeakingCharacters (VARIANT_BOOL * NonSpeakingCharacters);
	HRESULT STDMETHODCALLTYPE put_NonSpeakingCharacters (VARIANT_BOOL NonSpeakingCharacters);
	HRESULT STDMETHODCALLTYPE get_VerifyVersion (VARIANT_BOOL * VerifyVersion);
	HRESULT STDMETHODCALLTYPE put_VerifyVersion (VARIANT_BOOL VerifyVersion);
	HRESULT STDMETHODCALLTYPE get_DefaultFilePath (BSTR * DefaultFilePath);
	HRESULT STDMETHODCALLTYPE get_DefaultFileName (BSTR * DefaultFileName);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCharacterFiles), DaCtlCharacterFiles)

/////////////////////////////////////////////////////////////////////////////
