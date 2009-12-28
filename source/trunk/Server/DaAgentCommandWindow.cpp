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
#include "DaServer.h"
#include "DaAgentCommandWindow.h"
#include "VoiceCommandsWnd.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentCommandWindow, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentCommandWindow, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentCommandWindow::CDaAgentCommandWindow (CVoiceCommandsWnd * pVoiceCommandsWnd, LPCTSTR pClientMutexName)
:	mVoiceCommandsWnd (pVoiceCommandsWnd)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommandWindow::CDaAgentCommandWindow (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	ManageObjectLifetime (this, pClientMutexName);
}

CDaAgentCommandWindow::~CDaAgentCommandWindow ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommandWindow::~CDaAgentCommandWindow (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	try
	{
		TheServerApp->OnDeleteAgentCommandWindow (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAgentCommandWindow::Terminate (bool pFinal, bool pAbandonned)
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
					ExternalDisconnect ();
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

void CDaAgentCommandWindow::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommandWindow::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

void CDaAgentCommandWindow::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommandWindow::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentCommandWindow, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentCommandWindow)
	DISP_FUNCTION_ID(CDaAgentCommandWindow, "GetVisible", DISPID_IAgentCommandWindow_GetVisible, DspGetVisible, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommandWindow, "SetVisible", DISPID_IAgentCommandWindow_SetVisible, DspSetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommandWindow, "GetPosition", DISPID_IAgentCommandWindow_GetPosition, DspGetPosition, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommandWindow, "GetSize", DISPID_IAgentCommandWindow_GetSize, DspGetSize, VT_EMPTY, VTS_PI4 VTS_PI4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentCommandWindow, CCmdTarget)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(IDaSvrCommandWindow), Window)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(IAgentCommandWindow), Window)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAgentCommandWindow, __uuidof(IOleWindow), OleWindow)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentCommandWindow, Window)
IMPLEMENT_DISPATCH_IID(CDaAgentCommandWindow, __uuidof(IDaSvrCommandWindow))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentCommandWindow, __uuidof(IDaSvrCommandWindow))

BEGIN_SUPPORTERRORINFO(CDaAgentCommandWindow)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommandWindow, __uuidof(IDaSvrCommandWindow))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommandWindow, __uuidof(IAgentCommandWindow))
END_SUPPORTERRORINFO(CDaAgentCommandWindow)

IMPLEMENT_IUNKNOWN(CDaAgentCommandWindow, OleWindow)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommandWindow::XWindow::GetVisible (long *pbVisible)
{
	METHOD_PROLOGUE(CDaAgentCommandWindow, Window)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::GetVisible"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mVoiceCommandsWnd->GetSafeHwnd())
	{
		if	(pbVisible)
		{
			(*pbVisible) = pThis->mVoiceCommandsWnd->IsWindowVisible ();
		}
		lResult = pThis->mVoiceCommandsWnd->IsWindowVisible () ? S_OK : S_FALSE;
	}
	else
	{
		if	(pbVisible)
		{
			(*pbVisible) = FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::GetVisible"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommandWindow::XWindow::SetVisible (long bVisible)
{
	METHOD_PROLOGUE(CDaAgentCommandWindow, Window)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::SetVisible [%d]"), pThis, pThis->m_dwRef, bVisible);
#endif
	HRESULT	lResult = S_OK;

	if	(bVisible)
	{
		if	(pThis->mVoiceCommandsWnd->GetSafeHwnd())
		{
			if	(pThis->mVoiceCommandsWnd->IsWindowVisible ())
			{
				lResult = S_FALSE;
			}
			else
			{
				pThis->mVoiceCommandsWnd->Show ();
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	else
	{
		if	(pThis->mVoiceCommandsWnd->GetSafeHwnd())
		{
			if	(pThis->mVoiceCommandsWnd->IsWindowVisible ())
			{
				pThis->mVoiceCommandsWnd->Hide ();
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

	PutServerError (lResult, __uuidof(IDaSvrCommandWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::SetVisible"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommandWindow::XWindow::GetPosition (long *plLeft, long *plTop)
{
	METHOD_PROLOGUE(CDaAgentCommandWindow, Window)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::GetPosition"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mVoiceCommandsWnd->GetSafeHwnd())
	{
		CRect	lWinRect;

		pThis->mVoiceCommandsWnd->GetWindowRect (&lWinRect);
		if	(plLeft)
		{
			(*plLeft) = lWinRect.left;
		}
		if	(plTop)
		{
			(*plTop) = lWinRect.top;
		}
	}
	else
	{
		if	(plLeft)
		{
			(*plLeft) = 0;
		}
		if	(plTop)
		{
			(*plTop) = 0;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::GetPosition"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommandWindow::XWindow::GetSize (long *plWidth, long *plHeight)
{
	METHOD_PROLOGUE(CDaAgentCommandWindow, Window)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::GetSize"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mVoiceCommandsWnd->GetSafeHwnd())
	{
		CRect	lWinRect;

		pThis->mVoiceCommandsWnd->GetWindowRect (&lWinRect);
		if	(plWidth)
		{
			(*plWidth) = lWinRect.Width();
		}
		if	(plHeight)
		{
			(*plHeight) = lWinRect.Height();
		}
	}
	else
	{
		if	(plWidth)
		{
			(*plWidth) = 0;
		}
		if	(plHeight)
		{
			(*plHeight) = 0;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommandWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommandWindow::XWindow::GetSize"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCommandWindow::DspGetVisible(long * Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::DspGetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xWindow.GetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommandWindow::DspSetVisible(long Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::DspSetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xWindow.SetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommandWindow::DspGetPosition(long * Left, long * Top)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::DspGetPosition"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xWindow.GetPosition (Left, Top);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommandWindow::DspGetSize(long * Width, long * Height)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::DspGetSize"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xWindow.GetSize (Width, Height);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommandWindow::XOleWindow::GetWindow (HWND *phwnd)
{
	METHOD_PROLOGUE(CDaAgentCommandWindow, OleWindow)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::XOleWindow::GetWindow"), pThis, pThis->m_dwRef);
#endif
	if	(phwnd)
	{
		(*phwnd) = pThis->mVoiceCommandsWnd->GetSafeHwnd();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommandWindow::XOleWindow::ContextSensitiveHelp (BOOL fEnterMode)
{
	METHOD_PROLOGUE(CDaAgentCommandWindow, OleWindow)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommandWindow::XOleWindow::ContextSensitiveHelp"), pThis, pThis->m_dwRef);
#endif
	return E_NOTIMPL;
}
