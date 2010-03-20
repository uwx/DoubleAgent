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
#ifndef DAAUDIOOUTPUTOBJ_H_INCLUDED_
#define DAAUDIOOUTPUTOBJ_H_INCLUDED_
#pragma once

#include "DaAgentCtl.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E537-A208-11DE-ABF2-002421116FB2}"))  CDaAudioOutputObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaAudioOutputObj)
	DECLARE_OLECREATE_EX(CDaAudioOutputObj)
	DECLARE_OLETYPELIB(CDaAudioOutputObj)
public:
	CDaAudioOutputObj (CDaAgentCtl & pOwner);
	~CDaAudioOutputObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrAudioOutputPropertiesPtr	mServerObject;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Overrides
	//{{AFX_VIRTUAL(CDaAudioOutputObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAudioOutputObj)
	afx_msg short DspGetStatus();
	afx_msg void DspSetStatus(short nNewValue);
	afx_msg BOOL DspGetEnabled();
	afx_msg void DspSetEnabled(BOOL bNewValue);
	afx_msg BOOL DspGetSoundEffects();
	afx_msg void DspSetSoundEffects(BOOL bNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(AudioOutput, IDaCtlAudioObject)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL *AudioEnabled);
		HRESULT STDMETHODCALLTYPE get_SoundEffects (VARIANT_BOOL *SoundEffects);
		HRESULT STDMETHODCALLTYPE get_Status (short *Available);
	END_INTERFACE_PART(AudioOutput)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAUDIOOUTPUTOBJ_H_INCLUDED_
