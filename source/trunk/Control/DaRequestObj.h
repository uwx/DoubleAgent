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
#ifndef DAREQUESTOBJ_H_INCLUDED_
#define DAREQUESTOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

enum DaRequestCategory
{
	DaRequestLoad = 1,
	DaRequestShow,
	DaRequestHide,
	DaRequestPlay,
	DaRequestSpeak,
	DaRequestPrepare,
	DaRequestOther,
	DaRequestCategoryMask = 0x00000FFF,
	DaRequestNotifyStart = 0x00001000,
	DaRequestNotifyComplete = 0x00002000,
	DaRequestNotifyMask = 0x00003000
};

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E536-A208-11DE-ABF2-002421116FB2}")) CDaRequestObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaRequestObj)
	DECLARE_OLECREATE_EX(CDaRequestObj)
	DECLARE_OLETYPELIB(CDaRequestObj)
public:
	CDaRequestObj (class CDaAgentCtl & pOwner, DaRequestCategory pCategory, long pReqID, HRESULT pResult = S_OK);
	~CDaRequestObj ();
	void Terminate (bool pFinal);

// Attributes
public:
	DaRequestCategory	mCategory;
	long				mReqID;
	long				mStatus;
	HRESULT				mResult;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Operations
public:
	CString StatusStr () const;
	CString CategoryStr () const;

	friend CString RequestStatusStr (long pStatus);
	friend CString RequestCategoryStr (DaRequestCategory pCategory);

// Overrides
	//{{AFX_VIRTUAL(CDaRequestObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaRequestObj)
	afx_msg long DspGetID();
	afx_msg void DspSetID(long nNewValue);
	afx_msg long DspGetStatus();
	afx_msg void DspSetStatus(long nNewValue);
	afx_msg BSTR DspGetDescription();
	afx_msg void DspSetDescription(LPCTSTR lpszNewValue);
	afx_msg long DspGetNumber();
	afx_msg void DspSetNumber(long nNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Request, IDaCtlRequest)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_ID (long *ID);
		HRESULT STDMETHODCALLTYPE get_Status (long *Status);
		HRESULT STDMETHODCALLTYPE get_Description (BSTR *Description);
		HRESULT STDMETHODCALLTYPE get_Number (long *Number);
	END_INTERFACE_PART(Request)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
	class CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAREQUESTOBJ_H_INCLUDED_
