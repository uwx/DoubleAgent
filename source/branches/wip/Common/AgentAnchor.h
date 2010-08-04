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
#include "AgentFileCache.h"
#include "EventNotify.h"

/////////////////////////////////////////////////////////////////////////////

class CGlobalAnchor : public CAgentFileCache
{
public:
	CGlobalAnchor ();
	virtual ~CGlobalAnchor ();

// Attributes
	virtual bool IsAppActive () const;

// Operations
public:
	virtual long NextCharID ();
	virtual long NextReqID ();
	virtual class CAgentWnd * GetRequestOwner (long pReqID);
	virtual class CAgentWnd * GetAgentWnd (HWND pWindow);

	virtual class CDaCmnCharacter * GetGlobalCharacter (long pCharID);
	virtual long GetActiveCharacter ();
	virtual long GetListenCharacter ();

// Implementation
public:
	static class CDaCmnCharacter * _GetCharacter (long pCharID, CAgentFileCache & pFileCache);
	static long _GetActiveCharacter (CAgentFileCache & pFileCache);
	static long _GetListenCharacter (CAgentFileCache & pFileCache);

protected:
	long	mNextCharID;
	long	mNextReqID;
};

/////////////////////////////////////////////////////////////////////////////

class CInstanceAnchor : public CAgentFileCache
{
public:
	CInstanceAnchor (CGlobalAnchor & pAnchor);
	virtual ~CInstanceAnchor ();

// Attributes
	CGlobalAnchor &	mAnchor;

// Operations
public:
	virtual class CDaCmnCharacter * GetInstanceCharacter (long pCharID);
	virtual class CDaCmnCharacter * GetDefaultCharacter ();
	virtual long GetActiveClient (long pCharID);
	virtual long GetNotifyClient (long pCharID);
};

/////////////////////////////////////////////////////////////////////////////
