// MsaWithDaDlg_C8_ATL8.cpp : Implementation of CMsaWithDaDlg

#include "stdafx.h"
#include "MsaWithDaDlg_C8_ATL8.h"
#include "AboutBox.h"

////////////////////////////////////////////////////////////////////////

CMsaWithDaDlg::CMsaWithDaDlg()
:	mMsCharacterName ("Genie"),
	mMsCharacterFile ("C:\\Windows\\MsAgent\\Chars\\Genie.acs"),
	mDaCharacterName ("Merlin"),
	mDaCharacterFile ("Merlin.acs"),
	mMsSvrCharacterId (0),
	mDaSvrCharacterId (0)
{
}

CMsaWithDaDlg::~CMsaWithDaDlg()
{
}

////////////////////////////////////////////////////////////////////////

LRESULT CMsaWithDaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CMsaWithDaDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	GetDlgControl (IDC_CONTROL_MS, __uuidof(IAgentCtlEx), (void**)&mMsControl);
	GetDlgControl (IDC_CONTROL_DA, __uuidof(IDaControl2), (void**)&mDaControl);

	SetMenu (LoadMenu (_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDD_MSAWITHDADLG)));
	SetMsServerButtons();
	SetMsControlButtons();
	SetDaServerButtons();
	SetDaControlButtons();
	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

LRESULT CMsaWithDaDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	mMsSvrCharacter = NULL;
	mMsServer = NULL;
	mDaSvrCharacter = NULL;
	mDaServer = NULL;
	mMsCtlCharacter = NULL;
	mMsControl = NULL;
	mDaCtlCharacter = NULL;
	mDaControl = NULL;
	
	EndDialog(IDOK);
	bHandled = TRUE;
	return 0;
}

////////////////////////////////////////////////////////////////////////

void CMsaWithDaDlg::SetMsServerButtons()
{
    if	(mMsSvrCharacter == NULL)
	{
        ::EnableWindow (GetDlgItem (IDC_LOAD_MS_SERVER_CHAR), TRUE);
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_MS_SERVER_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_MS_SERVER_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_MS_SERVER_CHAR), FALSE);
	}
    else
	{
		long	lVisible;
		
		mMsSvrCharacter->GetVisible (&lVisible);
		
        ::EnableWindow (GetDlgItem (IDC_LOAD_MS_SERVER_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_MS_SERVER_CHAR), TRUE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_MS_SERVER_CHAR), lVisible?FALSE:TRUE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_MS_SERVER_CHAR), lVisible?TRUE:FALSE);
    }
}

void CMsaWithDaDlg::SetMsControlButtons()
{
    if	(mMsCtlCharacter == NULL)
	{
        ::EnableWindow (GetDlgItem (IDC_LOAD_MS_CONTROL_CHAR), (mMsControl!=NULL));
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_MS_CONTROL_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_MS_CONTROL_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_MS_CONTROL_CHAR), FALSE);
	}
    else
	{
		VARIANT_BOOL	lVisible;
		
		mMsCtlCharacter->get_Visible (&lVisible);
		
        ::EnableWindow (GetDlgItem (IDC_LOAD_MS_CONTROL_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_MS_CONTROL_CHAR), TRUE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_MS_CONTROL_CHAR), lVisible?FALSE:TRUE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_MS_CONTROL_CHAR), lVisible?TRUE:FALSE);
    }
}

void CMsaWithDaDlg::SetDaServerButtons()
{
    if	(mDaSvrCharacter == NULL)
	{
        ::EnableWindow (GetDlgItem (IDC_LOAD_DA_SERVER_CHAR), TRUE);
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_DA_SERVER_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_DA_SERVER_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_DA_SERVER_CHAR), FALSE);
	}
    else
	{
        ::EnableWindow (GetDlgItem (IDC_LOAD_DA_SERVER_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_DA_SERVER_CHAR), TRUE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_DA_SERVER_CHAR), mDaSvrCharacter.Visible?FALSE:TRUE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_DA_SERVER_CHAR), mDaSvrCharacter.Visible?TRUE:FALSE);
    }
}

void CMsaWithDaDlg::SetDaControlButtons()
{
    if	(mDaCtlCharacter == NULL)
	{
        ::EnableWindow (GetDlgItem (IDC_LOAD_DA_CONTROL_CHAR), (mDaControl!=NULL));
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_DA_CONTROL_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_DA_CONTROL_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_DA_CONTROL_CHAR), FALSE);
	}
    else
	{
        ::EnableWindow (GetDlgItem (IDC_LOAD_DA_CONTROL_CHAR), FALSE);
        ::EnableWindow (GetDlgItem (IDC_UNLOAD_DA_CONTROL_CHAR), TRUE);
        ::EnableWindow (GetDlgItem (IDC_SHOW_DA_CONTROL_CHAR), mDaCtlCharacter.Visible?FALSE:TRUE);
        ::EnableWindow (GetDlgItem (IDC_HIDE_DA_CONTROL_CHAR), mDaCtlCharacter.Visible?TRUE:FALSE);
    }
}

////////////////////////////////////////////////////////////////////////

LRESULT CMsaWithDaDlg::OnLoadMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    long			lReqId;
    IDispatchPtr	lDispatch;

    if	(mMsServer == NULL)
	{
        mMsServer.CreateInstance (__uuidof(AgentServer));
    }
    if	(
			(mMsServer != NULL)
		&&	(mMsSvrCharacter == NULL)
		)
	{
        mMsServer->Load (CComVariant ((BSTR)mMsCharacterFile), &mMsSvrCharacterId, &lReqId);
        mMsServer->GetCharacter (mMsSvrCharacterId, &lDispatch);
        mMsSvrCharacter = lDispatch;
	    if	(mMsSvrCharacter != NULL)
		{
			mMsSvrCharacter->MoveTo (800, 100, 0, &lReqId);
		}
	}
    SetMsServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnUnloadMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	mMsSvrCharacter = NULL;

    if	(
			(mMsServer != NULL)
		&&	(mMsSvrCharacterId != 0)
		)
	{
        mMsServer->Unload (mMsSvrCharacterId);
        mMsSvrCharacterId = 0;
    }
    SetMsServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    long lReqId;

	if	(mMsSvrCharacter != NULL)
	{
        mMsSvrCharacter->Show (FALSE, &lReqId);
        mMsSvrCharacter->Speak (mMsCharacterName + _T(", Microsoft Agent"), _bstr_t(), &lReqId);
    }
    SetMsServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnHideMsServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    long lReqId;

	if	(mMsSvrCharacter != NULL)
	{
        mMsSvrCharacter->Hide (FALSE, &lReqId);
    }
    SetMsServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowMsServerOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    IAgentPropertySheetPtr lServerProps;

    if	(mMsServer == NULL)
	{
        mMsServer.CreateInstance (__uuidof(AgentServer));
    }
    if	(mMsServer != NULL)
	{
		lServerProps = mMsServer;
	    lServerProps->SetVisible (TRUE);
	}
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowMsServerCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if	(mMsServer == NULL)
	{
        mMsServer.CreateInstance (__uuidof(AgentServer));
    }
    if	(mMsServer != NULL)
	{
	    mMsServer->ShowDefaultCharacterProperties (0, 0, TRUE);
	}
	bHandled = TRUE;
	return 0;
}

////////////////////////////////////////////////////////////////////////

LRESULT CMsaWithDaDlg::OnLoadDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    long			lReqId;
    IDispatchPtr	lDispatch;

    if	(mDaServer == NULL)
	{
        mDaServer.CreateInstance (__uuidof(DaServer));
    }
    if	(
			(mDaServer != NULL)
		&&	(mDaSvrCharacter == NULL)
		)
	{
        mDaServer->Load (CComVariant ((BSTR)mDaCharacterFile), &mDaSvrCharacterId, &lReqId);
        mDaSvrCharacter = mDaServer.Character [mDaSvrCharacterId];
	    if	(mDaSvrCharacter != NULL)
		{
			mDaSvrCharacter->MoveTo (800, 300, 0, &lReqId);
		}
	}
    SetDaServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnUnloadDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	mDaSvrCharacter = NULL;

    if	(
			(mDaServer != NULL)
		&&	(mDaSvrCharacterId != 0)
		)
	{
        mDaServer->Unload (mDaSvrCharacterId);
        mDaSvrCharacterId = 0;
    }
    SetDaServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    long lReqId;

	if	(mDaSvrCharacter != NULL)
	{
        mDaSvrCharacter->Show (FALSE, &lReqId);
        mDaSvrCharacter->Speak (mDaCharacterName + _T(", Double Agent"), _bstr_t(), &lReqId);
    }
    SetDaServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnHideDaServerChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    long lReqId;

	if	(mDaSvrCharacter != NULL)
	{
        mDaSvrCharacter->Hide (FALSE, &lReqId);
    }
    SetDaServerButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowDaServerOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if	(mDaServer == NULL)
	{
        mDaServer.CreateInstance (__uuidof(DaServer));
    }
    if	(mDaServer != NULL)
	{
		mDaServer.PropertySheet.Visible = true;
	}
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowDaServerCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	    if	(mDaServer == NULL)
	{
        mDaServer.CreateInstance (__uuidof(DaServer));
    }
    if	(mDaServer != NULL)
	{
		mDaServer.PropertySheet.Page = _bstr_t(PropertySheet_PageName_Character);
		mDaServer.PropertySheet.Visible = true;
	}
bHandled = TRUE;
	return 0;
}

////////////////////////////////////////////////////////////////////////

LRESULT CMsaWithDaDlg::OnLoadMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IAgentCtlCharactersPtr	lCharacters;

    if	(
			(mMsControl != NULL)
		&&	(mMsCtlCharacter == NULL)
		&&	(SUCCEEDED (mMsControl->get_Characters (&lCharacters)))
		)
	{
		lCharacters->Load (mMsCharacterName, CComVariant ((BSTR)mMsCharacterFile), NULL);
        lCharacters->get_Item (mMsCharacterName, &mMsCtlCharacter);
	    if	(mMsCtlCharacter != NULL)
		{
			mMsCtlCharacter->MoveTo (800, 100, CComVariant(), NULL);
		}
	}
    SetMsControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnUnloadMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IAgentCtlCharactersPtr	lCharacters;
	
	mMsCtlCharacter = NULL;

    if	(
			(mMsControl != NULL)
		&&	(SUCCEEDED (mMsControl->get_Characters (&lCharacters)))
		)
	{
		lCharacters->Unload (mMsCharacterName);
    }
    SetMsControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(mMsCtlCharacter != NULL)
	{
        mMsCtlCharacter->Show (CComVariant(), NULL);
        mMsCtlCharacter->Speak (CComVariant ((BSTR)(mMsCharacterName + _T(", Microsoft Agent"))), CComVariant(), NULL);
    }
    SetMsControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnHideMsControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(mMsCtlCharacter != NULL)
	{
        mMsCtlCharacter->Hide (CComVariant(), NULL);
    }
    SetMsControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowMsControlOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IAgentCtlPropertySheetPtr	lPropSheet;
	
    if	(
			(mMsControl != NULL)
		&&	(SUCCEEDED (mMsControl->get_PropertySheet (&lPropSheet)))
		)
	{
		lPropSheet->put_Visible (VARIANT_TRUE);
    }
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowMsControlCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if	(mMsControl != NULL)
    {
		mMsControl->ShowDefaultCharacterProperties (CComVariant(), CComVariant());
    }
	bHandled = TRUE;
	return 0;
}

////////////////////////////////////////////////////////////////////////

LRESULT CMsaWithDaDlg::OnLoadDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if	(
			(mDaControl != NULL)
		&&	(mDaCtlCharacter == NULL)
		)
	{
		mDaControl.Characters->Load (mDaCharacterName, CComVariant ((BSTR)mDaCharacterFile), NULL);
        mDaCtlCharacter = mDaControl.Characters.Item [mDaCharacterName];
	    if	(mDaCtlCharacter != NULL)
		{
			mDaCtlCharacter->MoveTo (800, 300, CComVariant(), NULL);
		}
	}
    SetDaControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnUnloadDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	mDaCtlCharacter = NULL;

    if	(mDaControl != NULL)
	{
		mDaControl.Characters->Unload (mDaCharacterName);
    }
    SetDaControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(mDaCtlCharacter != NULL)
	{
        mDaCtlCharacter->Show (CComVariant(), NULL);
        mDaCtlCharacter->Speak (CComVariant ((BSTR)(mDaCharacterName + _T(", Double Agent"))), CComVariant(), NULL);
    }
    SetDaControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnHideDaControlChar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if	(mDaCtlCharacter != NULL)
	{
        mDaCtlCharacter->Hide (CComVariant(), NULL);
    }
    SetDaControlButtons();
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowDaControlOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if	(mDaControl != NULL)
    {
		mDaControl.PropertySheet.Visible = true;
    }
	bHandled = TRUE;
	return 0;
}

LRESULT CMsaWithDaDlg::OnShowDaControlCharacters(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if	(mDaControl != NULL)
    {
		mDaControl.PropertySheet.Page = _bstr_t(PropertySheet_PageName_Character);
		mDaControl.PropertySheet.Visible = true;
    }
	bHandled = TRUE;
	return 0;
}

////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CMsaWithDaDlg::OnShowMsCtlChar (BSTR CharacterID, short Cause)
{
	SetMsControlButtons();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMsaWithDaDlg::OnHideMsCtlChar (BSTR CharacterID, short Cause)
{
	SetMsControlButtons();
	return S_OK;
}
//
//HRESULT STDMETHODCALLTYPE CMsaWithDaDlg::OnVisibleDaSvrChar (long CharacterID, long Visible, long Cause)
//{
//	SetDaServerButtons();
//	return S_OK;
//}

HRESULT STDMETHODCALLTYPE CMsaWithDaDlg::OnShowDaCtlChar (BSTR CharacterID, short Cause)
{
	SetDaControlButtons();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMsaWithDaDlg::OnHideDaCtlChar (BSTR CharacterID, short Cause)
{
	SetDaControlButtons();
	return S_OK;
}

////////////////////////////////////////////////////////////////////////

LRESULT CMsaWithDaDlg::OnAboutBox(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAboutBox	lAboutBox;

	lAboutBox.DoModal (m_hWnd);
	bHandled = TRUE;
	return 0;
}
