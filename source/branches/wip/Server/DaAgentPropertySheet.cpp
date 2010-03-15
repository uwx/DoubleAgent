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
#include "DaAgentPropertySheet.h"
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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentPropertySheet, CPropSheetOptions)
IMPLEMENT_OLETYPELIB(CDaAgentPropertySheet, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentPropertySheet::CDaAgentPropertySheet (LPCTSTR pClientMutexName)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentPropertySheet::CDaAgentPropertySheet (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	ManageObjectLifetime (this, pClientMutexName);
}

CDaAgentPropertySheet::~CDaAgentPropertySheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentPropertySheet::~CDaAgentPropertySheet (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	try
	{
		TheServerApp->OnDeleteAgentPropertySheet (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAgentPropertySheet::Terminate (bool pFinal, bool pAbandonned)
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

void CDaAgentPropertySheet::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentPropertySheet::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

void CDaAgentPropertySheet::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentPropertySheet::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HWND CDaAgentPropertySheet::GetPropSheetWnd (bool pCreate)
{
	if	(
			(!IsWindow (m_hWnd))
		&&	(pCreate)
		)
	{
		try
		{
			Create ();
		}
		catch AnyExceptionDebug
	}
	if	(IsWindow (m_hWnd))
	{
		return m_hWnd;
	}
	return NULL;
}

BOOL CDaAgentPropertySheet::OnInitDialog()
{
	BOOL lRet = CPropSheetOptions::OnInitDialog();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentPropertySheet::OnInitDialog [%p]"), this, m_dwRef, m_hWnd);
	}
#endif
	ExternalAddRef ();
	return lRet;
}

void CDaAgentPropertySheet::OnDestroy()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentPropertySheet::OnDestroy [%p]"), this, m_dwRef, m_hWnd);
	}
#endif
	CPropSheetOptions::OnDestroy();
	ExternalRelease ();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDaAgentPropertySheet, CPropSheetOptions)
	//{{AFX_MSG_MAP(CDaAgentPropertySheet)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDaAgentPropertySheet, CPropSheetOptions)
	//{{AFX_DISPATCH_MAP(CDaAgentPropertySheet)
	DISP_FUNCTION_ID(CDaAgentPropertySheet, "GetVisible", DISPID_IAgentPropertySheet_GetVisible, DspGetVisible, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentPropertySheet, "SetVisible", DISPID_IAgentPropertySheet_SetVisible, DspSetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentPropertySheet, "GetPosition", DISPID_IAgentPropertySheet_GetPosition, DspGetPosition, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentPropertySheet, "GetSize", DISPID_IAgentPropertySheet_GetSize, DspGetSize, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentPropertySheet, "GetPage", DISPID_IAgentPropertySheet_GetPage, DspGetPage, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentPropertySheet, "SetPage", DISPID_IAgentPropertySheet_SetPage, DspSetPage, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentPropertySheet, CPropSheetOptions)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(IDaSvrPropertySheet), PropSheet)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(IAgentPropertySheet), PropSheet)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAgentPropertySheet, __uuidof(IOleWindow), OleWindow)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentPropertySheet, PropSheet)
IMPLEMENT_DISPATCH_IID(CDaAgentPropertySheet, __uuidof(IDaSvrPropertySheet))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentPropertySheet, __uuidof(IDaSvrPropertySheet))

BEGIN_SUPPORTERRORINFO(CDaAgentPropertySheet)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentPropertySheet, __uuidof(IDaSvrPropertySheet))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentPropertySheet, __uuidof(IAgentPropertySheet))
END_SUPPORTERRORINFO(CDaAgentPropertySheet)

IMPLEMENT_IUNKNOWN(CDaAgentPropertySheet, OleWindow)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XPropSheet::GetVisible (long *pbVisible)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, PropSheet)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetVisible"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->GetPropSheetWnd (false))
	{
		if	(pbVisible)
		{
			(*pbVisible) = pThis->IsWindowVisible ();
		}
		lResult = pThis->IsWindowVisible () ? S_OK : S_FALSE;
	}
	else
	{
		if	(pbVisible)
		{
			(*pbVisible) = FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetVisible"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XPropSheet::SetVisible (long bVisible)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, PropSheet)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::SetVisible [%d]"), pThis, pThis->m_dwRef, bVisible);
#endif
	HRESULT	lResult = S_OK;

	if	(bVisible)
	{
		if	(pThis->GetPropSheetWnd (true))
		{
			if	(pThis->IsWindowVisible ())
			{
				lResult = S_FALSE;
			}
			else
			{
				pThis->ShowWindow (SW_SHOW);
			}
			pThis->BringWindowToTop ();
			pThis->SetForegroundWindow ();
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	else
	{
		if	(pThis->GetPropSheetWnd (false))
		{
			if	(pThis->IsWindowVisible ())
			{
				pThis->ShowWindow (SW_HIDE);
			}
			else
			{
				lResult = S_FALSE;
			}
			pThis->DestroyWindow ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::SetVisible"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XPropSheet::GetPosition (long *plLeft, long *plTop)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, PropSheet)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetPosition"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (pThis->m_hWnd == NULL);

	if	(pThis->GetPropSheetWnd (true))
	{
		CRect	lWinRect;

		pThis->GetWindowRect (&lWinRect);
		if	(plLeft)
		{
			(*plLeft) = lWinRect.left;
		}
		if	(plTop)
		{
			(*plTop) = lWinRect.top;
		}

		if	(lCreated)
		{
			pThis->DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetPosition"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XPropSheet::GetSize (long *plWidth, long *plHeight)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, PropSheet)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetSize"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (pThis->m_hWnd == NULL);

	if	(pThis->GetPropSheetWnd (true))
	{
		CRect	lWinRect;

		pThis->GetWindowRect (&lWinRect);
		if	(plWidth)
		{
			(*plWidth) = lWinRect.Width();
		}
		if	(plHeight)
		{
			(*plHeight) = lWinRect.Height();
		}

		if	(lCreated)
		{
			pThis->DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetSize"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sPageNameOutput = _T("Output");
static LPCTSTR	sPageNameSpeech = _T("Speech");
static LPCTSTR	sPageNameCopyright = _T("Copyright");

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XPropSheet::GetPage (BSTR *pbszPage)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, PropSheet)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetPage"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (pThis->m_hWnd == NULL);

	if	(pThis->GetPropSheetWnd (true))
	{
		if	(!pbszPage)
		{
			lResult = E_POINTER;
		}
		else
		{
			int	lPage = PropSheet_HwndToIndex (pThis->m_hWnd, PropSheet_GetCurrentPageHwnd (pThis->m_hWnd));

			(*pbszPage) = NULL;

			if	(lPage == 0)
			{
				(*pbszPage) = _bstr_t (sPageNameOutput).Detach();
			}
			else
			if	(lPage == 1)
			{
				(*pbszPage) = _bstr_t (sPageNameSpeech).Detach();
			}
			else
			if	(lPage == 2)
			{
				(*pbszPage) = _bstr_t (sPageNameCopyright).Detach();
			}
		}

		if	(lCreated)
		{
			pThis->DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::GetPage"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XPropSheet::SetPage (BSTR bszPage)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, PropSheet)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::SetPage [%ls]"), pThis, pThis->m_dwRef, bszPage);
#endif
	HRESULT	lResult = S_OK;
	bool	lCreated = (pThis->m_hWnd == NULL);

	if	(pThis->GetPropSheetWnd (true))
	{
		CString	lPageName (bszPage);

		if	(lPageName.CompareNoCase (sPageNameOutput) == 0)
		{
			PropSheet_SetCurSel (pThis->m_hWnd, NULL, 0);
		}
		else
		if	(lPageName.CompareNoCase (sPageNameSpeech) == 0)
		{
			PropSheet_SetCurSel (pThis->m_hWnd, NULL, 1);
		}
		else
		if	(lPageName.CompareNoCase (sPageNameCopyright) == 0)
		{
			PropSheet_SetCurSel (pThis->m_hWnd, NULL, 2);
		}
		else
		{
			lResult = E_INVALIDARG;
		}

		if	(lCreated)
		{
			pThis->DestroyWindow ();
		}
	}
	else
	{
		lResult = E_FAIL;
	}

	PutServerError (lResult, __uuidof(IDaSvrPropertySheet));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaAgentPropertySheet::XPropSheet::SetPage"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentPropertySheet::DspGetVisible(long * Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::DspGetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xPropSheet.GetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentPropertySheet::DspSetVisible(long Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::DspSetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xPropSheet.SetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentPropertySheet::DspGetPosition(long * Left, long * Top)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::DspGetPosition"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xPropSheet.GetPosition (Left, Top);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentPropertySheet::DspGetSize(long * Width, long * Height)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::DspGetSize"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xPropSheet.GetSize (Width, Height);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentPropertySheet::DspGetPage(BSTR * Page)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::DspGetPage"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xPropSheet.GetPage (Page);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentPropertySheet::DspSetPage(LPCTSTR Page)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::DspSetPage"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xPropSheet.SetPage (_bstr_t(Page));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XOleWindow::GetWindow (HWND *phwnd)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, OleWindow)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XOleWindow::GetWindow"), pThis, pThis->m_dwRef);
#endif
	if	(phwnd)
	{
		(*phwnd) = pThis->m_hWnd;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaAgentPropertySheet::XOleWindow::ContextSensitiveHelp (BOOL fEnterMode)
{
	METHOD_PROLOGUE(CDaAgentPropertySheet, OleWindow)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaAgentPropertySheet::XOleWindow::ContextSensitiveHelp"), pThis, pThis->m_dwRef);
#endif
	return E_NOTIMPL;
}
