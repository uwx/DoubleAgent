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
#include "SapiVoiceEventSink.h"

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CSapiVoice : public CObject
{
protected:
	CSapiVoice ();
public:
	virtual ~CSapiVoice ();
	DECLARE_DYNAMIC (CSapiVoice)

// Attributes
	UINT SafeIsValid () const;
	bool SafeIsPrepared () const;
	bool SafeIsSpeaking () const;

	virtual tBstrPtr GetUniqueId ();
	virtual tBstrPtr GetDisplayName ();
	virtual ULONG GetRate ();
	virtual USHORT GetVolume ();
	virtual USHORT GetPitch ();

// Operations
	virtual HRESULT PrepareToSpeak (bool pHighPriority = false) = 0;
	virtual HRESULT Speak (LPCTSTR pMessage, bool pAsync = true) = 0;
	virtual HRESULT Stop () = 0;

	virtual HRESULT GetUniqueId (tBstrPtr & pUniqueId);
	virtual HRESULT GetDisplayName (tBstrPtr & pDisplayName);

	virtual HRESULT GetRate (ULONG & pRate);
	virtual HRESULT SetRate (ULONG pRate);

	virtual HRESULT GetVolume (USHORT & pVolume);
	virtual HRESULT SetVolume (USHORT pVolume);

	virtual HRESULT GetPitch (USHORT & pPitch);
	virtual HRESULT SetPitch (USHORT pPitch);

	void AddEventSink (ISapiVoiceEventSink * pEventSink);
	void RemoveEventSink (ISapiVoiceEventSink * pEventSink);
	void ClearEventSinks ();

	long GetEventCharID () const;
	bool SetEventCharID (long pEventCharID);

// Implementation
protected:
	virtual UINT _IsValid () const = 0;
	virtual bool _IsPrepared () const = 0;
	virtual bool _IsSpeaking () const = 0;

protected:
	long								mEventCharID;
	CPtrTypeArray <ISapiVoiceEventSink>	mEventSinks;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // _SAPIVOICE_H_INCLUDED
