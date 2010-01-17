/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include <shlwapi.h>
#ifdef	_DEBUG
#define	_LOCALIZE_BY_THREAD
#endif
//////////////////////////////////////////////////////////////////////
static CString _DaMakeMuiPath (LPCTSTR pResModulePath, LPCTSTR pMuiPath, LANGID pLangId, UINT pPathNum)
{
	CString	lMuiPath;
	CString	lBasePath;
	CString	lFileName;

	lFileName.Format (_T("Agt%4.4hX.dll"), pLangId);

	switch (pPathNum)
	{
		case 0:
		{
			lBasePath = pResModulePath;
			PathRemoveFileSpec (lBasePath.GetBuffer (MAX_PATH));
			PathAppend (lBasePath.GetBuffer (MAX_PATH), _T("intl"));
			lBasePath.ReleaseBuffer ();
		}	break;
		case 1:
		{
			GetWindowsDirectory (lBasePath.GetBuffer (MAX_PATH), MAX_PATH);
			PathAppend (lBasePath.GetBuffer (MAX_PATH), _T("MsAgent\\intl"));
			lBasePath.ReleaseBuffer ();
		}	break;
		case 2:
		{
			lBasePath = pResModulePath;
			PathRemoveFileSpec (lBasePath.GetBuffer (MAX_PATH));
			lBasePath.ReleaseBuffer ();
		}	break;
	}
	if	(!lBasePath.IsEmpty ())
	{
		PathCombine (lMuiPath.GetBuffer (MAX_PATH), lBasePath, lFileName);
		lMuiPath.ReleaseBuffer ();
	}

	return lMuiPath;
}

#define	MakeMuiPath _DaMakeMuiPath
//////////////////////////////////////////////////////////////////////
#include "LocalizeMui.cpp"
