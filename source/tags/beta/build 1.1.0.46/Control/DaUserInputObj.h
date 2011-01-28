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
#ifndef DAANIMATIONNAMESOBJ_H_INCLUDED_
#define DAANIMATIONNAMESOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E53A-A208-11DE-ABF2-002421116FB2}")) CDaUserInputObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaUserInputObj)
	DECLARE_OLECREATE_EX(CDaUserInputObj)
	DECLARE_OLETYPELIB(CDaUserInputObj)
public:
	CDaUserInputObj();
	~CDaUserInputObj();
	void Terminate (bool pFinal);

// Attributes
	CString					mCharacterID;
	long					mCommandID;
	class CDaCommandsObj *	mCommands;
	IDaSvrUserInputPtr		mServerObject;

// Overrides
	//{{AFX_VIRTUAL(CDaUserInputObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaUserInputObj)
	afx_msg short DspGetCount();
	afx_msg void DspSetCount(short nNewValue);
	afx_msg BSTR DspGetName();
	afx_msg void DspSetName(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetCharacterID();
	afx_msg void DspSetCharacterID(LPCTSTR lpszNewValue);
	afx_msg long DspGetConfidence();
	afx_msg void DspSetConfidence(long nNewValue);
	afx_msg BSTR DspGetVoice();
	afx_msg void DspSetVoice(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetAlt1Name();
	afx_msg void DspSetAlt1Name(LPCTSTR lpszNewValue);
	afx_msg long DspGetAlt1Confidence();
	afx_msg void DspSetAlt1Confidence(long nNewValue);
	afx_msg BSTR DspGetAlt1Voice();
	afx_msg void DspSetAlt1Voice(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetAlt2Name();
	afx_msg void DspSetAlt2Name(LPCTSTR lpszNewValue);
	afx_msg long DspGetAlt2Confidence();
	afx_msg void DspSetAlt2Confidence(long nNewValue);
	afx_msg BSTR DspGetAlt2Voice();
	afx_msg void DspSetAlt2Voice(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(UserInput, IDaCtlUserInput)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Count (short *pCount);
		HRESULT STDMETHODCALLTYPE get_Name (BSTR *pName);
		HRESULT STDMETHODCALLTYPE get_CharacterID (BSTR *pCharacterID);
		HRESULT STDMETHODCALLTYPE get_Confidence (long *pConfidence);
		HRESULT STDMETHODCALLTYPE get_Voice (BSTR *pVoice);
		HRESULT STDMETHODCALLTYPE get_Alt1Name (BSTR *pAlt1Name);
		HRESULT STDMETHODCALLTYPE get_Alt1Confidence (long *pAlt1Confidence);
		HRESULT STDMETHODCALLTYPE get_Alt1Voice (BSTR *pAlt1Voice);
		HRESULT STDMETHODCALLTYPE get_Alt2Name (BSTR *pAlt2Name);
		HRESULT STDMETHODCALLTYPE get_Alt2Confidence (long *pAlt2Confidence);
		HRESULT STDMETHODCALLTYPE get_Alt2Voice (BSTR *pAlt2Voice);
	END_INTERFACE_PART(UserInput)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAANIMATIONNAMESOBJ_H_INCLUDED_
