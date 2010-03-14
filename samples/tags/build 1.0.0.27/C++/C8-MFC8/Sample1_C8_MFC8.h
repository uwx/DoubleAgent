// Sample1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "Sample1Res.h"		// main symbols


// CSample1App:
// See Sample1.cpp for the implementation of this class
//

class CSample1App : public CWinApp
{
public:
	CSample1App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSample1App theApp;