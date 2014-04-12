/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "DaCmnCommands.h"
#include "DaCmnCharacter.h"
#include "AgentAnchor.h"
#include "AgentCharacterWnd.h"
#include "VoiceCommandsWnd.h"
#include "Sapi5Input.h"
#include "Sapi5Err.h"
#include "Localize.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnCommands::CDaCmnCommands ()
:	mCharID (0),
	mLangID (GetUserDefaultUILanguage ()),
	mNotify (NULL),
	mVisible (true),
	mFontSize (0),
	mGlobalVoiceCommandsEnabled (true),
	mHideCharacterCmdId (100),
	mShowCharacterCmdId (101),
	mShowCommandsCmdId (ID_COMMANDS_WINDOW_OPEN),
	mHideCommandsCmdId (ID_COMMANDS_WINDOW_CLOSE),
	mNextCommandId (105)
{
}

CDaCmnCommands::~CDaCmnCommands ()
{
	mCommands.DeleteAll ();
	mRemoved.DeleteAll ();
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnCommands::Initialize (long pCharID, CEventNotify* pNotify)
{
	mCharID = pCharID;
	mNotify = pNotify;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCommands::SetLangID (LANGID pLangID)
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

long CDaCmnCommands::DoContextMenu (HWND pOwner, const CPoint& pPosition, CVoiceCommandsWnd* pVoiceCommandsWnd)
{
	long				lRet = 0;
	CMenuHandle			lMenu;
	CAtlString			lMenuText;
	CAgentCharacterWnd*	lOwner;

	if	(lMenu.Attach (::CreatePopupMenu ()))
	{
#ifndef	_DACORE_LOCAL
		HINSTANCE	lResourceInstance = _AtlBaseModule.GetResourceInstance ();
		_AtlBaseModule.SetResourceInstance (GetModuleHandle (_T("DaCore")));
#endif

		try
		{
			if	(
					(mNotify)
				&&	(lOwner = dynamic_cast <CAgentCharacterWnd*> (mNotify->GetAgentWnd (pOwner)))
				&&	(
						(!lOwner->IsCharShown ())
					||	(lOwner->IsHidingQueued ())
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
					&&	(mCommands.GetCount() > 0)
					)
				{
					INT_PTR			lCommandNdx;
					CDaCmnCommand*	lCommand;
					bool			lFirstCommand = true;

					for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
					{
						if	(
								(lCommand->mVisible)
							&&	(!lCommand->mCaption.IsEmpty ())
							)
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

			if	(
					(mNotify)
				&&	(mNotify->mAnchor)
				)
			{
				CAgentFile*	lFile;
				INT_PTR			lFileNdx;

				for	(lFileNdx = 0; lFile = mNotify->mAnchor->mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
				{
					CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
					INT_PTR								lClientNdx;
					CDaCmnCharacter*					lCharacter;

					if	(mNotify->mAnchor->mAnchor.GetFileClients (lFile, lFileClients))
					{
						for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
						{
							if	(
									(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
								&&	(lCharacter->IsValid (lFile))
								&&	(lCharacter->GetCharID() == mCharID)
								)
							{
								break;
							}
							if	(lClientNdx > 0)
							{
								CDaCmnCommands*	lCommands;
								bool			lFirstCommand = true;

								for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
								{
									if	(
											(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
										&&	(lCharacter->IsValid (lFile))
										&&	(lCharacter->GetCharID() != mCharID)
										)
									{
										if	(
												(lCommands = lCharacter->GetCommands (false))
											&&	(lCommands->mVisible)
											&&	(!lCommands->mCaption.IsEmpty ())
											)
										{
											if	(lFirstCommand)
											{
												::AppendMenu (lMenu, MF_SEPARATOR, 0, NULL);
												lFirstCommand = false;
											}
											::AppendMenu (lMenu, MF_BYCOMMAND | MF_ENABLED, MAKELONG(lCharacter->GetCharID(),1), lCommands->mCaption);
										}
									}
								}
								break;
							}
						}
					}
				}
			}

			if	(mCommands.GetCount() > 0)
			{
				if	(
						(pVoiceCommandsWnd)
					&&	(pVoiceCommandsWnd->IsWindow ())
					&&	(pVoiceCommandsWnd->IsWindowVisible ())
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
		}
		catch AnyExceptionDebug

#ifndef	_DACORE_LOCAL
		_AtlBaseModule.SetResourceInstance (lResourceInstance);
#endif
		::SetForegroundWindow (pOwner);
		lRet = (long)::TrackPopupMenu (lMenu, TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_RIGHTBUTTON, pPosition.x, pPosition.y, 0, pOwner, NULL);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CDaCmnCommand* CDaCmnCommands::GetDefaultCommand ()
{
	if	(mDefaultId)
	{
		return mCommands (FindCommand (mDefaultId));
	}
	return NULL;
}

INT_PTR CDaCmnCommands::FindCommand (USHORT pCommandId)
{
	INT_PTR			lCommandNdx;
	CDaCmnCommand*	lCommand;

	for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
	{
		if	(lCommand->mCommandId == pCommandId)
		{
			return lCommandNdx;
		}
	}
	return -1;
}

CDaCmnCommand* CDaCmnCommands::GetCommand (USHORT pCommandId)
{
	INT_PTR			lCommandNdx;
	CDaCmnCommand*	lCommand;

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

CDaCmnCommand* CDaCmnCommands::NewCommand (LPCTSTR pCaption, LPCTSTR pVoice, LPCTSTR pVoiceCaption, bool pEnabled, bool pVisible)
{
	CDaCmnCommand*	lCommand;

	if	(lCommand = new CDaCmnCommand)
	{
		lCommand->mCommandId = mNextCommandId++;
		lCommand->mCaption = pCaption;
		lCommand->mVoiceGrammar = pVoice;
		lCommand->mVoiceCaption = pVoiceCaption;
		lCommand->mEnabled = pEnabled;
		lCommand->mVisible = pVisible;
	}
	return lCommand;
}

bool CDaCmnCommands::RemoveCommand (INT_PTR pCommandNdx)
{
//
//	Keep removed commands around in case they have active references.
//
	if	(
			(pCommandNdx >= 0)
		&&	(pCommandNdx < (INT_PTR)mCommands.GetCount())
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

CAtlString CDaCmnCommands::GetVoiceCommandsCaption () const
{
	CAtlString	lCaption (mVoiceCaption);
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

bool CDaCmnCommands::ShowVoiceCommands (CVoiceCommandsWnd* pVoiceCommandsWnd)
{
	bool	lRet = false;

	if	(
			(pVoiceCommandsWnd)
		&&	(pVoiceCommandsWnd->IsWindow ())
		)
	{
		INT_PTR					lCommandNdx;
		CDaCmnCommand*			lCommand;
		CAtlTypeArray <long>	lCmdId;
		CAtlStringArray			lCmdName;

		if	(mCommands.GetCount() > 0)
		{
			for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
			{
				int lLength = lCommand->mVoiceGrammar.GetLength();

				if	(
						(lCommand->mEnabled)
					&&	(
							(!lCommand->mVoiceGrammar.IsEmpty ())
#ifndef	_STRICT_COMPATIBILITY
						||	(!lCommand->mVoiceCaption.IsEmpty())
						||	(!lCommand->mCaption.IsEmpty())
#endif
						)
					)
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

bool CDaCmnCommands::SetupVoiceContext (class CSapi5InputContext* pInputContext)
{
	bool	lRet = false;

	if	(pInputContext)
	{
		INT_PTR					lCommandNdx;
		CDaCmnCommand*			lCommand;
		CAtlTypeArray <long>	lCmdId;
		CAtlStringArray			lCmdName;
		CAtlStringArray			lCmdVoice;

		if	(mCommands.GetCount() > 0)
		{
			for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
			{
				if	(
						(lCommand->mEnabled)
					&&	(!lCommand->mVoiceGrammar.IsEmpty())
					)
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

					lCmdVoice.Add (lCommand->mVoiceGrammar);
				}
#ifndef	_STRICT_COMPATIBILITY
				else
				if	(
						(lCommand->mEnabled)
					&&	(
							(!lCommand->mVoiceCaption.IsEmpty())
						||	(!lCommand->mCaption.IsEmpty())
						)
					)
				{
					lCmdId.Add (lCommand->mCommandId);
					if	(!lCommand->mVoiceCaption.IsEmpty())
					{
						lCmdName.Add (lCommand->mVoiceCaption);
						lCmdVoice.Add (lCommand->mVoiceCaption);
						lCmdVoice [lCmdVoice.GetCount()-1].MakeLower ();
					}
					else
					{
						lCmdName.Add (lCommand->mCaption);
						lCmdVoice.Add (lCommand->mCaption);
						lCmdVoice [lCmdVoice.GetCount()-1].MakeLower ();
					}
				}
#endif
			}
		}

		if	(mGlobalVoiceCommandsEnabled)
		{
			LogComErr (LogNormal|LogTime, pInputContext->SetGlobalCommands (mShowCommandsCmdId, mHideCommandsCmdId, mHideCharacterCmdId));
		}
		else
		{
			pInputContext->RemoveGlobalCommands ();
		}
		if	(SUCCEEDED (LogComErr (LogNormal|LogTime, pInputContext->SetTheseCommands (mCharID, GetVoiceCommandsCaption (), lCmdId, lCmdName, lCmdVoice))))
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::Add (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long *CommandID)
{
	HRESULT			lResult = S_OK;
	CDaCmnCommand*	lCommand;

	if	(CommandID)
	{
		(*CommandID) = 0;
	}

	if	(lCommand = NewCommand (CAtlString (Caption), CAtlString (VoiceGrammar), CAtlString (VoiceCaption), (Enabled != FALSE), (Visible != FALSE)))
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::Insert (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long RefCommandID, long Before, long *CommandID)
{
	HRESULT			lResult = S_OK;
	CDaCmnCommand*	lCommand;
	INT_PTR			lInsertNdx;

	if	(CommandID)
	{
		(*CommandID) = 0;
	}

	if	(lCommand = NewCommand (CAtlString (Caption), CAtlString (VoiceGrammar), CAtlString (VoiceCaption), (Enabled != FALSE), (Visible != FALSE)))
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
				lInsertNdx = mCommands.GetCount();
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::Remove (long CommandID)
{
	HRESULT	lResult = S_OK;

	if	(!RemoveCommand (FindCommand ((USHORT)CommandID)))
	{
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCommands::RemoveAll (void)
{
	HRESULT	lResult = S_OK;

	if	(mCommands.GetCount() <= 0)
	{
		lResult = S_FALSE;
	}
	else
	{
		mRemoved.Append (mCommands);
		mCommands.RemoveAll ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_Count (long *Count)
{
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mCommands.GetCount();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_Caption (BSTR *Caption)
{
	HRESULT	lResult = S_OK;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = mCaption.AllocSysString();
	}

	return lResult;
}

HRESULT CDaCmnCommands::put_Caption (BSTR Caption)
{
	HRESULT		lResult = S_OK;
	CAtlString	lCaption (Caption);

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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_VoiceGrammar (BSTR *VoiceGrammar)
{
	HRESULT	lResult = S_OK;

	if	(!VoiceGrammar)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceGrammar) = mVoiceGrammar.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_VoiceGrammar (BSTR VoiceGrammar)
{
	HRESULT		lResult = S_OK;
	CAtlString	lVoiceGrammar (VoiceGrammar);

	if	(lVoiceGrammar == mVoiceGrammar)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceGrammar = lVoiceGrammar;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_Visible (VARIANT_BOOL *Visible)
{
	HRESULT	lResult = mVisible ? S_OK : S_FALSE;

	if	(Visible)
	{
		(*Visible) = (mVisible ? VARIANT_TRUE : VARIANT_FALSE);
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_Visible (VARIANT_BOOL Visible)
{
	HRESULT	lResult = S_OK;

	if	(mVisible == (Visible != FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mVisible = (Visible != FALSE);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_DefaultCommand (long *CommandID)
{
	HRESULT	lResult = S_OK;

	if	(!CommandID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CommandID) = (long)mDefaultId;
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_DefaultCommand (long CommandID)
{
	HRESULT	lResult = S_OK;

	if	(mDefaultId == (USHORT)CommandID)
	{
		lResult = S_FALSE;
	}
	else
	{
		mDefaultId = (USHORT)CommandID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_FontName (BSTR *FontName)
{
	HRESULT	lResult = S_OK;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontName) = mFontName.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_FontName (BSTR FontName)
{
	HRESULT		lResult = S_OK;
	CAtlString	lFontName (FontName);

	if	(mFontName == lFontName)
	{
		lResult = S_FALSE;
	}
	else
	{
		mFontName = lFontName;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_FontSize (long *FontSize)
{
	HRESULT	lResult = S_OK;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontSize) = mFontSize;
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_FontSize (long FontSize)
{
	HRESULT	lResult = S_OK;

	if	(mFontSize == FontSize)
	{
		lResult = S_FALSE;
	}
	else
	{
		mFontSize = FontSize;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_VoiceCaption (BSTR *VoiceCaption)
{
	HRESULT	lResult = S_OK;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = mVoiceCaption.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_VoiceCaption (BSTR VoiceCaption)
{
	HRESULT		lResult = S_OK;
	CAtlString	lVoiceCaption (VoiceCaption);

	if	(lVoiceCaption == mVoiceCaption)
	{
		lResult = S_FALSE;
	}
	else
	{
		mVoiceCaption = lVoiceCaption;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommands::get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enabled)
{
	HRESULT	lResult = mGlobalVoiceCommandsEnabled ? S_OK : S_FALSE;

	if	(Enabled)
	{
		(*Enabled) = (mGlobalVoiceCommandsEnabled) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCommands::put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled)
{
	HRESULT	lResult = S_OK;

	if	(mGlobalVoiceCommandsEnabled == (Enabled != VARIANT_FALSE))
	{
		lResult = S_FALSE;
	}
	else
	{
		mGlobalVoiceCommandsEnabled = (Enabled != VARIANT_FALSE);
	}
	return lResult;
}
