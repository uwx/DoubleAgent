/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "DaCoreExp.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CDaBalloonConfig
{
	DECLARE_DLL_OBJECT(CDaBalloonConfig)
public:
	CDaBalloonConfig ();
	virtual ~CDaBalloonConfig ();

// Attributes
public:
	bool				mEnabled;
	tPtr <OLE_COLOR>	mFgColor;
	tPtr <OLE_COLOR>	mBkColor;
	tPtr <OLE_COLOR>	mBrColor;
	tPtr <LOGFONT>		mFont;
	tPtr <OLE_COLOR>	mLastFgColor;
	tPtr <LOGFONT>		mLastFont;

// Operations
public:
	CDaBalloonConfig& LoadConfig ();
	CDaBalloonConfig& SaveConfig ();
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CDaSettingsConfig
{
	DECLARE_DLL_OBJECT(CDaSettingsConfig)
public:
	CDaSettingsConfig ();
	virtual ~CDaSettingsConfig ();

// Attributes
public:
	bool				mEffectsEnabled;
	bool				mTtsEnabled;
	USHORT				mTtsSpeed;
	static const USHORT	mTtsSpeedMin;
	static const USHORT	mTtsSpeedMax;
	bool				mSrEnabled;
	WORD				mSrHotKey;
	DWORD				mSrHotKeyDelay;
	bool				mSrListeningTip;
	bool				mSrListeningPrompt;
	static const DWORD	mSrHotKeyDelayMin;
	static const DWORD	mSrHotKeyDelayMax;

// Operations
public:
	CDaSettingsConfig& LoadConfig ();
	CDaSettingsConfig& SaveConfig ();

	long CalcVoiceRate (UINT pSapiVersion = 5);
	long ApplyVoiceRate (long pVoiceSpeed, UINT pSapiVersion = 5);
};

/////////////////////////////////////////////////////////////////////////////
#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////

extern _DACORE_IMPEXP LPCTSTR gProfileKeySettings;
extern _DACORE_IMPEXP LPCTSTR gProfileKeyDa;
extern _DACORE_IMPEXP LPCTSTR gProfileKeyDaSettings;
extern _DACORE_IMPEXP LPCTSTR gProfileKeyMaSettings;

#define	DA_BROADCAST_OPTIONS_CHANGED RegisterWindowMessage (_T("1147E50D-A208-11DE-ABF2-002421116FB2"))
#define	DA_BROADCAST_DEFCHAR_CHANGED RegisterWindowMessage (_T("1147E513-A208-11DE-ABF2-002421116FB2"))

/////////////////////////////////////////////////////////////////////////////
