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
#ifndef DAAGENTCOMMANDS_H_INCLUDED_
#define DAAGENTCOMMANDS_H_INCLUDED_
#pragma once

#include "DaAgentCommand.h"
#include "DaAgentNotify.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E50F-A208-11DE-ABF2-002421116FB2}")) CDaAgentCommands : public CCmdTarget
{
public:
	CDaAgentCommands (long pCharID, IDaNotify & pNotify);
	virtual ~CDaAgentCommands ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentCommands)
	DECLARE_OLETYPELIB(CDaAgentCommands)

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
	COwnPtrArray <CDaAgentCommand>	mCommands;
	USHORT							mDefaultId;
	const USHORT					mShowCharacterCmdId;
	const USHORT					mHideCharacterCmdId;
	const USHORT					mShowCommandsCmdId;
	const USHORT					mHideCommandsCmdId;

	long GetCharID () const;
	LANGID GetLangID () const;

// Operations
public:
	bool SetLangID (LANGID pLangId);
	USHORT DoContextMenu (HWND pOwner, const CPoint & pPosition);

	CDaAgentCommand * GetCommand (USHORT pCommandId);
	CDaAgentCommand * GetDefaultCommand ();

	CString GetVoiceCommandsCaption () const;
	bool ShowVoiceCommands (CVoiceCommandsWnd * pVoiceCommandsWnd);
	bool SetupVoiceContext (class CSapi5InputContext * pInputContext);

// Overrides
	//{{AFX_VIRTUAL(CDaAgentCommands)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentCommands)
	afx_msg void DspGetCommand(long CommandID, LPUNKNOWN * Command);
	afx_msg void DspGetCount(long * Count);
	afx_msg void DspSetCaption(LPCTSTR Caption);
	afx_msg void DspGetCaption(BSTR * Caption);
	afx_msg void DspSetVoice(LPCTSTR Voice);
	afx_msg void DspGetVoice(BSTR * Voice);
	afx_msg void DspSetVisible(long Visible);
	afx_msg void DspGetVisible(long * Visible);
	afx_msg void DspAdd(LPCTSTR Caption, LPCTSTR Voice, long Enabled, long Visible, long * ID);
	afx_msg void DspInsert(LPCTSTR Caption, LPCTSTR Voice, long Enabled, long Visible, long RefID, long Before, long * ID);
	afx_msg void DspRemove(long ID);
	afx_msg void DspRemoveAll();
	afx_msg void DspSetDefaultID(long ID);
	afx_msg void DspGetDefaultID(long * ID);
	afx_msg void DspSetHelpContextID(long HelpID);
	afx_msg void DspGetHelpContextID(long * HelpID);
	afx_msg void DspSetFontName(LPCTSTR FontName);
	afx_msg void DspGetFontName(BSTR * FontName);
	afx_msg void DspSetFontSize(long FontSize);
	afx_msg void DspGetFontSize(long * FontSize);
	afx_msg void DspSetVoiceCaption(LPCTSTR VoiceCaption);
	afx_msg void DspGetVoiceCaption(BSTR * VoiceCaption);
	afx_msg void DspAddEx(LPCTSTR Caption, LPCTSTR Voice, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpID, long * ID);
	afx_msg void DspInsertEx(LPCTSTR Caption, LPCTSTR Voice, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpID, long RefID, long Before, long * ID);
	afx_msg void DspSetGlobalVoiceCommandsEnabled(long Enable);
	afx_msg void DspGetGlobalVoiceCommandsEnabled(long * Enabled);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Commands, IDaSvrCommands)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

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
	END_INTERFACE_PART(Commands)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
	int FindCommand (USHORT pCommandId);
	CDaAgentCommand * NewCommand (LPCTSTR pCaption, LPCTSTR pVoice = NULL, LPCTSTR pVoiceCaption = NULL, bool pEnabled = true, bool pVisible = true, ULONG pHelpContextId = 0);
	bool RemoveCommand (int pCommandNdx);

protected:
	long							mCharID;
	LANGID							mLangID;
	IDaNotify &						mNotify;
	COwnPtrArray <CDaAgentCommand>	mRemoved;
	USHORT							mNextCommandId;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTCOMMANDS_H_INCLUDED_
