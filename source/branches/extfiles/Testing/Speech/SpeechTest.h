#ifndef SPEECHTEST_H_INCLUDED_
#define SPEECHTEST_H_INCLUDED_
#pragma once

#include "SpeechTestRes.h"

/////////////////////////////////////////////////////////////////////////////

class CSpeechTestApp : public CWinApp
{
public:
	CSpeechTestApp();
	~CSpeechTestApp();

// Overrides
	//{{AFX_VIRTUAL(CSpeechTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSpeechTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // SPEECHTEST_H_INCLUDED_
