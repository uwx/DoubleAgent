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
#ifndef DAAGENTPROPERTYSHEET_H_INCLUDED_
#define DAAGENTPROPERTYSHEET_H_INCLUDED_
#pragma once

#include "PropSheetOptions.h"
#include "DaServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E513-A208-11DE-ABF2-002421116FB2}")) CDaAgentPropertySheet : public CPropSheetOptions, public CDaObjectLifetime
{
public:
	CDaAgentPropertySheet (LPCTSTR pClientMutexName = NULL);
	virtual ~CDaAgentPropertySheet ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentPropertySheet)
	DECLARE_OLETYPELIB(CDaAgentPropertySheet)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaAgentPropertySheet)
	public:
	virtual void OnFinalRelease();
	virtual void OnClientEnded ();
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDaAgentPropertySheet)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_DISPATCH(CDaAgentPropertySheet)
	afx_msg void DspGetVisible(long * Visible);
	afx_msg void DspSetVisible(long Visible);
	afx_msg void DspGetPosition(long * Left, long * Top);
	afx_msg void DspGetSize(long * Width, long * Height);
	afx_msg void DspGetPage(BSTR * Page);
	afx_msg void DspSetPage(LPCTSTR Page);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(PropSheet, IDaSvrPropertySheet)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
		HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
		HRESULT STDMETHODCALLTYPE GetPosition (long *plLeft, long *plTop);
		HRESULT STDMETHODCALLTYPE GetSize (long *plWidth, long *plHeight);
		HRESULT STDMETHODCALLTYPE GetPage (BSTR *pbszPage);
		HRESULT STDMETHODCALLTYPE SetPage (BSTR bszPage);
	END_INTERFACE_PART(PropSheet)

	BEGIN_INTERFACE_PART(OleWindow, IOleWindow)
        HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
        HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);
	END_INTERFACE_PART(OleWindow)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
	HWND GetPropSheetWnd (bool pCreate);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTPROPERTYSHEET_H_INCLUDED_
