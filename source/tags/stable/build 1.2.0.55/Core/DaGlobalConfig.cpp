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
#include "StdAfx.h"
#include "DaVersion.h"
#include "DaGlobalConfig.h"
#include "Registry.h"

//////////////////////////////////////////////////////////////////////

LPCTSTR	gProfileKeyDa						= _T("Software\\")_T(_DOUBLEAGENT_NAME);
LPCTSTR	gProfileKeyDaSettings				= _T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")_T("Settings");
LPCTSTR	gProfileKeySettings					= _T("Settings");
LPCTSTR	gProfileKeyMaSettings				= _T("Software\\Microsoft\\Microsoft Agent");

static LPCTSTR	sProfileEnableSpeaking		= _T("EnableSpeaking");
static LPCTSTR	gProfileKeyMaSettingsHoldHotKey		= _T("KeyHoldHotKey");
static LPCTSTR	sProfileSpeakingSpeed		= _T("SpeakingSpeed");
static LPCTSTR	sProfileUseBalloon			= _T("UseBalloon");
static LPCTSTR	sProfileUseBeepSRPrompt		= _T("UseBeepSRPrompt");
static LPCTSTR	sProfileUseCharacterFont	= _T("UseCharacterFont");
static LPCTSTR	sProfileUserFont			= _T("UserFont");
static LPCTSTR	sProfileUserFontForeColor	= _T("UserFontForeColor");
static LPCTSTR	sProfileUseSoundEffects		= _T("UseSoundEffects");
static LPCTSTR	sProfileUseVoiceTips		= _T("UseVoiceTips");
static LPCTSTR	sProfileVoiceEnabled		= _T("VoiceEnabled");
static LPCTSTR	sProfileSRModeID			= _T("SRModeID");
static LPCTSTR	sProfileSRTimerDelay		= _T("SRTimerDelay");

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CDaBalloonConfig)

CDaBalloonConfig::CDaBalloonConfig ()
:	mEnabled (true)
{
}

CDaBalloonConfig::~CDaBalloonConfig ()
{
}

//////////////////////////////////////////////////////////////////////

CDaBalloonConfig& CDaBalloonConfig::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);
	CRegBinary	lRegFont (lRegKey, sProfileUserFont);
	CRegDWord	lRegColor (lRegKey, sProfileUserFontForeColor);

	mEnabled = CRegDWord (lRegKey, sProfileUseBalloon, true, mEnabled ? TRUE : FALSE).Value () ? true : false;

	mFgColor = NULL;
	mFont = NULL;
	mLastFgColor = NULL;
	mLastFont = NULL;

	if	(
			(lRegFont.IsValid ())
		&&	(lRegFont.Value ().GetCount() == sizeof (LOGFONT))
		&&	(mFont = new tS <LOGFONT>)
		)
	{
		memcpy (mFont.Ptr(), lRegFont.Value ().GetData(), sizeof (LOGFONT));
	}

	if	(
			(lRegColor.IsValid ())
		&&	(mFgColor = new OLE_COLOR)
		)
	{
		(*mFgColor) = (OLE_COLOR) lRegColor.Value ();
	}

	if	(CRegDWord (lRegKey, sProfileUseCharacterFont, true, TRUE).Value ())
	{
		mLastFont = mFont.Detach();
		mLastFgColor = mFgColor.Detach();
	}
	return *this;
}

CDaBalloonConfig& CDaBalloonConfig::SaveConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
	CRegBinary	lRegFont (lRegKey, sProfileUserFont, true);
	CRegDWord	lRegColor (lRegKey, sProfileUserFontForeColor, true);

	CRegDWord (lRegKey, sProfileUseBalloon, true).SetValue (mEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileUseCharacterFont, true).SetValue (mFont ? FALSE : TRUE).Update ();

	if	(mFont)
	{
		lRegFont.Value().SetCount (sizeof (LOGFONT));
		memcpy (lRegFont.Value().GetData(), mFont.Ptr(), sizeof (LOGFONT));
		lRegFont.Update();
	}
	else
	if	(mLastFont)
	{
		lRegFont.Value().SetCount (sizeof (LOGFONT));
		memcpy (lRegFont.Value().GetData(), mLastFont.Ptr(), sizeof (LOGFONT));
		lRegFont.Update();
	}

	if	(mFgColor)
	{
		lRegColor.SetValue (*mFgColor).Update();
	}
	else
	if	(mLastFgColor)
	{
		lRegColor.SetValue (*mLastFgColor).Update();
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

const USHORT	CDaSettingsConfig::mTtsSpeedMin = 0;
const USHORT	CDaSettingsConfig::mTtsSpeedMax = 10;
const DWORD		CDaSettingsConfig::mSrHotKeyDelayMin = 0;
const DWORD		CDaSettingsConfig::mSrHotKeyDelayMax = 10000;

IMPLEMENT_DLL_OBJECT(CDaSettingsConfig)

CDaSettingsConfig::CDaSettingsConfig ()
:	mEffectsEnabled (true),
	mTtsEnabled (true),
	mTtsSpeed (5),
	mSrEnabled (true),
	mSrHotKey (VK_SCROLL),
	mSrHotKeyDelay (2000),
	mSrListeningTip (true),
	mSrListeningPrompt (true)
{
}

CDaSettingsConfig::~CDaSettingsConfig ()
{
}

//////////////////////////////////////////////////////////////////////

CDaSettingsConfig& CDaSettingsConfig::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

	mEffectsEnabled = CRegDWord (lRegKey, sProfileUseSoundEffects, true, mEffectsEnabled ? TRUE : FALSE).Value () ? true : false;
	mTtsEnabled = CRegDWord (lRegKey, sProfileEnableSpeaking, true, mTtsEnabled ? TRUE : FALSE).Value () ? true : false;
	mTtsSpeed = (USHORT) CRegDWord (lRegKey, sProfileSpeakingSpeed, true, mTtsSpeed).Value ();
	mTtsSpeed = min (max (mTtsSpeed, mTtsSpeedMin), mTtsSpeedMax);
	mSrEnabled = CRegDWord (lRegKey, sProfileVoiceEnabled, true, mSrEnabled ? TRUE : FALSE).Value () ? true : false;
	mSrListeningTip = CRegDWord (lRegKey, sProfileUseVoiceTips, true, mSrListeningTip ? TRUE : FALSE).Value () ? true : false;
	mSrListeningPrompt = CRegDWord (lRegKey, sProfileUseBeepSRPrompt, true, mSrListeningPrompt ? TRUE : FALSE).Value () ? true : false;
	mSrHotKey = (WORD) CRegDWord (lRegKey, gProfileKeyMaSettingsHoldHotKey, true, mSrHotKey).Value ();
	mSrHotKeyDelay = min (max (CRegDWord (lRegKey, sProfileSRTimerDelay, true, mSrHotKeyDelay).Value (), mSrHotKeyDelayMin), mSrHotKeyDelayMax);

	return *this;
}

CDaSettingsConfig& CDaSettingsConfig::SaveConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

	CRegDWord (lRegKey, sProfileUseSoundEffects, true).SetValue (mEffectsEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileEnableSpeaking, true).SetValue (mTtsEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileSpeakingSpeed, true).SetValue (min (max (mTtsSpeed, mTtsSpeedMin), mTtsSpeedMax)).Update ();
	CRegDWord (lRegKey, sProfileVoiceEnabled, true).SetValue (mSrEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileUseVoiceTips, true).SetValue (mSrListeningTip ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileUseBeepSRPrompt, true).SetValue (mSrListeningPrompt ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, gProfileKeyMaSettingsHoldHotKey, true).SetValue ((DWORD) mSrHotKey).Update ();
	CRegDWord (lRegKey, sProfileSRTimerDelay, true).SetValue (min (max (mSrHotKeyDelay, mSrHotKeyDelayMin), mSrHotKeyDelayMax)).Update ();

	return *this;
}

//////////////////////////////////////////////////////////////////////

long CDaSettingsConfig::CalcVoiceRate (UINT pSapiVersion)
{
	LoadConfig ();
	if	(pSapiVersion >= 5)
	{
		return MulDiv (mTtsSpeed, 8, mTtsSpeedMax - mTtsSpeedMin) + 5;
	}
	else
	{
		return MulDiv (mTtsSpeed, 10, mTtsSpeedMax - mTtsSpeedMin) - 5;
	}
}

long CDaSettingsConfig::ApplyVoiceRate (long pVoiceSpeed, UINT pSapiVersion)
{
	long lVoiceRate = CalcVoiceRate (pSapiVersion);
	if	(pSapiVersion >= 5)
	{
		return dtol ((double)pVoiceSpeed * pow (3.0, min (max ((double)(lVoiceRate-9)/8.0, -1.0), 1.0)) * 0.86);
	}
	else
	{
		return pVoiceSpeed + MulDiv (pVoiceSpeed, lVoiceRate, 10);
	}
}
