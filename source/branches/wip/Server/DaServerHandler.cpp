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
#include "DaHandler.h"
#include "DaServerHandler.h"
#include "Registry.h"
#include "GuidStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Handler"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Handler"),LogVerbose,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

CDaServerHandler::CDaServerHandler ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::CDaServerHandler (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

CDaServerHandler::~CDaServerHandler ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::~CDaServerHandler (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaServerHandler::FinalConstruct ()
{
	HRESULT	lResult;

	lResult = CoGetStdMarshalEx (m_pOuterUnknown, SMEXF_HANDLER, &mProxyManager);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::FinalConstruct Identity [%p] Manager [%p]"), this, m_dwRef, m_pOuterUnknown, mProxyManager.GetInterfacePtr());
	}
#endif
	return lResult;
}

void CDaServerHandler::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::FinalRelease [%p] [%p]"), this, m_dwRef, m_pOuterUnknown, mProxyManager.GetInterfacePtr());
	}
#endif
	SafeFreeSafePtr (mProxyManager);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaServerHandler::_InternalQueryInterface (REFIID iid, void** ppvObject)
{
	HRESULT		lResult = E_NOINTERFACE;

	if	(mProxyManager != NULL)
	{
		lResult = mProxyManager->QueryInterface (iid, ppvObject);
	}
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_INTERFACE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::QueryInterface [%p] [%s]]"), this, m_dwRef, *ppvObject, CGuidStr::GuidName(iid));
	}
#endif
	return lResult;
}

//LPUNKNOWN CDaServerHandler::GetInterfaceHook(const void* iid)
//{
//	LPUNKNOWN	lUnknown = NULL;
//	HRESULT		lResult = S_FALSE;
//
//	if	(mProxyManager != NULL)
//	{
//		if	(SUCCEEDED (lResult = mProxyManager->QueryInterface (*(const IID *)iid, (void**)&lUnknown)))
//		{
//			ExternalRelease ();
//		}
//	}
//#ifdef	_DEBUG_INTERFACE
//	if	(LogIsActive (_DEBUG_INTERFACE))
//	{
//		LogComErrAnon (MinLogLevel(_DEBUG_INTERFACE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::QueryInterface [%p] [%s]]"), this, m_dwRef, lUnknown, CGuidStr::GuidName(*(GUID*)iid));
//	}
//#endif
//	return lUnknown;
//}
