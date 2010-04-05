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
#ifndef DAAGENTCOMMANDWINDOW_H_INCLUDED_
#define DAAGENTCOMMANDWINDOW_H_INCLUDED_
#pragma once

#include "DaServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class CVoiceCommandsWnd;

class __declspec(uuid("{1147E510-A208-11DE-ABF2-002421116FB2}")) CDaAgentCommandWindow : public CCmdTarget, public CDaObjectLifetime
{
public:
	CDaAgentCommandWindow (CVoiceCommandsWnd * pVoiceCommandsWnd, LPCTSTR pClientMutexName = NULL);
	virtual ~CDaAgentCommandWindow ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentCommandWindow)
	DECLARE_OLETYPELIB(CDaAgentCommandWindow)

// Attributes
public:
	CVoiceCommandsWnd *	mVoiceCommandsWnd;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaAgentCommandWindow)
	public:
	virtual void OnFinalRelease();
	virtual void OnClientEnded ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentCommandWindow)
	afx_msg void DspGetVisible(long * Visible);
	afx_msg void DspSetVisible(long Visible);
	afx_msg void DspGetPosition(long * Left, long * Top);
	afx_msg void DspGetSize(long * Width, long * Height);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Window, IDaSvrCommandWindow)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
		HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
		HRESULT STDMETHODCALLTYPE GetPosition (long *plLeft, long *plTop);
		HRESULT STDMETHODCALLTYPE GetSize (long *plWidth, long *plHeight);
	END_INTERFACE_PART(Window)

	BEGIN_INTERFACE_PART(OleWindow, IOleWindow)
        HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
        HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);
	END_INTERFACE_PART(OleWindow)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTCOMMANDWINDOW_H_INCLUDED_
