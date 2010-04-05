/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP ATL_NO_VTABLE __declspec(uuid("{1147E561-A208-11DE-ABF2-002421116FB2}")) CAgentPreviewWnd :
	public CAgentWnd,
	public IDaPreview,
	public CComCoClass<CAgentPreviewWnd, &__uuidof(CAgentPreviewWnd)>
{
	DECLARE_DLL_OBJECT(CAgentPreviewWnd)
protected:
	CAgentPreviewWnd ();
public:
	virtual ~CAgentPreviewWnd ();
	static CAgentPreviewWnd * CreateInstance ();

// Attributes
public:

// Operations
public:
	bool Create (HWND pParentWnd, CRect * pInitialRect = NULL);
	void FinalRelease();

// Overrides
protected:
	virtual bool DoAnimationQueue ();
	virtual bool DoIdle ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_AGENTPREVIEWWND)
	DECLARE_NOT_AGGREGATABLE(CAgentPreviewWnd)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CAgentPreviewWnd)
		COM_INTERFACE_ENTRY(IDaPreview)
		COM_INTERFACE_ENTRY_CHAIN(CAgentWnd)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlAudioOutput)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	END_CATEGORY_MAP()

// Interfaces
public:
	// IDaPreview
    HRESULT STDMETHODCALLTYPE PlayAnimation (BSTR pAnimationName);
    HRESULT STDMETHODCALLTYPE PlayState (BSTR pStateName);
    HRESULT STDMETHODCALLTYPE GetIsPlaying (boolean *pIsPlaying);
    HRESULT STDMETHODCALLTYPE StopPlaying ();
    HRESULT STDMETHODCALLTYPE GetSoundsEnabled (boolean *pEnabled);
    HRESULT STDMETHODCALLTYPE EnableSounds (boolean pEnabled);
    HRESULT STDMETHODCALLTYPE GetIdleEnabled (boolean *pEnabled);
    HRESULT STDMETHODCALLTYPE EnableIdle (boolean pEnabled);
	HRESULT STDMETHODCALLTYPE SetBkColor (COLORREF pBkColor);
	HRESULT STDMETHODCALLTYPE GetFrameSize (SIZE * pFrameSize);
	HRESULT STDMETHODCALLTYPE GetCharacterFrameSize (BSTR pCharacterPath, SIZE * pFrameSize);
    HRESULT STDMETHODCALLTYPE GetFrameFormat (BYTE **pFrameFormat);
    HRESULT STDMETHODCALLTYPE GetCharacterFrameFormat (BSTR pCharacterPath, BYTE **pFrameFormat);
	HRESULT STDMETHODCALLTYPE OnAppActive (boolean pActive);
	HRESULT STDMETHODCALLTYPE RenderFrame (HDC pDC, POINT *pPosition);
	HRESULT STDMETHODCALLTYPE RenderAnimationFrame (BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition);
    HRESULT STDMETHODCALLTYPE RenderCharacterFrame (BSTR pCharacterPath, BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition);
    HRESULT STDMETHODCALLTYPE OpenFile (BSTR pCharacterPath, HWND pParentWnd);
    HRESULT STDMETHODCALLTYPE GetCharacterPath (BSTR *pCharacterPath);
    HRESULT STDMETHODCALLTYPE GetCharacterName (BSTR *pCharacterName, USHORT pLangID = 0);
    HRESULT STDMETHODCALLTYPE GetCharacterDescription (BSTR *pCharacterDescription, USHORT pLangID = 0);
    HRESULT STDMETHODCALLTYPE GetCharacterGuid (GUID *pGuid);

// Implementation
protected:
	LRESULT OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentPreviewWnd)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		CHAIN_MSG_MAP(CAgentWnd)
	END_MSG_MAP()

protected:
	HRESULT InternalRenderFrame (HDC pDC, const POINT * pPosition);
	HRESULT InternalRenderAnimationFrame (CAgentFile * pAgentFile, LPCTSTR pAnimationName, long pFrameNum, HDC pDC, const POINT * pPosition);
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_AUTO(__uuidof(CAgentPreviewWnd), CAgentPreviewWnd)

/////////////////////////////////////////////////////////////////////////////
