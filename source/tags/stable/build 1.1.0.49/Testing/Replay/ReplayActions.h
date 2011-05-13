#pragma once
#include "ReplayActionsRes.h"

/////////////////////////////////////////////////////////////////////////////

class CReplayActionsApp : public CWinApp
{
public:
	CReplayActionsApp ();
	~CReplayActionsApp ();

// Overrides
	//{{AFX_VIRTUAL(CReplayActionsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CReplayActionsApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
