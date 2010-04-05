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
#ifndef DAAGENTSPEECHINPUTPROPERTIES_H_INCLUDED_
#define DAAGENTSPEECHINPUTPROPERTIES_H_INCLUDED_
#pragma once

#include "DaServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E515-A208-11DE-ABF2-002421116FB2}")) CDaAgentSpeechInputProperties : public CCmdTarget, public CDaObjectLifetime
{
public:
	CDaAgentSpeechInputProperties (LPCTSTR pClientMutexName = NULL);
	virtual ~CDaAgentSpeechInputProperties ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentSpeechInputProperties)
	DECLARE_OLETYPELIB(CDaAgentSpeechInputProperties)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaAgentSpeechInputProperties)
	public:
	virtual void OnFinalRelease();
	virtual void OnClientEnded ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentSpeechInputProperties)
	afx_msg void DspGetInstalled(long * Installed);
	afx_msg void DspGetEnabled(long * Enabled);
	afx_msg void DspGetHotKey(BSTR * HotCharKey);
	afx_msg void DspGetLCid(unsigned long * lcidCurrent);
	afx_msg void DspGetEngine(BSTR * Engine);
	afx_msg void DspSetEngine(LPCTSTR Engine);
	afx_msg void DspGetListeningTip(long * ListeningTip);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(SpeechInputProperties, IDaSvrSpeechInputProperties)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE GetInstalled (long *pbInstalled);
		HRESULT STDMETHODCALLTYPE GetEnabled (long *pbEnabled);
		HRESULT STDMETHODCALLTYPE GetHotKey (BSTR *pbszHotCharKey);
		HRESULT STDMETHODCALLTYPE GetLCID (unsigned long *plcidCurrent);
		HRESULT STDMETHODCALLTYPE GetEngine (BSTR *pbszEngine);
		HRESULT STDMETHODCALLTYPE SetEngine (BSTR bszEngine);
		HRESULT STDMETHODCALLTYPE GetListeningTip (long *pbListeningTip);
	END_INTERFACE_PART(SpeechInputProperties)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTSPEECHINPUTPROPERTIES_H_INCLUDED_
