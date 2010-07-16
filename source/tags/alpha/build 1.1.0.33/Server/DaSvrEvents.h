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

class CDaSvrEventDispatch
{
public:
	void FireCommand(long CommandID, IDaSvrUserInput2* UserInput);
	void FireActivateInputState(long CharacterID, long Activated);
	void FireVisibleState(long CharacterID, long Visible, long Cause);
	void FireClick(long CharacterID, short Keys, long X, long Y);
	void FireDblClick(long CharacterID, short Keys, long X, long Y);
	void FireDragStart(long CharacterID, short Keys, long X, long Y);
	void FireDragComplete(long CharacterID, short Keys, long X, long Y);
	void FireRequestStart(long RequestID);
	void FireRequestComplete(long RequestID, long Result);
	void FireBookMark(long BookMarkID);
	void FireIdle(long CharacterID, long Start);
	void FireMove(long CharacterID, long X, long Y, long Cause);
	void FireSize(long CharacterID, long Width, long Height);
	void FireBalloonVisibleState(long CharacterID, long Visible);
	void FireListeningState(long CharacterID, long Listening, long Cause);
	void FireDefaultCharacterChange(LPCTSTR CharGUID);
	void FireAgentPropertyChange();
	void FireActiveClientChange(long CharacterID, long Status);

public:
	CDaSvrEventDispatch (CComDynamicUnkArray & pUnkArray) : mUnkArray (pUnkArray) {}
protected:
	CComDynamicUnkArray &	mUnkArray;
};

/////////////////////////////////////////////////////////////////////////////
