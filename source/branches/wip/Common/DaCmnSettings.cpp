/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "StdAfx.h"
#include "DaCmnSettings.h"
#include "DaGlobalConfig.h"
#include "DaCmnCharacter.h"
#include "AgentAnchor.h"
#include "MallocPtr.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnSettings::CDaCmnSettings (CGlobalAnchor * pAnchor)
:	mAnchor (pAnchor)
{
}

CDaCmnSettings::~CDaCmnSettings ()
{
}

/////////////////////////////////////////////////////////////////////////////

AudioStatusType CDaCmnSettings::GetAudioStatus ()
{
	AudioStatusType	lStatus = AudioStatus_Available;

	if	(mAnchor)
	{
		try
		{
			INT_PTR			lFileNdx;
			CAgentFile *	lFile;

			for	(lFileNdx = 0; lFile = mAnchor->GetCachedFile (lFileNdx); lFileNdx++)
			{
				CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
				INT_PTR								lClientNdx;
				CDaCmnCharacter *						lCharacter;

				if	(mAnchor->GetFileClients (lFile, lFileClients))
				{
					for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
					{
						if	(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
						{
							if	(lCharacter->IsSpeaking ())
							{
								lStatus = AudioStatus_CharacterSpeaking;
							}
							else
							if	(lCharacter->IsHearing ())
							{
								lStatus = AudioStatus_UserSpeaking;
							}
							else
							if	(lCharacter->IsListening ())
							{
								lStatus = AudioStatus_CharacterListening;
							}
						}
					}
					if	(lStatus != AudioStatus_Available)
					{
						break;
					}
				}
				if	(lStatus != AudioStatus_Available)
				{
					break;
				}
			}

			if	(
					(lStatus == AudioStatus_Available)
				&&	(!CDaSettingsConfig().LoadConfig().mTtsEnabled)
				)
			{
				lStatus = AudioStatus_Disabled;
			}
		}
		catch AnyExceptionDebug
	}
	return lStatus;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSettings::get_SoundEffectsEnabled (VARIANT_BOOL *SoundEffectsEnabled)
{
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mEffectsEnabled ? S_OK : S_FALSE;

	if	(SoundEffectsEnabled)
	{
		(*SoundEffectsEnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSettings::get_BalloonEnabled (VARIANT_BOOL *BalloonEnabled)
{
	HRESULT	lResult = CDaBalloonConfig().LoadConfig().mEnabled ? S_OK : S_FALSE;

	if	(BalloonEnabled)
	{
		(*BalloonEnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnSettings::get_BalloonFont (IFontDisp **BalloonFont)
{
	HRESULT	lResult = S_FALSE;

	if	(!BalloonFont)
	{
		lResult = E_POINTER;
	}
	else
	{
		CDaBalloonConfig		lBalloonConfig;
		tSS <FONTDESC, UINT>	lFontDesc;
		tMallocPtr <OLECHAR>	lFontName;

		(*BalloonFont) = NULL;

		if	(lBalloonConfig.LoadConfig ().mFont)
		{
			CMemDCHandle	lDC;
			CPoint			lFontSize (0, lBalloonConfig.mFont->lfHeight);
			CPoint			lOrigin (0, 0);

			lDC.Attach (CreateCompatibleDC (NULL));
			LPtoDP (lDC, &lFontSize, 1);
			LPtoDP (lDC, &lOrigin, 1);
			lFontSize.y = abs(lOrigin.y - lFontSize.y);
			lFontDesc.cySize.int64 = MulDiv (lFontSize.y, 720, GetDeviceCaps (lDC, LOGPIXELSY)) * 1000;

			lFontDesc.lpstrName = lFontName = AtlAllocTaskOleString (lBalloonConfig.mFont->lfFaceName);
			lFontDesc.sWeight = (short)lBalloonConfig.mFont->lfWeight;
			lFontDesc.sCharset = lBalloonConfig.mFont->lfCharSet;
			lFontDesc.fItalic = lBalloonConfig.mFont->lfItalic;
			lFontDesc.fUnderline = lBalloonConfig.mFont->lfUnderline;
			lFontDesc.fStrikethrough = lBalloonConfig.mFont->lfStrikeOut;

			lResult = OleCreateFontIndirect (&lFontDesc, __uuidof(IFontDisp), (void**)BalloonFont);
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSettings::get_TTSEnabled (VARIANT_BOOL *TTSEnabled)
{
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mTtsEnabled ? S_OK : S_FALSE;

	if	(TTSEnabled)
	{
		(*TTSEnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnSettings::get_TTSSpeed (short *TTSSpeed)
{
	HRESULT	lResult = S_OK;

	if	(!TTSSpeed)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSSpeed) = (short)CDaSettingsConfig().LoadConfig().mTtsSpeed;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSettings::get_SREnabled (VARIANT_BOOL *SREnabled)
{
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mSrEnabled ? S_OK : S_FALSE;

	if	(SREnabled)
	{
		(*SREnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnSettings::get_SRHotKey (BSTR *SRHotKey)
{
	HRESULT	lResult = S_FALSE;

	if	(SRHotKey)
	{
		CDaSettingsConfig	lSettingsConfig;
		CAtlString			lKeyName;
		CAtlString			lModName;
		UINT				lHotKeyCode;
		long				lHotKeyScan;

		lSettingsConfig.LoadConfig ();
		if	(
				(lHotKeyCode = LOBYTE (lSettingsConfig.mSrHotKey))
			&&	(lHotKeyScan = MapVirtualKey (lHotKeyCode, MAPVK_VK_TO_VSC))
			&&	(GetKeyNameText (lHotKeyScan<<16, lKeyName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
			)
		{
			lKeyName.ReleaseBuffer ();

			if	(
					(HIBYTE (lSettingsConfig.mSrHotKey) & HOTKEYF_ALT)
				&&	(lHotKeyScan = MapVirtualKey (VK_MENU, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
			if	(
					(HIBYTE (lSettingsConfig.mSrHotKey) & HOTKEYF_SHIFT)
				&&	(lHotKeyScan = MapVirtualKey (VK_SHIFT, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
			if	(
					(HIBYTE (lSettingsConfig.mSrHotKey) & HOTKEYF_CONTROL)
				&&	(lHotKeyScan = MapVirtualKey (VK_CONTROL, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
		}
		else
		{
			lKeyName.Empty ();
		}
		(*SRHotKey) = lKeyName.AllocSysString();
	}
	return lResult;
}

HRESULT CDaCmnSettings::get_SRHotKeyTime (short *SRHotKeyTime)
{
	HRESULT	lResult = S_OK;

	if	(!SRHotKeyTime)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SRHotKeyTime) = (short)(CDaSettingsConfig().LoadConfig().mSrHotKeyDelay / 1000);
	}
	return lResult;
}

HRESULT CDaCmnSettings::get_SRListeningTip (VARIANT_BOOL *SRListeningTip)
{
	HRESULT	lResult = CDaSettingsConfig().mSrListeningTip ? S_OK : S_FALSE;

	if	(SRListeningTip)
	{
		(*SRListeningTip) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnSettings::get_SRListeningPrompt (VARIANT_BOOL *SRListeningPrompt)
{
	HRESULT	lResult = CDaSettingsConfig().mSrListeningPrompt ? S_OK : S_FALSE;

	if	(SRListeningPrompt)
	{
		(*SRListeningPrompt) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSettings::get_AudioStatus (AudioStatusType *AudioStatus)
{
	HRESULT	lResult = S_FALSE;

	if	(!AudioStatus)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AudioStatus) = GetAudioStatus ();
	}
	return lResult;
}
