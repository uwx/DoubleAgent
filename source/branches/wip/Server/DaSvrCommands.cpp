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
#include "DaSvrCommands.h"
#include "AgentPopupWnd.h"
#include "VoiceCommandsWnd.h"
#include "Sapi5Input.h"
#include "Sapi5Err.h"
#include "Localize.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCommands::DaSvrCommands ()
:	mCharID (0),
	mLangID (GetUserDefaultUILanguage ()),
	mNotify (NULL),
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCommands::DaSvrCommands (%d)"), this, m_dwRef, mCharID, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCommands::~DaSvrCommands ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCommands::~DaSvrCommands (%d)"), this, m_dwRef, mCharID, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommands * DaSvrCommands::CreateInstance (long pCharID, _IServerNotify * pNotify)
{
	CComObject<DaSvrCommands> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrCommands>::CreateInstance (&lInstance))))
	{
		lInstance->mCharID = pCharID;
		lInstance->mNotify = pNotify;
	}
	return lInstance;
}

void DaSvrCommands::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		DaSvrCommand *	lCommand;
		INT_PTR			lNdx;

		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
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
						try
						{
							delete lCommand;
						}
						catch AnyExceptionSilent
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
					try
					{
						delete lCommand;
					}
					catch AnyExceptionSilent
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void DaSvrCommands::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCommands::FinalRelease"), this, m_dwRef, mCharID);
	}
#endif
	Terminate (false);
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrCommands::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrCommands2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrCommands), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommands), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommandsEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long DaSvrCommands::GetCharID () const
{
	return mCharID;
}

LANGID DaSvrCommands::GetLangID () const
{
	return mLangID;
}

bool DaSvrCommands::SetLangID (LANGID pLangID)
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

USHORT DaSvrCommands::DoContextMenu (HWND pOwner, const CPoint & pPosition)
{
	USHORT				lRet = 0;
	CMenuHandle			lMenu;
	CString				lMenuText;
	CAgentPopupWnd *	lOwner;
	CVoiceCommandsWnd *	lVoiceCommandsWnd;

	if	(lMenu.Attach (::CreatePopupMenu ()))
	{
		if	(
				(!::IsWindowVisible (pOwner))
			||	(
					(mNotify)
				&&	(lOwner = dynamic_cast <CAgentPopupWnd *> (mNotify->_GetAgentWnd (pOwner)))
				&&	(lOwner->IsHidingQueued ())
				)
			)
		{
			lMenuText = CLocalize::LoadString (IDS_COMMAND_SHOW, mLangID);
			::AppendMenu (lMenu, MF_BYCOMMAND, mShowCharacterCmdId, lMenuText);
		}
		else
		{
			lMenuText = CLocalize::LoadString (IDS_COMMAND_HIDE, mLangID);
			::AppendMenu (lMenu, MF_BYCOMMAND, mHideCharacterCmdId, lMenuText);

			if	(
					(mVisible)
				&&	(mCommands.GetSize() > 0)
				)
			{
				int				lCommandNdx;
				DaSvrCommand *	lCommand;
				bool			lFirstCommand = true;

				for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
				{
					if	(lCommand->mVisible)
					{
						if	(lFirstCommand)
						{
							::AppendMenu (lMenu, MF_SEPARATOR, 0, NULL);
							lFirstCommand = false;
						}

						::AppendMenu (lMenu, MF_BYCOMMAND | (lCommand->mEnabled ? MF_ENABLED : MF_DISABLED|MF_GRAYED), lCommand->mCommandId, lCommand->mCaption);
					}
				}

				if	(mDefaultId)
				{
					::SetMenuDefaultItem (lMenu, mDefaultId, FALSE);
				}
			}
		}

		if	(mCommands.GetSize() > 0)
		{
			if	(
					(lVoiceCommandsWnd = _AtlModule.GetVoiceCommandsWnd (false))
				&&	(lVoiceCommandsWnd->IsWindow ())
				&&	(lVoiceCommandsWnd->IsWindowVisible ())
				)
			{
				lMenuText = CLocalize::LoadString (ID_COMMANDS_WINDOW_CLOSE, mLangID);
				::InsertMenu (lMenu, 0, MF_STRING|MF_BYPOSITION, mHideCommandsCmdId, lMenuText);
			}
			else
			{
				lMenuText = CLocalize::LoadString (ID_COMMANDS_WINDOW_OPEN, mLangID);
				::InsertMenu (lMenu, 0, MF_STRING|MF_BYPOSITION, mShowCommandsCmdId, lMenuText);
			}
		}

		::SetForegroundWindow (pOwner);
		lRet = (USHORT)::TrackPopupMenu (lMenu, TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_RIGHTBUTTON, pPosition.x, pPosition.y, 0, pOwner, NULL);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommand * DaSvrCommands::GetDefaultCommand ()
{
	if	(mDefaultId)
	{
		return mCommands (FindCommand (mDefaultId));
	}
	return NULL;
}

int DaSvrCommands::FindCommand (USHORT pCommandId)
{
	int				lCommandNdx;
	DaSvrCommand *	lCommand;

	for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
	{
		if	(lCommand->mCommandId == pCommandId)
		{
			return lCommandNdx;
		}
	}
	return -1;
}

DaSvrCommand * DaSvrCommands::GetCommand (USHORT pCommandId)
{
	int				lCommandNdx;
	DaSvrCommand *	lCommand;

	for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
	{
		if	(lCommand->mCommandId == pCommandId)
		{
			return lCommand;
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommand * DaSvrCommands::NewCommand (LPCTSTR pCaption, LPCTSTR pVoice, LPCTSTR pVoiceCaption, bool pEnabled, bool pVisible, ULONG pHelpContextId)
{
	DaSvrCommand *	lCommand;

	if	(lCommand = DaSvrCommand::CreateInstance ())
	{
		lCommand->mCommandId = mNextCommandId++;
		lCommand->mCaption = pCaption;
		lCommand->mVoiceGrammar = pVoice;
		lCommand->mVoiceCaption = pVoiceCaption;
		lCommand->mEnabled = pEnabled;
		lCommand->mVisible = pVisible;
		lCommand->mHelpContextId = pHelpContextId;

		lCommand->AddRef ();
	}
	return lCommand;
}

bool DaSvrCommands::RemoveCommand (int pCommandNdx)
{
//
//	Keep removed commands around in case they have active references.
//
	if	(
			(pCommandNdx >= 0)
		&&	(pCommandNdx < (INT_PTR)mCommands.GetCount ())
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

CString DaSvrCommands::GetVoiceCommandsCaption () const
{
	CString	lCaption (mVoiceCaption);
	if	(lCaption.IsEmpty ())
	{
		lCaption = mCaption;
	}
#ifdef	_STRICT_COMPATIBILITY
	if	(mVoiceGrammar.IsEmpty ())
	{
		lCaption.Empty ();
	}
#endif
	return lCaption;
}

bool DaSvrCommands::ShowVoiceCommands (CVoiceCommandsWnd * pVoiceCommandsWnd)
{
	bool	lRet = false;

	if	(
			(pVoiceCommandsWnd)
		&&	(pVoiceCommandsWnd->IsWindow ())
		)
	{
		int						lCommandNdx;
		DaSvrCommand *			lCommand;
		CAtlTypeArray <long>	lCmdId;
		CAtlStringArray			lCmdName;

		if	(mCommands.GetSize() > 0)
		{
			for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
			{
				if	(lCommand->mEnabled)
				{
					lCmdId.Add (lCommand->mCommandId);
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

bool DaSvrCommands::SetupVoiceContext (class CSapi5InputContext * pInputContext)
{
	bool	lRet = false;

	if	(pInputContext)
	{
		int						lCommandNdx;
		DaSvrCommand *			lCommand;
		CAtlTypeArray <long>	lCmdId;
		CAtlStringArray			lCmdName;
		CAtlStringArray			lCmdVoice;

		if	(mCommands.GetSize() > 0)
		{
			for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
			{
				if	(lCommand->mEnabled)
				{
					lCmdId.Add (lCommand->mCommandId);
					if	(!lCommand->mVoiceCaption.IsEmpty())
					{
						lCmdName.Add (lCommand->mVoiceCaption);
					}
					else
					{
						lCmdName.Add (lCommand->mCaption);
					}

					if	(!lCommand->mVoiceGrammar.IsEmpty())
					{
						lCmdVoice.Add (lCommand->mVoiceGrammar);
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

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetCaption (BSTR Caption)
{
	return put_Caption (Caption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCaption (BSTR *Caption)
{
	return get_Caption (Caption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVoice (BSTR Voice)
{
	return put_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVoice (BSTR *Voice)
{
	return get_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVoiceCaption (BSTR VoiceCaption)
{
	return put_VoiceCaption (VoiceCaption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVoiceCaption (BSTR *VoiceCaption)
{
	return get_VoiceCaption (VoiceCaption);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;
	HRESULT			lResult = get_Visible (&lVisible);
	
	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetDefaultID (long CommandID)
{
	return put_DefaultCommand (CommandID);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetDefaultID (long *CommandID)
{
	return get_DefaultCommand (CommandID);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetHelpContextID (long HelpContextID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mHelpContextId == (ULONG)HelpContextID)
	{
		lResult = S_FALSE;
	}
	else
	{
		mHelpContextId = (ULONG)HelpContextID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetHelpContextID (long *pulHelpID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pulHelpID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pulHelpID) = (long)mHelpContextId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetFontName (BSTR FontName)
{
	return put_FontName (FontName);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetFontName (BSTR *FontName)
{
	return get_FontName (FontName);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetFontSize (long FontSize)
{
	return put_FontSize (FontSize);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetFontSize (long *FontSize)
{
	return get_FontSize (FontSize);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetGlobalVoiceCommandsEnabled (long Enabled)
{
	return put_GlobalVoiceCommandsEnabled (Enabled ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetGlobalVoiceCommandsEnabled (long *Enabled)
{
	VARIANT_BOOL	lEnabled = VARIANT_FALSE;
	HRESULT			lResult = get_GlobalVoiceCommandsEnabled (&lEnabled);

	if	(Enabled)
	{
		(*Enabled) = (lEnabled != VARIANT_FALSE);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCount (long *Count)
{
	return get_Count (Count);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCommand (long CommandID, IUnknown **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetCommand"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = GetCommand ((USHORT) CommandID))
		{
			IUnknownPtr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetCommand"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCommandEx (long CommandID, IDaSvrCommand **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetCommandEx"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = GetCommand ((USHORT) CommandID))
		{
			IDaSvrCommandPtr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetCommandEx"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Add (BSTR Caption, BSTR VoiceGrammar, long Enabled, long Visible, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::Add [%ls] [%ls] [%d] [%d]"), this, m_dwRef, mCharID, Caption, VoiceGrammar, Enabled, Visible);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(CommandID)
	{
		(*CommandID) = 0;
	}

	if	(lCommand = NewCommand (CString (Caption), CString (VoiceGrammar), NULL, (Enabled != FALSE), (Visible != FALSE)))
	{
		mCommands.Add (lCommand);

		if	(CommandID)
		{
			(*CommandID) = (long)lCommand->mCommandId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::Add"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::AddEx (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::AddEx"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(CommandID)
	{
		(*CommandID) = 0;
	}

	if	(lCommand = NewCommand (CString (Caption), CString (VoiceGrammar), CString (VoiceCaption), (Enabled != FALSE), (Visible != FALSE), (ULONG)HelpContextID))
	{
		mCommands.Add (lCommand);

		if	(CommandID)
		{
			(*CommandID) = (long)lCommand->mCommandId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::AddEx"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Insert (BSTR Caption, BSTR VoiceGrammar, long Enabled, long Visible, long RefCommandID, long Before, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::Insert"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;
	INT_PTR			lInsertNdx;

	if	(CommandID)
	{
		(*CommandID) = 0;
	}

	if	(lCommand = NewCommand (CString (Caption), CString (VoiceGrammar), NULL, (Enabled != FALSE), (Visible != FALSE)))
	{
		lInsertNdx = FindCommand ((USHORT)RefCommandID);
		if	(lInsertNdx < 0)
		{
			if	(Before)
			{
				lInsertNdx = 0;
			}
			else
			{
				lInsertNdx = mCommands.GetSize();
			}
		}
		else
		{
			if	(!Before)
			{
				lInsertNdx++;
			}
		}

		mCommands.InsertAt (lInsertNdx,lCommand);

		if	(CommandID)
		{
			(*CommandID) = (long)lCommand->mCommandId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::Insert"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::InsertEx (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::InsertEx"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;
	INT_PTR			lInsertNdx;

	if	(CommandID)
	{
		(*CommandID) = 0;
	}

	if	(lCommand = NewCommand (CString (Caption), CString (VoiceGrammar), CString (VoiceCaption), (Enabled != FALSE), (Visible != FALSE), (ULONG)HelpContextID))
	{
		lInsertNdx = FindCommand ((USHORT)RefCommandID);
		if	(lInsertNdx < 0)
		{
			if	(Before)
			{
				lInsertNdx = 0;
			}
			else
			{
				lInsertNdx = mCommands.GetSize();
			}
		}
		else
		{
			if	(!Before)
			{
				lInsertNdx++;
			}
		}

		mCommands.InsertAt (lInsertNdx,lCommand);

		if	(CommandID)
		{
			(*CommandID) = (long)lCommand->mCommandId;
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::InsertEx"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Remove (long CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::Remove"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!RemoveCommand (FindCommand ((USHORT)CommandID)))
	{
//		lResult = E_INVALIDARG;
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::Remove"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::RemoveAll (void)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::RemoveAll"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mCommands.GetSize() <= 0)
	{
		lResult = S_FALSE;
	}
	else
	{
		mRemoved.Append (mCommands);
		mCommands.RemoveAll ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::RemoveAll"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Item (long Index, IDaSvrCommand2 **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_Item"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = mCommands (Index))
		{
			IDaSvrCommand2Ptr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_Item"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_Count"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mCommands.GetSize();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_Count"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Command (long CommandID, IDaSvrCommand2 **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_Command"), this, m_dwRef, mCharID);
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = GetCommand ((USHORT) CommandID))
		{
			IDaSvrCommand2Ptr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_Command"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Caption (BSTR *Caption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_Caption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = mCaption.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_Caption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_Caption (BSTR Caption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::put_Caption [%ls]"), this, m_dwRef, mCharID, Caption);
#endif
	HRESULT	lResult = S_OK;
	CString	lCaption (Caption);

	if	(lCaption.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(lCaption == mCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mCaption = lCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::put_Caption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_VoiceGrammar (BSTR *VoiceGrammar)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_VoiceGrammar"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!VoiceGrammar)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceGrammar) = mVoiceGrammar.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_VoiceGrammar"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_VoiceGrammar (BSTR VoiceGrammar)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::put_VoiceGrammar"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoiceGrammar (VoiceGrammar);

	if	(lVoiceGrammar == mVoiceGrammar)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceGrammar = lVoiceGrammar;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::put_VoiceGrammar"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_Visible"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = mVisible ? S_OK : S_FALSE;

	if	(Visible)
	{
		(*Visible) = (mVisible ? VARIANT_TRUE : VARIANT_FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_Visible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetVisible [%d]"), this, m_dwRef, mCharID, Visible);
#endif
	HRESULT	lResult = S_OK;

	if	(mVisible == (Visible != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mVisible = (Visible != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_DefaultCommand (long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_DefaultCommand"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!CommandID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CommandID) = (long)mDefaultId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_DefaultCommand"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_DefaultCommand (long CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::put_DefaultCommand"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mDefaultId == (USHORT)CommandID)
	{
		lResult = S_FALSE;
	}
	else
	{
		mDefaultId = (USHORT)CommandID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::put_DefaultCommand"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_FontName (BSTR *FontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_FontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontName) = mFontName.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_FontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_FontName (BSTR FontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lFontName (FontName);

	if	(mFontName == lFontName)
	{
		lResult = S_FALSE;
	}
	else
	{
		mFontName = lFontName;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetFontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_FontSize (long *FontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_FontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontSize) = mFontSize;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_FontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_FontSize (long FontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::put_FontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mFontSize == FontSize)
	{
		lResult = S_FALSE;
	}
	else
	{
		mFontSize = FontSize;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::put_FontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_VoiceCaption (BSTR *VoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_VoiceCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = mVoiceCaption.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_VoiceCaption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_VoiceCaption (BSTR VoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::put_VoiceCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoiceCaption (VoiceCaption);

	if	(lVoiceCaption == mVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceCaption = lVoiceCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::put_VoiceCaption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get_GlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = mGlobalVoiceCommandsEnabled ? S_OK : S_FALSE;

	if	(Enabled)
	{
		(*Enabled) = (mGlobalVoiceCommandsEnabled) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get_GlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::put_GlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mGlobalVoiceCommandsEnabled == (Enabled != VARIANT_FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mGlobalVoiceCommandsEnabled = (Enabled != VARIANT_FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::put_GlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::get__NewEnum"), this, m_dwRef, mCharID);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <CComVariant>	lArray;
	IEnumVARIANTPtr			lInterface;
	INT_PTR					lNdx;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(
				(lEnum = new CComObject <CEnumVARIANT>)
			&&	(lArray = new CComVariant [mCommands.GetCount()+1])
			)
		{
			for	(lNdx = 0; lNdx < (INT_PTR)mCommands.GetCount(); lNdx++)
			{
				lArray [lNdx] = (LPDISPATCH)(mCommands [lNdx]);
			}
			if	(SUCCEEDED (lResult = lEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mCommands.GetCount()]), (LPDISPATCH)this, AtlFlagCopy)))
			{
				lInterface = lEnum.Detach ();
				(*ppunkEnum) = lInterface.Detach ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::get__NewEnum"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
