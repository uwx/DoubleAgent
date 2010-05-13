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
#include "UserSecurity.h"
#include "DaGuid.h"
#include "DaVersion.h"

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
/////////////////////////////////////////////////////////////////////////////

CMsControlModule				_AtlModule;
LPCTSTR __declspec(selectany)	_AtlProfileName = _LOG_SECTION_NAME;
LPCTSTR __declspec(selectany)	_AtlProfilePath = _LOG_ROOT_PATH;

/////////////////////////////////////////////////////////////////////////////

STDAPI DllRegisterServer(void)
{
	HRESULT	lResult = S_FALSE;

	LogStart (false);
	if	(IsWindows7_AtLeast ())
	{
		if	(CUserSecurity::IsUserAdministrator ())
		{
			if	(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
			{
			    lResult = AtlRegisterTypeLib (_AtlBaseModule.GetModuleInstance(), NULL);
			}
		}
		else
		{
			lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
		}
	}
	return lResult;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT	lResult = S_FALSE;

	LogStart (false);
	if	(IsWindows7_AtLeast ())
	{
		if	(CUserSecurity::IsUserAdministrator ())
		{
			if	(CRegistrySearch::GetClassViProgId (__uuidof(AgentControl), HKEY_CLASSES_ROOT).IsEmpty())
			{
				lResult = AtlUnRegisterTypeLib (_AtlBaseModule.GetModuleInstance(), NULL);
			}
		}
		else
		{
			lResult = HRESULT_FROM_WIN32 (ERROR_ELEVATION_REQUIRED);
		}
	}
	return lResult;
}
