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
#include "DaSvrCommand.h"
#include "ServerNotify.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E50F-A208-11DE-ABF2-002421116FB2}")) DaSvrCommands :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCommands, &__uuidof(DaSvrCommands)>,
	public IDispatchImpl<IDaSvrCommands2, &__uuidof(IDaSvrCommands2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCommands), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo
{
public:
	DaSvrCommands ();
	virtual ~DaSvrCommands ();

// Attributes
public:
	bool							mVisible;
	CString							mCaption;
	CString							mVoiceGrammar;
	CString							mVoiceCaption;
	ULONG							mHelpContextId;
	CString							mFontName;
	long							mFontSize;
	bool							mGlobalVoiceCommandsEnabled;
	CAtlOwnPtrArray <DaSvrCommand>	mCommands;
	USHORT							mDefaultId;
	const USHORT					mShowCharacterCmdId;
	const USHORT					mHideCharacterCmdId;
	const USHORT					mShowCommandsCmdId;
	const USHORT					mHideCommandsCmdId;

	long GetCharID () const;
	LANGID GetLangID () const;

// Operations
public:
	static DaSvrCommands * CreateInstance (long pCharID, _IServerNotify * pNotify);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

	bool SetLangID (LANGID pLangId);
	USHORT DoContextMenu (HWND pOwner, const CPoint & pPosition);

	DaSvrCommand * GetCommand (USHORT pCommandId);
	DaSvrCommand * GetDefaultCommand ();

	CString GetVoiceCommandsCaption () const;
	bool ShowVoiceCommands (CVoiceCommandsWnd * pVoiceCommandsWnd);
	bool SetupVoiceContext (class CSapi5InputContext * pInputContext);

// Overrides

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRCOMMANDS)
	DECLARE_NOT_AGGREGATABLE(DaSvrCommands)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrCommands)
		COM_INTERFACE_ENTRY(IDaSvrCommands2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCommands2)
		COM_INTERFACE_ENTRY2(IDaSvrCommands2, IDaSvrCommands)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommands), IDaSvrCommands2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommandsEx), IDaSvrCommands2)
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

	// IDaSvrCommands2
	HRESULT STDMETHODCALLTYPE GetCommand (long CommandID, IUnknown **Command);
	HRESULT STDMETHODCALLTYPE GetCount (long *Count);
	HRESULT STDMETHODCALLTYPE SetCaption (BSTR Caption);
	HRESULT STDMETHODCALLTYPE GetCaption (BSTR *Caption);
	HRESULT STDMETHODCALLTYPE SetVoice (BSTR Voice);
	HRESULT STDMETHODCALLTYPE GetVoice (BSTR *Voice);
	HRESULT STDMETHODCALLTYPE SetVisible (long Visible);
	HRESULT STDMETHODCALLTYPE GetVisible (long *Visible);
	HRESULT STDMETHODCALLTYPE Add (BSTR Caption, BSTR Voice, long Enabled, long Visible, long *CommandID);
	HRESULT STDMETHODCALLTYPE Insert (BSTR Caption, BSTR Voice, long Enabled, long Visible, long RefCommandID, long Before, long *CommandID);
	HRESULT STDMETHODCALLTYPE Remove (long CommandID);
	HRESULT STDMETHODCALLTYPE RemoveAll (void);

	HRESULT STDMETHODCALLTYPE GetCommandEx (long CommandID, IDaSvrCommand **Command);
	HRESULT STDMETHODCALLTYPE SetDefaultID (long CommandID);
	HRESULT STDMETHODCALLTYPE GetDefaultID (long *CommandID);
	HRESULT STDMETHODCALLTYPE SetHelpContextID (long HelpContextID);
	HRESULT STDMETHODCALLTYPE GetHelpContextID (long *pulHelpID);
	HRESULT STDMETHODCALLTYPE SetFontName (BSTR FontName);
	HRESULT STDMETHODCALLTYPE GetFontName (BSTR *FontName);
	HRESULT STDMETHODCALLTYPE SetFontSize (long FontSize);
	HRESULT STDMETHODCALLTYPE GetFontSize (long *FontSize);
	HRESULT STDMETHODCALLTYPE SetVoiceCaption (BSTR bszVoiceCaption);
	HRESULT STDMETHODCALLTYPE GetVoiceCaption (BSTR *bszVoiceCaption);
	HRESULT STDMETHODCALLTYPE AddEx (BSTR Caption, BSTR Voice, BSTR bszVoiceCaption, long Enabled, long Visible, long HelpContextID, long *CommandID);
	HRESULT STDMETHODCALLTYPE InsertEx (BSTR Caption, BSTR Voice, BSTR bszVoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long *CommandID);
	HRESULT STDMETHODCALLTYPE SetGlobalVoiceCommandsEnabled (long Enabled);
	HRESULT STDMETHODCALLTYPE GetGlobalVoiceCommandsEnabled (long *Enabled);

	HRESULT STDMETHODCALLTYPE get_Item (long Index, IDaSvrCommand2 **Command);
	HRESULT STDMETHODCALLTYPE get_Count (long *Count);
	HRESULT STDMETHODCALLTYPE get_Command (long CommandID, IDaSvrCommand2 **Command);
	HRESULT STDMETHODCALLTYPE get_Caption (BSTR *Caption);
	HRESULT STDMETHODCALLTYPE put_Caption (BSTR Caption);
	HRESULT STDMETHODCALLTYPE get_VoiceGrammar (BSTR *VoiceGrammar);
	HRESULT STDMETHODCALLTYPE put_VoiceGrammar (BSTR VoiceGrammar);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_DefaultCommand (long *CommandID);
	HRESULT STDMETHODCALLTYPE put_DefaultCommand (long CommandID);
	HRESULT STDMETHODCALLTYPE get_FontName (BSTR *FontName);
	HRESULT STDMETHODCALLTYPE put_FontName (BSTR FontName);
	HRESULT STDMETHODCALLTYPE get_FontSize (long *FontSize);
	HRESULT STDMETHODCALLTYPE put_FontSize (long FontSize);
	HRESULT STDMETHODCALLTYPE get_VoiceCaption (BSTR *VoiceCaption);
	HRESULT STDMETHODCALLTYPE put_VoiceCaption (BSTR VoiceCaption);
	HRESULT STDMETHODCALLTYPE get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get__NewEnum (IUnknown **ppunkEnum);

// Implementation
protected:
	int FindCommand (USHORT pCommandId);
	DaSvrCommand * NewCommand (LPCTSTR pCaption, LPCTSTR pVoice = NULL, LPCTSTR pVoiceCaption = NULL, bool pEnabled = true, bool pVisible = true, ULONG pHelpContextId = 0);
	bool RemoveCommand (int pCommandNdx);

protected:
	long							mCharID;
	LANGID							mLangID;
	_IServerNotify *				mNotify;
	CAtlOwnPtrArray <DaSvrCommand>	mRemoved;
	USHORT							mNextCommandId;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCommands), DaSvrCommands)

/////////////////////////////////////////////////////////////////////////////
