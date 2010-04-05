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
#include "DaServerApp.h"
#include "AgentFile.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E517-A208-11DE-ABF2-002421116FB2}")) DaSvrAnimationNames :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrAnimationNames, &__uuidof(DaSvrAnimationNames)>,
	public CEnumVARIANTImpl,
	public IProvideClassInfoImpl<&__uuidof(DaSvrAnimationNames), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>
{
protected:
	DaSvrAnimationNames ();
	virtual ~DaSvrAnimationNames ();

// Attributes
public:
	static DaSvrAnimationNames * CreateInstance (CAgentFile & pAgentFile);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Operations
public:

// Overrides

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRANIMATIONNAMES)
	DECLARE_NOT_AGGREGATABLE(DaSvrAnimationNames)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrAnimationNames)
		COM_INTERFACE_ENTRY(IEnumVARIANT)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrAnimationNames)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
	// IEnumVARIANT
	STDMETHODIMP Clone(IEnumVARIANT** ppEnum);

// Implementation
protected:
	tArrayPtr <CComVariant>	mAnimationNames;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrAnimationNames), DaSvrAnimationNames)

/////////////////////////////////////////////////////////////////////////////
