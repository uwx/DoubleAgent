/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));

//////////////////////////////////////////////////////////////////////

class CSapi5Voice : public CSapiVoice
{
	DECLARE_DLL_OBJECT_EX(CSapi5Voice, _DACORE_IMPEXP)
protected:
	CSapi5Voice ();
public:
	_DACORE_IMPEXP virtual ~CSapi5Voice ();
	_DACORE_IMPEXP static CSapi5Voice* CreateInstance ();

// Attributes
	_DACORE_IMPEXP static tBstrPtr ShortVoiceId (LPCTSTR pLongVoiceId);
	_DACORE_IMPEXP static tBstrPtr LongVoiceId (LPCTSTR pShortVoiceId);

	_DACORE_IMPEXP static tBstrPtr ShortOutputId (LPCTSTR pLongOutputId);
	_DACORE_IMPEXP static tBstrPtr LongOutputId (LPCTSTR pShortOutputId);

// Operations
	_DACORE_IMPEXP HRESULT virtual PrepareToSpeak (bool pHighPriority = false);
	_DACORE_IMPEXP HRESULT Speak (LPCTSTR pMessage, bool pAsync = true);
	_DACORE_IMPEXP HRESULT Stop ();
	_DACORE_IMPEXP HRESULT Pause ();
	_DACORE_IMPEXP HRESULT Resume ();

	_DACORE_IMPEXP tBstrPtr GetVoiceId ();
	_DACORE_IMPEXP virtual HRESULT GetVoiceId (tBstrPtr& pVoiceId);
	_DACORE_IMPEXP HRESULT SetVoiceId (LPCTSTR pVoiceId);

	_DACORE_IMPEXP tBstrPtr GetOutputId ();
	_DACORE_IMPEXP HRESULT GetOutputId (tBstrPtr& pOutputId);
	_DACORE_IMPEXP HRESULT SetOutputId (LPCTSTR pOutputId);

	_DACORE_IMPEXP tBstrPtr GetVoiceName ();
	_DACORE_IMPEXP HRESULT GetVoiceName (tBstrPtr& pVoiceName);
	_DACORE_IMPEXP HRESULT GetVoiceLanguages (CAtlTypeArray <LANGID>& pLanguages);

	_DACORE_IMPEXP friend int VoiceVisemeOverlay (int pViseme);
	_DACORE_IMPEXP friend CAtlString VoiceVisemeStr (int pViseme);

// Overrides
protected:
	_DACORE_IMPEXP virtual UINT _IsValid () const;
	_DACORE_IMPEXP virtual bool _IsPrepared () const;
	_DACORE_IMPEXP virtual bool _IsSpeaking () const;
	_DACORE_IMPEXP virtual bool _IsPaused () const;
public:
	_DACORE_IMPEXP virtual tBstrPtr GetUniqueId ();
	_DACORE_IMPEXP virtual HRESULT GetUniqueId (tBstrPtr& pUniqueId);
	_DACORE_IMPEXP virtual tBstrPtr GetDisplayName ();
	_DACORE_IMPEXP virtual HRESULT GetDisplayName (tBstrPtr& pDisplayName);
	_DACORE_IMPEXP virtual ULONG GetRate ();
	_DACORE_IMPEXP virtual HRESULT GetRate (ULONG& pRate);
	_DACORE_IMPEXP virtual HRESULT SetRate (ULONG pRate);
	_DACORE_IMPEXP virtual USHORT GetVolume ();
	_DACORE_IMPEXP virtual HRESULT GetVolume (USHORT& pVolume);
	_DACORE_IMPEXP virtual HRESULT SetVolume (USHORT pVolume);

	_DACORE_IMPEXP virtual void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	_DACORE_IMPEXP static void LogToken (UINT pLogLevel, ISpObjectToken* pToken, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	_DACORE_IMPEXP static void LogDataKey (UINT pLogLevel, ISpDataKey* pDataKey, LPCTSTR pIndent = NULL);

// Implementation
public:
	__declspec(property(get=_get_VoiceToken)) ISpObjectTokenPtr VoiceToken;
	_DACORE_IMPEXP ISpObjectTokenPtr _get_VoiceToken () const;
private:
	static void __stdcall VoiceNotifyCallback(WPARAM wParam, LPARAM lParam);

protected:
	ISpVoicePtr	mVoice;
	ULONG		mVoiceStreamNum;
	SPEVENTENUM	mLastVoiceEvent;
	bool		mPrepared;
	bool		mPaused;
};

//////////////////////////////////////////////////////////////////////
