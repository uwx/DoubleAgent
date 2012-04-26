#include "StdAfx.h"
#include "DaGuid.h"
#include "DaVersion.h"
#include "SpeechTest.h"
#include "SpeechTestDlg.h"
#include "WerOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define _LOG_LEVEL_DEBUG		LogNormal
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T("SpeechTest")
#define _LOG_DEF_LOGNAME		_T("SpeechTest.log")
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSpeechTestApp, CWinApp)
	//{{AFX_MSG_MAP(CSpeechTestApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CSpeechTestApp gApp;

CSpeechTestApp::CSpeechTestApp()
:	CWinApp (_LOG_SECTION_NAME)
{
	SetRegistryKeyEx (_T("Double Agent"), _LOG_SECTION_NAME);
	LogStart (true);
}

CSpeechTestApp::~CSpeechTestApp()
{
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CSpeechTestApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);
	WerOptOut (false);

	CSpeechTestDlg	lDlg;
	INT_PTR				lResult;

	m_pMainWnd = &lDlg;
	lResult = lDlg.DoModal();
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
