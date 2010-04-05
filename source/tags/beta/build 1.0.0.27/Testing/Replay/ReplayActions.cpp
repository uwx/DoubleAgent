#include "StdAfx.h"
#include "ReplayActions.h"
#include "ReplayActionsDlg.h"
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
#define	_LOG_SECTION_NAME		_T("ReplayActions")
#define _LOG_DEF_LOGNAME		_T("ReplayActions.log")
static tPtr <CCriticalSection>	sLogCriticalSection = new CCriticalSection;
#define	_LOG_CRITICAL_SECTION	(!sLogCriticalSection?NULL:(CRITICAL_SECTION*)(*sLogCriticalSection))
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CReplayActionsApp, CWinApp)
	//{{AFX_MSG_MAP(CReplayActionsApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CReplayActionsApp gApp;

CReplayActionsApp::CReplayActionsApp()
:	CWinApp (_LOG_SECTION_NAME)
{
	SetRegistryKeyEx (_T("Double Agent"), _LOG_SECTION_NAME);
	LogStart (true);
	CDaCoreApp::InitLogging (gLogFileName, gLogLevel);
}

CReplayActionsApp::~CReplayActionsApp()
{
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CReplayActionsApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);
	WerOptOut (false);

	CCommandLineInfo	lCmdLine;
	CReplayActionsDlg	lDlg;
	INT_PTR				lResult;

	ParseCommandLine (lCmdLine);
	if	(lCmdLine.m_nShellCommand == lCmdLine.FileOpen)
	{
		lDlg.mDefaultFileName = lCmdLine.m_strFileName;
	}

	m_pMainWnd = &lDlg;
	lResult = lDlg.DoModal();
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
