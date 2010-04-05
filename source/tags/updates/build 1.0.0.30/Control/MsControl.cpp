/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "MsControl.h"
#include "Registry.h"
#include "RegistrySearch.h"
#include "GuidStr.h"
#include "UserSecurity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG		LogNormal
#endif
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T(_CONTROL_REGNAME)
#define _LOG_DEF_LOGNAME		_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX				_T("Stub ")
#include "LogAccess.inl"
#include "Log.inl"

////////////////////////////////////////////////////////////////////////////

CMsControlApp gApp;

IMPLEMENT_DYNAMIC(CMsControlApp, CWinApp);

CMsControlApp::CMsControlApp ()
:	CWinApp (_T(_DOUBLEAGENT_NAME))
{
	SetRegistryKeyEx (_T(_DOUBLEAGENT_NAME), _T(_CONTROL_REGNAME));
	LogStart (false);
}

CMsControlApp::~CMsControlApp ()
{
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static const GUID sMsTypeLibId = __uuidof(AgentControlTypeLib);
static const WORD sMsTypeLibVerMajor = 2;
static const WORD sMsTypeLibVerMinor = 0;

/////////////////////////////////////////////////////////////////////////////

STDAPI DllRegisterServer(void)
{
	HRESULT	lResult = S_FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LogStart (false);
	if	(IsWindows7_AtLeast ())
	{
		if	(CUserSecurity::IsUserAdministrator ())
		{
			if	(
					(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
				&&	(!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), sMsTypeLibId))
				)
			{
				lResult = SELFREG_E_TYPELIB;
			}
		}
		else
		{
			lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
		}
	}
	return lResult;
}


/////////////////////////////////////////////////////////////////////////////

STDAPI DllUnregisterServer(void)
{
	HRESULT	lResult = S_FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LogStart (false);
	if	(IsWindows7_AtLeast ())
	{
		if	(CUserSecurity::IsUserAdministrator ())
		{
			if	(
					(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
				&&	(!AfxOleUnregisterTypeLib(sMsTypeLibId, sMsTypeLibVerMajor, sMsTypeLibVerMinor))
				)
			{
				lResult = SELFREG_E_TYPELIB;
			}
		}
		else
		{
			lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
		}
	}
	return lResult;
}
