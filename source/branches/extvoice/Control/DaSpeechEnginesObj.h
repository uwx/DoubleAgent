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
#ifndef DASPEECHENGINESOBJ_H_INCLUDED_
#define DASPEECHENGINESOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"
#include "StringMap.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E554-A208-11DE-ABF2-002421116FB2}")) CDaSpeechEnginesObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaSpeechEnginesObj)
	DECLARE_OLECREATE_EX(CDaSpeechEnginesObj)
	DECLARE_OLETYPELIB(CDaSpeechEnginesObj)
public:
	CDaSpeechEnginesObj (IDaSvrSpeechEngines * pServerObject);
	~CDaSpeechEnginesObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrSpeechEnginesPtr									mServerObject;
	CArrayEx <IDaCtlSpeechEnginePtr, IDaCtlSpeechEngine *>	mSpeechEngines;

// Operations

// Overrides
	//{{AFX_VIRTUAL(CDaSpeechEnginesObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaSpeechEnginesObj)
	afx_msg LPDISPATCH DspGetItem(VARIANT Index);
	afx_msg void DspSetItem(VARIANT Index, LPDISPATCH Item);
	afx_msg long DspGetCount();
	afx_msg void DspSetCount(long Count);
	afx_msg LPUNKNOWN Dsp_NewEnum();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(SpeechEngines, IDaCtlSpeechEngines)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Item (VARIANT Index, IDaCtlSpeechEngine **Item);
		HRESULT STDMETHODCALLTYPE get_Count (long *Count);
		HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown **ppunkEnum);
	END_INTERFACE_PART(SpeechEngines)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	void Initialize ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASpeechEngineSOBJ_H_INCLUDED_
