#include "StdAfx.h"
#include "DaGuid.h"
#include "DaVersion.h"
#include "SabotageTest.h"
#include "SabotageTestDlg.h"
#include "WerOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define _LOG_LEVEL_DEBUG		LogNormal
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T("SabotageTest")
#define _LOG_DEF_LOGNAME		_T("SabotageTest.log")
#define	_LOG_PREFIX				_T("Sabot   ")
#include "LogAccess.inl"
#include "Log.inl"
#include "LogCrash.inl"
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSabotageTestApp, CWinApp)
	//{{AFX_MSG_MAP(CSabotageTestApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CSabotageTestApp gApp;

CSabotageTestApp::CSabotageTestApp()
:	CWinApp (_LOG_SECTION_NAME)
{
	LogCrash_Initialize ();
	SetRegistryKeyEx (_T("Double Agent"), _LOG_SECTION_NAME);
	LogStart (false);
}

CSabotageTestApp::~CSabotageTestApp()
{
	LogStop (LogIfActive);
	LogCrash_Terminate ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

INT_PTR CSabotageTestApp::RunDlg (CSabotageTestDlg & pDlg)
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

BOOL CSabotageTestApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);
	WerOptOut (false);

	CSabotageTestDlg lDlg;
	m_pMainWnd = &lDlg;
	RunDlg (lDlg);
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
