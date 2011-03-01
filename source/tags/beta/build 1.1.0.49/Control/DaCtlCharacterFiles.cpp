/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif
/////////////////////////////////////////////////////////////////////////////

DaCtlCharacterFiles::DaCtlCharacterFiles ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::DaCtlCharacterFiles (%d) [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::~DaCtlCharacterFiles (%d) [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::FinalRelease [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, max(m_dwRef,-1), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::Terminate [%u] [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
		}
#endif
#endif

		Disconnect (pFinal);
		mOwner = NULL;

#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::Terminate [%u] [%p] [%p] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

void DaCtlCharacterFiles::Disconnect (bool pFinal)
{
	if	(pFinal)
	{
		mServerObject.Detach ();
	}
	else
	{
		SafeFreeSafePtr (mServerObject);
	}
	SafeFreeSafePtr (mLocalObject);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlCharacterFiles::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	mOwner = pOwner;
	if	(mOwner->mServer != NULL)
	{
		LogComErr (LogNormal|LogTime, lResult = pOwner->mServer->get_CharacterFiles (&mServerObject));
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::SetOwner (%d) [%p] [%p]"), mOwner, SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
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
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
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

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_FilePaths"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_FilePaths"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_SearchPath (BSTR SearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_SearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DefaultSearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DefaultSearchPath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DoubleAgentFiles (VARIANT_BOOL *DoubleAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_DoubleAgentFiles (VARIANT_BOOL DoubleAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_DoubleAgentFiles [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), DoubleAgentFiles);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_DoubleAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_MsAgentFiles (VARIANT_BOOL *MsAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_MsAgentFiles (VARIANT_BOOL MsAgentFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_MsAgentFiles [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), MsAgentFiles);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_MsAgentFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_MsOfficeFiles (VARIANT_BOOL *MsOfficeFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_MsOfficeFiles (VARIANT_BOOL MsOfficeFiles)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_MsOfficeFiles [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), MsOfficeFiles);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_MsOfficeFiles"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_CompliantCharacters (VARIANT_BOOL *CompliantCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_CompliantCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!CompliantCharacters)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CompliantCharacters) = VARIANT_TRUE;

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
			(*CompliantCharacters) = (lFilter & FilesFilter_ExcludeCompliant) ? VARIANT_FALSE : VARIANT_TRUE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_CompliantCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_CompliantCharacters (VARIANT_BOOL CompliantCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_CompliantCharacters [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), CompliantCharacters);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(CompliantCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeCompliant;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeCompliant;
					lFilter &= ~FilesFilter_ExcludeNonCompliant;
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
				if	(CompliantCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeCompliant;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeCompliant;
					lFilter &= ~FilesFilter_ExcludeNonCompliant;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_CompliantCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_NonCompliantCharacters (VARIANT_BOOL *NonCompliantCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_NonCompliantCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!NonCompliantCharacters)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*NonCompliantCharacters) = VARIANT_TRUE;

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
			(*NonCompliantCharacters) = (lFilter & FilesFilter_ExcludeNonCompliant) ? VARIANT_FALSE : VARIANT_TRUE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_NonCompliantCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_NonCompliantCharacters (VARIANT_BOOL NonCompliantCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_NonCompliantCharacters [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), NonCompliantCharacters);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Filter (&lFilter)))
			{
				if	(NonCompliantCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeNonCompliant;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeNonCompliant;
					lFilter &= ~FilesFilter_ExcludeCompliant;
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
				if	(NonCompliantCharacters)
				{
					lFilter &= ~FilesFilter_ExcludeNonCompliant;
				}
				else
				{
					lFilter |= FilesFilter_ExcludeNonCompliant;
					lFilter &= ~FilesFilter_ExcludeCompliant;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_NonCompliantCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_SpeakingCharacters (VARIANT_BOOL *SpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_SpeakingCharacters (VARIANT_BOOL SpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_SpeakingCharacters [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), SpeakingCharacters);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_SpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_NonSpeakingCharacters (VARIANT_BOOL *NonSpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_NonSpeakingCharacters (VARIANT_BOOL NonSpeakingCharacters)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_NonSpeakingCharacters [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), NonSpeakingCharacters);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_NonSpeakingCharacters"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_VerifyVersion (VARIANT_BOOL *VerifyVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::put_VerifyVersion (VARIANT_BOOL VerifyVersion)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_VerifyVersion [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), VerifyVersion);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::put_VerifyVersion"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DefaultFilePath (BSTR *DefaultFilePath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DefaultFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DefaultFilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacterFiles::get_DefaultFileName (BSTR *DefaultFileName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DefaultFileName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacterFiles::get_DefaultFileName"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
