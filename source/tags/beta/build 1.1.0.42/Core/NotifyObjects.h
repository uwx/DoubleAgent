/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
#ifndef NOTIFYOBJECTS_H_INCLUDED_
#define NOTIFYOBJECTS_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "AtlUtil.h"
#include "AtlCollEx.h"

//////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
#include <typeinfo.h>
//#define	_TRACE_NOTIFY_OBJECTS	LogNormal|LogTimeMs|LogHighVolume
#endif

//////////////////////////////////////////////////////////////////////

class CNotifyObjects
{
protected:
	_DACORE_IMPEXP CNotifyObjects ();
	_DACORE_IMPEXP virtual ~CNotifyObjects ();

protected:
	_DACORE_IMPEXP virtual ULONG AddNotifyObject (void * pNotifyObject, void * pSourceObject, CNotifyObjects * pTargetObjects);
	_DACORE_IMPEXP virtual bool RemoveNotifyObject (void * pNotifyObject, void * pSourceObject, CNotifyObjects * pTargetObjects);
	_DACORE_IMPEXP virtual bool RemoveNotifyObject (INT_PTR pObjectNdx, void * pSourceObject, CNotifyObjects * pTargetObjects);
	_DACORE_IMPEXP virtual bool ClearNotifyObjects ();

	_DACORE_IMPEXP virtual INT_PTR GetNotifyObjectCount () const;
	_DACORE_IMPEXP virtual void * GetNotifyObject (INT_PTR pNdx) const;
	_DACORE_IMPEXP virtual ULONG FindNotifyObject (const void * pNotifyObject) const;
	_DACORE_IMPEXP virtual void * FindNotifyObject (ULONG pNotifyObjectId) const;

private:
	struct tNotifyObject
	{
		void *	mObject;
		ULONG	mObjectId;
		tNotifyObject (void * pObject = NULL, ULONG pObjectId = 0) : mObject (pObject), mObjectId (pObjectId) {}
	};
	CAtlStructArray <tNotifyObject>	mNotifyObjects;
	ULONG							mNextObjectId;
};

//////////////////////////////////////////////////////////////////////

template <class aSink> class CNotifySinksOwner : public CNotifyObjects
{
public:
	CNotifySinksOwner () {}
	~CNotifySinksOwner () {ClearNotifySinks ();}

public:
	ULONG AddNotifySink (aSink * pNotifySink);
	bool RemoveNotifySink (aSink * pNotifySink);
	bool ClearNotifySinks ();

	INT_PTR GetNotifySinkCount () const {return GetNotifyObjectCount();}
	aSink * GetNotifySink (INT_PTR pNdx) const {return (aSink *) GetNotifyObject (pNdx);}
	ULONG FindNotifySink (const aSink * pNotifySink) const {return FindNotifyObject (pNotifySink);}
	aSink * FindNotifySink (ULONG pNotifySinkId) const {return (aSink *) FindNotifyObject (pNotifySinkId);}
};

//////////////////////////////////////////////////////////////////////

template <class aSource> class CNotifySourcesOwner : public CNotifyObjects
{
public:
	CNotifySourcesOwner () {}
	~CNotifySourcesOwner () {ClearNotifySources ();}

public:
	ULONG AddNotifySource (aSource * pNotifySource);
	bool RemoveNotifySource (aSource * pNotifySource);
	bool ClearNotifySources ();

	INT_PTR GetNotifySourceCount () const {return GetNotifyObjectCount();}
	aSource * GetNotifySource (INT_PTR pNdx) const {return (aSource *) GetNotifyObject (pNdx);}
	ULONG FindNotifySource (const aSource * pNotifySource) const {return FindNotifyObject (pNotifySource);}
	aSource * FindNotifySource (ULONG pNotifySourceId) const {return (aSource *) FindNotifyObject (pNotifySourceId);}
};

//////////////////////////////////////////////////////////////////////

template <class aSink, class aSource> class CNotifySinksOwner2 : public CNotifySinksOwner <aSink>
{
public:
	CNotifySinksOwner2 () {}
	~CNotifySinksOwner2 () {ClearNotifySinks ();}

public:
	ULONG AddNotifySink (aSink * pNotifySink);
	bool RemoveNotifySink (aSink * pNotifySink);
	bool ClearNotifySinks ();

public:
	virtual __forceinline aSource * _GetNotifySource () {return dynamic_cast <aSource *> (this);}
	virtual __forceinline CNotifySinksOwner <aSink> * _GetNotifySinks () {return this;}
};

//////////////////////////////////////////////////////////////////////

template <class aSource, class aSink> class CNotifySourcesOwner2 : public CNotifySourcesOwner <aSource>
{
public:
	CNotifySourcesOwner2 () {}
	~CNotifySourcesOwner2 () {ClearNotifySources ();}

public:
	ULONG AddNotifySource (aSource * pNotifySource);
	bool RemoveNotifySource (aSource * pNotifySource);
	bool ClearNotifySources ();

public:
	virtual __forceinline aSink * _GetNotifySink () {return dynamic_cast <aSink *> (this);}
	virtual __forceinline CNotifySourcesOwner <aSource> * _GetNotifySources () {return this;}
};

//////////////////////////////////////////////////////////////////////

template <class aSink, class aSource> class CNotifySinkSource : public CNotifySinksOwner2 <aSink, aSource>, public aSource
{
public:
	CNotifySinkSource () {}
	~CNotifySinkSource () {ClearNotifySinks ();}

public:
	virtual __forceinline aSource * _GetNotifySource () {return this;}
	virtual __forceinline CNotifySinksOwner <aSink> * _GetNotifySinks () {return this;}
};

//////////////////////////////////////////////////////////////////////

template <class aSource, class aSink> class CNotifySourceSink : public CNotifySourcesOwner2 <aSource, aSink>, public aSink
{
public:
	CNotifySourceSink () {}
	~CNotifySourceSink () {ClearNotifySources ();}

public:
	virtual __forceinline aSink * _GetNotifySink () {return this;}
	virtual __forceinline CNotifySourcesOwner <aSource> * _GetNotifySources () {return this;}
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

template <class aSink>
ULONG CNotifySinksOwner<aSink>::AddNotifySink (aSink * pNotifySink)
{
	ULONG	lNotifySinkId = AddNotifyObject (pNotifySink, NULL, NULL);
#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(lNotifySinkId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::AddNotifySink [%u] [%p] (%hs)"), this, typeid(*this).name(), lNotifySinkId, pNotifySink, typeid(*pNotifySink).name());
	}
#endif
	return lNotifySinkId;
}

template <class aSink>
bool CNotifySinksOwner<aSink>::RemoveNotifySink (aSink * pNotifySink)
{
#ifdef	_TRACE_NOTIFY_OBJECTS
	ULONG	lNotifySinkId = FindNotifySink (pNotifySink);
	if	(lNotifySinkId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::RemoveNotifySink [%u] [%p] (%hs)"), this, typeid(*this).name(), lNotifySinkId, pNotifySink, typeid(*pNotifySink).name());
	}
#endif
	return RemoveNotifyObject (pNotifySink, NULL, NULL);
}

template <class aSink>
bool CNotifySinksOwner<aSink>::ClearNotifySinks ()
{
#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(GetNotifyObjectCount() > 0)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::ClearNotifySinks [%u] (%hs)"), this, typeid(*this).name(), GetNotifyObjectCount(), typeid(aSink).name());
	}
#endif
	return ClearNotifyObjects ();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

template <class aSource>
ULONG CNotifySourcesOwner<aSource>::AddNotifySource (aSource * pNotifySource)
{
	ULONG	lNotifySourceId = AddNotifyObject (pNotifySource, NULL, NULL);
#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(lNotifySourceId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::AddNotifySource [%u] [%p] (%hs)"), this, typeid(*this).name(), lNotifySourceId, pNotifySource, typeid(*pNotifySource).name());
	}
#endif
	return lNotifySourceId;
}

template <class aSource>
bool CNotifySourcesOwner<aSource>::RemoveNotifySource (aSource * pNotifySource)
{
#ifdef	_TRACE_NOTIFY_OBJECTS
	ULONG	lNotifySourceId = FindNotifySource (pNotifySource);
	if	(lNotifySourceId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::RemoveNotifySource [%u] [%p] (%hs)"), this, typeid(*this).name(), lNotifySourceId, pNotifySource, typeid(*pNotifySource).name());
	}
#endif
	return RemoveNotifyObject (pNotifySource, NULL, NULL);
}

template <class aSource>
bool CNotifySourcesOwner<aSource>::ClearNotifySources ()
{
#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(GetNotifyObjectCount() > 0)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::ClearNotifySources [%u] (%hs)"), this, typeid(*this).name(), GetNotifyObjectCount(), typeid(aSource).name());
	}
#endif
	return ClearNotifyObjects ();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

template <class aSink, class aSource>
ULONG CNotifySinksOwner2<aSink,aSource>::AddNotifySink (aSink * pNotifySink)
{
	ULONG						lNotifySinkId;
	aSource *					lNotifySource;
	CNotifySourcesOwner <aSource> *	lNotifySources;

	try
	{
		lNotifySource = _GetNotifySource();
		lNotifySources = pNotifySink ? pNotifySink->_GetNotifySources() : NULL;
	}
	catch AnyExceptionSilent

	lNotifySinkId = AddNotifyObject (pNotifySink, lNotifySource, lNotifySources);
#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(lNotifySinkId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::AddNotifySink [%u] [%p] (%hs)"), lNotifySource, typeid(*lNotifySource).name(), lNotifySinkId, pNotifySink, typeid(*pNotifySink).name());
	}
#endif
	return lNotifySinkId;
}

template <class aSink, class aSource>
bool CNotifySinksOwner2<aSink,aSource>::RemoveNotifySink (aSink * pNotifySink)
{
	aSource *					lNotifySource;
	CNotifySourcesOwner <aSource> *	lNotifySources;

	try
	{
		lNotifySource = _GetNotifySource();
		lNotifySources = pNotifySink ? pNotifySink->_GetNotifySources() : NULL;
	}
	catch AnyExceptionSilent

#ifdef	_TRACE_NOTIFY_OBJECTS
	ULONG	lNotifySinkId = FindNotifySink (pNotifySink);
	if	(lNotifySinkId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::RemoveNotifySink [%u] [%p] (%hs)"), lNotifySource, typeid(*lNotifySource).name(), lNotifySinkId, pNotifySink, typeid(*pNotifySink).name());
	}
#endif
	return RemoveNotifyObject (pNotifySink, lNotifySource, lNotifySources);
}

template <class aSink, class aSource>
bool CNotifySinksOwner2<aSink,aSource>::ClearNotifySinks ()
{
	bool						lRet = false;
	aSink *						lNotifySink;
	aSource *					lNotifySource;
	CNotifySourcesOwner <aSource> *	lNotifySources;

	try
	{
		lNotifySource = _GetNotifySource();
	}
	catch AnyExceptionSilent

#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(GetNotifyObjectCount() > 0)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::ClearNotifySinks [%u] (%hs)"), lNotifySource, typeid(aSource).name(), GetNotifyObjectCount(), typeid(aSink).name());
	}
#endif
	while	(
				(GetNotifyObjectCount() > 0)
			&&	(lNotifySink = GetNotifySink (0))
			)
	{
		try
		{
			lNotifySources = lNotifySink->_GetNotifySources();
		}
		catch AnyExceptionSilent

		if	(RemoveNotifyObject ((INT_PTR)0, lNotifySource, lNotifySources))
		{
			lRet = true;
		}
		else
		{
#ifdef	_TRACE_NOTIFY_OBJECTS
			LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::ClearNotifySinks [%u] (%hs) failed!"), lNotifySource, typeid(aSource).name(), GetNotifyObjectCount(), typeid(aSink).name());
#endif
			break;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

template <class aSource, class aSink>
ULONG CNotifySourcesOwner2<aSource,aSink>::AddNotifySource (aSource * pNotifySource)
{
	ULONG					lNotifySourceId;
	aSink *					lNotifySink;
	CNotifySinksOwner <aSink> *	lNotifySinks;

	try
	{
		lNotifySink = _GetNotifySink();
		lNotifySinks = pNotifySource ? pNotifySource->_GetNotifySinks() : NULL;
	}
	catch AnyExceptionSilent

	lNotifySourceId = AddNotifyObject (pNotifySource, lNotifySink, lNotifySinks);
#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(lNotifySourceId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::AddNotifySource [%u] [%p] (%hs)"), lNotifySink, typeid(*lNotifySink).name(), lNotifySourceId, pNotifySource, typeid(*pNotifySource).name());
	}
#endif
	return lNotifySourceId;
}

template <class aSource, class aSink>
bool CNotifySourcesOwner2<aSource,aSink>::RemoveNotifySource (aSource * pNotifySource)
{
	aSink *					lNotifySink;
	CNotifySinksOwner <aSink> *	lNotifySinks;

	try
	{
		lNotifySink = _GetNotifySink();
		lNotifySinks = pNotifySource ? pNotifySource->_GetNotifySinks() : NULL;
	}
	catch AnyExceptionSilent

#ifdef	_TRACE_NOTIFY_OBJECTS
	ULONG	lNotifySourceId = FindNotifySource (pNotifySource);
	if	(lNotifySourceId)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::RemoveNotifySource [%u] [%p] (%hs)"), lNotifySink, typeid(*lNotifySink).name(), lNotifySourceId, pNotifySource, typeid(*pNotifySource).name());
	}
#endif
	return RemoveNotifyObject (pNotifySource, lNotifySink, lNotifySinks);
}

template <class aSource, class aSink>
bool CNotifySourcesOwner2<aSource,aSink>::ClearNotifySources ()
{
	bool					lRet = false;
	aSource *				lNotifySource;
	aSink *					lNotifySink;
	CNotifySinksOwner <aSink> *	lNotifySinks;

	try
	{
		lNotifySink = _GetNotifySink();
	}
	catch AnyExceptionSilent

#ifdef	_TRACE_NOTIFY_OBJECTS
	if	(GetNotifyObjectCount() > 0)
	{
		LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::ClearNotifySources [%u] (%hs)"), lNotifySink, typeid(aSink).name(), GetNotifyObjectCount(), typeid(aSource).name());
	}
#endif
	while	(
				(GetNotifyObjectCount() > 0)
			&&	(lNotifySource = GetNotifySource (0))
			)
	{
		try
		{
			lNotifySinks = lNotifySource->_GetNotifySinks();
		}
		catch AnyExceptionSilent

		if	(RemoveNotifyObject ((INT_PTR)0, lNotifySink, lNotifySinks))
		{
			lRet = true;
		}
		else
		{
#ifdef	_TRACE_NOTIFY_OBJECTS
			LogMessage (_TRACE_NOTIFY_OBJECTS, _T("[%p] %hs::ClearNotifySources [%u] (%hs) failed!"), lNotifySink, typeid(aSink).name(), GetNotifyObjectCount(), typeid(aSource).name());
#endif
			break;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(NOTIFYOBJECTS_H_INCLUDED_)
