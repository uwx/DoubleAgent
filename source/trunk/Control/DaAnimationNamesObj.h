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

class __declspec(uuid("{1147E54B-A208-11DE-ABF2-002421116FB2}")) CDaAnimationNamesObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaAnimationNamesObj)
	DECLARE_OLECREATE_EX(CDaAnimationNamesObj)
	DECLARE_OLETYPELIB(CDaAnimationNamesObj)
public:
	CDaAnimationNamesObj (class CDaCharacterObj & pOwner);
	~CDaAnimationNamesObj ();
	void Terminate (bool pFinal);

// Attributes
	IEnumVARIANTPtr	mServerObject;

	class CDaCharacterObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Operations

// Overrides
	//{{AFX_VIRTUAL(CDaAnimationNamesObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAnimationNamesObj)
	afx_msg LPUNKNOWN DspEnum();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(AnimationNames, IDaCtlAnimationNames)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Enum (IUnknown **ppunkEnum);
	END_INTERFACE_PART(AnimationNames)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	class CDaCharacterObj *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAANIMATIONNAMESOBJ_H_INCLUDED_
