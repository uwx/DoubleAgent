// MsaWithDaDlg.h : Declaration of the CMsaWithDaDlg

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "DaControlInterface.h"
#include "DaServerInterface.h"

#pragma warning(push)
#pragma warning(disable: 4005)
#include "AgtSvr.h"
#include "AgtCtl.h"
_COM_SMARTPTR_TYPEDEF (IAgentEx, __uuidof(IAgentEx));
_COM_SMARTPTR_TYPEDEF (IAgentCharacterEx, __uuidof(IAgentCharacterEx));
_COM_SMARTPTR_TYPEDEF (IAgentPropertySheet, __uuidof(IAgentPropertySheet));
_COM_SMARTPTR_TYPEDEF (IAgentCtlEx, __uuidof(IAgentCtlEx));
_COM_SMARTPTR_TYPEDEF (IAgentCtlCharacters, __uuidof(IAgentCtlCharacters));
_COM_SMARTPTR_TYPEDEF (IAgentCtlCharacterEx, __uuidof(IAgentCtlCharacterEx));
_COM_SMARTPTR_TYPEDEF (IAgentCtlPropertySheet, __uuidof(IAgentCtlPropertySheet));

class __declspec(uuid("{D45FD2FC-5C6E-11D1-9EC1-00C04FD7081F}")) AgentServer;
class __declspec(uuid("{A7B93C73-7B81-11D0-AC5F-00C04FD97575}")) AgentServerTypeLib;
class __declspec(uuid("{D45FD31B-5C6E-11D1-9EC1-00C04FD7081F}")) AgentControl;
class __declspec(uuid("{F5BE8BC2-7DE6-11D0-91FE-00C04FD701A5}")) AgentControlTypeLib;
#pragma warning(pop)

// CMsaWithDaDlg

class CMsaWithDaDlg 
:	public CAxDialogImpl<CMsaWithDaDlg>,
	//public IDispEventImpl<1, CMsaWithDaDlg, &__uuidof(_DaSvrEvents2), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IDispEventImpl<IDC_CONTROL_MS, CMsaWithDaDlg, &__uuidof(_AgentEvents), &__uuidof(AgentControlTypeLib), 2, 0>,
	public IDispEventImpl<2, CMsaWithDaDlg, &__uuidof(_DaSvrEvents2), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IDispEventImpl<IDC_CONTROL_DA, CMsaWithDaDlg, &__uuidof(_DaCtlEvents2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>
{
public:
	CMsaWithDaDlg();
	~CMsaWithDaDlg();

	enum { IDD = IDD_MSAWITHDADLG };

protected:
	BEGIN_MSG_MAP(CMsaWithDaDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDC_LOAD_MS_SERVER_CHAR, BN_CLICKED, OnLoadMsServerChar)
		COMMAND_HANDLER(IDC_UNLOAD_MS_SERVER_CHAR, BN_CLICKED, OnUnloadMsServerChar)
		COMMAND_HANDLER(IDC_SHOW_MS_SERVER_CHAR, BN_CLICKED, OnShowMsServerChar)
		COMMAND_HANDLER(IDC_HIDE_MS_SERVER_CHAR, BN_CLICKED, OnHideMsServerChar)
		COMMAND_HANDLER(IDC_SHOW_MS_SERVER_OPTIONS, BN_CLICKED, OnShowMsServerOptions)
		COMMAND_HANDLER(IDC_SHOW_MS_SERVER_CHARS, BN_CLICKED, OnShowMsServerCharacters)
		COMMAND_HANDLER(IDC_LOAD_DA_SERVER_CHAR, BN_CLICKED, OnLoadDaServerChar)
		COMMAND_HANDLER(IDC_UNLOAD_DA_SERVER_CHAR, BN_CLICKED, OnUnloadDaServerChar)
		COMMAND_HANDLER(IDC_SHOW_DA_SERVER_CHAR, BN_CLICKED, OnShowDaServerChar)
		COMMAND_HANDLER(IDC_HIDE_DA_SERVER_CHAR, BN_CLICKED, OnHideDaServerChar)
		COMMAND_HANDLER(IDC_SHOW_DA_SERVER_OPTIONS, BN_CLICKED, OnShowDaServerOptions)
		COMMAND_HANDLER(IDC_SHOW_DA_SERVER_CHARS, BN_CLICKED, OnShowDaServerCharacters)
		COMMAND_HANDLER(IDC_LOAD_MS_CONTROL_CHAR, BN_CLICKED, OnLoadMsControlChar)
		COMMAND_HANDLER(IDC_UNLOAD_MS_CONTROL_CHAR, BN_CLICKED, OnUnloadMsControlChar)
		COMMAND_HANDLER(IDC_SHOW_MS_CONTROL_CHAR, BN_CLICKED, OnShowMsControlChar)
		COMMAND_HANDLER(IDC_HIDE_MS_CONTROL_CHAR, BN_CLICKED, OnHideMsControlChar)
		COMMAND_HANDLER(IDC_SHOW_MS_CONTROL_OPTIONS, BN_CLICKED, OnShowMsControlOptions)
		COMMAND_HANDLER(IDC_SHOW_MS_CONTROL_CHARS, BN_CLICKED, OnShowMsControlCharacters)
		COMMAND_HANDLER(IDC_LOAD_DA_CONTROL_CHAR, BN_CLICKED, OnLoadDaControlChar)
		COMMAND_HANDLER(IDC_UNLOAD_DA_CONTROL_CHAR, BN_CLICKED, OnUnloadDaControlChar)
		COMMAND_HANDLER(IDC_SHOW_DA_CONTROL_CHAR, BN_CLICKED, OnShowDaControlChar)
		COMMAND_HANDLER(IDC_HIDE_DA_CONTROL_CHAR, BN_CLICKED, OnHideDaControlChar)
		COMMAND_HANDLER(IDC_SHOW_DA_CONTROL_OPTIONS, BN_CLICKED, OnShowDaControlOptions)
		COMMAND_HANDLER(IDC_SHOW_DA_CONTROL_CHARS, BN_CLICKED, OnShowDaControlCharacters)
		COMMAND_ID_HANDLER(IDD_ABOUTBOX, OnAboutBox)
		CHAIN_MSG_MAP(CAxDialogImpl<CMsaWithDaDlg>)
	END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLoadMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUnloadMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHideMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowMsServerOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowMsServerCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLoadDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUnloadDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHideDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowDaServerOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowDaServerCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLoadMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUnloadMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHideMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowMsControlOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowMsControlCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLoadDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUnloadDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHideDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowDaControlOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnShowDaControlCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAboutBox(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	BEGIN_SINK_MAP(CMsaWithDaDlg)
		SINK_ENTRY_EX(IDC_CONTROL_MS, __uuidof(_AgentEvents), DISPID_AgentEvents_Show, &CMsaWithDaDlg::OnShowMsCtlChar)
		SINK_ENTRY_EX(IDC_CONTROL_MS, __uuidof(_AgentEvents), DISPID_AgentEvents_Hide, &CMsaWithDaDlg::OnHideMsCtlChar)
//		SINK_ENTRY_EX(2, __uuidof(_DaSvrEvents2), DISPID_IAgentNotifySink_VisibleState, &CMsaWithDaDlg::OnVisibleDaSvrChar)
		SINK_ENTRY_EX(IDC_CONTROL_DA, __uuidof(_DaCtlEvents2), DISPID_AgentEvents_Show, &CMsaWithDaDlg::OnShowDaCtlChar)
		SINK_ENTRY_EX(IDC_CONTROL_DA, __uuidof(_DaCtlEvents2), DISPID_AgentEvents_Hide, &CMsaWithDaDlg::OnHideDaCtlChar)
	END_SINK_MAP()

	STDMETHOD (OnShowMsCtlChar) (BSTR CharacterID, short Cause);
	STDMETHOD (OnHideMsCtlChar) (BSTR CharacterID, short Cause);
//	STDMETHOD (OnVisibleDaSvrChar) (long CharacterID, long Visible, long Cause);
	STDMETHOD (OnShowDaCtlChar) (BSTR CharacterID, short Cause);
	STDMETHOD (OnHideDaCtlChar) (BSTR CharacterID, short Cause);

protected:
	void SetMsServerButtons();
	void SetMsControlButtons();
	void SetDaServerButtons();
	void SetDaControlButtons();

protected:
	_bstr_t					mMsCharacterName;
	_bstr_t					mMsCharacterFile;
	IAgentExPtr				mMsServer;
	IAgentCharacterExPtr	mMsSvrCharacter;
	long					mMsSvrCharacterId;
	IAgentCtlExPtr			mMsControl;
	IAgentCtlCharacterExPtr	mMsCtlCharacter;

	_bstr_t					mDaCharacterName;
	_bstr_t					mDaCharacterFile;
	IDaServer2Ptr			mDaServer;
	IDaSvrCharacter2Ptr		mDaSvrCharacter;
	long					mDaSvrCharacterId;
	IDaControl2Ptr			mDaControl;
	IDaCtlCharacter2Ptr		mDaCtlCharacter;
};


