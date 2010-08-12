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
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireCommand (CommandID, UserInput);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::ActivateInputState (long CharacterID, long Activated)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireActivateInputState (CharacterID, Activated);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::VisibleState (long CharacterID, long Visible, long Cause)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireVisibleState (CharacterID, Visible, Cause);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Click (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireClick (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DblClick (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDblClick (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DragStart (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDragStart (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DragComplete (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDragComplete (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::RequestStart (long RequestID)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireRequestStart (RequestID);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::RequestComplete (long RequestID, long Result)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireRequestComplete (RequestID, Result);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::BookMark (long BookMarkID)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireBookMark (BookMarkID);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Idle (long CharacterID, long Start)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireIdle (CharacterID, Start);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Move (long CharacterID, long X, long Y, long Cause)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireMove (CharacterID, X, Y, Cause);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Size (long CharacterID, long Width, long Height)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSize (CharacterID, Width, Height);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::BalloonVisibleState (long CharacterID, long Visible)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireBalloonVisibleState (CharacterID, Visible);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::ListeningState (long CharacterID, long Listening, long Cause)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireListeningState (CharacterID, Listening, Cause);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DefaultCharacterChange (BSTR CharGUID)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDefaultCharacterChange (CharGUID);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::AgentPropertyChange (void)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireAgentPropertyChange ();
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::ActiveClientChange (long CharacterID, long Status)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireActiveClientChange (CharacterID, Status);
	}
	catch AnyExceptionSilent
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechStart (long CharacterID, IDaSvrFormattedText* FormattedText)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSpeechStart (CharacterID, FormattedText);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechEnd (long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSpeechEnd (CharacterID, FormattedText, Stopped);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechWord (long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSpeechWord (CharacterID, FormattedText, WordIndex);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT CServerNotify::OnSpeechStart (long CharacterID, LPUNKNOWN FormattedText)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
		lResult = FireSpeechStart (CharacterID, lFormattedText);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT CServerNotify::OnSpeechEnd (long CharacterID, LPUNKNOWN FormattedText, VARIANT_BOOL Stopped)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
		lResult = FireSpeechEnd (CharacterID, lFormattedText, Stopped);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT CServerNotify::OnSpeechWord (long CharacterID, LPUNKNOWN FormattedText, long WordIndex)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
		lResult = FireSpeechWord (CharacterID, lFormattedText, WordIndex);
	}
	catch AnyExceptionSilent
	return lResult;
}
