/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include "DaCmnFormattedText.h"
#include "AgentTextParse.h"
#include "SapiVoice.h"
#include "DebugStr.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnFormattedText::CDaCmnFormattedText ()
:	mText (NULL)
{
}

CDaCmnFormattedText::~CDaCmnFormattedText ()
{
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnFormattedText::Initialize (LPUNKNOWN pObject, CSapiVoice* pSapiVoice)
{
	return Initialize (pObject, pSapiVoice->SafeIsValid ());
}

HRESULT CDaCmnFormattedText::Initialize (LPUNKNOWN pObject, UINT pSapiVersion)
{
	mText.m_pOuterUnknown = pObject;
	mRawText.Empty ();
	mText.mText.SetSapiVersion (pSapiVersion);
	mText.mText.ResetState (true);
	return (pObject) ? S_OK : E_FAIL;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_RawText (BSTR *RawText)
{
	HRESULT	lResult = E_POINTER;

	if	(RawText)
	{
		*RawText = mRawText.AllocSysString();
		lResult = S_OK;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::put_RawText (BSTR RawText)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(mText.mText.GetWordDisplayed () < 0)
	{
		CAgentTextParse	lTextParse;
		CAtlStringArray	lTextWords;

		mRawText = RawText;
		lTextParse.SetSapiVersion (mText.mText.GetSapiVersion());
		lTextParse.SplitText (mRawText, lTextWords);
		mText.mText = lTextWords;
		mText.mText.ResetState (true);
		lResult = S_OK;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_ParsedText(BSTR *ParsedText)
{
	HRESULT	lResult = E_POINTER;

	if	(ParsedText)
	{
		*ParsedText = mText.mText.GetFullText().AllocSysString();
		lResult = S_OK;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_SpeechText(BSTR *SpeechText)
{
	HRESULT	lResult = E_POINTER;

	if	(SpeechText)
	{
		*SpeechText = mText.mText.GetSpeechText().AllocSysString();
		lResult = S_OK;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_WordCount (long *WordCount)
{
	HRESULT	lResult = E_POINTER;

	if	(WordCount)
	{
		*WordCount = (long)mText.mText.GetWordCount();
		lResult = S_OK;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_DisplayWord (long WordIndex, BSTR *DisplayWord)
{
	HRESULT	lResult = E_POINTER;

	if	(DisplayWord)
	{
		*DisplayWord = mText.mText.GetDisplayWord (WordIndex).AllocSysString ();
		lResult = S_OK;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_WordIndex (long *WordIndex)
{
	HRESULT	lResult = E_POINTER;

	if	(WordIndex)
	{
		*WordIndex = (long)mText.mText.GetWordDisplayed ();
		lResult = S_OK;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::get_DisplayText (BSTR *DisplayText)
{
	HRESULT	lResult = E_POINTER;

	if	(DisplayText)
	{
		*DisplayText = mText.mText.GetDisplayText ().AllocSysString ();
		lResult = S_OK;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::Parse (BSTR RawText)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(mText.mText.GetWordDisplayed () < 0)
	{
		CAgentTextParse	lTextParse;

		mRawText = RawText;
		lTextParse.SetSapiVersion (mText.mText.GetSapiVersion());
		lTextParse = mRawText;
		mText.mText = lTextParse;
		mText.mText.ResetState (true);

		lResult = S_OK;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCmnFormattedText::Append (CDaCmnFormattedText* FormattedText)
{
	HRESULT	lResult = E_INVALIDARG;

	if	(FormattedText)
	{
		if	(mText.mText.GetWordDisplayed () >= 0)
		{
			lResult = E_UNEXPECTED;
		}
		else
		{
			mText.mText.Append (FormattedText->mText.mText, true);
			mText.mText.ResetState (true);
			mRawText = mText.mText.GetSpeechText ();
			if	(mRawText.IsEmpty ())
			{
				mRawText = mText.mText.GetFullText ();
			}
			lResult = S_OK;
		}
	}
	return lResult;
}