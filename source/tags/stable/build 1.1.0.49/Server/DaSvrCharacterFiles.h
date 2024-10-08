/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include "DaServerApp.h"
#include "DaCmnCharacterFiles.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E524-A208-11DE-ABF2-002421116FB2}")) DaSvrCharacterFiles :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCharacterFiles, &__uuidof(DaSvrCharacterFiles)>,
	public IDispatchImpl<IDaSvrCharacterFiles, &__uuidof(IDaSvrCharacterFiles), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCharacterFiles), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CDaCmnCharacterFiles,
	public CSvrObjLifetime
{
public:
	DaSvrCharacterFiles ();
	virtual ~DaSvrCharacterFiles ();

// Attributes
public:

// Operations
public:
	static DaSvrCharacterFiles * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRCHARACTERFILES)
	DECLARE_NOT_AGGREGATABLE(DaSvrCharacterFiles)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrCharacterFiles)
		COM_INTERFACE_ENTRY(IDaSvrCharacterFiles)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCharacterFiles)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrCharacterFiles)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrCharacterFiles
	HRESULT STDMETHODCALLTYPE get_FilePaths (SAFEARRAY **FilePaths);
	HRESULT STDMETHODCALLTYPE get_SearchPath (BSTR *SearchPath);
	HRESULT STDMETHODCALLTYPE put_SearchPath (BSTR SearchPath);
	HRESULT STDMETHODCALLTYPE get_DefaultSearchPath (BSTR *DefaultSearchPath);
	HRESULT STDMETHODCALLTYPE get_Filter (long *Filter);
	HRESULT STDMETHODCALLTYPE put_Filter (long Filter);
	HRESULT STDMETHODCALLTYPE get_DefaultFilePath (BSTR *DefaultFilePath);
	HRESULT STDMETHODCALLTYPE get_DefaultFileName (BSTR *DefaultFileName);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCharacterFiles), DaSvrCharacterFiles)

/////////////////////////////////////////////////////////////////////////////
