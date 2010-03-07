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
#ifndef DASVRRECOGNITIONENGINE_H_INCLUDED_
#define DASVRRECOGNITIONENGINE_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E521-A208-11DE-ABF2-002421116FB2}")) CDaSvrRecognitionEngine : public CCmdTarget
{
public:
	CDaSvrRecognitionEngine ();
	virtual ~CDaSvrRecognitionEngine ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaSvrRecognitionEngine)
	DECLARE_OLETYPELIB(CDaSvrRecognitionEngine)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaSvrRecognitionEngine)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaSvrRecognitionEngine)
	afx_msg void DspGetSRModeID(BSTR * SRModeID);
	afx_msg void DspGetDisplayName(BSTR * DisplayName);
	afx_msg void DspGetManufacturer(BSTR * Manufacturer);
	afx_msg void DspGetVersion(short * MajorVersion, short * MinorVersion);
	afx_msg void DspGetLanguageID(long * LanguageID);
	afx_msg void DspGetLanguageName(BSTR * LanguageName, boolean EnglishName = TRUE);
	afx_msg void DspGetLanguages (long *Count, long **Languages = NULL, BSTR **LanguageNames = NULL, boolean EnglishNames = TRUE);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(RecognitionEngine, IDaSvrRecognitionEngine)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE GetSRModeID (BSTR *SRModeID);
		HRESULT STDMETHODCALLTYPE GetDisplayName (BSTR *DisplayName);
		HRESULT STDMETHODCALLTYPE GetManufacturer (BSTR *Manufacturer);
		HRESULT STDMETHODCALLTYPE GetVersion (short *MajorVersion, short *MinorVersion);
		HRESULT STDMETHODCALLTYPE GetLanguageID (long *LanguageID);
		HRESULT STDMETHODCALLTYPE GetLanguageName (BSTR *LanguageName, boolean EnglishName = TRUE);
		HRESULT STDMETHODCALLTYPE GetLanguages (long *Count, long **Languages = NULL, BSTR **LanguageNames = NULL, boolean EnglishNames = TRUE);
	END_INTERFACE_PART(RecognitionEngine)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASVRRECOGNITIONENGINE_H_INCLUDED_
