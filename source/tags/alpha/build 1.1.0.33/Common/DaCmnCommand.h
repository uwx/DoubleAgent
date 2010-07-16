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

/////////////////////////////////////////////////////////////////////////////

class CDaCmnCommand
{
public:
	CDaCmnCommand();
	virtual ~CDaCmnCommand();

// Attributes
public:
	USHORT	mCommandId;
	bool	mEnabled;
	bool	mVisible;
	CString	mCaption;
	CString	mVoiceGrammar;
	CString	mVoiceCaption;
	short	mConfidenceThreshold;
	CString	mConfidenceText;

// Operations
public:

// Interfaces
public:
	// ICDaCommand2
	HRESULT get_Caption (BSTR *Caption);
	HRESULT put_Caption (BSTR Caption);
	HRESULT get_Enabled (VARIANT_BOOL *Enabled);
	HRESULT put_Enabled (VARIANT_BOOL Enabled);
	HRESULT get_Visible (VARIANT_BOOL *Visible);
	HRESULT put_Visible (VARIANT_BOOL Visible);
	HRESULT get_CommandID (long *CommandID);
	HRESULT get_VoiceCaption (BSTR *VoiceCaption);
	HRESULT put_VoiceCaption (BSTR VoiceCaption);
	HRESULT get_VoiceGrammar (BSTR *VoiceGrammar);
	HRESULT put_VoiceGrammar (BSTR VoiceGrammar);
	HRESULT get_ConfidenceThreshold (long *ConfidenceThreshold);
	HRESULT put_ConfidenceThreshold (long ConfidenceThreshold);
	HRESULT get_ConfidenceText (BSTR *ConfidenceText);
	HRESULT put_ConfidenceText (BSTR ConfidenceText);
};

/////////////////////////////////////////////////////////////////////////////
