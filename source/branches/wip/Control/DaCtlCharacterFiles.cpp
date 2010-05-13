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
#include "DaControlMod.h"
#include "DaCtlCharacterFiles.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
/////////////////////////////////////////////////////////////////////////////

DaCtlCharacterFiles::DaCtlCharacterFiles ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::DaCtlCharacterFiles (%d) [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlCharacterFiles::~DaCtlCharacterFiles ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::~DaCtlCharacterFiles (%d) [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	Terminate (true);
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlCharacterFiles::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::FinalRelease [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	Terminate (false);
}

void DaCtlCharacterFiles::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::Terminate [%u] [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
		}
#endif
#endif
		if	(pFinal)
		{
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}

		SafeFreeSafePtr (mLocalObject);
		mOwner = NULL;
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::Terminate [%u] [%p] [%p] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlCharacterFiles::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	mOwner = pOwner;
	if	(mOwner->mServer != NULL)
	{
		LogComErr (LogNormal, lResult = pOwner->mServer->get_CharacterFiles (&mServerObject));
	}
	else
	if	(mLocalObject = new CDaCmnCharacterFiles)
	{
		mLocalObject->Initialize ();
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::SetOwner (%d) [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	return lResult;
}

DaControl * DaCtlCharacterFiles::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlCharacterFiles::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlCharacterFiles::InterfaceSupportsErrorInfo(REFIID riid)
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

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get__NewEnum (ppunkEnum);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get__NewEnum (ppunkEnum);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_FilePaths"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FilePaths)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePaths) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_FilePaths (FilePaths);
			}
			catch AnyExceptionDebug
		}
		else
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_FilePaths"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!SearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SearchPath) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_SearchPath (SearchPath);
			}
			catch AnyExceptionDebug
		}
		else
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_SearchPath (BSTR SearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_SearchPath (SearchPath);
		}
		catch AnyExceptionDebug
	}
	else
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DefaultSearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!DefaultSearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultSearchPath) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DefaultSearchPath (DefaultSearchPath);
			}
			catch AnyExceptionDebug
		}
		else
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DefaultSearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DoubleAgentFiles (VARIANT_BOOL *DoubleAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			(*DoubleAgentFiles) = (lFilter & FilesFilter_PathDoubleAgent) ? VARIANT_TRUE : VARIANT_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_DoubleAgentFiles (VARIANT_BOOL DoubleAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_DoubleAgentFiles [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, DoubleAgentFiles);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(DoubleAgentFiles)
				{
					lFilter |= FilesFilter_PathDoubleAgent;
				}
				else
				{
					lFilter &= ~FilesFilter_PathDoubleAgent;
				}
				lResult = mLocalObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(DoubleAgentFiles)
				{
					lFilter |= FilesFilter_PathDoubleAgent;
				}
				else
				{
					lFilter &= ~FilesFilter_PathDoubleAgent;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_MsAgentFiles (VARIANT_BOOL *MsAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			(*MsAgentFiles) = (lFilter & FilesFilter_PathMsAgent) ? VARIANT_TRUE : VARIANT_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_MsAgentFiles (VARIANT_BOOL MsAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_MsAgentFiles [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, MsAgentFiles);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(MsAgentFiles)
				{
					lFilter |= FilesFilter_PathMsAgent;
				}
				else
				{
					lFilter &= ~FilesFilter_PathMsAgent;
				}
				lResult = mLocalObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(MsAgentFiles)
				{
					lFilter |= FilesFilter_PathMsAgent;
				}
				else
				{
					lFilter &= ~FilesFilter_PathMsAgent;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_MsOfficeFiles (VARIANT_BOOL *MsOfficeFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			(*MsOfficeFiles) = (lFilter & FilesFilter_PathMsOffice) ? VARIANT_TRUE : VARIANT_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_MsOfficeFiles (VARIANT_BOOL MsOfficeFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_MsOfficeFiles [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, MsOfficeFiles);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(MsOfficeFiles)
				{
					lFilter |= FilesFilter_PathMsOffice;
				}
				else
				{
					lFilter &= ~FilesFilter_PathMsOffice;
				}
				lResult = mLocalObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(MsOfficeFiles)
				{
					lFilter |= FilesFilter_PathMsOffice;
				}
				else
				{
					lFilter &= ~FilesFilter_PathMsOffice;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_SpeakingCharacters (VARIANT_BOOL *SpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			(*SpeakingCharacters) = (lFilter & FilesFilter_ExcludeSpeaking) ? VARIANT_FALSE : VARIANT_TRUE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_SpeakingCharacters (VARIANT_BOOL SpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_SpeakingCharacters [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, SpeakingCharacters);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(SpeakingCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeSpeaking;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeSpeaking;
					lFilter &= ~FilesFilter_ExcludeNonSpeaking;
				}
				lResult = mLocalObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(SpeakingCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeSpeaking;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeSpeaking;
					lFilter &= ~FilesFilter_ExcludeNonSpeaking;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_NonSpeakingCharacters (VARIANT_BOOL *NonSpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			(*NonSpeakingCharacters) = (lFilter & FilesFilter_ExcludeNonSpeaking) ? VARIANT_FALSE : VARIANT_TRUE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_NonSpeakingCharacters (VARIANT_BOOL NonSpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_NonSpeakingCharacters [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, NonSpeakingCharacters);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(NonSpeakingCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeNonSpeaking;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeNonSpeaking;
					lFilter &= ~FilesFilter_ExcludeSpeaking;
				}
				lResult = mLocalObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(NonSpeakingCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeNonSpeaking;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeNonSpeaking;
					lFilter &= ~FilesFilter_ExcludeSpeaking;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_VerifyVersion (VARIANT_BOOL *VerifyVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Filter (&lFilter);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			(*VerifyVersion) = (lFilter & FilesFilter_NoValidateVersion) ? VARIANT_FALSE : VARIANT_TRUE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_VerifyVersion (VARIANT_BOOL VerifyVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_VerifyVersion [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, VerifyVersion);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(VerifyVersion)
				{
					lFilter &= ~FilesFilter_NoValidateVersion;
				}
				else
				{
					lFilter |= FilesFilter_NoValidateVersion;
					lFilter &= ~FilesFilter_ExcludeNonSpeaking;
					lFilter &= ~FilesFilter_ExcludeSpeaking;
				}
				lResult = mLocalObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Filter (&lFilter)))
			{
				if	(VerifyVersion)
				{
					lFilter &= ~FilesFilter_NoValidateVersion;
				}
				else
				{
					lFilter |= FilesFilter_NoValidateVersion;
					lFilter &= ~FilesFilter_ExcludeNonSpeaking;
					lFilter &= ~FilesFilter_ExcludeSpeaking;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::put_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DefaultFilePath (BSTR *DefaultFilePath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DefaultFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!DefaultFilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultFilePath) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DefaultFilePath (DefaultFilePath);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DefaultFilePath (DefaultFilePath);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DefaultFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DefaultFileName (BSTR *DefaultFileName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DefaultFileName"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!DefaultFileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultFileName) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DefaultFileName (DefaultFileName);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DefaultFileName (DefaultFileName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacterFiles::get_DefaultFileName"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
