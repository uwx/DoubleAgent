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
