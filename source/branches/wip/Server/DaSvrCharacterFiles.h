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
#ifndef DASVRCHARACTERFILES_H_INCLUDED_
#define DASVRCHARACTERFILES_H_INCLUDED_
#pragma once

#include "DaServerLifetime.h"
#include "EnumVariant.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E514-A208-11DE-ABF2-002421116FB2}")) CDaSvrCharacterFiles : public CEnumVariant, public CDaObjectLifetime
{
protected:
	CDaSvrCharacterFiles (const CDaSvrCharacterFiles & pSource);
public:
	CDaSvrCharacterFiles (LPCTSTR pClientMutexName = NULL);
	virtual ~CDaSvrCharacterFiles ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaSvrCharacterFiles)
	DECLARE_OLETYPELIB(CDaSvrCharacterFiles)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaSvrCharacterFiles)
	public:
	virtual void OnFinalRelease();
	virtual void OnClientEnded ();
	protected:
	virtual long GetItemCount ();
	virtual void PutItem (long pItemNdx, VARIANT & pItem);
	virtual CEnumVariant * Clone ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaSvrCharacterFiles)
	afx_msg SAFEARRAY * DspGetFilePaths();
	afx_msg void DspSetFilePaths(SAFEARRAY * FilePaths);
	afx_msg BSTR DspGetSearchPath();
	afx_msg void DspSetSearchPath(LPCTSTR SearchPath);
	afx_msg BSTR DspGetDefaultSearchPath();
	afx_msg void DspSetDefaultSearchPath(LPCTSTR SearchPath);
	afx_msg long DspGetFilter();
	afx_msg void DspSetFilter(long Filter);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(CharacterFiles, IDaSvrCharacterFiles)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_FilePaths (SAFEARRAY **FilePaths);
		HRESULT STDMETHODCALLTYPE get_SearchPath (BSTR *SearchPath);
		HRESULT STDMETHODCALLTYPE put_SearchPath (BSTR SearchPath);
		HRESULT STDMETHODCALLTYPE get_DefaultSearchPath (BSTR *DefaultSearchPath);
		HRESULT STDMETHODCALLTYPE get_Filter (long *Filter);
		HRESULT STDMETHODCALLTYPE put_Filter (long Filter);
	END_INTERFACE_PART(CharacterFiles)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
	void GetDefaultSearch ();
	void GetFilePaths ();
	void UpdateFilter (DWORD pNewFilter);

protected:
	CStringArray	mDefaultSearchPath;
	CStringArray	mSearchPath;
	CStringArray	mFilePaths;
	DWORD			mFilter;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASVRCHARACTERFILES_H_INCLUDED_
