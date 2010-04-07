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
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class CVoiceCommandsWnd;

class ATL_NO_VTABLE __declspec(uuid("{1147E510-A208-11DE-ABF2-002421116FB2}")) DaSvrCommandsWindow :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCommandsWindow, &__uuidof(DaSvrCommandsWindow)>,
	public IDispatchImpl<IDaSvrCommandsWindow, &__uuidof(IDaSvrCommandsWindow), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCommandsWindow), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public IOleWindow,
	public CSvrObjLifetime
{
public:
	DaSvrCommandsWindow ();
	virtual ~DaSvrCommandsWindow ();

// Attributes
public:
	CVoiceCommandsWnd *	mVoiceCommandsWnd;

// Operations
public:
	static DaSvrCommandsWindow * CreateInstance (CVoiceCommandsWnd * pVoiceCommandsWnd, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRCOMMANDSWINDOW)
	DECLARE_NOT_AGGREGATABLE(DaSvrCommandsWindow)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrCommandsWindow)
		COM_INTERFACE_ENTRY(IDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommandWindow), IDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IOleWindow)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrPropertySheet)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrCommandsWindow
	HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
	HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
	HRESULT STDMETHODCALLTYPE GetPosition (long *plLeft, long *plTop);
	HRESULT STDMETHODCALLTYPE GetSize (long *plWidth, long *plHeight);

	// IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCommandsWindow), DaSvrCommandsWindow)

/////////////////////////////////////////////////////////////////////////////
