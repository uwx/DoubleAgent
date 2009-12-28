// Sample1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sample1_C8_MFC8.h"
#include "Sample1Dlg_C8_MFC8.h"
#include "CAgentPropertySheet.h"
#include "CAgentCtlCharacters.h"
#include "CAgentCtlPropertySheet.h"
#include "CDaSvrPropertySheet.h"
#include "CDaCtlCharacters.h"
#include "CDaCtlPropertySheet.h"

interface DECLSPEC_UUID("A7B93C8B-7B81-11D0-AC5F-00C04FD97575") _IAgentPropertySheet;
interface DECLSPEC_UUID("1147E50A-A208-11DE-ABF2-002421116FB2") _IDaSvrPropertySheet;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSample1Dlg dialog

CSample1Dlg::CSample1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSample1Dlg::IDD, pParent),
	mMsCharacterName ("Genie"),
	mMsCharacterFile ("C:\\Windows\\MsAgent\\Chars\\Genie.acs"),
	mDaCharacterName ("Merlin"),
	mDaCharacterFile ("C:\\Windows\\MsAgent\\Chars\\Merlin.acs"),
	mMsServerCharId (0),
	mDaServerCharId (0)
{
}

void CSample1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOAD_MS_SERVER_CHAR, mLoadMsServerChar);
	DDX_Control(pDX, IDC_UNLOAD_MS_SERVER_CHAR, mUnloadMsServerChar);
	DDX_Control(pDX, IDC_SHOW_MS_SERVER_CHAR, mShowMsServerChar);
	DDX_Control(pDX, IDC_HIDE_MS_SERVER_CHAR, mHideMsServerChar);
	DDX_Control(pDX, IDC_SHOW_MS_SERVER_OPTIONS, mShowMsServerOptions);
	DDX_Control(pDX, IDC_SHOW_MS_SERVER_CHARS, mShowMsServerChars);
	DDX_Control(pDX, IDC_LOAD_DA_SERVER_CHAR, mLoadDaServerChar);
	DDX_Control(pDX, IDC_UNLOAD_DA_SERVER_CHAR, mUnloadDaServerChar);
	DDX_Control(pDX, IDC_SHOW_DA_SERVER_CHAR, mShowDaServerChar);
	DDX_Control(pDX, IDC_HIDE_DA_SERVER_CHAR, mHideDaServerChar);
	DDX_Control(pDX, IDC_SHOW_DA_SERVER_OPTIONS, mShowDaServerOptions);
	DDX_Control(pDX, IDC_SHOW_DA_SERVER_CHARS, mShowDaServerChars);
	DDX_Control(pDX, IDC_LOAD_MS_CONTROL_CHAR, mLoadMsControlChar);
	DDX_Control(pDX, IDC_UNLOAD_MS_CONTROL_CHAR, mUnloadMsControlChar);
	DDX_Control(pDX, IDC_SHOW_MS_CONTROL_CHAR, mShowMsControlChar);
	DDX_Control(pDX, IDC_HIDE_MS_CONTROL_CHAR, mHideMsControlChar);
	DDX_Control(pDX, IDC_SHOW_MS_CONTROL_OPTIONS, mShowMsControlOptions);
	DDX_Control(pDX, IDC_SHOW_MS_CONTROL_CHARS, mShowMsControlChars);
	DDX_Control(pDX, IDC_LOAD_DA_CONTROL_CHAR, mLoadDaControlChar);
	DDX_Control(pDX, IDC_UNLOAD_DA_CONTROL_CHAR, mUnloadDaControlChar);
	DDX_Control(pDX, IDC_SHOW_DA_CONTROL_CHAR, mShowDaControlChar);
	DDX_Control(pDX, IDC_HIDE_DA_CONTROL_CHAR, mHideDaControlChar);
	DDX_Control(pDX, IDC_SHOW_DA_CONTROL_OPTIONS, mShowDaControlOptions);
	DDX_Control(pDX, IDC_SHOW_DA_CONTROL_CHARS, mShowDaControlChars);
	DDX_Control(pDX, IDC_CONTROL_DA, mDaControl);
	DDX_Control(pDX, IDC_CONTROL_MS, mMsControl);
}

BEGIN_MESSAGE_MAP(CSample1Dlg, CDialog)
	//{{AFX_MSG_MAP(CSample1Dlg)
	ON_BN_CLICKED(IDC_LOAD_MS_SERVER_CHAR, &CSample1Dlg::OnBnClickedLoadMsServerChar)
	ON_BN_CLICKED(IDC_UNLOAD_MS_SERVER_CHAR, &CSample1Dlg::OnBnClickedUnloadMsServerChar)
	ON_BN_CLICKED(IDC_SHOW_MS_SERVER_CHAR, &CSample1Dlg::OnBnClickedShowMsServerChar)
	ON_BN_CLICKED(IDC_HIDE_MS_SERVER_CHAR, &CSample1Dlg::OnBnClickedHideMsServerChar)
	ON_BN_CLICKED(IDC_SHOW_MS_SERVER_OPTIONS, &CSample1Dlg::OnBnClickedShowMsServerOptions)
	ON_BN_CLICKED(IDC_SHOW_MS_SERVER_CHARS, &CSample1Dlg::OnBnClickedShowMsServerChars)
	ON_BN_CLICKED(IDC_LOAD_MS_CONTROL_CHAR, &CSample1Dlg::OnBnClickedLoadMsControlChar)
	ON_BN_CLICKED(IDC_UNLOAD_MS_CONTROL_CHAR, &CSample1Dlg::OnBnClickedUnloadMsControlChar)
	ON_BN_CLICKED(IDC_SHOW_MS_CONTROL_CHAR, &CSample1Dlg::OnBnClickedShowMsControlChar)
	ON_BN_CLICKED(IDC_HIDE_MS_CONTROL_CHAR, &CSample1Dlg::OnBnClickedHideMsControlChar)
	ON_BN_CLICKED(IDC_SHOW_MS_CONTROL_OPTIONS, &CSample1Dlg::OnBnClickedShowMsControlOptions)
	ON_BN_CLICKED(IDC_SHOW_MS_CONTROL_CHARS, &CSample1Dlg::OnBnClickedShowMsControlChars)
	ON_BN_CLICKED(IDC_LOAD_DA_SERVER_CHAR, &CSample1Dlg::OnBnClickedLoadDaServerChar)
	ON_BN_CLICKED(IDC_UNLOAD_DA_SERVER_CHAR, &CSample1Dlg::OnBnClickedUnloadDaServerChar)
	ON_BN_CLICKED(IDC_SHOW_DA_SERVER_CHAR, &CSample1Dlg::OnBnClickedShowDaServerChar)
	ON_BN_CLICKED(IDC_HIDE_DA_SERVER_CHAR, &CSample1Dlg::OnBnClickedHideDaServerChar)
	ON_BN_CLICKED(IDC_SHOW_DA_SERVER_OPTIONS, &CSample1Dlg::OnBnClickedShowDaServerOptions)
	ON_BN_CLICKED(IDC_SHOW_DA_SERVER_CHARS, &CSample1Dlg::OnBnClickedShowDaServerChars)
	ON_BN_CLICKED(IDC_LOAD_DA_CONTROL_CHAR, &CSample1Dlg::OnBnClickedLoadDaControlChar)
	ON_BN_CLICKED(IDC_UNLOAD_DA_CONTROL_CHAR, &CSample1Dlg::OnBnClickedUnloadDaControlChar)
	ON_BN_CLICKED(IDC_SHOW_DA_CONTROL_CHAR, &CSample1Dlg::OnBnClickedShowDaControlChar)
	ON_BN_CLICKED(IDC_HIDE_DA_CONTROL_CHAR, &CSample1Dlg::OnBnClickedHideDaControlChar)
	ON_BN_CLICKED(IDC_SHOW_DA_CONTROL_OPTIONS, &CSample1Dlg::OnBnClickedShowDaControlOptions)
	ON_BN_CLICKED(IDC_SHOW_DA_CONTROL_CHARS, &CSample1Dlg::OnBnClickedShowDaControlChars)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSample1Dlg, CDialog)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_MS, 15, CSample1Dlg::OnShowMsChar, VTS_BSTR VTS_I2)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_MS, 7, CSample1Dlg::OnHideMsChar, VTS_BSTR VTS_I2)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_DA, 15, CSample1Dlg::OnShowDaChar, VTS_BSTR VTS_I2)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_DA, 7, CSample1Dlg::OnHideDaChar, VTS_BSTR VTS_I2)
END_EVENTSINK_MAP()

BOOL CSample1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetMsControlButtons();
	SetMsServerButtons();
	SetDaControlButtons();
	SetDaServerButtons();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnShowMsChar(LPCTSTR CharacterID, short Cause)
{
	SetMsControlButtons();
}

void CSample1Dlg::OnHideMsChar(LPCTSTR CharacterID, short Cause)
{
	SetMsControlButtons();
}

void CSample1Dlg::OnShowDaChar(LPCTSTR CharacterID, short Cause)
{
	SetDaControlButtons();
}

void CSample1Dlg::OnHideDaChar(LPCTSTR CharacterID, short Cause)
{
	SetDaControlButtons();
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::SetMsControlButtons(void)
{
    if	(mMsControlChar.m_lpDispatch == NULL)
	{
        mLoadMsControlChar.EnableWindow (TRUE);
        mUnloadMsControlChar.EnableWindow (FALSE);
        mShowMsControlChar.EnableWindow (FALSE);
        mHideMsControlChar.EnableWindow (FALSE);
	}
    else
	{
        mLoadMsControlChar.EnableWindow (FALSE);
        mUnloadMsControlChar.EnableWindow (TRUE);
        mShowMsControlChar.EnableWindow (!mMsControlChar.get_Visible());
        mHideMsControlChar.EnableWindow (mMsControlChar.get_Visible());
    }
}

void CSample1Dlg::SetMsServerButtons(void)
{
    if	(mMsServerChar.m_lpDispatch == NULL)
	{
        mLoadMsServerChar.EnableWindow (TRUE);
        mUnloadMsServerChar.EnableWindow (FALSE);
        mShowMsServerChar.EnableWindow (FALSE);
        mHideMsServerChar.EnableWindow (FALSE);
	}
    else
	{
        mLoadMsServerChar.EnableWindow (FALSE);
        mUnloadMsServerChar.EnableWindow (TRUE);
        mShowMsServerChar.EnableWindow (TRUE);
        mHideMsServerChar.EnableWindow (TRUE);
    }
}

void CSample1Dlg::SetDaControlButtons(void)
{
    if	(mDaControlChar.m_lpDispatch == NULL)
	{
        mLoadDaControlChar.EnableWindow (TRUE);
        mUnloadDaControlChar.EnableWindow (FALSE);
        mShowDaControlChar.EnableWindow (FALSE);
        mHideDaControlChar.EnableWindow (FALSE);
	}
    else
	{
        mLoadDaControlChar.EnableWindow (FALSE);
        mUnloadDaControlChar.EnableWindow (TRUE);
        mShowDaControlChar.EnableWindow (!mDaControlChar.get_Visible());
        mHideDaControlChar.EnableWindow (mDaControlChar.get_Visible());
    }
}

void CSample1Dlg::SetDaServerButtons(void)
{
    if	(mDaServerChar.m_lpDispatch == NULL)
	{
        mLoadDaServerChar.EnableWindow (TRUE);
        mUnloadDaServerChar.EnableWindow (FALSE);
        mShowDaServerChar.EnableWindow (FALSE);
        mHideDaServerChar.EnableWindow (FALSE);
	}
    else
	{
        mLoadDaServerChar.EnableWindow (FALSE);
        mUnloadDaServerChar.EnableWindow (TRUE);
        mShowDaServerChar.EnableWindow (TRUE);
        mHideDaServerChar.EnableWindow (TRUE);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CSample1Dlg message handlers
/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnBnClickedLoadMsServerChar()
{
    long lReqId;
    
    if	(mMsServer.m_lpDispatch == NULL)
	{
        mMsServer.CreateDispatch (_T("Agent.Server.2"));
    }
    
    if	(mMsServerChar.m_lpDispatch == NULL)
	{
        mMsServer.Load(COleVariant (mMsCharacterFile), &mMsServerCharId, &lReqId);
        mMsServer.GetCharacter(mMsServerCharId, &mMsServerChar.m_lpDispatch);
	    if	(mMsServerChar.m_lpDispatch != NULL)
		{
			mMsServerChar.MoveTo(700, 100, 0, &lReqId);
		}
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnBnClickedUnloadMsServerChar()
{
    if	((mMsServer.m_lpDispatch != NULL) && (mMsServerChar.m_lpDispatch != NULL))
	{
        mMsServerChar.ReleaseDispatch();
        mMsServer.Unload(mMsServerCharId);
        mMsServerCharId = 0;
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnBnClickedShowMsServerChar()
{
    long lReqId;
    
    if	(mMsServerChar.m_lpDispatch != NULL)
	{
        mMsServerChar.Show(FALSE, &lReqId);
        mMsServerChar.Speak(mMsCharacterName + _T(", Microsoft Agent"), _T(""), &lReqId);
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnBnClickedHideMsServerChar()
{
    long lReqId;
    
    if	(mMsServerChar.m_lpDispatch != NULL)
	{
        mMsServerChar.Hide(FALSE, &lReqId);
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnBnClickedShowMsServerOptions()
{
    CAgentPropertySheet lServerProps;

    if	(mMsServer.m_lpDispatch == NULL)
	{
        mMsServer.CreateDispatch (_T("Agent.Server.2"));
    }
    
    mMsServer.m_lpDispatch->QueryInterface (__uuidof(_IAgentPropertySheet), (void**)&lServerProps.m_lpDispatch);
    lServerProps.SetVisible(TRUE);
}

void CSample1Dlg::OnBnClickedShowMsServerChars()
{
    if	(mMsServer.m_lpDispatch == NULL)
	{
        mMsServer.CreateDispatch (_T("Agent.Server.2"));
    }
    mMsServer.ShowDefaultCharacterProperties(0, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnBnClickedLoadMsControlChar()
{
    CAgentCtlCharacters lCharacters;

    if	(mMsControlChar.m_lpDispatch == NULL)
	{
		lCharacters.AttachDispatch (mMsControl.get_Characters());
        mMsControl.put_RaiseRequestErrors (FALSE);
        lCharacters.Load(mMsCharacterName, COleVariant(mMsCharacterFile));
        mMsControl.put_RaiseRequestErrors (TRUE);
		mMsControlChar.AttachDispatch (lCharacters.Character(mMsCharacterName));
        if	(mMsControlChar.m_lpDispatch != NULL)
		{
            mMsControlChar.MoveTo(700, 100, COleVariant());
        }
    }
    SetMsControlButtons();
}

void CSample1Dlg::OnBnClickedUnloadMsControlChar()
{
    CAgentCtlCharacters lCharacters;

    if	(mMsControlChar.m_lpDispatch != NULL)
	{
		lCharacters.AttachDispatch (mMsControl.get_Characters());
		mMsControlChar.ReleaseDispatch ();
		lCharacters.Unload(mMsCharacterName);
	}
    SetMsControlButtons();
}

void CSample1Dlg::OnBnClickedShowMsControlChar()
{
    if	(mMsControlChar.m_lpDispatch != NULL)
	{
        mMsControlChar.Show(COleVariant (VARIANT_FALSE,VT_BOOL));
        mMsControlChar.Speak(COleVariant(mMsControlChar.get_Name() + _T(", Microsoft Agent")), COleVariant());
    }
}

void CSample1Dlg::OnBnClickedHideMsControlChar()
{
    if	(mMsControlChar.m_lpDispatch != NULL)
	{
        mMsControlChar.Hide(COleVariant (VARIANT_FALSE,VT_BOOL));
	}	
}

void CSample1Dlg::OnBnClickedShowMsControlOptions()
{
	CAgentCtlPropertySheet lPropSheet;

	lPropSheet.AttachDispatch (mMsControl.get_PropertySheet());
	lPropSheet.put_Visible (TRUE);	
}

void CSample1Dlg::OnBnClickedShowMsControlChars()
{
    mMsControl.ShowDefaultCharacterProperties(COleVariant(), COleVariant());
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnBnClickedLoadDaServerChar()
{
    long lReqId;
    
    if	(mDaServer.m_lpDispatch == NULL)
	{
        mDaServer.CreateDispatch (_T("DoubleAgent.Agent"));
    }
    
    if	(mDaServerChar.m_lpDispatch == NULL)
	{
        mDaServer.Load(COleVariant (mDaCharacterFile), &mDaServerCharId, &lReqId);
        mDaServer.GetCharacter(mDaServerCharId, &mDaServerChar.m_lpDispatch);
	    if	(mDaServerChar.m_lpDispatch != NULL)
		{
			mDaServerChar.MoveTo(700, 300, 0, &lReqId);
		}
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnBnClickedUnloadDaServerChar()
{
    if	((mDaServer.m_lpDispatch != NULL) && (mDaServerChar.m_lpDispatch != NULL))
	{
        mDaServerChar.ReleaseDispatch();
        mDaServer.Unload(mDaServerCharId);
        mDaServerCharId = 0;
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnBnClickedShowDaServerChar()
{
    long lReqId;
    
    if	(mDaServerChar.m_lpDispatch != NULL)
	{
        mDaServerChar.Show(FALSE, &lReqId);
        mDaServerChar.Speak(mDaCharacterName + _T(", Double Agent"), _T(""), &lReqId);
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnBnClickedHideDaServerChar()
{
    long lReqId;
    
    if	(mDaServerChar.m_lpDispatch != NULL)
	{
        mDaServerChar.Hide(FALSE, &lReqId);
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnBnClickedShowDaServerOptions()
{
    CDaSvrPropertySheet lServerProps;

    if	(mDaServer.m_lpDispatch == NULL)
	{
        mDaServer.CreateDispatch (_T("DoubleAgent.Agent"));
    }
    
    mDaServer.m_lpDispatch->QueryInterface (__uuidof(_IDaSvrPropertySheet), (void**)&lServerProps.m_lpDispatch);
    lServerProps.SetVisible(TRUE);
}

void CSample1Dlg::OnBnClickedShowDaServerChars()
{
    if	(mDaServer.m_lpDispatch == NULL)
	{
        mDaServer.CreateDispatch (_T("DoubleAgent.Agent"));
    }
    mDaServer.ShowDefaultCharacterProperties(0, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnBnClickedLoadDaControlChar()
{
    CDaCtlCharacters lCharacters;

    if	(mDaControlChar.m_lpDispatch == NULL)
	{
		lCharacters.AttachDispatch (mDaControl.get_Characters());
        mDaControl.put_RaiseRequestErrors (FALSE);
        lCharacters.Load(mDaCharacterName, COleVariant(mDaCharacterFile));
        mDaControl.put_RaiseRequestErrors (TRUE);
		mDaControlChar.AttachDispatch (lCharacters.Character(mDaCharacterName));
        if	(mDaControlChar.m_lpDispatch != NULL)
		{
            mDaControlChar.MoveTo(700, 300, COleVariant());
        }
    }
    SetDaControlButtons();
}

void CSample1Dlg::OnBnClickedUnloadDaControlChar()
{
    CDaCtlCharacters lCharacters;

    if	(mDaControlChar.m_lpDispatch != NULL)
	{
		lCharacters.AttachDispatch (mDaControl.get_Characters());
		mDaControlChar.ReleaseDispatch ();
		lCharacters.Unload(mDaCharacterName);
	}
    SetDaControlButtons();
}

void CSample1Dlg::OnBnClickedShowDaControlChar()
{
    if	(mDaControlChar.m_lpDispatch != NULL)
	{
        mDaControlChar.Show(COleVariant (VARIANT_FALSE,VT_BOOL));
        mDaControlChar.Speak(COleVariant(mDaControlChar.get_Name() + _T(", Double Agent")), COleVariant());
    }
}

void CSample1Dlg::OnBnClickedHideDaControlChar()
{
    if	(mDaControlChar.m_lpDispatch != NULL)
	{
        mDaControlChar.Hide(COleVariant (VARIANT_FALSE,VT_BOOL));
	}	
}

void CSample1Dlg::OnBnClickedShowDaControlOptions()
{
	CDaCtlPropertySheet lPropSheet;

	lPropSheet.AttachDispatch (mDaControl.get_PropertySheet());
	lPropSheet.put_Visible (TRUE);	
}

void CSample1Dlg::OnBnClickedShowDaControlChars()
{
    mDaControl.ShowDefaultCharacterProperties(COleVariant(), COleVariant());
}
