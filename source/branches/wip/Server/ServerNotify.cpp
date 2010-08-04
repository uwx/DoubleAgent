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
#include "StdAfx.h"
#include <AgtSvr.h>
#include "DaServerApp.h"
#include "DaServer.h"
#include "ServerNotify.h"
#include "GuidStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#pragma comment(lib, "psapi.lib")

#ifdef	_DEBUG
//#define	_DEBUG_CONNECTIONS	LogNormal
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(IAgentNotifySink, __uuidof(IAgentNotifySink));
_COM_SMARTPTR_TYPEDEF(IAgentNotifySinkEx, __uuidof(IAgentNotifySinkEx));

/////////////////////////////////////////////////////////////////////////////

CServerNotify::CServerNotify ()
:	mOwner (NULL),
	mEventDispatch (tDaSvrEvents::m_vec)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotify::CServerNotify (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

CServerNotify::~CServerNotify ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotify::~CServerNotify (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static INT_PTR CountUnk (CComDynamicUnkArray & pUnkArray)
{
	int	lRet = 0;
	int	lNdx;

	for	(lNdx = 0; lNdx < pUnkArray.GetSize(); lNdx++)
	{
		if	(pUnkArray.GetAt (lNdx))
		{
			lRet++;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CServerNotify::Register (IUnknown * punkNotifySink, long * pdwSinkID)
{
	HRESULT					lResult = S_FALSE;
	IDaSvrNotifySink2Ptr	lDaSink2 (punkNotifySink);
	IDaSvrNotifySinkPtr		lDaSink (punkNotifySink);
	IAgentNotifySinkPtr		lMsSink (punkNotifySink);
	IAgentNotifySinkExPtr	lMsSinkEx (punkNotifySink);

	if	(lDaSink2 != NULL)
	{
		lResult = tDaSvrNotifySink2::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register2 (DA) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lDaSink != NULL)
	{
		lResult = tDaSvrNotifySink::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register (DA) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lMsSinkEx != NULL)
	{
		lResult = tAgentNotifySinkEx::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] RegisterEx (MS) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lMsSink != NULL)
	{
		lResult = tAgentNotifySink::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register (MS)   [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	{
		lResult = CONNECT_E_CANNOTCONNECT;
	}
	return lResult;
}

HRESULT CServerNotify::Unregister (long dwSinkID)
{
	HRESULT	lResult = S_FALSE;

	lResult = tDaSvrNotifySink2::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
	if	(SUCCEEDED (lResult))
	{
		LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister2 (DA) [%u]"), this, dwSinkID);
	}
#endif
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = tDaSvrNotifySink::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (DA) [%u]"), this, dwSinkID);
		}
#endif
	}
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = tAgentNotifySinkEx::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u]"), this, dwSinkID);
		}
#endif
	}
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = tAgentNotifySink::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS) [%u]"), this, dwSinkID);
		}
#endif
	}
	return lResult;
}

void CServerNotify::UnregisterAll ()
{
#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterAll [%d] [%d] [%d] [%d]"), this, CountUnk(tDaSvrNotifySink2::m_vec), CountUnk(tDaSvrNotifySink::m_vec), CountUnk(tAgentNotifySinkEx::m_vec), CountUnk(tAgentNotifySink::m_vec));
#endif
	int	lNdx;

	for	(lNdx = tDaSvrNotifySink2::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tDaSvrNotifySink2::m_vec.GetAt (lNdx))
			{
				tDaSvrNotifySink2::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister2 (DA) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = tDaSvrNotifySink::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tDaSvrNotifySink::m_vec.GetAt (lNdx))
			{
				tDaSvrNotifySink::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (DA) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = tAgentNotifySinkEx::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tAgentNotifySinkEx::m_vec.GetAt (lNdx))
			{
				tAgentNotifySinkEx::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = tAgentNotifySink::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tAgentNotifySink::m_vec.GetAt (lNdx))
			{
				tAgentNotifySink::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS)   [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////

void CServerNotify::AbandonAll ()
{
#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] AbandonAll [%d] [%d] {%d] [%d]"), this, CountUnk(tDaSvrNotifySink2::m_vec), CountUnk(tDaSvrNotifySink::m_vec), CountUnk(tAgentNotifySinkEx::m_vec), CountUnk(tAgentNotifySink::m_vec));
#endif
	tDaSvrNotifySink2::m_vec.clear ();
	tDaSvrNotifySink::m_vec.clear ();
	tAgentNotifySinkEx::m_vec.clear ();
	tAgentNotifySink::m_vec.clear ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CServerNotify::PreFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s (%d %d)"), mOwner, max(mOwner->m_dwRef,-1), pEventName, _AtlModule.GetLockCount(), (_AtlModule.GetLockCount()==0));
	}
#endif
	CEventNotify::PreFireEvent (pEventName);
	return mOwner->PreNotify ();
}

bool CServerNotify::PostFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s done (%d %d)"), mOwner, max(mOwner->m_dwRef,-1), pEventName, _AtlModule.GetLockCount(), (_AtlModule.GetLockCount()==0));
	}
#endif
	CEventNotify::PostFireEvent (pEventName);
	return mOwner->PostNotify ();
}

/////////////////////////////////////////////////////////////////////////////

#define	FIRE_EVENT(n,e,c)\
HRESULT CServerNotify::Fire##n e \
{ \
	if	(PreFireEvent (_T(#n))) \
	{ \
		try \
		{ \
			int lNdx; \
			IDaSvrNotifySink2 * lDaSink2; \
			for	(lNdx = 0; lNdx < tDaSvrNotifySink2::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lDaSink2 = (IDaSvrNotifySink2 *) tDaSvrNotifySink2::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lDaSink2->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
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
HRESULT CServerNotify::Fire##n e \
{ \
	if	(PreFireEvent (_T(#n))) \
	{ \
		try \
		{ \
			int lNdx; \
			IDaSvrNotifySink2 * lDaSink2; \
			for	(lNdx = 0; lNdx < tDaSvrNotifySink2::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lDaSink2 = (IDaSvrNotifySink2 *) tDaSvrNotifySink2::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lDaSink2->n c; \
					} \
					catch AnyExceptionDebug \
				} \
			} \
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

#define	FIRE_EVENT_2(n,e,c)\
HRESULT CServerNotify::Fire##n e \
{ \
	if	(PreFireEvent (_T(#n))) \
	{ \
		try \
		{ \
			int lNdx; \
			IDaSvrNotifySink2 * lDaSink2; \
			for	(lNdx = 0; lNdx < tDaSvrNotifySink2::m_vec.GetSize(); lNdx++) \
			{ \
				if	(lDaSink2 = (IDaSvrNotifySink2 *) tDaSvrNotifySink2::m_vec.GetAt (lNdx)) \
				{ \
					try \
					{ \
						lDaSink2->n c; \
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

FIRE_EVENT(Command, (long CommandID, IDaSvrUserInput2* UserInput), (CommandID, UserInput))
FIRE_EVENT(ActivateInputState, (long CharacterID, long Activated), (CharacterID, Activated))
FIRE_EVENT(VisibleState, (long CharacterID, long Visible, long Cause), (CharacterID, Visible, Cause))
FIRE_EVENT(Click, (long CharacterID, short Keys, long X, long Y), (CharacterID, Keys, X, Y))
FIRE_EVENT(DblClick, (long CharacterID, short Keys, long X, long Y), (CharacterID, Keys, X, Y))
FIRE_EVENT(DragStart, (long CharacterID, short Keys, long X, long Y), (CharacterID, Keys, X, Y))
FIRE_EVENT(DragComplete, (long CharacterID, short Keys, long X, long Y), (CharacterID, Keys, X, Y))
FIRE_EVENT(RequestStart, (long RequestID), (RequestID))
FIRE_EVENT(RequestComplete, (long RequestID, long Status), (RequestID, Status))
FIRE_EVENT(BookMark, (long BookMarkID), (BookMarkID))
FIRE_EVENT(Idle, (long CharacterID, long Start), (CharacterID, Start))
FIRE_EVENT(Move, (long CharacterID, long X, long Y, long Cause), (CharacterID, X, Y, Cause))
FIRE_EVENT(Size, (long CharacterID, long Width, long Height), (CharacterID, Width, Height))
FIRE_EVENT(BalloonVisibleState, (long CharacterID, long Visible), (CharacterID, Visible))

FIRE_EVENT_EX(ListeningState, (long CharacterID, long Listening, long Cause), (CharacterID, Listening, Cause))
FIRE_EVENT_EX(DefaultCharacterChange, (BSTR CharGUID), (CharGUID))
FIRE_EVENT_EX(AgentPropertyChange, (), ())
FIRE_EVENT_EX(ActiveClientChange, (long CharacterID, long Status), (CharacterID, Status))

FIRE_EVENT_2(SpeechStart, (long CharacterID, IDaSvrFormattedText* FormattedText), (CharacterID, FormattedText))
FIRE_EVENT_2(SpeechEnd, (long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped), (CharacterID, FormattedText, Stopped))
FIRE_EVENT_2(SpeechWord, (long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex), (CharacterID, FormattedText, WordIndex))

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotify::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	return FireCommand (CommandID, UserInput);
}

HRESULT STDMETHODCALLTYPE CServerNotify::ActivateInputState (long CharacterID, long Activated)
{
	return FireActivateInputState (CharacterID, Activated);
}

HRESULT STDMETHODCALLTYPE CServerNotify::VisibleState (long CharacterID, long Visible, long Cause)
{
	return FireVisibleState (CharacterID, Visible, Cause);
}

HRESULT STDMETHODCALLTYPE CServerNotify::Click (long CharacterID, short Keys, long X, long Y)
{
	return FireClick (CharacterID, Keys, X, Y);
}

HRESULT STDMETHODCALLTYPE CServerNotify::DblClick (long CharacterID, short Keys, long X, long Y)
{
	return FireDblClick (CharacterID, Keys, X, Y);
}

HRESULT STDMETHODCALLTYPE CServerNotify::DragStart (long CharacterID, short Keys, long X, long Y)
{
	return FireDragStart (CharacterID, Keys, X, Y);
}

HRESULT STDMETHODCALLTYPE CServerNotify::DragComplete (long CharacterID, short Keys, long X, long Y)
{
	return FireDragComplete (CharacterID, Keys, X, Y);
}

HRESULT STDMETHODCALLTYPE CServerNotify::RequestStart (long RequestID)
{
	return FireRequestStart (RequestID);
}

HRESULT STDMETHODCALLTYPE CServerNotify::RequestComplete (long RequestID, long Result)
{
	return FireRequestComplete (RequestID, Result);
}

HRESULT STDMETHODCALLTYPE CServerNotify::BookMark (long BookMarkID)
{
	return FireBookMark (BookMarkID);
}

HRESULT STDMETHODCALLTYPE CServerNotify::Idle (long CharacterID, long Start)
{
	return FireIdle (CharacterID, Start);
}

HRESULT STDMETHODCALLTYPE CServerNotify::Move (long CharacterID, long X, long Y, long Cause)
{
	return FireMove (CharacterID, X, Y, Cause);
}

HRESULT STDMETHODCALLTYPE CServerNotify::Size (long CharacterID, long Width, long Height)
{
	return FireSize (CharacterID, Width, Height);
}

HRESULT STDMETHODCALLTYPE CServerNotify::BalloonVisibleState (long CharacterID, long Visible)
{
	return FireBalloonVisibleState (CharacterID, Visible);
}

HRESULT STDMETHODCALLTYPE CServerNotify::ListeningState (long CharacterID, long Listening, long Cause)
{
	return FireListeningState (CharacterID, Listening, Cause);
}

HRESULT STDMETHODCALLTYPE CServerNotify::DefaultCharacterChange (BSTR CharGUID)
{
	return FireDefaultCharacterChange (CharGUID);
}

HRESULT STDMETHODCALLTYPE CServerNotify::AgentPropertyChange (void)
{
	return FireAgentPropertyChange ();
}

HRESULT STDMETHODCALLTYPE CServerNotify::ActiveClientChange (long CharacterID, long Status)
{
	return FireActiveClientChange (CharacterID, Status);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechStart (long CharacterID, IDaSvrFormattedText* FormattedText)
{
	return FireSpeechStart (CharacterID, FormattedText);
}

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechEnd (long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	return FireSpeechEnd (CharacterID, FormattedText, Stopped);
}

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechWord (long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex)
{
	return FireSpeechWord (CharacterID, FormattedText, WordIndex);
}

HRESULT CServerNotify::OnSpeechStart (long CharacterID, LPUNKNOWN FormattedText)
{
	IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
	return FireSpeechStart (CharacterID, lFormattedText);
}

HRESULT CServerNotify::OnSpeechEnd (long CharacterID, LPUNKNOWN FormattedText, VARIANT_BOOL Stopped)
{
	IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
	return FireSpeechEnd (CharacterID, lFormattedText, Stopped);
}

HRESULT CServerNotify::OnSpeechWord (long CharacterID, LPUNKNOWN FormattedText, long WordIndex)
{
	IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
	return FireSpeechWord (CharacterID, lFormattedText, WordIndex);
}
