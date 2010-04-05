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
#ifndef QUEUEDACTIONS_H_INCLUDED_
#define QUEUEDACTIONS_H_INCLUDED_
#pragma once

#include "QueuedAction.h"

/////////////////////////////////////////////////////////////////////////////

class CQueuedActions : public COwnPtrList <CQueuedAction>
{
public:
	CQueuedActions ();
	virtual ~CQueuedActions ();

// Attributes
public:
	UINT HasActions (long pCharID = -1) const;

// Operations
public:
	CQueuedAction * GetCharAction (QueueAction pAction, long pCharID, bool pReverseOrder = false) const;
	CQueuedAction * GetNextAction () const;
	CQueuedAction * GetNextAction (QueueAction pAction) const;
	CQueuedAction * FindNextAction (long pCharID = -1) const;

	void PushQueue (COwnPtrList <CQueuedAction> & pQueue);
	void PopQueue (COwnPtrList <CQueuedAction> & pQueue);

// Overrides
	//{{AFX_VIRTUAL(CQueuedActions)
	//}}AFX_VIRTUAL

};

/////////////////////////////////////////////////////////////////////////////

#endif // QUEUEDACTIONS_H_INCLUDED_
