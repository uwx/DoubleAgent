/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "DaCmnCommand.h"
#include "EventNotify.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnCommands
{
public:
	CDaCmnCommands ();
	virtual ~CDaCmnCommands ();

// Attributes
public:
	bool							mVisible;
	CString							mCaption;
	CString							mVoiceGrammar;
	CString							mVoiceCaption;
	CString							mFontName;
	long							mFontSize;
	bool							mGlobalVoiceCommandsEnabled;
	CAtlOwnPtrArray <CDaCmnCommand>	mCommands;
	USHORT							mDefaultId;
	const USHORT					mShowCharacterCmdId;
	const USHORT					mHideCharacterCmdId;
	const USHORT					mShowCommandsCmdId;
	const USHORT					mHideCommandsCmdId;

// Operations
public:
	void Initialize (long pCharID, CEventNotify * pNotify);

	bool SetLangID (LANGID pLangId);
	long DoContextMenu (HWND pOwner, const CPoint & pPosition, class CVoiceCommandsWnd * pVoiceCommandsWnd);

	CDaCmnCommand * GetCommand (USHORT pCommandId);
	CDaCmnCommand * GetDefaultCommand ();

	CString GetVoiceCommandsCaption () const;
	bool ShowVoiceCommands (class CVoiceCommandsWnd * pVoiceCommandsWnd);
	bool SetupVoiceContext (class CSapi5InputContext * pInputContext);

// Overrides

// Declarations
public:

// Interfaces
public:
	// ICDaCommands2
	HRESULT Add (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long *CommandID);
	HRESULT Insert (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long RefCommandID, long Before, long *CommandID);
	HRESULT Remove (long CommandID);
	HRESULT RemoveAll (void);

	HRESULT get_Count (long *Count);
	HRESULT get_Caption (BSTR *Caption);
	HRESULT put_Caption (BSTR Caption);
	HRESULT get_VoiceGrammar (BSTR *VoiceGrammar);
	HRESULT put_VoiceGrammar (BSTR VoiceGrammar);
	HRESULT get_Visible (VARIANT_BOOL *Visible);
	HRESULT put_Visible (VARIANT_BOOL Visible);
	HRESULT get_DefaultCommand (long *CommandID);
	HRESULT put_DefaultCommand (long CommandID);
	HRESULT get_FontName (BSTR *FontName);
	HRESULT put_FontName (BSTR FontName);
	HRESULT get_FontSize (long *FontSize);
	HRESULT put_FontSize (long FontSize);
	HRESULT get_VoiceCaption (BSTR *VoiceCaption);
	HRESULT put_VoiceCaption (BSTR VoiceCaption);
	HRESULT get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enabled);
	HRESULT put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled);

// Implementation
protected:
	INT_PTR FindCommand (USHORT pCommandId);
	virtual CDaCmnCommand * NewCommand (LPCTSTR pCaption, LPCTSTR pVoice = NULL, LPCTSTR pVoiceCaption = NULL, bool pEnabled = true, bool pVisible = true);
	bool RemoveCommand (INT_PTR pCommandNdx);

protected:
	long							mCharID;
	LANGID							mLangID;
	CEventNotify *					mNotify;
	CAtlOwnPtrArray <CDaCmnCommand>	mRemoved;
	USHORT							mNextCommandId;
};

/////////////////////////////////////////////////////////////////////////////
