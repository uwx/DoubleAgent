/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009 Cinnamon Software Inc.
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
#ifndef _SAPIVOICE_H_INCLUDED
#define _SAPIVOICE_H_INCLUDED
#pragma once

#include "DaCoreExp.h"
#include "NotifyObjects.h"

//////////////////////////////////////////////////////////////////////

interface _ISapiVoiceEventSink
{
public:
	virtual void OnVoiceStart (long pCharID) {}
	virtual void OnVoiceEnd (long pCharID) {}
	virtual void OnVoiceBookMark (long pCharID, long pBookMarkId) {}
	virtual void OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength) {}
	virtual void OnVoiceVisual (long pCharID, int pMouthOverlay) {}
};

//////////////////////////////////////////////////////////////////////

class CSapiVoice : public CNotifySinksOwner2 <_ISapiVoiceEventSink, CSapiVoice>
{
protected:
	CSapiVoice ();
public:
	_DACORE_IMPEXP virtual ~CSapiVoice ();

// Attributes
	_DACORE_IMPEXP UINT SafeIsValid () const;
	_DACORE_IMPEXP bool SafeIsPrepared () const;
	_DACORE_IMPEXP bool SafeIsSpeaking () const;
	_DACORE_IMPEXP bool SafeIsPaused () const;

	_DACORE_IMPEXP virtual tBstrPtr GetUniqueId ();
	_DACORE_IMPEXP virtual tBstrPtr GetDisplayName ();
	_DACORE_IMPEXP virtual ULONG GetRate ();
	_DACORE_IMPEXP virtual USHORT GetVolume ();
	_DACORE_IMPEXP virtual USHORT GetPitch ();

// Operations
	_DACORE_IMPEXP virtual HRESULT PrepareToSpeak (bool pHighPriority = false) = 0;
	_DACORE_IMPEXP virtual HRESULT Speak (LPCTSTR pMessage, bool pAsync = true) = 0;
	_DACORE_IMPEXP virtual HRESULT Stop () = 0;
	_DACORE_IMPEXP virtual HRESULT Pause () = 0;
	_DACORE_IMPEXP virtual HRESULT Resume () = 0;

	_DACORE_IMPEXP virtual HRESULT GetUniqueId (tBstrPtr & pUniqueId);
	_DACORE_IMPEXP virtual HRESULT GetDisplayName (tBstrPtr & pDisplayName);

	_DACORE_IMPEXP virtual HRESULT GetRate (ULONG & pRate);
	_DACORE_IMPEXP virtual HRESULT SetRate (ULONG pRate);

	_DACORE_IMPEXP virtual HRESULT GetVolume (USHORT & pVolume);
	_DACORE_IMPEXP virtual HRESULT SetVolume (USHORT pVolume);

	_DACORE_IMPEXP virtual HRESULT GetPitch (USHORT & pPitch);
	_DACORE_IMPEXP virtual HRESULT SetPitch (USHORT pPitch);

	_DACORE_IMPEXP long GetEventCharID () const;
	_DACORE_IMPEXP bool SetEventCharID (long pEventCharID);

// Implementation
protected:
	virtual UINT _IsValid () const = 0;
	virtual bool _IsPrepared () const = 0;
	virtual bool _IsSpeaking () const = 0;
	virtual bool _IsPaused () const = 0;

protected:
	long	mEventCharID;
};

//////////////////////////////////////////////////////////////////////

#endif // _SAPIVOICE_H_INCLUDED
