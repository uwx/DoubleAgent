#ifndef STRESSTEST_H_INCLUDED_
#define STRESSTEST_H_INCLUDED_
#pragma once

#include "StressTestRes.h"

/////////////////////////////////////////////////////////////////////////////

class CStressTestApp : public CWinApp
{
public:
	CStressTestApp();
	~CStressTestApp();

// Overrides
	//{{AFX_VIRTUAL(CStressTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CStressTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // STRESSTEST_H_INCLUDED_
