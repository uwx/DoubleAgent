/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef TIMERNOTIFY_H_INCLUDED_
#define TIMERNOTIFY_H_INCLUDED_
#pragma once

/**/#ifndef	__ATLCOLL_H__
#ifdef	__AFXCOLL_H__
#include "AfxTemplEx.h"
#endif
/**/#endif

//////////////////////////////////////////////////////////////////////

interface _ITimerNotifySink
{
	virtual void OnTimerNotify (class CTimerNotify* pTimerNotify, UINT_PTR pTimerId) = 0;
};

//////////////////////////////////////////////////////////////////////

class CTimerNotify
{
public:
	CTimerNotify (UINT_PTR pTimerId, _ITimerNotifySink * pNotifySink);
	virtual ~CTimerNotify ();

// Attributes
	_ITimerNotifySink * GetNotifySink () const {return mNotifySink;}
	UINT_PTR GetTimerId () const {return mTimerId;}
	UINT_PTR GetTimerStarted () const {return mTimerIdSet;}
	HWND GetTimerWnd () const {return mTimerWnd;}

// Operations
public:
	bool StartTimer (HWND pTimerWnd, DWORD pInterval);
	bool StopTimer (HWND pTimerWnd);
	bool OnTimer (UINT_PTR pTimerId);

	bool SetNotifySink (_ITimerNotifySink * pNotifySink);

// Implementation
private:
	_ITimerNotifySink *	mNotifySink;
	const UINT_PTR		mTimerId;
	UINT_PTR			mTimerIdSet;
	HWND				mTimerWnd;
};

//////////////////////////////////////////////////////////////////////

class CTimerNotifies : public COwnPtrArray <CTimerNotify>
{
public:
	CTimerNotifies ();
	~CTimerNotifies ();

// Operations
public:
	INT_PTR FindTimer (UINT_PTR pTimerId) const;
	CTimerNotify* GetTimer (UINT_PTR pTimerId);
	bool RemoveTimer (UINT_PTR pTimerId);

	bool OnTimer (UINT_PTR pTimerId);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(TIMERNOTIFY_H_INCLUDED_)
