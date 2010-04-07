#pragma once
#include "AnimationTestRes.h"

/////////////////////////////////////////////////////////////////////////////

class CAnimationTestApp : public CWinApp
{
public:
	CAnimationTestApp();
	~CAnimationTestApp();

// Overrides
	//{{AFX_VIRTUAL(CAnimationTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAnimationTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
