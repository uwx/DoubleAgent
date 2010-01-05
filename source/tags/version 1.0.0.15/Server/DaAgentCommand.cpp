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
#include "DaAgentCommand.h"
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
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentCommand, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentCommand, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentCommand::CDaAgentCommand()
:	mId (0),
	mEnabled (true),
	mVisible (true),
	mConfidenceThreshold (-25),
	mHelpContextId (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommand::CDaAgentCommand (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}

CDaAgentCommand::~CDaAgentCommand()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommand::~CDaAgentCommand (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAgentCommand::Terminate (bool pFinal, bool pAbandonned)
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
	}
}

void CDaAgentCommand::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentCommand::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentCommand, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentCommand)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetCaption", DISPID_IAgentCommand_SetCaption, DspSetCaption, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetCaption", DISPID_IAgentCommand_GetCaption, DspGetCaption, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetVoice", DISPID_IAgentCommand_SetVoice, DspSetVoice, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetVoice", DISPID_IAgentCommand_GetVoice, DspGetVoice, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetEnabled", DISPID_IAgentCommand_SetEnabled, DspSetEnabled, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetEnabled", DISPID_IAgentCommand_GetEnabled, DspGetEnabled, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetVisible", DISPID_IAgentCommand_SetVisible, DspSetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetVisible", DISPID_IAgentCommand_GetVisible, DspGetVisible, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetConfidenceThreshold", DISPID_IAgentCommand_SetConfidenceThreshold, DspSetConfidenceThreshold, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetConfidenceThreshold", DISPID_IAgentCommand_GetConfidenceThreshold, DspGetConfidenceThreshold, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetConfidenceText", DISPID_IAgentCommand_SetConfidenceText, DspSetConfidenceText, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetConfidenceText", DISPID_IAgentCommand_GetConfidenceText, DspGetConfidenceText, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetID", DISPID_IAgentCommand_GetID, DspGetid, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetHelpContextID", DISPID_IAgentCommandEx_SetHelpContextID, DspSetHelpContextID, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetHelpContextID", DISPID_IAgentCommandEx_GetHelpContextID, DspGetHelpContextID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommand, "SetVoiceCaption", DISPID_IAgentCommandEx_SetVoiceCaption, DspSetVoiceCaption, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommand, "GetVoiceCaption", DISPID_IAgentCommandEx_GetVoiceCaption, DspGetVoiceCaption, VT_EMPTY, VTS_PBSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentCommand, CCmdTarget)
	INTERFACE_PART(CDaAgentCommand, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentCommand, __uuidof(IDaSvrCommand), Command)
	INTERFACE_PART(CDaAgentCommand, __uuidof(IAgentCommand), Command)
	INTERFACE_PART(CDaAgentCommand, __uuidof(IAgentCommandEx), Command)
	INTERFACE_PART(CDaAgentCommand, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentCommand, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentCommand, Command)
IMPLEMENT_DISPATCH_IID(CDaAgentCommand, __uuidof(IDaSvrCommand))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentCommand, __uuidof(IDaSvrCommand))

BEGIN_SUPPORTERRORINFO(CDaAgentCommand)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommand, __uuidof(IDaSvrCommand))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommand, __uuidof(IAgentCommand))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommand, __uuidof(IAgentCommandEx))
END_SUPPORTERRORINFO(CDaAgentCommand)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetID (long *pdwID)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pdwID) = (long)pThis->mId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetCaption (BSTR bszCaption)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetCaption"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lCaption (bszCaption);

	if	(lCaption.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(lCaption == pThis->mCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mCaption = lCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetCaption"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetCaption (BSTR *pbszCaption)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetCaption"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszCaption) = pThis->mCaption.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetCaption"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetVoice (BSTR bszVoice)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetVoice"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoice (bszVoice);

	if	(lVoice == pThis->mVoice)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mVoice = lVoice;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetVoice"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetVoice (BSTR *pbszVoice)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetVoice"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszVoice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszVoice) = pThis->mVoice.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetVoice"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetVoiceCaption (BSTR bszVoiceCaption)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetVoiceCaption"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoiceCaption (bszVoiceCaption);

	if	(pThis->mVoiceCaption == lVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mVoiceCaption = lVoiceCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetVoiceCaption"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetVoiceCaption (BSTR *pbszVoiceCaption)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetVoiceCaption"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszVoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszVoiceCaption) = pThis->mVoiceCaption.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetVoiceCaption"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetEnabled (long bEnabled)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetEnabled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mEnabled == (bEnabled != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mEnabled = (bEnabled != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetEnabled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetEnabled (long *pbEnabled)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetEnabled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = pThis->mEnabled ? S_OK : S_FALSE;

	if	(*pbEnabled)
	{
		(*pbEnabled) = (pThis->mEnabled!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetEnabled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetVisible (long bVisible)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetVisible"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mVisible == (bVisible != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mVisible = (bVisible != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetVisible"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetVisible (long *pbVisible)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetVisible"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = pThis->mVisible ? S_OK : S_FALSE;

	if	(pbVisible)
	{
		(*pbVisible) = (pThis->mVisible!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetVisible"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetConfidenceThreshold (long lThreshold)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetConfidenceThreshold"), pThis, pThis->m_dwRef);
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
	if	(pThis->mConfidenceThreshold == (short)lThreshold)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mConfidenceThreshold = (short)lThreshold;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetConfidenceThreshold"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetConfidenceThreshold (long *plThreshold)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetConfidenceThreshold"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!plThreshold)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*plThreshold) = (long)pThis->mConfidenceThreshold;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetConfidenceThreshold"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetConfidenceText (BSTR bszTipText)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetConfidenceText"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lConfidenceText (bszTipText);

	if	(pThis->mConfidenceText == lConfidenceText)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mConfidenceText = lConfidenceText;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetConfidenceText"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetConfidenceText (BSTR *pbszTipText)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetConfidenceText"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszTipText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszTipText) = pThis->mConfidenceText.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetConfidenceText"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::SetHelpContextID (long ulID)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::SetHelpContextID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mHelpContextId == (ULONG)ulID)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mHelpContextId = (ULONG)ulID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::SetHelpContextID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommand::XCommand::GetHelpContextID (long *pulID)
{
	METHOD_PROLOGUE(CDaAgentCommand, Command)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentCommand::XCommand::GetHelpContextID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pulID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pulID) = (long)pThis->mHelpContextId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommand));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentCommand::XCommand::GetHelpContextID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCommand::DspSetCaption(LPCTSTR Caption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetCaption (_bstr_t(Caption));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetCaption(BSTR * Caption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetCaption (Caption);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetVoice(LPCTSTR Voice)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetVoice"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetVoice (_bstr_t(Voice));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetVoice(BSTR * Voice)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetVoice"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetVoice (Voice);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetEnabled(long Enabled)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetEnabled (Enabled);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetEnabled(long * Enabled)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetEnabled (Enabled);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetVisible(long Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetVisible(long * Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetConfidenceThreshold(long Threshold)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetConfidenceThreshold"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetConfidenceThreshold (Threshold);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetConfidenceThreshold(long * Threshold)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetConfidenceThreshold"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetConfidenceThreshold (Threshold);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetConfidenceText(LPCTSTR TipText)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetConfidenceText"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetConfidenceText (_bstr_t(TipText));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetConfidenceText(BSTR * TipText)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetConfidenceText"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetConfidenceText (TipText);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetid(long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetid"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetHelpContextID(long ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetHelpContextID"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetHelpContextID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetHelpContextID(long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetHelpContextID"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetHelpContextID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspSetVoiceCaption(LPCTSTR VoiceCaption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspSetVoiceCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.SetVoiceCaption (_bstr_t(VoiceCaption));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommand::DspGetVoiceCaption(BSTR * VoiceCaption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentCommand::DspGetVoiceCaption"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommand.GetVoiceCaption (VoiceCaption);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
