/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "EventNotify.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCtlEventDispatch
{
public:
	void FireActivateInput(LPCTSTR CharacterID);
	void FireDeactivateInput(LPCTSTR CharacterID);
	void FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireShow(LPCTSTR CharacterID, VisibilityCauseType Cause);
	void FireHide(LPCTSTR CharacterID, VisibilityCauseType Cause);
	void FireRequestStart(LPDISPATCH Request);
	void FireRequestComplete(LPDISPATCH Request);
	void FireBookmark(long BookmarkID);
	void FireCommand(LPDISPATCH UserInput);
	void FireIdleStart(LPCTSTR CharacterID);
	void FireIdleComplete(LPCTSTR CharacterID);
	void FireMove(LPCTSTR CharacterID, short x, short y, MoveCauseType Cause);
	void FireSize(LPCTSTR CharacterID, short Width, short Height);
	void FireBalloonShow(LPCTSTR CharacterID);
	void FireBalloonHide(LPCTSTR CharacterID);
	void FireListenStart(LPCTSTR CharacterID);
	void FireListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause);
	void FireDefaultCharacterChange(LPCTSTR CharGUID);
	void FireAgentPropertyChange();
	void FireActiveClientChange(LPCTSTR CharacterID, BOOL Active);

protected:
	CDaCtlEventDispatch (CComDynamicUnkArray & pUnkArray) : mUnkArray (pUnkArray) {}

	CComDynamicUnkArray &	mUnkArray;
};

/////////////////////////////////////////////////////////////////////////////

template <class T>
class CProxy_DaCtlEvents : public IConnectionPointImpl<T, &__uuidof(_DaCtlEvents), CComDynamicUnkArray>, public CDaCtlEventDispatch
{
public:
	CProxy_DaCtlEvents () : CDaCtlEventDispatch (m_vec) {}
};

template <class T>
class CProxy_AgentEvents : public IConnectionPointImpl<T, &__uuidof(_AgentEvents), CComDynamicUnkArray>, public CDaCtlEventDispatch
{
public:
	CProxy_AgentEvents () : CDaCtlEventDispatch (m_vec) {}
};

/////////////////////////////////////////////////////////////////////////////
#pragma page
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CServerNotifySink :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IDaSvrNotifySink, &__uuidof(IDaSvrNotifySink), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>
{
public:
	CServerNotifySink ();
	~CServerNotifySink ();

	HRESULT Initialize (DaControl * pOwner);
	HRESULT Terminate ();

	BEGIN_COM_MAP(CServerNotifySink)
		COM_INTERFACE_ENTRY(IDaSvrNotifySink)
	END_COM_MAP()

	HRESULT STDMETHODCALLTYPE Command (long CommandID, IDaSvrUserInput2 *UserInput);
	HRESULT STDMETHODCALLTYPE ActivateInputState (long CharacterID, long Activated);
	HRESULT STDMETHODCALLTYPE Restart (void);
	HRESULT STDMETHODCALLTYPE Shutdown (void);
	HRESULT STDMETHODCALLTYPE VisibleState (long CharacterID, long Visible, long Cause);
	HRESULT STDMETHODCALLTYPE Click (long CharacterID, short Keys, long x, long y);
	HRESULT STDMETHODCALLTYPE DblClick (long CharacterID, short Keys, long x, long y);
	HRESULT STDMETHODCALLTYPE DragStart (long CharacterID, short Keys, long x, long y);
	HRESULT STDMETHODCALLTYPE DragComplete (long CharacterID, short Keys, long x, long y);
	HRESULT STDMETHODCALLTYPE RequestStart (long RequestID);
	HRESULT STDMETHODCALLTYPE RequestComplete (long RequestID, long Result);
	HRESULT STDMETHODCALLTYPE BookMark (long BookMarkID);
	HRESULT STDMETHODCALLTYPE Idle (long CharacterID, long Start);
	HRESULT STDMETHODCALLTYPE Move (long CharacterID, long x, long y, long Cause);
	HRESULT STDMETHODCALLTYPE Size (long CharacterID, long Width, long Height);
	HRESULT STDMETHODCALLTYPE BalloonVisibleState (long CharacterID, long Visible);
	HRESULT STDMETHODCALLTYPE HelpComplete (long CharacterID, long CommandID, long Cause);
	HRESULT STDMETHODCALLTYPE ListeningState (long CharacterID, long Listening, long Cause);
	HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR CharGUID);
	HRESULT STDMETHODCALLTYPE AgentPropertyChange ();
	HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status);

public:
	DaControl *	mOwner;
	long		mServerNotifyId;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page
/////////////////////////////////////////////////////////////////////////////

class CEventNotifyReflect : public CEventNotify, public CComObjectStack <CServerNotifySink>
{
public:
	CEventNotifyReflect (DaControl * pOwner);
	virtual ~CEventNotifyReflect ();
	
public:
	typedef CComObjectStack <CServerNotifySink> _NotifyBase;

	STDMETHOD_(ULONG, AddRef) () {return _NotifyBase::AddRef ();}
	STDMETHOD_(ULONG, Release) () {return _NotifyBase::Release ();}
	STDMETHOD(QueryInterface) (REFIID iid, void ** ppvObject) {return _NotifyBase::QueryInterface (iid, ppvObject);}

	HRESULT STDMETHODCALLTYPE Command (long CommandID, IDaSvrUserInput2 *UserInput) {return _NotifyBase::Command (CommandID, UserInput);}
	HRESULT STDMETHODCALLTYPE ActivateInputState (long CharacterID, long Activated) {return _NotifyBase::ActivateInputState (CharacterID, Activated);}
	HRESULT STDMETHODCALLTYPE Restart (void) {return _NotifyBase::Restart ();}
	HRESULT STDMETHODCALLTYPE Shutdown (void) {return _NotifyBase::Shutdown ();}
	HRESULT STDMETHODCALLTYPE VisibleState (long CharacterID, long Visible, long Cause) {return _NotifyBase::VisibleState (CharacterID, Visible, Cause);}
	HRESULT STDMETHODCALLTYPE Click (long CharacterID, short Keys, long x, long y) {return _NotifyBase::Click (CharacterID, Keys, x, y);}
	HRESULT STDMETHODCALLTYPE DblClick (long CharacterID, short Keys, long x, long y) {return _NotifyBase::DblClick (CharacterID, Keys, x, y);}
	HRESULT STDMETHODCALLTYPE DragStart (long CharacterID, short Keys, long x, long y) {return _NotifyBase::DragStart (CharacterID, Keys, x, y);}
	HRESULT STDMETHODCALLTYPE DragComplete (long CharacterID, short Keys, long x, long y) {return _NotifyBase::DragComplete (CharacterID, Keys, x, y);}
	HRESULT STDMETHODCALLTYPE RequestStart (long RequestID) {return _NotifyBase::RequestStart (RequestID);}
	HRESULT STDMETHODCALLTYPE RequestComplete (long RequestID, long Result) {return _NotifyBase::RequestComplete (RequestID, Result);}
	HRESULT STDMETHODCALLTYPE BookMark (long BookMarkID) {return _NotifyBase::BookMark (BookMarkID);}
	HRESULT STDMETHODCALLTYPE Idle (long CharacterID, long Start) {return _NotifyBase::Idle (CharacterID, Start);}
	HRESULT STDMETHODCALLTYPE Move (long CharacterID, long x, long y, long Cause) {return _NotifyBase::Move (CharacterID, x, y, Cause);}
	HRESULT STDMETHODCALLTYPE Size (long CharacterID, long Width, long Height) {return _NotifyBase::Size (CharacterID, Width, Height);}
	HRESULT STDMETHODCALLTYPE BalloonVisibleState (long CharacterID, long Visible) {return _NotifyBase::BalloonVisibleState (CharacterID, Visible);}
	HRESULT STDMETHODCALLTYPE HelpComplete (long CharacterID, long CommandID, long Cause) {return _NotifyBase::HelpComplete (CharacterID, CommandID, Cause);}
	HRESULT STDMETHODCALLTYPE ListeningState (long CharacterID, long Listening, long Cause) {return _NotifyBase::ListeningState (CharacterID, Listening, Cause);}
	HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR CharGUID) {return _NotifyBase::DefaultCharacterChange (CharGUID);}
	HRESULT STDMETHODCALLTYPE AgentPropertyChange () {return _NotifyBase::AgentPropertyChange ();}
	HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status) {return _NotifyBase::ActiveClientChange (CharacterID, Status);}
};

/////////////////////////////////////////////////////////////////////////////
