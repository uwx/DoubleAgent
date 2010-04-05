/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef PROPSHEETOPTIONS_H_INCLUDED_
#define PROPSHEETOPTIONS_H_INCLUDED_
#pragma once

#include "PropSheetBase.h"

/////////////////////////////////////////////////////////////////////////////

class CPropSheetOptions : public CPropSheetBase
{
public:
	CPropSheetOptions(CWnd* pParentWnd = NULL);
	virtual ~CPropSheetOptions();
	DECLARE_DYNAMIC(CPropSheetOptions)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CPropSheetOptions)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void LoadConfig ();
	virtual void SaveConfig (int pSheetResult);
	virtual void OnApplied ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropSheetOptions)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PROPSHEETOPTIONS_H_INCLUDED_
