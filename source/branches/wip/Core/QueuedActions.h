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
#pragma once
#include "QueuedAction.h"

/////////////////////////////////////////////////////////////////////////////

class CQueuedActions : public CAtlOwnPtrList <CQueuedAction>
{
public:
	CQueuedActions ();
	virtual ~CQueuedActions ();

// Attributes
public:
	UINT_PTR	mTimer;
	DWORD		mTime;
	const DWORD	mTimeMin;
	const DWORD	mTimeMax;
	const DWORD	mTimeDefault;

	UINT HasActions (long pCharID = -1) const;
	void LogActions (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;

// Operations
public:
	CQueuedAction * GetCharAction (QueueAction pAction, long pCharID, bool pReverseOrder = false) const;
	CQueuedAction * GetNextAction () const;
	CQueuedAction * GetNextAction (QueueAction pAction) const;
	CQueuedAction * FindNextAction (long pCharID = -1) const;

	void PushQueue (CAtlOwnPtrList <CQueuedAction> & pQueue);
	void PopQueue (CAtlOwnPtrList <CQueuedAction> & pQueue);
};

/////////////////////////////////////////////////////////////////////////////
