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
