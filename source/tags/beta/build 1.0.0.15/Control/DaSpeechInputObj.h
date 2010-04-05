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
#ifndef DASPEECHINPUTOBJ_H_INCLUDED_
#define DASPEECHINPUTOBJ_H_INCLUDED_
#pragma once

#include "DaAgentCtl.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E538-A208-11DE-ABF2-002421116FB2}")) CDaSpeechInputObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaSpeechInputObj)
	DECLARE_OLECREATE_EX(CDaSpeechInputObj)
	DECLARE_OLETYPELIB(CDaSpeechInputObj)
public:
	CDaSpeechInputObj (CDaAgentCtl & pOwner);
	~CDaSpeechInputObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrSpeechInputPropertiesPtr	mServerObject;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Overrides
	//{{AFX_VIRTUAL(CDaSpeechInputObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaSpeechInputObj)
	afx_msg BOOL DspGetEnabled();
	afx_msg void DspSetEnabled(BOOL bNewValue);
	afx_msg BSTR DspGetLanguage();
	afx_msg void DspSetLanguage(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetHotKey();
	afx_msg void DspSetHotKey(LPCTSTR lpszNewValue);
	afx_msg BOOL DspGetVoiceInstalled();
	afx_msg void DspSetVoiceInstalled(BOOL bNewValue);
	afx_msg BSTR DspGetEngine();
	afx_msg void DspSetEngine(LPCTSTR lpszNewValue);
	afx_msg BOOL DspGetListeningTip();
	afx_msg void DspSetListeningTip(BOOL bNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(SpeechInput, IDaCtlSpeechInput)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL *VoiceEnabled);
		HRESULT STDMETHODCALLTYPE get_Language (BSTR *Language);
		HRESULT STDMETHODCALLTYPE get_HotKey (BSTR *HotKey);
		HRESULT STDMETHODCALLTYPE get_Installed (VARIANT_BOOL *VoiceInstalled);
		HRESULT STDMETHODCALLTYPE get_Engine (BSTR *Engine);
		HRESULT STDMETHODCALLTYPE put_Engine (BSTR Engine);
		HRESULT STDMETHODCALLTYPE get_ListeningTip (VARIANT_BOOL *ListeningTip);
	END_INTERFACE_PART(SpeechInput)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASPEECHINPUTOBJ_H_INCLUDED_
