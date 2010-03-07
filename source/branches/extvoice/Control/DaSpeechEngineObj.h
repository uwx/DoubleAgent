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
#ifndef DASPEECHENGINEOBJ_H_INCLUDED_
#define DASPEECHENGINEOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E553-A208-11DE-ABF2-002421116FB2}")) CDaSpeechEngineObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaSpeechEngineObj)
	DECLARE_OLECREATE_EX(CDaSpeechEngineObj)
	DECLARE_OLETYPELIB(CDaSpeechEngineObj)
public:
	CDaSpeechEngineObj (IDaSvrSpeechEngine * pServerObject);
	~CDaSpeechEngineObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrSpeechEnginePtr	mServerObject;

// Overrides
	//{{AFX_VIRTUAL(CDaSpeechEngineObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaSpeechEngineObj)
	afx_msg BSTR DspGetTTSModeID();
	afx_msg void DspSetTTSModeID(LPCTSTR TTSModeID);
	afx_msg BSTR DspGetDisplayName();
	afx_msg void DspSetDisplayName(LPCTSTR DisplayName);
	afx_msg BSTR DspGetManufacturer();
	afx_msg void DspSetManufacturer(LPCTSTR Manufacturer);
	afx_msg void DspGetVersion(short *MajorVersion, short *MinorVersion);
	afx_msg short DspGetGender();
	afx_msg void DspSetGender(short Gender);
	afx_msg long DspGetLanguageID();
	afx_msg void DspSetLanguageID(long LanguageID);
	afx_msg BSTR DspGetLanguageName(BOOL EnglishName = TRUE);
	afx_msg void DspSetLanguageName(BOOL EnglishName, BSTR LanguageName);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(SpeechEngine, IDaCtlSpeechEngine)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR *TTSModeID);
		HRESULT STDMETHODCALLTYPE get_DisplayName (BSTR *DisplayName);
		HRESULT STDMETHODCALLTYPE get_Manufacturer (BSTR *Manufacturer);
		HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
		HRESULT STDMETHODCALLTYPE get_Gender (short *Gender);
		HRESULT STDMETHODCALLTYPE get_LanguageID (long *LanguageID);
		HRESULT STDMETHODCALLTYPE get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName);
	END_INTERFACE_PART(SpeechEngine)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASPEECHENGINEOBJ_H_INCLUDED_
