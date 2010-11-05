/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCore.h"
#include "QueuedActions.h"

//////////////////////////////////////////////////////////////////////

CQueuedActions::CQueuedActions ()
:	mTimer (0),
	mTime (0),
	mTimeMin (10),
	mTimeMax (200),
	mTimeDefault (100)
{
}

CQueuedActions::~CQueuedActions ()
{
	try
	{
		DeleteAll ();
	}
	catch AnyExceptionSilent
	RemoveAll ();
}

//////////////////////////////////////////////////////////////////////

UINT CQueuedActions::HasActions (long pCharID) const
{
	if	(pCharID >= 0)
	{
		UINT			lRet = 0;
		CQueuedAction *	lQueuedAction;
		POSITION		lPos;

		for	(lPos = GetHeadPosition (); lPos;)
		{
			if	(
					(lQueuedAction = GetNext (lPos))
				&&	(lQueuedAction->mCharID == pCharID)
				)
			{
				lRet++;
			}
		}
		return lRet;
	}
	else
	{
		return (UINT)GetCount();
	}
}

//////////////////////////////////////////////////////////////////////

CQueuedAction * CQueuedActions::GetCharAction (QueueAction pAction, long pCharID, bool pReverseOrder) const
{
	CQueuedAction *	lRet = NULL;
	CQueuedAction *	lQueuedAction;
	POSITION		lPos;

	if	(!IsEmpty ())
	{
		if	(pReverseOrder)
		{
			for	(lPos = GetTailPosition (); lPos;)
			{
				if	(
						(lQueuedAction = GetPrev (lPos))
					&&	(lQueuedAction->mAction == pAction)
					&&	(
							(pCharID <= 0)
						||	(lQueuedAction->mCharID == pCharID)
						)
					)
				{
					lRet = lQueuedAction;
					break;
				}
			}
		}
		else
		{
			for	(lPos = GetHeadPosition (); lPos;)
			{
				if	(
						(lQueuedAction = GetNext (lPos))
					&&	(lQueuedAction->mAction == pAction)
					&&	(
							(pCharID <= 0)
						||	(lQueuedAction->mCharID == pCharID)
						)
					)
				{
					lRet = lQueuedAction;
					break;
				}
			}
		}
	}
	return lRet;
}

CQueuedAction * CQueuedActions::GetNextAction () const
{
	CQueuedAction *	lRet = NULL;

	if	(!IsEmpty ())
	{
		lRet = GetHead ();
	}
	return lRet;
}

CQueuedAction * CQueuedActions::GetNextAction (QueueAction pAction) const
{
	CQueuedAction *	lRet = NULL;
	CQueuedAction *	lQueuedAction;

	if	(
			(!IsEmpty ())
		&&	(lQueuedAction = GetHead ())
		&&	(lQueuedAction->mAction == pAction)
		)
	{
		lRet = lQueuedAction;
	}
	return lRet;
}

CQueuedAction * CQueuedActions::FindNextAction (long pCharID) const
{
	CQueuedAction *	lRet = NULL;

	if	(pCharID >= 0)
	{
		CQueuedAction *	lQueuedAction;
		POSITION		lPos;

		for	(lPos = GetHeadPosition (); lPos;)
		{
			if	(
					(lQueuedAction = GetNext (lPos))
				&&	(lQueuedAction->mCharID == pCharID)
				)
			{
				lRet = lQueuedAction;
				break;
			}
		}
	}
	else
	if	(!IsEmpty ())
	{
		lRet = GetHead();
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

void CQueuedActions::PushQueue (CAtlOwnPtrList <CQueuedAction> & pQueue)
{
	CQueuedAction *	lAction;

	while	(
				(!IsEmpty ())
			&&	(lAction = RemoveHead ())
			)
	{
		pQueue.AddTail (lAction);
	}
}

void CQueuedActions::PopQueue (CAtlOwnPtrList <CQueuedAction> & pQueue)
{
	CQueuedAction *	lAction;

	while	(
				(!pQueue.IsEmpty ())
			&&	(lAction = pQueue.RemoveHead ())
			)
	{
		AddTail (lAction);
	}
}
