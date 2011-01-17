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
#include "DaServerApp.h"
#include "DaSvrPropertySheet.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED			MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrPropertySheet::DaSvrPropertySheet ()
:	CDaCmnPropertySheet (&_AtlModule, _AtlModule)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::DaSvrPropertySheet (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrPropertySheet::~DaSvrPropertySheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::~DaSvrPropertySheet (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrPropertySheet (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrPropertySheet * DaSvrPropertySheet::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrPropertySheet> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrPropertySheet>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrPropertySheet::Terminate (bool pFinal, bool pAbandonned)
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

void DaSvrPropertySheet::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::FinalRelease [%p]"), this, max(m_dwRef,-1), m_hWnd);
	}
#endif
	Terminate (false, !CSvrObjLifetime::VerifyClientLifetime());
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrPropertySheet::PreShowSheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::PreShowSheet [%p]"), this, max(m_dwRef,-1), m_hWnd);
	}
#endif
	AddRef ();
	CPropSheetOptions::PreShowSheet ();
}

void DaSvrPropertySheet::OnFinalMessage (HWND pWnd)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrPropertySheet::OnFinalMessage [%p]"), this, max(m_dwRef,-1), m_hWnd);
	}
#endif
	CPropSheetOptions::OnFinalMessage (pWnd);
	Release ();
}

void DaSvrPropertySheet::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrPropertySheet::OnClientEnded [%p]"), this, max(m_dwRef,-1), m_hWnd);
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrPropertySheet::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrPropertySheet), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrPropertySheet2), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentPropertySheet), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;
	HRESULT			lResult = get_Visible (&lVisible);

	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetPosition (long *Left, long *Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetPosition"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::GetPosition (Left, Top);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetPosition"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetPosition (long Left, long Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::SetPosition [%d %d]"), this, max(m_dwRef,-1), Left, Top);
#endif
	HRESULT	lResult = CDaCmnPropertySheet::SetPosition (Left, Top);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::SetPosition [%d %d]"), this, max(m_dwRef,-1), Left, Top);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetSize"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::GetSize (Width, Height);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::GetSize"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetPage (BSTR *Page)
{
	return get_Page (Page);
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::SetPage (BSTR Page)
{
	return put_Page (Page);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Left (short Left)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::put_Left"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::put_Left (Left);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::put_Left"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Left (short *Left)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Left"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::get_Left (Left);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Left"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Top (short Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::put_Top"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::put_Top (Top);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::put_Top"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Top (short *Top)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Top"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::get_Top (Top);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Top"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Height (short *Height)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Height"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::get_Height (Height);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Height"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Width (short *Width)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Width"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::get_Width (Width);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Width"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::put_Visible [%d]"), this, max(m_dwRef,-1), Visible);
#endif
	HRESULT	lResult = CDaCmnPropertySheet::put_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::put_Visible"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Visible"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::get_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Visible"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::put_Page (BSTR Page)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::put_Page [%ls]"), this, max(m_dwRef,-1), Page);
#endif
	HRESULT	lResult = CDaCmnPropertySheet::put_Page (Page);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::put_Page"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::get_Page (BSTR *Page)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::get_Page"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnPropertySheet::get_Page (Page);

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrPropertySheet::get_Page"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::GetWindow (HWND *phwnd)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::GetWindow"), this, max(m_dwRef,-1));
#endif
	if	(phwnd)
	{
		(*phwnd) = m_hWnd;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaSvrPropertySheet::ContextSensitiveHelp (BOOL fEnterMode)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrPropertySheet::ContextSensitiveHelp"), this, max(m_dwRef,-1));
#endif
	return E_NOTIMPL;
}
