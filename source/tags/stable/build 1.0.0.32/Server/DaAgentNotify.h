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
#ifndef DAAGENTNOTIFY_H_INCLUDED_
#define DAAGENTNOTIFY_H_INCLUDED_
#pragma once

#include "DaServerOdl.h"
#include "DaInternalNotify.h"
#include "AgentFileCache.h"

/////////////////////////////////////////////////////////////////////////////

class CFileDownload;
class CVoiceCommandsWnd;

interface IDaNotify : public IDaSvrNotifySink
{
public:
	virtual long NextReqID () = 0;
	virtual class CAgentWnd * _GetRequestOwner (long pReqID) = 0;

	virtual class CDaAgentCharacter * _GetCharacter (long pCharID) = 0;
	virtual long _GetActiveCharacter () = 0;
	virtual long _GetActiveClient (long pCharID, bool pUseDefault = true) = 0;
	virtual long _GetNotifyClient (long pCharID, bool pUseDefault = true) = 0;
	virtual bool _ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) = 0;
	virtual bool _ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) = 0;

	virtual void _CharacterLoaded (long pCharID) = 0;
	virtual void _CharacterUnloaded (long pCharID) = 0;
	virtual void _CharacterNameChanged (long pCharID) = 0;
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) = 0;
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause) = 0;
	virtual bool _DownloadComplete (CFileDownload * pDownload) = 0;
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl) = 0;

	virtual long _GetVisibilityCause (long pCharID) = 0;
	virtual void _PutVisibilityCause (long pCharID, long pVisibilityCause) = 0;
	virtual long _GetMoveCause (long pCharID) = 0;
	virtual void _PutMoveCause (long pCharID, long pMoveCause) = 0;

	virtual bool _DoContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition) = 0;
	virtual bool _DoDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition) = 0;
	virtual void _OptionsChanged () = 0;
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid) = 0;

	virtual void _RegisterInternalNotify (IDaInternalNotify * pNotify, bool pRegister) = 0;

private:
	ULONG STDMETHODCALLTYPE AddRef () {return 0;}
	ULONG STDMETHODCALLTYPE Release () {return 0;}
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID* ppvObj) {return E_NOTIMPL;}

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*) {return E_NOTIMPL;}

	HRESULT STDMETHODCALLTYPE Restart (void) {return E_NOTIMPL;}
	HRESULT STDMETHODCALLTYPE Shutdown (void) {return E_NOTIMPL;}
};

/////////////////////////////////////////////////////////////////////////////

#define	FIRE_EVENT(n,e,c)\
HRESULT STDMETHODCALLTYPE n e \
{ \
	if	(PreFireEvent (_T(#n))) \
	{ \
		try \
		{ \
			POSITION lPos; \
			IDaSvrNotifySink15 * lDaSink15; \
			for	(lPos = m_xDaNotifySink15.GetStartPosition (); lPos;) \
			{ \
				lDaSink15 = (IDaSvrNotifySink15 *) m_xDaNotifySink15.GetNextConnection (lPos); \
				try \
				{ \
					lDaSink15->n c; \
				} \
				catch AnyExceptionDebug \
			} \
			IDaSvrNotifySink * lDaSink; \
			for	(lPos = m_xDaNotifySink.GetStartPosition (); lPos;) \
			{ \
				lDaSink = (IDaSvrNotifySink *) m_xDaNotifySink.GetNextConnection (lPos); \
				try \
				{ \
					lDaSink->n c; \
				} \
				catch AnyExceptionDebug \
			} \
			IDaSvrNotifySink15 * lMsSink; \
			for	(lPos = m_xMsNotifySink.GetStartPosition (); lPos;) \
			{ \
				lMsSink = (IDaSvrNotifySink15 *) m_xMsNotifySink.GetNextConnection (lPos); \
				try \
				{ \
					lMsSink->n c; \
				} \
				catch AnyExceptionDebug \
			} \
			IDaSvrNotifySink * lMsSinkEx; \
			for	(lPos = m_xMsNotifySinkEx.GetStartPosition (); lPos;) \
			{ \
				lMsSinkEx = (IDaSvrNotifySink *) m_xMsNotifySinkEx.GetNextConnection (lPos); \
				try \
				{ \
					lMsSinkEx->n c; \
				} \
				catch AnyExceptionDebug \
			} \
		} \
		catch AnyExceptionDebug \
		PostFireEvent (_T(#n)); \
	} \
	return S_OK; \
}

#define	FIRE_EVENT_EX(n,e,c)\
HRESULT STDMETHODCALLTYPE n e \
{ \
	if	(PreFireEvent (_T(#n))) \
	{ \
		try \
		{ \
			POSITION lPos; \
			IDaSvrNotifySink * lDaSink; \
			for	(lPos = m_xDaNotifySink.GetStartPosition (); lPos;) \
			{ \
				lDaSink = (IDaSvrNotifySink *) m_xDaNotifySink.GetNextConnection (lPos); \
				try \
				{ \
					lDaSink->n c; \
				} \
				catch AnyExceptionDebug \
			} \
			IDaSvrNotifySink * lMsSinkEx; \
			for	(lPos = m_xMsNotifySinkEx.GetStartPosition (); lPos;) \
			{ \
				lMsSinkEx = (IDaSvrNotifySink *) m_xMsNotifySinkEx.GetNextConnection (lPos); \
				try \
				{ \
					lMsSinkEx->n c; \
				} \
				catch AnyExceptionDebug \
			} \
		} \
		catch AnyExceptionDebug \
		PostFireEvent (_T(#n)); \
	} \
	return S_OK; \
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CDaAgentNotify : public CCmdTarget, public IDaNotify, public CAgentFileCache
{
public:
	CDaAgentNotify (class CDaAgent & pOwner);
	virtual ~CDaAgentNotify ();
	DECLARE_DYNAMIC (CDaAgentNotify)

// Attributes
public:

// Operations
public:
	FIRE_EVENT(Command, (long CommandID, LPUNKNOWN UserInput), (CommandID, UserInput))
	FIRE_EVENT(ActivateInputState, (long CharID, long Activated), (CharID, Activated))
	FIRE_EVENT(VisibleState, (long CharID, long Visible, long Cause), (CharID, Visible, Cause))
	FIRE_EVENT(Click, (long CharID, short Keys, long x, long y), (CharID, Keys, x, y))
	FIRE_EVENT(DblClick, (long CharID, short Keys, long x, long y), (CharID, Keys, x, y))
	FIRE_EVENT(DragStart, (long CharID, short Keys, long x, long y), (CharID, Keys, x, y))
	FIRE_EVENT(DragComplete, (long CharID, short Keys, long x, long y), (CharID, Keys, x, y))
	FIRE_EVENT(RequestStart, (long RequestID), (RequestID))
	FIRE_EVENT(RequestComplete, (long RequestID, long Status), (RequestID, Status))
	FIRE_EVENT(BookMark, (long dwBookMarkID), (dwBookMarkID))
	FIRE_EVENT(Idle, (long CharID, long Start), (CharID, Start))
	FIRE_EVENT(Move, (long CharID, long x, long y, long Cause), (CharID, x, y, Cause))
	FIRE_EVENT(Size, (long CharID, long Width, long Height), (CharID, Width, Height))
	FIRE_EVENT(BalloonVisibleState, (long CharID, long Visible), (CharID, Visible))

	FIRE_EVENT_EX(HelpComplete, (long CharID, long CommandID, long Cause), (CharID, CommandID, Cause))
	FIRE_EVENT_EX(ListeningState, (long CharID, long Listening, long Cause), (CharID, Listening, Cause))
	FIRE_EVENT_EX(DefaultCharacterChange, (BSTR GUID), (GUID))
	FIRE_EVENT_EX(AgentPropertyChange, (), ())
	FIRE_EVENT_EX(ActiveClientChange, (long CharID, long Status), (CharID, Status))

	HRESULT Register (IUnknown * punkNotifySink, long * pdwSinkID);
	HRESULT Unregister (long dwSinkID);
	void UnregisterAll ();
	void AbandonAll ();

// Overrides
	//{{AFX_VIRTUAL(CDaAgent)
	public:
	virtual long NextReqID ();
	virtual class CAgentWnd * _GetRequestOwner (long pReqID);
	virtual class CDaAgentCharacter * _GetCharacter (long pCharID);
	virtual long _GetActiveCharacter ();
	virtual long _GetActiveClient (long pCharID, bool pUseDefault = true);
	virtual long _GetNotifyClient (long pCharID, bool pUseDefault = true);
	virtual bool _ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual bool _ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _CharacterListening (long pCharID, bool pListening, long pCause);
	virtual bool _DownloadComplete (CFileDownload * pDownload);
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl);
	virtual long _GetVisibilityCause (long pCharID);
	virtual void _PutVisibilityCause (long pCharID, long pVisibilityCause);
	virtual long _GetMoveCause (long pCharID);
	virtual void _PutMoveCause (long pCharID, long pMoveCause);
	virtual bool _DoContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual bool _DoDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual void _OptionsChanged ();
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid);
	virtual void _RegisterInternalNotify (IDaInternalNotify * pNotify, bool pRegister);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_CONNECTION_PART(CDaAgentNotify, DaNotifySink15)
		CONNECTION_IID(__uuidof(IDaSvrNotifySink15))
		void AbandonAll ();
	END_CONNECTION_PART(DaNotifySink15)

	BEGIN_CONNECTION_PART(CDaAgentNotify, DaNotifySink)
		CONNECTION_IID(__uuidof(IDaSvrNotifySink))
		void AbandonAll ();
	END_CONNECTION_PART(DaNotifySink)

	BEGIN_CONNECTION_PART(CDaAgentNotify, MsNotifySink)
		CONNECTION_IID(__uuidof(IAgentNotifySink))
		void AbandonAll ();
	END_CONNECTION_PART(MsNotifySink)

	BEGIN_CONNECTION_PART(CDaAgentNotify, MsNotifySinkEx)
		CONNECTION_IID(__uuidof(IAgentNotifySinkEx))
		void AbandonAll ();
	END_CONNECTION_PART(MsNotifySinkEx)

	DECLARE_CONNECTION_MAP()

protected:
	bool PreFireEvent (LPCTSTR pEventName);
	bool PostFireEvent (LPCTSTR pEventName);

protected:
	class CDaAgent &					mOwner;
	long								mNextReqID;
	CMap <long, long, long, long>		mVisibilityCause;
	CMap <long, long, long, long>		mMoveCause;
	CPtrTypeArray <IDaInternalNotify>	mInternalNotify;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTNOTIFY_H_INCLUDED_
