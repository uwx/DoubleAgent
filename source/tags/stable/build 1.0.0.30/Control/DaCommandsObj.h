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
#ifndef DACOMMANDSOBJ_H_INCLUDED_
#define DACOMMANDSOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"
#include "StringMap.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E534-A208-11DE-ABF2-002421116FB2}")) CDaCommandsObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaCommandsObj)
	DECLARE_OLECREATE_EX(CDaCommandsObj)
	DECLARE_OLETYPELIB(CDaCommandsObj)
public:
	CDaCommandsObj (class CDaCharacterObj & pOwner);
	~CDaCommandsObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrCommandsPtr						mServerObject;
	CStringMap <IDispatchPtr, LPDISPATCH>	mCommands;

	class CDaCharacterObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Operations
	HRESULT RemoveAll ();

	class CDaCommandObj * GetCommand (LPCTSTR pCommandName);
	class CDaCommandObj * GetCommand (long pCommandId);
	CString GetCommandName (long pCommandId);
	long GetCommandId (LPCTSTR pCommandName);

// Overrides
	//{{AFX_VIRTUAL(CDaCommandsObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaCommandsObj)
	afx_msg LPDISPATCH DspGetItem(LPCTSTR Name);
	afx_msg void DspSetItem(LPCTSTR Name, LPDISPATCH newValue);
	afx_msg long DspGetCount();
	afx_msg void DspSetCount(long nNewValue);
	afx_msg BSTR DspGetCaption();
	afx_msg void DspSetCaption(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetVoice();
	afx_msg void DspSetVoice(LPCTSTR lpszNewValue);
	afx_msg BOOL DspGetVisible();
	afx_msg void DspSetVisible(BOOL bNewValue);
	afx_msg LPDISPATCH DspCommand(LPCTSTR Name);
	afx_msg LPDISPATCH DspAdd(LPCTSTR Name, const VARIANT & Caption, const VARIANT & Voice, const VARIANT & Enabled, const VARIANT & Visible);
	afx_msg LPDISPATCH DspInsert(LPCTSTR Name, LPCTSTR RefName, const VARIANT & Before, const VARIANT & Caption, const VARIANT & Voice, const VARIANT & Enabled, const VARIANT & Visible);
	afx_msg void DspRemove(LPCTSTR Name);
	afx_msg void DspRemoveAll();
	afx_msg BSTR DspGetDefaultCommand();
	afx_msg void DspSetDefaultCommand(LPCTSTR lpszNewValue);
	afx_msg long DspGetHelpContextID();
	afx_msg void DspSetHelpContextID(long nNewValue);
	afx_msg BSTR DspGetFontName();
	afx_msg void DspSetFontName(LPCTSTR lpszNewValue);
	afx_msg long DspGetFontSize();
	afx_msg void DspSetFontSize(long nNewValue);
	afx_msg BSTR DspGetVoiceCaption();
	afx_msg void DspSetVoiceCaption(LPCTSTR lpszNewValue);
	afx_msg BOOL DspGetGlobalVoiceCommandsEnabled();
	afx_msg void DspSetGlobalVoiceCommandsEnabled(BOOL bNewValue);
	afx_msg LPUNKNOWN DspEnum();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Commands, IDaCtlCommands)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Item (BSTR Name, IDaCtlCommand **Item);
		HRESULT STDMETHODCALLTYPE Command (BSTR Name, IDaCtlCommand **Item);
		HRESULT STDMETHODCALLTYPE get_Count (long *Count);
		HRESULT STDMETHODCALLTYPE get_Caption (BSTR *Caption);
		HRESULT STDMETHODCALLTYPE put_Caption (BSTR Caption);
		HRESULT STDMETHODCALLTYPE get_Voice (BSTR *Voice);
		HRESULT STDMETHODCALLTYPE put_Voice (BSTR Voice);
		HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
		HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
		HRESULT STDMETHODCALLTYPE get_Enum (IUnknown **ppunkEnum);
		HRESULT STDMETHODCALLTYPE Add (BSTR Name, VARIANT Caption, VARIANT Voice, VARIANT Enabled, VARIANT Visible, IDaCtlCommand **Command);
		HRESULT STDMETHODCALLTYPE Insert (BSTR Name, BSTR RefName, VARIANT Before, VARIANT Caption, VARIANT Voice, VARIANT Enabled, VARIANT Visible, IDaCtlCommand **Command);
		HRESULT STDMETHODCALLTYPE Remove (BSTR Name);
		HRESULT STDMETHODCALLTYPE RemoveAll (void);

		HRESULT STDMETHODCALLTYPE get_DefaultCommand (BSTR *Name);
		HRESULT STDMETHODCALLTYPE put_DefaultCommand (BSTR Name);
		HRESULT STDMETHODCALLTYPE get_HelpContextID (long *ID);
		HRESULT STDMETHODCALLTYPE put_HelpContextID (long ID);
		HRESULT STDMETHODCALLTYPE get_FontName (BSTR *FontName);
		HRESULT STDMETHODCALLTYPE put_FontName (BSTR FontName);
		HRESULT STDMETHODCALLTYPE get_FontSize (long *FontSize);
		HRESULT STDMETHODCALLTYPE put_FontSize (long FontSize);
		HRESULT STDMETHODCALLTYPE get_VoiceCaption (BSTR *VoiceCaption);
		HRESULT STDMETHODCALLTYPE put_VoiceCaption (BSTR VoiceCaption);
		HRESULT STDMETHODCALLTYPE get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enable);
		HRESULT STDMETHODCALLTYPE put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enable);
	END_INTERFACE_PART(Commands)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	class CDaCharacterObj *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACOMMANDSOBJ_H_INCLUDED_
