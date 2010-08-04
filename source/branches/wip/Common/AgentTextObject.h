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
#include "AgentText.h"
#include "EventNotify.h"
#include "SapiVoiceCache.h"
#include "SapiVoiceEventSink.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentTextObject :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IUnknown,
	public CEventNotifyClient<CAgentTextObject>,
	public CSapiVoiceClient,
	public _ISapiVoiceEventSink
{
public:
	CAgentTextObject ();
	virtual ~CAgentTextObject ();

// Attributes
public:
	CAgentTextDraw	mText;

// Operations
public:
	void Attach (long pCharID, CEventNotify * pNotify, CSapiVoice * pVoice);
	void Detach (bool pCancelled = false);

// Overrides
public:
	virtual void OnVoiceStart (long pCharID);
	virtual void OnVoiceEnd (long pCharID);
	virtual void OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength);

// Implementation
protected:
	LRESULT OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceWordMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentTextObject)
		MESSAGE_HANDLER(mVoiceStartMsg, OnVoiceStartMsg)
		MESSAGE_HANDLER(mVoiceEndMsg, OnVoiceEndMsg)
		MESSAGE_HANDLER(mVoiceWordMsg, OnVoiceWordMsg)
	END_MSG_MAP()

protected:
	long										mCharID;
	CSapiVoice *								mVoice;
	tPtr <CMsgPostingWnd <CAgentTextObject> >	mMsgPostingWnd;
	bool										mVoiceStartNotified;
	bool										mVoiceEndNotified;
	static const UINT							mVoiceStartMsg;
	static const UINT							mVoiceEndMsg;
	static const UINT							mVoiceWordMsg;
};

/////////////////////////////////////////////////////////////////////////////
