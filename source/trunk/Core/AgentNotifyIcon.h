/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include <shellapi.h>

/////////////////////////////////////////////////////////////////////////////

struct CAgentIconData
{
	bool		mShowIcon;
	bool		mGenerateIcon;
	CRect		mGenerateIconClip;
	GUID		mIdentity;
	CAtlString	mTip;

	CAgentIconData () : mShowIcon(false), mGenerateIcon(false), mGenerateIconClip(0,0,0,0), mIdentity(GUID_NULL) {}
};

/////////////////////////////////////////////////////////////////////////////

class CAgentNotifyIcon
{
public:
	CAgentNotifyIcon ();
	virtual ~CAgentNotifyIcon ();

// Attributes
public:
	static const UINT	mNotifyIconMsg;
	static const UINT	mTaskbarCreatedMsg;

	bool SafeIsValid () const;
	bool SafeIsVisible () const;

	long GetCharID () const;
	const CAgentIconData& GetSettings () const;

// Operations
public:
	bool Attach (long pCharID, const CAgentIconData* pIconData);
	bool Detach (long pCharID);
	bool Remove ();

	bool ShowState (HWND pOwnerWnd, class CAgentFile* pAgentFile);
	bool SetIconTip (const CAgentIconData* pIconData, class CAgentFile* pAgentFile, LANGID pLangID = 0);
	bool SetIconTip (LPCTSTR pIconTip);

	UINT OnNotifyIcon (HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnTaskbarCreated (HWND hWnd, WPARAM wParam, LPARAM lParam);

// Overrides

// Implementation
protected:
	long							mCharID;
	CIconHandle						mGeneratedIcon;
	tSS <NOTIFYICONDATA, DWORD>		mNotifyIconData;
	CAgentIconData					mAgentIconData;
};

/////////////////////////////////////////////////////////////////////////////
