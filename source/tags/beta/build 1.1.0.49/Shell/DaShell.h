/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#pragma once
#include "DaShellRes.h"
#include "DaGuid.h"
#include "DaVersion.h"
#include "ThemedModule.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E564-A208-11DE-ABF2-002421116FB2}")) CDaShellModule : public CAtlDllModuleT <CDaShellModule>, public CThemedModule
{
public:
	CDaShellModule ();
	virtual ~CDaShellModule ();
	DECLARE_LIBID(GUID_NULL)

// Attributes
public:
	tPtr <class CPropSheetCpl>	mCplPropSheet;
	CAtlString					mCplStartPage;

// Operations
	static void RegisterCpl ();
	static void UnregisterCpl ();
};

extern CDaShellModule _AtlModule;

/////////////////////////////////////////////////////////////////////////////
