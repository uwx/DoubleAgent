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
#include "DaSvrCommandsWindow.h"
#include "VoiceCommandsWnd.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCommandsWindow::DaSvrCommandsWindow ()
:	mVoiceCommandsWnd (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::DaSvrCommandsWindow (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCommandsWindow::~DaSvrCommandsWindow ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::~DaSvrCommandsWindow (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
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

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrCommandsWindow>::CreateInstance (&lInstance))))
	{
		lInstance->mVoiceCommandsWnd = pVoiceCommandsWnd;
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrCommandsWindow::Terminate (bool pFinal, bool pAbandonned)
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

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void DaSvrCommandsWindow::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrCommandsWindow::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCommandsWindow::OnClientEnded"), this, m_dwRef);
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::GetPosition"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CRect	lWinRect (0,0,0,0);

	if	(
			(mVoiceCommandsWnd)
		&&	(mVoiceCommandsWnd->IsWindow())
		)
	{
		mVoiceCommandsWnd->GetWindowRect (&lWinRect);
	}
	else
	{
		lResult = S_FALSE;
	}
	if	(Left)
	{
		(*Left) = lWinRect.left;
	}
	if	(Top)
	{
		(*Top) = lWinRect.top;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::GetPosition"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::GetSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::GetSize"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CRect	lWinRect (0,0,0,0);

	if	(
			(mVoiceCommandsWnd)
		&&	(mVoiceCommandsWnd->IsWindow())
		)
	{
		mVoiceCommandsWnd->GetWindowRect (&lWinRect);
	}
	else
	{
		lResult = S_FALSE;
	}
	if	(Width)
	{
		(*Width) = lWinRect.Width();
	}
	if	(Height)
	{
		(*Height) = lWinRect.Height();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::GetSize"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::get_Visible"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(
			(mVoiceCommandsWnd)
		&&	(mVoiceCommandsWnd->IsWindow())
		)
	{
		lResult = mVoiceCommandsWnd->IsWindowVisible () ? S_OK : S_FALSE;
	}
	if	(Visible)
	{
		(*Visible) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::get_Visible"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::put_Visible [%d]"), this, m_dwRef, Visible);
#endif
	HRESULT	lResult = S_OK;

	if	(Visible)
	{
		if	(
				(mVoiceCommandsWnd)
			&&	(mVoiceCommandsWnd->IsWindow())
			)
		{
			if	(mVoiceCommandsWnd->IsWindowVisible ())
			{
				lResult = S_FALSE;
			}
			else
			{
				mVoiceCommandsWnd->Show ();
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	else
	{
		if	(
				(mVoiceCommandsWnd)
			&&	(mVoiceCommandsWnd->IsWindow())
			)
		{
			if	(mVoiceCommandsWnd->IsWindowVisible ())
			{
				mVoiceCommandsWnd->Hide ();
			}
			else
			{
				lResult = S_FALSE;
			}
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandsWindow2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCommandsWindow::put_Visible"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Left (short *Left)
{
	long	lLeft = 0;
	HRESULT	lResult = GetPosition (&lLeft, NULL);
	
	if	(Left)
	{
		(*Left) = (short)lLeft;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Top (short *Top)
{
	long	lTop = 0;
	HRESULT	lResult = GetPosition (NULL, &lTop);
	
	if	(Top)
	{
		(*Top) = (short)lTop;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Height (short *Height)
{
	long	lHeight = 0;
	HRESULT	lResult = GetSize (NULL, &lHeight);
	
	if	(Height)
	{
		(*Height) = (short)lHeight;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::get_Width (short *Width)
{
	long	lWidth = 0;
	HRESULT	lResult = GetSize (&lWidth, NULL);
	
	if	(Width)
	{
		(*Width) = (short)lWidth;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommandsWindow::GetWindow (HWND *phwnd)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::GetWindow"), this, m_dwRef);
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCommandsWindow::ContextSensitiveHelp"), this, m_dwRef);
#endif
	return E_NOTIMPL;
}
