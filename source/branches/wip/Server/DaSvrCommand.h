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
#pragma once
#include "DaServerApp.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E50E-A208-11DE-ABF2-002421116FB2}")) DaSvrCommand :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCommand, &__uuidof(DaSvrCommand)>,
	public IDispatchImpl<IDaSvrCommand, &__uuidof(IDaSvrCommand), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCommand), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo
{
public:
	DaSvrCommand();
	virtual ~DaSvrCommand();

// Attributes
public:
	USHORT	mId;
	bool	mEnabled;
	bool	mVisible;
	CString	mCaption;
	CString	mVoice;
	CString	mVoiceCaption;
	short	mConfidenceThreshold;
	CString	mConfidenceText;
	ULONG	mHelpContextId;

// Operations
public:
	static DaSvrCommand * CreateInstance();
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRCOMMAND)
	DECLARE_NOT_AGGREGATABLE(DaSvrCommand)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrCommand)
		COM_INTERFACE_ENTRY(IDaSvrCommand)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCommand)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommand), IDaSvrCommand)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommandEx), IDaSvrCommand)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrPropertySheet)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrCommand
	HRESULT STDMETHODCALLTYPE SetCaption (BSTR bszCaption);
	HRESULT STDMETHODCALLTYPE GetCaption (BSTR *pbszCaption);
	HRESULT STDMETHODCALLTYPE SetVoice (BSTR bszVoice);
	HRESULT STDMETHODCALLTYPE GetVoice (BSTR *pbszVoice);
	HRESULT STDMETHODCALLTYPE SetEnabled (long bEnabled);
	HRESULT STDMETHODCALLTYPE GetEnabled (long *pbEnabled);
	HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
	HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
	HRESULT STDMETHODCALLTYPE SetConfidenceThreshold (long lThreshold);
	HRESULT STDMETHODCALLTYPE GetConfidenceThreshold (long *plThreshold);
	HRESULT STDMETHODCALLTYPE SetConfidenceText (BSTR bszTipText);
	HRESULT STDMETHODCALLTYPE GetConfidenceText (BSTR *pbszTipText);
	HRESULT STDMETHODCALLTYPE GetID (long *pdwID);

	HRESULT STDMETHODCALLTYPE SetHelpContextID (long ulID);
	HRESULT STDMETHODCALLTYPE GetHelpContextID (long *pulID);
	HRESULT STDMETHODCALLTYPE SetVoiceCaption (BSTR bszVoiceCaption);
	HRESULT STDMETHODCALLTYPE GetVoiceCaption (BSTR *pbszVoiceCaption);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCommand), DaSvrCommand)

/////////////////////////////////////////////////////////////////////////////
