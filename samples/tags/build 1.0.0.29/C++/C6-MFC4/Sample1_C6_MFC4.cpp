#include "StdAfx.h"
#include "Sample1_C6_MFC4.h"
#include "Sample1Dlg_C6_MFC4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSample1App, CWinApp)
	//{{AFX_MSG_MAP(CSample1App)
	//}}AFX_MSG
END_MESSAGE_MAP()

CSample1App gApp;

/////////////////////////////////////////////////////////////////////////////

CSample1App::CSample1App()
{
}

CSample1App::~CSample1App()
{
}

/////////////////////////////////////////////////////////////////////////////

BOOL CSample1App::InitInstance()
{
	AfxEnableControlContainer();
	OleInitialize (NULL);

	CSample1Dlg	lDlg;
	INT_PTR		lResult;

	m_pMainWnd = &lDlg;
	lResult = lDlg.DoModal();
	m_pMainWnd = NULL;

	OleUninitialize ();
	return FALSE;
}
