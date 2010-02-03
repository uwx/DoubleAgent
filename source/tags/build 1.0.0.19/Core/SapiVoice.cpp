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
#include "StdAfx.h"
#include "SapiVoice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CSapiVoice, CObject)

CSapiVoice::CSapiVoice ()
:	mEventCharID (0)
{
}

CSapiVoice::~CSapiVoice ()
{
}

//////////////////////////////////////////////////////////////////////

UINT CSapiVoice::SafeIsValid () const
{
	UINT	lRet = 0;

	if	(this)
	{
		try
		{
			lRet = _IsValid ();
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

bool CSapiVoice::IsPrepared () const
{
	bool	lRet = false;

	if	(this)
	{
		try
		{
			lRet = _IsPrepared ();
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

bool CSapiVoice::SafeIsSpeaking () const
{
	bool	lRet = false;

	if	(this)
	{
		try
		{
			lRet = _IsSpeaking ();
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

tBstrPtr CSapiVoice::GetDisplayName ()
{
	tBstrPtr lDisplayName;
	GetDisplayName (lDisplayName);
	return lDisplayName.Detach();
}

HRESULT CSapiVoice::GetDisplayName (tBstrPtr & pDisplayName)
{
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////

tBstrPtr CSapiVoice::GetUniqueId ()
{
	tBstrPtr lUniqueId;
	GetUniqueId (lUniqueId);
	return lUniqueId.Detach();
}

HRESULT CSapiVoice::GetUniqueId (tBstrPtr & pUniqueId)
{
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////

ULONG CSapiVoice::GetRate ()
{
	ULONG lRate = 0;
	GetRate (lRate);
	return lRate;
}

HRESULT CSapiVoice::GetRate (ULONG & pRate)
{
	return E_NOTIMPL;
}

HRESULT CSapiVoice::SetRate (ULONG pRate)
{
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////

USHORT CSapiVoice::GetVolume ()
{
	USHORT lVolume = 0;
	GetVolume (lVolume);
	return lVolume;
}

HRESULT CSapiVoice::GetVolume (USHORT & pVolume)
{
	return E_NOTIMPL;
}

HRESULT CSapiVoice::SetVolume (USHORT pVolume)
{
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////

USHORT CSapiVoice::GetPitch ()
{
	USHORT lPitch = 0;
	GetPitch (lPitch);
	return lPitch;
}

HRESULT CSapiVoice::GetPitch (USHORT & pPitch)
{
	return E_NOTIMPL;
}

HRESULT CSapiVoice::SetPitch (USHORT pPitch)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CSapiVoice::AddEventSink (ISapiVoiceEventSink * pEventSink)
{
	if	(pEventSink)
	{
		mEventSinks.AddUnique (pEventSink);
	}
}

void CSapiVoice::RemoveEventSink (ISapiVoiceEventSink * pEventSink)
{
	if	(pEventSink)
	{
		mEventSinks.Remove (pEventSink);
	}
}

void CSapiVoice::ClearEventSinks ()
{
	if	(this)
	{
		mEventSinks.RemoveAll ();
	}
}

/////////////////////////////////////////////////////////////////////////////

long CSapiVoice::GetEventCharID () const
{
	return mEventCharID;
}

bool CSapiVoice::SetEventCharID (long pEventCharID)
{
	if	(mEventCharID != pEventCharID)
	{
		mEventCharID = pEventCharID;
		return true;
	}
	return false;
}
