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
#ifndef DAAGENTCOMMAND_H_INCLUDED_
#define DAAGENTCOMMAND_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E50E-A208-11DE-ABF2-002421116FB2}")) CDaAgentCommand : public CCmdTarget
{
public:
	CDaAgentCommand();
	virtual ~CDaAgentCommand();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentCommand)
	DECLARE_OLETYPELIB(CDaAgentCommand)

// Attributes
public:
	USHORT	mId;
	bool	mEnabled;
	bool	mVisible;
	CString	mCaption;
	CString	mVoice;
	CString	mVoiceCaption;
	short	mConfidenceThreshold;
	CString	mConfidenceText;
	ULONG	mHelpContextId;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaAgentCommand)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentCommand)
	afx_msg void DspSetCaption(LPCTSTR Caption);
	afx_msg void DspGetCaption(BSTR * Caption);
	afx_msg void DspSetVoice(LPCTSTR Voice);
	afx_msg void DspGetVoice(BSTR * Voice);
	afx_msg void DspSetEnabled(long Enabled);
	afx_msg void DspGetEnabled(long * Enabled);
	afx_msg void DspSetVisible(long Visible);
	afx_msg void DspGetVisible(long * Visible);
	afx_msg void DspSetConfidenceThreshold(long Threshold);
	afx_msg void DspGetConfidenceThreshold(long * Threshold);
	afx_msg void DspSetConfidenceText(LPCTSTR TipText);
	afx_msg void DspGetConfidenceText(BSTR * TipText);
	afx_msg void DspGetid(long * ID);
	afx_msg void DspSetHelpContextID(long ID);
	afx_msg void DspGetHelpContextID(long * ID);
	afx_msg void DspSetVoiceCaption(LPCTSTR VoiceCaption);
	afx_msg void DspGetVoiceCaption(BSTR * VoiceCaption);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Command, IDaSvrCommand)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE SetCaption (BSTR bszCaption);
		HRESULT STDMETHODCALLTYPE GetCaption (BSTR *pbszCaption);
		HRESULT STDMETHODCALLTYPE SetVoice (BSTR bszVoice);
		HRESULT STDMETHODCALLTYPE GetVoice (BSTR *pbszVoice);
		HRESULT STDMETHODCALLTYPE SetEnabled (long bEnabled);
		HRESULT STDMETHODCALLTYPE GetEnabled (long *pbEnabled);
		HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
		HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
		HRESULT STDMETHODCALLTYPE SetConfidenceThreshold (long lThreshold);
		HRESULT STDMETHODCALLTYPE GetConfidenceThreshold (long *plThreshold);
		HRESULT STDMETHODCALLTYPE SetConfidenceText (BSTR bszTipText);
		HRESULT STDMETHODCALLTYPE GetConfidenceText (BSTR *pbszTipText);
		HRESULT STDMETHODCALLTYPE GetID (long *pdwID);

		HRESULT STDMETHODCALLTYPE SetHelpContextID (long ulID);
		HRESULT STDMETHODCALLTYPE GetHelpContextID (long *pulID);
		HRESULT STDMETHODCALLTYPE SetVoiceCaption (BSTR bszVoiceCaption);
		HRESULT STDMETHODCALLTYPE GetVoiceCaption (BSTR *pbszVoiceCaption);
	END_INTERFACE_PART(Command)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTCOMMAND_H_INCLUDED_
