// AboutBox.h : header file
//

#pragma once

#include <comutil.h>
#include "CDaControl.h"
#include "afxwin.h"

// CAboutBox dialog
class CAboutBox : public CDialog
{
// Construction
public:
	CAboutBox(CDaControl & pDaControl, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ABOUTBOX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:

	DECLARE_MESSAGE_MAP()

	CDaControl & mDaControl;
	
	afx_msg void OnDaMore();
};
