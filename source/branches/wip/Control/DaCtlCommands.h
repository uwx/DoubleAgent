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
#include "DaControl.h"
#include "DaCtlCharacter.h"
#include "StringMap.h"

class ATL_NO_VTABLE CDaCtlCommands :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDaCtlCommands, &__uuidof(DaCtlCommands)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommands), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCommands, &__uuidof(IDaCtlCommands), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	CDaCtlCommands();
	~CDaCtlCommands();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCOMMANDS)
	DECLARE_NOT_AGGREGATABLE(CDaCtlCommands)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaCtlCommands)
		COM_INTERFACE_ENTRY(IDaCtlCommands)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCommands)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommands), IDaCtlCommands)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommandsEx), IDaCtlCommands)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CDaCtlCommands)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces:
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCommands
	STDMETHOD(get_Item)(BSTR Name,  IDaCtlCommand * * Item);
	STDMETHOD(Command)(BSTR Name,  IDaCtlCommand * * Item);
	STDMETHOD(get_Count)(long * Count);
	STDMETHOD(get_Caption)(BSTR * Caption);
	STDMETHOD(put_Caption)(BSTR Caption);
	STDMETHOD(get_Voice)(BSTR * Voice);
	STDMETHOD(put_Voice)(BSTR Voice);
	STDMETHOD(get_Visible)(VARIANT_BOOL * Visible);
	STDMETHOD(put_Visible)(VARIANT_BOOL Visible);
	STDMETHOD(get__NewEnum)(IUnknown * * ppunkEnum);
	STDMETHOD(Add)(BSTR Name,  VARIANT Caption,  VARIANT Voice,  VARIANT Enabled,  VARIANT Visible,  IDaCtlCommand * * Command);
	STDMETHOD(Insert)(BSTR Name,  BSTR RefName,  VARIANT Before,  VARIANT Caption,  VARIANT Voice,  VARIANT Enabled,  VARIANT Visible,  IDaCtlCommand * * Command);
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

// Attributes
public:
	IDaSvrCommandsPtr						mServerObject;
	CStringMap <IDispatchPtr, LPDISPATCH>	mCommands;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	void SetOwner (CDaCtlCharacter * pOwner);
	CDaCtlCharacter * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	class CDaCtlCommand * GetCommand (LPCTSTR pCommandName);
	class CDaCtlCommand * GetCommand (long pCommandId);
	CString GetCommandName (long pCommandId);
	long GetCommandId (LPCTSTR pCommandName);

// Implementation
private:
	CDaCtlCharacter *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCommands), CDaCtlCommands)

/////////////////////////////////////////////////////////////////////////////
