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
