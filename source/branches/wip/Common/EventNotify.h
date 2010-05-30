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
	void RegisterEventReflect (_IEventReflect * pEventReflect, bool pRegister);

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
