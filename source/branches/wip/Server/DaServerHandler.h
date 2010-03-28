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
#ifndef DASERVERHANDLER_H_INCLUDED_
#define DASERVERHANDLER_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E518-A208-11DE-ABF2-002421116FB2}")) CDaServerHandler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaServerHandler, &__uuidof(DaServerHandler)>
{
public:
	CDaServerHandler ();
	~CDaServerHandler ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(101)
	DECLARE_POLY_AGGREGATABLE(CDaServerHandler)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_CATEGORY_MAP(CDaServerHandler)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	END_CATEGORY_MAP()

// Attributes
public:

// Operations
public:
	HRESULT FinalConstruct ();
	void FinalRelease ();
	HRESULT _InternalQueryInterface (REFIID iid, void** ppvObject);

// Implementation
protected:
	IUnknownPtr	mProxyManager;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_AUTO(__uuidof(DaServerHandler), CDaServerHandler)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASERVERHANDLER_H_INCLUDED_
