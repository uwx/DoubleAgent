/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaCtlTTSPrivate.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

#ifdef	_DACORE_LOCAL
#define	LogServerPtr	(void*)NULL
#else
#define	LogServerPtr	mServerObject.GetInterfacePtr()
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlTTSPrivate::DaCtlTTSPrivate ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::DaCtlTTSPrivate (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlTTSPrivate::~DaCtlTTSPrivate ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::~DaCtlTTSPrivate (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlTTSPrivate::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::FinalRelease (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
	Terminate (false);
}

void DaCtlTTSPrivate::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::Terminate [%u] [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, LogServerPtr, mLocalObject.Ptr());
		}
#endif
#endif
		if	(pFinal)
		{
			mOwner = NULL;
		}
		SafeFreeSafePtr (mLocalObject);
#ifndef	_DACORE_LOCAL
		if	(pFinal)
		{
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#endif
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::Terminate [%u] [%p] [%p] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, LogServerPtr, mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlTTSPrivate::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
#ifndef	_DACORE_LOCAL
		if	(mOwner->mServer)
		{
			if	(!mServerObject)
			{
				lResult = E_FAIL;
			}
		}
		else
#endif
		{
			mLocalObject = new CDaCmnTTSPrivate;
		}
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::SetOwner (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
	return lResult;
}

DaControl * DaCtlTTSPrivate::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlTTSPrivate::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlTTSPrivate::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaCtlTTSPrivate), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_TTSModeID (BSTR *TTSModeID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!TTSModeID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSModeID) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_TTSModeID (TTSModeID);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_TTSModeID (TTSModeID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_DisplayName (BSTR *DisplayName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_DisplayName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DisplayName) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DisplayName (DisplayName);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DisplayName (DisplayName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_DisplayName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_Manufacturer (BSTR *Manufacturer)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_Manufacturer"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Manufacturer) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Manufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Manufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_Manufacturer"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::GetVersion (short *MajorVersion, short *MinorVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::GetVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(MajorVersion)
	{
		(*MajorVersion) = 0;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = 0;
	}

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->GetVersion (MajorVersion, MinorVersion);
		}
		catch AnyExceptionDebug
	}
#ifndef	_DACORE_LOCAL
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetVersion (MajorVersion, MinorVersion);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
#endif

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::GetVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_Gender (SpeechGenderType *Gender)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_Gender"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Gender)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Gender) = SpeechGender_Neutral;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Gender (Gender);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Gender (Gender);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_Gender"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_LanguageID (long *LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageID) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_LanguageID (LanguageID);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_LanguageID (LanguageID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_LanguageName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageName) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_LanguageName (EnglishName, LanguageName);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_LanguageName (EnglishName, LanguageName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_LanguageName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitDisplayName (BSTR *DisplayName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitDisplayName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DisplayName) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitDisplayName (DisplayName);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitDisplayName (DisplayName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitDisplayName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitDisplayName (BSTR DisplayName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitDisplayName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!DisplayName)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_InitDisplayName (DisplayName);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_InitDisplayName (DisplayName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitDisplayName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitManufacturer (BSTR *Manufacturer)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitManufacturer"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Manufacturer) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitManufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitManufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitManufacturer"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitManufacturer (BSTR Manufacturer)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitManufacturer"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Manufacturer)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_InitManufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_InitManufacturer (Manufacturer);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitManufacturer"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitVersion (BSTR *Version)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Version)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Version) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitVersion (Version);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitVersion (Version);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitVersion (BSTR Version)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Version)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_InitVersion (Version);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_InitVersion (Version);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitGender (SpeechGenderType *Gender)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitGender"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!Gender)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Gender) = SpeechGender_Neutral;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitGender (Gender);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitGender (Gender);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitGender"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitGender (SpeechGenderType Gender)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitGender"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_InitGender (Gender);
		}
		catch AnyExceptionDebug
	}
#ifndef	_DACORE_LOCAL
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_InitGender (Gender);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
#endif

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitGender"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitLanguageID (long *LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitLanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageID) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitLanguageID (LanguageID);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitLanguageID (LanguageID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitLanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitLanguageID (long LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitLanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_InitLanguageID (LanguageID);
		}
		catch AnyExceptionDebug
	}
#ifndef	_DACORE_LOCAL
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_InitLanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
#endif

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitLanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitString (BSTR ValuePath, BSTR *StringValue)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitString"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!ValuePath)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!StringValue)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*StringValue) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitString (ValuePath, StringValue);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitString (ValuePath, StringValue);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitString"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitString (BSTR ValuePath, BSTR StringValue)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitString"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(
			(!ValuePath)
		||	(!StringValue)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_InitString (ValuePath, StringValue);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_InitString (ValuePath, StringValue);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitString"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitAttribute (BSTR AttributeName, BSTR *AttributeValue)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitAttribute"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!AttributeName)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!AttributeValue)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AttributeValue) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitAttribute (AttributeName, AttributeValue);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitAttribute (AttributeName, AttributeValue);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitAttribute"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitAttribute (BSTR AttributeName, BSTR AttributeValue)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitAttribute"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(
			(!AttributeName)
		||	(!AttributeValue)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_InitAttribute (AttributeName, AttributeValue);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_InitAttribute (AttributeName, AttributeValue);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitAttribute"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::get_InitFilePath (BSTR FileId, BSTR *FilePath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!FileId)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!FilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePath) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_InitFilePath (FileId, FilePath);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_InitFilePath (FileId, FilePath);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::get_InitFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSPrivate::put_InitFilePath (BSTR FileId, BSTR FilePath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(
			(!FileId)
		||	(!FilePath)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_InitFilePath (FileId, FilePath);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_InitFilePath (FileId, FilePath);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSPrivate));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSPrivate::put_InitFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
