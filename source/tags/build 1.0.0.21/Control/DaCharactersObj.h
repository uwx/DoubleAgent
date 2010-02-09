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
#ifndef DACHARACTERSOBJ_H_INCLUDED_
#define DACHARACTERSOBJ_H_INCLUDED_
#pragma once

#include "DaAgentCtl.h"
#include "StringMap.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E531-A208-11DE-ABF2-002421116FB2}")) CDaCharactersObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaCharactersObj)
	DECLARE_OLECREATE_EX(CDaCharactersObj)
	DECLARE_OLETYPELIB(CDaCharactersObj)
public:
	CDaCharactersObj (CDaAgentCtl & pOwner);
	~CDaCharactersObj ();
	void Terminate (bool pFinal);

// Attributes
	CStringMap <IDispatchPtr, LPDISPATCH>	mCharacters;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Operations

// Overrides
	//{{AFX_VIRTUAL(CDaCharactersObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaCharactersObj)
	afx_msg LPDISPATCH DspLoad(LPCTSTR CharacterID, const VARIANT & LoadKey);
	afx_msg void DspUnload(LPCTSTR CharacterID);
	afx_msg LPDISPATCH DspCharacter(LPCTSTR CharacterID);
	afx_msg LPDISPATCH DspGetItem(LPCTSTR CharacterID);
	afx_msg void DspSetItem(LPCTSTR CharacterID, LPDISPATCH newValue);
	afx_msg LPUNKNOWN DspEnum();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Characters, IDaCtlCharacters)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Item (BSTR CharacterID, IDaCtlCharacter **ppidItem);
		HRESULT STDMETHODCALLTYPE Character (BSTR CharacterID, IDaCtlCharacter **ppidItem);
		HRESULT STDMETHODCALLTYPE get_Enum (IUnknown **ppunkEnum);
		HRESULT STDMETHODCALLTYPE Unload (BSTR CharacterID);
		HRESULT STDMETHODCALLTYPE Load (BSTR CharacterID, VARIANT LoadKey, IDaCtlRequest **ppidRequest);
	END_INTERFACE_PART(Characters)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACHARACTERSOBJ_H_INCLUDED_
