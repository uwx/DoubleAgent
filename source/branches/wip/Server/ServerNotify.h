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
#pragma once
#include "DaGuid.h"
#include "DaSvrEvents.h"
#include "EventNotify.h"

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CServerNotify :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CEventNotify,
	public IConnectionPointContainerImpl<CServerNotify>,
	public IConnectionPointImpl<CServerNotify, &__uuidof(IDaSvrNotifySink), CComDynamicUnkArray>,
	public IConnectionPointImpl<CServerNotify, &__uuidof(IAgentNotifySink), CComDynamicUnkArray>,
	public IConnectionPointImpl<CServerNotify, &__uuidof(IAgentNotifySinkEx), CComDynamicUnkArray>,
	public IConnectionPointImpl<CServerNotify, &__uuidof(_DaSvrEvents), CComDynamicUnkArray>
{
public:
	CServerNotify ();
	virtual ~CServerNotify ();

// Attributes
public:
	class DaServer *	mOwner;

// Operations
public:

	HRESULT Register (IUnknown * punkNotifySink, long * pdwSinkID);
	HRESULT Unregister (long dwSinkID);
	void UnregisterAll ();
	void AbandonAll ();

// Overrides
public:
	virtual bool _ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause);
protected:
	virtual bool PreFireEvent (LPCTSTR pEventName);
	virtual bool PostFireEvent (LPCTSTR pEventName);

// Declarations
public:
	BEGIN_COM_MAP(CServerNotify)
		COM_INTERFACE_ENTRY(IDaSvrNotifySink)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CServerNotify)
		CONNECTION_POINT_ENTRY(__uuidof(IDaSvrNotifySink))
		CONNECTION_POINT_ENTRY(__uuidof(IAgentNotifySink))
		CONNECTION_POINT_ENTRY(__uuidof(IAgentNotifySinkEx))
		CONNECTION_POINT_ENTRY(__uuidof(_DaSvrEvents))
	END_CONNECTION_POINT_MAP()
	
// IDaSvrNotifySink
public:
	HRESULT STDMETHODCALLTYPE Command (long CommandID, IDaSvrUserInput2 *UserInput);
	HRESULT STDMETHODCALLTYPE ActivateInputState (long CharacterID, long Activated);
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
	HRESULT STDMETHODCALLTYPE ListeningState (long CharacterID, long Listening, long Cause);
	HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR CharGUID);
	HRESULT STDMETHODCALLTYPE AgentPropertyChange (void);
	HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status);
private:
	HRESULT STDMETHODCALLTYPE Restart (void) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE Shutdown (void) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE HelpComplete (long CharacterID, long CommandID, long Cause) {return E_NOTIMPL;}

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*) {return E_NOTIMPL;}

// Implementation
protected:
	HRESULT FireCommand(long CommandID, IDaSvrUserInput2* UserInput);
	HRESULT FireActivateInputState(long CharacterID, long Activated);
	HRESULT FireVisibleState(long CharacterID, long Visible, long Cause);
	HRESULT FireClick(long CharacterID, short Keys, long x, long y);
	HRESULT FireDblClick(long CharacterID, short Keys, long x, long y);
	HRESULT FireDragStart(long CharacterID, short Keys, long x, long y);
	HRESULT FireDragComplete(long CharacterID, short Keys, long x, long y);
	HRESULT FireRequestStart(long RequestID);
	HRESULT FireRequestComplete(long RequestID, long Result);
	HRESULT FireBookMark(long BookMarkID);
	HRESULT FireIdle(long CharacterID, long Start);
	HRESULT FireMove(long CharacterID, long x, long y, long Cause);
	HRESULT FireSize(long CharacterID, long Width, long Height);
	HRESULT FireBalloonVisibleState(long CharacterID, long Visible);
	HRESULT FireListeningState(long CharacterID, long Listening, long Cause);
	HRESULT FireDefaultCharacterChange(BSTR CharGUID);
	HRESULT FireAgentPropertyChange();
	HRESULT FireActiveClientChange(long CharacterID, long Status);

protected:
	typedef IConnectionPointImpl<CServerNotify, &__uuidof(IDaSvrNotifySink), CComDynamicUnkArray> tDaSvrNotifySink;
	typedef IConnectionPointImpl<CServerNotify, &__uuidof(IAgentNotifySink), CComDynamicUnkArray> tAgentNotifySink;
	typedef IConnectionPointImpl<CServerNotify, &__uuidof(IAgentNotifySinkEx), CComDynamicUnkArray> tAgentNotifySinkEx;
	typedef IConnectionPointImpl<CServerNotify, &__uuidof(_DaSvrEvents), CComDynamicUnkArray> tDaSvrEvents;

protected:
	CDaSvrEventDispatch	mEventDispatch;
};

/////////////////////////////////////////////////////////////////////////////
