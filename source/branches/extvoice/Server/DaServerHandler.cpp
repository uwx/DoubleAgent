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
#include "GuidStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM		LogNormal
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Handler"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Handler"),LogVerbose,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaServerHandler, CCmdTarget)
	INTERFACE_PART(CDaServerHandler, __uuidof(IUnknown), InnerUnknown)
END_INTERFACE_MAP()

IMPLEMENT_DYNCREATE(CDaServerHandler, CCmdTarget)
IMPLEMENT_OLECREATE_EX(CDaServerHandler, "", 0x1147E518, 0xA208, 0x11DE, 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2)

BOOL CDaServerHandler::CDaServerHandlerFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_SERVER_HANDLER_NAME)))
	{
		if	(bRegister)
		{
			CString	lModuleName;

			GetModuleFileName (AfxGetInstanceHandle (), lModuleName.GetBuffer (MAX_PATH), MAX_PATH);
			lModuleName.ReleaseBuffer ();
			RegisterServer (true, lModuleName);
			RegisterBothThreaded (true, lModuleName);
			RegisterDefCategory ();
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaServerHandler::CDaServerHandler ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::CDaServerHandler (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();
	EnableAggregation();
}

CDaServerHandler::~CDaServerHandler ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::~CDaServerHandler (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleUnlockApp();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::~CDaServerHandler (%d) Done"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerHandler::OnFinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaServerHandler::OnFinalRelease [%p] [%p]"), this, m_dwRef, m_pOuterUnknown, mProxyManager.GetInterfacePtr());
	}
#endif
	SafeFreeSafePtr (mProxyManager);
	CCmdTarget::OnFinalRelease();
}

BOOL CDaServerHandler::OnCreateAggregates ()
{
	HRESULT	lResult;

	lResult = CoGetStdMarshalEx (m_pOuterUnknown, SMEXF_HANDLER, &mProxyManager);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] CoGetStdMarshalEx Identity [%p] Manager [%p]"), this, m_dwRef, m_pOuterUnknown, mProxyManager.GetInterfacePtr());
	}
#endif
	return (SUCCEEDED (lResult));
}

/////////////////////////////////////////////////////////////////////////////

LPUNKNOWN CDaServerHandler::GetInterfaceHook(const void* iid)
{
	LPUNKNOWN	lUnknown = NULL;
	HRESULT		lResult = S_FALSE;

	if	(mProxyManager != NULL)
	{
		if	(SUCCEEDED (lResult = mProxyManager->QueryInterface (*(const IID *)iid, (void**)&lUnknown)))
		{
			ExternalRelease ();
		}
	}
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_INTERFACE,LogAlways), lResult, _T("[%p(%d)] CDaServerHandler::QueryInterface [%p] [%s]]"), this, m_dwRef, lUnknown, CGuidStr::GuidName(*(GUID*)iid));
	}
#endif
	return lUnknown;
}
