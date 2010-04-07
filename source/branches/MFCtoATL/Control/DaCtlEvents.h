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

class CDaCtlEventDispatch
{
public:
	void FireActivateInput(LPCTSTR CharacterID);
	void FireDeactivateInput(LPCTSTR CharacterID);
	void FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireShow(LPCTSTR CharacterID, short Cause);
	void FireHide(LPCTSTR CharacterID, short Cause);
	void FireRequestStart(LPDISPATCH Request);
	void FireRequestComplete(LPDISPATCH Request);
	void FireRestart();
	void FireShutdown();
	void FireBookmark(long BookmarkID);
	void FireCommand(LPDISPATCH UserInput);
	void FireIdleStart(LPCTSTR CharacterID);
	void FireIdleComplete(LPCTSTR CharacterID);
	void FireMove(LPCTSTR CharacterID, short x, short y, short Cause);
	void FireSize(LPCTSTR CharacterID, short Width, short Height);
	void FireBalloonShow(LPCTSTR CharacterID);
	void FireBalloonHide(LPCTSTR CharacterID);
	void FireHelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause);
	void FireListenStart(LPCTSTR CharacterID);
	void FireListenComplete(LPCTSTR CharacterID, short Cause);
	void FireDefaultCharacterChange(LPCTSTR GUID);
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
