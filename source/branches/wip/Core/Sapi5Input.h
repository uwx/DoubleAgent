/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaCoreExp.h"
#include "NotifyObjects.h"
#include <sapi.h>

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ISpRecognizer, __uuidof(ISpRecognizer));
_COM_SMARTPTR_TYPEDEF (ISpRecoContext, __uuidof(ISpRecoContext));
_COM_SMARTPTR_TYPEDEF (ISpRecoGrammar, __uuidof(ISpRecoGrammar));
_COM_SMARTPTR_TYPEDEF (ISpRecoResult, __uuidof(ISpRecoResult));
_COM_SMARTPTR_TYPEDEF (ISpPhrase, __uuidof(ISpPhrase));
_COM_SMARTPTR_TYPEDEF (ISpPhraseAlt, __uuidof(ISpPhraseAlt));

//////////////////////////////////////////////////////////////////////

interface _ISapi5InputEventSink
{
public:
	virtual void OnSapi5InputEvent (const class CSpEvent & pEvent) = 0;
};

//////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
/////////////////////////////////////////////////////////////////////////////

class CSapi5Input
{
	DECLARE_DLL_OBJECT_EX(CSapi5Input, _DACORE_IMPEXP)
protected:
	CSapi5Input ();
public:
	_DACORE_IMPEXP virtual ~CSapi5Input ();
	_DACORE_IMPEXP static CSapi5Input * CreateInstance ();

// Attributes
	_DACORE_IMPEXP ISpRecognizer * SafeGetRecognizer () const;
	_DACORE_IMPEXP bool SafeIsPrepared () const;

	_DACORE_IMPEXP static tBstrPtr ShortEngineId (LPCTSTR pLongEngineId);
	_DACORE_IMPEXP static tBstrPtr LongEngineId (LPCTSTR pShortEngineId);

// Operations
	_DACORE_IMPEXP HRESULT PrepareToListen (bool pAllowAudioFormatChange = true);

	_DACORE_IMPEXP tBstrPtr GetEngineId ();
	_DACORE_IMPEXP HRESULT GetEngineId (CAtlString & pEngineId);
	_DACORE_IMPEXP HRESULT SetEngineId (LPCTSTR pEngineId);

	_DACORE_IMPEXP tBstrPtr GetEngineName ();
	_DACORE_IMPEXP HRESULT GetEngineName (CAtlString & pEngineName);
	_DACORE_IMPEXP HRESULT GetEngineLanguages (CAtlTypeArray <LANGID> & pLanguages);

	_DACORE_IMPEXP tBstrPtr GetInputId ();
	_DACORE_IMPEXP HRESULT GetInputId (CAtlString & pInputId);
	_DACORE_IMPEXP tBstrPtr GetInputName ();
	_DACORE_IMPEXP HRESULT GetInputName (CAtlString & pInputName);

	_DACORE_IMPEXP void LogStatus (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;

// Implementation
protected:
	ISpRecognizerPtr	mRecognizer;
};

//////////////////////////////////////////////////////////////////////

class CSapi5InputContext : public CNotifySinksOwner2 <_ISapi5InputEventSink, CSapi5InputContext>
{
	DECLARE_DLL_OBJECT_EX(CSapi5InputContext, _DACORE_IMPEXP)
protected:
	CSapi5InputContext ();
public:
	_DACORE_IMPEXP virtual ~CSapi5InputContext ();
	_DACORE_IMPEXP static CSapi5InputContext * CreateInstance ();

// Attributes
	_DACORE_IMPEXP bool SafeIsValid () const;
	_DACORE_IMPEXP bool IsInitialized () const;
	_DACORE_IMPEXP bool IsListening () const;
	_DACORE_IMPEXP bool IsHearing () const;
	_DACORE_IMPEXP bool IsPaused () const;

	const ULONGLONG	mGrammarIdCommands;
	const ULONGLONG	mGrammarIdGlobal;

// Operations
	_DACORE_IMPEXP HRESULT Initialize (CSapi5Input * pInput, LANGID pLangID);
	_DACORE_IMPEXP LANGID GetLangID ();

	_DACORE_IMPEXP HRESULT SetTheseCommands (long pCharID, LPCTSTR pCaption, const CAtlTypeArray <long> & pIds, const CAtlStringArray & pNames, const CAtlStringArray & pCommands);
	_DACORE_IMPEXP HRESULT SetGlobalCommands (USHORT pShowWndCmdId = 0, USHORT pHideWndCmdId = 0, USHORT pHideCharCmdId = 0);
	_DACORE_IMPEXP bool RemoveGlobalCommands ();

	_DACORE_IMPEXP bool SetCharacterClient (long pCharID, long pActiveCharID);
	_DACORE_IMPEXP bool SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption = NULL);
	_DACORE_IMPEXP bool RemoveCharacter (long pCharID);

	_DACORE_IMPEXP HRESULT StartListening (ULONG pMaxCommandAlternates = 2);
	_DACORE_IMPEXP HRESULT StopListening ();
	_DACORE_IMPEXP HRESULT PauseListening (bool pPause);

	_DACORE_IMPEXP void FromPrevInputContext (CSapi5InputContext * pPrevInputContext);

	_DACORE_IMPEXP void LogStatus (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP friend void LogRecoResult (UINT pLogLevel, ISpRecoResult * pResult, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP friend void LogRecoPhrase (UINT pLogLevel, ISpPhrase * pPhrase, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP friend void LogRecoPhraseAlt (UINT pLogLevel, ISpPhraseAlt * pPhrase, LPCTSTR pFormat = NULL, ...);

	_DACORE_IMPEXP friend tBstrPtr AudioStateStr (SPAUDIOSTATE pAudioState);
	_DACORE_IMPEXP friend tBstrPtr RecoStateStr (SPRECOSTATE pRecoState);
	_DACORE_IMPEXP friend tBstrPtr GrammarStateStr (SPGRAMMARSTATE pGrammarState);
	_DACORE_IMPEXP friend tBstrPtr ConfidenceStr (signed char pConfidence);
	_DACORE_IMPEXP friend tBstrPtr InterferenceStr (SPINTERFERENCE pInterference);

// Implementation
protected:
	HRESULT MakeCharacterCommands ();
	SPSTATEHANDLE MakeSpeechRule (ISpGrammarBuilder * pGrammar, DWORD pRuleId, LPCTSTR pRuleName, LPCTSTR pSpeech);
	INT_PTR PutSpeechPhrases (ISpGrammarBuilder * pGrammar, SPSTATEHANDLE pInitialState, SPSTATEHANDLE & pFinalState, const CAtlString & pSpeech, INT_PTR pBegNdx = 0, INT_PTR pEndNdx = INT_MAX, TCHAR pEndAt = 0);

private:
	static void __stdcall InputNotifyCallback(WPARAM wParam, LPARAM lParam);

protected:
	CSapi5Input *																		mInput;
	LANGID																				mLangID;
	ISpRecoContextPtr																	mRecoContext;
	ISpRecoGrammarPtr																	mRecoGrammarCommands;
	ISpRecoGrammarPtr																	mRecoGrammarGlobal;
	CAtlMap <long, CZeroInit<long> >													mCharClients;
	CAtlMap <long, CAtlString, CElementTraits<long>, CStringElementTraits<CAtlString> >	mCharNames;
	CAtlMap <long, CAtlString, CElementTraits<long>, CStringElementTraits<CAtlString> >	mCharCommands;
	ULONG																				mEventLastStream;
	bool																				mEventSoundStarted;
	bool																				mPaused;
};

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////
