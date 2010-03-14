#ifndef SABOTAGETEST_H_INCLUDED_
#define SABOTAGETEST_H_INCLUDED_
#pragma once

#include "SabotageTestRes.h"

/////////////////////////////////////////////////////////////////////////////

class CSabotageTestApp : public CWinApp
{
public:
	CSabotageTestApp();
	~CSabotageTestApp();

// Overrides
	//{{AFX_VIRTUAL(CSabotageTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSabotageTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // SABOTAGETEST_H_INCLUDED_
