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
#ifndef DACOMMANDSWINDOWOBJ_H_INCLUDED_
#define DACOMMANDSWINDOWOBJ_H_INCLUDED_
#pragma once

#include "DaAgentCtl.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E53B-A208-11DE-ABF2-002421116FB2}")) CDaCommandsWindowObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaCommandsWindowObj)
	DECLARE_OLECREATE_EX(CDaCommandsWindowObj)
	DECLARE_OLETYPELIB(CDaCommandsWindowObj)

// Constructor
public:
	CDaCommandsWindowObj (CDaAgentCtl & pOwner);
	~CDaCommandsWindowObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrCommandWindowPtr	mServerObject;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Overrides
	//{{AFX_VIRTUAL(CDaCommandsWindowObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaCommandsWindowObj)
	afx_msg BOOL DspGetVisible();
	afx_msg void DspSetVisible(BOOL bNewValue);
	afx_msg short DspGetLeft();
	afx_msg void DspSetLeft(short nNewValue);
	afx_msg short DspGetTop();
	afx_msg void DspSetTop(short nNewValue);
	afx_msg short DspGetHeight();
	afx_msg void DspSetHeight(short nNewValue);
	afx_msg short DspGetWidth();
	afx_msg void DspSetWidth(short nNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(CommandsWindow, IDaCtlCommandsWindow)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
		HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
		HRESULT STDMETHODCALLTYPE get_Left (short *Left);
		HRESULT STDMETHODCALLTYPE get_Top (short *Top);
		HRESULT STDMETHODCALLTYPE get_Height (short *Height);
		HRESULT STDMETHODCALLTYPE get_Width (short *Width);
	END_INTERFACE_PART(CommandsWindow)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACOMMANDSWINDOWOBJ_H_INCLUDED_
