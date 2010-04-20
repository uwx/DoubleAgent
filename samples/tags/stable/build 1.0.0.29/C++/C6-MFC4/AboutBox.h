#if !defined(AFX_ABOUTBOX_H__EC6C49AC_2E28_4F81_BE5B_9FF1DA0D73D8__INCLUDED_)
#define AFX_ABOUTBOX_H__EC6C49AC_2E28_4F81_BE5B_9FF1DA0D73D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog

class CAboutBox : public CDialog
{
// Construction
public:
	CAboutBox(IDaControl & pDaControl, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutBox)
	enum { IDD = IDD_ABOUTBOX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutBox)
	afx_msg void OnDaMore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	IDaControl & mDaControl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTBOX_H__EC6C49AC_2E28_4F81_BE5B_9FF1DA0D73D8__INCLUDED_)
