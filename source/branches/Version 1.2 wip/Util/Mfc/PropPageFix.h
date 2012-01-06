/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
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
#if !defined(AFX_PROPPAGEFIX_H__1FC23952_8A42_43B0_82D5_E33671AD1BE7__INCLUDED_)
#define AFX_PROPPAGEFIX_H__1FC23952_8A42_43B0_82D5_E33671AD1BE7__INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CPropPageFix
{
public:
	CPropPageFix ();
	virtual ~CPropPageFix ();

	LPCDLGTEMPLATE GetWritableTemplate (UINT pTemplateId, WORD pLangId = LANG_USER_DEFAULT);
	static CString GetTemplateCaption (LPCDLGTEMPLATE pTemplate);
	LPCDLGTEMPLATE SetTemplateCaption (LPCDLGTEMPLATE pTemplate, LPCTSTR pCaption);

protected:
	HGLOBAL	mWritableTemplate;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEFIX_H__1FC23952_8A42_43B0_82D5_E33671AD1BE7__INCLUDED_)
