/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "DaSvrTTSPrivate.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif
#include "GuidStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#define	_DEBUG_ABANDONED	_LOG_ABANDONED
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrTTSPrivate::DaSvrTTSPrivate ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSPrivate::DaSvrTTSPrivate (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrTTSPrivate::~DaSvrTTSPrivate()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSPrivate::~DaSvrTTSPrivate (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrTTSPrivate* DaSvrTTSPrivate::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrTTSPrivate>*	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrTTSPrivate>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrTTSPrivate::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
		if	(pAbandonned)
		{
			Disconnect (pAbandonned);
		}
		if	(pFinal)
		{
			Disconnect (pAbandonned);
		}
	}
}

void DaSvrTTSPrivate::Disconnect (bool pAbandonned)
{
	if	(m_dwRef > 0)
	{
		if	(pAbandonned)
		{
			m_dwRef = SHRT_MIN;
		}
		__try
		{
			LogComErr (LogIfActive, CoDisconnectObject (GetUnknown(), 0));
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__, EXCEPTION_EXECUTE_HANDLER))
		{}
	}
	if	(pAbandonned)
	{
		m_dwRef = min (m_dwRef, SHRT_MIN);
	}
	else
	{
		m_dwRef = min (m_dwRef, 0);
	}
}

void DaSvrTTSPrivate::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrTTSPrivate::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, !_VerifyClientLifetime());
}

void DaSvrTTSPrivate::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrTTSPrivate::OnClientEnded"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, true);
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrTTSPrivate::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrTTSPrivate), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_TTSModeID (BSTR *TTSModeID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_TTSModeID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_TTSModeID (TTSModeID);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_TTSModeID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_DisplayName (BSTR *DisplayName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_DisplayName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_DisplayName (DisplayName);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_DisplayName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_Manufacturer (BSTR *Manufacturer)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_Manufacturer"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_Manufacturer (Manufacturer);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_Manufacturer"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::GetVersion (short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::GetVersion"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::GetVersion (MajorVersion, MinorVersion);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::GetVersion"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_Gender (SpeechGenderType *Gender)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_Gender"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_Gender (Gender);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_Gender"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_LanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_LanguageID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_LanguageID (LanguageID);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_LanguageID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_LanguageName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_LanguageName (EnglishName, LanguageName);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_LanguageName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitDisplayName (BSTR *DisplayName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitDisplayName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitDisplayName (DisplayName);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitDisplayName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitDisplayName (BSTR DisplayName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitDisplayName"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitDisplayName (DisplayName);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitDisplayName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitManufacturer (BSTR *Manufacturer)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitManufacturer"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitManufacturer (Manufacturer);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitManufacturer"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitManufacturer (BSTR Manufacturer)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitManufacturer"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitManufacturer (Manufacturer);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitManufacturer"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitGender (SpeechGenderType *Gender)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitGender"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitGender (Gender);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitGender"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitGender (SpeechGenderType Gender)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitGender"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitGender (Gender);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitGender"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitVersion (BSTR *Version)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitVersion"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitVersion (Version);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitVersion"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitVersion (BSTR Version)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitVersion"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitVersion (Version);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitVersion"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitLanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitLanguageID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitLanguageID (LanguageID);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitLanguageID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitLanguageID (long LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitLanguageID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitLanguageID (LanguageID);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitLanguageID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitString (BSTR ValuePath, BSTR *StringValue)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitString"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitString (ValuePath, StringValue);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitString"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitString (BSTR ValuePath, BSTR StringValue)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitString"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitString (ValuePath, StringValue);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitString"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitAttribute (BSTR AttributeName, BSTR *AttributeValue)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitAttribute"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitAttribute (AttributeName, AttributeValue);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitAttribute"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitAttribute (BSTR AttributeName, BSTR AttributeValue)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitAttribute"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitAttribute (AttributeName, AttributeValue);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitAttribute"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::get_InitFilePath (BSTR FileId, BSTR *FilePath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::get_InitFilePath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::get_InitFilePath (FileId, FilePath);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::get_InitFilePath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrTTSPrivate::put_InitFilePath (BSTR FileId, BSTR FilePath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrTTSPrivate::put_InitFilePath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnTTSPrivate::put_InitFilePath (FileId, FilePath);

	PutServerError (lResult, __uuidof(IDaSvrTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrTTSPrivate::put_InitFilePath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
