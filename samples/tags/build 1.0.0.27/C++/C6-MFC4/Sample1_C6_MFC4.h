#if !defined(AFX_SAMPLE1_H__796061B9_5A9B_420B_B97E_4E97874841D4__INCLUDED_)
#define AFX_SAMPLE1_H__796061B9_5A9B_420B_B97E_4E97874841D4__INCLUDED_
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Sample1Res.h"

/////////////////////////////////////////////////////////////////////////////

class CSample1App : public CWinApp
{
public:
	CSample1App();
	~CSample1App();

// Overrides
	//{{AFX_VIRTUAL(CSample1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSample1App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLE1_H__796061B9_5A9B_420B_B97E_4E97874841D4__INCLUDED_)
