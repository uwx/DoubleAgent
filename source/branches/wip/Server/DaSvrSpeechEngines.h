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
#ifndef DASVRSPEECHENGINES_H_INCLUDED_
#define DASVRSPEECHENGINES_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E520-A208-11DE-ABF2-002421116FB2}")) CDaSvrSpeechEngines : public CCmdTarget
{
public:
	CDaSvrSpeechEngines ();
	virtual ~CDaSvrSpeechEngines ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaSvrSpeechEngines)
	DECLARE_OLETYPELIB(CDaSvrSpeechEngines)

// Attributes
public:
	CPtrTypeArray <class CSapi5VoiceInfo>	mSapi5Voices;
#ifndef	_WIN64
	CPtrTypeArray <class CSapi4VoiceInfo>	mSapi4Voices;
#endif

// Operations
public:
	void UseAllVoices ();

// Overrides
	//{{AFX_VIRTUAL(CDaSvrSpeechEngines)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaSvrSpeechEngines)
	afx_msg LPDISPATCH DspGetItem(long Index);
	afx_msg void DspSetItem(long Index, LPDISPATCH SpeechEngine);
	afx_msg long DspGetCount();
	afx_msg void DspSetCount(long Count);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(SpeechEngines, IDaSvrSpeechEngines)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Item (long Index, IDaSvrSpeechEngine **SpeechEngine);
		HRESULT STDMETHODCALLTYPE get_Count (long *Count);
	END_INTERFACE_PART(SpeechEngines)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASVRSPEECHENGINES_H_INCLUDED_
