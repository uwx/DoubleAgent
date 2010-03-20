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
#ifndef DAAGENTUSERINPUT_H_INCLUDED_
#define DAAGENTUSERINPUT_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CDaAgentUserInput : public CCmdTarget
{
public:
	CDaAgentUserInput (interface ISpRecoResult * pRecoResult, bool pGlobalCommand);
	virtual ~CDaAgentUserInput ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentUserInput)
	DECLARE_OLETYPELIB(CDaAgentUserInput)

// Attributes
public:

// Operations
public:
	void Initialize (interface ISpRecoResult * pRecoResult, bool pGlobalCommand);

// Overrides
	//{{AFX_VIRTUAL(CDaAgentUserInput)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentUserInput)
	afx_msg void DspGetCount(long * Count);
	afx_msg void DspGetItemid(long ItemIndex, long * CommandID);
	afx_msg void DspGetItemConfidence(long ItemIndex, long * Confidence);
	afx_msg void DspGetItemText(long ItemIndex, BSTR * Text);
	afx_msg void DspGetAllItemData(VARIANT * ItemIndices, VARIANT * Confidences, VARIANT * Text);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(UserInput, IDaSvrUserInput)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE GetCount (long *pdwCount);
		HRESULT STDMETHODCALLTYPE GetItemID (long dwItemIndex, long *pdwCommandID);
		HRESULT STDMETHODCALLTYPE GetItemConfidence (long dwItemIndex, long *plConfidence);
		HRESULT STDMETHODCALLTYPE GetItemText (long dwItemIndex, BSTR *pbszText);
		HRESULT STDMETHODCALLTYPE GetAllItemData (VARIANT *pdwItemIndices, VARIANT *plConfidences, VARIANT *pbszText);
	END_INTERFACE_PART(UserInput)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
	CArrayEx <long>	mItemId;
	CArrayEx <long>	mItemConfidence;
	CStringArray	mItemText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTUSERINPUT_H_INCLUDED_
