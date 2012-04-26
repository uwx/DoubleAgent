/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "AgentWnd.h"
#include "DaGuid.h"
#include "DaCoreRes.h"
#include "DaCoreOdl.h"

_COM_SMARTPTR_TYPEDEF (IDaPreview, __uuidof(IDaPreview));

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentPreviewObjectRoot : public CComObjectRootEx<CComMultiThreadModel> {};

class ATL_NO_VTABLE __declspec(uuid("{1147E561-A208-11DE-ABF2-002421116FB2}")) CAgentPreviewWnd :
	public CAgentPreviewObjectRoot,
	public CAgentWnd,
	public IDaPreview,
	public IOleWindow,
	public CComCoClass<CAgentPreviewWnd, &__uuidof(CAgentPreviewWnd)>
{
	DECLARE_DLL_OBJECT_EX(CAgentPreviewWnd, _DACORE_IMPEXP)
protected:
	_DACORE_IMPEXP CAgentPreviewWnd ();
public:
	_DACORE_IMPEXP virtual ~CAgentPreviewWnd ();
	_DACORE_IMPEXP static CAgentPreviewWnd* CreateInstance ();

// Attributes
public:

// Operations
public:
	_DACORE_IMPEXP bool Create (HWND pParentWnd, CRect* pInitialRect = NULL);
	_DACORE_IMPEXP void FinalRelease();

// Overrides
public:
	_DACORE_IMPEXP virtual DWORD GetAlphaSmoothing () const;
protected:
	_DACORE_IMPEXP virtual void Opened ();
	_DACORE_IMPEXP virtual bool DoAnimationQueue (bool& pNextActivateImmediate, DWORD& pNextQueueTime);
	_DACORE_IMPEXP virtual bool DoIdle ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_AGENTPREVIEWWND)
	DECLARE_NOT_AGGREGATABLE(CAgentPreviewWnd)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CAgentPreviewWnd)
		COM_INTERFACE_ENTRY(IDaPreview)
		COM_INTERFACE_ENTRY(IOleWindow)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CAgentPreviewWnd)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	END_CATEGORY_MAP()

// Interfaces
public:
	// IOleWindow
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);

	// IDaPreview
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE PlayAnimation (BSTR pAnimationName);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE PlayState (BSTR pStateName);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetIsPlaying (boolean *pIsPlaying);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE StopPlaying ();
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetSoundsEnabled (boolean *pEnabled);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE EnableSounds (boolean pEnabled);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetIdleEnabled (boolean *pEnabled);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE EnableIdle (boolean pEnabled);
	_DACORE_IMPEXP HRESULT STDMETHODCALLTYPE SetBkColor (COLORREF pBkColor);
	_DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetFrameSize (SIZE* pFrameSize);
	_DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetCharacterFrameSize (BSTR pCharacterPath, SIZE* pFrameSize);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetFrameFormat (BYTE**pFrameFormat);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetCharacterFrameFormat (BSTR pCharacterPath, BYTE**pFrameFormat);
	_DACORE_IMPEXP HRESULT STDMETHODCALLTYPE OnAppActive (boolean pActive);
	_DACORE_IMPEXP HRESULT STDMETHODCALLTYPE RenderFrame (HDC pDC, POINT *pPosition);
	_DACORE_IMPEXP HRESULT STDMETHODCALLTYPE RenderAnimationFrame (BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE RenderCharacterFrame (BSTR pCharacterPath, BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE OpenFile (BSTR pCharacterPath, HWND pParentWnd);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetCharacterPath (BSTR *pCharacterPath);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetCharacterName (BSTR *pCharacterName, USHORT pLangID = 0);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetCharacterDescription (BSTR *pCharacterDescription, USHORT pLangID = 0);
    _DACORE_IMPEXP HRESULT STDMETHODCALLTYPE GetCharacterGuid (GUID *pGuid);

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAgentPreviewWnd)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		CHAIN_MSG_MAP(CAgentWnd)
	END_MSG_MAP()

protected:
	HRESULT InternalRenderFrame (HDC pDC, const POINT* pPosition);
	HRESULT InternalRenderAnimationFrame (CAgentFile* pAgentFile, LPCTSTR pAnimationName, long pFrameNum, HDC pDC, const POINT* pPosition);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DACORE_EXPORT
OBJECT_ENTRY_AUTO(__uuidof(CAgentPreviewWnd), CAgentPreviewWnd)
#endif

/////////////////////////////////////////////////////////////////////////////
