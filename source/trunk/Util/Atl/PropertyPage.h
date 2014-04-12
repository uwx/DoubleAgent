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
#ifndef PROPERTYPAGE_H_INCLUDED_
#define PROPERTYPAGE_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CAtlPropertyPage : public CDialogImpl<CAtlPropertyPage>
{
public:
	CAtlPropertyPage (UINT pTemplateId);
	virtual ~CAtlPropertyPage();

// Attributes
public:
	tSS <PROPSHEETPAGE, DWORD>	mPsp;

// Operations
public:

// Implementation
protected:
	LRESULT HandleInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAtlPropertyPage)
		MESSAGE_HANDLER(WM_INITDIALOG, HandleInitDialog)
	END_MSG_MAP()

	virtual BOOL OnInitDialog () {return TRUE;}
	void SetModified (BOOL pModified = TRUE) {PropSheet_Changed (::GetParent(m_hWnd), m_hWnd);}

private:
	static UINT CALLBACK PropPageCallback (HWND hwnd, UINT message, PROPSHEETPAGE* pPropPage);

protected:
	CString	mCaption;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPERTYPAGE_H_INCLUDED_
