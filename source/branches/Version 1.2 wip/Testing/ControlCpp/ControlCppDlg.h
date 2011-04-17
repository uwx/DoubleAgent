#pragma once
#include "DaControlInterface.h"

/////////////////////////////////////////////////////////////////////////////

class CControlCppDlg
:	public CAxDialogImpl<CControlCppDlg>,
	public IDispEventImpl<IDC_DACONTROL, CControlCppDlg, &__uuidof(_DaCtlEvents2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>
{
public:
	CControlCppDlg();
	~CControlCppDlg();

// Dialog Data
	enum { IDD = IDD_CONTROLCPP_DIALOG };
//	CContainedWindow	mVisibleButton;

// Implementation
protected:
	BEGIN_MSG_MAP(CControlCppDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_SPEAK, OnSpeak)
		COMMAND_ID_HANDLER(IDC_THINK, OnThink)
		COMMAND_ID_HANDLER(IDC_VISIBLE, OnVisible)
		CHAIN_MSG_MAP(CAxDialogImpl<CControlCppDlg>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSpeak(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnThink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnVisible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	BEGIN_SINK_MAP(CControlCppDlg)
		SINK_ENTRY_EX(IDC_DACONTROL, __uuidof(_DaCtlEvents2), DISPID_AgentEvents_BalloonShow, &CControlCppDlg::OnBalloonShow)
		SINK_ENTRY_EX(IDC_DACONTROL, __uuidof(_DaCtlEvents2), DISPID_AgentEvents_BalloonHide, &CControlCppDlg::OnBalloonHide)
	END_SINK_MAP()

	STDMETHOD (OnBalloonShow) (BSTR CharacterID);
	STDMETHOD (OnBalloonHide) (BSTR CharacterID);

protected:
	IDaControl2Ptr	mDaControl;
};

/////////////////////////////////////////////////////////////////////////////
