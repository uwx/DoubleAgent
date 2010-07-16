/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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

class ATL_NO_VTABLE __declspec(uuid("{1147E518-A208-11DE-ABF2-002421116FB2}")) CDaServerHandler :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDaServerHandler, &__uuidof(DaServerHandler)>,
	public IMultiQI
{
public:
	CDaServerHandler ();
	~CDaServerHandler ();

// Attributes
public:

// Operations
public:
	HRESULT FinalConstruct ();
	void FinalRelease ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(101)
	DECLARE_POLY_AGGREGATABLE(CDaServerHandler)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaServerHandler)
		COM_INTERFACE_ENTRY(IMultiQI)
		COM_INTERFACE_ENTRY_FUNC_BLIND(0, &DelegateInterface)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaServerHandler)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	END_CATEGORY_MAP()

	static HRESULT WINAPI DelegateInterface (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw);

// Interfaces
	HRESULT STDMETHODCALLTYPE QueryMultipleInterfaces (ULONG cMQIs, MULTI_QI *pMQIs);

// Implementation
protected:
	IUnknownPtr	mProxyManager;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_AUTO(__uuidof(DaServerHandler), CDaServerHandler)

/////////////////////////////////////////////////////////////////////////////
