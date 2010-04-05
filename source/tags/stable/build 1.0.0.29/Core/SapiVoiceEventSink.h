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
#ifndef _SAPIVOICEEVENTSINK_H
#define _SAPIVOICEEVENTSINK_H
//////////////////////////////////////////////////////////////////////

interface ISapiVoiceEventSink
{
public:
	virtual void OnVoiceStart (long pCharID) {}
	virtual void OnVoiceEnd (long pCharID) {}
	virtual void OnVoiceBookMark (long pCharID, long pBookMarkId) {}
	virtual void OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength) {}
	virtual void OnVoiceVisual (long pCharID, int pMouthOverlay) {}
};

//////////////////////////////////////////////////////////////////////
#endif // _SAPIVOICEEVENTSINK_H
