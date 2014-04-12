/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
[assembly:AssemblyTitleAttribute("DaCharFileCLR")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute(_DOUBLEAGENT_COMPANY)];
[assembly:AssemblyProductAttribute(_DOUBLEAGENT_NAME)];
[assembly:AssemblyCopyrightAttribute(_DOUBLEAGENT_COPYRIGHT)];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

[assembly:AssemblyKeyFileAttribute("Managed\\CinSoft.snk")]
[assembly:AssemblyDelaySignAttribute(true)]

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
// You can specify all the value or you can default the Revision and Build Numbers
// by using the '*' as shown below:

[assembly:AssemblyVersionAttribute(_DOUBLEAGENT_VERSION_STR)];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME	_T(_CORE_REGNAME)
#define _LOG_DEF_LOGNAME	_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX			_T("CharFileCLR ")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////
