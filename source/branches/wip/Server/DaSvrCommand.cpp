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
			(InlineIsEqualGUID (__uuidof(IDaSvrCommand2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrCommand), riid))
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

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetID (long *CommandID)
{
	return get_CommandID (CommandID);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetCaption (BSTR Caption)
{
	return put_Caption (Caption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetCaption (BSTR *Caption)
{
	return get_Caption (Caption);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetVoice (BSTR Voice)
{
	return put_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetVoice (BSTR *Voice)
{
	return get_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetVoiceCaption (BSTR VoiceCaption)
{
	return put_VoiceCaption (VoiceCaption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetVoiceCaption (BSTR *pbszVoiceCaption)
{
	return get_VoiceCaption (pbszVoiceCaption);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetEnabled (long Enabled)
{
	return put_Enabled (Enabled ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetEnabled (long *Enabled)
{
	VARIANT_BOOL	lEnabled = VARIANT_FALSE;
	HRESULT			lResult = get_Enabled (&lEnabled);
	
	if	(Enabled)
	{
		(*Enabled) = (lEnabled != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;
	HRESULT			lResult = get_Visible (&lVisible);
	
	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetConfidenceThreshold (long Threshold)
{
	return put_ConfidenceThreshold (Threshold);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetConfidenceThreshold (long *Threshold)
{
	return get_ConfidenceThreshold (Threshold);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetConfidenceText (BSTR TipText)
{
	return put_ConfidenceText (TipText);
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetConfidenceText (BSTR *TipText)
{
	return get_ConfidenceText (TipText);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::SetHelpContextID (long ContextID)
{
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::SetHelpContextID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::GetHelpContextID (long *ContextID)
{
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetHelpContextID"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_CommandID (long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_CommandID"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_CommandID (CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_CommandID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_Caption (BSTR *Caption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_Caption"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_Caption (Caption);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_Caption"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::put_Caption (BSTR Caption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_Caption"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_Caption (Caption);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_Caption"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_Enabled (VARIANT_BOOL *Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_Enabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_Enabled (Enabled);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_Enabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::put_Enabled (VARIANT_BOOL Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_Enabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_Enabled (Enabled);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_Enabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::GetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::GetVisible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_Visible"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_Visible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_VoiceCaption (BSTR *VoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_VoiceCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_VoiceCaption (VoiceCaption);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_VoiceCaption"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::put_VoiceCaption (BSTR VoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_VoiceCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_VoiceCaption (VoiceCaption);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_VoiceCaption"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_VoiceGrammar (BSTR *VoiceGrammar)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_VoiceGrammar"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_VoiceGrammar (VoiceGrammar);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_VoiceGrammar"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::put_VoiceGrammar (BSTR VoiceGrammar)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_VoiceGrammar"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_VoiceGrammar (VoiceGrammar);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_VoiceGrammar"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_ConfidenceThreshold (long *ConfidenceThreshold)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_ConfidenceThreshold"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_ConfidenceThreshold (ConfidenceThreshold);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_ConfidenceThreshold"), this, m_dwRef);
	}
#endif
	return lResult;
}
	
HRESULT STDMETHODCALLTYPE DaSvrCommand::put_ConfidenceThreshold (long ConfidenceThreshold)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_ConfidenceThreshold"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_ConfidenceThreshold (ConfidenceThreshold);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_ConfidenceThreshold"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommand::get_ConfidenceText (BSTR *ConfidenceText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::get_ConfidenceText"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::get_ConfidenceText (ConfidenceText);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::get_ConfidenceText"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommand::put_ConfidenceText (BSTR ConfidenceText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommand::put_ConfidenceText"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaCmnCommand::put_ConfidenceText (ConfidenceText);

	PutServerError (lResult, __uuidof(IDaSvrCommand2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommand::put_ConfidenceText"), this, m_dwRef);
	}
#endif
	return lResult;
}
