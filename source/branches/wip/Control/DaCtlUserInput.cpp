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
#include "DaCtlUserInput.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlUserInput::DaCtlUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaCtlUserInput::DaCtlUserInput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlUserInput::~DaCtlUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaCtlUserInput::~DaCtlUserInput (%d) [%p]"), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlUserInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaCtlUserInput::FinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void DaCtlUserInput::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaCtlUserInput::Terminate [%u] [%p(%u)]"), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
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
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaCtlUserInput::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlUserInput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlUserInput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlUserInput), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Count (short *Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Count"), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lCount = 0;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Count (&lCount);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*Count) = (short)lCount;
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Name (BSTR *Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Name"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Name) = NULL;

		if	(
				(mServerCommands != NULL)
			&&	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommand2Ptr	lCommand;

				if	(
						(SUCCEEDED (mServerObject->get_ItemCommandID (0, &lCommandId)))
					&&	(SUCCEEDED (mServerCommands->get_Command (lCommandId, &lCommand)))
					)
				{
					lCommand->get_Caption (Name);
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Name"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_CharacterID (BSTR *CharacterID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_CharacterID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!CharacterID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharacterID) = mCharacterID.AllocSysString();
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_CharacterID"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Confidence (long *Confidence)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Confidence"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Confidence) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ItemConfidence (0, Confidence);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Confidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Voice (BSTR *Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Voice"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Voice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ItemText (0, Voice);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Voice"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Alt1Name (BSTR *Alt1Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Alt1Name"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!Alt1Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Alt1Name) = NULL;

		if	(
				(mServerCommands != NULL)
			&&	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommand2Ptr	lCommand;

				if	(
						(SUCCEEDED (mServerObject->get_ItemCommandID (1, &lCommandId)))
					&&	(SUCCEEDED (mServerCommands->get_Command (lCommandId, &lCommand)))
					)
				{
					lCommand->get_Caption (Alt1Name);
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Alt1Name"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Alt1Confidence (long *Alt1Confidence)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Alt1Confidence"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Alt1Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Alt1Confidence) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ItemConfidence (1, Alt1Confidence);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Alt1Confidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Alt1Voice (BSTR *Alt1Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Alt1Voice"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Alt1Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Alt1Voice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ItemText (1, Alt1Voice);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Alt1Voice"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Alt2Name (BSTR *Alt2Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Alt2Name"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!Alt2Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Alt2Name) = NULL;

		if	(
				(mServerCommands != NULL)
			&&	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommand2Ptr	lCommand;

				if	(
						(SUCCEEDED (mServerObject->get_ItemCommandID (2, &lCommandId)))
					&&	(SUCCEEDED (mServerCommands->get_Command (lCommandId, &lCommand)))
					)
				{
					lCommand->get_Caption (Alt2Name);
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Alt2Name"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Alt2Confidence (long *Alt2Confidence)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Alt2Confidence"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Alt2Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Alt2Confidence) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ItemConfidence (2, Alt2Confidence);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Alt2Confidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlUserInput::get_Alt2Voice (BSTR *Alt2Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaCtlUserInput::get_Alt2Voice"), this, m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(!Alt2Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Alt2Voice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ItemText (2, Alt2Voice);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaCtlUserInput::get_Alt2Voice"), this, m_dwRef);
	}
#endif
	return lResult;
}
