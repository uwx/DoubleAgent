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
#include "DaServerOdl.h"
#include "AgentTextObject.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnFormattedText
{
public:
	CDaCmnFormattedText ();
	virtual ~CDaCmnFormattedText ();

// Attributes
public:
	CComContainedObject <CAgentTextObject>	mText;

	const CAtlString & GetOriginal () const {return mRawText;}

// Operations
public:
	HRESULT Initialize (LPUNKNOWN pObject, class CSapiVoice * pSapiVoice);
	HRESULT Initialize (LPUNKNOWN pObject, UINT pSapiVersion = 5);

// Interfaces
public:
	HRESULT STDMETHODCALLTYPE get_RawText (BSTR *RawText);
	HRESULT STDMETHODCALLTYPE put_RawText (BSTR RawText);
	HRESULT STDMETHODCALLTYPE get_ParsedText(BSTR *ParsedText);
	HRESULT STDMETHODCALLTYPE get_SpeechText(BSTR *SpeechText);
	HRESULT STDMETHODCALLTYPE get_WordCount (long *WordCount);
	HRESULT STDMETHODCALLTYPE get_DisplayWord (long WordIndex, BSTR *DisplayWord);
	HRESULT STDMETHODCALLTYPE get_WordIndex (long *WordIndex);
	HRESULT STDMETHODCALLTYPE get_DisplayText (BSTR *DisplayText);
	HRESULT STDMETHODCALLTYPE Parse (BSTR RawText);
	HRESULT STDMETHODCALLTYPE Append (CDaCmnFormattedText * FormattedText);

// Implementation
protected:
	CAtlString	mRawText;
};

////////////////////////////////////////////////////////////////////////////
