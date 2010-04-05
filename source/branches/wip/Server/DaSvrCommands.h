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
	public IDispatchImpl<IDaSvrCommands, &__uuidof(IDaSvrCommands), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
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
	CString							mVoice;
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
		COM_INTERFACE_ENTRY(IDaSvrCommands)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCommands)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommands), IDaSvrCommands)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommandsEx), IDaSvrCommands)
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

	// IDaSvrCommands
	HRESULT STDMETHODCALLTYPE GetCommand (long dwCommandID, IUnknown **ppunkCommand);
	HRESULT STDMETHODCALLTYPE GetCount (long *pdwCount);
	HRESULT STDMETHODCALLTYPE SetCaption (BSTR bszCaption);
	HRESULT STDMETHODCALLTYPE GetCaption (BSTR *pbszCaption);
	HRESULT STDMETHODCALLTYPE SetVoice (BSTR bszVoice);
	HRESULT STDMETHODCALLTYPE GetVoice (BSTR *pbszVoice);
	HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
	HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
	HRESULT STDMETHODCALLTYPE Add (BSTR bszCaption, BSTR bszVoice, long bEnabled, long bVisible, long *pdwID);
	HRESULT STDMETHODCALLTYPE Insert (BSTR bszCaption, BSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long *pdwID);
	HRESULT STDMETHODCALLTYPE Remove (long dwID);
	HRESULT STDMETHODCALLTYPE RemoveAll (void);

	HRESULT STDMETHODCALLTYPE GetCommandEx (long dwCommandID, IDaSvrCommand **ppCommandEx);
	HRESULT STDMETHODCALLTYPE SetDefaultID (long dwID);
	HRESULT STDMETHODCALLTYPE GetDefaultID (long *pdwID);
	HRESULT STDMETHODCALLTYPE SetHelpContextID (long ulHelpID);
	HRESULT STDMETHODCALLTYPE GetHelpContextID (long *pulHelpID);
	HRESULT STDMETHODCALLTYPE SetFontName (BSTR bszFontName);
	HRESULT STDMETHODCALLTYPE GetFontName (BSTR *pbszFontName);
	HRESULT STDMETHODCALLTYPE SetFontSize (long lFontSize);
	HRESULT STDMETHODCALLTYPE GetFontSize (long *lFontSize);
	HRESULT STDMETHODCALLTYPE SetVoiceCaption (BSTR bszVoiceCaption);
	HRESULT STDMETHODCALLTYPE GetVoiceCaption (BSTR *bszVoiceCaption);
	HRESULT STDMETHODCALLTYPE AddEx (BSTR bszCaption, BSTR bszVoice, BSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long *pdwID);
	HRESULT STDMETHODCALLTYPE InsertEx (BSTR bszCaption, BSTR bszVoice, BSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long *pdwID);
	HRESULT STDMETHODCALLTYPE SetGlobalVoiceCommandsEnabled (long bEnable);
	HRESULT STDMETHODCALLTYPE GetGlobalVoiceCommandsEnabled (long *pbEnabled);

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
