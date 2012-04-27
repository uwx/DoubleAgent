/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "DaHandler.h"
#include "DaServerHandler.h"
#include "Registry.h"
#include "GuidStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Handler"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Handler"),LogVerbose,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

CDaServerHandler::CDaServerHandler ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::CDaServerHandler (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

CDaServerHandler::~CDaServerHandler ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::~CDaServerHandler (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaServerHandler::FinalConstruct ()
{
	HRESULT	lResult;

	lResult = CoGetStdMarshalEx (m_pOuterUnknown, SMEXF_HANDLER, &mProxyManager);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::FinalConstruct Identity [%p] Manager [%p]"), this, max(m_dwRef,-1), m_pOuterUnknown, mProxyManager.GetInterfacePtr());
	}
#endif
	return lResult;
}

void CDaServerHandler::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::FinalRelease [%p] [%p]"), this, max(m_dwRef,-1), m_pOuterUnknown, mProxyManager.GetInterfacePtr());
	}
#endif
	SafeFreeSafePtr (mProxyManager);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI CDaServerHandler::DelegateInterface (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	CDaServerHandler *	lHandler = (CDaServerHandler *)pv;
	HRESULT				lResult = E_NOINTERFACE;

#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaServerHandler::QueryInterface [%p] [%s]"), lHandler, lHandler->m_dwRef, lHandler->mProxyManager.GetInterfacePtr(), CGuidStr::GuidName(iid));
	}
#endif

	if	(lHandler->mProxyManager != NULL)
	{
		lResult = lHandler->mProxyManager->QueryInterface (iid, ppvObject);
	}

#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_INTERFACE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::QueryInterface [%p] [%s]"), lHandler, lHandler->m_dwRef, *ppvObject, CGuidStr::GuidName(iid));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaServerHandler::QueryMultipleInterfaces (ULONG cMQIs, MULTI_QI *pMQIs)
{
	HRESULT	lResult = E_NOINTERFACE;
	ULONG	lNdx;

#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaServerHandler::QueryMultipleInterfaces [%u]"), this, max(m_dwRef,-1), cMQIs);
	}
#endif

	if	(!pMQIs)
	{
		lResult = E_POINTER;
	}
	else
	{
		for	(lNdx = 0; lNdx < cMQIs; lNdx++)
		{
			if	(pMQIs[lNdx].pIID)
			{
				pMQIs[lNdx].hr = DelegateInterface (this, *(pMQIs[lNdx].pIID), (LPVOID*)&pMQIs[lNdx].pItf, 0);
			}
			else
			{
				lResult = E_INVALIDARG;
			}
			if	(SUCCEEDED (pMQIs[lNdx].hr))
			{
				if	(lResult == E_POINTER)
				{
					lResult = S_OK;
				}
			}
			else
			{
				if	(lResult == S_OK)
				{
					lResult = S_FALSE;
				}
			}
		}
	}

#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_INTERFACE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::QueryMultipleInterfaces [%u]"), this, max(m_dwRef,-1), cMQIs);
	}
#endif
	return lResult;
}
