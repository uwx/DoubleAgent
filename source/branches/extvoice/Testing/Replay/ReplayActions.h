#ifndef REPLAYACTIONS_H_INCLUDED_
#define REPLAYACTIONS_H_INCLUDED_
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // REPLAYACTIONS_H_INCLUDED_
