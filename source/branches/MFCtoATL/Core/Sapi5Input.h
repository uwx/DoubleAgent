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
#include "DaCoreExp.h"
#include "Sapi5InputEventSink.h"
#include <sapi.h>

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ISpRecognizer, __uuidof(ISpRecognizer));
_COM_SMARTPTR_TYPEDEF (ISpRecoContext, __uuidof(ISpRecoContext));
_COM_SMARTPTR_TYPEDEF (ISpRecoGrammar, __uuidof(ISpRecoGrammar));
_COM_SMARTPTR_TYPEDEF (ISpRecoResult, __uuidof(ISpRecoResult));
_COM_SMARTPTR_TYPEDEF (ISpPhrase, __uuidof(ISpPhrase));
_COM_SMARTPTR_TYPEDEF (ISpPhraseAlt, __uuidof(ISpPhraseAlt));

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi5Input
{
	DECLARE_DLL_OBJECT(CSapi5Input)
protected:
	CSapi5Input ();
public:
	virtual ~CSapi5Input ();
	static CSapi5Input * CreateInstance ();

// Attributes
	ISpRecognizer * SafeGetRecognizer () const;
	bool SafeIsPrepared () const;

	static tBstrPtr ShortEngineId (LPCTSTR pLongEngineId);
	static tBstrPtr LongEngineId (LPCTSTR pShortEngineId);

// Operations
	HRESULT PrepareToListen (bool pAllowAudioFormatChange = true);

	tBstrPtr GetEngineId ();
	HRESULT GetEngineId (CAtlString & pEngineId);
	HRESULT SetEngineId (LPCTSTR pEngineId);

	tBstrPtr GetEngineName ();
	HRESULT GetEngineName (CAtlString & pEngineName);
	HRESULT GetEngineLanguages (CAtlTypeArray <LANGID> & pLanguages);

	tBstrPtr GetInputId ();
	HRESULT GetInputId (CAtlString & pInputId);
	tBstrPtr GetInputName ();
	HRESULT GetInputName (CAtlString & pInputName);

	void LogStatus (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;

// Implementation
protected:
	ISpRecognizerPtr	mRecognizer;
};

//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi5InputContext
{
	DECLARE_DLL_OBJECT(CSapi5InputContext)
protected:
	CSapi5InputContext ();
public:
	virtual ~CSapi5InputContext ();
	static CSapi5InputContext * CreateInstance ();

// Attributes
	bool SafeIsValid () const;
	bool IsInitialized () const;
	bool IsListening () const;
	bool IsHearing () const;

	const ULONGLONG	mGrammarIdCommands;
	const ULONGLONG	mGrammarIdGlobal;

// Operations
	HRESULT Initialize (CSapi5Input * pInput, LANGID pLangID);
	LANGID GetLangID ();

	HRESULT SetTheseCommands (long pCharID, LPCTSTR pCaption, const CAtlTypeArray <long> & pIds, const CAtlStringArray & pNames, const CAtlStringArray & pCommands);
	HRESULT SetGlobalCommands (USHORT pShowWndCmdId = 0, USHORT pHideWndCmdId = 0, USHORT pHideCharCmdId = 0);
	bool RemoveGlobalCommands ();

	bool SetCharacterClient (long pCharID, long pActiveCharID);
	bool SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption = NULL);
	bool RemoveCharacter (long pCharID);

	HRESULT StartListening ();
	HRESULT StopListening ();

	bool AddEventSink (ISapi5InputEventSink * pEventSink, CSapi5InputContext * pPrevSourceContext = NULL);
	bool RemoveEventSink (ISapi5InputEventSink * pEventSink);
	bool FindEventSink (ISapi5InputEventSink * pEventSink) const;
	void ClearEventSinks ();

	void LogStatus (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	friend void _DACORE_IMPEXP LogRecoResult (UINT pLogLevel, ISpRecoResult * pResult, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogRecoPhrase (UINT pLogLevel, ISpPhrase * pPhrase, LPCTSTR pFormat = NULL, ...);
	friend void _DACORE_IMPEXP LogRecoPhraseAlt (UINT pLogLevel, ISpPhraseAlt * pPhrase, LPCTSTR pFormat = NULL, ...);

	friend tBstrPtr _DACORE_IMPEXP AudioStateStr (SPAUDIOSTATE pAudioState);
	friend tBstrPtr _DACORE_IMPEXP RecoStateStr (SPRECOSTATE pRecoState);
	friend tBstrPtr _DACORE_IMPEXP GrammarStateStr (SPGRAMMARSTATE pGrammarState);
	friend tBstrPtr _DACORE_IMPEXP ConfidenceStr (signed char pConfidence);
	friend tBstrPtr _DACORE_IMPEXP InterferenceStr (SPINTERFERENCE pInterference);

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
	CAtlPtrTypeArray <ISapi5InputEventSink>												mEventSinks;
	ULONG																				mEventLastStream;
	bool																				mEventSoundStarted;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////
