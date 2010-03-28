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
#ifndef DASERVERLIFETIME_H_INCLUDED_
#define DASERVERLIFETIME_H_INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CDaObjectLifetime
{
public:
	CDaObjectLifetime ();
	virtual ~CDaObjectLifetime ();

// Attributes
public:

// Operations
public:
	virtual bool VerifyClientLifetime ();
	virtual void OnClientEnded ();

// Implementation
protected:
	bool ManageObjectLifetime (CCmdTarget * pObject, LPCTSTR pClientMutexName = NULL);
	void UnmanageObjectLifetime (CCmdTarget * pObject);

protected:
	CString			mClientMutexName;
private:
	tPtr <CMutex>	mClientMutex;
};

/////////////////////////////////////////////////////////////////////////////

class CDaServerLifetime
{
public:
	CDaServerLifetime ();
	virtual ~CDaServerLifetime ();

// Attributes
public:

// Operations
public:
	void ManageObjectLifetime (CDaObjectLifetime * pObject);
	void UnmanageObjectLifetime (CDaObjectLifetime * pObject);

// Implementation
protected:
	void VerifyObjectLifetimes ();

private:
	::CCriticalSection					mCriticalSection;
	CPtrTypeArray <CDaObjectLifetime>	mObjectLifetimes;
	DWORD								mLastVerifyTime;
	const DWORD							mVerifyInterval;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DASERVERLIFETIME_H_INCLUDED_
