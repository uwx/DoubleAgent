#include "StdAfx.h"
#include "DaGuid.h"
#include "AnimationTest.h"
#include "AnimationTestDlg.h"
#include "DaCore.h"
#include "WerOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define _LOG_LEVEL_DEBUG		LogNormal
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T("AnimationTest")
#define _LOG_DEF_LOGNAME		_T("AnimationTest.log")
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAnimationTestApp, CWinApp)
	//{{AFX_MSG_MAP(CAnimationTestApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CAnimationTestApp gApp;

CAnimationTestApp::CAnimationTestApp()
:	CWinApp (_LOG_SECTION_NAME)
{
	SetRegistryKeyEx (_T("Double Agent"), _LOG_SECTION_NAME);
	LogStart (true);
	CDaCoreApp::InitLogging (gLogFileName, gLogLevel);
}

CAnimationTestApp::~CAnimationTestApp()
{
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CAnimationTestApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);
	WerOptOut (false);

	CAnimationTestDlg	lDlg;
	INT_PTR				lResult;

	m_pMainWnd = &lDlg;
	lResult = lDlg.DoModal();
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
