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
#pragma once
#include "DaControlMod.h"
#include "DaCtlCharacter.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E534-A208-11DE-ABF2-002421116FB2}")) DaCtlCommands :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCommands, &__uuidof(DaCtlCommands)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommands), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCommands2, &__uuidof(IDaCtlCommands2), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	DaCtlCommands();
	~DaCtlCommands();

// Attributes
public:
	IDaSvrCommands2Ptr														mServerObject;
	CAtlMap <CAtlString, IDispatchPtr, CStringElementTraitsI <CAtlString> >	mCommands;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (DaCtlCharacter * pOwner);
	DaCtlCharacter * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	class DaCtlCommand * GetCommand (LPCTSTR pCommandName);
	class DaCtlCommand * GetCommand (long pCommandId);
	CAtlString GetCommandName (long pCommandId);
	long GetCommandId (LPCTSTR pCommandName);

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCOMMANDS)
	DECLARE_NOT_AGGREGATABLE(DaCtlCommands)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlCommands)
		COM_INTERFACE_ENTRY(IDaCtlCommands2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCommands2)
		COM_INTERFACE_ENTRY2(IDaCtlCommands, IDaCtlCommands2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommands), IDaCtlCommands2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommandsEx), IDaCtlCommands2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCommands)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCommands2
	STDMETHOD(get_Item)(BSTR Name, IDaCtlCommand2 ** Item);
	STDMETHOD(Command)(BSTR Name, IDaCtlCommand2 ** Item);
	STDMETHOD(get_Count)(long * Count);
	STDMETHOD(get_Caption)(BSTR * Caption);
	STDMETHOD(put_Caption)(BSTR Caption);
	STDMETHOD(get_Voice)(BSTR * Voice);
	STDMETHOD(put_Voice)(BSTR Voice);
	STDMETHOD(get_Visible)(VARIANT_BOOL * Visible);
	STDMETHOD(put_Visible)(VARIANT_BOOL Visible);
	STDMETHOD(get__NewEnum)(IUnknown ** ppunkEnum);
	STDMETHOD(Add)(BSTR Name, VARIANT Caption, VARIANT VoiceGrammar, VARIANT Enabled, VARIANT Visible, IDaCtlCommand2 ** Command);
	STDMETHOD(Insert)(BSTR Name, BSTR RefName, VARIANT Before, VARIANT Caption, VARIANT VoiceGrammar, VARIANT Enabled, VARIANT Visible, IDaCtlCommand2 ** Command);
	STDMETHOD(Remove)(BSTR Name);
	STDMETHOD(RemoveAll)();
	STDMETHOD(get_DefaultCommand)(BSTR * Name);
	STDMETHOD(put_DefaultCommand)(BSTR Name);
	STDMETHOD(get_HelpContextID)(long * ID);
	STDMETHOD(put_HelpContextID)(long ID);
	STDMETHOD(get_FontName)(BSTR * FontName);
	STDMETHOD(put_FontName)(BSTR FontName);
	STDMETHOD(get_FontSize)(long * FontSize);
	STDMETHOD(put_FontSize)(long FontSize);
	STDMETHOD(get_VoiceCaption)(BSTR * VoiceCaption);
	STDMETHOD(put_VoiceCaption)(BSTR VoiceCaption);
	STDMETHOD(get_GlobalVoiceCommandsEnabled)(VARIANT_BOOL * Enable);
	STDMETHOD(put_GlobalVoiceCommandsEnabled)(VARIANT_BOOL Enable);

	HRESULT STDMETHODCALLTYPE get_VoiceGrammar (BSTR * VoiceGrammar);
	HRESULT STDMETHODCALLTYPE put_VoiceGrammar (BSTR VoiceGrammar);

// Implementation
private:
	DaCtlCharacter *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCommands), DaCtlCommands)

/////////////////////////////////////////////////////////////////////////////
