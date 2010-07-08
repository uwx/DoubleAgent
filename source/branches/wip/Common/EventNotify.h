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
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////

interface _IEventNotify
{
	virtual void _CharacterLoaded (long pCharID) {}
	virtual void _CharacterUnloaded (long pCharID) {}
	virtual void _CharacterNameChanged (long pCharID) {}
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) {}
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause) {}
	virtual bool _DownloadComplete (class CFileDownload * pDownload) {return false;}
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl) {return NULL;}
	virtual bool _ContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition) {return false;}
	virtual bool _DefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition) {return false;}
	virtual void _OptionsChanged () {}
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid) {}
};

/////////////////////////////////////////////////////////////////////////////

interface _IEventReflect
{
	virtual void _OnCharacterLoaded (long pCharID) {}
	virtual void _OnCharacterUnloaded (long pCharID) {}
	virtual void _OnCharacterNameChanged (long pCharID) {}
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) {}
	virtual bool _OnDownloadComplete (class CFileDownload * pDownload) {return false;}
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl) {return NULL;}
	virtual bool _OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition) {return false;}
	virtual bool _OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition) {return false;}
	virtual void _OnOptionsChanged () {}
	virtual void _OnDefaultCharacterChanged () {}
};

/////////////////////////////////////////////////////////////////////////////

interface _IEventLock
{
	virtual bool _PreNotify () {return true;}
	virtual bool _PostNotify () {return true;}
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CEventNotify : public IDaSvrNotifySink, public _IEventNotify
{
public:
	CEventNotify ();
	virtual ~CEventNotify ();

// Attributes
	class CInstanceAnchor *	mAnchor;
	_IEventNotify *			mGlobal;

// Operations
public:
	virtual void RegisterEventReflect (_IEventReflect * pEventReflect, bool pRegister);
	virtual void RegisterEventLock (_IEventLock * pEventLock, bool pRegister);

	virtual long NextReqID ();
	virtual class CAgentWnd * GetRequestOwner (long pReqID);
	virtual class CAgentWnd * GetAgentWnd (HWND pWindow);
	virtual long GetActiveClient (long pCharID, bool pUseDefault = true);
	virtual long GetNotifyClient (long pCharID, bool pUseDefault = true);

	virtual bool ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual bool ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);

	virtual VisibilityCauseType _GetVisibilityCause (long pCharID);
	virtual void _PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause);
	virtual MoveCauseType _GetMoveCause (long pCharID);
	virtual void _PutMoveCause (long pCharID, MoveCauseType pMoveCause);

// Overrides
public:
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause);
	virtual bool _DownloadComplete (class CFileDownload * pDownload);
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl);
	virtual bool _ContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual bool _DefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual void _OptionsChanged ();
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid);

// Implementation
protected:
	virtual bool PreFireEvent (LPCTSTR pEventName);
	virtual bool PostFireEvent (LPCTSTR pEventName);

protected:
	CAtlPtrTypeArray <_IEventReflect>	mEventReflect;
	CAtlPtrTypeArray <_IEventLock>		mEventLock;
	CAtlMap <long, CZeroInit<long> >	mVisibilityCause;
	CAtlMap <long, CZeroInit<long> >	mMoveCause;
};

/////////////////////////////////////////////////////////////////////////////

class CEventGlobal : public virtual _IEventNotify
{
public:
	CEventGlobal ();
	virtual ~CEventGlobal ();

// Attributes
public:
	CAtlPtrTypeArray <CEventNotify>	mInstanceNotify;

// Overrides
public:
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause);
	virtual void _OptionsChanged ();
	virtual void _DefaultCharacterChanged ();
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

template <class aBase> class CEventNotifyHolder : public _IEventLock
{
public:
	CEventNotifyHolder ();
	virtual ~CEventNotifyHolder () {}

// Operations
public:
	bool PreNotify ();
	bool PostNotify ();
	UINT IsInNotify () const;

// Overrides
protected:
	virtual bool _PreNotify ();
	virtual bool _PostNotify ();

// Implementation
private:
	UINT	mInNotify;
};

template <class aBase> class CEventNotifyClient : public CEventNotifyHolder<aBase>
{
public:
	CEventNotifyClient ();
	virtual ~CEventNotifyClient () {}

// Attributes
public:
	CEventNotify *	mNotify;

// Operations
public:
	bool PreNotify ();
};

template <class aBase> class CEventNotifiesClient : public CEventNotifyHolder<aBase>
{
public:
	CEventNotifiesClient () {}
	virtual ~CEventNotifiesClient () {}

// Attributes
public:
	CAtlPtrTypeArray <CEventNotify>	mNotify;

// Operations
public:
	bool PreNotify ();
};

/////////////////////////////////////////////////////////////////////////////

template <class aBase>
CEventNotifyHolder<aBase>::CEventNotifyHolder ()
:	mInNotify (0)
{
}

template <class aBase>
bool CEventNotifyHolder<aBase>::PreNotify ()
{
	if	(
			((aBase *)this != NULL)
		&&	(_PreNotify ())
		)
	{
		return true;
	}
	return false;
}

template <class aBase>
bool CEventNotifyHolder<aBase>::PostNotify ()
{
	if	((aBase *)this != NULL)
	{
		return _PostNotify ();
	}
	return false;
}

template <class aBase>
UINT CEventNotifyHolder<aBase>::IsInNotify () const
{
	if	((aBase *)this != NULL)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

template <class aBase>
bool CEventNotifyHolder<aBase>::_PreNotify ()
{
	mInNotify++;
	return true;
}

template <class aBase>
bool CEventNotifyHolder<aBase>::_PostNotify ()
{
	if	(mInNotify > 0)
	{
		mInNotify--;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

template <class aBase>
CEventNotifyClient<aBase>::CEventNotifyClient ()
:	mNotify (NULL)
{
}

template <class aBase>
bool CEventNotifyClient<aBase>::PreNotify ()
{
	if	(
			((aBase *)this != NULL)
		&&	(mNotify)
		&&	(_PreNotify ())
		)
	{
		return true;
	}
	return false;
}

template <class aBase>
bool CEventNotifiesClient<aBase>::PreNotify ()
{
	if	(
			((aBase *)this != NULL)
		&&	(mNotify.GetCount() > 0)
		&&	(_PreNotify ())
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
