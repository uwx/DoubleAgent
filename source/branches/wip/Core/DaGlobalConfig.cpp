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
#include "StdAfx.h"
#include "DaGuid.h"
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

CDaBalloonConfig & CDaBalloonConfig::LoadConfig ()
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
		&&	(lRegFont.Value ().GetSize () == sizeof (LOGFONT))
		&&	(mFont = new tS <LOGFONT>)
		)
	{
		memcpy (mFont.Ptr(), lRegFont.Value ().GetData (), sizeof (LOGFONT));
	}

	if	(
			(lRegColor.IsValid ())
		&&	(mFgColor = new COLORREF)
		)
	{
		(*mFgColor) = (COLORREF) lRegColor.Value ();
	}

	if	(CRegDWord (lRegKey, sProfileUseCharacterFont, true, TRUE).Value ())
	{
		mLastFont = mFont.Detach();
		mLastFgColor = mFgColor.Detach();
	}
	return *this;
}

CDaBalloonConfig & CDaBalloonConfig::SaveConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);
	CRegBinary	lRegFont (lRegKey, sProfileUserFont, true);
	CRegDWord	lRegColor (lRegKey, sProfileUserFontForeColor, true);

	CRegDWord (lRegKey, sProfileUseBalloon, true).SetValue (mEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileUseCharacterFont, true).SetValue (mFont ? FALSE : TRUE).Update ();

	if	(mFont)
	{
		lRegFont.Value().SetSize (sizeof (LOGFONT));
		memcpy (lRegFont.Value().GetData(), mFont.Ptr(), sizeof (LOGFONT));
		lRegFont.Update();
	}
	else
	if	(mLastFont)
	{
		lRegFont.Value().SetSize (sizeof (LOGFONT));
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

const USHORT	CDaAudioOutputConfig::mSpeechSpeedMin = 0;
const USHORT	CDaAudioOutputConfig::mSpeechSpeedMax = 10;

IMPLEMENT_DLL_OBJECT(CDaAudioOutputConfig)

CDaAudioOutputConfig::CDaAudioOutputConfig ()
:	mEffectsEnabled (true),
	mTtsEnabled (true),
	mSpeechSpeed (5)
{
}

CDaAudioOutputConfig::~CDaAudioOutputConfig ()
{
}

//////////////////////////////////////////////////////////////////////

CDaAudioOutputConfig & CDaAudioOutputConfig::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

	mEffectsEnabled = CRegDWord (lRegKey, sProfileUseSoundEffects, true, mEffectsEnabled ? TRUE : FALSE).Value () ? true : false;
	mTtsEnabled = CRegDWord (lRegKey, sProfileEnableSpeaking, true, mTtsEnabled ? TRUE : FALSE).Value () ? true : false;
	mSpeechSpeed = (USHORT) CRegDWord (lRegKey, sProfileSpeakingSpeed, true, mSpeechSpeed).Value ();
	mSpeechSpeed = min (max (mSpeechSpeed, mSpeechSpeedMin), mSpeechSpeedMax);
	return *this;
}

CDaAudioOutputConfig & CDaAudioOutputConfig::SaveConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

	CRegDWord (lRegKey, sProfileUseSoundEffects, true).SetValue (mEffectsEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileEnableSpeaking, true).SetValue (mTtsEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileSpeakingSpeed, true).SetValue (min (max (mSpeechSpeed, mSpeechSpeedMin), mSpeechSpeedMax)).Update ();
	return *this;
}

//////////////////////////////////////////////////////////////////////

long CDaAudioOutputConfig::CalcVoiceRate (UINT pSapiVersion)
{
	LoadConfig ();
	if	(pSapiVersion >= 5)
	{
		return MulDiv (mSpeechSpeed, 8, mSpeechSpeedMax - mSpeechSpeedMin) + 5;
	}
	else
	{
		return MulDiv (mSpeechSpeed, 10, mSpeechSpeedMax - mSpeechSpeedMin) - 5;
	}
}

long CDaAudioOutputConfig::ApplyVoiceRate (long pVoiceSpeed, UINT pSapiVersion)
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

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

const DWORD	CDaSpeechInputConfig::mHotKeyDelayMin = 0;
const DWORD	CDaSpeechInputConfig::mHotKeyDelayMax = 10000;
const int	CDaSpeechInputConfig::mHotKeyRegisterId = 1;

IMPLEMENT_DLL_OBJECT(CDaSpeechInputConfig)

CDaSpeechInputConfig::CDaSpeechInputConfig ()
:	mEnabled (true),
	mHotKey (VK_SCROLL),
	mHotKeyDelay (2000),
	mListeningTip (true),
	mListeningPrompt (true)
{
}

CDaSpeechInputConfig::~CDaSpeechInputConfig ()
{
}

//////////////////////////////////////////////////////////////////////

CDaSpeechInputConfig & CDaSpeechInputConfig::LoadConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, true);

	mEnabled = CRegDWord (lRegKey, sProfileVoiceEnabled, true, mEnabled ? TRUE : FALSE).Value () ? true : false;
	mListeningTip = CRegDWord (lRegKey, sProfileUseVoiceTips, true, mListeningTip ? TRUE : FALSE).Value () ? true : false;
	mListeningPrompt = CRegDWord (lRegKey, sProfileUseBeepSRPrompt, true, mListeningPrompt ? TRUE : FALSE).Value () ? true : false;
	mHotKey = (WORD) CRegDWord (lRegKey, gProfileKeyMaSettingsHoldHotKey, true, mHotKey).Value ();
	mHotKeyDelay = min (max (CRegDWord (lRegKey, sProfileSRTimerDelay, true, mHotKeyDelay).Value (), mHotKeyDelayMin), mHotKeyDelayMax);
	return *this;
}

CDaSpeechInputConfig & CDaSpeechInputConfig::SaveConfig ()
{
	CRegKeyEx	lRegKey (HKEY_CURRENT_USER, gProfileKeyMaSettings, false, true);

	CRegDWord (lRegKey, sProfileVoiceEnabled, true).SetValue (mEnabled ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileUseVoiceTips, true).SetValue (mListeningTip ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, sProfileUseBeepSRPrompt, true).SetValue (mListeningPrompt ? TRUE : FALSE).Update ();
	CRegDWord (lRegKey, gProfileKeyMaSettingsHoldHotKey, true).SetValue ((DWORD) mHotKey).Update ();
	CRegDWord (lRegKey, sProfileSRTimerDelay, true).SetValue (min (max (mHotKeyDelay, mHotKeyDelayMin), mHotKeyDelayMax)).Update ();
	return *this;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifndef	MOD_NOREPEAT
#define MOD_NOREPEAT	0x4000
#endif

bool CDaSpeechInputConfig::RegisterHotKey (bool pRegister)
{
	bool			lRet = false;
	static DWORD	sHotKeyRegistered = 0;

	if	(pRegister)
	{
		RegisterHotKey (false);
	}

	if	(pRegister)
	{
		CDaSpeechInputConfig	lConfig;
		UINT					lHotKeyCode;
		UINT					lHotKeyMod = 0;

		if	(LOBYTE (lConfig.LoadConfig().mHotKey) != 0)
		{
			lHotKeyCode = LOBYTE (lConfig.mHotKey);
			if	(HIBYTE (lConfig.mHotKey) & HOTKEYF_ALT)
			{
				lHotKeyMod |= MOD_ALT;
			}
			if	(HIBYTE (lConfig.mHotKey) & HOTKEYF_CONTROL)
			{
				lHotKeyMod |= MOD_CONTROL;
			}
			if	(HIBYTE (lConfig.mHotKey) & HOTKEYF_SHIFT)
			{
				lHotKeyMod |= MOD_SHIFT;
			}

			if	(::RegisterHotKey (NULL, mHotKeyRegisterId, lHotKeyMod|MOD_NOREPEAT, lHotKeyCode))
			{
				sHotKeyRegistered = MAKELONG (lHotKeyCode, lHotKeyMod);
				if	(LogIsActive())
				{
					LogMessage (LogVerbose, _T("RegisterHotKey [%d] [%8.8X]"), mHotKeyRegisterId, sHotKeyRegistered);
				}
				lRet = true;
			}
		}
	}
	else
	if	(sHotKeyRegistered)
	{
		if	(LogIsActive())
		{
			LogMessage (LogVerbose, _T("UnregisterHotKey [%d] [%8.8X]"), mHotKeyRegisterId, sHotKeyRegistered);
		}
		::UnregisterHotKey (NULL, mHotKeyRegisterId);
		sHotKeyRegistered = 0;
		lRet = true;
	}

	return lRet;
}
