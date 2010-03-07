/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaServer.h"
#include "DaAgentCommands.h"
#include "AgentPopupWnd.h"
#include "VoiceCommandsWnd.h"
#include "Sapi5Input.h"
#include "Sapi5Err.h"
#include "Localize.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentCommands, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentCommands, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentCommands::CDaAgentCommands (long pCharID, IDaNotify & pNotify)
:	mCharID (pCharID),
	mLangID (GetUserDefaultUILanguage ()),
	mNotify (pNotify),
	mVisible (true),
	mHelpContextId (0),
	mFontSize (0),
	mGlobalVoiceCommandsEnabled (true),
	mHideCharacterCmdId (100),
	mShowCharacterCmdId (101),
	mShowCommandsCmdId (ID_COMMANDS_WINDOW_OPEN),
	mHideCommandsCmdId (ID_COMMANDS_WINDOW_CLOSE),
	mNextCommandId (105)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] CDaAgentCommands::CDaAgentCommands (%d)"), this, m_dwRef, mCharID, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
}

CDaAgentCommands::~CDaAgentCommands ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] CDaAgentCommands::~CDaAgentCommands (%d)"), this, m_dwRef, mCharID, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] CDaAgentCommands::~CDaAgentCommands (%d) Done"), this, m_dwRef, mCharID, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

void CDaAgentCommands::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		CDaAgentCommand *	lCommand;
		INT_PTR				lNdx;

		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					ExternalDisconnect ();
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		try
		{
			for	(lNdx = mCommands.GetUpperBound (); lNdx >= 0; lNdx--)
			{
				lCommand = mCommands (lNdx);
				if	(pFinal)
				{
					mCommands.RemoveAt (lNdx);
				}
				if	(lCommand)
				{
					lCommand->Terminate (pFinal, pAbandonned);
					if	(pFinal)
					{
						lCommand->OnFinalRelease ();
					}
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			for	(lNdx = mRemoved.GetUpperBound (); lNdx >= 0; lNdx--)
			{
				lCommand = mRemoved (lNdx);
				mCommands.RemoveAt (lNdx);
				if	(lCommand)
				{
					lCommand->Terminate (pFinal, pAbandonned);
					lCommand->OnFinalRelease ();
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void CDaAgentCommands::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] CDaAgentCommands::OnFinalRelease"), this, m_dwRef, mCharID);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentCommands, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentCommands)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetCommand", DISPID_IAgentCommands_GetCommand, DspGetCommand, VT_EMPTY, VTS_I4 VTS_PUNKNOWN)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetCount", DISPID_IAgentCommands_GetCount, DspGetCount, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetCaption", DISPID_IAgentCommands_SetCaption, DspSetCaption, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetCaption", DISPID_IAgentCommands_GetCaption, DspGetCaption, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetVoice", DISPID_IAgentCommands_SetVoice, DspSetVoice, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetVoice", DISPID_IAgentCommands_GetVoice, DspGetVoice, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetVisible", DISPID_IAgentCommands_SetVisible, DspSetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetVisible", DISPID_IAgentCommands_GetVisible, DspGetVisible, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "Add", DISPID_IAgentCommands_Add, DspAdd, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "Insert", DISPID_IAgentCommands_Insert, DspInsert, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "Remove", DISPID_IAgentCommands_Remove, DspRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommands, "RemoveAll", DISPID_IAgentCommands_RemoveAll, DspRemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetCommandEx", DISPID_IAgentCommandsEx_GetCommandEx, DspGetCommand, VT_EMPTY, VTS_I4 VTS_PUNKNOWN)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetDefaultID", DISPID_IAgentCommandsEx_SetDefaultID, DspSetDefaultID, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetDefaultID", DISPID_IAgentCommandsEx_GetDefaultID, DspGetDefaultID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetHelpContextID", DISPID_IAgentCommandsEx_SetHelpContextID, DspSetHelpContextID, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetHelpContextID", DISPID_IAgentCommandsEx_GetHelpContextID, DspGetHelpContextID, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetFontName", DISPID_IAgentCommandsEx_SetFontName, DspSetFontName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetFontName", DISPID_IAgentCommandsEx_GetFontName, DspGetFontName, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetFontSize", DISPID_IAgentCommandsEx_SetFontSize, DspSetFontSize, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetFontSize", DISPID_IAgentCommandsEx_GetFontSize, DspGetFontSize, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetVoiceCaption", DISPID_IAgentCommandsEx_SetVoiceCaption, DspSetVoiceCaption, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetVoiceCaption", DISPID_IAgentCommandsEx_GetVoiceCaption, DspGetVoiceCaption, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CDaAgentCommands, "AddEx", DISPID_IAgentCommandsEx_AddEx, DspAddEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "InsertEx", DISPID_IAgentCommandsEx_InsertEx, DspInsertEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CDaAgentCommands, "SetGlobalVoiceCommandsEnabled", DISPID_IAgentCommandsEx_SetGlobalVoiceCommandsEnabled, DspSetGlobalVoiceCommandsEnabled, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CDaAgentCommands, "GetGlobalVoiceCommandsEnabled", DISPID_IAgentCommandsEx_GetGlobalVoiceCommandsEnabled, DspGetGlobalVoiceCommandsEnabled, VT_EMPTY, VTS_PI4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentCommands, CCmdTarget)
	INTERFACE_PART(CDaAgentCommands, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaAgentCommands, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentCommands, __uuidof(IDaSvrCommands), Commands)
	INTERFACE_PART(CDaAgentCommands, __uuidof(IAgentCommands), Commands)
	INTERFACE_PART(CDaAgentCommands, __uuidof(IAgentCommandsEx), Commands)
	INTERFACE_PART(CDaAgentCommands, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentCommands, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentCommands, Commands)
IMPLEMENT_DISPATCH_IID(CDaAgentCommands, __uuidof(IDaSvrCommands))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentCommands, __uuidof(IDaSvrCommands))

BEGIN_SUPPORTERRORINFO(CDaAgentCommands)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommands, __uuidof(IDaSvrCommands))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommands, __uuidof(IAgentCommands))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentCommands, __uuidof(IAgentCommandsEx))
END_SUPPORTERRORINFO(CDaAgentCommands)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaAgentCommands::GetCharID () const
{
	return mCharID;
}

LANGID CDaAgentCommands::GetLangID () const
{
	return mLangID;
}

bool CDaAgentCommands::SetLangID (LANGID pLangID)
{
	if	(pLangID == LANG_USER_DEFAULT)
	{
#ifdef	_DEBUG
		pLangID = LANGIDFROMLCID (GetThreadLocale ());
#else
		pLangID = GetUserDefaultUILanguage();
#endif
	}
	else
	if	(pLangID == LANG_SYSTEM_DEFAULT)
	{
		pLangID = GetSystemDefaultUILanguage();
	}

	if	(mLangID != pLangID)
	{
		mLangID = pLangID;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

USHORT CDaAgentCommands::DoContextMenu (HWND pOwner, const CPoint & pPosition)
{
	USHORT				lRet = 0;
	CMenu				lMenu;
	CString				lMenuText;
	CAgentPopupWnd *	lOwner;
	CVoiceCommandsWnd *	lVoiceCommandsWnd;

	if	(lMenu.CreatePopupMenu ())
	{
		if	(
				(!::IsWindowVisible (pOwner))
			||	(
					(lOwner = DYNAMIC_DOWNCAST (CAgentPopupWnd, CWnd::FromHandle (pOwner)))
				&&	(lOwner->IsHidingQueued ())
				)
			)
		{
			lMenuText = CLocalize::LoadString (IDS_COMMAND_SHOW, mLangID);
			lMenu.AppendMenu (MF_BYCOMMAND, mShowCharacterCmdId, lMenuText);
		}
		else
		{
			lMenuText = CLocalize::LoadString (IDS_COMMAND_HIDE, mLangID);
			lMenu.AppendMenu (MF_BYCOMMAND, mHideCharacterCmdId, lMenuText);

			if	(
					(mVisible)
				&&	(mCommands.GetSize() > 0)
				)
			{
				int					lCommandNdx;
				CDaAgentCommand *	lCommand;
				bool				lFirstCommand = true;

				for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
				{
					if	(lCommand->mVisible)
					{
						if	(lFirstCommand)
						{
							lMenu.AppendMenu (MF_SEPARATOR);
							lFirstCommand = false;
						}

						lMenu.AppendMenu (MF_BYCOMMAND | (lCommand->mEnabled ? MF_ENABLED : MF_DISABLED|MF_GRAYED), lCommand->mId, lCommand->mCaption);
					}
				}

				if	(mDefaultId)
				{
					lMenu.SetDefaultItem (mDefaultId);
				}
			}
		}

		if	(mCommands.GetSize() > 0)
		{
			if	(
					(lVoiceCommandsWnd = TheServerApp->GetVoiceCommandsWnd (false))
				&&	(lVoiceCommandsWnd->GetSafeHwnd ())
				&&	(::IsWindowVisible (lVoiceCommandsWnd->GetSafeHwnd ()))
				)
			{
				lMenuText = CLocalize::LoadString (ID_COMMANDS_WINDOW_CLOSE, mLangID);
				lMenu.InsertMenu (0, MF_STRING|MF_BYPOSITION, mHideCommandsCmdId, lMenuText);
			}
			else
			{
				lMenuText = CLocalize::LoadString (ID_COMMANDS_WINDOW_OPEN, mLangID);
				lMenu.InsertMenu (0, MF_STRING|MF_BYPOSITION, mShowCommandsCmdId, lMenuText);
			}
		}

		::SetForegroundWindow (pOwner);
		lRet = (USHORT)::TrackPopupMenu (lMenu, TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_RIGHTBUTTON, pPosition.x, pPosition.y, 0, pOwner, NULL);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCommand * CDaAgentCommands::GetDefaultCommand ()
{
	if	(mDefaultId)
	{
		return mCommands (FindCommand (mDefaultId));
	}
	return NULL;
}

int CDaAgentCommands::FindCommand (USHORT pCommandId)
{
	int					lCommandNdx;
	CDaAgentCommand *	lCommand;

	for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
	{
		if	(lCommand->mId == pCommandId)
		{
			return lCommandNdx;
		}
	}
	return -1;
}

CDaAgentCommand * CDaAgentCommands::GetCommand (USHORT pCommandId)
{
	int					lCommandNdx;
	CDaAgentCommand *	lCommand;

	for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
	{
		if	(lCommand->mId == pCommandId)
		{
			return lCommand;
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCommand * CDaAgentCommands::NewCommand (LPCTSTR pCaption, LPCTSTR pVoice, LPCTSTR pVoiceCaption, bool pEnabled, bool pVisible, ULONG pHelpContextId)
{
	CDaAgentCommand *	lCommand;

	if	(lCommand = new CDaAgentCommand)
	{
		lCommand->mId = mNextCommandId++;
		lCommand->mCaption = pCaption;
		lCommand->mVoice = pVoice;
		lCommand->mVoiceCaption = pVoiceCaption;
		lCommand->mEnabled = pEnabled;
		lCommand->mVisible = pVisible;
		lCommand->mHelpContextId = pHelpContextId;
	}
	return lCommand;
}

bool CDaAgentCommands::RemoveCommand (int pCommandNdx)
{
//
//	Keep removed commands around in case they have active references.
//
	if	(
			(pCommandNdx >= 0)
		&&	(pCommandNdx <= mCommands.GetUpperBound ())
		)
	{
		mRemoved.Add (mCommands.GetAt (pCommandNdx));
		mCommands.RemoveAt (pCommandNdx);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString CDaAgentCommands::GetVoiceCommandsCaption () const
{
	CString	lCaption (mCaption);
#ifdef	_STRICT_COMPATIBILITY
	if	(mVoice.IsEmpty ())
	{
		lCaption.Empty ();
	}
#endif
	return lCaption;
}

bool CDaAgentCommands::ShowVoiceCommands (CVoiceCommandsWnd * pVoiceCommandsWnd)
{
	bool	lRet = false;

	if	(pVoiceCommandsWnd->GetSafeHwnd ())
	{
		int					lCommandNdx;
		CDaAgentCommand *	lCommand;
		CArrayEx <long>		lCmdId;
		CStringArray		lCmdName;

		if	(mCommands.GetSize() > 0)
		{
			for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
			{
				if	(lCommand->mEnabled)
				{
					lCmdId.Add (lCommand->mId);
					if	(!lCommand->mVoiceCaption.IsEmpty())
					{
						lCmdName.Add (lCommand->mVoiceCaption);
					}
					else
					{
						lCmdName.Add (lCommand->mCaption);
					}
				}
			}
		}

		if	(mGlobalVoiceCommandsEnabled)
		{
			pVoiceCommandsWnd->ShowGlobalCommands (mHideCommandsCmdId, mHideCharacterCmdId);
		}
		else
		{
			pVoiceCommandsWnd->HideGlobalCommands ();
		}
		if	(pVoiceCommandsWnd->ShowTheseCommands (mCharID, GetVoiceCommandsCaption (), lCmdId, lCmdName))
		{
			lRet = true;
		}
	}

	return lRet;
}

bool CDaAgentCommands::SetupVoiceContext (class CSapi5InputContext * pInputContext)
{
	bool	lRet = false;

	if	(pInputContext)
	{
		int					lCommandNdx;
		CDaAgentCommand *	lCommand;
		CArrayEx <long>		lCmdId;
		CStringArray		lCmdName;
		CStringArray		lCmdVoice;

		if	(mCommands.GetSize() > 0)
		{
			for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
			{
				if	(lCommand->mEnabled)
				{
					lCmdId.Add (lCommand->mId);
					if	(!lCommand->mVoiceCaption.IsEmpty())
					{
						lCmdName.Add (lCommand->mVoiceCaption);
					}
					else
					{
						lCmdName.Add (lCommand->mCaption);
					}

					if	(!lCommand->mVoice.IsEmpty())
					{
						lCmdVoice.Add (lCommand->mVoice);
					}
					else
					if	(!lCommand->mVoiceCaption.IsEmpty())
					{
						lCmdVoice.Add (lCommand->mVoiceCaption);
						lCmdVoice [lCmdVoice.GetUpperBound()].MakeLower ();
					}
					else
					{
						lCmdVoice.Add (lCommand->mCaption);
						lCmdVoice [lCmdVoice.GetUpperBound()].MakeLower ();
					}
				}
			}
		}

		if	(mGlobalVoiceCommandsEnabled)
		{
			LogComErr (LogNormal, pInputContext->SetGlobalCommands (mShowCommandsCmdId, mHideCommandsCmdId, mHideCharacterCmdId));
		}
		else
		{
			pInputContext->RemoveGlobalCommands ();
		}
		if	(SUCCEEDED (LogComErr (LogNormal, pInputContext->SetTheseCommands (mCharID, GetVoiceCommandsCaption (), lCmdId, lCmdName, lCmdVoice))))
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetCaption (BSTR bszCaption)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lCaption (bszCaption);

	if	(lCaption.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(lCaption == pThis->mCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mCaption = lCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetCaption (BSTR *pbszCaption)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszCaption) = pThis->mCaption.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetVoice (BSTR bszVoice)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetVoice"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoice (bszVoice);

	if	(lVoice == pThis->mVoice)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mVoice = lVoice;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetVoice"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetVoice (BSTR *pbszVoice)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetVoice"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszVoice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszVoice) = pThis->mVoice.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetVoice"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetVoiceCaption (BSTR bszVoiceCaption)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetVoiceCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoiceCaption (bszVoiceCaption);

	if	(lVoiceCaption == pThis->mVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mVoiceCaption = lVoiceCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetVoiceCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetVoiceCaption (BSTR *bszVoiceCaption)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetVoiceCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!bszVoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*bszVoiceCaption) = pThis->mVoiceCaption.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetVoiceCaption"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetVisible (long bVisible)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetVisible [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bVisible);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mVisible == (bVisible != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mVisible = (bVisible != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetVisible (long *pbVisible)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = pThis->mVisible ? S_OK : S_FALSE;

	if	(pbVisible)
	{
		(*pbVisible) = (pThis->mVisible!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetDefaultID (long dwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetDefaultID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mDefaultId == (USHORT)dwID)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mDefaultId = (USHORT)dwID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetDefaultID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetDefaultID (long *pdwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetDefaultID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pdwID) = (long)pThis->mDefaultId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetDefaultID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetHelpContextID (long ulHelpID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mHelpContextId == (ULONG)ulHelpID)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mHelpContextId = (ULONG)ulHelpID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetHelpContextID (long *pulHelpID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pulHelpID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pulHelpID) = (long)pThis->mHelpContextId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetHelpContextID"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetFontName (BSTR bszFontName)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lFontName (bszFontName);

	if	(pThis->mFontName == lFontName)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mFontName = lFontName;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetFontName (BSTR *pbszFontName)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszFontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszFontName) = pThis->mFontName.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetFontSize (long lFontSize)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mFontSize == lFontSize)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mFontSize = lFontSize;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetFontSize (long *lFontSize)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!lFontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*lFontSize) = pThis->mFontSize;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::SetGlobalVoiceCommandsEnabled (long bEnable)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetGlobalVoiceCommandsEnabled"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mGlobalVoiceCommandsEnabled == (bEnable != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mGlobalVoiceCommandsEnabled = (bEnable != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::SetGlobalVoiceCommandsEnabled"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetGlobalVoiceCommandsEnabled (long *pbEnabled)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetGlobalVoiceCommandsEnabled"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbEnabled)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbEnabled) = (pThis->mGlobalVoiceCommandsEnabled!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetGlobalVoiceCommandsEnabled"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetCount (long *pdwCount)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCount"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pdwCount) = (long)pThis->mCommands.GetSize();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCount"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetCommand (long dwCommandID, IUnknown **ppunkCommand)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCommand"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCommand *	lCommand;

	if	(!ppunkCommand)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkCommand) = NULL;

		if	(lCommand = pThis->GetCommand ((USHORT) dwCommandID))
		{
			IUnknownPtr lInterface (lCommand->GetControllingUnknown ());
			(*ppunkCommand) = lInterface.Detach();
		}
		else
		{
//			lResult = E_INVALIDARG;
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCommand"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::GetCommandEx (long dwCommandID, IDaSvrCommand **ppCommandEx)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCommandEx"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCommand *	lCommand;

	if	(!ppCommandEx)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppCommandEx) = NULL;

		if	(lCommand = pThis->GetCommand ((USHORT) dwCommandID))
		{
			IDaSvrCommandPtr lInterface (lCommand->GetControllingUnknown ());
			(*ppCommandEx) = lInterface.Detach ();
		}
		else
		{
//			lResult = E_INVALIDARG;
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::GetCommandEx"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::Add (BSTR bszCaption, BSTR bszVoice, long bEnabled, long bVisible, long *pdwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::Add [%ls] [%ls] [%d] [%d]"), pThis, pThis->m_dwRef, pThis->mCharID, bszCaption, bszVoice, bEnabled, bVisible);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCommand *	lCommand;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = pThis->NewCommand (CString (bszCaption), CString (bszVoice), NULL, (bEnabled != FALSE), (bVisible != FALSE)))
	{
		pThis->mCommands.Add (lCommand);

		if	(pdwID)
		{
			(*pdwID) = (long)lCommand->mId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::Add"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::AddEx (BSTR bszCaption, BSTR bszVoice, BSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long *pdwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::AddEx"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCommand *	lCommand;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = pThis->NewCommand (CString (bszCaption), CString (bszVoice), CString (bszVoiceCaption), (bEnabled != FALSE), (bVisible != FALSE), (ULONG)ulHelpID))
	{
		pThis->mCommands.Add (lCommand);

		if	(pdwID)
		{
			(*pdwID) = (long)lCommand->mId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::AddEx"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::Insert (BSTR bszCaption, BSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long *pdwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::Insert"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCommand *	lCommand;
	INT_PTR				lInsertNdx;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = pThis->NewCommand (CString (bszCaption), CString (bszVoice), NULL, (bEnabled != FALSE), (bVisible != FALSE)))
	{
		lInsertNdx = pThis->FindCommand ((USHORT)dwRefID);
		if	(lInsertNdx < 0)
		{
			if	(bBefore)
			{
				lInsertNdx = 0;
			}
			else
			{
				lInsertNdx = pThis->mCommands.GetSize();
			}
		}
		else
		{
			if	(!bBefore)
			{
				lInsertNdx++;
			}
		}

		pThis->mCommands.InsertAt (lInsertNdx,lCommand);

		if	(pdwID)
		{
			(*pdwID) = (long)lCommand->mId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::Insert"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::InsertEx (BSTR bszCaption, BSTR bszVoice, BSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long *pdwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::InsertEx"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CDaAgentCommand *	lCommand;
	INT_PTR				lInsertNdx;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = pThis->NewCommand (CString (bszCaption), CString (bszVoice), CString (bszVoiceCaption), (bEnabled != FALSE), (bVisible != FALSE), (ULONG)ulHelpID))
	{
		lInsertNdx = pThis->FindCommand ((USHORT)dwRefID);
		if	(lInsertNdx < 0)
		{
			if	(bBefore)
			{
				lInsertNdx = 0;
			}
			else
			{
				lInsertNdx = pThis->mCommands.GetSize();
			}
		}
		else
		{
			if	(!bBefore)
			{
				lInsertNdx++;
			}
		}

		pThis->mCommands.InsertAt (lInsertNdx,lCommand);

		if	(pdwID)
		{
			(*pdwID) = (long)lCommand->mId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::InsertEx"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::Remove (long dwID)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::Remove"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->RemoveCommand (pThis->FindCommand ((USHORT)dwID)))
	{
//		lResult = E_INVALIDARG;
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::Remove"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentCommands::XCommands::RemoveAll (void)
{
	METHOD_PROLOGUE(CDaAgentCommands, Commands)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::RemoveAll"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(pThis->mCommands.GetSize() <= 0)
	{
		lResult = S_FALSE;
	}
	else
	{
		pThis->mRemoved.Append (pThis->mCommands);
		pThis->mCommands.RemoveAll ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] CDaAgentCommands::XCommands::RemoveAll"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentCommands::DspGetCommand(long CommandID, LPUNKNOWN * Command)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetCommand"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetCommand (CommandID, Command);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetCount(long * Count)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetCount"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetCount (Count);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetCaption(LPCTSTR Caption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetCaption (_bstr_t(Caption));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetCaption(BSTR * Caption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetCaption (Caption);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetVoice(LPCTSTR Voice)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetVoice"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetVoice (_bstr_t(Voice));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetVoice(BSTR * Voice)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetVoice"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetVoice (Voice);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetVisible(long Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetVisible"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetVisible(long * Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetVisible"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspAdd(LPCTSTR Caption, LPCTSTR Voice, long Enabled, long Visible, long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspAdd"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.Add (_bstr_t(Caption), _bstr_t(Voice), Enabled, Visible, ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspInsert(LPCTSTR Caption, LPCTSTR Voice, long Enabled, long Visible, long RefID, long Before, long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspInsert"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.Insert (_bstr_t(Caption), _bstr_t(Voice), Enabled, Visible, RefID, Before, ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspRemove(long ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspRemove"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.Remove (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspRemoveAll()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspRemoveAll"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.RemoveAll ();
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetDefaultID(long ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetDefaultID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetDefaultID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetDefaultID(long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetDefaultID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetDefaultID (ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetHelpContextID(long HelpID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetHelpContextID (HelpID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetHelpContextID(long * HelpID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetHelpContextID (HelpID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetFontName(LPCTSTR FontName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetFontName (_bstr_t(FontName));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetFontName(BSTR * FontName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetFontName (FontName);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetFontSize(long FontSize)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetFontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetFontSize (FontSize);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetFontSize(long * FontSize)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetFontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetFontSize (FontSize);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetVoiceCaption(LPCTSTR VoiceCaption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetVoiceCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetVoiceCaption (_bstr_t(VoiceCaption));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetVoiceCaption(BSTR * VoiceCaption)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetVoiceCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetVoiceCaption (VoiceCaption);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspAddEx(LPCTSTR Caption, LPCTSTR Voice, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpID, long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspAddEx"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.AddEx (_bstr_t(Caption), _bstr_t(Voice), _bstr_t(VoiceCaption), Enabled, Visible, HelpID, ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspInsertEx(LPCTSTR Caption, LPCTSTR Voice, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpID, long RefID, long Before, long * ID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspInsertEx"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.InsertEx (_bstr_t(Caption), _bstr_t(Voice), _bstr_t(VoiceCaption), Enabled, Visible, HelpID, RefID, Before, ID);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspSetGlobalVoiceCommandsEnabled(long Enable)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspSetGlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.SetGlobalVoiceCommandsEnabled (Enable);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentCommands::DspGetGlobalVoiceCommandsEnabled(long * Enabled)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%d] CDaAgentCommands::DspGetGlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = m_xCommands.GetGlobalVoiceCommandsEnabled (Enabled);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
