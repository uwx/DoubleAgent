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
#include "DaSvrUserInput.h"
#include "Sapi5Input.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF,_LOG_INSTANCE)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrUserInput::DaSvrUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrUserInput::DaSvrUserInput (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrUserInput::~DaSvrUserInput()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrUserInput::~DaSvrUserInput (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrUserInput * DaSvrUserInput::CreateInstance (ISpRecoResult * pRecoResult, bool pGlobalCommand, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrUserInput> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrUserInput>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize (pRecoResult, pGlobalCommand);
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrUserInput::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}

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

void DaSvrUserInput::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrUserInput::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrUserInput::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrUserInput::OnClientEnded"), this, max(m_dwRef,-1));
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

STDMETHODIMP DaSvrUserInput::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrUserInput2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrUserInput), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentUserInput), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetCount (long *Count)
{
	return get_Count (Count);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_Count"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnUserInput::get_Count (Count);

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_Count"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemID (long ItemIndex, long *CommandID)
{
	return get_ItemCommandID (ItemIndex, CommandID);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_ItemCommandID (long ItemIndex, long *ItemCommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_ItemCommandID"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnUserInput::get_ItemCommandID (ItemIndex, ItemCommandID);

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_ItemCommandID"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemConfidence (long ItemIndex, long *Confidence)
{
	return get_ItemConfidence (ItemIndex, Confidence);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_ItemConfidence (long ItemIndex, long *ItemConfidence)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_ItemConfidence"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnUserInput::get_ItemConfidence (ItemIndex, ItemConfidence);

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_ItemConfidence"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetItemText (long ItemIndex, BSTR *Text)
{
	return get_ItemText (ItemIndex, Text);
}

HRESULT STDMETHODCALLTYPE DaSvrUserInput::get_ItemText (long ItemIndex, BSTR *ItemText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::get_ItemText"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnUserInput::get_ItemText (ItemIndex, ItemText);

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::get_ItemText"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrUserInput::GetAllItemData (VARIANT *ItemIndices, VARIANT *ItemConfidences, VARIANT *ItemText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrUserInput::GetAllItemData"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnUserInput::GetAllItemData (ItemIndices, ItemConfidences, ItemText);

	PutServerError (lResult, __uuidof(IDaSvrUserInput2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrUserInput::GetAllItemData"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
