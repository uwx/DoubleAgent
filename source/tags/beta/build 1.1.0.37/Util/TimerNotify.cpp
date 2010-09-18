/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include "TimerNotify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CTimerNotify::CTimerNotify (UINT_PTR pTimerId, _ITimerNotifySink * pNotifySink)
:	mNotifySink (pNotifySink),
	mTimerId (pTimerId),
	mTimerIdSet (0),
	mTimerWnd (NULL)
{
}

CTimerNotify::~CTimerNotify ()
{
	if	(mTimerIdSet)
	{
		StopTimer (mTimerWnd);
	}
}

//////////////////////////////////////////////////////////////////////

bool CTimerNotify::StartTimer (HWND pTimerWnd, DWORD pInterval)
{
	if	(
			(!pTimerWnd)
		||	(IsWindow (pTimerWnd))
		)
	{
		if	(mTimerIdSet = ::SetTimer (pTimerWnd, mTimerId, pInterval, NULL))
		{
			mTimerWnd = pTimerWnd;
			return true;
		}
	}
	return false;
}

bool CTimerNotify::StopTimer (HWND pTimerWnd)
{
	if	(pTimerWnd == mTimerWnd)
	{
		if	(mTimerIdSet)
		{
			::KillTimer (mTimerWnd, mTimerIdSet);
			mTimerIdSet = 0;
		}
		mTimerWnd = NULL;
		return true;
	}
	return false;
}

bool CTimerNotify::OnTimer (UINT_PTR pTimerId)
{
	if	(pTimerId == mTimerIdSet)
	{
		_ITimerNotifySink *	lNotifySink;

		if	(lNotifySink = mNotifySink)
		{
			try
			{
				lNotifySink->OnTimerNotify (this, mTimerId); // Might delete this instance
			}
			catch AnyExceptionSilent
		}
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CTimerNotify::SetNotifySink (_ITimerNotifySink * pNotifySink)
{
	if	(
			(pNotifySink)
		&&	(mNotifySink != pNotifySink)
		)
	{
		mNotifySink = pNotifySink;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CTimerNotifies::CTimerNotifies ()
{
}

CTimerNotifies::~CTimerNotifies ()
{
}

//////////////////////////////////////////////////////////////////////

INT_PTR CTimerNotifies::FindTimer (UINT_PTR pTimerId) const
{
	INT_PTR					lNdx;
	const CTimerNotify *	lTimer;

	for	(lNdx = 0; lNdx < (INT_PTR)GetCount(); lNdx++)
	{
		if	(
				(lTimer = GetAt (lNdx))
			&&	(lTimer->GetTimerId () == pTimerId)
			)
		{
			return lNdx;
		}
	}
	return -1;
}

CTimerNotify * CTimerNotifies::GetTimer (UINT_PTR pTimerId)
{
	INT_PTR	lNdx = FindTimer (pTimerId);

	if	(lNdx >= 0)
	{
		return GetAt (lNdx);
	}
	return NULL;
}

bool CTimerNotifies::RemoveTimer (UINT_PTR pTimerId)
{
	INT_PTR	lNdx = FindTimer (pTimerId);

	if	(lNdx >= 0)
	{
		DeleteAt (lNdx);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CTimerNotifies::OnTimer (UINT_PTR pTimerId)
{
	INT_PTR			lNdx;
	CTimerNotify *	lTimer;

	for	(lNdx = 0; lNdx < (INT_PTR)GetCount(); lNdx++)
	{
		if	(
				(lTimer = GetAt (lNdx))
			&&	(lTimer->GetTimerStarted () == pTimerId)
			)
		{
			return lTimer->OnTimer (pTimerId);
		}
	}
	return false;
}
