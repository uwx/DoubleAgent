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
#ifndef DACHARACTERFILESOBJ_H_INCLUDED_
#define DACHARACTERFILESOBJ_H_INCLUDED_
#pragma once

#include "DaAgentCtl.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E558-A208-11DE-ABF2-002421116FB2}")) CDaCharacterFilesObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaCharacterFilesObj)
	DECLARE_OLECREATE_EX(CDaCharacterFilesObj)
	DECLARE_OLETYPELIB(CDaCharacterFilesObj)
public:
	CDaCharacterFilesObj (CDaAgentCtl & pOwner);
	~CDaCharacterFilesObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrCharacterFilesPtr	mServerObject;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Overrides
	//{{AFX_VIRTUAL(CDaCharacterFilesObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaCharacterFilesObj)
	afx_msg SAFEARRAY * DspGetFilePaths();
	afx_msg void DspSetFilePaths(SAFEARRAY * FilePaths);
	afx_msg BSTR DspGetSearchPath();
	afx_msg void DspSetSearchPath(LPCTSTR SearchPath);
	afx_msg BSTR DspGetDefaultSearchPath();
	afx_msg void DspSetDefaultSearchPath(LPCTSTR SearchPath);
	afx_msg BOOL DspGetDoubleAgentFiles();
	afx_msg void DspSetDoubleAgentFiles(BOOL SearchPath);
	afx_msg BOOL DspGetMsAgentFiles();
	afx_msg void DspSetMsAgentFiles(BOOL SearchPath);
	afx_msg BOOL DspGetMsOfficeFiles();
	afx_msg void DspSetMsOfficeFiles(BOOL SearchPath);
	afx_msg BOOL DspGetSpeakingCharacters();
	afx_msg void DspSetSpeakingCharacters(BOOL SearchPath);
	afx_msg BOOL DspGetNonSpeakingCharacters();
	afx_msg void DspSetNonSpeakingCharacters(BOOL SearchPath);
	afx_msg BOOL DspGetVerifyVersion();
	afx_msg void DspSetVerifyVersion(BOOL SearchPath);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(CharacterFiles, IDaCtlCharacterFiles)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_FilePaths (SAFEARRAY **FilePaths);
		HRESULT STDMETHODCALLTYPE get_SearchPath (BSTR *SearchPath);
		HRESULT STDMETHODCALLTYPE put_SearchPath (BSTR SearchPath);
        HRESULT STDMETHODCALLTYPE get_DefaultSearchPath (BSTR *DefaultSearchPath);
		HRESULT STDMETHODCALLTYPE get_DoubleAgentFiles (VARIANT_BOOL *DoubleAgentFiles);
		HRESULT STDMETHODCALLTYPE put_DoubleAgentFiles (VARIANT_BOOL DoubleAgentFiles);
		HRESULT STDMETHODCALLTYPE get_MsAgentFiles (VARIANT_BOOL *MsAgentFiles);
		HRESULT STDMETHODCALLTYPE put_MsAgentFiles (VARIANT_BOOL MsAgentFiles);
		HRESULT STDMETHODCALLTYPE get_MsOfficeFiles (VARIANT_BOOL *MsOfficeFiles);
		HRESULT STDMETHODCALLTYPE put_MsOfficeFiles (VARIANT_BOOL MsOfficeFiles);
		HRESULT STDMETHODCALLTYPE get_SpeakingCharacters (VARIANT_BOOL *SpeakingCharacters);
		HRESULT STDMETHODCALLTYPE put_SpeakingCharacters (VARIANT_BOOL SpeakingCharacters);
		HRESULT STDMETHODCALLTYPE get_NonSpeakingCharacters (VARIANT_BOOL *NonSpeakingCharacters);
		HRESULT STDMETHODCALLTYPE put_NonSpeakingCharacters (VARIANT_BOOL NonSpeakingCharacters);
		HRESULT STDMETHODCALLTYPE get_VerifyVersion (VARIANT_BOOL *VerifyVersion);
		HRESULT STDMETHODCALLTYPE put_VerifyVersion (VARIANT_BOOL VerifyVersion);
	END_INTERFACE_PART(CharacterFiles)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACHARACTERFILESOBJ_H_INCLUDED_
