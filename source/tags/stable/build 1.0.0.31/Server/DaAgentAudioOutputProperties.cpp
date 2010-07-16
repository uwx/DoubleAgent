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
#include "DaAgentAudioOutputProperties.h"
#include "DaGlobalConfig.h"
#include "DaAgentCharacter.h"
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

IMPLEMENT_DYNAMIC(CDaAgentAudioOutputProperties, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentAudioOutputProperties, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentAudioOutputProperties::CDaAgentAudioOutputProperties (LPCTSTR pClientMutexName)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentAudioOutputProperties::CDaAgentAudioOutputProperties (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	ManageObjectLifetime (this, pClientMutexName);
}

CDaAgentAudioOutputProperties::~CDaAgentAudioOutputProperties ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentAudioOutputProperties::~CDaAgentAudioOutputProperties (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	try
	{
		TheServerApp->OnDeleteAgentAudioOutputProperties (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAgentAudioOutputProperties::Terminate (bool pFinal, bool pAbandonned)
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

void CDaAgentAudioOutputProperties::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentAudioOutputProperties::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

void CDaAgentAudioOutputProperties::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentAudioOutputProperties::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaAgentAudioOutputProperties::GetAudioStatus ()
{
	long	lStatus = AUDIO_STATUS_AVAILABLE;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = TheServerApp->GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CDaAgentCharacter *		lCharacter;

			if	(TheServerApp->GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
					{
						if	(lCharacter->IsSpeaking ())
						{
							lStatus = AUDIO_STATUS_CHARACTERSPEAKING;
						}
						else
						if	(lCharacter->IsHearing ())
						{
							lStatus = AUDIO_STATUS_USERSPEAKING;
						}
						else
						if	(lCharacter->IsListening ())
						{
							lStatus = AUDIO_STATUS_SROVERRIDEABLE;
						}
					}
				}
				if	(lStatus != AUDIO_STATUS_AVAILABLE)
				{
					break;
				}
			}
			if	(lStatus != AUDIO_STATUS_AVAILABLE)
			{
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lStatus;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentAudioOutputProperties, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentAudioOutputProperties)
	DISP_FUNCTION_ID(CDaAgentAudioOutputProperties, "GetEnabled", DISPID_IAgentAudioOutputProperties_GetEnabled, DspGetEnabled, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentAudioOutputProperties, "GetUsingSoundEffects", DISPID_IAgentAudioOutputProperties_GetUsingSoundEffects, DspGetUsingSoundEffects, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentAudioOutputProperties, "GetStatus", DISPID_IAgentAudioOutputPropertiesEx_GetStatus, DspGetStatus, VT_EMPTY, VTS_PI4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentAudioOutputProperties, CCmdTarget)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties), AudioOutputProperties)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(IAgentAudioOutputProperties), AudioOutputProperties)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(IAgentAudioOutputPropertiesEx), AudioOutputProperties)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentAudioOutputProperties, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentAudioOutputProperties, AudioOutputProperties)
IMPLEMENT_DISPATCH_IID(CDaAgentAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties))

BEGIN_SUPPORTERRORINFO(CDaAgentAudioOutputProperties)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentAudioOutputProperties, __uuidof(IAgentAudioOutputProperties))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentAudioOutputProperties, __uuidof(IAgentAudioOutputPropertiesEx))
END_SUPPORTERRORINFO(CDaAgentAudioOutputProperties)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentAudioOutputProperties::XAudioOutputProperties::GetEnabled (long *pbEnabled)
{
	METHOD_PROLOGUE(CDaAgentAudioOutputProperties, AudioOutputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentAudioOutputProperties::XAudioOutputProperties::GetEnabled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = CDaAudioOutputConfig().mTtsEnabled ? S_OK : S_FALSE;

	if	(pbEnabled)
	{
		(*pbEnabled) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrAudioOutputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentAudioOutputProperties::XAudioOutputProperties::GetEnabled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentAudioOutputProperties::XAudioOutputProperties::GetUsingSoundEffects (long *pbUsingSoundEffects)
{
	METHOD_PROLOGUE(CDaAgentAudioOutputProperties, AudioOutputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentAudioOutputProperties::XAudioOutputProperties::GetUsingSoundEffects"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = CDaAudioOutputConfig().mEffectsEnabled ? S_OK : S_FALSE;

	if	(pbUsingSoundEffects)
	{
		(*pbUsingSoundEffects) = (lResult == S_OK);
	}

	PutServerError (lResult, __uuidof(IDaSvrAudioOutputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentAudioOutputProperties::XAudioOutputProperties::GetUsingSoundEffects"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentAudioOutputProperties::XAudioOutputProperties::GetStatus (long *plStatus)
{
	METHOD_PROLOGUE(CDaAgentAudioOutputProperties, AudioOutputProperties)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAgentAudioOutputProperties::XAudioOutputProperties::GetStatus"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!plStatus)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*plStatus) = pThis->GetAudioStatus ();
	}

	PutServerError (lResult, __uuidof(IDaSvrAudioOutputProperties));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAgentAudioOutputProperties::XAudioOutputProperties::GetStatus"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentAudioOutputProperties::DspGetEnabled(long * Enabled)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentAudioOutputProperties::DspGetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAudioOutputProperties.GetEnabled (Enabled);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentAudioOutputProperties::DspGetUsingSoundEffects(long * UsingSoundEffects)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentAudioOutputProperties::DspGetUsingSoundEffects"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAudioOutputProperties.GetUsingSoundEffects (UsingSoundEffects);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentAudioOutputProperties::DspGetStatus(long * Status)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentAudioOutputProperties::DspGetStatus"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xAudioOutputProperties.GetStatus (Status);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
