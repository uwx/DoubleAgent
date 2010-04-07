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
#include "DaSvrCommand.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCommand::DaSvrCommand()
:	mId (0),
	mEnabled (true),
	mVisible (true),
	mConfidenceThreshold (-25),
	mHelpContextId (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommand::DaSvrCommand (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCommand::~DaSvrCommand()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommand::~DaSvrCommand (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommand * DaSvrCommand::CreateInstance ()
{
	CComObject<DaSvrCommand> *	lInstance = NULL;

	LogComErr (LogIfActive, CComObject<DaSvrCommand>::CreateInstance (&lInstance));
	return lInstance;
}

void DaSvrCommand::Terminate (bool pFinal, bool pAbandonned)
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
	}
}

void DaSvrCommand::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommand::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrCommand::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrCommand), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommand), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommandEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetID (long *pdwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pdwID) = (long)mId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetID"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetCaption (BSTR bszCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lCaption (bszCaption);

	if	(lCaption.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(lCaption == mCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mCaption = lCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetCaption"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetCaption (BSTR *pbszCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszCaption) = mCaption.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetCaption"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetVoice (BSTR bszVoice)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetVoice"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoice (bszVoice);

	if	(lVoice == mVoice)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoice = lVoice;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetVoice"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetVoice (BSTR *pbszVoice)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetVoice"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszVoice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszVoice) = mVoice.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetVoice"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetVoiceCaption (BSTR bszVoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetVoiceCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoiceCaption (bszVoiceCaption);

	if	(mVoiceCaption == lVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceCaption = lVoiceCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetVoiceCaption"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetVoiceCaption (BSTR *pbszVoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetVoiceCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszVoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszVoiceCaption) = mVoiceCaption.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetVoiceCaption"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetEnabled (long bEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mEnabled == (bEnabled != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mEnabled = (bEnabled != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetEnabled (long *pbEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = mEnabled ? S_OK : S_FALSE;

	if	(*pbEnabled)
	{
		(*pbEnabled) = (mEnabled!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetVisible (long bVisible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mVisible == (bVisible != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mVisible = (bVisible != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetVisible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetVisible (long *pbVisible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = mVisible ? S_OK : S_FALSE;

	if	(pbVisible)
	{
		(*pbVisible) = (mVisible!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetVisible"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetConfidenceThreshold (long lThreshold)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetConfidenceThreshold"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(lThreshold < -100)
		||	(lThreshold > 100)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(mConfidenceThreshold == (short)lThreshold)
	{
		lResult = S_FALSE;
	}
	else
	{
		mConfidenceThreshold = (short)lThreshold;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetConfidenceThreshold"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetConfidenceThreshold (long *plThreshold)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetConfidenceThreshold"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!plThreshold)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*plThreshold) = (long)mConfidenceThreshold;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetConfidenceThreshold"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetConfidenceText (BSTR bszTipText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetConfidenceText"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lConfidenceText (bszTipText);

	if	(mConfidenceText == lConfidenceText)
	{
		lResult = S_FALSE;
	}
	else
	{
		mConfidenceText = lConfidenceText;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetConfidenceText"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetConfidenceText (BSTR *pbszTipText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetConfidenceText"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszTipText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszTipText) = mConfidenceText.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetConfidenceText"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetHelpContextID (long ulID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::SetHelpContextID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mHelpContextId == (ULONG)ulID)
	{
		lResult = S_FALSE;
	}
	else
	{
		mHelpContextId = (ULONG)ulID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetHelpContextID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetHelpContextID (long *pulID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetHelpContextID"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pulID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pulID) = (long)mHelpContextId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetHelpContextID"), this, m_dwRef);
	}
#endif
	return lResult;
}
