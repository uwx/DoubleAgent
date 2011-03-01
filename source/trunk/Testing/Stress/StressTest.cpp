#include "StdAfx.h"
#include "DaGuid.h"
#include "DaVersion.h"
#include "StressTest.h"
#include "StressTestDlg.h"
#include "OleMessageFilterEx.h"
#include "WerOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable: 4722)

/////////////////////////////////////////////////////////////////////////////
#define _LOG_LEVEL_DEBUG		LogNormal
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T("StressTest")
#define _LOG_DEF_LOGNAME		_T("StressTest.log")
#include "Log.inl"
#include "LogCrash.inl"
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CStressTestApp, CWinApp)
	//{{AFX_MSG_MAP(CStressTestApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CStressTestApp gApp;

CStressTestApp::CStressTestApp()
:	CWinApp (_LOG_SECTION_NAME)
{
	LogCrash_Initialize ();
	SetRegistryKeyEx (_T("Double Agent"), _LOG_SECTION_NAME);
	LogStart (true);
}

CStressTestApp::~CStressTestApp()
{
	LogStop (LogIfActive);
	LogCrash_Terminate ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

INT_PTR CStressTestApp::RunDlg (CStressTestDlg & pDlg)
{
	INT_PTR	lResult = IDCANCEL;

	__try
	{
		lResult = pDlg.DoModal();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation()))
	{}

	return lResult;
}

void CStressTestApp::_ExitInstance()
{
	CWinApp::ExitInstance();
	CoFreeUnusedLibraries ();
	OleUninitialize ();
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CStressTestApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);
	WerOptOut (false);

#if	TRUE
	if	(m_pMessageFilter)
	{
		delete m_pMessageFilter;
	}
	m_pMessageFilter = new COleMessageFilterEx (60000);
#endif

	CStressTestDlg	lDlg;
	m_pMainWnd = &lDlg;
	RunDlg (lDlg);
	m_pMainWnd = NULL;

	return FALSE;
}

int CStressTestApp::ExitInstance()
{
	__try
	{
		_ExitInstance ();
	}
	__except (LogCrash (GetExceptionCode(), GetExceptionInformation()))
	{}
	return 0;
};