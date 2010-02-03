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
#ifndef DACORE_H_INCLUDED_
#define DACORE_H_INCLUDED_
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "DaCoreRes.h"
#include "DaCoreExp.h"
#include "AgentFileCache.h"
#include "SapiVoiceCache.h"
#include "SapiInputCache.h"
#include "AgtErr.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef	_STRICT_COMPATIBILITY
#define	_STRICT_COMPATIBILITY
#endif
/////////////////////////////////////////////////////////////////////////////

class CDaCoreApp : public CWinApp, public CAgentFileCache, public CSapiVoiceCache, public CSapiInputCache
{
public:
	CDaCoreApp();
	~CDaCoreApp();
	DECLARE_DYNAMIC(CDaCoreApp)

// Operations
	static void _DACORE_IMPEXP InitLogging (LPCTSTR pLogFileName = NULL, UINT pLogLevel = 0);

// Overrides
	//{{AFX_VIRTUAL(CDaCoreApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDaCoreApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#define TheCoreApp ((CDaCoreApp *) AfxGetApp ())

/////////////////////////////////////////////////////////////////////////////

static inline CString EncodeTraceString (LPCTSTR pString)
{
	CString	lString (pString);
	lString.Replace (_T("\r"), _T("%0D"));
	lString.Replace (_T("\n"), _T("%0A"));
	lString.Replace (_T("\t"), _T("%09"));
	lString.Replace (_T("\f"), _T("%0C"));
	return lString;
}

static inline CString DecodeTraceString (LPCTSTR pString)
{
	CString	lString (pString);
	lString.Replace (_T("%0D"), _T("\r"));
	lString.Replace (_T("%0A"), _T("\n"));
	lString.Replace (_T("%09"), _T("\t"));
	lString.Replace (_T("%0C"), _T("\f"));
	return lString;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACORE_H_INCLUDED_
