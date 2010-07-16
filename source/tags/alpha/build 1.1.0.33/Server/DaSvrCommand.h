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
#include "DaCmnCommand.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E50E-A208-11DE-ABF2-002421116FB2}")) DaSvrCommand :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCommand, &__uuidof(DaSvrCommand)>,
	public IDispatchImpl<IDaSvrCommand2, &__uuidof(IDaSvrCommand2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCommand), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CDaCmnCommand,
	public CSvrObjLifetime
{
public:
	DaSvrCommand();
	virtual ~DaSvrCommand();

// Attributes
public:
	class DaSvrCommands *	mOwner;

// Operations
public:
	static DaSvrCommand * CreateInstance (class DaSvrCommands * pOwner, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRCOMMAND)
	DECLARE_NOT_AGGREGATABLE(DaSvrCommand)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrCommand)
		COM_INTERFACE_ENTRY(IDaSvrCommand2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCommand2)
		COM_INTERFACE_ENTRY2(IDaSvrCommand, IDaSvrCommand2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommand), IDaSvrCommand2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommandEx), IDaSvrCommand2)
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
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrCommand2
	HRESULT STDMETHODCALLTYPE SetCaption (BSTR Caption);
	HRESULT STDMETHODCALLTYPE GetCaption (BSTR *Caption);
	HRESULT STDMETHODCALLTYPE SetVoice (BSTR Voice);
	HRESULT STDMETHODCALLTYPE GetVoice (BSTR *Voice);
	HRESULT STDMETHODCALLTYPE SetEnabled (long Enabled);
	HRESULT STDMETHODCALLTYPE GetEnabled (long *Enabled);
	HRESULT STDMETHODCALLTYPE SetVisible (long Visible);
	HRESULT STDMETHODCALLTYPE GetVisible (long *Visible);
	HRESULT STDMETHODCALLTYPE SetConfidenceThreshold (long Threshold);
	HRESULT STDMETHODCALLTYPE GetConfidenceThreshold (long *Threshold);
	HRESULT STDMETHODCALLTYPE SetConfidenceText (BSTR TipText);
	HRESULT STDMETHODCALLTYPE GetConfidenceText (BSTR *TipText);
	HRESULT STDMETHODCALLTYPE GetID (long *CommandID);

	HRESULT STDMETHODCALLTYPE SetHelpContextID (long ContextID);
	HRESULT STDMETHODCALLTYPE GetHelpContextID (long *ContextID);
	HRESULT STDMETHODCALLTYPE SetVoiceCaption (BSTR VoiceCaption);
	HRESULT STDMETHODCALLTYPE GetVoiceCaption (BSTR *VoiceCaption);

	HRESULT STDMETHODCALLTYPE get_Caption (BSTR *Caption);
	HRESULT STDMETHODCALLTYPE put_Caption (BSTR Caption);
	HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE put_Enabled (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_CommandID (long *CommandID);
	HRESULT STDMETHODCALLTYPE get_VoiceCaption (BSTR *VoiceCaption);
	HRESULT STDMETHODCALLTYPE put_VoiceCaption (BSTR VoiceCaption);
	HRESULT STDMETHODCALLTYPE get_VoiceGrammar (BSTR *VoiceGrammar);
	HRESULT STDMETHODCALLTYPE put_VoiceGrammar (BSTR VoiceGrammar);
	HRESULT STDMETHODCALLTYPE get_ConfidenceThreshold (long *ConfidenceThreshold);
	HRESULT STDMETHODCALLTYPE put_ConfidenceThreshold (long ConfidenceThreshold);
	HRESULT STDMETHODCALLTYPE get_ConfidenceText (BSTR *ConfidenceText);
	HRESULT STDMETHODCALLTYPE put_ConfidenceText (BSTR ConfidenceText);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCommand), DaSvrCommand)

/////////////////////////////////////////////////////////////////////////////
