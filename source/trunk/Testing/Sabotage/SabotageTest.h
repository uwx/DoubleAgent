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

	INT_PTR RunDlg (class CSabotageTestDlg & pDlg);
};

/////////////////////////////////////////////////////////////////////////////
