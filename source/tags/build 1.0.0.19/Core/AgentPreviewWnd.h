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
#ifndef AGENTPREVIEWWND_H_INCLUDED_
#define AGENTPREVIEWWND_H_INCLUDED_
#pragma once

#include "AgentWnd.h"
#include "DaCoreOdl.h"
#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP __declspec(uuid("{1147E561-A208-11DE-ABF2-002421116FB2}")) CAgentPreviewWnd : public CAgentWnd
{
	DECLARE_DYNCREATE(CAgentPreviewWnd)
	DECLARE_OLECREATE_EX(CAgentPreviewWnd)
protected:
	CAgentPreviewWnd ();
public:
	virtual ~CAgentPreviewWnd ();

// Attributes
public:

// Operations
public:
	bool Create (HWND pParentWnd, CRect * pInitialRect = NULL);

// Overrides
	//{{AFX_VIRTUAL(CAgentPreviewWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual bool DoAnimationQueue ();
	virtual bool DoIdle ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAgentPreviewWnd)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg _MFC_NCHITTEST_RESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(OleWindow, IOleWindow)
        HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
        HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);
	END_INTERFACE_PART(OleWindow)

	BEGIN_INTERFACE_PART(DaPreview, IDaPreview)
        HRESULT STDMETHODCALLTYPE PlayAnimation (BSTR pAnimationName);
        HRESULT STDMETHODCALLTYPE PlayState (BSTR pStateName);
        HRESULT STDMETHODCALLTYPE GetIsPlaying (boolean *pIsPlaying);
        HRESULT STDMETHODCALLTYPE Stop ();
        HRESULT STDMETHODCALLTYPE GetSoundsEnabled (boolean *pEnabled);
        HRESULT STDMETHODCALLTYPE EnableSounds (boolean pEnabled);
        HRESULT STDMETHODCALLTYPE GetIdleEnabled (boolean *pEnabled);
        HRESULT STDMETHODCALLTYPE EnableIdle (boolean pEnabled);
		HRESULT STDMETHODCALLTYPE SetBkColor (COLORREF pBkColor);
		HRESULT STDMETHODCALLTYPE GetFrameSize (SIZE * pFrameSize);
		HRESULT STDMETHODCALLTYPE GetCharacterFrameSize (BSTR pCharacterPath, SIZE * pFrameSize);
        HRESULT STDMETHODCALLTYPE GetFrameFormat (BYTE **pFrameFormat);
        HRESULT STDMETHODCALLTYPE GetCharacterFrameFormat (BSTR pCharacterPath, BYTE **pFrameFormat);
		HRESULT STDMETHODCALLTYPE OnActivateApp (boolean pActivated);
		HRESULT STDMETHODCALLTYPE RenderFrame (HDC pDC, POINT *pPosition);
		HRESULT STDMETHODCALLTYPE RenderAnimationFrame (BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition);
        HRESULT STDMETHODCALLTYPE RenderCharacterFrame (BSTR pCharacterPath, BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition);
        HRESULT STDMETHODCALLTYPE Open (BSTR pCharacterPath, HWND pParentWnd);
        HRESULT STDMETHODCALLTYPE GetCharacterPath (BSTR *pCharacterPath);
        HRESULT STDMETHODCALLTYPE GetCharacterName (BSTR *pCharacterName, USHORT pLangID = 0);
        HRESULT STDMETHODCALLTYPE GetCharacterDescription (BSTR *pCharacterDescription, USHORT pLangID = 0);
        HRESULT STDMETHODCALLTYPE GetCharacterGuid (GUID *pGuid);
	END_INTERFACE_PART(DaPreview)

	DECLARE_INTERFACE_MAP()

protected:
	HRESULT RenderFrame (HDC pDC, const POINT * pPosition);
	HRESULT RenderAnimationFrame (CAgentFile * pAgentFile, LPCTSTR pAnimationName, long pFrameNum, HDC pDC, const POINT * pPosition);
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTPREVIEWWND_H_INCLUDED_
