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
#include "DaCmnCommandsWindow.h"

/////////////////////////////////////////////////////////////////////////////

class CVoiceCommandsWnd;

class ATL_NO_VTABLE __declspec(uuid("{1147E510-A208-11DE-ABF2-002421116FB2}")) DaSvrCommandsWindow :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrCommandsWindow, &__uuidof(DaSvrCommandsWindow)>,
	public IDispatchImpl<IDaSvrCommandsWindow2, &__uuidof(IDaSvrCommandsWindow2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrCommandsWindow), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public IOleWindow,
	public CDaCmnCommandsWindow,
	public CSvrObjLifetime
{
public:
	DaSvrCommandsWindow ();
	virtual ~DaSvrCommandsWindow ();

// Attributes
public:

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
		COM_INTERFACE_ENTRY(IDaSvrCommandsWindow2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrCommandsWindow2)
		COM_INTERFACE_ENTRY2(IDaSvrCommandsWindow2, IDaSvrCommandsWindow)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCommandWindow), IDaSvrCommandsWindow2)
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
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrCommandsWindow2
	HRESULT STDMETHODCALLTYPE GetVisible (long *Visible);
	HRESULT STDMETHODCALLTYPE SetVisible (long Visible);
	HRESULT STDMETHODCALLTYPE GetPosition (long *Left, long *Top);
	HRESULT STDMETHODCALLTYPE GetSize (long *Width, long *Height);

	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_Left (short *Left);
	HRESULT STDMETHODCALLTYPE get_Top (short *Top);
	HRESULT STDMETHODCALLTYPE get_Height (short *Height);
	HRESULT STDMETHODCALLTYPE get_Width (short *Width);

	// IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrCommandsWindow), DaSvrCommandsWindow)

/////////////////////////////////////////////////////////////////////////////
