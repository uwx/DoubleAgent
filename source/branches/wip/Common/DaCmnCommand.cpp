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
#include "StdAfx.h"
#include "DaCmnCommand.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnCommand::CDaCmnCommand()
:	mCommandId (0),
	mEnabled (true),
	mVisible (true),
	mConfidenceThreshold (-25)
{
}

CDaCmnCommand::~CDaCmnCommand()
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommand::get_CommandID (long *CommandID)
{
	HRESULT	lResult = S_OK;

	if	(!CommandID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CommandID) = (long)mCommandId;
	}
	return lResult;
}

HRESULT CDaCmnCommand::get_Caption (BSTR *Caption)
{
	HRESULT	lResult = S_OK;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = mCaption.AllocSysString();
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_Caption (BSTR Caption)
{
	HRESULT		lResult = S_OK;
	CAtlString	lCaption (Caption);

	if	(lCaption.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(lCaption == mCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mCaption = lCaption;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommand::get_Enabled (VARIANT_BOOL *Enabled)
{
	HRESULT	lResult = mEnabled ? S_OK : S_FALSE;

	if	(Enabled)
	{
		(*Enabled) = (mEnabled) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_Enabled (VARIANT_BOOL Enabled)
{
	HRESULT	lResult = S_OK;

	if	(mEnabled == (Enabled != VARIANT_FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mEnabled = (Enabled != VARIANT_FALSE);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommand::get_Visible (VARIANT_BOOL *Visible)
{
	HRESULT	lResult = mVisible ? S_OK : S_FALSE;

	if	(Visible)
	{
		(*Visible) = (mVisible) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_Visible (VARIANT_BOOL Visible)
{
	HRESULT	lResult = S_OK;

	if	(mVisible == (Visible != VARIANT_FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mVisible = (Visible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT CDaCmnCommand::get_VoiceCaption (BSTR *VoiceCaption)
{
	HRESULT	lResult = S_OK;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = mVoiceCaption.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_VoiceCaption (BSTR VoiceCaption)
{
	HRESULT		lResult = S_OK;
	CAtlString	lVoiceCaption (VoiceCaption);

	if	(mVoiceCaption == lVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceCaption = lVoiceCaption;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommand::get_VoiceGrammar (BSTR *VoiceGrammar)
{
	HRESULT	lResult = S_OK;

	if	(!VoiceGrammar)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceGrammar) = mVoiceGrammar.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_VoiceGrammar (BSTR VoiceGrammar)
{
	HRESULT		lResult = S_OK;
	CAtlString	lVoice (VoiceGrammar);

	if	(lVoice == mVoiceGrammar)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceGrammar = lVoice;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommand::get_ConfidenceThreshold (long *ConfidenceThreshold)
{
	HRESULT	lResult = S_OK;

	if	(!ConfidenceThreshold)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ConfidenceThreshold) = (long)mConfidenceThreshold;
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_ConfidenceThreshold (long ConfidenceThreshold)
{
	HRESULT	lResult = S_OK;

	if	(
			(ConfidenceThreshold < -100)
		||	(ConfidenceThreshold > 100)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(mConfidenceThreshold == (short)ConfidenceThreshold)
	{
		lResult = S_FALSE;
	}
	else
	{
		mConfidenceThreshold = (short)ConfidenceThreshold;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommand::get_ConfidenceText (BSTR *ConfidenceText)
{
	HRESULT	lResult = S_OK;

	if	(!ConfidenceText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ConfidenceText) = mConfidenceText.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCommand::put_ConfidenceText (BSTR ConfidenceText)
{
	HRESULT		lResult = S_OK;
	CAtlString	lConfidenceText (ConfidenceText);

	if	(mConfidenceText == lConfidenceText)
	{
		lResult = S_FALSE;
	}
	else
	{
		mConfidenceText = lConfidenceText;
	}
	return lResult;
}
