#include "StdAfx.h"
#include "Sample1_C6_MFC4.h"
#include "Sample1Dlg_C6_MFC4.h"
#include "AboutBox.h"

interface DECLSPEC_UUID("A7B93C8B-7B81-11D0-AC5F-00C04FD97575") _IAgentPropertySheet;
interface DECLSPEC_UUID("1147E50A-A208-11DE-ABF2-002421116FB2") _IDaSvrPropertySheet;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSample1Dlg, CDialog)
	//{{AFX_MSG_MAP(CSample1Dlg)
	ON_BN_CLICKED(IDC_LOAD_MS_SERVER_CHAR, OnLoadMsServerChar)
	ON_BN_CLICKED(IDC_UNLOAD_MS_SERVER_CHAR, OnUnloadMsServerChar)
	ON_BN_CLICKED(IDC_SHOW_MS_SERVER_CHAR, OnShowMsServerChar)
	ON_BN_CLICKED(IDC_HIDE_MS_SERVER_CHAR, OnHideMsServerChar)
	ON_BN_CLICKED(IDC_SHOW_MS_SERVER_OPTIONS, OnShowMsServerOptions)
	ON_BN_CLICKED(IDC_SHOW_MS_SERVER_CHARS, OnShowMsServerChars)
	ON_BN_CLICKED(IDC_LOAD_MS_CONTROL_CHAR, OnLoadMsControlChar)
	ON_BN_CLICKED(IDC_UNLOAD_MS_CONTROL_CHAR, OnUnloadMsControlChar)
	ON_BN_CLICKED(IDC_SHOW_MS_CONTROL_CHAR, OnShowMsControlChar)
	ON_BN_CLICKED(IDC_HIDE_MS_CONTROL_CHAR, OnHideMsControlChar)
	ON_BN_CLICKED(IDC_SHOW_MS_CONTROL_OPTIONS, OnShowMsControlOptions)
	ON_BN_CLICKED(IDC_SHOW_MS_CONTROL_CHARS, OnShowMsControlChars)
	ON_BN_CLICKED(IDC_LOAD_DA_SERVER_CHAR, OnLoadDaServerChar)
	ON_BN_CLICKED(IDC_UNLOAD_DA_SERVER_CHAR, OnUnloadDaServerChar)
	ON_BN_CLICKED(IDC_SHOW_DA_SERVER_CHAR, OnShowDaServerChar)
	ON_BN_CLICKED(IDC_HIDE_DA_SERVER_CHAR, OnHideDaServerChar)
	ON_BN_CLICKED(IDC_SHOW_DA_SERVER_OPTIONS, OnShowDaServerOptions)
	ON_BN_CLICKED(IDC_SHOW_DA_SERVER_CHARS, OnShowDaServerChars)
	ON_BN_CLICKED(IDC_LOAD_DA_CONTROL_CHAR, OnLoadDaControlChar)
	ON_BN_CLICKED(IDC_UNLOAD_DA_CONTROL_CHAR, OnUnloadDaControlChar)
	ON_BN_CLICKED(IDC_SHOW_DA_CONTROL_CHAR, OnShowDaControlChar)
	ON_BN_CLICKED(IDC_HIDE_DA_CONTROL_CHAR, OnHideDaControlChar)
	ON_BN_CLICKED(IDC_SHOW_DA_CONTROL_OPTIONS, OnShowDaControlOptions)
	ON_BN_CLICKED(IDC_SHOW_DA_CONTROL_CHARS, OnShowDaControlChars)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CSample1Dlg::CSample1Dlg(CWnd* pParent)
:	CDialog(CSample1Dlg::IDD, pParent),
	mMsCharacterName ("Genie"),
	mMsCharacterFile ("C:\\Windows\\MsAgent\\Chars\\Genie.acs"),
	mDaCharacterName ("Merlin"),
	mDaCharacterFile ("C:\\Windows\\MsAgent\\Chars\\Merlin.acs"),
	mMsServerCharId (0),
	mDaServerCharId (0)
{
	//{{AFX_DATA_INIT(CSample1Dlg)
	//}}AFX_DATA_INIT
	EnableAutomation ();
}

CSample1Dlg::~CSample1Dlg()
{
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSample1Dlg)
	DDX_Control(pDX, IDC_UNLOAD_MS_SERVER_CHAR, mUnloadMsServerChar);
	DDX_Control(pDX, IDC_UNLOAD_MS_CONTROL_CHAR, mUnloadMsControlChar);
	DDX_Control(pDX, IDC_UNLOAD_DA_SERVER_CHAR, mUnloadDaServerChar);
	DDX_Control(pDX, IDC_UNLOAD_DA_CONTROL_CHAR, mUnloadDaControlChar);
	DDX_Control(pDX, IDC_SHOW_MS_SERVER_OPTIONS, mShowMsServerOptions);
	DDX_Control(pDX, IDC_SHOW_MS_SERVER_CHARS, mShowMsServerChars);
	DDX_Control(pDX, IDC_SHOW_MS_SERVER_CHAR, mShowMsServerChar);
	DDX_Control(pDX, IDC_SHOW_MS_CONTROL_OPTIONS, mShowMsControlOptions);
	DDX_Control(pDX, IDC_SHOW_MS_CONTROL_CHARS, mShowMsControlChars);
	DDX_Control(pDX, IDC_SHOW_MS_CONTROL_CHAR, mShowMsControlChar);
	DDX_Control(pDX, IDC_SHOW_DA_SERVER_OPTIONS, mShowDaServerOptions);
	DDX_Control(pDX, IDC_SHOW_DA_SERVER_CHARS, mShowDaServerChars);
	DDX_Control(pDX, IDC_SHOW_DA_SERVER_CHAR, mShowDaServerChar);
	DDX_Control(pDX, IDC_SHOW_DA_CONTROL_OPTIONS, mShowDaControlOptions);
	DDX_Control(pDX, IDC_SHOW_DA_CONTROL_CHARS, mShowDaControlChars);
	DDX_Control(pDX, IDC_SHOW_DA_CONTROL_CHAR, mShowDaControlChar);
	DDX_Control(pDX, IDC_LOAD_MS_SERVER_CHAR, mLoadMsServerChar);
	DDX_Control(pDX, IDC_LOAD_MS_CONTROL_CHAR, mLoadMsControlChar);
	DDX_Control(pDX, IDC_LOAD_DA_SERVER_CHAR, mLoadDaServerChar);
	DDX_Control(pDX, IDC_LOAD_DA_CONTROL_CHAR, mLoadDaControlChar);
	DDX_Control(pDX, IDC_HIDE_MS_SERVER_CHAR, mHideMsServerChar);
	DDX_Control(pDX, IDC_HIDE_MS_CONTROL_CHAR, mHideMsControlChar);
	DDX_Control(pDX, IDC_HIDE_DA_SERVER_CHAR, mHideDaServerChar);
	DDX_Control(pDX, IDC_HIDE_DA_CONTROL_CHAR, mHideDaControlChar);
	DDX_Control(pDX, IDC_CONTROL_MS, mMsControl);
	DDX_Control(pDX, IDC_CONTROL_DA, mDaControl);
	//}}AFX_DATA_MAP
}

BOOL CSample1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	::SetMenu (m_hWnd, ::LoadMenu (AfxGetResourceHandle(), MAKEINTRESOURCE(IDD_SAMPLE1_DIALOG)));
	SetMsControlButtons();
	SetMsServerButtons();
	SetDaControlButtons();
	SetDaServerButtons();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


void CSample1Dlg::SetMsControlButtons()
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
        mShowMsControlChar.EnableWindow (!mMsControlChar.GetVisible());
        mHideMsControlChar.EnableWindow (mMsControlChar.GetVisible());
    }
}

void CSample1Dlg::SetMsServerButtons()
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

void CSample1Dlg::SetDaControlButtons()
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
        mShowDaControlChar.EnableWindow (!mDaControlChar.GetVisible());
        mHideDaControlChar.EnableWindow (mDaControlChar.GetVisible());
    }
}

void CSample1Dlg::SetDaServerButtons()
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

BEGIN_EVENTSINK_MAP(CSample1Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSample1Dlg)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_MS, 15 /* Show */, OnShowControlMs, VTS_BSTR VTS_I2)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_MS, 7 /* Hide */, OnHideControlMs, VTS_BSTR VTS_I2)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_DA, 15 /* Show */, OnShowControlDa, VTS_BSTR VTS_I2)
	ON_EVENT(CSample1Dlg, IDC_CONTROL_DA, 7 /* Hide */, OnHideControlDa, VTS_BSTR VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnShowControlMs(LPCTSTR CharacterID, short Cause) 
{
    SetMsControlButtons();
}

void CSample1Dlg::OnHideControlMs(LPCTSTR CharacterID, short Cause) 
{
    SetMsControlButtons();
}

void CSample1Dlg::OnShowControlDa(LPCTSTR CharacterID, short Cause) 
{
	SetDaControlButtons();
}

void CSample1Dlg::OnHideControlDa(LPCTSTR CharacterID, short Cause) 
{
	SetDaControlButtons();
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnLoadMsServerChar() 
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

void CSample1Dlg::OnUnloadMsServerChar() 
{
    if	((mMsServer.m_lpDispatch != NULL) && (mMsServerChar.m_lpDispatch != NULL))
	{
        mMsServerChar.ReleaseDispatch();
        mMsServer.Unload(mMsServerCharId);
        mMsServerCharId = 0;
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnShowMsServerChar() 
{
    long lReqId;
    
    if	(mMsServerChar.m_lpDispatch != NULL)
	{
        mMsServerChar.Show(FALSE, &lReqId);
        mMsServerChar.Speak(mMsCharacterName + _T(", Microsoft Agent"), _T(""), &lReqId);
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnHideMsServerChar() 
{
    long lReqId;
    
    if	(mMsServerChar.m_lpDispatch != NULL)
	{
        mMsServerChar.Hide(FALSE, &lReqId);
    }
    SetMsServerButtons();
}

void CSample1Dlg::OnShowMsServerOptions() 
{
    IAgentPropertySheet lServerProps;

    if	(mMsServer.m_lpDispatch == NULL)
	{
        mMsServer.CreateDispatch (_T("Agent.Server.2"));
    }
    
    mMsServer.m_lpDispatch->QueryInterface (__uuidof(_IAgentPropertySheet), (void**)&lServerProps.m_lpDispatch);
    lServerProps.SetVisible(TRUE);
}

void CSample1Dlg::OnShowMsServerChars() 
{
    if	(mMsServer.m_lpDispatch == NULL)
	{
        mMsServer.CreateDispatch (_T("Agent.Server.2"));
    }
    mMsServer.ShowDefaultCharacterProperties(0, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnLoadMsControlChar() 
{
    IAgentCtlCharacters lCharacters;

    if	(mMsControlChar.m_lpDispatch == NULL)
	{
		lCharacters.AttachDispatch (mMsControl.GetCharacters());
        mMsControl.SetRaiseRequestErrors (FALSE);
        lCharacters.Load(mMsCharacterName, COleVariant(mMsCharacterFile));
        mMsControl.SetRaiseRequestErrors (TRUE);
		mMsControlChar.AttachDispatch (lCharacters.Character(mMsCharacterName));
        if	(mMsControlChar.m_lpDispatch != NULL)
		{
            mMsControlChar.MoveTo(700, 100, COleVariant());
        }
    }
    SetMsControlButtons();
	
}

void CSample1Dlg::OnUnloadMsControlChar() 
{
    IAgentCtlCharacters lCharacters;

    if	(mMsControlChar.m_lpDispatch != NULL)
	{
		lCharacters.AttachDispatch (mMsControl.GetCharacters());
		mMsControlChar.ReleaseDispatch ();
		lCharacters.Unload(mMsCharacterName);
	}
    SetMsControlButtons();
}

void CSample1Dlg::OnShowMsControlChar() 
{
    if	(mMsControlChar.m_lpDispatch != NULL)
	{
        mMsControlChar.Show(COleVariant (VARIANT_FALSE,VT_BOOL));
        mMsControlChar.Speak(COleVariant(mMsControlChar.GetName() + _T(", Microsoft Agent")), COleVariant());
    }
}

void CSample1Dlg::OnHideMsControlChar() 
{
    if	(mMsControlChar.m_lpDispatch != NULL)
	{
        mMsControlChar.Hide(COleVariant (VARIANT_FALSE,VT_BOOL));
	}	
}

void CSample1Dlg::OnShowMsControlOptions() 
{
	IAgentCtlPropertySheet lPropSheet;

	lPropSheet.AttachDispatch (mMsControl.GetPropertySheet());
	lPropSheet.SetVisible (TRUE);	
}

void CSample1Dlg::OnShowMsControlChars() 
{
    mMsControl.ShowDefaultCharacterProperties(COleVariant(), COleVariant());
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnLoadDaServerChar() 
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

void CSample1Dlg::OnUnloadDaServerChar() 
{
    if	((mDaServer.m_lpDispatch != NULL) && (mDaServerChar.m_lpDispatch != NULL))
	{
        mDaServerChar.ReleaseDispatch();
        mDaServer.Unload(mDaServerCharId);
        mDaServerCharId = 0;
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnShowDaServerChar() 
{
    long lReqId;
    
    if	(mDaServerChar.m_lpDispatch != NULL)
	{
        mDaServerChar.Show(FALSE, &lReqId);
        mDaServerChar.Speak(mDaCharacterName + _T(", Double Agent"), _T(""), &lReqId);
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnHideDaServerChar() 
{
    long lReqId;
    
    if	(mDaServerChar.m_lpDispatch != NULL)
	{
        mDaServerChar.Hide(FALSE, &lReqId);
    }
    SetDaServerButtons();
}

void CSample1Dlg::OnShowDaServerOptions() 
{
    IDaSvrPropertySheet lServerProps;

    if	(mDaServer.m_lpDispatch == NULL)
	{
        mDaServer.CreateDispatch (_T("DoubleAgent.Agent"));
    }
    
    mDaServer.m_lpDispatch->QueryInterface (__uuidof(_IDaSvrPropertySheet), (void**)&lServerProps.m_lpDispatch);
    lServerProps.SetVisible(TRUE);
}

void CSample1Dlg::OnShowDaServerChars() 
{
    if	(mDaServer.m_lpDispatch == NULL)
	{
        mDaServer.CreateDispatch (_T("DoubleAgent.Agent"));
    }
    mDaServer.ShowDefaultCharacterProperties(0, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CSample1Dlg::OnLoadDaControlChar() 
{
    IDaCtlCharacters lCharacters;

    if	(mDaControlChar.m_lpDispatch == NULL)
	{
		lCharacters.AttachDispatch (mDaControl.GetCharacters());
        mDaControl.SetRaiseRequestErrors (FALSE);
        lCharacters.Load(mDaCharacterName, COleVariant(mDaCharacterFile));
        mDaControl.SetRaiseRequestErrors (TRUE);
		mDaControlChar.AttachDispatch (lCharacters.Character(mDaCharacterName));
        if	(mDaControlChar.m_lpDispatch != NULL)
		{
            mDaControlChar.MoveTo(700, 300, COleVariant());
        }
    }
    SetDaControlButtons();
}

void CSample1Dlg::OnUnloadDaControlChar() 
{
    IDaCtlCharacters lCharacters;

    if	(mDaControlChar.m_lpDispatch != NULL)
	{
		lCharacters.AttachDispatch (mDaControl.GetCharacters());
		mDaControlChar.ReleaseDispatch ();
		lCharacters.Unload(mDaCharacterName);
	}
    SetDaControlButtons();
}

void CSample1Dlg::OnShowDaControlChar() 
{
    if	(mDaControlChar.m_lpDispatch != NULL)
	{
        mDaControlChar.Show(COleVariant (VARIANT_FALSE,VT_BOOL));
        mDaControlChar.Speak(COleVariant(mDaControlChar.GetName() + _T(", Double Agent")), COleVariant());
    }
}

void CSample1Dlg::OnHideDaControlChar() 
{
    if	(mDaControlChar.m_lpDispatch != NULL)
	{
        mDaControlChar.Hide(COleVariant (VARIANT_FALSE,VT_BOOL));
	}	
}

void CSample1Dlg::OnShowDaControlOptions() 
{
	IDaCtlPropertySheet lPropSheet;

	lPropSheet.AttachDispatch (mDaControl.GetPropertySheet());
	lPropSheet.SetVisible (TRUE);	
}

void CSample1Dlg::OnShowDaControlChars() 
{
    mDaControl.ShowDefaultCharacterProperties(COleVariant(), COleVariant());
}

/////////////////////////////////////////////////////////////////////////////


void CSample1Dlg::OnAppAbout() 
{
	CAboutBox	lAboutBox (mDaControl, this);

	lAboutBox.DoModal ();
}
