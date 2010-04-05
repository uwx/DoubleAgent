/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include "DaSvrSpeechEngine.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif
#include "GuidStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechEngine::DaSvrSpeechEngine ()
:	mSapi5Voice (NULL)
{
#ifndef	_WIN64
	mSapi4Voice = NULL;
#endif
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngine::DaSvrSpeechEngine (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSpeechEngine::~DaSvrSpeechEngine()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngine::~DaSvrSpeechEngine (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSpeechEngine* DaSvrSpeechEngine::CreateInstance (CSapi5VoiceInfo * pVoiceInfo, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSpeechEngine> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSpeechEngine>::CreateInstance (&lInstance))))
	{
		lInstance->mSapi5Voice = pVoiceInfo;
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

#ifndef	_WIN64
DaSvrSpeechEngine* DaSvrSpeechEngine::CreateInstance (CSapi4VoiceInfo * pVoiceInfo, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSpeechEngine> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSpeechEngine>::CreateInstance (&lInstance))))
	{
		lInstance->mSapi4Voice = pVoiceInfo;
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}
#endif

void DaSvrSpeechEngine::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void DaSvrSpeechEngine::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngine::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrSpeechEngine::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSpeechEngine::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrSpeechEngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrSpeechEngine), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetTTSModeID (BSTR *TTSModeID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetTTSModeID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!TTSModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*TTSModeID) = SysAllocString (mSapi5Voice->mVoiceIdShort);
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		CString	lTTSModeId = (CString) CGuidStr (mSapi4Voice->mModeId);
		(*TTSModeID) = lTTSModeId.AllocSysString ();
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetTTSModeID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetDisplayName (BSTR *DisplayName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetDisplayName"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*DisplayName) = SysAllocString (mSapi5Voice->mVoiceName);
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*DisplayName) = SysAllocString (mSapi4Voice->mVoiceName);
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetDisplayName"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetManufacturer (BSTR *Manufacturer)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetManufacturer"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*Manufacturer) = SysAllocString (mSapi5Voice->mManufacturer);
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*Manufacturer) = SysAllocString (mSapi4Voice->mManufacturer);
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetManufacturer"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mSapi5Voice)
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = 5;
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = 0;
		}
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = 4;
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = 0;
		}
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetVersion"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetGender (short *Gender)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetGender"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Gender)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*Gender) = (short)mSapi5Voice->mSpeakerGender;
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*Gender) = (short)mSapi4Voice->mSpeakerGender;
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetGender"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetLanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetLanguageID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		(*LanguageID) = (long)mSapi5Voice->mLangId;
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		(*LanguageID) = (long)mSapi4Voice->mLangId;
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetLanguageID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSpeechEngine::GetLanguageName (BSTR *LanguageName, boolean EnglishName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechEngine::GetLanguageName"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Voice)
	{
		LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
		int 	lInfoSize;
		CString lInfoValue;

		if	(lInfoSize = GetLocaleInfo (MAKELCID (mSapi5Voice->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
		{
			GetLocaleInfo (MAKELCID (mSapi5Voice->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
		}
		else
		{
			lResult = S_FALSE;
		}
		lInfoValue.ReleaseBuffer ();
		(*LanguageName) = lInfoValue.AllocSysString ();
	}
#ifndef	_WIN64
	else
	if	(mSapi4Voice)
	{
		LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
		int 	lInfoSize;
		CString lInfoValue;

		if	(lInfoSize = GetLocaleInfo (MAKELCID (mSapi4Voice->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
		{
			GetLocaleInfo (MAKELCID (mSapi4Voice->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
		}
		else
		{
			lResult = S_FALSE;
		}
		lInfoValue.ReleaseBuffer ();
		(*LanguageName) = lInfoValue.AllocSysString ();
	}
#endif
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechEngine::GetLanguageName"), this, m_dwRef);
	}
#endif
	return lResult;
}
