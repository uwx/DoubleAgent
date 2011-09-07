#include "StdAfx.h"
#include "ControlCpp.h"
#include "ControlCppDlg.h"
#include "Registry.h"
#include "DebugStr.h"

/////////////////////////////////////////////////////////////////////////////

static _bstr_t	sCharName ("Default");

/////////////////////////////////////////////////////////////////////////////

CControlCppDlg::CControlCppDlg()
{
}

CControlCppDlg::~CControlCppDlg()
{
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CControlCppDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CControlCppDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	AtlAdviseSinkMap (this, TRUE);
//	mVisibleButton.Attach (GetDlgItem (IDC_VISIBLE));
	Button_SetCheck (GetDlgItem (IDC_VISIBLE), FALSE);

	if	(SUCCEEDED (LogComErr (LogNormal, GetDlgControl (IDC_DACONTROL, __uuidof(IDaControl2), (void**)&mDaControl))))
	{
		mDaControl.AutoConnect = 0;

		if	(SUCCEEDED (LogComErr (LogNormal, mDaControl.Characters->Load (sCharName, CComVariant(), NULL))))
		{
			LogComErr (LogNormal, mDaControl.ControlCharacter = mDaControl.Characters.Item [sCharName]);

			try
			{
				IDaCtlCommands2Ptr	lCommands = mDaControl.ControlCharacter.Commands;
				IDaCtlCommand2Ptr	lCommand;

				lCommands.Caption = "My Commands";
				lCommands.VoiceCaption = "My Commands";
				lCommands.VoiceGrammar = "my commands";
				lCommands->Add (_bstr_t("Be Happy"), CComVariant ("Be Happy"), CComVariant("be happy"), CComVariant (VARIANT_TRUE), CComVariant (VARIANT_TRUE), &lCommand);
			}
			catch AnyExceptionSilent

			LogComErr (LogNormal, mDaControl.ControlCharacter->Show (CComVariant(VARIANT_FALSE), NULL));
		}
	}

	bHandled = TRUE;
	return TRUE;
}

LRESULT CControlCppDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AtlAdviseSinkMap (this, FALSE);
	return 0;
}

LRESULT CControlCppDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog (IDCANCEL);
	bHandled = TRUE;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CControlCppDlg::OnSpeak(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		mDaControl.ControlCharacter.Balloon.SizeToText = true;
		mDaControl.ControlCharacter.Balloon.AutoPace = true;
		mDaControl.ControlCharacter.Balloon.AutoHide = true;
		mDaControl.ControlCharacter.Balloon.FontSize = 16;
		LogComErr (LogNormal, mDaControl.ControlCharacter->Speak (CComVariant("Say something"), CComVariant(), NULL));
	}

	bHandled = TRUE;
	return 0;
}

LRESULT CControlCppDlg::OnThink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		mDaControl.ControlCharacter.Balloon.SizeToText = true;
		mDaControl.ControlCharacter.Balloon.AutoPace = true;
		mDaControl.ControlCharacter.Balloon.AutoHide = false;
		mDaControl.ControlCharacter.Balloon.FontSize = 16;
		LogComErr (LogNormal, mDaControl.ControlCharacter->Think (_bstr_t("Think something"), NULL));
		LogComErr (LogNormal, mDaControl.ControlCharacter->Think (_bstr_t("Think something else and maybe something more"), NULL));
	}

	bHandled = TRUE;
	return 0;
}

LRESULT CControlCppDlg::OnListen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		VARIANT_BOOL	lStartedListening = VARIANT_FALSE;

		if	(Button_GetCheck (GetDlgItem (IDC_LISTEN)))
		{
			LogComErr (LogNormal, mDaControl.ControlCharacter->Listen (VARIANT_FALSE, &lStartedListening));
		}
		else
		{
			LogComErr (LogNormal, mDaControl.ControlCharacter->Listen (VARIANT_TRUE, &lStartedListening));
		}
	}

	bHandled = TRUE;
	return 0;
}

LRESULT CControlCppDlg::OnVisible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		mDaControl.ControlCharacter.Balloon.Visible = !mDaControl.ControlCharacter.Balloon.Visible;
		Button_SetCheck (GetDlgItem (IDC_VISIBLE), mDaControl.ControlCharacter.Balloon.Visible ? TRUE : FALSE);
	}
	bHandled = TRUE;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CControlCppDlg::OnBalloonShow (BSTR CharacterID)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		Button_SetCheck (GetDlgItem (IDC_VISIBLE), mDaControl.ControlCharacter.Balloon.Visible ? TRUE : FALSE);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CControlCppDlg::OnBalloonHide (BSTR CharacterID)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		Button_SetCheck (GetDlgItem (IDC_VISIBLE), mDaControl.ControlCharacter.Balloon.Visible ? TRUE : FALSE);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CControlCppDlg::OnListenStart (BSTR CharacterID)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		Button_SetCheck (GetDlgItem (IDC_LISTEN), TRUE);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CControlCppDlg::OnListenComplete (BSTR CharacterID, ListenCompleteType Cause)
{
	if	(
			(mDaControl)
		&&	(mDaControl.ControlCharacter)
		)
	{
		Button_SetCheck (GetDlgItem (IDC_LISTEN), TRUE);
	}
	return S_OK;
}
