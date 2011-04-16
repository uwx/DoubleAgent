// AboutBox.h : Declaration of the CAboutBox

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>


// CAboutBox

class CAboutBox : 
	public CAxDialogImpl<CAboutBox>
{
public:
	CAboutBox()
	{
	}

	~CAboutBox()
	{
	}

	enum { IDD = IDD_ABOUTBOX };

BEGIN_MSG_MAP(CAboutBox)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(ID_DA_MORE, BN_CLICKED, OnClickedMore)
	CHAIN_MSG_MAP(CAxDialogImpl<CAboutBox>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CAboutBox>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedMore(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}
};


