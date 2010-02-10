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
#ifndef DAGLOBALCONFIG_H_INCLUDED_
#define DAGLOBALCONFIG_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CDaBalloonConfig
{
public:
	CDaBalloonConfig ();
	virtual ~CDaBalloonConfig ();

// Attributes
public:
	bool			mEnabled;
	tPtr <COLORREF>	mFgColor;
	tPtr <COLORREF>	mBkColor;
	tPtr <COLORREF>	mBrColor;
	tPtr <LOGFONT>	mFont;
	tPtr <COLORREF>	mLastFgColor;
	tPtr <LOGFONT>	mLastFont;

// Operations
public:
	CDaBalloonConfig & LoadConfig ();
	CDaBalloonConfig & SaveConfig ();
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CDaAudioOutputConfig
{
public:
	CDaAudioOutputConfig ();
	virtual ~CDaAudioOutputConfig ();

// Attributes
public:
	bool				mEffectsEnabled;
	bool				mTtsEnabled;
	USHORT				mSpeechSpeed;
	static const USHORT	mSpeechSpeedMin;
	static const USHORT	mSpeechSpeedMax;

// Operations
public:
	CDaAudioOutputConfig & LoadConfig ();
	CDaAudioOutputConfig & SaveConfig ();

	long CalcVoiceRate (UINT pSapiVersion = 5);
	long ApplyVoiceRate (long pVoiceSpeed, UINT pSapiVersion = 5);
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CDaSpeechInputConfig
{
public:
	CDaSpeechInputConfig ();
	virtual ~CDaSpeechInputConfig ();

// Attributes
public:
	bool				mEnabled;
	WORD				mHotKey;
	DWORD				mHotKeyDelay;
	bool				mListeningTip;
	bool				mListeningPrompt;
	static const DWORD	mHotKeyDelayMin;
	static const DWORD	mHotKeyDelayMax;
	static const int	mHotKeyRegisterId;

// Operations
public:
	CDaSpeechInputConfig & LoadConfig ();
	CDaSpeechInputConfig & SaveConfig ();

	static bool RegisterHotKey (bool pRegister);
};

/////////////////////////////////////////////////////////////////////////////
#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

extern _DACORE_IMPEXP LPCTSTR gProfileKeySettings;
extern _DACORE_IMPEXP LPCTSTR gProfileKeyDa;
extern _DACORE_IMPEXP LPCTSTR gProfileKeyDaSettings;
extern _DACORE_IMPEXP LPCTSTR gProfileKeyMaSettings;

#define	DA_BROADCAST_OPTIONS_CHANGED RegisterWindowMessage (_T("1147E50D-A208-11DE-ABF2-002421116FB2"))
#define	DA_BROADCAST_DEFCHAR_CHANGED RegisterWindowMessage (_T("1147E513-A208-11DE-ABF2-002421116FB2"))

/////////////////////////////////////////////////////////////////////////////

#endif // DAGLOBALCONFIG_H_INCLUDED_
