/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaControl.h"
#include "DaCtlCharacterFiles.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
/////////////////////////////////////////////////////////////////////////////

CDaCtlCharacterFiles::CDaCtlCharacterFiles ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::CDaCtlCharacterFiles (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

CDaCtlCharacterFiles::~CDaCtlCharacterFiles ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::~CDaCtlCharacterFiles (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlCharacterFiles::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::FinalRelease [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void CDaCtlCharacterFiles::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::Terminate [%u] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
		}
#endif
#endif
		if	(pFinal)
		{
			mOwner = NULL;
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlCharacterFiles::SetOwner (CDaControlObj * pOwner)
{
	mOwner = pOwner;
	if	(mOwner->mServer != NULL)
	{
		LogComErr (LogNormal, pOwner->mServer->GetCharacterFiles (&mServerObject));
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacterFiles::SetOwner (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

CDaControlObj * CDaCtlCharacterFiles::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCtlCharacterFiles::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlCharacterFiles::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaCtlCharacterFiles), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_FilePaths"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FilePaths)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePaths) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FilePaths (FilePaths);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_FilePaths"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!SearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SearchPath) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_SearchPath (SearchPath);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_SearchPath (BSTR SearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_SearchPath (SearchPath);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_DefaultSearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!DefaultSearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultSearchPath) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DefaultSearchPath (DefaultSearchPath);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_DefaultSearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_DoubleAgentFiles (VARIANT_BOOL *DoubleAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!DoubleAgentFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DoubleAgentFiles) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
				{
					(*DoubleAgentFiles) = (lFilter & FILES_PATH_DOUBLE_AGENT) ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_DoubleAgentFiles (VARIANT_BOOL DoubleAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(DoubleAgentFiles)
				{
					lFilter |= FILES_PATH_DOUBLE_AGENT;
				}
				else
				{
					lFilter &= ~FILES_PATH_DOUBLE_AGENT;
				}
				lResult = mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_MsAgentFiles (VARIANT_BOOL *MsAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!MsAgentFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*MsAgentFiles) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
				{
					(*MsAgentFiles) = (lFilter & FILES_PATH_MS_AGENT) ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_MsAgentFiles (VARIANT_BOOL MsAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(MsAgentFiles)
				{
					lFilter |= FILES_PATH_MS_AGENT;
				}
				else
				{
					lFilter &= ~FILES_PATH_MS_AGENT;
				}
				lResult = mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_MsOfficeFiles (VARIANT_BOOL *MsOfficeFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!MsOfficeFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*MsOfficeFiles) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
				{
					(*MsOfficeFiles) = (lFilter & FILES_PATH_MS_OFFICE) ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_MsOfficeFiles (VARIANT_BOOL MsOfficeFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(MsOfficeFiles)
				{
					lFilter |= FILES_PATH_MS_OFFICE;
				}
				else
				{
					lFilter &= ~FILES_PATH_MS_OFFICE;
				}
				lResult = mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_SpeakingCharacters (VARIANT_BOOL *SpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!SpeakingCharacters)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeakingCharacters) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
				{
					(*SpeakingCharacters) = (lFilter & FILES_EXCLUDE_SPEAKING) ? VARIANT_FALSE : VARIANT_TRUE;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_SpeakingCharacters (VARIANT_BOOL SpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(SpeakingCharacters)
				{
					lFilter &= ~FILES_EXCLUDE_SPEAKING;
				}
				else
				{
					lFilter |= FILES_EXCLUDE_SPEAKING;
					lFilter &= ~FILES_EXCLUDE_NONSPEAKING;
				}
				lResult = mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_NonSpeakingCharacters (VARIANT_BOOL *NonSpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!NonSpeakingCharacters)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*NonSpeakingCharacters) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
				{
					(*NonSpeakingCharacters) = (lFilter & FILES_EXCLUDE_NONSPEAKING) ? VARIANT_FALSE : VARIANT_TRUE;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_NonSpeakingCharacters (VARIANT_BOOL NonSpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(NonSpeakingCharacters)
				{
					lFilter &= ~FILES_EXCLUDE_NONSPEAKING;
				}
				else
				{
					lFilter |= FILES_EXCLUDE_NONSPEAKING;
					lFilter &= ~FILES_EXCLUDE_SPEAKING;
				}
				lResult = mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::get_VerifyVersion (VARIANT_BOOL *VerifyVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!VerifyVersion)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VerifyVersion) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
				{
					(*VerifyVersion) = (lFilter & FILES_NO_VALIDATE_VERSION) ? VARIANT_FALSE : VARIANT_TRUE;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::get_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacterFiles::put_VerifyVersion (VARIANT_BOOL VerifyVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(VerifyVersion)
				{
					lFilter &= ~FILES_NO_VALIDATE_VERSION;
				}
				else
				{
					lFilter |= FILES_NO_VALIDATE_VERSION;
					lFilter &= ~FILES_EXCLUDE_NONSPEAKING;
					lFilter &= ~FILES_EXCLUDE_SPEAKING;
				}
				lResult = mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacterFiles::put_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
