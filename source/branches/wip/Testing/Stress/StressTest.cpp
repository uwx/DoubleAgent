#include "StdAfx.h"
#include "DaGuid.h"
#include "StressTest.h"
#include "StressTestDlg.h"
#include "WerOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define _LOG_LEVEL_DEBUG		LogNormal
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T("StressTest")
#define _LOG_DEF_LOGNAME		_T("StressTest.log")
#include "Log.inl"
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
	SetRegistryKeyEx (_T("Double Agent"), _LOG_SECTION_NAME);
	LogStart (true);
}

CStressTestApp::~CStressTestApp()
{
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CStressTestApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);
	WerOptOut (false);

	CStressTestDlg	lDlg;
	INT_PTR			lResult;

	m_pMainWnd = &lDlg;
	lResult = lDlg.DoModal();
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
