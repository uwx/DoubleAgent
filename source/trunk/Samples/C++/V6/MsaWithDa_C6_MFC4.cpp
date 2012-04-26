#include "StdAfx.h"
#include "MsaWithDa_C6_MFC4.h"
#include "MsaWithDaDlg_C6_MFC4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMsaWithDaApp, CWinApp)
	//{{AFX_MSG_MAP(CMsaWithDaApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

CMsaWithDaApp gApp;

/////////////////////////////////////////////////////////////////////////////

CMsaWithDaApp::CMsaWithDaApp()
{
}

CMsaWithDaApp::~CMsaWithDaApp()
{
}

/////////////////////////////////////////////////////////////////////////////

BOOL CMsaWithDaApp::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);

	CMsaWithDaDlg	lDlg;
	INT_PTR		lResult;

	m_pMainWnd = &lDlg;
	lResult = lDlg.DoModal();
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
