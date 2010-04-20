#if !defined(AFX_MSAWITHDA_H__796061B9_5A9B_420B_B97E_4E97874841D4__INCLUDED_)
#define AFX_MSAWITHDA_H__796061B9_5A9B_420B_B97E_4E97874841D4__INCLUDED_
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "MsaWithDaRes.h"

/////////////////////////////////////////////////////////////////////////////

class CMsaWithDaApp : public CWinApp
{
public:
	CMsaWithDaApp();
	~CMsaWithDaApp();

// Overrides
	//{{AFX_VIRTUAL(CMsaWithDaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMsaWithDaApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSAWITHDA_H__796061B9_5A9B_420B_B97E_4E97874841D4__INCLUDED_)
