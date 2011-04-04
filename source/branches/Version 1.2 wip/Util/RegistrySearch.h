/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef REGISTRYSEARCH_H_INCLUDED_
#define REGISTRYSEARCH_H_INCLUDED_
#pragma once

#include "Registry.h"

/////////////////////////////////////////////////////////////////////////////

class CRegistrySearch
{
// Operations
public:
	static bool GetGlobalClassesKey (CRegKeyEx& pGlobalClasses, bool pAltPlatform = false);
	static bool GetUserClassesKey (CRegKeyEx& pUserClasses, bool pAltPlatform = false);
	static bool GetClassesRootKey (CRegKeyEx& pClassesRoot, bool pAltPlatform = false);
	static void GetRootKeys (CRegKeyEx& pGlobalClasses, CRegKeyEx& pUserClasses, CRegKeyEx& pClassesRoot, bool pAltPlatform = false);

	static CString GetClassProgId (REFGUID pClassId, HKEY pRootKey);
	static CString GetClassViProgId (REFGUID pClassId, HKEY pRootKey);
	static CString GetServerPath (REFGUID pClassId, HKEY pRootKey);
	static CString GetServerPath (LPCTSTR pProgId, HKEY pRootKey);

	static CString GetAltTypeLibPath (UINT pTypeLibNum);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // REGISTRYSEARCH_H_INCLUDED_
