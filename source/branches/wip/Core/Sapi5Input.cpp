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
#pragma warning(push)
#pragma warning(disable: 4005)
#include <sphelper.h>
#pragma warning(pop)
#include "DaCoreRes.h"
#include "Sapi5Input.h"
#include "Sapi5Err.h"
#include "MallocPtr.h"
#include "StringArrayEx.h"
#include "LocalizeEx.h"
#include "GuidStr.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_GRAMMAR		LogNormal
//#define	_DEBUG_STATUS		LogNormal
//#define	_DEBUG_EVENTS		LogNormal|LogHighVolume
//#define	_DEBUG_EVENTS_EX	LogNormal|LogHighVolume
#define	_LOG_GRAMMAR_ERRS		LogNormal
#endif

#ifndef	_LOG_GRAMMAR_ERRS
#define	_LOG_GRAMMAR_ERRS	LogDetails
#endif

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ISpGrammarBuilder, __uuidof(ISpGrammarBuilder));
_COM_SMARTPTR_TYPEDEF (ISpGramCompBackend, __uuidof(ISpGramCompBackend));
_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));
_COM_SMARTPTR_TYPEDEF (ISpObjectTokenCategory, __uuidof(ISpObjectTokenCategory));
_COM_SMARTPTR_TYPEDEF (ISpDataKey, __uuidof(ISpDataKey));

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi5Input)

CSapi5Input::CSapi5Input ()
{
	if	(SUCCEEDED (LogComErr (LogIfActive, CoCreateInstance (CLSID_SpInprocRecognizer, NULL, CLSCTX_SERVER, __uuidof (ISpRecognizer), (void **) &mRecognizer))))
	{
		mRecognizer->SetRecoState (SPRST_INACTIVE);
	}
}

CSapi5Input::~CSapi5Input ()
{
	SafeFreeSafePtr (mRecognizer);
}

CSapi5Input * CSapi5Input::CreateInstance ()
{
	return new CSapi5Input;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

ISpRecognizer * CSapi5Input::SafeGetRecognizer () const
{
	ISpRecognizer *	lRet = NULL;

	if	(this)
	{
		try
		{
			lRet = mRecognizer;
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

bool CSapi5Input::SafeIsPrepared () const
{
	bool				lRet = false;
	ISpObjectTokenPtr	lToken;

	if	(
			(mRecognizer != NULL)
		&&	(SUCCEEDED (mRecognizer->GetInputObjectToken (&lToken)))
		&&	(lToken != NULL)
		)
	{
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

HRESULT CSapi5Input::PrepareToListen (bool pAllowAudioFormatChange)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(mRecognizer != NULL)
	{
		ISpObjectTokenPtr	lToken;

		if	(SUCCEEDED (lResult = LogSapi5Err (LogNormal, SpGetDefaultTokenFromCategoryId (SPCAT_AUDIOIN, &lToken))))
		{
			lResult = LogSapi5Err (LogNormal, mRecognizer->SetInput (lToken, (pAllowAudioFormatChange!=false)));
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Input::GetEngineId ()
{
	CAtlString lEngineId;
	GetEngineId (lEngineId);
	return lEngineId.AllocSysString();
}

HRESULT CSapi5Input::GetEngineId (CAtlString & pEngineId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeGetRecognizer ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lTokenId;

		pEngineId.Empty ();

		if	(
				(SUCCEEDED (lResult = mRecognizer->GetRecognizer (&lToken)))
			&&	(SUCCEEDED (lResult = lToken->GetId (lTokenId.Free ())))
			)
		{
			pEngineId = lTokenId;
		}
	}
	return lResult;
}

HRESULT CSapi5Input::SetEngineId (LPCTSTR pEngineId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeGetRecognizer ())
	{
		CAtlString			lNewEngineId (pEngineId);
		ISpObjectTokenPtr	lOldToken;
		ISpObjectTokenPtr	lNewToken;
		tMallocPtr <WCHAR>	lOldTokenId;
		tMallocPtr <WCHAR>	lNewTokenId;

		if	(lNewEngineId.IsEmpty ())
		{
			SpGetDefaultTokenIdFromCategoryId (SPCAT_RECOGNIZERS, lNewTokenId.Free ());
		}
		else
		{
			lNewTokenId = AtlAllocTaskWideString (CSapi5Input::LongEngineId (lNewEngineId));
		}

		if	(
				((LPCWSTR) lNewTokenId != NULL)
			&&	(SUCCEEDED (lResult = mRecognizer->GetRecognizer (&lOldToken)))
			&&	(SUCCEEDED (lResult = lOldToken->GetId (lOldTokenId.Free ())))
			)
		{
			if	(wcscmp (lOldTokenId, lNewTokenId) == 0)
			{
				lResult = S_FALSE;
			}
			else
			if	(
					(SUCCEEDED (lResult = SpGetTokenFromId (lNewTokenId, &lNewToken, NULL)))
				&&	(SUCCEEDED (lResult = mRecognizer->SetRecognizer (lNewToken)))
				)
			{
				lResult = S_OK;
			}
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Input::GetEngineName ()
{
	CAtlString	lInputName;
	GetEngineName (lInputName);
	return lInputName.AllocSysString();
}

HRESULT CSapi5Input::GetEngineName (CAtlString & pEngineName)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeGetRecognizer ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lDescription;

		pEngineName.Empty ();

		if	(
				(SUCCEEDED (lResult = mRecognizer->GetRecognizer (&lToken)))
			&&	(SUCCEEDED (lResult = SpGetDescription (lToken, lDescription.Free (), NULL)))
			)
		{
			pEngineName = lDescription;
		}
	}
	return lResult;
}

HRESULT CSapi5Input::GetEngineLanguages (CAtlTypeArray <LANGID> & pLanguages)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeGetRecognizer ())
	{
		tS <SPRECOGNIZERSTATUS>	Status;
		ULONG					lLangNdx;

		pLanguages.RemoveAll ();

		if	(SUCCEEDED (lResult = mRecognizer->GetStatus (&Status)))
		{
			for	(lLangNdx = 0; lLangNdx < Status.cLangIDs; lLangNdx++)
			{
				pLanguages.Add (Status.aLangID [lLangNdx]);
			}
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Input::GetInputId ()
{
	CAtlString lInputId;
	GetInputId (lInputId);
	return lInputId.AllocSysString();
}

HRESULT CSapi5Input::GetInputId (CAtlString & pInputId)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeGetRecognizer ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lTokenId;

		pInputId.Empty ();

		if	(
				(SUCCEEDED (lResult = mRecognizer->GetInputObjectToken (&lToken)))
			&&	(SUCCEEDED (lResult = lToken->GetId (lTokenId.Free ())))
			)
		{
			pInputId = lTokenId;
		}
	}
	return lResult;
}

tBstrPtr CSapi5Input::GetInputName ()
{
	CAtlString	lInputName;
	GetInputName (lInputName);
	return lInputName.AllocSysString();
}

HRESULT CSapi5Input::GetInputName (CAtlString & pInputName)
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeGetRecognizer ())
	{
		ISpObjectTokenPtr	lToken;
		tMallocPtr <WCHAR>	lDescription;

		pInputName.Empty ();

		if	(
				(SUCCEEDED (lResult = mRecognizer->GetInputObjectToken (&lToken)))
			&&	(SUCCEEDED (lResult = SpGetDescription (lToken, lDescription.Free (), NULL)))
			)
		{
			pInputName = lDescription;
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

tBstrPtr CSapi5Input::ShortEngineId (LPCTSTR pLongEngineId)
{
	CAtlString	lEngineId (pLongEngineId);

	PathStripPath (lEngineId.GetBuffer (lEngineId.GetLength ()));
	lEngineId.ReleaseBuffer ();
	return lEngineId.AllocSysString();
}

tBstrPtr CSapi5Input::LongEngineId (LPCTSTR pShortEngineId)
{
	CAtlString	lEngineId;

	PathCombine (lEngineId.GetBuffer (MAX_PATH), SPCAT_RECOGNIZERS, _T("Tokens"));
	PathAppend (lEngineId.GetBuffer (MAX_PATH), pShortEngineId);
	lEngineId.ReleaseBuffer ();
	return lEngineId.AllocSysString();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi5InputContext)

CSapi5InputContext::CSapi5InputContext ()
:	mInput (NULL),
	mLangID (0),
	mGrammarIdCommands ((ULONGLONG)&mGrammarIdCommands),
	mGrammarIdGlobal ((ULONGLONG)&mGrammarIdGlobal),
	mEventLastStream (0),
	mEventSoundStarted (false)
{
}

CSapi5InputContext::~CSapi5InputContext ()
{
	if	(mRecoContext != NULL)
	{
		try
		{
			mRecoContext->SetNotifySink (NULL);
		}
		catch AnyExceptionSilent
	}
	SafeFreeSafePtr (mRecoContext);
	SafeFreeSafePtr (mRecoGrammarCommands);
	SafeFreeSafePtr (mRecoGrammarGlobal);
}

CSapi5InputContext * CSapi5InputContext::CreateInstance ()
{
	return new CSapi5InputContext;
}

//////////////////////////////////////////////////////////////////////

bool CSapi5InputContext::SafeIsValid () const
{
	bool	lRet = false;

	if	(this)
	{
		try
		{
			if	(
					(mRecoContext != NULL)
				&&	(mInput->SafeGetRecognizer ())
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

bool CSapi5InputContext::IsInitialized () const
{
	return (mInput != NULL);
}

//////////////////////////////////////////////////////////////////////

bool CSapi5InputContext::IsListening () const
{
	bool	lRet = false;

	try
	{
		ISpRecognizerPtr	lRecognizer;
		SPCONTEXTSTATE		lContextState = (SPCONTEXTSTATE)-1;
		SPRECOSTATE			lRecoState = (SPRECOSTATE)-1;

		if	(
				(mRecoContext != NULL)
			&&	(SUCCEEDED (mRecoContext->GetContextState (&lContextState)))
			&&	(lContextState == SPCS_ENABLED)
			&&	(SUCCEEDED (mRecoContext->GetRecognizer (&lRecognizer)))
			&&	(SUCCEEDED (lRecognizer->GetRecoState (&lRecoState)))
			&&	(
					(lRecoState == SPRST_ACTIVE)
				||	(lRecoState == SPRST_ACTIVE_ALWAYS)
				)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

bool CSapi5InputContext::IsHearing () const
{
	bool	lRet = false;

	if	(
			(IsListening ())
		&&	(mEventSoundStarted)
		)
	{
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CSapi5InputContext::Initialize (CSapi5Input * pInput, LANGID pLangID)
{
	HRESULT				lResult = E_FAIL;
	ISpRecognizerPtr	lRecognizer;

	SafeFreeSafePtr (mRecoContext);
	SafeFreeSafePtr (mRecoGrammarCommands);
	SafeFreeSafePtr (mRecoGrammarGlobal);

	if	(mInput = pInput)
	{
		mLangID = pLangID;
		lRecognizer = mInput->SafeGetRecognizer ();

		if	(lRecognizer != NULL)
		{
			lResult = LogSapi5Err (LogNormal, lRecognizer->CreateRecoContext (&mRecoContext));
		}
	}
	return lResult;
}

LANGID CSapi5InputContext::GetLangID ()
{
	LANGID	lRet = mLangID;

	if	(!lRet)
	{
		CAtlTypeArray <LANGID>	lLanguages;

		if	(
				(mInput->SafeGetRecognizer ())
			&&	(SUCCEEDED (mInput->GetEngineLanguages (lLanguages)))
			&&	(lLanguages.GetCount() > 0)
			)
		{
			lRet = lLanguages [0];
		}
	}
	if	(!lRet)
	{
		lRet = SpGetUserDefaultUILanguage();
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

HRESULT CSapi5InputContext::SetTheseCommands (long pCharID, LPCTSTR pCaption, const CAtlTypeArray <long> & pIds, const CAtlStringArray & pNames, const CAtlStringArray & pCommands)
{
	HRESULT	lResult = S_OK;
	INT_PTR	lNdx;

	if	(mRecoGrammarCommands == NULL)
	{
		lResult = S_FALSE;
	}
	SafeFreeSafePtr (mRecoGrammarCommands);

	if	(!SafeIsValid ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		mCharCommands.SetAt (pCharID, pCaption);

		if	(
				(pNames.GetCount() != pIds.GetCount())
			||	(pCommands.GetCount() != pIds.GetCount())
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(pIds.GetCount() > 0)
		{
			lResult = LogSapi5Err (LogNormal, mRecoContext->CreateGrammar (mGrammarIdCommands, &mRecoGrammarCommands));

			if	(mRecoGrammarCommands != NULL)
			{
#ifdef	_DEBUG_GRAMMAR
				LogMessage (_DEBUG_GRAMMAR, _T("Make commands grammar"));
#endif
				mRecoGrammarCommands->SetDictationState (SPRS_INACTIVE);
				mRecoGrammarCommands->SetGrammarState (SPGS_DISABLED);
				LogSapi5Err (LogNormal, mRecoGrammarCommands->ResetGrammar (GetLangID()));

				for	(lNdx = 0; lNdx < (INT_PTR)pIds.GetCount(); lNdx++)
				{
					MakeSpeechRule (mRecoGrammarCommands, pIds [lNdx], pNames [lNdx], pCommands [lNdx]);
				}
				lResult = LogSapi5Err (LogNormal, mRecoGrammarCommands->Commit (0));

#ifdef	_DEBUG_GRAMMAR
				LogMessage (_DEBUG_GRAMMAR, _T("Made commands grammar"));
#endif
			}
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CSapi5InputContext::SetGlobalCommands (USHORT pShowWndCmdId, USHORT pHideWndCmdId, USHORT pHideCharCmdId)
{
	HRESULT	lResult = S_OK;

	SafeFreeSafePtr (mRecoGrammarGlobal);

	if	(!SafeIsValid ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		lResult = LogSapi5Err (LogNormal, mRecoContext->CreateGrammar (mGrammarIdGlobal, &mRecoGrammarGlobal));

		if	(mRecoGrammarGlobal != NULL)
		{
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("Make global grammar"));
#endif
			mRecoGrammarGlobal->SetDictationState (SPRS_INACTIVE);
			mRecoGrammarGlobal->SetGrammarState (SPGS_DISABLED);
			LogSapi5Err (LogNormal, mRecoGrammarGlobal->ResetGrammar (GetLangID()));

			if	(pShowWndCmdId)
			{
				MakeSpeechRule (mRecoGrammarGlobal, pShowWndCmdId, CLocalize::LoadString (ID_COMMANDS_WINDOW_OPEN), CLocalize::LoadString (IDS_COMMANDS_WINDOW_OPEN_SPEECH));
			}
			if	(pHideWndCmdId)
			{
				MakeSpeechRule (mRecoGrammarGlobal, pHideWndCmdId, CLocalize::LoadString (ID_COMMANDS_WINDOW_CLOSE), CLocalize::LoadString (IDS_COMMANDS_WINDOW_CLOSE_SPEECH));
			}
			if	(pHideCharCmdId)
			{
				MakeSpeechRule (mRecoGrammarGlobal, pHideCharCmdId, CLocalize::LoadString (IDS_COMMANDS_HIDE_PROMPT), CLocalize::LoadString (IDS_COMMANDS_HIDE_SPEECH));
			}
			MakeCharacterCommands ();

			lResult = LogSapi5Err (LogNormal, mRecoGrammarGlobal->Commit (0));

#ifdef	_DEBUG_GRAMMAR_NOT
			try
			{
				IStreamPtr			lGrammarStream;
				HGLOBAL				lGrammarData;
				tMallocPtr <WCHAR>	lErrors;
				CAtlStringArray		lErrorLines;
				INT_PTR				lErrorNdx;

				if	(SUCCEEDED (CreateStreamOnHGlobal (NULL, TRUE, &lGrammarStream)))
				{
					LogSapi5ErrAnon (_DEBUG_GRAMMAR, mRecoGrammarGlobal->SaveCmd (lGrammarStream, lErrors.Free()), _T("SaveCmd"));
					if	(lErrors)
					{
						MakeStringArray (lErrors, lErrorLines, _T("\r\n"));
						for	(lErrorNdx = 0; lErrorNdx < lErrorLines.GetCount(); lErrorNdx++)
						{
							LogMessage (_DEBUG_GRAMMAR, _T("  Error [%s]"), DebugStr(lErrorLines[lErrorNdx]));
						}
					}
				}
			}
			catch AnyExceptionSilent
#endif
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("Made global grammar"));
#endif
		}
	}
	return lResult;
}

bool CSapi5InputContext::RemoveGlobalCommands ()
{
	bool	lRet = false;

	if	(mRecoGrammarGlobal != NULL)
	{
		lRet = true;
	}
	SafeFreeSafePtr (mRecoGrammarGlobal);

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CSapi5InputContext::SetCharacterClient (long pCharID, long pActiveCharID)
{
	bool	lRet = false;

	if	(
			(pCharID > 0)
		&&	(pActiveCharID > 0)
		&&	(mCharClients [pCharID] != pActiveCharID)
		)
	{
		mCharClients [pCharID] = pActiveCharID;
		lRet = true;
	}
	if	(
			(lRet)
		&&	(IsListening ())
		)
	{
		MakeCharacterCommands ();
	}
	return lRet;
}

bool CSapi5InputContext::SetCharacterName (long pCharID, LPCTSTR pCharName, LPCTSTR pCommandsCaption)
{
	bool		lRet = false;
	CAtlString	lCharName (pCharName);
	CAtlString	lCommandsCaption (pCommandsCaption);

	if	(pCharID)
	{
		if	(
				(!lCharName.IsEmpty ())
			&&	(mCharNames [pCharID] != lCharName)
			)
		{
			mCharNames [pCharID] = lCharName;
			lRet = true;
		}
		if	(
				(pCommandsCaption)
			&&	(mCharCommands [pCharID] != lCommandsCaption)
			)
		{
			mCharCommands [pCharID] = lCommandsCaption;
			lRet = true;
		}
		if	(
				(lRet)
			&&	(IsListening ())
			)
		{
			MakeCharacterCommands ();
		}
	}
	return lRet;
}

bool CSapi5InputContext::RemoveCharacter (long pCharID)
{
	bool	lRet = false;

	mCharClients.RemoveKey (pCharID);
	if	(mCharCommands.RemoveKey (pCharID))
	{
		lRet = true;
	}
	if	(mCharNames.RemoveKey (pCharID))
	{
		lRet = true;
	}
	if	(
			(lRet)
		&&	(IsListening ())
		)
	{
		MakeCharacterCommands ();
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

HRESULT CSapi5InputContext::MakeCharacterCommands ()
{
	HRESULT	lResult = S_FALSE;

	if	(
			(SafeIsValid ())
		&&	(mRecoGrammarGlobal != NULL)
		)
	{
		POSITION	lPos;
		long		lCharID;
		CAtlString	lCharName;
		CAtlString	lCommandsCaption;

		for	(lPos = mCharNames.GetStartPosition(); lPos;)
		{
			mCharNames.GetNextAssoc (lPos, (lCharID=0), lCharName);

			if	(lCharID > 0)
			{
				lCommandsCaption = mCharCommands [lCharID];

				if	(
						(mCharClients [lCharID] == lCharID)
					&&	(!lCharName.IsEmpty ())
					)
				{
					if	(MakeSpeechRule (mRecoGrammarGlobal, MAKELONG(lCharID,1), NULL, lCharName))
					{
						lResult = S_OK;
					}
				}
				else
				if	(
						(mCharClients [lCharID] != lCharID)
					&&	(!lCommandsCaption.IsEmpty ())
					)
				{
					if	(MakeSpeechRule (mRecoGrammarGlobal, MAKELONG(lCharID,1), NULL, lCommandsCaption))
					{
						lResult = S_OK;
					}
				}
				else
				{
					SPSTATEHANDLE	lRule = NULL;

#ifdef	_DEBUG_GRAMMAR
						LogMessage (_DEBUG_GRAMMAR, _T("  [%p] Rule [%hu %hu] clear"), lRule, 1, lCharID);
#endif
					if	(
							(SUCCEEDED (mRecoGrammarGlobal->GetRule (NULL, MAKELONG(lCharID,1), SPRAF_TopLevel, FALSE, &lRule)))
						&&	(SUCCEEDED (mRecoGrammarGlobal->ClearRule (lRule)))
						)
					{
#ifdef	_DEBUG_GRAMMAR
						LogMessage (_DEBUG_GRAMMAR, _T("  [%p] Rule [%hu %hu] cleared"), lRule, 1, lCharID);
#endif
					}
				}
			}
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CSapi5InputContext::StartListening ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(SafeIsValid())
	{
		HRESULT		lTempResult;
		ULONGLONG	lEventInterest = SPFEI_ALL_SR_EVENTS & ~((ULONGLONG)1 << SPEI_SR_AUDIO_LEVEL);

#ifdef	_DEBUG_EVENTS_EX
		lEventInterest |= ((ULONGLONG)1 << SPEI_SR_AUDIO_LEVEL);
#endif
		if	(
				(
					(FAILED (lTempResult = mRecoContext->SetNotifyCallbackFunction (&InputNotifyCallback, 0, (LPARAM)this)))
				||	(FAILED (lTempResult = mRecoContext->SetInterest (lEventInterest, lEventInterest)))
				)
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lTempResult);
		}

		if	(
				(mRecoGrammarCommands != NULL)
			&&	(
					(FAILED (lTempResult = mRecoGrammarCommands->SetGrammarState (SPGS_ENABLED)))
				||	(FAILED (lTempResult = mRecoGrammarCommands->SetRuleState (NULL, NULL, SPRS_ACTIVE)))
				)
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lTempResult, _T("Enable commands grammar"));
		}

		if	(
				(mRecoGrammarGlobal != NULL)
			&&	(
					(FAILED (lTempResult = mRecoGrammarGlobal->SetGrammarState (SPGS_ENABLED)))
				||	(FAILED (lTempResult = mRecoGrammarGlobal->SetRuleState (NULL, NULL, SPRS_ACTIVE)))
				)
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lTempResult, _T("Enable global grammar"));
		}

		if	(
				(FAILED (lResult = mRecoContext->SetContextState (SPCS_ENABLED)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lResult, _T("Enable context"));
		}

#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("[%p] StartListening --- LastStream [%u] ---"), this, mEventLastStream);
#endif
		if	(
				(SUCCEEDED (lResult))
			&&	(FAILED (lResult = mInput->SafeGetRecognizer()->SetRecoState (SPRST_ACTIVE)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lResult, _T("StartListening"));
		}
#ifdef	_DEBUG_STATUS
		LogStatus (_DEBUG_STATUS, _T("StartListening"));
#endif
	}
	return lResult;
}

HRESULT CSapi5InputContext::StopListening ()
{
	HRESULT	lResult = E_UNEXPECTED;

	if	(this)
	{
		HRESULT	lTempResult;

#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("[%p] StopListening --- LastStream [%u] ---"), this, mEventLastStream);
#endif
		if	(
				(mInput->SafeGetRecognizer ())
			&&	(FAILED (lResult = mInput->SafeGetRecognizer()->SetRecoState (SPRST_INACTIVE)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lResult, _T("StopListening"));
		}

		if	(
				(mRecoContext != NULL)
			&&	(FAILED (lResult = mRecoContext->SetContextState (SPCS_DISABLED)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lResult, _T("Disable context"));
		}

		if	(
				(mRecoGrammarCommands != NULL)
			&&	(FAILED (lTempResult = mRecoGrammarCommands->SetGrammarState (SPGS_DISABLED)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lTempResult, _T("Disable commands grammar"));
		}

		if	(
				(mRecoGrammarGlobal != NULL)
			&&	(FAILED (lTempResult = mRecoGrammarGlobal->SetGrammarState (SPGS_DISABLED)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lTempResult, _T("Disable global grammar"));
		}

		if	(
				(mRecoContext != NULL)
			&&	(FAILED (lTempResult = mRecoContext->SetNotifySink (NULL)))
			&&	(LogIsActive ())
			)
		{
			LogSapi5Err (LogNormal, lTempResult);
		}

		mEventSoundStarted = false;
#ifdef	_DEBUG_STATUS
		LogStatus (_DEBUG_STATUS, _T("StopListening"));
#endif
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

typedef CAtlOwnPtrList <struct GrammarPhrase> GrammarPhrases;

struct GrammarPhrase
{
	TCHAR					mTypeFlag;
	tPtr <CAtlString>		mPhrase;
	tPtr <GrammarPhrases>	mPhrases;

	GrammarPhrase (TCHAR pTypeFlag = 0) : mTypeFlag (pTypeFlag) {}
	GrammarPhrase (const CAtlString & pPhrase, TCHAR pTypeFlag = 0) : mTypeFlag (pTypeFlag), mPhrase (new CAtlString (pPhrase)) {}
};

//////////////////////////////////////////////////////////////////////

static inline bool IsolatePhrase (const CAtlString & pSpeech, int pBegNdx, int pEndNdx, CAtlString & pPhrase)
{
	pPhrase = pSpeech.Mid (pBegNdx, pEndNdx-pBegNdx+1);
	pPhrase.TrimLeft ();
	pPhrase.TrimRight ();
	return !pPhrase.IsEmpty ();
}

//////////////////////////////////////////////////////////////////////

static GrammarPhrase * ParseSpeech (const CAtlString & pSpeech, int pBegNdx, int & pEndNdx, TCHAR pEndAt = 0)
{
	tPtr <GrammarPhrase>	lPhrase;
	tPtr <GrammarPhrases>	lPhrases = new GrammarPhrases;
	tPtr <GrammarPhrases>	lAlternates;
	CAtlString				lPhraseStr;
	int						lNdx;
	int						lEndNdx;

	pBegNdx = max (pBegNdx, 0);
	pEndNdx = min (pEndNdx, pSpeech.GetLength()-1);

	for	(lNdx = pBegNdx; lNdx <= pEndNdx; lNdx++)
	{
		TCHAR	lChar = pSpeech [lNdx];

		if	(lChar == _T('('))
		{
			if	(
					(IsolatePhrase (pSpeech, pBegNdx, lNdx-1, lPhraseStr))
				&&	(lPhrase = new GrammarPhrase (lPhraseStr))
				)
			{
				lPhrases->AddTail (lPhrase.Detach());
			}
			if	(lPhrase = ParseSpeech (pSpeech, lNdx+1, lEndNdx=INT_MAX, _T(')')))
			{
				lPhrases->AddTail (lPhrase.Detach());
				lNdx = lEndNdx;
				pBegNdx = lEndNdx+1;
			}
		}
		else
		if	(lChar == _T('['))
		{
			if	(
					(IsolatePhrase (pSpeech, pBegNdx, lNdx-1, lPhraseStr))
				&&	(lPhrase = new GrammarPhrase (lPhraseStr))
				)
			{
				lPhrases->AddTail (lPhrase.Detach());
			}
			if	(lPhrase = ParseSpeech (pSpeech, lNdx+1, lEndNdx=INT_MAX, _T(']')))
			{
				lPhrases->AddTail (lPhrase.Detach());
				lNdx = lEndNdx;
				pBegNdx = lEndNdx+1;
			}
		}
		else
		if	(lChar == _T('|'))
		{
			if	(
					(IsolatePhrase (pSpeech, pBegNdx, lNdx-1, lPhraseStr))
				&&	(lPhrase = new GrammarPhrase (lPhraseStr))
				)
			{
				lPhrases->AddTail (lPhrase.Detach());
			}
			if	(!lAlternates)
			{
				lAlternates = new GrammarPhrases;
			}
			if	(lAlternates)
			{
				if	(
						(lPhrases->GetCount() > 1)
					&&	(lPhrase = new GrammarPhrase)
					)
				{
					lPhrase->mPhrases = lPhrases.Detach ();
				}
				else
				{
					lPhrase = lPhrases->RemoveHead ();
				}
				lPhrases = new GrammarPhrases;

				lAlternates->AddTail (lPhrase.Detach());
				pBegNdx = lNdx+1;
			}
		}
		else
		if	(
				(lChar == _T('*'))
			||	(lChar == _T('+'))
			)
		{
			GrammarPhrase *	lLastPhrase;

			for	(lEndNdx = lNdx-1; lEndNdx >= pBegNdx; lEndNdx--)
			{
				if	(!_istspace (pSpeech [lEndNdx]))
				{
					break;
				}
			}
			for	(lEndNdx--; lEndNdx >= pBegNdx; lEndNdx--)
			{
				if	(_istspace (pSpeech [lEndNdx]))
				{
					lEndNdx--;
					break;
				}
			}
			if	(lEndNdx > pBegNdx)
			{
				if	(
						(IsolatePhrase (pSpeech, pBegNdx, lEndNdx, lPhraseStr))
					&&	(lPhrase = new GrammarPhrase (lPhraseStr))
					)
				{
					lPhrases->AddTail (lPhrase.Detach());
				}
				pBegNdx = lEndNdx+1;
			}
			lEndNdx = lNdx-1;
			if	(lEndNdx > pBegNdx)
			{
				if	(
						(IsolatePhrase (pSpeech, pBegNdx, lEndNdx, lPhraseStr))
					&&	(lPhrase = new GrammarPhrase (lPhraseStr))
					)
				{
					lPhrases->AddTail (lPhrase.Detach());
				}
			}

			if	(
					(!lPhrases->IsEmpty ())
				&&	(lLastPhrase = lPhrases->GetTail())
				)
			{
				if	(lLastPhrase->mTypeFlag == 0)
				{
					lLastPhrase->mTypeFlag = lChar;
				}
				else
				if	(
						(lLastPhrase->mTypeFlag != _T('*'))
					&&	(lLastPhrase->mTypeFlag != _T('+'))
					&&	(lPhrase = new GrammarPhrase (lChar))
					&&	(lPhrase->mPhrases = new GrammarPhrases)
					)
				{
					lPhrase->mPhrases->AddTail (lPhrases->RemoveTail());
					lPhrases->AddTail (lPhrase.Detach());
				}
			}
			pBegNdx = lNdx+1;
		}
		else
		if	(
				(lChar == _T('…'))
			||	(
					(lChar == _T('.'))
				&&	(pSpeech.Mid (lNdx, 3) == _T("..."))
				)
			)
		{
			for	(lEndNdx = lNdx-1; lEndNdx >= pBegNdx; lEndNdx--)
			{
				if	(!_istspace (pSpeech [lEndNdx]))
				{
					break;
				}
			}
			if	(lEndNdx > pBegNdx)
			{
				if	(
						(IsolatePhrase (pSpeech, pBegNdx, lEndNdx, lPhraseStr))
					&&	(lPhrase = new GrammarPhrase (lPhraseStr))
					)
				{
					lPhrases->AddTail (lPhrase.Detach());
				}
				pBegNdx = lEndNdx+1;
			}
			if	(lPhrase = new GrammarPhrase (_T('…')))
			{
				lPhrases->AddTail (lPhrase.Detach());
			}
			if	(lChar == _T('…'))
			{
				pBegNdx = lNdx+1;
			}
			else
			{
				pBegNdx = lNdx+3;
			}
		}
		else
		if	(lChar == pEndAt)
		{
			break;
		}
	}

	if	(
			(IsolatePhrase (pSpeech, pBegNdx, lNdx-1, lPhraseStr))
		&&	(lPhrase = new GrammarPhrase (lPhraseStr))
		)
	{
		lPhrases->AddTail (lPhrase.Detach());
	}
	if	(
			(lPhrases->GetCount() == 1)
		&&	(lPhrases->GetHead ()->mTypeFlag == 0)
		)
	{
		lPhrase = lPhrases->RemoveHead ();
		lPhrase->mTypeFlag = pEndAt;
	}
	else
	if	(lPhrase = new GrammarPhrase (pEndAt))
	{
		lPhrase->mPhrases = lPhrases.Detach ();
	}

	if	(lAlternates)
	{
		lAlternates->AddTail (lPhrase.Detach ());
		if	(lPhrase = new GrammarPhrase (_T('|')))
		{
			lPhrase->mPhrases = lAlternates.Detach ();
		}
	}

	pEndNdx = min (lNdx, pEndNdx);
	return lPhrase.Detach ();
}

//////////////////////////////////////////////////////////////////////

static HRESULT PutPhrase (ISpGrammarBuilder * pGrammar, SPSTATEHANDLE pInitialState, SPSTATEHANDLE pFinalState, GrammarPhrase * pPhrase)
{
	HRESULT			lResult = S_OK;
	SPSTATEHANDLE	lInitialState = pInitialState;
	SPSTATEHANDLE	lFinalState = pFinalState;
	SPSTATEHANDLE	lInterimState = NULL;

	if	(pPhrase->mTypeFlag == _T(']'))
	{
		if	(SUCCEEDED (lResult = pGrammar->AddWordTransition (lInitialState, lFinalState, NULL, NULL, SPWT_LEXICAL, 1.0f, NULL)))
		{
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("    [%p] to [%p] Transition [%s]"), lInitialState, lFinalState, NULL);
#endif
		}
	}
	else
	if	(pPhrase->mTypeFlag == _T('*'))
	{
		lResult = pGrammar->CreateNewState (lInitialState, &lInterimState);
		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = pGrammar->AddWordTransition (lInterimState, lInitialState, NULL, NULL, SPWT_LEXICAL, 1.0f, NULL)))
			)
		{
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("      [%p] to [%p] Transition [%s]"), lInterimState, lInitialState, NULL);
#endif
		}
		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = pGrammar->AddWordTransition (lInitialState, lFinalState, NULL, NULL, SPWT_LEXICAL, 1.0f, NULL)))
			)
		{
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("      [%p] to [%p] Transition [%s]"), lInitialState, lFinalState, NULL);
#endif
		}
		if	(SUCCEEDED (lResult))
		{
			lFinalState = lInterimState;
		}
	}
	else
	if	(pPhrase->mTypeFlag == _T('+'))
	{
		lResult = pGrammar->CreateNewState (lInitialState, &lInterimState);
		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = pGrammar->AddWordTransition (lInterimState, lInitialState, NULL, NULL, SPWT_LEXICAL, 1.0f, NULL)))
			)
		{
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("      [%p] to [%p] Transition [%s]"), lInterimState, lInitialState, NULL);
#endif
		}
		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = pGrammar->AddWordTransition (lInterimState, lFinalState, NULL, NULL, SPWT_LEXICAL, 1.0f, NULL)))
			)
		{
#ifdef	_DEBUG_GRAMMAR
			LogMessage (_DEBUG_GRAMMAR, _T("      [%p] to [%p] Transition [%s]"), lInterimState, lFinalState, NULL);
#endif
		}
		if	(SUCCEEDED (lResult))
		{
			lFinalState = lInterimState;
		}
	}

	if	(SUCCEEDED (lResult))
	{
		if	(pPhrase->mTypeFlag == _T('…'))
		{
			if(SUCCEEDED (lResult = pGrammar->AddRuleTransition (lInitialState, lFinalState, SPRULETRANS_WILDCARD, 1.0f, NULL)))
			{
#ifdef	_DEBUG_GRAMMAR
				LogMessage (_DEBUG_GRAMMAR, _T("    [%p] to [%p] Transition Wildcard"), lInitialState, lFinalState);
#endif
			}
		}
		else
		if	(pPhrase->mPhrase)
		{
			if	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = pGrammar->AddWordTransition (lInitialState, lFinalState, *(pPhrase->mPhrase), _T(" "), SPWT_LEXICAL, 1.0f, NULL)))
				)
			{
#ifdef	_DEBUG_GRAMMAR
				LogMessage (_DEBUG_GRAMMAR, _T("    [%p] to [%p] Transition [%s]"), lInitialState, lFinalState, *(pPhrase->mPhrase));
#endif
			}
		}
		else
		if	(
				(pPhrase->mPhrases)
			&&	(!pPhrase->mPhrases->IsEmpty ())
			)
		{
			POSITION		lPos;
			GrammarPhrase *	lPhrase;

			for	(lPos = pPhrase->mPhrases->GetHeadPosition (); lPos;)
			{
				if	(lPhrase = pPhrase->mPhrases->GetNext (lPos))
				{
					if	(
							(pPhrase->mTypeFlag == _T('|'))
						||	(lPhrase == pPhrase->mPhrases->GetTail ())
						)
					{
						lInterimState = lFinalState;
					}
					else
					if	(FAILED (lResult = pGrammar->CreateNewState (lInitialState, &lInterimState)))
					{
						break;
					}

					if	(SUCCEEDED (lResult = PutPhrase (pGrammar, lInitialState, lInterimState, lPhrase)))
					{
						if	(pPhrase->mTypeFlag != _T('|'))
						{
							lInitialState = lInterimState;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

#ifdef	_LOG_GRAMMAR_ERRS
	if	(
			(FAILED (lResult))
		&&	(LogIsActive())
		)
	{
		LogSapi5ErrAnon (_LOG_GRAMMAR_ERRS, lResult, _T("PutPhrase"));
	}
#endif
	return lResult;
}

//////////////////////////////////////////////////////////////////////

SPSTATEHANDLE CSapi5InputContext::MakeSpeechRule (ISpGrammarBuilder * pGrammar, DWORD pRuleId, LPCTSTR pRuleName, LPCTSTR pSpeech)
{
	CAtlString				lRuleName (pRuleName);
	CAtlString				lSpeech (pSpeech);
	SPSTATEHANDLE			lRule = NULL;
	tPtr <GrammarPhrase>	lPhrase;
	int						lEndNdx;
	HRESULT					lResult;

	if	(pRuleName)
	{
		lRuleName.Replace (_T("&&"), _T("\x01"));
		lRuleName.Replace (_T("&"), _T(""));
		lRuleName.Replace (_T("\x01"), _T("&&"));
		pRuleName = lRuleName;
	}

	lSpeech.Replace (_T('\r'), _T(' '));
	lSpeech.Replace (_T('\n'), _T(' '));
	lSpeech.Replace (_T('\t'), _T(' '));
	while (lSpeech.Replace (_T("  "), _T(" ")) > 0)
	{}
	while (lSpeech.Replace (_T(" ("), _T("(")) > 0)
	{}
	while (lSpeech.Replace (_T("( "), _T("(")) > 0)
	{}
	while (lSpeech.Replace (_T(" )"), _T(")")) > 0)
	{}
	while (lSpeech.Replace (_T(") "), _T(")")) > 0)
	{}
	while (lSpeech.Replace (_T(" ["), _T("[")) > 0)
	{}
	while (lSpeech.Replace (_T("[ "), _T("[")) > 0)
	{}
	while (lSpeech.Replace (_T(" ]"), _T("]")) > 0)
	{}
	while (lSpeech.Replace (_T("] "), _T("]")) > 0)
	{}
	while (lSpeech.Replace (_T(" |"), _T("|")) > 0)
	{}
	while (lSpeech.Replace (_T("| "), _T("|")) > 0)
	{}
	while (lSpeech.Replace (_T(" *"), _T("*")) > 0)
	{}
	while (lSpeech.Replace (_T(" +"), _T("+")) > 0)
	{}
	lSpeech.TrimLeft ();
	lSpeech.TrimRight ();

	if	(
			(SUCCEEDED (lResult = pGrammar->GetRule (pRuleName, pRuleId, SPRAF_TopLevel|SPRAF_Active|SPRAF_Dynamic, TRUE, &lRule)))
		&&	(SUCCEEDED (lResult = pGrammar->ClearRule (lRule)))
		)
	{
#ifdef	_DEBUG_GRAMMAR
		LogMessage (_DEBUG_GRAMMAR, _T("  [%p] Rule [%hu %hu] [%s] Speech [%s]"), lRule, HIWORD(pRuleId), LOWORD(pRuleId), DebugStr(pRuleName), DebugStr(lSpeech));
#endif
		if	(lPhrase = ParseSpeech (lSpeech, 0, lEndNdx=INT_MAX))
		{
			lResult = PutPhrase (pGrammar, lRule, NULL, lPhrase);
		}
	}
#ifdef	_LOG_GRAMMAR_ERRS
	else
	if	(LogIsActive())
	{
		LogSapi5Err (_LOG_GRAMMAR_ERRS, lResult);
	}
#endif
	return lRule;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void __stdcall CSapi5InputContext::InputNotifyCallback(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CSapi5InputContext *	lThis = (CSapi5InputContext *) lParam;
		CSpEvent			lEvent;

#ifdef	_DEBUG_EVENTS_EX
		tS <SPEVENTSOURCEINFO>	lEventInfo;
		lEventInfo.ullEventInterest = SPFEI_ALL_TTS_EVENTS;
		lEventInfo.ullQueuedInterest = SPFEI_ALL_TTS_EVENTS;
		LogSapi5Err (LogIfActive|LogHighVolume, lThis->mRecoContext->GetInfo (&lEventInfo));
		LogMessage (_DEBUG_EVENTS, _T("[%p] EventCount [%u]"), lThis, lEventInfo.ulCount);
#endif

		while (lEvent.GetFrom (lThis->mRecoContext) == S_OK)
		{
#ifdef	_DEBUG_EVENTS
			CAtlString	lEventStr;

			switch (lEvent.eEventId)
			{
				case SPEI_RECO_STATE_CHANGE:	lEventStr.Format (_T("RECO_STATE_CHANGE %d %s"), lEvent.wParam, RecoStateStr(lEvent.RecoState())); break;
				case SPEI_RECO_OTHER_CONTEXT:	lEventStr = _T("RECO_OTHER_CONTEXT"); break;
				case SPEI_START_INPUT_STREAM:	lEventStr = _T("START_INPUT_STREAM"); break;
				case SPEI_END_INPUT_STREAM:		lEventStr = _T("END_INPUT_STREAM"); break;
				case SPEI_START_SR_STREAM:		lEventStr = _T("START_SR_STREAM"); break;
				case SPEI_END_SR_STREAM:		lEventStr = _T("END_SR_STREAM"); break;
				case SPEI_SOUND_START:			lEventStr = _T("SOUND_START"); break;
				case SPEI_SOUND_END:			lEventStr = _T("SOUND_END"); break;
				case SPEI_PHRASE_START:			lEventStr = _T("PHRASE_START"); break;
				case SPEI_RECOGNITION:			lEventStr = _T("RECOGNITION"); break;
				case SPEI_FALSE_RECOGNITION:	lEventStr = _T("FALSE_RECOGNITION"); break;
				case SPEI_HYPOTHESIS:			lEventStr = _T("HYPOTHESIS"); break;
				case SPEI_INTERFERENCE:			lEventStr.Format (_T("INTERFERENCE %d %s"), lEvent.wParam, InterferenceStr(lEvent.Interference())); break;
				case SPEI_ADAPTATION:			lEventStr = _T("ADAPTATION"); break;
				case SPEI_SR_BOOKMARK:			lEventStr = _T("SR_BOOKMARK"); break;
				case SPEI_SR_AUDIO_LEVEL:		lEventStr.Format (_T("SR_AUDIO_LEVEL %d"), lEvent.wParam); break;
				case SPEI_SR_PRIVATE:			lEventStr = _T("SR_PRIVATE"); break;
				case SPEI_UNDEFINED:			lEventStr = _T("UNDEFINED"); break;
				default:						lEventStr.Format (_T("%u"), lEvent.eEventId);
			}
			LogMessage (_DEBUG_EVENTS, _T("  [%p] Stream [%u] Event [%u] [%s]"), lThis->mRecoContext.GetInterfacePtr(), lEvent.ulStreamNum, lEvent.eEventId, lEventStr);
#endif
			switch (lEvent.eEventId)
			{
				case SPEI_START_SR_STREAM:
				{
					lThis->mEventLastStream = max (lThis->mEventLastStream, lEvent.ulStreamNum);
					lThis->mEventSoundStarted = false;
				}	break;
				case SPEI_END_SR_STREAM:
				case SPEI_SOUND_END:
				{
					lThis->mEventSoundStarted = false;
				}	break;
				case SPEI_SOUND_START:
				{
					lThis->mEventSoundStarted = true;
				}	break;
			}

			if	(
					(lEvent.ulStreamNum >= lThis->mEventLastStream)
				&&	(lThis->mEventSinks.GetCount() > 0)
				)
			{
				INT_PTR					lNdx;
				ISapi5InputEventSink *	lEventSink;

				for	(lNdx = 0; lNdx < (INT_PTR)lThis->mEventSinks.GetCount(); lNdx++)
				{
					if	(lEventSink = lThis->mEventSinks [lNdx])
					{
						try
						{
							lEventSink->OnSapi5InputEvent (lEvent);
						}
						catch AnyExceptionSilent
					}
				}
			}
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

bool CSapi5InputContext::AddEventSink (ISapi5InputEventSink * pEventSink, CSapi5InputContext * pPrevSourceContext)
{
	bool	lRet = false;

	if	(pEventSink)
	{
		if	(mEventSinks.AddUnique (pEventSink) >= 0)
		{
			if	(pPrevSourceContext)
			{
				mEventLastStream = max (mEventLastStream, pPrevSourceContext->mEventLastStream);
			}
			lRet = true;
		}
	}
	return lRet;
}

bool CSapi5InputContext::RemoveEventSink (ISapi5InputEventSink * pEventSink)
{
	bool	lRet = false;

	if	(pEventSink)
	{
		if	(mEventSinks.Remove (pEventSink) >= 0)
		{
			lRet = true;
		}
	}
	return lRet;
}

bool CSapi5InputContext::FindEventSink (ISapi5InputEventSink * pEventSink) const
{
	if	(
			(pEventSink)
		&&	(mEventSinks.Find (pEventSink) >= 0)
		)
	{
		return true;
	}
	return false;
}

void CSapi5InputContext::ClearEventSinks ()
{
	mEventSinks.RemoveAll ();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

tBstrPtr AudioStateStr (SPAUDIOSTATE pAudioState)
{
	CAtlString	lState;

	switch (pAudioState)
	{
		case SPAS_CLOSED:	lState = _T("CLOSED"); break;
		case SPAS_STOP:		lState = _T("STOP"); break;
		case SPAS_PAUSE:	lState = _T("PAUSE"); break;
		case SPAS_RUN:		lState = _T("RUN"); break;
		default:			lState.Format (_T("%d"), pAudioState);
	}
	return lState.AllocSysString();
}

tBstrPtr RecoStateStr (SPRECOSTATE pRecoState)
{
	CAtlString	lState;

	switch (pRecoState)
	{
		case SPRST_INACTIVE:			lState = _T("INACTIVE"); break;
		case SPRST_ACTIVE:				lState = _T("ACTIVE"); break;
		case SPRST_ACTIVE_ALWAYS:		lState = _T("ACTIVE_ALWAYS"); break;
		case SPRST_INACTIVE_WITH_PURGE:	lState = _T("INACTIVE_WITH_PURGE"); break;
		default:						lState.Format (_T("%d"), pRecoState);
	}
	return lState.AllocSysString();
}

tBstrPtr ContextStateStr (SPCONTEXTSTATE pContextState)
{
	CAtlString	lState;

	switch (pContextState)
	{
		case SPCS_ENABLED:		lState = _T("ENABLED"); break;
		case SPCS_DISABLED:		lState = _T("DISABLED"); break;
		default:				lState.Format (_T("%d"), pContextState);
	}
	return lState.AllocSysString();
}

tBstrPtr GrammarStateStr (SPGRAMMARSTATE pGrammarState)
{
	CAtlString	lState;

	switch (pGrammarState)
	{
		case SPGS_ENABLED:		lState = _T("ENABLED"); break;
		case SPGS_DISABLED:		lState = _T("DISABLED"); break;
		case SPGS_EXCLUSIVE:	lState = _T("EXCLUSIVE"); break;
		default:				lState.Format (_T("%d"), pGrammarState);
	}
	return lState.AllocSysString();
}

tBstrPtr ConfidenceStr (signed char pConfidence)
{
	CAtlString	lConfidence;

	switch (pConfidence)
	{
		case SP_LOW_CONFIDENCE:		lConfidence = _T("LOW"); break;
		case SP_NORMAL_CONFIDENCE:	lConfidence = _T("NORMAL"); break;
		case SP_HIGH_CONFIDENCE:	lConfidence = _T("HIGH"); break;
		default:					lConfidence.Format (_T("%d"), (int)(short)pConfidence);
	}
	return lConfidence.AllocSysString();
}

tBstrPtr InterferenceStr (SPINTERFERENCE pInterference)
{
	CAtlString	lInterference;

	switch (pInterference)
	{
		case SPINTERFERENCE_NOISE:		lInterference = _T("NOISE"); break;
		case SPINTERFERENCE_NOSIGNAL:	lInterference = _T("NOSIGNAL"); break;
		case SPINTERFERENCE_TOOLOUD:	lInterference = _T("TOOLOUD"); break;
		case SPINTERFERENCE_TOOQUIET:	lInterference = _T("TOOQUIET"); break;
		case SPINTERFERENCE_TOOFAST:	lInterference = _T("TOOFAST"); break;
		case SPINTERFERENCE_TOOSLOW:	lInterference = _T("TOOSLOW"); break;
		default:						lInterference.Format (_T("%d"), pInterference);
	}
	return lInterference.AllocSysString();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CSapi5Input::LogStatus (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString				lTitle;
			CAtlString				lIndent;
			tS <SPRECOGNIZERSTATUS>	Status;
			SPRECOSTATE				lRecoState;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Recognizer");
			}
			LogMessage (pLogLevel, _T("%s%s [%p] [%ls]"), lIndent, lTitle, mRecognizer.GetInterfacePtr(), (BSTR)const_cast<CSapi5Input*>(this)->GetEngineName());

			if	(mRecognizer != NULL)
			{
				LogSapi5Err (pLogLevel, mRecognizer->GetRecoState (&(lRecoState=(SPRECOSTATE)-1)));
				LogMessage (pLogLevel, _T("%s  State     [%s]"), lIndent, RecoStateStr(lRecoState));

				if	(SUCCEEDED (LogSapi5Err (pLogLevel, mRecognizer->GetStatus (&Status))))
				{
					LogMessage (pLogLevel, _T("%s  Rules     [%u]"), lIndent, Status.ulNumActive);
					LogMessage (pLogLevel, _T("%s  Audio     [%s] [%u] [%ls]"), lIndent, AudioStateStr(Status.AudioStatus.State), Status.AudioStatus.dwAudioLevel, (BSTR)const_cast<CSapi5Input*>(this)->GetInputName());
					LogMessage (pLogLevel, _T("%s  Stream    [%u] at [%I64u]"), lIndent, Status.ulStreamNumber, Status.ullRecognitionStreamPos);
					LogMessage (pLogLevel, _T("%s  Engine    [%s]"), lIndent, (CString)CGuidStr(Status.clsidEngine));
					LogMessage (pLogLevel, _T("%s  Languages [%u] [%s]"), lIndent, Status.cLangIDs, FormatArray(Status.aLangID, (INT_PTR)Status.cLangIDs, _T("%4.4X"), _T(" ")));
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void CSapi5InputContext::LogStatus (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString					lTitle;
			CAtlString					lIndent;
			tS <SPRECOCONTEXTSTATUS>	Status;
			SPCONTEXTSTATE				lContextState;
			SPGRAMMARSTATE				lGrammarState;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Recognizer Context");
			}

			LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, mRecoContext.GetInterfacePtr());
			if	(mInput)
			{
				mInput->LogStatus (pLogLevel, _T("%s  Recognizer"), lIndent);
			}
			if	(mRecoGrammarCommands)
			{
				LogSapi5Err (pLogLevel, mRecoGrammarCommands->GetGrammarState (&(lGrammarState=(SPGRAMMARSTATE)-1)));
				LogMessage (pLogLevel, _T("%s  CommandsGrammar [%I64u] [%p] state [%s]"), lIndent, mGrammarIdCommands, mRecoGrammarCommands, GrammarStateStr(lGrammarState));
			}
			if	(mRecoGrammarGlobal)
			{
				LogSapi5Err (pLogLevel, mRecoGrammarGlobal->GetGrammarState (&(lGrammarState=(SPGRAMMARSTATE)-1)));
				LogMessage (pLogLevel, _T("%s  GlobalGrammar  [%I64u] [%p] state [%s]"), lIndent, mGrammarIdGlobal, mRecoGrammarGlobal, GrammarStateStr(lGrammarState));
			}
			if	(mRecoContext != NULL)
			{
				LogSapi5Err (pLogLevel, mRecoContext->GetContextState (&(lContextState=(SPCONTEXTSTATE)-1)));
				LogMessage (pLogLevel, _T("%s  ContextState   [%s]"), lIndent, ContextStateStr(lContextState));
				if	(SUCCEEDED (LogSapi5Err (pLogLevel, mRecoContext->GetStatus (&Status))))
				{
					LogMessage (pLogLevel, _T("%s  Interference   [%u]"), lIndent, Status.eInterference);
				}
			}
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void LogRecoResult (UINT pLogLevel, ISpRecoResult * pResult, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString				lTitle;
			CAtlString				lIndent;
			tS <SPRECORESULTTIMES>	lResultTimes;
			ULONG					lAlternateCount;
			ULONG					lAlternateNdx;
			ISpPhraseAltPtr			lAlternates [100];

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Recognized");
			}

			LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pResult);
			if	(pResult)
			{
				if	(SUCCEEDED (LogSapi5Err (pLogLevel, pResult->GetResultTimes (&lResultTimes))))
				{
					LogMessage (pLogLevel, _T("%s  Time [%s] at [%s] for [%s]"), lIndent, LclFormatTime(lResultTimes.ftStreamTime), LclFormatDuration((LONGLONG)lResultTimes.ullStart), LclFormatDuration((LONGLONG)lResultTimes.ullLength));
				}
				LogRecoPhrase (pLogLevel, pResult, _T("%s  Phrase"), lIndent);
				if	(SUCCEEDED (LogSapi5Err (MaxLogLevel(pLogLevel,LogVerbose), pResult->GetAlternates (0, SPPR_ALL_ELEMENTS, 100, (ISpPhraseAlt**)lAlternates, &lAlternateCount))))
				{
					for	(lAlternateNdx = 0; lAlternateNdx < lAlternateCount; lAlternateNdx++)
					{
						LogRecoPhraseAlt (pLogLevel, lAlternates [lAlternateNdx], _T("%s  Alternate %u"), lIndent, lAlternateNdx);
					}
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void LogRecoPhrase (UINT pLogLevel, ISpPhrase * pPhrase, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString				lTitle;
			CAtlString				lIndent;
			tMallocPtr <SPPHRASE>	lPhrase;
			tMallocPtr <WCHAR>		lPhraseText;
			BYTE					lPhraseDisplay;
			ULONG					lElementNdx;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Phrase");
			}

			if	(
					(pPhrase)
				&&	(SUCCEEDED (LogSapi5Err (pLogLevel, pPhrase->GetPhrase (lPhrase.Free()))))
				)
			{
				LogSapi5Err (pLogLevel, pPhrase->GetText (SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, lPhraseText.Free(), &lPhraseDisplay));
				LogMessage (pLogLevel, _T("%s%s [%ls]"), lIndent, lTitle, (LPCWSTR)lPhraseText);
				LogMessage (pLogLevel, _T("%s  Grammar [%I64u] Rule [%u] [%s] LangID [%4.4X]"), lIndent, lPhrase->ullGrammarID, lPhrase->Rule.ulId, lPhrase->Rule.pszName, lPhrase->LangID);
				LogMessage (pLogLevel, _T("%s  Confidence [%f] [%s]"), lIndent, lPhrase->Rule.SREngineConfidence, ConfidenceStr(lPhrase->Rule.Confidence));
				for	(lElementNdx = 0; lElementNdx < lPhrase->Rule.ulCountOfElements; lElementNdx++)
				{
					LogMessage (pLogLevel, _T("%s  Element [%ls] [%ls] confidence [%s] of [%s]"), lIndent, lPhrase->pElements[lElementNdx].pszLexicalForm, lPhrase->pElements[lElementNdx].pszDisplayText, ConfidenceStr(lPhrase->pElements[lElementNdx].ActualConfidence), ConfidenceStr(lPhrase->pElements[lElementNdx].RequiredConfidence));
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pPhrase);
			}
		}
		catch AnyExceptionSilent
	}
}

void LogRecoPhraseAlt (UINT pLogLevel, ISpPhraseAlt * pPhrase, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			CAtlString	lIndent;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
				lIndent = lTitle;
				lTitle.TrimLeft ();
				lIndent = CAtlString (_T(' '), lIndent.GetLength()-lTitle.GetLength());
			}
			else
			{
				lTitle = _T("Alternate");
			}

			LogRecoPhrase (pLogLevel, pPhrase, _T("%s%s"), lIndent, lTitle);
		}
		catch AnyExceptionSilent
	}
}
