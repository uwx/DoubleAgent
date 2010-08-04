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
#pragma once
#include "SapiVoice.h"
#include <sapi.h>

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ISpVoice, __uuidof(ISpVoice));

//////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi5Voice : public CSapiVoice
{
	DECLARE_DLL_OBJECT(CSapi5Voice)
protected:
	CSapi5Voice ();
public:
	virtual ~CSapi5Voice ();
	static CSapi5Voice * CreateInstance ();

// Attributes
	static tBstrPtr ShortVoiceId (LPCTSTR pLongVoiceId);
	static tBstrPtr LongVoiceId (LPCTSTR pShortVoiceId);

	static tBstrPtr ShortOutputId (LPCTSTR pLongOutputId);
	static tBstrPtr LongOutputId (LPCTSTR pShortOutputId);

// Operations
	HRESULT PrepareToSpeak (bool pHighPriority = false);
	HRESULT Speak (LPCTSTR pMessage, bool pAsync = true);
	HRESULT Stop ();
	HRESULT Pause ();
	HRESULT Resume ();

	tBstrPtr GetVoiceId ();
	HRESULT GetVoiceId (tBstrPtr & pVoiceId);
	HRESULT SetVoiceId (LPCTSTR pVoiceId);

	tBstrPtr GetOutputId ();
	HRESULT GetOutputId (tBstrPtr & pOutputId);
	HRESULT SetOutputId (LPCTSTR pOutputId);

	tBstrPtr GetVoiceName ();
	HRESULT GetVoiceName (tBstrPtr & pVoiceName);
	HRESULT GetVoiceLanguages (CAtlTypeArray <LANGID> & pLanguages);

	friend int VoiceVisemeOverlay (int pViseme);
	friend CAtlString VoiceVisemeStr (int pViseme);

// Overrides
protected:
	virtual UINT _IsValid () const;
	virtual bool _IsPrepared () const;
	virtual bool _IsSpeaking () const;
	virtual bool _IsPaused () const;
public:
	virtual tBstrPtr GetUniqueId ();
	virtual HRESULT GetUniqueId (tBstrPtr & pUniqueId);
	virtual tBstrPtr GetDisplayName ();
	virtual HRESULT GetDisplayName (tBstrPtr & pDisplayName);
	virtual ULONG GetRate ();
	virtual HRESULT GetRate (ULONG & pRate);
	virtual HRESULT SetRate (ULONG pRate);
	virtual USHORT GetVolume ();
	virtual HRESULT GetVolume (USHORT & pVolume);
	virtual HRESULT SetVolume (USHORT pVolume);

// Implementation
private:
	static void __stdcall VoiceNotifyCallback(WPARAM wParam, LPARAM lParam);

protected:
	ISpVoicePtr	mVoice;
	ULONG		mVoiceStreamNum;
	SPEVENTENUM	mLastVoiceEvent;
	bool		mPrepared;
	bool		mPaused;
};

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////
