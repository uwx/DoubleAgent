#include "StdAfx.h"
#include "DaVersion.h"

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
[assembly:AssemblyTitleAttribute("TlbToAsm")];
[assembly:AssemblyDescriptionAttribute("Import TypeLib to Assembly")];
[assembly:AssemblyCompanyAttribute(_DOUBLEAGENT_COMPANY)];
[assembly:AssemblyProductAttribute(_DOUBLEAGENT_NAME)];
[assembly:AssemblyCopyrightAttribute(_DOUBLEAGENT_COPYRIGHT)];

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
