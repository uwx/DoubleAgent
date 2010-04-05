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
#include "PropertySheet.h"
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CPropSheetCharSelObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IUnknown
{
public:
	CPropSheetCharSelObj () {}
	~CPropSheetCharSelObj () {}

	DECLARE_NOT_AGGREGATABLE(CPropSheetCharSelObj)
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(CPropSheetCharSelObj)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CPropSheetCharSel :
	public CPropSheetCharSelObj,
	public CAtlPropertySheet,
	public CSvrObjLifetime
{
public:
	CPropSheetCharSel ();
	virtual ~CPropSheetCharSel ();

// Attributes
public:

// Operations
public:
	static CPropSheetCharSel * CreateInstance (HWND pParentWnd = NULL, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);

// Overrides
public:
	virtual void OnClientEnded ();
protected:
	virtual bool PreCreateSheet (bool pModal);
	virtual void PreShowSheet ();
	virtual void OnFinalMessage (HWND);
	virtual void LoadConfig ();
	virtual void SaveConfig (int pSheetResult);
};

/////////////////////////////////////////////////////////////////////////////
