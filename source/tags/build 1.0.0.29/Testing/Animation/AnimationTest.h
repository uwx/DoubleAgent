#ifndef ANIMATIONTEST_H_INCLUDED_
#define ANIMATIONTEST_H_INCLUDED_
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // ANIMATIONTEST_H_INCLUDED_
