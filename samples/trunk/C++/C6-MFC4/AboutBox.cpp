// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "MsaWithDa_C6_MFC4.h"
#include "DoubleAgentCtl.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog


CAboutBox::CAboutBox(IDaControl2 & pDaControl, CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD, pParent),
	mDaControl (pDaControl)
{
	//{{AFX_DATA_INIT(CAboutBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAboutBox)
	ON_BN_CLICKED(ID_DA_MORE, OnDaMore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBox message handlers

void CAboutBox::OnDaMore() 
{
	IDaCtlPropertySheet2 lPropSheet;

	lPropSheet.AttachDispatch (mDaControl.GetPropertySheet());
	lPropSheet.SetVisible (TRUE);	
	lPropSheet.SetPage (_T("Copyright"));
}
