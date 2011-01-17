/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaSvrCommandsWindow.h"
#include "VoiceCommandsWnd.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED			MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCommandsWindow::DaSvrCommandsWindow ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::DaSvrCommandsWindow (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCommandsWindow::~DaSvrCommandsWindow ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::~DaSvrCommandsWindow (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrCommandsWindow (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommandsWindow * DaSvrCommandsWindow::CreateInstance (CVoiceCommandsWnd * pVoiceCommandsWnd, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrCommandsWindow> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrCommandsWindow>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize (pVoiceCommandsWnd);
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrCommandsWindow::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrCommandsWindow::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, !CSvrObjLifetime::VerifyClientLifetime());
}

void DaSvrCommandsWindow::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrCommandsWindow::OnClientEnded"), this, max(m_dwRef,-1));
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

STDMETHODIMP DaSvrCommandsWindow::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrCommandsWindow), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrCommandsWindow2), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommandWindow), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;
	HRESULT			lResult = get_Visible (&lVisible);

	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::GetPosition (long *Left, long *Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::GetPosition"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommandsWindow::GetPosition (Left, Top);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::GetPosition"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::GetSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::GetSize"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommandsWindow::GetSize (Width, Height);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::GetSize"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::get_Visible"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommandsWindow::get_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::get_Visible"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::put_Visible [%d]"), this, max(m_dwRef,-1), Visible);
#endif
	HRESULT	lResult = CDaCmnCommandsWindow::put_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::put_Visible"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Left (short *Left)
{
	HRESULT	lResult = CDaCmnCommandsWindow::get_Left (Left);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::get_Left"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Top (short *Top)
{
	HRESULT	lResult = CDaCmnCommandsWindow::get_Top (Top);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::get_Top"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Height (short *Height)
{
	HRESULT	lResult = CDaCmnCommandsWindow::get_Height (Height);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::get_Height"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Width (short *Width)
{
	HRESULT	lResult = CDaCmnCommandsWindow::get_Width (Width);

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::get_Width"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::GetWindow (HWND *phwnd)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::GetWindow"), this, max(m_dwRef,-1));
#endif
	if	(phwnd)
	{
		if	(
				(mVoiceCommandsWnd)
			&&	(mVoiceCommandsWnd->IsWindow())
			)
		{
			(*phwnd) = mVoiceCommandsWnd->m_hWnd;
		}
		else
		{
			(*phwnd) = NULL;
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::ContextSensitiveHelp (BOOL fEnterMode)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::ContextSensitiveHelp"), this, max(m_dwRef,-1));
#endif
	return E_NOTIMPL;
}
