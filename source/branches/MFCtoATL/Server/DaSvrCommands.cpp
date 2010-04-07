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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
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
			(InlineIsEqualGUID (__uuidof(IDaSvrCommands), riid))
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

						::AppendMenu (lMenu, MF_BYCOMMAND | (lCommand->mEnabled ? MF_ENABLED : MF_DISABLED|MF_GRAYED), lCommand->mId, lCommand->mCaption);
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
		if	(lCommand->mId == pCommandId)
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
		if	(lCommand->mId == pCommandId)
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
		lCommand->mId = mNextCommandId++;
		lCommand->mCaption = pCaption;
		lCommand->mVoice = pVoice;
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
	if	(mVoice.IsEmpty ())
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

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetCaption (BSTR bszCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetCaption [%ls]"), this, m_dwRef, mCharID, bszCaption);
#endif
	HRESULT	lResult = S_OK;
	CString	lCaption (bszCaption);

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

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetCaption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCaption (BSTR *pbszCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszCaption) = mCaption.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetCaption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVoice (BSTR bszVoice)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetVoice"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoice (bszVoice);

	if	(lVoice == mVoice)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoice = lVoice;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetVoice"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVoice (BSTR *pbszVoice)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetVoice"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszVoice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszVoice) = mVoice.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetVoice"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVoiceCaption (BSTR bszVoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetVoiceCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lVoiceCaption (bszVoiceCaption);

	if	(lVoiceCaption == mVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceCaption = lVoiceCaption;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetVoiceCaption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVoiceCaption (BSTR *bszVoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetVoiceCaption"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!bszVoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*bszVoiceCaption) = mVoiceCaption.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetVoiceCaption"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVisible (long bVisible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetVisible [%d]"), this, m_dwRef, mCharID, bVisible);
#endif
	HRESULT	lResult = S_OK;

	if	(mVisible == (bVisible != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mVisible = (bVisible != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVisible (long *pbVisible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetVisible"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = mVisible ? S_OK : S_FALSE;

	if	(pbVisible)
	{
		(*pbVisible) = (mVisible!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetDefaultID (long dwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetDefaultID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mDefaultId == (USHORT)dwID)
	{
		lResult = S_FALSE;
	}
	else
	{
		mDefaultId = (USHORT)dwID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetDefaultID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetDefaultID (long *pdwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetDefaultID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pdwID) = (long)mDefaultId;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetDefaultID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetHelpContextID (long ulHelpID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetHelpContextID"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mHelpContextId == (ULONG)ulHelpID)
	{
		lResult = S_FALSE;
	}
	else
	{
		mHelpContextId = (ULONG)ulHelpID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
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

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetFontName (BSTR bszFontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;
	CString	lFontName (bszFontName);

	if	(mFontName == lFontName)
	{
		lResult = S_FALSE;
	}
	else
	{
		mFontName = lFontName;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetFontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetFontName (BSTR *pbszFontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszFontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbszFontName) = mFontName.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetFontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetFontSize (long lFontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetFontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mFontSize == lFontSize)
	{
		lResult = S_FALSE;
	}
	else
	{
		mFontSize = lFontSize;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetFontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetFontSize (long *lFontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetFontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!lFontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*lFontSize) = mFontSize;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetFontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetGlobalVoiceCommandsEnabled (long bEnable)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::SetGlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(mGlobalVoiceCommandsEnabled == (bEnable != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mGlobalVoiceCommandsEnabled = (bEnable != FALSE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::SetGlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetGlobalVoiceCommandsEnabled (long *pbEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetGlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbEnabled)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pbEnabled) = (mGlobalVoiceCommandsEnabled!=false);
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetGlobalVoiceCommandsEnabled"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCount (long *pdwCount)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetCount"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pdwCount) = (long)mCommands.GetSize();
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetCount"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCommand (long dwCommandID, IUnknown **ppunkCommand)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetCommand"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!ppunkCommand)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkCommand) = NULL;

		if	(lCommand = GetCommand ((USHORT) dwCommandID))
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetCommand"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCommandEx (long dwCommandID, IDaSvrCommand **ppCommandEx)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::GetCommandEx"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!ppCommandEx)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppCommandEx) = NULL;

		if	(lCommand = GetCommand ((USHORT) dwCommandID))
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::GetCommandEx"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Add (BSTR bszCaption, BSTR bszVoice, long bEnabled, long bVisible, long *pdwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::Add [%ls] [%ls] [%d] [%d]"), this, m_dwRef, mCharID, bszCaption, bszVoice, bEnabled, bVisible);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = NewCommand (CString (bszCaption), CString (bszVoice), NULL, (bEnabled != FALSE), (bVisible != FALSE)))
	{
		mCommands.Add (lCommand);

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::Add"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::AddEx (BSTR bszCaption, BSTR bszVoice, BSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long *pdwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::AddEx"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = NewCommand (CString (bszCaption), CString (bszVoice), CString (bszVoiceCaption), (bEnabled != FALSE), (bVisible != FALSE), (ULONG)ulHelpID))
	{
		mCommands.Add (lCommand);

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::AddEx"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Insert (BSTR bszCaption, BSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long *pdwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::Insert"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCommand *	lCommand;
	INT_PTR				lInsertNdx;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = NewCommand (CString (bszCaption), CString (bszVoice), NULL, (bEnabled != FALSE), (bVisible != FALSE)))
	{
		lInsertNdx = FindCommand ((USHORT)dwRefID);
		if	(lInsertNdx < 0)
		{
			if	(bBefore)
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
			if	(!bBefore)
			{
				lInsertNdx++;
			}
		}

		mCommands.InsertAt (lInsertNdx,lCommand);

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::Insert"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::InsertEx (BSTR bszCaption, BSTR bszVoice, BSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long *pdwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::InsertEx"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	DaSvrCommand *	lCommand;
	INT_PTR				lInsertNdx;

	if	(pdwID)
	{
		(*pdwID) = 0;
	}

	if	(lCommand = NewCommand (CString (bszCaption), CString (bszVoice), CString (bszVoiceCaption), (bEnabled != FALSE), (bVisible != FALSE), (ULONG)ulHelpID))
	{
		lInsertNdx = FindCommand ((USHORT)dwRefID);
		if	(lInsertNdx < 0)
		{
			if	(bBefore)
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
			if	(!bBefore)
			{
				lInsertNdx++;
			}
		}

		mCommands.InsertAt (lInsertNdx,lCommand);

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::InsertEx"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Remove (long dwID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCommands::Remove"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!RemoveCommand (FindCommand ((USHORT)dwID)))
	{
//		lResult = E_INVALIDARG;
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands));
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

	PutServerError (lResult, __uuidof(IDaSvrCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCommands::RemoveAll"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
