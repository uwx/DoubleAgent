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
#include "DaCtlUserInput.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

CDaCtlUserInput::CDaCtlUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCtlUserInput::CDaCtlUserInput (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

CDaCtlUserInput::~CDaCtlUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCtlUserInput::~CDaCtlUserInput (%d) [%p]"), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlUserInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCtlUserInput::FinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void CDaCtlUserInput::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCtlUserInput::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaCtlUserInput::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlUserInput::InterfaceSupportsErrorInfo(REFIID riid)
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

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Count (short *pCount)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Count"), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lCount = 0;

	if	(!pCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetCount (&lCount);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*pCount) = (short)lCount;
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Count"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Name (BSTR *pName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Name"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pName) = NULL;

		if	(
				(mServerCommands != NULL)
			&&	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommandPtr	lCommand;

				if	(
						(SUCCEEDED (mServerObject->GetItemID (0, &lCommandId)))
					&&	(SUCCEEDED (mServerCommands->GetCommandEx (lCommandId, &lCommand)))
					)
				{
					lCommand->GetCaption (pName);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Name"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_CharacterID (BSTR *pCharacterID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_CharacterID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pCharacterID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pCharacterID) = mCharacterID.AllocSysString();
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_CharacterID"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Confidence (long *pConfidence)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Confidence"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pConfidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pConfidence) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetItemConfidence (0, pConfidence);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Confidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Voice (BSTR *pVoice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Voice"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pVoice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pVoice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetItemText (0, pVoice);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Voice"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Alt1Name (BSTR *pAlt1Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Alt1Name"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pAlt1Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt1Name) = NULL;

		if	(
				(mServerCommands != NULL)
			&&	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommandPtr	lCommand;

				if	(
						(SUCCEEDED (mServerObject->GetItemID (1, &lCommandId)))
					&&	(SUCCEEDED (mServerCommands->GetCommandEx (lCommandId, &lCommand)))
					)
				{
					lCommand->GetCaption (pAlt1Name);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Alt1Name"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Alt1Confidence (long *pAlt1Confidence)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Alt1Confidence"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pAlt1Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt1Confidence) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetItemConfidence (1, pAlt1Confidence);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Alt1Confidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Alt1Voice (BSTR *pAlt1Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Alt1Voice"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pAlt1Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt1Voice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetItemText (1, pAlt1Voice);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Alt1Voice"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Alt2Name (BSTR *pAlt2Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Alt2Name"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pAlt2Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt2Name) = NULL;

		if	(
				(mServerCommands != NULL)
			&&	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommandPtr	lCommand;

				if	(
						(SUCCEEDED (mServerObject->GetItemID (2, &lCommandId)))
					&&	(SUCCEEDED (mServerCommands->GetCommandEx (lCommandId, &lCommand)))
					)
				{
					lCommand->GetCaption (pAlt2Name);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Alt2Name"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Alt2Confidence (long *pAlt2Confidence)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Alt2Confidence"), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pAlt2Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt2Confidence) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetItemConfidence (2, pAlt2Confidence);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Alt2Confidence"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlUserInput::get_Alt2Voice (BSTR *pAlt2Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaCtlUserInput::get_Alt2Voice"), this, m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(!pAlt2Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt2Voice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetItemText (2, pAlt2Voice);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaCtlUserInput::get_Alt2Voice"), this, m_dwRef);
	}
#endif
	return lResult;
}
