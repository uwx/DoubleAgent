/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaCmnCommands.h"
#include "CorEnumVariant.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E534-A208-11DE-ABF2-002421116FB2}")) DaCtlCommands :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCommands, &__uuidof(DaCtlCommands)>,
	public IDispatchImpl<IDaCtlCommands2, &__uuidof(IDaCtlCommands2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCommands), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlCommands();
	~DaCtlCommands();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrCommands2Ptr														mServerObject;
#endif
	tPtr <CDaCmnCommands>													mLocalObject;
	CAtlMap <CAtlString, IDispatchPtr, CStringElementTraitsI <CAtlString> >	mCommands;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaCtlCharacter * pOwner);
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
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(DaCtlCommands)
		COM_INTERFACE_ENTRY(IDaCtlCommands2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCommands2)
		COM_INTERFACE_ENTRY2(IDaCtlCommands, IDaCtlCommands2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommands), IDaCtlCommands2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCommandsEx), IDaCtlCommands2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(IEnumVARIANT), CCorEnumVariant<DaCtlCommands>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IEnumerable), CCorEnumVariant<DaCtlCommands>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::IList), CCorEnumVariant<DaCtlCommands>, mCachedEnum)
		COM_INTERFACE_ENTRY_CACHED_TEAR_OFF(__uuidof(mscorlib::ICollection), CCorEnumVariant<DaCtlCommands>, mCachedEnum)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCommands)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlCommands2
	HRESULT STDMETHODCALLTYPE get_Item (BSTR Name, IDaCtlCommand2 ** Item);
	HRESULT STDMETHODCALLTYPE Command (BSTR Name, IDaCtlCommand2 ** Command);
	HRESULT STDMETHODCALLTYPE get_Count (long* Count);
	HRESULT STDMETHODCALLTYPE get_Caption (BSTR * Caption);
	HRESULT STDMETHODCALLTYPE put_Caption (BSTR Caption);
	HRESULT STDMETHODCALLTYPE get_Voice (BSTR * Voice);
	HRESULT STDMETHODCALLTYPE put_Voice (BSTR Voice);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL * Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown ** EnumVariant);
	HRESULT STDMETHODCALLTYPE Add (BSTR Name, VARIANT Caption, VARIANT VoiceGrammar, VARIANT Enabled, VARIANT Visible, IDaCtlCommand2 ** Command);
	HRESULT STDMETHODCALLTYPE Insert (BSTR Name, BSTR RefName, VARIANT Before, VARIANT Caption, VARIANT VoiceGrammar, VARIANT Enabled, VARIANT Visible, IDaCtlCommand2 ** Command);
	HRESULT STDMETHODCALLTYPE Remove (BSTR Name);
	HRESULT STDMETHODCALLTYPE RemoveAll ();
	HRESULT STDMETHODCALLTYPE get_DefaultCommand (BSTR * Name);
	HRESULT STDMETHODCALLTYPE put_DefaultCommand (BSTR Name);
	HRESULT STDMETHODCALLTYPE get_HelpContextID (long* ID);
	HRESULT STDMETHODCALLTYPE put_HelpContextID (long ID);
	HRESULT STDMETHODCALLTYPE get_FontName (BSTR * FontName);
	HRESULT STDMETHODCALLTYPE put_FontName (BSTR FontName);
	HRESULT STDMETHODCALLTYPE get_FontSize (long* FontSize);
	HRESULT STDMETHODCALLTYPE put_FontSize (long FontSize);
	HRESULT STDMETHODCALLTYPE get_VoiceCaption (BSTR * VoiceCaption);
	HRESULT STDMETHODCALLTYPE put_VoiceCaption (BSTR VoiceCaption);
	HRESULT STDMETHODCALLTYPE get_GlobalVoiceCommandsEnabled (VARIANT_BOOL * Enabled);
	HRESULT STDMETHODCALLTYPE put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled);

	HRESULT STDMETHODCALLTYPE get_VoiceGrammar (BSTR * VoiceGrammar);
	HRESULT STDMETHODCALLTYPE put_VoiceGrammar (BSTR VoiceGrammar);
	HRESULT STDMETHODCALLTYPE get_Index (long Index, IDaCtlCommand2 ** Command);

// Implementation
public:
	HRESULT InitEnumVariant (CEnumVARIANTImpl * pEnum);

private:
	DaCtlCharacter *	mOwner;
	IUnknownPtr			mCachedEnum;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCommands), DaCtlCommands)

/////////////////////////////////////////////////////////////////////////////
