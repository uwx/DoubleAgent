/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "NotifyObjects.h"

/////////////////////////////////////////////////////////////////////////////

interface _IEventNotify
{
	virtual void _CharacterLoaded (long pCharID) {}
	virtual void _CharacterUnloaded (long pCharID) {}
	virtual void _CharacterNameChanged (long pCharID) {}
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) {}
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause) {}
	virtual bool _DownloadComplete (class CFileDownload* pDownload) {return false;}
	virtual class CFileDownload* _FindSoundDownload (LPCTSTR pSoundUrl) {return NULL;}
	virtual bool _ContextMenu (long pCharID, HWND pOwner, const CPoint& pPosition) {return false;}
	virtual bool _DefaultCommand (long pCharID, HWND pOwner, const CPoint& pPosition) {return false;}
	virtual void _AppActivated (bool pActive) {}
	virtual void _OptionsChanged () {}
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid, LPCTSTR pFilePath) {}
};

/////////////////////////////////////////////////////////////////////////////

interface _IEventReflect
{
	virtual void _OnCharacterLoaded (long pCharID) {}
	virtual void _OnCharacterUnloaded (long pCharID) {}
	virtual void _OnCharacterNameChanged (long pCharID) {}
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) {}
	virtual bool _OnDownloadComplete (class CFileDownload* pDownload) {return false;}
	virtual class CFileDownload* _FindSoundDownload (LPCTSTR pSoundUrl) {return NULL;}
	virtual bool _OnContextMenu (long pCharID, HWND pOwner, const CPoint& pPosition) {return false;}
	virtual bool _OnDefaultCommand (long pCharID, HWND pOwner, const CPoint& pPosition) {return false;}
	virtual void _OnAppActivated (bool pActive) {}
	virtual void _OnOptionsChanged () {}
	virtual void _OnDefaultCharacterChanged (REFGUID pCharGuid, LPCTSTR pFilePath) {}
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

class CEventNotify : public IDaSvrNotifySink2, public _IEventNotify, public CNotifySinksOwner <_IEventReflect>, public CNotifySinksOwner <_IEventLock>
{
public:
	CEventNotify ();
	virtual ~CEventNotify ();

// Attributes
	class CInstanceAnchor*	mAnchor;
	_IEventNotify *			mGlobal;

	typedef CNotifySinksOwner <_IEventReflect> ReflectSinks;
	typedef CNotifySinksOwner <_IEventLock> LockSinks;

// Operations
public:
	virtual long NextReqID ();
	virtual class CAgentWnd* GetRequestOwner (long pReqID);
	virtual class CAgentWnd* GetAgentWnd (HWND pWindow);
	virtual long GetActiveClient (long pCharID, bool pUseDefault = true);
	virtual long GetNotifyClient (long pCharID, bool pUseDefault = true);

	virtual bool ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual bool ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);

	virtual VisibilityCauseType _GetVisibilityCause (long pCharID);
	virtual void _PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause);
	virtual MoveCauseType _GetMoveCause (long pCharID);
	virtual void _PutMoveCause (long pCharID, MoveCauseType pMoveCause);

	virtual HRESULT OnSpeechStart (long CharacterID, LPUNKNOWN FormattedText) = 0;
	virtual HRESULT OnSpeechEnd (long CharacterID, LPUNKNOWN FormattedText, VARIANT_BOOL Stopped) = 0;
	virtual HRESULT OnSpeechWord (long CharacterID, LPUNKNOWN FormattedText, long WordIndex) = 0;

// Overrides
public:
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause);
	virtual bool _DownloadComplete (class CFileDownload* pDownload);
	virtual class CFileDownload* _FindSoundDownload (LPCTSTR pSoundUrl);
	virtual bool _ContextMenu (long pCharID, HWND pOwner, const CPoint& pPosition);
	virtual bool _DefaultCommand (long pCharID, HWND pOwner, const CPoint& pPosition);
	virtual void _AppActivated (bool pActive);
	virtual void _OptionsChanged ();
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid, LPCTSTR pFilePath);

// Implementation
protected:
	virtual bool PreFireEvent (LPCTSTR pEventName = NULL);
	virtual bool PostFireEvent (LPCTSTR pEventName = NULL, UINT pEventSinkCount = 0);

protected:
//	CAtlPtrTypeArray <_IEventReflect>	mEventReflect;
//	CAtlPtrTypeArray <_IEventLock>		mEventLock;
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
	virtual void _AppActivated (bool pActive);
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
	CEventNotify*	mNotify;

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

	CEventNotify* GetActiveClientNotify (long pCharID, bool pUseDefault = true);
	CEventNotify* GetNotifyClientNotify (long pCharID, bool pUseDefault = true);
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

template <class aBase>
CEventNotify* CEventNotifiesClient<aBase>::GetActiveClientNotify (long pCharID, bool pUseDefault)
{
	int				lNotifyNdx;
	CEventNotify*	lNotify;

	for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
	{
		if	(lNotify->GetActiveClient (pCharID, pUseDefault) == pCharID)
		{
			return lNotify;
		}
	}
	return NULL;
}

template <class aBase>
CEventNotify* CEventNotifiesClient<aBase>::GetNotifyClientNotify (long pCharID, bool pUseDefault)
{
	int				lNotifyNdx;
	CEventNotify*	lNotify;

	for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
	{
		if	(lNotify->GetNotifyClient (pCharID, pUseDefault) == pCharID)
		{
			return lNotify;
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
