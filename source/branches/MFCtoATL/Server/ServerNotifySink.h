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

interface _IServerNotifySink
{
	virtual void _OnCharacterLoaded (long pCharID) {}
	virtual void _OnCharacterUnloaded (long pCharID) {}
	virtual void _OnCharacterNameChanged (long pCharID) {}
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID) {}
	virtual bool _OnDownloadComplete (class CFileDownload * pDownload) {return false;}
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl) {return NULL;}
	virtual bool _OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition) {return false;}
	virtual bool _OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition) {return false;}
	virtual void _OnOptionsChanged () {}
	virtual void _OnDefaultCharacterChanged () {}
};

interface _IServerNotify;

/////////////////////////////////////////////////////////////////////////////
