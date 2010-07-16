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

#include "DaGuid.h"
#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E518-A208-11DE-ABF2-002421116FB2}")) CDaServerHandler : public CCmdTarget
{
	DECLARE_DYNCREATE (CDaServerHandler)
	DECLARE_OLECREATE_EX(CDaServerHandler)
protected:
	CDaServerHandler ();
public:
	virtual ~CDaServerHandler ();

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaServerHandler)
	public:
	virtual void OnFinalRelease();
	virtual BOOL OnCreateAggregates();
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//BEGIN_INTERFACE_PART(Balloon, IDaSvrBalloon)
	//END_INTERFACE_PART(Balloon)
	DECLARE_INTERFACE_MAP()

protected:
	IUnknownPtr	mProxyManager;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASERVERHANDLER_H_INCLUDED_
