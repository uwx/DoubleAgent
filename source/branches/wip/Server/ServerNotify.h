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
#include <AgtSvr.h>
#include "DaServerOdl.h"
#include "DaGuid.h"
#include "ServerNotifySink.h"
#include "DaSvrEvents.h"
#include "AgentFileCache.h"

/////////////////////////////////////////////////////////////////////////////

class CFileDownload;
class CVoiceCommandsWnd;

interface _IServerNotify : public IDaSvrNotifySink
{
public:
	virtual long NextReqID () = 0;
	virtual class CAgentWnd * _GetRequestOwner (long pReqID) = 0;
	virtual class CAgentWnd * _GetAgentWnd (HWND pWindow) = 0;

	virtual class DaSvrCharacter * _GetCharacter (long pCharID) = 0;
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

	virtual VisibilityCauseType _GetVisibilityCause (long pCharID) = 0;
	virtual void _PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause) = 0;
	virtual MoveCauseType _GetMoveCause (long pCharID) = 0;
	virtual void _PutMoveCause (long pCharID, MoveCauseType pMoveCause) = 0;

	virtual bool _DoContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition) = 0;
	virtual bool _DoDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition) = 0;
	virtual void _OptionsChanged () = 0;
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid) = 0;

	virtual void _RegisterInternalNotify (_IServerNotifySink * pNotify, bool pRegister) = 0;

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
			int lNdx; \
			IDaSvrNotifySink * lDaSink; \
			for	(lNdx = 0; lNdx < tDaSvrNotifySink::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lDaSink = (IDaSvrNotifySink *) tDaSvrNotifySink::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lDaSink->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
			IAgentNotifySink * lMsSink; \
			for	(lNdx = 0; lNdx < tAgentNotifySink::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lMsSink = (IAgentNotifySink *) tAgentNotifySink::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lMsSink->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
			IAgentNotifySinkEx * lMsSinkEx; \
			for	(lNdx = 0; lNdx < tAgentNotifySinkEx::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lMsSinkEx = (IAgentNotifySinkEx *) tAgentNotifySinkEx::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lMsSinkEx->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
			mEventDispatch.Fire##n c; \
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
			int lNdx; \
			IDaSvrNotifySink * lDaSink; \
			for	(lNdx = 0; lNdx < tDaSvrNotifySink::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lDaSink = (IDaSvrNotifySink *) tDaSvrNotifySink::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lDaSink->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
			IAgentNotifySinkEx * lMsSinkEx; \
			for	(lNdx = 0; lNdx < tAgentNotifySinkEx::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lMsSinkEx = (IAgentNotifySinkEx *) tAgentNotifySinkEx::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lMsSinkEx->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
			mEventDispatch.Fire##n c; \
		} \
		catch AnyExceptionDebug \
		PostFireEvent (_T(#n)); \
	} \
	return S_OK; \
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CServerNotifyBase :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IConnectionPointContainerImpl<CServerNotifyBase>,
	public IConnectionPointImpl<CServerNotifyBase, &__uuidof(IDaSvrNotifySink), CComDynamicUnkArray>,
	public IConnectionPointImpl<CServerNotifyBase, &__uuidof(IAgentNotifySink), CComDynamicUnkArray>,
	public IConnectionPointImpl<CServerNotifyBase, &__uuidof(IAgentNotifySinkEx), CComDynamicUnkArray>,
	public IConnectionPointImpl<CServerNotifyBase, &__uuidof(_DaSvrEvents), CComDynamicUnkArray>,
	public _IServerNotify
{
public:
	BEGIN_COM_MAP(CServerNotifyBase)
		COM_INTERFACE_ENTRY(IDaSvrNotifySink)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CServerNotifyBase)
		CONNECTION_POINT_ENTRY(__uuidof(IDaSvrNotifySink))
		CONNECTION_POINT_ENTRY(__uuidof(IAgentNotifySink))
		CONNECTION_POINT_ENTRY(__uuidof(IAgentNotifySinkEx))
		CONNECTION_POINT_ENTRY(__uuidof(_DaSvrEvents))
	END_CONNECTION_POINT_MAP()

protected:
	typedef IConnectionPointImpl<CServerNotifyBase, &__uuidof(IDaSvrNotifySink), CComDynamicUnkArray> tDaSvrNotifySink;
	typedef IConnectionPointImpl<CServerNotifyBase, &__uuidof(IAgentNotifySink), CComDynamicUnkArray> tAgentNotifySink;
	typedef IConnectionPointImpl<CServerNotifyBase, &__uuidof(IAgentNotifySinkEx), CComDynamicUnkArray> tAgentNotifySinkEx;
	typedef IConnectionPointImpl<CServerNotifyBase, &__uuidof(_DaSvrEvents), CComDynamicUnkArray> tDaSvrEvents;

	CDaSvrEventDispatch	mEventDispatch;
	CServerNotifyBase () : mEventDispatch (tDaSvrEvents::m_vec) {}
};

/////////////////////////////////////////////////////////////////////////////

class CServerNotify :
	public CComObjectStackEx<CServerNotifyBase>,
	public CAgentFileCache
{
public:
	CServerNotify (class DaServer & pOwner);
	virtual ~CServerNotify ();

// Attributes
public:

// Operations
public:
	FIRE_EVENT(Command, (long CommandID, IDaSvrUserInput2* UserInput), (CommandID, UserInput))
	FIRE_EVENT(ActivateInputState, (long CharacterID, long Activated), (CharacterID, Activated))
	HRESULT STDMETHODCALLTYPE Restart () {return S_OK;} // Obsolete
	HRESULT STDMETHODCALLTYPE Shutdown () {return S_OK;} // Obsolete
	FIRE_EVENT(VisibleState, (long CharacterID, long Visible, long Cause), (CharacterID, Visible, Cause))
	FIRE_EVENT(Click, (long CharacterID, short Keys, long x, long y), (CharacterID, Keys, x, y))
	FIRE_EVENT(DblClick, (long CharacterID, short Keys, long x, long y), (CharacterID, Keys, x, y))
	FIRE_EVENT(DragStart, (long CharacterID, short Keys, long x, long y), (CharacterID, Keys, x, y))
	FIRE_EVENT(DragComplete, (long CharacterID, short Keys, long x, long y), (CharacterID, Keys, x, y))
	FIRE_EVENT(RequestStart, (long RequestID), (RequestID))
	FIRE_EVENT(RequestComplete, (long RequestID, long Status), (RequestID, Status))
	FIRE_EVENT(BookMark, (long BookMarkID), (BookMarkID))
	FIRE_EVENT(Idle, (long CharacterID, long Start), (CharacterID, Start))
	FIRE_EVENT(Move, (long CharacterID, long x, long y, long Cause), (CharacterID, x, y, Cause))
	FIRE_EVENT(Size, (long CharacterID, long Width, long Height), (CharacterID, Width, Height))
	FIRE_EVENT(BalloonVisibleState, (long CharacterID, long Visible), (CharacterID, Visible))

	HRESULT STDMETHODCALLTYPE HelpComplete (long CharacterID, long CommandID, long Cause) {return S_OK;} // Obsolete
	FIRE_EVENT_EX(ListeningState, (long CharacterID, long Listening, long Cause), (CharacterID, Listening, Cause))
	FIRE_EVENT_EX(DefaultCharacterChange, (BSTR CharGUID), (CharGUID))
	FIRE_EVENT_EX(AgentPropertyChange, (), ())
	FIRE_EVENT_EX(ActiveClientChange, (long CharacterID, long Status), (CharacterID, Status))

	HRESULT Register (IUnknown * punkNotifySink, long * pdwSinkID);
	HRESULT Unregister (long dwSinkID);
	void UnregisterAll ();
	void AbandonAll ();

// Overrides
public:
	virtual long NextReqID ();
	virtual class CAgentWnd * _GetRequestOwner (long pReqID);
	virtual class CAgentWnd * _GetAgentWnd (HWND pWindow);
	virtual class DaSvrCharacter * _GetCharacter (long pCharID);
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
	virtual VisibilityCauseType _GetVisibilityCause (long pCharID);
	virtual void _PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause);
	virtual MoveCauseType _GetMoveCause (long pCharID);
	virtual void _PutMoveCause (long pCharID, MoveCauseType pMoveCause);
	virtual bool _DoContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual bool _DoDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual void _OptionsChanged ();
	virtual void _DefaultCharacterChanged (REFGUID pCharGuid);
	virtual void _RegisterInternalNotify (_IServerNotifySink * pNotify, bool pRegister);

// Implementation
protected:
	bool PreFireEvent (LPCTSTR pEventName);
	bool PostFireEvent (LPCTSTR pEventName);

protected:
	class DaServer &						mOwner;
	long									mNextReqID;
	CAtlMap <long, CZeroInit<long> >		mVisibilityCause;
	CAtlMap <long, CZeroInit<long> >		mMoveCause;
	CAtlPtrTypeArray <_IServerNotifySink>	mInternalNotify;
};

/////////////////////////////////////////////////////////////////////////////
