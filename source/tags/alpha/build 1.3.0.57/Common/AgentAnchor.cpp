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
#include "AgentAnchor.h"
#include "DaCmnCharacter.h"
#include "AgentCharacterWnd.h"
#include "GuidStr.h"

#ifdef	_DEBUG
//#define	_DEBUG_INTERNAL		LogDebug
//#define	_DEBUG_ACTIVATE		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

CGlobalAnchor::CGlobalAnchor ()
:	mNextCharID (256),
	mNextReqID (100)
{
	CAgentFileCache::mIdCode = 1;
}

CGlobalAnchor::~CGlobalAnchor ()
{
}

/////////////////////////////////////////////////////////////////////////////

bool CGlobalAnchor::IsAppActive () const
{
	return true;
}

long CGlobalAnchor::NextCharID ()
{
	return mNextCharID++;
}

long CGlobalAnchor::NextReqID ()
{
	return mNextReqID++;
}

/////////////////////////////////////////////////////////////////////////////

CAgentWnd* CGlobalAnchor::GetRequestOwner (long pReqID)
{
	CAgentWnd*	lRet = NULL;

	try
	{
		INT_PTR		lFileNdx;
		CAgentFile*	lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentWnd*							lAgentWnd;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = dynamic_cast <CAgentWnd*> (lFileClients [lClientNdx]))
						&&	(lAgentWnd->FindQueuedAction (pReqID))
						)
					{
						lRet = lAgentWnd;
						break;
					}
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

CAgentWnd* CGlobalAnchor::GetAgentWnd (HWND pWindow)
{
	CAgentWnd*	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile*	lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentWnd*							lAgentWnd;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = dynamic_cast <CAgentWnd*> (lFileClients [lClientNdx]))
						&&	(lAgentWnd->m_hWnd == pWindow)
						)
					{
						lRet = lAgentWnd;
						break;
					}
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CDaCmnCharacter* CGlobalAnchor::GetGlobalCharacter (long pCharID)
{
	return _GetCharacter (pCharID, *this);
}

long CGlobalAnchor::GetActiveCharacter ()
{
	return _GetActiveCharacter (*this);
}

long CGlobalAnchor::GetListenCharacter ()
{
	return _GetListenCharacter (*this);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCmnCharacter* CGlobalAnchor::_GetCharacter (long pCharID, CAgentFileCache& pFileCache)
{
	CDaCmnCharacter*	lRet = NULL;

	try
	{
		INT_PTR		lFileNdx;
		CAgentFile*	lFile;

		for	(lFileNdx = 0; lFile = pFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter*					lCharacter;

			if	(pFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
						&&	(lCharacter->IsValid (lFile))
						&&	(
								(pCharID < 0)
							||	(lCharacter->GetCharID() == pCharID)
							)
						)
					{
						lRet = lCharacter;
						break;
					}
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

long CGlobalAnchor::_GetActiveCharacter (CAgentFileCache& pFileCache)
{
	long	lRet = 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile*	lFile;

		for	(lFileNdx = 0; lFile = pFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentCharacterWnd*					lAgentWnd;

			if	(pFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = dynamic_cast <CAgentCharacterWnd*> (lFileClients [lClientNdx]))
						&&	(lAgentWnd->IsWindow ())
						&&	(lAgentWnd->GetLastActive() == lAgentWnd->m_hWnd)
						)
					{
						lRet = lAgentWnd->GetCharID();
						break;
					}
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

long CGlobalAnchor::_GetListenCharacter (CAgentFileCache& pFileCache)
{
	long	lRet = 0;

	try
	{
		CDaCmnCharacter*	lActiveCharacter = NULL;
		CDaCmnCharacter*	lClientActiveCharacter = NULL;
		INT_PTR				lFileNdx;
		CAgentFile*		lFile;

		for	(lFileNdx = 0; lFile = pFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter*					lCharacter;

			if	(pFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
						&&	(lCharacter->IsValid (lFile))
						)
					{
						if	(lCharacter->IsInputActive ())
						{
							lActiveCharacter = lCharacter;
							break;
						}
						else
						if	(
								(!lClientActiveCharacter)
							&&	(lCharacter->IsClientActive ())
							)
						{
							lClientActiveCharacter = lCharacter;
						}
					}
				}
			}
			if	(lActiveCharacter)
			{
				break;
			}
		}

		if	(lActiveCharacter)
		{
			lRet = lActiveCharacter->GetCharID();
		}
		else
		if	(lClientActiveCharacter)
		{
			lRet = lClientActiveCharacter->GetCharID();
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CInstanceAnchor::CInstanceAnchor (CGlobalAnchor& pAnchor)
:	mAnchor (pAnchor),
	mOwnerWnd (NULL)
{
	static UINT lCacheId = 1;
	CAgentFileCache::mIdCode = ++lCacheId;
}

CInstanceAnchor::~CInstanceAnchor ()
{
}

/////////////////////////////////////////////////////////////////////////////

CDaCmnCharacter* CInstanceAnchor::GetInstanceCharacter (long pCharID)
{
	return CGlobalAnchor::_GetCharacter (pCharID, *this);
}

CDaCmnCharacter* CInstanceAnchor::GetDefaultCharacter ()
{
	CDaCmnCharacter*	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile*	lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter*					lCharacter;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
						&&	(lCharacter->IsValid (lFile))
						&&	(lCharacter->IsDefault ())
						)
					{
						lRet = lCharacter;
						break;
					}
				}
				if	(lClientNdx >= 0)
				{
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CInstanceAnchor::GetActiveClient (long pCharID)
{
	long	lRet = 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile*	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter*					lCharacter;

			if	(mAnchor.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
						&&	(lCharacter->IsValid (lFile))
						&&	(lCharacter->GetCharID() == pCharID)
						)
					{
						lRet = lCharacter->GetActiveClient ();
						break;
					}
				}
				if	(lClientNdx >= 0)
				{
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

long CInstanceAnchor::GetNotifyClient (long pCharID)
{
	long	lRet = 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile*	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter*					lCharacter;

			if	(mAnchor.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
						&&	(lCharacter->IsValid (lFile))
						&&	(lCharacter->GetCharID() == pCharID)
						)
					{
						break;
					}
				}

				if	(lClientNdx >= 0)
				{
//
//	Find the character for this file in the local cache
//
					if	(GetFileClients (lFile, lFileClients))
					{
						for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
						{
							if	(
									(lCharacter = dynamic_cast <CDaCmnCharacter*> (lFileClients [lClientNdx]))
								&&	(lCharacter->IsValid (lFile))
								)
							{
								lRet = lCharacter->GetCharID ();
								break;
							}
						}
					}
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}
