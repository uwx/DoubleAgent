// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "Sample1_C8_MFC8.h"
#include "AboutBox.h"
#include "CDaCtlPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutBox dialog

CAboutBox::CAboutBox(CDaControl & pDaControl, CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD, pParent),
	mDaControl (pDaControl)
{
}

void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAboutBox)
	ON_BN_CLICKED(ID_DA_MORE, &CAboutBox::OnDaMore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CAboutBox::OnDaMore()
{
	CDaCtlPropertySheet lPropSheet;

	lPropSheet.AttachDispatch (mDaControl.get_PropertySheet());
	lPropSheet.put_Visible (TRUE);	
	lPropSheet.put_Page (_T("Copyright"));
}
