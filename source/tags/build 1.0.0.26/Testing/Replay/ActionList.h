#ifndef ACTIONLIST_H_INCLUDED_
#define ACTIONLIST_H_INCLUDED_
#pragma once

#include "ActionFile.h"

/////////////////////////////////////////////////////////////////////////////

class CActionList : public CListCtrl
{
public:
	CActionList();
	virtual ~CActionList();

// Attributes
public:

// Operations
public:
	int ShowAction (CActionLine * pActionLine);

// Overrides
	//{{AFX_VIRTUAL(CActionList)
	virtual void PreSubclassWindow ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CActionList)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // ACTIONLIST_H_INCLUDED_
