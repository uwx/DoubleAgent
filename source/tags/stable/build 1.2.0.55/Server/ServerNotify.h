/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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

class CServerNotify :
	public CEventNotify,
	public CComObjectRootEx<CComMultiThreadModel>,
	public IConnectionPointContainerImpl<CServerNotify>,
	public CProxyIDaSvrNotifySink<CServerNotify>,
	public CProxyIDaSvrNotifySink2<CServerNotify>,
	public CProxyIAgentNotifySink<CServerNotify>,
	public CProxyIAgentNotifySinkEx<CServerNotify>,
	public CProxy_DaSvrEvents2<CServerNotify>
{
public:
	CServerNotify ();
	virtual ~CServerNotify ();

// Attributes
public:
	class DaServer *	mOwner;

// Operations
public:
	HRESULT Register (IUnknown * punkNotifySink, long* pdwSinkID);
	HRESULT Unregister (long dwSinkID, bool pDelay);
	void UnregisterDelayed ();
	void UnregisterAll ();
	void AbandonAll ();

// Overrides
public:
	virtual bool PreFireEvent (LPCTSTR pEventName = NULL);
	virtual bool PostFireEvent (LPCTSTR pEventName = NULL, UINT pEventSinkCount = 0);

	HRESULT OnSpeechStart (long CharacterID, LPUNKNOWN FormattedText);
	HRESULT OnSpeechEnd (long CharacterID, LPUNKNOWN FormattedText, VARIANT_BOOL Stopped);
	HRESULT OnSpeechWord (long CharacterID, LPUNKNOWN FormattedText, long WordIndex);

// Declarations
public:
	BEGIN_COM_MAP(CServerNotify)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CServerNotify)
		CONNECTION_POINT_ENTRY(__uuidof(IDaSvrNotifySink))
		CONNECTION_POINT_ENTRY(__uuidof(IDaSvrNotifySink2))
		CONNECTION_POINT_ENTRY(__uuidof(IAgentNotifySink))
		CONNECTION_POINT_ENTRY(__uuidof(IAgentNotifySinkEx))
		CONNECTION_POINT_ENTRY(__uuidof(_DaSvrEvents2))
	END_CONNECTION_POINT_MAP()

// IDaSvrNotifySink2
public:
	HRESULT STDMETHODCALLTYPE Command (long CommandID, IDaSvrUserInput2 *UserInput);
	HRESULT STDMETHODCALLTYPE ActivateInputState (long CharacterID, long Activated);
	HRESULT STDMETHODCALLTYPE VisibleState (long CharacterID, long Visible, long Cause);
	HRESULT STDMETHODCALLTYPE Click (long CharacterID, short Keys, long X, long Y);
	HRESULT STDMETHODCALLTYPE DblClick (long CharacterID, short Keys, long X, long Y);
	HRESULT STDMETHODCALLTYPE DragStart (long CharacterID, short Keys, long X, long Y);
	HRESULT STDMETHODCALLTYPE DragComplete (long CharacterID, short Keys, long X, long Y);
	HRESULT STDMETHODCALLTYPE RequestStart (long RequestID);
	HRESULT STDMETHODCALLTYPE RequestComplete (long RequestID, long Result);
	HRESULT STDMETHODCALLTYPE BookMark (long BookMarkID);
	HRESULT STDMETHODCALLTYPE Idle (long CharacterID, long Start);
	HRESULT STDMETHODCALLTYPE Move (long CharacterID, long X, long Y, long Cause);
	HRESULT STDMETHODCALLTYPE Size (long CharacterID, long Width, long Height);
	HRESULT STDMETHODCALLTYPE BalloonVisibleState (long CharacterID, long Visible);
	HRESULT STDMETHODCALLTYPE ListeningState (long CharacterID, long Listening, long Cause);
	HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR CharGUID);
	HRESULT STDMETHODCALLTYPE AgentPropertyChange (void);
	HRESULT STDMETHODCALLTYPE ActiveClientChange (long CharacterID, long Status);

	HRESULT STDMETHODCALLTYPE SpeechStart (long CharacterID, IDaSvrFormattedText* FormattedText);
	HRESULT STDMETHODCALLTYPE SpeechEnd (long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped);
	HRESULT STDMETHODCALLTYPE SpeechWord (long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex);

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
	HRESULT FireClick(long CharacterID, short Keys, long X, long Y);
	HRESULT FireDblClick(long CharacterID, short Keys, long X, long Y);
	HRESULT FireDragStart(long CharacterID, short Keys, long X, long Y);
	HRESULT FireDragComplete(long CharacterID, short Keys, long X, long Y);
	HRESULT FireRequestStart(long RequestID);
	HRESULT FireRequestComplete(long RequestID, long Result);
	HRESULT FireBookMark(long BookMarkID);
	HRESULT FireIdle(long CharacterID, long Start);
	HRESULT FireMove(long CharacterID, long X, long Y, long Cause);
	HRESULT FireSize(long CharacterID, long Width, long Height);
	HRESULT FireBalloonVisibleState(long CharacterID, long Visible);
	HRESULT FireListeningState(long CharacterID, long Listening, long Cause);
	HRESULT FireDefaultCharacterChange(BSTR CharGUID);
	HRESULT FireAgentPropertyChange();
	HRESULT FireActiveClientChange(long CharacterID, long Status);

	HRESULT FireSpeechStart(long CharacterID, IDaSvrFormattedText* FormattedText);
	HRESULT FireSpeechEnd(long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped);
	HRESULT FireSpeechWord(long CharacterID, IDaSvrFormattedText* FormattedText, long WordNdx);

protected:
	CAtlTypeArray <long>	mUnregisterDelayed;
};

/////////////////////////////////////////////////////////////////////////////
