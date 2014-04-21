/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#pragma once
#include "AtlCollEx.h"

/////////////////////////////////////////////////////////////////////////////

class CSvrObjLifetime
{
public:
	CSvrObjLifetime ();
	virtual ~CSvrObjLifetime ();

// Attributes
public:

// Operations
public:
	virtual bool VerifyClientLifetime ();
	virtual void OnClientEnded ();

// Implementation
protected:
	bool ManageObjectLifetime (CComObjectRootBase * pObject, LPCTSTR pClientMutexName = NULL);
	void UnmanageObjectLifetime (CComObjectRootBase * pObject);
	bool _VerifyClientLifetime () const;

protected:
	CAtlString					mClientMutexName;
private:
	mutable tPtr <CAutoMutex>	mClientMutex;
};

/////////////////////////////////////////////////////////////////////////////

class CServerLifetime
{
public:
	CServerLifetime ();
	virtual ~CServerLifetime ();

// Attributes
public:
	bool	mObjectWasAbandoned;

// Operations
public:
	void ManageObjectLifetime (CSvrObjLifetime * pObject);
	void UnmanageObjectLifetime (CSvrObjLifetime * pObject);
	void VerifyObjectLifetimes (bool pImmediate = false);

// Implementation
private:
	CComAutoCriticalSection				mCriticalSection;
	CAtlPtrTypeArray <CSvrObjLifetime>	mObjectLifetimes;
	DWORD								mLastVerifyTime;
	const DWORD							mVerifyInterval;
};

/////////////////////////////////////////////////////////////////////////////
