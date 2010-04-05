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
#ifndef DACOMMANDOBJ_H_INCLUDED_
#define DACOMMANDOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E535-A208-11DE-ABF2-002421116FB2}")) CDaCommandObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaCommandObj)
	DECLARE_OLECREATE_EX(CDaCommandObj)
	DECLARE_OLETYPELIB(CDaCommandObj)
public:
	CDaCommandObj (class CDaCharacterObj & pOwner);
	~CDaCommandObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrCommandPtr		mServerObject;
	long					mServerId;

	class CDaCharacterObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Overrides
	//{{AFX_VIRTUAL(CDaCommandObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaCommandObj)
	afx_msg BSTR DspGetVoice();
	afx_msg void DspSetVoice(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetCaption();
	afx_msg void DspSetCaption(LPCTSTR lpszNewValue);
	afx_msg BOOL DspGetEnabled();
	afx_msg void DspSetEnabled(BOOL bNewValue);
	afx_msg BOOL DspGetVisible();
	afx_msg void DspSetVisible(BOOL bNewValue);
	afx_msg long DspGetConfidence();
	afx_msg void DspSetConfidence(long nNewValue);
	afx_msg BSTR DspGetConfidenceText();
	afx_msg void DspSetConfidenceText(LPCTSTR lpszNewValue);
	afx_msg long DspGetHelpContextID();
	afx_msg void DspSetHelpContextID(long nNewValue);
	afx_msg BSTR DspGetVoiceCaption();
	afx_msg void DspSetVoiceCaption(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Command, IDaCtlCommand)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Voice (BSTR *Voice);
		HRESULT STDMETHODCALLTYPE put_Voice (BSTR Voice);
		HRESULT STDMETHODCALLTYPE get_Caption (BSTR *Caption);
		HRESULT STDMETHODCALLTYPE put_Caption (BSTR Caption);
		HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL *Enabled);
		HRESULT STDMETHODCALLTYPE put_Enabled (VARIANT_BOOL Enabled);
		HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
		HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
		HRESULT STDMETHODCALLTYPE get_Confidence (long *Confidence);
		HRESULT STDMETHODCALLTYPE put_Confidence (long Confidence);
		HRESULT STDMETHODCALLTYPE get_ConfidenceText (BSTR *Text);
		HRESULT STDMETHODCALLTYPE put_ConfidenceText (BSTR Text);

		HRESULT STDMETHODCALLTYPE get_HelpContextID (long *ID);
		HRESULT STDMETHODCALLTYPE put_HelpContextID (long ID);
		HRESULT STDMETHODCALLTYPE get_VoiceCaption (BSTR *VoiceCaption);
		HRESULT STDMETHODCALLTYPE put_VoiceCaption (BSTR VoiceCaption);
	END_INTERFACE_PART(Command)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	class CDaCharacterObj *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACOMMANDOBJ_H_INCLUDED_
