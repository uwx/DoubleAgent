// MsaWithDa.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "MsaWithDaRes.h"		// main symbols


// CMsaWithDaApp:
// See MsaWithDa.cpp for the implementation of this class
//

class CMsaWithDaApp : public CWinApp
{
public:
	CMsaWithDaApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMsaWithDaApp theApp;
