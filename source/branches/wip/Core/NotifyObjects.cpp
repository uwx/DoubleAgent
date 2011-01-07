/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
#include "NotifyObjects.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////

CNotifyObjects::CNotifyObjects ()
:	mNextObjectId (1)
{
}

CNotifyObjects::~CNotifyObjects ()
{
}

//////////////////////////////////////////////////////////////////////

ULONG CNotifyObjects::AddNotifyObject (void * pNotifyObject, void * pSourceObject, CNotifyObjects * pTargetObjects)
{
	ULONG	lObjectId = 0;

	if	(
			(pNotifyObject)
		&&	(!FindNotifyObject (pNotifyObject))
		)
	{
		mNotifyObjects.Add (tNotifyObject (pNotifyObject, lObjectId = mNextObjectId++));
#ifdef	_TRACE_NOTIFY_OBJECTS
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::AddNotifyObject [%u] [%p] [%p]"), pSourceObject, typeid(*this).name(), lObjectId, pNotifyObject, pTargetObjects);
#endif

		if	(
				(pSourceObject)
			&&	(pTargetObjects)
			)
		{
			pTargetObjects->AddNotifyObject (pSourceObject, pNotifyObject, NULL);
		}
	}
	return lObjectId;
}

bool CNotifyObjects::RemoveNotifyObject (void * pNotifyObject, void * pSourceObject, CNotifyObjects * pTargetObjects)
{
	bool	lRet = false;
	INT_PTR	lNdx;

	if	(pNotifyObject)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mNotifyObjects.GetCount(); lNdx++)
		{
			if	(mNotifyObjects [lNdx].mObject == pNotifyObject)
			{
#ifdef	_TRACE_NOTIFY_OBJECTS
				LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::RemoveNotifyObject [%u] [%p] [%p]"), pSourceObject, typeid(*this).name(), mNotifyObjects [lNdx].mObjectId, mNotifyObjects [lNdx].mObject, pTargetObjects);
#endif
				mNotifyObjects.RemoveAt (lNdx);
				lRet = true;
				break;
			}
		}
		if	(
				(pSourceObject)
			&&	(pTargetObjects)
			)
		{
			pTargetObjects->RemoveNotifyObject (pSourceObject, pNotifyObject, NULL);
		}
	}
	return lRet;
}

bool CNotifyObjects::RemoveNotifyObject (INT_PTR pObjectNdx, void * pSourceObject, CNotifyObjects * pTargetObjects)
{
	bool	lRet = false;
	void *	lNotifyObject = NULL;

	if	(
			(pObjectNdx >= 0)
		&&	(pObjectNdx < (INT_PTR)mNotifyObjects.GetCount())
		)
	{
#ifdef	_TRACE_NOTIFY_OBJECTS
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::RemoveNotifyObject [%u] [%p] [%p]"), pSourceObject, typeid(*this).name(), mNotifyObjects [pObjectNdx].mObjectId, mNotifyObjects [pObjectNdx].mObject, pTargetObjects);
#endif
		lNotifyObject = mNotifyObjects [pObjectNdx].mObject;
		mNotifyObjects.RemoveAt (pObjectNdx);
		lRet = true;

		if	(
				(lNotifyObject)
			&&	(pSourceObject)
			&&	(pTargetObjects)
			)
		{
			pTargetObjects->RemoveNotifyObject (pSourceObject, lNotifyObject, NULL);
		}
	}
	return lRet;
}

bool CNotifyObjects::ClearNotifyObjects ()
{
	bool	lRet = (mNotifyObjects.GetCount() > 0);

	mNotifyObjects.RemoveAll ();
	return lRet;
}

//////////////////////////////////////////////////////////////////////

INT_PTR CNotifyObjects::GetNotifyObjectCount () const
{
	return (INT_PTR)mNotifyObjects.GetCount();
}

void * CNotifyObjects::GetNotifyObject (INT_PTR pNdx) const
{
	if	(
			(pNdx >= 0)
		&&	(pNdx < (INT_PTR)mNotifyObjects.GetCount())
		)
	{
		return mNotifyObjects [pNdx].mObject;
	}
	return NULL;
}

ULONG CNotifyObjects::FindNotifyObject (const void * pNotifyObject) const
{
	INT_PTR	lNdx;

	if	(pNotifyObject)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mNotifyObjects.GetCount(); lNdx++)
		{
			if	(mNotifyObjects [lNdx].mObject == pNotifyObject)
			{
				return mNotifyObjects [lNdx].mObjectId;
			}
		}
	}
	return 0;
}

void * CNotifyObjects::FindNotifyObject (ULONG pNotifyObjectId) const
{
	INT_PTR	lNdx;

	if	(pNotifyObjectId)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mNotifyObjects.GetCount(); lNdx++)
		{
			if	(mNotifyObjects [lNdx].mObjectId == pNotifyObjectId)
			{
				return mNotifyObjects [lNdx].mObject;
			}
		}
	}
	return 0;
}
